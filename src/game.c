#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include "../include/definitions.h"
#include "../include/backgammon.h"

SGameState* Game_Init()
{
	SGameState *game;
	int i;
	
	game = (SGameState*)malloc(sizeof(SGameState));
	
	game->score = 0;
	game->scoreP2 = 0;
	
	game->die1 = 0;
	game->die2 = 0;
	
	for(i=0;i<28;i++) game->zones[i].nb_checkers=0;
	
	game->zones[0].player=EPlayer1;
	game->zones[0].nb_checkers=2;
	game->zones[23].player=EPlayer2;
	game->zones[23].nb_checkers=2;
	
	game->zones[11].player=EPlayer1;
	game->zones[11].nb_checkers=8;//5
	game->zones[12].player=EPlayer2;
	game->zones[12].nb_checkers=8;//5
	
	game->zones[16].player=EPlayer1;
	game->zones[16].nb_checkers=3;
	game->zones[7].player=EPlayer2;
	game->zones[7].nb_checkers=3;
	
	game->zones[18].player=EPlayer1;
	game->zones[18].nb_checkers=5;
	game->zones[5].player=EPlayer2;
	game->zones[5].nb_checkers=5;
	
	
	return game;
}

int Game_FirstToPlay( SDisplay* display, EGameMode gameMode, SGame* game, SGameState *gameState)
{
	SDL_Event event;
	
	char* tmp = (char*)malloc(100*sizeof(char));
	SDL_Rect msg_position;
	msg_position.x = 80;
	msg_position.y = 325;
	SDL_Color msg_color = {255, 255, 255, 0};
	
	int quit=0;
	int dieP1[2];
	int dieP2[2];
	
	while(!quit)
	{
		SDL_WaitEvent(&event);
		
		switch(gameMode)
		{
			case HUMAN_HUMAN:
				sprintf( tmp, "%s, lance les des !", game->player1_name);
				quit = Display_Message_Click( display, tmp, msg_position, msg_color,1);
				if(quit)
				{
					quit = 1;
					break;
				}
				Game_LaunchDie(gameState, game);
				Display_RefreshGameBoard(display, gameState, game);
				SDL_Delay(1500);
				dieP1[0] = gameState->die1;
				dieP1[1] = gameState->die2;
			
				sprintf(tmp,"%s, lance les des !", game->player2_name);
				quit = Display_Message_Click( display, tmp, msg_position, msg_color,1);
				if(quit)
				{
					quit = 1;
					break;
				}
				Game_LaunchDie(gameState, game);
				Display_RefreshGameBoard(display, gameState, game);
				SDL_Delay(1500);
				dieP2[0] = gameState->die1;
				dieP2[1] = gameState->die2;
				if((dieP1[0]+dieP1[1]) > (dieP2[0]+dieP2[1]))
				{
					/* On rétablit les des dans le gameState*/
					gameState->die1 = dieP1[0];
					gameState->die2 = dieP1[1];
					free(tmp);
					return 1; // Joueur1 commence la partie
				}
				else if((dieP1[0]+dieP1[1]) < (dieP2[0]+dieP2[1]))
				{
					/* On rétablit les des dans le gameState*/
					gameState->die1 = dieP2[0];
					gameState->die2 = dieP2[1];
					free(tmp);
					return 2; // Joueur2 commence la partie
				}
				else
				{
					Display_Message( display, "Il y a egalite, on recommence !", msg_position, msg_color,1);
					SDL_Flip(display->screen);
					SDL_Delay(1500);
				}
				break;
			case HUMAN_AI:
				sprintf( tmp, "%s, lance les des !", game->player1_name);
				quit = Display_Message_Click( display, tmp, msg_position, msg_color,1);
				if(quit)
				{
					quit = 1;
					break;
				}
				Game_LaunchDie(gameState, game);
				Display_RefreshGameBoard(display, gameState, game);
				SDL_Delay(1500);
				dieP1[0] = gameState->die1;
				dieP1[1] = gameState->die2;
			
				sprintf(tmp,"%s, lance les des ...", game->player2_name);
				Display_Message( display, tmp, msg_position, msg_color,1);
				Game_LaunchDie(gameState, game);
				Display_RefreshGameBoard(display, gameState, game);
				SDL_Delay(1500);
				dieP2[0] = gameState->die1;
				dieP2[1] = gameState->die2;
				if((dieP1[0]+dieP1[1]) > (dieP2[0]+dieP2[1]))
				{
					/* On rétablit les des dans le gameState*/
					gameState->die1 = dieP1[0];
					gameState->die2 = dieP1[1];
					free(tmp);
					return 1; // Joueur1 commence la partie
				}
				else if((dieP1[0]+dieP1[1]) < (dieP2[0]+dieP2[1]))
				{
					/* On rétablit les des dans le gameState*/
					gameState->die1 = dieP2[0];
					gameState->die2 = dieP2[1];
					free(tmp);
					return 2; // Joueur2 commence la partie
				}
				else
				{
					Display_Message( display, "Il y a egalite, on recommence !", msg_position, msg_color,1);
					SDL_Flip(display->screen);
					SDL_Delay(1500);
				}
				break;
			case AI_AI:
				
				Game_LaunchDie(gameState, game);
				dieP1[0] = gameState->die1;
				dieP1[1] = gameState->die2;
			
				Game_LaunchDie(gameState, game);
				dieP2[0] = gameState->die1;
				dieP2[1] = gameState->die2;
				
				if((dieP1[0]+dieP1[1]) > (dieP2[0]+dieP2[1]))
				{
					/* On rétablit les des dans le gameState*/
					gameState->die1 = dieP1[0];
					gameState->die2 = dieP1[1];
					free(tmp);
					return 1; // Joueur1 commence la partie
				}
				else if((dieP1[0]+dieP1[1]) < (dieP2[0]+dieP2[1]))
				{
					/* On rétablit les des dans le gameState*/
					gameState->die1 = dieP2[0];
					gameState->die2 = dieP2[1];
					free(tmp);
					return 2; // Joueur2 commence la partie
				}
				break;
			case ERROR :
				free(tmp);
				return -1;
		}
		if(event.type == SDL_QUIT)
		{
			free(tmp);
			return -1;
		}
	}
	free(tmp);
	return -1;
}
			
		
int Game_Play( SDisplay* display, EGameMode gameMode, SGame* game, SAI_Functions* ai_struct)
{
	
	SGameState *gameState;
	gameState = Game_Init(); /* Initialisation de la partie */
	
	EPlayer curentP;
	
	char* tmp = (char*)malloc(100*sizeof(char));
	
	SDL_Event event;
	int quit = 0;
	int finish;
	
	SMove mouvement; // Mouvement pour le joueur humain
	
	int Die_For_Play[7]={0,1,1,1,1,1,1}; // Nb dés à utiliser dans le tour ex : {0,1,0,0,1,0,0} Dés encore à jouer : 1 et 4
	/** DECLARATIONS IA **/
	SMove mvmt_ia[4]; // Tableau de mouvement a fournir à l'IA
	unsigned int lastTimeError=0; // 
	
	
	SDL_Rect msg_position;
	msg_position.x = 80;
	msg_position.y = 325;
	SDL_Color msg_color = {255, 255, 255, 0};
	
	int cptAI[2]; // Compteur d'erreur des AI
	cptAI[0] = 0; 
	cptAI[1] = 0; 
	
	
	while( gameState->score < game->scoreLimit && gameState->scoreP2 < game->scoreLimit && !quit ) // Boucle de jeu
	{
		SDL_WaitEvent(&event);

		Display_Clear(display);
		Display_Score(display,gameState,game);
		Display_RefreshGameBoard(display, gameState, game);
		SDL_Flip(display->screen);
		Display_Message(display, "La partie commence !", msg_position, msg_color, 1);
		SDL_Flip(display->screen);
		SDL_Delay(1500);
		
		finish = 0;
		
		/*Détermination du joueur qui commence*/
		int getFirst=1;
		//getFirst = Game_FirstToPlay( display, gameMode, game, gameState);
		if(getFirst == 1)
		{
			curentP = EPlayer1;
			sprintf( tmp, "%s, commence !", game->player1_name);
			Display_Message( display, tmp, msg_position, msg_color,1);
			SDL_Flip(display->screen);
			SDL_Delay(1000);
		}
		else if(getFirst == 2)
		{
			curentP = EPlayer2;
			sprintf( tmp, "%s, commence !", game->player2_name);
			Display_Message( display, tmp, msg_position, msg_color,1);
			SDL_Flip(display->screen);
			SDL_Delay(1000);
		}
		else
		{
			quit = 1;
		}
		
		while(!finish && !quit) // Boucle de partie
		{
			SDL_WaitEvent(&event);
			
			switch (gameMode)
			{
				case HUMAN_HUMAN :
					if(curentP == EPlayer1)
					{
						sprintf( tmp, "%s, c'est ton tour !", game->player1_name);
						Display_Message( display, tmp, msg_position, msg_color,1);
						SDL_Flip(display->screen);
						quit = Display_GameActions(display, gameState, game);
						if(quit)
						{
							break;
						}

						Display_RefreshGameBoard(display, gameState, game);
						quit = Display_CheckersPossibilities(display, gameState, curentP, game);
						if(quit)
						{
							break;
						}
						curentP = EPlayer2;
					}
					else
					{
						sprintf( tmp, "%s, c'est ton tour !", game->player2_name);
						Display_Message( display, tmp, msg_position, msg_color,1);
						quit = Display_GameActions(display, gameState, game);
						if(quit)
						{
							break;
						}
						Display_RefreshGameBoard(display, gameState, game);
						quit = Display_CheckersPossibilities(display, gameState, curentP, game);
						if(quit)
						{
							break;
						}
						curentP = EPlayer1;
					}
					break;
					
				case HUMAN_AI :
					if(curentP == EPlayer1)
					{
						sprintf( tmp, "%s, c'est ton tour !", game->player1_name);
						Display_Message( display, tmp, msg_position, msg_color,1);
						SDL_Flip(display->screen);
						quit = Display_GameActions(display, gameState, game);
						if(quit)
						{
							break;
						}

						Display_RefreshGameBoard(display, gameState, game);
						quit = Display_CheckersPossibilities(display, gameState, curentP, game);
						if(quit)
						{
							break;
						}
						curentP = EPlayer2;
					}
					else
					{
	
						Game_LaunchDie(gameState, game);
						Display_RefreshGameBoard(display, gameState, game);
						/* DEROULEMENT JEU AI */
						curentP = EPlayer1;
					}
					break;
					
				case AI_AI :
					if(curentP == EPlayer1)
					{
						Game_LaunchDie(gameState, game);
						Display_RefreshGameBoard(display, gameState, game);
						/* DEROULEMENT JEU AI */
						curentP = EPlayer2;
					}
					else
					{
	
						Game_LaunchDie(gameState, game);
						Display_RefreshGameBoard(display, gameState, game);
						/* DEROULEMENT JEU AI */
						curentP = EPlayer1;
					}
					break;
				case ERROR :
					quit=1;
					break;
			}
			//finish = 1;
			if(event.type == SDL_QUIT)
				quit = 1;
		}
		if(event.type == SDL_QUIT)
			quit = 1;
		quit = 1;
	}
	free(tmp);
	free(gameState);
	free(game->player1_name);
	free(game->player2_name);
	
	return 1;
}

void Game_LaunchDie(SGameState *gameState, SGame *game) // ajouter le Die_For_play
{
	srand( time(NULL) ); // Initialisation du generateur de nombre aleatoire
	gameState->die1 = 1+(rand()/(float)RAND_MAX)*6; // Tirage aléatoire d'un chiffre entre 1 et 6
	gameState->die2 = 1+(rand()/(float)RAND_MAX)*6; // Tirage aléatoire d'un chiffre entre 1 et 6
	printf("Dé 1 : %d Dé 2 : %d\n",gameState->die1,gameState->die2);
	int i;
	for(i=0;i<4;i++) game->die_To_Play[i]=1;
}
		
