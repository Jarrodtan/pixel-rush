#ifndef GAME_STATE_PLATFORM_LEVEL1_H
#define GAME_STATE_PLATFORM_LEVEL1_H

/******************************************************************************/
/*!
	Load meshes,particle,font and map. Allocate memory for game object
*/
/******************************************************************************/
void GameStateLevel1PlatformLoad(void);
/******************************************************************************/
/*!
	Initialize all varaibles and funtions
*/
/******************************************************************************/
void GameStateLevel1PlatformInit(void);
/******************************************************************************/
/*!
	Update function of the state, calculate position and logics for the objects
*/
/******************************************************************************/
void GameStateLevel1PlatformUpdate(void);
/******************************************************************************/
/*!
	Draws all the objects, particle and text for the game
*/
/******************************************************************************/
void GameStateLevel1PlatformDraw(void);
/******************************************************************************/
/*!
	Destroy all objects when function is called
*/
/******************************************************************************/
void GameStateLevel1PlatformFree(void);
/******************************************************************************/
/*!
	Free all memory when function is called
*/
/******************************************************************************/
void GameStateLevel1PlatformUnload(void);

// ---------------------------------------------------------------------------



#endif // GAME_STATE_PLATFORM_LEVEL1_H