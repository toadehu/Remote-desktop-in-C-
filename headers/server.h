#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef __WIN32__
#include <winsock2.h>
#include <w2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define socklen_t int

#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define SOCKET int
#endif

#include "buffer_receiver.h"

// An arbitrary value for the MSS (Maximum Segment Size)
#define MSS 256
#define HDR_TYPE __uint64_t

/*
 The first byte of the packet will say what type of information it can contain. Here is how information is encoded:
                        1   1   1   1   1   1   1   1
                        ^   ^   ^   ^   ^   ^   ^   ^
                        |   |   |   |   |   |   |   |
                        |   |   |   |   |   |   |   It's a new frame
                        |   |   |   |   |   |   It's a mouse input, the mouse moved
                        |   |   |   |   |   It's a mouse input, either left or right click
                        |   |   |   |   It's a mouse scroll
                        |   |   |   It's a different mouse button, oem defined instruction, or something of this sort
                        |   |   Keyboard input
                        |   Undefined at the moment
                        It's the first packet for connection & authentification
*/
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

/**
 * Structure representing a TCP socket.
 */
typedef struct TCP_Socket
{
    int tcp_socket;            // Socket file descriptor
    struct sockaddr_in server; // Server address structure
    SOCKET *client;             // Clients' sockets
    int client_num;            // Number of clients
    int port;                  // Port number
    unsigned char buffer[263]; // Buffer for data
    int buffer_len;            // Length of the buffer
    receiver_t *receiver;      // Receiver structure for buffering received data
} TCP_SOCKET;

/**
 * Creates a TCP socket and initializes the server structure.
 *
 * @param port The port number to bind the socket.
 * @param ip_listen The IP address to bind the socket.
 * @param start_listen Flag indicating whether to start listening immediately.
 * @param max_connections Flag indicating how many connections to accept. The function is blocking untill all of the connections have been accepted
 * @return A pointer to the created TCP_SOCKET structure.
 */
TCP_SOCKET *TCP_SOCKET_create(int port, int ip_listen, bool start_listen, int max_connections)
{
    TCP_SOCKET *server = (TCP_SOCKET *)malloc(sizeof(TCP_SOCKET));

    // initialize winsock for Windows
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Error initializing winsock, you should reconsider what you are doing\n");
        return NULL;
    }
#endif

    // Creating the socket file descriptor
    server->port = port;
    server->tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server->tcp_socket == -1)
    {
        printf("Error creating socket, you should reconsider what you are doing\n");
        return NULL;
    }

    server->receiver = NULL;

    // Binding the socket to the port and to any address
    memset(&server->server, 0, sizeof(server->server));
    server->server.sin_family = AF_INET;
    server->server.sin_addr.s_addr = htonl(ip_listen);
    server->server.sin_port = htons(port);

    if (bind(server->tcp_socket, (struct sockaddr *)&server->server, sizeof(server->server)) < 0)
    {
        printf("Error binding socket");
        return NULL;
    }

    if ((start_listen = true))
    {
        server -> client = (SOCKET*)malloc(max_connections * sizeof(SOCKET));
        server -> client_num = max_connections;
        if (listen(server->tcp_socket, 3) < 0)
        {
            printf("Error listening to socket");
            return NULL;
        }

        for (int i = 0; i < max_connections; i++)
        {
            // Accept a new connection
            struct sockaddr_in client_addr;
            socklen_t addr_len = sizeof(client_addr);
            int client_socket = accept(server->tcp_socket, (struct sockaddr *)&client_addr, &addr_len);
            if (client_socket < 0)
            {
                printf("Error accepting connection number: %d\n", i);
                return NULL;
            }
            //store the client
            server -> client[i] = client_socket;
        }
    }
    return server;
}

/**
 * Creates a TCP socket for decoding packets and initializes the server structure.
 *
 * @param port The port number to bind the socket.
 * @return A pointer to the created TCP_SOCKET structure.
 */
TCP_SOCKET *TCP_SOCKET_decoder_create(int port)
{
    TCP_SOCKET *server = (TCP_SOCKET *)malloc(sizeof(TCP_SOCKET));
    server->port = port;
    server->tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    server->receiver = receiver_create(10, 1024 * 1024);
    if (server->tcp_socket == -1)
    {
        printf("Error creating socket, you should reconsider what you are doing\n");
        return NULL;
    }
    memset(&server->server, 0, sizeof(server->server));
    server->server.sin_family = AF_INET;
    server->server.sin_addr.s_addr = htonl(INADDR_ANY);
    server->server.sin_port = htons(port);
    bind(server->tcp_socket, (struct sockaddr *)&server->server, sizeof(server->server));
    return server;
}

