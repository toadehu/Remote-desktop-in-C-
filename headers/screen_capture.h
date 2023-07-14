#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef _WIN32

#include <windows.h>
#include <wingdi.h>
#include "memoryapi.h"

#elif __linux__

#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#ifdef HAVE_X11

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#elif HAVE_WAYLAND
#include <wayland-client.h>
#include <wayland-client-protocol.h>

#endif
#endif

void* __aligned_malloc(unsigned long size, unsigned long alignment)
{
    if (alignment < 1024)
        alignment = 1024;
    #ifdef _WIN32
    return VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    #elif __unix__
    void* ptr = NULL;
    if (posix_memalign(&ptr, alignment, size) != 0)
        return NULL;
    return ptr;
    #endif
}

void* __aligned_realloc(void* old_ptr, unsigned long old_size, unsigned long new_size, unsigned long alignment)
{
    if (alignment < 1024)
        alignment = 1024;

    void* new_ptr = NULL;

    #ifdef _WIN32
    new_ptr = VirtualAlloc(NULL, new_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (new_ptr != NULL) {
        memcpy(new_ptr, old_ptr, old_size < new_size ? old_size : new_size);
        VirtualFree(old_ptr, 0, MEM_RELEASE);
    }
    #elif __unix__
    if (posix_memalign(&new_ptr, alignment, new_size) == 0) {
        if (new_ptr != NULL) {
            memcpy(new_ptr, old_ptr, old_size < new_size ? old_size : new_size);
            free(old_ptr);
        }
    }
    #endif

    return new_ptr;
}

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
Uint32 rmask = 0xff000000;
Uint32 gmask = 0x00ff0000;
Uint32 bmask = 0x0000ff00;
Uint32 amask = 0x000000ff;
#else
Uint32 rmask = 0x000000ff;
Uint32 gmask = 0x0000ff00;
Uint32 bmask = 0x00ff0000;
Uint32 amask = 0xff000000;
#endif

#ifdef HAVE_WAYLAND
#if __has_include(<wayland-client.h>)

#define WAYLAND_AVAILABLE

#endif
#endif

#ifdef HAVE_WAYLAND

struct wl_data {
    struct wl_shm *shm;
    struct wl_compositor *compositor;
    struct wl_output *output;
    int32_t width, height;
};

static void registry_global(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version)
{
    struct wl_data *wl_data = (struct wl_data *)data;

    if (strcmp(interface, wl_shm_interface.name) == 0)
    {
        wl_data->shm = wl_registry_bind(registry, name, &wl_shm_interface, 1);
    }
    else if (strcmp(interface, wl_compositor_interface.name) == 0)
    {
        wl_data->compositor = wl_registry_bind(registry, name, &wl_compositor_interface, 1);
    }
    else if (strcmp(interface, wl_output_interface.name) == 0)
    {
        wl_data->output = wl_registry_bind(registry, name, &wl_output_interface, 1);
    }
}

static void registry_global_remove(void *data, struct wl_registry *registry, uint32_t name) {}

static const struct wl_registry_listener registry_listener = {
    .global = registry_global,
    .global_remove = registry_global_remove,
};

static void output_geometry(void *data, struct wl_output *output, int32_t x, int32_t y,
 int32_t physical_width, int32_t physical_height, int32_t subpixel,
  const char *make, const char *model, int32_t transform)
{
    struct wl_data *wl_data = (struct wl_data *)data;
    wl_data->width = physical_width;
    wl_data->height = physical_height;
}

static void output_mode(void *data, struct wl_output *output, uint32_t flags,
 int32_t width, int32_t height, int32_t refresh)
 {
    if (flags & WL_OUTPUT_MODE_CURRENT) {
        struct wl_data *wl_data = (struct wl_data *)data;
        wl_data->width = width;
        wl_data->height = height;
    }
}

static void output_done(void *data, struct wl_output *output) {}

static void output_scale(void *data, struct wl_output *output, int32_t factor) {}

static const struct wl_output_listener output_listener = {
    .geometry = output_geometry,
    .mode = output_mode,
    .done = output_done,
    .scale = output_scale,
};

#endif

typedef struct SCREEN_CAPTURE
{
    int placeholder; /*place holder for something*/
} screen_capture;

void capture_screen(char **_buffer, int *_size, int *_width, int *_height)
{
    /* Screenshots are operating system dependent sadly*/
    /* So this is needed*/

#ifdef _WIN32
    /* Get the dimensions of the screen*/
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    *_width = width;
    *_height = height;

    /* Create a buffer to hold the image data*/
    if ((*_size) != width * height * 4)
    {
        if (*_buffer != NULL)
        {
            free(*_buffer);
        }
        (* _size) = width * height * 4;
        *_buffer = (char *)__aligned_malloc(*_size, 1024);
    }

    *_size = width * height * 4;

    /* Get the screen device context*/
    HDC hdcScreen = GetDC(NULL);

    /* Create a compatible device context and bitmap*/
    HDC hdcMemDC = CreateCompatibleDC(hdcScreen);
    HBITMAP hbmScreen = CreateCompatibleBitmap(hdcScreen, width, height);

    /* Select the bitmap into the device context*/
    HGDIOBJ oldObj = SelectObject(hdcMemDC, hbmScreen);

    /* Copy the screen contents into the bitmap*/
    BitBlt(hdcMemDC, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY | CAPTUREBLT);

    /* Copy the bitmap data into the buffer*/
    BITMAPINFOHEADER bi = {sizeof(BITMAPINFOHEADER), width, height, 1, 32, BI_RGB, 0, 0, 0, 0, 0};

    /* Get the cursor position and dimensions*/
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    int cursorX = cursorPos.x;
    int cursorY = cursorPos.y;
    CURSORINFO cursorInfo = { sizeof(CURSORINFO) };
    GetCursorInfo(&cursorInfo);

    /* Capture the cursor if it is visible*/
    if (cursorInfo.flags == CURSOR_SHOWING)
    {
        ICONINFO cursorIconInfo;
        GetIconInfo(cursorInfo.hCursor, &cursorIconInfo);

        /* Get the size of the cursor */
        BITMAP bm;
        GetObject(cursorIconInfo.hbmColor, sizeof(BITMAP), &bm);
        int cursorWidth = bm.bmWidth;
        int cursorHeight = bm.bmHeight;

        /* Create a bitmap for the cursor and select it into a device context*/
        HDC hdcCursor = CreateCompatibleDC(hdcScreen);
        HBITMAP hbmCursor = CreateBitmap(cursorWidth, cursorHeight, 1, 32, NULL);
        HGDIOBJ oldCursorObj = SelectObject(hdcCursor, hbmCursor);

        /* Draw the cursor into the bitmap*/
        DrawIconEx(hdcCursor, 0, 0, cursorInfo.hCursor, cursorWidth, cursorHeight, 0, NULL, DI_NORMAL);

        /* Copy the cursor bitmap into the screen bitmap at the cursor position*/
        BITMAP bmCursor;
        GetObject(hbmCursor, sizeof(BITMAP), &bmCursor);
        BITMAPINFO bmi = {sizeof(BITMAPINFOHEADER), bmCursor.bmWidth, bmCursor.bmHeight, 1, 32, BI_RGB, 0, 0, 0, 0, 0};
        GetDIBits(hdcCursor, hbmCursor, 0, bmCursor.bmHeight, NULL, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);
        /* Draw the cursor into the screen bitmap at the cursor position */
        BitBlt(hdcMemDC, cursorX - cursorIconInfo.xHotspot, cursorY - cursorIconInfo.yHotspot, cursorWidth, cursorHeight, hdcCursor, 0, 0, SRCCOPY);

        /* Clean up the cursor bitmap and device context*/
        /*free(cursorBits);
        DeleteObject(hbmCursor);
        DeleteDC(hdcCursor);*/
    }

    GetDIBits(hdcScreen, hbmScreen, 0, height, *_buffer, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    /*We have to flip the image vertically because windows is funny and quirky
    BUT ONLY BEFORE WE ATTEMPT TO DRAW THE CURSOR BECAUSE THAT IS OBVIOUSLY NOT FLIPPED*/
    int rowSize = width * 4;
    char* tempRow = (char*)malloc(rowSize);

    int i;
    for (i = 0; i < height / 2; i++) {
        int j = height - i - 1;
        char* row1 = (*_buffer) + i * rowSize;
        char* row2 = (*_buffer) + j * rowSize;

        // Swap rows using a temporary buffer
        memcpy(tempRow, row1, rowSize);
        memcpy(row1, row2, rowSize);
        memcpy(row2, tempRow, rowSize);
    }

    free(tempRow);

    /* Clean up the screen bitmap and device contexts*/
    SelectObject(hdcMemDC, oldObj);
    DeleteObject(hbmScreen);
    DeleteDC(hdcMemDC);
    ReleaseDC(NULL, hdcScreen);

#elif __linux__

#ifdef HAVE_X11
    /* Connect to the X server*/
    Display* display = XOpenDisplay(NULL);
    Window root = DefaultRootWindow(display);

    XWindowAttributes attributes;
    XGetWindowAttributes(display, root, &attributes);

    int Width = attributes.width;
    int Height = attributes.height;

    *_width = Width;
    *_height = Height;

    XImage* image = XGetImage(display, root, 0, 0 , Width, Height, AllPlanes, ZPixmap);

    if (image == NULL) {
    printf("Unable to get image data from the root window\n");
    XCloseDisplay(display);
    exit(EXIT_FAILURE);
    }

    if ((*_size) != Width * Height * (image -> bits_per_pixel >> 3))
    {
        if (*_buffer != NULL)
        {
            free(*_buffer);
        }
        *_buffer = (unsigned char *)__aligned_malloc(Width * Height * (image -> bits_per_pixel >> 3), 16);
    }

    (*_size) = Width * Height * (image -> bits_per_pixel >> 3);

    #ifdef _DEBUG
    printf("My attemp at the size: %d, the bit depth: %d\n", (*_size), image -> bits_per_pixel);
    #endif

    memcpy(*_buffer, image->data, *_size);

    #ifdef _DEBUG
    printf("Memcpy has executed succesfullt\n");
    #endif

    XDestroyImage(image);
    XCloseDisplay(display);
    #ifdef _DEBUG
    printf("Finished capturing X screenshot\n");
    #endif
#else
    struct wl_display *display = wl_display_connect(NULL);
    if (!display) {
        fprintf(stderr, "Failed to connect to Wayland display.\n");
        exit(EXIT_FAILURE);
    }

    struct wl_data wl_data = {0};

    struct wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, &wl_data);
    wl_display_roundtrip(display);

    if (!wl_data.shm || !wl_data.compositor || !wl_data.output) {
        fprintf(stderr, "Failed to get Wayland interfaces.\n");
        exit(EXIT_FAILURE);
    }

    wl_output_add_listener(wl_data.output, &output_listener, &wl_data);
    wl_display_roundtrip(display);

    int stride = wl_data.width * 4; /* Assuming ARGB8888 format*/
    int size = stride * wl_data.height;

    int fd = memfd_create("screenshot", 0);
    ftruncate(fd, size);

    void *data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    struct wl_shm_pool *pool = wl_shm_create_pool(wl_data.shm, fd, size);
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, 0, wl_data.width, wl_data.height, stride, WL_SHM_FORMAT_ARGB8888);

    struct wl_surface *surface = wl_compositor_create_surface(wl_data.compositor);
    wl_surface_attach(surface, buffer, 0, 0);
    wl_surface_damage(surface, 0, 0, wl_data.width, wl_data.height);
    wl_surface_commit(surface);
    wl_display_roundtrip(display);

    /* Copy the screenshot data to an unsigned char* buffer*/
    if ((*_size) != size)
    {
        if (*_buffer != NULL)
        {
            free(*_buffer);
        }
       *_buffer = (unsigned char *)__aligned_malloc(size, 16);
    }
    /*printf("Top 10 printf before segfault\n");*/
    memcpy(*_buffer, data, size);

#ifdef _DEBUG
    printf("Size of screenshot: %d\n", size);
#endif

    /*copy the size to our variable, width and height also*/
    (*_size) = size;
    *_width = wl_data.width;
    *_height = wl_data.height;

    /* Clean up*/
    munmap(data, size);
    close(fd);
    wl_buffer_destroy(buffer);
    wl_shm_pool_destroy(pool);
    wl_surface_destroy(surface);
    wl_output_destroy(wl_data.output);
    wl_compositor_destroy(wl_data.compositor);
    wl_shm_destroy(wl_data.shm);
    wl_registry_destroy(registry);
    wl_display_disconnect(display);
#endif
#endif
}
