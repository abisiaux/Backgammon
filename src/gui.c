#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h> // Pour utiliser des formats d'image autre que bmp
#include <SDL/SDL_ttf.h> // Pour la gestion des textes sous SDL

#include "../include/definitions.h"
#include "../include/backgammon.h"


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
	display->img_path = "ressources/pictures/";
	
	// Enregistrement du path des polices
	display->font_path = "ressources/fonts/";
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
	
	// Chargement de l'image de pion noir
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "green_checker.png");
	display->green_checker = IMG_Load(temp_path);
	
	//Chargement de l'image de pion selectionne
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "selected_checker.png");
	display->selected_checker = IMG_Load(temp_path);
	
	// Chargement du cadre de possibilité
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "flecheSelected1_12.png");
	display->possibility1_12 = IMG_Load(temp_path);
	
	// Chargement du cadre de possibilité
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "flecheSelected13_24.png");
	display->possibility13_24 = IMG_Load(temp_path);
	
	
	// Chargement de l'image du cadre d'un message
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "msgBox.png");
	display->msg_box = IMG_Load(temp_path);
	
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
	// !!! AJOUTER POSITIONS PRISONNIERS,... !!!!
	
	
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
	game->scoreLimit = 5; // Par défaut, limite de score est de 
	
	free(temp_path); // On libère
	free(temp_name);
}


void Display_Message(SDisplay	*display, char	*message, SDL_Rect position, SDL_Color color, int box)
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
}

int Display_Message_Click(SDisplay	*display, char	*message, SDL_Rect position, SDL_Color color, int box)
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
	SDL_FreeSurface(display->selected_checker);
	SDL_FreeSurface(display->msg_box);
	SDL_FreeSurface(display->possibility1_12);
	SDL_FreeSurface(display->possibility13_24);
	
	for( i=0; i<6; i++ )
	{
		SDL_FreeSurface(display->die[i]);
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

void Display_RefreshGameBoard(SDisplay *display, SGameState *game)
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
	Display_Checkers(display, game);
	
	// Raffraichissement des dés
	Display_Die(display,game);
	
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
	Display_Message(display, game->player1_name, pos, c, 0);
	pos.x = 310;
	Display_Message(display, scoreP1, pos, c, 0);
	pos.y = 85;
	Display_Message(display, scoreP2, pos, c, 0);
	pos.x = 108;
	Display_Message(display, game->player2_name, pos, c, 0);
	
	free(scoreP1);
	free(scoreP2);
}


void Display_Die(SDisplay *display,SGameState *game)
{
	SDL_Rect new_pos;
	
	if( game->die1 == game->die2 ) // Si c'est un double, on affiche 4 fois le dé
	{
		new_pos.x = (display->die1_position).x - 40;
		new_pos.y = (display->die1_position).y;
		SDL_BlitSurface(display->die[game->die1-1], NULL,display->screen, &new_pos);
		new_pos.x = (display->die1_position).x;
		SDL_BlitSurface(display->die[game->die1-1], NULL,display->screen, &new_pos);
		new_pos.x = (display->die1_position).x + 40;
		SDL_BlitSurface(display->die[game->die1-1], NULL,display->screen, &new_pos);
		new_pos.x = (display->die1_position).x + 80;
		SDL_BlitSurface(display->die[game->die1-1], NULL,display->screen, &new_pos);
	}
	else // Sinon simple affichage des deux dés
	{
		SDL_BlitSurface(display->die[game->die1-1], NULL,display->screen, &display->die1_position);
		SDL_BlitSurface(display->die[game->die2-1], NULL,display->screen, &display->die2_position);
	}
}


void Checker_Draw(SDisplay *display, SDL_Rect position, int player)
{
	if( player==0 )
	{
		SDL_BlitSurface(display->green_checker, NULL,display->screen, &position);
	}
	else
	{
		SDL_BlitSurface(display->white_checker, NULL,display->screen, &position);
	}
}

void Display_Checkers(SDisplay *display, SGameState *game)
{
	unsigned int i,j;
	SDL_Rect new_pos;
	SDL_Surface *num;
	char* tmp = (char*)malloc(2*sizeof(char));
	SDL_Color color = {0, 0, 0, 0};
	
	for( i=0 ; i<28; i++ )
	{
		for( j=0; j<game->zones[i].nb_checkers; j++)
		{
			if(j>5)
			{
				
				if(game->zones[i].nb_checkers>9) // Pour centrer l'affichage en cas de plus de 10 jetons
				{
					new_pos.x += 5;
				}else
				{
					new_pos.x += 8;
				}
				sprintf(tmp, "%d", game->zones[i].nb_checkers);
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
				Checker_Draw(display,new_pos,game->zones[i].player);
				
			}
		}
	}
	free(tmp);
}



