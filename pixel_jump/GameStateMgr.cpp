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
		case States::GS_PLATFORM_LVL1:
			GameStateLoad = GameStateLevel1PlatformLoad;
			GameStateInit = GameStateLevel1PlatformInit;
			GameStateUpdate = GameStateLevel1PlatformUpdate;
			GameStateDraw = GameStateLevel1PlatformDraw;
			GameStateFree = GameStateLevel1PlatformFree;
			GameStateUnload = GameStateLevel1PlatformUnload;
			break;

		case States::GS_PLATFORM_LVL2:
			GameStateLoad = GameStateLevel2PlatformLoad;
			GameStateInit = GameStateLevel2PlatformInit;
			GameStateUpdate = GameStateLevel2PlatformUpdate;
			GameStateDraw = GameStateLevel2PlatformDraw;
			GameStateFree = GameStateLevel2PlatformFree;
			GameStateUnload = GameStateLevel2PlatformUnload;
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

