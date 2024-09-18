//====================================
// Framework
// Save Data
//====================================

#include "SaveData.h"
#include "IOStream.h"
#include "Resolution.h"
#include "SConstants.h"



SaveData::SaveData()
{
	ProfileVersion = 0;
}

SaveData::~SaveData()
{
}

void SaveData::Save(CIOStream *stream)
{
	stream->WriteInt(PROFILESAVE_VER_2);
    
	stream->WriteInt(Resolution);
	stream->WriteInt(BPP);
	stream->WriteBool(Windowed);

	Write(stream);
	#ifdef K_ANDROID
		//KSysAndroid::showToast("Profile saved!",false);
	#endif
}

bool SaveData::Load(CIOStream *stream)
{
	ProfileVersion = stream->ReadInt();

	
	if( ProfileVersion < PROFILE_SAVE_VER_BUG )//FROM PROFILE_SAVE_VER 885 onwards we won't invalidate to maintain backward save compatiblity!!
	{
		Invalidate();

		return false;
	}
	

	Resolution	= stream->ReadInt();
	BPP	= stream->ReadInt();
	Windowed	= stream->ReadBool();

	Read(stream);
	#ifdef K_ANDROID
		//KSysAndroid::showToast("Profile Loaded!",false);
	#endif
	return true;
}

void SaveData::Invalidate()
{
	Resolution = (int)R_INVALID;
	BPP = -1;
    Windowed = false;
}
