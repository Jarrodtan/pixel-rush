/******************************************************************************/
/*!
	Include Files and static variables
*/
/******************************************************************************/
#include "main.h"

namespace {
	s32 i_PlayerLives;
	s32 i_PlayerInitialX;
	s32 i_PlayerInitialY;
	s32 i_TotalCoins;

	// List of original objects
	GameObject* p_GameObjectList = nullptr;
	u32 u_GameObjectNum;

	// List of object instances
	GameObjectInstance* p_GameObjectInstanceList = nullptr;
	u32 u_GameObjectInstanceNum;

	// Binary map data
	s32** p_MapData;
	s32** p_BinaryCollisionArray;
	s32 i_BINARY_MAP_WIDTH;
	s32 i_BINARY_MAP_HEIGHT;

	GameObjectInstance* p_BlackCell;
	GameObjectInstance* p_WhiteCell;
	AEMtx33 MapTransform;

	// Pointer to player object
	GameObjectInstance* p_Player;

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

	AE_ASSERT_MESG(::p_GameObjectList, "Fail to calloc");
}

/******************************************************************************/
/*!
	Initialize all varaibles and funtions
*/
/******************************************************************************/
void GameStateLevel1PlatformInit(void) {
	AESysSetWindowTitle("Level 1");

	AEVec2 scale = { 1.0f, 1.0f };
	p_BlackCell = gameObjectInstanceCreate(::p_GameObjectList, ::p_GameObjectInstanceList, TYPE_OBJECT_EMPTY, &scale, 0, 0, 0.0f, STATE_NONE); // create empty object instance
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
	// kill all objects in the list
	for (u32 i{}; i < GAME_OBJECT_INST_NUM_MAX; ++i) {
		gameObjectInstanceDestroy(::p_GameObjectInstanceList + i);
	}
}

/******************************************************************************/
/*!
	Free all memory when function is called
*/
/******************************************************************************/
void GameStateLevel1PlatformUnload(void) {
	// free all created mesh
	for (u32 i{}; i < ::u_GameObjectNum; ++i) {
		AEGfxMeshFree(::p_GameObjectList[i].p_Mesh);
	}

	delete[] ::p_GameObjectList;
	delete[] ::p_GameObjectInstanceList;

	FreeMapData(::i_BINARY_MAP_WIDTH, ::p_MapData, ::p_BinaryCollisionArray);
}

