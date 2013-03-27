#include <stdlib.h>
#include <string.h>

#include "SharedFileLinkedList.h"

//creaza o lista goala
SharedFileLinkedList* SharedFileLinkedList_create()
{
    SharedFileLinkedList* result = (SharedFileLinkedList*)malloc(sizeof(SharedFileLinkedList));
    
    result->first = NULL;
    
    return result;
}

//sterge o lista
void SharedFileLinkedList_destroy(SharedFileLinkedList* list)
{
    SharedFile* sharedFile = list->first;
    
    while(sharedFile != NULL)
    {
	SharedFile* tmp = sharedFile;
	
	sharedFile = sharedFile->next;
	
	free(tmp->name);
	free(tmp);
    }
    
    free(list);
}

//adauga un fisier nou in lista
void SharedFileLinkedList_add(SharedFileLinkedList* list, char* name)
{
    SharedFile* sharedFile = (SharedFile*)malloc(sizeof(SharedFile));
    
    sharedFile->name = (char*)malloc(strlen(name) + 1);
    strcpy(sharedFile->name, name);

    sharedFile->next = NULL;
    
    if(list->first == NULL)
	list->first = sharedFile;
    else
    {
	SharedFile* last = list->first;
	
	while(last->next != NULL)
	    last = last->next;
	
	last->next = sharedFile;
    }
}

//sterge un fisier din lista
int SharedFileLinkedList_remove(SharedFileLinkedList* list, char* name)
{
    SharedFile* sharedFile = list->first;
    
    if(sharedFile == NULL)
	return 0;
    
    //verifica daca este primul fisier
    if(strcmp(name, sharedFile->name) == 0)
    {
	list->first = sharedFile->next;
      
	free(sharedFile->name);
	free(sharedFile);
	
	return 1;	//fisierul a fost gasit
    }
	
    while(sharedFile->next != NULL)
    {
	if(strcmp(name, sharedFile->next->name) == 0)
	{
	    SharedFile* tmp = sharedFile->next;
	    
	    sharedFile->next = sharedFile->next->next;
	 
	    free(tmp->name);
	    free(tmp);
	    
	    return 1;	//fisierul a fost gasit
	}
	else
	    sharedFile = sharedFile->next;
    }
    
    return 0;  //fisierul nu exista in lista
} 
