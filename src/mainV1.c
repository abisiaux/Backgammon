#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <SDL/SDL.h>

#include "../include/gui.h"
#include "../include/backgammon.h"
#include "../include/arbitrator.h"
#include "../include/library.h"
#include "../include/menu.h"

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
	EPlayer curentPlayer = EPlayer2;
	
	int quit = 0;

	char tmp[100];
	SDL_Rect msg_position;
	SMove mouvement;
	
	msg_position.x = 80;
	msg_position.y = 325;
	
	SDL_Color msg_color = {255, 255, 255, 0};
	
	/* Affichage du menu */

	quit = Display_Menu(&display,gameMode);

	Display_Refresh(&display, gameState);
	Display_Message(&display, "La partie commence !", msg_position, msg_color, 1);
	SDL_Flip(display.screen);
	SDL_Delay(1500);
	
	while (!quit) // Boucle principale
	{
		Display_Refresh(&display, gameState);
		SDL_WaitEvent(&event);
		
		switch (gameMode)
		{
			case HUMAN_HUMAN :
				if(curentPlayer == EPlayer2)
				{
					curentPlayer = EPlayer1;
					sprintf(tmp,"C'est au tour de %s",display.game->player1_name);
					Display_Message(&display, tmp, msg_position, msg_color,1);
					SDL_Delay(2000);
					sprintf(tmp,"C'est au tour de %s",display.game->player1_name);
					quit = Display_Message_Click(&display, "Lancer les des", msg_position, msg_color,1);
					Launch_Die(gameState);
					Display_Refresh(&display, gameState);
					SDL_Delay(2000);
					mouvement.src_point = 8;
					mouvement.dest_point = 14;
					if(authorized_deplacement(gameState, &mouvement, EPlayer1))
						Checker_Move(&display,gameState,&mouvement);
				}
				else
				{
					curentPlayer = EPlayer2;
					sprintf(tmp,"C'est au tour de %s",display.game->player2_name);
					Display_Message(&display, tmp, msg_position, msg_color,1);
					SDL_Delay(2000);
					sprintf(tmp,"C'est au tour de %s",display.game->player1_name);
					quit = Display_Message_Click(&display, "Lancer les des", msg_position, msg_color,1);
					Launch_Die(gameState);
					Display_Refresh(&display, gameState);
					mouvement.src_point = 8;
					mouvement.dest_point = 14;
					if(authorized_deplacement(gameState, &mouvement, EPlayer2))
						Checker_Move(&display,gameState,&mouvement);
				}
				break;
			}
		//Display_Refresh(&display, gameState);
		
		
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
		
		SDL_Flip(display.screen);
	}
	
	Display_Exit(&display);
	
	exit(EXIT_SUCCESS);
}
