#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h> // Pour utiliser des formats d'image autre que bmp
#include <SDL/SDL_ttf.h> // Pour la gestion des textes sous SDL

#include "../include/definitions.h"
#include "../include/backgammon.h"

#define VITESSE 8

void Display_Init(SDisplay *display, SGame* game)
{	
	int i;
	char *temp_path;
	char *temp_name;
	
	// Initialisation de la SDL
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de la SDL");
		free(temp_path); // On libère
		free(temp_name);
		exit(EXIT_FAILURE);
	}
	
	// Enregistrement du path des images
	display->img_path = "../ressources/pictures/";
	
	// Enregistrement du path des polices
	display->font_path = "../ressources/fonts/";
	temp_path = (char*)malloc(100*sizeof(char));
	
	// Chargement de l'icone de la fenêtre
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "icon.bmp");
	SDL_WM_SetIcon(IMG_Load(temp_path), NULL);
	
	// Chargement du background et initialisation de sa position
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "background.png");
	display->background = IMG_Load(temp_path);
	display->background_position.x = 0;
	display->background_position.y = 0;
	
	// Chargement du plateau de jeu et initialisation de sa position
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "gameBoard.png");
	display->gameBoard = IMG_Load(temp_path);
	display->gameBoard_position.x = 0;
	display->gameBoard_position.y = 140;
	
	// Chargement de l'image de pion blanc
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "white_checker.png");
	display->white_checker = IMG_Load(temp_path);
	
	// Chargement de l'image de pion vert
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "green_checker.png");
	display->green_checker = IMG_Load(temp_path);
	
	// Chargement de l'image de pion blanc pour les barres
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "out_white_checker.png");
	display->out_white_checker = IMG_Load(temp_path);
	
	// Chargement de l'image de pion vert pour les barres
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "out_green_checker.png");
	display->out_green_checker = IMG_Load(temp_path);
	
	//Chargement de l'image de pion selectionne
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "selected_checker.png");
	display->selected_checker = IMG_Load(temp_path);
	
	// Chargement du cadre de possibilité 1
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "flecheSelected1_12.png");
	display->possibility1_12 = IMG_Load(temp_path);
	
	// Chargement du cadre de possibilité 2
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "flecheSelected13_24.png");
	display->possibility13_24 = IMG_Load(temp_path);
	
	// Chargement du cadre de possibilité 3
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "out_selected.png");
	display->possibility_out = IMG_Load(temp_path);
	
	// Chargement de l'image du cadre d'un message
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "msgBox.png");
	display->msg_box = IMG_Load(temp_path);
	
	// Chargement de l'image du videau
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "videau.png");
	display->videau = IMG_Load(temp_path);
	
	display->videau_position.x = 15;
	display->videau_position.y = 340;
	
	// Chargement des images des dés et initialisation de leur position
	temp_name = (char*)malloc(100*sizeof(char));

	for( i=1; i<=6; i++ )
	{
		strcpy(temp_path, display->img_path);
		sprintf(temp_name,"die/die[%d].png",i);
		strcat(temp_path,temp_name);
		display->die[i-1] = IMG_Load(temp_path);
	}
	
	// Position du dé 1
	display->die1_position.x = 410;
	display->die1_position.y = 340;
	// Position du dé 2
	display->die2_position.x = 450;
	display->die2_position.y = 340;
	
	// Chargement des images des dés joués
	for( i=1; i<=6; i++ )
	{
		strcpy(temp_path, display->img_path);
		sprintf(temp_name,"die/die[%d]_played.png",i);
		strcat(temp_path,temp_name);
		display->die_played[i-1] = IMG_Load(temp_path);
	}
	
	
	// Initialisation de la taille de la fenêtre
	display->window_width = 800;
	display->window_height = 600;
	
	// Initialisation du tableau de positions
	display->positions[0].x = 520;
	display->positions[0].y = 562;
	
	for( i=1 ; i<24 ; i++ )
	{
		display->positions[i].y = display->positions[i-1].y  ;
		if( i<6 )
			display->positions[i].x = display->positions[i-1].x - 38;
		else if ( i==6 )
			display->positions[i].x = display->positions[i-1].x - 74;
		else if( i<12 )
			display->positions[i].x = display->positions[i-1].x - 38;
		else if( i==12 )
		{
			display->positions[i].x = display->positions[i-1].x ;
			display->positions[i].y = 145;
		}
		else if( i<18 )
			display->positions[i].x = display->positions[i-1].x + 38;
		else if( i==18 )
			display->positions[i].x=display->positions[i-1].x + 74;
		else
			display->positions[i].x=display->positions[i-1].x + 38;
	}

	// Out Player 1
	display->positions[24].x = 568;
	display->positions[24].y = 140;
	// Bar Player 1
	display->positions[25].x = 292;
	display->positions[25].y = 480;
	// Out Player 2
	display->positions[26].x = 568;
	display->positions[26].y = 590;
	// Bar PLayer 2
	display->positions[27].x = 292;
	display->positions[27].y = 220;
	
	// Chargement des actions de jeu
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "double.png");
	display->gameActions[0] = IMG_Load(temp_path);
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "launch.png");
	display->gameActions[1] = IMG_Load(temp_path);
	
	// Initialisation de la fenêtre
	display->screen = SDL_SetVideoMode(display->window_width, display->window_height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	
	if (display->screen == NULL) // Si l'ouverture a échoué, on écrit l'erreur et on arrête
	{
		fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
		free(temp_path); // On libère
		free(temp_name);
		exit(EXIT_FAILURE);
	}
	
	SDL_WM_SetCaption("Backgammon",NULL);
	
	// Chargement de SDL_ttf
	if(TTF_Init() == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		free(temp_path); // On libère
		free(temp_name);
		exit(EXIT_FAILURE);
	}
   
   // Chargement de la police
   strcpy(temp_path, display->font_path);
	strcat(temp_path, "comic.ttf");
   display->font = TTF_OpenFont(temp_path,22);
	
	// Initialisation de la structure de jeu et initialisation
	game->player1_name = (char*)malloc(12*sizeof(char));
	game->player2_name = (char*)malloc(12*sizeof(char));
	game->player1_checker = GREEN;
	game->player2_checker = WHITE;
	game->withDouble = 1; // Par défaut, on joue avec le double
	game->doubleValue = 1;
	game->scoreLimit = 5; // Par défaut, limite de score est de 
	
	free(temp_path); // On libère
	free(temp_name);
}


