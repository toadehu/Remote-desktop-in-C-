#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include <SDL_ttf.h>

#include "screen_capture.h"

#ifdef _WIN32
/* This is so stupid */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#endif

#define IMAGE_FROM_FILE 1
#define IMAGE_FROM_RGB24 2
#define IMAGE_FROM_RGB32 4

#define TEXTURE_STREAMLINED 1

#define RATIO (16/9)

typedef struct text_element
{
    TTF_Font* font;
    SDL_Color text_color;
    SDL_Surface* text_surface;
    SDL_Texture* text_texture;
    char* text;
    int nr_chars;
    int text_len;

    bool is_visible;

    /*This function is used to update the image's position, the second parameter is the position of the parent window*/
    void (*update_rect)(SDL_Rect*, SDL_Rect);
} text_element;

text_element* new_text_element(char* font_path, int font_size, SDL_Color color, char* text, SDL_Renderer* renderer)
{
    text_element* new_text = (text_element*)malloc(sizeof(text_element));
    new_text -> font = TTF_OpenFont(font_path, font_size);
    if (new_text->font == NULL)
    {
        printf("Error: %s\n", SDL_GetError());
        return NULL;
    }
    new_text -> text_color = color;
    new_text -> text_surface = TTF_RenderText_Blended(new_text -> font, text, new_text -> text_color);
    if (new_text->text_surface == NULL)
    {
        printf("Error: %s", SDL_GetError());
        return NULL;
    }
    new_text -> text_texture = SDL_CreateTextureFromSurface(renderer, new_text -> text_surface);

    new_text -> text = (char*)malloc(10);
    new_text -> text_len = 10;
    new_text -> nr_chars = 0;
    return new_text;
}

/**
 * Adds a character to the text_element
 * @param te a pointer to the text_element struct
 * @param c the character to be added
 * @return 0 on succes, 1 on failure
*/
bool text_element_add_character(text_element* te, char c)
{
    if (te -> text_len == (te -> nr_chars + 1))
    {
        te -> text_len *= 2;
        char* new_text = (char*)malloc(te->text_len);
        /* Something went horribly wrong */
        if (new_text == NULL)
        {
            return 1;
        }
        memcpy(new_text, te->text, te->nr_chars);
        free(te->text);
        te->text = new_text;
    }
    te -> text[te -> nr_chars + 1] = c;
    te -> nr_chars += 1;
    te -> text[te -> nr_chars + 1] = '\0';
    return 0;
}

/**
 * Removes a character from the text_element
 * @param te a pointer to the text_element struct
 * @return 0 on normal operation, 1 if there is no character to remove
*/
bool text_element_remove_character(text_element* te)
{
    if (te -> nr_chars == 0)
    {
        return 1;
    }
    te -> text[te -> nr_chars] = '\0';
    te -> nr_chars -= 1;
    return 0;
}

typedef struct image_element
{
    SDL_Rect rect;
    SDL_Texture* texture;
    void* pixels;
    SDL_Renderer* renderer;
    bool is_visible;
    int flags;

    /*This function is used to update the image's position, the second parameter is supposed to be the position of the parent window*/
    void (*update_rect)(SDL_Rect*, SDL_Rect, void*);
} image_element;


