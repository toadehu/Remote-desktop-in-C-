#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef _WIN32
#include <winsock2.h>
#include <WS2tcpip.h>
#define socklen_t int

/*The inconsistencies are so fun*/
typedef int32_t __int32_t;
typedef uint64_t __uint64_t;
/*typedef int ssize_t;*/

#else
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define SOCKET int
#define closesocket close
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#endif

#include "buffer_receiver.h"

const int buffer_default_size = 1024 * 32;    /* 32 KiB */
const int buffer_big_size     = 1024 * 1024; /* 1 MiB  */
const int buffer_small_size   = 1024;       /* 1 KiB  */

#define ALLOW_REALLOC 1
#define ALLOW_DROP 2 /*This flag allows the socket to drop extra data that does not fit in the buffer*/
#define CLIENT 4
#define BIG_BUFFER 8 /* This flag makes the socket's buffer large to a size of 1MB */
#define SMALL_BUFFER 16 /* This flag makes the socket's buffer small, just 1KB*/

#define IP4 0
#define IP6 1
#define NO_BLOCKING 2 /*the call to connect() is non blocking*/

/* An arbitrary value for the MSS (Maximum Segment Size)*/
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

/**
 * Structure representing a TCP socket.
 */
typedef struct TCP_Socket
{
    SOCKET tcp_socket;          /* Socket file descriptor*/
    struct sockaddr_in server;  /* Server address structure*/
    SOCKET *client;             /* Clients' sockets*/
    int client_num;             /* Number of clients*/
    int port;                   /* Port number*/
    char* buffer;               /* Buffer for data*/
    __int32_t buffer_len;       /* Length of the buffer*/
    receiver_t *receiver;       /* Receiver structure for buffering received data*/
    fd_set master_fds, read_fds;/* File descriptor set for select(), and a master file descriptor */
} TCP_SOCKET;

/**
 * Creates a TCP socket and initializes the server structure.
 *
 * @param port The port number to bind the socket.
 * @param ip_listen The IP address to bind to the socket (if 0 then the socket listens for everyone).
 * @param start_listen Flag indicating whether to start listening immediately.
 * @param max_connections Flag indicating how many connections to accept. The function is blocking untill all of the connections have been accepted
 * @return A pointer to the created TCP_SOCKET structure.
 */
TCP_SOCKET *TCP_SOCKET_create(int port, int ip_listen, bool start_listen, int max_connections, int flags)
{
    TCP_SOCKET *server = (TCP_SOCKET *)malloc(sizeof(TCP_SOCKET));

    if (server == NULL)
    {
        printf("Error allocating memory for the server\n");
        return NULL;
    }

    /* initialize winsock for Windows*/
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Error initializing winsock, you should reconsider what you are doing\n");
        return NULL;
    }
