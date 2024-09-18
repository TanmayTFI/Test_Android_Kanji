//====================================
// Framework
// BUILD CONSTANTS class
//====================================

#ifndef FRMWRK_BUILDCONSTANTS_DEF
#define FRMWRK_BUILDCONSTANTS_DEF


//#define _PAKBUILD	true
//#define _DEMOBUILD true


// Ideally set the below Splash parameter trhough the build parameters and not here
// _BIGFISH_BUILD is true for BIGFISH LOGO to be used in Splash, only 1 can be true at a time
//or
//  _GAMEHOUSE_BUILD is true for GAME HOUSE LOGO to be used in Splash, only 1 can be true at a time
//#define _BIGFISH_BUILD true
//#define _GAMEHOUSE_BUILD true


//#define _PUBLISHER_BUILD true
//#define _STEAM_BUILD true

//#define _DISABLE_EXTRA true
/*
#if !defined K_MACOS
	#define _ENABLE_CURL true
#endif
 */

#ifdef K_iOS

    #define _CEBUILD true
    #define TOUCH_UI true
    #define _LEVEL1_ENABLED true
    #define _LEVEL2_ENABLED true
    #define _LEVEL3_ENABLED true
    #define _LEVEL4_ENABLED true
    #define _LEVEL5_ENABLED true
    #define _LEVEL6_ENABLED true
    #define _LEVEL7_ENABLED true
    #define _LEVEL8_ENABLED true

#endif

//#define DISABLE_LANG true
#ifdef _STEAM_BUILD
#define DISABLE_LANG true
#endif // _STEAM_BUILD

#define _DISABLE_WIDESCREEN true
#define _DISABLE_BUG_REPORT true
#define FORCE_CLOSE_VIDEO true
//#define FORCE_CLOSE_VID_END true
#define M3_BUTTON true

#ifdef K_ANDROID

	#define _CEBUILD true
	#define TOUCH_UI true
	//#define INSTANT_RECHARGE true
	#define UNLOCKED true

	//#define _DEMOBUILD true

	//#define DISABLE_DIFF true;
	#define DISABLE_PREV_CUR true //added for testing
	//#define ENABLE_ANDROID_BACK_BUTTON true

#endif // K_ANDROID



#ifdef _DEBUGL1
	#define _LEVEL1_ENABLED true
#elif _DEBUGL2
	#define _LEVEL2_ENABLED true
#elif _DEBUGL3
	#define _LEVEL3_ENABLED true
#elif _DEBUGL4
	#define _LEVEL4_ENABLED true
#elif _DEBUGL5
	#define _LEVEL5_ENABLED true
#elif _DEBUGL6
	#define _LEVEL6_ENABLED true
#elif _DEBUGL7
	#define _LEVEL7_ENABLED true
#elif _DEBUGL8
	#define _LEVEL8_ENABLED true
#elif _DEBUG
	#define _ALL_LEVELS_BUILD true	//All Levels Debug Compile
#elif NDEBUG
	//#define _LEVEL1_ENABLED true
	#define _ALL_LEVELS_BUILD true	//All Levels Release Compile
#endif

//constants to reduce compile time during debug mode
#ifdef _ALL_LEVELS_BUILD
#define _LEVEL1_ENABLED true
#define _LEVEL2_ENABLED true
#define _LEVEL3_ENABLED true
#define _LEVEL4_ENABLED true
#define _LEVEL5_ENABLED true
#define _LEVEL6_ENABLED true
#define _LEVEL7_ENABLED true
#define _LEVEL8_ENABLED true
#endif

#endif
