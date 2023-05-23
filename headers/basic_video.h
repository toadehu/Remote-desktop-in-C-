#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIDEO_YUV420 1
#define RLE_TWO_PASS 2

typedef struct basic_video_enc{
    int width;
    int height;
    int bitrate;
    int profile;
    int preset;
    int image_size;
    unsigned char* prev_image;
    unsigned char* rle_image;
    unsigned char* rle_image_size;
    unsigned char* rle_image_pass2;
    int rle_size1, rle_size2;
} basic_video_enc;

basic_video_enc* create_basic_video_enc(int width, int height, int bitrate, int profile, int preset)
{
    basic_video_enc* basic = (basic_video_enc*)malloc(sizeof(basic_video_enc));
    basic -> width = width;
    basic -> height = height;
    basic -> bitrate = bitrate;
    basic -> profile = profile;
    basic -> preset = preset;
    if (profile & VIDEO_YUV420) 
    {
        basic -> image_size = (width * height * 3) / 2;
        basic -> prev_image = (unsigned char*)malloc(basic -> image_size);
        memset(basic -> prev_image, 0, basic -> image_size);
        basic -> rle_image = (unsigned char*)malloc(basic -> image_size);
        memset(basic -> rle_image, 0, basic -> image_size);
        basic -> rle_image_size = (unsigned char*)malloc(basic -> image_size);
        memset(basic -> rle_image_size, 0, basic -> image_size);
        basic -> rle_image_pass2 = (unsigned char*)malloc(basic -> image_size);
        memset(basic -> rle_image_pass2, 0, basic -> image_size);
    }
    return basic;
}


int rle_first_pass(unsigned char* original, basic_video_enc* video)
{
    video -> rle_size1 = 0;
    unsigned char curmatch = 0, comp = 0, cval = 0;
    if (original[0] != video -> prev_image[0])
    {
        cval = original[0];
    }
    for (int i = 0; i < video -> image_size; i+=1)
    {
        cval = (original[i] != video->prev_image[i]) ? original[i] : 0;
        if (cval != comp)
        {
            if (curmatch != 0) 
            {
                video -> rle_image[video -> rle_size1] = comp;
                video -> rle_image_size[video -> rle_size1] = curmatch;
                video -> rle_size1 += 1;
                curmatch = 0;
            }
            comp = cval;

        }
        if (curmatch == 255)
        {
            video -> rle_image[video -> rle_size1] = comp;
            video -> rle_image_size[video -> rle_size1] = 255;
            video -> rle_size1 += 1;
            curmatch = 0;
        }
        curmatch++;
    }
    if (curmatch != 0)
    {
        video -> rle_image[video -> rle_size1] = comp;
        video -> rle_image_size[video -> rle_size1] = curmatch;
        video -> rle_size1 += 1;
    }
    return video -> rle_size1;
}

int rle_second_pass(basic_video_enc* video)
{
    video -> rle_size2 = 0;
    unsigned char curmatch = 0, comp = video -> rle_image_size[0];
    for (int i = 0; i < video -> rle_size1; i+=1)
    {
        if (comp != video -> rle_image_size[i])
        {
            if (curmatch)
            {
                video -> rle_image_pass2[video -> rle_size2] = curmatch;
                video -> rle_size2 += 1;
                curmatch = 0;
            }
            comp = video -> rle_image_size[i];
        }
        if (curmatch == 255)
        {
            video -> rle_image_pass2[video -> rle_size2] = 255;
            video -> rle_size2 += 1;
            curmatch = 0;
        }
        curmatch++;
        /*
        if (video -> rle_image_size[i] == comp)
        {
            if (curmatch == 255)
            {
                video -> rle_image_pass2[video -> rle_size2] = comp;
                video -> rle_size2 += 1;
                curmatch = 0;
            }
            curmatch++;
        }
        else
        {
            if (curmatch)
            {
                video -> rle_image_pass2[video -> rle_size2] = comp;
                video -> rle_size2 += 1;
                curmatch = 1;
            }
            comp = video -> rle_image_size[i];
        }
        */
    }
    if (curmatch)
    {
        video -> rle_image_pass2[video -> rle_size2] = curmatch;
        video -> rle_size2 += 1;
    }
    int cur_sum = 0;
    for (int i = 0; i < video -> rle_size2; i+=1)
    {
        video -> rle_image_size[i] = video -> rle_image_size[cur_sum];
        cur_sum += video -> rle_image_pass2[i];
    }
    return video -> rle_size2;
}

int basic_encode_next_frame(basic_video_enc* video, unsigned char* next_frame)
{
    if (video -> profile & RLE_TWO_PASS)
    {
        int first_arr_size = rle_first_pass(next_frame, video);
        int second_arr_size = 2 * rle_second_pass(video);
        memcpy(video -> prev_image, next_frame, video -> image_size);
        #ifdef _DEBUG
        printf("Finished first pass, size: %d\n", first_arr_size);
        printf("Finished second pass, size: %d\n", second_arr_size);
        #endif
        return first_arr_size + second_arr_size;
    }
    return -1;
}

typedef struct basic_video_dec{
    int width;
    int height;
    int bitrate;
    int profile;
    int preset;
    int image_size;
    unsigned char* rle_image;
    unsigned char* rle_image_size;
    int rle_size1, rle_size2;
} basic_video_dec;

basic_video_dec* create_basic_video_dec(int width, int height, int bitrate, int profile, int preset)
{
    basic_video_dec* basic = (basic_video_dec*)malloc(sizeof(basic_video_dec));
    basic -> width = width;
    basic -> height = height;
    basic -> bitrate = bitrate;
    basic -> profile = profile;
    basic -> preset = preset;
    if (profile & VIDEO_YUV420) 
    {
        basic -> image_size = (width * height * 3) / 2;
        basic -> rle_image = (unsigned char*)malloc(basic -> image_size);
        memset(basic -> rle_image, 0, basic -> image_size);
        basic -> rle_image_size = (unsigned char*)malloc(basic -> image_size);
        memset(basic -> rle_image_size, 0, basic -> image_size);
    }
    return basic;
}

void rle_first_decode(unsigned char* data, int size, basic_video_dec* video)
{
    video -> rle_size2 = 0;
    for (int i = 0; i < size; i+=1)
    {
        while (data[i + size] > 0)
        {
            video -> rle_image_size[video -> rle_size2] = data[i];
            video -> rle_size2 += 1;
            data[i + size] -= 1;
        }
    }
}

void rle_second_decode(unsigned char* data, basic_video_dec* video)
{
    video -> rle_size1 = 0;
    for (int i = 0; i < video -> rle_size2; i+=1)
    {
        while (video -> rle_image_size[i] > 0)
        {
            video -> rle_image[video -> rle_size1] = data[i];
            video -> rle_size1 += 1;
            video -> rle_image_size[i] -= 1;
        }
    }
}

void basic_decode_next_frame(basic_video_dec* video, unsigned char* data, int size_of_data, int offset)
{
    if (video -> profile & RLE_TWO_PASS)
    {
        rle_first_decode(data + offset, (size_of_data - offset) / 2, video);
        printf("Value of offset: %d, and computed value: %d\n", offset, video -> rle_size2);
        rle_second_decode(data, video);
    }
}
