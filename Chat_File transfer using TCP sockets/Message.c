#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Message.h"
#include "request_type.h"

Message* Message_create(char* dst_name, char* msg)
{
    Message* result = (Message*)malloc(sizeof(Message));
    
    result->dst_name = (char*)malloc(strlen(dst_name) + 1);
    result->msg = (char*)malloc(strlen(msg) + 1);
    
    strcpy(result->dst_name, dst_name);
    strcpy(result->msg, msg);
    
    result->next = NULL;
    
    return result;
}

void Message_send(Message* msg, char* src_client_name, char* ip, int port)
{
    int peer_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
 
    if (peer_socket_fd < 0) 
        error("ERROR opening socket");
    
    struct sockaddr_in serv_addr;
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_aton(ip, &serv_addr.sin_addr);
    
    if (connect(peer_socket_fd,(struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0)
    {
        printf("\nERROR connecting to message peer !");   
	printf("\nclient# ");
	fflush(stdout);
    }
    
    unsigned int data_len = 3 + strlen(src_client_name) + 1 + strlen(msg->msg) + 1;
    char* data = (char*)malloc(data_len);
    data[0] = (unsigned char) (data_len / 256);
    data[1] = (unsigned char) (data_len % 256);
    data[2] = REQUEST_TYPE_MESSAGE;
    strcpy(&data[3], src_client_name);
    
    strcpy(&data[3 + 1 + strlen(src_client_name)], msg->msg);
    
    send(peer_socket_fd, data, data_len, 0);
    
    free(data);
    
    close(peer_socket_fd);
}

void Message_destroy(Message* message)
{
    free(message->dst_name);
    free(message->msg);
    
    free(message);
} 
