#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
#define _BSD_SOURCE
#define _USLEEP

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>


/* Alphabet keys */
#define SDLK_TO_UINPUT_A      KEY_A
#define SDLK_TO_UINPUT_B      KEY_B
#define SDLK_TO_UINPUT_C      KEY_C
#define SDLK_TO_UINPUT_D      KEY_D
#define SDLK_TO_UINPUT_E      KEY_E
#define SDLK_TO_UINPUT_F      KEY_F
#define SDLK_TO_UINPUT_G      KEY_G
#define SDLK_TO_UINPUT_H      KEY_H
#define SDLK_TO_UINPUT_I      KEY_I
#define SDLK_TO_UINPUT_J      KEY_J
#define SDLK_TO_UINPUT_K      KEY_K
#define SDLK_TO_UINPUT_L      KEY_L
#define SDLK_TO_UINPUT_M      KEY_M
#define SDLK_TO_UINPUT_N      KEY_N
#define SDLK_TO_UINPUT_O      KEY_O
#define SDLK_TO_UINPUT_P      KEY_P
#define SDLK_TO_UINPUT_Q      KEY_Q
#define SDLK_TO_UINPUT_R      KEY_R
#define SDLK_TO_UINPUT_S      KEY_S
#define SDLK_TO_UINPUT_T      KEY_T
#define SDLK_TO_UINPUT_U      KEY_U
#define SDLK_TO_UINPUT_V      KEY_V
#define SDLK_TO_UINPUT_W      KEY_W
#define SDLK_TO_UINPUT_X      KEY_X
#define SDLK_TO_UINPUT_Y      KEY_Y
#define SDLK_TO_UINPUT_Z      KEY_Z


/* Numeric keys */
#define SDLK_TO_UINPUT_0      KEY_0
#define SDLK_TO_UINPUT_1      KEY_1
#define SDLK_TO_UINPUT_2      KEY_2
#define SDLK_TO_UINPUT_3      KEY_3
#define SDLK_TO_UINPUT_4      KEY_4
#define SDLK_TO_UINPUT_5      KEY_5
#define SDLK_TO_UINPUT_6      KEY_6
#define SDLK_TO_UINPUT_7      KEY_7
#define SDLK_TO_UINPUT_8      KEY_8
#define SDLK_TO_UINPUT_9      KEY_9


/* Modifier keys */
#define SDLK_TO_UINPUT_LSHIFT KEY_LEFTSHIFT
#define SDLK_TO_UINPUT_RSHIFT KEY_RIGHTSHIFT
#define SDLK_TO_UINPUT_LCTRL  KEY_LEFTCTRL
#define SDLK_TO_UINPUT_RCTRL  KEY_RIGHTCTRL
#define SDLK_TO_UINPUT_LALT   KEY_LEFTALT
#define SDLK_TO_UINPUT_RALT   KEY_RIGHTALT
#define SDLK_TO_UINPUT_LGUI   KEY_LEFTMETA   /* Left "Windows" or "Command" key */
#define SDLK_TO_UINPUT_RGUI   KEY_RIGHTMETA  /* Right "Windows" or "Command" key */

/* Arrow keys */
#define SDLK_TO_UINPUT_UP     KEY_UP
#define SDLK_TO_UINPUT_DOWN   KEY_DOWN
#define SDLK_TO_UINPUT_LEFT   KEY_LEFT
#define SDLK_TO_UINPUT_RIGHT  KEY_RIGHT

/* SDL keys to uint32_tegers */
#define SDLK_A 97
#define SDLK_B 98
#define SDLK_C 99
#define SDLK_D 100
#define SDLK_E 101
#define SDLK_F 102
#define SDLK_G 103
#define SDLK_H 104
#define SDLK_I 105
#define SDLK_J 106
#define SDLK_K 107
#define SDLK_L 108
#define SDLK_M 109
#define SDLK_N 110
#define SDLK_O 111
#define SDLK_P 112
#define SDLK_Q 113
#define SDLK_R 114
#define SDLK_S 115
#define SDLK_T 116
#define SDLK_U 117
#define SDLK_V 118
#define SDLK_W 119
#define SDLK_X 120
#define SDLK_Y 121
#define SDLK_Z 122

/* Regular number keys */
#define SDLK_0 48
#define SDLK_1 49
#define SDLK_9 57

/* Numpad keys */
#define SDLK_KP_0 1073741922
#define SDLK_KP_1 1073741913
#define SDLK_KP_2 1073741914
#define SDLK_KP_3 1073741915
#define SDLK_KP_4 1073741916
#define SDLK_KP_5 1073741917
#define SDLK_KP_6 1073741918
#define SDLK_KP_7 1073741919
#define SDLK_KP_8 1073741920
#define SDLK_KP_9 1073741921
#define SDLK_KP_DIVIDE 1073741908
#define SDLK_KP_MULTIPLY 1073741909
#define SDLK_KP_MINUS 1073741910
#define SDLK_KP_PLUS 1073741911
#define SDLK_KP_ENTER 1073741912
#define SDLK_KP_PERIOD 1073741923
#define SDLK_KP_EQUALS 1073741927
#define SDLK_KP_COMMA 1073741957
#define SDLK_KP_EQUALSAS400 1073741958

/* Function keys */
#define SDLK_F1 1073741882
#define SDLK_F12 1073741893
#define SDLK_F13 1073741928
#define SDLK_F24 1073741939

/* Some modifier keys*/
#define SDLK_LSHIFT 1073742049
#define SDLK_RSHIFT 1073742053
#define SDLK_LCTRL 1073742048
#define SDLK_RCTRL 1073742052
#define SDLK_LALT 1073742050
#define SDLK_RALT 1073742054
#define SDLK_LGUI 1073742047 /* This is the "Windows" key on Windows, "Command" key on Mac */
#define SDLK_RGUI 1073742055
#define SDLK_MODE 1073742081

/* The symbol keys like period, colon and so on*/
#define SDLK_PERIOD 0x2E
#define SDLK_COMMA 0x2C
#define SDLK_SLASH 0x2F
#define SDLK_BACKSLASH 0x5C
#define SDLK_SEMICOLON 0x3B
#define SDLK_QUOTE 0x27
#define SDLK_LEFTBRACKET 0x5B
#define SDLK_RIGHTBRACKET 0x5D
#define SDLK_MINUS 0x2D
#define SDLK_EQUALS 0x3D

/* More special keys */
#define SDLK_RETURN 13
#define SDLK_ESCAPE 27
#define SDLK_BACKSPACE 8
#define SDLK_TAB 9
#define SDLK_SPACE 32
#define SDLK_UP 1073741906
#define SDLK_DOWN 1073741905
#define SDLK_LEFT 1073741904
#define SDLK_RIGHT 1073741903

/* Navigation keys */
#define SDLK_INSERT 1073741897
#define SDLK_DELETE 127
#define SDLK_HOME 1073741898
#define SDLK_END 1073741899
#define SDLK_PAGEUP 1073741902
#define SDLK_PAGEDOWN 1073741903

/* Lock keys */
#define SDLK_CAPSLOCK 1073741881
#define SDLK_NUMLOCKCLEAR 1073741907
#define SDLK_SCROLLLOCK 1073741895

