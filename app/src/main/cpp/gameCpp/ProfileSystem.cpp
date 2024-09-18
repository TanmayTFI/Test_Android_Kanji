//====================================
// Framework
// Profile System Class [singleton]
//====================================
#include "KPTK.h"
#include "ProfileSystem.h"
#include "IOStream.h"
#include "Util.h"
#include "Console.h"
#include "Resources.h"
#include "LangDefs.h"
#include "BuildConstants.h"
#ifdef K_ANDROID
	#include "Application.h"
#endif

#ifdef K_WIN32
	extern std::wstring AppTitle_Str;
	extern std::wstring GWRNG_5_WStr;
#else
		extern std::string AppTitle_Str;
		extern std::string GWRNG_5_WStr;
#endif


//Class wide member and methods
ProfileSystem* ProfileSystem::Instance = NULL;


void ProfileSystem::Create()
{
	if(ProfileSystem::Instance == NULL)
		ProfileSystem::Instance = new ProfileSystem();
}

void ProfileSystem::Destroy()
{
	if(ProfileSystem::Instance != NULL)
	{
		delete ProfileSystem::Instance;
		ProfileSystem::Instance = NULL;
	}
}

//Instance methods
ProfileSystem::ProfileSystem()
{
	UserPath = "invalid";

	ProfileSlot = 0;
	_isExtrasGamePlay = false;

	for(int i=0; i < MAX_PROFILES; ++i)
	{
		ProfileName[i] = "invalid";
		ProfileValid[i] = false;
		ProfileMode[i] = 0;	//default is casual Mode
		#ifdef K_ANDROID
			ProfileGameVersionMode[i] = 0;
		#endif
	}
	BackupProfileSystem();
}

ProfileSystem::~ProfileSystem()
{
}

bool ProfileSystem::Initialize()
{
	std::string USERNAME = KMiscTools::getUserName();
	
	std::string GamePath;
	#if defined (K_WIN32)
		{
			OSVERSIONINFOEXA vi;
			/* Check Windows version */
			ZeroMemory (&vi, sizeof (vi));
			vi.dwOSVersionInfoSize = sizeof (vi);
			GetVersionExA ((OSVERSIONINFOA *) &vi);
			
			
			
			switch (vi.dwMajorVersion) 
			{
				default:
				case 4:
					/* DOS based Windows: 95, 98 or ME */
					{
						std::string UserFolder = KMiscTools::getUserFolder();
						GamePath = UserFolder + std::string("\\Tuttifrutti_Games\\");
					}
					break;

				case 5:
					/* NT kernel based Windows 5.x: Win2000,Windows Server 2003 or XP */
					//C:\Documents and Settings\USERNAME\Application Data\STUDIONAME\GAMENAME
					{
						
						std::string UserFolder = KMiscTools::getUserFolder();
						K_LOG("Windows User folder detected is %s",UserFolder.c_str());
						std::string DriveLetter = std::string(1,(char)UserFolder[0]);
						GamePath = DriveLetter+std::string(":\\Documents and Settings\\")+USERNAME+std::string("\\Application Data\\Tuttifrutti_Games\\");
					}
					break;

				case 6:
						if (vi.dwMinorVersion == 2 ) //windows 10 machine detected!!
						{
							K_LOG("Windows 10 machine detected");
							LPWSTR roamingPath;
							SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &roamingPath);						
							GamePath = FileSystem::ws2s(roamingPath);
							GamePath += "\\Tuttifrutti_Games\\"; 
							K_LOG(GamePath.c_str());
						}
						else 
						{					  
							/* Vista,Windows Server 2008,Windows 7,Windows Server 2008 RC2 */
							//C:\Users\ USERNAME\AppData\Roaming\STUDIONAME\GAMENAME
					
							std::string UserFolder = KMiscTools::getUserFolder();
							K_LOG("Windows User folder detected is %s",UserFolder.c_str());
							std::string DriveLetter = std::string(1,(char)UserFolder[0]);
							GamePath = DriveLetter+std::string(":\\Users\\")+USERNAME+std::string("\\AppData\\Roaming\\Tuttifrutti_Games\\") ;
					}
					break;  
			}
			#ifndef _PUBLISHER_BUILD
				UserPath = GamePath + std::string("Darkarta_ABHQ_CE\\");// Not changing to make backward compatible with all old players
			#else
				#ifdef _CEBUILD
					UserPath = GamePath + std::string("Darkarta_ABHQ_CE\\");// Not changing to make backward compatible with all old players
				#else
					UserPath = GamePath + std::string("Darkarta_ABHQ_SE\\");// Not changing to make backward compatible with all old players
				#endif
			#endif	

			if(!KMiscTools::fileExists(UserPath.c_str()))
			{
				if(KMiscTools::fileExists("SaveGames"))
					UserPath = std::string("SaveGames\\");
			}

		}

	#elif defined (K_ANDROID)

		// profiles are stored inside the App in Android, in the location like this, 
		// /data/user/0/in.tuttifrutti.darkarata.abhq.en.full/files/data.sav
		// or
		// /data/user/0/in.tuttifrutti.darkarata.abhq.en.free/files/data.sav 
		// KMiscTools::getPreferencesFolder() gives /data/user/0/in.tuttifrutti.darkarata.abhq.en.free/files
		UserPath = std::string(KMiscTools::getPreferencesFolder()) + std::string("/");
    	
	#else
		{
			GamePath = std::string(KMiscTools::getPreferencesFolder()) + std::string("//Tuttifrutti_Games//");
			UserPath = GamePath + std::string("Darkarta_ABHQ_CE//");// Not changing to make backward compatible with all old players		

			if(!KMiscTools::fileExists(UserPath.c_str()))
			{
				if(KMiscTools::fileExists("SaveGames"))
					UserPath = std::string("SaveGames\\");
			}
		}
	#endif

	
	

	std::string SaveFile = UserPath + "data.sav";

