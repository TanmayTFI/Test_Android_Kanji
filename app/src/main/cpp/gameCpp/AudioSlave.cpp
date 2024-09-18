//====================================
//DQFULP
// Audio Slave
//====================================

#include "AudioSlave.h"
#include "Console.h"
#include "GameFlow.h"
#include "Resources.h"
#include "ProfileSystem.h"


#define MAX_FOOTSTEPS_SFX 3

#define MAX_COLLECT_SFX  3
#define MAX_HINT_SFX 3

AudioSlave::AudioSlave()
{
	_jsonDocument = NULL;

	//7 tracks in first mansion outside scene
	PrevTrack1 = Audio_Nil; 
	PrevTrack2 = Audio_Nil;
	PrevTrack3 = Audio_Nil;
	PrevTrack4 = Audio_Nil;
	PrevTrack5 = Audio_Nil;
	PrevTrack6 = Audio_Nil;
	PrevTrack7 = Audio_Nil;

	for(int i=0; i < Audio_Max; ++i)
	{
		Samples[i] = new AudioSample();
	}
	for( int i = 0; i < Level_Max; i++ )
		IsLevelAssetsLoaded[i] = false;

	GlobalSFXVol = 100;
	GlobalMusicVol = 100;
	Silent = false;

	CollectSFX  = rand()%MAX_COLLECT_SFX;
	HintSFX	   = rand()%MAX_HINT_SFX;	
	FootStepsSFX = rand()%MAX_FOOTSTEPS_SFX;

}

AudioSlave::~AudioSlave()
{
	UnloadQuickLoadedSFX();

	for( int i=0; i < Audio_Max; i++ )
	{
		if( Samples[i]->AudioType != AudioUndefined )
			Samples[i]->Unload();
		delete Samples[i];
	}
}

void AudioSlave::PlayCurLevelTrack()
{
	switch( ProfileSystem::Instance->ActiveProfile.CurrentLevel )
	{
	default:
	case Level_1:if( !IsPlaying(aTrackL1Mansion) )
					 PlaySample(aTrackL1Mansion,true);
		break;
	case Level_2:if( !IsPlaying(aTrackLevel2) )
					 PlaySample(aTrackLevel2,true);
		break;			
	case Level_3:if( !IsPlaying(aTrackLevel3) )
					 PlaySample(aTrackLevel3,true);
		break;		
	case Level_4:break;		
	case Level_5:if( !IsPlaying(aTrackLevel5) )
					 PlaySample(aTrackLevel5,true);
		break;	
	case Level_6:break;
	case Level_7:break;
	case Level_8:break;

	}
}

void AudioSlave::StopCurLevelTrack()
{
	switch( ProfileSystem::Instance->ActiveProfile.CurrentLevel )
	{
	default:
	case Level_1:if( IsPlaying(aTrackL1Mansion) )
					 StopSample(aTrackL1Mansion);
		break;
	case Level_2: if( IsPlaying(aTrackLevel2) )
					  StopSample(aTrackLevel2);
		break;
	case Level_3: if( IsPlaying(aTrackLevel3) )
					  StopSample(aTrackLevel3);
		break;	
	case Level_4: break;
	case Level_5: if( IsPlaying(aTrackLevel5) )
					  StopSample(aTrackLevel5);
		break;
	case Level_6: break;
	case Level_7: break;
	}
}

void AudioSlave::LoadSample(eAudio sample, eAudioType sampleType, bool sfxLoop, int sfxChannels)
{
	Samples[sample]->Load(sample, sampleType, sfxLoop, sfxChannels);
}

