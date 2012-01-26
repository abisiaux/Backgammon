/**
 * \file definitions.h
 * \brief Contient les signatures des fonctions et les différentes structures.
 * \author Alexandre BISIAUX et Antonin BIRET
 * \date 26 janvier 2012
 *
 *	Contient les signatures des fonctions et les différentes structures.
 *
 */


#ifndef DEFINITIONS_H
	#define DEFINITIONS_H

	#define WHITE 1
	#define GREEN 0
	

#include "backgammon.h"

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

/**
 * \struct SGame
 * \brief Structure contenant les informations du jeu.
 *
 * Contient les informations du jeu à savoir
 * le nom des deux joueurs, la couleur de leurs pions,
 * l'utilisation du double, sa valeur, la limite de score
 * et le tableau de statut des dés.
 *
 */
typedef struct Game
{
	char *player1_name;
	char *player2_name;
	
	int player1_checker;
	int player2_checker;
	
	int withDouble;
	int doubleValue;
	
	unsigned int scoreLimit;
	
	int die_To_Play[4];
}SGame;

/**
 * \enum EGameMode
 * \brief Enumération des modes de jeu.
 *
 * 4 Modes de jeu :
 *		1) Humain contre Humain
 *		2) Humain contre IA
 *		3) IA contre IA
 *		4) Invalide
 *
 */
typedef enum
{
	HUMAN_HUMAN,
	HUMAN_IA,
	IA_IA,
	ERROR=-1
}EGameMode;

/**
 * \struct SDisplay
 * \brief Structure d'affichage.
 *
 * Contient les éléments nécessaires à l'affichage
 *
 */
typedef struct Display
{
	
	SDL_Surface *screen; // Fenêtre principale

	char *font_path; // Le chemin du dossier contenant les polices
	
	char *img_path; // Le chemin du dossier contenant les images
	
	// Taille par défaut de la fenêtre
	int window_width;
	int window_height;
	
	/********************* MENU **************************/
	SDL_Surface *background_menu; // Image de fond du menu
	SDL_Surface *checked; // option cochée
	/********************* FIN MENU *********************/
	
	SDL_Surface *background; // Arriere plan
	SDL_Rect background_position; // Position de l'arrière plan
	
	SDL_Surface *gameBoard;// Plateau de jeu
	SDL_Rect gameBoard_position;// Position du plateau de jeu
	
	TTF_Font *font; // Police d'écriture
	
	SDL_Surface *msg_box; // Fond message
	
	SDL_Surface *green_checker; // Pion vert
	
	SDL_Surface *white_checker; // Pion blanc
	
	SDL_Surface *out_green_checker; // Pion vert dans les sorties
	
	SDL_Surface *out_white_checker; // Pion blanc dans les sorties
	
	SDL_Surface *selected_checker; // Pion selectionné
	
	SDL_Surface *videau; // Image du videau
	
	SDL_Rect videau_position; // Position du videau
	
	SDL_Surface *die[6]; // Image du dé !! Attention : le dé de valeur i est stocké dans la case i-1 !!
	
	SDL_Surface *die_played[6]; // Image des dés joués !! Attention : le dé de valeur i est stocké dans la case i-1 !!
	
	SDL_Rect die1_position; // Position du dé n°1
	
	SDL_Rect die2_position; // Position du dé n°2
	
	SDL_Surface *possibility1_12; // Cadre autour de la flèche pour indiquer une possibilité de déplacement
	
	SDL_Surface *possibility13_24; // Cadre autour de la flèche pour indiquer une possibilité de déplacement
	
	SDL_Surface *possibility_out; // Cadre autour de la zone out pour indiquer une possibilité de déplacement
	
	SDL_Surface *gameActions[2]; // Actions du jeu
	
	SDL_Rect positions[28]; // Tableau contenant la position de chaque flèche et des bars	
}SDisplay;

/**
 * \struct SIA_Functions
 * \brief Structure des fonctions de l'IA.
 *
 * Contient les pointeurs de fonctions sur les fonctions de l'IA
 *
 */
typedef struct
{
	void* IA_Handle;
	pfInitLibrary IA_InitLibrary;
	pfStartMatch IA_StartMatch;
	pfEndMatch IA_EndMatch;
	pfStartGame IA_StartGame;
	pfEndGame IA_EndGame;
	pfDoubleStack IA_DoubleStack;
	pfTakeDouble IA_TakeDouble;
	pfMakeDecision IA_MakeDecision;
} SIA_Functions;

