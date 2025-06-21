all: F1 clean
F1: F1.o fonctions.o
	gcc F1.o fonctions.o -o F1 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2 -lm
	
F1.o : F1.c includeX.h
	gcc -c F1.c -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2 -lm

fonctions.o : fonctions.c includeX.h 
	gcc -c fonctions.c -lm
clean :
	rm -f *.o