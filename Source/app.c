/* Tendo testado o programa de diversas maneiras concluimos que o trabalho foi conseguido com sucesso visto até ao momento não termos encontrado nenhum
erro. Em termos da hash-table mudamos a estrurua de dados adoptada (em relação ao diagrama) visto agora agruparmos os artistas e os albums no mesmo sitio
Concluimos que o objetivo das estruturas de dados foi comprida pois funcionam como esperado e o nosso programa tem uma melhor eficiencia e ocupa menos
memoria (em relação ao trabalho 2). Em termos do array para a hash-table nos escolhemos alocar memoria dinamicamente com função calloc pois a dimensão
da hash-table vai mudando automaticamente de acordo com o numero de elementos diferentes que encontrar. Numa fase inicial definimos simplesmente um tamanho
de array fixo de 136 elementos (tendo em conta um load factor de 75% e de que existem 102 strings diferentes nos ficheiros), que para o caso em questão
funcionava bem, mas decidimos usar rehashing no nosso trabalho para o tornar mais apto a diversas dimensões (sem ter de se conhecer a lista a priori).
Concluimos que é uma boa abordagem pois é um bom ponto medio entre os compromissos de memoria e eficiencia. Como vimos a hash-table é muito rapida e 
muito eficiente se não houverem muitos conflitos, mas para não haver muitos confliots tem de se aumentar a dimensão da hash-table, e nessa altura estamos a 
ocupar mais memoria, sem um conhecimento previo de quantos elementos queremos guardar, escolher um tamanho fixo para o array não nos pareceu a escolha
mais adequada, e portanto foi por isso que optamos por criar uma função de rehashing, assim neste caso se a lista for pequena a dimensão do array não 
vai ser muito grande (escolhemos um tamanho default para o array de 35 elementos) tal como se a lista for muito grande não vão existir muitos conflitos. */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "strset.h"
#include "setdata.h"
#include "listfiles.h"

#define CMD_BUFFER 32 //buffer para o comando introduzido (o caso maximo é 1 caracter do comando s, 1 espaço em branco e depois uma palavra, com um maximo de 30 caracteres)

int main(int argc, char *argv[]) {


    if(argc == 1) {         //Se argc for igual a 1 então não existem argumentos depois do nome do programa
		fprintf(stderr, "Por favor indique o nome do ficheiro de entrada.\n"); 
		exit(1);
	}
    else if(argc > 2) {     //Se argc for amior que  2, estamos a passar demasiados argumentos na linha de comando
        fprintf(stderr, "Demasiados argumentos.\n");
		exit(1);
    }

    TagSet_t *set = setCreate();                    //ciar o descritor do conjunto de tags.
    if( set != NULL ) {

        strSet_t *sSet = strSetCreate();            //criar o descritor com as strings dos artistas e albums.
        if(sSet == NULL)                            //Verificar se foi possivel alocar memoria para o sSet
            printf("Erro no ficheiro %s, linha %d.\n", __FILE__, __LINE__), exit(1);

        int list = listScan( argv[1], set, sSet);
        if( !list ) { 
            setReady( set ); //preparar os dois arrays para casos possiveis ("a", "t" e "s word")
            if(set != NULL) {

                char temp[CMD_BUFFER+1];
                char *CMD;

                char *cmdLine = calloc(CMD_BUFFER+1, sizeof(char));     //buffer para a função fgets
                if(cmdLine == NULL)
                    printf("Erro no ficheiro %s, linha %d.\n", __FILE__, __LINE__), exit(1);
                
                do { //vamos permanecer neste loop do while enquanto não for inserido o comando q para terminar o programa
                    printf("\nO que deseja realizar?\n\n\t[a]: Organizar a lista por artistas\n\t[t]: Organizar a lista pelas faixas\n\t[s word]: Procurar a lista das faixas áudio\n\t[q]: Terminar a aplicação\nComando: ");      
                    if(fgets(cmdLine, CMD_BUFFER, stdin) != NULL){
                        if(cmdLine[0] == '\n' || cmdLine[0] == ' '|| cmdLine[0] == '\t') {        //Se o comando for apenas um enter
                            printf("Nenhum comando inserido, por favor tente outra vez\n");
                            continue;                                      //proximo iteração do loop para o utilizador inserir outro comando
                        }
                        if(cmdLine[strlen(cmdLine) - 1] != '\n'){          //verificar se o comando inserido não excede o limite pretendido
                            bufferControl();                               //funçãoo de controlo de error desenvolvida no módulo setdata.c
                            continue;                                      //proximo iteração do loop para o utilizador inserir outro comando
                        }
                    }
                    else 
                        printf("Não foi possível ler o comando\n");

                    strcpy(temp, cmdLine);               //copiar a string para não se perder a string original
                    CMD = strtok( temp, " \t\n" );       //buscar apenas a primeira palavra do comando inserida

                    if(!strcmp(CMD, "q"))
                        break;                           //fazemos um break para não imprimir mais nada

                    if( (!strcmp(CMD, "a")) || (!strcmp(CMD, "t")) || (!strcmp(CMD, "s")) ) { //se o comando inserido for a ou t então apresentamos a informação pretendida
                        setCommand( set, cmdLine );
                    }
                    else {
                        printf("Comando invalido, por favor tente outra vez\n"); //se o comando inserido não for a ou t (ou q)
                    }
                
                }while(strcmp(CMD, "q"));
                            
            free(cmdLine); //libertar a memoria alocada para o cmdLine
            printf("Programa terminado, obrigado\n");
            }
        }
    
        else if(list == -1) {
            fprintf(stderr, "Falha na leitura da lista.\n");
		    exit(2);
        }
        
        else if(list == -2){
            fprintf(stderr, "Erro, não existem ficheiros MP3\n");
		    exit(2);
        }

    setDelete( set );
    }

    else {
        fprintf(stderr, "Falha na alocação de memoria.\n");
		exit(3);
    }

    return 0;
}

