/*******************************************************************/
/*!
	File:		GameStateWinScreenPlatform.cpp
	Project:	Pixel Rush
	Author(s):  Lau Xiang Yang (50% of Code), Marco Ng Wei Zhen (50% of Code)
	Brief: Game Win State for designing the win scene using button features, audio manager, etc.

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

/******************************************************************************/
/*!
	Include Files
*/
/******************************************************************************/
#include "main.h"
#include "hpbar.hpp"

// Axe Whye Code Function //
namespace {
	struct WinSceneText {
		f32 f_PositionX, f_PositionY, f_VelocityY;
		s8 c_Char;
		bool b_Effect;

		WinSceneText(char c_ch) :
			f_PositionX(0.f), f_PositionY(0.f), f_VelocityY(0.f), c_Char(c_ch), b_Effect(false) {
		}
	};

	static WinSceneText Title[] = { 'Y', 'O', 'U', ' ', 'W', 'O', 'N', '!' };
	static const f32 f_FirstCharOffsetX = 650.f;
	static const f32 f_CharOffsetX = 50.f;
	static const f32 f_CharOffsetY = 20.f;
	static const f32 f_TitleFallEffect = 10.f;

	static const s8 c_MaxButtons = 4, e_MaxButtons = 3;
	static Button button[c_MaxButtons];
	static const f32 f_ButtonWidth = 200.f;
	static const f32 f_ButtonHeight = 75.f;
	static const f32 f_ButtonOffset = 25.f;

	static const bool is_animatedTitle = false;
	static bool hideExitButton = false;
}

AEMtx33 scaleBG4 = { 0 };
AEMtx33 rotateBG4 = { 0 };
AEMtx33 translateBG4 = { 0 };
AEMtx33 transformBG4 = { 0 };

/******************************************************************************/
/*!
	Load meshes,particle,font and map. Allocate memory for game object
*/
/******************************************************************************/
void GameStateWinLoad(void) {
	AESysSetWindowTitle("Win");
	AudioManager::PlayBGM("Assets/AudioBGM/Sadness_Cosmo_by_Bruno_ph_Id_109409_trim.wav");
}

/******************************************************************************/
/*!
	Initialize all varaibles and funtions
*/
/******************************************************************************/
void GameStateWinInit(void) {
	button[0].Init(g_WindowWidth * 0.5f, g_WindowHeight * 0.4f, f_ButtonWidth, f_ButtonHeight, "Next", "Next", 0.4f, 0.4f, 1.f);
	button[1].Init(g_WindowWidth * 0.5f, g_WindowHeight * 0.4f - (f_ButtonHeight + f_ButtonOffset), f_ButtonWidth, f_ButtonHeight, "Replay", "Replay", 0.4f, 0.4f, 1.f);
	button[2].Init(g_WindowWidth * 0.5f, g_WindowHeight * 0.4f - (f_ButtonHeight + f_ButtonOffset) * 2, f_ButtonWidth, f_ButtonHeight, "Home", "Home", 0.4f, 0.4f, 1.f);
	//button[3].Init(g_WindowWidth * 0.5f, g_WindowHeight * 0.4f - (f_ButtonHeight + f_ButtonOffset) * 3, f_ButtonWidth, f_ButtonHeight, "Quit", "Quit", 0.4f, 0.4f, 1.f);
	for (s32 i = 0; i < sizeof(Title) / sizeof(*Title); ++i) {
		Title[i].f_PositionX = f_FirstCharOffsetX + f_CharOffsetX * i;
		Title[i].f_PositionY = g_WindowHeight + f_CharOffsetY * i;
	}

	//background image
	renderUI_transform(0.f, 0.f, g_WindowWidth, g_WindowHeight, 0.f, scaleBG4, rotateBG4, translateBG4, transformBG4);

	/*AEMtx33Scale(&scaleBG4, g_WindowWidth, g_WindowHeight);
	AEMtx33Rot(&rotateBG4, 0);
	AEMtx33Trans(&translateBG4, 0.f, 0.f);
	AEMtx33Concat(&transformBG4, &rotateBG4, &scaleBG4);
	AEMtx33Concat(&transformBG4, &translateBG4, &transformBG4);*/
}

