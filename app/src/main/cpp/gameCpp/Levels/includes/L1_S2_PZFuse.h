//====================================
//DQFYLH
// Hidden Object System
//====================================

#ifndef DQFYLH_LEVEL1_SCENE2_PZFuse_DEF
#define DQFYLH_LEVEL1_SCENE2_PZFuse_DEF

#include "HiddenObjectSystem.h"
#include "HOObject.h"

class CHOScene;
class CObject;

#define MAX_FUSES 4

class RNode{
public:
	float x;
	float y;

	RNode(float x,float y);
};

class Rope{
public:
	int numOfNodes;
	float damp;
	float gravity;

	RNode *nodes[20];

	KGraphic *RopeGraphic;

	void Render();
	float DistanceBetween(float x1,float y1,float x2,float y2);

	Rope(float x1, float y1, float x2, float y2);
	~Rope();
};


class L1_S2_PZFuse : public HiddenObjectSystem, CRenderListEntity
{
	public:
		
		bool IsStaticFuse1InHand, IsStaticFuse2InHand;
		bool IsStaticFuse1Fixed, IsStaticFuse2Fixed;		
		bool IsStaticFuseLeftFixed, IsStaticFuseRightFixed;

		bool IsAllStaticFusesFixed;

		bool IsRotatingFusesCorrect;

		CObject *StaticFuse1, *StaticFuse2;		
		CObject *StaticFuseTaken;
		CObject *StaticFuseFixedLeft, *StaticFuseFixedRight;

		CObject *LightPlus, *LightMinus;

		CObject *GreenWireDown, *GreenWireTop;
		bool IsGreenWireDownFixed, IsGreenWireTopFixed;

		CObject *FuseSpark1, *FuseSpark2, *LeverGlow, *LeverLiftGlow, *Lightning;
		CObject *FuseSparkGlow1, *FuseSparkGlow2;

		CObject *ResetBtn;
		CHOScene *ResetDia;
		bool IsResetPop;

		float timer;
		bool isFadeIn;
		bool isFadeOut;
		bool isResetDiaFadeIn;
		bool isResetDiaFadeOut;

		float sparkTimer;
		float sparkTimeGap;

		float greenSparkTimer;
		float greenSparkTimeGap;

		float sparkWireTimer;
		float sparkWireTimeGap;

		//CObject	*Lightning;
		float lightningTimer,lightFrame, centerLightTimer, nextLightningThreshold;
		float musicFadeOutTimer;
		CObject *CenterLight;

		float startAngle;
		float endAngle;
		float rotateTime;
		float tweenTime;

		float wFuseTimer;

		int rotatingFuseId;

		bool IsRotating;
		bool IsLeverAnimating;

		bool IsBgLightAnimationPlaying;

		//CObject *BgLitAnim;

		SuperFX *RainBgVFX, *SparkVFX, *FliesLeftVFX, *FliesRightVFX, *RaindropsVFX;

		CObject *RotFuses[MAX_FUSES], *RotFuseGlows[MAX_FUSES];

		CObject *GreenLightUpLeft,*GreenLightUpRight;
		CObject *GreenLightGlowLeft,*GreenLightGlowRight;

		CObject *WireFuses[MAX_FUSES];
		CObject *WireFuseFixed[MAX_FUSES];
		CObject *WireFuseFixedAlt[MAX_FUSES];

		Rope	*Ropes[MAX_FUSES];

		bool	IsWireFuseInHand;
		int		wireFuseTakenId;

		bool	IsWireFusesFixCorrect;

		bool	IsLeverTurningUp;
		bool	IsLeverTurningDown;
		float	LeverAnimatingTimer;

		CRectangle	*RotFuseRects[MAX_FUSES];
		CRectangle	*WireFuseRects[MAX_FUSES];
		CRectangle	*WireFuseFixRects[MAX_FUSES];

		CRectangle	*WireFuseTakeRects[MAX_FUSES];
		CObject		*Lever;

		KGraphic	*RopeGraphic;

		L1_S2_PZFuse();
		~L1_S2_PZFuse();

		bool IsShaking;
		CObject *ShakeObject;
		float ShakeTimer;
		float ShakeRotValue;
		void Shake(CObject *obj);
		
		float easing(float t,float b,float c,float d);
		void InitPuzzle();
		void InitRotatingFuses();
		void InitWireFuses();
		void ShowMessage(int messageId);
		void ShowChargeLight(bool show,int fuseId);
		void setStatusOfLights();
		void CheckWireFusePlacements(CPoint pos);	
		void PlayRandomSparkSFX();

		//Inherited Methods
		void ProcessSkip();
		void Update();
		void Render(const CPoint* pCam);
		void Required();//add
		void ResetPuzzle();

	void NavigateToScene();
	protected:
		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif