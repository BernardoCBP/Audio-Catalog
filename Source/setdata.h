#ifndef SETDATA_H
#define SETDATA_H

#include "binarytree.h"

#define MAX_WORD 30
#define NO_INFO "NO INFO" //Usado para os casos em que os campos da tag não tem informação

typedef struct tagSet_t TagSet_t;
typedef struct tNode TNode;

struct tagSet_t{
    int size; 
    int count; 
    MP3Tag_t **data; 
    MP3Tag_t **aux;
    TNode *root; 
}; 

TagSet_t *setCreate( void );
void setAdd( TagSet_t *, MP3Tag_t * );
void setReady( TagSet_t * );
void setCommand( TagSet_t *, char * );

void bufferControl();

void setDelete( TagSet_t *);

#endif