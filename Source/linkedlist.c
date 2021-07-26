#include <stdlib.h>
#include <stdio.h>
#include "setdata.h"
#include "linkedlist.h"

/*----------------------------------------------------------------------------------------
Nome da função: createList

Descrição: Esta função cria o primeiro elemento duma lista ligada

Parâmetros:
List *head: referência para a cabeça da lista.
MP3Tag_t *tag: elemento a ser inserido.

Retorno:
Não tem.
----------------------------------------------------------------------------------------*/
void createList(List *head, MP3Tag_t *tag){

    head->tag = tag;                //aponta para a tag correspondente
    head->next = NULL;              //o next é nulo visto que é o primeiro elemento a ser inserido
}

/*----------------------------------------------------------------------------------------
Nome da função: insertElem

Descrição: Esta função insere o elemento ao fim da lista ligada. Escolhemos inserir ao fim
pelo facto de que a árvore binária vai ser criada depois dos elementos já estarem ordenados
por ordem alfabetica. Escolhemos usar uma lista simplesmente ligada so com head, escolhemos não
usar uma tail (que faria a inserção dos elementos ao fim da lista mais facil) para poupar memória.

Parâmetros:
List *head: referência para a cabeça da lista.
MP3Tag_t *tag: elemento a ser inserido.

Retorno:
Não tem.
----------------------------------------------------------------------------------------*/
void insertElem(List *head, MP3Tag_t *tag){
    
    List  *newElem, *tempElem;

    newElem = malloc(sizeof(List));         //tentamos alocar memoria para o novo elemento 
    if(newElem == NULL)                     //em caso de insucesso mostramos um erro
		printf("Erro no fich %s, linha %d.\n", __FILE__, __LINE__), exit(1);

    newElem->tag = tag;                     //aponta para a tag correspondente
    newElem->next = NULL;                   //o next é nulo visto que está a ser inserido no fim da lista ligada

    tempElem = head;                        

    while(tempElem->next != NULL)           //percorremos a lista ligada para o next do elemento que antes era o último apontar para o novo elemento
        tempElem = tempElem->next;

    tempElem->next = newElem; 
}

/*----------------------------------------------------------------------------------------
Nome da função: printList
 
Descrição: Esta função afixa na consola todos os elemento da lista. Escolhemos uma abordagem idêntica 
a usada no setdata.c para imprimir os parametros de cada tag.

Parâmetros:
List *head: referência para a cabeça da lista.

Retorno:
Não tem. 
----------------------------------------------------------------------------------------*/
void printList(List * head){

    printf("Title: %-30s  ", (head->tag->title[0] != '~' ? head->tag->title  : NO_INFO) );
    printf("Artist: %-30s  ", (head->tag->artist[0] != '~' ? head->tag->artist  : NO_INFO) );
    printf("Album: %-30s  ", (head->tag->album[0] != '~' ? head->tag->album  : NO_INFO) );
            
    if(head->tag->year == 0)
        printf("Year: %-7s  ", NO_INFO);
    else
        printf("Year: %-7hi  ", head->tag->year);

    printf("Comment: %-30s  ", (head->tag->comment[0] != '\0' ? head->tag->comment : NO_INFO) );
        
    if(head->tag->track == 0)
        printf("Track: %-7s  ", NO_INFO);
    else
        printf("Track: %-7d  ", head->tag->track);

    if(head->tag->genre == -1)
        printf("Genre: %-7s  ", NO_INFO);
    else
        printf("Genre: %-7d  ", head->tag->genre);
            
    printf("FileName: %s\n", head->tag->fileName);

    if(head->next != NULL)  //Se ainda há um elemento a seguir na lista ligada chamamos a função de novo com esse novo elemento
		printList(head->next);

}
    
/*----------------------------------------------------------------------------------------
Nome da função: freeList
 
Descrição: libertar a memória da lista ligada

Parâmetros:
Elem *head: referência para a cabeça da lista.

Retorno:
Não tem. 
----------------------------------------------------------------------------------------*/
void freeList(List * head){

	if(head->next != NULL)
		freeList(head->next);

	free(head);
}
