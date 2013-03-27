#ifndef SHARE_CLIENT_LIST_H
#define SHARE_CLIENT_LIST_H

struct _ShareClient;

typedef struct _ShareClientList 
{
    struct _ShareClient* first;
} ShareClientList;

ShareClientList* ShareClientList_create();

void ShareClientList_add(ShareClientList* list, struct _ShareClient* shareClient);

void ShareClientList_print(ShareClientList* list);

#endif
