#include "ShareClient.h"
#include "ShareClientList.h"
#include "SharedFileLinkedList.h"
#include "response.h"
#include "request_type.h"

ShareClient* ShareClient_create(int socket_fd, struct sockaddr_in* cli_addr)
{
    ShareClient* result = (ShareClient*) malloc(sizeof(ShareClient));
    
    result->socket_fd = socket_fd;
    result->connection_time = time(NULL);
    result->state = STATE_WAIT_INFO;
    result->port = 0;
    result->received_bytes = 0;
    result->shared_file_list = SharedFileLinkedList_create();
    strcpy(result->name, "");
    
    inet_ntop(AF_INET, &cli_addr->sin_addr.s_addr, result->ip, INET_ADDRSTRLEN);

    //printf("\nNew connection from %s", result->ip);fflush(stdout);
    
    return result;
}

void ShareClient_read_data(ShareClient* shareClient)
{
    int n = recv(shareClient->socket_fd, &shareClient->data[shareClient->received_bytes], sizeof(shareClient->data) - shareClient->received_bytes, 0);

    if(n == 0)
    {
	//conexiunea s-a inchis
	shareClient->state = STATE_DISCONNECTED;

	return;
    }
    
    if(n < 0)
    {
	error("ERROR in recv");
	return;
    }
    
    shareClient->received_bytes += n;
}

//2 byti lungimea, 1 byte raspunsul
void ShareClient_send_response(ShareClient* shareClient, unsigned char response)
{
    char data[] = {0, 3, response};
    send(shareClient->socket_fd, data, 3, 0);
}

void ShareClient_send_response_with_param(ShareClient* shareClient, unsigned char response, char* param)
{
    int data_len = 3 + strlen(param) + 1;
    char* data = (char*)malloc(data_len);
    
    data[0] = (unsigned char) (data_len / 256);
    data[1] = (unsigned char) (data_len % 256);
    data[2] = response;
    
    strcpy(&data[3], param);
    
    send(shareClient->socket_fd, data, data_len, 0);
    
    free(data);
}


//2 byti lungimea cererii, 1 byte tipul cererii, 2 byti portul, restul este numele terminat cu '\0'
int ShareClient_set_info(ShareClient* shareClient, unsigned int complete_data_size, ShareClientList* client_list)
{
    if(complete_data_size < 6)  //cerere invalida
	return 0;

    if(shareClient->data[complete_data_size - 1] != 0)  //numele trebuie sa se termine cu '\0'
	return 0;

    if(complete_data_size > MAX_NAME_LENGTH + 5)
	return 0; //numele e prea lung

    //verifica daca numele este unic
    ShareClient* current = client_list->first;
    
    while(current != NULL)
    {
	if(strcmp(current->name, &shareClient->data[5]) == 0)
	{
	    //numele exista deja
	    ShareClient_send_response(shareClient, RESPONSE_NAME_EXISTS);
	    return 1;
	}
      
	current = current->next;
    }

    shareClient->port = ((unsigned int)((unsigned char)shareClient->data[3])) * 256 + shareClient->data[4];
    strcpy(shareClient->name, &shareClient->data[5]);
    
    shareClient->state = STATE_CONNECTED;
    
    ShareClient_send_response(shareClient, RESPONSE_SUCCESS);
    
    return 1;
}

//2 byti lungimea cererii, 1 byte tipul cererii, restul este numele fisierului terminat cu '\0'
int ShareClient_share_file(ShareClient* shareClient, unsigned int complete_data_size)
{
    //verifica daca fisierul este deja adaugat
    SharedFile* sharedFile = shareClient->shared_file_list->first;
    
    while(sharedFile != NULL)
    {
	if(strcmp(sharedFile->name, &shareClient->data[3]) == 0)
	    return 0;  //fisierul e deja pus la share
	
	sharedFile = sharedFile->next;
    }
    
    SharedFileLinkedList_add(shareClient->shared_file_list, &shareClient->data[3]);
    
    return 1;
}

