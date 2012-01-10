#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h> // Pour utiliser des formats d'image autre que bmp
#include <SDL/SDL_ttf.h> // Pour la gestion des textes sous SDL

#include "../include/gui.h"
#include "../include/backgammon.h"

int Menu_Click(int x, int y)
{
	if ( x>=260 && x<=300 && y>=284 && y<=315 ) // Jeton vert choisi
	{
		return 1;
	}
	if ( x>=402 && x<=444 && y>=284 && y<=315 ) // Jeton blanc choisi
	{
		return 2;
	}
	if ( x>=260 && x<=300 && y>=330 && y<=360 ) // Jouer avec le double
	{
		return 3;
	}
	if ( x>=402 && x<=444 && y>=330 && y<=360 ) // Jouer sans le double
	{
		return 4;
	}
	if ( x>=64 && x<=210 && y>=442 && y<=480 ) // Quitter la partie
	{
		return 5;
	}
	if ( x>=295 && x<=412 && y>=442 && y<=480 ) // Jouer
	{
		return 6;
	}
	
	return -1; // Clic ailleur
}

void Menu_PlayerName(SDisplay	*display, char	*message, SDL_Rect position, SDL_Color color)
{

	SDL_Surface *rect = SDL_CreateRGBSurface(SDL_HWSURFACE, 150, 30, 32, 0, 0, 0, 0);
	SDL_FillRect(rect, NULL, SDL_MapRGB(display->screen->format, 0, 0, 0));
	SDL_BlitSurface(rect, NULL, display->screen, &position);
	SDL_Surface *msg = TTF_RenderText_Blended(display->font,message,color);
	SDL_BlitSurface(msg, NULL, display->screen, &position);
	SDL_FreeSurface(msg);
	SDL_Flip(display->screen);
}

