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

enum frame_type
{
    new_frame = 0x01,
    mouse_input_move = 0x02,
    mouse_input_click = 0x04,
    mouse_input_scroll = 0x08,
    mouse_input_oem = 0x10,
    keybd_input = 0x20,
    auth = 0x80
};


#define CLICK_LEFT 1
#define CLICK_RIGHT 2
#define CLICK_MIDDLE 3
#define CLICK_OEM1 5
#define CLICK_OEM2 6

#ifdef _MSC_VER
#ifdef _WIN32
#pragma comment(lib, "msvcrt.lib")
#endif
#endif
