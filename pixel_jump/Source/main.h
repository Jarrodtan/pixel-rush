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
#include "GameStateWinScreenPlatform.h"
#include "GameStateLoseScreenPlatform.h"
#include "Collision.h"
#include "Particle.h"
#include "GameState_MainMenu.h"
#include "MeshCreation.h"
#include "GameObjects.h"
#include "FileReader.h"
//-------------------------------------------------------------------------/

/**************************************************************************/
/*!
	Global Variables
*/
/**************************************************************************/
extern f32 g_dt;
extern f64 g_appTime;
extern s8 g_Font;
//-------------------------------------------------------------------------/

/**************************************************************************/
/*!
	Defines (TODO: Read the variables of this section ideally from a file)
*/
/**************************************************************************/
const u32 GAME_OBJECT_NUM_MAX = 32;
const u32 GAME_OBJECT_INST_NUM_MAX = 256;

// set width and heigh (TODO: Needa change this to read depending on the resolution and not magic numbers)
const f32 RECTANGLE_WIDTH = 1600.f / 20.f;
const f32 RECTANGLE_HEIGHT = 900.f / 20.f;

// gameplay related variables and values
const f32 GRAVITY = -20.f;
const f32 JUMP_VELOCITY = 11.f;
const f32 PLAYER_MOVE_VELOCITY = 4.f;
const f32 ENEMY_MOVE_VELOCITY = 7.5f;
const f64 ENEMY_IDLE_TIME = 2.0;
const s32 PLAYER_LIVES = 3;

// flags
const u32 FLAG_ACTIVE = 0x00000001;
const u32 FLAG_VISIBLE = 0x00000002;
const u32 FLAG_NON_COLLIDABLE = 0x00000004;

// collision flags
const u32 COLLISION_LEFT = 0x00000001;
const u32 COLLISION_RIGHT = 0x00000002;
const u32 COLLISION_TOP = 0x00000004;
const u32 COLLISION_BOTTOM = 0x00000008;

// bounding box
const f32 BOUNDING_RECT_SIZE = 1.f;
//-------------------------------------------------------------------------/

#endif // HEADER_MAIN_H