/**
 * Sends a big frame over TCP by breaking it into multiple packets.
 *
 * @param sock The TCP socket.
 * @param frame The frame data to send.
 * @param frame_size The size of the frame data.
 * @param header The header value for the frame.
 */
void TCP_Send_Big_Frame(TCP_SOCKET *sock, unsigned char *frame, int frame_size, HDR_TYPE header)
{
    memcpy(sock->buffer, &header, sizeof(HDR_TYPE));
    memcpy(sock->buffer + 8, frame, 248);
    send(sock->client, sock->buffer, 256, 0);
    int loop_end = (frame_size - 248) / 256;
    for (int i = 0; i < loop_end; i += 1)
    {
        send(sock->client, frame + 248 + i * 256, 256, 0);
    }
    if ((frame_size - 248) & 255)
    {
        send(sock->client, frame + 248 + frame_size * 256, (frame_size - 248) & 255, 0);
    }
}

/**
 * Creates a header value for a frame with the specified size.
 *
 * @param frame_size The size of the frame.
 * @param header A pointer to store the created header value.
 */
void create_frame_header(int frame_size, HDR_TYPE *header)
{
    *header = new_frame | (frame_size << 1);
}

/**
 * Sends data over a TCP socket.
 *
 * @param server The TCP socket.
 * @param buffer The buffer containing the data to send.
 * @param buffer_size The size of the data to send.
 */
void TCP_Socket_send(TCP_SOCKET *server, unsigned char *buffer, int buffer_size)
{
    sendto(server->tcp_socket, buffer, buffer_size, 0, (struct sockaddr *)&server->client, sizeof(server->client));
}

/*  Represenatation of frame:
 *   byte0 : frame type
 *   byte1 : packet id (frame number 0-255)
 *   byte2 & byte3: total number of packets
 *   byte4 & byte5: id of packet (0-65535) used to order packets on the client side
 *   byte6: packet size (0 for all 256 bytes, otherwise the number of bytes)
 *   bytes 7-262: packet data
 */
void create_encoded_frame(TCP_SOCKET *server, unsigned char *buffer, int buffer_size, unsigned char packet_id)
{
    int number_of_packets = buffer_size / 256;
    for (int i = 0; i < number_of_packets; i += 1)
    {
        server->buffer[0] = new_frame;
        server->buffer[1] = packet_id;
        server->buffer[2] = number_of_packets >> 8;
        server->buffer[3] = number_of_packets & 0xFF;
        server->buffer[4] = i >> 8;
        server->buffer[5] = i & 0xFF;
        server->buffer[6] = 0;
        memcpy(&server->buffer[7], &buffer[i * 256], 256);
        // TCP_SOCKET_send(server, server -> buffer, 263);
    }
    server->buffer[0] = new_frame;
    server->buffer[1] = packet_id;
    server->buffer[2] = number_of_packets >> 8;
    server->buffer[3] = number_of_packets & 0xFF;
    server->buffer[4] = number_of_packets >> 8;
    server->buffer[5] = number_of_packets & 0xFF;
    server->buffer[6] = number_of_packets & 0XFF;
    memcpy(&server->buffer[7], &buffer[number_of_packets * 256], server->buffer[6]);
    // TCP_SOCKET_send(server, server -> buffer, 7 + (number_of_packets & 0XFF));
}

/**
 * Creates an authentication frame for sending over TCP.
 *
 * @param server The TCP socket.
 * @param auth1 The first authentication value.
 * @param auth2 The second authentication value.
 * @param auth3 The third authentication value.
 * @param auth4 The fourth authentication value.
 */
void create_auth_frame(TCP_SOCKET *server, long long int auth1, long long int auth2, long long int auth3, long long int auth4)
{
    server->buffer[0] = auth;
    memcpy(server->buffer + 1, &auth1, 8);
    memcpy(server->buffer + 9, &auth2, 8);
    memcpy(server->buffer + 17, &auth3, 8);
    memcpy(server->buffer + 25, &auth4, 8);
    // TCP_SOCKET_send(server, server -> buffer, 33);
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
void create_other_frame(TCP_SOCKET *server, unsigned char frame_type, int data1, unsigned char data2)
{
    server->buffer[0] = frame_type;
    memcpy(server->buffer + 1, &data1, 4);
    server->buffer[5] = data2;
    // TCP_SOCKET_send(server, server -> buffer, 6);
}

ssize_t receive_single_frame(TCP_SOCKET *sock, unsigned char *buf, __int32_t buf_size)
{
    return recv(sock->client, buf, buf_size, 0);
}