/******************************************************************************/
/*!
	Update function of the state, calculate position and logics for the objects
*/
/******************************************************************************/
void GameStateWinUpdate(void) {
	for (s32 i = 0; i < sizeof(Title) / sizeof(*Title); ++i) {
		Title[i].f_PositionY += Title[i].f_VelocityY * g_dt;
		Title[i].f_VelocityY -= f_TitleFallEffect;

		if (Title[i].b_Effect == false) {
			if (Title[i].f_PositionY < g_WindowHeight * 0.6f) {
				Title[i].b_Effect = true;
				Title[i].f_VelocityY = g_WindowHeight * 0.5f;
			}
		}

		if (Title[i].f_PositionY < 0) {
			Title[i].f_PositionY = g_WindowHeight;
			Title[i].b_Effect = false;
		}
	}

	if (GameStateManager::g_GameStatePrev == States::GS_TUTORIAL_LVL3 || GameStateManager::g_GameStatePrev == States::GS_PLATFORM_LVL3) {
		hideExitButton = true;
	}
	else {
		hideExitButton = false;
	}

	switch (GameStateManager::g_GameStatePrev) {
	default:
	case States::GS_PLATFORM_LVL1:
		saveprevlevel = 2;
		break;
	case States::GS_PLATFORM_LVL2:
		saveprevlevel = 3;
		break;
	case States::GS_PLATFORM_LVL3:
		saveprevlevel = 1;
		break;
	case States::GS_TUTORIAL_LVL1:
		saveprevtut = -2;
		break;
	case States::GS_TUTORIAL_LVL2:
		saveprevtut = -3;
		break;
	case States::GS_TUTORIAL_LVL3:
		saveprevtut = -1;
		break;
	}

	s32 i_MousePosX, i_MousePosY;
	bool b_MouseTriggered = AEInputCheckTriggered(AEVK_LBUTTON), b_MouseReleased = AEInputCheckReleased(AEVK_LBUTTON);
	AEInputGetCursorPosition(&i_MousePosX, &i_MousePosY);
	i_MousePosY = static_cast<s32>(g_WindowHeight) - i_MousePosY;

	for (s32 j = 0; j < c_MaxButtons; ++j) {
		switch (j) {
		case 0:
			if (button[j].Update(static_cast<f32>(i_MousePosX), static_cast<f32>(i_MousePosY), b_MouseTriggered, b_MouseReleased) && !hideExitButton) {

				switch (GameStateManager::g_GameStatePrev) {
				case States::GS_TUTORIAL_LVL1:
					GameStateManager::g_GameStateNext = States::GS_TUTORIAL_LVL2;
					break;
				case States::GS_TUTORIAL_LVL2:
					GameStateManager::g_GameStateNext = States::GS_TUTORIAL_LVL3;
					break;
				case States::GS_PLATFORM_LVL1:
					GameStateManager::g_GameStateNext = States::GS_PLATFORM_LVL2;
					break;
				case States::GS_PLATFORM_LVL2:
					GameStateManager::g_GameStateNext = States::GS_PLATFORM_LVL3;
					break;
				}
			}
			break;
		case 1:
			if (button[j].Update(static_cast<f32>(i_MousePosX), static_cast<f32>(i_MousePosY), b_MouseTriggered, b_MouseReleased) && hideExitButton) {
				switch (GameStateManager::g_GameStatePrev) {
				/*case States::GS_TUTORIAL_LVL2:
					GameStateManager::g_GameStateNext = States::GS_TUTORIAL_LVL1;
					break;*/
				case States::GS_TUTORIAL_LVL3:
					GameStateManager::g_GameStateNext = States::GS_TUTORIAL_LVL1;
					break;
				case States::GS_PLATFORM_LVL3:
					GameStateManager::g_GameStateNext = States::GS_PLATFORM_LVL1;
					break;
				}
			}
			break;

		case 2:
			if (button[j].Update(static_cast<f32>(i_MousePosX), static_cast<f32>(i_MousePosY), b_MouseTriggered, b_MouseReleased)) {
				GameStateManager::g_GameStateNext = States::GS_MAINMENU;
			}
			break;
		}
	}

}

/******************************************************************************/
/*!
	Draws all the objects, particle and text for the game
*/
/******************************************************************************/
void GameStateWinDraw(void) {
	AEGfxSetBackgroundColor(0.5f, 0.5f, 0.8f);
	Render::RenderShapes(Render::SHAPES::SQUARE, transformBG4, Render::TEXTURE::TITLE_WIN, 0, 0, 0, 1);
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(0.5f);

	std::stringstream ss, sr;
	if (is_animatedTitle) {
		for (u32 i = 0; i < sizeof(Title) / sizeof(*Title); ++i) {
			ss << Title[i].c_Char;
			Render::RenderFont(ss.str(), 2.5f, Title[i].f_PositionX, Title[i].f_PositionY, Render::COLOUR::LIGHT_BLUE, Render::FONT_ALIGN::CENTER);
			ss.str("");
		}
	}
	else {
		//sr << "You Won!";
		//Render::RenderFont(sr.str(), 2.5f, g_WindowWidth * 0.50f, g_WindowHeight * 0.65f, Render::COLOUR::LIGHT_BLUE, Render::FONT_ALIGN::CENTER);
	}
	if (!hideExitButton) {
		for (s32 i = 0; i < e_MaxButtons;) {
			button[i].Render(g_WindowWidth * 0.5f, g_WindowHeight * 0.5f);
			i += 2;
		}
	}
	else {
		for (s32 i = 1; i < e_MaxButtons; ++i) {
			button[i].Render(g_WindowWidth * 0.5f, g_WindowHeight * 0.5f);
		}
	}

}

/******************************************************************************/
/*!
	Destroy all objects when function is called
*/
/******************************************************************************/
void GameStateWinFree(void) {

}

/******************************************************************************/
/*!
	Free all memory when function is called
*/
/******************************************************************************/
void GameStateWinUnload(void) {

}