/*
	// alert to check where the profiles are saved physically in the platform device
	char _szPathBuf[K_MAXPATH];
	KStringFmt (_szPathBuf, K_MAXPATH, "%s", SaveFile.c_str());
    KMiscTools::alertBox ("game path is at", _szPathBuf, "OK", NULL);
*/
	bool IsFilePresent = false;
	if( KMiscTools::fileExists(SaveFile.c_str()) )
	{
		IsFilePresent = true; 
		if( ReadProfileSystemData() )//Read profile slot too...
		{
			if(ProfileSlot >= 0 && ProfileSlot < MAX_PROFILES )
			{
				if( LoadProfile(ProfileSlot) )
				    return true; 
			}
		}
		#ifdef K_WIN32
			KMiscTools::messageBox((const char*)((FileSystem::ws2s(AppTitle_Str)).c_str()),(const char*)((FileSystem::ws2s(GWRNG_5_WStr)).c_str()) );			
		#else			
			KMiscTools::messageBox((const char*)AppTitle_Str.c_str(),(const char*)GWRNG_5_WStr.c_str() );			
		#endif
		KInput::hidePointer();//since user may see this before window creation - the system cursor will follow to game!!
	}
	
	if( !IsFilePresent )
	{
		//FirstPlaySetup        
		KMiscTools::createDirectory(GamePath.c_str());//Game store directory
		KMiscTools::createDirectory(UserPath.c_str());//Userdata store directory

		if(!KMiscTools::fileExists(UserPath.c_str()))
			{
				KMiscTools::createDirectory("SaveGames");
				UserPath = std::string("SaveGames\\");
			}
	}

	//Clear if any previous profile!
	ProfileSlot = 0;
	std::string PrevSaveFile;
	
	#ifdef K_WIN32
		_flushall();
	#endif    
	for(int i=0; i < MAX_PROFILES; ++i)
	{
		ProfileName[i] = "invalid";
		ProfileValid[i] = false;
		ProfileMode[i] = 0;
		PrevSaveFile = UserPath + MKSTR(i) + ".sav";
		if( KMiscTools::fileExists(PrevSaveFile.c_str()) )
			remove(PrevSaveFile.c_str());//check mac compatibility
	}

	WriteProfileSystemData();//data.sav
	ActiveProfile.Invalidate();
	ActiveProfile.Clear();
	SaveProfile(0);//temporary 0.sav profile creation to avoid handle sysclose before actual creation!!


	return false;
}



bool ProfileSystem::LoadProfile(int slot)
{
	CIOStream stream;

	std::string FileName = UserPath + MKSTR(slot) + ".sav";
	if( !KMiscTools::fileExists(FileName.c_str()) )
		return false;
	if( !stream.Open(FileName, File_Read) )
		return false;

	ProfileSlot = slot;
	if( !ActiveProfile.Load(&stream) )
	{
		stream.Close();
		return false;
	}

	ActiveProfile.InitGameVars();

	//Read 8 file savetime stampings!!
	SYSTEMTIME InFileST;
	ReadCurSysTimeStamps( &stream, &InFileST); 		
	stream.Close();

	return MatchSysTimeStamps( FileName, &InFileST );	
}

