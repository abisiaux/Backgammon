#include "backgammon.h"
#include <SDL/SDL.h>

#ifndef GUI_H
	#define GUI_H

struct Display
{
	// Contient le contenu de la fenêtre
	
	SDL_Surface *screen; // Fenêtre principale
	
	// Arriere plan
	SDL_Surface *background; 
	SDL_Rect background_position; // Sa position
	
	SDL_Surface *green_checker; // Les pions verts
	
	SDL_Surface *white_checker; // Les pions blancs
	
	SDL_Surface *button_play; // Bouton jouer
	
	SDL_Surface *die1[6]; // Dé n°1  !! Attention : la dé de valeur i est stocké dans la case i-1 !!
	
	SDL_Surface *die2[6]; // Dé n°2 !! Attention : la dé de valeur i est stocké dans la case i-1 !!
	
	SDL_Rect die1_position; // Position du dé n°1
	
	SDL_Rect die2_position; // Position du dé n°2
	
	char *img_path; // Le chemin du dossier contenant les images
	
	// Taille par défaut de la fenêtre
	int window_width;
	int window_height;
	
	SDL_Rect positions[28]; // Tableau contenant la position de chaque flèche et des bars
};

typedef struct Display SDisplay;

/* Initialisation de la SDL et de l'affichage */
void display_init(SDisplay *display);

/* Libère les surfaces utilisées et quitte la SDL */
void display_exit(SDisplay *display);

/* Affiche les pions */
void display_checkers(SDisplay *display, SGameState *game);

/* Affiche les deux dés */
void display_die(SDisplay *display,SGameState *game);

/* Lancer les dés */
void launch_die(SGameState *game);

/* Raffraichit l'affichage */
void display_refresh(SDisplay *display, SGameState *game);

/* Dessine un pion d'un joueur, à une position donnée */
void draw_checker(SDisplay *display, SDL_Rect position, int player);

/* Initialise la partie */
SGameState* initPartie();

#endif


