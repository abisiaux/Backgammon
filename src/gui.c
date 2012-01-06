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


void Display_Init(SDisplay *display)
{	
	int i;
	char *temp_path;
	char *temp_name;
	
	// Initialisation de la SDL
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de la SDL");
		exit(EXIT_FAILURE);
	}
	
	// Enregistrement du path des images
	display->img_path = "../ressources/pictures/";
	
	// Enregistrement du path des polices
	display->font_path = "../ressources/fonts/";
	temp_path = (char*)malloc(100*sizeof(char));
	
	// Chargement de l'icone de la fenêtre
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "icon.bmp");
	SDL_WM_SetIcon(IMG_Load(temp_path), NULL);
	
	// Chargement du background et initialisation de sa position
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "background.png");
	display->background = IMG_Load(temp_path);
	display->background_position.x = 0;
	display->background_position.y = 0;
	
	// Chargement de l'image de pion blanc
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "white_checker.png");
	display->white_checker = IMG_Load(temp_path);
	
	// Chargement de l'image de pion noir
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "green_checker.png");
	display->green_checker = IMG_Load(temp_path);
	
	// Chargement du cadre de possibilité
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "possibility.png");
	display->possibility = IMG_Load(temp_path);
	
	// Chargement de l'image du cadre d'un message
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "msgBox.png");
	display->msg_box = IMG_Load(temp_path);
	
	// Chargement des images des dés et initialisation de leur position
	temp_name = (char*)malloc(100*sizeof(char));

	for( i=1; i<=6; i++ )
	{
		strcpy(temp_path, display->img_path);
		sprintf(temp_name,"die/die[%d].png",i);
		strcat(temp_path,temp_name);
		display->die[i-1] = IMG_Load(temp_path);
	}
	// Position du dé 1
	display->die1_position.x = 410;
	display->die1_position.y = 340;
	// Position du dé 2
	display->die2_position.x = 450;
	display->die2_position.y = 340;
	
	// Initialisation de la taille de la fenêtre
	display->window_width = 800;
	display->window_height = 600;
	
	// Initialisation du tableau de positions
	display->positions[0].x = 525;
	display->positions[0].y = 570;
	
	for( i=1 ; i<24 ; i++ )
	{
		display->positions[i].y = display->positions[i-1].y  ;
		if( i<6 )
			display->positions[i].x = display->positions[i-1].x - 38;
		else if ( i==6 )
			display->positions[i].x = display->positions[i-1].x - 74;
		else if( i<12 )
			display->positions[i].x = display->positions[i-1].x - 38;
		else if( i==12 )
		{
			display->positions[i].x = display->positions[i-1].x ;
			display->positions[i].y = 145;
		}
		else if( i<18 )
			display->positions[i].x = display->positions[i-1].x + 38;
		else if( i==18 )
			display->positions[i].x=display->positions[i-1].x + 74;
		else
			display->positions[i].x=display->positions[i-1].x + 38;
	}
	
	// Initialisation de la fenêtre
	display->screen = SDL_SetVideoMode(display->window_width, display->window_height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	
	if (display->screen == NULL) // Si l'ouverture a échoué, on écrit l'erreur et on arrête
	{
		fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	
	SDL_WM_SetCaption("Backgammon",NULL);
	
	// Chargement de SDL_ttf
	if(TTF_Init() == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}
   
   // Chargement de la police
   strcpy(temp_path, display->font_path);
	strcat(temp_path, "comic.ttf");
   display->font = TTF_OpenFont(temp_path,22);
	
	// Allocation de la structure de jeu et initialisation
	display->game = (SGame*)malloc(sizeof(SGame));
	display->game->player1_name = (char*)malloc(50*sizeof(char));
	display->game->player2_name = (char*)malloc(50*sizeof(char));
	display->game->player1_checker = GREEN;
	display->game->player2_checker = WHITE;
	display->game->withDouble = 1; // Par défaut, on joue avec le double
	display->game->scoreLimit = 5; // Par défaut, limite de score est de 
	
	free(temp_path); // On libère temp_path car on ne l'utilise plus
}

int Display_Menu(SDisplay *display, E_GameMode gameMode)
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
		Display_Message(display,display->game->player1_name,player1_name_position,c,0);
		
		checker_check_position.x = 255;
		checker_check_position.y = 275;
		SDL_BlitSurface(display->checked, NULL, display->screen, &checker_check_position);
		
		double_check_position.x = 255;
		double_check_position.y = 320;
		SDL_BlitSurface(display->checked, NULL, display->screen, &double_check_position);
	}
	else
	{
		Display_Message(display,display->game->player1_name,player1_name_position,c,0);
		Display_Message(display,display->game->player2_name,player2_name_position,c,0);
		
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
								/* Rechargement du menu */
								strcpy(temp_path, display->img_path);
								strcat(temp_path, background);
								display->background_menu = IMG_Load(temp_path);
								SDL_BlitSurface(display->background_menu, NULL, display->screen, &menu_position);
								
								/* Affichage du nom du joueur */
								Display_Message(display,display->game->player1_name,player1_name_position,c,0);
								
								/* Affichage de l'option double */
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
								/* Rechargement du menu */
								strcpy(temp_path, display->img_path);
								strcat(temp_path, background);
								display->background_menu = IMG_Load(temp_path);
								SDL_BlitSurface(display->background_menu, NULL, display->screen, &menu_position);
								
								/* Affichage du nom du joueur */
								Display_Message(display,display->game->player1_name,player1_name_position,c,0);
								
								/* Affichage de l'option double */
								SDL_BlitSurface(display->checked, NULL, display->screen, &double_check_position);
									
								/* Affichage de l'option couleur du pion */
								checker_check_position.x = 400;
								checker_check_position.y = 275;
								SDL_BlitSurface(display->checked, NULL, display->screen, &checker_check_position);
								
								display->game->player1_checker = WHITE;
								display->game->player2_checker = GREEN;
							}
							break;
							
						case 3 : // Clic sur oui
						
							/* Rechargement du menu */
							strcpy(temp_path, display->img_path);
							strcat(temp_path, background);
							display->background_menu = IMG_Load(temp_path);
							SDL_BlitSurface(display->background_menu, NULL, display->screen, &menu_position);
							
							/* Affichage du nom du joueur */	
							if(gameMode == HUMAN_AI)
							{
								Display_Message(display,display->game->player1_name,player1_name_position,c,0);
								SDL_BlitSurface(display->checked, NULL, display->screen, &checker_check_position);
							}
							else
							{ 
								Display_Message(display,display->game->player1_name,player1_name_position,c,0);
								Display_Message(display,display->game->player2_name,player2_name_position,c,0);
							}
								
							/* Affichage de l'option double */
							double_check_position.x = 255;
							double_check_position.y = 320;
							SDL_BlitSurface(display->checked, NULL, display->screen, &double_check_position);	
							display->game->withDouble = 1;
							break;
						
						case 4 : // Clic sur non
						
							/* Rechargement du menu */
							strcpy(temp_path, display->img_path);
							strcat(temp_path, background);
							display->background_menu = IMG_Load(temp_path);
							SDL_BlitSurface(display->background_menu, NULL, display->screen, &menu_position);
							
							/* Affichage du nom du joueur */	
							if(gameMode == HUMAN_AI)
							{
								Display_Message(display,display->game->player1_name,player1_name_position,c,0);
								SDL_BlitSurface(display->checked, NULL, display->screen, &checker_check_position);
							}
							else
							{ 
								Display_Message(display,display->game->player1_name,player1_name_position,c,0);
								Display_Message(display,display->game->player2_name,player2_name_position,c,0);
							}
								
							/* Affichage de l'option double */
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
		}
		SDL_Flip(display->screen);
	}
}

