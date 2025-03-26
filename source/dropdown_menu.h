#ifndef DROPDOWN_MENU_H
#define DROPDOWN_MENU_H

#include <stdbool.h>
#include "SDL_include.h"

typedef struct DropdownMenu *DropdownMenu;

DropdownMenu createDropdownMenu(SDL_Rect rect, SDL_Color bg, SDL_Color fg, TTF_Font *font);
int dropdownMenu_addItem(SDL_Renderer *rend, DropdownMenu dm, char *text);
int destroyDropdownMenu(DropdownMenu dm);
int dropdownMenu_event(DropdownMenu dm, int mouse_x, int mouse_y, bool is_mouse_down);
void dropdownMenu_render(SDL_Renderer *rend, DropdownMenu dm);
void dropdownMenu_setVisibilityTrue(DropdownMenu dm);
void dropdownMenu_getItemText(DropdownMenu dm, int idx, char *text);

#endif