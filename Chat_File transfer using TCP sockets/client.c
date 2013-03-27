#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <time.h>

#include "request_type.h"
#include "response.h"
#include "Message.h"
#include "MessageList.h"

#define MAX_CLIENTS 64
#define BUFLEN 1024
#define STDIN_FILENO 0

#define DEFAULT_PORT 47112

char server_data[0xFFFF];
int server_data_received_bytes = 0;

void error(char *msg)
{
    perror(msg);
    exit(1);
}

void send_request(int client_socket_fd, int request_type)
{
    unsigned int data_len = 3;
    char data[3];
    
    data[0] = (unsigned char) (data_len / 256);
    data[1] = (unsigned char) (data_len % 256);
    data[2] = request_type;
    
    send(client_socket_fd, data, data_len, 0);
}

#define PEER_STATE_CONNECTED 0
#define PEER_STATE_DISCONNECTED 1
#define PEER_STATE_RECEIVING_DATA 2

typedef struct _Peer
{
    int socket_fd;
    int state;
    char data[2048];
    int received_bytes;
    FILE* output_file;
    FILE* input_file;
    struct _Peer* next;
}Peer;

typedef struct _PeerList
{
    Peer* first;
}PeerList;

PeerList* PeerList_create()
{
    PeerList* result = (PeerList*)malloc(sizeof(PeerList));
    
    result->first = NULL;
    
    return result;
}

void PeerList_add(PeerList* list, Peer* peer)
{
    peer->next = NULL;

    if(list->first == NULL)
    {
	list->first = peer;
	
	return;
    }
    
    Peer* last = list->first;
    
    while(last->next != NULL)
	last = last->next;
    
    last->next = peer;
}

Peer* Peer_create(int socket_fd)
{
    Peer* peer = (Peer*)malloc(sizeof(Peer));
    
    peer->next = NULL;
    peer->socket_fd = socket_fd;
    peer->state = PEER_STATE_CONNECTED;
    peer->received_bytes = 0;
    peer->output_file = NULL;
    peer->input_file = NULL;
    
    return peer;
}

Peer_destroy(Peer* peer)
{
    if(peer->output_file != NULL)
	fclose(peer->output_file);
    
    if(peer->input_file != NULL)
	fclose(peer->input_file);
    
    free(peer);
}

void Peer_read_data(Peer* peer)
{
    int n = recv(peer->socket_fd, &peer->data[peer->received_bytes], sizeof(peer->data) - peer->received_bytes, 0);

    if(n == 0)
    {
	//conexiunea s-a inchis
	peer->state = PEER_STATE_DISCONNECTED;

	return;
    }
    
    if(n < 0)
    {
	error("ERROR in recv");
	return;
    }
    
    peer->received_bytes += n;
}


