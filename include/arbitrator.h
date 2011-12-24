#include "backgammon.h"

#ifndef ARBITRATOR_H
	#define ARBITRATOR_H
int authorized_deplacement(SGameState* game, SMove *move, EPlayer player);
int case_appartenant_au_joueur(SZone zone, EPlayer player);
int case_appartenant_joueur_adverse_avec_un_pion(SZone zone, EPlayer player);
int position_vide(SZone zone);
unsigned int get_distance(EPosition pos1, EPosition pos2);
int sens_rotation_correct(EPlayer joueur, EPosition depart, EPosition arrivee);
#endif
