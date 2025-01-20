/******************************************************************************/
/*!
	Include Files
*/
/******************************************************************************/
#include "main.h"

/******************************************************************************/
/*!
	Load meshes,particle,font and map. Allocate memory for game object
*/
/******************************************************************************/
void GameStateLevel2PlatformLoad(void) {

}

/******************************************************************************/
/*!
	Initialize all varaibles and funtions
*/
/******************************************************************************/
void GameStateLevel2PlatformInit(void) {
	AESysSetWindowTitle("Level 2");
}

/******************************************************************************/
/*!
	Update function of the state, calculate position and logics for the objects
*/
/******************************************************************************/
void GameStateLevel2PlatformUpdate(void) {
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
void GameStateLevel2PlatformDraw(void) {

}

/******************************************************************************/
/*!
	Destroy all objects when function is called
*/
/******************************************************************************/
void GameStateLevel2PlatformFree(void) {

}

/******************************************************************************/
/*!
	Free all memory when function is called
*/
/******************************************************************************/
void GameStateLevel2PlatformUnload(void) {

}