void Checker_Move(SDisplay *display, SGameState* game, SMove *move)
{
	/* Calcul de la droite passant par les deux zones */
	int pos_src_x;
	int pos_src_y;
	int pos_dest_x;
	int pos_dest_y;
	
	pos_src_x = display->positions[move->src_point-1].x;
	
	if(move->src_point>12)
	{
		if(game->zones[move->src_point-1].nb_checkers>6)
			pos_src_y =  display->positions[move->src_point-1].y + 20*6;
		else
			pos_src_y =  display->positions[move->src_point-1].y + 20*(game->zones[move->src_point-1].nb_checkers);
	}
	else
	{
		if(game->zones[move->src_point-1].nb_checkers>6)
			pos_src_y =  display->positions[move->src_point-1].y - 20*6;
		else
			pos_src_y =  display->positions[move->src_point-1].y - 20*(game->zones[move->src_point-1].nb_checkers);
	}
	
	pos_dest_x = display->positions[move->dest_point-1].x;
	
	if(move->dest_point>12)
	{
		if(game->zones[move->dest_point-1].nb_checkers>6)
			pos_dest_y = display->positions[move->dest_point-1].y + 20*6;
		else
			pos_dest_y = display->positions[move->dest_point-1].y + 20*(game->zones[move->dest_point-1].nb_checkers);
	}
	else
	{
		if(game->zones[move->dest_point-1].nb_checkers>6)
			pos_dest_y = display->positions[move->dest_point-1].y - 20*6;
		else
			pos_dest_y = display->positions[move->dest_point-1].y - 20*(game->zones[move->dest_point-1].nb_checkers);
	}
	
	double a = (double)(pos_src_y-pos_dest_y)/(pos_src_x - pos_dest_x);
	double b = pos_dest_y - a*pos_dest_x;
	
	/*printf("SRC = fleche %d x=%d y=%d\n",move->src_point,pos_src_x,pos_src_y);
	printf("DEST = fleche %d x=%d y=%d\n",move->dest_point,pos_dest_x,pos_dest_y);
	printf("a=%f b=%f\n",a,b);*/
	
	int pas = 6; // Vitesse de deplacement du pion 1=lentement 8=rapide
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
	game->zones[move->src_point-1].nb_checkers--;
	while( !depassement )
	{
		new_pos.x = new_pos.x + pas;
		new_pos.y = (int)(a*new_pos.x + b);
		Display_RefreshGameBoard(display,game);
		Checker_Draw(display,new_pos,game->zones[move->src_point-1].player);
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
	game->zones[move->dest_point-1].nb_checkers++;
	game->zones[move->dest_point-1].player = game->zones[move->src_point-1].player;
	Display_RefreshGameBoard(display, game);
}

/*void Display_DrawPossibility(SDisplay *display,EPosition numFleche)
{
	SDL_Rect new_pos = display->positions[numFleche-1];
	new_pos.x = new_pos.x -5;
	if(numFleche<13)
		new_pos.y = new_pos.y - 140;
		
	SDL_BlitSurface(display->possibility, NULL,display->screen, &new_pos);
	
}*/

/*

void Display_Possibilities(SDisplay *display, SGameState *game, EPlayer player)
{
	int i=0;
	
	if(game->die1 != game->die2) // Si dés différents
	{
		for( i=0; i<24; i++) // Pour chaque flèche
		{
			if(game->zones[i].nb_checkers>0 && game->zones[i].player==player)
			{
				if( (i+game->die1)<24 && (game->zones[i+game->die1].nb_checkers==0 || game->zones[i+game->die1].player==player) )
				{
					printf("Possibilite sur fleche %d avec de1 et pion sur fleche %d\n",i+1+game->die1,i+1);
					draw_possibility(display,i+1+game->die1);
				}
				if( (i+game->die2)<24 && (game->zones[i+game->die2].nb_checkers==0 || game->zones[i+game->die2].player==player) )
				{
					printf("Possibilite sur fleche %d avec de2 et pion sur fleche %d\n",i+1+game->die2,i+1);
					draw_possibility(display,i+1+game->die2);
				}
				if( (i+game->die1+game->die2)<24 && (game->zones[i+game->die1+game->die2].nb_checkers==0 || game->zones[i+game->die1+game->die2].player==player) )
				{
					printf("Possibilite sur fleche %d avec de1+de2 et pion sur fleche %d\n",i+1+game->die1+game->die2,i+1);
					draw_possibility(display,i+1+game->die1+game->die2);
				}
			}
		}
	}
	SDL_Flip(display->screen);
}
*/


int Quit_Zone(int x, int y)
{

	if( (x >= 635 && x <= 795) && (y >=244 && y <= 325) )
	{
		return 1;
	}
	return 0;
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
/*
void Display_CheckersPossibilities(SDisplay *display, SGameState *game, EPlayer player)
{
	int i=0;

	
	if(game->die1 != game->die2) // Si dés différents
	{
		for( i=0; i<24; i++) // Pour chaque flèche
		{
			if(game->zones[i].nb_checkers>0 && game->zones[i].player==player)
			{
				if( (i+game->die1)<24 && (game->zones[i+game->die1].nb_checkers==0 || game->zones[i+game->die1].player==player) )
				{
					printf("Possibilite sur fleche %d avec de1 et pion sur fleche %d\n",i+1+game->die1,i+1);
					colorChecker(display,game,i);
			
				}
				if( (i+game->die2)<24 && (game->zones[i+game->die2].nb_checkers==0 || game->zones[i+game->die2].player==player) )
				{
					printf("Possibilite sur fleche %d avec de2 et pion sur fleche %d\n",i+1+game->die2,i+1);
					colorChecker(display,game,i);
				
				}
				if( (i+game->die1+game->die2)<24 && (game->zones[i+game->die1+game->die2].nb_checkers==0 || game->zones[i+game->die1+game->die2].player==player) )
				{
					printf("Possibilite sur fleche %d avec de1+de2 et pion sur fleche %d\n",i+1+game->die1+game->die2,i+1);
					colorChecker(display,game,i);
					
				}
			}
		}
	}
	//SDL_Flip(display->screen);
}

*/
int Display_CheckersPossibilities(SDisplay *display, SGameState *game, EPlayer player)
{
	int i=0;
	int * tab=(int *)malloc(28*sizeof(int));
	int* arriveesValides;
	int cpt=0;
	int posDepartSelected = 0;
	for(int i=0;i<28;i++) tab[i]=-1;
	SDL_Event event;
	
	if(game->die1 != game->die2) // Si dés différents
	{
		for( i=0; i<24; i++) // Pour chaque flèche
		{
			if(game->zones[i].nb_checkers>0 && game->zones[i].player==player)
			{
				if( (i+game->die1)<24 && (game->zones[i+game->die1].nb_checkers==0 || game->zones[i+game->die1].player==player) )
				{
					printf("Possibilite sur fleche %d avec de1 et pion sur fleche %d\n",i+1+game->die1,i+1);
					colorChecker(display,game,i);
					tab[cpt]=i;
					cpt++;
				}
				if( (i+game->die2)<24 && (game->zones[i+game->die2].nb_checkers==0 || game->zones[i+game->die2].player==player) )
				{
					printf("Possibilite sur fleche %d avec de2 et pion sur fleche %d\n",i+1+game->die2,i+1);
					colorChecker(display,game,i);
					tab[cpt]=i;
					cpt++;
				}
				if( (i+game->die1+game->die2)<24 && (game->zones[i+game->die1+game->die2].nb_checkers==0 || game->zones[i+game->die1+game->die2].player==player) )
				{
					printf("Possibilite sur fleche %d avec de1+de2 et pion sur fleche %d\n",i+1+game->die1+game->die2,i+1);
					colorChecker(display,game,i);
					tab[cpt]=i;
					cpt++;
				}
			}
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
					return 1;
					break;
				
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						//Si il y a clic sur un autre pion de départ possible, malgré qu'un pion de départ ait déja été choisi
						if(CheckerWithScreenPosition(event.button.x, event.button.y, &posDepart) && inTab(posDepart, tab) && posDepartSelected)
						{
							posDepartSelected = !posDepartSelected;
						}
						
						if(!posDepartSelected) // selection de la position depart
						{
							if(CheckerWithScreenPosition(event.button.x, event.button.y, &posDepart)) // il a trouvé une position valide 
							{
								if(inTab(posDepart, tab))
								{
									arriveesValides = Display_Arrow_Possibilities(display, game, player, posDepart);
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
									Checker_Move(display,game,&mouvement);
									return 0;
								}
							}
						}
					}
					break;
				}
			}
	}
	free(arriveesValides);
	free(tab);
	//SDL_Flip(display->screen);
}

