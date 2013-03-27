#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <time.h>

#include "ShareClient.h"
#include "ShareClientList.h"
#include "SharedFileLinkedList.h"
#include "request_type.h"
#include "response.h"

#define MAX_CLIENTS 64
#define BUFLEN 256
#define STDIN_FILENO 0


void error(char *msg)
{
    perror(msg);
    exit(1);
}




//returneaza un file descriptor corespunzator socketului pe care se asculta conexiuni
int start_server(int portno)
{   
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socket_fd < 0)
      error("ERROR opening socket");
    
    struct sockaddr_in serv_addr;
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
  
    if(bind(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	  error("ERROR on binding");
    
    listen(socket_fd, MAX_CLIENTS);
    
    return socket_fd;
}

void handle_stdin_data(ShareClientList* client_list)
{
    //printf("\nHandling stdin data");
    //fflush(stdout);
    
    char buffer[BUFLEN];
  
    memset(buffer, 0, BUFLEN);
    
    fgets(buffer, BUFLEN - 1, stdin);
    buffer[strlen(buffer) - 1] = 0;
    
    if(strcasecmp(buffer, "quit") == 0)
	exit(0);
    else if(strcasecmp(buffer, "status") == 0)
    {
	printf("\nConnected users:");
	ShareClientList_print(client_list);
    }
    else
    {
	printf("Unknown command '%s'", buffer);
	printf("\nAvailable commands:\nquit - stops the share server\nstatus - display the connected users");
	fflush(stdout);
    }
    
    printf("\nserver# ");fflush(stdout);
}

void handle_new_connection(ShareClientList* client_list, int server_socket_fd, fd_set* read_fds)
{
    //printf("\nHandling a new connection");fflush(stdout);
  
    struct sockaddr_in cli_addr;
    int clilen = sizeof(cli_addr);
    
    int client_socket_fd;
    
    if((client_socket_fd = accept(server_socket_fd, (struct sockaddr *)&cli_addr, &clilen)) == -1)
    {
	    error("ERROR in accept");
    }
    
    ShareClient* shareClient = ShareClient_create(client_socket_fd, &cli_addr);
    
    ShareClientList_add(client_list, shareClient);
    
    FD_SET(shareClient->socket_fd, read_fds);
}

void check_clients(ShareClientList* client_list, fd_set* tmp_fds)
{
    ShareClient* shareClient = client_list->first;
    
    if(shareClient == NULL)
	return;
    
    while(shareClient != NULL)
    {
	if(FD_ISSET(shareClient->socket_fd, tmp_fds))
	{
	    ShareClient_read_data(shareClient);
	    ShareClient_handle_data(shareClient, client_list);
	}
	
	shareClient = shareClient->next;
    }
}

void remove_disconnected_clients(ShareClientList* client_list, fd_set* read_fds)
{
    //sterge din lista clientii deconectati
    ShareClient* shareClient = client_list->first;
    
    if(shareClient == NULL)
	return;
    
    if(shareClient->state == STATE_DISCONNECTED)
    {
	client_list->first = shareClient->next;
	
	SharedFileLinkedList_destroy(shareClient->shared_file_list);
	close(shareClient->socket_fd);
	FD_CLR(shareClient->socket_fd, read_fds);
	
	free(shareClient);
	
	return;
    }
    
    while(shareClient->next != NULL)
    {
	if(shareClient->next->state == STATE_DISCONNECTED)
	{
	    ShareClient* tmp = shareClient->next;
	    
	    shareClient->next = shareClient->next->next;
	    
	    SharedFileLinkedList_destroy(tmp->shared_file_list);
	    close(tmp->socket_fd);
	    FD_CLR(tmp->socket_fd, read_fds);
	    
	    free(tmp);
	}
	else
	    shareClient = shareClient->next;
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2) 
    {
         fprintf(stderr,"Usage: %s port\n", argv[0]);
         exit(1);
    }    

    int portno = atoi(argv[1]);
    
    int server_socket_fd = start_server(portno);
    
    fd_set read_fds;
    
    FD_ZERO(&read_fds);
    FD_SET(server_socket_fd, &read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    
    ShareClientList* client_list = ShareClientList_create();
    
    printf("\nserver# ");fflush(stdout);
    
    while(1)
    {
	fd_set tmp_fds = read_fds;
	
	if(select(FD_SETSIZE, &tmp_fds, NULL, NULL, NULL) == -1)
	    error("ERROR in select");
	
	if(FD_ISSET(STDIN_FILENO, &tmp_fds))
	    handle_stdin_data(client_list);
	
	if(FD_ISSET(server_socket_fd, &tmp_fds))
	    handle_new_connection(client_list, server_socket_fd, &read_fds);
	
	check_clients(client_list, &tmp_fds);
	remove_disconnected_clients(client_list, &read_fds);
    }
    
    return 0;
}