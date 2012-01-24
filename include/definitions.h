#ifndef DEFINITIONS_H
	#define DEFINITIONS_H

	#define WHITE 1
	#define GREEN 0
	

#include "backgammon.h"

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>


/**************************** DEFINITIONS DE NOUVEAUX TYPES *****************************************************/

/* Structure contenant les informations du jeu */
typedef struct Game
{
	char *player1_name; // Nom du Joueur 1
	char *player2_name; // Nom du Joueur 2
	
	int player1_checker; // Couleur des pions du Joueur 1 0=vert 1=blanc
	int player2_checker; // Couleur des pions du Joueur 2
	
	int withDouble; // Présence du double ou non
	
	unsigned 
	int scoreLimit; // Limite de score
}SGame;

/* Différents modes de jeu
		1) Humain contre Humain
		2) Humain contre AI
		3) AI contre AI
		4) Invalide
*/
typedef enum
{
	HUMAN_HUMAN,
	HUMAN_AI,
	AI_AI,
	ERROR=-1
}EGameMode;


typedef struct Display
{
	// Contient le contenu de la fenêtre
	
	SDL_Surface *screen; // Fenêtre principale

	/********************* MENU **************************/
	SDL_Surface *background_menu; // Image de fond du menu
	SDL_Surface *checked; // option cochée
	
	// Arriere plan
	SDL_Surface *background; 
	SDL_Rect background_position; // Sa position
	
	// Plateau de jeu
	SDL_Surface *gameBoard;
	SDL_Rect gameBoard_position; // Sa position
	
	TTF_Font *font;
	
	SDL_Surface *msg_box; // Fond message
	
	SDL_Surface *green_checker; // Pion vert
	
	SDL_Surface *white_checker; // Pion blanc
	
	SDL_Surface *bar_green_checker; // Pion vert dans les barres
	
	SDL_Surface *bar_white_checker; // Pion blanc dans les barres
	
	SDL_Surface *selected_checker; // Les pions Selectionnés
	
	SDL_Surface *die[6]; // Image du dé !! Attention : le dé de valeur i est stocké dans la case i-1 !!
	
	SDL_Rect die1_position; // Position du dé n°1
	
	SDL_Rect die2_position; // Position du dé n°2
	
	SDL_Surface *possibility1_12; // Cadre autour de la flèche pour indiquer une possibilité de déplacement
	
	SDL_Surface *possibility13_24; // Cadre autour de la flèche pour indiquer une possibilité de déplacement
	
	SDL_Surface *possibility_out; // Cadre autour de la zone out pour indiquer une possibilité de déplacement
	
	char *font_path; // Le chemin du dossier contenant les polices
	
	char *img_path; // Le chemin du dossier contenant les images
	
	// Taille par défaut de la fenêtre
	int window_width;
	int window_height;
	
	SDL_Rect positions[28]; // Tableau contenant la position de chaque flèche et des bars	
	
}SDisplay;

typedef struct
{
	void* AI_Handle;
	pfInitLibrary AI_InitLibrary;
	pfStartMatch AI_StartMatch;
	pfEndMatch AI_EndMatch;
	pfStartGame AI_StartGame;
	pfEndGame AI_EndGame;
	pfDoubleStack AI_DoubleStack;
	pfTakeDouble AI_TakeDouble;
	pfMakeDecision AI_MakeDecision;
} SAI_Functions;

/**************************** DEFINITIONS POUR LE MOTEUR DE JEU ******************************/

/* Initialise la partie */
SGameState* Game_Init();

int Game_Play( SDisplay* display, EGameMode gameMode, SGame* game, SAI_Functions* ai_struct);


/* Lancer les dés */
void Game_LaunchDie(SGameState *game);

int Game_FirstToPlay( SDisplay* display, EGameMode gameMode, SGame* game, SGameState *gameState);

/**************************** FIN DEFINITIONS POUR LE MOTEUR DE JEU ******************************/

/**************************** DEFINITIONS POUR L'INTERFACE GRAPHIQUE ******************************/

/* Initialisation de la SDL et de l'affichage */
void Display_Init(SDisplay *display, SGame* game);

/* Libère les surfaces utilisées et quitte la SDL */
void Display_Exit(SDisplay *display);

/* Affiche les pions */
void Display_Checkers(SDisplay *display, SGameState *gameState, SGame *game);

/* Affiche les deux dés */
void Display_Die(SDisplay *display,SGameState *gameState);

/* Affiche les scores */
void Display_Score(SDisplay *display, SGameState *gameState, SGame* game);

/* Réinitialise l'affichage */
void Display_Clear(SDisplay *display);

