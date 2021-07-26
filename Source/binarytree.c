#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "binarytree.h"
#include "tagtype.h"
#include "setdata.h"

/*----------------------------------------------------------------------------------------
Nome da função: insertNode

Descrição: Esta função insere um nó na árvore. Vai descendo pela árvore até encontrar o nó 
sob o qual deve ser criado o novo nó e cria uma lista ligada com um elemento. Se a string já
existe na árvore acrescenta um elemento a lista ligada

Parâmetros:
TNode ** rp: referência para o nó onde começa a procura.
char * word: string a guardar na árvore.
MP3Tag_t *tag: elemento a ser inserido.
    
Retorno:
int: retorna 1 se conseguimos adicionar uma tag.
----------------------------------------------------------------------------------------*/
static int insertNode(TNode ** rp, char * word,  MP3Tag_t *tag){

    if(*rp == NULL) {                                                       //Se o nó não existe.
		*rp = malloc(sizeof(TNode));                                        //Aloca-se memória para o nó.
        if(*rp == NULL)                                                     //Se não se conseguiu alocar memória apresentamos mensagem de erro.
            printf("Erro no ficheiro %s, linha %d.\n", __FILE__, __LINE__), exit(1);

		(*rp)->word = strdup(word);                                         //Inicia-se o ponteiro para a palavra do nó
        (*rp)->data = malloc(sizeof(List));                                 //Alocamos memória para o primeiro elemento da lista ligada.
        if((*rp)->data == NULL)
		    printf("Erro no fich %s, linha %d.\n", __FILE__, __LINE__), exit(1);
            
		createList((*rp)->data, tag);                                       //Inicia-se o ponteiro para a lista ligada
		(*rp)->left = (*rp)->right = NULL;                                  //e os ponteiros para os nós descendentes

		return 1;                                                           //Tivemos sucesso ao criar o nó
    }

    int aux = strcmp(word, (*rp)->word);									//Comparam-se as strings
	if(aux == 0){														    //Se as palavras forem iguais
		insertElem((*rp)->data, tag);            							//Inserimos o novo elemento no fim da lista
        return 1;
    }

	if(aux < 0)																//Se a nova string é menor tenta-se criar novo nó à esquerda	
		return insertNode(&((*rp)->left), word, tag);												  

	return insertNode(&((*rp)->right), word, tag);							//Se a nova string é maior tenta-se criar novo nó à direita		
}

/*----------------------------------------------------------------------------------------
Nome da função: findNode

Descrição: Esta função procura o nó que contem a palavra pretendida
É uma função recursiva, chamando-se a si própria para todos os nós.

Parâmetros:
TNode *r: nó de início da representação.
char *word: palavra a procurar.

Retorno:
int: retorna 1 se a palavra foi encontrada. Caso contrário retorna zero.
----------------------------------------------------------------------------------------*/
int findNode(TNode *r, char *word){
	
  	if(r == NULL)
    	return 0;

  	int aux = strcmp(word, r->word);			// Comparam-se as strings
  	if(aux == 0) {								// Se forem iguais imprimimos as tags da lista ligada
		printList(r->data);						
    	return 1;								// sai-se da função, indicando que a string foi encontrada
	}							
    
  	if(aux < 0)                                 //Se a nova string é menor tenta-se encontrar no nó da esquerda
		return findNode(r->left, word);         
	
  	return findNode(r->right, word);            //Se a nova string é maior tenta-se criar novo nó à direita	
}

/*----------------------------------------------------------------------------------------
Nome da função: countNodes

Descrição: Conta o número de nós existentes na árvore binária.

Parâmetros:
TNode *r: nó de início da árvore.
long count: variável para contar o número de nós.

Retorno:
long: Número de nós.
----------------------------------------------------------------------------------------*/
static long countNodes(TNode *r, long count){

    if(r == NULL)
        return count;

    count = countNodes(r->left, count);         
    count = countNodes(r->right, count);
    count++;

    return count;
}

