#include "main.h"

namespace {
	static f32 f_CreditsScroll;
	static f32 f_CreditsScrollTime;
	static f32 f_MaxCreditsScrollTime = 0.25f;
	static bool b_CreditsState;
}

void GameStateCreditsLoad() {

}

void GameStateCreditsInit() {

}

void GameStateCreditsUpdate() {
	f_CreditsScroll += g_dt;
	f_CreditsScrollTime += g_dt;

	if (f_CreditsScrollTime > f_MaxCreditsScrollTime) {
		f_CreditsScrollTime = 0.f;
		b_CreditsState = 0;
	}


}

void GameStateCreditsDraw() {

}

void GameStateCreditsFree() {

}

void GameStateCreditsUnload() {

}
