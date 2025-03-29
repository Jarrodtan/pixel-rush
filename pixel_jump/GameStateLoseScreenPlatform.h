/*******************************************************************/
/*!
	File:		GameStateLoseScreenPlatform.h
	Project:	Pixel Rush
	Author(s):  Lau Xiang Yang

	Brief:		All Functional Declaration to use for GameStateLoseScreenPlatform.cpp file

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

#ifndef GAME_STATE_PLATFORM_LEVEL_LOSE_H
#define GAME_STATE_PLATFORM_LEVEL_LOSE_H

/******************************************************************************/
/*!
	Load meshes,particle,font and map. Allocate memory for game object
*/
/******************************************************************************/
void GameStateLoseLoad(void);

/******************************************************************************/
/*!
	Initialize all varaibles and funtions
*/
/******************************************************************************/
void GameStateLoseInit(void);

/******************************************************************************/
/*!
	Update function of the state, calculate position and logics for the objects
*/
/******************************************************************************/
void GameStateLoseUpdate(void);

/******************************************************************************/
/*!
	Draws all the objects, particle and text for the game
*/
/******************************************************************************/
void GameStateLoseDraw(void);

/******************************************************************************/
/*!
	Destroy all objects when function is called
*/
/******************************************************************************/
void GameStateLoseFree(void);

/******************************************************************************/
/*!
	Free all memory when function is called
*/
/******************************************************************************/
void GameStateLoseUnload(void);

// ---------------------------------------------------------------------------

#endif // GAME_STATE_PLATFORM_LEVEL_LOSE_H