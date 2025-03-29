/*******************************************************************/
/*!
	File:		GameStateCredits.h
	Project:	Pixel Rush
	Author(s):  Lau Xiang Yang

	Brief:		All Functional Declaration to use for GameStateCredits.cpp file

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

#ifndef CREDITS_H
#define CREDITS_H

// Credits pointer functions for load, draw, free, update, etc.

void GameStateCreditsLoad(void);
void GameStateCreditsInit(void);
void GameStateCreditsUpdate(void);
void GameStateCreditsDraw(void);
void GameStateCreditsFree(void);
void GameStateCreditsUnload(void);

#endif