bool ProfileSystem::LoadProfileBackup(int slot)
{
	CIOStream stream;

	std::string FileName = UserPath + MKSTR(slot) + "Backup.sav";
	if( !KMiscTools::fileExists(FileName.c_str()) )
		return false;
	if( !stream.Open(FileName, File_Read) )
		return false;

	ProfileSlot = slot;
	if( !ActiveProfile.Load(&stream) )
	{
		stream.Close();
		return false;
	}

	ActiveProfile.InitGameVars();

	//Read 8 file savetime stampings!!
	SYSTEMTIME InFileST;
	ReadCurSysTimeStamps( &stream, &InFileST); 		
	stream.Close();

	return MatchSysTimeStamps( FileName, &InFileST );	
}

void ProfileSystem::setForExtrasGamePlay()
{
	_isExtrasGamePlay = true;
	ActiveProfile.setForExtras();
}


void ProfileSystem::SaveProfile(int slot)
{
	if (_isExtrasGamePlay)
	{
		return;
	}

	CIOStream stream;
	std::string SaveLoc = UserPath + MKSTR(slot) + ".sav";
	if( !stream.Open(SaveLoc, File_Write) )
	{
		std::string SaveErrorMsg = StringTable::getString("ERR_SAVE")+SaveLoc;
		K_LOG(SaveErrorMsg.c_str());
		#ifdef K_WIN32
			FATAL(SaveErrorMsg.c_str());	////Removed Wide string conversion for Windows	
		#else
			FATAL((const char*)(SaveErrorMsg.c_str()) );	
		#endif		
		
	}

	ActiveProfile.Save(&stream);	

	WriteCurSysTimeStamps( &stream );
	stream.Close();	

	#ifdef K_WIN32
		_flushall();//write all open output streams
	#endif
}

void ProfileSystem::SaveProfileBackup(int slot)
{
	if (_isExtrasGamePlay)
	{
		return;
	}

	CIOStream stream;
	
	std::string SaveLoc = UserPath + MKSTR(slot) + "Backup.sav";
	if( !stream.Open(SaveLoc, File_Write) )
	{
		std::string SaveErrorMsg = StringTable::getString("ERR_SAVE")+SaveLoc;
		K_LOG(SaveErrorMsg.c_str());
		#ifdef K_WIN32
			FATAL(SaveErrorMsg.c_str());	////Removed Wide string conversion for Windows	
		#else
			FATAL((const char*)(SaveErrorMsg.c_str()) );	
		#endif		
		
	}

	ActiveProfile.Save(&stream);	

	WriteCurSysTimeStamps( &stream );
	stream.Close();	

	#ifdef K_WIN32
		_flushall();//write all open output streams
	#endif
}


void ProfileSystem::BackupProfileSystem()
{
	BackupProfileSlot = ProfileSlot;
	for( int i = 0; i < MAX_PROFILES; i++ )
	{
		BackupProfileName[i] = ProfileName[i];
		BackupProfileValid[i] = ProfileValid[i];
		BackupProfileMode[i] = ProfileMode[i];
		#ifdef K_ANDROID
			BackupProfileGameVersionMode[i] = ProfileGameVersionMode[i];
		#endif
	}	
}


void ProfileSystem::RestoreProfileSystem()
{
	ProfileSlot = BackupProfileSlot;
	for( int i = 0; i < MAX_PROFILES; i++ )
	{
		ProfileName[i] = BackupProfileName[i];
		ProfileValid[i] = BackupProfileValid[i];
		ProfileMode[i] = BackupProfileMode[i];
		#ifdef K_ANDROID
			ProfileGameVersionMode[i] = BackupProfileGameVersionMode[i];
		#endif
	}
}



void ProfileSystem::SetProfileName(const std::string name)
{
	if(ProfileSlot >= 0 && ProfileSlot < MAX_PROFILES )
	{
		ProfileName[ProfileSlot] = name;
		ProfileValid[ProfileSlot] = true;
		ProfileMode[ProfileSlot] = 0;//default casual mode
		#ifdef K_ANDROID
			ProfileGameVersionMode[ProfileSlot] = GFApp->MyVersion+ProfileSlot;
		#endif
	}
}

void ProfileSystem::ClearAllCurrentObjectives()
{
	ProfileSystem::Instance->ActiveProfile.NumObjectivesActive = 0;
	for (int i = 0; i < MAX_CURRENT_OBJECTIVES; i++)
	{
		ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[i] = -1;
	}
}

