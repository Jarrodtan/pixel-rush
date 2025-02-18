/**************************************************************************/
/*!
	Include Files
*/
/**************************************************************************/
#include "main.h"
#include <memory>

/**************************************************************************/
/*!
	Global Variables
*/
/**************************************************************************/
f32 g_dt;
f64 g_appTime;
s8 g_Font;
AEGfxTexture** g_Texture;
AEGfxVertexList** g_Shapes;

/******************************************************************************/
/*!
	Starting point of the application
*/
/******************************************************************************/
int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
	UNREFERENCED_PARAMETER(prevInstanceH);
	UNREFERENCED_PARAMETER(command_line);

	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// Initialize the system
	AESysInit(instanceH, show, 1600, 900, 1, 60, false, NULL);
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	Render::Load();
	AudioManager::Init();
	GameStateManager::GameStateMgrInit(States::GS_PLATFORM_LVL1);

	while (GameStateManager::g_GameStateCurr != States::GS_QUIT)
	{
		// reset the system modules
		AESysReset();

		// If not restarting, load the gamestate
		if (GameStateManager::g_GameStateCurr != States::GS_RESTART)
		{
			GameStateManager::GameStateMgrUpdate();
			GameStateManager::GameStateLoad();
		}
		else
			GameStateManager::g_GameStateNext = GameStateManager::g_GameStateCurr = GameStateManager::g_GameStatePrev;

		// Initialize the gamestate
		GameStateManager::GameStateInit();

		while (GameStateManager::g_GameStateCurr == GameStateManager::g_GameStateNext)
		{
			AESysFrameStart();

			AudioManager::Update();

			GameStateManager::GameStateUpdate();

			GameStateManager::GameStateDraw();

			AESysFrameEnd();

			// check if forcing the application to quit
			if (AESysDoesWindowExist() == false) //((AESysDoesWindowExist() == false) || AEInputCheckTriggered(AEVK_ESCAPE))
				GameStateManager::g_GameStateNext = States::GS_QUIT;

			g_dt = (f32)AEFrameRateControllerGetFrameTime();

			//capping the game loop - delta time, to 1/60.0f
			if (g_dt > 0.01667f)	//0.01667f = 1/60.0f
				g_dt = 0.01667f;

			g_appTime += g_dt;
		}

		GameStateManager::GameStateFree();

		if (GameStateManager::g_GameStateNext != States::GS_RESTART)
			GameStateManager::GameStateUnload();

		GameStateManager::g_GameStatePrev = GameStateManager::g_GameStateCurr;
		GameStateManager::g_GameStateCurr = GameStateManager::g_GameStateNext;
	}

	Render::Exit();
	AudioManager::Exit();

	// free the system
	AESysExit();
}