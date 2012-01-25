
int Display_CheckersPossibilities(SDisplay *display, SGameState *gameState, EPlayer player, SGame *game, int Die_For_Play[7])
{
	
	
	int mat[28][28]
	int i,j,quit=0;
	
	int noneMove = 1;
	
	// Initialisation de la matrice de stockage des couples de mouvement possibles (src,dest)
	for(i=0;i<28;i++)
	{
		for(j=0;j<28;j++)
		{
			mat[i][j] = 0;
		}
	}
	
	SMove tmp;
	
	// Remplissage de la matrice
	for(i=0;i<28;i++)
	{
		if(gameState->zones[i].player == player && gameState->zones[i].nb_checkers>0) // Si les pions sur la flèche appartiennent au joueur
		{
			// Alors on regarde les destinations possibles et on stocke les couples possibles dans la matrice en inscrivant 1 dans la case correspondante
			for(j=0;j<28;j++)
			{
				tmp.src_point = (EPosition)i;
				tmp.dest_point = (EPosition)j;
				if(authorized_deplacement(game, tmp, player, Die_For_Play) // Si le déplacement flèche est autorisé
				{
					mat[i][j] = 1;
					noneMove = 0;
				}
			}
		}
	}
	
	// On appelle la fonction pour choisir le mouvement
	if( noneMove)
	{
		SDL_Rect msg_position;
		msg_position.x = 80;
		msg_position.y = 325;
		SDL_Color msg_color = {255, 255, 255, 0};
		Display_Message( display, "Pas de mouvement possible!", msg_position, msg_color,1);
		SDL_Flip(display->screen);
		SDL_Delay(10000);
	}
	else
	{
		quit = makeChoice(display, gameState, game, mat);
	}
	return quit
}

int src_valid(int mat[28][28], EPosition src)
{
	int i,j;
	for(i=0;i<28;i++)
	{
		for(j=0;j<28;j++)
		{
			if(mat[i][j])
				return 1
		}
	}
	return 0;
}

int dest_valid(int mat[28][28], EPosition src, EPosition dest)
{
	return mat[src][dest];
}

int makeChoice(SDisplay *display, SGameState *gameState, SGame *game, int mat[28][28])
{
	SDL_Event event;
	int posDepartSelected = 0;
	
	// Affichage des pions qui peuvent être bougés
	int i,j;
	for(i=0;i<28;i++)
	{
		for(j=0;j<28;j++)
		{
			if(mat[i][j])
				colorChecker(display,gameState,i);
		}
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
				return 1;
				
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					//Si il y a clic sur un autre pion de départ cliqué, malgré qu'un pion de départ ait déja été choisi avant
					if( posDepartSelected && CheckerWithScreenPosition(event.button.x, event.button.y, &posDepart) && src_valid(mat,posDepart) && !dest_valid(mat, posDepart, posDepart))
					{
						posDepartSelected = !posDepartSelected;
					}
					
					if(!posDepartSelected) // selection de la position depart
					{
						if(CheckerWithScreenPosition(event.button.x, event.button.y, &posDepart)) // il a trouvé une position valide 
						{
							if(src_valid(mat,posDepart))
							{
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
							if(dest_valid(mat, posDepart, posArrivee))
							{
								printf("POS ARRIVEE VALIDE\n");
								mouvement.dest_point = posArrivee+1;

								printf("JOUEUR %d\tSRC %d\tDEST %d\tDIE1 %d\tDIE2 %d MVT AUTORISE %d\n",player,mouvement.src_point,mouvement.dest_point,gameState->die1,gameState->die2,aut);
								
								Checker_Move(display,gameState,&mouvement, game);
								return 0;
							}
						}
						else // Si clic ailleur on efface les possibilités et on réaffiche les pions déplaçables.
						{
							Display_RefreshGameBoard(display, gameState, game);
							return makeChoice(display, gameState, game, mat);
						}
					}
				}
				break;
		}
	}
}
	
