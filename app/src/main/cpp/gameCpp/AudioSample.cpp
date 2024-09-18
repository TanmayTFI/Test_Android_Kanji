//====================================
//DQFULP
// Audio Sample
//====================================

#include "AudioSample.h"
#include "Console.h"
#include "Resources.h"
#include "exRandom.h"
#include "ProfileSystem.h"
#include "Constants.h"
#include "Util.h"
#include "Control.h"
#include "iPadDefs.h"
#include "GameFlow.h"

// #define min(a,b)            (((a) < (b)) ? (a) : (b))
//Sac: why redefine min ?
AudioSample::AudioSample()
{
	RawSound = NULL;
	iAudioSample = Audio_Nil;
	Is_TrackPlaying_in_iPad = false;

	AudioType = AudioUndefined;
	SampleState = Sample_Normal;
	Volume = 100.0f;
	UnloadOnFade = false;
	Speed = 1.0f;
	Looping = false;
	IsPlaying = false;
}

AudioSample::~AudioSample()
{
	Unload();
}



void AudioSample::Load(eAudio sample, eAudioType sampleType, bool sfxLoop, int sfxChannels, std::string audPath)
{
	if( RawSound != NULL )//Avoid multiple loading!!
	{
		//PRINT("Unable to load audio sample that already exists.");
		return;
	}

#ifdef K_WIN32
	if( KSound::g_bErrSoundDeviceInit )//AudioUndefined
		return;
#endif
	
	RawSound = new KSound;		
	
	AudioType = sampleType;
	iAudioSample = sample;
	Is_TrackPlaying_in_iPad = false;

	const char* AudioFileStr = AudioFiles[sample];	

	if(audPath != "")
		AudioFileStr = audPath.c_str();

	if( AudioType == AudioSFX )
	{
		Looping = sfxLoop;

		if( sfxChannels > 1 && sfxLoop  )
		#ifdef K_WIN32
			WARN("Er0x1201- Audio sample is being loaded to loop with multiple channels.");////Removed Wide string conversion for Windows	
		#else
			WARN((const char*)"Er0x1201- Audio sample is being loaded to loop with multiple channels.");
		#endif
		
		if( !RawSound->loadSample( FileSystem::charPath(AudioFileStr), 100, sfxLoop, sfxChannels) )
		{
		#ifndef K_WIN32
			{
				delete RawSound;
				std::string TmpStr = std::string("Er0x1202- Audio sample failed to open file: " + MKSTR(AudioFileStr)).c_str();		
				FATAL( (const char*)(TmpStr.c_str()) );	
			}
		#else
			{
				if( KSound::g_bErrSoundDeviceInit )//No need to delete since sample not loaded!!
					PRINT("Error sound initializing: Please install proper drivers or select another device in configuration" );			
				else
				{
					delete RawSound;		
					std::string TmpStr = std::string("Er0x1202- Audio sample failed to open file: " + MKSTR(AudioFileStr)).c_str();		
					//FATAL( (const char*)(TmpStr).c_str());			
					FATAL(TmpStr.c_str());
				}
			}
		#endif
			RawSound = NULL;
			iAudioSample = Audio_Nil;
			Is_TrackPlaying_in_iPad = false;
			return;
		}
		if( ProfileSystem::Instance->ActiveProfile.IsMuted )
			RawSound->setVolume(0);
		else if( Control::Audio->GlobalSFXVol < 100 )
			RawSound->setVolume(Control::Audio->GlobalSFXVol);
	}
	else if( AudioType == AudioVO )
	{
		Looping = sfxLoop;

		if( sfxChannels > 1 && sfxLoop  )
			#ifdef K_WIN32
				WARN("Er0x1203- Audio sample is being loaded to loop with multiple channels.");////Removed Wide string conversion for Windows	
			#else
				WARN((const char*)"Er0x1203- Audio sample is being loaded to loop with multiple channels.");
			#endif
			
		
		if( !RawSound->loadSample( FileSystem::charPath(AudioFileStr), 100, sfxLoop, sfxChannels) )
		{
		#ifndef K_WIN32
			{
				delete RawSound;
				std::string TmpStr = std::string("Er0x1204- Audio sample failed to open file: " + MKSTR(AudioFileStr)).c_str();		
				FATAL( (const char*)(TmpStr.c_str()) );//WIDESTR WARNINGS		
				
			}
		#else //K_WIN32
			{
				if( KSound::g_bErrSoundDeviceInit )//No need to delete since sample not loaded!!
					PRINT("Error sound initializing: Please install proper drivers or select another device in configuration" );			
				else
				{
					delete RawSound;		
					std::string TmpStr = std::string("Er0x1204- Audio sample failed to open file: " + MKSTR(AudioFileStr)).c_str();		
					FATAL((TmpStr).c_str());//WIDESTR WARNINGS		
					
				}
			}
		#endif
			RawSound = NULL;
			iAudioSample = Audio_Nil;
			Is_TrackPlaying_in_iPad = false;
			return;
		}
		if( ProfileSystem::Instance->ActiveProfile.IsMuted )
			RawSound->setVolume(0);
		else if( Control::Audio->GlobalVoiceVol < 100 )
			RawSound->setVolume(Control::Audio->GlobalVoiceVol);
	}
	else //AudioTrack, AudioAmb
	{
		//looping decided at play time
		//Looping = sfxLoop;

		if( !RawSound->loadStream( FileSystem::charPath(AudioFileStr) ) )
		{
			delete RawSound;
			RawSound = NULL;
			iAudioSample = Audio_Nil;
			Is_TrackPlaying_in_iPad = false;
			
			std::string TmpStr = std::string("Er0x1205- Audio sample failed to open file: " + MKSTR(AudioFileStr)).c_str();		
			#ifdef K_WIN32
				FATAL((TmpStr).c_str());//WIDESTR WARNINGS		
            #else
				FATAL( (const char*)(TmpStr.c_str()) );//WIDESTR WARNINGS		
			#endif
			return;
		}		
		//Set volume based on master settings !!
		if( ProfileSystem::Instance->ActiveProfile.IsMuted )
			RawSound->setVolume(0);		
		else if( AudioType == AudioAmb )
		{
			if( Control::Audio->GlobalSFXVol < 100 )		
				RawSound->setVolume(Control::Audio->GlobalSFXVol);
		}
		else //audio stream
		{
			if( Control::Audio->GlobalMusicVol < 100 )
				RawSound->setVolume(Control::Audio->GlobalMusicVol);
		}
	}
}