#endif

    /* Creating the socket file descriptor*/
    server->port = port;
    server->tcp_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (flags & BIG_BUFFER)
    {
        server->buffer = (char*)malloc(buffer_big_size);
        server->buffer_len = buffer_big_size;
    }
    else if (flags & SMALL_BUFFER)
    {
        server->buffer = (char*)malloc(buffer_small_size);
        server->buffer_len = buffer_small_size;
    }
    else
    {
        server->buffer = (char*)malloc(buffer_default_size);
        server->buffer_len = buffer_default_size;
    }

    if (server->tcp_socket == -1)
    {
        printf("Error creating socket, you should reconsider what you are doing\n");
        free(server);
        return NULL;
    }

    //if it's a client socket, it doesn't need any of the other stuff
    if (flags & CLIENT)
    {
        return server;
    }

    server->receiver = NULL;

    /* Binding the socket to the port and to the addresses*/
    memset(&server->server, 0, sizeof(server->server));
    server->server.sin_family = AF_INET;
    server->server.sin_addr.s_addr = htonl(ip_listen);
    server->server.sin_port = htons(port);

    if (bind(server->tcp_socket, (struct sockaddr *)&server->server, sizeof(server->server)) < 0)
    {
        printf("Error binding socket");
        free(server);
        return NULL;
    }

    if ((start_listen == true))
    {
        server -> client = (SOCKET*)malloc(max_connections * sizeof(SOCKET));
        if (server->client == NULL)
        {
            printf("Error allocating memory for the client array\n");
            free(server);
            return NULL;
        }
        server -> client_num = max_connections;
        if (listen(server->tcp_socket, 3) != 0)
        {
            printf("Error listening to socket");
            free(server);
            return NULL;
        }

        int i;
        for (i = 0; i < max_connections; i++)
        {
            /* Accept a new connection*/
            struct sockaddr_in client_addr;
            socklen_t addr_len = sizeof(client_addr);
            int client_socket = accept(server->tcp_socket, (struct sockaddr *)&client_addr, &addr_len);

        #ifdef _DEBUG
            struct sockaddr_in clientAddr;
            socklen_t clientAddrLen = sizeof(clientAddr);

            // Retrieve the client's address information
            if (getpeername(client_socket, (struct sockaddr *)&clientAddr, &clientAddrLen) == -1) {
                perror("getpeername");
                return NULL;
            }

            // Convert IP address from network byte order to presentation format
            char clientIP[INET_ADDRSTRLEN];
            if (inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN) == NULL) {
                perror("inet_ntop");
                return NULL;
            }

            // Print the endpoint information
            printf("Accepted connection from: %s:%d\n", clientIP, ntohs(clientAddr.sin_port));
        #endif

            if (client_socket < 0)
            {
                printf("Error accepting connection number: %d\n", i);
                return NULL;
            }
            printf("Received a connection from someone\n");
            /*store the client*/
            server -> client[i] = client_socket;
        }
    }
    return server;
}

/**
 * @brief Accepts a new connection on the given server socket, and client_id
 * 
 * @param server a pointer to the TCP_SOCKET that accepts the connection
 * @param client_id the id of the client that will be overwritten with the new client
 * @return 0 if the connection was accepted, 1 otherwise
*/
bool TCP_Socket_accept_new_connection(TCP_SOCKET* server, int client_id)
{
    if (server -> client_num <= client_id)
    {
        return 1;
    }
    /* Accept a new connection*/
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_socket = accept(server->tcp_socket, (struct sockaddr *)&client_addr, &addr_len);
    if (client_socket < 0)
    {
        printf("Error accepting connection number: %d\n", client_id);
        return 1;
    }
    /*store the client*/
    server -> client[client_id] = client_socket;
    return 0;
}

/**
 * Connects a TCP_SOCKET to a given remote host
 *
 * @param sock, pointer to the struct TCP_Socket that is connected to the server 
 * @param ipaddr, the ip address of the remote host
 * @param port, the port of the remote server 
 * @param flags if last bit set then IP6 is used, otherwise it is IP4, there are also other flags
 * @return the return value of the call to connect(), or -1 upon failure 
*/
int TCP_SOCKET_connect_from_string(struct TCP_Socket* sock, char* ipaddr, int port, uint32_t flags)
{
    if (!(flags & NO_BLOCKING))
    {
        struct sockaddr_in server;
        struct sockaddr_in server6;
        int con;

        /* It's ip4 */
        if (!(flags & IP6))
        {
            /* Checking if the conversion of the address is successful */
            if (inet_pton(AF_INET, ipaddr, &(server.sin_addr)) == 1)
            {
                server.sin_family = AF_INET;
                server.sin_port = htons(port);

                con = connect(sock->tcp_socket, (struct sockaddr*)(&server), sizeof(server));
            }
            // inet_pton failed
            else
            {
                return -1;
            }
        }
        /* It's ip6 */
        else
        {
            /* Checking if the conversion of the address is successful */
            if (inet_pton(AF_INET6, ipaddr, &(server6.sin_addr)) == 1)
            {
                server6.sin_family = AF_INET6;
                server6.sin_port = htons(port);

                con = connect(sock->tcp_socket, (struct sockaddr*)(&server6), sizeof(server6));
            }
            /* inet_pton failed */
            else
            {
                return -1;
            }
        }
        if (con)
        {
            return -1; /* Connect failed */
        }
        else
        {
            return con; /* Return the return value of the connect() function */
        }
    }

    return 0;
}

/**
 * Sends a big frame over TCP by breaking it into multiple packets.
 *
 * @param sock The TCP socket.
 * @param frame The frame data to send.
 * @param frame_size The size of the frame data.
 * @param header The header value for the frame.
 */
