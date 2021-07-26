#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "setdata.h"
#include "binarytree.h"

/*----------------------------------------------------------------------------------------
Nome da função: setCreate

Descrição: Esta função cria o descritor de conjunto de tags no estado inicial, pronto para adicionar a informação
das tags. Começamos por alocar memoria para o set com a funcao malloc, se conseguimos alocar a memoria então criamos 
o descritor de conjunto de tags no estado inicial, ou seja, o size e count a 0, e o data e o aux com NULL

Parâmetros:
Não tem.

Retorno:
TagSet_t *: endereço do descritor alojado dinamicamente ou NULL em caso de insucesso
----------------------------------------------------------------------------------------*/
TagSet_t *setCreate( void ) {

    TagSet_t *set = malloc(sizeof (TagSet_t));      //alocação de memoria para o set
    if(set == NULL)                                 //Verificar se foi possivel alocar memoria para o set
        return NULL;

    set->size = 0;
    set->count = 0;
    set->data = NULL;
    set->aux = NULL;
    set->root = NULL;

    return set;                                     //se tivermos sucesso retornamos o endereço do descritor criado
}

/*----------------------------------------------------------------------------------------
Nome da função: setAdd

Descrição: Esta função adiciona ao descritor, indicado por set, uma tag, indicada por tag. 
Sempre que adicionamos uma tag ao descritor temos de adaptar automaticamente a quantidade 
de faixas de audio catalogadas, fazemos isto com recurso a funcao realloc da biblioteca normalizada

Parâmetros:
TagSet_t *set: descritor do conjunto de tags
MP3Tag_t *tag: tag a adicionar

Retorno:
Não tem.
----------------------------------------------------------------------------------------*/
void setAdd( TagSet_t *set, MP3Tag_t *tag ) {

    set->data = realloc(set->data, sizeof(MP3Tag_t *)*(set->size)+1 );

    if(set->data != NULL) { //se conseguimos alocar a memoria
        if(tag != NULL) {  //se o endereço da tag recebida não for nula
            set->data[set->count] = tag; //atribuimos o endereço da tag nova ao array data
            set->count++;  //a quantidade de delementos preenchidos em data aumenta se tivemos sucesso
        }
    set->size++; //a quantidade de delementos alojados em data aumenta sempre que tentamos adicionar uma tag
    }
}

/*----------------------------------------------------------------------------------------
Nome da função: compareArtist

Descrição: Função de comparação dos artist para usar como ultimo parametro da função qsort. 
Usamos a função strcmp da biblioteca de c para realizar a comparação. Se os artistas forem diferentes, 
retornamos a comparação desses artistas, se forem iguais então vamos comparar os albuns, se estes forem 
diferentes retornamos a comparação dos albuns, se forem iguais retornamos a comparação do nome da faixa de audio

Parâmetros:
const MP3Tag_t **ptr1: ponteiro para a primeira tag a comparar
const MP3Tag_t **ptr2: ponteiro para a segunda tag a comparar

Retorno:
int: comparação entre as string introduzidas
----------------------------------------------------------------------------------------*/
static int compareArtist(const MP3Tag_t **ptr1, const MP3Tag_t **ptr2) {

    if(strcmp((*ptr1)->artist, (*ptr2)->artist)) 
        return strcmp((*ptr1)->artist, (*ptr2)->artist);
    
    else if(strcmp((*ptr1)->album, (*ptr2)->album)) 
        return strcmp((*ptr1)->album, (*ptr2)->album);

    return strcmp((*ptr1)->title, (*ptr2)->title);     
}

