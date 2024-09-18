//====================================
// DQFYLH
// Hidden Object System
// Level 2 - Scene 06 : PUZZLE TUNNELCLOCK
//====================================

#ifndef DQFYLH_LEVEL2_SCENE6_PZTUNNELCLOCK_DEF
#define DQFYLH_LEVEL2_SCENE6_PZTUNNELCLOCK_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "AssetsStringTable.h"
#include "CVideo.h"

class CHOScene;

class L2_S6_PZTunnelClock: public HiddenObjectSystem
{
	public:
		float musicFadeOutTimer;
		//Scene Elements
		CObject *Scn_Bg;

		bool isPuzzleDoorOpenVideoPlaying;
		CVideo *cv_PuzzleDoorOpen;
		float time; float vidTimer;
		int subIndex;
		subText subtitles[5];
		CBitmapText* subtitleText; int subMax;

		SuperFX *bgFogVFX_0;
		SuperFX *bgFogVFX_1;

		CHOScene *sceneTunnel;

		int Popup1_State;		

		CObject *pearl[4];
		CObject *arrow[3];
		CObject *arrowOverlay[3];
		float arrowAngle[3];
		int getNearestArrowIndex(float angle);

		int lastSolvedIndex;

		CObject *activeArrow;
		CObject *centerPoint;
		CObject *chart;

		SuperFX *s6ClockWrongFX[4], *s6ClockRightFX[4];

		CObject *closedShutter[4];
		CObject *shutterOpenAni[4];

		CObject *greenPearlGlows[4];
		CObject *greenAnimation[4];
		CObject *redAnimation[4];
		CObject *solierPuzleBg;
		CObject *fireAnim;

		CObject *ResetBtn;
		CHOScene *ResetDia;
		bool IsResetPop;

		float timer;
		bool isFadeIn;
		bool isFadeOut;
		bool isResetDiaFadeIn;
		bool isResetDiaFadeOut;

		int currentIndex;

		float leftSuccessAngles[3];
		float rightSuccessAngles[3];
		float topSuccessAngles[3];
		float bottomSuccessAngles[3];
		
		float snapValues[12];
		float snapValues2[12];

		float startAngle[3]; // for reverse arrow animation
		float endAngle[3];

		CObject *snapGlow[12];		

		bool isMouseMove;
		bool isMouseDown;

		bool isPuzzleSolved[4];
		bool isPuzzleOver;

		float clockLoopSFXInterval;
		float prevAngle;

		
		L2_S6_PZTunnelClock();
		~L2_S6_PZTunnelClock();

		float getMouseAngle(CPoint *pos);
		float getArrowAngle(int index);

		CRectangle getNewRect(float angle,CRectangle rect);
		int checkShutterImageStatus();

		bool isPearlPlaced;
		bool isPearlAlphaChanging;
		bool isShutterOpening;
		float startAlpha;
		float endAlpha;
		float tweenTime;
		float alphaTime;
		float shutterOpenTime;
		float waitTime;
		float rotTime;
	
		bool isPreparingReverseClockAnimation;
		bool isReverseClockAnimation;

		void InitPuzzle();

		void Update();
		void Required();

		float easing(float t,float b,float c,float d);
		void ProcessSkip();

		void HandlePuzzleComplete();

		void ResetPuzzle();

		void NavigateToScene();

	protected:

		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif