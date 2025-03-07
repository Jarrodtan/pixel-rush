#ifndef GAME_STATE_LIST_H
#define GAME_STATE_LIST_H

namespace States {
	enum
	{
		// list of all game states 
		GS_PLATFORM_LVL1,
		GS_PLATFORM_LVL2,
		GS_PLATFORM_LVL3,

		GS_TUTORIAL_LVL1,
		GS_TUTORIAL_LVL2,

		GS_SPLASHSCREEN,
		GS_MAINMENU,
		GS_CREDITS,
		GS_WIN,
		GS_LOSE,

		// special game state. Do not change
		GS_RESTART,
		GS_QUIT,

		GS_NUM
	};
}



#endif // GAME_STATE_LIST_H