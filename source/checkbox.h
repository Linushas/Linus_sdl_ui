#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <stdbool.h>
#include "SDL_include.h"

typedef struct Checkbox *Checkbox;

Checkbox createCheckbox(int x, int y, int size, SDL_Color fg, TTF_Font *font);
int checkbox_addItem(SDL_Renderer *rend, Checkbox cb, char *text);
int destroyCheckbox(Checkbox cb);
int checkbox_event(Checkbox cb, int mouse_x, int mouse_y);
void checkbox_render(SDL_Renderer *rend, Checkbox cb);

#endif