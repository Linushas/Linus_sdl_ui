#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "panel.h"
#include "button.h"
#include "slider.h"
#include "checklist.h"
#include "text_input_field.h"
#include "dropdown_menu.h"
#include "ui_extra.h"

typedef struct windowModel {
        SDL_Window *win;
        SDL_Renderer *rend;
        bool is_running;
        int w, h;
} WM;

typedef struct UIResources {
        TTF_Font *font_russo;
        TTF_Font *russo_small;
        TTF_Font *russo_medium;
        SDL_Color black, white, red, green, blue;
} UIRes;

int UI_Init(UIRes *res);
void cleanUp(UIRes *ui_res, Panel panel, WM *wm);

int main(int argc, char **argv) {
        SDL_Init(SDL_INIT_EVERYTHING);
        TTF_Init();

        WM wm = {.is_running = true, .w = 600*16/9, .h = 600};
        wm.win = SDL_CreateWindow("UI test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, wm.w, wm.h, 0);
        wm.rend = SDL_CreateRenderer(wm.win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        // UI INIT
        UIRes ui_res;
        UI_Init(&ui_res);

        // PANEL AND COMPONENT INIT
        Panel panel = createPanel(createRect(50, 50, wm.w-100, wm.h-100), ui_res.black, createColor(50, 50, 50, 255));
        UI_Event ui_event;

        Button my_button = createButton(wm.rend, "Button 1", createRect(51, 51, 100, 30), ui_res.white, ui_res.black, ui_res.russo_small);
        panel_addComponent(panel, COMPONENT_BUTTON, my_button, "my_button1");
        button_setColorsHovered(wm.rend, my_button, createColor(50, 50, 50, 255), ui_res.white);

        Button my_button2 = createButton(wm.rend, "Click Me!", createRect(300, 300, 200, 100), ui_res.white, ui_res.black, ui_res.font_russo);
        panel_addComponent(panel, COMPONENT_BUTTON, my_button2, "click_me");
        button_setColorsHovered(wm.rend, my_button2, createColor(50, 50, 50, 255), ui_res.white);

        Button dropdown_button = createButton(wm.rend, "Choose Language", createRect(400, 100, 150, 30), ui_res.white, ui_res.black, ui_res.russo_small);
        panel_addComponent(panel, COMPONENT_BUTTON, dropdown_button, "lang_dropdown_button");
        button_setColorsHovered(wm.rend, dropdown_button, createColor(50, 50, 50, 255), ui_res.white);

        Slider my_slider = createSlider(100, 200, 200, ui_res.white);
        panel_addComponent(panel, COMPONENT_SLIDER, my_slider, "slider123");

        Checklist my_checklist = createChecklist(100, 300, 30, ui_res.white, ui_res.russo_small);
        panel_addComponent(panel, COMPONENT_CHECKLIST, my_checklist, "hello_checklist");
        checklist_addItem(wm.rend, my_checklist, "Item 1");
        checklist_addItem(wm.rend, my_checklist, "Item 2");

        DropdownMenu my_dropdownMenu = createDropdownMenu(createRect(400, 130, 150, 30), ui_res.black, ui_res.white, ui_res.russo_small);
        panel_addComponent(panel, COMPONENT_DROPDOWN_MENU, my_dropdownMenu, "lang_dropdown_menu");
        dropdownMenu_addItem(wm.rend, my_dropdownMenu, "English");
        dropdownMenu_addItem(wm.rend, my_dropdownMenu, "Chinese");
        dropdownMenu_addItem(wm.rend, my_dropdownMenu, "Swedish");
        dropdownMenu_addItem(wm.rend, my_dropdownMenu, "Spanish");
        dropdownMenu_addItem(wm.rend, my_dropdownMenu, "Japanese");
        dropdownMenu_addItem(wm.rend, my_dropdownMenu, "French");
        
        TextInputField my_tif = createTextInputField(wm.rend, createRect(600, 100, 200, 100), ui_res.black, ui_res.white, ui_res.russo_small);
        panel_addComponent(panel, COMPONENT_TEXT_INPUT_FIELD, my_tif, "text_input_field1");
        
        // MAIN LOOP
        SDL_Event event;
        int mouse_x, mouse_y;
        bool is_me_clicked = false;
        bool is_mouse_down = 0;
        while(wm.is_running) {
                while(SDL_PollEvent(&event)) {
                        switch(event.type) {
                                case SDL_QUIT:
                                        wm.is_running = false;
                                        break;
                                case SDL_MOUSEBUTTONDOWN:
                                        if(event.button.button == SDL_BUTTON_LEFT)
                                                is_mouse_down = true;
                                        break;
                                case SDL_MOUSEBUTTONUP:
                                        if(event.button.button == SDL_BUTTON_LEFT)
                                                is_mouse_down = false;
                                        break;
                                case SDL_KEYDOWN:
                                        if(textInputField_getFocus(my_tif)) {
                                                if(event.key.keysym.scancode == SDL_SCANCODE_LEFT)
                                                        textInputField_moveCursor(my_tif, 0);
                                                else if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
                                                        textInputField_moveCursor(my_tif, 1);
                                                
                                                if(event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
                                                        textInputField_updateBuffer(my_tif, INPUT_BACKSPACE, "");
                                        }       
                                        break;
                                case SDL_TEXTINPUT:
                                        if(textInputField_getFocus(my_tif)) {
                                                textInputField_updateBuffer(my_tif, INPUT_TEXT, event.text.text);
                                        }  
                                        break;
                        }
                }

                SDL_GetMouseState(&mouse_x, &mouse_y);
                panel_update(wm.rend, panel, &ui_event, mouse_x, mouse_y, is_mouse_down);

                switch(ui_event.event_type) {
                        case BUTTON_CLICKED:
                                if(strcmp(ui_event.component_key, "my_button1") == 0) {
                                        printf("Button was clicked\n");
                                }
                                else if(strcmp(ui_event.component_key, "lang_dropdown_button") == 0) {
                                        dropdownMenu_setVisibilityTrue(my_dropdownMenu);
                                }
                                else if(strcmp(ui_event.component_key, "click_me") == 0) {
                                        if(!is_me_clicked) {
                                                checklist_addItem(wm.rend, my_checklist, "Hello");
                                                checklist_addItem(wm.rend, my_checklist, "World!");
                                                is_me_clicked = true;
                                        }
                                }
                                break;

                        case DROPDOWN_ITEM_CLICKED:
                                if(strcmp(ui_event.component_key, "lang_dropdown_menu") == 0) {
                                        char language[256];
                                        dropdownMenu_getItemText(my_dropdownMenu, ui_event.item_idx, language);
                                        printf("Language: %s\n", language);
                                }
                                break;
                        
                        case SLIDER_UPDATED:
                                Slider secret_slider;
                                if((secret_slider = panel_getComponent(panel, "slider123")) != NULL) {
                                        float value = slider_getValue(secret_slider);
                                        printf("value: %.3f\n", value);    
                                }
                                break;
                }
                
                SDL_SetRenderDrawColor(wm.rend, 0,0,10,0);
                SDL_RenderClear(wm.rend);
                panel_render(wm.rend, panel);
                SDL_RenderPresent(wm.rend);
        }

        cleanUp(&ui_res, panel, &wm);
        TTF_Quit();
        SDL_Quit();
        return 0;
}

void cleanUp(UIRes *ui_res, Panel panel, WM *wm) {
        TTF_CloseFont(ui_res->font_russo);
        TTF_CloseFont(ui_res->russo_small);
        TTF_CloseFont(ui_res->russo_medium);

        destroyPanel(panel);

        SDL_DestroyRenderer(wm->rend);
        SDL_DestroyWindow(wm->win);
}

int UI_Init(UIRes *res) {
        res->font_russo = TTF_OpenFont("res/RussoOne-Regular.ttf", 24);
        res->russo_small = TTF_OpenFont("res/RussoOne-Regular.ttf", 12);
        res->russo_medium = TTF_OpenFont("res/RussoOne-Regular.ttf", 16);

        res->black = createColor(0, 0, 0, 255);
        res->white = createColor(255, 255, 255, 255);
        res->red = createColor(255, 0, 0, 255);
        res->green = createColor(0, 255, 0, 255);
        res->blue = createColor(0, 0, 255, 255);
        return 1;
}