void Peer_handle_data(Peer* peer, PeerList* peer_list)
{
    if(peer->received_bytes < 3)
	return;
    
    unsigned int complete_data_size = ((unsigned int)peer->data[0]) * 256 + peer->data[1];
    
    if(peer->received_bytes < complete_data_size) //verifica daca s-a primit toata cererea
	return;
    
    int request_type = peer->data[2];
    
    char* pmsg;
    
    switch(request_type)
    {
      case REQUEST_TYPE_MESSAGE:
	    pmsg = &peer->data[3];
	    
	    while(*pmsg)
	      pmsg++;
	    
	    pmsg++;
	    
	    printf("\n[%s] %s", &peer->data[3], pmsg);
	    
	    printf("\nclient# ");
	    fflush(stdout);
      break;
      
      case REQUEST_TYPE_FILE_WRITE_DATA:
	    fwrite(&peer->data[3], 1, complete_data_size - 3, peer->output_file);
      break;
      
      case REQUEST_TYPE_EOF:
	    printf("\nFile transfer complete !");
	    printf("\nclient# ");
	    fflush(stdout);
	    fclose(peer->output_file);
	    peer->output_file = NULL;
	    peer->state = PEER_STATE_DISCONNECTED;
      break;
      
      case REQUEST_TYPE_START_TRANSFER:
	    peer->input_file = fopen(&peer->data[3], "rb");
	    peer->state = PEER_STATE_RECEIVING_DATA;
	    
	    if(peer->input_file == NULL)
	    {
		printf("\nFailed to open file '%s' for reading !", &peer->data[3]);
		printf("\nclient# ");
		fflush(stdout);
		
		send_request(peer->socket_fd, REQUEST_TYPE_ABORT_UNABLE_TO_OPEN_FILE);
		  
		peer->state = PEER_STATE_DISCONNECTED;
	    }
      break;
      
      case REQUEST_TYPE_ABORT_UNABLE_TO_OPEN_FILE:
	    printf("\nFile transfer aborted - unable to open file !");
	    printf("\nclient# ");
	    fflush(stdout);
	    fclose(peer->output_file);
	    peer->output_file = NULL;
	    peer->state = PEER_STATE_DISCONNECTED;
      break;
      
      default:;
    }
    
    int i;
    for(i = 0; i < peer->received_bytes - complete_data_size; i++)
	peer->data[i] = peer->data[complete_data_size + i];
      
    peer->received_bytes -= complete_data_size;
}

typedef struct _FileTransfer
{
    char* client_name;
    char* filename;
    struct _FileTransfer* next;
}FileTransfer;

FileTransfer* FileTransfer_create(char* client_name, char* filename)
{
    FileTransfer* result = (FileTransfer*)malloc(sizeof(FileTransfer));
    
    result->next = NULL;
    result->client_name = (char*)malloc(strlen(client_name) + 1);
    result->filename = (char*)malloc(strlen(filename) + 1);
    
    strcpy(result->client_name, client_name);
    strcpy(result->filename, filename);
    
    return result;
}

void FileTransfer_destroy(FileTransfer* file_transfer)
{
    free(file_transfer->client_name);
    free(file_transfer->filename);
    free(file_transfer);
}

typedef struct _FileTransferList
{
    FileTransfer* first;
}FileTransferList;

FileTransferList* FileTransferList_create()
{
    FileTransferList* result = (FileTransferList*)malloc(sizeof(FileTransferList));
    
    result->first = NULL;
    
    return result;
}

void FileTransferList_add(FileTransferList* list, FileTransfer* ft)
{
    ft->next = NULL;

    if(list->first == NULL)
    {
	list->first = ft;
	
	return;
    }
    
    FileTransfer* last = list->first;
    
    while(last->next != NULL)
	last = last->next;
    
    last->next = ft;
}

void FileTransferList_remove_file_transfers(FileTransferList* list, char* client_name)
{
    FileTransfer* ft = list->first;
    
    if(ft == NULL)
	return;
    
    while(strcmp(ft->client_name, client_name) == 0)
    {
	list->first = ft->next;
	
	FileTransfer_destroy(ft);
	
	ft = list->first;
	
	if(ft == NULL)
	    return;
    }
    
    while(ft->next != NULL)
    {
	if(strcmp(ft->next->client_name, client_name) == 0)
	{
	    FileTransfer* tmp = ft->next;
	    
	    ft->next = ft->next->next;
	    
	    FileTransfer_destroy(tmp);
	}
	else
	    ft = ft->next;
    }
}

void FileTransfer_start(FileTransfer* ft, char* ip, int port, fd_set* read_fds, PeerList* peer_list)
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
    
    unsigned int data_len = 3 + strlen(ft->filename) + 1;
    char* data = (char*)malloc(data_len);
    data[0] = (unsigned char) (data_len / 256);
    data[1] = (unsigned char) (data_len % 256);
    data[2] = REQUEST_TYPE_START_TRANSFER;
    strcpy(&data[3], ft->filename);
    
    send(peer_socket_fd, data, data_len, 0);
    
    free(data);
    
    Peer* peer = Peer_create(peer_socket_fd);
    
    char* local_fname = (char*)malloc(strlen(ft->filename) + 1 + strlen("_primit"));
    strcpy(local_fname, ft->filename);
    strcat(local_fname, "_primit");
    
    peer->output_file = fopen(local_fname, "wb");
    
    free(local_fname);
      
    PeerList_add(peer_list, peer);
    
    FD_SET(peer->socket_fd, read_fds);
}

