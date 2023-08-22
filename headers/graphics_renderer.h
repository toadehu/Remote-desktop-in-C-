#pragma once

#include "graphics_objects.h"

#define BACKGROUND_FROM_FILE 1
#define BACKGROUND_FROM_DATA 2
#define _TEXTURE_STREAMLINED 0x10

typedef struct _GRAPHICS_RENDERER 
{
    /*The window that will be rendered on*/
    SDL_Window* window;

    /*The renderer that will be used to render on the window*/
    SDL_Renderer* renderer;

    /*The rectangle for the window size*/
    SDL_Rect win_rect;
    
    /*the frames per second*/
    Uint32 ms;

    /*The first array is for background images, the second for buttons, third for text or popups*/
    image_element** images[3];

    /*How many images are in the array*/
    int no_images[3];

    /*The size of the arrays*/
    int images_size[3];

    /*The array of buttons*/
    button_element** buttons[2];

    /*How many buttons are in the array*/
    int no_buttons[2];

    /*The size of the arrays*/
    int buttons_size[2];

    /*Is it visibe*/
    bool* is_button_visible[2];

}GRAPHICS_RENDERER;

void update_background_rectangle(SDL_Rect* backg_rect, SDL_Rect window_rect)
{
    if (backg_rect->w != window_rect.w || backg_rect->h != window_rect.h)
    {

    }
    backg_rect -> w = window_rect.w;
    backg_rect -> h = window_rect.h / RATIO;
    backg_rect -> x = (window_rect.w - backg_rect -> w) / 2;
    backg_rect -> y = (window_rect.h - backg_rect -> h) / 2;
}

void update_rectangle_size(SDL_Rect* rect, int w, int h)
{
    rect -> w = w;
    rect -> h = h;
}

GRAPHICS_RENDERER* create_graphics_renderer(int w, int h, char* Name, char* img_path, int flags, Uint32 render_flags)
{
    GRAPHICS_RENDERER* graphics_renderer = (GRAPHICS_RENDERER*)malloc(sizeof(GRAPHICS_RENDERER));
    memset(graphics_renderer, 0, sizeof(GRAPHICS_RENDERER));

	graphics_renderer -> window = SDL_CreateWindow(Name,
						SDL_WINDOWPOS_CENTERED,
						SDL_WINDOWPOS_CENTERED,
						w, h, flags);

	/*create a renderer*/
	graphics_renderer ->  renderer = SDL_CreateRenderer(graphics_renderer -> window, -1, render_flags);

    image_element* background = NULL;

    if (flags & BACKGROUND_FROM_FILE)
        background = create_new_image_element(graphics_renderer -> renderer, img_path, 0, 0, 0, 0, update_background_rectangle, 0);
    else if (flags & BACKGROUND_FROM_DATA)
        background = create_new_image_element(graphics_renderer -> renderer, NULL, 0, 0, w, h, update_background_rectangle, TEXTURE_STREAMLINED);
	else /* Lazy default */
		background = create_new_image_element(graphics_renderer -> renderer, NULL, 0, 0, 0, 0, update_background_rectangle, TEXTURE_STREAMLINED);


    graphics_renderer -> images[0] = (image_element**)malloc(sizeof(image_element*));

    graphics_renderer -> images[0][0] = background;

    /*Set the correct number of images for the background, this should always be 1*/
    graphics_renderer -> no_images[0] = 1;
    graphics_renderer -> images_size[0] = 1;

    /*Set the correct number of images for the buttons and overlays, this should be 0 initially*/
    graphics_renderer -> no_images[1] = 0;
    graphics_renderer -> images_size[1] = 0;
    graphics_renderer -> no_images[2] = 0;
    graphics_renderer -> images_size[2] = 0;

	/*Get the window size for the rendering the image*/
	SDL_GetWindowSize(graphics_renderer -> window, &graphics_renderer -> win_rect.w, &graphics_renderer -> win_rect.h);

	/*set position*/
    graphics_renderer -> images[0][0] -> update_rect(&graphics_renderer -> images[0][0] -> rect, graphics_renderer -> win_rect, NULL);

    /*set the delay for 24ms*/
    graphics_renderer -> ms = 1000 / 24;

    return graphics_renderer;
}

