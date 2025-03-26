#include <stdio.h>
#include <stdlib.h>
#include "dropdown_menu.h"

typedef struct Item {
        char text[256];
        SDL_Texture *texture;
        SDL_Texture *texture_selected;
        SDL_Rect text_rect;
} Item;

typedef struct DropdownMenu {
        TTF_Font *font;
        SDL_Color bg, fg;
        SDL_Rect rect;
        bool is_visible;
        int selected_idx;
        int item_count;
        int item_height;
        Item items[16];
} *DropdownMenu;

DropdownMenu createDropdownMenu(SDL_Rect rect, SDL_Color bg, SDL_Color fg, TTF_Font *font) {
        DropdownMenu dm = malloc(sizeof(struct DropdownMenu));
        if (dm == NULL) {
                printf("Error: Failed to allocate memory for DropdownMenu.\n");
                return NULL;
        }

        dm->rect = rect;
        dm->fg = fg;
        dm->bg = bg;
        dm->font = font;
        dm->item_count = 0;
        dm->item_height = TTF_FontHeight(font)*1.5;
        dm->is_visible = false;

        return dm;
}

int dropdownMenu_addItem(SDL_Renderer *rend, DropdownMenu dm, char *text) {
        strcpy(dm->items[dm->item_count].text, text);

        SDL_Surface *surf = TTF_RenderText_Blended(dm->font, text, dm->fg);
        if (surf == NULL) {
                printf("Error: Failed to create text surface for DropdownMenu.\n");
                free(dm);
                return false;
        }

        dm->items[dm->item_count].texture = SDL_CreateTextureFromSurface(rend, surf);
        if (dm->items[dm->item_count].texture == NULL) {
                printf("Error: Failed to create texture from text surface.\n");
                SDL_FreeSurface(surf);
                free(dm);
                return false;
        }

        SDL_FreeSurface(surf);

        surf = TTF_RenderText_Blended(dm->font, text, dm->bg);
        if (surf == NULL) {
                printf("Error: Failed to create text surface for DropdownMenu.\n");
                free(dm);
                return false;
        }

        dm->items[dm->item_count].texture_selected = SDL_CreateTextureFromSurface(rend, surf);
        if (dm->items[dm->item_count].texture_selected == NULL) {
                printf("Error: Failed to create texture from text surface.\n");
                SDL_FreeSurface(surf);
                free(dm);
                return false;
        }

        dm->items[dm->item_count].text_rect.x = dm->rect.x + 2;
        dm->items[dm->item_count].text_rect.y = (dm->rect.y + dm->item_count*dm->item_height); 
        dm->items[dm->item_count].text_rect.w = surf->w;
        dm->items[dm->item_count].text_rect.h = surf->h;
        SDL_FreeSurface(surf);
        (dm->item_count)++;
        dm->rect.h = dm->item_count*dm->item_height;

        return true;
}

void dropdownMenu_getItemText(DropdownMenu dm, int idx, char *text) {
        strcpy(text, dm->items[idx].text);
}

int destroyDropdownMenu(DropdownMenu dm) {
        for(int i = 0; i < dm->item_count; i++) {
                SDL_DestroyTexture(dm->items[i].texture);
        }
        free(dm);
}

void dropdownMenu_setVisibilityTrue(DropdownMenu dm) {
        dm->is_visible = true;
}

int dropdownMenu_event(DropdownMenu dm, int mouse_x, int mouse_y, bool is_mouse_down) {
        static int counter = 0;
        if(dm->is_visible) {
                counter++;
        }
        else return -1;

        bool mouse_over_menu = (
                (mouse_x <= dm->rect.x + dm->rect.w) &&
                (mouse_x >= dm->rect.x) &&
                (mouse_y <= dm->rect.y + dm->rect.h) &&
                (mouse_y >= dm->rect.y - 30)
        );

        if(!mouse_over_menu) {
                dm->is_visible = false;
                counter = 0;
                return -1;
        }

        int idx = ((mouse_y - dm->rect.y) + dm->item_height/4)/dm->item_height;
        if(idx < dm->item_count) { 
                dm->selected_idx = idx;
                if(is_mouse_down && counter > 20) {
                        counter = 0;
                        dm->is_visible = false;
                        return dm->selected_idx;
                } 
        } 
        return -1;
}

void dropdownMenu_render(SDL_Renderer *rend, DropdownMenu dm) {
        if(!dm->is_visible) return;

        int border_width = 1;

        SDL_SetRenderDrawColor(rend, dm->fg.r, dm->fg.g, dm->fg.b, dm->fg.a);
        SDL_Rect outline = {dm->rect.x - border_width, dm->rect.y - border_width, dm->rect.w + border_width*2, dm->rect.h + border_width};
        SDL_RenderFillRect(rend, &outline);
        
        for(int i = 0; i < dm->item_count; i++) {
                if(i == dm->selected_idx) {
                        SDL_SetRenderDrawColor(rend, dm->bg.r, dm->bg.g, dm->bg.b, dm->bg.a);
                        SDL_Rect box = {
                                dm->rect.x, 
                                dm->rect.y + i*dm->item_height,
                                dm->rect.w,
                                dm->item_height - border_width
                        };
                        SDL_RenderDrawRect(rend, &box);
                        SDL_RenderCopy(rend, dm->items[i].texture_selected, NULL, &dm->items[i].text_rect);
                } 
                else {
                        SDL_SetRenderDrawColor(rend, dm->bg.r, dm->bg.g, dm->bg.b, dm->bg.a);
                        SDL_Rect box = {
                                dm->rect.x, 
                                dm->rect.y + i*dm->item_height,
                                dm->rect.w,
                                dm->item_height - border_width
                        };
                        SDL_RenderFillRect(rend, &box);
                        SDL_RenderCopy(rend, dm->items[i].texture, NULL, &dm->items[i].text_rect);
                }
        }
}