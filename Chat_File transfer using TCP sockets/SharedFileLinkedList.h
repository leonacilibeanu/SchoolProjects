#ifndef SHARED_FILE_LINKED_LIST_H
#define SHARED_FILE_LINKED_LIST_H

typedef struct _SharedFile
{
    char* name;
    struct _SharedFile* next;
} SharedFile;

typedef struct _SharedFileLinkedList
{
    SharedFile* first;
} SharedFileLinkedList;

SharedFileLinkedList* SharedFileLinkedList_create();
void SharedFileLinkedList_destroy(SharedFileLinkedList* list);
void SharedFileLinkedList_add(SharedFileLinkedList* list, char* name);
int SharedFileLinkedList_remove(SharedFileLinkedList* list, char* name);

#endif
