#pragma once

#include <stdlib.h>
#include <stdint.h>

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


// Numeric keys
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


// Modifier keys
#define SDLK_TO_UINPUT_LSHIFT KEY_LEFTSHIFT
#define SDLK_TO_UINPUT_RSHIFT KEY_RIGHTSHIFT
#define SDLK_TO_UINPUT_LCTRL  KEY_LEFTCTRL
#define SDLK_TO_UINPUT_RCTRL  KEY_RIGHTCTRL
#define SDLK_TO_UINPUT_LALT   KEY_LEFTALT
#define SDLK_TO_UINPUT_RALT   KEY_RIGHTALT
#define SDLK_TO_UINPUT_LGUI   KEY_LEFTMETA   // Left "Windows" or "Command" key
#define SDLK_TO_UINPUT_RGUI   KEY_RIGHTMETA  // Right "Windows" or "Command" key

// Arrow keys
#define SDLK_TO_UINPUT_UP     KEY_UP
#define SDLK_TO_UINPUT_DOWN   KEY_DOWN
#define SDLK_TO_UINPUT_LEFT   KEY_LEFT
#define SDLK_TO_UINPUT_RIGHT  KEY_RIGHT

/* SDL keys to integers */
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

/* International keys */
#define SDLK_WORLD_1 161 // non-US #1
#define SDLK_WORLD_2 162 // non-US #2
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

#ifdef _WIN32

#else

#include <linux/uinput.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

/* This only extracts the key */
int32_t convertSDL2KeyToUInput(int32_t keycode)
{
    int32_t key = 0;
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
       /* International keys */
        case SDLK_WORLD_1 : return KEY_LEFTCTRL;
        case SDLK_WORLD_2 : return KEY_RIGHTCTRL;
        case SDLK_THOUSANDSSEPARATOR : return KEY_COMPOSE;
        case SDLK_DECIMALSEPARATOR : return KEY_COMPOSE;
        case SDLK_CURRENCYUNIT : return KEY_COMPOSE;
        default : return 0;
    }
}

/* This extracts the modifier too */
int32_t convertSDL2ModToUInput(int32_t mod)
{
    switch(mod)
    {
        case SDLK_LSHIFT : return KEY_LEFTSHIFT;
        case SDLK_RSHIFT : return KEY_RIGHTSHIFT;
        case SDLK_LCTRL  : return KEY_LEFTCTRL;
        case SDLK_RCTRL  : return KEY_RIGHTCTRL;
        case SDLK_LALT   : return KEY_LEFTALT;
        case SDLK_RALT   : return KEY_RIGHTALT;
        case SDLK_LGUI   : return KEY_LEFTMETA;
        case SDLK_RGUI   : return KEY_RIGHTMETA;
        case SDLK_MODE   : return KEY_MODE;
        default: return 0;
    }
}



#endif

#define SDL_INPUT 1
#define MAX_KEY 256

typedef struct inputs
{
    int fd;
#ifndef _WIN32
    struct uinput_setup usetup;
    struct input_event ev_key, ev_mod;
#endif
    int last_input_result;
}inputs;

#ifndef _WIN32
void send_syn(inputs* inp) 
{
    struct input_event ev_syn = { .type = EV_SYN, .code = SYN_REPORT, .value = 0 };
    if (write(inp->fd, &ev_syn, sizeof(ev_syn)) == -1) {
        /* perror("Error sending EV_SYN event"); */
    }
}

/* This needs to be done manually ig */
void register_regular_keys(inputs* inp)
{
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_A);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_B);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_C);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_D);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_E);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_F);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_G);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_H);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_I);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_J);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_K);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_L);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_M);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_N);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_O);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_P);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_Q);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_R);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_S);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_T);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_U);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_V);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_W);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_X);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_Y);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_Z);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_0);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_1);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_2);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_3);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_4);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_5);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_6);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_7);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_8);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_9);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_LEFTCTRL);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_RIGHTCTRL);
    ioctl(inp->fd, UI_SET_KEYBIT, SDLK_LCTRL);
    ioctl(inp->fd, UI_SET_KEYBIT, SDLK_RCTRL);
    ioctl(inp->fd, UI_SET_KEYBIT, SDLK_LALT);
    ioctl(inp->fd, UI_SET_KEYBIT, SDLK_RALT);
    ioctl(inp->fd, UI_SET_KEYBIT, SDLK_LGUI);
    ioctl(inp->fd, UI_SET_KEYBIT, SDLK_RGUI);
    ioctl(inp->fd, UI_SET_KEYBIT, SDLK_MODE);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_ENTER);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_ESC);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_BACKSPACE);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_TAB);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_SPACE);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_UP);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_DOWN);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_LEFT);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_RIGHT);
}

