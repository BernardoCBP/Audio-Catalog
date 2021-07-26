#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tagtype.h"
#include "tag.h"
#include "strset.h"

/*----------------------------------------------------------------------------------------
Nome da função: tagRead

Descrição: Esta função lê a tag existente no ficheiro com o nome indicado por fileName, 
e armazena-a em memória alojada dinamicamente e retorna o seu endereço.

Parâmetros:
char *fileName: indica o nome do ficheiro onde vamos ler a tag
int *resPtr: indica uma variável que deve ser afetada com um código de resultado
strSet_t *sSet: descritor com as strings dos artistas e albums.

Retorno:
MP3Tag_t *: endereço da tag preenchida em caso de sucesso ou NULL caso contrário.
----------------------------------------------------------------------------------------*/
MP3Tag_t *tagRead( char *fileName, int *resPtr, strSet_t *sSet ) {

    char verify[VERIFY_TAG];      //array para guardar os 3 primeiros bytes a partir do ponto de offset do fseek
    char year[YEAR+1];            //string para guardar os 4 bytes que representam o year
    char tempString[MAX_TEMP+1];  //string para guardar temporariamente alguns parametros
 
    FILE *audio = fopen(fileName, "rb" );
    if(audio == NULL) {                 //falha ao aceder ao ficheiro
        if(resPtr != NULL) 
            *resPtr = -1;                           
        return NULL;
    }  

    fseek(audio, -TAG_SIZE, SEEK_END);  //verificar os ultimos 128 bytes do ficheiro
    fread(verify, sizeof(char), 3, audio); //copiar os 3 primeiros bytes a partir do ponto de offset do fseek

    /******************verificar se existe tag na faixa ****************************/
    if(verify[0] == 'T' && verify[1] == 'A' && verify[2] == 'G') {  //se os 3 bytes lidos forem "TAG" então temos uma tag e queremos ler a informação

        MP3Tag_t *tag = malloc(sizeof (MP3Tag_t)); //alocação de memoria com o tamanho da nossa estrutura
        if(tag == NULL){                 //se houver erro na alocação de memoria
            if(resPtr != NULL) 
                *resPtr = -3;            //por iniciativa atribuimos o codigo de erro -3 se houve erro não foi possivel alocar memoria
            fclose(audio);               //fechar o ficheiro antes de retornar NULL
            
            return NULL;
        }

        fseek(audio, -TAG_INFO, SEEK_END);  //offset de -125 para ir buscar a informação da tag

        /*quando não existe o nome do titulo na tag, os 30 bytes podem estar preenchidos com 0's ou com espaços, portanto vamos verificar
        se o inicio são 0's em binario ou se os dois primeiros bytes são espaços(há uma musica em que o primeiro byte é um espaço, a sample0415)
        nesse caso, por convenção nossa vamos escrever no primeiro byte o caracter '~' que é o penultimo caracter na tabela de ASCII, isto serve
        para depois ser mais facil detetar as situações de erro, e para mais facil organização por titulos. Para o casos dos artists e albums
        a situação é a mesma */
        
        /******************título da faixa ****************************/
        fread(tag->title, sizeof(char), MAX_TIT, audio);
        if((tag->title[0] == ' ' && tag->title[1] == ' ') || tag->title[0] == '\0')
            tag->title[0] = '~';

        /******************artista da faixa ****************************/
        fread(tempString, sizeof(char), MAX_ART, audio);
        tag->artist = strSearchAdd(sSet, tempString);
        if((tag->artist[0] == ' ' && tag->artist[1] == ' ') || tag->artist[0] == '\0')
            tag->artist[0] = '~';

        /******************album da faixa ****************************/
        fread(tempString, sizeof(char), MAX_ALB, audio);
        tag->album = strSearchAdd(sSet, tempString);
        if((tag->album[0] == ' ' && tag->album[1] == ' ') || tag->album[0] == '\0')
            tag->album[0] = '~';

        /******************ano da faixa ****************************/
        fread(year, sizeof(char), 4, audio); //ler os 4 bytes do ano
        year[YEAR] = '\0';                   //ultimo byte a '\0' para "criar" uma string
        tag->year = (short)atoi(year);       //função que transforma uma string num valor inteiro, retorna 0 se não conseguir

        /*lemos os 30 bytes que é o maximo que o comentario pode ter e depois vamos verificar se estamos em ID3v1 ou em ID3v1.1 usando o byte 28 
        da string e verificando se é '\0' se for sabemos que estamos em ID3v1.1 e então o ultimo byte do comentario esta reservado para o 
        numero da track e o comentario vai apenas ter no maximo 28 bytes, se nao for '\0' então tamos em ID3v1 e o comentario pode ocupar os 30 bytes
        Tal como nos casos anteriores, tentamos verificar se os comentarios não estavam vazios, desta vez optando por atribuir o valor 0 ao 
        primeiro byte do comment nos casos em que não temos comentario*/

        /******************comentario e track da faixa ****************************/
        fread(tempString, sizeof(char), MAX_COM, audio);
        if(tempString[0] == ' ' && tempString[1] == ' ') { 
            if(tempString[MAX_COM_ID3v1_1] == 0){
                tag->track = tempString[29];
                tag->comment[0] = '\0';
            }
            else {
                tag->track = '\0';
                tag->comment[0] = '\0';
            }
        }

        else{
            if(tempString[0] != '\0') {
                if(tempString[MAX_COM_ID3v1_1] == '\0'){
                    tag->track = tempString[29];
                    strncpy(tag->comment, tempString, MAX_COM_ID3v1_1);
                }
                else{
                    tag->track = '\0'; //para ser mais facil depois de identificar o que aconteceu
                    strcpy(tag->comment, tempString);
                }
            }
            else{
                tag->track = '\0';
                tag->comment[0] = '\0';      
            }
        }

        /******************genero da faixa ****************************/
        tag->genre = fgetc(audio); //vamos buscar só um byte que representa o genero do ficheiro
        if(tag->genre == ' ') {
            tag->genre = '\0';
        }

        /******************string com o nome do ficheiro que contem a faixa ****************************/
        tag->fileName = strdup(fileName); //utilizamos a função strdup para alocar a string com o nome do ficheiro, em alojamento dinâmico.
        
        if(resPtr != NULL)  //se nao for null tivemos sucesso
            *resPtr = 0;

        fclose(audio);

        return tag;  //retornar o endereço da estrutura criada
    }
    
    //se não demos nenhum return até agora então é porque o file não tinha TAG então retornamos NULL e se resPtr nao for NULL atribuimos o codigo de erro -2
    if(resPtr != NULL) 
        *resPtr = -2;
    
    fclose(audio);

    return NULL;
}
