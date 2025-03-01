/******************************************************************************/
/*!
	Include Files
*/
/******************************************************************************/
#include "main.h"
#include "hpbar.hpp"

struct GameEnity
{
	AEGfxVertexList* pMesh{ nullptr };
	AEMtx33 scale{ 0 }, rotate{ 0 }, translate{ 0 }, transform{ 0 };
};
GameEnity PlayButton{}, ExitButton;
float BposX[99] = { 0 }, BposY[99] = { 0 };
s32 kposX, kposY;
/******************************************************************************/
/*!
	"Load" function of this state
*/
/******************************************************************************/
void GameStateMainMenuLoad(void) {

}

/******************************************************************************/
/*!
	"Initialize" function of this state
*/
/******************************************************************************/
void GameStateMainMenuInit(void) {
	AESysSetWindowTitle("Main Menu");
	AudioManager::PlayBGM("Assets/AudioBGM/Retro_Funk_20_by_sonically_sound_Id_624874.wav");
	PlayButton.pMesh = mesh_create_square(4278255360);
	ExitButton.pMesh = mesh_create_square(4294901760);
	BposX[0] = 0;
	BposY[0] = 100;
	BposX[1] = 0;
	BposY[1] = -300;
}

/******************************************************************************/
/*!
	"Update" function of this state
*/
/******************************************************************************/
void GameStateMainMenuUpdate(void) {
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

	AEInputGetCursorPosition(&kposX, &kposY);

	if (AEInputCheckTriggered(RI_MOUSE_LEFT_BUTTON_DOWN))
	{
		if (kposX > BposX[0] + 700 && kposX < BposX[0] + 900
			&& kposY > BposY[0] + 200 && kposY < BposY[0] + 300)
		{
			GameStateManager::g_GameStateNext = States::GS_PLATFORM_LVL1;
		}
		if (kposX > BposX[1] + 700 && kposX < BposX[1] + 900
			&& kposY > BposY[1] + 1000 && kposY < BposY[1] + 1100)
		{
			GameStateManager::g_GameStateNext = States::GS_QUIT;
		}
	}
}

/******************************************************************************/
/*!
	Draw all the meshes and print out all the text in the game state
*/
/******************************************************************************/
void GameStateMainMenuDraw(void) {
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	mesh_square_transform(BposX[0], BposY[0], 200, 100, 0, PlayButton.scale, PlayButton.rotate, PlayButton.translate, PlayButton.transform, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);

	AEGfxMeshDraw(PlayButton.pMesh, AE_GFX_MDM_TRIANGLES);

	mesh_square_transform(BposX[1], BposY[1], 200, 100, 0, ExitButton.scale, ExitButton.rotate, ExitButton.translate, ExitButton.transform, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	AEGfxMeshDraw(ExitButton.pMesh, AE_GFX_MDM_TRIANGLES);


	AEGfxPrint(g_Font, "PIXEL RUSH", -0.26f, 0.6f, 2.5f, 1.f, 1.f, 1.f, 1.f);
	AEGfxPrint(g_Font, "Play", -0.06f, 0.20f, 1.5f, 1.f, 1.f, 1.f, 1.f);
	AEGfxPrint(g_Font, "  Exit", -0.09f, -0.7f, 1.5f, 1.f, 1.f, 1.f, 1.f);
}

/******************************************************************************/
/*!
	Destroy all objects in the game
*/
/******************************************************************************/
void GameStateMainMenuFree(void) {
	AEGfxMeshFree(PlayButton.pMesh);
	AEGfxMeshFree(ExitButton.pMesh);
}

/******************************************************************************/
/*!
	Free all memory
*/
/******************************************************************************/
void GameStateMainMenuUnload(void) {

}