int Display_Message(SDisplay	*display, char	*message, SDL_Rect position, SDL_Color color, int box, int clic)
{
	int leng = strlen(message);
	if(box)
	{
		SDL_BlitSurface(display->msg_box, NULL, display->screen, &position);
		
		position.x= position.x + 260 - 12*leng/2;
		position.y = position.y + 25;
	}
	SDL_Surface *msg = TTF_RenderText_Blended(display->font,message,color);
	SDL_BlitSurface(msg, NULL, display->screen, &position);
	SDL_FreeSurface(msg);
	SDL_Flip(display->screen);
	
	if(clic)
	{
		SDL_Event event;
		while(1)
		{
			SDL_WaitEvent(&event);
			switch(event.type)
			{
				case SDL_QUIT:
					return 1;
					break;
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						if(event.button.x>=80 && event.button.x<=548 && event.button.y>=325 && event.button.y<=415)
							return 0;
					}
			  		break;

			}
		}
	}
	return 0;
}

void Display_Double(SDisplay *display, SGame* game)
{
	if(game->withDouble)
	{
		SDL_BlitSurface(display->videau, NULL, display->screen, &display->videau_position);
		
		SDL_Rect new_pos;
		new_pos.x = display->videau_position.x + 10;
		new_pos.y = display->videau_position.y - 3;
		SDL_Surface *num;
		char* tmp = (char*)malloc(2*sizeof(char));
		SDL_Color color = {255, 255, 255, 0};
		if(game->doubleValue>10)
			new_pos.x -= 8;
		sprintf(tmp, "%d", game->doubleValue);
		num = TTF_RenderText_Blended(display->font,tmp,color);
		SDL_BlitSurface(num, NULL, display->screen, &new_pos);
		SDL_FreeSurface(num);
		free(tmp);
	}
}
		
void Display_Exit(SDisplay *display)
{
	int i;
	
	// Libération de toutes les surfaces utilisées
	SDL_FreeSurface(display->background);
	SDL_FreeSurface(display->background_menu);
	SDL_FreeSurface(display->gameBoard);
	SDL_FreeSurface(display->checked);
	SDL_FreeSurface(display->white_checker);
	SDL_FreeSurface(display->green_checker);
	SDL_FreeSurface(display->out_white_checker);
	SDL_FreeSurface(display->out_green_checker);
	SDL_FreeSurface(display->selected_checker);
	SDL_FreeSurface(display->msg_box);
	SDL_FreeSurface(display->possibility1_12);
	SDL_FreeSurface(display->possibility13_24);
	SDL_FreeSurface(display->possibility_out);

	for( i=0; i<6; i++ )
	{
		SDL_FreeSurface(display->die[i]);
		SDL_FreeSurface(display->die_played[i]);
	}
	SDL_FreeSurface(display->screen);
	
	TTF_CloseFont(display->font);

	TTF_Quit();
	SDL_Quit();
}

void Display_Clear(SDisplay *display)
{
	// Rechargement du background
	char *temp_path;
	temp_path = (char*)malloc(100*sizeof(char));
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "background.png");
	display->background = IMG_Load(temp_path);

	// Chargement du plateau vide
	SDL_BlitSurface(display->background, NULL, display->screen, &(display->background_position));
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "gameBoard.png");
	display->background = IMG_Load(temp_path);
	
	SDL_BlitSurface(display->gameBoard, NULL, display->screen, &(display->gameBoard_position));
	
	free(temp_path);
}

void Display_RefreshGameBoard(SDisplay *display, SGameState *gameState, SGame *game)
{
	// Rechargement du plateau de jeu
	char *temp_path;
	temp_path = (char*)malloc(100*sizeof(char));
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "gameBoard.png");
	display->background = IMG_Load(temp_path);
	free(temp_path);
	
	SDL_BlitSurface(display->gameBoard, NULL, display->screen, &(display->gameBoard_position));
	
	// Raffraichissement des pions
	Display_Checkers(display, gameState, game);
	Display_DrawOut(display, gameState, game);
	Display_DrawBar(display, gameState, game);
	
	// Raffraichissement des dés
	Display_Die(display,gameState, game);
	
	// Affichage du videau
	Display_Double(display, game);
	
	SDL_Flip(display->screen);
}

void Display_Score(SDisplay *display, SGameState *gameState, SGame* game)
{
	SDL_Rect pos;
	char *scoreP1 = (char*)malloc(3*(sizeof(char)));
	char *scoreP2 = (char*)malloc(3*(sizeof(char)));
	SDL_Color c = {255, 255, 255, 0};
	
	sprintf(scoreP1, "%d", gameState->score);
	sprintf(scoreP2, "%d", gameState->scoreP2);
	
	pos.x = 108;
	pos.y = 40;
	Display_Message(display, game->player1_name, pos, c, 0, 0);
	pos.x = 310;
	Display_Message(display, scoreP1, pos, c, 0, 0);
	pos.y = 85;
	Display_Message(display, scoreP2, pos, c, 0, 0);
	pos.x = 108;
	Display_Message(display, game->player2_name, pos, c, 0, 0);
	
	free(scoreP1);
	free(scoreP2);
}


void Display_Die(SDisplay *display,SGameState *gameState, SGame* game)
{
	SDL_Rect new_pos;
	
	if( gameState->die1 == gameState->die2 ) // Si c'est un double, on affiche 4 fois le dé
	{
		new_pos.x = (display->die1_position).x - 40;
		new_pos.y = (display->die1_position).y;
		if(game->die_To_Play[0]) // S'il n'a pas été déjà joué
			SDL_BlitSurface(display->die[gameState->die1-1], NULL,display->screen, &new_pos);
		else
			SDL_BlitSurface(display->die_played[gameState->die1-1], NULL,display->screen, &new_pos);
			
		new_pos.x = (display->die1_position).x;
		if(game->die_To_Play[1])
			SDL_BlitSurface(display->die[gameState->die1-1], NULL,display->screen, &new_pos);
		else
			SDL_BlitSurface(display->die_played[gameState->die1-1], NULL,display->screen, &new_pos);
		
		new_pos.x = (display->die1_position).x + 40;
		if(game->die_To_Play[2])
			SDL_BlitSurface(display->die[gameState->die1-1], NULL,display->screen, &new_pos);
		else
			SDL_BlitSurface(display->die_played[gameState->die1-1], NULL,display->screen, &new_pos);
		
		new_pos.x = (display->die1_position).x + 80;
		if(game->die_To_Play[2])
			SDL_BlitSurface(display->die[gameState->die1-1], NULL,display->screen, &new_pos);
		else
			SDL_BlitSurface(display->die_played[gameState->die1-1], NULL,display->screen, &new_pos);
}
	else // Sinon simple affichage des deux dés
	{
		if(game->die_To_Play[0]) // S'il n'a pas été déjà joué
			SDL_BlitSurface(display->die[gameState->die1-1], NULL,display->screen, &display->die1_position);
		else
			SDL_BlitSurface(display->die_played[gameState->die1-1], NULL,display->screen, &display->die1_position);
			
		if(game->die_To_Play[1]) // S'il n'a pas été déjà joué
			SDL_BlitSurface(display->die[gameState->die2-1], NULL,display->screen, &display->die2_position);
		else
			SDL_BlitSurface(display->die[gameState->die2-1], NULL,display->screen, &display->die2_position);
	}
}


void Display_CheckerDraw(SDisplay *display, SDL_Rect position, int player, SGame *game)
{
	if( player==0)
	{
		if(game->player1_checker == GREEN)
			SDL_BlitSurface(display->green_checker, NULL,display->screen, &position);
		else
			SDL_BlitSurface(display->white_checker, NULL,display->screen, &position);
	}
	else
	{
		if(game->player2_checker == GREEN)
			SDL_BlitSurface(display->green_checker, NULL,display->screen, &position);
		else
			SDL_BlitSurface(display->white_checker, NULL,display->screen, &position);
	}
}

void Display_DrawOut(SDisplay *display, SGameState *gameState, SGame *game)
{
	SDL_Rect pos;
	unsigned int i;
	pos.x = display->positions[24].x;
	pos.y = display->positions[24].y;
	for(i=0;i<gameState->zones[24].nb_checkers;i++)
	{
		pos.y += 10;
		if(game->player1_checker == GREEN)
			SDL_BlitSurface(display->out_green_checker, NULL,display->screen, &pos);
		else
			SDL_BlitSurface(display->out_white_checker, NULL,display->screen, &pos);
	}
	pos.x = display->positions[26].x;
	pos.y = display->positions[26].y;
	for(i=0;i<gameState->zones[26].nb_checkers;i++)
	{
		pos.y -= 10;
		if(game->player2_checker == GREEN)
			SDL_BlitSurface(display->out_green_checker, NULL,display->screen, &pos);
		else
			SDL_BlitSurface(display->out_white_checker, NULL,display->screen, &pos);
	}
}

void Display_DrawBar(SDisplay *display, SGameState *gameState, SGame *game)
{
	SDL_Rect pos;
	unsigned int i;
	pos.x = display->positions[25].x;
	pos.y = display->positions[25].y;
	for(i=0;i<gameState->zones[25].nb_checkers;i++) // Barre du joueur 1
	{
		pos.y -= 10;
		if(game->player1_checker == GREEN)
			SDL_BlitSurface(display->green_checker, NULL,display->screen, &pos);
		else
			SDL_BlitSurface(display->white_checker, NULL,display->screen, &pos);
	}
	pos.x = display->positions[27].x;
	pos.y = display->positions[27].y;
	for(i=0;i<gameState->zones[27].nb_checkers;i++) // Barre du joueur 2
	{
		pos.y += 10;
		if(game->player2_checker == GREEN)
			SDL_BlitSurface(display->green_checker, NULL,display->screen, &pos);
		else
			SDL_BlitSurface(display->white_checker, NULL,display->screen, &pos);
	}
}		
void Display_Checkers(SDisplay *display, SGameState *gameState, SGame *game)
{
	unsigned int i,j;
	SDL_Rect new_pos;
	SDL_Surface *num;
	char* tmp = (char*)malloc(2*sizeof(char));
	SDL_Color color = {0, 0, 0, 0};
	
	for( i=0 ; i<24; i++ )
	{
		for( j=0; j<gameState->zones[i].nb_checkers; j++)
		{
			if(j>5)
			{
			
				if(gameState->zones[i].nb_checkers>9) // Pour centrer l'affichage en cas de plus de 10 jetons
				{
					new_pos.x += 5;
				}else
				{
					new_pos.x += 8;
				}
				sprintf(tmp, "%d", gameState->zones[i].nb_checkers);
				num = TTF_RenderText_Blended(display->font,tmp,color);
				SDL_BlitSurface(num, NULL, display->screen, &new_pos);
				SDL_FreeSurface(num);
				break;
			}
			else
			{
				new_pos.x = display->positions[i].x;
				if( i<12)
					new_pos.y = display->positions[i].y - j*20;
				else
					new_pos.y = display->positions[i].y + j*20;
				Display_CheckerDraw(display,new_pos,gameState->zones[i].player,game);
			
			}
		}
	}
	free(tmp);
}



