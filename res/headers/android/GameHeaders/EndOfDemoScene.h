//====================================
// DQFYLH
// Hidden Object System
// Level 1 - END OF DEMO
//====================================

#ifndef DQFYLH_LEVEL1_ENDOFDEMO_DEF
#define DQFYLH_LEVEL1_ENDOFDEMO_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"

#include "VideoDefs.h" 
#include "KVideo.h"
#include "CVideo.h"
#include "AssetsStringTable.h"
#include "MultilineTextBox.h"

class ParticleEffect;
class CHOScene;

#define MAX_NUM_WALLS 10

class EndOfDemoScene: public HiddenObjectSystem, CRenderListEntity
{
	public:

		//Scene Elements
		#ifdef K_ANDROID
			CObject *BtnUnlockNow;
			CObject *BtnMainMenu;
			CObject *BoughtImage;

			CObject *BtnExit;
		#else		
			CObject *BtnExit, *BtnExitText;
		#endif
		
		CObject* exitTxt;
		CObject *wallThumbArr[MAX_NUM_WALLS];

		MultilineTextBox* desc, * thankyou;

		bool IsOutroVideoPlaying;

		bool	IsVidSkippable, IsSkipMouseOver, IsSkipMouseClicked;
		float	SkipShowTimer;
		CObject *skipObj;

		int ActivePopupID;

		int activeWallpaper;

		int Popup1_State;
		CHOScene *SCNWallPop;
		CObject *PopMaskBg;

		CVideo *cv_OutroVideo;
		float time;
		float vidTimer;
		int subIndex;
		subText subtitles[15];
		CBitmapText* subtitleText; int subMax;

		EndOfDemoScene();
		~EndOfDemoScene();

		void InitPopups();
		void UpdatePopups(CPoint pos,bool ItemDropped=false);		

		bool IsPopupTransitionActive;
		float PopupAlpha, PopVal;
		void SetupPopupTransition(CHOScene *Popup, CRectangle popRect);
		void UpdatePopupTransition();

		float easingQuadOut(float t,float b,float c,float d);

		//Inherit this CRenderListEntity function to render anything to scene!! 
		void   Render(const CPoint* pCam);

		void Update();
		void Required(){};

	protected:

		void ReceiveMessage(int val){};

};

#endif