/* Other keys */
#define SDLK_PAUSE 1073741896
#define SDLK_PRINTSCREEN 1073741894
#define SDLK_APPLICATION 1073741925 /* Often referred to as the 'menu' */
#define SDLK_EXECUTE 1073741940
#define SDLK_HELP 1073741941
#define SDLK_MENU 1073741942
#define SDLK_SELECT 1073741943
#define SDLK_STOP 1073741944
#define SDLK_AGAIN 1073741945
#define SDLK_UNDO 1073741946
#define SDLK_CUT 1073741947
#define SDLK_COPY 1073741948
#define SDLK_PASTE 1073741949
#define SDLK_FIND 1073741950
#define SDLK_MUTE 1073741951

/* Media keys */
#define SDLK_AUDIOMUTE 1073742080
#define SDLK_AUDIONEXT 1073742082
#define SDLK_AUDIOPLAY 1073742083
#define SDLK_AUDIOPREV 1073742081
#define SDLK_AUDIOSTOP 1073742084
#define SDLK_VOLUMEDOWN 1073742085
#define SDLK_VOLUMEUP 1073742086

/* Keypad specific keys */
#define SDLK_KP_PERCENT 1073741954
#define SDLK_KP_LESS 1073741955
#define SDLK_KP_GREATER 1073741956
#define SDLK_KP_AMPERSAND 1073741957
#define SDLK_KP_DBLAMPERSAND 1073741958
#define SDLK_KP_VERTICALBAR 1073741959
#define SDLK_KP_DBLVERTICALBAR 1073741960
#define SDLK_KP_COLON 1073741961
#define SDLK_KP_HASH 1073741962
#define SDLK_KP_SPACE 1073741963
#define SDLK_KP_AT 1073741964
#define SDLK_KP_EXCLAM 1073741965
#define SDLK_KP_MEMSTORE 1073741966
#define SDLK_KP_MEMRECALL 1073741967
#define SDLK_KP_MEMCLEAR 1073741968
#define SDLK_KP_MEMADD 1073741969
#define SDLK_KP_MEMSUBTRACT 1073741970
#define SDLK_KP_MEMMULTIPLY 1073741971
#define SDLK_KP_MEMDIVIDE 1073741972
#define SDLK_KP_PLUSMINUS 1073741973
#define SDLK_KP_CLEAR 1073741974
#define SDLK_KP_CLEARENTRY 1073741975
#define SDLK_KP_BINARY 1073741976
#define SDLK_KP_OCTAL 1073741977
#define SDLK_KP_DECIMAL 1073741978
#define SDLK_KP_HEXADECIMAL 1073741979

/* international keys */
#define SDLK_WORLD_1 161 /* non-US 1 */
#define SDLK_WORLD_2 162 /* non-US 2 */
#define SDLK_THOUSANDSSEPARATOR 1073741952
#define SDLK_DECIMALSEPARATOR 1073741953
#define SDLK_CURRENCYUNIT 1073741954
#define SDLK_CURRENCYSUBUNIT 1073741955

/* Even more miscelaneous keys */
#define SDLK_SYSREQ 1073741980
#define SDLK_CANCEL 1073741979
#define SDLK_CLEAR 1073741981
#define SDLK_PRIOR 1073741982
#define SDLK_RETURN2 1073741983
#define SDLK_SEPARATOR 1073741984
#define SDLK_OUT 1073741985
#define SDLK_OPER 1073741986
#define SDLK_CLEARAGAIN 1073741987
#define SDLK_CRSEL 1073741988
#define SDLK_EXSEL 1073741989

/* Modifiers as actual modifiers, because if you press shift + a for example then you get SDLK_A plus KMOD_SHIFT */
#define KMOD_LSHIFT 0x0001
#define KMOD_RSHIFT 0x0002
#define KMOD_LCTRL 0x0040
#define KMOD_RCTRL 0x0080
#define KMOD_LALT 0x0100
#define KMOD_RALT 0x0200
#define KMOD_LGUI 0x0400
#define KMOD_RGUI 0x0800
#define KMOD_NUM 0x1000
#define KMOD_CAPS 0x2000
#define KMOD_MODE 0x4000
#define KMOD_SCROLL 0x8000

#define KMOD_SHIFT KMOD_LSHIFT | KMOD_RSHIFT
#define KMOD_CTRL KMOD_LCTRL | KMOD_RCTRL
#define KMOD_ALT KMOD_LALT | KMOD_RALT
#define KMOD_GUI KMOD_LGUI | KMOD_RGUI

uint32_t MODIFIERS[12] = {KMOD_LSHIFT, KMOD_RSHIFT, KMOD_LCTRL, KMOD_RCTRL, KMOD_LALT, KMOD_RALT, KMOD_LGUI, KMOD_RGUI, KMOD_NUM, KMOD_CAPS, KMOD_MODE, KMOD_SCROLL};

#ifdef _WIN32

#include <winuser.h>

uint32_t MOD_KEYS[12] = {VK_LSHIFT, VK_RSHIFT, VK_LCONTROL, VK_RCONTROL, VK_LMENU, VK_RMENU, VK_LWIN, VK_RWIN, VK_NUMLOCK, VK_CAPITAL, VK_MODECHANGE, VK_SCROLL};

