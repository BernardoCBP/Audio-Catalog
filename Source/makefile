# Isto e´ um comentario

app: app.o listfiles.o setdata.o binarytree.o linkedlist.o tag.o strset.o
	gcc -g app.o listfiles.o setdata.o binarytree.o linkedlist.o tag.o strset.o -o app 

app.o: app.c listfiles.h setdata.h tagtype.h tag.h
	gcc -g -c app.c -Wall

listfiles.o: listfiles.c listfiles.h setdata.h tagtype.h tag.h
	gcc -g -c listfiles.c -Wall

setdata.o: setdata.c setdata.h binarytree.h tagtype.h
	gcc -g -c setdata.c -Wall

binarytree.o: binarytree.c binarytree.h linkedlist.h tagtype.h
	gcc -g -c binarytree.c -Wall

linkedlist.o: linkedlist.c linkedlist.h tagtype.h
	gcc -g -c linkedlist.c -Wall

tag.o: tag.c tag.h tagtype.h
	gcc -g -c tag.c -Wall

strset.o: strset.c strset.h 
	gcc -g -c strset.c -Wall

