#ifndef LABEL_H
#define LABEL_H

#include "SDL_include.h"

typedef struct Label *Label;

Label createLabel(SDL_Renderer *rend, int x, int y, char *text, SDL_Color fg, TTF_Font *font);
void label_render(SDL_Renderer *rend, Label l);
int destroyLabel(Label l);

#endif