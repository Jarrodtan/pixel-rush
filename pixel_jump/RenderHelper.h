/*******************************************************************/
/*!
	File:		RenderHelper.h
	Project:	Pixel Rush
	Author(s):  Lau Xiang Yang (85% of Code), Mira Tristan Dumlao (5% of Code), Tan Wei Leong (5% of Code), Marco Ng Wei Zhen (5% of Code)
	Brief: All Functional Declaration to use for RenderHelper.cpp file

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

#ifndef RENDER_HELPER_HEADER
#define RENDER_HELPER_HEADER
#include <string>
#include "GameObjects.h"
#include <AEEngine.h>
// render helper namespace for their data container using enum names, can add on easily if needed for more textures or colours rgb
namespace Render {
	enum FONT_ALIGN {
		LEFT = 0,
		RIGHT,
		CENTER,
		MAX_FONT_ALIGN
	};

	enum COLOUR {
		WHITE = 0,
		RED,
		LIGHT_BLUE,
		BLUE,
		YELLOW,
		BLACK,

		MAX_COLOUR
	};

	enum SHAPES {
		TRIANGLE = 0,
		SQUARE,
		INVERTED_SQUARE,
		CIRCLE,

		MAX_SHAPES
	};

	enum TEXTURE {
		LIGHT_PLAYER = 0,
		SHADOW_PLAYER,
		TILE_FLOOR,
		TILE_FOREGROUND,
		TILE_MIDDLEGROUND,
		TILE_BACKGROUND,
		TITLE_MAINMENU,
		TITLE_CREDIT,
		TITLE_WIN,
		TITLE_LOSE,
		UI_BASIC,
		UI_TIPS,
		UI_PLives,
		UI_SFrag,
		UI_LShard,
		LEVEL_BASIC,
		TOKEN,
		SPIKE,
		MOVEABLE_SPIKE,
		PLATFORM,
		PRESSURE_PLATE,
		DOOR,
		PORTAL,
		GRAVITY_TOKEN,
		TUTORIAL_INTRO,
		TUTORIAL_TIPS,
		SPLASHSCREEN,
		FMODSCREEN,
		TILE_FLOOR2,
		TILE_FOREGROUND2,
		TILE_MIDDLEGROUND2,
		TILE_BACKGROUND2,
		LEVEL_BASIC2,
		UI_BASIC2,
		UI_TIMER,

		// TODO: Add more textures here as needed

		MAX_TEXTURE
	};

	void Load();
	void InitRender(Objects::GameObject*& p_ObjectList, u32 max);
	void RenderShapes(Objects::GameObjectInstance*& p_Object, AEMtx33 transform);
	void RenderShapes(Objects::GameObjectInstance*& p_Object, AEMtx33 transform, f32 r, f32 g, f32 b, f32 a);
	void RenderShapes(Objects::GameObjectInstance*& p_Object, AEMtx33 transform, TEXTURE tex, f32 r = 0.f, f32 g = 0.f, f32 b = 0.f, f32 a = 1.f);
	void RenderShapes(SHAPES shape, AEMtx33 transform, f32 r, f32 g, f32 b, f32 a);
	void RenderShapes(SHAPES shape, AEMtx33 transform, TEXTURE tex, f32 r = 0.f, f32 g = 0.f, f32 b = 0.f, f32 a = 1.f);
	void RenderFont(std::string const& text, f32 size, f32 x, f32 y, COLOUR colour, FONT_ALIGN alignment = LEFT);
	void RenderFont(std::string const& text, f32 size, f32 x, f32 y, f32 r, f32 g, f32 b, f32 a, FONT_ALIGN alignment = CENTER);
	void Exit();

}

#endif // RENDER_HELPER_HEADER