#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOWER_BITRATE_PROFILE 0 /* At the moment LOWER_BITRATE_PROFILE will use RLE where possible for the encoding, otherwise, it may resize the image and apply rle again, or jpeg */
#define VIDEO_YUV420 1
#define RLE_TWO_PASS 2
#define RLE_ONE_PASS 4

typedef struct basic_video_enc{
    int width;
    int height;
    int bitrate;
    int profile;
    int preset;
    int image_size;
    int final_flags;           /* Here we store the final flags for the encoding*/
    char* prev_image;
    char* first_pass;          /* This is where the first pass of encoding is stored, whether it is rle with one or two passes or something else */
    char* first_pass_size;
    char* first_pass_pass2;
    int pass1_size, pass2_size;
} basic_video_enc;

/**
 * @brief This function creates a basic video encoding structure
 * 
 * @param width The width of the video
 * @param height The height of the video
 * @param bitrate The bitrate of the video
 * @param profile The profile of the video (like YUV420, YUV444, RGB, ARGB and so on)
 * @param preset The preset of the encoding (like RLE_TWO_PASS, RLE_ONE_PASS, LOWER_BITRATE_PROFILE)
 * @return basic_video_enc* A pointer to the basic video encoding structure
*/
basic_video_enc* basic_create_video_enc(int width, int height, int bitrate, int profile, int preset)
{
    basic_video_enc* basic = (basic_video_enc*)malloc(sizeof(basic_video_enc));
    if (basic == NULL)
    {
        printf("Error allocating memory for the video encoder\n");
        return NULL;
    }
    basic -> width = width;
    basic -> height = height;
    basic -> bitrate = bitrate;
    basic -> profile = profile;
    basic -> preset = preset;
    if (profile == VIDEO_YUV420) 
    {
        basic -> image_size = (width * height * 3) / 2;
    }
    basic -> prev_image = (char*)malloc(basic -> image_size);
    if (basic->prev_image == NULL)
    {
        free(basic);
        return NULL;
    }
    memset(basic -> prev_image, 0, basic -> image_size);
    basic -> first_pass = (char*)malloc(basic -> image_size);
    if (basic->first_pass == NULL)
    {
        free(basic);
        return NULL;
    }
    memset(basic -> first_pass, 0, basic -> image_size);
    basic -> first_pass_size = (char*)malloc(basic -> image_size);
    if (basic->first_pass_size == NULL)
    {
        free(basic);
        return NULL;
    }
    memset(basic -> first_pass_size, 0, basic -> image_size);
    basic -> first_pass_pass2 = (char*)malloc(basic -> image_size);
    if (basic->first_pass_pass2 == NULL)
    {
        free(basic);
        return NULL;
    }
    memset(basic -> first_pass_pass2, 0, basic -> image_size);
    return basic;
}

/**
 * @brief This function performs the first pass of rle encoding
 * 
 * @param original The original image
 * @param video The video encoding structure
 * @return int The size of the first pass
*/
int rle_first_pass(char* original, basic_video_enc* video)
{
    video -> pass1_size = 0;
    char curmatch = 1, comp = original[0];

    int i;
    for (i = 1; i < video -> image_size; i++)
    {
        char cval = original[i];
        if (cval != comp)
        {
            video -> first_pass[video -> pass1_size] = comp;
            video -> first_pass_size[video -> pass1_size] = curmatch;
            video -> pass1_size += 1;
            comp = cval;
            curmatch = 1;
        }
        else if (curmatch == (char)255)
        {
            video -> first_pass[video -> pass1_size] = comp;
            video -> first_pass_size[video -> pass1_size] = (char)255;
            video -> pass1_size += 1;
            curmatch = 1;
        }
        else
        {
            curmatch++;
        }
    }
    if (curmatch != 0)
    {
        video -> first_pass[video -> pass1_size] = comp;
        video -> first_pass_size[video -> pass1_size] = curmatch;
        video -> pass1_size += 1;
    }
    return video -> pass1_size;
}

