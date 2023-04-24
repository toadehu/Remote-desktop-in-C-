#pragma once

#include <threads.h>

/*
* Layout of the buffer_left array:
* If the 31st bit is set, the buffer is waiting to initialize.
*/
typedef struct receiver
{
    unsigned char** buffer;
    int *buffer_left;
    clock_t *first_received;
} receiver_t;

receiver_t* receiver_create(int no_of_buffers, int buffer_max_size)
{
    receiver_t* receiver = (receiver_t*)malloc(sizeof(receiver_t));
    receiver -> buffer = (unsigned char**)malloc(no_of_buffers * sizeof(unsigned char*));
    receiver -> buffer_left = (int*)malloc(no_of_buffers * sizeof(int));
    receiver -> first_received = (clock_t*)malloc(no_of_buffers * sizeof(clock_t));
    memset(receiver -> first_received, 0, no_of_buffers * sizeof(clock_t));
    for (int i = 0; i < no_of_buffers; i+=1)
    {
        receiver -> buffer[i] = (unsigned char*)malloc(buffer_max_size * sizeof(unsigned char));
        receiver -> buffer_left[i] = (1<<31);
    }
    return receiver;
}

