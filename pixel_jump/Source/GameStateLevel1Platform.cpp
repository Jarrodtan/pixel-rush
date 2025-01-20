/******************************************************************************/
/*!
	Include Files and external variables
*/
/******************************************************************************/
#include "main.h"

namespace {
	GameObject* p_GameObjectList = nullptr;
	GameObjectInstance* p_GameObjectInstanceList = nullptr;
	GameObjectInstance* p_BlackCell;
}

/******************************************************************************/
/*!
	Load meshes,particle,font and map. Allocate memory for game object
*/
/******************************************************************************/
void GameStateLevel1PlatformLoad(void) {
	// allocate memory for the object type and mesh
	::p_GameObjectList = (GameObject*)calloc(GAME_OBJECT_NUM_MAX, sizeof(GameObject));
	// allocate memory for the object instance
	::p_GameObjectInstanceList = (GameObjectInstance*)calloc(GAME_OBJECT_INST_NUM_MAX, sizeof(GameObjectInstance));

	AE_ASSERT_MESG(::p_GameObjectList, "Faile to calloc");
}

/******************************************************************************/
/*!
	Initialize all varaibles and funtions
*/
/******************************************************************************/
void GameStateLevel1PlatformInit(void) {
	AESysSetWindowTitle("Level 1");

	//Create an object instance representing the black cell.
	//This object instance should not be visible. When rendering the grid cells, each time we have
	//a non collision cell, we position this instance in the correct location and then we render it
	AEVec2 scale = { 1.0f, 1.0f };
	p_BlackCell = GameObjectInstance::gameObjectInstanceCreate(TYPE_OBJECT_EMPTY, &scale, 0, 0, 0.0f, STATE_NONE); // create empty object instance
	p_BlackCell->u_Flag ^= FLAG_VISIBLE;
	p_BlackCell->u_Flag |= FLAG_NON_COLLIDABLE;
}

/******************************************************************************/
/*!
	Update function of the state, calculate position and logics for the objects
*/
/******************************************************************************/
void GameStateLevel1PlatformUpdate(void) {
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

