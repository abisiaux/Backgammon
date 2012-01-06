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
	int quit;
	int nbPlayer;
	
	SGameState *gameState;
	SDL_Event event;
	//SMove move;
	
	if( argc != 2 && argc != 3 )
	{
		fprintf(stderr, "Utilisation du jeu :\n\t 1 Joueur : ./backgammon player_name\n\t2 Joueur : ./backgammon player1_name player2_name\n");
		exit(1);
	}
	
	display_init(&display); /* Initialisation de l'interface graphique */
	gameState = initPartie();
	
	if(argc == 2) // Un joueur
	{
		nbPlayer=1;
		display.player1_name = argv[1];
		display.player2_name = "Ordinateur";
	}
	else // Deux joueurs
	{
		nbPlayer=2;
		display.player1_name = argv[1];
		display.player2_name = argv[2];
	}
	
	quit = 0;

	int withDouble=-1,colorChecker=-1;
	/* Affichage du menu et traitement du codeRetour */
	quit = display_menu(&display,nbPlayer,&colorChecker,&withDouble);

	
	while (quit != -1) // Boucle principale
	{
	
		display_refresh(&display, gameState);
		
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				quit = -1;
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT && zoneQuit(event.button.x,event.button.y))
				{
					quit = -1;
				}
        			break;

		}
		SDL_Flip(display.screen);
	}
	
	/*
	launch_die(gameState);
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
	display_possibilities(&display,gameState, 0);*/
	//sleep(6); // TEST Juste pour voir les possibilités avant de raffraichir l'écran!!
	//display_refresh(&display, gameState);
	
	display_exit(&display);
	return EXIT_SUCCESS;
}
