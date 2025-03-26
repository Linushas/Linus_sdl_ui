#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "text_input_field.h"

typedef struct TextInputField {
        TTF_Font *font;
        SDL_Texture *font_texture;
        SDL_Color bg, fg;
        SDL_Rect box_rect, text_rect;

        bool is_focused;

        char buffer[4096];
        int buffer_lenght;
        int cursor_idx;
        int cursor_x, cursor_y;
} *TextInputField;

TextInputField createTextInputField(SDL_Renderer *rend, SDL_Rect rect, SDL_Color bg, SDL_Color fg, TTF_Font *font) {
        TextInputField tif = malloc(sizeof(struct TextInputField));
        if (tif == NULL) {
                printf("Error: Failed to allocate memory for TextInputField.\n");
                return NULL;
        }

        tif->box_rect = rect;
        tif->bg = bg;
        tif->fg = fg;
        tif->font = font;

        tif->is_focused = false;

        tif->cursor_idx = 0;
        tif->buffer_lenght = 1;
        strcpy(tif->buffer, " ");

        textInputField_update(rend, tif);

        return tif;
}

int destroyTextInputField(TextInputField tif) {
        SDL_DestroyTexture(tif->font_texture);
        free(tif);
        return true;
}

void textInputField_update(SDL_Renderer *rend, TextInputField tif) {
        int text_margin = 5;
        int font_size = TTF_FontHeight(tif->font);
    
        if (!tif->buffer || tif->cursor_idx < 0 || tif->cursor_idx > strlen(tif->buffer)) {
            printf("Error: Invalid buffer or cursor index.\n");
            return;
        }
    
        SDL_Surface *surf = TTF_RenderText_Blended_Wrapped(tif->font, tif->buffer, tif->fg, tif->box_rect.w - 2 * text_margin);
        if (!surf) {
            printf("Error: Failed to render text surface.\n");
            return;
        }
    
        tif->font_texture = SDL_CreateTextureFromSurface(rend, surf);
        SDL_FreeSurface(surf);

        tif->cursor_x = 0;
        tif->cursor_y = 0;
    
        int line_height = font_size;
        int max_width = tif->box_rect.w - 2 * text_margin;
        int current_width = 0;
        char tmp_buffer[4096] = {0};
        int tmp_length = 0;
    
        for (int i = 0; i < tif->cursor_idx; i++) {
            tmp_buffer[tmp_length++] = tif->buffer[i];
            tmp_buffer[tmp_length] = '\0';
    
            int char_width = 0, char_height = 0;
            TTF_SizeUTF8(tif->font, tmp_buffer, &char_width, &char_height);

            if (char_width > max_width || tif->buffer[i] == '\n') {
                tif->cursor_y += line_height;
                current_width = 0; 
                tmp_length = 0;   
            } else {
                current_width = char_width;
            }
        }

        tif->cursor_x = current_width;
        tif->cursor_y += line_height;

        tif->text_rect = (SDL_Rect){
            .x = tif->box_rect.x + text_margin,
            .y = tif->box_rect.y + text_margin,
            .w = surf->w,
            .h = surf->h
        };
}

