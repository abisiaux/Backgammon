#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h> // Pour utiliser des formats d'image autre que bmp

#include "../include/gui.h"
#include "../include/backgammon.h"


void display_init(SDisplay *display)
{	
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
	strcat(temp_path, "black_checker.png");
	display->black_checker = IMG_Load(temp_path);
	free(temp_path);
	
	// Initialisation de la taille de la fenêtre
	display->window_width = 800;
	display->window_height = 600;
	
	// Initialisation de la fenêtre
	display->screen = SDL_SetVideoMode(780, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	
	if (display->screen == NULL) // Si l'ouverture a échoué, on écrit l'erreur et on arrête
	{
		fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
    
	SDL_WM_SetCaption("Backgammon",NULL);
}


void display_exit(SDisplay *display)
{
	// Libération de toutes les surfaces utilisées
	SDL_FreeSurface(display->background);
	SDL_FreeSurface(display->white_checker);
	SDL_FreeSurface(display->black_checker);
	SDL_FreeSurface(display->screen);
	
	SDL_Quit();
}

void display_refresh(SDisplay* display)
{
	SDL_BlitSurface(display->background, NULL, display->screen, &(display->background_position));
}



