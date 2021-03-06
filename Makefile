#Makefile Backgammon

# Définition des variables
CC=gcc
CFLAGS=-W -Wall -std=c99 -Wswitch -Wunused-parameter
LDFLAGS=-lSDL -lSDL_image -lSDL_ttf -ldl -g
EXEC=backgammon

# Définition des paths
SRC=src/
INC=include/
BIN=bin/
LIB=lib/
OBJ=obj/

all: ${EXEC}

${EXEC}: ${OBJ}main.o ${OBJ}gui.o ${OBJ}arbitrator.o ${OBJ}library.o ${OBJ}menu.o ${OBJ}game.o
			${CC} -o ${BIN}$@ $^ ${LDFLAGS}

#Règle pour compiler l'interface graphique 
${OBJ}gui.o:  	${SRC}gui.c
					${CC} -o $@ -c $^ ${CFLAGS}

#Règle pour compiler le moteur de jeu
${OBJ}game.o:  	${SRC}game.c
					${CC} -o $@ -c $^ ${CFLAGS}

#Règle pour compiler le menu
${OBJ}menu.o:  	${SRC}menu.c
					${CC} -o $@ -c $^ ${CFLAGS}
					
# Règle pour compiler le programme
${OBJ}main.o: ${SRC}main.c
					${CC} -o $@ -c $^ ${CFLAGS}

# Règle pour compiler l'arbitre
${OBJ}arbitrator.o: ${SRC}arbitrator.c
					${CC} -o $@ -c $^ ${CFLAGS}

# Règle pour compiler la librairie
${OBJ}library.o: ${SRC}library.c
					${CC} -o $@ -c $^ ${CFLAGS}
					
clean:
			rm ${OBJ}*.o
			#rm ${LIB}*.o
			
mrproper:	
				make clean
				rm ${BIN}${EXEC}