uint32_t convertSDL2KeyToWIN32(uint32_t keycode)
{
    switch (keycode)
    {
        case SDLK_A: return 'A';
        case SDLK_B: return 'B';
        case SDLK_C: return 'C';
        case SDLK_D: return 'D';
        case SDLK_E: return 'E';
        case SDLK_F: return 'F';
        case SDLK_G: return 'G';
        case SDLK_H: return 'H';
        case SDLK_I: return 'I';
        case SDLK_J: return 'J';
        case SDLK_K: return 'K';
        case SDLK_L: return 'L';
        case SDLK_M: return 'M';
        case SDLK_N: return 'N';
        case SDLK_O: return 'O';
        case SDLK_P: return 'P';
        case SDLK_Q: return 'Q';
        case SDLK_R: return 'R';
        case SDLK_S: return 'S';
        case SDLK_T: return 'T';
        case SDLK_U: return 'U';
        case SDLK_V: return 'V';
        case SDLK_W: return 'W';
        case SDLK_X: return 'X';
        case SDLK_Y: return 'Y';
        case SDLK_Z: return 'Z';
        case SDLK_0: return '0';
        case SDLK_1: return '1';
        case SDLK_1+1: return '2';
        case SDLK_1+2: return '3';
        case SDLK_1+3: return '4';
        case SDLK_1+4: return '5';
        case SDLK_1+5: return '6';
        case SDLK_1+6: return '7';
        case SDLK_1+7: return '8';
        case SDLK_1+8: return '9';
        case SDLK_KP_0: return VK_NUMPAD0;
        case SDLK_KP_1: return VK_NUMPAD1;
        case SDLK_KP_2: return VK_NUMPAD2;
        case SDLK_KP_3: return VK_NUMPAD3;
        case SDLK_KP_4: return VK_NUMPAD4;
        case SDLK_KP_5: return VK_NUMPAD5;
        case SDLK_KP_6: return VK_NUMPAD6;
        case SDLK_KP_7: return VK_NUMPAD7;
        case SDLK_KP_8: return VK_NUMPAD8;
        case SDLK_KP_9: return VK_NUMPAD9;
        case SDLK_KP_DIVIDE: return VK_DIVIDE;
        case SDLK_KP_MULTIPLY: return VK_MULTIPLY;
        case SDLK_KP_MINUS: return VK_SUBTRACT;
        case SDLK_KP_PLUS: return VK_ADD;
        case SDLK_KP_ENTER: return VK_RETURN;
        case SDLK_KP_PERIOD: return VK_DECIMAL;
        case SDLK_KP_EQUALS: return VK_OEM_NEC_EQUAL;
        case SDLK_KP_COMMA: return VK_SEPARATOR;
        case SDLK_KP_EQUALSAS400: return VK_OEM_NEC_EQUAL;
        case SDLK_F1: return VK_F1;
        case SDLK_F1+1: return VK_F2;
        case SDLK_F1+2: return VK_F3;
        case SDLK_F1+3: return VK_F4;
        case SDLK_F1+4: return VK_F5;
        case SDLK_F1+5: return VK_F6;
        case SDLK_F1+6: return VK_F7;
        case SDLK_F1+7: return VK_F8;
        case SDLK_F1+8: return VK_F9;
        case SDLK_F1+9: return VK_F10;
        case SDLK_F1+10: return VK_F11;
        case SDLK_F1+11: return VK_F12;
        case SDLK_F13: return VK_F13;
        case SDLK_F13+1: return VK_F14;
        case SDLK_F13+2: return VK_F15;
        case SDLK_F13+3: return VK_F16;
        case SDLK_F13+4: return VK_F17;
        case SDLK_F13+5: return VK_F18;
        case SDLK_F13+6: return VK_F19;
        case SDLK_F13+7: return VK_F20;
        case SDLK_F13+8: return VK_F21;
        case SDLK_F13+9: return VK_F22;
        case SDLK_F13+10: return VK_F23;
        case SDLK_F13+11: return VK_F24;
        case SDLK_PERIOD: return VK_OEM_PERIOD;
        case SDLK_COMMA: return VK_OEM_COMMA;
        /* Such intuitive names VK_OEM_7 really feels like it should be the quote button, yeah sure */
        case SDLK_SLASH: return VK_OEM_2;
        case SDLK_BACKSLASH: return VK_OEM_5;
        case SDLK_SEMICOLON: return VK_OEM_1;
        case SDLK_QUOTE: return VK_OEM_7;
        case SDLK_LEFTBRACKET: return VK_OEM_4;
        case SDLK_RIGHTBRACKET: return VK_OEM_6;
        case SDLK_MINUS: return VK_OEM_MINUS;
        case SDLK_EQUALS: return VK_OEM_PLUS;
        case SDLK_LSHIFT: return VK_LSHIFT;
        case SDLK_RSHIFT: return VK_RSHIFT;
        case SDLK_LCTRL: return VK_LCONTROL;
        case SDLK_RCTRL: return VK_RCONTROL;
        case SDLK_LALT: return VK_LMENU;
        case SDLK_RALT: return VK_RMENU;
        case SDLK_LGUI: return VK_LWIN;
        case SDLK_RGUI: return VK_RWIN;
        /* case SDLK_MODE: return VK_MODECHANGE; */
        case SDLK_UP: return VK_UP;
        case SDLK_DOWN: return VK_DOWN;
        case SDLK_LEFT: return VK_LEFT;
        case SDLK_RIGHT: return VK_RIGHT;
        case SDLK_RETURN: return VK_RETURN;
        case SDLK_ESCAPE: return VK_ESCAPE;
        case SDLK_BACKSPACE: return VK_BACK;
        case SDLK_TAB: return VK_TAB;
        case SDLK_SPACE: return VK_SPACE;
        case SDLK_INSERT: return VK_INSERT;
        case SDLK_HOME: return VK_HOME;
        case SDLK_PAGEUP: return VK_PRIOR;
        case SDLK_DELETE: return VK_DELETE;
        case SDLK_END: return VK_END;
        case SDLK_PAUSE: return VK_PAUSE;
        case SDLK_PRINTSCREEN: return VK_SNAPSHOT;
        case SDLK_APPLICATION: return VK_APPS;
        case SDLK_HELP: return VK_HELP;
        case SDLK_MENU: return VK_MENU;
        case SDLK_SELECT: return VK_SELECT;
        case SDLK_STOP: return VK_CANCEL;
        case SDLK_MUTE: return VK_VOLUME_MUTE;
        case SDLK_AUDIOMUTE: return VK_VOLUME_MUTE;
        case SDLK_AUDIONEXT: return VK_MEDIA_NEXT_TRACK;
        case SDLK_AUDIOPLAY: return VK_MEDIA_PLAY_PAUSE;
        case SDLK_AUDIOPREV: return VK_MEDIA_PREV_TRACK;
        case SDLK_AUDIOSTOP: return VK_MEDIA_STOP;
        case SDLK_VOLUMEDOWN: return VK_VOLUME_DOWN;
        case SDLK_VOLUMEUP: return VK_VOLUME_UP;
        case SDLK_KP_PERCENT: return VK_OEM_5;
        case SDLK_KP_LESS: return VK_OEM_102;
        case SDLK_KP_GREATER: return VK_OEM_102;
        case SDLK_KP_VERTICALBAR: return VK_OEM_5;
        case SDLK_KP_DBLVERTICALBAR: return VK_OEM_5;
        case SDLK_KP_PLUSMINUS: return VK_OEM_PLUS;
        case SDLK_KP_COLON: return VK_OEM_1;
        case SDLK_KP_HASH: return VK_OEM_3;
        case SDLK_KP_SPACE: return VK_SPACE;
        case SDLK_KP_AT: return VK_OEM_3;
        case SDLK_KP_EXCLAM: return VK_OEM_3;
        case SDLK_KP_MEMSTORE: return VK_OEM_3;
        case SDLK_KP_MEMRECALL: return VK_OEM_3;
        case SDLK_KP_MEMCLEAR: return VK_OEM_3;
        case SDLK_KP_MEMADD: return VK_OEM_3;
        case SDLK_KP_MEMSUBTRACT: return VK_OEM_3;
        case SDLK_KP_MEMMULTIPLY: return VK_OEM_3;
        case SDLK_KP_MEMDIVIDE: return VK_OEM_3;
        case SDLK_KP_CLEAR: return VK_OEM_3;
        case SDLK_KP_CLEARENTRY: return VK_OEM_3;
        case SDLK_KP_BINARY: return VK_OEM_3;
        case SDLK_KP_OCTAL: return VK_OEM_3;
        case SDLK_KP_DECIMAL: return VK_OEM_3;
        case SDLK_KP_HEXADECIMAL: return VK_OEM_3;
        case SDLK_WORLD_1: return VK_OEM_3;
        case SDLK_WORLD_2: return VK_OEM_3;
        case SDLK_THOUSANDSSEPARATOR: return VK_OEM_3;
        case SDLK_DECIMALSEPARATOR: return VK_OEM_3;
        default: return 0;
    }
}

uint32_t convertSDL2ModToWIN32(uint32_t mod)
{
    switch(mod)
    {
        case KMOD_LSHIFT: return VK_LSHIFT;
        case KMOD_RSHIFT: return VK_RSHIFT;
        case KMOD_LCTRL: return VK_LCONTROL;
        case KMOD_RCTRL: return VK_RCONTROL;
        case KMOD_LALT: return VK_LMENU;
        case KMOD_RALT: return VK_RMENU;
        case KMOD_LGUI: return VK_LWIN;
        case KMOD_RGUI: return VK_RWIN;
        case KMOD_NUM: return VK_NUMLOCK;
        case KMOD_CAPS: return VK_CAPITAL;
        case KMOD_MODE: return VK_MODECHANGE;
        case KMOD_SCROLL: return VK_SCROLL;
        default: return 0;
    }
}

