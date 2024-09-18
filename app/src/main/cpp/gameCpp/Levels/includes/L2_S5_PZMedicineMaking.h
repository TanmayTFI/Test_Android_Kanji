//====================================
// DQFYLH
// Hidden Object System
// Level 2 - Scene 05 : Medicine Making Puzzle
//====================================

#ifndef DQFYLH_LEVEL2_SCENE5_PZMEDICINEMAKING_DEF
#define DQFYLH_LEVEL2_SCENE5_PZMEDICINEMAKING_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "SuperLog.h"
#include "CustomAnimGroup.h"
#include "JSONAnimator.h"
#include "L2S5_Pop6Chart.h"
#include "ClickablePolygon.h"

class CHOScene;

class L2_S5_PZMedicineMaking: public HiddenObjectSystem
{
	public:

		float musicFadeOutTimer;
		//scene objects

		CObject *AloeVeraCut;
		CObject *AloeVeraFull;
		CObject *AloeVeraHalf;
		CObject *ChirattaEmber;
		CObject *CrusherInHand;
		CObject *CrusherOnGround;
		CObject *Grinder;
		CObject *PestleInHand;
		CObject *PestleOnGround;
		CObject *HoneyDrip1;
		CObject *HoneyDrip2;
		CObject *HoneyDrip3;
		CObject *HoneyMangoInGrinder0;
		CObject *HoneyMangoInGrinder1;
		CObject *HoneyMangoInGrinder2;
		CObject *HoneyPotDrop;
		CObject *HoneyPotEmpty;
		CObject *HoneyPotFilled;
		CObject *HoneyPotInHand;
		CObject *HoneyWaxCut;
		CObject *HoneyWaxFull;
		CObject *HoneyInGrinder;
		CObject *MangoCut;
		CObject *MangoFull;
		CObject *MangoHalfCut;
		CObject *MangoInGrinder;
		CObject *PotEmpty;
		CObject *SpoonPour;
		CObject *SpoonOnGround;
		CObject *SpoonInHand;
		CObject *Mortar1;
		CObject *Mortar2;
		CObject *Mortar3;
		CObject *Mortar4;
		CObject *TurmericCut;
		CObject *TurmericFull;
		CObject *TurmericHalfCut;
		CObject *MortarDrop;
		CObject *TurmericPice;
		CObject *WoodLog;
		CObject *HangingRoot1;
		CObject *HangingRoot2;
		CObject *knife;
		CObject *PotFilled;
		CObject *MortarInHand;
		CObject *WindowLight;
		CObject *Kindi;
		CObject *MortarEmpty;
		CObject *PotMask, *KindiMask;
		CObject *WoodFireAnim;
		CObject *TorchAnim;
		CObject *SpoonWithMedicine;
		CObject *HoneyPotInHandHalf, *HoneyPotInHandEmpty, *HoneyPourMask;
		CObject *AloeLeaf1, *AloeLeaf2, *AloeLeaf3, *AloeLeaf4, *AloeLeaf5, *AloeLeaf6;
		CObject *MedicineMask, *SpoonStir;
		CObject *MortarAnim1, *MortarAnim2;

		bool isChartPopupActivated;
		bool isPuzzleModeActive;
		bool isObjectReturning;
		CPoint startPos,endPos;
		CObject *returnObject;
		int objectID;
		float returnTimer;
		float returnEndTimer;
		CObject *pointerInHand;
		bool isObjectInHand;
		bool isAnimPlaying;
		std::vector<CPoint> returnObjPos;

		SuperFX *S5WaterBoilSteamFX;

		CustomAnimGroup *CAnim_AloeveraDrop;
		CustomAnimGroup *CAnim_TurmericCrush;
		CustomAnimGroup *CAnim_HoneyMangoGrind;
		CustomAnimGroup *CAnim_HoneyMangoDrip;
		CustomAnimGroup *CAnim_MedicineTake;
		CustomAnimGroup *CAnim_AddHoneyToPot;
		CustomAnimGroup *CAnim_AddTurmericToPot;

		JSONAnimator	*JAnim_AloeveraCut;
		JSONAnimator	*JAnim_HoneyCut;
		JSONAnimator	*JAnim_MangoCut;
		JSONAnimator	*JAnim_TurmericCut;
		JSONAnimator	*JAnim_WoodFire;
		JSONAnimator	*JAnim_MedicineMix;
		JSONAnimator	*JAnim_MedicinePour;
		JSONAnimator	*JAnim_TurmericAddToPot;

		std::vector<SuperFX*> superfxArray;
		SuperFX *MedicineWindowDustFX;
		SuperFX *TurmericFX;

		int ActivePopupID;
		int StartZorder;

		bool isMangoCutAnimPlaying;
		bool isAloeveraCutAnimPlaying;
		bool isAloeveraDropAnimPlaying;
		bool isHoneyCutAnimPlaying;
		bool isTurmericCutAnimPlaying;
		bool isTurmericCrushAnimPlaying;
		bool isGrinderAnimPlaying;
		bool isWoodFireAnimPlaying;
		bool isMedicineTakeAnimPlaying;
		bool isAddHoneyToPotAnimPlaying;
		bool isTurmericAddToPotPlaying;
		bool isAloeveraInHand;
		bool isHoneyInHand;
		bool isTurmericInHand;
		bool isMangoInHand;
		bool isSpoonInHand;
		bool isGrinderPestleInHand;
		bool isMortarPestleInHand;
		bool isMortarInHand;
		bool isHoneyPotInHand;
		bool isSpoonWithMedicineInHand;


		//Popups
		L2S5_Pop6Chart *pop6Chart;
		//CPolygon *ChartPoly;

		L2_S5_PZMedicineMaking();
		~L2_S5_PZMedicineMaking();


		void OnPopAreaClicked(int popId);
		void OnPopupClosed(Popup * popup);

		void InitVariables();
		void InitAnimations();
		
		void SetSceneElements();

		void Update();
		void Required();

		float easing(float t,float b,float c,float d);
		void OnAnimationComplete(int id);

		void NavigateToScene();

	protected:

		CPolygon *InvKIndiPoly, *ChartPoly, *MedicineTablePoly, *StovePoly, *GrinderPoly, *HoneyPotPoly, *FireWoodDestPoly;
		CPolygon *MortarPoly, *CrusherPoly, *PestlePoly, *SpoonPoly, *MangoPoly, *HoneyPoly, *TurmericPoly, *AloveraPoly;

		void InitControlPolys();
		void UpdateBannerMessages( const CPoint pos );
		void TaskPendingUpdate();
		void ReceiveMessage(int val);
		void ProcessNonHOHint();

};

#endif