void Display_Message(SDisplay	*display, char	*message, SDL_Rect position, SDL_Color color, int box)
{
	if(box)
	{
		SDL_BlitSurface(display->msg_box, NULL, display->screen, &position);
		position.x= position.x + 30;
		position.y = position.y + 10;
	}
	SDL_Surface *msg = TTF_RenderText_Blended(display->font,message,color);
	SDL_BlitSurface(msg, NULL, display->screen, &position);
	SDL_FreeSurface(msg);
	SDL_Flip(display->screen);
}

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
	
void Display_Exit(SDisplay *display)
{
	int i;
	
	// Libération de toutes les surfaces utilisées
	SDL_FreeSurface(display->background);
	SDL_FreeSurface(display->background_menu);
	SDL_FreeSurface(display->checked);
	SDL_FreeSurface(display->white_checker);
	SDL_FreeSurface(display->green_checker);

	for( i=0; i<6; i++ )
	{
		SDL_FreeSurface(display->die[i]);
	}
	SDL_FreeSurface(display->screen);
	
	free(display->game->player1_name);
	free(display->game->player2_name);
	free(display->game);
	
	TTF_CloseFont(display->font);
	TTF_Quit();
	SDL_Quit();
}

void Display_Clear(SDisplay *display)
{
	// Rechargement du background
	char *temp_path;
	temp_path = (char*)malloc(100*sizeof(char));
	strcpy(temp_path, display->img_path);
	strcat(temp_path, "background.png");
	display->background = IMG_Load(temp_path);
	free(temp_path);
	
	SDL_BlitSurface(display->background, NULL, display->screen, &(display->background_position));
}

