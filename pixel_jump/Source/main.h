#ifndef HEADER_MAIN_H
#define HEADER_MAIN_H

/**************************************************************************/
/*!
	Include Files
*/
/**************************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "AEEngine.h"
#include "math.h"

#include "GameStateMgr.h"
#include "GameStateLevel1Platform.h"
#include "GameStateLevel2Platform.h"
#include "Collision.h"
#include "Particle.h"
#include "GameState_MainMenu.h"

/**************************************************************************/

/**************************************************************************/
/*!
	Global Variables
*/
/**************************************************************************/
extern f32 g_dt;
extern f64 g_appTime;
extern s8 g_Font;

#endif // HEADER_MAIN_H