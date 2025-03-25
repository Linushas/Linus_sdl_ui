#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "button.h"

typedef struct Button {
        TTF_Font *font;
        SDL_Texture *font_texture, *font_hov_texture;
        SDL_Color bg, fg, hov_bg, hov_fg;
        SDL_Rect box_rect, text_rect;
        bool is_hovered;
        char text[64];
} *Button;

Button createButton(SDL_Renderer *rend, char *text, SDL_Rect rect, SDL_Color bg, SDL_Color fg, TTF_Font *font) {
        Button b = malloc(sizeof(struct Button));
        if (b == NULL) {
                printf("Error: Failed to allocate memory for Button.\n");
                return NULL;
        }

        b->font = font;
        b->bg = bg;
        b->fg = fg;
        b->hov_bg = bg;
        b->hov_fg = fg;
        b->box_rect = rect;
        strcpy(b->text, text);
        b->is_hovered = false;

        SDL_Surface *surf = TTF_RenderText_Blended(font, text, b->fg);
        if (surf == NULL) {
                printf("Error: Failed to create text surface for Button.\n");
                free(b);
                return NULL;
        }

        b->font_texture = SDL_CreateTextureFromSurface(rend, surf);
        if (b->font_texture == NULL) {
                printf("Error: Failed to create texture from text surface.\n");
                SDL_FreeSurface(surf);
                free(b);
                return NULL;
        }
        b->font_hov_texture = b->font_texture;

        b->text_rect.x = rect.x + rect.w/2 - surf->w/2;
        b->text_rect.y = rect.y + rect.h/2 - surf->h/2;
        b->text_rect.w = surf->w;
        b->text_rect.h = surf->h;

        SDL_FreeSurface(surf);

        return b;
}

int button_setColorsHovered(SDL_Renderer *rend, Button b, SDL_Color bg, SDL_Color fg) {
        if(b->font_hov_texture != b->font_texture) {
                SDL_DestroyTexture(b->font_hov_texture);
        }
        b->hov_bg = bg;
        b->hov_fg = fg;

        SDL_Surface *surf = TTF_RenderText_Blended(b->font, b->text, b->hov_fg);
        if (surf == NULL) {
                printf("Error: Failed to create text surface for Button.\n");
                free(b);
                return false;
        }

        b->font_hov_texture = SDL_CreateTextureFromSurface(rend, surf);
        if (b->font_hov_texture == NULL) {
                printf("Error: Failed to create texture from text surface.\n");
                SDL_FreeSurface(surf);
                free(b);
                return false;
        }

        SDL_FreeSurface(surf);

        return true;
}

int button_event(Button b, int mouse_x, int mouse_y) {
        if(mouse_x <= (b->box_rect.x + b->box_rect.w) && mouse_x >= b->box_rect.x &&
           mouse_y <= (b->box_rect.y + b->box_rect.h) && mouse_y >= b->box_rect.y)
        {
                return b->is_hovered = BUTTON_HOVERED;
        } else {
                return b->is_hovered = BUTTON_NONE;
        }
}

void button_render(SDL_Renderer *rend, Button b) {
        if (b == NULL) {
                printf("Error: Attempting to render a NULL Button.\n");
                return;
        }
        if(b->is_hovered) {
                SDL_SetRenderDrawColor(rend, b->hov_bg.r, b->hov_bg.g, b->hov_bg.b, b->hov_bg.a);
                SDL_RenderFillRect(rend, &b->box_rect);
                SDL_RenderCopy(rend, b->font_hov_texture, NULL, &b->text_rect);
        } else {
                SDL_SetRenderDrawColor(rend, b->bg.r, b->bg.g, b->bg.b, b->bg.a);
                SDL_RenderFillRect(rend, &b->box_rect);
                SDL_RenderCopy(rend, b->font_texture, NULL, &b->text_rect);
        }
        
}

int destroyButton(Button b) {
        if (b == NULL) {
                printf("Error: Attempting to destroy a NULL Button.\n");
                return false;
        }

        SDL_DestroyTexture(b->font_texture);
        SDL_DestroyTexture(b->font_hov_texture);
        free(b);
        return true;
}