/**
 * @brief This function performs the second pass of rle encoding, this time to the lengths of the runs
 * 
 * @param video The video encoding structure
 * @return int The size of the second pass
*/
int rle_second_pass(basic_video_enc* video)
{
    video -> pass2_size = 0;
    char curmatch = 1, comp = video -> first_pass_size[0];
    int i;
    for (i = 1; i < video -> pass1_size; i++)
    {
        if (comp != video -> first_pass_size[i])
        {
            video -> first_pass_pass2[video -> pass2_size] = curmatch;
            video -> pass2_size += 1;
            comp = video -> first_pass_size[i];
            curmatch = 1;
        }
        else if (curmatch == (char)255)
        {
            video -> first_pass_pass2[video -> pass2_size] = (char)255;
            video -> pass2_size += 1;
            curmatch = 1;
        }
        else
        {
            curmatch++;
        }
    }
    if (curmatch != 0)
    {
        video -> first_pass_pass2[video -> pass2_size] = curmatch;
        video -> pass2_size += 1;
    }
    int ci = 0;
    for (i = 0; i < video -> pass2_size; i++)
    {
        //if ((u_char)video -> first_pass_pass2[i] > 50)
            //printf("Current data: %d, at %d\n", (u_char)video -> first_pass_pass2[i], i);
        video -> first_pass_size[i] = video -> first_pass_size[ci];
        ci += (unsigned char)video -> first_pass_pass2[i];
    }
    return video -> pass2_size;
}

/**
 * @brief This function encodes the next frame of the video
 * 
 * @param video The video encoding structure
 * @param next_frame The next frame to encode
 * @param flags The flags to be set by the encoder
 * @return int The size of the encoded frame. Negative number indicates an error or unim4plemented feature
*/
int basic_encode_next_frame(basic_video_enc* video, char* next_frame, int* flags)
{
    if (video -> preset & RLE_TWO_PASS)
    {
        int first_arr_size = rle_first_pass(next_frame, video);
        int second_arr_size = 2 * rle_second_pass(video);
        printf("First pass size: %d, second pass size: %d\n", first_arr_size, second_arr_size);
        memcpy(video -> prev_image, next_frame, video -> image_size);
        *flags = RLE_TWO_PASS;
        return first_arr_size + second_arr_size;
    }
    else if (video -> preset & RLE_ONE_PASS)
    {
        int first_arr_size = 2 * rle_first_pass(next_frame, video);
        memcpy(video -> prev_image, next_frame, video -> image_size);
        *flags = RLE_ONE_PASS;
        return first_arr_size;
    }
    return -1;
}

/**
 * @brief This function copies the encoded frame to a given buffer
 * 
 * @param video The video encoding structure
 * @param buffer The buffer where the frame is copied
 * @param size_of_buffer The size of the buffer
 * @param offset The offset in the buffer where the frame is copied
 * @param flags The flags given by the basic_encode_next_frame function
 * @return int 0 on success, non-zero values indicate an error, such as invalid flags or buffer too small
*/
int basic_copy_frame(basic_video_enc* video, char* buffer, int size_of_buffer, int offset, int flags)
{
    int req_size = 0;
    switch (video -> preset)
    {
        case RLE_TWO_PASS:
            req_size = video -> pass1_size + 2 * video -> pass2_size;
            if (size_of_buffer < req_size + offset)
            {
                return 1; /* Buffer too small */
            }
            memcpy(buffer + offset, video -> first_pass, video -> pass1_size);
            memcpy(buffer + offset + video -> pass1_size, video -> first_pass_size, video -> pass2_size);
            memcpy(buffer + offset + video -> pass1_size + video -> pass2_size, video -> first_pass_pass2, video -> pass2_size);
            return 0;
        case RLE_ONE_PASS:
            req_size = video -> pass1_size * 2;
            if (size_of_buffer < req_size + offset)
            {
                return 1; /* Buffer too small */
            }
            memcpy(buffer + offset, video -> first_pass, video -> pass1_size);
            memcpy(buffer + offset + video -> pass1_size, video -> first_pass_size, video -> pass1_size);
            return 0;
        default:
            return 1; /* Invalid flags */
    }
}