#else

#include <linux/uinput.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

/* One day I will add Wayland support, maybe */
#include <X11/Xlib.h>

uint32_t MOD_KEYS[12] = {KEY_LEFTSHIFT, KEY_RIGHTSHIFT, KEY_LEFTCTRL, KEY_RIGHTCTRL, KEY_LEFTALT, KEY_RIGHTALT, KEY_LEFTMETA, KEY_RIGHTMETA, KEY_NUMLOCK, KEY_CAPSLOCK, KEY_MODE, KEY_SCROLLLOCK};

/* This only extracts the key */
uint32_t convertSDL2KeyToUInput(uint32_t keycode)
{
    /* Regular keys needs to be changed to switch */
    switch (keycode)
    {
        case SDLK_A : return KEY_A;
        case SDLK_B : return KEY_B;
        case SDLK_C : return KEY_C;
        case SDLK_D : return KEY_D;
        case SDLK_E : return KEY_E;
        case SDLK_F : return KEY_F;
        case SDLK_G : return KEY_G;
        case SDLK_H : return KEY_H;
        case SDLK_I : return KEY_I;
        case SDLK_J : return KEY_J;
        case SDLK_K : return KEY_K;
        case SDLK_L : return KEY_L;
        case SDLK_M : return KEY_M;
        case SDLK_N : return KEY_N;
        case SDLK_O : return KEY_O;
        case SDLK_P : return KEY_P;
        case SDLK_Q : return KEY_Q;
        case SDLK_R : return KEY_R;
        case SDLK_S : return KEY_S;
        case SDLK_T : return KEY_T;
        case SDLK_U : return KEY_U;
        case SDLK_V : return KEY_V;
        case SDLK_W : return KEY_W;
        case SDLK_X : return KEY_X;
        case SDLK_Y : return KEY_Y;
        case SDLK_Z : return KEY_Z;
    }
    if (keycode >= SDLK_A && keycode <= SDLK_Z)
    {
        return keycode - SDLK_A + KEY_A;
    }
    /* Number keys */
    if (keycode >= SDLK_1 && keycode <= SDLK_9)
    {
        return keycode - SDLK_1 + KEY_1;
    }
    if (keycode == SDLK_0)
    {
        return KEY_0;
    }
    /* Numpad keys */
    if (keycode >= SDLK_KP_0 && keycode <= SDLK_KP_9)
    {
        return keycode - SDLK_KP_0 + KEY_KP0;
    }
    switch (keycode)
    {
        case SDLK_KP_DIVIDE      : return KEY_KPSLASH;
        case SDLK_KP_MULTIPLY    : return KEY_KPASTERISK;
        case SDLK_KP_MINUS       : return KEY_KPMINUS;
        case SDLK_KP_PLUS        : return KEY_KPPLUS;
        case SDLK_KP_ENTER       : return KEY_KPENTER;
        case SDLK_KP_PERIOD      : return KEY_KPDOT;
        case SDLK_KP_EQUALS      : return KEY_KPEQUAL;
        case SDLK_KP_COMMA       : return KEY_KPCOMMA;
        case SDLK_KP_EQUALSAS400 : return KEY_KPEQUAL;
    }
    /* Function keys needs to be changed to a switch */
    if (keycode >= SDLK_F1 && keycode <= SDLK_F12)
    {
        switch(keycode)
        {
            case SDLK_F1: return KEY_F1;
            case SDLK_F1+1: return KEY_F2;
            case SDLK_F1+2: return KEY_F3;
            case SDLK_F1+3: return KEY_F4;
            case SDLK_F1+4: return KEY_F5;
            case SDLK_F1+5: return KEY_F6;
            case SDLK_F1+6: return KEY_F7;
            case SDLK_F1+7: return KEY_F8;
            case SDLK_F1+8: return KEY_F9;
            case SDLK_F1+9: return KEY_F10;
            case SDLK_F1+10: return KEY_F11;
            case SDLK_F1+11: return KEY_F12;
        }
    }
    if (keycode >= SDLK_F13 && keycode <= SDLK_F24)
    {
        return keycode - SDLK_F13 + KEY_F13;
    }
    /* All other special keys */
    switch (keycode)
    {
        /* Symbols */
        case SDLK_PERIOD: return KEY_DOT;
        case SDLK_COMMA: return KEY_COMMA;
        case SDLK_SLASH: return KEY_SLASH;
        case SDLK_BACKSLASH: return KEY_BACKSLASH;
        case SDLK_SEMICOLON: return KEY_SEMICOLON;
        case SDLK_QUOTE: return KEY_APOSTROPHE;
        case SDLK_LEFTBRACKET: return KEY_LEFTBRACE;
        case SDLK_RIGHTBRACKET: return KEY_RIGHTBRACE;
        case SDLK_MINUS: return KEY_MINUS;
        case SDLK_EQUALS: return KEY_EQUAL;
        /* "Usual" special keys */
        case SDLK_RETURN: return KEY_ENTER;
        case SDLK_ESCAPE: return KEY_ESC;
        case SDLK_BACKSPACE: return KEY_BACKSPACE;
        case SDLK_TAB: return KEY_TAB;
        case SDLK_SPACE: return KEY_SPACE;
        case SDLK_UP: return KEY_UP;
        case SDLK_DOWN: return KEY_DOWN;
        case SDLK_LEFT: return KEY_LEFT;
        case SDLK_RIGHT: return KEY_RIGHT;
        /* Modifier keys */
        case SDLK_LSHIFT: return KEY_LEFTSHIFT;
        case SDLK_RSHIFT: return KEY_RIGHTSHIFT;
        case SDLK_LCTRL: return KEY_LEFTCTRL;
        case SDLK_RCTRL: return KEY_RIGHTCTRL;
        case SDLK_LALT: return KEY_LEFTALT;
        case SDLK_RALT: return KEY_RIGHTALT;
        case SDLK_LGUI: return KEY_LEFTMETA;
        case SDLK_RGUI: return KEY_RIGHTMETA;
        /* Lock keys */
        case SDLK_CAPSLOCK: return KEY_CAPSLOCK;
        case SDLK_NUMLOCKCLEAR: return KEY_NUMLOCK;
        case SDLK_SCROLLLOCK: return KEY_SCROLLLOCK;
        /* Navigation keys */
        case SDLK_INSERT: return KEY_INSERT;
        case SDLK_HOME: return KEY_HOME;
        case SDLK_PAGEUP: return KEY_PAGEUP;
        case SDLK_DELETE: return KEY_DELETE;
        case SDLK_END: return KEY_END;
        /* case SDLK_PAGEDOWN: return KEY_PAGEDOWN; */
        /* Other keys */
        case SDLK_PAUSE : return KEY_PAUSE;
        case SDLK_PRINTSCREEN : return KEY_SYSRQ;
        case SDLK_APPLICATION : return KEY_COMPOSE;
        /* case SDLK_EXECUTE : return KEY_EXE; */
        case SDLK_HELP : return KEY_HELP;
        case SDLK_MENU : return KEY_MENU;
        case SDLK_SELECT : return KEY_SELECT;
        case SDLK_STOP : return KEY_STOP;
        case SDLK_AGAIN : return KEY_AGAIN;
        case SDLK_UNDO : return KEY_UNDO;
        case SDLK_CUT : return KEY_CUT;
        case SDLK_COPY : return KEY_COPY;
        case SDLK_PASTE : return KEY_PASTE;
        case SDLK_FIND : return KEY_FIND;
        case SDLK_MUTE : return KEY_MUTE;
        /* Media keys */
        case SDLK_AUDIOMUTE : return KEY_MUTE;
        case SDLK_AUDIONEXT : return KEY_NEXTSONG;
        case SDLK_AUDIOPLAY : return KEY_PLAYPAUSE;
        case SDLK_AUDIOPREV : return KEY_PREVIOUSSONG;
        case SDLK_AUDIOSTOP : return KEY_STOPCD;
        case SDLK_VOLUMEDOWN : return KEY_VOLUMEDOWN;
        case SDLK_VOLUMEUP : return KEY_VOLUMEUP;
        /* Keypad specific keys */
        /* case SDLK_KP_PERCENT : return KEY_KPPERIOD; */
        case SDLK_KP_LESS : return KEY_KPLEFTPAREN;
        case SDLK_KP_GREATER : return  KEY_KPRIGHTPAREN;
        case SDLK_KP_AMPERSAND : return KEY_KPLEFTPAREN;
        case SDLK_KP_DBLAMPERSAND : return KEY_KPRIGHTPAREN;
        case SDLK_KP_VERTICALBAR : return  KEY_KPLEFTPAREN;
        case SDLK_KP_DBLVERTICALBAR : return KEY_KPRIGHTPAREN;
        case SDLK_KP_PLUSMINUS : return KEY_KPPLUSMINUS;
        /*
        case SDLK_KP_COLON : return KEY_KPCOLON;
        case SDLK_KP_HASH : return KEY_KPHASH;
        case SDLK_KP_SPACE : return KEY_KPSPACE;
        case SDLK_KP_AT : return KEY_KPMEMADD;
        case SDLK_KP_EXCLAM : return KEY_KPMEMSUBTRACT;
        case SDLK_KP_MEMSTORE : return KEY_KPMEMSTORE;
        case SDLK_KP_MEMRECALL : return KEY_KPMEMRECALL;
        case SDLK_KP_MEMCLEAR : return KEY_KPMEMCLEAR;
        case SDLK_KP_MEMADD : return KEY_KPMEMADD;
        case SDLK_KP_MEMSUBTRACT : return KEY_KPMEMSUBTRACT;
        case SDLK_KP_MEMMULTIPLY : return KEY_KPMEMMULTIPLY;
        case SDLK_KP_MEMDIVIDE : return KEY_KPMEMDIVIDE;
        case SDLK_KP_CLEAR : return KEY_KPCLEAR;
        case SDLK_KP_CLEARENTRY : return KEY_KPCLEARENTRY;
        case SDLK_KP_BINARY : return KEY_KPBINARY;
        case SDLK_KP_OCTAL : return KEY_KPOCTAL;
        case SDLK_KP_DECIMAL : return KEY_KPDECIMAL;
        case SDLK_KP_HEXADECIMAL : return KEY_KPHEXADECIMAL;
        */
       /* uint32_ternational keys */
        case SDLK_WORLD_1 : return KEY_LEFTCTRL;
        case SDLK_WORLD_2 : return KEY_RIGHTCTRL;
        case SDLK_THOUSANDSSEPARATOR : return KEY_COMPOSE;
        case SDLK_DECIMALSEPARATOR : return KEY_COMPOSE;
        case SDLK_CURRENCYUNIT : return KEY_COMPOSE;
        default : return 0;
    }
}

