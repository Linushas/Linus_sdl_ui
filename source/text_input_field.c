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

        tif->cursor_idx = 10;
        tif->buffer_lenght = 31;
        strcpy(tif->buffer, "This is a text input field...\0");

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

        SDL_Surface *surf = TTF_RenderText_Blended_Wrapped(tif->font, tif->buffer, tif->fg, (tif->box_rect.w - text_margin*2));
        if (surf == NULL) {
                printf("Error: Failed to create text surface for TextInputField.\n");
                free(tif);
                return;
        }

        tif->font_texture = SDL_CreateTextureFromSurface(rend, surf);
        if (tif->font_texture == NULL) {
                printf("Error: Failed to create texture from text surface.\n");
                SDL_FreeSurface(surf);
                free(tif);
                return;
        }

        tif->text_rect.x = tif->box_rect.x + text_margin;
        tif->text_rect.y = tif->box_rect.y + text_margin;
        tif->text_rect.w = surf->w;
        tif->text_rect.h = surf->h;

        SDL_FreeSurface(surf);

        char tmp_buffer[4096]; 
        memcpy(tmp_buffer, tif->buffer, sizeof(char)*tif->cursor_idx);
        tmp_buffer[tif->cursor_idx] = '\0';
        surf = TTF_RenderText_Blended_Wrapped(tif->font, tmp_buffer, tif->fg, (tif->box_rect.w - text_margin*2));
        tif->cursor_x = surf->w;
        tif->cursor_y = surf->h;

        SDL_FreeSurface(surf);
}

void textInputField_render(SDL_Renderer *rend, TextInputField tif) {
        int border_width = 1;
        int font_size = 12;
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
                (tif->cursor_idx)--;
        }
        else {
                (tif->cursor_idx)++;
        }
}

bool textInputField_getFocus(TextInputField tif) {return tif->is_focused;}

bool textInputField_updateFocus(TextInputField tif, int mouse_x, int mouse_y, bool is_mouse_down) {
        bool mouse_over_field = (
                (mouse_x <= (tif->box_rect.x + tif->box_rect.w)) &&
                (mouse_x >= tif->box_rect.x) &&
                (mouse_y <= (tif->box_rect.y + tif->box_rect.h)) &&
                (mouse_y >= tif->box_rect.y)
        );

        if(is_mouse_down) {
                tif->is_focused = mouse_over_field;
        }

        return tif->is_focused;
}