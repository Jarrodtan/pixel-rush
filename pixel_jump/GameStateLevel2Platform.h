#ifndef GAME_STATE_PLATFORM_LEVEL2_H
#define GAME_STATE_PLATFORM_LEVEL2_H

/******************************************************************************/
/*!
	Load meshes,particle,font and map. Allocate memory for game object
*/
/******************************************************************************/
void GameStateLevel2PlatformLoad(void);
/******************************************************************************/
/*!
	Initialize all varaibles and funtions
*/
/******************************************************************************/
void GameStateLevel2PlatformInit(void);
/******************************************************************************/
/*!
	Update function of the state, calculate position and logics for the objects
*/
/******************************************************************************/
void GameStateLevel2PlatformUpdate(void);
/******************************************************************************/
/*!
	Draws all the objects, particle and text for the game
*/
/******************************************************************************/
void GameStateLevel2PlatformDraw(void);
/******************************************************************************/
/*!
	Destroy all objects when function is called
*/
/******************************************************************************/
void GameStateLevel2PlatformFree(void);
/******************************************************************************/
/*!
	Free all memory when function is called
*/
/******************************************************************************/
void GameStateLevel2PlatformUnload(void);

// ---------------------------------------------------------------------------



#endif // GAME_STATE_PLATFORM_LEVEL1_H