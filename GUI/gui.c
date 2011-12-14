#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h> // Pour utiliser des formats d'image autre que bmp
#include "gui.h"
#include "backgammon.h"


/*Fonction d'initialisation de la SDL*/
void initSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de la SDL");
		exit(EXIT_FAILURE);
	}
}

/*Fonction qui quitte la SDL*/
void quitSDL(SWindow *w)
{
	SDL_FreeSurface(w->background);
	//SDL_FreeSurface(w->checkers);
	free(w);
	SDL_Quit();
}

/*Fonction qui dessine les pions sur le jeu*/
void drawGame(SWindow *w, SZone zones[28])
{
	int i,j;
	SDL_Rect p;
	p.x=525;
	p.y=435;
	double tmp;
	
	w->checkers = SDL_CreateRGBSurface(SDL_HWSURFACE, 640, 480, 32, 0, 0, 0, 0);
	
	for(i=0;i<24;i++)
	{
		tmp=p.y;
		for(j=0;j<zones[i].nb_checkers;j++)
		{
			drawChecker(w,&p,checkerColor(zones[i].player));
			if(i<12)
				p.y = p.y - 20;
			else
				p.y = p.y + 20;
		}
		p.y=tmp;
		if(i<6)
			p.x=p.x - 38;
		else if (i==6)
			p.x=p.x - 74;
		else if(i<11)
			p.x = p.x - 38;
		else if(i==11)
			p.y = 8;
		else if(i<17)
			p.x=p.x + 38;
		else if(i==17)
			p.x=p.x+74;
		else
			p.x=p.x+38;
	}
}
/*Fonction qui renvoie la couleur associée au joueur*/
SColor checkerColor(EPlayer e)
{
	SColor c;
	if(e==EPlayer1)
	{
		c.r=0;
		c.g=255;
		c.b=0;
	}
	else
	{
		c.r=255;
		c.g=255;
		c.b=255;
	}
	return c;
}
/* Fonction qui dessine un pion à une position donnée */
void drawChecker(SWindow *w,SDL_Rect *position,SColor c)
{
	SDL_Surface *checker;
	checker = SDL_CreateRGBSurface(SDL_HWSURFACE, 15, 15, 32, 0, 0, 0, 0);
	SDL_FillRect(checker, NULL, SDL_MapRGB(checker->format, c.r, c.g, c.b));
	SDL_BlitSurface(checker, NULL,w->background, position);
	SDL_FreeSurface(checker);
}
/*Fonction d'initialisation du plateau de jeu*/
void initPartie(SZone *zones)
{
	int i,j;
	for(i=0;i<28;i++) zones[i].nb_checkers=0;
	
	zones[0].player=EPlayer1;
	zones[0].nb_checkers=2;
	zones[23].player=EPlayer2;
	zones[23].nb_checkers=2;
	
	zones[11].player=EPlayer1;
	zones[11].nb_checkers=5;
	zones[12].player=EPlayer2;
	zones[12].nb_checkers=5;
	
	zones[16].player=EPlayer1;
	zones[16].nb_checkers=3;
	zones[7].player=EPlayer2;
	zones[7].nb_checkers=3;
	
	zones[18].player=EPlayer1;
	zones[18].nb_checkers=5;
	zones[5].player=EPlayer2;
	zones[5].nb_checkers=5;
}	


int main(int argc, char **argv)
{
	/**********************/
	int quit=0;
	SWindow *w;
	SDL_Event event;
	SZone zones[28];
	/*********************/
	initPartie(zones);
	initSDL();
	SDL_WM_SetIcon(IMG_Load("icon.png"),NULL);
	
	w = (SWindow*)malloc(sizeof(SWindow));
	
	w->screen = SDL_SetVideoMode(780, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (w->screen == NULL) // Si l'ouverture a échoué, on écrit l'erreur et on arrête
    {
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
	SDL_WM_SetCaption("Backgammon",NULL);
	int i=0;
	while (!quit) // Boucle principale
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				quit=1;
		}
		w->background = IMG_Load("background.png");
		if(i==1) zones[0].nb_checkers--;
		drawGame(w,zones);
		SDL_BlitSurface(w->background, NULL, w->screen, NULL);
		i++;
		SDL_Flip(w->screen);
		
	}
	
	quitSDL(w);
	return EXIT_SUCCESS;
}
