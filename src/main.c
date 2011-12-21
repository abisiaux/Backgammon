#include <stdlib.h>
#include <stdio.h>

#include <SDL/SDL.h>

#include "../include/gui.h"
#include "../include/backgammon.h"

int main(int argc, char **argv)
{
	SDisplay display;
	
	SGameState *gameState = initPartie();
	
	int quit = 0;
	SDL_Event event;
	
	SMove move;
	
	display_init(&display);
	
	/*Test Cas1*/
	move.src_point = 12;
	move.dest_point = 1;
	checker_move(&display,gameState,&move);
	
	/*Test Cas2*/
	move.src_point = 6;
	move.dest_point = 8;
	checker_move(&display,gameState,&move);
	
	/*Test Cas3*/
	move.src_point = 17;
	move.dest_point = 24;
	checker_move(&display,gameState,&move);
	
	/*Test Cas4*/
	move.src_point = 19;
	move.dest_point = 14;
	checker_move(&display,gameState,&move);
	
	/*Test Cas5*/
	move.src_point = 13;
	move.dest_point = 8;
	checker_move(&display,gameState,&move);
	
	/*Test Cas6*/
	move.src_point = 12;
	move.dest_point = 19;
	checker_move(&display,gameState,&move);
	
	/*Test Cas7*/
	move.src_point = 1;
	move.dest_point = 19;
	checker_move(&display,gameState,&move);
	
	/*Test Cas8*/
	move.src_point = 24;
	move.dest_point = 8;
	checker_move(&display,gameState,&move);
	//launch_die(gameState);
		
	while (!quit) // Boucle principale
	{
		display_refresh(&display, gameState);
		
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				quit = 1;
				break;
		}
		SDL_Flip(display.screen);
	}
	
	display_exit(&display);
	return EXIT_SUCCESS;
}
