#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "checkbox.h"

typedef struct Item {
        char text[256];
        bool is_selected;
        SDL_Texture *texture;
        SDL_Rect text_rect;
} Item;

typedef struct Checkbox {
        TTF_Font *font;
        SDL_Color fg;
        int x, y, size;
        int item_count;
        Item items[16];
} *Checkbox;

Checkbox createCheckbox(int x, int y, int size, SDL_Color fg, TTF_Font *font) {
        Checkbox cb = malloc(sizeof(struct Checkbox));
        if (cb == NULL) {
                printf("Error: Failed to allocate memory for Checkbox.\n");
                return NULL;
        }

        cb->x = x;
        cb->y = y;
        cb->size = size;
        cb->fg = fg;
        cb->font = font;
        cb->item_count = 0;
}

int checkbox_addItem(SDL_Renderer *rend, Checkbox cb, char *text) {
        cb->items[cb->item_count].is_selected = false;
        strcpy(cb->items[cb->item_count].text, text);

        SDL_Surface *surf = TTF_RenderText_Blended(cb->font, text, cb->fg);
        if (surf == NULL) {
                printf("Error: Failed to create text surface for Button.\n");
                free(cb);
                return false;
        }

        cb->items[cb->item_count].texture = SDL_CreateTextureFromSurface(rend, surf);
        if (cb->items[cb->item_count].texture == NULL) {
                printf("Error: Failed to create texture from text surface.\n");
                SDL_FreeSurface(surf);
                free(cb);
                return false;
        }

        cb->items[cb->item_count].text_rect.x = cb->x + cb->size;
        cb->items[cb->item_count].text_rect.y = cb->y + cb->item_count*cb->size;
        cb->items[cb->item_count].text_rect.w = surf->w;
        cb->items[cb->item_count].text_rect.h = surf->h;
        SDL_FreeSurface(surf);
        (cb->item_count)++;

        return true;
}

int destroyCheckbox(Checkbox cb) {
        for(int i = 0; i < cb->item_count; i++) {
                SDL_DestroyTexture(cb->items[i].texture);
        }

        free(cb);
}

int checkbox_event(Checkbox cb, int mouse_x, int mouse_y) {

}

void checkbox_render(SDL_Renderer *rend, Checkbox cb) {
        int box_size = cb->size/2;

        for(int i = 0; i < cb->item_count; i++) {
                SDL_Rect box = {
                        cb->x, 
                        cb->y + i*cb->size,
                        box_size,
                        box_size
                };
                SDL_RenderDrawRect(rend, &box);
                SDL_RenderCopy(rend, cb->items[i].texture, NULL, &cb->items[i].text_rect);
        }
}