//2 byti lungimea cererii, 1 byte tipul cererii, restul este numele fisierului terminat cu '\0'
int ShareClient_unshare_file(ShareClient* shareClient, unsigned int complete_data_size)
{  
    SharedFileLinkedList_remove(shareClient->shared_file_list, &shareClient->data[3]);
    
    return 1;
}

//2 byti lungimea raspunsului, 1 byte tipul raspunsului, numele clientilor terminate cu '\0', unul dupa celalalt
void ShareClient_list_clients(ShareClient* shareClient, int complete_data_size, ShareClientList* client_list)
{
    int data_len = 3;
    
    ShareClient* current = client_list->first;
    
    while(current != NULL)
    {
	data_len += strlen(current->name) + 1;
      
	current = current->next;
    }
    
    char* data = (char*)malloc(data_len);
    
    data[0] = (unsigned char) (data_len / 256);
    data[1] = (unsigned char) (data_len % 256);
    data[2] = RESPONSE_LISTCLIENTS;
    
    current = client_list->first;
    
    int pos = 3;
    while(current != NULL)
    {
	strcpy(&data[pos], current->name);
	
	pos += strlen(current->name) + 1;
	
	current = current->next;
    }    
    
    send(shareClient->socket_fd, data, data_len, 0);
    
    free(data);
}

//2 byti lungimea raspunsului, 1 byte tipul raspunsului, 2 byti portul, 4 byti de cat timp e conectat, numele clientului
void ShareClient_send_clientinfo(ShareClient* shareClient, int complete_data_size, ShareClientList* client_list)
{
    char* client_name = &shareClient->data[3];
    
    ShareClient* current = client_list->first;
    
    //cauta clientul in lista
    while(current != NULL)
    {
	if(strcmp(current->name, client_name) == 0)
	    break;
	
	current = current->next;
    }
    
    if(current == NULL)
    {
	ShareClient_send_response_with_param(shareClient, RESPONSE_INEXISTENT_CLIENT, client_name);
	return;
    }
    
    int data_len = 3 + 2 + 4 + strlen(current->name) + 1;
    char* data = (char*)malloc(data_len);
    
    data[0] = (unsigned char) (data_len / 256);
    data[1] = (unsigned char) (data_len % 256);
    data[2] = RESPONSE_CLIENTINFO;
    data[3] = (unsigned char) (current->port / 256);
    data[4] = (unsigned char) (current->port % 256);
    
    unsigned int connected_since = (unsigned int)(time(NULL) - current->connection_time);

    data[8] = (unsigned char) (connected_since % 256);
    connected_since /= 256;
    data[7] = (unsigned char) (connected_since % 256);
    connected_since /= 256;
    data[6] = (unsigned char) (connected_since % 256);
    connected_since /= 256;
    data[5] = (unsigned char) (connected_since % 256);
    
    strcpy(&data[9], current->name);
    data[data_len - 1] = 0;
    
    send(shareClient->socket_fd, data, data_len, 0);

    free(data);
}

//2 byti lungimea raspunsului, 1 byte tipul raspunsului, 2 byti portul, 4 byti de cat timp e conectat, numele clientului, ip-ul
void ShareClient_send_clientinfo_silent(ShareClient* shareClient, int complete_data_size, ShareClientList* client_list)
{
    char* client_name = &shareClient->data[3];
    
    ShareClient* current = client_list->first;
    
    //cauta clientul in lista
    while(current != NULL)
    {
	if(strcmp(current->name, client_name) == 0)
	    break;
	
	current = current->next;
    }
    
    if(current == NULL)
    {
	ShareClient_send_response_with_param(shareClient, RESPONSE_INEXISTENT_CLIENT, client_name);
	return;
    }
    
    int data_len = 3 + 2 + 4 + strlen(current->name) + 1 + strlen(current->ip) + 1;
    char* data = (char*)malloc(data_len);
    
    data[0] = (unsigned char) (data_len / 256);
    data[1] = (unsigned char) (data_len % 256);
    data[2] = RESPONSE_CLIENTINFO_SILENT;
    data[3] = (unsigned char) (current->port / 256);
    data[4] = (unsigned char) (current->port % 256);
    
    unsigned int connected_since = (unsigned int)(time(NULL) - current->connection_time);

    data[8] = (unsigned char) (connected_since % 256);
    connected_since /= 256;
    data[7] = (unsigned char) (connected_since % 256);
    connected_since /= 256;
    data[6] = (unsigned char) (connected_since % 256);
    connected_since /= 256;
    data[5] = (unsigned char) (connected_since % 256);
    
    strcpy(&data[9], current->name);
    strcpy(&data[9 + strlen(current->name) + 1], current->ip);
    data[data_len - 1] = 0;
    
    send(shareClient->socket_fd, data, data_len, 0);

    free(data);
}

//2 byti lungimea raspunsului, 1 byte tipul raspunsului, lista de fisiere separate print '\0'
void ShareClient_send_share(ShareClient* shareClient, int complete_data_size, ShareClientList* client_list)
{
    char* client_name = &shareClient->data[3];
    
    ShareClient* current = client_list->first;
    
    //cauta clientul in lista
    while(current != NULL)
    {
	if(strcmp(current->name, client_name) == 0)
	    break;
	
	current = current->next;
    }
    
    if(current == NULL)
    {
	ShareClient_send_response(shareClient, RESPONSE_INEXISTENT_CLIENT);
	return;
    }
    
    int data_len = 3;
    
    SharedFile* shareFile = current->shared_file_list->first;
    
    while(shareFile != NULL)
    {
	data_len += strlen(shareFile->name) + 1;
	
	shareFile = shareFile->next;
    }
    
    char* data = (char*)malloc(data_len);
    
    data[0] = (unsigned char) (data_len / 256);
    data[1] = (unsigned char) (data_len % 256);
    data[2] = RESPONSE_GETSHARE;
    
    shareFile = current->shared_file_list->first;
    
    int pos = 3;
    while(shareFile != NULL)
    {
	strcpy(&data[pos], shareFile->name);
      
	pos += strlen(shareFile->name) + 1;
	
	shareFile = shareFile->next;
    }
    
    send(shareClient->socket_fd, data, data_len, 0);

    free(data);
}

void ShareClient_handle_data(ShareClient* shareClient, ShareClientList* client_list)
{
    if(shareClient->received_bytes < 3)
	return;
    
    unsigned int complete_data_size = ((unsigned int)shareClient->data[0]) * 256 + shareClient->data[1];
    
    if(shareClient->received_bytes < complete_data_size) //verifica daca s-a primit toata cererea
	return;
    
    int request_type = shareClient->data[2];
    
    switch(request_type)
    {
      case REQUEST_TYPE_SET_INFO:
	    ShareClient_set_info(shareClient, complete_data_size, client_list);
      break;
      
      case REQUEST_TYPE_LISTCLIENTS:
	    ShareClient_list_clients(shareClient, complete_data_size, client_list);
      break;
      
      case REQUEST_TYPE_INFOCLIENT:
	    ShareClient_send_clientinfo(shareClient, complete_data_size, client_list);
      break;
      
      case REQUEST_TYPE_INFOCLIENT_SILENT:
	    ShareClient_send_clientinfo_silent(shareClient, complete_data_size, client_list);
      break;
      
      case REQUEST_TYPE_SHARE:
	    ShareClient_share_file(shareClient, complete_data_size);
      break;
      
      case REQUEST_TYPE_UNSHARE:
	    ShareClient_unshare_file(shareClient, complete_data_size);
      break;
      
      case REQUEST_TYPE_GETSHARE:
	    ShareClient_send_share(shareClient, complete_data_size, client_list);
      break;
	
      case REQUEST_TYPE_DISCONNECT:
	    shareClient->state = STATE_DISCONNECTED;
      break;
      
      default:;
    }
    
    int i;
    for(i = 0; i < shareClient->received_bytes - complete_data_size; i++)
	shareClient->data[i] = shareClient->data[complete_data_size + i];
      
    shareClient->received_bytes -= complete_data_size;
} 
