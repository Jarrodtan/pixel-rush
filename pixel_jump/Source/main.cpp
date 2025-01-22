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
	//Create your font here, and use it for all your levels
	g_Font = AEGfxCreateFont("Assets/Fonts/Arial Italic.ttf", 30);// load font 

	GameStateMgrInit(GS_PLATFORM_LVL1);

	while (g_GameStateCurr != GS_QUIT)
	{
		// reset the system modules
		AESysReset();

		// If not restarting, load the gamestate
		if (g_GameStateCurr != GS_RESTART)
		{
			GameStateMgrUpdate();
			GameStateLoad();
		}
		else
			g_GameStateNext = g_GameStateCurr = g_GameStatePrev;

		// Initialize the gamestate
		GameStateInit();

		while (g_GameStateCurr == g_GameStateNext)
		{
			AESysFrameStart();

			GameStateUpdate();

			GameStateDraw();

			AESysFrameEnd();

			// check if forcing the application to quit
			if (AESysDoesWindowExist() == false) //((AESysDoesWindowExist() == false) || AEInputCheckTriggered(AEVK_ESCAPE))
				g_GameStateNext = GS_QUIT;

			g_dt = (f32)AEFrameRateControllerGetFrameTime();

			//capping the game loop - delta time, to 1/60.0f
			if (g_dt > 0.01667f)	//0.01667f = 1/60.0f
				g_dt = 0.01667f;

			g_appTime += g_dt;
		}

		GameStateFree();

		if (g_GameStateNext != GS_RESTART)
			GameStateUnload();

		g_GameStatePrev = g_GameStateCurr;
		g_GameStateCurr = g_GameStateNext;
	}

	//free you font here
	AEGfxDestroyFont(g_Font);// free font

	// free the system
	AESysExit();
}