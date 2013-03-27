#ifndef MESSAGE_LIST_H
#define MESSAGE_LIST_H

#include "Message.h"

typedef struct _MessageList
{
    Message* first;
}MessageList;

MessageList* MessageList_create();

void MessageList_add(MessageList* list, Message* message);

void MessageList_remove_messages_to_client(MessageList* list, char* client_name);

void MessageList_deliver_messages_to_client(MessageList* list, char* src_client_name, char* dst_client_name, char* ip, int port);

#endif
