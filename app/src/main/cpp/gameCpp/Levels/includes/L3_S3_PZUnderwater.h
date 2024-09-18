//====================================
// DQFULP
// Hidden Object System
// Author : Faisal
// LEVEL3 - SCENE3 : PZUnderwater
//====================================

#ifndef DQFYLH_LEVEL3_SCENE3_PZUNDERWATER_DEF
#define DQFYLH_LEVEL3_SCENE3_PZUNDERWATER_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "SuperFX.h"

#include "MeshObject.h"

#include "VideoDefs.h"
#include "KVideo.h"

class CHOScene;

class L3_S3_PZUnderwater: public HiddenObjectSystem
{
	public:			
		std::vector<CObject*>	PinkGlowObjs;
		std::vector<CObject*>	BlueGlowObjs;
		std::vector<CObject*>	RedGlowObjs;
		float musicFadeOutTimer;
		CObject *SymbolGlowBlue, *SymbolGlowPink, *SymbolGlowRed;

		CHOScene *UWScene;
		int prevPosTop;
		int prevPosUW;

		MeshObject *meshTopWater, *meshUGWater;

		SuperFX *s3bubbleRFX, *s3bubbleLFX;

		CObject *UWOxyBar, *UWOxyRedLight;
		float OxyTimer;

		SuperFX *WinParticleFX;
		bool IsWinningFXActive;
		float winFXTimer;

		bool isSkippedUnderwater;

		bool IsScreenScrolling;
		bool IsDivingToWater;
		bool IsRisingToSurface;

		bool IsUnderwater;

		float ScrollTimer;
		float ScrollTime;
		
		//--------------------------- Functions -------------------------------//
		L3_S3_PZUnderwater();
		~L3_S3_PZUnderwater();
		
		//_PUZZLE
		void InitPuzzle();
		
		float easing(float t,float b,float c,float d);

		CObject *OuterKeys[8],*DiagonalKeys[8],*InnerKeys[4];
		bool OuterKeyState[8],DiagonalKeyState[8],InnerKeyState[8];

		CRectangle *OuterRects[8], *OuterBackRects[8];
		CRectangle *DiagRects[8], *DiagBackRects[8];
		CRectangle *InnerRects[4], *InnerBackRects[4];

		CObject *ResetBtn;
		CHOScene *ResetDia;
		bool IsResetPop;

		float timer;
		bool isFadeIn;
		bool isFadeOut;
		bool isResetDiaFadeIn;
		bool isResetDiaFadeOut;

		void RiseupFromUnderwater();
		void StartDivingUnderwater(int colorID);
		void SetupUnderWaterScene();
		void SaveKeyStatesUpdateTop(bool saveState);
		void HideKeys();

		void SetButtonGlow(int colorID, bool enable);

		bool IsGameWon;
		bool CheckIfGameWon();

		int activeColorId;

		//_PUZZLE
		void ProcessSkip();
		void Update();
		void Required();
		void ResetPuzzle();

		void NavigateToScene();

	protected:
		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif