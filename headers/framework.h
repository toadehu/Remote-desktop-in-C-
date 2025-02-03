#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include <SDL_ttf.h>
#include <SDL_syswm.h>
#include "graphics_objects.h"
#include "server.h"
#include "graphics_renderer.h"
#include "screen_capture.h"
#include "video_encoder.h"
#include "basic_video.h"
#include "inputs.h"
#include "opencl_wrap.h"
#include "cl_shared_items.h"


const int YUV420 = 1; /* Y Cb Cr with 4 : 2 : 0 subsamping (channels Cb and Cr are down to 1/4 size) */
const int YUV444 = 2; /* Y Cb Cr with no subsampling */
const int RGB8   = 3; /* RGB with 8 bits per pixel */
const int ARGB8 = 4; /* ARGB with 8 bits per pixel */

enum frame_type
{
    new_frame = 0x01,
    mouse_input_release = 0x00,
    mouse_input_hold = 0x02,
    mouse_input_move = 0x03,
    mouse_input_click = 0x04,
    mouse_input_scroll = 0x05,
    mouse_input_oem = 0x05,
    keybd_input = 0x06
};

enum encoded_type
{
    jpeg_no_huff = 0x01,
    jpeg_reg = 0x02,
    rle = 0x0F /* Ideally this is never used from now on but who knows*/
};

#define JPEG_ENCODED jpeg_no_huff | jpeg_reg


#define CLICK_LEFT 1
#define CLICK_RIGHT 2
#define CLICK_MIDDLE 3
#define CLICK_OEM1 5
#define CLICK_OEM2 6

#define VALIDATE(expr) if (!(expr)) {exit(0);}

#ifdef _MSC_VER
#ifdef _WIN32
#pragma comment(lib, "msvcrt.lib")
#endif
#endif
