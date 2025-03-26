#ifndef SLIDER_H
#define SLIDER_H

#include <stdbool.h>
#include "SDL_include.h"

typedef struct Slider *Slider;

Slider createSlider(int x, int y, int lenght, SDL_Color fg);
float slider_updateValue(Slider s, int mouse_x, int mouse_y, bool is_mouse_down);
float slider_getValue(Slider s);
void slider_render(SDL_Renderer *rend, Slider s);
int destroySlider(Slider s);

#endif