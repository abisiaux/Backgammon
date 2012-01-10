#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <SDL/SDL.h>

#include "../include/gui.h"
#include "../include/backgammon.h"
#include "../include/arbitrator.h"
#include "../include/library.h"

int main(int argc, char **argv)
{
	S_AI_Functions* ai_struct = Init_S_AI();
	
	E_GameMode gameMode = Check_Args( argc, argv, ai_struct);

	
	SDisplay display;
	
	Display_Init(&display); /* Initialisation de l'interface graphique */
	
	switch(gameMode)
	{
		case HUMAN_HUMAN :
			sprintf(display.game->player1_name,"Joueur1");
			sprintf(display.game->player2_name,"Joueur2");
			break;
		case HUMAN_AI :
			sprintf(display.game->player1_name,"Joueur");
			sprintf(display.game->player2_name,"Ordinateur");
			break;
		case AI_AI :
			sprintf(display.game->player1_name,"Ordinateur1");
			sprintf(display.game->player1_name,"Ordinateur2");
			break;
		default :
			fprintf(stderr, "Les consignes d'utilisations du jeu sont definies dans le README.\n");
			exit(EXIT_FAILURE);
			break;
	}
	
	SGameState *gameState;
	
	gameState = Game_Init(); /* Initialisation de la partie */
	
	SDL_Event event;
	SMove move;
	EPlayer curentPlayer = EPlayer2;
	
	int quit = 0;

	char tmp[100];
	SDL_Rect msg_position;
	
	msg_position.x = 115;
	msg_position.y = 325;
	
	SDL_Color msg_color = {255, 255, 255, 0};
	
	/* Affichage du menu */
	//quit = Display_Menu(&display,gameMode);
	Display_Refresh(&display, gameState);
	
	
	
	while (!quit) // Boucle principale
	{printf("BOUCLE\n");
		
		
		/*while(!quit) // Boucle de Match
		{*/
			/*printf("AVANT JOUEUR");
			if(curentPlayer == EPlayer1)
			{
				curentPlayer = EPlayer2;
				sprintf(tmp,"%s, c'est a toi de jouer.",display.game->player2_name);
				Display_Message(&display, tmp, msg_position, msg_color,1);
				SDL_Delay(2000);
			}
			else
			{
				curentPlayer = EPlayer1;
				sprintf(tmp,"%s, c'est a toi de jouer.",display.game->player1_name);
				Display_Message(&display, tmp, msg_position, msg_color,1);
				SDL_Delay(2000);
			}*/
			//Display_Refresh(&display, gameState);
			
			//Launch_Die(gameState);
			//Display_Die(&display,gameState);
			
			int nonJoue=1;
			while(nonJoue)//Tant que le joueur n'a pas joué tous ses coups
			{
				EPosition pos=0;
				SDL_WaitEvent(&event);
				switch(event.type)
				{
					case SDL_QUIT:
						quit = 1;
						break;
					case SDL_MOUSEBUTTONUP:
						;
						if (event.button.button == SDL_BUTTON_LEFT ) //&& Pion_Depart_Autorise(event.button.x,event.button.y)
						{// si le joueur clic sur un des pions correspondant aux pions qui peuvent être déplacés sur ce tour
							printf("CLIC\n");
							
							printf("posSx=%d\tposSy=%d\n",event.button.x,event.button.y);
							if(CheckerWithScreenPosition(event.button.x,event.button.y, &pos))
							{
								printf("Position trouvée, POS =%d\n",pos+1);
								colorChecker(&display, gameState, pos);
								SDL_Flip(display.screen);
								printf("CHECKER COLORE\n");
								SDL_Delay(5000);
								nonJoue=0;
								quit=1;
							}
							else 	printf("Position non trouvée\n");
						}
			  			break;

				}
			
			}
			
			
			
			
			
			/*Display_Possibilities(&display,gameState, 0);
			
			
			move.src_point = 8;
			move.dest_point = 6;
			if(authorized_deplacement(gameState, &move, EPlayer2))// JOUEUR 1 = VERT
			{
				printf("DEPLACEMENT AUTORISE\n");
				Checker_Move(&display,gameState,&move);
		
			}
			else
				printf("EPLACEMENT NON AUTORISE\n");
	
			*/
			SDL_WaitEvent(&event);
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
			Display_Refresh(&display, gameState);
		//}
		SDL_Flip(display.screen);
	}
	
	Display_Exit(&display);
	
	return EXIT_SUCCESS;
}
