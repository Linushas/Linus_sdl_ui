#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "label.h"

typedef struct Label {
        TTF_Font *font;
        SDL_Texture *font_texture;
        SDL_Color fg;
        SDL_Rect text_rect;
        char text[1024];
} *Label;

Label createLabel(SDL_Renderer *rend, int x, int y, char *text, SDL_Color fg, TTF_Font *font) {
        Label l = malloc(sizeof(struct Label));

        strcpy(l->text, text);
        l->fg = fg;
        l->font = font;

        SDL_Surface *surf = TTF_RenderText_Blended(l->font, l->text, l->fg);
        if (surf == NULL) {
                printf("Error: Failed to create text surface for Label.\n");
                free(l);
                return NULL;
        }

        l->font_texture = SDL_CreateTextureFromSurface(rend, surf);
        if (l->font_texture == NULL) {
                printf("Error: Failed to create texture from text surface.\n");
                SDL_FreeSurface(surf);
                free(l);
                return NULL;
        }

        l->text_rect.x = x;
        l->text_rect.y = y;
        l->text_rect.w = surf->w;
        l->text_rect.h = surf->h;

        SDL_FreeSurface(surf);

        return l;
}

void label_render(SDL_Renderer *rend, Label l) {
        SDL_RenderCopy(rend, l->font_texture, NULL, &l->text_rect);
}

int destroyLabel(Label l) {
        SDL_DestroyTexture(l->font_texture);
        free(l);
}