/* This extracts the modifier too */
uint32_t convertSDL2ModToUInput(uint32_t mod)
{
    switch(mod)
    {
        case KMOD_LSHIFT : return KEY_LEFTSHIFT;
        case KMOD_RSHIFT : return KEY_RIGHTSHIFT;
        case KMOD_LCTRL  : return KEY_LEFTCTRL;
        case KMOD_RCTRL  : return KEY_RIGHTCTRL;
        case KMOD_LALT   : return KEY_LEFTALT;
        case KMOD_RALT   : return KEY_RIGHTALT;
        case KMOD_LGUI   : return KEY_LEFTMETA;
        case KMOD_RGUI   : return KEY_RIGHTMETA;
        case KMOD_NUM    : return KEY_NUMLOCK;
        case KMOD_CAPS   : return KEY_CAPSLOCK;
        case KMOD_MODE   : return KEY_MODE;
        case KMOD_SCROLL   : return KEY_SCROLLLOCK;
        default: return 0;
    }
}



#endif

#define SDL_INPUT 1
#define MAX_KEY 256

typedef struct inputs
{
    int screen_w, screen_h;
    uint32_t fd_keybd, fd_mouse;
#ifdef _WIN32
    /* This is to press and release a key*/
    INPUT inputs[2];
#endif

#ifndef _WIN32
    struct uinput_setup usetup;
    struct input_event ev_key, ev_mod;
    Display* display;
    Screen* screen;
    Window root_window;
#endif
    uint32_t last_input_result;
}inputs;

#ifndef _WIN32
void send_syn(inputs* inp)
{
    struct input_event ev_syn = { .type = EV_SYN, .code = SYN_REPORT, .value = 0 };
    if (write(inp->fd_keybd, &ev_syn, sizeof(ev_syn)) == -1) {
        /* perror("Error sending EV_SYN event"); */
    }
}

/* This needs to be done manually ig */
void register_regular_keys(inputs* inp)
{
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_A);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_B);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_C);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_D);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_E);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_F);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_G);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_H);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_I);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_J);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_K);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_L);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_M);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_N);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_O);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_P);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_Q);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_R);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_S);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_T);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_U);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_V);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_W);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_X);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_Y);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_Z);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_0);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_1);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_2);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_3);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_4);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_5);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_6);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_7);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_8);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_9);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_LEFTCTRL);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_RIGHTCTRL);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_LEFTSHIFT);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_RIGHTSHIFT);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, SDLK_LCTRL);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, SDLK_RCTRL);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, SDLK_LALT);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, SDLK_RALT);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, SDLK_LGUI);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, SDLK_RGUI);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, SDLK_MODE);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_ENTER);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_ESC);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_BACKSPACE);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_TAB);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_SPACE);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_UP);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_DOWN);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_LEFT);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_RIGHT);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_INSERT);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_HOME);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_PAGEUP);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_DELETE);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_END);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_PAGEDOWN);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_DOT);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_COMMA);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_SLASH);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_BACKSLASH);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_SEMICOLON);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_APOSTROPHE);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_LEFTBRACE);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_RIGHTBRACE);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_MINUS);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_EQUAL);
}

/* The name is misleading, this also registers some more keys that are usually next to the numpad */
void register_function_keys(inputs* inp)
{
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_F1);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_F2);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_F3);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_F4);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_F5);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_F6);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_F7);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_F8);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_F9);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_F10);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_F11);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_F12);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_INSERT);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_HOME);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_PAGEUP);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_PAGEDOWN);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_DELETE);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_END);
}

