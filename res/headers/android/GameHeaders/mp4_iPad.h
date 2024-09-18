
#ifndef K_MP4_IPAD_DEF
#define K_MP4_IPAD_DEF true
#include "iPadDefs.h"
	#ifdef K_IPAD
#include "KPTK.h"
#include "KGame.h"
#include "KMath2d.h"


        CRectangle getSysVideoRect();
		void  sysPlayVideo (KWindow *g_lpKWindow, const char *lpszVideoName, int StartX, int StartY,int Width, int Height, bool Repeat, bool Pause = false);
		void  sysStopVideo (void);
        bool  isSysVideoHidden();
        bool  isSysVideoCached( std::string lpszVideoName );
        bool  isSysVideoLoading( std::string lpszVideoName );
        bool  isSysVideoPBStatePaused();
        bool  isSysVideoLoaded();
        bool  isPrevPauseStatus();

        bool  unHideIfSysVideoLoaded();

		bool  sysHasVideoStopped (void);
		void  sysVideoHidePause( bool Status );
		void  sysVideoPause( bool Status );
        void  sysVideoHide( bool Status );
		float sysVideoPlaybackTime(void);
        void  sysVideoRewind();
         
	#endif
#endif
