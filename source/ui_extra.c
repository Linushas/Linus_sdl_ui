#include "ui_extra.h"

SDL_Rect createRect(int x, int y, int w, int h) {
        SDL_Rect r = {x,y,w,h};
        return r;
}

SDL_Color createColor(int r, int g, int b, int a) {
        SDL_Color c = {r,g,b,a};
        return c;
}