/* BUG!!!!
int Display_WaitMove(SDisplay *display, SGameState* game, EPlayer player)
{
	EPosition tab[28];
	
	int quit = 0;
	int i,j;
	SDL_Event event;
	
	Display_CheckersPossibilities(display, game, player, tab);
	//SDL_Flip(display->screen);
	
	while(!quit)
	{
		SDL_WaitEvent(&event);
		
		switch(event.type)
		{
			case SDL_QUIT:
				quit = 1;
				break;
				
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					for(i=0;i<28;i++)
					{
						if(tab[i]!=-1 && CheckerWithScreenPosition(event.button.x, event.button.y,&tab[i]))
							printf("Clic sur pion %d\n",i);
					}
					//quit = 1;
				}
		  		break;

		}
	}
	return 0;
}
*/

int* Display_Arrow_Possibilities(SDisplay *display, SGameState *game, EPlayer player, EPosition depart)
{
	Display_RefreshGameBoard(display, game);
	colorChecker(display,game,depart);
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
	if(game->die1 != game->die2) // Si dés différents
	{
		SZone ArriveeAvecDE1 = game->zones[depart+(game->die1)*sens];
		SZone ArriveeAvecDE2 = game->zones[depart+(game->die2)*sens];
		SZone ArriveeAvecDE1ETDE2 = game->zones[depart+(game->die1)*sens+(game->die2)*sens];
		unsigned int arrowWithDie1=0;
		unsigned int arrowWithDie2=0;
		
		if(ArriveeAvecDE1.player == player ||  ArriveeAvecDE1.nb_checkers == 0)
		{
			//on dessine sur la zone ArriveeAvecDE1
			Draw_Selected_Arrow(display, depart+(game->die1)*sens);
			arrowWithDie1 = 1;
			arriveesPossibles[cpt]= depart+(game->die1)*sens;
			cpt++;
		}
		if(ArriveeAvecDE2.player == player ||  ArriveeAvecDE2.nb_checkers == 0)
		{
			//on dessine sur la zone ArriveeAvecDE2
			Draw_Selected_Arrow(display, depart+(game->die2)*sens);
			printf("POSSIBILITE AVEC DE2\n");
			arrowWithDie2 = 1;
			arriveesPossibles[cpt]= depart+(game->die2*sens);
			cpt++;
		}
		if( (ArriveeAvecDE1ETDE2.player == player ||  ArriveeAvecDE1ETDE2.nb_checkers == 0) && (arrowWithDie1 || arrowWithDie2) )
		{
			//on dessine sur la zone ArriveeAvecDE1ETDE2
			Draw_Selected_Arrow(display, depart+(game->die1)*sens+(game->die2)*sens);
			printf("POSSIBILITE AVEC DE1 et DE2\n");
			arriveesPossibles[cpt]= depart+(game->die1)*sens+(game->die2)*sens;
			cpt++;
		}
		
	}
	else
	{
		int nbpossibilite=1;
		for(int i=1;i<=4;i++)
		{
			SZone ArriveeAvecDE = game->zones[depart+((game->die1)*sens*i)];
			if((ArriveeAvecDE.player == player ||  ArriveeAvecDE.nb_checkers == 0) && (nbpossibilite==i))
			{
				//on dessine sur la zone ArriveeAvecDE
				Draw_Selected_Arrow(display, depart+((game->die1)*sens*i));
				printf("POSSIBILITE AVEC DE\n");
				nbpossibilite++;
				arriveesPossibles[cpt]= depart+((game->die1)*sens*i);
				cpt++;
			}	
		}
	
	}
	SDL_Flip(display->screen);
	return arriveesPossibles;
}

void Draw_Selected_Arrow(SDisplay *display, EPosition pos)
{
	SDL_Rect posArrow = display->positions[pos];
	posArrow.x = posArrow.x - 2;
	
	if(pos < 12)
	{
		printf("X:%d\tY:%d\n",posArrow.x,posArrow.y);
		posArrow.y = posArrow.y - 130;
		SDL_BlitSurface(display->possibility1_12, NULL,display->screen, &posArrow);
	}
	else if( pos < 24)
	{
		posArrow.y = 145; 
		SDL_BlitSurface(display->possibility13_24, NULL,display->screen, &posArrow);
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
 