/**************************** SPECIFICATION MOTEUR DE JEU ******************************/

/** \fn SGameState* Game_Init()
 * \brief Fonction qui permet d'initialiser l'état du jeu (disposition des jetons au début de la partie).
 *	\return SGameState* Structure d'état du jeu
 */
SGameState* Game_Init();

/**
 * \fn int Game_Play()
 * \brief Lance la boucle de jeu.
 * \param SDisplay* display Structure d'affichage
 * \param EGameMode gameMode Mode de jeu
 * \param SGame* game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 * \param SIA_Functions* IA_struct Tableau de structures contenant les fonctions de l'AI
 *	\return int Retourne 1 si quitter / 0 si bon déroulement
 */
int Game_Play( SDisplay* display, EGameMode gameMode, SGame* game, SIA_Functions* IA_struct);

/**
 * \fn void Game_LaunchDie()
 * \brief Lance les dés et réinitialise le tableau d'état des dés.
 * \param SGameState* gameState Etat du jeu
 * \param SGame* game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Game_LaunchDie( SGameState* gameState, SGame* game);

/**
 * \fn int Game_FirstToPlay()
 * \brief Détermine quel joueur va commencer la partie par lancement des dés.
 * \param SDisplay* display Structure d'affichage
 * \param EGameMode gameMode Mode de jeu
 * \param SGame* game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 * \param SGameState* gameState Etat du jeu
 *	\return int Retourne 1 si joueur1 commence / 2 si joueur2 / autre si quitter
 */
int Game_FirstToPlay( SDisplay* display, EGameMode gameMode, SGame* game, SGameState* gameState);

/**
 * \fn SGameState* copyGameState()
 * \brief Effectue une copie de l'état du jeu suivant le joueur à qui cette copie est destinée.
 * \param SGameState* gameState Etat du jeu
 * \param EPlayer player Joueur à qui est destinée cette copie
 *	\return SGameState* Retourne une copie de l'état du jeu
 */
SGameState* copyGameState( SGameState* gameState, EPlayer player);

/**************************** FIN SPECIFICATION MOTEUR DE JEU ******************************/

/**************************** SPECIFICATION INTERFACE GRAPHIQUE ******************************/

/**
 * \fn void Display_Init()
 * \brief Initialise la SDL, la structure d'affichage et la structure d'informations sur le jeu.
 * \param SDisplay* display Structure d'affichage
 * \param SGame* game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Display_Init( SDisplay* display, SGame* game);

/**
 * \fn void Display_Exit()
 * \brief Libère les surfaces utilisées dans la structure d'affichage et quitte la SDL.
 * \param SDisplay* display Structure d'affichage
 */
void Display_Exit( SDisplay* display);

/**
 * \fn void Display_Checkers()
 * \brief Affiche les pions à l'écran.
 * \param SDisplay* display Structure d'affichage
 * \param SGameState* gameState Etat du jeu
 * \param SGame* game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Display_Checkers( SDisplay* display, SGameState* gameState, SGame* game);

/**
 * \fn void Display_Die()
 * \brief Affiche les dés à l'écran.
 * \param SDisplay* display Structure d'affichage
 * \param SGameState* gameState Etat du jeu
 * \param SGame* game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Display_Die( SDisplay* display, SGameState* gameState, SGame* game);

/**
 * \fn void Display_Score()
 * \brief Affiche les scores à l'écran.
 * \param SDisplay* display Structure d'affichage
 * \param SGameState* gameState Etat du jeu
 * \param SGame* game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Display_Score( SDisplay* display, SGameState* gameState, SGame* game);

/**
 * \fn void Display_Clear()
 * \brief Réinitialise l'affichage.
 * \param SDisplay* display Structure d'affichage
 */
void Display_Clear( SDisplay* display);

/**
 * \fn void Display_CheckerDraw()
 * \brief Dessine un pion d'un joueur à une position donnée.
 * \param SDisplay* display Structure d'affichage
 * \param SDL_Rect position Position où l'on veut dessiner le pion
 * \param int player Joueur possèdant ce pion
 * \param SGame* game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Display_CheckerDraw( SDisplay* display, SDL_Rect position, int player, SGame* game);

/**
 * \fn void Display_CheckerMove()
 * \brief Effectue le déplacement d'un pion suivant un mouvement donné et met à jour l'état du jeu.
 * \param SDisplay* display Structure d'affichage
 * \param SGameState* gameState Etat du jeu
 * \param SMove* move Mouvement à effectuer
 * \param SGame* game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Display_CheckerMove( SDisplay* display, SGameState* gameState, SMove* move, SGame* game);

/**
 * \fn void Display_Message()
 * \brief Affiche un message à l'écran.
 * \param SDisplay* display Structure d'affichage
 * \param char* message Message à afficher
 * \param SDL_Rect position Position du message
 * \param SDL_Color color Couleur du message
 * \param int box Présence ou non d'un cadre autour du message
 * \return int Retourne 1 si quitter / 0 sinon
 */