void Display_Score(SDisplay *display, SGameState *game)
{
	SDL_Rect pos;
	char *scoreP1 = (char*)malloc(3*(sizeof(char)));
	char *scoreP2 = (char*)malloc(3*(sizeof(char)));
	SDL_Color c = {255, 255, 255, 0};
	
	sprintf(scoreP1, "%d", game->score);
	sprintf(scoreP2, "%d", game->scoreP2);
	
	pos.x = 108;
	pos.y = 40;
	Display_Message(display, display->game->player1_name, pos, c, 0);
	pos.x = 295;
	Display_Message(display, scoreP1, pos, c, 0);
	pos.y = 85;
	Display_Message(display, scoreP2, pos, c, 0);
	pos.x = 108;
	Display_Message(display, display->game->player2_name, pos, c, 0);
}

void Display_Refresh(SDisplay *display, SGameState *game)
{
	Display_Clear(display);
	
	// Affichage du score
	Display_Score(display, game);
	
	// Affichage des pions
	Display_Checkers(display, game);
	
	// Affichage des dés
	//Display_Die(display,game);
}

void Launch_Die(SGameState *game)
{
	srand( time(NULL) ); // Initialisation du generateur de nombre aleatoire
	game->die1 = 1+(rand()/(float)RAND_MAX)*6; // Tirage aléatoire d'un chiffre entre 1 et 6
	game->die2 = 1+(rand()/(float)RAND_MAX)*6; // Tirage aléatoire d'un chiffre entre 1 et 6
	printf("Dé 1 : %d Dé 2 : %d\n",game->die1,game->die2);
}

void Display_Die(SDisplay *display,SGameState *game)
{
	SDL_Rect new_pos;
	
	if( game->die1 == game->die2 ) // Si c'est un double, on affiche 4 fois le dé
	{
		new_pos.x = (display->die1_position).x - 40;
		new_pos.y = (display->die1_position).y;
		SDL_BlitSurface(display->die[game->die1-1], NULL,display->screen, &new_pos);
		new_pos.x = (display->die1_position).x;
		SDL_BlitSurface(display->die[game->die1-1], NULL,display->screen, &new_pos);
		new_pos.x = (display->die1_position).x + 40;
		SDL_BlitSurface(display->die[game->die1-1], NULL,display->screen, &new_pos);
		new_pos.x = (display->die1_position).x + 80;
		SDL_BlitSurface(display->die[game->die1-1], NULL,display->screen, &new_pos);
	}
	else // Sinon simple affichage des deux dés
	{
		SDL_BlitSurface(display->die[game->die1-1], NULL,display->screen, &display->die1_position);
		SDL_BlitSurface(display->die[game->die2-1], NULL,display->screen, &display->die2_position);
	}
}
void Checker_Draw(SDisplay *display, SDL_Rect position, int player)
{
	if( player==0 )
	{
		SDL_BlitSurface(display->green_checker, NULL,display->screen, &position);
	}
	else
	{
		SDL_BlitSurface(display->white_checker, NULL,display->screen, &position);
	}
}

