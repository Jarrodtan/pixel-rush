/*******************************************************************/
/*!
	File:		GameState_MainMenu.h
	Project:	Pixel Rush
	Author(s):

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

#ifndef MAIN_MENU_HEADER
#define MAIN_MENU_HEADER

/******************************************************************************/
/*!
	"Load" function of this state
*/
/******************************************************************************/
void GameStateMainMenuLoad(void);
/******************************************************************************/
/*!
	"Initialize" function of this state
*/
/******************************************************************************/
void GameStateMainMenuInit(void);
/******************************************************************************/
/*!
	"Update" function of this state
*/
/******************************************************************************/
void GameStateMainMenuUpdate(void);
/******************************************************************************/
/*!
	Draw all the meshes and print out all the text in the game state
*/
/******************************************************************************/
void GameStateMainMenuDraw(void);
/******************************************************************************/
/*!
	Destroy all objects in the game
*/
/******************************************************************************/
void GameStateMainMenuFree(void);
/******************************************************************************/
/*!
	Free all memory
*/
/******************************************************************************/
void GameStateMainMenuUnload(void);


#endif // MAIN_MENU_HEADER