int Display_Message( SDisplay* display, char* message, SDL_Rect position, SDL_Color color, int box, int clic);

/**
 * \fn int* Display_Arrow_Possibilities()
 * \brief Determine en fonction du pion d'un joueur les flèches qui sont possibles à atteindre.
 * \param SDisplay* display Structure d'affichage
 * \param SGameState* gameState Etat du jeu
 * \param EPlayer player Le joueur qui veut faire le déplacement
 * \param EPosition depart Le numéro de la flèche où se trouve le pion à déplacer
 * \param SGame* game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 *	\return int* Retourne un tableau contenant le numéro des flèches possibles à atteindre
 */
int* Display_Arrow_Possibilities( SDisplay* display, SGameState* gameState, EPlayer player, EPosition depart, SGame* game);

/**
 * \fn void Display_DrawSelectedArrow()
 * \brief Colorie la flèche à la position pos en bleu.
 * \param SDisplay* display Structure d'affichage
 * \param EPosition pos Le numéro de la flèche à colorer
 */
void Display_DrawSelectedArrow( SDisplay* display, EPosition pos);

/**
 * \fn int Display_CheckersPossibilities()
 * \brief Affiche à l'écran les pions qui peuvent être déplacé suivant l'état du jeu et le joueur courant.
 * \param SDisplay* display Structure d'affichage
 * \param SGameState* gameState Etat du jeu
 * \param EPlayer player Le joueur qui veut faire le déplacement
 * \param SGame* game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 *	\return int Retourne 1 si quitter / 0 sinon
 */
int Display_CheckersPossibilities( SDisplay* display, SGameState* gameState, EPlayer player, SGame* game);

/**
 * \fn int CheckerWithScreenPosition()
 * \brief Donne la position en fonction d'un clic sur le jeu dans le poiteur de EPosition pos.
 * \param int x Position en abscisse
 * \param int y Position en ordonnée
 * \param EPosition* pos Position sur laquelle on veut savoir si on a cliqué dessus
 *	\return int Retourne Vrai si position trouve, faux sinon
 */
int CheckerWithScreenPosition( int x, int y, EPosition* pos);

/**
 * \fn void colorChecker()
 * \brief Colorie le dernier pion d'une flèche donnée.
 * \param SDisplay* display Structure d'affichage
 * \param SGameState* gameState Etat du jeu
 * \param EPosition pos Position de la flèche sur lequel on veut colorier le dernier pion
 */
void colorChecker( SDisplay* display, SGameState* gameState, EPosition pos);

/**
 * \fn void Display_RefreshGameBoard()
 * \brief Raffraichit l'affichage du plateau de jeu( pions, dés et videau).
 * \param SDisplay* display Structure d'affichage
 * \param SGameState* gameState Etat du jeu
 * \param SGame* game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Display_RefreshGameBoard( SDisplay* display, SGameState* gameState, SGame* game);

/**
 * \fn int inTab()
 * \brief Détermine si la position p est dans le tableau tab.
 * \param EPosition p Position recherchée
 * \param int* tab Tableau dans lequel on cherche
 * \return int Retourne vrai si position trouvée, faux sinon
 */
int inTab( EPosition p, int* tab);

/**
 * \fn void Display_DrawOut()
 * \brief Affiche les pions présents dans les zones de sorties.
 * \param SDisplay* display Structure d'affichage
 * \param SGameState* gameState Etat du jeu
 * \param SGame* game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Display_DrawOut( SDisplay* display, SGameState* gameState, SGame* game);

/**
 * \fn void Display_DrawBar()
 * \brief Affiche les pions présents dans les barres (prisonniers).
 * \param SDisplay* display Structure d'affichage
 * \param SGameState* gameState Etat du jeu
 * \param SGame* game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Display_DrawBar( SDisplay* display, SGameState* gameState, SGame* game);

/**
 * \fn int Display_GameActions()
 * \brief Affiche à l'écran les actions de jeu possibles ( doubler la mise, lancer les dés).
 * \param SDisplay* display Structure d'affichage
 * \param SGameState* gameState Etat du jeu
 * \param SGame* game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
int Display_GameActions( SDisplay* display, SGameState* gameState, SGame* game);


/**************************** FIN SPECIFICATION INTERFACE GRAPHIQUE ******************************/


