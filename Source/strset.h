#ifndef STRSET_H
#define STRSET_H

#define DEF_HASHSIZE 35             //valor primo (baixo) escolhido  
#define LOAD_FACTOR 0.75

typedef struct hashList {
    struct hashList *next;
	char *string;
}HashList;

typedef struct {
    int hashSize;                   //dimensão do array da hash-table
    int inElem;                     //numero de elementos inseridos na tabela
    HashList **hashtable;
}strSet_t;

strSet_t *strSetCreate( void );
char *strSearchAdd( strSet_t *, const char * );
void strSetDelete( strSet_t * );

#endif

