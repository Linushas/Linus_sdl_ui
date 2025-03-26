#include <stdio.h>
#include <stdlib.h>
#include "slider.h"

typedef struct Slider {
        int x, y, w;
        float value;
        bool is_dragging;
        float drag_offset;
        SDL_Color fg;
} *Slider;

Slider createSlider(int x, int y, int w, SDL_Color fg) {
        Slider s = malloc(sizeof(struct Slider));
        if (s == NULL) {
                printf("Error: Failed to allocate memory for Slider.\n");
                return NULL;
        }

        s->x = x;
        s->y = y;
        s->w = w;
        s->fg = fg;
        s->value = 0.5f;
        s->is_dragging = false;
        s->drag_offset = 0.0f;

        return s;
}

float slider_updateValue(Slider s, int mouse_x, int mouse_y, bool is_mouse_down) {
        const int knob_size = 10;
        int knob_center_x = s->x + (int)(s->value * s->w);
    
        bool is_over_knob = (
                (mouse_x >= knob_center_x - knob_size/2) &&
                (mouse_x <= knob_center_x + knob_size/2) &&
                (mouse_y >= s->y - knob_size/2) &&
                (mouse_y <= s->y + knob_size/2)
        );
    
        if (is_mouse_down) {
            if (is_over_knob && !s->is_dragging) {
                s->is_dragging = true;
                s->drag_offset = mouse_x - knob_center_x;
            }
        } 
        else {
            s->is_dragging = false;
        }
    
        if (s->is_dragging) {
            float raw_value = (float)(mouse_x - s->x - s->drag_offset) / (float)s->w;
            s->value = fmaxf(0.0f, fminf(1.0f, raw_value));
            return s->value;
        }

        return -1;
}

float slider_getValue(Slider s) {return s->value;}

void slider_render(SDL_Renderer *rend, Slider s) {
        int knob_size = 10;
        SDL_SetRenderDrawColor(rend, s->fg.r, s->fg.g, s->fg.b, s->fg.a);

        SDL_RenderDrawLine(rend, s->x, s->y, s->x + s->w, s->y);

        SDL_Rect knob = {s->x + (int)(s->value*s->w) - knob_size/2, s->y - knob_size/2, knob_size, knob_size};
        SDL_RenderFillRect(rend, &knob);
}

int destroySlider(Slider s) {
        free(s);
}