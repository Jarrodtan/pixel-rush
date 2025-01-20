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
void GameStateLevel1PlatformLoad(void) {

}

/******************************************************************************/
/*!
	Initialize all varaibles and funtions
*/
/******************************************************************************/
void GameStateLevel1PlatformInit(void) {
	AESysSetWindowTitle("Level 1");
}

/******************************************************************************/
/*!
	Update function of the state, calculate position and logics for the objects
*/
/******************************************************************************/
void GameStateLevel1PlatformUpdate(void) {
	if (AEInputCheckTriggered(VK_ESCAPE)) {
		g_GameStateNext = GS_QUIT;
	}
}

/******************************************************************************/
/*!
	Draws all the objects, particle and text for the game
*/
/******************************************************************************/
void GameStateLevel1PlatformDraw(void) {

}

/******************************************************************************/
/*!
	Destroy all objects when function is called
*/
/******************************************************************************/
void GameStateLevel1PlatformFree(void) {

}

/******************************************************************************/
/*!
	Free all memory when function is called
*/
/******************************************************************************/
void GameStateLevel1PlatformUnload(void) {

}

