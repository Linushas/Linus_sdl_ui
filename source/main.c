#include <stdio.h>
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
int createButtons(Button *buttons, WM *wm, const UIRes ui_res);

int main(int argc, char **argv) {
        // SDL INIT
        SDL_Init(SDL_INIT_EVERYTHING);
        TTF_Init();

        WM wm = {.is_running = true, .w = 600*16/9, .h = 600};
        wm.win = SDL_CreateWindow("UI test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, wm.w, wm.h, 0);
        wm.rend = SDL_CreateRenderer(wm.win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        // UI INIT
        UIRes ui_res;
        UI_Init(&ui_res);

        Panel panel = createPanel(createRect(50, 50, wm.w-100, wm.h-100), ui_res.black, createColor(50, 50, 50, 255));

        Button my_button = createButton(wm.rend, "Button 1", createRect(51, 51, 100, 30), ui_res.white, ui_res.black, ui_res.russo_small);
        panel_addComponent(panel, COMPONENT_BUTTON, my_button, "my_button1");
        button_setColorsHovered(wm.rend, my_button, createColor(50, 50, 50, 255), ui_res.white);

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
                                        break;
                                case SDL_TEXTINPUT:
                                        break;
                        }
                }
                SDL_GetMouseState(&mouse_x, &mouse_y);

                button_event(my_button, mouse_x, mouse_y);

                // RENDER
                SDL_SetRenderDrawColor(wm.rend, 0,0,10,0);
                SDL_RenderClear(wm.rend);

                panel_render(wm.rend, panel);

                SDL_RenderPresent(wm.rend);
        }

        // CLEANUP
        TTF_CloseFont(ui_res.font_russo);
        TTF_CloseFont(ui_res.russo_small);
        TTF_CloseFont(ui_res.russo_medium);

        destroyPanel(panel);

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