//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL4 - SCENE2 : RIVER SIDE
//====================================

#ifndef DQFYLH_LEVEL4_SCENE3_CAVE_DEF
#define DQFYLH_LEVEL4_SCENE3_CAVE_DEF

#include "HiddenObjectSystem.h"
#include "ClickablePolygon.h"
#include "L4S3_Pop1AntiquePot.h"


class L4_S3_Cave: public HiddenObjectSystem, CRenderListEntity
{
	public:
		float musicFadeOutTimer;
		//popups
		std::vector<ClickablePolygon*> PopRectArray;
		std::vector<Popup*> PopupArray;

		int ActivePopupId;
		L4S3_Pop1AntiquePot *pop1;
		CVideo *cv_EndCutscn;
		float time;
		float vidTimer;
		int subIndex;
		subText subtitles[10];
		CBitmapText* subtitleText; int subMax;
		JSONAnimator *DoorOpenAnim;
		bool	isVideoPlaying;

		CObject *MagicBluePlant01;
		CObject *MagicBluePlant02;
		CObject *MagicBluePlant03;
		CObject *PlantAnim;
		CObject *CraniumBolts01;
		CObject *CraniumBolts02;
		CObject *CraniumBolts03;
		CObject *OpenDoor;
		CObject *DoorPuzzleDark;
		CObject *DoorPuzzleLight;
		CObject *AfterPuzzle;
		CObject *BeforePuzzle;
		CObject *WaterFall;
		CObject *Plant01;
		CObject *Plant02;
		CObject *Plant03;
		CObject *Plant04;
		CObject *MagicGlitters02;
		CObject *MagicGlitters01;
		CObject *MagicPlant02;
		CObject *MagicPlant01;
		CObject *ScaryFace;

		SuperFX *CaveGlowPlant1FX, *CaveGlowPlant2FX, *CaveGlowPlant3FX, *CaveGlowPlant4FX, *CaveGlowPlant5FX, *CaveGlowPlant6FX;
		SuperFX *WaterFallFX, *WaterFallBottomFX;

		//--------------------------- Functions -------------------------------//
		L4_S3_Cave();
		~L4_S3_Cave();

		void InitPopups();
		void OnPopupClosed(Popup * popup);
		
		float easing(float t,float b,float c,float d);
		
		void TakeInventoryItem(EHOInventory ItemID);
		//Inherit this CRenderListEntity function to render anything to scene!!
		void Render(const CPoint* pCam);
		void Update();
		void Required();

		void NavigateToScene();
	protected:

		bool IsDoorOpenAnimPlayed;

		void StopParticleEmission();
		void OnAnimationComplete(int index);
		void TaskPendingUpdate();
		void InitSceneElements();
		void initAnimations();
		void ReceiveMessage(int val);
		void ProcessNonHOHint();
		void SparkleUpdate();
		void CheckForPendingTask();
};

#endif