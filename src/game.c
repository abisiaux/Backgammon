/**
 * \file game.c
 * \brief Contient les fonctions du moteur de jeu.
 * \author Alexandre BISIAUX et Antonin BIRET
 * \date 26 janvier 2012
 *
 */

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
	
	game->die1 = 0;
	game->die2 = 0;
	
	for(i=0;i<28;i++) game->zones[i].nb_checkers=0;
	
	game->zones[0].player=EPlayer1;
	game->zones[0].nb_checkers=2;
	game->zones[23].player=EPlayer2;
	game->zones[23].nb_checkers=2;
	
	game->zones[11].player=EPlayer1;
	game->zones[11].nb_checkers=5;
	game->zones[12].player=EPlayer2;
	game->zones[12].nb_checkers=5;
	
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

void initScore(SGameState* gameState)
{
	gameState->score = 0;
	gameState->scoreP2 = 0;
}

int Game_FirstToPlay(SDisplay* display, EGameMode gameMode, SGame* game, SGameState *gameState)
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
				sprintf(tmp, "%s, lance les des !", game->player1_name);
				quit = Display_Message(display, tmp, msg_position, msg_color,1,1);
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
				quit = Display_Message(display, tmp, msg_position, msg_color,1,1);
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
					Display_Message(display, "Il y a egalite, on recommence !", msg_position, msg_color,1,0);
					SDL_Flip(display->screen);
					SDL_Delay(1500);
				}
				break;
			case HUMAN_IA:
				sprintf(tmp, "%s, lance les des !", game->player1_name);
				quit = Display_Message(display, tmp, msg_position, msg_color,1,1);
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
				Display_Message(display, tmp, msg_position, msg_color,1,0);
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
					Display_Message(display, "Il y a egalite, on recommence !", msg_position, msg_color,1,0);
					SDL_Flip(display->screen);
					SDL_Delay(1500);
				}
				break;
			case IA_IA:
				
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

void initMvmt_ia(SMove mvmt_ia[4])
{
	int i=0;
	for(i=0;i<4;i++)
	{
		mvmt_ia[i].src_point = EPos_nopos;
		mvmt_ia[i].dest_point = EPos_nopos;
	}
}			


int Game_AcceptDouble(SDisplay* display, EPlayer player, SGame* game)
{

	
	SDL_Rect msg_position;
	msg_position.x = 80;
	msg_position.y = 325;
	SDL_Color msg_color = {255, 255, 255, 0};
	
	char* tmp = (char*)malloc(100*sizeof(char));
	SDL_Event event;
	
	if(player == EPlayer1)
		sprintf(tmp,"%s, acceptes-tu le double ? Oui    Non",game->player1_name);
	else
		sprintf(tmp,"%s, acceptes-tu le double ? Oui    Non",game->player2_name);
	printf("ACCEPT DOUBLE\n");
	Display_Message(display,tmp,msg_position,msg_color,1,0);
	SDL_Flip(display->screen);
	
	while(1)
	{
		SDL_WaitEvent(&event);
		
		switch(event.type)
		{
			case SDL_QUIT:
				return -1;
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					if(event.button.x>=422 && event.button.x<=500 && event.button.y>=330 && event.button.y<=415) // Si clic sur oui
					{
						printf("Clic oui\n");
						sprintf(tmp,"Votre adversaire a accepte le double.");
						Display_Message(display,tmp,msg_position,msg_color,1,0);
						return 1;
					}
					if(event.button.x>=510 && event.button.x<=535 && event.button.y>=330 && event.button.y<=415) // Si clic sur non
					{
						printf("Clic non\n");
						return 0;
					}
				}
		  		break;

		}
	}
}
		