void Display_CheckerMove(SDisplay *display, SGameState* gameState, SMove *move, SGame *game)
{
	/* Calcul de la droite passant par les deux zones */
	int pos_src_x;
	int pos_src_y;
	int pos_dest_x;
	int pos_dest_y;
	
	pos_src_x = display->positions[move->src_point-1].x;
	
	if(move->src_point>12)
	{
		if(gameState->zones[move->src_point-1].nb_checkers>6)
			pos_src_y =  display->positions[move->src_point-1].y + 20*6;
		else
			pos_src_y =  display->positions[move->src_point-1].y + 20*(gameState->zones[move->src_point-1].nb_checkers);
	}
	else
	{
		if(gameState->zones[move->src_point-1].nb_checkers>6)
			pos_src_y =  display->positions[move->src_point-1].y - 20*6;
		else
			pos_src_y =  display->positions[move->src_point-1].y - 20*(gameState->zones[move->src_point-1].nb_checkers);
	}
	
	pos_dest_x = display->positions[move->dest_point-1].x;
	
	if(move->dest_point>12)
	{
		if(gameState->zones[move->dest_point-1].nb_checkers>6)
			pos_dest_y = display->positions[move->dest_point-1].y + 20*6;
		else
			pos_dest_y = display->positions[move->dest_point-1].y + 20*(gameState->zones[move->dest_point-1].nb_checkers);
	}
	else
	{
		if(gameState->zones[move->dest_point-1].nb_checkers>6)
			pos_dest_y = display->positions[move->dest_point-1].y - 20*6;
		else
			pos_dest_y = display->positions[move->dest_point-1].y - 20*(gameState->zones[move->dest_point-1].nb_checkers);
	}
	
	double a = (double)(pos_src_y-pos_dest_y)/(pos_src_x - pos_dest_x);
	double b = pos_dest_y - a*pos_dest_x;
	
	/*printf("SRC = fleche %d x=%d y=%d\n",move->src_point,pos_src_x,pos_src_y);
	printf("DEST = fleche %d x=%d y=%d\n",move->dest_point,pos_dest_x,pos_dest_y);
	printf("a=%f b=%f\n",a,b);*/
	
	int pas = VITESSE; // Vitesse de deplacement du pion 1=lentement 8=rapide
	int numCas;
	
	if(move->dest_point<13 && move->src_point<13) // Si les deux pions sont situés sur la barre du bas
	{
		if((move->dest_point-1)<=(move->src_point-1)) // Si le déplacement s'effectue vers une flèche à droite
			numCas=1;
		else // Sinon le déplacement s'effectue vers une flèche à gauche
		{
			numCas=2;
			pas = -pas;
		}
	}
	else if(move->dest_point>12 && move->src_point>12) // Si les deux pions sont situés sur la barre du haut
	{
		if((move->dest_point-1)>=(move->src_point-1)) // Si le déplacement s'effectue vers une flèche à droite
			numCas=3;
		else // Sinon le déplacement s'effectue vers une flèche à gauche
		{
			numCas=4;
			pas = -pas;
		}
	}
	else if(a>=0 && (move->dest_point-1)<=(move->src_point-1)) // src : barre du haut / dest : barre du bas / coef>0
	{
		numCas=5;
	}else if(a<=0 && (move->dest_point-1)>=(move->src_point-1)) // src : barre du bas / dest : barre du haut / coef<0
	{
		numCas=6;
	}
	else if(a>=0 && (move->dest_point-1)>=(move->src_point-1)) // src : barre du bas / dest : barre du haut / coef>0
	{
		numCas=7;
		pas = -pas;
	}
	else // src : barre du haut / dest : barre du bas / coef<0
	{
		numCas=8;
		pas = -pas;
	}
	printf("Cas n°%d\n",numCas);
	
	SDL_Rect new_pos;
	new_pos.x = pos_src_x;
	new_pos.y = pos_src_y;
	int depassement = 0;
	gameState->zones[move->src_point-1].nb_checkers--;
	while( !depassement )
	{
		new_pos.x = new_pos.x + pas;
		new_pos.y = (int)(a*new_pos.x + b);
		Display_RefreshGameBoard(display,gameState,game);
		Display_CheckerDraw(display,new_pos,gameState->zones[move->src_point-1].player,game);
		SDL_Flip(display->screen);
		switch(numCas) // Suivant le cas de figure, on vérifie si on ne dépasse pas la flèche destinatrice
		{
			case 1:
				if(new_pos.x>(pos_dest_x-(2*pas))) // On s'arrete à chaque fois 2*pas pixel avant par souci d'esthétique
					depassement = 1;
				break;
			case 2:
				if(new_pos.x<(pos_dest_x-(2*pas)))
					depassement = 1;
				break;
			case 3:
				if(new_pos.x>(pos_dest_x-(2*pas)))
					depassement = 1;
				break;
			case 4:
				if(new_pos.x<(pos_dest_x-(2*pas)))
					depassement = 1;
				break;
			case 5:
				if(new_pos.x>(pos_dest_x-(2*pas)))
					depassement = 1;
				break;
			case 6:
				if(new_pos.x>(pos_dest_x-(2*pas)))
					depassement = 1;
				break;
			case 7:
				if(new_pos.x<(pos_dest_x-(2*pas)))
					depassement = 1;
				break;
			case 8:
				if(new_pos.x<(pos_dest_x-(2*pas)))
					depassement = 1;
				break;
			default:
				depassement = 1;
				break;
		}
	}
	if(gameState->zones[move->dest_point-1].nb_checkers==1 && (gameState->zones[move->src_point-1].player != gameState->zones[move->dest_point-1].player) ) // Si il y a qu'un pion sur la flèche destinatrice et que les couleurs sont différentes
	{
		if(gameState->zones[move->src_point-1].player == EPlayer1 && gameState->zones[move->dest_point-1].player == EPlayer2)
		{
			gameState->zones[27].nb_checkers++;
			gameState->zones[27].player = gameState->zones[move->dest_point-1].player;
			gameState->zones[move->dest_point-1].nb_checkers = 1;
			gameState->zones[move->dest_point-1].player = gameState->zones[move->src_point-1].player;
		}
		else
		{
			gameState->zones[25].nb_checkers++;
			gameState->zones[25].player = gameState->zones[move->dest_point-1].player;
			gameState->zones[move->dest_point-1].nb_checkers = 1;
			gameState->zones[move->dest_point-1].player = gameState->zones[move->src_point-1].player;
		}
	}
	else
	{
		gameState->zones[move->dest_point-1].nb_checkers++;
		gameState->zones[move->dest_point-1].player = gameState->zones[move->src_point-1].player;
	}
	Display_RefreshGameBoard(display, gameState, game);
	
	//Mise a jour du tableau des dés utilisables.
	unsigned int taille_mouvement = get_distance(move->src_point,move->dest_point);
	for(int i=0;i<4;i++)
	{
		if(game->die_To_Play[i] == 1) // dé non utilisé jusqu'a present
		{
			switch(i)
			{
				case 0: //Dé 1 non joué  
					if(taille_mouvement == gameState->die1) // taille du mouvement correspond au dé 1
					{
						game->die_To_Play[i] = 0;	
					}
					break;
				case 1: // Dé 2 non joué
					if(taille_mouvement == gameState->die2) // taille du mouvement correspond au dé 2
					{
						game->die_To_Play[i] = 0;	
					}
					break;
				default:
					if(taille_mouvement == gameState->die1) // taille du mouvement correspond au dé 3 ou 4
					{
						game->die_To_Play[i] = 0;	
					}
					break;	
			}
		}
	}	
	
}

