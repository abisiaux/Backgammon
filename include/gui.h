#include "backgammon.h"
#include "library.h"

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#ifndef GUI_H
	#define GUI_H

	#define WHITE 1
	#define GREEN 0

struct Game
{
	char *player1_name; // Nom du Joueur 1
	char *player2_name; // Nom du Joueur 2
	
	int player1_checker; // Couleur des pions du Joueur 1
	int player2_checker; // Couleur des pions du Joueur 2
	
	int withDouble; // Présence du double ou non
	
	int scoreLimit; // Limite de score
};

typedef struct Game SGame;

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
	
	SDL_Surface *msg_box; // Fond message
	
	SDL_Surface *green_checker; // Les pions verts
	
	SDL_Surface *white_checker; // Les pions blancs
	
	SDL_Surface *selected_checker; // Les pions Selectionnés
	
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
	
	SGame *game;
};



typedef struct Display SDisplay;

/* Initialisation de la SDL et de l'affichage */
void Display_Init(SDisplay *display);

/* Libère les surfaces utilisées et quitte la SDL */
void Display_Exit(SDisplay *display);

/* Affiche les pions */
void Display_Checkers(SDisplay *display, SGameState *game);

/* Affiche les deux dés */
void Display_Die(SDisplay *display,SGameState *game);

/* Affiche les scores */
void Display_Score(SDisplay *display, SGameState *game);

/* Lancer les dés */
void Launch_Die(SGameState *game);

/* Raffraichit l'affichage */
void Display_Refresh(SDisplay *display, SGameState *game);

/* Réinitialise l'affichage */
void Display_Clear(SDisplay *display);

/* Dessine un pion d'un joueur, à une position donnée */
void Checker_Draw(SDisplay *display, SDL_Rect position, int player);

/* Initialise la partie */
SGameState* Game_Init();

/* Effectue le déplacement d'un pion */
void Checker_Move(SDisplay *display, SGameState* game, SMove *move);

/* Determine si la souris est sur le bouton quitter */
int Quit_Zone(int x, int y);

void Display_Message(SDisplay	*display, char	*message, SDL_Rect position, SDL_Color color, int box);

void Display_Possibilities(SDisplay *display, SGameState *game, EPlayer player); // A MODIFIER

/*Donne la position en fonction d'un clic sur le jeu dans le poiteur de EPosition pos
	Retourne Vrai si position trouve, faux sinon.*/
int CheckerWithScreenPosition(int x, int y, EPosition *pos);


/*Colore le pion selectionne*/
void colorChecker(SDisplay *display, SGameState* game, EPosition pos);

/*Colore le pion selectionne*/
void colorChecker(SDisplay *display, SGameState* game, EPosition pos);


int Menu_Click(int x, int y);

#endif