void Display_Checkers(SDisplay *display, SGameState *game)
{
	unsigned int i,j;
	SDL_Rect new_pos;
	
	for( i=0 ; i<28; i++ )
	{
		for( j=0; j<game->zones[i].nb_checkers; j++)
		{
			new_pos.x = display->positions[i].x;
			if( i<12)
				new_pos.y = display->positions[i].y - j*20;
			else
				new_pos.y = display->positions[i].y + j*20;
				
			Checker_Draw(display,new_pos,game->zones[i].player);
		}
	}
}

SGameState* Game_Init()
{
	SGameState *game;
	int i;
	
	game = (SGameState*)malloc(sizeof(SGameState));
	
	game->score = 0;
	game->scoreP2 = 0;
	
	for(i=0;i<28;i++) game->zones[i].nb_checkers=0;
	
	game->zones[0].player=EPlayer1;
	game->zones[0].nb_checkers=2;
	game->zones[23].player=EPlayer2;
	game->zones[23].nb_checkers=2;
	
	game->zones[11].player=EPlayer1;
	game->zones[11].nb_checkers=5;
	game->zones[12].player=EPlayer2;
	game->zones[12].nb_checkers=5;
	
	game->zones[16].player=EPlayer1;
	game->zones[16].nb_checkers=3;
	game->zones[7].player=EPlayer2;
	game->zones[7].nb_checkers=3;
	
	game->zones[18].player=EPlayer1;
	game->zones[18].nb_checkers=5;
	game->zones[5].player=EPlayer2;
	game->zones[5].nb_checkers=5;
	
	return game;
}

void Checker_Move(SDisplay *display, SGameState* game, SMove *move)
{
	/* Calcul de la droite passant par les deux zones */
	int pos_src_x;
	int pos_src_y;
	int pos_dest_x;
	int pos_dest_y;
	
	pos_src_x = display->positions[move->src_point-1].x;
	
	if(move->src_point>12)
	{
		pos_src_y =  display->positions[move->src_point-1].y + 20*(game->zones[move->src_point-1].nb_checkers);
	}
	else
	{
		pos_src_y =  display->positions[move->src_point-1].y - 20*(game->zones[move->src_point-1].nb_checkers);
	}
	
	pos_dest_x = display->positions[move->dest_point-1].x;
	
	if(move->dest_point>12)
	{
		pos_dest_y = display->positions[move->dest_point-1].y + 20*(game->zones[move->dest_point-1].nb_checkers);
	}
	else
	{
		pos_dest_y = display->positions[move->dest_point-1].y - 20*(game->zones[move->dest_point-1].nb_checkers);
	}
	
	double a = (double)(pos_src_y-pos_dest_y)/(pos_src_x - pos_dest_x);
	double b = pos_dest_y - a*pos_dest_x;
	
	/*printf("SRC = fleche %d x=%d y=%d\n",move->src_point,pos_src_x,pos_src_y);
	printf("DEST = fleche %d x=%d y=%d\n",move->dest_point,pos_dest_x,pos_dest_y);
	printf("a=%f b=%f\n",a,b);*/
	
	int pas = 1; // Vitesse de deplacement du pion 1=lentement 8=rapide
	int numCas;
	
	if(move->dest_point<13 && move->src_point<13) // Si les deux pions sont situés sur la barre du bas
	{
		if((move->dest_point-1)<=(move->src_point-1)) // Si le déplacement s'effectue vers une flèche à droite
			numCas=1;
		else // Sinon le déplacement s'effectue vers une flèche à gauche
		{
			numCas=2;
			pas = -pas;
		}
	}
	else if(move->dest_point>12 && move->src_point>12) // Si les deux pions sont situés sur la barre du haut
	{
		if((move->dest_point-1)>=(move->src_point-1)) // Si le déplacement s'effectue vers une flèche à droite
			numCas=3;
		else // Sinon le déplacement s'effectue vers une flèche à gauche
		{
			numCas=4;
			pas = -pas;
		}
	}
	else if(a>=0 && (move->dest_point-1)<=(move->src_point-1)) // src : barre du haut / dest : barre du bas / coef>0
	{
		numCas=5;
	}else if(a<=0 && (move->dest_point-1)>=(move->src_point-1)) // src : barre du bas / dest : barre du haut / coef<0
	{
		numCas=6;
	}
	else if(a>=0 && (move->dest_point-1)>=(move->src_point-1)) // src : barre du bas / dest : barre du haut / coef>0
	{
		numCas=7;
		pas = -pas;
	}
	else // src : barre du haut / dest : barre du bas / coef<0
	{
		numCas=8;
		pas = -pas;
	}
	printf("Cas n°%d\n",numCas);
	
	SDL_Rect new_pos;
	new_pos.x = pos_src_x;
	new_pos.y = pos_src_y;
	int depassement = 0;
	game->zones[move->src_point-1].nb_checkers--;
	while( !depassement )
	{
		new_pos.x = new_pos.x + pas;
		new_pos.y = (int)(a*new_pos.x + b);
		Display_Refresh(display,game);
		Checker_Draw(display,new_pos,game->zones[move->src_point-1].player);
		SDL_Flip(display->screen);
		switch(numCas) // Suivant le cas de figure, on vérifie si on ne dépasse pas la flèche destinatrice
		{
			case 1:
				if(new_pos.x>(pos_dest_x-(2*pas))) // On s'arrete à chaque fois 2*pas pixel avant par souci d'esthétique
					depassement = 1;
				break;
			case 2:
				if(new_pos.x<(pos_dest_x-(2*pas)))
					depassement = 1;
				break;
			case 3:
				if(new_pos.x>(pos_dest_x-(2*pas)))
					depassement = 1;
				break;
			case 4:
				if(new_pos.x<(pos_dest_x-(2*pas)))
					depassement = 1;
				break;
			case 5:
				if(new_pos.x>(pos_dest_x-(2*pas)))
					depassement = 1;
				break;
			case 6:
				if(new_pos.x>(pos_dest_x-(2*pas)))
					depassement = 1;
				break;
			case 7:
				if(new_pos.x<(pos_dest_x-(2*pas)))
					depassement = 1;
				break;
			case 8:
				if(new_pos.x<(pos_dest_x-(2*pas)))
					depassement = 1;
				break;
			default:
				depassement = 1;
				break;
		}
	}
	game->zones[move->dest_point-1].nb_checkers++;
	game->zones[move->dest_point-1].player = game->zones[move->src_point-1].player;
}