void AudioSlave::LoadLevelSFXList(int levelID)
{
	std::string jsonFileStr="";

	switch (levelID)
	{
	case Level_1:
		jsonFileStr = "AUD\\L2\\SFXLIST.JSON";
		break;
	case Level_2:
		jsonFileStr = "AUD\\L2\\SFXLIST.JSON";
		break;
	case Level_3:
		jsonFileStr = "AUD\\L3\\SFXLIST.JSON";
		break;
	case Level_4:
		jsonFileStr = "AUD\\L4\\SFXLIST.JSON";
		break;
	case Level_5:
		jsonFileStr = "AUD\\L5\\SFXLIST.JSON";
		break;
	case Level_6:
		jsonFileStr = "AUD\\L6\\SFXLIST.JSON";
		break;
	case Level_7:
		jsonFileStr = "AUD\\L7\\SFXLIST.JSON";
		break;
	case Level_8:
		jsonFileStr = "AUD\\L8\\SFXLIST.JSON";
		break;
	default:
		jsonFileStr = "AUD\\L2\\SFXLIST.JSON";
		break;
	}

	#pragma region LOADING_FILE

	//Converts unsigned char_array to stream
	unsigned char *lpCompressedData = NULL;
	size_t nCompressedDataSize = 0;
	if(! KResource::loadResource (FileSystem::charPath(jsonFileStr), lpCompressedData, nCompressedDataSize) )
	{
		#ifdef K_WIN32
			WARN("Er0x1301- JSON failed to open");////Removed Wide string conversion for Windows	
		#else
			WARN((const char*)"Er0x1301- JSON failed to open");
		#endif
		return;
	}
	/* Copy contents and add a terminal 0 */

	char *_lpfileBytes = new char [nCompressedDataSize+1];
	memcpy (_lpfileBytes, lpCompressedData, nCompressedDataSize);//copy unsigned_chararray to const_chararray
	_lpfileBytes[nCompressedDataSize] = 0;
	std::stringstream stream(_lpfileBytes);//create stream from "const"_chararray 
	delete [] _lpfileBytes;
	_lpfileBytes = NULL;
	/////////////////////////////////////////////////////////////////////

	if( stream.fail() )
	{
		#ifdef K_WIN32
			WARN("Er0x1302- JSON failed to open");////Removed Wide string conversion for Windows	
		#else
			WARN((const char*)"Er0x1302- JSON failed to open");
		#endif
		//WARN(std::string("JSON Animation file failed to open: " + FileSystem::strPath(jsonFileStr)).c_str() );
		return;
	}  

	#pragma endregion


	//converting stream data to string
	std::string jsonString = stream.str();

	const char* cstr = jsonString.c_str();
	
	//_jsonDocument.Clear();
	_jsonDocument.Parse(cstr);
 
    /* Accessing values in document. */
	if(_jsonDocument.IsObject() )
	{
		assert(_jsonDocument.IsObject());    // Document is a JSON value represents the root of DOM. Root can be either an object or array.

		//WARN((const char*)L"JSON parse success"); //Debug purpose only

        /*  Sac: Are we using this code block ??
		//Retrieving objects array
		assert(_jsonDocument.HasMember("_Level"));
		assert(_jsonDocument["_Level"].IsNumber());
		assert(_jsonDocument["_Level"].IsInt());
		int levelID = _jsonDocument["_Level"].GetInt();
*/
	//	K_LOG("Level ID SFX List Loaded Level:%d", levelID);

		/*
		if( objDataArray[i].HasMember("pivotX") )
		{
			assert(objDataArray[i]["pivotX"].IsNumber());
			assert(objDataArray[i]["pivotX"].IsInt());
			pivotX = objDataArray[i]["pivotX"].GetInt();
		}
		*/
	}
	else
	{
		#ifdef K_WIN32
			WARN("Er0x1303- JSON parse failed");////Removed Wide string conversion for Windows	
		#else
			WARN((const char*)"Er0x1303- JSON parse failed");
		#endif

	}
}

void AudioSlave::QuickLoadAndPlaySFX(eAudio sample)
{
    int size = (int) LoadedSFXArray.size(); //Sac: Implicit to explicit
	bool isLoaded = false;
	for (int i = 0; i < size; i++)
	{
		if(LoadedSFXArray[i] == sample)
		{
			isLoaded = true;
			break;
		}
	}
	if(!isLoaded)
	{
		LoadedSFXArray.push_back(sample);
		Samples[sample]->Load(sample, AudioSFX, false, 1);
	}
	Samples[sample]->Play();
}

void AudioSlave::StopSFX(const std::string& sampleKey)
{
	//check if already loaded
	int size = (int)LoadedSampleArrayKeys.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		if(sampleKey == LoadedSampleArrayKeys[i])
		{
			LoadedSampleArray[i]->Stop();
			return;
		}
	}
}

bool AudioSlave::IsPlaying(const std::string& sampleKey)
{
	int size = (int)LoadedSampleArrayKeys.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		if(sampleKey == LoadedSampleArrayKeys[i])
		{
			return LoadedSampleArray[i]->IsSamplePlaying();
		}
	}
	return false;
}

