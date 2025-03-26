#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "checklist.h"

typedef struct Item {
        char text[256];
        bool is_selected;
        SDL_Texture *texture;
        SDL_Rect text_rect;
} Item;

typedef struct Checklist {
        TTF_Font *font;
        SDL_Color fg;
        int x, y, size;
        int item_count;
        int cooldown;
        Item items[16];
} *Checklist;

Checklist createChecklist(int x, int y, int size, SDL_Color fg, TTF_Font *font) {
        Checklist cb = malloc(sizeof(struct Checklist));
        if (cb == NULL) {
                printf("Error: Failed to allocate memory for Checklist.\n");
                return NULL;
        }

        cb->x = x;
        cb->y = y;
        cb->size = size;
        cb->fg = fg;
        cb->font = font;
        cb->item_count = 0;
        cb->cooldown = 0;

        return cb;
}

int checklist_addItem(SDL_Renderer *rend, Checklist cb, char *text) {
        cb->items[cb->item_count].is_selected = false;
        strcpy(cb->items[cb->item_count].text, text);

        SDL_Surface *surf = TTF_RenderText_Blended(cb->font, text, cb->fg);
        if (surf == NULL) {
                printf("Error: Failed to create text surface for Checklist.\n");
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

int destroyChecklist(Checklist cb) {
        for(int i = 0; i < cb->item_count; i++) {
                SDL_DestroyTexture(cb->items[i].texture);
        }

        free(cb);
}

int checklist_event(Checklist cb, int mouse_x, int mouse_y, bool is_mouse_down) {
        (cb->cooldown)++;
        if(mouse_x > cb->x && mouse_y > cb->y && is_mouse_down && mouse_x < (cb->x + cb->size*2)) {
                int idx = ((mouse_y - cb->y) + cb->size/4)/cb->size;
                if(idx < cb->item_count && cb->cooldown > 10){
                        cb->items[idx].is_selected = !cb->items[idx].is_selected;
                        cb->cooldown = 0;
                        return (mouse_y - cb->y)/cb->size;
                }               
        }
        else return -1;
}

void checklist_render(SDL_Renderer *rend, Checklist cb) {
        int box_size = cb->size/2;
        SDL_SetRenderDrawColor(rend, cb->fg.r, cb->fg.g, cb->fg.b, cb->fg.a);

        for(int i = 0; i < cb->item_count; i++) {
                SDL_Rect box = {
                        cb->x, 
                        cb->y + i*cb->size,
                        box_size,
                        box_size
                };

                if(cb->items[i].is_selected)
                        SDL_RenderFillRect(rend, &box);
                else SDL_RenderDrawRect(rend, &box);

                SDL_RenderCopy(rend, cb->items[i].texture, NULL, &cb->items[i].text_rect);
        }
}