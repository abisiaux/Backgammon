/**
 * \file definitions.h
 * \brief Contient les signatures des fonctions et les différentes structures.
 * \author Alexandre BISIAUX et Antonin BIRET
 * \date 26 janvier 2012
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
typedef struct
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
typedef struct
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
 *	\return Structure d'état du jeu
 */
SGameState* Game_Init();

/**
 * \fn int Game_Play(SDisplay* display, EGameMode gameMode, SGame* game, SIA_Functions* IA_struct)
 * \brief Lance la boucle de jeu.
 * \param display Structure d'affichage
 * \param gameMode Mode de jeu
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 * \param IA_struct Tableau de structures contenant les fonctions de l'AI
 *	\return Retourne 1 si quitter / 0 si bon déroulement
 */
int Game_Play(SDisplay* display, EGameMode gameMode, SGame* game, SIA_Functions* IA_struct);

/**
 * \fn void Game_LaunchDie(SGameState* gameState, SGame* game)
 * \brief Lance les dés et réinitialise le tableau d'état des dés.
 * \param gameState Etat du jeu
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Game_LaunchDie(SGameState* gameState, SGame* game);

/**
 * \fn int Game_FirstToPlay(SDisplay* display, EGameMode gameMode, SGame* game, SGameState* gameState)
 * \brief Détermine quel joueur va commencer la partie par lancement des dés.
 * \param display Structure d'affichage
 * \param gameMode Mode de jeu
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 * \param gameState Etat du jeu
 *	\return Retourne 1 si joueur1 commence / 2 si joueur2 / autre si quitter
 */
int Game_FirstToPlay(SDisplay* display, EGameMode gameMode, SGame* game, SGameState* gameState);

/**
 * \fn SGameState* copyGameState(SGameState* gameState, EPlayer player)
 * \brief Effectue une copie de l'état du jeu suivant le joueur à qui cette copie est destinée.
 * \param gameState Etat du jeu
 * \param player Joueur à qui est destinée cette copie
 *	\return Retourne une copie de l'état du jeu
 */
SGameState* copyGameState(SGameState* gameState, EPlayer player);

/**
 * \fn int Game_AcceptDouble(SDisplay* display, EPlayer player, SGame* game)
 * \brief Demande d'acceptation du double.
 * \param display Structure d'affichage
 * \param player Joueur à qui on demande
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 *	\return Retourne 1 si acceptation, 0 si refus, -1 si quitter
 */
int Game_AcceptDouble(SDisplay* display, EPlayer player, SGame* game);

/**************************** FIN SPECIFICATION MOTEUR DE JEU ******************************/

/**************************** SPECIFICATION INTERFACE GRAPHIQUE ******************************/

/**
 * \fn void Display_Init(SDisplay* display, SGame* game)
 * \brief Initialise la SDL, la structure d'affichage et la structure d'informations sur le jeu.
 * \param display Structure d'affichage
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Display_Init(SDisplay* display, SGame* game);

/**
 * \fn void Display_Exit(SDisplay* display)
 * \brief Libère les surfaces utilisées dans la structure d'affichage et quitte la SDL.
 * \param display Structure d'affichage
 */
void Display_Exit(SDisplay* display);

/**
 * \fn void Display_Checkers(SDisplay* display, SGameState* gameState, SGame* game)
 * \brief Affiche les pions à l'écran.
 * \param display Structure d'affichage
 * \param gameState Etat du jeu
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Display_Checkers(SDisplay* display, SGameState* gameState, SGame* game);

/**
 * \fn void Display_Die(SDisplay* display, SGameState* gameState, SGame* game)
 * \brief Affiche les dés à l'écran.
 * \param display Structure d'affichage
 * \param gameState Etat du jeu
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Display_Die(SDisplay* display, SGameState* gameState, SGame* game);

/**
 * \fn void Display_Score(SDisplay* display, SGameState* gameState, SGame* game);
 * \brief Affiche les scores à l'écran.
 * \param display Structure d'affichage
 * \param gameState Etat du jeu
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Display_Score(SDisplay* display, SGameState* gameState, SGame* game);

/**
 * \fn void Display_Clear(SDisplay* display)
 * \brief Réinitialise l'affichage.
 * \param display Structure d'affichage
 */
