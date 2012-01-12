#include <stdlib.h>
#include <stdio.h>

#include <SDL/SDL.h>

#include "../include/definitions.h"
#include "../include/backgammon.h"


int main(int argc, char **argv)
{
	SAI_Functions* ai_struct = Init_SAI();
	
	EGameMode gameMode = Check_Args( argc, argv, ai_struct);

	
	SDisplay display;
	SGame game;
	
	Display_Init(&display,&game); /* Initialisation de l'interface graphique */
	
	switch(gameMode)
	{
		case HUMAN_HUMAN :
			sprintf(game.player1_name,"Joueur1");
			sprintf(game.player2_name,"Joueur2");
			break;
		case HUMAN_AI :
			sprintf(game.player1_name,"Joueur");
			sprintf(game.player2_name,"Ordinateur");
			break;
		case AI_AI :
			sprintf(game.player1_name,"Ordinateur1");
			sprintf(game.player1_name,"Ordinateur2");
			break;
		default :
			fprintf(stderr, "Les consignes d'utilisations du jeu sont definies dans le README.\n");
			exit(EXIT_FAILURE);
			break;
	}
	
	
	SDL_Event event;
	int quit = 0;
	
	
	/* Affichage du menu */

	quit = Display_Menu(&display,gameMode, &game);
	
	while (!quit) // Boucle principale
	{
		SDL_WaitEvent(&event);
		
		quit = Game_Play( &display, gameMode, &game);
		
		switch(event.type)
		{
			case SDL_QUIT:
				quit = 1;
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT && Quit_Zone(event.button.x,event.button.y))
				{
					quit = 1;
				}
		  			break;

		}
		
		
	}
	
	Free_SAI(ai_struct);
	
	Display_Exit(&display);
	
	exit(EXIT_SUCCESS);
}
