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

        int mouse_x, mouse_y;
        bool is_mouse_down;
} WM;

typedef struct UIResources {
        TTF_Font *russo_big;
        TTF_Font *russo_medium;
        TTF_Font *russo_small;
        TTF_Font *montserrat_big;
        TTF_Font *montserrat_medium;
        TTF_Font *montserrat_small;
        
        SDL_Color black, white, red, green, blue;
        SDL_Color grey, dim_grey, slate_grey, raisin_black, charcoal, spanish_blue;
} UIRes;

int UI_Init(UIRes *res);
void cleanUp(UIRes *ui_res, Panel panel, WM *wm);
void render(SDL_Renderer *rend, Panel panel);
int setupComponents(SDL_Renderer *rend, Panel panel, UIRes ui_res);
int eventHandler(WM *wm);

int main(int argc, char **argv) {
        SDL_Init(SDL_INIT_EVERYTHING);
        TTF_Init();

        WM wm = {.is_running = true, .w = 600*16/9, .h = 600};
        wm.win = SDL_CreateWindow("UI test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, wm.w, wm.h, 0);
        wm.rend = SDL_CreateRenderer(wm.win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        UIRes ui_res;
        UI_Init(&ui_res);
        Panel panel = createPanel(createRect(0, 0, wm.w, wm.h), ui_res.raisin_black, ui_res.raisin_black);
        UI_Event ui_event;
        setupComponents(wm.rend, panel, ui_res);
        
        int current_tab = 1;
        int tab_count = 4;
        char *tab_keys[4] = {"tab1", "tab2", "tab3", "tab4"};
        char *slider_keys[6] = {"sldr1", "sldr2", "sldr3", "sldr4", "sldr5", "sldr6"};
        char *btn_keys[4] = {"btn1", "btn2", "btn3", "btn4"};
        while(wm.is_running) {
                eventHandler(&wm);

                SDL_GetMouseState(&wm.mouse_x, &wm.mouse_y);
                panel_update(wm.rend, panel, &ui_event, wm.mouse_x, wm.mouse_y, wm.is_mouse_down);

                switch(ui_event.event_type) {
                        case BUTTON_CLICKED:
                                for(int i = 0; i < tab_count; i++) {
                                        if(strcmp(ui_event.component_key, tab_keys[i]) == 0) {
                                                current_tab = i+1;
                                                break;
                                        }
                                }
                                break;
                }
                
                switch(current_tab) {
                        case 1:
                                for(int i = 0; i < 6; i++) {
                                        panel_hideComponent(panel, slider_keys[i], true);
                                }
                                for(int i = 0; i < 4; i++) {
                                        panel_hideComponent(panel, btn_keys[i], false);
                                }
                                break;
                        case 2:
                                for(int i = 0; i < 6; i++) {
                                        panel_hideComponent(panel, slider_keys[i], false);
                                }
                                for(int i = 0; i < 4; i++) {
                                        panel_hideComponent(panel, btn_keys[i], true);
                                }
                                break;
                        case 3:
                                for(int i = 0; i < 6; i++) {
                                        panel_hideComponent(panel, slider_keys[i], true);
                                }
                                for(int i = 0; i < 4; i++) {
                                        panel_hideComponent(panel, btn_keys[i], true);
                                }
                                break;
                        case 4:
                                for(int i = 0; i < 6; i++) {
                                        panel_hideComponent(panel, slider_keys[i], true);
                                }
                                for(int i = 0; i < 4; i++) {
                                        panel_hideComponent(panel, btn_keys[i], true);
                                }
                                break;
                }
                
                render(wm.rend, panel);
        }

        cleanUp(&ui_res, panel, &wm);
        TTF_Quit();
        SDL_Quit();
        return 0;
}

int eventHandler(WM *wm) {
        static SDL_Event event;

        while(SDL_PollEvent(&event)) {
                switch(event.type) {
                        case SDL_QUIT:
                                wm->is_running = false;
                                break;
                        case SDL_MOUSEBUTTONDOWN:
                                if(event.button.button == SDL_BUTTON_LEFT)
                                        wm->is_mouse_down = true;
                                break;
                        case SDL_MOUSEBUTTONUP:
                                if(event.button.button == SDL_BUTTON_LEFT)
                                        wm->is_mouse_down = false;
                                break;
                        case SDL_KEYDOWN:
                                // if(textInputField_getFocus(my_tif)) {
                                //         if(event.key.keysym.scancode == SDL_SCANCODE_LEFT)
                                //                 textInputField_moveCursor(my_tif, 0);
                                //         else if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
                                //                 textInputField_moveCursor(my_tif, 1);
                                        
                                //         if(event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
                                //                 textInputField_updateBuffer(my_tif, INPUT_BACKSPACE, "");
                                // }       
                                break;
                        case SDL_TEXTINPUT:
                                // if(textInputField_getFocus(my_tif)) {
                                //         textInputField_updateBuffer(my_tif, INPUT_TEXT, event.text.text);
                                // }  
                                break;
                }
        }
}

int setupComponents(SDL_Renderer *rend, Panel panel, UIRes ui_res) {
        Button tabs[4];
        tabs[0] = createButton(rend, "Tab 1", createRect(100, 0, 100, 20), ui_res.charcoal, ui_res.grey, ui_res.montserrat_small);
        panel_addComponent(panel, COMPONENT_BUTTON, tabs[0], "tab1");
        button_setColorsHovered(rend, tabs[0], ui_res.slate_grey, ui_res.white);

        tabs[1] = createButton(rend, "Tab 2", createRect(200, 0, 100, 20), ui_res.charcoal, ui_res.grey, ui_res.montserrat_small);
        panel_addComponent(panel, COMPONENT_BUTTON, tabs[1], "tab2");
        button_setColorsHovered(rend, tabs[1], ui_res.slate_grey, ui_res.white);

        tabs[2] = createButton(rend, "Tab 3", createRect(300, 0, 100, 20), ui_res.charcoal, ui_res.grey, ui_res.montserrat_small);
        panel_addComponent(panel, COMPONENT_BUTTON, tabs[2], "tab3");
        button_setColorsHovered(rend, tabs[2], ui_res.slate_grey, ui_res.white);

        tabs[3] = createButton(rend, "Tab 4", createRect(400, 0, 100, 20), ui_res.charcoal, ui_res.grey, ui_res.montserrat_small);
        panel_addComponent(panel, COMPONENT_BUTTON, tabs[3], "tab4");
        button_setColorsHovered(rend, tabs[3], ui_res.slate_grey, ui_res.white);

        Slider sldrs[6];
        char *slider_keys[6] = {"sldr1", "sldr2", "sldr3", "sldr4", "sldr5", "sldr6"};
        for(int i = 0; i < 6; i++) {
                sldrs[i] = createSlider(50, 100 + i*20, 160, ui_res.slate_grey);
                panel_addComponent(panel, COMPONENT_SLIDER, sldrs[i], slider_keys[i]);
        }

        Button btns[4];
        char *btn_keys[4] = {"btn1", "btn2", "btn3", "btn4"};
        for(int i = 0; i < 4; i++) {
                btns[i] = createButton(rend, btn_keys[i], createRect(250, 100 + i*60, 160, 40), ui_res.charcoal, ui_res.grey, ui_res.montserrat_medium);
                panel_addComponent(panel, COMPONENT_BUTTON, btns[i], btn_keys[i]);
                button_setColorsHovered(rend, btns[i], ui_res.slate_grey, ui_res.white);
        }
        
        return true;
}

void render(SDL_Renderer *rend, Panel panel) {
        SDL_SetRenderDrawColor(rend, 0,0,10,0);
        SDL_RenderClear(rend);
        panel_render(rend, panel);
        SDL_RenderPresent(rend);
}

void cleanUp(UIRes *ui_res, Panel panel, WM *wm) {
        TTF_CloseFont(ui_res->russo_big);
        TTF_CloseFont(ui_res->russo_small);
        TTF_CloseFont(ui_res->russo_medium);

        TTF_CloseFont(ui_res->montserrat_big);
        TTF_CloseFont(ui_res->montserrat_small);
        TTF_CloseFont(ui_res->montserrat_medium);

        destroyPanel(panel);

        SDL_DestroyRenderer(wm->rend);
        SDL_DestroyWindow(wm->win);
}

int UI_Init(UIRes *res) {
        res->russo_big = TTF_OpenFont("res/RussoOne-Regular.ttf", 24);
        res->russo_medium = TTF_OpenFont("res/RussoOne-Regular.ttf", 16);
        res->russo_small = TTF_OpenFont("res/RussoOne-Regular.ttf", 12);

        res->montserrat_big = TTF_OpenFont("res/Montserrat-Regular.ttf", 24);
        res->montserrat_medium = TTF_OpenFont("res/Montserrat-Regular.ttf", 16);
        res->montserrat_small = TTF_OpenFont("res/Montserrat-Regular.ttf", 12);

        res->black = createColor(0, 0, 0, 255);
        res->white = createColor(255, 255, 255, 255);
        res->red = createColor(255, 0, 0, 255);
        res->green = createColor(0, 255, 0, 255);
        res->blue = createColor(0, 0, 255, 255);
        res->spanish_blue = createColor(0, 112, 184, 255);
        res->grey = createColor(160, 160, 160, 255);
        res->charcoal = createColor(51, 51, 51, 255);
        res->dim_grey = createColor(105, 105, 105, 255);
        res->raisin_black = createColor(36, 33, 36, 255);
        res->slate_grey = createColor(112, 128, 144, 255);
        return 1;
}