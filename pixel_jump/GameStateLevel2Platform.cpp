/******************************************************************************/
/*!
	Include Files and external variables
*/
/******************************************************************************/
#include "main.h"

static GameObject* p_GameObjectList = nullptr;
static GameObjectInstance* p_GameObjectInstanceList = nullptr;

//-----------------------------------------------------------------------------/

/******************************************************************************/
/*!
	Load meshes,particle,font and map. Allocate memory for game object
*/
/******************************************************************************/
void GameStateLevel2PlatformLoad(void) {
	// allocate memory for the object type and mesh
	p_GameObjectList = (GameObject*)calloc(GAME_OBJECT_NUM_MAX, sizeof(GameObject));
	// allocate memory for the object instance
	p_GameObjectInstanceList = (GameObjectInstance*)calloc(GAME_OBJECT_INST_NUM_MAX, sizeof(GameObjectInstance));

	AE_ASSERT_MESG(p_GameObjectList, "Fail to calloc");
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