void register_numpad_keys(inputs* inp)
{
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KP0);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KP1);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KP2);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KP3);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KP4);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KP5);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KP6);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KP7);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KP8);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KP9);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KPSLASH);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KPASTERISK);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KPMINUS);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KPPLUS);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KPENTER);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KPDOT);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KPEQUAL);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KPCOMMA);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KPEQUAL);
}

void register_other_key(inputs* inp)
{
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_PAUSE);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_SYSRQ);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_COMPOSE);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_HELP);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_MENU);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_SELECT);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_STOP);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_AGAIN);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_UNDO);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_CUT);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_COPY);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_PASTE);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_FIND);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_MUTE);
}

void register_media_keys(inputs* inp)
{
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_PLAYPAUSE);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_MUTE);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_NEXTSONG);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_PREVIOUS);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_STOPCD);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_VOLUMEUP);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_VOLUMEDOWN);
}

void register_intl_keys(inputs* inp)
{
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_LEFTCTRL); /* What is this key for? */
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_RIGHTCTRL); /* What is this key for? */
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_COMPOSE); /* What is this key? */
}

void register_keypad_special_keys(inputs* inp)
{
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KPLEFTPAREN);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KPRIGHTPAREN);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KPLEFTPAREN);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KPRIGHTPAREN);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KPLEFTPAREN);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KPRIGHTPAREN);
    ioctl(inp->fd_keybd, UI_SET_KEYBIT, KEY_KPPLUSMINUS);
}

void register_mouse_keys(inputs* inp)
{
    ioctl(inp->fd_mouse, UI_SET_EVBIT, EV_REL);
    ioctl(inp->fd_mouse, UI_SET_RELBIT, REL_X);
    ioctl(inp->fd_mouse, UI_SET_RELBIT, REL_Y);
    ioctl(inp->fd_mouse, UI_SET_RELBIT, REL_WHEEL);
    ioctl(inp->fd_mouse, UI_SET_RELBIT, REL_HWHEEL);
    ioctl(inp->fd_mouse, UI_SET_EVBIT, EV_KEY);
    ioctl(inp->fd_mouse, UI_SET_KEYBIT, BTN_LEFT);
    ioctl(inp->fd_mouse, UI_SET_KEYBIT, BTN_RIGHT);
    ioctl(inp->fd_mouse, UI_SET_KEYBIT, BTN_MIDDLE);
    ioctl(inp->fd_mouse, UI_SET_KEYBIT, BTN_SIDE);
    ioctl(inp->fd_mouse, UI_SET_KEYBIT, BTN_EXTRA);
}

#endif


#define REGULAR_KEYS 1    /* Regular keys include letters, modifiers, locks, and numbers basically a 60% layout with the arrows and some modifiers */
#define FUNCTION_KEYS 2   /* Self explainatory*/
#define NUMPAD_KEYS 4     /* Self explainatory*/
#define OTHER_KEYS 8      /* This includes weird buttons like printscreen, help, menu, copy, paste and some more stuff*/
#define MEDIA_KEYS 16     /* Keys for media playback, and volume changes*/
#define INTL_KEYS 32      /* I don't really know what these are, but they exist */
#define KEYPAD_SPECIAL 64 /* Some more odd keypad keys, I don't know if they are used at all on any modern keyboards*/
#define EXTENDED_KEYS 128 /* Even more odd keys, I don't think anyone really needs this */
#define MOUSE 256         /* Mouse support*/
#define SIXTY_PERCENT_KEY REGULAR_KEYS
#define TENKEYLESS (REGULAR_KEYS | FUNCTION_KEYS)
#define FULL_LAYOUT (REGULAR_KEYS | FUNCTION_KEYS | NUMPAD_KEYS | MEDIA_KEYS)
#define EXTENDED_LAYOUT (REGULAR_KEYS | FUNCTION_KEYS | NUMPAD_KEYS | MEDIA_KEYS | uint32_tL_KEYS | KEYPAD_SPECIAL | EXTENDED_KEYS)

/*
 * @brief Creates a struct that can be used to send inputs
 *
 * @param flags The flags that specify what keys to register (see REGULAR_KEYS, FUNCTION_KEYS, NUMPAD_KEYS, OTHER_KEYS, MEDIA_KEYS, uint32_tL_KEYS, KEYPAD_SPECIAL, EXTENDED_KEYS, MOUSE)
*/
inputs* create_inputs_struct(uint32_t flags)
{
    if (flags == 0)
    {
        flags = FULL_LAYOUT | MOUSE;
    }
    inputs* inp;
    inp = (inputs*)malloc(sizeof(inputs));
#ifdef _WIN32
    inp->screen_h=GetSystemMetrics(SM_CYSCREEN);
    inp->screen_w=GetSystemMetrics(SM_CXSCREEN);
#endif
#ifndef _WIN32
    inp->fd_keybd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (inp->fd_keybd < 0)
        return NULL;
    memset(&inp->usetup, 0, sizeof(inp->usetup));
    strncpy(inp->usetup.name, "virtual-Input-Device", UINPUT_MAX_NAME_SIZE);
    inp->usetup.id.bustype = BUS_USB;
    inp->usetup.id.vendor = 0x0001;
    inp->usetup.id.product = 0x0001;

    ioctl(inp->fd_keybd, UI_SET_EVBIT, EV_KEY);

    usleep(100000);
    if (flags & REGULAR_KEYS)
    {
        register_regular_keys(inp);
    }
    if (flags & FUNCTION_KEYS)
    {
        register_function_keys(inp);
    }
    if (flags & NUMPAD_KEYS)
    {
        register_numpad_keys(inp);
    }
    if (flags & OTHER_KEYS)
    {
        register_other_key(inp);
    }
    if (flags & MEDIA_KEYS)
    {
        register_media_keys(inp);
    }
    if (flags & INTL_KEYS)
    {
        register_intl_keys(inp);
    }
    if (flags & KEYPAD_SPECIAL)
    {
        register_keypad_special_keys(inp);
    }
    if (flags & MOUSE)
    {
        register_mouse_keys(inp);
    }

    ioctl(inp->fd_keybd, UI_DEV_SETUP, &inp->usetup);
    ioctl(inp->fd_keybd, UI_DEV_CREATE);

    inp -> display = XOpenDisplay(NULL);
    inp -> screen = DefaultScreenOfDisplay(inp -> display);
    /* Kind of redundant but it keeps everything consistent */
    inp -> screen_w = inp -> screen -> width;
    inp -> screen_h = inp -> screen -> height;
    inp -> root_window = XRootWindow(inp -> display, XDefaultScreen(inp -> display));
#endif
    /* By default */
    inp->last_input_result=0;
    return inp;
}