int CheckerWithScreenPosition(int x, int y, EPosition *pos)
{
	int i=0;
	
	if(x>=323 && x<=554)//Barre de 1 à 6 ou de 19 à 24
	{
		
		if(y>=423 && y<=590)// Barre de 1 à 6
		{
			
			EPosition tmpPos=5;
			for(i=1; i<=6; i++)
			{
				
				if(x<=(323+(i*40)))
				{	
					*pos=tmpPos;
					return 1;
				}
				tmpPos--;
			}
		}
		else if(y>=142 && y<=303)//Barre de 19 à 24
		{
			
			EPosition tmpPos=18;
			for(i=1; i<=6; i++)
			{
				if(x<=(323+(i*40)))
				{	
					*pos=tmpPos;
					return 1;
				}
				tmpPos++;
			}
			
		}
	}
	else if(x>=59 && x<=289)//Barre 7 à 12 ou de 13 à 18
	{
		if(y>=423 && y<=590)// Barre de 7 à 12
		{
			
			EPosition tmpPos=11;
			for(i=1; i<=6; i++)
			{
				if(x<=(59+(i*40)))
				{	
					*pos=tmpPos;
					return 1;
				}
				tmpPos--;
			}
		}
		else if(y>=142 && y<=303)//Barre de 13 à 18 --
		{
			
			EPosition tmpPos=12;
			for(i=1; i<=6; i++)
			{
				if(x<=(59+(i*40)))	
				{	
					*pos=tmpPos;
					return 1;
				}
				tmpPos++;
			}
		}
	}
	
	
	return 0;

}


void colorChecker(SDisplay *display, SGameState* game, EPosition pos)
{
	
	SDL_Rect posSelectedChecker;
	SZone selectedZone = game->zones[pos];
	posSelectedChecker=display->positions[pos];
	//lprintf("E_POS:%d\tNB_CHECKER:%d\tY:%d\n",pos,selectedZone.nb_checkers,posSelectedChecker.y);
	if( pos < 12)
		if(selectedZone.nb_checkers > 6)
			posSelectedChecker.y -= 5*20; //mise a jour de y pour selectionner le pion du haut
		else
			posSelectedChecker.y -= (selectedZone.nb_checkers-1)*20; //mise a jour de y pour selectionner le pion du haut
	else
		if(selectedZone.nb_checkers > 6)
			posSelectedChecker.y += 5*20; //mise a jour de y pour selectionner le pion du haut
		else
			posSelectedChecker.y += (selectedZone.nb_checkers-1)*20; //mise a jour de y pour selectionner le pion du haut
			
	SDL_BlitSurface(display->selected_checker, NULL,display->screen, &posSelectedChecker);
}

