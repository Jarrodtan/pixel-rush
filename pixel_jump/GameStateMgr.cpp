/*******************************************************************/
/*!
	File:		GameStateMgr.cpp
	Project:	Pixel Rush
	Author(s):  Lau Xiang Yang

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

/**************************************************************************/
/*!
	Include Files
*/
/**************************************************************************/
#include "main.h"

namespace GameStateManager {
	/**************************************************************************/
	/*!
		Global Variables
	*/
	/**************************************************************************/
	u32 g_GameStateInit{},
		g_GameStateCurr{},
		g_GameStatePrev{},
		g_GameStateNext{};

	void (*GameStateLoad)() {},
		 (*GameStateInit)() {},
		 (*GameStateUpdate)() {},
		 (*GameStateDraw)() {},
		 (*GameStateFree)() {},
		 (*GameStateUnload)() {};

	void GameStateMgrInit(u32 gameStateInit) {
		// set the initial game state
		g_GameStateInit = gameStateInit;

		// reset the current, previous and the next game states
		g_GameStateCurr = g_GameStatePrev = g_GameStateNext = g_GameStateInit;

		// call the update function to set the function pointers
		GameStateMgrUpdate();
	}

	void GameStateMgrUpdate() {
		if ((g_GameStateCurr == States::GS_RESTART) || (g_GameStateCurr == States::GS_QUIT)) {
			return;
		}

		switch (g_GameStateCurr) {
		case States::GS_TUTORIAL_MENU:
		case States::GS_TUTORIAL_TIP:
			GameStateLoad = GameStateTutorialMenuLoad;
			GameStateInit = GameStateTutorialMenuInit;
			GameStateUpdate = GameStateTutorialMenuUpdate;
			GameStateDraw = GameStateTutorialMenuDraw;
			GameStateFree = GameStateTutorialMenuFree;
			GameStateUnload = GameStateTutorialMenuUnload;
			break;

		case States::GS_PLATFORM_LVL1:
		case States::GS_PLATFORM_LVL2:
		case States::GS_PLATFORM_LVL3:
		case States::GS_TUTORIAL_LVL1:
		case States::GS_TUTORIAL_LVL2:
		case States::GS_TUTORIAL_LVL3:
			GameStateLoad = GameStateLevelLoad;
			GameStateInit = GameStateLevelInit;
			GameStateUpdate = GameStateLevelUpdate;
			GameStateDraw = GameStateLevelDraw;
			GameStateFree = GameStateLevelFree;
			GameStateUnload = GameStateLevelUnload;
			break;

		case States::GS_SPLASHSCREEN:
			GameStateLoad = GameStateSplashLoad;
			GameStateInit = GameStateSplashInit;
			GameStateUpdate = GameStateSplashUpdate;
			GameStateDraw = GameStateSplashDraw;
			GameStateFree = GameStateSplashFree;
			GameStateUnload = GameStateSplashUnload;
			break;

		case States::GS_CREDITS:
			GameStateLoad = GameStateCreditsLoad;
			GameStateInit = GameStateCreditsInit;
			GameStateUpdate = GameStateCreditsUpdate;
			GameStateDraw = GameStateCreditsDraw;
			GameStateFree = GameStateCreditsFree;
			GameStateUnload = GameStateCreditsUnload;
			break;

		case States::GS_MAINMENU:
			GameStateLoad = GameStateMainMenuLoad;
			GameStateInit = GameStateMainMenuInit;
			GameStateUpdate = GameStateMainMenuUpdate;
			GameStateDraw = GameStateMainMenuDraw;
			GameStateFree = GameStateMainMenuFree;
			GameStateUnload = GameStateMainMenuUnload;
			break;

		case States::GS_LOSE:
			GameStateLoad = GameStateLoseLoad;
			GameStateInit = GameStateLoseInit;
			GameStateUpdate = GameStateLoseUpdate;
			GameStateDraw = GameStateLoseDraw;
			GameStateFree = GameStateLoseFree;
			GameStateUnload = GameStateLoseUnload;
			break;

		case States::GS_WIN:
			GameStateLoad = GameStateWinLoad;
			GameStateInit = GameStateWinInit;
			GameStateUpdate = GameStateWinUpdate;
			GameStateDraw = GameStateWinDraw;
			GameStateFree = GameStateWinFree;
			GameStateUnload = GameStateWinUnload;
			break;

		default:
			AE_FATAL_ERROR("Invalid Game State");
		}
	}
}

