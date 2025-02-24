#include "AudioManager.h"
#include <fmod.hpp>
#include <map>
#include "AEEngine.h"
#include <vector>

namespace {
	static const s32 s_MaxChannels = 32;
	static const f32 f_MinVolume = 0.f, f_MaxVolume = 1.f;
	f32 ConvertVolume(f32 f_Volume) {
		if (f_Volume < f_MinVolume) return f_MinVolume;
		if(f_Volume > f_MaxVolume) return f_MaxVolume;
		return f_Volume;
	}

	bool playBGMAudio = true;
}

namespace AudioManager {
	static std::map<std::string, FMOD::Sound*> SoundMap;
	static std::vector<FMOD::Channel*> sfxList;
	static std::map<std::string, FMOD::Channel*> sfxMap;
	FMOD::Channel* bgm;
	static FMOD::System* mpSystem;
	static f32 f_BGMVolume = 0.4f, f_SfxVolume = 0.4f;

	static FMOD::Sound* TryLoadSound(std::string const& s_Name, bool b_Loop);

	void Init() {
		mpSystem = NULL;
		if (FMOD::System_Create(&mpSystem) != FMOD_OK) return;

		s32 driverCount;
		mpSystem->getNumDrivers(&driverCount);
		if (driverCount == 0) return;
		mpSystem->init(s_MaxChannels, FMOD_INIT_NORMAL, 0);

		if (playBGMAudio)
		{
			// TODO: Load all sounds (add sound path here to try load)
			//TryLoadSound("filepath", false);

			//PlayBGM("filepath to bgm");

			PlayBGM("Assets/AudioBGM/Retro_Funk_20_by_sonically_sound_Id_624874.wav");
		}
	}

	void Update() {
		for (std::vector<FMOD::Channel*>::iterator it = sfxList.begin(); it != sfxList.end();) {
			bool b_IsPlaying = false;
			(*it)->isPlaying(&b_IsPlaying);
			if (b_IsPlaying) it = sfxList.erase(it);
			else ++it;
		}
		mpSystem->update();
	}

	void Exit() {
		if (mpSystem != NULL) mpSystem->release();
	}

	static FMOD::Sound* TryLoadSound(std::string const& s_Name, bool b_Loop) {
		std::map<std::string, FMOD::Sound*>::iterator sound = SoundMap.find(s_Name);
		if (sound == SoundMap.end()) {
			FMOD_MODE mode = FMOD_DEFAULT;
			FMOD::Sound* loaded = nullptr;
			mpSystem->createSound(s_Name.c_str(), mode, nullptr, &loaded);
			if (loaded) {
				SoundMap[s_Name] = loaded;
				if (b_Loop) {
					loaded->setMode(FMOD_LOOP_NORMAL);
					loaded->setLoopCount(-1);
				}
				return loaded;
			}
		}
		else { return sound->second; }

		AE_ASSERT_MESG(0, "Failed to load sound: %s", s_Name.c_str());
		return nullptr;
	}

	void UnloadSound(std::string const& s_Name) {
		std::map<std::string, FMOD::Sound*>::iterator sound = SoundMap.find(s_Name);
		if (sound == SoundMap.end()) return;

		sound->second->release();
		SoundMap.erase(sound);
	}

	void PlayBGM(std::string const& s_Name) {
		if (bgm) {
			bgm->setPaused(true);
			bgm = nullptr;
		}

		FMOD::Sound* sound = TryLoadSound(s_Name, true);
		mpSystem->playSound(sound, nullptr, true, &bgm);
		if (bgm) {
			bgm->setVolume(ConvertVolume(f_BGMVolume));
			bgm->setPaused(false);
		}
	}

	void PlayAudio(std::string const& s_Name, f32 f_Volume, bool b_Loop) {
		if (b_Loop) {
			std::map<std::string, FMOD::Channel*>::iterator it = sfxMap.find(s_Name);
			if (it != sfxMap.end()) {
				it->second->setVolume(ConvertVolume(f_SfxVolume * f_Volume));
				it->second->setPaused(false);
				return;
			}

			if (sfxMap.size() + sfxList.size() + 1 >= s_MaxChannels) return;

			FMOD::Sound* sound = TryLoadSound(s_Name, b_Loop);
			if (sound != nullptr) {
				FMOD::Channel* p_Channel = nullptr;
				mpSystem->playSound(sound, nullptr, true, &p_Channel);

				if (p_Channel) {
					p_Channel->setVolume(ConvertVolume(f_SfxVolume * f_Volume));
					p_Channel->setPaused(false);
				}

				if (b_Loop) sfxMap[s_Name] = p_Channel;
				else sfxList.push_back(p_Channel);
			}
		}
	}

	void PauseAudio(std::string const& s_Name) {
		std::map<std::string, FMOD::Channel*>::iterator it = sfxMap.find(s_Name);
		if (it != sfxMap.end()) it->second->setPaused(true);
	}

	void SetBGMVolume(f32 f_Percentage) {
		f_BGMVolume = f_Percentage;
	}

	void SetSfxVolume(f32 f_Percentage) {
		f_SfxVolume = f_Percentage;
	}
}