void textInputField_render(SDL_Renderer *rend, TextInputField tif) {
        int border_width = 1;
        int font_size = TTF_FontHeight(tif->font);
        int text_margin = 5;
        static int cursor_counter = 0;
        static bool show_cursor = false;

        SDL_SetRenderDrawColor(rend, tif->fg.r, tif->fg.g, tif->fg.b, tif->fg.a);
        SDL_Rect outline = {tif->box_rect.x - border_width, tif->box_rect.y - border_width, tif->box_rect.w + border_width*2, tif->box_rect.h + border_width*2};
        SDL_RenderFillRect(rend, &outline);

        SDL_SetRenderDrawColor(rend, tif->bg.r, tif->bg.g, tif->bg.b, tif->bg.a);
        SDL_RenderFillRect(rend, &tif->box_rect);

        SDL_RenderCopy(rend, tif->font_texture, NULL, &tif->text_rect);

        show_cursor = (cursor_counter++%40) > 10;
        if(tif->is_focused) {
                if(show_cursor) {
                        SDL_SetRenderDrawColor(rend, tif->fg.r, tif->fg.g, tif->fg.b, tif->fg.a);
                        SDL_RenderDrawLine(rend, 
                                text_margin + tif->box_rect.x + tif->cursor_x, 
                                text_margin + tif->box_rect.y + tif->cursor_y - font_size, 
                                text_margin + tif->box_rect.x + tif->cursor_x, 
                                text_margin + tif->box_rect.y + tif->cursor_y
                        );  
                }
        }
        
}

void textInputField_moveCursor(TextInputField tif, int direction) {
        if(direction == 0) {
                if(tif->cursor_idx > 0)
                        (tif->cursor_idx)--;
        }
        else {
                if(tif->cursor_idx <= tif->buffer_lenght - 1)
                        (tif->cursor_idx)++;
        }
        if(tif->cursor_idx < 0) tif->cursor_idx = 0;
        if(tif->cursor_idx > tif->buffer_lenght) tif->cursor_idx = tif->buffer_lenght;
}

bool textInputField_getFocus(TextInputField tif) {return tif->is_focused;}

bool textInputField_updateFocus(TextInputField tif, int mouse_x, int mouse_y, bool is_mouse_down) {
        bool old_state = tif->is_focused;
        bool mouse_over_field = (
                (mouse_x <= (tif->box_rect.x + tif->box_rect.w)) &&
                (mouse_x >= tif->box_rect.x) &&
                (mouse_y <= (tif->box_rect.y + tif->box_rect.h)) &&
                (mouse_y >= tif->box_rect.y)
        );

        if(is_mouse_down) {
                tif->is_focused = mouse_over_field;
        }

        if(old_state != tif->is_focused && tif->is_focused){
                SDL_StartTextInput();
        }
        else if(old_state != tif->is_focused) {
                SDL_StopTextInput();
        }
                
        return tif->is_focused;
}

void textInputField_updateBuffer(TextInputField tif, int input_state, char *input) {
        if(input_state == INPUT_TEXT) {
                char tmp_buffer[4096];
                char tmp_end[4096];

                memcpy(tmp_buffer, tif->buffer, sizeof(char)*tif->cursor_idx);
                tmp_buffer[tif->cursor_idx] = '\0';
                strcat(tmp_buffer, input);

                memcpy(tmp_end, tif->buffer + tif->cursor_idx, sizeof(char)*(tif->buffer_lenght - tif->cursor_idx));
                tmp_end[(tif->buffer_lenght - tif->cursor_idx)] = '\0';
                strcat(tmp_buffer, tmp_end);

                strcpy(tif->buffer, tmp_buffer);
                (tif->cursor_idx)++;
                (tif->buffer_lenght)++;
        }
        else if(input_state == INPUT_BACKSPACE && tif->cursor_idx > 0) {
                char tmp_buffer[4096];
                char tmp_end[4096];

                memcpy(tmp_buffer, tif->buffer, sizeof(char)*tif->cursor_idx);
                tmp_buffer[tif->cursor_idx-1] = '\0';

                memcpy(tmp_end, tif->buffer + tif->cursor_idx, sizeof(char)*(tif->buffer_lenght - tif->cursor_idx));
                tmp_end[(tif->buffer_lenght - tif->cursor_idx)] = '\0';
                strcat(tmp_buffer, tmp_end);

                strcpy(tif->buffer, tmp_buffer);
                (tif->cursor_idx)--;
                (tif->buffer_lenght)--;
                if(tif->buffer_lenght < 1){
                        tif->buffer_lenght = 1;
                        strcpy(tif->buffer, " ");
                } 
        }
}