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
void GameStateWinLoad(void) {

}

/******************************************************************************/
/*!
	Initialize all varaibles and funtions
*/
/******************************************************************************/
void GameStateWinInit(void) {
	AESysSetWindowTitle("Win");
}

/******************************************************************************/
/*!
	Update function of the state, calculate position and logics for the objects
*/
/******************************************************************************/
void GameStateWinUpdate(void) {
	if (AEInputCheckTriggered(AEVK_Q)) {
		GameStateManager::g_GameStateNext = States::GS_MAINMENU;
	}
	else if (AEInputCheckTriggered(AEVK_1)) {
		GameStateManager::g_GameStateNext = States::GS_PLATFORM_LVL1;
	}
	else if (AEInputCheckTriggered(AEVK_2)) {
		GameStateManager::g_GameStateNext = States::GS_PLATFORM_LVL2;
	}
	else if (AEInputCheckTriggered(AEVK_3)) {
		GameStateManager::g_GameStateNext = States::GS_WIN;
	}
	else if (AEInputCheckTriggered(AEVK_4)) {
		GameStateManager::g_GameStateNext = States::GS_LOSE;
	}
}

/******************************************************************************/
/*!
	Draws all the objects, particle and text for the game
*/
/******************************************************************************/
void GameStateWinDraw(void) {

}

/******************************************************************************/
/*!
	Destroy all objects when function is called
*/
/******************************************************************************/
void GameStateWinFree(void) {

}

/******************************************************************************/
/*!
	Free all memory when function is called
*/
/******************************************************************************/
void GameStateWinUnload(void) {

}