void Display_Clear(SDisplay* display);

/**
 * \fn void Display_CheckerDraw(SDisplay* display, SDL_Rect position, int player, SGame* game)
 * \brief Dessine un pion d'un joueur à une position donnée.
 * \param display Structure d'affichage
 * \param position Position où l'on veut dessiner le pion
 * \param player Joueur possèdant ce pion
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Display_CheckerDraw(SDisplay* display, SDL_Rect position, int player, SGame* game);

/**
 * \fn void Display_CheckerMove(SDisplay* display, SGameState* gameState, SMove* move, SGame* game)
 * \brief Effectue le déplacement d'un pion suivant un mouvement donné et met à jour l'état du jeu.
 * \param display Structure d'affichage
 * \param gameState Etat du jeu
 * \param move Mouvement à effectuer
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Display_CheckerMove(SDisplay* display, SGameState* gameState, SMove* move, SGame* game);

/**
 * \fn int Display_Message(SDisplay* display, char* message, SDL_Rect position, SDL_Color color, int box, int clic)
 * \brief Affiche un message à l'écran.
 * \param display Structure d'affichage
 * \param message Message à afficher
 * \param position Position du message
 * \param color Couleur du message
 * \param box Présence ou non d'un cadre autour du message
 * \return Retourne 1 si quitter / 0 sinon
 */
int Display_Message(SDisplay* display, char* message, SDL_Rect position, SDL_Color color, int box, int clic);

/**
 * \fn int* Display_Arrow_Possibilities(SDisplay* display, SGameState* gameState, EPlayer player, EPosition depart, SGame* game)
 * \brief Determine en fonction du pion d'un joueur les flèches qui sont possibles à atteindre.
 * \param display Structure d'affichage
 * \param gameState Etat du jeu
 * \param player Le joueur qui veut faire le déplacement
 * \param depart Le numéro de la flèche où se trouve le pion à déplacer
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 *	\return Retourne un tableau contenant le numéro des flèches possibles à atteindre
 */
int* Display_Arrow_Possibilities(SDisplay* display, SGameState* gameState, EPlayer player, EPosition depart, SGame* game);

/**
 * \fn void Display_DrawSelectedArrow(SDisplay* display, EPosition pos)
 * \brief Colorie la flèche à la position pos en bleu.
 * \param display Structure d'affichage
 * \param pos Le numéro de la flèche à colorer
 */
void Display_DrawSelectedArrow(SDisplay* display, EPosition pos);

/**
 * \fn int Display_CheckersPossibilities(SDisplay* display, SGameState* gameState, EPlayer player, SGame* game)
 * \brief Affiche à l'écran les pions qui peuvent être déplacé suivant l'état du jeu et le joueur courant.
 * \param display Structure d'affichage
 * \param gameState Etat du jeu
 * \param player Le joueur qui veut faire le déplacement
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 *	\return Retourne 1 si quitter / 0 sinon
 */
int Display_CheckersPossibilities(SDisplay* display, SGameState* gameState, EPlayer player, SGame* game);

/**
 * \fn int CheckerWithScreenPosition(int x, int y, EPosition* pos)
 * \brief Donne la position en fonction d'un clic sur le jeu dans le poiteur de EPosition pos.
 * \param x Position en abscisse
 * \param y Position en ordonnée
 * \param pos Position sur laquelle on veut savoir si on a cliqué dessus
 *	\return int Retourne Vrai si position trouve, faux sinon
 */
int CheckerWithScreenPosition(int x, int y, EPosition* pos);

/**
 * \fn void colorChecker(SDisplay* display, SGameState* gameState, EPosition pos)
 * \brief Colorie le dernier pion d'une flèche donnée.
 * \param display Structure d'affichage
 * \param gameState Etat du jeu
 * \param pos Position de la flèche sur lequel on veut colorier le dernier pion
 */
void colorChecker(SDisplay* display, SGameState* gameState, EPosition pos);

/**
 * \fn void Display_RefreshGameBoard(SDisplay* display, SGameState* gameState, SGame* game)
 * \brief Raffraichit l'affichage du plateau de jeu(pions, dés et videau).
 * \param display Structure d'affichage
 * \param gameState Etat du jeu
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Display_RefreshGameBoard(SDisplay* display, SGameState* gameState, SGame* game);

/**
 * \fn int inTab(EPosition p, int* tab)
 * \brief Détermine si la position p est dans le tableau tab.
 * \param p Position recherchée
 * \param tab Tableau dans lequel on cherche
 * \return Retourne vrai si position trouvée, faux sinon
 */
