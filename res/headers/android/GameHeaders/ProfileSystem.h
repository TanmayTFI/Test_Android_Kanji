//====================================
// Framework
// Profile System Class [singleton]
//====================================

#ifndef FRMWRK_PROFILESYSTEM_DEF
#define FRMWRK_PROFILESYSTEM_DEF

#include "Profile.h"


#define MAX_PROFILES 5

#ifndef K_WIN32
    typedef unsigned short WORD;
    typedef struct _SYSTEMTIME
    {
        WORD wYear;
        WORD wMonth;
        WORD wDayOfWeek;
        WORD wDay;
        WORD wHour;
        WORD wMinute;
        WORD wSecond;
        WORD wMilliseconds;
        
    }SYSTEMTIME;
#endif
class ProfileSystem
{
	public:
		//Singleton Instance
		static ProfileSystem* Instance;
		std::string ProfileName[MAX_PROFILES];
		bool		ProfileValid[MAX_PROFILES];
		int			ProfileMode[MAX_PROFILES];
	#ifdef K_ANDROID
		int	ProfileGameVersionMode[MAX_PROFILES];
		int NoOfHints;
	#endif
		std::string UserPath;
		std::string GamePath;

		static void				Create();
		static void				Destroy();

		Profile ActiveProfile;

		bool _isExtrasGamePlay;
		void setForExtrasGamePlay();

		//Returns false if there are no profiles saved. In this case
		//the function will create the appropriate directory and
		//invalidate the ActiveProfile member. Otherwise, loads the 
		//last profile saved.
		bool Initialize();
		//Returns false if the profile load fails.
		bool LoadProfile(int slot);
		bool LoadProfileBackup(int slot);
		//Will save the said profile to the associated slot.
		void SaveProfile(int slot);
		void SaveProfileBackup(int slot);
		//Save general data
		void WriteProfileSystemData();
		//Sets the current profile's name
		void SetProfileName(const std::string name);
		//Sets the current profile's Game Mode
		void SetProfileMode(int mode);
		#ifdef K_ANDROID
			// Sets the Game Version mode for all the profiles in incremental order. the game becomes Full version from Free here
			void SetProfileGameVersionMode();
			// Checks if the game is free version or not
			bool IsGameVersionMode();
		#endif
		//DELETE
		void DeleteSlot(int slot);
		void DeleteUnwantedSlotFiles();

		

		void SetProfileSlot(int slot);		
		inline bool IsBackupProfileValid(int slot)		{ return BackupProfileValid[slot]; }
		inline bool IsProfileValid(int slot)		{ return ProfileValid[slot]; }
		inline std::string	GetBackupProfileName(int slot) { return BackupProfileName[slot]; }
		inline std::string	GetProfileName(int slot) { return ProfileName[slot]; }
		inline int			GetProfileMode(int slot) { return ProfileMode[slot]; }
		inline std::string GetCurrentName()			{ return ProfileName[ProfileSlot]; }
		inline void Invalidate()					{ ActiveProfile.Invalidate(); }

		int	 ProfileSlot;
		int BackupProfileSlot;
		void BackupProfileSystem();
		void RestoreProfileSystem();

		void WriteCurSysTimeStamps( CIOStream *stream );
		void ReadCurSysTimeStamps( CIOStream *stream, SYSTEMTIME* InFileST );
		bool MatchSysTimeStamps( std::string FileName, SYSTEMTIME *InFileST );

		void ClearAllCurrentObjectives();
		void ClearInventoryBox();

	private:
		ProfileSystem();
		~ProfileSystem();

		//Internal technicals				
		std::string BackupProfileName[MAX_PROFILES];
		bool		BackupProfileValid[MAX_PROFILES];	
		int			BackupProfileMode[MAX_PROFILES];	
	#ifdef K_ANDROID
		int	BackupProfileGameVersionMode[MAX_PROFILES];
	#endif
		//Methods
		bool ReadProfileSystemData();
};

static Profile* PROFILE()
{
	return &(ProfileSystem::Instance->ActiveProfile);
}

static void SAVEALLPROFILES()
{
	ProfileSystem::Instance->SaveProfile(ProfileSystem::Instance->ProfileSlot);
	ProfileSystem::Instance->WriteProfileSystemData();	
}

#endif
