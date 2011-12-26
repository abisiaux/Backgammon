#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <SDL/SDL.h>

#include "../include/gui.h"
#include "../include/backgammon.h"
#include "../include/arbitrator.h"

int main(int argc, char **argv)
{
	SDisplay display;
	
	SGameState *gameState = initPartie();
	
	int quit = 0;
	SDL_Event event;
	
	SMove move;
	
	display_init(&display);
	launch_die(gameState);
	/*Test Cas1*/
	move.src_point = 8;
	move.dest_point = 6;
	if(authorized_deplacement(gameState, &move, EPlayer2))// JOUEUR 1 = VERT
	{
		printf("DEPLACEMENT AUTORISE\n");
		checker_move(&display,gameState,&move);
		
	}
	else
		printf("EPLACEMENT NON AUTORISE\n");
	
	
	display_refresh(&display, gameState);
	display_possibilities(&display,gameState, 0);
	//sleep(6); // TEST Juste pour voir les possibilités avant de raffraichir l'écran!!
	//display_refresh(&display, gameState);
	

	
	
	
	while (!quit) // Boucle principale
	{
	
		//display_refresh(&display, gameState);
		
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				quit = 1;
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT && zoneQuit(event.button.x,event.button.y))
				{
					quit = 1;
				}
        			break;

		}
		SDL_Flip(display.screen);
	}
	
	display_exit(&display);
	return EXIT_SUCCESS;
}