int allInJan( SGameState* gameState, EPlayer player)
{
	int i;
	if(player == EPlayer1)
	{
		for(i=0;i<18;i++)
		{
			if((gameState->zones[i].player == EPlayer1) && (gameState->zones[i].nb_checkers != 0))
				return 0;
		}
		return 1;
	}
	else
	{
		for(i=23;i>5;i--)
		{
			if((gameState->zones[i].player == EPlayer2) && (gameState->zones[i].nb_checkers != 0))
				return 0;
		}
		return 1;
	}
}

				
int Display_CheckersPossibilities(SDisplay *display, SGameState *gameState, EPlayer player, SGame *game)
{
	int i=0;
	int tab[28];
	int* arriveesValides;
	int cpt=0;
	int posDepartSelected = 0;
	for(int i=0;i<28;i++) tab[i]=-1;
	SDL_Event event;
	
	if(gameState->die1 != gameState->die2) // Si dés différents
	{
		for( i=0; i<24; i++) // Pour chaque flèche
		{
			if(gameState->zones[i].nb_checkers>0 && gameState->zones[i].player==player)
			{
				if( (i+gameState->die1)<=28 && (i+gameState->die1)!=26 && (i+gameState->die1)!=28 && (gameState->zones[i+gameState->die1].nb_checkers==1 || gameState->zones[i+gameState->die1].nb_checkers==0 || gameState->zones[i+gameState->die1].player==player) )
				{
					printf("Possibilite sur fleche %d avec de1 et pion sur fleche %d\n",i+1+gameState->die1,i+1);
					colorChecker(display,gameState,i);
					tab[cpt]=i;
					cpt++;
				}
				if( (i+gameState->die2)<=28 && (i+gameState->die2)!=26 && (i+gameState->die2)!=28 && (gameState->zones[i+gameState->die2].nb_checkers==0 || gameState->zones[i+gameState->die2].nb_checkers==1 || gameState->zones[i+gameState->die2].player==player) )
				{
					printf("Possibilite sur fleche %d avec de2 et pion sur fleche %d\n",i+1+gameState->die2,i+1);
					colorChecker(display,gameState,i);
					tab[cpt]=i;
					cpt++;
				}
				if( (i+gameState->die1+gameState->die2)<=28 && (i+gameState->die1+gameState->die2)!=26 && (i+gameState->die1+gameState->die2)!=28 &&(gameState->zones[i+gameState->die1+gameState->die2].nb_checkers==1 || gameState->zones[i+gameState->die1+gameState->die2].nb_checkers==0 || gameState->zones[i+gameState->die1+gameState->die2].player==player) )
				{
					printf("Possibilite sur fleche %d avec de1+de2 et pion sur fleche %d\n",i+1+gameState->die1+gameState->die2,i+1);
					colorChecker(display,gameState,i);
					tab[cpt]=i;
					cpt++;
				}
			}
		}
		if(cpt == 0) // Pas de pions bougeables
		{
			SDL_Rect msg_position;
			msg_position.x = 80;
			msg_position.y = 325;
			SDL_Color msg_color = {255, 255, 255, 0};
			Display_Message( display, "Pas de mouvement possible!", msg_position, msg_color,1, 0);
			SDL_Flip(display->screen);
			SDL_Delay(10000);
			return 1;
		}
		SDL_Flip(display->screen);
		while(1)
		{
			SDL_WaitEvent(&event);
			EPosition posDepart;
			EPosition posArrivee;
			SMove mouvement;
			switch(event.type)
			{
				case SDL_QUIT:
					if(posDepartSelected)
						free(arriveesValides);
					return 1;
					break;
				
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						//Si il y a clic sur un autre pion de départ cliqué, malgré qu'un pion de départ ait déja été choisi avant
						if(CheckerWithScreenPosition(event.button.x, event.button.y, &posDepart) && inTab(posDepart, tab) && posDepartSelected && !inTab(posDepart, arriveesValides))
						{
							posDepartSelected = !posDepartSelected;
						}
						
						if(!posDepartSelected) // selection de la position depart
						{
							if(CheckerWithScreenPosition(event.button.x, event.button.y, &posDepart)) // il a trouvé une position valide 
							{
								if(inTab(posDepart, tab))
								{
									arriveesValides = Display_Arrow_Possibilities(display, gameState, player, posDepart, game);
									posDepartSelected = 1;
									mouvement.src_point = posDepart+1;
								}
							}
						}
						else // selection de la position arrivée
						{
							if(CheckerWithScreenPosition(event.button.x, event.button.y, &posArrivee)) // il a trouvé une position valide
							{
								printf("POS ARRIVEE\n");
								if(inTab(posArrivee, arriveesValides))
								{
									printf("POS ARRIVEE VALIDE\n");
									mouvement.dest_point = posArrivee+1;
									free(arriveesValides);

									
									printf("JOUEUR %d\tSRC %d\tDEST %d\tDIE1 %d\tDIE2 %d MVT AUTORISE %d\n",player,mouvement.src_point,mouvement.dest_point,gameState->die1,gameState->die2,Arbitrator_AuthorizedDeplacement(gameState, &mouvement, player, game));
									
									Display_CheckerMove(display,gameState,&mouvement, game);
									return 0;
								}
							}
							else // Si clic ailleur on efface les possibilités et on réaffiche les pions déplaçables.
							{
								free(arriveesValides);
								Display_RefreshGameBoard(display, gameState, game);
								return Display_CheckersPossibilities(display, gameState, player, game);
							}
						}
					}
					break;
				}
			}
	}
	return 0;
}

int* Display_Arrow_Possibilities(SDisplay *display, SGameState *gameState, EPlayer player, EPosition depart, SGame *game)
{
	Display_RefreshGameBoard(display, gameState, game);
	colorChecker(display,gameState,depart);
	int *arriveesPossibles;
	arriveesPossibles=(int *)malloc(28*sizeof(int));
	int cpt=0;
	for(int i=0;i<28;i++) arriveesPossibles[i]=-1;
	int sens = 1;
	if(player == EPlayer2)
	{
			sens = -1;
	}
	printf("sens = %d\n",sens);
	if(gameState->die1 != gameState->die2) // Si dés différents
	{
		SZone ArriveeAvecDE1 = gameState->zones[depart+(gameState->die1)*sens];
		SZone ArriveeAvecDE2 = gameState->zones[depart+(gameState->die2)*sens];
		SZone ArriveeAvecDE1ETDE2 = gameState->zones[depart+(gameState->die1)*sens+(gameState->die2)*sens];
		unsigned int arrowWithDie1=0;
		unsigned int arrowWithDie2=0;
		
		if(ArriveeAvecDE1.player == player ||  ArriveeAvecDE1.nb_checkers == 0 || (ArriveeAvecDE1.player != player &&  ArriveeAvecDE1.nb_checkers == 1) )
		{
			//on dessine sur la zone ArriveeAvecDE1
			Display_DrawSelectedArrow(display, depart+(gameState->die1)*sens);
			arrowWithDie1 = 1;
			arriveesPossibles[cpt]= depart+(gameState->die1)*sens;
			cpt++;
		}
		if(ArriveeAvecDE2.player == player ||  ArriveeAvecDE2.nb_checkers == 0 || (ArriveeAvecDE2.player != player &&  ArriveeAvecDE2.nb_checkers == 1))
		{
			//on dessine sur la zone ArriveeAvecDE2
			Display_DrawSelectedArrow(display, depart+(gameState->die2)*sens);
			printf("POSSIBILITE AVEC DE2\n");
			arrowWithDie2 = 1;
			arriveesPossibles[cpt]= depart+(gameState->die2*sens);
			cpt++;
		}
		if( (ArriveeAvecDE1ETDE2.player == player ||  ArriveeAvecDE1ETDE2.nb_checkers == 0 || (ArriveeAvecDE1ETDE2.player != player &&  ArriveeAvecDE1ETDE2.nb_checkers == 1 )) && (arrowWithDie1 || arrowWithDie2) )
		{
			//on dessine sur la zone ArriveeAvecDE1ETDE2
			Display_DrawSelectedArrow(display, depart+(gameState->die1)*sens+(gameState->die2)*sens);
			printf("POSSIBILITE AVEC DE1 et DE2\n");
			arriveesPossibles[cpt]= depart+(gameState->die1)*sens+(gameState->die2)*sens;
			cpt++;
		}
		
	}
	else
	{
		int nbpossibilite=1;
		for(int i=1;i<=4;i++)
		{
			SZone ArriveeAvecDE = gameState->zones[depart+((gameState->die1)*sens*i)];
			if((ArriveeAvecDE.player == player ||  ArriveeAvecDE.nb_checkers == 0 || (ArriveeAvecDE.player != player ||  ArriveeAvecDE.nb_checkers == 1)) && (nbpossibilite==i))
			{
				//on dessine sur la zone ArriveeAvecDE
				Display_DrawSelectedArrow(display, depart+((gameState->die1)*sens*i));
				printf("POSSIBILITE AVEC DE\n");
				nbpossibilite++;
				arriveesPossibles[cpt]= depart+((gameState->die1)*sens*i);
				cpt++;
			}	
		}
	
	}
	SDL_Flip(display->screen);
	return arriveesPossibles;
}