void AudioSlave::QuickLoadAndPlaySFX(const std::string& sampleKey, bool isLooping)
{
	//check if already loaded
	int size = (int)LoadedSampleArrayKeys.size(); //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		if(sampleKey == LoadedSampleArrayKeys[i])
		{
			LoadedSampleArray[i]->Play(isLooping);
			return;
		}
	}

	const char* cstr = sampleKey.c_str();
	if(_jsonDocument.HasMember(cstr))
	{
		std::string audPath = _jsonDocument[cstr].GetString(); //get audio file path

		AudioSample* newAudio = new AudioSample();
		newAudio->Load(aSFXTeleShow, AudioSFX, isLooping, 1, audPath); //aSFXTeleShow just dummy

		LoadedSampleArray.push_back(newAudio);

		LoadedSampleArrayKeys.push_back(sampleKey);

		newAudio->Play(isLooping);
	}
	else
	{
		#ifdef K_WIN32
			WARN("Er0x1304- JSON unknown SFX ID");////Removed Wide string conversion for Windows	
		#else
			WARN((const char*)"Er0x1304- JSON unknown SFX ID");
		#endif
	}
}

void AudioSlave::UnloadQuickLoadedSFX()
{
	int size = (int)LoadedSFXArray.size(); //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
	//	K_LOG("Unload SFX id %d",LoadedSFXArray[i]);
		UnloadSample((eAudio)LoadedSFXArray[i]);
	}
	LoadedSFXArray.clear();

	size = (int)LoadedSampleArray.size(); //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		LoadedSampleArray[i]->Unload();
		delete LoadedSampleArray[i];

	//	K_LOG("Unloaded SFX key %s",LoadedSampleArrayKeys[i].c_str());
		LoadedSampleArrayKeys[i].clear();
	}
	LoadedSampleArrayKeys.clear();
	LoadedSampleArray.clear();
}

void AudioSlave::UnloadSample(eAudio sample)
{
	Samples[sample]->Unload();
}



void AudioSlave::PlayFootStepsFX()
{
	switch( FootStepsSFX )
	{
	case 0:PlaySample(aSFXFootsteps0);break;
	case 1:PlaySample(aSFXFootsteps1);break;
	case 2:PlaySample(aSFXFootsteps2);break;
	}

	FootStepsSFX = (FootStepsSFX+1)%MAX_FOOTSTEPS_SFX;
}


