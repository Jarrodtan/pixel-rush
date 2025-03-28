/*******************************************************************/
/*!
	File:		GameStateSplash.h
	Project:	Pixel Rush
	Author(s):  Lau Xiang Yang
	Brief: All Functional Declaration to use for GameStateSplash.cpp file

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

#ifndef SPLASH_H
#define SPLASH_H
// splash screen pointer function for load, initialized, free, update, etc.
void GameStateSplashLoad(void);
void GameStateSplashInit(void);
void GameStateSplashUpdate(void);
void GameStateSplashDraw(void);
void GameStateSplashFree(void);
void GameStateSplashUnload(void);


#endif