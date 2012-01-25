#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../include/definitions.h"
#include "../include/backgammon.h"

int authorized_deplacement(SGameState* gameState, SMove *move, EPlayer player, SGame *game)
{
	;
	//1- position de départ lui appartient bien ?
	//2- mouvement(nombre de case à parcourir) correspond au nombre indiqué par un des dés ?
	//4- Sens du mouvement autorisé? 
	//3- position d'arrivée est vide ou lui appartenant ou avec un seul pion adverse ?
	EPosition posDepart = (move->src_point)-1;
	printf("SRC : %d\n",posDepart);
	EPosition posArrivee = (move->dest_point)-1;
	printf("DEST : %d\n",posArrivee);
 	SZone zoneDepart = (gameState->zones)[posDepart]; // zone de départ
 	printf("PLAYER : %d\n",(gameState->zones)[posDepart].player);
 	/*for(int i=0; i<28; i++)
 		printf("i=%d -> %d -> %d\n",i,(game->zones)[i].player,(game->zones)[i].nb_checkers);*/
	SZone zoneArrivee = (gameState->zones)[posArrivee]; // zone d arrivee
	// si le joueur ne joue pas le pion en prison, alors qu'il y en a un, le coup est incorrect
	if( (nb_Pion_prison(gameState,player) > 0) && ( ((posDepart != EPos_BarP1) && (player == EPlayer1)) || ((posDepart != EPos_BarP2) && (player == EPlayer2)) ) )
	{
		printf("PION EN PRISON !! %d\n",nb_Pion_prison(gameState,player));
		return 0;
	}
	printf("anto\n");
	//Position départ = position appartenant au joueur ?
	if(case_appartenant_au_joueur(zoneDepart,player))
	{
		printf("ok : %d\n",posDepart);
		unsigned int nb_sauts = get_distance(posDepart,posArrivee, player);
		printf("DISTANCE : %u\n",nb_sauts);
		printf("DE1 : %u, DE2: %u\n",gameState->die1,gameState->die2);
		// Si le mouvement correspont au nombre d'un des dés
		if(Taille_Mouvement_Correcte(nb_sauts, game, gameState ))
		{
			//printf("RES DE = NB SAUTS\n");
			//on regarde si le sens de rotation est correct pour le joueur			
			if(sens_rotation_correct(player, posDepart, posArrivee))
			{
				// on regarde si la position d'arrivée est vide, ou lui appartient, ou appartient à l'adversaire avec un seul et unique pion
				if(case_appartenant_au_joueur(zoneArrivee, player) || case_appartenant_joueur_adverse_avec_un_pion(zoneArrivee, player) || position_vide(zoneArrivee))
				{
					//si le joueur joue en out
					if( (	(posArrivee == EPos_OutP1 && player == EPlayer1) || (posArrivee == EPos_OutP2 && player == EPlayer2)	) 
						&& (!jeu_out_posible(gameState, player)) )
					{
						return 0;
					}
					return 1;
				}	
				//else printf("CASE ARRIVEE PAS VIDE OU APPARTENANT AU JOUEUR ADVERSE AVEC + d'1 PION\n");
			}
			//else printf("SENS NON AUTORISE\n");
		}
		//else printf("NB SAUTS CORRESPOND PAS AU NOMBRE DU DE\n");
	}


	else printf("CASE DEPART APPARTENANT JOUEUR ADVERSE player : %d\n",zoneDepart.player);

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
unsigned int get_distance(EPosition depart, EPosition arrivee, EPlayer player)
{
	//depart est une case prison
	if((depart == EPos_BarP1) && (player == EPlayer1))
	{
		depart = EPos_1;
	}
	else if((depart == EPos_BarP2) && (player == EPlayer2))
	{
		depart = EPos_24;
	}
	printf("GETDIST:%d // %d",depart,arrivee);
	

	if(((int)(depart) - (int)(arrivee)) < 0)
		return (unsigned int)( -1*((int)(depart) - (int)(arrivee)));
	else return ((depart) - (arrivee));
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



/*int numberofDieForMove(SGameState* game, EPlayer player, SMove move)
{
	unsigned int saut=0;
	if(player == EPlayer2)
		saut = move.dest_point - move.src_point;
	else
		saut = move.src_point - move.dest_point;
	printf("SAUT:%d\n",saut);
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
}*/




int nb_Pion_prison(SGameState* game, EPlayer player)
{
	if(player == EPlayer1)
	{
		
<<<<<<< HEAD
		return (game->zones[EPos_BarP1]).nb_checkers;
=======
		return (game->zones[EPos_BarP1]).nb_checkers; /// ATTENTION CHANGER EN BAR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
>>>>>>> beb3e89875315f90ac5634d7b695f7e010d7449e
	}
	else
	{
		return (game->zones[EPos_BarP2]).nb_checkers;
	}

}

int jeu_out_posible(SGameState* game, EPlayer player)
{
	unsigned int cpt=0;
	EPosition temp = EPos_nopos;
	if(player == EPlayer1) // carré final ( 19/24 )
	{
		 temp = EPos_19;
		
		while( temp <= EPos_24 )
		{
			if( ((game->zones[temp]).player == player) )
				cpt += (game->zones[temp]).nb_checkers;
			temp++;
		}
		cpt += (game->zones[EPos_OutP1]).nb_checkers;
		if(cpt == 15) return 1;
		
	}
	else // carré final ( 1/6 )
	{
		temp = EPos_1;
		
		while( temp <= EPos_6 )
		{
			if( ((game->zones[temp]).player == player) )
				cpt += (game->zones[temp]).nb_checkers;
			temp++;
		}
		cpt += (game->zones[EPos_OutP2]).nb_checkers;
		if(cpt == 15) return 1;
	}
	return 0;

}

int Taille_Mouvement_Correcte(unsigned int taille_mouvement, SGame *game, SGameState *gameState)
{
	printf("FCT TAILLE MOVE\n");

	for(int i=0; i<4; i++)
	{
		if(game->die_To_Play[i] == 1) // si le dé n'est pas encore joué
		{
			switch(i)
			{
				case 0: //Dé 1 non joué  
					if(taille_mouvement == gameState->die1) // taille du mouvement correspond au dé 1
					{
						return 1;	
					}
					break;
				case 1: // Dé 2 non joué
					if(taille_mouvement == gameState->die2) // taille du mouvement correspond au dé 2
					{
						return 1;	
					}
					break;
				default:
					if(taille_mouvement == gameState->die1) // taille du mouvement correspond au dé 3 ou 4
					{
						return 1;	
					}
					break;	
			}
		}
	}
	return 0;
}