void TCP_Send_Big_Frame(TCP_SOCKET *sock, __int32_t client_id, char *frame, int frame_size, HDR_TYPE header)
{
    memcpy(sock->buffer, &header, sizeof(HDR_TYPE));
    memcpy(sock->buffer + 8, frame, 248);
    send(sock->client[client_id], (const char*)sock->buffer, 256, 0);
    int loop_end = (frame_size - 248) / 256;

    int i;
    for (i = 0; i < loop_end; i += 1)
    {
        send(sock->client[client_id], frame + 248 + i * 256, 256, 0);
    }
    if ((frame_size - 248) & 255)
    {
        send(sock->client[client_id], frame + 248 + frame_size * 256, (frame_size - 248) & 255, 0);
    }
}


/**
 * Sends data on the given socket to the given client or the server, the buffer should contain all necessary information
 * 
 * @param sock a pointer to the TCP_SOCKET that sends the data
 * @param client the client to send the data to, if -1 the data will be sent to the server
 * @param buffer the buffer containing the data to send
 * @param size the size of the buffer
*/
int TCP_Socket_send_data(TCP_SOCKET *sock, int client, char* buffer, __int32_t size)
{
    SOCKET client_socket;
    if (client == -1)
    {
        client_socket = sock->tcp_socket;
    }
    else
    {
        /* Sanity check for invalid client */
        if (client >= sock->client_num)
        {
            return -1;
        }
        client_socket = sock->client[client];
    }
	int total_sent = 0;
	while (total_sent < size)
	{
		int sent = 0;
		sent = send(client_socket, buffer + total_sent, size - total_sent, 0);
        #ifdef _DEBUG
        printf("Sent %d bytes\n", sent);
        #endif
		if (sent == -1)
		{
			return -1;
		}
		total_sent += sent;
	}
	return total_sent;
}

#define NO_REALLOC 1
#define NO_DROP 2 /* If NO_REALLOC is set, the the function call will stop receiving when the data it may receive will be dropped*/

/**
 * Receive data on the given socket, the data will be received continously untill the stream is empty
 * @param sock a pointer to the TCP_SOCKET that receives the data
 * @param buffer the buffer where the data will be written to
 * @param size a pointer to the variable storing the size of the buffer
 * @return on succes the number of bytes received, -1 on failure of the recv function call or the realloc call, other negative value if NO_REALLOC is set and there is no room in the buffer (the ammount of bytes that could not be received if it is -1 it's unlucky)
 */
int TCP_Socket_receive_data_stream(TCP_SOCKET *sock, char** buffer, __int32_t* size, int32_t flags)
{
    int total_rec = 0;
    while (1)
    {
        int rec;
        rec = recv(sock -> tcp_socket, (char*)sock -> buffer, (int)sock -> buffer_len, 0);
        if (rec == 0)
        {
            return total_rec;
        }
        if (rec < 0)
        {
            return INT32_MIN;
        }
        if (total_rec + rec >= *size)
        {
            if (flags & NO_REALLOC)
            {
                (void)memcpy(*buffer + total_rec, sock -> buffer, *size - total_rec);
                return total_rec + rec - *size;
            }
            else
            {
                *buffer = (char*)realloc(*buffer, 2 * (*size));
                *size *= 2;
            }
        }
        (void)memcpy(*buffer + total_rec, sock -> buffer, rec);
        total_rec += rec;
    }
    return total_rec;
}

/**
 * Receive data on the given client socket
 * 
 * @param sock a pointer to the TCP_SOCKET that receives the data
 * @param client the client to receive the data from, -1 if the data will be received from the server
 * @param buffer the buffer where the data will be written to
 * @param size the number of bytes to receive
*/
int TCP_Socket_receive_data(TCP_SOCKET *sock, int client, char* buffer, __int32_t size)
{
    SOCKET client_socket;
    if (client == -1)
    {
        client_socket = sock->tcp_socket;
    }
    else
    {
        /* Sanity check for invalid client */
        if (client >= sock->client_num)
        {
            return -1;
        }
        client_socket = sock->client[client];
    }
    int total_rec = 0;
    while (total_rec < size)
    {
        int rec;
        rec = recv(client_socket, buffer + total_rec, size - total_rec, 0);
        if (rec == 0)
        {
            return total_rec;
        }
        if (rec < 0)
        {
            return -1;
        }
        total_rec += rec;
    }
    return total_rec;
}

