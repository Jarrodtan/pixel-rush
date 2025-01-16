#ifndef GAME_STATE_MGR_H
#define GAME_STATE_MGR_H

/******************************************************************************/
/*!
	Include Files
*/
/******************************************************************************/
#include "AEEngine.h"
#include "GameStateList.h"

/******************************************************************************/
/*!
	External Variables
*/
/******************************************************************************/
extern u32 gGameStateInit;
extern u32 gGameStateCurr;
extern u32 gGameStatePrev;
extern u32 gGameStateNext;

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

#endif // GAME_STATE_MGR_H