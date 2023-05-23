#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include <SDL_ttf.h>

#define IMAGE_FROM_FILE 1
#define IMAGE_FROM_RGB24 2
#define IMAGE_FROM_RGB32 4

#define RATIO (16/9)

typedef struct text_element
{
    TTF_Font* font;
    SDL_Color text_color;
    SDL_Surface* text_surface;
    SDL_Texture* text_texture;
} text_element;

text_element* new_text_element(char* font_path, int font_size, SDL_Color color, char* text, SDL_Renderer* renderer)
{
    text_element* new_text = malloc(sizeof(text_element));
    new_text -> font = TTF_OpenFont(font_path, font_size);
    if (new_text -> font == NULL)
        printf("Error: %s\n", SDL_GetError());
    return NULL;
    new_text -> text_color = color;
    new_text -> text_surface = TTF_RenderText_Blended(new_text -> font, text, new_text -> text_color);
    if (new_text -> text_surface == NULL)
        printf("Error: %s", SDL_GetError());
    new_text -> text_texture = SDL_CreateTextureFromSurface(renderer, new_text -> text_surface);
    return new_text;
}

typedef struct image_element
{
    SDL_Rect rect;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    bool is_visible;
    
    //This function is used to update the image's position, the second parameter is supposed to be the position of the parent window
    void (*update_rect)(SDL_Rect*, SDL_Rect);
} image_element;

image_element* create_new_image_element(SDL_Renderer* renderer, char* Img_path, int x, int y, int w, int h, void (*update_rect_func)(SDL_Rect*, SDL_Rect))
{
    image_element* new_image = (image_element*)malloc(sizeof(image_element));
    new_image -> rect.x = x;
    new_image -> rect.y = y;
    new_image -> rect.w = w;
    new_image -> rect.h = h;

    if (Img_path != NULL)
    {
        //Load the image as a surface
        SDL_Surface *surface = IMG_Load(Img_path);

        //Load the surface into the graphics hardware's memory
        new_image -> texture = SDL_CreateTextureFromSurface(renderer, surface);

        //Load the renderer pointer
        new_image -> renderer = renderer;

        //Free the surface from main memory
        SDL_FreeSurface(surface);
    }

    new_image -> is_visible = true;

    //Set the update_rect function
    new_image -> update_rect = update_rect_func;

    //printf("%p: pointer \n", new_image);

    return new_image;
}

void image_element_update_graphics(image_element* img, char* data, int type_of_image)
{
    //if (img -> texture)
    //{
        //Destroy the old texture
        SDL_DestroyTexture(img -> texture);
    //}

    SDL_Surface* surface = NULL;
    if (type_of_image == IMAGE_FROM_FILE)
    {
        //Load the image as a surface
        surface = IMG_Load(data);

        //Load the surface into the graphics hardware's memory
        img -> texture = SDL_CreateTextureFromSurface(img -> renderer, surface);

        //Free the surface from main memory
        SDL_FreeSurface(surface);
        return;
    }
    else if (type_of_image == IMAGE_FROM_RGB24)
    {
        //Load the image as a surface
        surface = SDL_CreateRGBSurfaceFrom(data, img -> rect.w, img -> rect.h, 24, img -> rect.w * 3, 0, 0, 0, 0);
    }
    else if (type_of_image == IMAGE_FROM_RGB32)
    {
        //Load the image as a surface
        surface = SDL_CreateRGBSurfaceFrom(data, img -> rect.w, img -> rect.h, 32, img -> rect.w * 4, 0, 0, 0, 0);
    }
    if (surface != NULL)    
    {
        img -> texture = SDL_CreateTextureFromSurface(img -> renderer, surface);

        SDL_FreeSurface(surface);
    }
    else
    {
        printf("Error: %s\n", SDL_GetError());
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
 char* Img_path_clicked, int x, int y, int w, int h, void (*update_rect_func)(SDL_Rect*, SDL_Rect))
{
    button_element* new_button = (button_element*)malloc(sizeof(button_element));
    new_button -> image_not_clicked = create_new_image_element(renderer, Img_path_not_clicked, x, y, w, h, update_rect_func);
    new_button -> image_clicked = create_new_image_element(renderer, Img_path_clicked, x, y, w, h, update_rect_func);
    new_button -> is_clicked = false;
    new_button -> on_click = NULL;
    return new_button;
}