/**
 * Attempt to receive data on the given socket, the data will be received once and up to size bytes
 * 
 * @param sock a pointer to the TCP_SOCKET that receives the data
 * @param client the client to receive the data from, -1 if the data will be received from the server
 * @param buffer the buffer where the data will be written to
 * @param size the number of bytes to receive 
 */
int TCP_Socket_receive_data_once(TCP_SOCKET* sock, int client, char* buffer, __int32_t size)
{
    SOCKET client_socket;
    if (client == -1)
    {
        client_socket = sock->tcp_socket;
    }
    else
    {
        /* Sanity check for invalid client */
        if (client >= sock->client_num)
        {
            return -1;
        }
        client_socket = sock->client[client];
    }
    return recv(client_socket, buffer, size, 0);
}

/**
 * Receive data on the given socket, the data will be received up to size bytes
 * 
 * @param sock a pointer to the TCP_SOCKET that receives the data
 * @param buffer the buffer where the data will be written to
 * @param size the number of bytes to receive
 * @return on succes the number of bytes received, -1 on failure of the recv function call or the realloc call
 */
int TCP_Socket_receive_data_fixed(TCP_SOCKET *sock, char* buffer, __int32_t size)
{
    return TCP_Socket_receive_data(sock, -1, buffer, size);
    /*
    int total_rec = 0;
    while (total_rec < size)
    {
        int rec;
        rec = recv(sock -> tcp_socket, buffer + total_rec, size - total_rec, 0);
        if (rec == 0)
        {
            return total_rec;
        }
        if (rec < 0)
        {
            return -1;
        }
        total_rec += rec;
    }
    return total_rec;
    */
}

void zero_file_descriptor_set(TCP_SOCKET *sock)
{
    FD_ZERO(&sock->master_fds);
    FD_ZERO(&sock->read_fds);
}

void set_file_descriptor_set(TCP_SOCKET *sock)
{
    FD_SET(sock->tcp_socket, &sock->master_fds);
    sock->read_fds = sock->master_fds;
}

void allow_select(TCP_SOCKET *sock)
{
    zero_file_descriptor_set(sock);
    set_file_descriptor_set(sock);
}

/*
 * @brief Finds the socket that has data to be read or if the socket has a client connecting to it
 * 
 * @param sock a pointer to the TCP_SOCKET that will be checked
 * @param microseconds the timeout interval in microseconds, if it is 0, find_hot_socket() will be called with a 1 milisecond timeout
 * 
 * @return the index of the socket that has data to be read, -1 if no socket has data to be read, -2 for any error, INT32_MAX if a new client has connected
*/
int find_hot_socket_with_timeout(TCP_SOCKET *sock, int32_t microseconds)
{
    if (microseconds == 0)
    {
        microseconds = 1000;
    }
    if  (sock == NULL)
    {
        printf("Wtf romenia %p\n\n\n", sock);
        return -2;
    }

    allow_select(sock);
    
    sock->read_fds = sock->master_fds;

    int max_fd = sock->tcp_socket;
    for (int i = 0; i < sock->client_num; i+=1)
    {
        FD_SET(sock->client[i], &sock->read_fds);
        if (sock->client[i] > max_fd)
        {
            max_fd = sock->client[i];
        }
    }

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = microseconds; /* the given timeout*/

    int activity = select(max_fd + 1, &sock->read_fds, NULL, NULL, &timeout);

    /* Something went wrong */
    if (activity == SOCKET_ERROR)
    {
        printf("Wtf italia %d\n\n\n", activity);
        return -2;
    }
    /* Nothing is happening */
    else if (activity == 0)
    {
        printf("Wtf germania %d\n\n\n", activity);
        return -1;
    }

    /* A new client has connected */
    if (FD_ISSET(sock->tcp_socket, &sock->read_fds))
    {
        return INT32_MAX;
    }

    int i;
    for (i = 0; i < sock->client_num; i+=1)
    {
        if (FD_ISSET(sock->client[i], &sock->read_fds))
        {
            return i;
        }
    }

    /* Unexpected behaviour */
    return -2;
}