bool AudioSlave::IsToLevelTrackRoom()
{
	//Uses level tracks!!
	switch( ProfileSystem::Instance->ActiveProfile.CurrentLevel )
	{
	case Level_1:switch( ProfileSystem::Instance->ActiveProfile.CurrentArea )
				 {
					 //Level 1 - demo
	case eArea_L1_S1_Beach:
	case eArea_L1_S2_Frontyard:
	case eArea_L1_S3_Foyer:
	case eArea_L1_S3_PZShelf:
	case eArea_L1_S2_PZFuse:
	case eArea_L1_S4_Bedroom:
	case eArea_L1_S5_TreasureRoom:
	case eArea_L1_S5_TunnelDoor:
	case eArea_L1_S4_PZPainting:
	case eArea_L1_S6_BoatShed:
	case eArea_L1_S7_BoatDock:
		return true;
				 }
				 break;
	case Level_2:switch( ProfileSystem::Instance->ActiveProfile.CurrentArea )
				 {
					 //Level 2
	case eArea_L2_S1_ForestEntrance:
	case eArea_L2_S1_UndergroundHO:
	case eArea_L2_S2_PZMango:
	case eArea_L2_S2_NagaTemple:
	case eArea_L2_S2_HONagaTemple:
	case eArea_L2_S3_CaveMouth:
	case eArea_L2_S3_PZStoneBeam:
	case eArea_L2_S4_Cave:
	case eArea_L2_S4_PZDeities:
	case eArea_L2_S5_MoopansDen:
	case eArea_L2_S5_PZEagleUnblock:
	case eArea_L2_S5_PZMedicine_Making:
	case eArea_L2_S6_TunnelEnd:
	case eArea_L2_S6_HOWaterfall:
	case eArea_L2_S6_PZTunnelClock:
	case eArea_L2_S7_PZSoldiers:
		return true;
				 }
				 break;
	case Level_3:switch( ProfileSystem::Instance->ActiveProfile.CurrentArea )
				 {
					 //Level 3
	case eArea_L3_S1_FortEntrance:
	case eArea_L3_S1_HOLadder:
	case eArea_L3_S1_PZLionHead :

	case eArea_L3_S2_FortInterior:
	case eArea_L3_S2_P1Room1Start:
	case eArea_L3_S2_P1Room2Wine:
	case eArea_L3_S2_P1Room3Lock:
	case eArea_L3_S2_P1Room4Match3:
	case eArea_L3_S2_P1Room5Pipe:
	case eArea_L3_S2_P1Room6Pillar:
	case eArea_L3_S2_P1DeadEnd:
	case eArea_L3_S3_Pool:
	case eArea_L3_S3_PZUnderwater:
	case eArea_L3_S4_FortFountain:
	case eArea_L3_S4_FortFountainHO:					
	case eArea_L3_S5_Cliff:
		return true;
				 }
	case Level_4:	
						return true;
					
	case Level_5:	
		switch( ProfileSystem::Instance->ActiveProfile.CurrentArea )
		{
			//Level 5
		case eArea_L5_S1_TigerDistant:
		case eArea_L5_S2_TigerCloseup:
		case eArea_L5_S2_PZBlocks:
		case eArea_L5_S3_Pond:
		case eArea_L5_S3_PZDungeonDoor:
		case eArea_L5_S3_HOUnderwater:
		case eArea_L5_S4_Dungeon:
		case eArea_L5_S4_PZHandCuff:
		case eArea_L5_S5_MountainBelow:
		case eArea_L5_S5_HOGanapathi:
		case eArea_L5_S5_PZSunClock:
		case eArea_L5_S6_MountainTop:	
		case eArea_L5_S6_HOTree:
			return true;
		}
		break;
	case Level_6:	switch( ProfileSystem::Instance->ActiveProfile.CurrentArea )
					{
	case	eArea_L6_S1_FloatingIsland1:
	case	eArea_L6_S1_DescriptionHO:
	case 	eArea_L6_S2_FloatingIsland2:
	case	eArea_L6_S2_ShereHO:
	case	eArea_L6_S2_PZPineGum:
	case 	eArea_L6_S3_FloatingIsland3:
	case	eArea_L6_S3_FakeJames:
	case 	eArea_L6_S3_GlowingHeartHO:
	case	eArea_L6_S3_PZPathway:
	case 	eArea_L6_S4_FloatingIsland4:
	case	eArea_L6_S4_PortalChest:
	case	eArea_L6_S4_PortalPuzzle:
	case 	eArea_L6_S5_MarysDream:
	case	eArea_L6_S5_TabMiniGame:
		return true;
					}
					break;
	case Level_7:	return true;	break;
	case Level_8:   return true;	break;
	}			

	return false;
}




void AudioSlave::PlayCollectFX()
{
	switch( CollectSFX )
	{
	case 0:Samples[aSFXMagic1]->Play(false);break;
	case 1:Samples[aSFXMagic2]->Play(false);break;
	case 2:Samples[aSFXMagic3]->Play(false);break;
	}	
	CollectSFX = (CollectSFX+1)%MAX_COLLECT_SFX;
}


void AudioSlave::PlayHintFX()
{	
	switch( HintSFX )
	{
	case 0:Samples[aSFXMagic1]->Play(false);break;
	case 1:Samples[aSFXMagic2]->Play(false);break;
	case 2:Samples[aSFXMagic3]->Play(false);break;
	}	
	HintSFX = (HintSFX+1)%MAX_HINT_SFX;
}