void draw_possibility(SDisplay *display,EPosition numFleche)
{
	SDL_Rect new_pos = display->positions[numFleche-1];
	new_pos.x = new_pos.x -5;
	if(numFleche<13)
		new_pos.y = new_pos.y - 140;
		
	SDL_BlitSurface(display->possibility, NULL,display->screen, &new_pos);
	
}

void Display_Possibilities(SDisplay *display, SGameState *game, EPlayer player)
{
	int i=0;
	
	if(game->die1 != game->die2) // Si dés différents
	{
		for( i=0; i<24; i++) // Pour chaque flèche
		{
			if(game->zones[i].nb_checkers>0 && game->zones[i].player==player)
			{
				if( (i+game->die1)<24 && (game->zones[i+game->die1].nb_checkers==0 || game->zones[i+game->die1].player==player) )
				{
					printf("Possibilite sur fleche %d avec de1 et pion sur fleche %d\n",i+1+game->die1,i+1);
					draw_possibility(display,i+1+game->die1);
				}
				if( (i+game->die2)<24 && (game->zones[i+game->die2].nb_checkers==0 || game->zones[i+game->die2].player==player) )
				{
					printf("Possibilite sur fleche %d avec de2 et pion sur fleche %d\n",i+1+game->die2,i+1);
					draw_possibility(display,i+1+game->die2);
				}
				if( (i+game->die1+game->die2)<24 && (game->zones[i+game->die1+game->die2].nb_checkers==0 || game->zones[i+game->die1+game->die2].player==player) )
				{
					printf("Possibilite sur fleche %d avec de1+de2 et pion sur fleche %d\n",i+1+game->die1+game->die2,i+1);
					draw_possibility(display,i+1+game->die1+game->die2);
				}
			}
		}
	}
	SDL_Flip(display->screen);
}

int Quit_Zone(int x, int y)
{

	if( (x >= 635 && x <= 795) && (y >=244 && y <= 325) )
	{
		return 1;
	}
	return 0;
}
