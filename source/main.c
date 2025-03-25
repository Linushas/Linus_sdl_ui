#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>
#include "button.h"

typedef struct windowModel {
        SDL_Window *win;
        SDL_Renderer *rend;

        bool is_running;
} WM;

typedef struct UIResources {
        TTF_Font *font_russo;
        SDL_Color black, white, red, green, blue;
} UIRes;

SDL_Rect createRect(int x, int y, int w, int h) {
        SDL_Rect r = {x,y,w,h};
        return r;
}

SDL_Color createColor(int r, int g, int b, int a) {
        SDL_Color c = {r,g,b,a};
        return c;
}

int UI_Init(UIRes *res) {
        res->font_russo = TTF_OpenFont("res/RussoOne-Regular.ttf", 24);

        res->black = createColor(0, 0, 0, 255);
        res->white = createColor(255, 255, 255, 255);
        res->red = createColor(255, 0, 0, 255);
        res->green = createColor(0, 255, 0, 255);
        res->blue = createColor(0, 0, 255, 255);
        return 1;
}

int createButtons(Button *buttons, WM *wm, const UIRes ui_res) {
        buttons[0] = createButton(
                wm->rend, "Button", createRect(100,100,400,200), 
                ui_res.white, ui_res.black, ui_res.font_russo
        );
        if(buttons[0] == NULL) return 1;
        button_setColorsHovered(wm->rend, buttons[0], ui_res.black, ui_res.white);
}

int main(int argc, char **argv) {
        SDL_Init(SDL_INIT_EVERYTHING);
        TTF_Init();

        WM wm = {.is_running = true};

        wm.win = SDL_CreateWindow("UI test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600*16/9, 600, 0);
        wm.rend = SDL_CreateRenderer(wm.win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        UIRes ui_res;
        UI_Init(&ui_res);

        Button buttons[1];
        int button_count = sizeof(buttons) / sizeof(Button);
        createButtons(buttons, &wm, ui_res);

        SDL_Event event;
        while(wm.is_running) {
                while(SDL_PollEvent(&event)) {
                        if(event.type == SDL_QUIT) {
                                wm.is_running = false;
                        }
                }

                int mouse_x, mouse_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);

                for(int i = 0; i < button_count; i++)
                        if(button_event(buttons[i], mouse_x, mouse_y)) 
                                break;

                SDL_SetRenderDrawColor(wm.rend, 0,0,10,0);
                SDL_RenderClear(wm.rend);

                for(int i = 0; i < button_count; i++)
                        button_render(wm.rend, buttons[i]);

                SDL_RenderPresent(wm.rend);
        }

        TTF_CloseFont(ui_res.font_russo);
        for(int i = 0; i < button_count; i++)
                destroyButton(buttons[i]);

        SDL_DestroyRenderer(wm.rend);
        SDL_DestroyWindow(wm.win);
        TTF_Quit();
        SDL_Quit();
        return 0;
}