void AudioSlave::ClearUnusedTracks()
{
	eAudio CurActiveTrack = Audio_Nil;
	switch( ProfileSystem::Instance->ActiveProfile.CurrentLevel )
	{
	default:
	case Level_1:if( IsPlaying(aTrackL1Broken) )
					 CurActiveTrack = aTrackL1Broken;
				 else if( IsPlaying(aTrackL1Mansion) )
					 CurActiveTrack = aTrackL1Mansion;
				 else if( IsPlaying(aTrackL1Trapped) )
					 CurActiveTrack = aTrackL1Trapped;
				 else if( IsPlaying(aTrackL1Hunted) )
					 CurActiveTrack = aTrackL1Hunted;
				 else if( IsPlaying(aTrackL1EscapePlan) )
					 CurActiveTrack = aTrackL1EscapePlan;
				 else if( IsPlaying(aTrackL1Puzzle) )
					 CurActiveTrack = aTrackL1Puzzle;
				 break;
	case Level_2:if( IsPlaying(aTrackLevel2) )
					 CurActiveTrack = aTrackLevel2;
				 else if( IsPlaying(aTrackL2theme) )
					 CurActiveTrack = aTrackL2theme;
				 else if( IsPlaying(aTrackL2theme1) )
					 CurActiveTrack = aTrackL2theme1;
				 else if( IsPlaying(aTrackL2ho) )
					 CurActiveTrack = aTrackL2ho;
				 else if( IsPlaying(aTrackL2puzzle) )
					 CurActiveTrack = aTrackL2puzzle;
				 else if( IsPlaying(aTrackL2s7Puzzle) )
					 CurActiveTrack = aTrackL2s7Puzzle;
		break;
	case Level_3: if( IsPlaying(aTrackL3theme) )
					 CurActiveTrack = aTrackL3theme;
				 else if( IsPlaying(aTrackL3ho) )
					 CurActiveTrack = aTrackL3ho;
				 else if( IsPlaying(aTrackL3puzzle) )
					 CurActiveTrack = aTrackL3puzzle;
		break;
	case Level_4:if( IsPlaying(aTrackL4theme) )
					 CurActiveTrack = aTrackL4theme;
				 else if( IsPlaying(aTrackL4ho) )
					 CurActiveTrack = aTrackL4ho;
				 else if( IsPlaying(aTrackL4puzzle) )
					 CurActiveTrack = aTrackL4puzzle;
		break;	
	case Level_5:if( IsPlaying(aTrackL5theme) )
					 CurActiveTrack = aTrackL5theme;
				 else if( IsPlaying(aTrackL5ho) )
					 CurActiveTrack = aTrackL5ho;
				 else if( IsPlaying(aTrackL5puzzle) )
					 CurActiveTrack = aTrackL5puzzle;
		break;	
	case Level_6: if( IsPlaying(aTrackL6theme) )
					 CurActiveTrack = aTrackL6theme;
				 else if( IsPlaying(aTrackL6ho) )
					 CurActiveTrack = aTrackL6ho;
				 else if( IsPlaying(aTrackL6puzzle) )
					 CurActiveTrack = aTrackL6puzzle;
				  else if( IsPlaying(aTrackL6s3Puzzle) )
					 CurActiveTrack = aTrackL6s3Puzzle;
		break;
	case Level_7: if( IsPlaying(aTrackL7theme) )
					 CurActiveTrack = aTrackL7theme;
				 else if( IsPlaying(aTrackL7ho) )
					 CurActiveTrack = aTrackL7ho;
				 else if( IsPlaying(aTrackL7puzzle) )
					 CurActiveTrack = aTrackL7puzzle;
				  else if( IsPlaying(aTrackL7s5Theme) )
					 CurActiveTrack = aTrackL7s5Theme;
		break;

	case Level_8: if( IsPlaying(aTrackL8theme) )
					 CurActiveTrack = aTrackL8theme;
				 else if( IsPlaying(aTrackL8ho) )
					 CurActiveTrack = aTrackL8ho;
				 else if( IsPlaying(aTrackL8puzzle) )
					 CurActiveTrack = aTrackL8puzzle;
		break;
	}

	bool IsNilAudioTrack = (CurActiveTrack == Audio_Nil);
	if( PrevTrack1 != Audio_Nil && (IsNilAudioTrack || PrevTrack1 != CurActiveTrack) )
	{
		if( IsPlaying(PrevTrack1) )
			StopSample(PrevTrack1);
		PrevTrack1 = Audio_Nil;
	}
	if( PrevTrack2 != Audio_Nil  && (IsNilAudioTrack || PrevTrack2 != CurActiveTrack) )
	{
		if( IsPlaying(PrevTrack2) )
			StopSample(PrevTrack2);
		PrevTrack2 = Audio_Nil;
	}
	if( PrevTrack3 != Audio_Nil  && (IsNilAudioTrack || PrevTrack3 != CurActiveTrack) )
	{
		if( IsPlaying(PrevTrack3) )
			StopSample(PrevTrack3);
		PrevTrack3 = Audio_Nil;
	}
	if( PrevTrack4 != Audio_Nil  && (IsNilAudioTrack || PrevTrack4 != CurActiveTrack) )
	{
		if( IsPlaying(PrevTrack4) )
			StopSample(PrevTrack4);
		PrevTrack4 = Audio_Nil;
	}
	if( PrevTrack5 != Audio_Nil  && (IsNilAudioTrack || PrevTrack5 != CurActiveTrack) )
	{
		if( IsPlaying(PrevTrack5) )
			StopSample(PrevTrack5);
		PrevTrack5 = Audio_Nil;
	}
	if( PrevTrack6 != Audio_Nil  && (IsNilAudioTrack || PrevTrack6 != CurActiveTrack) )
	{
		if( IsPlaying(PrevTrack6) )
			StopSample(PrevTrack6);
		PrevTrack6 = Audio_Nil;
	}
	if( PrevTrack7 != Audio_Nil  && (IsNilAudioTrack || PrevTrack7 != CurActiveTrack) )
	{
		if( IsPlaying(PrevTrack7) )
			StopSample(PrevTrack7);
		PrevTrack7 = Audio_Nil;
	}
}

