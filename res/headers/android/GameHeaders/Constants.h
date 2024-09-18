//===========================================================
// 
// Framework
//===========================================================

#ifndef DATA_CONSTANTS_H
#define DATA_CONSTANTS_H

#ifdef K_WIN32 //Linux:  added because there was a error for max and min definitions below which needed deque
	// add nothing
#else
	#include <deque>
#endif

#define MAX_DISPLAY_WIDTH 1366.0f 
#define MAX_DISPLAY_HEIGHT 768.0f

//#ifndef max 
// Sac: max is too common to be used as overloaded Macro, so changing it to g_max
// #define max(a,b)            (((a) > (b)) ? (a) : (b))
 #define g_max(a,b)            (((a) > (b)) ? (a) : (b))
//#endif

//#ifndef min
//#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define g_min(a,b)            (((a) < (b)) ? (a) : (b))
//#endif
#if !defined _STEAM_BUILD
    #if (defined K_MACOS)
        #define RES_PASSWRD "iK74ILyJ7*smY3#XyFj9TJ3r&"
    #elif (defined K_ANDROID)
        #define RES_PASSWRD "jh98^(*JKG^RHLJH(&FJjhg97t9KUJg&6kjg$f7r4ghf"
    #elif (defined K_WIN32)
        #define RES_PASSWRD "iK74ILyJ7*smY3#XyFj9TJ3r&"
    #elif (defined K_LINUX)
        #define RES_PASSWRD "iK74ILyJ7*smY3#XyFj9TJ3r&"
    #elif (defined K_iOS)
        #define RES_PASSWRD "jh98^(*JKG^RHLJH(&FJjhg97t9KUJg&6kjg$f7r4ghf"
    #else
        #define RES_PASSWRD "T"
    #endif
#else
    #if (defined K_ANDROID)
        #define RES_PASSWRD "jh98^(*JKG^RHLJH(&FJjhg97t9KUJg&6kjg$f7r4ghf"
    #else
        #define RES_PASSWRD "ThisIsCommon@TF934hjf0ewk6&afHDE*IW$@FbWyv3iFU*"
    #endif
#endif

#define EMPTY "empty"

#define CLOSE	"close"
#define OPEN	"open"

#define HIDDEN_OBJECT	"HiddenObject"
#define OBJECT			"Object"
#define BACKGROUND		"Background"

#define INV_CLOSE		10001
#define VAL_CLOSE		10002
#define VAL_FLYING		10003
#define MAP_NEW_AREA	10012
#define UPDATE_SCENE_OBJECTS	10013
#define UPDATE_AREA_TASK 10016
#define MAP_HUD_CLICK 10017
#define MEMORY_OBJFX_REACHED 10018


#define MAX_LOWEND_PEAKMEMORY	500//3000//trial build 

enum EBlend
{
	eBlend_AlphaAdditive,
	eBlend_AlphaNormal,//Normal
	eBlend_AlphaSatSub,
	eBlend_AlphaSubtractive,
	eBlend_AlphaMultiply
};


#endif