/*int Display_Menu(SDisplay *display, E_GameMode gameMode)
{
	SDL_Rect menu_position;
	SDL_Rect checker_check_position;
	SDL_Rect double_check_position;
	SDL_Color c = {255, 255, 255, 0};
	SDL_Rect player1_name_position;
	SDL_Rect player2_name_position;
	
	SDL_Event event;
	char *temp_path;
	char* background;
	
	temp_path = (char*)malloc(100*sizeof(char));
	background = (char*)malloc(100*sizeof(char));

	menu_position.x = 0;
	menu_position.y = 0;
	
	player1_name_position.x = 260;
	player1_name_position.y = 205;
	player2_name_position.x = 265;
	player2_name_position.y = 245;
	
	if(gameMode == HUMAN_AI)
	{
		background = "background_menu1P.png";
	}
	else
	{
		background = "background_menu2P.png";
	}
	
	// Chargement du menu
	strcpy(temp_path, display->img_path);
	strcat(temp_path, background);
	display->background_menu = IMG_Load(temp_path);

	strcpy(temp_path, display->img_path);
	strcat(temp_path, "check.png");
	display->checked = IMG_Load(temp_path);
	
	SDL_BlitSurface(display->background_menu, NULL, display->screen, &menu_position);
	
	if(gameMode == HUMAN_AI)
	{
		Menu_PlayerName(display,display->game->player1_name,player1_name_position,c);
		
		checker_check_position.x = 255;
		checker_check_position.y = 275;
		SDL_BlitSurface(display->checked, NULL, display->screen, &checker_check_position);
		
		double_check_position.x = 255;
		double_check_position.y = 320;
		SDL_BlitSurface(display->checked, NULL, display->screen, &double_check_position);
	}
	else
	{
		Menu_PlayerName(display,display->game->player1_name,player1_name_position,c);
		Menu_PlayerName(display,display->game->player2_name,player2_name_position,c);
		
		double_check_position.x = 255;
		double_check_position.y = 320;
		SDL_BlitSurface(display->checked, NULL, display->screen, &double_check_position);
	}
	
	SDL_Flip(display->screen);
	
	while(1)
	{
		SDL_WaitEvent(&event);
	
		switch(event.type)
		{
			case SDL_QUIT:
				free(temp_path);
				return -1;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					switch( Menu_Click(event.button.x, event.button.y) )
					{
						case 1 : // Clic sur le jeton vert
							if(gameMode == HUMAN_AI)
							{
								// Rechargement du menu
								strcpy(temp_path, display->img_path);
								strcat(temp_path, background);
								display->background_menu = IMG_Load(temp_path);
								SDL_BlitSurface(display->background_menu, NULL, display->screen, &menu_position);
								
								// Affichage du nom du joueur
								Menu_PlayerName(display,display->game->player1_name,player1_name_position,c);
								
								// Affichage de l'option double
								SDL_BlitSurface(display->checked, NULL, display->screen, &double_check_position);
									
								checker_check_position.x = 255;
								checker_check_position.y = 275;
								
								SDL_BlitSurface(display->checked, NULL, display->screen, &checker_check_position);
								
								display->game->player1_checker = GREEN;
								display->game->player2_checker = WHITE;
							}
							break;
							
						case 2 : // Clic sur jeton blanc
							if(gameMode == HUMAN_AI)
							{
								// Rechargement du menu
								strcpy(temp_path, display->img_path);
								strcat(temp_path, background);
								display->background_menu = IMG_Load(temp_path);
								SDL_BlitSurface(display->background_menu, NULL, display->screen, &menu_position);
								
								// Affichage du nom du joueur
								Menu_PlayerName(display,display->game->player1_name,player1_name_position,c);
								
								// Affichage de l'option double
								SDL_BlitSurface(display->checked, NULL, display->screen, &double_check_position);
									
								// Affichage de l'option couleur du pion
								checker_check_position.x = 400;
								checker_check_position.y = 275;
								SDL_BlitSurface(display->checked, NULL, display->screen, &checker_check_position);
								
								display->game->player1_checker = WHITE;
								display->game->player2_checker = GREEN;
							}
							break;
							
						case 3 : // Clic sur oui
						
							// Rechargement du menu
							strcpy(temp_path, display->img_path);
							strcat(temp_path, background);
							display->background_menu = IMG_Load(temp_path);
							SDL_BlitSurface(display->background_menu, NULL, display->screen, &menu_position);
							
							// Affichage du nom du joueur
							if(gameMode == HUMAN_AI)
							{
								Menu_PlayerName(display,display->game->player1_name,player1_name_position,c);
								SDL_BlitSurface(display->checked, NULL, display->screen, &checker_check_position);
							}
							else
							{ 
								Menu_PlayerName(display,display->game->player1_name,player1_name_position,c);
								Menu_PlayerName(display,display->game->player2_name,player2_name_position,c);
							}
								
							// Affichage de l'option double
							double_check_position.x = 255;
							double_check_position.y = 320;
							SDL_BlitSurface(display->checked, NULL, display->screen, &double_check_position);	
							display->game->withDouble = 1;
							break;
						
						case 4 : // Clic sur non
						
							// Rechargement du menu
							strcpy(temp_path, display->img_path);
							strcat(temp_path, background);
							display->background_menu = IMG_Load(temp_path);
							SDL_BlitSurface(display->background_menu, NULL, display->screen, &menu_position);
							
							// Affichage du nom du joueur
							if(gameMode == HUMAN_AI)
							{
								Menu_PlayerName(display,display->game->player1_name,player1_name_position,c);
								SDL_BlitSurface(display->checked, NULL, display->screen, &checker_check_position);
							}
							else
							{ 
								Menu_PlayerName(display,display->game->player1_name,player1_name_position,c);
								Menu_PlayerName(display,display->game->player2_name,player2_name_position,c);
							}
								
							// Affichage de l'option double
							double_check_position.x = 400;
							double_check_position.y = 320;
							SDL_BlitSurface(display->checked, NULL, display->screen, &double_check_position);	
							display->game->withDouble = 0;
							break;
						
						case 5 : // Clic sur quitter
							free(temp_path);
							return 1;
							
						case 6 : // Clic sur jouer
							free(temp_path);
							return 0;
							
						default :
							break;
					}
				}
				break;
			case SDL_KEYUP:
				TextInput(display->game->player1_name, event.key.keysym);
				// Affichage du nom du joueur
				Menu_PlayerName(display,display->game->player1_name,player1_name_position,c);
				printf("%s\n",display->game->player1_name);
				break;
		}
		
		SDL_Flip(display->screen);
	}
}*/

