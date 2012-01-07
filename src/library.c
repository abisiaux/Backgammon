#include<stdio.h>
#include<stdlib.h>
#include<dlfcn.h>

#include "../include/library.h"

S_AI_Functions* Init_S_AI(void)
{
	S_AI_Functions* ai_struct;
	
	ai_struct = (S_AI_Functions*)malloc(2*sizeof(S_AI_Functions));
	
	int i;
	
	for(i=0; i<2; i++)
	{
		ai_struct[i].AI_Handle = NULL;
		ai_struct[i].AI_InitLibrary = NULL;
		ai_struct[i].AI_StartMatch = NULL;
		ai_struct[i].AI_EndMatch = NULL;
		ai_struct[i].AI_StartGame = NULL;
		ai_struct[i].AI_EndGame = NULL;
		ai_struct[i].AI_EndMatch = NULL;
		ai_struct[i].AI_DoubleStack = NULL;
		ai_struct[i].AI_TakeDouble = NULL;
		ai_struct[i].AI_MakeDecision = NULL;
	}
	
	return ai_struct;
}

void Free_S_AI( S_AI_Functions* ai_struct)
{
	if (ai_struct[0].AI_Handle != NULL) // Si la librairie a été chargée
		dlclose(ai_struct[0].AI_Handle);
		
	if (ai_struct[1].AI_Handle != NULL) // Si la librairie a été chargée
		dlclose(ai_struct[1].AI_Handle);
	
	free(ai_struct);
}

int Load_API( char* path, S_AI_Functions* ai_struct, int ind)
{
	if((ai_struct[ind].AI_Handle = dlopen(path,RTLD_LAZY)) == NULL)
	{
		fprintf(stderr, "Erreur lors du chargement de l'API\nInformations sur l'erreur :\n%s\n\n",dlerror());
		return 0;
	}
	
	 if ((ai_struct[ind].AI_InitLibrary = (pfInitLibrary)dlsym(ai_struct[ind].AI_Handle, "InitLibrary")) == NULL) return 0;
    if ((ai_struct[ind].AI_StartMatch = (pfStartMatch)dlsym(ai_struct[ind].AI_Handle, "StartMatch")) == NULL) return 0;
    if ((ai_struct[ind].AI_StartGame = (pfStartGame)dlsym(ai_struct[ind].AI_Handle, "StartGame")) == NULL) return 0;
    if ((ai_struct[ind].AI_EndGame = (pfEndGame)dlsym(ai_struct[ind].AI_Handle, "EndGame")) == NULL) return 0;
    if ((ai_struct[ind].AI_EndMatch = (pfEndMatch)dlsym(ai_struct[ind].AI_Handle, "EndMatch")) == NULL) return 0;
    if ((ai_struct[ind].AI_DoubleStack = (pfDoubleStack)dlsym(ai_struct[ind].AI_Handle, "DoubleStack")) == NULL) return 0;
    if ((ai_struct[ind].AI_TakeDouble = (pfTakeDouble)dlsym(ai_struct[ind].AI_Handle, "TakeDouble")) == NULL) return 0;
    if ((ai_struct[ind].AI_MakeDecision = (pfMakeDecision)dlsym(ai_struct[ind].AI_Handle, "MakeDecision")) == NULL) return 0;

    return 1;
}

E_GameMode Check_Args( int argc, char** argv, S_AI_Functions* ai_struct)
{

	switch(argc)
	{
		case 1 :
			return HUMAN_HUMAN;
		case 2 :
			if( Load_API(argv[1], ai_struct, 0) )
				return HUMAN_AI;
			else
				return ERROR;
		case 3 :
			if( Load_API(argv[1], ai_struct, 0) && Load_API(argv[2], ai_struct, 0) )
				return AI_AI;
			else
				return ERROR;
		default :
			return ERROR;
	}
}