/* Dessine un pion d'un joueur, à une position donnée */
void Checker_Draw(SDisplay *display, SDL_Rect position, int player, SGame *game);

/* Effectue le déplacement d'un pion */
void Checker_Move(SDisplay *display, SGameState* gameState, SMove *move, SGame* game);

/* Determine si la souris est sur le bouton quitter */
int Quit_Zone(int x, int y);

void Display_Message(SDisplay	*display, char	*message, SDL_Rect position, SDL_Color color, int box);

int Display_Message_Click(SDisplay	*display, char	*message, SDL_Rect position, SDL_Color color, int box);

/*Determine les zones diponibles pour acceuillir un pion*/
int* Display_Arrow_Possibilities(SDisplay *display, SGameState *gameState, EPlayer player, EPosition depart, SGame *game);

/*Dessine un contour de fleche*/
void Draw_Selected_Arrow(SDisplay *display, EPosition pos);

//void Display_Possibilities(SDisplay *display, SGameState *gameState, EPlayer player); // A MODIFIER
int Display_CheckersPossibilities(SDisplay *display, SGameState *gameState, EPlayer player, SGame *game, int Die_For_Play[7]);

/*Donne la position en fonction d'un clic sur le jeu dans le poiteur de EPosition pos
	Retourne Vrai si position trouve, faux sinon.*/
int CheckerWithScreenPosition(int x, int y, EPosition *pos);


/*Colore le pion selectionne*/
void colorChecker(SDisplay *display, SGameState* gameState, EPosition pos);

int Menu_Click(int x, int y);

void Display_RefreshGameBoard(SDisplay *display, SGameState *gameState, SGame *game);

int inTab(EPosition p,int* tab);

void Display_DrawOut(SDisplay *display, SGameState *gameState, SGame *game);
void Display_DrawBar(SDisplay *display, SGameState *gameState, SGame *game);

/**************************** FIN DEFINITIONS POUR L'INTERFACE GRAPHIQUE ******************************/


/**************************** DEFINITIONS POUR LE MENU ******************************/

/* Remplis le menu avec les options correspondants au mode de jeu */
void Menu_Fill(SDisplay *display,EGameMode gameMode, SGame* game);

/* Ajoute à une chaine de caractère un caractère obtenu par l'événement SDL_KEYUP */
void TextInput(char* name, SDL_keysym key);

/* Affiche un texte à une certaine position d'une certaine couleur */
void Menu_Text(SDisplay *display, char	*message, SDL_Rect position,SDL_Color color);

/* Renvoie le code correspond à un élément clickable sur le menu sinon -1 si clic ailleur */
int Menu_Click(int x, int y);

/* Affiche le menu et retourne 0 si jouer ou 1 si quitter */
int Display_Menu(SDisplay *display, EGameMode gameMode, SGame* game);

/**************************** FIN DEFINITIONS POUR LE MENU ******************************/

/**************************** DEFINITIONS POUR LES LIBRAIRIES ******************************/

SAI_Functions* Init_SAI(void);

void Free_SAI( SAI_Functions* ai_struct);

int Load_API( char* path, SAI_Functions* ai_struct, int ind);

EGameMode Check_Args( int argc, char** argv, SAI_Functions* ai_struct);

/**************************** FIN DEFINITIONS POUR LES LIBRAIRIES ******************************/

/**************************** DEFINITIONS POUR L'ARBITRE ******************************/

int authorized_deplacement(SGameState* game, SMove *move, EPlayer player,int Die_For_Play[7]);
int case_appartenant_au_joueur(SZone zone, EPlayer player);
int case_appartenant_joueur_adverse_avec_un_pion(SZone zone, EPlayer player);
int position_vide(SZone zone);
unsigned int get_distance(EPosition depart, EPosition arrivee, EPlayer player);
int sens_rotation_correct(EPlayer joueur, EPosition depart, EPosition arrivee);

/*retourne vrai si le joueur a cliqué sur une zone contenant un pion déplacable*/
int Pion_Depart_Autorise(int x, int y, EPlayer player, SGameState* game,EPosition posDepart);
/*retourne le nombre de dés utilisés pour le mouvement*/
int numberOfDieForMove(SGameState* game, EPlayer player, SMove move);
/*retourne le nombre de dés qui peuvent être joués dans la partie ( utile si le joueur est bloqué par exemple)*/
int numberOfDieCanPlay(SGameState* game, EPlayer player); // A IMPLEMENTER

/*Retourne 1 si le mouvement correspond à un des dés encore jouable, 0 sinon*/
int Taille_Mouvement_Correcte(unsigned int taille_mouvement, int Die_For_Play[7] );
/**************************** FIN DEFINITIONS POUR L'ARBITRE ******************************/
#endif