/**************************** SPECIFICATION LE MENU ******************************/

/**
 * \fn void Menu_Fill()
 * \brief Remplis le menu avec les options correspondants au mode de jeu.
 * \param SDisplay* display Structure d'affichage
 * \param EGameMode gameMode Mode de jeu
 * \param SGame* game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Menu_Fill( SDisplay* display, EGameMode gameMode, SGame* game);

/**
 * \fn void Menu_TextInput()
 * \brief Ajoute à une chaine de caractère un caractère obtenu par l'événement SDL_KEYUP.
 * \param char* name Chaine de caractère à modifier
 * \param SDL_keysym key Symbole qui a été tapé au clavier
 */
void Menu_TextInput( char* name, SDL_keysym key);

/**
 * \fn void Menu_Text()
 * \brief Affiche un texte à une certaine position d'une certaine couleur.
 * \param SDisplay* display Structure d'affichage
 * \param char* message Message à afficher
 * \param SDL_Rect position Position à laquelle on veut afficher le message
 * \param SDL_Color color Couleur dans laquelle on veut afficher le message
 */
void Menu_Text( SDisplay* display, char* message, SDL_Rect position, SDL_Color color);

/**
 * \fn int Menu_Click()
 * \brief Renvoie le code de l'élément cliqué sur le menu.
 * \param int x Abscisse de la position du clic de souris
 * \param int y Ordonnée de la position du clic de souris
 * \return int Retourne code de l'élément cliqué, -1 si ne correspond à aucun élément du menu
 */
int Menu_Click( int x, int y);

/**
 * \fn int Menu_Display()
 * \brief Affiche le menu.
 * \param SDisplay* display Structure d'affichage
 * \param EGameMode gameMode Mode de jeu
 * \param SGame* game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 * \return int Retourne 0 si jouer, 1 si quitter
 */
int Menu_Display( SDisplay* display, EGameMode gameMode, SGame* game);

/**************************** FIN SPECIFICATION MENU ******************************/

/**************************** SPECIFICATION LIBRAIRIES ******************************/

/**
 * \fn SIA_Functions* Init_SAI()
 * \brief Retourne une structure de fonctions de l'IA initialisée.
 * \return SIA_Functions* Retourn une structure de fonctions d'une IA
 */
SIA_Functions* Init_SAI();

/**
 * \fn void Free_SAI()
 * \brief Libère une structure SIA.
 * \param SIA_Functions* IA_struct Structure à libérer
 */
void Free_SAI( SIA_Functions* IA_struct);

/**
 * \fn int Load_API()
 * \brief Charge la librairie passé en paramètre et initialise la structure IA.
 * \param char* path Chemin de la librairie à charger
 * \param SIA_Functions* IA_struct Structure à initialiser avec la librairie
 * \param int ind Indice de la structure SIA
 * \return int Retourne 1 si chargement effectué, 0 si erreur de chargement
 */
int Load_API( char* path, SIA_Functions* IA_struct, int ind);

/**
 * \fn EGameMode Check_Args()
 * \brief Parse les arguments et détermine ainsi le mode de jeu et les SIA à utiliser.
 * \param int argc Nombre d'arguments
 * \param char** argv Liste des arguments
 * \param SIA_Functions* IA_struct Tableau de deux structures SIA
 * \return EGameMode Retourne le mode de jeu
 */
EGameMode Check_Args( int argc, char** argv, SIA_Functions* IA_struct);

/**************************** FIN SPECIFICATION LIBRAIRIES ******************************/

/**************************** SPECIFICATION ARBITRE ******************************/

/**
 * \fn int Arbitrator_AuthorizedDeplacement()
 * \brief Analyse la validité d'un mouvement.
 * \param SGameState* gameState Etat du jeu
 * \param SMove* move Mouvement à analyser
 * \param EPlayer player Joueur voulant effectuer ce mouvement
 * \param SGame* game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 * \return int Retourne vrai si le mouvement est autorisé, faux sinon.
 */
int Arbitrator_AuthorizedDeplacement( SGameState* gameState, SMove* move, EPlayer player, SGame* game);

