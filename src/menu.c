#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h> // Pour utiliser des formats d'image autre que bmp
#include <SDL/SDL_ttf.h> // Pour la gestion des textes sous SDL

#include "../include/gui.h"
#include "../include/menu.h"
#include "../include/backgammon.h"

int Menu_Click(int x, int y)
{
	if ( x>=260 && x<=300 && y>=284 && y<=315 ) // Jeton vert choisi
		return 1;
		
	if ( x>=402 && x<=444 && y>=284 && y<=315 ) // Jeton blanc choisi
		return 2;
		
	if ( x>=260 && x<=300 && y>=330 && y<=360 ) // Jouer avec le double
		return 3;
		
	if ( x>=402 && x<=444 && y>=330 && y<=360 ) // Jouer sans le double
		return 4;

	if ( x>=64 && x<=210 && y>=442 && y<=480 ) // Quitter la partie
		return 5;
	
	if ( x>=295 && x<=412 && y>=442 && y<=480 ) // Jouer
		return 6;
	
	if( x>=262 && x<=532 && y>=207 && y<=244 ) // Clic sur changer nom Joueur 1
		return 7;
		
	if( x>=262 && x<=532 && y>=260 && y<=297 ) // Clic sur changer nom Joueur 2
		return 8;

	if( x>=273 && x<=300 && y>=379 && y<=411 ) // Clic sur flèche score gauche
		return 9;
	
	if( x>=357 && x<=386 && y>=379 && y<=411 ) // Clic sur flèche score gauche
		return 10;
	
	return -1; // Clic ailleur
}

void Menu_Text(SDisplay	*display, char	*message, SDL_Rect position, SDL_Color color)
{
	SDL_Surface *msg = TTF_RenderText_Blended(display->font,message,color);
	SDL_BlitSurface(msg, NULL, display->screen, &position);
	SDL_FreeSurface(msg);
	SDL_Flip(display->screen);
}


void TextInput(char* name, SDL_keysym key)
{
    int length = strlen(name);

    // Gestion de l'effacement
    if (length > 0 && key.sym == SDLK_BACKSPACE)
        name[length - 1] = '\0';

    // Gestion des touches
    else if (length < 20) // Limite à 20 caractères le nom du joueur
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
	SDL_Color c = {0, 0, 0, 0};
	SDL_Color col = {255, 255, 255, 0};
	SDL_Rect player1_name_position;
	SDL_Rect player2_name_position;
	
	char* background;
	char* temp_path;
	char* scoreLimit;
	
	background = (char*)malloc(100*sizeof(char));
	temp_path = (char*)malloc(100*sizeof(char));
	scoreLimit = (char*)malloc(3*sizeof(char));
	
	sprintf(scoreLimit, "%d", display->game->scoreLimit);
	
	menu_position.x = 0;
	menu_position.y = 0;
	
	player1_name_position.x = 275;
	player1_name_position.y = 210;
	player2_name_position.x = 275;
	player2_name_position.y = 260;
	
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
		
		SDL_BlitSurface(display->background_menu, NULL, display->screen, &menu_position);
		
		// Affichage du nom du joueur
		Menu_Text(display,display->game->player1_name,player1_name_position,c);
		
		// Affichage du score
		check_position.x = 320;
		check_position.y = 375;
		Menu_Text(display, scoreLimit, check_position,col);
		
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
		
		SDL_BlitSurface(display->background_menu, NULL, display->screen, &menu_position);
		
		// Affichage des noms des joueurs
		Menu_Text(display,display->game->player1_name,player1_name_position,c);
		Menu_Text(display,display->game->player2_name,player2_name_position,c);
		
		// Affichage du score
		check_position.x = 320;
		check_position.y = 378;
		Menu_Text(display, scoreLimit, check_position,col);
		
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
	SDL_Flip(display->screen);
	
	free(temp_path);
	free(scoreLimit);
	//free(background);
}
	
int Display_Menu(SDisplay *display, E_GameMode gameMode)
{
	
	SDL_Surface *player_name_selection;
	int numPlayerSelected = 0;
	SDL_Rect temp_position;
	
	char *temp_path;
	
	temp_path = (char*)malloc(100*sizeof(char));
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "selection.png");
	player_name_selection = IMG_Load(temp_path);
	
	
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
								numPlayerSelected = 0;
								display->game->player1_checker = GREEN;
								display->game->player2_checker = WHITE;
								Menu_Fill(display, gameMode);
							}
							break;
							
						case 2 : // Clic sur jeton blanc
							if(gameMode == HUMAN_AI)
							{
								numPlayerSelected = 0;
								display->game->player1_checker = WHITE;
								display->game->player2_checker = GREEN;
								Menu_Fill(display, gameMode);
							}
							break;
							
						case 3 : // Clic sur oui
							numPlayerSelected = 0;
							display->game->withDouble = 1;
							Menu_Fill(display, gameMode);
							break;
						
						case 4 : // Clic sur non
							numPlayerSelected = 0;
							display->game->withDouble = 0;
							Menu_Fill(display, gameMode);
							break;
						
						case 5 : // Clic sur quitter
							return 1;
							
						case 6 : // Clic sur jouer
							return 0;
						
						case 7 : // Clic sur changer nom joueur 1
							numPlayerSelected = 1;
							Menu_Fill(display, gameMode);
							temp_position.x = 258;
							temp_position.y = 203;
							SDL_BlitSurface(player_name_selection, NULL, display->screen, &temp_position);
							SDL_Flip(display->screen);
							break;
							
						case 8 :
							if(gameMode == HUMAN_HUMAN)
							{	
								numPlayerSelected = 2;
								Menu_Fill(display, gameMode);
								temp_position.x = 258;
								temp_position.y = 255;
								SDL_BlitSurface(player_name_selection, NULL, display->screen, &temp_position);
								SDL_Flip(display->screen);
							}
							break;
						
						case 9 :
							if(display->game->scoreLimit >1)
							{
								display->game->scoreLimit = display->game->scoreLimit - 2 ;
								Menu_Fill(display, gameMode);
							}
							break;
							
						case 10 :
							if(display->game->scoreLimit < 15)
							{
								display->game->scoreLimit = display->game->scoreLimit + 2 ;
								Menu_Fill(display, gameMode);
							}
							break;
							
						default :
							break;
					}
				}
				break;
			case SDL_KEYUP:
				if(numPlayerSelected == 1 )
				{
					TextInput(display->game->player1_name, event.key.keysym);
					Menu_Fill(display, gameMode);
					temp_position.x = 258;
					temp_position.y = 203;
					SDL_BlitSurface(player_name_selection, NULL, display->screen, &temp_position);
					SDL_Flip(display->screen);
				}
				else if(numPlayerSelected == 2 && gameMode == HUMAN_HUMAN)
				{
					TextInput(display->game->player2_name, event.key.keysym);
					Menu_Fill(display, gameMode);
					temp_position.x = 258;
					temp_position.y = 255;
					SDL_BlitSurface(player_name_selection, NULL, display->screen, &temp_position);
					SDL_Flip(display->screen);
				}
				break;
		}
	}
}