void AudioSlave::TurnOffLvlMscTracks()
{
	if( PrevTrack1 != Audio_Nil )
		if( IsPlaying(aTrackBonusCredit) )
			return;

	if( PrevTrack1 != Audio_Nil )
		if( IsPlaying(PrevTrack1) )
			StopSample(PrevTrack1);
	if( PrevTrack2 != Audio_Nil )
		if( IsPlaying(PrevTrack2) )
			StopSample(PrevTrack2);
	if( PrevTrack3 != Audio_Nil )
		if( IsPlaying(PrevTrack3) )
			StopSample(PrevTrack3);
	if( PrevTrack4 != Audio_Nil )
		if( IsPlaying(PrevTrack4) )
			StopSample(PrevTrack4);
	if( PrevTrack5 != Audio_Nil )
		if( IsPlaying(PrevTrack5) )
			StopSample(PrevTrack5);
	if( PrevTrack6 != Audio_Nil )
		if( IsPlaying(PrevTrack6) )
			StopSample(PrevTrack6);
	if( PrevTrack7 != Audio_Nil )
		if( IsPlaying(PrevTrack7) )
			StopSample(PrevTrack7);


	PrevTrack1 = Audio_Nil; 
	PrevTrack2 = Audio_Nil;
	PrevTrack3 = Audio_Nil;
	PrevTrack4 = Audio_Nil;
	PrevTrack5 = Audio_Nil;
	PrevTrack6 = Audio_Nil;
	PrevTrack7 = Audio_Nil;

}

void AudioSlave::PlaySample(eAudio sample, bool streamLoop)
{
	Samples[sample]->Play(streamLoop);
	//Save last played audio track for journal
	if( Samples[sample]->AudioType == AudioTrack || Samples[sample]->AudioType == AudioAmb )
	{
		if( PrevTrack1 == Audio_Nil )
			PrevTrack1 = sample;
		else if( PrevTrack2 == Audio_Nil )
			PrevTrack2 = sample;
		else if( PrevTrack3 == Audio_Nil )
			PrevTrack3 = sample;
		else if( PrevTrack4 == Audio_Nil )
			PrevTrack4 = sample;
		else if( PrevTrack5 == Audio_Nil )
			PrevTrack5 = sample;
		else if( PrevTrack6 == Audio_Nil )
			PrevTrack6 = sample;
		else if( PrevTrack7 == Audio_Nil )
			PrevTrack7 = sample;
	}
}


bool AudioSlave::IsPlaying(eAudio sample)
{
	return Samples[sample]->IsSamplePlaying();
}

