#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../include/arbitrator.h"
#include "../include/backgammon.h"

int authorized_deplacement(SGameState* game, SMove *move, EPlayer player)
{
	//1- position de départ lui appartient bien ?
	//2- mouvement(nombre de case à parcourir) correspond au nombre indiqué par un des dés ?
	//4- Sens du mouvement autorisé? 
	//3- position d'arrivée est vide ou lui appartenant ou avec un seul pion adverse ?

 	SZone zoneDepart = (game->zones)[(move->src_point)-1]; // zone de départ
	SZone zoneArrivee = (game->zones)[(move->dest_point)-1]; // zone de départ
	EPosition posDepart = (move->src_point)-1;
	EPosition posArrivee = (move->dest_point)-1;
	//Position départ = position appartenant au joueur ?
	if(case_appartenant_au_joueur(zoneDepart,player))
	{
		//printf("src : %u, indice :%u\n",move->src_point,posDepart);
		//printf("destS : %u, indice :%u\n",move->dest_point,posArrivee);
		unsigned int nb_sauts = get_distance(posArrivee,posDepart);
		//printf("DISTANCE : %u\n",nb_sauts);
		//printf("DE1 : %u, DE2: %u\n",game->die1,game->die2);
		// Si le mouvement correspont au nombre d'un des dés
		if(nb_sauts == game->die1 || nb_sauts == game->die2)
		{
			//printf("RES DE = NB SAUTS\n");
			//on regarde si le sens de rotation est correct pour le joueur			
			if(sens_rotation_correct(player, posDepart, posArrivee))
			{
				// on regarde si la position d'arrivée est vide, ou lui appartient, ou appartient à l'adversaire avec un seul et unique pion
				if(case_appartenant_au_joueur(zoneArrivee, player) || case_appartenant_joueur_adverse_avec_un_pion(zoneArrivee, player) || position_vide(zoneArrivee) )
				{
					return 1;
				}	
				//else printf("CASE ARRIVEE PAS VIDE OU APPARTENANT AU JOUEUR ADVERSE AVEC + d'1 PION\n");
			}
			//else printf("SENS NON AUTORISE\n");
		}
		//else printf("NB SAUTS CORRESPOND PAS AU NOMBRE DU DE\n");
	}
	//else printf("CASE DEPART APPARTENANT JOUEUR ADVERSE\n");

	return 0;	
	
}

int case_appartenant_au_joueur(SZone zone, EPlayer player)
{
	
	if(zone.player == player)
	{
		//printf("POS APPARTIENT AU JOUEUR\n");
	 	return 1;
	}	
	return 0;
}
int case_appartenant_joueur_adverse_avec_un_pion(SZone zone, EPlayer player)
{
	if((zone.player != player) && (zone.nb_checkers == 1))
	{
		//printf("CASE JOUEUR ADVERSE AVEC 1 SEUL PION\n");
		return 1;
	}
	return 0;
}
int position_vide(SZone zone)
{
	if(zone.nb_checkers == 0)
	{
		//printf("POS VIDE\n");
		return 1;
	}
	//else printf("POS NON VIDE\n");
	return 0;
}
unsigned int get_distance(EPosition pos1, EPosition pos2)
{
	if(((int)(pos1) - (int)(pos2)) < 0)
		return (unsigned int)( -1*((int)(pos1) - (int)(pos2)));
	else return ((pos1) - (pos2));
}
int sens_rotation_correct(EPlayer joueur, EPosition depart, EPosition arrivee)
{
	//Joueur1(vert) rotation horaire
	//Joueur2(blanc) rotation trigo 
	switch (joueur) 
	{

		case EPlayer1 :
			if(((int)(arrivee) - (int)(depart)) > 0)// SENS CORRECT
			{
				return 1;
			}
			return 0;

		break;

		case EPlayer2 :
			if(((int)(depart) - (int)(arrivee)) > 0)// SENS CORRECT
			{
				return 1;
			}
			return 0;

		break;
	}
	return 0;
}

int Pion_Depart_Autorise(int x, int y, EPlayer player, SGameState* game, EPosition posDepart)
{
	//On cherche la position correspondant aux coordonnées de la souris au moment du clic sur le pion
	if(CheckerWithScreenPosition(x, y, &posDepart))
	{
		
		SZone zoneCliquee = game->zones[posDepart];
		if( (zoneCliquee.player == player) && (zoneCliquee.nb_checkers >= 1) )
		{
			return 1;
		}		
	}
	return 0;	
}

int numberofDieForMove(SGameState* game, EPlayer player, SMove move)
{
	unsigned int saut=0;
	if(player == EPlayer1)
			saut = move.dest_point - move.src_point;
		else
			saut = move.src_point - move.dest_point;
			
	if(game->die1 != game->die2)
	{
		if(saut == game->die1 || saut == game->die2)
			return 1;
		else if(saut == (game->die1) + (game->die2))
			return 2;
		else return -1; // Normalement impossible  
		
	}
	else
	{
		return (int)(saut/(game->die1));
	}
}

int numberOfDieCanPlay(SGameState* game, EPlayer player)
{
	return 2;
}





