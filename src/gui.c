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


void display_init(SDisplay *display)
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
	display->img_path = "ressources/pictures/";
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
	
	// Chargement de l'image du bouton jouer
	/*strcpy(temp_path, display->img_path);
	strcat(temp_path, "button_play.png");
	display->button_play = IMG_Load(temp_path);*/
	
	// Chargement des images deux dés et initialisation de leur position
	temp_name = (char*)malloc(100*sizeof(char));

	for( i=1; i<=6; i++ )
	{
		strcpy(temp_path, display->img_path);
		sprintf(temp_name,"die/die[%d].png",i);
		strcat(temp_path,temp_name);
		display->die1[i-1] = IMG_Load(temp_path);
		display->die2[i-1] = IMG_Load(temp_path);
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
	free(temp_path); // On libère temp_path car on ne l'utilise plus
}


void display_exit(SDisplay *display)
{
	int i;
	
	// Libération de toutes les surfaces utilisées
	SDL_FreeSurface(display->background);
	SDL_FreeSurface(display->white_checker);
	SDL_FreeSurface(display->green_checker);
	//SDL_FreeSurface(display->button_play);

	for( i=0; i<6; i++ )
	{
		SDL_FreeSurface(display->die1[i]);
		SDL_FreeSurface(display->die2[i]);
	}
	SDL_FreeSurface(display->screen);
	
	SDL_Quit();
}

void display_clear(SDisplay *display)
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
	
void display_refresh(SDisplay *display, SGameState *game)
{
	display_clear(display);
	
	// Affichage des pions
	display_checkers(display, game);
	
	// Affichage des dés
	display_die(display,game);
}

void launch_die(SGameState *game)
{
	srand( time(NULL) ); // Initialisation du generateur de nombre aleatoire
	game->die1 = 1+(rand()/(float)RAND_MAX)*6; // Tirage aléatoire d'un chiffre entre 1 et 6
	game->die2 = 1+(rand()/(float)RAND_MAX)*6; // Tirage aléatoire d'un chiffre entre 1 et 6
	printf("Dé 1 : %d Dé 2 : %d\n",game->die1,game->die2);
}
void display_die(SDisplay *display,SGameState *game)
{
	SDL_Rect new_pos;
	
	if( game->die1 == game->die2 ) // Si c'est un double, on affiche 4 fois le dé
	{
		new_pos.x = (display->die1_position).x - 40;
		new_pos.y = (display->die1_position).y;
		SDL_BlitSurface(display->die1[game->die1-1], NULL,display->screen, &new_pos);
		new_pos.x = (display->die1_position).x;
		SDL_BlitSurface(display->die1[game->die1-1], NULL,display->screen, &new_pos);
		new_pos.x = (display->die1_position).x + 40;
		SDL_BlitSurface(display->die1[game->die1-1], NULL,display->screen, &new_pos);
		new_pos.x = (display->die1_position).x + 80;
		SDL_BlitSurface(display->die1[game->die1-1], NULL,display->screen, &new_pos);
	}
	else // Sinon simple affichage des deux dés
	{
		SDL_BlitSurface(display->die1[game->die1-1], NULL,display->screen, &display->die1_position);
		SDL_BlitSurface(display->die2[game->die2-1], NULL,display->screen, &display->die2_position);
	}
}
void draw_checker(SDisplay *display, SDL_Rect position, int player)
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

void display_checkers(SDisplay *display, SGameState *game)
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
				
			draw_checker(display,new_pos,game->zones[i].player);
		}
	}
}

SGameState* initPartie()
{
	SGameState *game;
	int i;
	
	game = (SGameState*)malloc(sizeof(SGameState));
	
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

void checker_move(SDisplay *display, SGameState* game, SMove *move)
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
		display_refresh(display,game);
		draw_checker(display,new_pos,game->zones[move->src_point-1].player);
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

void display_possibilities(SDisplay *display, SGameState *game, EPlayer player)
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
