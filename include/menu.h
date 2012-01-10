#include "backgammon.h"
#include "gui.h"
#include <SDL/SDL.h>

#ifndef MENU_H
	#define MENU_H

/* Remplis le menu avec les options correspondants au mode de jeu */
void Menu_Fill(SDisplay *display,E_GameMode gameMode);

/* Ajoute à une chaine de caractère un caractère obtenu par l'événement SDL_KEYUP */
void TextInput(char* name, SDL_keysym key);

/* Affiche un texte à une certaine position d'une certaine couleur */
void Menu_Text(SDisplay *display, char	*message, SDL_Rect position,SDL_Color color);

/* Renvoie le code correspond à un élément clickable sur le menu sinon -1 si clic ailleur */
int Menu_Click(int x, int y);

/* Affiche le menu et retourne 0 si jouer ou 1 si quitter */
int Display_Menu(SDisplay *display, E_GameMode gameMode);

#endif
