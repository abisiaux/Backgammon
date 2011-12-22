#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
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
	move.src_point = 8;
	move.dest_point = 19;
	checker_move(&display,gameState,&move);
	
	launch_die(gameState);
	display_refresh(&display, gameState);
	display_possibilities(&display,gameState, 1);
	sleep(60); // TEST Juste pour voir les possibilités avant de raffraichir l'écran!!
	display_refresh(&display, gameState);
	
	
	while (!quit) // Boucle principale
	{
	
		//display_refresh(&display, gameState);
		
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
