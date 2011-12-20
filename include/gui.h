#include "backgammon.h"
#include <SDL/SDL.h>

#ifndef GUI_H
	#define gui_H

struct Display
{
	// Contient le contenu de la fenêtre
	
	SDL_Surface *screen; // Fenêtre principale
	
	// Arriere plan
	SDL_Surface *background; 
	SDL_Rect background_position; // Sa position
	
	SDL_Surface *black_checker; // Les pions noirs
	
	SDL_Surface *white_checker; // Les pions blancs
	
	char *img_path; // Le chemin du dossier contenant les images
	
	// Taille par défaut de la fenêtre
	int window_width;
	int window_height;
};

typedef struct Display SDisplay;

/* Initialisation de la SDL et de l'affichage */
void display_init(SDisplay *display);

/* Libère les surfaces utilisées et quitte la SDL */
void display_exit(SDisplay *display);

/*void drawGame(SWindow *w, SZone zones[28]);
void drawChecker(SWindow *w,SDL_Rect *position,SColor c);
SColor checkerColor(EPlayer e);
void initPartie(SZone *zones);*/

#endif