void FileTransferList_start_transfers(FileTransferList* list, char* client_name, char* ip, int port, fd_set* read_fds, PeerList* peer_list)
{
    FileTransfer* ft = list->first;
    
    if(ft == NULL)
	return;
    
    while(strcmp(ft->client_name, client_name) == 0)
    {
	list->first = ft->next;
	
	FileTransfer_start(ft, ip, port, read_fds, peer_list);
	FileTransfer_destroy(ft);
	
	ft = list->first;
	
	if(ft == NULL)
	    return;
    }
    
    while(ft->next != NULL)
    {
	if(strcmp(ft->next->client_name, client_name) == 0)
	{
	    FileTransfer* tmp = ft->next;
	    
	    ft->next = ft->next->next;
	    
	    FileTransfer_start(tmp, ip, port, read_fds, peer_list);
	    FileTransfer_destroy(tmp);
	}
	else
	    ft = ft->next;
    }
}

void send_request_with_param(int client_socket_fd, int request_type, char* param)
{
    unsigned int data_len = 3 + strlen(param) + 1;
    char* data = (char*)malloc(data_len);
    
    data[0] = (unsigned char) (data_len / 256);
    data[1] = (unsigned char) (data_len % 256);
    data[2] = request_type;
    
    strcpy(&data[3], param);
    data[data_len - 1] = 0;
    
    send(client_socket_fd, data, data_len, 0);
    
    free(data);
}

void send_list_clients_request(int client_socket_fd)
{
    send_request(client_socket_fd, REQUEST_TYPE_LISTCLIENTS);
}

void send_infoclient_request(int client_socket_fd, char* client_name)
{
    send_request_with_param(client_socket_fd, REQUEST_TYPE_INFOCLIENT, client_name);
}

void send_share_request(int client_socket_fd, char* filename)
{
    send_request_with_param(client_socket_fd, REQUEST_TYPE_SHARE, filename);
}

void send_unshare_request(int client_socket_fd, char* filename)
{
    send_request_with_param(client_socket_fd, REQUEST_TYPE_UNSHARE, filename);
}

void send_getshare_request(int client_socket_fd, char* filename)
{
    send_request_with_param(client_socket_fd, REQUEST_TYPE_GETSHARE, filename);
}


void queue_message(int client_socket_fd, char* buffer, MessageList* message_list)
{
    unsigned int tmp_len = strlen(buffer + 1);
    char* tmp = (char*)malloc(tmp_len);
    strcpy(tmp, buffer);
    
    int i = 0;
    while(tmp[i] == ' ' && i < tmp_len)
      i++;
    
    while(tmp[i] != ' ' && i < tmp_len)
      i++;
    
    if(i == tmp_len)
    {
	printf("\nInvalid input format !");
	free(tmp);
	return;
    }
    
    tmp[i] = 0;
    
    char* dst_name = tmp;
    char* msg = &tmp[i + 1];
    
    Message* message = Message_create(dst_name, msg);
    MessageList_add(message_list, message);
    
    send_request_with_param(client_socket_fd, REQUEST_TYPE_INFOCLIENT_SILENT, dst_name);
    
    free(tmp);
}

