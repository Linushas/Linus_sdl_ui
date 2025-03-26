#include <stdio.h>
#include <stdbool.h>
#include "button.h"
#include "slider.h"
#include "checklist.h"
#include "text_input_field.h"
#include "ui_extra.h"

typedef struct windowModel {
        SDL_Window *win;
        SDL_Renderer *rend;
        bool is_running;
} WM;

typedef struct UIResources {
        TTF_Font *font_russo;
        TTF_Font *russo_small;
        TTF_Font *russo_medium;
        SDL_Color black, white, red, green, blue;
} UIRes;

int UI_Init(UIRes *res);
int createButtons(Button *buttons, WM *wm, const UIRes ui_res);

int main(int argc, char **argv) {
        // SDL INIT
        SDL_Init(SDL_INIT_EVERYTHING);
        TTF_Init();

        WM wm = {.is_running = true};
        wm.win = SDL_CreateWindow("UI test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600*16/9, 600, 0);
        wm.rend = SDL_CreateRenderer(wm.win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        // UI INIT
        UIRes ui_res;
        UI_Init(&ui_res);

        Button buttons[8];
        createButtons(buttons, &wm, ui_res);
        int button_count = sizeof(buttons) / sizeof(Button);

        Checklist cb = createChecklist(50, 50, 40, ui_res.white, ui_res.russo_medium);
        checklist_addItem(wm.rend, cb, "item 1");
        checklist_addItem(wm.rend, cb, "item 2");
        checklist_addItem(wm.rend, cb, "item 3");
        checklist_addItem(wm.rend, cb, "item 5");
        checklist_addItem(wm.rend, cb, "item 6");

        Checklist cb2 = createChecklist(200, 50, 40, ui_res.white, ui_res.russo_medium);
        checklist_addItem(wm.rend, cb2, "checkbox 7");
        checklist_addItem(wm.rend, cb2, "checkbox 8");
        checklist_addItem(wm.rend, cb2, "checkbox 9");
        checklist_addItem(wm.rend, cb2, "checkbox 10");
        checklist_addItem(wm.rend, cb2, "checkbox 11");
        checklist_addItem(wm.rend, cb2, "checkbox 12");
        checklist_addItem(wm.rend, cb2, "checkbox 13");
        checklist_addItem(wm.rend, cb2, "checkbox 14");

        Slider slider = createSlider(wm.rend, 400, 50, 160, ui_res.white);
        Slider slider2 = createSlider(wm.rend, 400, 100, 200, ui_res.red);
        Slider slider3 = createSlider(wm.rend, 400, 150, 240, ui_res.blue);

        TextInputField tif = createTextInputField(wm.rend, createRect(400, 200, 200, 100), ui_res.black, ui_res.white, ui_res.russo_small);

        // MAIN LOOP
        SDL_Event event;
        int mouse_x, mouse_y;
        bool is_mouse_down = 0;
        while(wm.is_running) {
                // SDL EVENT
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
                                        if(textInputField_getFocus(tif)) {
                                                if(event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
                                                        textInputField_moveCursor(tif, 0);
                                                }
                                                if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
                                                        textInputField_moveCursor(tif, 1);
                                                }  
                                                if(event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
                                                        textInputField_updateBuffer(tif, INPUT_BACKSPACE, "**");
                                                }  
                                        }
                                        break;
                                case SDL_TEXTINPUT:
                                        if(SDL_IsTextInputActive()) {
                                                textInputField_updateBuffer(tif, INPUT_TEXT, event.text.text);
                                        }
                                        break;
                        }
                }
                SDL_GetMouseState(&mouse_x, &mouse_y);

                // POLL UI COMPONENTS
                for(int i = 0; i < button_count; i++) {
                        button_event(buttons[i], mouse_x, mouse_y);
                }
                checklist_event(cb, mouse_x, mouse_y, is_mouse_down);
                checklist_event(cb2, mouse_x, mouse_y, is_mouse_down);
                slider_updateValue(slider, mouse_x, mouse_y, is_mouse_down);
                slider_updateValue(slider2, mouse_x, mouse_y, is_mouse_down);
                slider_updateValue(slider3, mouse_x, mouse_y, is_mouse_down);
                textInputField_update(wm.rend, tif);
                textInputField_updateFocus(tif, mouse_x, mouse_y, is_mouse_down);

                // RENDER
                SDL_SetRenderDrawColor(wm.rend, 0,0,10,0);
                SDL_RenderClear(wm.rend);
                for(int i = 0; i < button_count; i++) {
                        button_render(wm.rend, buttons[i]);
                }
                checklist_render(wm.rend, cb);
                checklist_render(wm.rend, cb2);
                slider_render(wm.rend, slider);
                slider_render(wm.rend, slider2);
                slider_render(wm.rend, slider3);
                textInputField_render(wm.rend, tif);
                SDL_RenderPresent(wm.rend);
        }

        // CLEANUP
        TTF_CloseFont(ui_res.font_russo);
        TTF_CloseFont(ui_res.russo_small);
        TTF_CloseFont(ui_res.russo_medium);

        for(int i = 0; i < button_count; i++) {
                destroyButton(buttons[i]);
        }
        destroyChecklist(cb);
        destroyChecklist(cb2);
        destroySlider(slider);
        destroySlider(slider2);
        destroySlider(slider3);
        destroyTextInputField(tif);

        SDL_DestroyRenderer(wm.rend);
        SDL_DestroyWindow(wm.win);
        TTF_Quit();
        SDL_Quit();
        return 0;
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

