/*******************************************************************/
/*!
	File:		GameStateMgr.h
	Project:	Pixel Rush
	Author(s):  Lau Xiang Yang

	Brief:		All Functional Declaration to use for GameStateMgr.cpp file

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

#ifndef GAME_STATE_MGR_H
#define GAME_STATE_MGR_H

#include "main.h"

namespace GameStateManager {
	/******************************************************************************/
	/*!
		External Variables
	*/
	/******************************************************************************/
	extern u32 g_GameStateInit;
	extern u32 g_GameStateCurr;
	extern u32 g_GameStatePrev;
	extern u32 g_GameStateNext;

	extern void (*GameStateLoad)();
	extern void (*GameStateInit)();
	extern void (*GameStateUpdate)();
	extern void (*GameStateDraw)();
	extern void (*GameStateFree)();
	extern void (*GameStateUnload)();

	/******************************************************************************/
	/*!
		Function Declarations
	*/
	/******************************************************************************/
	void GameStateMgrInit(u32 gameStateInit);
	void GameStateMgrUpdate();
}

#endif // GAME_STATE_MGR_H