/******************************************************************************/
/*!
	Load meshes,particle,font and map. Allocate memory for game object
*/
/******************************************************************************/
#include "main.h"
#include "hpbar.hpp"

struct GamePrefabs
{
	AEGfxVertexList* pMesh{ nullptr };
	AEMtx33 scale{ 0 }, rotate{ 0 }, translate{ 0 }, transform{ 0 };
};
GamePrefabs RestartButton{}, ExitButton;
float BtnposX[99] = { 0 }, BtnposY[99] = { 0 };
s32 mouseposX, mouseposY;

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
	
	/*RestartButton.pMesh = mesh_create_square(4278255360);
	ExitButton.pMesh = mesh_create_square(4294901760);
	BtnposX[0] = 0;
	BtnposY[0] = 100;
	BtnposX[1] = 0;
	BtnposY[1] = -100;*/

	button[0].Init(g_WindowWidth * 0.5f, g_WindowHeight * 0.4f, f_ButtonWidth, f_ButtonHeight, "Restart", "Restart", 0.4f, 0.4f, 1.f);
	button[1].Init(g_WindowWidth * 0.5f, g_WindowHeight * 0.4f - (f_ButtonHeight + f_ButtonOffset), f_ButtonWidth, f_ButtonHeight, "Home", "Home", 0.4f, 0.4f, 1.f);
	//button[2].Init(g_WindowWidth * 0.5f, g_WindowHeight * 0.4f - (f_ButtonHeight + f_ButtonOffset) * 2, f_ButtonWidth, f_ButtonHeight, "Credits", "Credits", 0.4f, 0.4f, 1.f);
	//button[3].Init(g_WindowWidth * 0.5f, g_WindowHeight * 0.4f - (f_ButtonHeight + f_ButtonOffset) * 3, f_ButtonWidth, f_ButtonHeight, "Quit", "Quit", 0.4f, 0.4f, 1.f);

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
	
	/*AEInputGetCursorPosition(&mouseposX, &mouseposY);

	if (AEInputCheckTriggered(RI_MOUSE_LEFT_BUTTON_DOWN))
	{
		if (mouseposX > BtnposX[0] + 700 && mouseposX < BtnposX[0] + 900
			&& mouseposY > BtnposY[0] + 200 && mouseposY < BtnposY[0] + 300)
		{
			GameStateManager::g_GameStateNext = States::GS_PLATFORM_LVL1;
		}
		if (mouseposX > BtnposX[1] + 700 && mouseposX < BtnposX[1] + 900
			&& mouseposY > BtnposY[1] + 600 && mouseposY < BtnposY[1] + 700)
		{
			GameStateManager::g_GameStateNext = States::GS_MAINMENU;
		}
	}*/

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
				GameStateManager::g_GameStateNext = States::GS_PLATFORM_LVL1;
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

	/*AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.f);

	//Render::RenderFont("Game Over", 2.5, 900.0f, 900.0f, Render::WHITE, Render::CENTER);


	//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	mesh_square_transform(BtnposX[0], BtnposY[0], 200, 100, 0, RestartButton.scale, RestartButton.rotate, RestartButton.translate, RestartButton.transform, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	//mesh_square_transform(0, 150, 200, 100, 0, RestartButton.scale, RestartButton.rotate, RestartButton.translate, RestartButton.transform, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	AEGfxMeshDraw(RestartButton.pMesh, AE_GFX_MDM_TRIANGLES);

	mesh_square_transform(BtnposX[1], BtnposY[1], 200, 100, 0, ExitButton.scale, ExitButton.rotate, ExitButton.translate, ExitButton.transform, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	AEGfxMeshDraw(ExitButton.pMesh, AE_GFX_MDM_TRIANGLES);
	

	AEGfxPrint(g_Font,"Game Over! :(", -0.26f, 0.6f, 2.5f, 1.f, 1.f, 1.f, 1.f);
	AEGfxPrint(g_Font, "Restart", -0.09f, 0.20f, 1.5f, 1.f, 1.f, 1.f, 1.f);
	AEGfxPrint(g_Font, "  Quit", -0.09f, -0.25f, 1.5f, 1.f, 1.f, 1.f, 1.f);*/

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

	ss << "A Game by Team Pixel Jump";
	Render::RenderFont(ss.str(), 1.f, g_WindowWidth, 18.f, Render::COLOUR::WHITE, Render::FONT_ALIGN::RIGHT);
}

/******************************************************************************/
/*!
	Destroy all objects when function is called
*/
/******************************************************************************/
void GameStateLoseFree(void) {
	//AEGfxMeshFree(RestartButton.pMesh);
	//AEGfxMeshFree(ExitButton.pMesh);
}

/******************************************************************************/
/*!
	Free all memory when function is called
*/
/******************************************************************************/
void GameStateLoseUnload(void) {

}
