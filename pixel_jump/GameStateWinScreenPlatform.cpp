/******************************************************************************/
/*!
	Include Files
*/
/******************************************************************************/
#include "main.h"
#include "hpbar.hpp"

struct GameItems
{
	AEGfxVertexList* pMesh{ nullptr };
	AEMtx33 scale{ 0 }, rotate{ 0 }, translate{ 0 }, transform{ 0 };
};
GameItems RestartButton{}, ExitButton;
float ButposX[99] = { 0 }, ButposY[99] = { 0 };
s32 keyposX, keyposY;
/******************************************************************************/
/*!
	Load meshes,particle,font and map. Allocate memory for game object
*/
/******************************************************************************/
void GameStateWinLoad(void) {
	AESysSetWindowTitle("Win");
	AudioManager::PlayBGM("Assets/AudioBGM/Sadness_Cosmo_by_Bruno_ph_Id_109409_trim.wav");
	// load the font
	g_Font = AEGfxCreateFont("Assets/Fonts/Arial Italic.ttf", 30);
}

/******************************************************************************/
/*!
	Initialize all varaibles and funtions
*/
/******************************************************************************/
void GameStateWinInit(void) {
	
	RestartButton.pMesh = mesh_create_square(4278255360);
	ExitButton.pMesh = mesh_create_square(4294901760);
	ButposX[0] = 0;
	ButposY[0] = 100;
	ButposX[1] = 0;
	ButposY[1] = -100;
}

/******************************************************************************/
/*!
	Update function of the state, calculate position and logics for the objects
*/
/******************************************************************************/
void GameStateWinUpdate(void) {
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

	AEInputGetCursorPosition(&keyposX, &keyposY);

	if (AEInputCheckTriggered(RI_MOUSE_LEFT_BUTTON_DOWN))
	{
		if (keyposX > ButposX[0] + 700 && keyposX < ButposX[0] + 900
			&& keyposY > ButposY[0] + 200 && keyposY < ButposY[0] + 300)
		{
			GameStateManager::g_GameStateNext = States::GS_PLATFORM_LVL1;
		}
		if (keyposX > ButposX[1] + 700 && keyposX < ButposX[1] + 900
			&& keyposY > ButposY[1] + 600 && keyposY < ButposY[1] + 700)
		{
			GameStateManager::g_GameStateNext = States::GS_MAINMENU;
		}
	}
}

/******************************************************************************/
/*!
	Draws all the objects, particle and text for the game
*/
/******************************************************************************/
void GameStateWinDraw(void) {
	//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	mesh_square_transform(ButposX[0], ButposY[0], 200, 100, 0, RestartButton.scale, RestartButton.rotate, RestartButton.translate, RestartButton.transform, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	//mesh_square_transform(0, 150, 200, 100, 0, RestartButton.scale, RestartButton.rotate, RestartButton.translate, RestartButton.transform, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	AEGfxMeshDraw(RestartButton.pMesh, AE_GFX_MDM_TRIANGLES);

	mesh_square_transform(ButposX[1], ButposY[1], 200, 100, 0, ExitButton.scale, ExitButton.rotate, ExitButton.translate, ExitButton.transform, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	AEGfxMeshDraw(ExitButton.pMesh, AE_GFX_MDM_TRIANGLES);


	AEGfxPrint(g_Font, "YOU WON! :)", -0.23f, 0.6f, 2.5f, 1.f, 1.f, 1.f, 1.f);
	AEGfxPrint(g_Font, "Replay", -0.09f, 0.20f, 1.5f, 1.f, 1.f, 1.f, 1.f);
	AEGfxPrint(g_Font, "  Quit", -0.09f, -0.25f, 1.5f, 1.f, 1.f, 1.f, 1.f);
}

/******************************************************************************/
/*!
	Destroy all objects when function is called
*/
/******************************************************************************/
void GameStateWinFree(void) {
	AEGfxMeshFree(RestartButton.pMesh);
	AEGfxMeshFree(ExitButton.pMesh);
	// Releases resources used by a font
	AEGfxDestroyFont(g_Font);
}

/******************************************************************************/
/*!
	Free all memory when function is called
*/
/******************************************************************************/
void GameStateWinUnload(void) {

}
