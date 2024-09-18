//====================================
//DQFULP
// Audio Slave
//====================================

#ifndef DQFULP_AUDIOSLAVE_DEF
#define DQFULP_AUDIOSLAVE_DEF

#include "AudioSample.h"
#include "DataEnums.h"

#include "rapidjson/document.h"     // rapidjson's DOM-style API

using namespace rapidjson;

class AudioSlave
{
	public:
		bool Silent;
		bool IsLevelAssetsLoaded[Level_Max];
		AudioSlave();
		~AudioSlave();
		void PlayCurLevelTrack();
		void StopCurLevelTrack();

		//Quickly Load and Play Non Looping SFX
		void QuickLoadAndPlaySFX(eAudio sample);
		std::vector<int> LoadedSFXArray;
		void UnloadQuickLoadedSFX();

		//Json loaded samples
		void LoadLevelSFXList(int levelID);
		std::vector<AudioSample*> LoadedSampleArray;
		std::vector<std::string> LoadedSampleArrayKeys;
		void QuickLoadAndPlaySFX(const std::string& sampleKey, bool isLooping=false);
		void StopSFX(const std::string& sampleKey);
		
		void LoadSample(eAudio sample, eAudioType sampleType, bool sfxLoop = false, int sfxChannels = 1);
		void UnloadSample(eAudio sample);

		bool IsPlaying(const std::string& sampleKey);
		bool IsPlaying(eAudio sample);
		void PlaySample(eAudio sample, bool streamLoop = true);
		void StopSample(eAudio sample);

		void PlayHintFX();//hints in HO scenes
		void PlayCollectFX();//small achievement
		void PlayFootStepsFX();

		bool IsToLevelTrackRoom();
		
		
			

		void FadeInSample(eAudio sample, float speed=1.0f);
		void FadeOutSample(eAudio sample, float speed=1.0f, bool unload = true);

		void SetGlobalVolumes(int sfxVol, int mscVol, int voVol, bool IsMute = false );
		void SetGlobalMusicVolume(int mscVol);
		void SetGlobalSFXVolume(int sfxVol);
		void SetGlobalVoiceVolume(int voVol);
		
		void UnloadAllSamples();

		void Silence();
		void EndSilence();

		int CollectSFX, FootStepsSFX,HintSFX;
		
		int GlobalSFXVol, GlobalMusicVol, GlobalVoiceVol;

		eAudio PrevTrack1, PrevTrack2, PrevTrack3, PrevTrack4, PrevTrack5, PrevTrack6, PrevTrack7; 
		void ClearUnusedTracks();
		void TurnOffLvlMscTracks();
		AudioSample *Samples[Audio_Max];		

	private:		
		Document _jsonDocument;
		
};

#endif