void AudioSample::Unload()
{

	if( RawSound != NULL ) 
	{
		if( AudioType == AudioTrack )
		{
			RawSound->stopStream();
			RawSound->freeSound();
		}
		else if( AudioType == AudioAmb )
		{
			RawSound->stopStream();//if not yet!!
			RawSound->freeSound();
		}
		delete RawSound;
		RawSound = NULL;
		iAudioSample = Audio_Nil;
		Is_TrackPlaying_in_iPad = false;
	}
}

bool AudioSample::IsSamplePlaying()
{
	if( RawSound == NULL )//not yet loaded!!
		return false;
	else
	{
		IsPlaying = RawSound->isPlaying();
		return IsPlaying;
	}	
}

void AudioSample::Pause( bool Status )
{
	if( RawSound == NULL )
		return;
	RawSound->pause(Status);
}

void AudioSample::Play(bool loopStream)
{
	if( RawSound == NULL )
		return;
	

	IsPlaying = true;

	if( AudioType == AudioSFX )
	{
		RawSound->playSample();
	}
	else if( AudioType == AudioVO )
	{
		RawSound->playSample();
	}
	else //AudioTrack, AudioAmb
	{		
		Looping = loopStream;
		RawSound->playStream(loopStream);
	}
}


void AudioSample::Stop()
{
	if( RawSound == NULL )
		return;
	

	if( AudioType == AudioSFX )
	{
		RawSound->stopSample();
	}
	else if( AudioType == AudioVO )
	{
		RawSound->stopSample();
	}
	else if( IsPlaying )//AudioTrack, AudioAmb
	{
		RawSound->stopStream();
	}
	IsPlaying = false;
}


void AudioSample::SetVolume(float vol)
{
	if( RawSound == NULL )
		return;
//Sac: changed min to g_min
	Volume = vol;
	if( AudioType == AudioSFX || AudioType == AudioAmb )
		Volume = g_min(Control::Audio->GlobalSFXVol, Volume);
	else if( AudioType == AudioVO )
		Volume = g_min(Control::Audio->GlobalVoiceVol, Volume);
	else //AudioTrack
		Volume = g_min(Control::Audio->GlobalMusicVol, Volume);
	if( !ProfileSystem::Instance->ActiveProfile.IsMuted )
	{
		if( AudioType == AudioTrack )
		{
			RawSound->setVolume((int)Volume);
		}
		else
			RawSound->setVolume((int)Volume);
	}
}

void AudioSample::ResetVolume()
{
	if( RawSound == NULL )
		return;
	//Reset to last volume!!
	if( !ProfileSystem::Instance->ActiveProfile.IsMuted )
	{
		if( AudioType == AudioTrack )
		{
			RawSound->setVolume((int)Volume);		
		}
		else
			RawSound->setVolume((int)Volume);		
	}
}

void AudioSample::FadeIn(float speed)
{
	SetVolume(0);
	Speed = speed;
	SampleState = Sample_FadeIn;
}

void AudioSample::FadeOut(float speed, bool unload)
{
	Speed = speed;
	UnloadOnFade = unload;
	SampleState = Sample_FadeOut;
}