/*----------------------------------------------------------------------------------------
Nome da função: treeToSortedList

Descrição: Esta função degenera uma árvore binária numa lista ordenada à direita.

Parâmetros:
TNode *r: nó de início da representação.
TNode *link: nó que antecede o nó apontado por r. Quando r é raiz da árvore, 
o parâmetro link deverá ser NULL.

Retorno:
TNode *: nó de início da nova árvore.
----------------------------------------------------------------------------------------*/
static TNode *treeToSortedList( TNode *r, TNode *link ){
    TNode * p;
    if( r == NULL ) return link;
    p = treeToSortedList( r->left, r );
    r->left = NULL;
    r->right = treeToSortedList( r->right, link );
    return p;
}

/*----------------------------------------------------------------------------------------
Nome da função: sortedListToBalancedTree

Descrição: Esta função balanceia uma árvore binária que esteja degenerada em 
lista ordenada à direita.

Parâmetros:
TNode **list: nó de início da representação.
int n: número de nós

Retorno:
TNode *: nó de início da lista.
----------------------------------------------------------------------------------------*/
static TNode* sortedListToBalancedTree(TNode **listRoot, int n){
    if( n == 0 )
        return NULL;
    TNode *leftChild = sortedListToBalancedTree(listRoot, n/2);
    TNode *parent = *listRoot;
    parent->left = leftChild;
    *listRoot = (*listRoot)->right;
    parent->right = sortedListToBalancedTree(listRoot, n-(n/2 + 1) );
    return parent;
}

/*----------------------------------------------------------------------------------------
Nome da função: Tbalance

Descrição: Esta função balanceia a árvore binária

Parâmetros:
TNode *root: nó de início da árvore.
int n: número de nós.

Retorno:
TNode *: novo nó da árvore.
----------------------------------------------------------------------------------------*/
static TNode* Tbalance(TNode *root, int n){
    root = treeToSortedList(root, NULL);
    root = sortedListToBalancedTree(&root, n);

    return root;
}

/*----------------------------------------------------------------------------------------
Nome da função: freeNode

Descrição: Esta função liberta a memória alocada por um nó da árvore e 
respetivo conteúdo.

Parâmetros:
TNode *node: nó da árvore para o qual se pretende libertar a memória.

Retorno:
Não tem.
----------------------------------------------------------------------------------------*/
void createBinaryTree(TagSet_t *set ) {

    int i;

    for(i = 0; i < set->count; i++){ 
        char str[MAX_WORD+1];                       
        strcpy(str, set->aux[i]->title);                           //para não destruir a string original
        char *word = strtok( str, " \t\n()-_[]" );                 //vamos buscar a primeira palavra desta nova tag
        while(word != NULL){                                       //enquanto houver palavras novas na tag
            insertNode(&(set->root), word,  set->aux[i]);          //tentamos inserior um novo nó na arvore
            word = strtok(NULL, " \t\n()-_[]");                    //vamos buscar uma nova palavra
        }

        if(i % 10 == 0)
            set->root = Tbalance(set->root, countNodes(set->root, 0)); //balanceamento a intervalos de várias inserções (a cada 10 faixas)
    }

    set->root = Tbalance(set->root, countNodes(set->root, 0));  //último balanceamento da árvore

}

/*----------------------------------------------------------------------------------------
Nome da função: freeNode

Descrição: Esta função liberta a memória alocada por um nó da árvore e 
respetivo conteúdo.

Parâmetros:
TNode *node: nó da árvore para o qual se pretende libertar a memória.

Retorno:
Não tem.
----------------------------------------------------------------------------------------*/
static void freeNode(TNode *node){
    freeList(node->data);                        // Liberta a mem dos dados contidos no nó. 
    free(node->word);                            // Liberta a mem da palavra contida no nó. 
	free(node);									 // Liberta a mem da estrutura do próprio nó. 
}

/*----------------------------------------------------------------------------------------
Nome da função: freeTree

Descrição: Esta função liberta a memória alocada por uma árvore.

Parâmetros:
TNode *r: nó da árvore para o qual se pretende libertar a memória.

Retorno:
Não tem.
----------------------------------------------------------------------------------------*/
void freeTree(TNode *r){
    if(r == NULL)
        return;
	
  	freeTree(r->right);	                // Liberta a memória alojada para a árvore que existe à direita 
  	freeTree(r->left);	                // Liberta a memória alojada para a árvore que existe à esquerda 
  	freeNode(r);		                // Liberta a memória alojada para este nó.
}
