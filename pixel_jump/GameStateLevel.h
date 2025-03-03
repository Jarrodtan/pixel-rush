#ifndef GAME_STATE_PLATFORM_LEVEL1_H
#define GAME_STATE_PLATFORM_LEVEL1_H

/******************************************************************************/
/*!
	Load meshes,particle,font and map. Allocate memory for game object
*/
/******************************************************************************/
void GameStateLevelLoad(void);
/******************************************************************************/
/*!
	Initialize all varaibles and funtions
*/
/******************************************************************************/
void GameStateLevelInit(void);
/******************************************************************************/
/*!
	Update function of the state, calculate position and logics for the objects
*/
/******************************************************************************/
void GameStateLevelUpdate(void);
/******************************************************************************/
/*!
	Draws all the objects, particle and text for the game
*/
/******************************************************************************/
void GameStateLevelDraw(void);
/******************************************************************************/
/*!
	Destroy all objects when function is called
*/
/******************************************************************************/
void GameStateLevelFree(void);
/******************************************************************************/
/*!
	Free all memory when function is called
*/
/******************************************************************************/
void GameStateLevelUnload(void);

// ---------------------------------------------------------------------------



#endif // GAME_STATE_PLATFORM_LEVEL1_H