/* The name is misleading, this also registers some more keys that are usually next to the numpad */
void register_function_keys(inputs* inp)
{
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_F1);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_F2);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_F3);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_F4);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_F5);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_F6);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_F7);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_F8);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_F9);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_F10);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_F11);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_F12);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_INSERT);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_HOME);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_PAGEUP);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_PAGEDOWN);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_DELETE);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_END);
}

void register_numpad_keys(inputs* inp)
{
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KP0);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KP1);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KP2);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KP3);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KP4);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KP5);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KP6);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KP7);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KP8);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KP9);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KPSLASH);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KPASTERISK);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KPMINUS);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KPPLUS);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KPENTER);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KPDOT);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KPEQUAL);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KPCOMMA);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KPEQUAL);
}

void register_other_key(inputs* inp)
{
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_PAUSE);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_SYSRQ);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_COMPOSE);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_HELP);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_MENU);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_SELECT);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_STOP);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_AGAIN);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_UNDO);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_CUT);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_COPY);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_PASTE);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_FIND);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_MUTE);
}

void register_intl_keys(inputs* inp)
{
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_LEFTCTRL); /* What is this key for? */
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_RIGHTCTRL); /* What is this key for? */
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_COMPOSE); /* What is this key? */
}

void register_keypad_special_keys(inputs* inp)
{
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KPLEFTPAREN);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KPRIGHTPAREN);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KPLEFTPAREN);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KPRIGHTPAREN);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KPLEFTPAREN);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KPRIGHTPAREN);
    ioctl(inp->fd, UI_SET_KEYBIT, KEY_KPPLUSMINUS);
}

#endif

/* Regular keys include letters, modifiers, locks, and numbers basically a 60% layout with the arrows and some modifiers */
#define REGULAR_KEYS 1
#define FUNCTION_KEYS 2
#define NUMPAD_KEYS 4
#define OTHER_KEYS 8
#define MEDIA_KEYS 16
#define INTL_KEYS 32
#define KEYPAD_SPECIAL 64
#define EXTENDED_KEYS 128
#define SIXTY_PERCENT_KEY REGULAR_KEYS
#define TENKEYLESS (REGULAR_KEYS | FUNCTION_KEYS)
#define FULL_LAYOUT (REGULAR_KEYS | FUNCTION_KEYS | NUMPAD_KEYS | MEDIA_KEYS)
#define EXTENDED_LAYOUT (REGULAR_KEYS | FUNCTION_KEYS | NUMPAD_KEYS | MEDIA_KEYS | INTL_KEYS | KEYPAD_SPECIAL | EXTENDED_KEYS)

inputs* create_inputs_struct(void)
{
    inputs* inp;
    inp = (inputs*)malloc(sizeof(inputs));
    #ifndef _WIN32
    inp->fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (inp->fd < 0)
        return NULL;
    memset(&inp->usetup, 0, sizeof(inp->usetup));
    strncpy(inp->usetup.name, "virtual-keyboard", UINPUT_MAX_NAME_SIZE);
    inp->usetup.id.bustype = BUS_USB;
    inp->usetup.id.vendor = 0x0001;
    inp->usetup.id.product = 0x0001;

    ioctl(inp->fd, UI_SET_EVBIT, EV_KEY);

    ioctl(inp->fd, UI_DEV_SETUP, &inp->usetup);
    ioctl(inp->fd, UI_DEV_CREATE);

    /* This should do the trick for a small delay with volatile */
    volatile int i;
    for (i = 0; i <1000000; i+=1);
    #endif
    /* By default */
    inp->last_input_result=0;
    return inp;
}

void send_key(inputs* inp, int key, int mod, int flags)
{
#ifndef _WIN32

    if (flags == SDL_INPUT)
    {
        key = convertSDL2KeyToUInput(key);
        mod = convertSDL2ModToUInput(mod);
    }
    memset(&inp->ev_key, 0, sizeof(inp->ev_key));
    memset(&inp->ev_mod, 0, sizeof(inp->ev_mod));
    if (mod != 0)
    {
        inp->ev_mod.type = EV_KEY;
        inp->ev_mod.code = mod;
        inp->ev_mod.value = 1;
        write(inp->fd, &inp->ev_mod, sizeof(inp->ev_mod));

        send_syn(inp);
    }
    inp->ev_key.type = EV_KEY;
    inp->ev_key.code = key;
    inp->ev_key.value = 1;
    write(inp->fd, &inp->ev_key, sizeof(inp->ev_key));
    send_syn(inp);
    if (mod != 0)
    {
        inp->ev_mod.value = 0;
        write(inp->fd, &inp->ev_mod, sizeof(inp->ev_mod));

        send_syn(inp);
    }
    inp->ev_key.value = 0;
    write(inp->fd, &inp->ev_key, sizeof(inp->ev_key));

    send_syn(inp);
#endif
}