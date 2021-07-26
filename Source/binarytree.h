#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "linkedlist.h"
#include "setdata.h"

typedef struct tNode TNode;
typedef struct tagSet_t TagSet_t;

struct tNode{
    struct tNode * left;        //ponteiro para o nó da esquerda
    struct tNode * right;       //ponteiro para o nó da direita
    char * word;                //palavra do nó
    List * data;                //cabeça da lista ligada
};

int findNode(TNode *, char *);

void createBinaryTree(TagSet_t *set );

void freeTree(TNode *);

#endif
