#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "tagtype.h"

typedef struct list {
    struct list *next;
	MP3Tag_t *tag;
}List;

void createList(List *, MP3Tag_t *);
void insertElem(List *, MP3Tag_t *);
void printList(List *);
void freeList(List *);

#endif
