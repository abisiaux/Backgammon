#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h> // Pour utiliser des formats d'image autre que bmp

#include "../include/gui.h"
#include "../include/backgammon.h"


void display_init(SDisplay *display)
{	
	int i;
	// Initialisation de la SDL
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de la SDL");
		exit(EXIT_FAILURE);
	}
	
	// Enregistrement du path des images
	display->img_path = "../ressources/pictures/";
	char *temp_path = (char*)malloc(100*sizeof(char));
	
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
	
	// Chargement de l'image de pion blanc
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "white_checker.png");
	display->white_checker = IMG_Load(temp_path);
	
	// Chargement de l'image de pion noir
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "green_checker.png");
	display->green_checker = IMG_Load(temp_path);
	
	// Chargement de l'image du bouton jouer
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "button_play.png");
	display->button_play = IMG_Load(temp_path);
	
	// Chargement des images deux dés et initialisation de leur position
	char *temp_name = (char*)malloc(100*sizeof(char));

	for( i=1; i<=6; i++ )
	{
		strcpy(temp_path, display->img_path);
		sprintf(temp_name,"die/die[%d].png",i);
		strcat(temp_path,temp_name);
		display->die1[i-1] = IMG_Load(temp_path);
		display->die2[i-1] = IMG_Load(temp_path);
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
	display->positions[0].x = 525;
	display->positions[0].y = 570;
	
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
		else if( i<17 )
			display->positions[i].x = display->positions[i-1].x + 38;
		else if( i==17 )
			display->positions[i].x=display->positions[i-1].x + 74;
		else
			display->positions[i].x=display->positions[i-1].x + 38;
	}
	
	// Initialisation de la fenêtre
	display->screen = SDL_SetVideoMode(display->window_width, display->window_height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	
	if (display->screen == NULL) // Si l'ouverture a échoué, on écrit l'erreur et on arrête
	{
		fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
    
	SDL_WM_SetCaption("Backgammon",NULL);
	free(temp_path); // On libère temp_path car on ne l'utilise plus
}


void display_exit(SDisplay *display)
{
	// Libération de toutes les surfaces utilisées
	SDL_FreeSurface(display->background);
	SDL_FreeSurface(display->white_checker);
	SDL_FreeSurface(display->green_checker);
	SDL_FreeSurface(display->button_play);
	int i;
	for( i=0; i<6; i++ )
	{
		SDL_FreeSurface(display->die1[i]);
		SDL_FreeSurface(display->die2[i]);
	}
	SDL_FreeSurface(display->screen);
	
	SDL_Quit();
}

void display_clear(SDisplay *display)
{
	// Rechargement du background
	char *temp_path = (char*)malloc(100*sizeof(char));
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "background.png");
	display->background = IMG_Load(temp_path);
	free(temp_path);
	
	SDL_BlitSurface(display->background, NULL, display->screen, &(display->background_position));
}
	
void display_refresh(SDisplay *display, SGameState *game)
{
	display_clear(display);
	
	// Affichage des pions
	display_checkers(display, game);
	
	// Affichage des dés
	display_die(display,game);
}

void launch_die(SGameState *game)
{
	srand( time(NULL) );
	game->die1 = 1+(rand()/(float)RAND_MAX)*6; // Tirage aléatoire d'un chiffre entre 1 et 6
	game->die2 = 1+(rand()/(float)RAND_MAX)*6; // Tirage aléatoire d'un chiffre entre 1 et 6
	printf("Dé 1 : %d Dé 2 : %d\n",game->die1,game->die2);
}
void display_die(SDisplay *display,SGameState *game)
{
	if( game->die1 == game->die2 ) // Si c'est un double, on affiche 4 fois le dé
	{
		SDL_Rect new_pos;
		new_pos.x = (display->die1_position).x - 40;
		new_pos.y = (display->die1_position).y;
		SDL_BlitSurface(display->die1[game->die1-1], NULL,display->screen, &new_pos);
		new_pos.x = (display->die1_position).x;
		SDL_BlitSurface(display->die1[game->die1-1], NULL,display->screen, &new_pos);
		new_pos.x = (display->die1_position).x + 40;
		SDL_BlitSurface(display->die1[game->die1-1], NULL,display->screen, &new_pos);
		new_pos.x = (display->die1_position).x + 80;
		SDL_BlitSurface(display->die1[game->die1-1], NULL,display->screen, &new_pos);
	}
	else
	{
		SDL_BlitSurface(display->die1[game->die1-1], NULL,display->screen, &display->die1_position);
		SDL_BlitSurface(display->die2[game->die2-1], NULL,display->screen, &display->die2_position);
	}
}
void draw_checker(SDisplay *display, SDL_Rect position, int player)
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

void display_checkers(SDisplay *display, SGameState *game)
{
	unsigned int i,j;
	SDL_Rect new_pos;
	
	for( i=0 ; i<28; i++ )
	{
		for( j=0; j<game->zones[i].nb_checkers; j++)
		{
			new_pos.x = display->positions[i].x;
			if( i<12)
				new_pos.y = display->positions[i].y - j*20;
			else
				new_pos.y = display->positions[i].y + j*20;
				
			draw_checker(display,new_pos,game->zones[i].player);
		}
	}
}

SGameState* initPartie()
{
	SGameState *game = (SGameState*)malloc(sizeof(SGameState));
	int i;
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


