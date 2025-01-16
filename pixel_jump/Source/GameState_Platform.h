#ifndef GAME_STATE_PLATFORM_H
#define GAME_STATE_PLATFORM_H

/******************************************************************************/
/*!
	Load meshes,particle,font and map. Allocate memory for game object
*/
/******************************************************************************/
void GameStatePlatformLoad(void);
/******************************************************************************/
/*!
	Initialize all varaibles and funtions
*/
/******************************************************************************/
void GameStatePlatformInit(void);
/******************************************************************************/
/*!
	Update function of the state, calculate position and logics for the objects
*/
/******************************************************************************/
void GameStatePlatformUpdate(void);
/******************************************************************************/
/*!
	Draws all the objects, particle and text for the game
*/
/******************************************************************************/
void GameStatePlatformDraw(void);
/******************************************************************************/
/*!
	Destroy all objects when function is called
*/
/******************************************************************************/
void GameStatePlatformFree(void);
/******************************************************************************/
/*!
	Free all memory when function is called
*/
/******************************************************************************/
void GameStatePlatformUnload(void);

// ---------------------------------------------------------------------------



#endif // GAME_STATE_PLATFORM_H