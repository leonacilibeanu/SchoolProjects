#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "MessageList.h"

MessageList* MessageList_create()
{
    MessageList* result = (MessageList*)malloc(sizeof(MessageList));
    
    result->first = NULL;
    
    return result;
}

void MessageList_add(MessageList* list, Message* message)
{
    message->next = NULL;

    if(list->first == NULL)
    {
	list->first = message;
	
	return;
    }
    
    Message* last = list->first;
    
    while(last->next != NULL)
	last = last->next;
    
    last->next = message;
}

void MessageList_remove_messages_to_client(MessageList* list, char* client_name)
{
    Message* msg = list->first;
    
    if(msg == NULL)
	return;
    
    while(strcmp(msg->dst_name, client_name) == 0)
    {
	list->first = msg->next;
	
	Message_destroy(msg);
	
	msg = list->first;
	
	if(msg == NULL)
	    return;
    }
    
    while(msg->next != NULL)
    {
	if(strcmp(msg->next->dst_name, client_name) == 0)
	{
	    Message* tmp = msg->next;
	    
	    msg->next = msg->next->next;
	    
	    Message_destroy(tmp);
	}
	else
	    msg = msg->next;
    }
}

void MessageList_deliver_messages_to_client(MessageList* list, char* src_client_name, char* dst_client_name, char* ip, int port)
{
    Message* msg = list->first;
    
    if(msg == NULL)
	return;
    
    while(strcmp(msg->dst_name, dst_client_name) == 0)
    {
	list->first = msg->next;
	
	Message_send(msg, src_client_name, ip, port);
	
	Message_destroy(msg);
	
	msg = list->first;
	
	if(msg == NULL)
	    return;
    }
    
    while(msg->next != NULL)
    {
	if(strcmp(msg->next->dst_name, dst_client_name) == 0)
	{
	    Message* tmp = msg->next;
	    
	    msg->next = msg->next->next;
	    
	    Message_send(tmp, src_client_name, ip, port);
	    
	    Message_destroy(tmp);
	}
	else
	    msg = msg->next;
    }
}

