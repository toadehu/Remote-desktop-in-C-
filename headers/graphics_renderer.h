#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graphics_objects.h"

/* C89 Boolean Definition */
#define TRUE  1
#define FALSE 0
typedef int BOOL;

/* Image layers enum */
enum IMAGE_LAYERS {
    LAYER_BG = 0,
    LAYER_UI = 1,
    LAYER_TEXT = 2,
    MAX_IMAGE_LAYERS = 3
};

/* Button layers enum */
enum BUTTON_LAYERS {
    BTN_LAYER_MAIN = 0,
    BTN_LAYER_OVERLAY = 1,
    MAX_BUTTON_LAYERS = 2
};

/* Constants */
#define BACKGROUND_FROM_FILE 1
#define BACKGROUND_FROM_DATA 2
#define _TEXTURE_STREAMLINED  0x10
#define DEFAULT_FPS          24

typedef struct GRAPHICS_RENDERER {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Rect win_rect;
    Uint32 ms;

    /* Image management */
    image_element** images[MAX_IMAGE_LAYERS];
    int no_images[MAX_IMAGE_LAYERS];
    int images_size[MAX_IMAGE_LAYERS];

    /* Button management */
    button_element** buttons[MAX_BUTTON_LAYERS];
    int no_buttons[MAX_BUTTON_LAYERS];
    int buttons_size[MAX_BUTTON_LAYERS];
    BOOL* is_button_visible[MAX_BUTTON_LAYERS];
} GRAPHICS_RENDERER;

/* Helper: Centralized Point-in-Rectangle check */
static BOOL is_point_in_rect(int x, int y, SDL_Rect r) {
    return (x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h);
}

/* Updated Background Logic */
void update_background_rectangle(SDL_Rect* backg_rect, SDL_Rect window_rect, void *null) {
    /* RATIO should be defined in your headers */
    backg_rect->w = window_rect.w;
    backg_rect->h = window_rect.h / RATIO;
    backg_rect->x = (window_rect.w - backg_rect->w) / 2;
    backg_rect->y = (window_rect.h - backg_rect->h) / 2;
}

GRAPHICS_RENDERER* create_graphics_renderer(int w, int h, char* Name, char* img_path, int flags, Uint32 render_flags) {
    GRAPHICS_RENDERER* gr;
    image_element* background = NULL;

    gr = (GRAPHICS_RENDERER*)malloc(sizeof(GRAPHICS_RENDERER));
    if (!gr) return NULL;
    memset(gr, 0, sizeof(GRAPHICS_RENDERER));

    gr->window = SDL_CreateWindow(Name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, flags);
    gr->renderer = SDL_CreateRenderer(gr->window, -1, render_flags);

    /* Determine background creation type */
    if (flags & BACKGROUND_FROM_FILE) {
        background = create_new_image_element(gr->renderer, img_path, 0, 0, 0, 0, update_background_rectangle, 0);
    } else {
        background = create_new_image_element(gr->renderer, NULL, 0, 0, w, h, update_background_rectangle, TEXTURE_STREAMLINED);
    }

    /* Initialize background layer */
    gr->images[LAYER_BG] = (image_element**)malloc(sizeof(image_element*));
    gr->images[LAYER_BG][0] = background;
    gr->no_images[LAYER_BG] = 1;
    gr->images_size[LAYER_BG] = 1;

    SDL_GetWindowSize(gr->window, &gr->win_rect.w, &gr->win_rect.h);

    /* Update initial position */
    background->update_rect(&background->rect, gr->win_rect, NULL);
    gr->ms = 1000 / DEFAULT_FPS;

    return gr;
}

void renderer_draw_images(GRAPHICS_RENDERER* gr) {
    int i, j;
    /* Draw Image Layers */
    for (i = 0; i < MAX_IMAGE_LAYERS; i++) {
        for (j = 0; j < gr->no_images[i]; j++) {
            image_element* img = gr->images[i][j];
            if (img->is_visible) {
                SDL_RenderCopy(gr->renderer, img->texture, NULL, &img->rect);
            }
        }
    }

    /* Draw Button Layers */
    for (i = 0; i < MAX_BUTTON_LAYERS; i++) {
        for (j = 0; j < gr->no_buttons[i]; j++) {
            if (gr->is_button_visible[i][j]) {
                button_element* btn = gr->buttons[i][j];
                /* Ternary for cleaner logic: Draw "not_clicked" if visible, else draw "clicked" */
                image_element* active = (btn->image_not_clicked->is_visible) ? btn->image_not_clicked : btn->image_clicked;
                SDL_RenderCopy(gr->renderer, active->texture, NULL, &active->rect);
            }
        }
    }
}

void renderer_handle_click_logic(GRAPHICS_RENDERER* gr, int x, int y) {
    int i, j;
    for (i = 0; i < MAX_BUTTON_LAYERS; i++) {
        for (j = 0; j < gr->no_buttons[i]; j++) {
            if (gr->is_button_visible[i][j]) {
                button_element* btn = gr->buttons[i][j];
                /* Check collision against the clicked state rect */
                if (is_point_in_rect(x, y, btn->image_clicked->rect)) {
                    btn->on_click();
                    /* Toggle visibility states */
                    btn->image_clicked->is_visible = !btn->image_clicked->is_visible;
                    btn->image_not_clicked->is_visible = !btn->image_not_clicked->is_visible;
                }
            }
        }
    }
}

/*
*   Wrapper function to update the background image according to the flags (which are taken from graphics_objects.h)
*/
void renderer_update_bg(GRAPHICS_RENDERER* renderer, char* data, int flags)
{
    image_element_update_graphics(renderer -> images[0][0], data, flags);
}

image_element *renderer_get_bg(GRAPHICS_RENDERER *renderer)
{
    return renderer->images[0][0];
}

/* Update the sizes of the images*/
void renderer_update_rects(GRAPHICS_RENDERER* renderer)
{
    int i,j;
    for (i = 0; i < 3; i+=1)
    {
        for (j = 0; j < renderer -> no_images[i]; j+=1)
        {
            /*update_rectangle_size(&renderer -> images[i][j] -> rect, renderer -> win_rect.w, renderer -> win_rect.h);*/
            renderer -> images[i][j] -> update_rect(&renderer -> images[i][j] -> rect, renderer -> win_rect, NULL);
        }
    }

    for (i = 0; i < 2; ++i)
    {
        for (j = 0; j < renderer -> no_buttons[i]; j+=1)
        {
            renderer -> buttons[i][j] -> image_not_clicked -> update_rect(&renderer -> buttons[i][j] -> image_not_clicked -> rect, renderer -> win_rect, NULL);
            renderer -> buttons[i][j] -> image_clicked -> update_rect(&renderer -> buttons[i][j] -> image_clicked -> rect, renderer -> win_rect, NULL);
        }
    }
}
