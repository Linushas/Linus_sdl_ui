#ifndef PANEL_H
#define PANEL_H

#include <stdbool.h>
#include "SDL_include.h"

enum ComponentType{COMPONENT_BUTTON, COMPONENT_SLIDER, COMPONENT_CHECKLIST, COMPONENT_TEXT_INPUT_FIELD, COMPONENT_DROPDOWN_MENU};

typedef struct Panel *Panel;

Panel createPanel(SDL_Rect rect, SDL_Color bg, SDL_Color border_color);
void panel_render(SDL_Renderer *rend, Panel p);
int destroyPanel(Panel p);
int panel_addComponent(Panel p, int type, void *component, char *key);

#endif