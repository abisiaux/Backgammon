#include "backgammon.h"

#ifndef ARBITRATOR_H
	#define ARBITRATOR_H
int authorized_deplacement(SGameState* game, SMove *move, EPlayer player);
int case_appartenant_au_joueur(SZone zone, EPlayer player);
int case_appartenant_joueur_adverse_avec_un_pion(SZone zone, EPlayer player);
int position_vide(SZone zone);
unsigned int get_distance(EPosition pos1, EPosition pos2);
int sens_rotation_correct(EPlayer joueur, EPosition depart, EPosition arrivee);

/*retourne vrai si le joueur a cliqué sur une zone contenant un pion déplacable*/
int Pion_Depart_Autorise(int x, int y, EPlayer player, SGameState* game,EPosition posDepart);
/*retourne le nombre de dés utilisés pour le mouvement*/
int numberOfDieForMove(SGameState* game, EPlayer player, SMove move);
/*retourne le nombre de dés qui peuvent être joués dans la partie ( utile si le joueur est bloqué par exemple)*/
int numberOfDieCanPlay(SGameState* game, EPlayer player); // A IMPLEMENTER
#endif