/*
*   Wrapper function to update the background image according to the flags (which are taken from graphics_objects.h)
*/
void renderer_update_bg(GRAPHICS_RENDERER* renderer, char* data, int flags)
{
    image_element_update_graphics(renderer -> images[0][0], data, flags);
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

/* Draw the images*/
void renderer_draw_images(GRAPHICS_RENDERER* renderer)
{
    int i,j;
    for (i = 0; i < 3; i+=1)
    {
        for (j = 0; j < renderer -> no_images[i]; j+=1)
        {
            if (renderer -> images[i][j] -> is_visible == true)
                SDL_RenderCopy(renderer -> renderer, renderer -> images[i][j] -> texture, NULL, &renderer -> images[i][j] -> rect);
        }
    }

    for (i = 0; i < 2; i += 1)
    {
        for (j = 0; j < renderer -> no_buttons[i]; j += 1)
        {
            if (renderer -> is_button_visible[i][j]) {
                if (renderer -> buttons[i][j] -> image_not_clicked -> is_visible == true){
                    SDL_RenderCopy(renderer -> renderer, renderer -> buttons[i][j] -> image_not_clicked -> texture, NULL, &renderer -> buttons[i][j] -> image_not_clicked -> rect);
                }
                else {
                    SDL_RenderCopy(renderer -> renderer, renderer -> buttons[i][j] -> image_clicked -> texture, NULL, &renderer -> buttons[i][j] -> image_clicked -> rect);}
            }
        }
    }
}

void renderer_add_button(GRAPHICS_RENDERER* renderer, button_element* button)
{
    if (renderer -> no_buttons[0] == renderer -> buttons_size[0])
    {
        if (renderer -> buttons_size[0] == 0)
            renderer -> buttons_size[0] = 1;
        else
            renderer -> buttons_size[0] *= 2;
        renderer -> buttons[0] = (button_element**)realloc(renderer -> buttons[0], renderer -> buttons_size[0] * sizeof(button_element*));
        renderer -> is_button_visible[0] = (bool*)realloc(renderer -> is_button_visible[0], renderer -> buttons_size[0] * sizeof(bool));
        if (renderer -> buttons[0] == NULL || renderer -> is_button_visible[0] == NULL)
        {
            printf("Error allocating memory for buttons");
        }
    }
    renderer -> buttons[0][renderer -> no_buttons[0]] = button;
    renderer -> buttons[0][renderer -> no_buttons[0]] -> image_not_clicked -> is_visible = true;
    renderer -> buttons[0][renderer -> no_buttons[0]] -> image_clicked -> is_visible = false;
    renderer -> is_button_visible[0][renderer -> no_buttons[0]] = true;
    renderer -> no_buttons[0] += 1;
}

void renderer_add_image(GRAPHICS_RENDERER* renderer, image_element* image)
{
    if (renderer -> no_images[1] == renderer -> images_size[1])
    {
        if (renderer -> images_size[1] == 0)
            renderer -> images_size[1] = 1;
        else
            renderer -> images_size[1] *= 2;
        renderer -> images[1] = (image_element**)realloc(renderer -> images[1], renderer -> images_size[1] * sizeof(image_element*));
        if (renderer -> images[1] == NULL)
        {
            printf("Error reallocating memory for images array, something went horribly wrong, exiting...\n");
            exit(1);
        }
    }
    renderer -> images[1][renderer -> no_images[1]] = image;
    renderer -> images[1][renderer -> no_images[1]]  -> is_visible = true;
    renderer -> no_images[1] += 1;
}

void renderer_handle_click(GRAPHICS_RENDERER* renderer, int x, int y)
{
    int i,j;
    for (i = 0; i < 2; i += 1)
    {
        for (j = 0; j < renderer -> no_images[i]; j += 1)
        {
            if (renderer -> is_button_visible[i][j])
            {
                if (x >= renderer -> buttons[i][j] -> image_clicked -> rect.x && x <= renderer -> buttons[i][j] -> image_clicked -> rect.x + renderer -> buttons[i][j] -> image_clicked -> rect.w &&
                    y >= renderer -> buttons[i][j] -> image_clicked -> rect.y && y <= renderer -> buttons[i][j] -> image_clicked -> rect.y + renderer -> buttons[i][j] -> image_clicked -> rect.h) 
                {
                    renderer -> buttons[i][j] -> on_click();
                    if (renderer -> buttons[i][j] -> image_clicked -> is_visible == true)
                    {
                        renderer -> buttons[i][j] -> image_clicked -> is_visible = false;
                        renderer -> buttons[i][j] -> image_not_clicked -> is_visible = true;
                    }
                    else
                    {
                        renderer -> buttons[i][j] -> image_clicked -> is_visible = true;
                        renderer -> buttons[i][j] -> image_not_clicked -> is_visible = false;
                    }
                }
            }
        }
    }
}

void renderer_handle_click_released(GRAPHICS_RENDERER* renderer, int x, int y)
{
    int i,j;    
    for (i = 0; i < 2; i += 1)
    {
        for (j = 0; j < renderer -> no_images[i]; j += 1)
        {
            if (renderer -> is_button_visible[i][j])
            {
                if (x >= renderer -> buttons[i][j] -> image_clicked -> rect.x && x <= renderer -> buttons[i][j] -> image_clicked -> rect.x + renderer -> buttons[i][j] -> image_clicked -> rect.w &&
                    y >= renderer -> buttons[i][j] -> image_clicked -> rect.y && y <= renderer -> buttons[i][j] -> image_clicked -> rect.y + renderer -> buttons[i][j] -> image_clicked -> rect.h) 
                {
                    renderer -> buttons[i][j] -> on_click();
                    if (renderer -> buttons[i][j] -> image_clicked -> is_visible == true)
                    {
                        renderer -> buttons[i][j] -> image_clicked -> is_visible = false;
                        renderer -> buttons[i][j] -> image_not_clicked -> is_visible = true;
                    }
                    else
                    {
                        renderer -> buttons[i][j] -> image_clicked -> is_visible = true;
                        renderer -> buttons[i][j] -> image_not_clicked -> is_visible = false;
                    }
                }
            }
        }
    }
}