void Display_DrawSelectedArrow(SDisplay *display, EPosition pos)
{
	SDL_Rect posArrow = display->positions[pos];
	posArrow.x = posArrow.x - 1;
	
	if(pos < 12)
	{
		printf("X:%d\tY:%d\n",posArrow.x,posArrow.y);
		posArrow.y = posArrow.y - 128;
		SDL_BlitSurface(display->possibility1_12, NULL,display->screen, &posArrow);
	}
	else if( pos < 24)
	{
		posArrow.y = 145; 
		SDL_BlitSurface(display->possibility13_24, NULL,display->screen, &posArrow);
	}
	else if(pos == 25)
	{
		//posArrow.x = posArrow.x - 2;
		printf("Poss sur out!");
		posArrow.y = 145; 
		SDL_BlitSurface(display->possibility_out, NULL,display->screen, &posArrow);
	}
	else if(pos == 27)
	{
		//posArrow.x = posArrow.x - 2;
		printf("Poss sur out!");
		posArrow.y = posArrow.y - 130;
		SDL_BlitSurface(display->possibility_out, NULL,display->screen, &posArrow);
	}
}

int inTab(EPosition p,int tab[28])
{
	for(int i=0; i<28; i++)
	{
		if(tab[i] == (int)p)
		 return 1;
	}
	return 0;
}


int Display_GameActions(SDisplay *display, SGameState* gameState, SGame *game, EPlayer player)
{

	EPlayer advers = EPlayer1;
	if(player == EPlayer1)
		advers = EPlayer2;
	int accept;
	
	SDL_Rect pos;
	int notSelected = 1;
	SDL_Surface *erase = SDL_CreateRGBSurface(SDL_HWSURFACE, 160, 40, 32, 0, 0, 0, 0);
	pos.x = 630;
	pos.y = 190;
	if(game->withDouble && game->doubleValue<64)
	{
		SDL_BlitSurface(display->gameActions[0], NULL, display->screen,&pos);
	}
	pos.y = 250;
	SDL_BlitSurface(display->gameActions[1], NULL, display->screen,&pos);
	
	SDL_Flip(display->screen);
	SDL_Event event;
	while(1)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				SDL_FreeSurface(erase);
				return 1;
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					if((event.button.x>=630 && event.button.x<=790 && event.button.y>=190 && event.button.y<=235) && game->withDouble && notSelected && game->doubleValue<=32) // Double
					{
						pos.x = 630;
						pos.y = 190;
						SDL_BlitSurface(erase, NULL, display->screen,&pos);
						SDL_Flip(display->screen);
						notSelected = 1;
						
						accept = Game_AcceptDouble(display,advers,game);
						if(accept == -1)
						{
							break;
						}
						if(!accept)
						{
							return 2;
						}
						else
						{
							game->doubleValue *= 2;
							Display_Double(display, game);
						}
					}
					if((event.button.x>=630 && event.button.x<=790 && event.button.y>=250 && event.button.y<=295) ) // Lancer
					{
						Game_LaunchDie(gameState, game);
						pos.x = 630;
						pos.y = 190;
						SDL_BlitSurface(erase, NULL, display->screen,&pos);
						pos.y = 250;
						SDL_BlitSurface(erase, NULL, display->screen,&pos);
						SDL_FreeSurface(erase);
						return 0;
					}
				}
		  		break;

		}
	}
}