/*----------------------------------------------------------------------------------------
Nome da função: compareTitle

Descrição: Função idêntica em funcionamento a anterior mas agora comparando os titulos de 
cada faixa de música.

Parâmetros:
const MP3Tag_t **ptr1: ponteiro para a primeira tag a comparar
const MP3Tag_t **ptr2: ponteiro para a segunda tag a comparar

Retorno:
int: comparação entre as string introduzidas
----------------------------------------------------------------------------------------*/
static int compareTitle(const MP3Tag_t **ptr1, const MP3Tag_t **ptr2) {

    if(strcmp((*ptr1)->title, (*ptr2)->title)) 
        return strcmp((*ptr1)->title, (*ptr2)->title);
    
    else if(strcmp((*ptr1)->artist, (*ptr2)->artist)) 
        return strcmp((*ptr1)->artist, (*ptr2)->artist);

    return strcmp((*ptr1)->album, (*ptr2)->album);
}

/*----------------------------------------------------------------------------------------
Nome da função: setReady

Descrição: Esta função marca como concluída a fase de inserção das tags e prepara o descritor 
do conjunto para dar as respostas pretendidas, na fase de comandos. No nosso caso a preparação
consiste em ordenar o array data e aux como pretendido e a criação da árvore binária para pesquisa.

Parâmetros:
TagSet_t *set: descritor do conjunto de tags

Retorno:
Não tem.
----------------------------------------------------------------------------------------*/
void setReady( TagSet_t *set ){

    if(set->data != NULL) { //so queremos criar o array aux se tivermos um array data

        int i = 0;

        set->aux = malloc(sizeof(MP3Tag_t *)*set->size);
        if(set->aux != NULL){
            for(i = 0; i < set->size; i++) { //copiar todos os elementos do array data para o array aux
                set->aux[i] = set->data[i];
            }
            
            qsort(set->data, set->count, sizeof(MP3Tag_t *), (int (*)(const void*, const void*))compareArtist);
            qsort(set->aux, set->count, sizeof(MP3Tag_t *), (int (*)(const void*, const void*))compareTitle);
        }
    
        createBinaryTree(set);
    }
}

