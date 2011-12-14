#include "backgammon.h"
#include <SDL/SDL.h>

#ifndef GUI_H
	#define gui_H

struct Color{
	int r;
	int g;
	int b;
};

struct Window
{
	SDL_Surface *screen;
	SDL_Surface *background;
	SDL_Surface *checkers;
};

typedef struct Window SWindow;
typedef struct Color SColor;

void initSDL();
void quitSDL(SWindow *w);
SWindow* createWindow();
void drawGame(SWindow *w, SZone zones[28]);
void drawChecker(SWindow *w,SDL_Rect *position,SColor c);
SColor checkerColor(EPlayer e);
void initPartie(SZone *zones);

#endif