/**
 * @brief Creates a new image_element
 * 
 * @param renderer a pointer to the renderer
 * @param Img_path the path to the image
 * @param x the x position of the image
 * @param y the y position of the image
 * @param w the width of the image
 * @param h the height of the image
 * @param update_rect_func a pointer to the function that updates the image's position
 * @param flags the flags for the image creation (If you load an image from a file DO NOT SET TETXURE_STREAMLINED)
 * @return a pointer to the new image_element
*/
image_element* create_new_image_element(SDL_Renderer* renderer, char* Img_path, int x, int y, int w, int h, void (*update_rect_func)(SDL_Rect*, SDL_Rect, void*), int flags)
{
    image_element* new_image = (image_element*)malloc(sizeof(image_element));
    new_image -> rect.x = x;
    new_image -> rect.y = y;
    new_image -> rect.w = w;
    new_image -> rect.h = h;
    new_image -> flags = flags;

    if (Img_path != NULL)
    {
        /*Load the image as a surface*/
        SDL_Surface *surface = IMG_Load(Img_path);

        /*Load the surface into the graphics hardware's memory*/
        new_image -> texture = SDL_CreateTextureFromSurface(renderer, surface);

        /*Load the renderer pointer*/
        new_image -> renderer = renderer;

        /*Free the surface from main memory*/
        SDL_FreeSurface(surface);
    }
    else if (flags & TEXTURE_STREAMLINED)
    {
        new_image -> pixels = __aligned_malloc(w * h * 4, 4096);
        new_image -> texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, w, h);
    }

    new_image -> is_visible = true;

    /*Set the update_rect function*/
    new_image -> update_rect = update_rect_func;

    /*printf("%p: pointer \n", new_image);*/

    return new_image;
}

/**
 * @brief Updates the image's position
 * 
 * @param img a pointer to the image_element
 * @param data a pointer to the data that is used by the update_rect function (If img uses streamlining, this needs to be ARGB888 data)
 * @param type_of_image the type of image (IMAGE_FROM_FILE or IMAGE_FROM_RGB24)
*/
void image_element_update_graphics(image_element* img, char* data, int type_of_image)
{
    SDL_Surface* surface = NULL;
    SDL_Texture* new_texture = NULL;

    if (img->flags == TEXTURE_STREAMLINED)
    {
        int pitch;

        SDL_LockTexture(img -> texture, NULL, &img -> pixels, &pitch);

        memcpy(img -> pixels, data, img -> rect.w * img -> rect.h * 4);

        SDL_UnlockTexture(img -> texture);

        return;
    }

    if (type_of_image == IMAGE_FROM_FILE)
    {
        /*Load the image as a surface*/
        surface = IMG_Load(data);
    }
    else if (type_of_image == IMAGE_FROM_RGB24)
    {
        /*Load the image as a surface*/
        surface = SDL_CreateRGBSurfaceFrom(data, img->rect.w, img->rect.h, 24, img->rect.w * 3, 0, 0, 0, 0);
    }
    else if (type_of_image == IMAGE_FROM_RGB32)
    {
        /*Load the image as a surface*/
        surface = SDL_CreateRGBSurfaceFrom(data, img->rect.w, img->rect.h, 32, img->rect.w * 4, 0, 0, 0, 0);
    }

    if (surface != NULL)
    {
        new_texture = SDL_CreateTextureFromSurface(img->renderer, surface);
        SDL_FreeSurface(surface);
    }
    else
    {
        printf("Error: %s\n", SDL_GetError());
    }

    if (new_texture != NULL)
    {
        if (img->texture)
        {
            SDL_DestroyTexture(img->texture);
        }

        img->texture = new_texture;
    }
    else
    {
        printf("Error: Failed to create texture: %s\n", SDL_GetError());
    }
}


typedef struct button_element
{
    image_element* image_not_clicked;
    image_element* image_clicked;
    bool is_clicked;
    void (*on_click)(void);
} button_element;

button_element* create_new_button_element(SDL_Renderer* renderer, char* Img_path_not_clicked,
 char* Img_path_clicked, int x, int y, int w, int h, void (*update_rect_func)(SDL_Rect*, SDL_Rect, void*))
{
    button_element* new_button = (button_element*)malloc(sizeof(button_element));
    new_button -> image_not_clicked = create_new_image_element(renderer, Img_path_not_clicked, x, y, w, h, update_rect_func, 0);
    new_button -> image_clicked = create_new_image_element(renderer, Img_path_clicked, x, y, w, h, update_rect_func, 0);
    new_button -> is_clicked = false;
    new_button -> on_click = NULL;
    return new_button;
}