/*----------------------------------------------------------------------------------------
Nome da função: setCommand

Descrição: Esta função apresenta, em standard output, os resultados do comando inserido 
pelo utilizador. Usamos a convenção definidos por nós no modulo tag.c para identificar 
melhor erros e fazer print so da informação que achamos relevante em cada caso.

Parâmetros:
TagSet_t *set: descritor do conjunto de tags
char *cmdLine: linha de comando escolhida

Retorno:
Não tem.
----------------------------------------------------------------------------------------*/
/* Nesta função usamos a convenção definidos por nós no modulo tag.c para identificar melhor erros e fazer print so da informação 
que achamos relevante em cada caso */
void setCommand( TagSet_t *set, char *cmdLine ) {

    int i = 0;
    char *r_word = strtok( cmdLine, " \t\n" ); //vamos buscar o comando introduzido

    if(!strcmp(r_word, "a")){
        
        if(strtok( NULL, " \t\n" ) == NULL) {       //verificar se só foi inserido o comando "a" e mais nada
            for(i = 0; i < set->count; i++) {
                printf("Artist: %-30s  ", (set->data[i]->artist[0] != '~' ? set->data[i]->artist  : NO_INFO) );
                printf("Album: %-30s  ", (set->data[i]->album[0] != '~' ? set->data[i]->album  : NO_INFO) );
                printf("Title: %-30s  ", (set->data[i]->title[0] != '~' ? set->data[i]->title  : NO_INFO) );

                if(set->data[i]->year == 0)
                    printf("Year: %-7s  ", NO_INFO);
                else
                    printf("Year: %-7hi  ", set->data[i]->year);

                printf("Comment: %-30s  ", (set->data[i]->comment[0] != '\0' ? set->data[i]->comment : NO_INFO) );

                if(set->data[i]->track == '\0')
                    printf("Track: %-7s  ", NO_INFO);
                else
                    printf("Track: %-7d  ", set->data[i]->track);

                if(set->data[i]->genre == -1)
                    printf("Genre: %-7s  ", NO_INFO);
                else
                    printf("Genre: %-7d  ", set->data[i]->genre);
                
                printf("FileName: %s\n", set->data[i]->fileName);
            }
        }
        else
            printf("Comando invalido, por favor tente outra vez\n");
    }

    else if(!strcmp(r_word, "t")) {

        if(strtok( NULL, " \t\n" ) == NULL) {       //verificar se só foi inserido o comando "t" e mais nada
            for(i = 0; i < set->count; i++) {
                printf("Title: %-30s  ", (set->aux[i]->title[0] != '~' ? set->aux[i]->title  : NO_INFO) );
                printf("Artist: %-30s  ", (set->aux[i]->artist[0] != '~' ? set->aux[i]->artist  : NO_INFO) );
                printf("Album: %-30s  ", (set->aux[i]->album[0] != '~' ? set->aux[i]->album  : NO_INFO) );
                
                if(set->aux[i]->year == 0)
                    printf("Year: %-7s  ", NO_INFO);
                else
                    printf("Year: %-7hi  ", set->aux[i]->year);

                printf("Comment: %-30s  ", (set->aux[i]->comment[0] != '\0' ? set->aux[i]->comment : NO_INFO) );
            
                if(set->aux[i]->track == '\0')
                    printf("Track: %-7s  ", NO_INFO);
                else
                    printf("Track: %-7d  ", set->aux[i]->track);

                if(set->aux[i]->genre == -1)
                    printf("Genre: %-7s  ", NO_INFO);
                else
                    printf("Genre: %-7d  ", set->aux[i]->genre);
                
                printf("FileName: %s\n", set->aux[i]->fileName);
            }
        }
        else
            printf("Comando invalido, por favor tente outra vez\n");
    }

    else if(!strcmp(r_word, "s")) {
        r_word = strtok( NULL, " \t\n" );                 //De seguida vamos buscar a palavra seguinte do cmdLine, que neste caso será a palavra a procurar
        if(r_word != NULL) {                              //verificar se foi mesmo inserida a palavra
            int check = findNode(set->root, r_word);      //Tentar encontrar a palavra inserida na árvore
            if(!check)
                printf("Palavra não encontrada\n");       //A palavra não existe na árvore
        }
        else 
            printf("Porfavor introduza uma palavra para procurar\n");   
    }
}

/*----------------------------------------------------------------------------------------
Nome da função: bufferControl

Descrição: Esta função ajuda a gerir melhor os comandos introduzidos pela função fgets, 
implementando isto no modulo principal podemos prevenir a situação em que temos demasiados
caracteres no buffer.

Parâmetros:
Não tem.

Retorno:
Não tem.
----------------------------------------------------------------------------------------*/
void bufferControl(){

    fprintf(stderr, "O comando excedeu o comprimento máximo, por favor tente outra vez\n");

    char c = '\0'; 
    while ((c = getchar()) != '\n' && c != EOF) { //vamos buscara todos os caracteres que restaram no buffer
    }

}

/*----------------------------------------------------------------------------------------
Nome da função: setDelete

Descrição: Esta função, usada em consequência do comando “q”, elimina o descritor de conjunto 
de tags e as estruturas de dados dependentes dele, libertando a respetiva memória de alojamento 
dinâmico. Neste caso usamos a função free para libertar a memoria alocada ao longo do programa.


Parâmetros:
TagSet_t *set: descritor do conjunto de tags

Retorno:
Não tem.
----------------------------------------------------------------------------------------*/
void setDelete( TagSet_t *set ){

    int i = 0;

    for(i = 0; i < set->count; i++){ //libertar a memoria da string com o nome do ficheiro
        free(set->data[i]->fileName);
    }

    for(i = 0; i < set->count; i++){ //libertar a memoria de cada tag
        free(set->data[i]);
    }
    
    freeTree(set->root);    
    free(set->data);
    free(set->aux);

    free(set);
}