void queue_file_transfer(int client_socket_fd, char* buffer, FileTransferList* file_transfer_list)
{
    unsigned int tmp_len = strlen(buffer + 1);
    char* tmp = (char*)malloc(tmp_len);
    strcpy(tmp, buffer);
    
    int i = 0;
    while(tmp[i] == ' ' && i < tmp_len)
      i++;
    
    while(tmp[i] != ' ' && i < tmp_len)
      i++;
    
    if(i == tmp_len)
    {
	printf("\nInvalid input format !");
	free(tmp);
	return;
    }
    
    tmp[i] = 0;
    
    char* client_name = tmp;
    char* filename = &tmp[i + 1];
    
    FileTransfer* ft = FileTransfer_create(client_name, filename);
    FileTransferList_add(file_transfer_list, ft);
    
    send_request_with_param(client_socket_fd, REQUEST_TYPE_INFOCLIENT_SILENT, client_name);
    
    free(tmp);
}

void handle_stdin_data(int client_socket_fd, int local_server_socket_fd, MessageList* message_list, FileTransferList* file_transfer_list)
{
    //printf("\nHandling stdin data");
    //fflush(stdout);
    
    char buffer[BUFLEN];
  
    memset(buffer, 0, BUFLEN);
    
    fgets(buffer, BUFLEN - 1, stdin);
    buffer[strlen(buffer) - 1] = 0;
    
    if(strcasecmp(buffer, "quit") == 0)
    {
	send_request(client_socket_fd, REQUEST_TYPE_DISCONNECT);
	close(client_socket_fd);
	close(local_server_socket_fd);
	exit(0);
    }
    else if(strcasecmp(buffer, "listclients") == 0)
    {
	send_list_clients_request(client_socket_fd);
    }
    else if(strncasecmp(buffer, "infoclient ", strlen("infoclient ")) == 0)
    {
	printf("\nRequesting information about %s", &buffer[strlen("infoclient ")]);
	send_infoclient_request(client_socket_fd, &buffer[strlen("infoclient ")]);
	//send_request(client_socket_fd, REQUEST_TYPE_LIST_CLIENTS);
    }
    else if(strncasecmp(buffer, "message ", strlen("message ")) == 0)
    {
	queue_message(client_socket_fd, &buffer[strlen("message ")], message_list);
	
	//send_request(client_socket_fd, REQUEST_TYPE_LIST_CLIENTS);
    }
    else if(strncasecmp(buffer, "sharefile ", strlen("sharefile ")) == 0)
    {
	printf("\nSharing file %s", &buffer[strlen("sharefile ")]);
	send_share_request(client_socket_fd, &buffer[strlen("sharefile ")]);
    }
    else if(strncasecmp(buffer, "unsharefile ", strlen("unsharefile ")) == 0)
    {
	printf("\nUnsharing file %s", &buffer[strlen("unsharefile ")]);
	send_unshare_request(client_socket_fd, &buffer[strlen("unsharefile ")]);
    }
    else if(strncasecmp(buffer, "getshare ", strlen("getshare ")) == 0)
    {
	printf("\nGetting shared files from %s", &buffer[strlen("getshare ")]);
	send_getshare_request(client_socket_fd, &buffer[strlen("getshare ")]);
    }
    else if(strncasecmp(buffer, "getfile ", strlen("getfile ")) == 0)
    {
	queue_file_transfer(client_socket_fd, &buffer[strlen("getfile ")], file_transfer_list);
    }
    else
    {
	printf("Unknown command '%s'", buffer);
	printf("\nAvailable commands:\nquit\nlistclients\ninfoclient nume_client\nmessage nume_client mesaj\nsharefile nume_fisier\nunsharefile nume_fisier\ngetshare nume_client\ngetfile nume_client nume_fisier");
	fflush(stdout);
    }
    
    printf("\nclient# ");fflush(stdout);
}

