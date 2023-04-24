#pragma once

#ifdef __WIN32__
#include <winsock2.h>
#else
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include "buffer_receiver.h"

/*
 The first byte of the packet will say what type of information it can contain. Here is how information is encoded:
                        1   1   1   1   1   1   1   1  
                        ^   ^   ^   ^   ^   ^   ^   ^
                        |   |   |   |   |   |   |   |
                        |   |   |   |   |   |   |   It's an h264 frame
                        |   |   |   |   |   |   It's a mouse input, the mouse moved
                        |   |   |   |   |   It's a mouse input, either left or right click
                        |   |   |   |   It's a mouse scroll
                        |   |   |   It's a different mouse button, oem defined instruction, or something of this sort
                        |   |   Keyboard input
                        |   Undefined at the moment
                        It's the first packet for connection & authentification
*/
enum frame_type{
    h264_frame = 0x01,
    mouse_input_move = 0x02,
    mouse_input_click = 0x04,
    mouse_input_scroll = 0x08,
    mouse_input_oem = 0x10,
    keybd_input = 0x20,
    auth = 0x80

};

typedef struct UDP_Server
{
    int udp_socket;
    struct sockaddr_in server;
    struct sockaddr_in client;
    int client_len;
    int port;
    unsigned char buffer[263];
    int buffer_len;
    receiver_t *receiver;
} UDP_SERVER;

UDP_SERVER* udp_server_encoder_create(int port)
{
    UDP_SERVER* server = (UDP_SERVER*)malloc(sizeof(UDP_SERVER));
    server -> port = port;
    server -> udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    server -> receiver = NULL;
    if (server -> udp_socket == -1)
    {
        printf("Error creating socket, you should reconsider what you are doing\n");
        return NULL;
    }
    memset(&server -> server, 0, sizeof(server -> server));
    server -> server.sin_family = AF_INET;
    server -> server.sin_addr.s_addr = htonl(INADDR_ANY);
    server -> server.sin_port = htons(port);
    bind(server -> udp_socket, (struct sockaddr*)&server -> server, sizeof(server -> server));
    return server;
}

UDP_SERVER* udp_server_decoder_create(int port)
{
    UDP_SERVER* server = (UDP_SERVER*)malloc(sizeof(UDP_SERVER));
    server -> port = port;
    server -> udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    server -> receiver = receiver_create(10, 1024 * 1024);
    if (server -> udp_socket == -1)
    {
        printf("Error creating socket, you should reconsider what you are doing\n");
        return NULL;
    }
    memset(&server -> server, 0, sizeof(server -> server));
    server -> server.sin_family = AF_INET;
    server -> server.sin_addr.s_addr = htonl(INADDR_ANY);
    server -> server.sin_port = htons(port);
    bind(server -> udp_socket, (struct sockaddr*)&server -> server, sizeof(server -> server));
    return server;
}

bool udp_server_bind_client(UDP_SERVER* server, int client_ip, int client_port)
{
    memset(&server -> client, 0, sizeof(server -> client));
    server -> client.sin_family = AF_INET;
    server -> client.sin_addr.s_addr = htonl(client_ip);
    server -> client.sin_port = htons(client_port);
    return true;
}

void udp_server_send(UDP_SERVER* server, unsigned char* buffer, int buffer_size)
{
    sendto(server -> udp_socket, buffer, buffer_size, 0, (struct sockaddr*)&server -> client, sizeof(server -> client));
}

/*  Represenatation of frame:
*   byte0 : frame type
*   byte1 : packet id (frame number 0-255)
*   byte2 & byte3: total number of packets
*   byte4 & byte5: id of packet (0-65535) used to order packets on the client side
*   byte6: packet size (0 for all 256 bytes, otherwise the number of bytes)
*   bytes 7-262: packet data
*/
void create_encoded_frame(UDP_SERVER* server, unsigned char* buffer, int buffer_size, unsigned char packet_id)
{
    int number_of_packets = buffer_size / 256;
    for (int i = 0; i < number_of_packets; i+=1)
    {
            server -> buffer[0] = h264_frame;
            server -> buffer[1] = packet_id;
            server -> buffer[2] = number_of_packets >> 8;
            server -> buffer[3] = number_of_packets & 0xFF;
            server -> buffer[4] = i >> 8;
            server -> buffer[5] = i & 0xFF;
            server -> buffer[6] = 0;
            memcpy(&server -> buffer[7], &buffer[i * 256], 256);
            udp_server_send(server, server -> buffer, 263);
    }
    server -> buffer[0] = h264_frame;
    server -> buffer[1] = packet_id;
    server -> buffer[2] = number_of_packets >> 8;
    server -> buffer[3] = number_of_packets & 0xFF;
    server -> buffer[4] = number_of_packets >> 8;
    server -> buffer[5] = number_of_packets & 0xFF;
    server -> buffer[6] = number_of_packets & 0XFF;
    memcpy(&server -> buffer[7], &buffer[number_of_packets * 256], server -> buffer[6]);
    udp_server_send(server, server -> buffer, 7 + (number_of_packets & 0XFF));
}

void create_auth_frame(UDP_SERVER* server, long long int auth1, long long int auth2, long long int auth3, long long int auth4)
{
    server -> buffer[0] = auth;
    memcpy(server -> buffer + 1, &auth1, 8);
    memcpy(server -> buffer + 9, &auth2, 8);
    memcpy(server -> buffer + 17, &auth3, 8);
    memcpy(server -> buffer + 25, &auth4, 8);   
    udp_server_send(server, server -> buffer, 33);
}

/*
*   Represenatation of frame:
*   byte0 : frame type
*   byte1 & byte2 & byte3 & byte4 : data
*   data layout:
*   If it is a mouse event, the first 2 bytes will be the x coordinate, the second 2 bytes will be the y coordinate, the 5th byte is information about the click event, and operating system
*   If it's a scroll event, the data will be the scroll amount, the 5th byte is information about the operating system, if needed
*   If it is a keyboard event, the first byte will be the key code, the 2nd byte is information about the key event, 3rd is information about the keyboard layout, 4th is information about the operating system
*/
void create_other_frame(UDP_SERVER* server, unsigned char frame_type, int data1, unsigned char data2)
{
    server -> buffer[0] = frame_type;
    memcpy(server -> buffer + 1, &data1, 4);
    server -> buffer[5] = data2;
    udp_server_send(server, server -> buffer, 6);
}