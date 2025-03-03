#include "main.h"

namespace {
	static f32 f_CreditsScroll;
	static f32 f_CreditsScrollTime;
	static f32 f_MaxCreditsScrollTime = 0.25f;
	static const f32 f_CreditsFontSize = 0.8f;
	static f32 f_CreditsOffset = 50.f;

	static const f32 f_ButtonOffsetX = 100.f;
	static const f32 f_ButtonOffsetY = 50.f;
	static bool b_CreditsState;
	static std::vector<std::string> Lines;

}

void GameStateCreditsLoad() {

}

void GameStateCreditsInit() {

	Lines.push_back("Credits:");
	Lines.push_back("All Content (C) 2025 Digipen Institute of Technology Singaporee");
	Lines.push_back("All Rights Reserved");
	Lines.push_back(" ");

	Lines.push_back("A game by Team Pixel Jump");
	// TODO: Add your names here
	// Lines.push_back(... put yall names here...);
	Lines.push_back(" ");

	Lines.push_back("Faculty & Advisors:");
	Lines.push_back("Gerald Wong");
	Lines.push_back("Cheng Ding Xiang");
	Lines.push_back(" ");

	Lines.push_back("President:");
	Lines.push_back("Claude Comair");
	Lines.push_back(" ");

	Lines.push_back("Executives:");
	Lines.push_back("Jason Chu   Michele COMAIR   Christopher COMAIR");
	Lines.push_back("Alexander COMAIR   Claude COMAIR   Michael GATS");
	Lines.push_back("TAN Chek Ming   Prasanna GHALI   Mandy WONG");
	Lines.push_back("Johnny DEEK   Raymond YAN   Dr. Xin LI");
	Lines.push_back("Dr. TANG Liang   Calvin TAN Eu Jin   Elie HOSRY");
	Lines.push_back("Dr. WU Yilin   GOH Xiao Shi");
	Lines.push_back(" ");

	Lines.push_back("Art Assets from:");
	// TODO: Add art asset credits here
	// Lines.push_back(... add art assets credits...);
	Lines.push_back("Sounds from Digipen Libary");
	Lines.push_back("Using AEngine");
	Lines.push_back("Audio Engine: FMOD Studio by Firelight Technologies Pty Ltd.");


}

void GameStateCreditsUpdate() {
	f_CreditsScroll += g_dt;
	f_CreditsScrollTime += g_dt;

	if (f_CreditsScrollTime > f_MaxCreditsScrollTime) {
		f_CreditsScrollTime = 0.f;
		b_CreditsState = 0;
	}

	s32 s_MouseX, s_MouseY;
	AEInputGetCursorPosition(&s_MouseX, &s_MouseY);

	// Find the min and max position of the button
	f32 f_MinX = g_WindowWidth * 0.1f - f_ButtonOffsetX;
	f32 f_MaxX = g_WindowWidth * 0.1f + f_ButtonOffsetX;
	f32 f_MinY = g_WindowHeight * 0.9f - f_ButtonOffsetY;
	f32 f_MaxY = g_WindowHeight * 0.9f + f_ButtonOffsetY;

	if (s_MouseX > f_MinX && s_MouseX < f_MaxX && s_MouseY > f_MinY && s_MouseY < f_MaxY) {
		if (AEInputCheckTriggered(AEVK_LBUTTON)) {
			GameStateManager::g_GameStateNext = States::GS_MAINMENU;
		}
	}
}

void GameStateCreditsDraw() {
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(0.5f);

	AEMtx33 trans {
		g_WindowWidth * 0.1f, 0, (g_WindowWidth * 0.1f) - (g_WindowWidth * 0.5f),
		0, g_WindowHeight * 0.1f, (g_WindowHeight * 0.1f) - (g_WindowHeight * 0.5f),
		0, 0, 1
	};

	Render::RenderShapes(Render::SHAPES::SQUARE, trans, 1.f, 0, 0, .1f);
	Render::RenderFont("Back", 0.8f, g_WindowWidth * 0.1f, g_WindowHeight * 0.1f, Render::COLOUR::WHITE, Render::FONT_ALIGN::CENTER);

	for (s32 i = 0; i < Lines.size(); ++i) {
		Render::RenderFont(Lines[i], f_CreditsFontSize, g_WindowWidth * 0.5f, 0.f + (f_CreditsScroll * 50.0f) - (f_CreditsOffset * static_cast<f32>(i)), Render::COLOUR::WHITE, Render::FONT_ALIGN::CENTER);
	}
}

void GameStateCreditsFree() {

}

void GameStateCreditsUnload() {

}
