/*******************************************************************/
/*!
	File:		TutorialMenu.h
	Project:	Pixel Rush
	Author(s):  Lau Xiang Yang

	Brief:		All Functional Declaration to use for TutorialMenu.cpp file

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

#ifndef TUTORIAL_MENU_H
#define TUTORIAL_MENU_H

// Game tutorial menu function pointers use for load, init, update, free, etc.
void GameStateTutorialMenuLoad(void);
void GameStateTutorialMenuInit(void);
void GameStateTutorialMenuUpdate(void);
void GameStateTutorialMenuDraw(void);
void GameStateTutorialMenuFree(void);
void GameStateTutorialMenuUnload(void);

#endif