int Game_Play(SDisplay* display, EGameMode gameMode, SGame* game, SIA_Functions* ia_struct)
{
	
	SGameState *gameState;
	gameState = Game_Init(); /* Initialisation de la partie */
	initScore(gameState);
	
	int i=0;
	EPlayer curentP;
	int action,accept;
	char* tmp = (char*)malloc(100*sizeof(char));
	
	SDL_Event event;
	int quit = 0;
	int finish;
	SGameState* copyGameS;
	SMove mouvement; // Mouvement pour le joueur humain
	
	/** DECLARATIONS IA **/
	SMove mvmt_ia[4]; // Tableau de mouvement a fournir à l'IA

	unsigned int lastTimeError=0; // 
	
	EPlayer winner;
	SDL_Rect msg_position;
	msg_position.x = 80;
	msg_position.y = 325;
	SDL_Color msg_color = {255, 255, 255, 0};
	
	if(gameMode == HUMAN_IA)
	{
		(*ia_struct[0].IA_StartMatch)(game->scoreLimit);
	}
	else if(gameMode == IA_IA)
	{
		(*ia_struct[0].IA_StartMatch)(game->scoreLimit);
		(*ia_struct[1].IA_StartMatch)(game->scoreLimit);
	}
	int nbTour = 0;
	Display_Clear(display);
	
	while(gameState->score < game->scoreLimit && gameState->scoreP2 < game->scoreLimit && !quit) // Boucle de jeu
	{
		nbTour++;
		gameState = Game_Init(); /* Initialisation de la partie */
		Display_Score(display,gameState,game);
		
		if(gameMode == HUMAN_IA)
		{
			(*ia_struct[0].IA_StartGame)();
		}
		else if(gameMode == IA_IA)
		{
			(*ia_struct[0].IA_StartGame)();
			(*ia_struct[1].IA_StartGame)();
		}
		
		SDL_WaitEvent(&event);

		
		Display_Score(display,gameState,game);
		Display_RefreshGameBoard(display, gameState, game);
		printf("Match %d\n",nbTour);
		SDL_Flip(display->screen);
		Display_Message(display, "La partie commence !", msg_position, msg_color, 1,0);
		SDL_Flip(display->screen);
		SDL_Delay(1500);
		
		finish = 0;
		
		/*Détermination du joueur qui commence*/
		int getFirst=1;
		//getFirst = Game_FirstToPlay(display, gameMode, game, gameState);
		if(getFirst == 1)
		{
			curentP = EPlayer1;
			sprintf(tmp, "%s, commence !", game->player1_name);
			Display_Message(display, tmp, msg_position, msg_color,1,0);
			SDL_Flip(display->screen);
			SDL_Delay(1000);
		}
		else if(getFirst == 2)
		{
			curentP = EPlayer2;
			sprintf(tmp, "%s, commence !", game->player2_name);
			Display_Message(display, tmp, msg_position, msg_color,1,0);
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
						sprintf(tmp, "%s, c'est ton tour !", game->player1_name);
						Display_Message(display, tmp, msg_position, msg_color,1,0);
						SDL_Flip(display->screen);
						action = Display_GameActions(display, gameState, game, curentP);
						if(action == 1)
						{
							quit = 1;
							break;
						}
						if(action == 2)
						{
							finish = 1;
							winner = EPlayer1;
							break;
						}
						
						Display_RefreshGameBoard(display, gameState, game);
						quit = Display_CheckersPossibilities(display, gameState, curentP, game);
						printf("etat des dés : 1=>[%d]\t2=>[%d]\t3=>[%d]\t4=>[%d]\n", game->die_To_Play[0],game->die_To_Play[1],game->die_To_Play[2],game->die_To_Play[3]);
						if(quit)
						{
							break;
						}
						curentP = EPlayer2;
					}
					else
					{
						sprintf(tmp, "%s, c'est ton tour !", game->player2_name);
						Display_Message(display, tmp, msg_position, msg_color,1,0);
						quit = Display_GameActions(display, gameState, game,curentP);
						if(quit)
						{
							break;
						}
						Display_RefreshGameBoard(display, gameState, game);
						quit = Display_CheckersPossibilities(display, gameState, curentP, game);
						printf("etat des dés : 1=>[%d]\t2=>[%d]\t3=>[%d]\t4=>[%d]\n", game->die_To_Play[0],game->die_To_Play[1],game->die_To_Play[2],game->die_To_Play[3]);
						if(quit)
						{
							break;
						}
						curentP = EPlayer1;
					}
					break;
					
				case HUMAN_IA :
					if(curentP == EPlayer1)
					{
						sprintf(tmp, "%s, c'est ton tour !", game->player1_name);
						Display_Message(display, tmp, msg_position, msg_color,1,0);
						SDL_Flip(display->screen);
						quit = Display_GameActions(display, gameState, game,curentP);
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
						/* DEMANDER DOUBLE PUIS MOUVEMENT*/
						
						for(i=0;i<4;i++)
						{
							mvmt_ia[i].src_point = EPos_nopos;
							mvmt_ia[i].dest_point = EPos_nopos;
						}
						copyGameS = copyGameState(gameState,curentP);
						(*ia_struct[0].IA_MakeDecision)(copyGameS, mvmt_ia, lastTimeError); 
						for(i=0;i<4;i++)
						{
							if(mvmt_ia[i].src_point != EPos_nopos && mvmt_ia[i].dest_point != EPos_nopos)
							{
								printf("AI JOUE src = %d dest = %d\n",mvmt_ia[i].src_point + 1,mvmt_ia[i].dest_point + 1);
								mvmt_ia[i].src_point += 1;
								mvmt_ia[i].dest_point += 1;
								if(Arbitrator_AuthorizedDeplacement(gameState, mvmt_ia, curentP, game))
									Display_CheckerMove(display, gameState, &mvmt_ia[i], game);
							}
						}
						free(copyGameS);
						curentP = EPlayer1;
					}
					break;
					
				case IA_IA :
					if(curentP == EPlayer1)
					{
						Game_LaunchDie(gameState, game);
						Display_RefreshGameBoard(display, gameState, game);
						/* DEROULEMENT JEU AI */
						/* DEMANDER DOUBLE PUIS MOUVEMENT*/
						curentP = EPlayer2;
					}
					else
					{
	
						Game_LaunchDie(gameState, game);
						Display_RefreshGameBoard(display, gameState, game);
						/* DEROULEMENT JEU AI */
						/* DEMANDER DOUBLE PUIS MOUVEMENT*/
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
		if(winner == EPlayer1)
		{
			sprintf(tmp, "%s gagne la manche !", game->player1_name);
			gameState->score = gameState->score + 1;
			printf("%s gagne SCORE = %d\n", game->player1_name,gameState->score);
		}
		else
		{
			sprintf(tmp, "%s gagne la manche !", game->player2_name);
			gameState->scoreP2 = gameState->scoreP2 + 1;
			printf("%s gagne SCORE = %d\n", game->player2_name,gameState->scoreP2);
		}
		finish = 0;
			
		Display_Message(display, tmp, msg_position, msg_color,1,0);
		
		if(gameMode == HUMAN_IA)
		{
			(*ia_struct[0].IA_EndGame)();
		}
		else if(gameMode == IA_IA)
		{
			(*ia_struct[0].IA_EndGame)();
			(*ia_struct[1].IA_EndGame)();
		}
		if(event.type == SDL_QUIT)
			quit = 1;
		//quit = 1;
	}
	if(gameMode == HUMAN_IA)
	{
		(*ia_struct[0].IA_EndMatch)();
	}
	else if(gameMode == IA_IA)
	{
		(*ia_struct[0].IA_EndMatch)();
		(*ia_struct[1].IA_EndMatch)();
	}
	free(tmp);
	free(gameState);
	free(game->player1_name);
	free(game->player2_name);
	
	return 1;
}

void Game_LaunchDie(SGameState *gameState, SGame *game) 
{
	srand(time(NULL)); // Initialisation du generateur de nombre aleatoire
	gameState->die1 = 1+(rand()/(float)RAND_MAX)*6; // Tirage aléatoire d'un chiffre entre 1 et 6
	gameState->die2 = 1+(rand()/(float)RAND_MAX)*6; // Tirage aléatoire d'un chiffre entre 1 et 6
	printf("Dé 1 : %d Dé 2 : %d\n",gameState->die1,gameState->die2);
	int i;
	if(gameState->die1 != gameState->die2)
	{
		for(i=0;i<2;i++) game->die_To_Play[i]=1;
		for(i=2;i<4;i++) game->die_To_Play[i]=0;
	}
	else
	{
		for(i=0;i<4;i++) game->die_To_Play[i]=1;
	}
}

SGameState* copyGameState(SGameState* gameState, EPlayer player)
{
	SGameState *toReturn = (SGameState*)malloc(sizeof(SGameState));
	int i =0;
	if(player == EPlayer1)
	{
		for(i=0;i<28;i++)
		{
			toReturn->zones[i].nb_checkers = gameState->zones[i].nb_checkers;
			toReturn->zones[i].player = gameState->zones[i].player;
		}
	}
	else
	{
		for(i=0;i<24;i++)
		{
			toReturn->zones[i].nb_checkers = gameState->zones[23-i].nb_checkers;
			toReturn->zones[i].player = gameState->zones[23-i].player;
		}
		toReturn->zones[24] = gameState->zones[26];
		toReturn->zones[25] = gameState->zones[27];
		toReturn->zones[26] = gameState->zones[24];
		toReturn->zones[27] = gameState->zones[25];
	}
	toReturn->die1 = gameState->die1;
	toReturn->die2 = gameState->die2;
	toReturn->score = gameState->score;
	toReturn->scoreP2 = gameState->scoreP2;
	toReturn->stake = gameState->stake;
	
	return toReturn;
}

