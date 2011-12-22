#Makefile Backgammon
# crée le 20/12/11 à 18h52 par Bibiz
# à modifier avec l'ajout de la librairie

# Définition des variables
CC=gcc
CFLAGS=-W -Wall -std=c99
LDFLAGS=-lSDL -lSDL_image -lSDL_ttf 
EXEC=backgammon

# Définition des paths
SRC=src/
INC=include/
BIN=bin/
LIB=lib/
OBJ=obj/

all: ${EXEC}

${EXEC}: ${OBJ}main.o ${OBJ}gui.o
			${CC} -o ${BIN}$@ $^ ${LDFLAGS}

#Règle pour compiler l'interface graphique 
${OBJ}gui.o:  	${SRC}gui.c
					${CC} -o $@ -c $^ ${CFLAGS}
					
# Règle pour compiler le programme
${OBJ}main.o: ${SRC}main.c
					${CC} -o $@ -c $^ ${CFLAGS}

clean:
			rm ${OBJ}*.o
			#rm ${LIB}*.o
			
mrproper:	
				make clean
				rm ${BIN}${EXEC}
