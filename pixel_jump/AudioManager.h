#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <AETypes.h>
#include <string>

namespace AudioManager {
	void Init();
	void Exit();
	void Update();

	void UnloadSound(std::string const& s_Name);
	void PlayBGM(std::string const& s_Name);
	void PlayAudio(std::string const& s_name, f32 f_vol, bool b_isLoop = false);
	void PauseAudio(std::string const& s_Name);
	void SetBGMVolume(f32 f_Percentage);
	void SetSfxVolume(f32 f_Percentage);

	const f32 f_JumpVolume = 0.6f;
}


#endif // AUDIO_MANAGER_H
