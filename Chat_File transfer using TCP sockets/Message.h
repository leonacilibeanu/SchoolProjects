#ifndef MESSAGE_H
#define MESSAGE_H

typedef struct _Message
{
    char* dst_name;
    char* msg;
    struct _Message* next;
}Message; 


Message* Message_create(char* dst_name, char* msg);

void Message_send(Message* msg, char* src_client_name, char* ip, int port);

void Message_destroy(Message* message);

#endif