int inTab(EPosition p, int* tab);

/**
 * \fn void Display_DrawOut(SDisplay* display, SGameState* gameState, SGame* game)
 * \brief Affiche les pions présents dans les zones de sorties.
 * \param display Structure d'affichage
 * \param gameState Etat du jeu
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Display_DrawOut(SDisplay* display, SGameState* gameState, SGame* game);

/**
 * \fn void Display_DrawBar(SDisplay* display, SGameState* gameState, SGame* game)
 * \brief Affiche les pions présents dans les barres (prisonniers).
 * \param display Structure d'affichage
 * \param gameState Etat du jeu
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Display_DrawBar(SDisplay* display, SGameState* gameState, SGame* game);

/**
 * \fn int Display_GameActions(SDisplay *display, SGameState* gameState, SGame *game, EPlayer player)
 * \brief Affiche à l'écran les actions de jeu possibles (doubler la mise, lancer les dés).
 * \param display Structure d'affichage
 * \param gameState Etat du jeu
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 * \param player Joueur courant
 * \return Retourne 1 si quitter, 0 sinon
 */
int Display_GameActions(SDisplay *display, SGameState* gameState, SGame *game, EPlayer player);

/**
 * \fn void Display_Double(SDisplay* display, SGame* game)
 * \brief Affiche le videau
 * \param display Structure d'affichage
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Display_Double(SDisplay* display, SGame* game);



void onClickChecker(int x, int y, int tab[28], EPlayer player, SGameState* gameState, SGame* game, SDisplay* display);
/**************************** FIN SPECIFICATION INTERFACE GRAPHIQUE ******************************/


/**************************** SPECIFICATION LE MENU ******************************/

/**
 * \fn void Menu_Fill(SDisplay* display, EGameMode gameMode, SGame* game);
 * \brief Remplis le menu avec les options correspondants au mode de jeu.
 * \param display Structure d'affichage
 * \param gameMode Mode de jeu
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 */
void Menu_Fill(SDisplay* display, EGameMode gameMode, SGame* game);

/**
 * \fn void Menu_TextInput(char* name, SDL_keysym key)
 * \brief Ajoute à une chaine de caractère un caractère obtenu par l'événement SDL_KEYUP.
 * \param name Chaine de caractère à modifier
 * \param key Symbole qui a été tapé au clavier
 */
void Menu_TextInput(char* name, SDL_keysym key);

/**
 * \fn void Menu_Text(SDisplay* display, char* message, SDL_Rect position, SDL_Color color)
 * \brief Affiche un texte à une certaine position d'une certaine couleur.
 * \param display Structure d'affichage
 * \param message Message à afficher
 * \param position Position à laquelle on veut afficher le message
 * \param color Couleur dans laquelle on veut afficher le message
 */
void Menu_Text(SDisplay* display, char* message, SDL_Rect position, SDL_Color color);

/**
 * \fn int Menu_Click(int x, int y)
 * \brief Renvoie le code de l'élément cliqué sur le menu.
 * \param x Abscisse de la position du clic de souris
 * \param y Ordonnée de la position du clic de souris
 * \return Retourne code de l'élément cliqué, -1 si ne correspond à aucun élément du menu
 */
int Menu_Click(int x, int y);

/**
 * \fn int Menu_Display(SDisplay* display, EGameMode gameMode, SGame* game)
 * \brief Affiche le menu.
 * \param display Structure d'affichage
 * \param gameMode Mode de jeu
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 * \return Retourne 0 si jouer, 1 si quitter
 */
int Menu_Display(SDisplay* display, EGameMode gameMode, SGame* game);

/**************************** FIN SPECIFICATION MENU ******************************/

/**************************** SPECIFICATION LIBRAIRIES ******************************/

/**
 * \fn SIA_Functions* Init_SAI()
 * \brief Retourne une structure de fonctions de l'IA initialisée.
 * \return Retourne une structure de fonctions d'une IA
 */
SIA_Functions* Init_SAI();

