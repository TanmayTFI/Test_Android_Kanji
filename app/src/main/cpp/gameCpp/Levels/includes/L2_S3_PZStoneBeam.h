//====================================
// DQFYLH
// Hidden Object System
// Level 2 - Scene 03 : STONEBEAMPUZZLE
//====================================

#ifndef DQFYLH_LEVEL2_SCENE3_PZSTONEBEAM_DEF
#define DQFYLH_LEVEL2_SCENE3_PZSTONEBEAM_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Polygon.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "Circle.h"

class CHOScene;
class SmallCircle
{
	public:
	
	CCircle *cirArea;
	int rotationSpeed;
	int rotationStartVal;
	int rotationEndVal;

	CObject *cir;
	int originalPos;
	int currentPosition;
	bool isSelected;

	SmallCircle();
	SmallCircle(CObject *obj);
	~SmallCircle();

	bool isInPlace();
	void swap(SmallCircle *obj);
	void swap2(SmallCircle *obj);
};

class L2_S3_PZStoneBeam: public HiddenObjectSystem
{
	public:
		float musicFadeOutTimer;
		//Puzzle Elements
		SmallCircle *cirPieces[10];
		CPolygon *ArrowPoly[10];
		CRectangle arrArrows[10];
		CObject *arrowGlowArray[10];
		CObject *redGlow[10], *correctFixGlow[10];
		CObject *patternSuccessGlow[10];

		CObject *centerWheelRedLit;
		CObject *centerWheelGreenLit;

		CObject *ResetBtn;
		CHOScene *ResetDia;
		bool IsResetPop;
		float timer;
		bool isFadeIn;
		bool isFadeOut;
		bool isResetDiaFadeIn;
		bool isResetDiaFadeOut;

		bool isCenterWheelActivated;

		SuperFX *s3smokeFX, *s3fliesFX;

		CObject *glow1, *glow2;

		L2_S3_PZStoneBeam();
		~L2_S3_PZStoneBeam();

		void InitPuzzle();
		void ShuffleCircles();
		void InitArrowArray();
		void TakeInventoryItem(EHOInventory ItemID);
		void Update();
		void Required();
		void ProcessSkip();
		void ResetPuzzle();
		SmallCircle* getSwapableCircle(SmallCircle *obj);
		void clearIsSelecteFlag();
		void rotateBigCircle(float easeTime);
		void rotateReverseBigCircle(float easeTime);
		bool isPuzzleSolved();
		void checkCirclePiecesInProperPlace();
		void checkRotationStatus();
		void clearRedGlow();
		bool isPuzzleOver;

		void showGlowArrow(int index);
		void hideGlowArrow();

		float easing(float t,float b,float c,float d);
		bool isRotating;
		bool isSwaping;
		bool isCenterWheelRotating;
		
		bool isCenterWheelEnabled;
		SmallCircle *swapingCircle;
		float startRotVal;
		float endRotVal;
		float tweenTime;
		float rotTime;
		float swapTime;

		bool isCenterWheelShaking;
		float ShakeTimer, ShakeRotValue;

		int rotPieceId;
		int swapPieceId;

		CPoint swapCircle1StartPos;
		CPoint swapCircle1EndPos;

		CPoint swapCircle2StartPos;
		CPoint swapCircle2EndPos;

		void fillSaveData();

		int centerWheelStartPos;
		int centerWheelEndPos;

		void NavigateToScene();


	protected:

		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};


#endif

