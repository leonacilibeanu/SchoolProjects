#include <stdio.h>
#include "ShareClientList.h"
#include "ShareClient.h"
#include "SharedFileLinkedList.h"

ShareClientList* ShareClientList_create()
{
    ShareClientList* result = (ShareClientList*)malloc(sizeof(ShareClientList));
    
    result->first = NULL;
    
    return result;
}

void ShareClientList_add(ShareClientList* list, ShareClient* shareClient)
{
    shareClient->next = NULL;
    
    if(list->first == NULL)
	list->first = shareClient;
    else
    {
	ShareClient* last = list->first;
	
	while(last->next != NULL)
	    last = last->next;
	
	last->next = shareClient;
    }
}

void ShareClientList_print(ShareClientList* list)
{
    ShareClient* shareClient = list->first;
    
    while(shareClient != NULL)
    {
	printf("\n%s %s:%u connected since %d s", shareClient->name, shareClient->ip, shareClient->port, (int)(time(NULL) - shareClient->connection_time));
	
	SharedFile* sharedFile = shareClient->shared_file_list->first;
	
	while(sharedFile != NULL)
	{
	    printf("\n\t%s", sharedFile->name);
	    sharedFile = sharedFile->next;
	}
	
	shareClient = shareClient->next;
    }
    
    fflush(stdout);
}
 
