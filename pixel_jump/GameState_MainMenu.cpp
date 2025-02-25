/******************************************************************************/
/*!
	Include Files
*/
/******************************************************************************/
#include "main.h"

/******************************************************************************/
/*!
	"Load" function of this state
*/
/******************************************************************************/
void GameStateMainMenuLoad(void) {

}

/******************************************************************************/
/*!
	"Initialize" function of this state
*/
/******************************************************************************/
void GameStateMainMenuInit(void) {
	AESysSetWindowTitle("Main Menu");
	
}

/******************************************************************************/
/*!
	"Update" function of this state
*/
/******************************************************************************/
void GameStateMainMenuUpdate(void) {
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
	Draw all the meshes and print out all the text in the game state
*/
/******************************************************************************/
void GameStateMainMenuDraw(void) {

}

/******************************************************************************/
/*!
	Destroy all objects in the game
*/
/******************************************************************************/
void GameStateMainMenuFree(void) {

}

/******************************************************************************/
/*!
	Free all memory
*/
/******************************************************************************/
void GameStateMainMenuUnload(void) {

}