int createButtons(Button *buttons, WM *wm, const UIRes ui_res) {
        buttons[0] = createButton(
                wm->rend, "Button 1", createRect(0,0,80,20), 
                ui_res.white, ui_res.black, ui_res.russo_small
        );
        if(buttons[0] == NULL) return 0;
        button_setColorsHovered(wm->rend, buttons[0], ui_res.black, ui_res.white);

        buttons[1] = createButton(
                wm->rend, "Button 2", createRect(80,0,80,20), 
                ui_res.white, ui_res.black, ui_res.russo_small
        );
        if(buttons[1] == NULL) return 0;
        button_setColorsHovered(wm->rend, buttons[1], ui_res.black, ui_res.white);

        buttons[2] = createButton(
                wm->rend, "Button 3", createRect(160,0,80,20), 
                ui_res.white, ui_res.black, ui_res.russo_small
        );
        if(buttons[2] == NULL) return 0;
        button_setColorsHovered(wm->rend, buttons[2], ui_res.black, ui_res.white);

        buttons[3] = createButton(
                wm->rend, "Button 4", createRect(240,0,80,20), 
                ui_res.white, ui_res.black, ui_res.russo_small
        );
        if(buttons[3] == NULL) return 0;
        button_setColorsHovered(wm->rend, buttons[3], ui_res.black, ui_res.white);

        buttons[4] = createButton(
                wm->rend, "Button 5", createRect(320,0,80,20), 
                ui_res.white, ui_res.black, ui_res.russo_small
        );
        if(buttons[4] == NULL) return 0;
        button_setColorsHovered(wm->rend, buttons[4], ui_res.black, ui_res.white);

        buttons[5] = createButton(
                wm->rend, "Button 6", createRect(400,0,80,20), 
                ui_res.white, ui_res.black, ui_res.russo_small
        );
        if(buttons[5] == NULL) return 0;
        button_setColorsHovered(wm->rend, buttons[5], ui_res.black, ui_res.white);

        buttons[6] = createButton(
                wm->rend, "Button 7", createRect(480,0,80,20), 
                ui_res.white, ui_res.black, ui_res.russo_small
        );
        if(buttons[6] == NULL) return 0;
        button_setColorsHovered(wm->rend, buttons[6], ui_res.black, ui_res.white);

        buttons[7] = createButton(
                wm->rend, "Button 8", createRect(560,0,80,20), 
                ui_res.white, ui_res.black, ui_res.russo_small
        );
        if(buttons[7] == NULL) return 0;
        button_setColorsHovered(wm->rend, buttons[7], ui_res.black, ui_res.white);

        return 1;
}