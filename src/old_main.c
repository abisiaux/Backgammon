/*while(!quit) // Boucle de Match
		{*/
			/*printf("AVANT JOUEUR");
>>>>>>> 53a5965c632f704360311b3887699e12f6335de3
			if(curentPlayer == EPlayer1)
			{
				curentPlayer = EPlayer2;
				sprintf(tmp,"%s, c'est a toi de jouer.",display.game->player2_name);
				Display_Message(&display, tmp, msg_position, msg_color,1);
				SDL_Delay(2000);
			}
			else
			{
				curentPlayer = EPlayer1;
				sprintf(tmp,"%s, c'est a toi de jouer.",display.game->player1_name);
				Display_Message(&display, tmp, msg_position, msg_color,1);
				SDL_Delay(2000);
<<<<<<< HEAD
			}
			Display_Refresh(&display, gameState);
			SDL_Flip(display.screen);
			Launch_Die(gameState);
			Display_Die(&display,gameState);
			
			
			int DepartSelected=0;
			SMove mouvement;
			int nb_die_used=0;
			int nb_die_can_play=numberOfDieCanPlay(gameState, curentPlayer);
			while(nb_die_used < nb_die_can_play)//Tant que le joueur n'a pas joué tous ses coups
			{
				EPosition pos=0;;
				
=======
			}*/
			/*Display_Refresh(&display, gameState);
			
			Launch_Die(gameState);
			Display_Die(&display,gameState);
			
			int DepartSelected=0;
			SMove mouvement;
			int nb_die_used=0;
			int nb_die_can_play = numberOfDieCanPlay(gameState, curentPlayer);
			printf("nb_die_can_play:%d\n",nb_die_can_play);
			
			
			while(nb_die_used < nb_die_can_play)//Tant que le joueur n'a pas joué tous ses coups
			{
				EPosition pos=0;
				
				SDL_WaitEvent(&event);
				switch(event.type)
				{
					case SDL_QUIT:
						quit = 1;
						break;
					case SDL_MOUSEBUTTONUP:

						
						if (event.button.button == SDL_BUTTON_LEFT ) //&& Pion_Depart_Autorise(event.button.x,event.button.y)
						{// si le joueur clic sur un des pions correspondant aux pions qui peuvent être déplacés sur ce tour
							if(CheckerWithScreenPosition(event.button.x,event.button.y, &pos))
							{
								printf("POS+1 =%d\tposDepart:%d\n",pos+1,DepartSelected);
								colorChecker(&display, gameState, pos);
								SDL_Flip(display.screen);
								if(!DepartSelected)// si la position de depart n'est pas definie
								{
									DepartSelected=1;
									mouvement.src_point = pos+1;// le plus 1 na rien a faire !!! modifier
									printf("pos DEPARRT\t%d\n",DepartSelected);
								}
								else // si la pos depart est select on creer le mouvement
								{
									printf("pos ARRIVEE\n");
									DepartSelected=0;
									mouvement.dest_point = pos+1;// le plus 1 na rien a faire !!! modifier
									printf("MOVE src:%d\tdest:%d",mouvement.src_point,mouvement.dest_point);
									Checker_Move(&display,gameState,&mouvement);
									
									nb_die_used += numberofDieForMove(gameState, curentPlayer, mouvement);
									printf("nb_die_used:%d\n",nb_die_used); 
									
									
								}
							}
;
						}
			  			break;

				}
			
			}
			printf("fin DE LA PARTIE\n");
						
			SDL_Delay(10000);
			quit = 1;
			
			
			
			
			
			
			/*Display_Possibilities(&display,gameState, 0);
			
			
			move.src_point = 8;
			move.dest_point = 6;
			if(authorized_deplacement(gameState, &move, EPlayer2))// JOUEUR 1 = VERT
			{
				printf("DEPLACEMENT AUTORISE\n");
				Checker_Move(&display,gameState,&move);
		
			}
			else
				printf("EPLACEMENT NON AUTORISE\n");
	
			*/
			/*SDL_WaitEvent(&event);
			switch(event.type)
			{
				case SDL_QUIT:
					quit = 1;
					break;
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT && Quit_Zone(event.button.x,event.button.y))
					{
						quit = 1;
					}
			  			break;

			}
			Display_Refresh(&display, gameState);
		//}