/**
 * @brief This function copies the encoded frame to a given buffer
 * 
 * @param video The video encoding structure
 * @param buffer The buffer where the frame is copied
 * @param size_of_buffer Pointer to the variable that stores the size of the buffer, needed in case the buffer is too small
 * @param offset The offset in the buffer where the frame is copied
 * @param flags The flags given by the basic_encode_next_frame function
 * @return int 0 on succes, non-zero values indicate an error, such as invalid flags
*/
int basic_copy_frame_realloc(basic_video_enc* video, char** buffer, int* size_of_buffer, int offset, int flags)
{
    int req_size = 0;
    switch (video -> preset)
    {
        case RLE_TWO_PASS:
            req_size = video -> pass1_size + 2*video -> pass2_size;
            if (*size_of_buffer < req_size)
            {
                int new_size = 2 * (*size_of_buffer) > req_size + offset ? 2 * (*size_of_buffer) : req_size + offset + 128 * 1024;
                *buffer = (char*)realloc((void*)*buffer, new_size);
                *size_of_buffer = new_size;
            }
            memcpy(*buffer + offset, video -> first_pass, video -> pass1_size);
            memcpy(*buffer + offset + video -> pass1_size, video -> first_pass_size, video -> pass2_size);
            memcpy(*buffer + offset + video -> pass1_size + video -> pass2_size, video -> first_pass_size, video -> pass2_size);
            return 0;
        case RLE_ONE_PASS:
            req_size = video -> pass1_size * 2;
            if (*size_of_buffer < req_size)
            {
                int new_size = 2 * (*size_of_buffer) > req_size ? 2 * (*size_of_buffer) : req_size + 1024*1024;
                *buffer = (char*)realloc((void*)*buffer, new_size);
                *size_of_buffer = new_size;
            }
            memcpy(*buffer + offset, video -> first_pass, video -> pass1_size);
            memcpy(*buffer + offset + video -> pass1_size, video -> first_pass_size, video -> pass1_size);
            return 0;
        default:
            return 1; /* Invalid flags */
    }
}

typedef struct basic_video_dec{
    int width;
    int height;
    int bitrate;
    int profile; /* The profile of the video (like YUV420, YUV444, RGB, ARGB and so on) */
    int preset; /* The preset of the video (like RLE_TWO_PASS, RLE_ONE_PASS, LOWER_BITRATE_PROFILE) */
    int image_size;
    char* first_pass;
    char* first_pass_size;
    int pass1_size, pass2_size;
} basic_video_dec;

/**
 * @brief This function creates a basic video decoder
 * 
 * @param width The width of the video
 * @param height The height of the video
 * @param bitrate The bitrate of the video
 * @param profile The profile of the video (like YUV420, YUV444, RGB, ARGB and so on)
 * @param preset The preset of the video (like RLE_TWO_PASS, RLE_ONE_PASS, LOWER_BITRATE_PROFILE)
 * @return basic_video_dec* A pointer to the newly created basic video decoder
*/
basic_video_dec* basic_create_video_dec(int width, int height, int bitrate, int profile, int preset)
{
    basic_video_dec* basic = (basic_video_dec*)malloc(sizeof(basic_video_dec));
    if (basic == NULL)
    {
        printf("Error allocating memory for the video decoder\n");
        return NULL;
    }
    basic -> width = width;
    basic -> height = height;
    basic -> bitrate = bitrate;
    basic -> profile = profile;
    basic -> preset = preset;
    if (basic -> profile == VIDEO_YUV420) 
    {
        basic -> image_size = (width * height * 3) / 2;
    }
    printf("Image size: %d\n", basic -> image_size);
    basic -> first_pass = (char*)malloc(basic -> image_size);
    if (basic -> first_pass == NULL)
    {
        printf("Error allocating memory for first pass\n");
        free(basic);
        return NULL;
    }
    memset(basic -> first_pass, 0, basic -> image_size);
    basic -> first_pass_size = (char*)malloc(basic -> image_size);
    if (basic->first_pass_size == NULL)
    {
        printf("Error allocating memory for first pass size\n");
        free(basic);
        return NULL;
    }
    memset(basic -> first_pass_size, 0, basic -> image_size);
    return basic;
}


/**
 * @brief This function decodes the last pass of the RLE encoding done
 * 
 * @param data The data to be decoded
 * @param size The size of the data to be decoded
 * @param video The video decoding structure
*/
void rle_first_decode(char* data, int size, basic_video_dec* video, char* orig)
{
    int i, j;
    video -> pass2_size = 0;
    for (i = 0; i < size; i+=1)
    {
        for (j = 0; j < (unsigned char)data[i + size]; j+=1)
        {
            video -> first_pass_size[video -> pass2_size + j] = data[i];
        }
        video -> pass2_size += (unsigned char)data[i + size];
    }
    printf("Pass2 size: %d\n^^^^^^^^^^^^^^^^^^^\n", video -> pass2_size);
}

void rle_second_decode(char* data, basic_video_dec* video)
{
    int i, j;
    video -> pass1_size = 0;
    for (i = 0; i < video -> pass2_size; i+=1)
    {
        for (j = 0; j < (u_char)video ->  first_pass_size[i]; j+=1)
        {
            video -> first_pass[video -> pass1_size + j] = data[i];
        }
        video -> pass1_size += (u_char)video -> first_pass_size[i];
    }
    printf("Pass1 size: %d\n^^^^^^^^^^^^^^^^^^^\n", video -> pass1_size);
}