/**
 * \fn void Free_SAI(SIA_Functions* IA_struct)
 * \brief Libère une structure SIA.
 * \param IA_struct Structure à libérer
 */
void Free_SAI(SIA_Functions* IA_struct);

/**
 * \fn int Load_API(char* path, SIA_Functions* IA_struct, int ind)
 * \brief Charge la librairie passé en paramètre et initialise la structure IA.
 * \param path Chemin de la librairie à charger
 * \param IA_struct Structure à initialiser avec la librairie
 * \param ind Indice de la structure SIA
 * \return Retourne 1 si chargement effectué, 0 si erreur de chargement
 */
int Load_API(char* path, SIA_Functions* IA_struct, int ind);

/**
 * \fn EGameMode Check_Args(int argc, char** argv, SIA_Functions* IA_struct)
 * \brief Parse les arguments et détermine ainsi le mode de jeu et les SIA à utiliser.
 * \param argc Nombre d'arguments
 * \param argv Liste des arguments
 * \param IA_struct Tableau de deux structures SIA
 * \return Retourne le mode de jeu
 */
EGameMode Check_Args(int argc, char** argv, SIA_Functions* IA_struct);

/**************************** FIN SPECIFICATION LIBRAIRIES ******************************/

/**************************** SPECIFICATION ARBITRE ******************************/

/**
 * \fn int Arbitrator_AuthorizedDeplacement(SGameState* gameState, SMove* move, EPlayer player, SGame* game)
 * \brief Analyse la validité d'un mouvement.
 * \param gameState Etat du jeu
 * \param move Mouvement à analyser
 * \param player Joueur voulant effectuer ce mouvement
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 * \return Retourne vrai si le mouvement est autorisé, faux sinon.
 */
int Arbitrator_AuthorizedDeplacement(SGameState* gameState, SMove* move, EPlayer player, SGame* game);

/**
 * \fn int Arbitrator_PlayerArrays(SZone zone, EPlayer player)
 * \brief Détermine si une flèche est possédée par un joueur donné.
 * \param zone Flèche à analyser
 * \param player Joueur supposé détenir la flèche
 * \return Retourne vrai si la flèche est possédée par le joueur, faux sinon.
 */
int Arbitrator_PlayerArrays(SZone zone, EPlayer player);

/**
 * \fn int Arbitrator_Case_Appartenant_Joueur_Adverse_Avec_Un_Pion( SZone zone, EPlayer player)
 * \brief 
 * \param zone
 * \param player
 * \return
 */
int Arbitrator_Case_Appartenant_Joueur_Adverse_Avec_Un_Pion( SZone zone, EPlayer player);

/**
 * \fn int Arbitrator_EmptyPosition(SZone zone)
 * \brief Détermine si une flèche est vide (sans pions).
 * \param zone Flèche à analyser
 * \return Retourne vrai si la flèche est vide, faux sinon.
 */
int Arbitrator_EmptyPosition(SZone zone);

/**
 * \fn unsigned int get_distance(EPosition depart, EPosition arrivee)
 * \brief Détermine la distance entre une position de départ et une d'arrivée.
 * \param depart Position de départ
 * \param arrivee Position d'arrivee
 * \return Retourne la distance entre ces deux positions
 */
unsigned int Arbitrator_Get_Distance(EPosition depart, EPosition arrivee);


/**
 * \fn int Arbitrator_Sens_rotation_correct(EPlayer player, EPosition depart, EPosition arrivee)
 * \brief Détermine si le sens d'un mouvement est correct.
 * \param player Joueur faisant le mouvement
 * \param depart Position de départ
 * \param arrivee Position d'arrivee
 * \return Retourne vrai si le sens est correct, faux sinon.S
 */
int Arbitrator_Sens_rotation_correct(EPlayer player, EPosition depart, EPosition arrivee);

/**
 * \fn int Arbitrator_Pion_Depart_Autorise(int x, int y, EPlayer player, SGameState* game, EPosition posDepart)
 * \brief Détermine si le joueur donné a cliqué sur une zone contenant un pion déplaçable par ce joueur.
 * \brief x Abscisse de la position du clic
 * \brief y Ordonnée de la position du clic
 * \param player Joueur voulant effectuer ce mouvement
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 * \param depart Position de départ
 * \return Retourne vrai si le joueur a cliqué sur une zone contenant un pion déplacable, faux sinon.
 */