/**
 * \fn int Arbitrator_PlayerArrays();
 * \brief Détermine si une flèche est possédée par un joueur donné.
 * \param SZone zone Flèche à analyser
 * \param EPlayer player Joueur supposé détenir la flèche
 * \return int Retourne vrai si la flèche est possédée par le joueur, faux sinon.
 */
int Arbitrator_PlayerArrays( SZone zone, EPlayer player);

/**
 * \fn int Arbitrator_Case_Appartenant_Joueur_Adverse_Avec_Un_Pion()
 * \brief 
 * \param SZone zone
 * \param EPlayer player
 * \return int 
 */
int Arbitrator_Case_Appartenant_Joueur_Adverse_Avec_Un_Pion( SZone zone, EPlayer player);

/**
 * \fn int Arbitrator_EmptyPosition()
 * \brief Détermine si une flèche est vide (sans pions).
 * \param SZone zone Flèche à analyser
 * \return int Retourne vrai si la flèche est vide, faux sinon.
 */
int Arbitrator_EmptyPosition( SZone zone);

/**
 * \fn unsigned int get_distance()
 * \brief Détermine la distance entre une position de départ et une d'arrivée.
 * \param EPosition depart Position de départ
 * \param EPosition arrivee Position d'arrivee
 * \return int Retourne la distance entre ces deux positions
 */
unsigned int Arbitrator_Get_Distance(EPosition depart, EPosition arrivee);

/**
 * \fn int Arbitrator_Sens_rotation_correct()
 * \brief Détermine si le sens d'un mouvement est correct.
 * \param EPlayer player Joueur faisant le mouvement
 * \param EPosition depart Position de départ
 * \param EPosition arrivee Position d'arrivee
 * \return int Retourne vrai si le sens est correct, faux sinon.S
 */
int Arbitrator_Sens_rotation_correct( EPlayer player, EPosition depart, EPosition arrivee);

/**
 * \fn int Arbitrator_Pion_Depart_Autorise()
 * \brief Détermine si le joueur donné a cliqué sur une zone contenant un pion déplaçable par ce joueur.
 * \brief int x Abscisse de la position du clic
 * \brief int y Ordonnée de la position du clic
 * \param EPlayer player Joueur voulant effectuer ce mouvement
 * \param SGame* game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 * \param EPosition depart Position de départ
 * \return int Retourne vrai si le joueur a cliqué sur une zone contenant un pion déplacable, faux sinon.
 */
int Arbitrator_Pion_Depart_Autorise( int x, int y, EPlayer player, SGameState* game, EPosition posDepart);

/**
 * \fn int Arbitrator_NumberOfDieForMove()
 * \brief Détermine le nombre de dés utilisés pour faire un mouvement.
 * \param SGameState* gameState Etat du jeu
 * \param EPlayer player Joueur voulant effectuer ce mouvement
 * \param SMove move Mouvement à analyser
 * \return int Retourne le nombre de dés utilisés
 */
int Arbitrator_NumberOfDieForMove( SGameState* game, EPlayer player, SMove move);


/*Retourne 1 si le mouvement correspond à un des dés encore jouable, 0 sinon*/
/**
 * \fn int Arbitrator_Taille_Mouvement_Correcte()
 * \brief Détermine si le mouvement correspond à un des dés encore jouable.
 * \param unsigned int taille_mouvement Taille du mouvement (Distance)
 * \param SGame* game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 * \param SGameState* gameState Etat du jeu
 * \return int Retourne vrai si correspondance, faux sinon.
 */
int Arbitrator_Taille_Mouvement_Correcte( unsigned int taille_mouvement, SGame* game, SGameState* gameState);

/*Retourne 1 si le joueur peut encore jouer dans le tour, 0 sinon*/
/**
 * \fn int Player_Can_Play()
 * \brief Détermine si le joueur peut encore jouer dans le tour.
 * \param SGameState* gameState Etat du jeu
 * \param SGame* game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 * \param EPlayer player Joueur à analyser
 * \return int Retourne vrai si un mouvement est encore possible, faux sinon.
 */
int Arbitrator_Player_Can_Play(SGameState *gameState, SGame *game, EPlayer player);




int Arbitrator_Zone_Accessible(SGameState *gameState, EPlayer player, EPosition pos);

int Arbitrator_Nb_Pion_Prison(SGameState* game, EPlayer player);
int Arbitrator_Jeu_Out_Possible(SGameState* game, EPlayer player);
/**************************** FIN SPECIFICATION ARBITRE ******************************/


#endif


