#include "backgammon.h"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#ifndef GUI_H
	#define GUI_H

struct Display
{
	// Contient le contenu de la fenêtre
	
	SDL_Surface *screen; // Fenêtre principale

	/********************* MENU **************************/
	SDL_Surface *background_menu; // Image de fond du menu
	SDL_Surface *checked; // option cochée
	
	// Arriere plan
	SDL_Surface *background; 
	SDL_Rect background_position; // Sa position
	
	TTF_Font *font;
	
	SDL_Surface *green_checker; // Les pions verts
	
	SDL_Surface *white_checker; // Les pions blancs
	
	SDL_Surface *button_play; // Bouton jouer
	
	SDL_Surface *die[6]; // Image du dé !! Attention : le dé de valeur i est stocké dans la case i-1 !!
	
	SDL_Rect die1_position; // Position du dé n°1
	
	SDL_Rect die2_position; // Position du dé n°2
	
	SDL_Surface *possibility; // Cadre autour de la flèche pour indiquer une possibilité de déplacement
	
	char *font_path; // Le chemin du dossier contenant les polices
	
	char *img_path; // Le chemin du dossier contenant les images
	
	// Taille par défaut de la fenêtre
	int window_width;
	int window_height;
	
	SDL_Rect positions[28]; // Tableau contenant la position de chaque flèche et des bars
	
	char *player1_name;
	char *player2_name;
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

/* Effectue le déplacement d'un pion */
void checker_move(SDisplay *display, SGameState* game, SMove *move);

/* Determine si la souris est sur le bouton quitter */
int zoneQuit(int x, int y);

/* Affiche le menu / Retourne -1 si événement quitter sinon retourne le nombre de joueur si évenement jouer */
int display_menu(SDisplay *display, int nbPlayer, int *color_checker, int *withDouble);

/* Retourne un entier correspondant à un événement clic sur le menu */
int click_menu(int x, int y);

void display_message(SDisplay	*display, char	*message, SDL_Rect position, SDL_Color color);
#endif


