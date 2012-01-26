/**
 * \file arbitrator.c
 * \brief Contient les fonctions de l'arbitre.
 * \author Alexandre BISIAUX et Antonin BIRET
 * \date 26 janvier 2012
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../include/definitions.h"
#include "../include/backgammon.h"

int Arbitrator_AuthorizedDeplacement(SGameState* gameState, SMove *move, EPlayer player, SGame *game)
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
	if( (Arbitrator_Nb_Pion_Prison(gameState,player) > 0) && ( ((posDepart != EPos_BarP1) && (player == EPlayer1)) || ((posDepart != EPos_BarP2) && (player == EPlayer2)) ) )
	{
		printf("PION EN PRISON !! %d\n",Arbitrator_Nb_Pion_Prison(gameState,player));
		return 0;
	}
	
	//Position départ = position appartenant au joueur ?
	if(Arbitrator_PlayerArrays(zoneDepart,player))
	{
		
		unsigned int nb_sauts = Arbitrator_Get_Distance(posDepart,posArrivee);
		printf("DISTANCE : %u\n",nb_sauts);
		// Si le mouvement correspont au nombre d'un des dés
		if(Arbitrator_Taille_Mouvement_Correcte(nb_sauts, game, gameState ))
		{

			//on regarde si le sens de rotation est correct pour le joueur			
			if(Arbitrator_Sens_rotation_correct(player, posDepart, posArrivee))
			{
				// on regarde si la position d'arrivée est vide, ou lui appartient, ou appartient à l'adversaire avec un seul et unique pion
				if(Arbitrator_PlayerArrays(zoneArrivee, player) || Arbitrator_Case_Appartenant_Joueur_Adverse_Avec_Un_Pion(zoneArrivee, player) || Arbitrator_EmptyPosition(zoneArrivee))
				{
					//si le joueur joue en out
					if( (	(posArrivee == EPos_OutP1 && player == EPlayer1) || (posArrivee == EPos_OutP2 && player == EPlayer2)	) 
						&& (!Arbitrator_Jeu_Out_Possible(gameState, player)) )
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

int Arbitrator_PlayerArrays(SZone zone, EPlayer player)
{
	
	if(zone.player == player)
	{
		//printf("POS APPARTIENT AU JOUEUR\n");
	 	return 1;
	}	
	return 0;
}
int Arbitrator_Case_Appartenant_Joueur_Adverse_Avec_Un_Pion(SZone zone, EPlayer player)
{
	if((zone.player != player) && (zone.nb_checkers == 1))
	{
		//printf("CASE JOUEUR ADVERSE AVEC 1 SEUL PION\n");
		return 1;
	}
	return 0;
}
int Arbitrator_EmptyPosition(SZone zone)
{
	if(zone.nb_checkers == 0)
	{
		//printf("POS VIDE\n");
		return 1;
	}
	//else printf("POS NON VIDE\n");
	return 0;
}
unsigned int Arbitrator_Get_Distance(EPosition depart, EPosition arrivee)
{
	//depart est une case prison
	if(depart == EPos_BarP1)
	{
		depart = EPos_1;
	}
	else if(depart == EPos_BarP2)
	{
		depart = EPos_24;
	}

	

	if(((int)(depart) - (int)(arrivee)) < 0)
		return (unsigned int)( -1*((int)(depart) - (int)(arrivee)));
	else return ((depart) - (arrivee));
}
int Arbitrator_Sens_rotation_correct(EPlayer joueur, EPosition depart, EPosition arrivee)
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

int Arbitrator_Pion_Depart_Autorise(int x, int y, EPlayer player, SGameState* game, EPosition posDepart)
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


int Arbitrator_Nb_Pion_Prison(SGameState* game, EPlayer player)
{
	if(player == EPlayer1)
	{
		return (game->zones[EPos_BarP1]).nb_checkers;
	}
	else
	{
		return (game->zones[EPos_BarP2]).nb_checkers;
	}

}

int Arbitrator_Jeu_Out_Possible(SGameState* game, EPlayer player)
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

int Arbitrator_Taille_Mouvement_Correcte(unsigned int taille_mouvement, SGame *game, SGameState *gameState)
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

int Arbitrator_Player_Can_Play(SGameState *gameState, SGame *game, EPlayer player)
{
	int Nb_Des_Encore_Jouables = 0;
	for(int i=0;i<4;i++)
	{
		if(game->die_To_Play[i] == 1)
			Nb_Des_Encore_Jouables++;
	}
	if(Nb_Des_Encore_Jouables > 0) // si il y a encore des dés a jouer
	{
		int sens = 1;
		if(player == EPlayer2)
		{
			sens = -1;
		}
			for(int i=0; i<24;i++) // pour toutes les fleches
			{
				if(  (gameState->zones[i].player == player)  &&  (gameState->zones[i].nb_checkers > 0)  ) // si la zone lui appartient
				{
					if(Nb_Des_Encore_Jouables == 1)// un seul dé jouable
					{
						int de = 0;
						for(int j=0; j<4; j++) // on récupère la valeur du dé non joué
						{
							if(game->die_To_Play[j] == 1)
							{
								switch(j)
								{
									case 1: // Le dé non joué est le dé 1
										de = gameState->die1;
										break;
										
									case 2:
										de = gameState->die2;
										break;
										
									default:
										de = gameState->die1;
										break;
								}
							}
						} 
						if( (i+(sens*de) <= EPos_24)  &&  (i+(sens*de) >= EPos_1) ) // si le dé permet de jouer sur une fleche (pas en out)
						{
							if(Arbitrator_Zone_Accessible(gameState, player, i+(sens*de)))
							{
								//mouvement possible ici
								return 1;
							}
						}
					}
					else if(Nb_Des_Encore_Jouables == 2)// deux dés jouables
					{
						int de1 = 0;
						int de2 = 0;
						for(int j=0; j<4; j++) // on récupère la valeur des dés non joués
						{
							if(game->die_To_Play[j] == 1)
							{
								switch(j)
								{
									case 1: // Un des dés non joués est le dé 1
										de1 = gameState->die1;
										break;
										
									case 2: // Un des dés non joués est le dé 2
										de2 = gameState->die2;
										break;
										
									default:
										break;
								}
							}
						}
						if(de1 == 0)
							de1 = gameState->die1;
						if(de2 == 0)
							de2 = gameState->die1;
								
						if((i+(sens*de1) <= EPos_24)  &&  (i+(sens*de1) >= EPos_1) ) // si le dé permet de jouer sur une fleche (pas en out)
						{
							if(Arbitrator_Zone_Accessible(gameState, player, i+(sens*de1)))
							{
								//mouvement possible ici
								return 1;
							}
						}
						if((i+(sens*de2) <= EPos_24)  &&  (i+(sens*de2) >= EPos_1) ) // si le dé permet de jouer sur une fleche (pas en out)
						{
							if(Arbitrator_Zone_Accessible(gameState, player, i+(sens*de2)))
							{
								//mouvement possible ici
								return 1;
							}
						}
						if((i+(sens*(de1+de2)) <= EPos_24)  &&  (i+(sens*(de1+de2)) >= EPos_1) ) // si le dé permet de jouer sur une fleche (pas en out)
						{
							if(Arbitrator_Zone_Accessible(gameState, player, i+(sens*(de1+de2))))
							{
								//mouvement possible ici
								return 1;
							}
						}
					
					}
					else if(Nb_Des_Encore_Jouables >= 3)// trois dés jouables
					{
						int de = gameState->die1; // 3 dés jouables, donc de1 = de2 = de3 = de4
						
						if((i+(sens*de) <= EPos_24)  &&  (i+(sens*de) >= EPos_1) ) // si le dé permet de jouer sur une fleche (pas en out)
						{
							if(Arbitrator_Zone_Accessible(gameState, player, i+(sens*de)))
							{
								//mouvement possible ici
								return 1;
							}
						}
						if((i+(2*(sens*de)) <= EPos_24)  &&  (i+(2*(sens*de)) >= EPos_1) ) // si le dé permet de jouer sur une fleche (pas en out)
						{
							if(Arbitrator_Zone_Accessible(gameState, player, i+(2*(sens*de))))
							{
								//mouvement possible ici
								return 1;
							}
						}
						if((i+(3*(sens*de)) <= EPos_24)  &&  (i+(3*(sens*de)) >= EPos_1) ) // si le dé permet de jouer sur une fleche (pas en out)
						{
							if(Arbitrator_Zone_Accessible(gameState, player, i+(3*(sens*de))))
							{
								//mouvement possible ici
								return 1;
							}
						}
						if(Nb_Des_Encore_Jouables > 3)
						{
							if((i+(4*(sens*de)) <= EPos_24)  &&  (i+(4*(sens*de)) >= EPos_1) ) // si le dé permet de jouer sur une fleche (pas en out)
						{
							if(Arbitrator_Zone_Accessible(gameState, player, i+(4*(sens*de))))
							{
								//mouvement possible ici
								return 1;
							}
						}
						}
					}
				}
			
			}
/************************		TEST POUR UNE POSSIBILITE EN SORTIE DE BAR		***************************************************/
			if(  ( (player == EPlayer1) && (gameState->zones[EPos_BarP1].nb_checkers > 0) )  ||  ( (player == EPlayer2) && (gameState->zones[EPos_BarP2].nb_checkers > 0) )  )
			{
				int pos_IN=EPos_1;
				if(player == EPlayer2)
					pos_IN=EPos_24;
				if(Nb_Des_Encore_Jouables == 1)// un seul dé jouable
				{
					int de = 0;
					for(int j=0; j<4; j++) // on récupère la valeur du dé non joué
					{
						if(game->die_To_Play[j] == 1)
						{
							switch(j)
							{
								case 1: // Le dé non joué est le dé 1
									de = gameState->die1;
									break;
									
								case 2:
									de = gameState->die2;
									break;
									
								default:
									de = gameState->die1;
									break;
							}
						}
					} 
					if( (pos_IN+(sens*de) <= EPos_24)  &&  (pos_IN+(sens*de) >= EPos_1) ) // si le dé permet de jouer sur une fleche (pas en out)
					{
						if(Arbitrator_Zone_Accessible(gameState, player, pos_IN+(sens*de)))
						{
							//mouvement possible ici
							return 1;
						}
					}
				}
				else if(Nb_Des_Encore_Jouables == 2)// deux dés jouables
				{
					int de1 = 0;
					int de2 = 0;
					for(int j=0; j<4; j++) // on récupère la valeur des dés non joués
					{
						if(game->die_To_Play[j] == 1)
						{
							switch(j)
							{
								case 1: // Un des dés non joués est le dé 1
									de1 = gameState->die1;
									break;
									
								case 2: // Un des dés non joués est le dé 2
									de2 = gameState->die2;
									break;
									
								default:
									break;
							}
						}
					}
					if(de1 == 0)
						de1 = gameState->die1;
					if(de2 == 0)
						de2 = gameState->die1;
							
					if((pos_IN+(sens*de1) <= EPos_24)  &&  (pos_IN+(sens*de1) >= EPos_1) ) // si le dé permet de jouer sur une fleche (pas en out)
					{
						if(Arbitrator_Zone_Accessible(gameState, player, pos_IN+(sens*de1)))
						{
							//mouvement possible ici
							return 1;
						}
					}
					if((pos_IN+(sens*de2) <= EPos_24)  &&  (pos_IN+(sens*de2) >= EPos_1) ) // si le dé permet de jouer sur une fleche (pas en out)
					{
						if(Arbitrator_Zone_Accessible(gameState, player, pos_IN+(sens*de2)))
						{
							//mouvement possible ici
							return 1;
						}
					}
					if((pos_IN+(sens*(de1+de2)) <= EPos_24)  &&  (pos_IN+(sens*(de1+de2)) >= EPos_1) ) // si le dé permet de jouer sur une fleche (pas en out)
					{
						if(Arbitrator_Zone_Accessible(gameState, player, pos_IN+(sens*(de1+de2))))
						{
							//mouvement possible ici
							return 1;
						}
					}
				
				}
				else if(Nb_Des_Encore_Jouables >= 3)// trois dés jouables
				{
					int de = gameState->die1; // 3 dés jouables, donc de1 = de2 = de3 = de4
					
					if((pos_IN+(sens*de) <= EPos_24)  &&  (pos_IN+(sens*de) >= EPos_1) ) // si le dé permet de jouer sur une fleche (pas en out)
					{
						if(Arbitrator_Zone_Accessible(gameState, player, pos_IN+(sens*de)))
						{
							//mouvement possible ici
							return 1;
						}
					}
					if((pos_IN+(2*(sens*de)) <= EPos_24)  &&  (pos_IN+(2*(sens*de)) >= EPos_1) ) // si le dé permet de jouer sur une fleche (pas en out)
					{
						if(Arbitrator_Zone_Accessible(gameState, player, pos_IN+(2*(sens*de))))
						{
							//mouvement possible ici
							return 1;
						}
					}
					if((pos_IN+(3*(sens*de)) <= EPos_24)  &&  (pos_IN+(3*(sens*de)) >= EPos_1) ) // si le dé permet de jouer sur une fleche (pas en out)
					{
						if(Arbitrator_Zone_Accessible(gameState, player, pos_IN+(3*(sens*de))))
						{
							//mouvement possible ici
							return 1;
						}
					}
					if(Nb_Des_Encore_Jouables > 3)
					{
						if((pos_IN+(4*(sens*de)) <= EPos_24)  &&  (pos_IN+(4*(sens*de)) >= EPos_1) ) // si le dé permet de jouer sur une fleche (pas en out)
					{
						if(Arbitrator_Zone_Accessible(gameState, player, pos_IN+(4*(sens*de))))
						{
							//mouvement possible ici
							return 1;
						}
					}	
					}
				}
					
			}
