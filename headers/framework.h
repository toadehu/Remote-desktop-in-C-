#pragma once

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
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define CLICK_LEFT 1
#define CLICK_RIGHT 2
#define CLICK_MIDDLE 3
#define CLICK_OEM1 5
#define CLICK_OEM2 6

#ifdef _WIN32
#pragma comment(lib, "msvcrt.lib")
#endif