void AudioSlave::StopSample(eAudio sample)
{
	Samples[sample]->Stop();
	//When music turned off - turn off it from register too....
	if( Samples[sample]->AudioType == AudioTrack || Samples[sample]->AudioType == AudioAmb )
	{
		if( PrevTrack1 == sample )
			PrevTrack1 = Audio_Nil;
		else if( PrevTrack2 == sample )
			PrevTrack2 = Audio_Nil;
		else if( PrevTrack3 == sample )
			PrevTrack3 = Audio_Nil;
		else if( PrevTrack4 == sample )
			PrevTrack4 = Audio_Nil;
		else if( PrevTrack5 == sample )
			PrevTrack5 = Audio_Nil;
		else if( PrevTrack6 == sample )
			PrevTrack6 = Audio_Nil;
		else if( PrevTrack7 == sample )
			PrevTrack7 = Audio_Nil;
	}
}





void AudioSlave::FadeInSample(eAudio sample, float speed)
{
	Samples[sample]->FadeIn(speed);
}

void AudioSlave::FadeOutSample(eAudio sample, float speed, bool unload)
{
	Samples[sample]->FadeOut(speed, unload);
}

void AudioSlave::SetGlobalVolumes(int sfxVol, int mscVol,int voVol, bool IsMute)
{
	if( !IsMute )
	{
		GlobalMusicVol = mscVol;
		GlobalSFXVol = sfxVol;
		GlobalVoiceVol = voVol;
	}


#ifdef K_WIN32
	if( KSound::g_bErrSoundDeviceInit )
		return;
#endif		

	for(int i=0; i < Audio_Max; ++i)
		if (Samples[i] != NULL )
		{
			if( Samples[i]->AudioType != AudioUndefined )
			{
				//SFX & AudioAmb
				if( Samples[i]->AudioType == AudioSFX || Samples[i]->AudioType == AudioAmb )
					Samples[i]->SetVolume((float)sfxVol);
				else if( Samples[i]->AudioType == AudioVO)
					Samples[i]->SetVolume((float)voVol);
				//AudioTrack		
				else if( Samples[i]->AudioType == AudioTrack )			
					Samples[i]->SetVolume((float)mscVol);					
			} 
		}
}

void AudioSlave::SetGlobalSFXVolume(int sfxVol)
{
	GlobalSFXVol = sfxVol;
#ifdef K_WIN32 
	if( KSound::g_bErrSoundDeviceInit )
		return;
#endif	

	for(int i=0; i < Audio_Max; ++i)
	{
		//SFX & AudioAmb
		if( Samples[i]->AudioType == AudioSFX || Samples[i]->AudioType == AudioAmb )
			Samples[i]->SetVolume((float)GlobalSFXVol);
	}
}

void AudioSlave::SetGlobalMusicVolume(int mscVol)
{
	GlobalMusicVol = mscVol;
#ifdef K_WIN32 
	if( KSound::g_bErrSoundDeviceInit )
		return;
#endif 	

	for(int i=0; i < Audio_Max; ++i)
	{
		//AudioTrack		
		if( Samples[i]->AudioType == AudioTrack )		
			Samples[i]->SetVolume((float)GlobalMusicVol);		

	}
}

void AudioSlave::SetGlobalVoiceVolume(int voVol)
{
	GlobalVoiceVol = voVol;
#ifdef K_WIN32 
	if( KSound::g_bErrSoundDeviceInit )
		return;
#endif 	
	for(int i=0; i < Audio_Max; ++i)
	{
		//AudioTrack		
		if( Samples[i]->AudioType == AudioVO )		
			Samples[i]->SetVolume((float)GlobalVoiceVol);		

	}

}

void AudioSlave::UnloadAllSamples()
{
	for(int i=0; i < Audio_Max; ++i)
	{
		Samples[i]->Unload();
	}
}

void AudioSlave::Silence()
{
	if( !ProfileSystem::Instance->ActiveProfile.IsMuted )
	{
#ifdef K_WIN32 
		if( KSound::g_bErrSoundDeviceInit )
			return;
#endif
		//Control::Audio->SetGlobalVolumes(0,0,0,true);
		KSound::suspend();


	}
	Silent = true;
}

void AudioSlave::EndSilence()
{
	if( !ProfileSystem::Instance->ActiveProfile.IsMuted )
	{
#ifdef K_WIN32
		if( KSound::g_bErrSoundDeviceInit )
			return;
#endif 
		//Control::Audio->SetGlobalVolumes(Control::Audio->GlobalSFXVol,Control::Audio->GlobalMusicVol,Control::Audio->GlobalVoiceVol);
		KSound::resume();

	}
	Silent = false;
}
