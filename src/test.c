int Display_CheckersPossibilities(SDisplay *display, SGameState *gameState, EPlayer player, SGame *game, int Die_For_Play[7])
{
	int i=0;
	int tab[28];
	int* arriveesValides;
	int cpt=0;
	int posDepartSelected = 0;
	for(int i=0;i<28;i++) tab[i]=-1;
	SDL_Event event;
	
	if(gameState->die1 != gameState->die2) // Si dés différents
	{
		for( i=0; i<24; i++) // Pour chaque flèche
		{
			if(gameState->zones[i].nb_checkers>0 && gameState->zones[i].player==player)
			{
				// On regarde s'il y a possibilité de sortie du pion suivant le joueur
				if(allInJan(gameState,player))
				{
					int out;
					
					if(player == EPlayer1)
						out = 24;
					else
						out = 26;
						
					if((i+gameState->die1)>=24)
					{
						printf("Possibilite de sortie avec de1 et pion sur fleche %d\n",i+1);
						colorChecker(display,gameState,i);
						tab[cpt]=i;
						cpt++;
					}
				
					if((i+gameState->die2)>=24)
					{
						printf("Possibilite de sortie avec de2 et pion sur fleche %d\n",i+1);
						colorChecker(display,gameState,i);
						tab[cpt]=i;
						cpt++;
					}
					if((i+gameState->die1+gameState->die2)>=24)
					{
						printf("Possibilite de sortie avec de1+de2 et pion sur fleche %d\n",i+1);
						colorChecker(display,gameState,i);
						tab[cpt]=i;
						cpt++;
					}
				}
				else if(player == EPlayer2 && allInJan(gameState,player))
				{
					if((i+gameState->die1)>=26)
					{
						printf("Possibilite de sortie avec de1 et pion sur fleche %d\n",i+1);
						colorChecker(display,gameState,i);
						tab[cpt]=i;
						cpt++;
					}
				
					if((i+gameState->die2)>=24)
					{
						printf("Possibilite de sortie avec de2 et pion sur fleche %d\n",i+1);
						colorChecker(display,gameState,i);
						tab[cpt]=i;
						cpt++;
					}
					if((i+gameState->die1+gameState->die2)>24)
					{
						printf("Possibilite de sortie avec de1+de2 et pion sur fleche %d\n",i+1);
						colorChecker(display,gameState,i);
						tab[cpt]=i;
						cpt++;
					}
				}
				
				if( (i+gameState->die1)<=28 && (i+gameState->die1)!=26 && (i+gameState->die1)!=28 && (gameState->zones[i+gameState->die1].nb_checkers==1 || gameState->zones[i+gameState->die1].nb_checkers==0 || gameState->zones[i+gameState->die1].player==player) )
				{
				
				
					printf("Possibilite sur fleche %d avec de1 et pion sur fleche %d\n",i+1+gameState->die1,i+1);
					colorChecker(display,gameState,i);
					tab[cpt]=i;
					cpt++;
				}
				if( (i+gameState->die2)<=28 && (i+gameState->die2)!=26 && (i+gameState->die2)!=28 && (gameState->zones[i+gameState->die2].nb_checkers==0 || gameState->zones[i+gameState->die2].nb_checkers==1 || gameState->zones[i+gameState->die2].player==player) )
				{
					printf("Possibilite sur fleche %d avec de2 et pion sur fleche %d\n",i+1+gameState->die2,i+1);
					colorChecker(display,gameState,i);
					tab[cpt]=i;
					cpt++;
				}
				if( (i+gameState->die1+gameState->die2)<=28 && (i+gameState->die1+gameState->die2)!=26 && (i+gameState->die1+gameState->die2)!=28 &&(gameState->zones[i+gameState->die1+gameState->die2].nb_checkers==1 || gameState->zones[i+gameState->die1+gameState->die2].nb_checkers==0 || gameState->zones[i+gameState->die1+gameState->die2].player==player) )
				{
					printf("Possibilite sur fleche %d avec de1+de2 et pion sur fleche %d\n",i+1+gameState->die1+gameState->die2,i+1);
					colorChecker(display,gameState,i);
					tab[cpt]=i;
					cpt++;
				}
			}
		}
		if(cpt == 0) // Pas de pions bougeables
		{
			SDL_Rect msg_position;
			msg_position.x = 80;
			msg_position.y = 325;
			SDL_Color msg_color = {255, 255, 255, 0};
			Display_Message( display, "Pas de mouvement possible!", msg_position, msg_color,1);
			SDL_Flip(display->screen);
			SDL_Delay(10000);
			return 1;
		}
		SDL_Flip(display->screen);
		while(1)
		{
			SDL_WaitEvent(&event);
			EPosition posDepart;
			EPosition posArrivee;
			int aut =0;
			SMove mouvement;
			switch(event.type)
			{
				case SDL_QUIT:
					if(posDepartSelected)
						free(arriveesValides);
					return 1;
					break;
				
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						//Si il y a clic sur un autre pion de départ cliqué, malgré qu'un pion de départ ait déja été choisi avant
						if(CheckerWithScreenPosition(event.button.x, event.button.y, &posDepart) && inTab(posDepart, tab) && posDepartSelected && !inTab(posDepart, arriveesValides))
						{
							posDepartSelected = !posDepartSelected;
						}
						
						if(!posDepartSelected) // selection de la position depart
						{
							if(CheckerWithScreenPosition(event.button.x, event.button.y, &posDepart)) // il a trouvé une position valide 
							{
								if(inTab(posDepart, tab))
								{
									arriveesValides = Display_Arrow_Possibilities(display, gameState, player, posDepart, game);
									posDepartSelected = 1;
									mouvement.src_point = posDepart+1;
								}
							}
						}
						else // selection de la position arrivée
						{
							if(CheckerWithScreenPosition(event.button.x, event.button.y, &posArrivee)) // il a trouvé une position valide
							{
								printf("POS ARRIVEE\n");
								if(inTab(posArrivee, arriveesValides))
								{
									printf("POS ARRIVEE VALIDE\n");
									mouvement.dest_point = posArrivee+1;
									free(arriveesValides);

									if(authorized_deplacement(gameState, &mouvement, player,Die_For_Play))
									{
										aut = 1;
									}
									printf("JOUEUR %d\tSRC %d\tDEST %d\tDIE1 %d\tDIE2 %d MVT AUTORISE %d\n",player,mouvement.src_point,mouvement.dest_point,gameState->die1,gameState->die2,aut);
									aut = 0;
									Checker_Move(display,gameState,&mouvement, game);
									return 0;
								}
							}
							else // Si clic ailleur on efface les possibilités et on réaffiche les pions déplaçables.
							{
								free(arriveesValides);
								Display_RefreshGameBoard(display, gameState, game);
								return Display_CheckersPossibilities(display, gameState, player, game, Die_For_Play);
							}
						}
					}
					break;
				}
			}
	}
	return 0;
}
