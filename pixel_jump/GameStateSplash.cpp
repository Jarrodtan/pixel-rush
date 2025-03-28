/*******************************************************************/
/*!
	File:		GameStateSplash.cpp
	Project:	Pixel Rush
	Author(s):  Lau Xiang Yang

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

#include "main.h"

namespace {
	static f32 f_SplashScreenTime = 0.f;
	static const f32 f_SplashScrrenUnChangedTime = 2.0f;
	static const f32 f_MaxSplashScreenTime = 1.5f;
	static const f32 f_MaxFMODSplashScreenTime = 1.5f;
}

void GameStateSplashLoad() {
	AESysSetWindowIcon("Assets/Art/icon.ico", 100, 100);

}

void GameStateSplashInit() {

}

void GameStateSplashUpdate() {
	if (AEInputCheckTriggered(AEVK_LBUTTON) || AEInputCheckTriggered(AEVK_SPACE)) {
		GameStateManager::g_GameStateNext = States::GS_MAINMENU;
	}
}

void GameStateSplashDraw() {
	AEGfxSetBackgroundColor(0.f, 0.f, 0.f);
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(0.5f);

	f_SplashScreenTime += g_dt;
	f32 f_SplashScreenProgress = max(0, f_SplashScreenTime - f_SplashScrrenUnChangedTime);

	if (f_SplashScreenProgress < f_MaxSplashScreenTime) {
		AEMtx33 trans{
			g_WindowWidth * 0.9f, 0, 0,
			0 ,g_WindowHeight * 0.5f , 0,
			0, 0, 1
		};
		Render::RenderShapes(Render::SQUARE, trans, Render::TEXTURE::SPLASHSCREEN, 1.f, 0, 0, 1 - (f_SplashScreenProgress / f_MaxSplashScreenTime));

		f32 f_CopyRightTextSize = 0.8f;
		std::string s_CopyRightText = "All content (C) 2025 Digipen Insitute of Technology Singapore. All rights reserved.";
		Render::RenderFont(s_CopyRightText, f_CopyRightTextSize, g_WindowWidth * 0.5f, g_WindowHeight * 0.15f, 1.f, 1.f, 1.f, 1 - (f_SplashScreenProgress / f_MaxSplashScreenTime));
	}

	if (f_SplashScreenProgress > f_MaxSplashScreenTime) {
		f_SplashScreenProgress -= f_MaxSplashScreenTime;
		AEMtx33 trans{
			g_WindowWidth * 0.5f, 0, 0,
			0, g_WindowHeight * 0.2f, 0,
			0, 0, 1
		};

		Render::RenderShapes(Render::SQUARE, trans, Render::TEXTURE::FMODSCREEN, 1.f, 0, 0, 1 - (f_SplashScreenProgress / f_MaxSplashScreenTime));
		if (f_SplashScreenProgress > f_MaxFMODSplashScreenTime) {
			GameStateManager::g_GameStateNext = States::GS_MAINMENU;
		}
	}
}

void GameStateSplashFree() {

}

void GameStateSplashUnload() {

}
