/******************************************************************************/
/*!
	Load meshes,particle,font and map. Allocate memory for game object
*/
/******************************************************************************/
#include "main.h"

/******************************************************************************/
/*!
	Load meshes,particle,font and map. Allocate memory for game object
*/
/******************************************************************************/
void GameStateLoseLoad(void) {
	AESysSetWindowTitle("Lose");
}

/******************************************************************************/
/*!
	Initialize all varaibles and funtions
*/
/******************************************************************************/
void GameStateLoseInit(void) {

}

/******************************************************************************/
/*!
	Update function of the state, calculate position and logics for the objects
*/
/******************************************************************************/
void GameStateLoseUpdate(void) {
	if (AEInputCheckTriggered(AEVK_Q)) {
		g_GameStateNext = GS_MAINMENU;
	}
	else if (AEInputCheckTriggered(AEVK_1)) {
		g_GameStateNext = GS_PLATFORM_LVL1;
	}
	else if (AEInputCheckTriggered(AEVK_2)) {
		g_GameStateNext = GS_PLATFORM_LVL2;
	}
	else if (AEInputCheckTriggered(AEVK_3)) {
		g_GameStateNext = GS_WIN;
	}
	else if (AEInputCheckTriggered(AEVK_4)) {
		g_GameStateNext = GS_LOSE;
	}
}

/******************************************************************************/
/*!
	Draws all the objects, particle and text for the game
*/
/******************************************************************************/
void GameStateLoseDraw(void) {

}

/******************************************************************************/
/*!
	Destroy all objects when function is called
*/
/******************************************************************************/
void GameStateLoseFree(void) {

}

/******************************************************************************/
/*!
	Free all memory when function is called
*/
/******************************************************************************/
void GameStateLoseUnload(void) {

}
