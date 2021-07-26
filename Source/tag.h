#ifndef TAG_H
#define TAG_H

#include "tagtype.h"
#include "strset.h"

#define YEAR 4

#define TAG_SIZE 128 //offset da tag
#define TAG_INFO 125 //offset com a informação dos campos da tag
#define VERIFY_TAG 2 //Utilizado para guardar e verificar os primeiros 3 bytes da tag


MP3Tag_t *tagRead( char *, int *, strSet_t * );

#endif
