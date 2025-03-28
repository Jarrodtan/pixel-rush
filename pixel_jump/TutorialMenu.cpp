/*******************************************************************/
/*!
	File:		TutorialMenu.cpp
	Project:	Pixel Rush
	Author(s):

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

#include "main.h"

namespace {
	static const f32 f_ButtonOffsetX = 100.f;
	static const f32 f_ButtonOffsetY = 50.f;
	static Button button;
	static const s32 i_ButtonWidth = 200;
	static const s32 i_ButtonHeight = 75;

	AEMtx33 scaleBG2 = { 0 };
	AEMtx33 rotateBG2 = { 0 };
	AEMtx33 translateBG2 = { 0 };
	AEMtx33 transformBG2 = { 0 };
}


void GameStateTutorialMenuLoad() {

}

void GameStateTutorialMenuInit() {
	button.Init(g_WindowWidth * 0.5f, g_WindowHeight * 0.1f, i_ButtonWidth, i_ButtonHeight, "Next", "Tutorial", 0.4f, 0.4f, 1.f);
	renderUI_transform(0.f, 0.f, g_WindowWidth, g_WindowHeight, 0.f, scaleBG2, rotateBG2, translateBG2, transformBG2);
}

void GameStateTutorialMenuUpdate() {
	s32 i_MousePosX, i_MousePosY;
	bool b_MouseTriggered = AEInputCheckTriggered(AEVK_LBUTTON), b_MouseReleased = AEInputCheckReleased(AEVK_LBUTTON);
	AEInputGetCursorPosition(&i_MousePosX, &i_MousePosY);
	i_MousePosY = static_cast<s32>(g_WindowHeight) - i_MousePosY;
	if (button.Update(static_cast<f32>(i_MousePosX), static_cast<f32>(i_MousePosY), b_MouseTriggered, b_MouseReleased)) {
		if (GameStateManager::g_GameStateCurr == States::GS_TUTORIAL_TIP) {
			GameStateManager::g_GameStateNext = States::GS_TUTORIAL_LVL1;
			/*switch (saveprevtut) {
			default:
			case -1:
				GameStateManager::g_GameStateNext = States::GS_TUTORIAL_LVL1;
				break;
			case -2:
				GameStateManager::g_GameStateNext = States::GS_TUTORIAL_LVL2;
				break;
			case -3:
				GameStateManager::g_GameStateNext = States::GS_TUTORIAL_LVL3;
				break;

			}*/
		}
		else {
			GameStateManager::g_GameStateNext = States::GS_TUTORIAL_TIP;
		}
	}
}

void GameStateTutorialMenuDraw() {
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(0.5f);

	switch (GameStateManager::g_GameStateCurr) {
	case States::GS_TUTORIAL_MENU:
		Render::RenderShapes(Render::SHAPES::SQUARE, transformBG2, Render::TEXTURE::TUTORIAL_INTRO);
		break;

	case States::GS_TUTORIAL_TIP:
		Render::RenderShapes(Render::SHAPES::SQUARE, transformBG2, Render::TEXTURE::TUTORIAL_TIPS);
		break;
	}

	button.Render(g_WindowWidth * 0.5f, g_WindowHeight * 0.5f);
}

void GameStateTutorialMenuFree() {

}

void GameStateTutorialMenuUnload() {

}
