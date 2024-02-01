all: exec

main.o : main.c Def.h
	gcc -c main.c -o main.o
	


IHM.o :	IHM.c  Def.h
	gcc -c IHM.c -o IHM.o

calcul.o :calcul.c Def.h
	gcc -c calcul.c -o calcul.o


AVL.o :AVL.c Def.h
	gcc -c AVL.c -o AVL.o


exec :main.o IHM.o calcul.o  AVL.o
	gcc *.o -o exec

 clean : 
	rm *.o
	rm exec
