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
	
	display_init(&display);
	
	launch_die(gameState);
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
