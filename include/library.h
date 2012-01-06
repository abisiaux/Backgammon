
#ifndef LIBRARY_H
	#define LIBRARY_H
	
#include "backgammon.h"

typedef struct
{
	void* AI_Handle;
	pfInitLibrary AI_InitLibrary;
	pfStartMatch AI_StartMatch;
	pfEndMatch AI_EndMatch;
	pfStartGame AI_StartGame;
	pfEndGame AI_EndGame;
	pfDoubleStack AI_DoubleStack;
	pfTakeDouble AI_TakeDouble;
	pfMakeDecision AI_MakeDecision;
} S_AI_Functions;

/* Différents modes de jeu
		1) Humain contre Humain
		2) Humain contre AI
		3) AI contre AI
		4) Invalide
*/
typedef enum
{
	HUMAN_HUMAN,
	HUMAN_AI,
	AI_AI,
	ERROR=-1
}E_GameMode;


S_AI_Functions* Init_S_AI(void);

void Free_S_AI( S_AI_Functions* ai_struct);

int Load_API( char* path, S_AI_Functions* ai_struct, int ind);

E_GameMode Check_Args( int argc, char** argv, S_AI_Functions* ai_struct);

#endif