void TextInput(char* name, SDL_keysym key)
{
    int length = strlen(name);

    // Gestion de l'effacement
    if (length > 0 && key.sym == SDLK_BACKSPACE)
        name[length - 1] = '\0';

    // Gestion des touches
    else if (length < 13)
    {
        int ascii = -1;

        if ((key.sym >= SDLK_a && key.sym <= SDLK_z) ||
            (key.sym >= SDLK_0 && key.sym <= SDLK_9) ||
            key.sym == SDLK_SPACE)
        {
            ascii = key.sym;

            // Gestion de la majuscule
            if ((key.mod & KMOD_SHIFT) && (key.sym >= SDLK_a && key.sym <= SDLK_z))
                ascii -= 32;
        }
        else if (key.sym >= SDLK_KP0 && key.sym <= SDLK_KP9)
            ascii = key.sym - 208;

        if (ascii != -1)
        {
            name[length] = ascii;
            name[length + 1] = '\0';
        }
   }
}

void Menu_Fill(SDisplay *display,E_GameMode gameMode)
{
	SDL_Rect menu_position;
	SDL_Rect check_position;
	SDL_Color c = {255, 255, 255, 0};
	SDL_Rect player1_name_position;
	SDL_Rect player2_name_position;
	
	char* background;
	char* temp_path;
	
	background = (char*)malloc(100*sizeof(char));
	temp_path = (char*)malloc(100*sizeof(char));
	
	menu_position.x = 0;
	menu_position.y = 0;
	
	player1_name_position.x = 260;
	player1_name_position.y = 205;
	player2_name_position.x = 265;
	player2_name_position.y = 245;
	
	// Chargement de l'image check
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "check.png");
	display->checked = IMG_Load(temp_path);
	
	if(gameMode == HUMAN_AI)
	{
		background = "background_menu1P.png";
		
		// Chargement du menu
		strcpy(temp_path, display->img_path);
		strcat(temp_path, background);
		display->background_menu = IMG_Load(temp_path);
		
		// Affichage du nom du joueur
		Menu_PlayerName(display,display->game->player1_name,player1_name_position,c);
		
		// Affichage des options cochées
		if(display->game->player1_checker == GREEN)
			check_position.x = 255;
		else
			check_position.x = 400;
			
		check_position.y = 275;
		
		SDL_BlitSurface(display->checked, NULL, display->screen, &check_position);
		
		if(display->game->withDouble)
			check_position.x = 255;
		else
			check_position.x = 400;
			
		check_position.y = 320;
		
		SDL_BlitSurface(display->checked, NULL, display->screen, &check_position);
		
		
	}
	else
	{
		background = "background_menu2P.png";
		
		// Chargement du menu
		strcpy(temp_path, display->img_path);
		strcat(temp_path, background);
		display->background_menu = IMG_Load(temp_path);
		
		// Affichage des noms des joueurs
		Menu_PlayerName(display,display->game->player1_name,player1_name_position,c);
		Menu_PlayerName(display,display->game->player2_name,player2_name_position,c);
		
		// Affichage des options cochées
				
		if(display->game->withDouble)
		{
			check_position.x = 255;
		}
		else
			check_position.x = 400;
			
		check_position.y = 320;
		
		SDL_BlitSurface(display->checked, NULL, display->screen, &check_position);
	}
	SDL_BlitSurface(display->background_menu, NULL, display->screen, &menu_position);
}
	
int Display_Menu(SDisplay *display, E_GameMode gameMode)
{
	
	SDL_Event event;
	
	Menu_Fill(display, gameMode);
	SDL_Flip(display->screen);
	while(1)
	{
	
		SDL_WaitEvent(&event);
	
		switch(event.type)
		{
			case SDL_QUIT:
				return -1;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					switch( Menu_Click(event.button.x, event.button.y) )
					{
						case 1 : // Clic sur le jeton vert
							if(gameMode == HUMAN_AI)
							{							
								display->game->player1_checker = GREEN;
								display->game->player2_checker = WHITE;
							}
							break;
							
						case 2 : // Clic sur jeton blanc
							if(gameMode == HUMAN_AI)
							{
								display->game->player1_checker = WHITE;
								display->game->player2_checker = GREEN;
							}
							break;
							
						case 3 : // Clic sur oui
							display->game->withDouble = 1;
							break;
						
						case 4 : // Clic sur non
							display->game->withDouble = 0;
							break;
						
						case 5 : // Clic sur quitter
							return 1;
							
						case 6 : // Clic sur jouer
							return 0;
							
						default :
							break;
					}
				}
				break;
			case SDL_KEYUP:
				TextInput(display->game->player1_name, event.key.keysym);
				Menu_Fill(display, gameMode);
				break;
		}
	}
}
