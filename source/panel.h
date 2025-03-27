#ifndef PANEL_H
#define PANEL_H

#include <stdbool.h>
#include "SDL_include.h"

enum ComponentType{COMPONENT_BUTTON, COMPONENT_SLIDER, COMPONENT_CHECKLIST, COMPONENT_TEXT_INPUT_FIELD, COMPONENT_DROPDOWN_MENU, COMPONENT_LABEL};
enum PanelEvent{BUTTON_CLICKED, CHECKLIST_UPDATED, SLIDER_UPDATED, TEXT_INPUT, DROPDOWN_ITEM_CLICKED};

typedef struct Panel *Panel;

typedef struct UI_Event {
        int event_type;
        char component_key[256];
        int item_idx;
} UI_Event;

Panel createPanel(SDL_Rect rect, SDL_Color bg, SDL_Color border_color);
void panel_render(SDL_Renderer *rend, Panel p);
int panel_update(SDL_Renderer *rend, Panel p, UI_Event *ui_event, int mouse_x, int mouse_y, bool is_mouse_down);
int destroyPanel(Panel p);
int panel_addComponent(Panel p, int type, void *component, char *key);
void *panel_getComponent(Panel p, char *key);
void panel_hideComponent(Panel p, char *key, bool hide);
void panel_hide(Panel p, bool hide);

#endif