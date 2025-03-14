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

	static Button button;
	static const s32 i_ButtonWidth = 200;
	static const s32 i_ButtonHeight = 75;

}

AEMtx33 scaleBG2 = { 0 };
AEMtx33 rotateBG2 = { 0 };
AEMtx33 translateBG2 = { 0 };
AEMtx33 transformBG2 = { 0 };

void GameStateCreditsLoad() {

}

void GameStateCreditsInit() {
	button.Init(g_WindowWidth * 0.1f, g_WindowHeight * 0.1f, i_ButtonWidth, i_ButtonHeight, "Back", "Main Menu", 0.4f, 0.4f, 1.f);

	Lines.push_back("Credits:");
	Lines.push_back("All Content (C) 2025 Digipen Institute of Technology Singaporee");
	Lines.push_back("All Rights Reserved");
	Lines.push_back(" ");

	Lines.push_back("A game by Team Pixel Jump");
	Lines.push_back("Xiang Yang");
	Lines.push_back("Wei Liang");
	Lines.push_back("Marco");
	Lines.push_back("Tristen");
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
	Lines.push_back("https://kenney.nl/assets/pixel-platformer");
	Lines.push_back("Sounds from Digipen Libary");
	Lines.push_back("Using AEngine");
	Lines.push_back("Audio Engine: FMOD Studio by Firelight Technologies Pty Ltd.");

	//background image
	AEMtx33Scale(&scaleBG2, g_WindowWidth, g_WindowHeight);
	AEMtx33Rot(&rotateBG2, 0);
	AEMtx33Trans(&translateBG2, 0.f, 0.f);
	AEMtx33Concat(&transformBG2, &rotateBG2, &scaleBG2);
	AEMtx33Concat(&transformBG2, &translateBG2, &transformBG2);
}

void GameStateCreditsUpdate() {
	f_CreditsScroll += g_dt;
	f_CreditsScrollTime += g_dt;

	if (f_CreditsScrollTime > f_MaxCreditsScrollTime) {
		f_CreditsScrollTime = 0.f;
		b_CreditsState = 0;
	}

	s32 i_MousePosX, i_MousePosY;
	bool b_MouseTriggered = AEInputCheckTriggered(AEVK_LBUTTON), b_MouseReleased = AEInputCheckReleased(AEVK_LBUTTON);
	AEInputGetCursorPosition(&i_MousePosX, &i_MousePosY);
	i_MousePosY = static_cast<s32>(g_WindowHeight) - i_MousePosY;
	if (button.Update(static_cast<f32>(i_MousePosX), static_cast<f32>(i_MousePosY), b_MouseTriggered, b_MouseReleased)) {
		GameStateManager::g_GameStateNext = States::GS_MAINMENU;
	}

}

void GameStateCreditsDraw() {
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	Render::RenderShapes(Render::SHAPES::SQUARE, transformBG2, Render::TEXTURE::TITLE_CREDIT, 0, 0, 0, 1);
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(0.5f);

	button.Render(g_WindowWidth * 0.5f, g_WindowHeight * 0.5f);
	for (s32 i = 0; i < Lines.size(); ++i) {
		Render::RenderFont(Lines[i], f_CreditsFontSize, g_WindowWidth * 0.5f, 0.f + (f_CreditsScroll * 50.0f) - (f_CreditsOffset * static_cast<f32>(i)), Render::COLOUR::WHITE, Render::FONT_ALIGN::CENTER);
	}
}

void GameStateCreditsFree() {

}

void GameStateCreditsUnload() {
	// reset the credits scroll
	f_CreditsScroll = 0.f;
	f_CreditsScrollTime = 0.f;
	b_CreditsState = 0;
}
