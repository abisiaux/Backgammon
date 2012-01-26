#include <stdlib.h>
#include <stdio.h>

#include <SDL/SDL.h>

#include "../include/definitions.h"
#include "../include/backgammon.h"


int main(int argc, char **argv)
{
	
	SIA_Functions* ia_struct = Init_SAI();
	
	EGameMode gameMode = Check_Args( argc, argv, ia_struct);
	
	SDisplay display;
	SGame game;
	
	Display_Init(&display,&game); /* Initialisation de l'interface graphique */
	
	
	
	switch(gameMode)
	{
		case HUMAN_HUMAN :
			sprintf(game.player1_name,"Joueur1");
			sprintf(game.player2_name,"Joueur2");
			break;
		case HUMAN_IA :
			sprintf(game.player1_name,"Joueur");
			sprintf(game.player2_name,"Ordinateur");
			break;
		case IA_IA :
			sprintf(game.player1_name,"Ordinateur1");
			sprintf(game.player2_name,"Ordinateur2");
			break;
		default :
			fprintf(stderr, "Les consignes d'utilisations du jeu sont definies dans le README.\n");
			exit(EXIT_FAILURE);
			break;
	}
	
	
	SDL_Event event;
	int quit = 0;
	
	
	/* Affichage du menu */

	quit = Menu_Display(&display,gameMode, &game);
	
	while (!quit) // Boucle principale
	{
		SDL_WaitEvent(&event);
		
		quit = Game_Play( &display, gameMode, &game, ia_struct);
		
		switch(event.type)
		{
			case SDL_QUIT:
				quit = 1;
				break;

		}
		
		
	}
	
	Free_SAI(ia_struct);
	
	Display_Exit(&display);
	
	exit(EXIT_SUCCESS);
}