/**
 * @brief This function decodes the next frame of the video
 * 
 * @param video A pointer to the video decoding structure
 * @param data The data to be decoded
 * @param size_of_data The size of the data to be decoded
 * @param offset The offset for the RLE_TWO_PASS encoding
 * @param flags The flags given by the basic_encode_next_frame function
*/
void basic_decode_next_frame(basic_video_dec* video, char* data, int size_of_data, int offset, int flags)
{
    if (video -> preset == RLE_TWO_PASS)
    {
        int offset_for_first_pass = size_of_data - 2 * offset;
        printf("Offset is %d so first pass size is %d\n", offset, offset_for_first_pass);
        rle_first_decode(data + offset_for_first_pass, offset , video, data);
        rle_second_decode(data, video);
    }
}

/**
 * @brief This function copies the encoded frame to a given buffer
 * 
 * @param video The video decoding structure
 * @param buffer The buffer where the frame is copied
 * @param size_of_buffer The size of the buffer
 * @param offset The offset in the buffer where the frame is copied
 * @param flags The flags given by the basic_encode_next_frame function
 * @return int 0 on success, non-zero values indicate an error, such as invalid flags or buffer too small
*/
int basic_copy_frame_d(basic_video_dec* video, char* buffer, int size_of_buffer, int offset, int flags)
{
    int req_size = 0;
    req_size = video -> pass1_size;
    if (size_of_buffer < req_size + offset)
    {
        return 1; /* Buffer too small */
    }
    memcpy(buffer + offset, video -> first_pass, video -> pass1_size);
    return 0;
    switch (video -> preset)
    {
        case RLE_TWO_PASS:
            req_size = video -> pass1_size + 2*video -> pass2_size;
            if (size_of_buffer < req_size + offset)
            {
                return 1; /* Buffer too small */
            }
            memcpy(buffer + offset, video -> first_pass, video -> pass1_size);
            return 0;
        case RLE_ONE_PASS:
            req_size = video -> pass1_size * 2;
            if (size_of_buffer < req_size + offset)
            {
                return 1; /* Buffer too small */
            }
            memcpy(buffer + offset, video -> first_pass,  video -> pass1_size);
            memcpy(buffer + offset + video -> pass1_size, video -> first_pass_size, video -> pass1_size);
            return 0;
        default:
            return 1; /* Invalid flags */
    }
}

/**
 * @brief This function copies the encoded frame to a given buffer
 * 
 * @param video The video decoding structure
 * @param buffer The buffer where the frame is copied
 * @param size_of_buffer Pointer to the variable that stores the size of the buffer, needed in case the buffer is too small, for the call to realloc
 * @param offset The offset in the buffer where the frame is copied
 * @param flags The flags given by the basic_encode_next_frame function
 * @return int 0 on succes, non-zero values indicate an error, such as invalid flags
*/
int basic_copy_frame_d_realloc(basic_video_dec* video, char** buffer, int* size_of_buffer, int offset, int flags)
{
    int req_size = 0;
    switch (video -> preset)
    {
        case RLE_TWO_PASS:
            req_size = video -> pass1_size + 2*video -> pass2_size;
            if (*size_of_buffer < req_size)
            {
                int new_size = 2 * (*size_of_buffer) > req_size + offset ? 2 * (*size_of_buffer) : req_size + offset + 128 * 1024;
                *buffer = (char*)realloc((void*)*buffer, new_size);
                *size_of_buffer = new_size;
            }
            memcpy(*buffer + offset, video -> first_pass, video -> pass1_size);
            memcpy(*buffer + offset + video -> pass1_size, video -> first_pass_size, video -> pass2_size);
            memcpy(*buffer + offset + video -> pass1_size + video -> pass2_size, video -> first_pass_size, video -> pass2_size);
            return 0;
        case RLE_ONE_PASS:
            req_size = video -> pass1_size * 2;
            if (*size_of_buffer < req_size)
            {
                int new_size = 2 * (*size_of_buffer) > req_size ? 2 * (*size_of_buffer) : req_size + 1024*1024;
                *buffer = (char*)realloc((void*)*buffer, new_size);
                *size_of_buffer = new_size;
            }
            memcpy(*buffer + offset, video -> first_pass, video -> pass1_size);
            memcpy(*buffer + offset + video -> pass1_size, video -> first_pass_size, video -> pass1_size);
            return 0;
        default:
            return 1; /* Invalid flags */
    }
}
