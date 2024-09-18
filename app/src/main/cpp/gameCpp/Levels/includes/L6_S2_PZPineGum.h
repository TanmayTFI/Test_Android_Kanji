

#ifndef DQFYLH_LEVEL6_SCENE2_PUZZLE_PINE_GUM_DEF
#define DQFYLH_LEVEL6_SCENE2_PUZZLE_PINE_GUM_DEF

//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "JSONAnimator.h"
#include "CustomAnimGroup.h"
#include "L6S2_PZPop1Chart.h"

class L6_S2_PZPineGum : public HiddenObjectSystem
{
public:
	L6_S2_PZPineGum();
	~L6_S2_PZPineGum();
	float musicFadeOutTimer;
	void Update();
	void Required();
	void NavigateToScene();
	CObject *ChartInScene;
	CObject *Coal;
	CObject *CoalPowder;
	CObject *FlintInHand;
	CObject *GlueStick_Scene;
	CObject *Leaves;
	CObject *Leaves01InScene;
	CObject *Leaves02InScene;
	CObject *Leaves03InScene;
	CObject *LeavesInHand;
	CObject *MortarInHand;
	CObject *MortarInScene;
	CObject *Pan;
	CObject *PanInHand;
	CObject *PanInScene;
	CObject *PestleInHand;
	CObject *PestleInScene;
	CObject *PineGlue;
	CObject *PinePitch;
	CObject *PinePitchMelted;
	CObject *PlantMask;
	CObject *Rock01;
	CObject *Rock01InHand, *Rock01InScene;
	CObject *Rock02;
	CObject *Rock02InHand, *Rock02InScene;
	CObject *Rock03;
	CObject *Rock03InHand;
	CObject *SteelInHand;
	CObject *StickInHand;
	CObject *StickInScene;
	CObject *Fire;
	

protected:
	std::vector<SuperFX*> superfxArray1, superfxArray2;
	SuperFX *FireSparks;
	SuperFX *VapourFX;

	bool IsStickInHand;
	bool IsAnimPlaying;
	bool IsPuzzleModeActive;
	CHOScene  *ChartScene;
	CPolygon  *StickPoly, *PanPoly;

	CObject		*PZScnObjects		[MAX_L6S2_PZOBJECTS];
	CObject		*PZInHandObjects	[MAX_L6S2_PZOBJECTS];
	CPolygon	*PZPolyArray		[MAX_L6S2_PZOBJECTS];
	CPolygon	*DestPolyArray		[MAX_L6S2_PZOBJECTS];
	int			 PZObjStates		[MAX_L6S2_PZOBJECTS];
	
	JSONAnimator *FireMakingAnim, *CharcoalCrushAnim, *PineGumMeltAnim, *GlueMixAnim, *CharcoalAddAnim;

	int ObjectInHandID;
	int ActivePopupID;
	bool IsFadingIn;
	bool IsFadingOut;
	bool IsChartActive;
	float FadeAlpha;

	//Popups
	L6S2_PZPop1Chart *InstructionChart;
	CPolygon *ChartPoly;
	void OnPopupClosed(Popup * popup);
	void UpdateBannerMessages(CPoint pos);
	void CloseChartScene();
	void UpdatePuzzleMode(CPoint pos);
	void UpdatePuzzleStates();
	void ResetDragItem();
	void OnAnimationComplete(int index);
	void InitAnimations();
	void InitControlPolys();
	void InitSceneElements();
	void TaskPendingUpdate();
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
	void SparkleUpdate();
	void CheckForPendingTask();
};

#endif
