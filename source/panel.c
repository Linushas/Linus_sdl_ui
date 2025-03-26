#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "panel.h"
#include "button.h"
#include "slider.h"
#include "checklist.h"
#include "text_input_field.h"
#include "dropdown_menu.h"

typedef struct Component {
        char *key;
        void *component;
        int type;
} Component;

typedef struct Panel {
        SDL_Rect rect;
        SDL_Color bg, border_color;

        struct ChildComponents {
                int component_count;
                Component *component_list;
        };
} *Panel;

Panel createPanel(SDL_Rect rect, SDL_Color bg, SDL_Color border_color) {
        Panel panel = malloc(sizeof(struct Panel));
        if (panel == NULL) {
                printf("Failed to allocate memory for Panel\n");
                return NULL;
        }

        panel->rect = rect;
        panel->bg = bg;
        panel->border_color = border_color;
        panel->component_count = 0;
        panel->component_list = NULL;

        return panel;
}

int panel_addComponent(Panel p, int type, void *component, char *key) {
        p->component_count++;

        p->component_list = realloc(p->component_list, p->component_count * sizeof(Component));
        if (p->component_list == NULL) {
                printf("Failed to reallocate memory for component list : panel\n");
                return false; 
        }

        p->component_list[p->component_count - 1].key = malloc(strlen(key) + 1);
        if (p->component_list[p->component_count - 1].key == NULL) {
                printf("Failed to allocate memory for component key\n");
                return false;
        }
        strcpy(p->component_list[p->component_count - 1].key, key);

        p->component_list[p->component_count - 1].component = component;
        p->component_list[p->component_count - 1].type = type;

        return true;
}

int panel_update(Panel p, UI_Event *ui_event, int mouse_x, int mouse_y, bool is_mouse_down) {
        ui_event->item_idx = -1;
        
        if (p == NULL) {
                printf("Error: Attempting to render a NULL Panel.\n");
                return ui_event->event_type = -1;
        }

        for (int i = 0; i < p->component_count; i++) {
                Component comp = p->component_list[i];
                switch (comp.type) {
                        case COMPONENT_BUTTON:
                                if(button_event((Button)comp.component, mouse_x, mouse_y) && is_mouse_down) {
                                        strcpy(ui_event->component_key, comp.key);
                                        return ui_event->event_type = BUTTON_CLICKED;
                                }
                                break;

                        case COMPONENT_CHECKLIST:
                                if(checklist_event((Checklist)comp.component, mouse_x, mouse_y, is_mouse_down) != -1) {
                                        strcpy(ui_event->component_key, comp.key);
                                        return ui_event->event_type = CHECKLIST_UPDATED;
                                }
                                break;

                        case COMPONENT_SLIDER:
                                if(slider_updateValue((Slider)comp.component, mouse_x, mouse_y, is_mouse_down) != -1) {
                                        strcpy(ui_event->component_key, comp.key);
                                        return ui_event->event_type = SLIDER_UPDATED;
                                }
                                break;

                        case COMPONENT_TEXT_INPUT_FIELD:
                                
                                break;

                        case COMPONENT_DROPDOWN_MENU:
                                int menu_value = dropdownMenu_event((DropdownMenu)comp.component, mouse_x, mouse_y, is_mouse_down);
                                if(menu_value != -1) {
                                        strcpy(ui_event->component_key, comp.key);
                                        ui_event->item_idx = menu_value;
                                        return ui_event->event_type = DROPDOWN_ITEM_CLICKED;
                                }
                                break;

                        default:
                                printf("Warning: Unknown component type (%d) in panel_update.\n", comp.type);
                                break;
                }
        }

        return ui_event->event_type = -1;
}

void *panel_getComponent(Panel p, char *key) {
        for(int i = 0; i < p->component_count; i++) {
                Component comp = p->component_list[i];
                if(strcmp(key, comp.key) == 0) {
                        return comp.component;
                }
        }
        return NULL;
}

void panel_render(SDL_Renderer *rend, Panel p) {
        if (p == NULL) {
            printf("Error: Attempting to render a NULL Panel.\n");
            return;
        }
    
        SDL_SetRenderDrawColor(rend, p->bg.r, p->bg.g, p->bg.b, p->bg.a);
        SDL_RenderFillRect(rend, &p->rect);
    
        SDL_SetRenderDrawColor(rend, p->border_color.r, p->border_color.g, p->border_color.b, p->border_color.a);
        SDL_RenderDrawRect(rend, &p->rect);
    
        for (int i = 0; i < p->component_count; i++) {
            Component comp = p->component_list[i];
            switch (comp.type) {
                case COMPONENT_BUTTON:
                    button_render(rend, (Button)comp.component);
                    break;
    
                case COMPONENT_CHECKLIST:
                    checklist_render(rend, (Checklist)comp.component);
                    break;

                case COMPONENT_SLIDER:
                    slider_render(rend, (Slider)comp.component);
                    break;
    
                case COMPONENT_TEXT_INPUT_FIELD:
                    textInputField_render(rend, (TextInputField)comp.component);
                    break;
    
                case COMPONENT_DROPDOWN_MENU:
                    dropdownMenu_render(rend, (DropdownMenu)comp.component);
                    break;
    
                default:
                    printf("Warning: Unknown component type (%d) in panel_render.\n", comp.type);
                    break;
            }
        }
}

int destroyPanel(Panel p) {
        if (p == NULL) return -1;

        for (int i = 0; i < p->component_count; i++) {
                free(p->component_list[i].key);
                free(p->component_list[i].component);
        }

        free(p->component_list);
        free(p);

        return 0;
}