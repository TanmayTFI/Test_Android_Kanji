//====================================
//DQFULP
// Audio Sample
//====================================

#ifndef DQFULP_AUDIOSAMPLE_DEF
#define DQFULP_AUDIOSAMPLE_DEF

#include "Framework.h"
#include "AudioData.h"
#include "UpdateListEntity.h"
#include "KSound.h"

enum eSampleState
{
	Sample_FadeIn,
	Sample_Normal,
	Sample_FadeOut
};

class AudioSample //: public UpdateListEntity
{
	public:
		eAudioType		AudioType;
		AudioSample();
		~AudioSample();

		bool Is_TrackPlaying_in_iPad; 
		eAudio  iAudioSample;
		void Load(eAudio sample, eAudioType sampleType, bool sfxLoop = false, int sfxChannels = 1, std::string audPath="");
		void Unload();

		bool IsSamplePlaying();
		void Play(bool loopStream=false);
		void Stop();
		void Pause(bool Status);

		//Effect next channel 'played' or the whole sound??		
		void SetVolume(float vol); 
		void ResetVolume();

		void FadeIn(float speed=1.0f);
		void FadeOut(float speed=1.0f, bool unload=true);

		KSound			*RawSound;
		float			Volume; //Only makes sense with a single channel
		
		
	private:
		
		
		eSampleState	SampleState;
		bool			UnloadOnFade;
		float			Speed;

		float			RandTimer;
		float			RandUpdate;
		bool			RandOn;

		bool			Looping;
		bool			IsPlaying;

};

#endif