//Handle force clear so that old players too can play game smoothly
void ProfileSystem::ClearInventoryBox() 
{
	for (int i = 0; i < eInv_Max; i++)
	{
		ProfileSystem::Instance->ActiveProfile.Inventory[i] = 0;		
	}
	for( int j= 0 ; j < MAX_INVBOX_OBJECTS; j++)
	{
		ProfileSystem::Instance->ActiveProfile.InvObjects[j].InvId = -1;
		ProfileSystem::Instance->ActiveProfile.InvObjects[j].nPiecePicked = 0;
		ProfileSystem::Instance->ActiveProfile.InvObjects[j].MaxPieces = 0;
	}
	ProfileSystem::Instance->ActiveProfile.InvObjectCnt = 0;
	ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj = 0;	
}

void ProfileSystem::SetProfileMode(int mode)
{	
	if(ProfileSlot >= 0 && ProfileSlot < MAX_PROFILES )
		ProfileMode[ProfileSlot] = mode;
}

#ifdef K_ANDROID
	void ProfileSystem::SetProfileGameVersionMode() // Sets the current game version of the game to data.sav , call WriteProfileSystemData(), after calling this
	{	
		// used to set the game as Free version of Full version
		// After the user buys the game, the game is set to Full version
		for( int i = 0; i < MAX_PROFILES; i++ )
		{
			ProfileGameVersionMode[i] =  GFApp->MyVersion+i;
		}
		WriteProfileSystemData(); // save the Data.sav file.
	}

	bool ProfileSystem::IsGameVersionMode() // Checks if the current game version of the game from data.sav is full or not
	{	
		// returns true if it is Free version and false if it is Full version
		bool isItfull = true;
		for( int i = 0; i < MAX_PROFILES; i++ )
		{
			if (ProfileGameVersionMode[i] >=456 && ProfileGameVersionMode[i] <=461)
				isItfull= false;
		}
		return isItfull;
	}

#endif

void ProfileSystem::SetProfileSlot(int slot)
{
	ProfileSlot = slot; 	
}

void ProfileSystem::DeleteSlot(int slot)
{
	ProfileName[slot] = "invalid";
	ProfileValid[slot] = false;
	ProfileMode[slot] = 0;//default casual mode
	#ifdef K_ANDROID
			ProfileGameVersionMode[slot] = GFApp->MyVersion+slot;
	#endif
	WriteProfileSystemData();
	
}

void ProfileSystem::DeleteUnwantedSlotFiles()
{
	#ifdef K_WIN32		
		_flushall();
	#endif
		std::string PrevSaveFile;
		for( int i = 0; i < MAX_PROFILES; i++ )
			if( ProfileName[i] == "invalid" )
			{
				PrevSaveFile = UserPath + MKSTR(i) + ".sav";
				if( KMiscTools::fileExists(PrevSaveFile.c_str()) )
					remove(PrevSaveFile.c_str());//check mac compatibility
			}
}






void ProfileSystem::WriteProfileSystemData()
{
	CIOStream stream;

	std::string SaveLoc = UserPath + "data.sav";
	if( !stream.Open(SaveLoc, File_Write))
	{
		K_LOG(std::string("File write access denied for location "+SaveLoc).c_str());
		K_LOG("Attempting to create user folders");
		KMiscTools::createDirectory(GamePath.c_str());//Game store directory
		KMiscTools::createDirectory(UserPath.c_str());//Userdata store directory
		if( !stream.Open(SaveLoc, File_Write))
		{
			std::string SaveErrorMsg = std::string("Er0x3102- Game can't write profilesytem data ")+SaveLoc + std::string(" and game path is ") + GamePath + std::string(" and User path is ") + UserPath;		
			K_LOG(SaveErrorMsg.c_str());
			#ifdef K_WIN32
				FATAL(SaveErrorMsg.c_str());	
			#else
				FATAL((const char*)(SaveErrorMsg.c_str()) );	
			#endif		
		}	
	}

	//Last Profile
	stream.WriteInt(ProfileSlot);

	//Profile names/validity
	for(int i=0; i < MAX_PROFILES; ++i)
	{
		stream.WriteBool(ProfileValid[i]);
		stream.WriteInt(ProfileMode[i]);
		stream.WriteString(ProfileName[i]);
		#ifdef K_ANDROID
			//stream.WriteInt(ProfileGameVersionMode[i]);
		#endif
	}


#ifdef K_ANDROID
	stream.WriteInt(GFApp->unlocked);
	if(GFApp->noOfHints !=  -777)
	{
		stream.WriteInt(GFApp->noOfHints);
		NoOfHints = GFApp->noOfHints;
	}
	else
	{
		stream.WriteInt(NoOfHints);
	}
	
#endif

	WriteCurSysTimeStamps( &stream );

	stream.Close();

	#ifdef K_WIN32
		_flushall();//write all open output streams
	#endif
	#ifdef K_ANDROID
		//KSysAndroid::showToast("Profile system saved!",true);
	#endif

	
}



