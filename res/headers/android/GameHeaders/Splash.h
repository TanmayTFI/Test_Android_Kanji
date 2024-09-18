//====================================
// Framework
// Splash Series Control
//====================================

#ifndef FRMWRK_SPLASH_DEF
#define FRMWRK_SPLASH_DEF

#include "Control.h"
#include "SuperFX.h"
#include "BuildConstants.h"

//#define THIRDPARTY_SPLASH true

#ifdef THIRDPARTY_SPLASH
	#define MAX_SPLASHES 3
#else
#ifdef _PUBLISHER_BUILD
	#define MAX_SPLASHES 2
#else
	#define MAX_SPLASHES 1
#endif
#endif

class SplashSeries : public Control
{
	public:
		void HandleSplashOver();
		SplashSeries();
		~SplashSeries();

		//Inherited Method
		void Update();
		KGraphic *SplashImage[MAX_SPLASHES];
		float SplashLen;
		float SplashFadeOutStart, SplashFadeInEnd;
		void DrawSplash();

	private:
		float	TimeElapsed;
		int		CurrentSplash;
};

#endif