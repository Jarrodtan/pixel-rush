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
GameEnity PlayButton{}, ExitButton, TutorialButton, CreditButton, OptionButton;
float BposX[99] = { 0 }, BposY[99] = { 0 };
s32 kposX, kposY;
bool haveOptionButton = true;
f32 delayClick;
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
	TutorialButton.pMesh = mesh_create_square(4294901760);
	CreditButton.pMesh = mesh_create_square(4278255360);
	OptionButton.pMesh = mesh_create_square(4294901760);
	BposX[0] = 0;
	BposY[0] = 100;
	BposX[1] = 0;
	BposY[1] = -300;
	BposX[2] = 0;
	BposY[2] = -100;

	BposX[3] = -700;
	BposY[3] = 350;

	BposX[4] = 700;
	BposY[4] = 350;

	delayClick = 0.5f;
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

	//if (AEInputCheckTriggered(RI_MOUSE_LEFT_BUTTON_DOWN))
	if(AEInputCheckCurr(RI_MOUSE_LEFT_BUTTON_DOWN) && delayClick <= 0)
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
		if (kposX > BposX[2] + 700 && kposX < BposX[2] + 900
			&& kposY > BposY[2] + 600 && kposY < BposY[2] + 700)
		{
			// go to tutorial stage...
			GameStateManager::g_GameStateNext = States::GS_PLATFORM_LVL2;
		}

		if ((kposX > BposX[0] + 50 && kposX < BposX[0] + 150
			&& kposY > BposY[0] - 50 && kposY < BposY[0] + 50) && haveOptionButton)
		{
			// go to game settings scene...
			GameStateManager::g_GameStateNext = States::GS_LOSE;
		}

		if (kposX > BposX[0] + 1450 && kposX < BposX[0] + 1550
			&& kposY > BposY[0] - 50 && kposY < BposY[0] + 50)
		{
			// go to game credits scene...
			GameStateManager::g_GameStateNext = States::GS_WIN;
		}
	}

	if (delayClick > 0)
	{
		delayClick -= 0.01f;
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

	mesh_square_transform(BposX[2], BposY[2], 200, 100, 0, TutorialButton.scale, TutorialButton.rotate, TutorialButton.translate, TutorialButton.transform, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f);
	AEGfxMeshDraw(TutorialButton.pMesh, AE_GFX_MDM_TRIANGLES);

	if (haveOptionButton) {
		mesh_square_transform(BposX[3], BposY[3], 100, 100, 0, OptionButton.scale, OptionButton.rotate, OptionButton.translate, OptionButton.transform, 0.2f, 0.3f, 0.0f, 0.0f, 1.0f);
		AEGfxMeshDraw(OptionButton.pMesh, AE_GFX_MDM_TRIANGLES);
	}

	mesh_square_transform(BposX[4], BposY[4], 100, 100, 0, CreditButton.scale, CreditButton.rotate, CreditButton.translate, CreditButton.transform, 0.0f, 0.8f, 0.3f, 0.0f, 1.0f);
	AEGfxMeshDraw(CreditButton.pMesh, AE_GFX_MDM_TRIANGLES);


	AEGfxPrint(g_Font, "PIXEL RUSH", -0.26f, 0.6f, 2.5f, 1.f, 1.f, 1.f, 1.f);
	AEGfxPrint(g_Font, "Play", -0.06f, 0.20f, 1.5f, 1.f, 1.f, 1.f, 1.f);
	AEGfxPrint(g_Font, "  Exit", -0.09f, -0.7f, 1.5f, 1.f, 1.f, 1.f, 1.f);
	AEGfxPrint(g_Font, "Tutorial", -0.09f, -0.25f, 1.5f, 1.f, 1.f, 1.f, 1.f);
}

/******************************************************************************/
/*!
	Destroy all objects in the game
*/
/******************************************************************************/
void GameStateMainMenuFree(void) {
	AEGfxMeshFree(PlayButton.pMesh);
	AEGfxMeshFree(ExitButton.pMesh);
	AEGfxMeshFree(TutorialButton.pMesh);
	if (haveOptionButton) {
		AEGfxMeshFree(OptionButton.pMesh);
	}
	AEGfxMeshFree(CreditButton.pMesh);
}

/******************************************************************************/
/*!
	Free all memory
*/
/******************************************************************************/
void GameStateMainMenuUnload(void) {

}