/************************		TEST POUR UNE POSSIBILITE EN SORTIE OUT		***************************************************/
			if(Arbitrator_Jeu_Out_Possible(gameState, player))
			{
				EPosition deb_zone_out = EPos_19;
				if(player == EPlayer2)
				{
					deb_zone_out = EPos_6;
				}
				for(int i=1;i <= 6 ;i++)
				{
					if(  (gameState->zones[deb_zone_out+(sens*i)-1].player == player)  &&  (gameState->zones[deb_zone_out+(sens*i)-1].nb_checkers > 0)  ) // si la zone lui appartient
					{
						if(Nb_Des_Encore_Jouables == 1)// un seul dé jouable
						{
							int de = 0;
							for(int j=0; j<4; j++) // on récupère la valeur du dé non joué
							{
								if(game->die_To_Play[j] == 1)
								{
									switch(j)
									{
										case 1: // Le dé non joué est le dé 1
											de = gameState->die1;
											break;
										
										case 2:
											de = gameState->die2;
											break;
										
										default:
											de = gameState->die1;
											break;
									}
								}
							} 
							if(i+de-1 == 6)
							{
								//mouvement possible ici
								return 1;
							}
						
						}
						else if(Nb_Des_Encore_Jouables == 2)// deux dés jouables
						{
							int de1 = 0;
							int de2 = 0;
							for(int j=0; j<4; j++) // on récupère la valeur des dés non joués
							{
								if(game->die_To_Play[j] == 1)
								{
									switch(j)
									{
										case 1: // Un des dés non joués est le dé 1
											de1 = gameState->die1;
											break;
										
										case 2: // Un des dés non joués est le dé 2
											de2 = gameState->die2;
											break;
										
										default:
											break;
									}
								}
							}
							if(de1 == 0)
								de1 = gameState->die1;
							if(de2 == 0)
								de2 = gameState->die1;
								
							if(i+de1-1 == 6)
							{
								//mouvement possible ici
								return 1;
							
							}
							if(i+de2-1 == 6)
							{
								//mouvement possible ici
								return 1;
							}
						
							if(i+de1+de2-1 == 6)
							{
								//mouvement possible ici
								return 1;
							}
						
					
						}
						else if(Nb_Des_Encore_Jouables >= 3)// trois dés jouables
						{
							int de = gameState->die1; // 3 dés jouables, donc de1 = de2 = de3 = de4
						
							if(i+de-1 == 6)
							{
								//mouvement possible ici
								return 1;
							}
						
							if(i+(2*de)-1 == 6)
							{
								//mouvement possible ici
								return 1;
							}
						
							if(i+(3*de)-1 == 6)
							{
								//mouvement possible ici
								return 1;
							}
							if(Nb_Des_Encore_Jouables > 3)
							{
								if(i+(4*de)-1 == 6)
								{
									//mouvement possible ici
									return 1;
								}
							}
						
						}
					}
				}
			}	
	
	}
	return 0;
}

int Arbitrator_Zone_Accessible(SGameState *gameState, EPlayer player, int pos)
{
	if( (pos <= (int)EPos_OutP2) && (pos >= (int)EPos_1) )
	{
		if(  (gameState->zones[pos].player == player)  ||  (gameState->zones[pos].nb_checkers < 2)  )
			return 1;
	}
}
