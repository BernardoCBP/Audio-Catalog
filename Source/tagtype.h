#ifndef TAGTYPE_H
#define TAGTYPE_H 

#define MAX_TIT 30
#define MAX_ART 30
#define MAX_ALB 30

#define MAX_TEMP 30        //maximo para uma string temporaria

#define MAX_COM 30
#define MAX_COM_ID3v1_1 28 //Comentario maximo em ID3v1.1

typedef struct{
    char title[MAX_TIT + 1];
    char *artist;
    char *album;
    short year;
    char comment[MAX_COM + 1];
    char track;
    char genre;
    char *fileName; 
} MP3Tag_t;

#endif
