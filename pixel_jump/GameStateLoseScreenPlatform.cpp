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
	RestartButton.pMesh = mesh_create_square();
	ExitButton.pMesh = mesh_create_square();
	BtnposX[0] = 0;
	BtnposY[0] = 150;
	BtnposX[1] = 0;
	BtnposY[1] = -50;

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

	AEInputGetCursorPosition(&mouseposX, &mouseposY);

	if (AEInputCheckTriggered(RI_MOUSE_LEFT_BUTTON_DOWN))
	{
		if (mouseposX > BtnposX[0] + 700 && mouseposX < BtnposX[0] + 900
			&& mouseposY > BtnposY[0] + 100 && mouseposY < BtnposY[0] + 200)
		{
			GameStateManager::g_GameStateNext = States::GS_PLATFORM_LVL1;
		}
		if (mouseposX > BtnposX[1] + 700 && mouseposX < BtnposX[1] + 900
			&& mouseposY > BtnposY[1] + 500 && mouseposY < BtnposY[1] + 600)
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
void GameStateLoseDraw(void) {
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);


	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	mesh_square_transform(BtnposX[0], BtnposY[0], 200, 100, 0, RestartButton.scale, RestartButton.rotate, RestartButton.translate, RestartButton.transform, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	//mesh_square_transform(0, 150, 200, 100, 0, RestartButton.scale, RestartButton.rotate, RestartButton.translate, RestartButton.transform, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	AEGfxMeshDraw(RestartButton.pMesh, AE_GFX_MDM_TRIANGLES);

	mesh_square_transform(BtnposX[1], BtnposY[1], 200, 100, 0, ExitButton.scale, ExitButton.rotate, ExitButton.translate, ExitButton.transform, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	AEGfxMeshDraw(ExitButton.pMesh, AE_GFX_MDM_TRIANGLES);

}

/******************************************************************************/
/*!
	Destroy all objects when function is called
*/
/******************************************************************************/
void GameStateLoseFree(void) {
	AEGfxMeshFree(RestartButton.pMesh);
	AEGfxMeshFree(ExitButton.pMesh);
}

/******************************************************************************/
/*!
	Free all memory when function is called
*/
/******************************************************************************/
void GameStateLoseUnload(void) {

}
