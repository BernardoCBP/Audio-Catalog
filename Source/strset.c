#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "strset.h"


/*----------------------------------------------------------------------------------------
Nome da função: hash

Descrição: Esta função realiza uma função de hash aplicada a uma string.

Parâmetros:
char *s: string a codificar (chave para a função).

Retorno:
unsigned int: nº inteiro positivo para indexação do array.
----------------------------------------------------------------------------------------*/
static unsigned int hash(strSet_t *set, const char *s) {
    unsigned hashval;

    for(hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;

    return hashval % set->hashSize; 
}


/*----------------------------------------------------------------------------------------
Nome da função: lookup

Descrição: Esta função procura se uma string já existe na tabela de hash.

Parâmetros:
char *s: string a procurar.

Retorno:
Elem *: ponteiro para a estrutura que contém a string ou NULL se a string não existir 
no array ponteiro para a estrutura que contém a string.
----------------------------------------------------------------------------------------*/
static HashList *lookup( strSet_t *set, const char *s) {
    HashList *elem;

    for(elem = set->hashtable[hash(set, s)]; elem != NULL; elem = elem->next)	     // Percorre-se a lista ligada até
	    if(strcmp(s, elem->string) == 0)							                // se encontrar a string "name", caso em que
	        return elem;											                // se retorna o ponteiro para o elemento.

    return NULL;												                    // Caso contrário, indica-se que não foi encontrada.
}

/*----------------------------------------------------------------------------------------
Nome da função: isPrime

Descrição: Esta função determina se o número inserido é um número primo

Parâmetros:
int n: número a determinar

Retorno:
int: 1 se o número é primo ou 0 se não é primo
----------------------------------------------------------------------------------------*/
static int isPrime(int n) {

    int i;

    if (n <= 1)  return 0;  
    if (n <= 3)  return 1;  
     
    if (n % 2 == 0 || n % 3 == 0) return 0;  
    
    for (i=5; i*i <= n; i = i+6)  
        if (n % i == 0 || n % (i+2) == 0)  
           return 0;  
    
    return 1;  
}

/*----------------------------------------------------------------------------------------
Nome da função: nextPrime

Descrição: Esta função retorna o menor número primo maior que o número inserido, esta 
função e ultima foram usadas no programa para determinar um tamanho para a hash-table primo, 
isto foi feito porque pelo que percebemos ter um tamanho da hash-table primo ajuda a 
evitar tantas colisões e assim melhora a eficiencia da hash-table.

Parâmetros:
int num: minimo número de elementos que a tabela deve ter.

Retorno:
int: menor número primo possível
----------------------------------------------------------------------------------------*/
static int nextPrime(int num) { 
  
    if (num <= 1)        //caso base
        return 2; 
  
    int found = 0; 

    while (!found) {     // percorremos o loop até a função isPrime retornar 1
        num++; 
  
        if (isPrime(num)) 
            found = 1; 
    } 
  
    return num; 
}

/*----------------------------------------------------------------------------------------
Nome da função: strSetCreate

Descrição: Esta função cria, em alojamento dinâmico, o descritor para o conjunto de strings.

Parâmetros:
Não tem.

Retorno:
strSet_t *: O endereço do descritor criado.
----------------------------------------------------------------------------------------*/

strSet_t *strSetCreate( void ) {

    strSet_t *set = malloc(sizeof (strSet_t)); //alocação de memoria para o set
    if(set == NULL)                            //Verificar se foi possivel alocar memoria para o set
        return NULL;

    set->hashSize = nextPrime(DEF_HASHSIZE/LOAD_FACTOR);
    set->inElem = 0;
    set->hashtable = calloc(set->hashSize, sizeof(HashList *));
    if(set->hashtable == NULL)                            //Verificar se foi possivel alocar memoria para o set
        printf("Erro no ficheiro %s, linha %d.\n", __FILE__, __LINE__), exit(1);

    return set;                                //se tivermos sucesso retornamos o endereço do descritor criado
}

/*----------------------------------------------------------------------------------------
Nome da função: rebuild_hash

Descrição: Esta função cria um novo array para a nossa hash-table se o numero de elementos
inseridos chegar a um certo patarmar, ou seja se o valor predefinido para o load factor 
(usamos a convenção encontrada de 75%) for ultrapassado. Este novo array vai ter aproximadamente
o dobro do tamanho que o array anterior.

Parâmetros:
strSet_t *set: ponteiro do descritor para o conjunto de strings.

Retorno:
Não tem.
----------------------------------------------------------------------------------------*/
static void rebuild_hash(strSet_t *set) {

    int i, prevHashSize = set->hashSize;
    unsigned hashval;
    HashList *tempElem, **prevHash = set->hashtable;

    //Criação da nova hash-table
    set->hashSize = nextPrime(2 * (prevHashSize));                  //a dimensão será aproximadamente o dobro      
    set->hashtable = calloc(set->hashSize, sizeof(HashList *));     //alocar memoria para o novo array com a nova dimensão
    if(set->hashtable == NULL)                                      //Verificar se foi possivel alocar memoria para o set
        printf("Erro no ficheiro %s, linha %d.\n", __FILE__, __LINE__), exit(1);

    for(i = 0; i< prevHashSize; i++){                               //percorrer a hash-table antiga
        while(prevHash[i]) {                                        //percorrer todos os elementos da lista ligada nessa posição

            tempElem = prevHash[i]->next;                           //guardar o endereço do elemento seguinte 

            hashval = hash(set, prevHash[i]->string);				// Obter o índice da string no novo array
		    prevHash[i]->next = set->hashtable[hashval];		    // O next aponta para o primeiro elemento da lista ou para NULL se é o primeiro elemento.
		    set->hashtable[hashval] = prevHash[i];		            // guardamos o elemento da lista antiga no novo array (com nova posição calculada)

            prevHash[i] = tempElem;                                 //passamos para o elemento seguinte da lista (se for NULL o while acaba)
        }
    }
    free(prevHash);                                                 //libertar a memoria alocada para o array do hash-table anterior
}

/*----------------------------------------------------------------------------------------
Nome da função: strSearchAdd

Descrição: Esta função procura no conjunto set a string indicada.

Parâmetros:
strSet_t *set: ponteiro do descritor para o conjunto de strings.
const char *str: string a procurar.

Retorno:
char *: O endereço da string armazenada no conjunto.
----------------------------------------------------------------------------------------*/
char *strSearchAdd( strSet_t *set, const char *str ) {

    HashList *elem;
	unsigned hashval;

	if((elem = lookup(set, str)) == NULL) { 		 // Vamos verificar se a string recebida já existe na hash-table
		elem = malloc(sizeof(HashList));			 // Se não existir alocamos memória para o novo elemento
		if(elem == NULL)						
			return NULL;
        
        if(set->inElem >= LOAD_FACTOR*set->hashSize) //verificar se a dimensão do array ainda é adequada
            rebuild_hash(set);

        elem->string = strdup(str);
        if(elem->string == NULL){
            free(elem);
            return NULL;
        }
        hashval = hash(set, str);				    // Obter o índice para a string recebida
		elem->next = set->hashtable[hashval];		// O next aponta para o primeiro elemento da lista ou para NULL é o primeiro elemento.
		set->hashtable[hashval] = elem;			    // O novo elemento passa a ser o 1º da lista (inserção à cabeça da lista).

        set->inElem++;
        return elem->string;
    }

    else 
        return elem->string;   //return elem;                 // Se já existe o elemento na hash-table então retornamos o endereço dessa string.

}

/*----------------------------------------------------------------------------------------
Nome da função: strListDelete
 
Descrição: libertar a memória da lista ligada de cada elemento da hash-table

Parâmetros:
Elem *head: referência para a cabeça da lista.

Retorno:
Não tem. 
----------------------------------------------------------------------------------------*/
static void strListDelete(HashList * head){

    if(head == NULL)                    //caso não exista uma lista neste elemento do array
        return;

	if(head->next != NULL)              //percorremos todo a lista ligada
		strListDelete(head->next);

    free(head->string);                 //libertamos a string
	free(head);                         //e o elemento em si
}

/*----------------------------------------------------------------------------------------
Nome da função: strSetDelete

Descrição: Esta função elimina o conjunto de strings set, libertando a memória de alojamento
dinâmico usada pelo descritor e pelas strings.

Parâmetros:
strSet_t *set: ponteiro do descritor para o conjunto de strings.

Retorno:
Não tem.
----------------------------------------------------------------------------------------*/

void strSetDelete( strSet_t *set ) {

    int i;
    for(i = 0; i < set->hashSize; i++){              //percorremos todo o array da hash-table   
        strListDelete(set->hashtable[i]);            //libertar a memoria das strings e da lista ligada de cada posição do array

    }
    free(set->hashtable);                            //libertar a memoria alocada para o array da hash-table
    free(set);                                       //libertar memoria do descritor
}

