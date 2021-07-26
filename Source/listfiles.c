#include <string.h>
#include <stdio.h>
#include "strset.h"
#include "tag.h"
#include "tagtype.h"
#include "setdata.h"
#include "listfiles.h"

/*----------------------------------------------------------------------------------------
Nome da função: listScan

Descrição: Esta função percorre o ficheiro de texto com nome indicado por listName, obtendo os nomes
dos ficheiros MP3, armazenados um em cada linha de texto. Por cada ficheiro MP3, obtém a
informação de tag, usando a função tagRead, e adiciona-a ao conjunto de tags, usando a função
setAdd. 

Parâmetros:
char *listName: indica o ficheiro de texto com os nomes dos ficheiros MP3.
TagSet_t *data: descritor do conjunto de tags.
strSet_t *sSet: descritor com as strings dos artistas e albums.

Retorno:
int: Retorna 0, em caso de sucesso. Em caso de insucesso retorna  -1, em caso de falha na leitura
da lista ou -2 se existe a lista com o nome dos ficheiros MP3 mas não conseguimos guardar a informação
de nenhuma tag.
----------------------------------------------------------------------------------------*/
int listScan( char *listName, TagSet_t *data, strSet_t *sSet ) {
    
    char temp_fileName[MAX_NAME+1];     //buffer para o nome do ficheiro a ler
    int resPtr = 1;                     //inicializado a 1 para não coincidir com nenhum dos codigos de erro ou sucesso
    int tagSuccess = 0;                 //utilizado para contar o numero de tags lidas com sucesso

    FILE *list = fopen(listName, "r" ); //abertura do ficheiro de texto com o nome dos ficheiros
    if(list == NULL) {                 
        return -1;                      //falha na leitura da lista
    }

    while(fgets(temp_fileName, MAX_NAME+1, list) != NULL) {
        char *fileName = strtok(temp_fileName, "\n");       //usado para remover o '\n' do final da string
        
        MP3Tag_t *tag =  tagRead (strtok(temp_fileName, "\n"), &resPtr, sSet); //obtenção da tag de alguma faixa
        setAdd( data, tag);                                 //adicionamos essa tag
        /*sempre que temos sucesso aumentamos o numero da variavel tagSuccess, esta variavel serve para controlar o caso especifico em que 
        temos a lista com o nome dos ficheiros mas não temos nenhum ficheiro mp3 na pasta */
        if(tag != NULL) {
            tagSuccess++;
        }

        //se houve algum erro na tag, apresentamos este atraves do stderr
        if(resPtr == -1)
            fprintf(stderr, "Erro ao abrir o ficheiro: %s\n", fileName);
        
        else if(resPtr == -2)
            fprintf(stderr, "A faixa de audio %s não tem informação na tag\n", fileName);

        else if(resPtr == -3)
            fprintf(stderr, "Erro na alocamento de memória no ficheiro: %s\n", fileName);
    }
    // se nenhuma tag for lida com sucesso, então retornamos o codigo de erro -2
    if(!tagSuccess) {
        return -2;
    }

    return 0; //retornamos 0 em caso de sucesso
}

