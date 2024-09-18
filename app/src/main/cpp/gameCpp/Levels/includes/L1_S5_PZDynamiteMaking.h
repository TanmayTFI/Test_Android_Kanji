//====================================
//DQFYLH
// Hidden Object System
//====================================

#ifndef DQFYLH_LEVEL1_SCENE5_DYNAMITEMAKING_DEF
#define DQFYLH_LEVEL1_SCENE5_DYNAMITEMAKING_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "CustomAnimGroup.h"
#include "Match3.h"
#include "MultilineTextBox.h"

class CHOScene;

class L1_S5_PZDynamiteMaking: public HiddenObjectSystem, CRenderListEntity
{
	public:

		//Scene Elements	

		float 	bgScaleOffsetX;
		float	bgScaleOffsetY;

		int	zOrderScnVideo, zOrderPopVideo;

		CObject *AmoniMixAnim;
		bool isAmoniMixAnimPlaying;
		CObject *CrystalTakeAnim;
		bool isCrystalTakeAnimPlaying;
		CObject *GreenPowderDropAnim;
		bool isGreenPowderDropAnimPlaying;
		CObject *GreenPowderTakeAnim;
		bool isGreenPowderTakeAnimPlaying;
		//CObject *GrinderAnim;
		bool isGrinderAnimPlaying;
		CObject *CapBottleAnim;
		bool isCapBottleAnimPlaying;
		//CObject *bluePowderTakeAnim;
		bool isbluePowderTakeAnimPlaying;
		
		CustomAnimGroup	*CAnim_Grinder, *CAnim_HammerLoop, *CAnim_BluePowderTake;
		CPoint hammerOffset[3];

		CObject *NailLoopAnim;
		CObject *NailPluckingAnim;

		CPoint NailLoopAnimPos[3];
		CPoint NailPluckingAnimPos[3];
		CPoint hammerPos[3];
		CPolygon *polyHammer[3];
		CRectangle *RectHammer[3];
		bool isNailPluckingAnimPlaying;
		bool isNailWrongPluckingAnimPlaying;

		bool isPuzzleOver;

		std::vector<SuperFX*>	VFXArray;
		SuperFX *MoonLightVFX, *LightVFX;

		CObject *amoniBottle;
		CObject *amoniBottleInHand;
		CObject *amoniBottleCork;
		CObject *bottle;
		CObject *closedBoxDoor;
		CObject *crystalInGrinder1;
		CObject *crystalInGrinder3;
		CObject *greenPowderOnPlate;
		CObject *bluePowderOnPlate;
		CObject *grinder;
		CObject *grinderWithCrystallInHand;
		bool isCrystallAddedToGrinder;
		CObject *hammer;
		CObject *hammerHand;
		CObject *map1;
		CObject *map2;
		CObject *nails[3];
		CObject *openedBoxDoor;
		CObject *rode;
		CObject *rodeInHand;
		CObject *speedBar;
		CObject *speedMeterBG;
		CObject *spoon;
		CObject *spoonWithBluePowder;
		bool isSpoonWithBluePowderInHand;
		CObject *spoonWithGreenPowder;
		bool isSpoonWithGreenPowderInHand;
		CObject *thread;
		CObject *threadOnBottle;
		CObject *threadWaxOnBottle;
		CObject *waxCylinder;

		CObject *grinderCoarse, *grinderMedium, *grinderFine, *rodInHand, *grinderScatter;
		CObject *Anim_HammerLoop, *Anim_HammerMaskLoop, *Anim_NailLoop;
		CObject *Anim_GrinderMask, *Anim_BluePowderMask, *Anim_Spoon, *Anim_SpoonWithPowder;

		CObject *ResetBtn;
		CHOScene *ResetDia;
		bool IsResetPop;

		float timer;
		bool isFadeIn;
		bool isFadeOut;
		bool isResetDiaFadeIn;
		bool isResetDiaFadeOut;

		bool isMapMoving;
		float movingTimer;
		float moveTime;

		float  mapStartPosX;
		float  mapStartPosY;

		CRectangle nailArea[3];
		int NumberOfNailsPlucked;
		int clickedNailIndex;

		std::vector<CPolygon*>	polyArray;

		CPolygon *polyBoxDoor;
		CPolygon *polyInvHammerHand;
		CPolygon *polyInvBomb;
		CPolygon *polyMap1;
		CPolygon *polyAmoniBottleCap;
		CPolygon *polyBottle;
		CPolygon *polyPlate;
		CPolygon *polyThreadGet;
		CPolygon *polyThreadSet;
		CPolygon *polyWaxCylinderGet;
		CPolygon *polyWaxCylinderSet;
		CPolygon *polyRodeGet;
		CPolygon *polyRodeSet;
		CPolygon *polyGrinderGet;
		CPolygon *polyGrinderSet;

		CPolygon *polySpoonGet;
		CPolygon *polySpoonSet;
		CPolygon *polyAmoniBottleGet;
		CPolygon *polyAmoniBottleSet;

		CPolygon *getPoly[13];
		CPolygon *setPoly[13];
		CObject *getObject[13];
		CObject *setObject[13];
		std::string mousePointerString[13];

		bool isObjectReturning;
		CPoint startPos,endPos;
		CObject *returnObject;
		int objectID;
		float returnTimer;
		float returnEndTimer;
		CObject *pointerInHand;
		bool isObjectInHand;

		CObject *UseObjects;
		float tweenTime;
		float swapTime;

		float easing(float t,float b,float c,float d);

		CPoint speedBarStarPos,speedBarEndPos;
		CPoint speedMeterBGPos;
		float barSpeed;
		bool isSpeedMeterRunning;

		//Popups
		int ActivePopupID;

		CHOScene *Popup1Notice;
		MultilineTextBox* NH4, *minigrinderTxt, *NH4NO3, * HNO3, *threadTxt, *waxTxt;
		
		int Popup1_State;
		int Popup2_State;
		CPolygon *polyNotice;
		//Popup Elements
		
		bool IsPopupTransitionActive;
		float PopupAlpha;
		void SetupPopupTransition(CHOScene *Popup, CRectangle popRect);
		void UpdatePopupTransition();

		L1_S5_PZDynamiteMaking();
		~L1_S5_PZDynamiteMaking();

		//Inherit this CRenderListEntity function to render anything to scene!! 
		void Render(const CPoint* pCam);
		
		void InitPopups();
		void UpdatePopups(CPoint pos,bool ItemDropped=false);
		void ClosePopup();
		void TakeInventoryItem(EHOInventory ItemID);
		void Update();
		void Required();
		
		void ProcessSkip();		
		void ResetPuzzle();

		void NavigateToScene();

	protected:

		void ReceiveMessage(int val);

		void TaskPendingUpdate();
		void ProcessNonHOHint();
};

#endif