int connect_to_server(char* client_name, char* server_ip, int port, int local_port)
{
    int client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
 
    if (client_socket_fd < 0) 
        error("ERROR opening socket");
    
    struct sockaddr_in serv_addr;
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_aton(server_ip, &serv_addr.sin_addr);
    
    if (connect(client_socket_fd,(struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");    
    
    unsigned int data_len = 3 + 2 + strlen(client_name) + 1;
    char* data = (char*)malloc(data_len);
    data[0] = (unsigned char) (data_len / 256);
    data[1] = (unsigned char) (data_len % 256);
    data[2] = REQUEST_TYPE_SET_INFO;
    data[3] = (unsigned char) (local_port / 256);
    data[4] = (unsigned char) (local_port % 256);
    strcpy(&data[5], client_name);
    data[data_len - 1] = 0;
    send(client_socket_fd, data, data_len, 0);
    
    //asteapta raspunsul de la server
    int n = recv(client_socket_fd, data, data_len, 0);
 
    if(n == 0)
    {
	//conexiunea s-a inchis
	error("Socket hung up");
    } 
    
    if(n < 0)
    {
	error("ERROR in recv");
    }
    
    if(data[0] == RESPONSE_NAME_EXISTS)
    {
	printf("\nUn client cu acelasi nume exista deja !\n");
	exit(0);
    }
 
    if(data[0] == RESPONSE_SUCCESS)
	printf("\nConectat la server !");
    
    free(data);
    
    return client_socket_fd;
}

void receive_server_data(int client_socket_fd)
{
    int n = recv(client_socket_fd, &server_data[server_data_received_bytes], sizeof(server_data) - server_data_received_bytes, 0);

    if(n == 0)
    {
	//conexiunea s-a inchis
	error("Disconnected from server !");

	return;
    }
    
    if(n < 0)
    {
	error("ERROR in recv");
	return;
    }
    
    server_data_received_bytes += n;
}

void handle_listclients_response(int client_socket_fd, int complete_data_size)
{
    printf("\nSERVER LISTCLIENTS RESPONSE: ");
    int i = 3;
    while(i < complete_data_size)
    {
	printf("\n%s", &server_data[i]);
	
	i += strlen(&server_data[i]) + 1;
    }
    
    printf("\nclient# ");
    fflush(stdout);
}

void handle_clientinfo_response(int client_socket_fd, int complete_data_size)
{
    unsigned int port = ((unsigned int)((unsigned char)server_data[3])) * 256 + server_data[4];
    unsigned int connected_since = 0;
    
    connected_since += ((unsigned int)((unsigned char)server_data[5])) * 256 * 256 * 256;
    connected_since += ((unsigned int)((unsigned char)server_data[6])) * 256 * 256;
    connected_since += ((unsigned int)((unsigned char)server_data[7])) * 256;
    connected_since += ((unsigned int)((unsigned char)server_data[8]));
    
    printf("\nSERVER: CLIENTINFO\n%s:%u connected since %u", &server_data[9], port, connected_since);
    printf("\nclient# ");
    fflush(stdout);
}

void handle_clientinfo_response_silent(MessageList* message_list, FileTransferList* file_transfer_list, fd_set* read_fds, PeerList* peer_list, char* src_client_name, int client_socket_fd, int complete_data_size)
{
    unsigned int port = ((unsigned int)((unsigned char)server_data[3])) * 256 + server_data[4];
    unsigned int connected_since = 0;
    
    connected_since += ((unsigned int)((unsigned char)server_data[5])) * 256 * 256 * 256;
    connected_since += ((unsigned int)((unsigned char)server_data[6])) * 256 * 256;
    connected_since += ((unsigned int)((unsigned char)server_data[7])) * 256;
    connected_since += ((unsigned int)((unsigned char)server_data[8]));
    
    char* name = &server_data[9];
    
    int i = 9;
    
    while(server_data[i] && i < complete_data_size)
	i++;
    
    if(i == complete_data_size)
	return; //format incorect
    
    char* ip = &server_data[i + 1];

    MessageList_deliver_messages_to_client(message_list, src_client_name, name, ip, port);
    FileTransferList_start_transfers(file_transfer_list, name, ip, port, read_fds, peer_list);
}

void handle_getshare_response(int client_socket_fd, int complete_data_size)
{
    printf("\nSERVER GETSHARE RESPONSE: ");
    int i = 3;
    while(i < complete_data_size)
    {
	printf("\n%s", &server_data[i]);
	
	i += strlen(&server_data[i]) + 1;
    }
    
    printf("\nclient# ");
    fflush(stdout);
}

void handle_server_data(int client_socket_fd, MessageList* message_list, FileTransferList* file_transfer_list, PeerList* peer_list, fd_set* read_fds, char* name)
{
    if(server_data_received_bytes < 3)
	return;
    
    unsigned int complete_data_size = ((unsigned int)server_data[0]) * 256 + server_data[1];
    
    if(server_data_received_bytes < complete_data_size) //verifica daca s-a primit toata cererea
	return;
    
    int response = server_data[2];
    
    switch(response)
    {
      case RESPONSE_SUCCESS:
	    printf("\nSERVER: SUCCESS");
	    printf("\nclient# ");
	    fflush(stdout);
      break;
      case RESPONSE_LISTCLIENTS:
	    handle_listclients_response(client_socket_fd, complete_data_size);
      break;
      
      case RESPONSE_INEXISTENT_CLIENT:
	    printf("\nSERVER: INEXISTENT CLIENT '%s'", &server_data[3]);
	    printf("\nclient# ");
	    fflush(stdout);
	    
	    MessageList_remove_messages_to_client(message_list, &server_data[3]);
      break;
      
      case RESPONSE_CLIENTINFO:
	    handle_clientinfo_response(client_socket_fd, complete_data_size);
      break;
      
      case RESPONSE_CLIENTINFO_SILENT:
	    handle_clientinfo_response_silent(message_list, file_transfer_list, read_fds, peer_list, name, client_socket_fd, complete_data_size);
      break;
      
      case RESPONSE_GETSHARE:
	    handle_getshare_response(client_socket_fd, complete_data_size);
      break;
	
      default:;
    }
    
    int i;
    for(i = 0; i < server_data_received_bytes - complete_data_size; i++)
	server_data[i] = server_data[complete_data_size + i];
      
    server_data_received_bytes -= complete_data_size;
}

//returneaza un file descriptor corespunzator socketului pe care se asculta conexiuni
int start_local_server(int portno)
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

void handle_new_peer_connection(PeerList* peer_list, int local_server_socket_fd, fd_set* read_fds)
{
    //printf("\nHandling a new connection");fflush(stdout);
  
    struct sockaddr_in cli_addr;
    int clilen = sizeof(cli_addr);
    
    int client_socket_fd;
    
    if((client_socket_fd = accept(local_server_socket_fd, (struct sockaddr *)&cli_addr, &clilen)) == -1)
    {
	    error("ERROR in accept");
    }
    
    Peer* peer = Peer_create(client_socket_fd);
    
    PeerList_add(peer_list, peer);
    
    FD_SET(peer->socket_fd, read_fds);
}

void check_peers(PeerList* peer_list, fd_set* tmp_fds)
{
    Peer* peer = peer_list->first;
    
    if(peer == NULL)
	return;
    
    while(peer != NULL)
    {
	if(FD_ISSET(peer->socket_fd, tmp_fds))
	{
	    Peer_read_data(peer);
	    Peer_handle_data(peer, peer_list);
	}
	
	peer = peer->next;
    }
}

void remove_disconnected_peers(PeerList* peer_list, fd_set* read_fds)
{
    //sterge din lista peerii deconectati
    Peer* peer = peer_list->first;
    
    if(peer == NULL)
	return;
    
    if(peer->state == PEER_STATE_DISCONNECTED)
    {
	peer_list->first = peer->next;
	
	close(peer->socket_fd);
	FD_CLR(peer->socket_fd, read_fds);
	
	Peer_destroy(peer);
	
	return;
    }
    
    while(peer->next != NULL)
    {
	if(peer->next->state == PEER_STATE_DISCONNECTED)
	{
	    Peer* tmp = peer->next;
	    
	    peer->next = peer->next->next;
	    
	    close(tmp->socket_fd);
	    FD_CLR(tmp->socket_fd, read_fds);
	    
	    Peer_destroy(tmp);
	}
	else
	    peer = peer->next;
    }
}

void send_file_data_to_peer(Peer* peer)
{
    char* data = malloc(3 + 1024);
    
    int bytes_read = fread(&data[3], 1, 1024, peer->input_file);
        
    unsigned int data_len;
    
    if(bytes_read > 0)
    {
	data_len = 3 + bytes_read;
    
	data[0] = (unsigned char) (data_len / 256);
	data[1] = (unsigned char) (data_len % 256);
	data[2] = REQUEST_TYPE_FILE_WRITE_DATA;
    
	send(peer->socket_fd, data, data_len, 0);
    }
    
    if(bytes_read < 1024)
    {
	data_len = 3;
	data[0] = (unsigned char) (data_len / 256);
	data[1] = (unsigned char) (data_len % 256);
	data[2] = REQUEST_TYPE_EOF;
	
	send(peer->socket_fd, data, data_len, 0);
	
	peer->state = PEER_STATE_DISCONNECTED;
	
	fclose(peer->input_file);
	peer->input_file = NULL;
    }
    
    free(data);
}

void send_file_data_to_peers(PeerList* peer_list)
{
    Peer* peer = peer_list->first;
    
    while(peer != NULL)
    {
	if(peer->state == PEER_STATE_RECEIVING_DATA)
	    send_file_data_to_peer(peer);
      
	peer = peer->next;
    }
}

int active_transfers(PeerList* peer_list)
{
    int result = 0;
    
    Peer* peer = peer_list->first;
    
    while(peer != NULL)
    {
	if(peer->state == PEER_STATE_RECEIVING_DATA)
	    result++;
      
	peer = peer->next;
    }
    
    return result;
}

int main(int argc, char* argv[])
{
    if(argc < 4) 
    {
         fprintf(stderr,"Usage: %s nume_client ip_server port_server [local_port]\n", argv[0]);
         exit(1);
    }
    
    unsigned int local_port = (argc == 5) ? atoi(argv[4]) : DEFAULT_PORT;
    
    printf("\nListening on local port %u", local_port);
    
    int local_server_socket_fd = start_local_server(local_port);
    
    int client_socket_fd = connect_to_server(argv[1], argv[2], atoi(argv[3]), local_port);
    
    fd_set read_fds;
    
    FD_ZERO(&read_fds);
    FD_SET(client_socket_fd, &read_fds);
    FD_SET(local_server_socket_fd, &read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    
    printf("\nclient# ");fflush(stdout);
    
    PeerList* peer_list = PeerList_create();
    MessageList* message_list = MessageList_create();
    FileTransferList* file_transfer_list = FileTransferList_create();
    
    while(1)
    {
	fd_set tmp_fds = read_fds;
	
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	
	int result = select(FD_SETSIZE, &tmp_fds, NULL, NULL, active_transfers(peer_list) > 0 ? &tv : NULL);
	
	if(result == -1)
	    error("\nERROR on select");
	
	if(result != 0)
	{    
	    if(FD_ISSET(STDIN_FILENO, &tmp_fds))
		handle_stdin_data(client_socket_fd, local_server_socket_fd, message_list, file_transfer_list);
	    
	    if(FD_ISSET(client_socket_fd, &tmp_fds))
	    {
		receive_server_data(client_socket_fd);
		handle_server_data(client_socket_fd, message_list, file_transfer_list, peer_list, &read_fds, argv[1]);
	    }
	    
	    if(FD_ISSET(local_server_socket_fd, &tmp_fds))
	    {
		handle_new_peer_connection(peer_list, local_server_socket_fd, &read_fds);
	    }
	    
	    check_peers(peer_list, &tmp_fds);
	}
	
	send_file_data_to_peers(peer_list);
	remove_disconnected_peers(peer_list, &read_fds);
    }
    
    return 0;
}