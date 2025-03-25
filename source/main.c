#include <stdio.h>
#include <stdbool.h>
#include "button.h"
#include "ui_extra.h"

typedef struct windowModel {
        SDL_Window *win;
        SDL_Renderer *rend;
        bool is_running;
} WM;

typedef struct UIResources {
        TTF_Font *font_russo;
        TTF_Font *russo_small;
        SDL_Color black, white, red, green, blue;
} UIRes;

int UI_Init(UIRes *res);
int createButtons(Button *buttons, WM *wm, const UIRes ui_res);

int main(int argc, char **argv) {
        SDL_Init(SDL_INIT_EVERYTHING);
        TTF_Init();

        WM wm = {.is_running = true};
        wm.win = SDL_CreateWindow("UI test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600*16/9, 600, 0);
        wm.rend = SDL_CreateRenderer(wm.win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        UIRes ui_res;
        UI_Init(&ui_res);

        Button buttons[6];
        createButtons(buttons, &wm, ui_res);
        int button_count = sizeof(buttons) / sizeof(Button);

        SDL_Event event;
        int mouse_x, mouse_y;
        while(wm.is_running) {
                while(SDL_PollEvent(&event)) {
                        if(event.type == SDL_QUIT) {
                                wm.is_running = false;
                        }
                }
                SDL_GetMouseState(&mouse_x, &mouse_y);

                for(int i = 0; i < button_count; i++) {
                        button_event(buttons[i], mouse_x, mouse_y);
                }

                SDL_SetRenderDrawColor(wm.rend, 0,0,10,0);
                SDL_RenderClear(wm.rend);

                for(int i = 0; i < button_count; i++)
                        button_render(wm.rend, buttons[i]);

                SDL_RenderPresent(wm.rend);
        }

        TTF_CloseFont(ui_res.font_russo);
        TTF_CloseFont(ui_res.russo_small);
        for(int i = 0; i < button_count; i++)
                destroyButton(buttons[i]);

        SDL_DestroyRenderer(wm.rend);
        SDL_DestroyWindow(wm.win);
        TTF_Quit();
        SDL_Quit();
        return 0;
}

int UI_Init(UIRes *res) {
        res->font_russo = TTF_OpenFont("res/RussoOne-Regular.ttf", 24);
        res->russo_small = TTF_OpenFont("res/RussoOne-Regular.ttf", 12);

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

        return 1;
}