bool ProfileSystem::ReadProfileSystemData()
{
	CIOStream stream;
	std::string FileName = UserPath + "data.sav";

	if( stream.Open(FileName, File_Read) )
	{
		K_LOG("Reading ProfileSystem data.sav");
		K_LOG( FileName.c_str() );
		//Last Profile
		ProfileSlot = stream.ReadInt();
		if( ProfileSlot == -1 )
		{
			stream.Close();
			return false;//No active profile present!!
		}

		//Profile names/validity
		for(int i=0; i < MAX_PROFILES; ++i)
		{
			ProfileValid[i] = stream.ReadBool();
			ProfileMode[i] = stream.ReadInt();
			ProfileName[i]  = stream.ReadString();
			#ifdef K_ANDROID
				//ProfileGameVersionMode[i]= stream.ReadInt();;
			#endif
		}

#ifdef K_ANDROID
		if(stream.ReadInt()==1)
			GFApp->unlocked = true;
		else
			GFApp->unlocked = false;
		NoOfHints = stream.ReadInt();
#endif
		//Read 8 file savetime stampings!!
		SYSTEMTIME InFileST;
		ReadCurSysTimeStamps( &stream, &InFileST); 		
		stream.Close();

		return MatchSysTimeStamps( FileName, &InFileST );			
	}	
	else
		return false;
}


void ProfileSystem::WriteCurSysTimeStamps( CIOStream *stream )
{
#ifdef K_WIN32
	//Save file savetime stampings(8) in itself!!
	SYSTEMTIME ST;
    GetSystemTime(&ST);        
	stream->WriteInt((int)ST.wDay);
	stream->WriteInt((int)ST.wDayOfWeek);
	stream->WriteInt((int)ST.wHour);
	stream->WriteInt((int)ST.wMilliseconds);
	stream->WriteInt((int)ST.wMinute);
	stream->WriteInt((int)ST.wMonth);
	stream->WriteInt((int)ST.wSecond);
	stream->WriteInt((int)ST.wYear);
#endif    
}


void ProfileSystem::ReadCurSysTimeStamps( CIOStream *stream, SYSTEMTIME* InFileST )
{
#ifdef K_WIN32
    InFileST->wDay = (WORD)stream->ReadInt();
	InFileST->wDayOfWeek = (WORD)stream->ReadInt();
	InFileST->wHour = (WORD)stream->ReadInt();
	InFileST->wMilliseconds = (WORD)stream->ReadInt();
	InFileST->wMinute = (WORD)stream->ReadInt();
	InFileST->wMonth = (WORD)stream->ReadInt();
	InFileST->wSecond = (WORD)stream->ReadInt();
	InFileST->wYear = (WORD)stream->ReadInt();
#endif
}

bool ProfileSystem::MatchSysTimeStamps( std::string FileName, SYSTEMTIME *InFileST )
{	
	
	//Aji - turned off sav file cheatingchecks - as in some users profilesavefiles are not loaded, due to this extreme checking!! 
//#ifdef _CHEATS
	return true;
//#endif 

	#ifdef K_WIN32
	{
		OFSTRUCT ofs;
		HFILE hFile = OpenFile(FileName.c_str(), (LPOFSTRUCT) &ofs, OF_READ);
		if( hFile == HFILE_ERROR ) 
			return false;

		FILETIME FileLastWriteTime;
		if( !GetFileTime(
			(HANDLE)hFile,                 // handle to file
			(LPFILETIME) NULL,    // creation time
			(LPFILETIME) NULL,  // last access time
			&FileLastWriteTime    // last write time
			))
		{
			CloseHandle((HANDLE) hFile);
			return false;
		}
		CloseHandle((HANDLE) hFile);

		SYSTEMTIME FileLastWriteST;
		if(!FileTimeToSystemTime(
			&FileLastWriteTime,  // file time to convert
			&FileLastWriteST    // receives system time
		))
			return false;			
		
		if( InFileST->wDay == FileLastWriteST.wDay &&
			InFileST->wDayOfWeek == FileLastWriteST.wDayOfWeek &&
			InFileST->wHour == FileLastWriteST.wHour &&			
			//drop milliseconds comparison!!
			InFileST->wMinute == FileLastWriteST.wMinute &&
			InFileST->wMonth == FileLastWriteST.wMonth &&
			//Handle 3 seconds file writing time!!
			abs(InFileST->wSecond - FileLastWriteST.wSecond) < 3 &&
			InFileST->wYear  == FileLastWriteST.wYear )
			return true;			
		else
			return false;		
	}
	#else
		return true;
	#endif		
}
