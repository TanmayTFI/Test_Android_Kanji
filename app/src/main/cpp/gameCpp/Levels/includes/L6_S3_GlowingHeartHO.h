
#ifndef DQFYLH_L6_S3_GLOWINGHEART_HO_DEF
#define DQFYLH_L6_S3_GLOWINGHEART_HO_DEF

//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Popup.h"
#include "Match3.h"

#include "L6S3_HOPop1Code.h"
#include "L6S3_HOPop2Vase.h"

enum L6S3HOObjects
{
	e_Wheel = 0,
	e_Rail,
	e_Spanner,
	e_TopHandle,
	e_LaserDevice,
	e_L6S3HOMax
};

enum L6S3HO_ObjectStates
{
	e_L6S3_Locked = -1,
	e_L6S3_Inactive,
	e_L6S3_Active,
	e_L6S3_Used
};

enum eL6S3HOPopEnums
{
	ePop1_L6S3Pop1Code,
	ePop2_L6S3Pop2Vase,
	ePopL6S3_Max
};

class L6_S3_GlowingHeartHO : public HiddenObjectSystem
{
public:
	L6_S3_GlowingHeartHO ();
	~L6_S3_GlowingHeartHO ();
	float musicFadeOutTimer;
	void Update();
	void Required();
	void NavigateToScene();

protected:

	//Match 3 code
	Match3 *match3Ctrl;
	CObject *BtnMatch3;
	CObject *BtnSwitchToHO;
	//Match 3
	CObject *match3Back, *match3BackText;
	L6S3_HOPop1Code *Pop1Code;
	L6S3_HOPop2Vase* Pop2Vase;
	JSONAnimator*	JanimPuzzleDoorRetreat;
	Popup*			PopupArray[ePopL6S3_Max];
	CPolygon*		PopPolyArray[ePopL6S3_Max];
	int				PopStates[ePopL6S3_Max];

	SuperFX *S3_HOLightEffectFX;
	SuperFX *S3_HO_GoldGlitterFX;
	SuperFX *S3_HO_GoldGlitter2FX;
	SuperFX *S3_HO_GoldGlitter3FX;

	CPoint	CartObjectsPos[20];
	CObject *CartObjects[20];
	CObject *Box, *BoxClosed, *BoxPuzzleDoor, *Code, *CodeInScene, *Effects, *GemGlow01, *GemGlow02, *GemGlow03;
	CObject *Gems, *LaserDevice, *LaserDeviceInHand, *LaserLight, *LesarDeviceInScene, *LightRays, *MainLock01, *MainLock01Open, *MainLock02, *MainLock02Open, *MainLock03, *MainLock03Open;
	CObject *Rail, *RailInScene, *RailOnClick, *SideLock, *SideLockOpen, *Spanner, *SpannerOnClick, *TopHandle, *TopHandleInScene, *TopHandleOnClick, *TyreInPlace, *TyreInScene, *TyreOnclick;
	CObject *Vase, *VaseBottom, *Vase_Door, *Bolt1, *Bolt2, *BoxMask, *Heart;
	CObject	*Lever[3];

	bool IsAnimationPlaying;
	int IsLockOpening;
	int ObjectInHandID;
	int ActivePopupID;
	CPolygon *CodePoly, *PuzzlePoly;

	CPolygon *HoBoxPoly;
	CPolygon *RailPoly2;
	CPolygon *RailPoly1;
	CPolygon *LeverPoly[3];

	int moveDirection;
	CRectangle	 HeartRect[3];
	CObject		*HOScnObjects		[e_L6S3HOMax];
	CObject		*HOInHandObjects	[e_L6S3HOMax];
	CObject		*HODestObjects		[e_L6S3HOMax];
	CPolygon	*HOPolyArray		[e_L6S3HOMax];
	CPolygon	*DestPolyArray		[e_L6S3HOMax];
	int			 HOObjStates		[e_L6S3HOMax];

	void CheckForPendingTask();
	void UpdateBannerMessages(CPoint pos);
	void CheckIfHOComplete();
	void MoveCart(int dir);
	void StopParticleEmission();
	void OnPopupClosed(Popup* popup);
	void InitPopups();
	void InitSceneElements();
	void InitAnimations();
	void OnAnimationComplete(int index);
	void InitControlPolys();
	void SparkleUpdate();
	void TaskPendingUpdate();
	void SceneObjectsUpdate();
	void UpdateGems();
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
	bool CheckIfPopupActive(int popupId);
};
#endif