/******************************************************************************/
/*!
	Load meshes,particle,font and map. Allocate memory for game object
*/
/******************************************************************************/
#include "main.h"
#include "hpbar.hpp"

// Axe Whye Code Function //
namespace {
	struct LoseSceneText {
		f32 f_PositionX, f_PositionY, f_VelocityY;
		s8 c_Char;
		bool b_Effect;

		LoseSceneText(char c_ch) :
			f_PositionX(0.f), f_PositionY(0.f), f_VelocityY(0.f), c_Char(c_ch), b_Effect(false) {
		}
	};

	static LoseSceneText Title[] = { 'G', 'A', 'M', 'E', ' ', 'O', 'V', 'E', 'R', '!' };
	static const f32 f_FirstCharOffsetX = 600.f;
	static const f32 f_CharOffsetX = 50.f;
	static const f32 f_CharOffsetY = 20.f;
	static const f32 f_TitleFallEffect = 10.f;

	static const s8 c_MaxButtons = 4;
	static Button button[c_MaxButtons];
	static const f32 f_ButtonWidth = 200.f;
	static const f32 f_ButtonHeight = 75.f;
	static const f32 f_ButtonOffset = 25.f;

	static const bool is_animatedTitle = false;
}

/******************************************************************************/
/*!
	Load meshes,particle,font and map. Allocate memory for game object
*/
/******************************************************************************/
void GameStateLoseLoad(void) {
	AESysSetWindowTitle("Lose");
	AudioManager::PlayBGM("Assets/AudioBGM/Sadness_Cosmo_by_Bruno_ph_Id_109409_trim.wav");
}

/******************************************************************************/
/*!
	Initialize all varaibles and funtions
*/
/******************************************************************************/
void GameStateLoseInit(void) {
	button[0].Init(g_WindowWidth * 0.5f, g_WindowHeight * 0.4f, f_ButtonWidth, f_ButtonHeight, "Restart", "Restart", 0.4f, 0.4f, 1.f);
	button[1].Init(g_WindowWidth * 0.5f, g_WindowHeight * 0.4f - (f_ButtonHeight + f_ButtonOffset), f_ButtonWidth, f_ButtonHeight, "Home", "Home", 0.4f, 0.4f, 1.f);

	for (s32 i = 0; i < sizeof(Title) / sizeof(*Title); ++i) {
		Title[i].f_PositionX = f_FirstCharOffsetX + f_CharOffsetX * i;
		Title[i].f_PositionY = g_WindowHeight + f_CharOffsetY * i;
	}
	
	
}

/******************************************************************************/
/*!
	Update function of the state, calculate position and logics for the objects
*/
/******************************************************************************/
void GameStateLoseUpdate(void) {
	if (AEInputCheckTriggered(AEVK_Q)) {
		GameStateManager::g_GameStateNext = States::GS_MAINMENU;
	}
	else if (AEInputCheckTriggered(AEVK_1)) {
		GameStateManager::g_GameStateNext = States::GS_PLATFORM_LVL1;
	}
	else if (AEInputCheckTriggered(AEVK_2)) {
		GameStateManager::g_GameStateNext = States::GS_PLATFORM_LVL2;
	}
	else if (AEInputCheckTriggered(AEVK_3)) {
		GameStateManager::g_GameStateNext = States::GS_WIN;
	}
	else if (AEInputCheckTriggered(AEVK_4)) {
		GameStateManager::g_GameStateNext = States::GS_LOSE;
	}

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

	s32 i_MousePosX, i_MousePosY;
	bool b_MouseTriggered = AEInputCheckTriggered(AEVK_LBUTTON), b_MouseReleased = AEInputCheckReleased(AEVK_LBUTTON);
	AEInputGetCursorPosition(&i_MousePosX, &i_MousePosY);
	i_MousePosY = static_cast<s32>(g_WindowHeight) - i_MousePosY;

	for (s32 j = 0; j < c_MaxButtons; ++j) {
		switch (j) {
		case 0:
			if (button[j].Update(static_cast<f32>(i_MousePosX), static_cast<f32>(i_MousePosY), b_MouseTriggered, b_MouseReleased)) {
				GameStateManager::g_GameStateNext = GameStateManager::g_GameStatePrev;
			}
			break;

		case 1:
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
void GameStateLoseDraw(void) {
	AEGfxSetBackgroundColor(0.5f, 0.5f, 0.8f);
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(0.5f);

	std::stringstream ss, sr;
	if (is_animatedTitle) {
		for (u32 i = 0; i < sizeof(Title) / sizeof(*Title); ++i) {
			ss << Title[i].c_Char;
			Render::RenderFont(ss.str(), 2.5f, Title[i].f_PositionX, Title[i].f_PositionY, Render::COLOUR::RED, Render::FONT_ALIGN::CENTER);
			ss.str("");
		}
	}
	else {
		sr << "Game Over!";
		Render::RenderFont(sr.str(), 2.5f, g_WindowWidth * 0.50f, g_WindowHeight * 0.65f, Render::COLOUR::RED, Render::FONT_ALIGN::CENTER);
	}
	for (s32 i = 0; i < c_MaxButtons; ++i) {
		button[i].Render(g_WindowWidth * 0.5f, g_WindowHeight * 0.5f);
	}

}

/******************************************************************************/
/*!
	Destroy all objects when function is called
*/
/******************************************************************************/
void GameStateLoseFree(void) {

}

/******************************************************************************/
/*!
	Free all memory when function is called
*/
/******************************************************************************/
void GameStateLoseUnload(void) {

}
