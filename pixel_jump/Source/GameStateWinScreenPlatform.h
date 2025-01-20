#ifndef GAME_STATE_PLATFORM_LEVEL_WIN_H
#define GAME_STATE_PLATFORM_LEVEL_WIN_H

/******************************************************************************/
/*!
	Load meshes,particle,font and map. Allocate memory for game object
*/
/******************************************************************************/
void GameStateWinLoad(void);
/******************************************************************************/
/*!
	Initialize all varaibles and funtions
*/
/******************************************************************************/
void GameStateWinInit(void);
/******************************************************************************/
/*!
	Update function of the state, calculate position and logics for the objects
*/
/******************************************************************************/
void GameStateWinUpdate(void);
/******************************************************************************/
/*!
	Draws all the objects, particle and text for the game
*/
/******************************************************************************/
void GameStateWinDraw(void);
/******************************************************************************/
/*!
	Destroy all objects when function is called
*/
/******************************************************************************/
void GameStateWinFree(void);
/******************************************************************************/
/*!
	Free all memory when function is called
*/
/******************************************************************************/
void GameStateWinUnload(void);

// ---------------------------------------------------------------------------



#endif // GAME_STATE_PLATFORM_LEVEL_WIN_H