int Arbitrator_Pion_Depart_Autorise(int x, int y, EPlayer player, SGameState* game, EPosition posDepart);


/**
 * \fn int Arbitrator_NumberOfDieForMove(SGameState* game, EPlayer player, SMove move)
 * \brief Détermine le nombre de dés utilisés pour faire un mouvement.
 * \param gameState Etat du jeu
 * \param player Joueur voulant effectuer ce mouvement
 * \param move Mouvement à analyser
 * \return Retourne le nombre de dés utilisés
 */
int Arbitrator_NumberOfDieForMove(SGameState* game, EPlayer player, SMove move);

/**
 * \fn int Arbitrator_NumberOfDieCanPlay(SGameState* gameState, EPlayer player)
 * \brief Détermine le nombre de dés qui peuvent être joués dans la partie (utile si le joueur est bloqué par exemple).
 * \param gameState Etat du jeu
 * \param player Joueur à analyser
 * \return Retourne le nombre de dés pouvant être joués
 */
int Arbitrator_NumberOfDieCanPlay(SGameState* gameState, EPlayer player); // A IMPLEMENTER


/**
 * \fn int Arbitrator_Taille_Mouvement_Correcte(unsigned int taille_mouvement, SGame* game, SGameState* gameState)
 * \brief Détermine si le mouvement correspond à un des dés encore jouable.
 * \param taille_mouvement Taille du mouvement (Distance)
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 * \param gameState Etat du jeu
 * \return Retourne vrai si correspondance, faux sinon.
 */
int Arbitrator_Taille_Mouvement_Correcte(unsigned int taille_mouvement, SGame* game, SGameState* gameState);

/*Retourne 1 si le joueur peut encore jouer dans le tour, 0 sinon*/
/**
 * \fn int Arbitrator_Player_Can_Play(SGameState *gameState, SGame *game, EPlayer player)
 * \brief Détermine si le joueur peut encore jouer dans le tour.
 * \param gameState Etat du jeu
 * \param game Informations sur le jeu (noms des joueurs, couleurs des pions des joueurs,...)
 * \param player Joueur à analyser
 * \return Retourne vrai si un mouvement est encore possible, faux sinon.
 */
int Arbitrator_Player_Can_Play(SGameState *gameState, SGame *game, EPlayer player);

/*Retourne 1 si la zone est accessible au player*/
/**
 * \fn int Arbitrator_Zone_Accessible(SGameState *gameState, EPlayer player, EPosition pos)
 * \brief Détermine si la case est accessible pour un joueur donné.
 * \param gameState Etat du jeu
 * \param player Joueur à analyser
 * \param pos position de destination
 * \return Retourne vrai si case accessible, faux sinon.
 */
int Arbitrator_Zone_Accessible(SGameState *gameState, EPlayer player, EPosition pos);

/*Retourne le nombre de pions prisonniers pour un joueur donné*/
/**
 * \fn int Arbitrator_Nb_Pion_Prison(SGameState* game, EPlayer player)
 * \brief Retourne le nombre de pion prisonniers pour un joueur donné.
 * \param gameState Etat du jeu
 * \param player Joueur à analyser
 * \return Nombre de pions prisonniers.
 */
int Arbitrator_Nb_Pion_Prison(SGameState* game, EPlayer player);

/*Retourne 1 si le joueur peut deplacer des pions dans le OUT*/
/**
 * \fn int Arbitrator_Jeu_Out_Possible(SGameState* game, EPlayer player)
 * \brief Détermine si un joueur peut envoyer des pions dans la zone OUT.
 * \param gameState Etat du jeu
 * \param player Joueur à analyser
 * \return Retourne vrai si le joueur peut jouer en OUT, faux sinon.
 */
int Arbitrator_Jeu_Out_Possible(SGameState* game, EPlayer player);
/**************************** FIN SPECIFICATION ARBITRE ******************************/

/********NOUVELLES FONCTIONS A COMMENTER*/
void updateDieStatue(SMove* mouvement, SGameState* gameState, SGame* game);

void convertMove(SMove* move,EPlayer player);

#define VITESSE 5
#endif


