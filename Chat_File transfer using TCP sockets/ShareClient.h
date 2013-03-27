#ifndef SHARECLIENT_H
#define SHARECLIENT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <time.h>

#define MAX_NAME_LENGTH 255

#define STATE_WAIT_INFO 1
#define STATE_CONNECTED 2
#define STATE_DISCONNECTED 3

struct _ShareClientList;
struct _SharedFileLinkedList;
//typedef struct _ShareClientList ShareClientList;

typedef struct _ShareClient
{
    int socket_fd;
    char ip[INET_ADDRSTRLEN + 1];
    unsigned int port;
    time_t connection_time;
    char name[MAX_NAME_LENGTH + 1];
    struct _SharedFileLinkedList* shared_file_list;
    struct _ShareClient* next;
    int state;
    char data[2048];
    int received_bytes;
} ShareClient;

ShareClient* ShareClient_create(int socket_fd, struct sockaddr_in* cli_addr);

void ShareClient_read_data(struct _ShareClient* shareClient);

void ShareClient_send_response(struct _ShareClient* shareClient, unsigned char response);

//2 byti lungimea cererii, 1 byte tipul cererii, restul este numele fisierului terminat cu '\0'
int ShareClient_share_file(struct _ShareClient* shareClient, unsigned int complete_data_size);

//2 byti lungimea cererii, 1 byte tipul cererii, restul este numele fisierului terminat cu '\0'
int ShareClient_unshare_file(struct _ShareClient* shareClient, unsigned int complete_data_size);

void ShareClient_handle_data(struct _ShareClient* shareClient, struct _ShareClientList* client_list);

#endif
