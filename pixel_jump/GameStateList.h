/*******************************************************************/
/*!
	File:		GameStateList.h
	Project:	Pixel Rush
	Author(s):

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

#ifndef GAME_STATE_LIST_H
#define GAME_STATE_LIST_H

namespace States {
	enum
	{
		// list of all game states 
		GS_PLATFORM_LVL1,
		GS_PLATFORM_LVL2,
		GS_PLATFORM_LVL3,

		GS_TUTORIAL_MENU,
		GS_TUTORIAL_TIP,

		GS_TUTORIAL_LVL1,
		GS_TUTORIAL_LVL2,
		GS_TUTORIAL_LVL3,

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