/*  Sends a key press and release event
    For specific key presses and releases use send_key_press and send_key_release
    Please see MODIFIERS for valid values of mods
*/
void send_key(inputs* inp, uint32_t key, uint32_t mods, uint32_t flags)
{
    uint32_t i;
#ifdef _WIN32
    if (flags == SDL_INPUT)
    {
        key = convertSDL2KeyToWIN32(key);
    }
    /* The actual key takes a place */
    uint32_t cInputs = 1;
    for (i = 0; i < 12; i+=1)
    {
        if (mods & MODIFIERS[i])
        {
            cInputs += 1;
        }
    }
    cInputs *= 2;
    INPUT inputs[cInputs];
    ZeroMemory(inputs, sizeof(inputs));
    uint32_t cv = 0;
    for (i = 0; i < 12; i+=1)
    {
        if (mods & MODIFIERS[i])
        {
            inputs[cv].type = INPUT_KEYBOARD;
            inputs[cv].ki.wVk = MOD_KEYS[i];
            cv += 1;
        }
    }
    inputs[cv].type = INPUT_KEYBOARD;
    inputs[cv].ki.wVk = key;
    cv += 1;
    for (i = 0; i < 12; i+=1)
    {
        if (mods & MODIFIERS[i])
        {
            inputs[cv].type = INPUT_KEYBOARD;
            inputs[cv].ki.wVk = MOD_KEYS[i];
            inputs[cv].ki.dwFlags = KEYEVENTF_KEYUP;
            cv += 1;
        }
    }
    inputs[cv].type = INPUT_KEYBOARD;
    inputs[cv].ki.wVk = key;
    inputs[cv].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(cInputs/2, inputs, sizeof(INPUT));
    usleep(100);
    SendInput(cInputs/2, inputs + cInputs/2, sizeof(INPUT));
#endif
#ifndef _WIN32

    if (flags == SDL_INPUT)
    {
        key = convertSDL2KeyToUInput(key);
    }
    memset(&inp->ev_key, 0, sizeof(inp->ev_key));
    memset(&inp->ev_mod, 0, sizeof(inp->ev_mod));
    if (mods != 0)
    {
        for (i = 0; i < 12; i+=1)
        {
            if (mods & MODIFIERS[i])
            {
                inp->ev_mod.type = EV_KEY;
                inp->ev_mod.code = MOD_KEYS[i];
                inp->ev_mod.value = 1;
                (void)write(inp->fd_keybd, &inp->ev_mod, sizeof(inp->ev_mod));
                send_syn(inp);
                usleep(100);
            }
        }
    }
    inp->ev_key.type = EV_KEY;
    inp->ev_key.code = key;
    inp->ev_key.value = 1;
    usleep(1000);
    (void)write(inp->fd_keybd, &inp->ev_key, sizeof(inp->ev_key));
    send_syn(inp);
    if (mods != 0)
    {
        for (i = 0; i < 12; i+=1)
        {
            if (mods & MODIFIERS[i])
            {
                inp->ev_mod.type = EV_KEY;
                inp->ev_mod.code = MOD_KEYS[i];
                inp->ev_mod.value = 0;
                (void)write(inp->fd_keybd, &inp->ev_mod, sizeof(inp->ev_mod));
                send_syn(inp);
                usleep(100);
            }
        }
    }
    inp->ev_key.value = 0;
    (void)write(inp->fd_keybd, &inp->ev_key, sizeof(inp->ev_key));

    send_syn(inp);
#endif
}

/* Sends a key press event */
void send_key_press(inputs* inp, uint32_t key, uint32_t mods, uint32_t flags)
{
#ifdef _WIN32
    if (flags == SDL_INPUT)
    {
        key = convertSDL2KeyToWIN32(key);
    }
    uint32_t cInputs;
    uint32_t i;
    /* The actual key */
    cInputs = 1;
    for (i = 0; i < 12; i+=1)
    {
        if (mods & MODIFIERS[i])
        {
            cInputs += 1;
        }
    }
    INPUT inputs[cInputs];
    ZeroMemory(inputs, sizeof(inputs));
    for (i = 0; i < 12; i+=1)
    {
        if (mods & MODIFIERS[i])
        {
            inputs[i].type = INPUT_KEYBOARD;
            inputs[i].ki.wVk = MOD_KEYS[i];
            inputs[i].ki.dwFlags = 0;
        }
    }
    inputs[cInputs - 1].type = INPUT_KEYBOARD;
    inputs[cInputs - 1].ki.wVk = key;
    inputs[cInputs - 1].ki.dwFlags = 0;
    SendInput(cInputs, inputs, sizeof(INPUT));
#endif
#ifndef _WIN32
    if (flags == SDL_INPUT)
    {
        key = convertSDL2KeyToUInput(key);
    }
    memset(&inp->ev_key, 0, sizeof(inp->ev_key));
    memset(&inp->ev_mod, 0, sizeof(inp->ev_mod));
    uint32_t i;
    for (i = 0; i < 12; i+=1)
    {
        if (mods & MODIFIERS[i])
        {
            inp->ev_mod.type = EV_KEY;
            inp->ev_mod.code = MOD_KEYS[i];
            inp->ev_mod.value = 1;
            (void)write(inp->fd_keybd, &inp->ev_mod, sizeof(inp->ev_mod));
            send_syn(inp);
        }
    }
    inp->ev_key.type = EV_KEY;
    inp->ev_key.code = key;
    inp->ev_key.value = 1;
    usleep(1000);
    (void)write(inp->fd_keybd, &inp->ev_key, sizeof(inp->ev_key));
    send_syn(inp);
#endif
}

void send_key_release(inputs* inp, uint32_t key, uint32_t mods, uint32_t flags)
{
#ifdef _WIN32
    if (flags == SDL_INPUT)
    {
        key = convertSDL2KeyToWIN32(key);
    }
    uint32_t cInputs;
    uint32_t i;
    /* The actual key */
    cInputs = 1;
    for (i = 0; i < 12; i+=1)
    {
        if (mods & MODIFIERS[i])
        {
            cInputs += 1;
        }
    }
    INPUT inputs[cInputs];
    ZeroMemory(inputs, sizeof(inputs));
    for (i = 0; i < 12; i+=1)
    {
        if (mods & MODIFIERS[i])
        {
            inputs[i].type = INPUT_KEYBOARD;
            inputs[i].ki.wVk = MOD_KEYS[i];
            inputs[i].ki.dwFlags = KEYEVENTF_KEYUP;
        }
    }
    inputs[cInputs - 1].type = INPUT_KEYBOARD;
    inputs[cInputs - 1].ki.wVk = key;
    inputs[cInputs - 1].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(cInputs, inputs, sizeof(INPUT));
#endif
#ifndef _WIN32
    if (flags == SDL_INPUT)
    {
        key = convertSDL2KeyToUInput(key);
    }
    memset(&inp->ev_key, 0, sizeof(inp->ev_key));
    memset(&inp->ev_mod, 0, sizeof(inp->ev_mod));
    uint32_t i;
    for (i = 0; i < 12; i+=1)
    {
        if (mods & MODIFIERS[i])
        {
            inp->ev_mod.type = EV_KEY;
            inp->ev_mod.code = MOD_KEYS[i];
            inp->ev_mod.value = 1;
            (void)write(inp->fd_keybd, &inp->ev_mod, sizeof(inp->ev_mod));
            send_syn(inp);
        }
    }
    inp->ev_key.type = EV_KEY;
    inp->ev_key.code = key;
    inp->ev_key.value = 0; /*silly billi it was 1 before and NOTHING worked*/
    usleep(1000);
    (void)write(inp->fd_keybd, &inp->ev_key, sizeof(inp->ev_key));
    send_syn(inp);
#endif
}

/**
 * @brief (void)writes the cursor position to the x and y variables
 *
 * @return 0 on failure, nonzero otherwise
*/
int get_mouse_position(inputs* inp, int *x, int *y)
{
#ifdef _WIN32
    /* Goofy aah windows */
    POINT point;
    int ret = GetCursorInfo(&point);
    *x = point.x;
    *y = point.y;
    return ret;
#else
    Window root_return, child_return;
    int win_x, win_y;
    unsigned int mask_return;

    int ret = XQueryPointer(inp -> display, inp -> root_window, &root_return, &child_return,
                  x, y, &win_x, &win_y, &mask_return);
    return ret;
#endif
}

