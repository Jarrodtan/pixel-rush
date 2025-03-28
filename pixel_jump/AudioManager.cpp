/*******************************************************************/
/*!
	File:		AudioManager.cpp
	Project:	Pixel Rush
	Author(s):  Lau Xiang Yang

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

/**************************************************************************/
/*!
	Include Files
*/
/**************************************************************************/
#include "AudioManager.h"
#include <fmod.hpp>
#include <map>
#include "main.h"
//-------------------------------------------------------------------------/

/**************************************************************************/
/*!
	Anonymous Namespace
*/
/**************************************************************************/
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
//-------------------------------------------------------------------------/

/**************************************************************************/
/*!
	Namespace for Audio Manager
*/
/**************************************************************************/
namespace AudioManager {
	static std::map<std::string, FMOD::Sound*> SoundMap;
	static std::vector<FMOD::Channel*> sfxList;
	static std::map<std::string, FMOD::Channel*> sfxMap;
	FMOD::Channel* bgm;
	static FMOD::System* mpSystem;
	static f32 f_BGMVolume = 0.4f, f_SfxVolume = 0.4f;

	static FMOD::Sound* TryLoadSound(std::string const& s_Name, bool b_Loop);

	/******************************************************************************/
	/*!
		Function to init the audio manager
		Call this function in main.cpp before the game loop.

		\param
			none

		\return
			none
	*/
	/******************************************************************************/
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
			
			TryLoadSound("Assets/AudioSFX/Jump_retro_up.wav", false);
			TryLoadSound("Assets/AudioSFX/Jump_retro_land.wav", false);

			TryLoadSound("Assets/AudioBGM/Retro_Funk_20_by_sonically_sound_Id_624874.wav", true);
			TryLoadSound("Assets/AudioBGM/Sadness_Cosmo_by_Bruno_ph_Id_109409_trim.wav", true);

			PlayBGM("Assets/AudioBGM/Retro_Funk_20_by_sonically_sound_Id_624874.wav");
		}
	}

	/******************************************************************************/
	/*!
		Function to update the audio manager
		Call this function in main.cpp in the game loop.

		\param
			none

		\return
			none
	*/
	/******************************************************************************/
	void Update() {
		for (std::vector<FMOD::Channel*>::iterator it = sfxList.begin(); it != sfxList.end();) {
			bool b_IsPlaying = false;
			(*it)->isPlaying(&b_IsPlaying);
			if (b_IsPlaying) it = sfxList.erase(it);
			else ++it;
		}
		mpSystem->update();
	}

	/******************************************************************************/
	/*!
		Function to exit the audio manager
		Call this function in main.cpp after the game loop.

		\param
			none

		\return
			none
	*/
	/******************************************************************************/
	void Exit() {
		if (mpSystem != NULL) mpSystem->release();
	}

	/******************************************************************************/
	/*!
		Function to try load a sound
		Call this function in main.cpp to load a sound.

		\param s_Name
			the name of the sound to load

		\param b_Loop
			true to loop the sound, false otherwise

		\return
			the loaded sound
	*/
	/******************************************************************************/
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

		return nullptr;
	}

	/******************************************************************************/
	/*!
		Function to unload a sound

		\param s_Name
			the name of the sound to unload

		\return
			none
	*/
	/******************************************************************************/
	void UnloadSound(std::string const& s_Name) {
		std::map<std::string, FMOD::Sound*>::iterator sound = SoundMap.find(s_Name);
		if (sound == SoundMap.end()) return;

		sound->second->release();
		SoundMap.erase(sound);
	}

	/******************************************************************************/
	/*!
		Function to play a background music

		\param s_Name
			the name of the background music to play

		\return
			none
	*/
	/******************************************************************************/
	void PlayBGM(std::string const& s_Name) {
		static std::string s_CurrentBGM{};

		if (s_Name == s_CurrentBGM) return;

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

		s_CurrentBGM = s_Name;
	}

	/******************************************************************************/
	/*!
		Function to play an audio

		\param s_Name
			the name of the audio to play

		\param f_Volume
			the volume of the audio

		\param b_Loop
			true to loop the audio, false otherwise

		\return
			none
	*/
	/******************************************************************************/
	void PlayAudio(std::string const& s_Name, f32 f_Volume, bool b_Loop) {
		if (b_Loop) {
			std::map<std::string, FMOD::Channel*>::iterator it = sfxMap.find(s_Name);
			if (it != sfxMap.end()) {
				it->second->setVolume(ConvertVolume(f_Volume * f_SfxVolume));
				it->second->setPaused(false);
				return;
			}
		}
		if (sfxMap.size() + sfxList.size() + 1 >= s_MaxChannels) return;

		FMOD::Sound* sound = TryLoadSound(s_Name, b_Loop);

		if (sound != nullptr) {
			FMOD::Channel* p_Channel = nullptr;
			mpSystem->playSound(sound, nullptr, true, &p_Channel);
			if (p_Channel) {
				p_Channel->setVolume(ConvertVolume(f_Volume * f_SfxVolume));
				p_Channel->setPaused(false);
			}
			if (b_Loop) sfxMap[s_Name] = p_Channel;
			else sfxList.push_back(p_Channel);
		}
	}

	/******************************************************************************/
	/*!
		Function to pause an audio

		\param s_Name
			the name of the audio to pause

		\return
			none
	*/
	/******************************************************************************/
	void PauseAudio(std::string const& s_Name) {
		std::map<std::string, FMOD::Channel*>::iterator it = sfxMap.find(s_Name);
		if (it != sfxMap.end()) it->second->setPaused(true);
	}

	/******************************************************************************/
	/*!
		Function to stop an audio

		\param s_Name
			the name of the audio to stop

		\return
			none
	*/
	/******************************************************************************/
	void SetBGMVolume(f32 f_Percentage) {
		f_BGMVolume = f_Percentage;
	}

	/******************************************************************************/
	/*!
		Function to set the sfx volume

		\param f_Percentage
			the percentage of the sfx volume

		\return
			none
	*/
	/******************************************************************************/
	void SetSfxVolume(f32 f_Percentage) {
		f_SfxVolume = f_Percentage;
	}
}
//-------------------------------------------------------------------------/