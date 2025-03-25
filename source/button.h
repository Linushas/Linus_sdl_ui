#ifndef BUTTON_H
#define BUTTON_H

#include "SDL_include.h"

typedef struct Button *Button;

enum{BUTTON_NONE, BUTTON_HOVERED};

Button createButton(SDL_Renderer *rend, char *text, SDL_Rect rect, SDL_Color bg, SDL_Color fg, TTF_Font *font);
int button_event(Button b, int mouse_x, int mouse_y); // 0: NONE , 1: HOVERED
void button_render(SDL_Renderer *rend, Button b);
int destroyButton(Button b);
int button_setColorsHovered(SDL_Renderer *rend, Button b, SDL_Color bg, SDL_Color fg);

#endif