/**
 * @brief Sets the mouse position to x and y as pixel coordinates, not absolute
 *
 * @param inp pointer to an instance of the inputs struct
 * @param x the x coordinate
 * @param y the y coordinate
*/
void set_mouse_pos(inputs* inp, int x, int y)
{
#ifdef _WIN32
    INPUT _inp;
    ZeroMemory(&_inp, sizeof(INPUT));
    _inp.type = INPUT_MOUSE;
    _inp.mi.dx = x * (float)((float)65536 / (float)inp->screen_w);
    _inp.mi.dy = y * (float)((float)65536 / (float)inp->screen_h);
    _inp.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
    SendInput(1, &_inp, sizeof(INPUT));
    usleep(500);
#else

    XWarpPointer(inp -> display, (Window)NULL, inp -> root_window, 0, 0, 0, 0, x, y);
    XFlush(inp -> display);
    usleep(500);
#endif
}

void send_Lclick(inputs* inp)
{
#ifdef _WIN32
    INPUT click[2];
    ZeroMemory(click, sizeof(click));
    click[0].type = INPUT_MOUSE;
    click[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    click[1].type = INPUT_MOUSE;
    click[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(2, click, sizeof(INPUT));
    usleep(200);
    return;
#else
    memset(&inp->ev_key, 0, sizeof(inp->ev_key));
    inp->ev_key.type = EV_KEY;
    inp->ev_key.code = BTN_LEFT;
    inp->ev_key.value = 1;
    (void)write(inp->fd_mouse, &inp->ev_key, sizeof(inp->ev_key));
    usleep(200);
    inp->ev_key.value = 0;
    (void)write(inp->fd_mouse, &inp->ev_key, sizeof(inp->ev_key));
    usleep(200);
    return;
#endif
}

void send_Lhold(inputs* inp)
{
#ifdef _WIN32
    INPUT click[1];
    ZeroMemory(click, sizeof(click));
    click[0].type = INPUT_MOUSE;
    click[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, click, sizeof(INPUT));
    usleep(200);
    return;
#else
    memset(&inp->ev_key, 0, sizeof(inp->ev_key));
    inp->ev_key.type = EV_KEY;
    inp->ev_key.code = BTN_LEFT;
    inp->ev_key.value = 1;
    (void)write(inp->fd_mouse, &inp->ev_key, sizeof(inp->ev_key));
    usleep(200);
    return;
#endif
}

void send_Lrelease(inputs* inp)
{   
#ifdef _WIN32
    INPUT click[1];
    ZeroMemory(click, sizeof(click));
    click[0].type = INPUT_MOUSE;
    click[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, click, sizeof(INPUT));
    usleep(200);
    return;
#else
    memset(&inp->ev_key, 0, sizeof(inp->ev_key));
    inp->ev_key.type = EV_KEY;
    inp->ev_key.code = BTN_LEFT;
    inp->ev_key.value = 0;
    (void)write(inp->fd_mouse, &inp->ev_key, sizeof(inp->ev_key));
    usleep(200);
    return;
#endif

}

void send_Rclick(inputs* inp)
{
#ifdef _WIN32
    INPUT click[2];
    ZeroMemory(click, sizeof(click));
    click[0].type = INPUT_MOUSE;
    click[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    click[1].type = INPUT_MOUSE;
    click[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    SendInput(1, click, sizeof(INPUT));
    usleep(100);
    SendInput(1, click + 1, sizeof(INPUT));
    usleep(100);
    return;
#else
    memset(&inp->ev_key, 0, sizeof(inp->ev_key));
    inp->ev_key.type = EV_KEY;
    inp->ev_key.code = BTN_RIGHT;
    inp->ev_key.value = 1;
    (void)write(inp->fd_mouse, &inp->ev_key, sizeof(inp->ev_key));
    send_syn(inp);
    usleep(100);
    inp->ev_key.value = 0;
    (void)write(inp->fd_mouse, &inp->ev_key, sizeof(inp->ev_key));
    send_syn(inp);
    usleep(100);
    return;
#endif
}

void send_Mclick(inputs* inp)
{
#ifdef _WIN32
    INPUT click[2];
    ZeroMemory(click, sizeof(click));
    click[0].type = INPUT_MOUSE;
    click[0].mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
    click[1].type = INPUT_MOUSE;
    click[1].mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
    SendInput(1, click, sizeof(INPUT));
    usleep(100);
    SendInput(1, click + 1, sizeof(INPUT));
    usleep(100);
    return;
#else
    memset(&inp->ev_key, 0, sizeof(inp->ev_key));
    inp->ev_key.type = EV_KEY;
    inp->ev_key.code = BTN_MIDDLE;
    inp->ev_key.value = 1;
    (void)write(inp->fd_mouse, &inp->ev_key, sizeof(inp->ev_key));
    send_syn(inp);
    usleep(100);
    inp->ev_key.value = 0;
    (void)write(inp->fd_mouse, &inp->ev_key, sizeof(inp->ev_key));
    send_syn(inp);
    usleep(100);
    return;
#endif
}

void send_mouse_scroll(inputs* inp, int amount)
{
#ifdef _WIN32
    INPUT scroll;
    ZeroMemory(&scroll, sizeof(INPUT));
    scroll.type = INPUT_MOUSE;
    scroll.mi.dwFlags = MOUSEEVENTF_WHEEL;
    scroll.mi.mouseData = amount;
    SendInput(1, &scroll, sizeof(INPUT));
    usleep(100);
    return;
#else
    memset(&inp->ev_key, 0, sizeof(inp->ev_key));
    inp->ev_key.type = EV_REL;
    inp->ev_key.code = REL_WHEEL;
    inp->ev_key.value = amount;
    (void)write(inp->fd_mouse, &inp->ev_key, sizeof(inp->ev_key));
    send_syn(inp);
    usleep(100);
    inp->ev_key.value = 0;
    (void)write(inp->fd_mouse, &inp->ev_key, sizeof(inp->ev_key));
    send_syn(inp);
    usleep(100);
    return;
#endif
}

/* As the name suggests this is a horizontal scroll, it's a thing that exists on quite some mice */
void send_mouse_scroll_horizontal(inputs* inp, int amount)
{
#ifdef _WIN32
    INPUT scroll;
    ZeroMemory(&scroll, sizeof(INPUT));
    scroll.type = INPUT_MOUSE;
    scroll.mi.dwFlags = MOUSEEVENTF_WHEEL;
    scroll.mi.mouseData = amount;
    SendInput(1, &scroll, sizeof(INPUT));
    usleep(100);
    return;
#else
    memset(&inp->ev_key, 0, sizeof(inp->ev_key));
    inp->ev_key.type = EV_REL;
    inp->ev_key.code = REL_WHEEL;
    inp->ev_key.value = amount;
    (void)write(inp->fd_mouse, &inp->ev_key, sizeof(inp->ev_key));
    send_syn(inp);
    usleep(100);
    inp->ev_key.value = 0;
    (void)write(inp->fd_mouse, &inp->ev_key, sizeof(inp->ev_key));
    send_syn(inp);
    usleep(100);
    return;
#endif
}
