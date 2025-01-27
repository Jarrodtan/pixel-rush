#ifndef GAME_STATE_MGR_H
#define GAME_STATE_MGR_H

/******************************************************************************/
/*!
	Include Files
*/
/******************************************************************************/
#include "AEEngine.h"
#include "GameStateList.h"

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
	/******************************************************************************/\
	void GameStateMgrInit(u32 gameStateInit);
	void GameStateMgrUpdate();
}



#endif // GAME_STATE_MGR_H