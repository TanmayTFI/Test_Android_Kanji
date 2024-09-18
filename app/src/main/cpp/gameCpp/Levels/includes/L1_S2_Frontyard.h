//====================================
//DQFYLH
// Hidden Object System
//====================================

#ifndef DQFYLH_LEVEL1_SCENE2_FRONTYARD_DEF
#define DQFYLH_LEVEL1_SCENE2_FRONTYARD_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "CustomAnimGroup.h"

#include "FlyingInsects.h"

class CHOScene;

class L1_S2_Frontyard: public HiddenObjectSystem, CRenderListEntity
{
	public:
		FlyingInsects *flyingInsects;

		//Scene Elements		
		CObject *RT_BgScn;

		CObject *scn_cloudBig, *scn_cloudSmall;

		float 	bgScaleOffsetX;
		float	bgScaleOffsetY;

		int	zOrderScnVideo, zOrderPopVideo;
		
		CObject *BaboonNormalAnim, *BaboonAttackAnim, *BaboonHungryAnim;
		bool isBaboonAttackAnimPlaying, isBaboonHungryAnimPlaying;

		float BaboonAnimTimer;
		float BaboonAnimThresholdMultiplier;

		SuperFX *RainBgVFX, *RainDropsVFX;
		SuperFX *P3RainVFX, *P3RaindropsVFX;

		CustomAnimGroup *CAnim_CarDickieOpen;
		CustomAnimator *CAnim_BananaThrow;

		//Videos
		KVideo *lpVid_fire;
		
		bool isVidFireLoaded;

		bool IsMansionLit;
		bool isCarDickeyAnimPlaying;

		float WolfHowlTimer, WolfHowlTimeThreshold;

		float FuseSparkTimer, FuseSparkThreshold;
		CObject *Scn_Sparks, *Scn_FuseBgGlow, *Scn_Lightning;

		float lightningTimer, nextLightningThreshold, lightFrame;

		//Popups
		int ActivePopupID;

		CObject *Pop_CloseButton;

		CHOScene *Popup1CarDickie;
		CHOScene *Popup2Love;
		CHOScene *Popup3Swing;

		CustomAnimGroup		*CAnim_RopeCut, *CAnim_TreeBranchFix;

		bool isRopeCutPlaying, isTreeBranchFixAnimPlaying;
		
		CObject *plant1, *plant2, *plant3, *plant4, *plant5, *plant6, *plant7;
		CObject *branch1, *branch2, *branch4;
		CObject *carDickyOpened, *DieselCan, *hose, *sack, *wood, *openedCarDickyDoor, *walkieTalkie, *rope, *bottle, *shoe;
		CObject *tree;

		CObject *clothLoveSign, *clothCarDicky;

		CObject *BananaThrowAnim;
		bool isBananaThrowAnimPlaying;

		CObject *BaboonRunAnim[2];
		int baboonRunPointer;
		bool isBaboonRunAnimPlaying;
		
		int Popup1_State;
		int Popup2_State;
		int Popup3_State;

		//Popup Elements
		CObject *p1DieselCan, *p1Hose, *p1ClosedDickie, *p1OpenedDickie, *p1Sack, *p1SpiderNet1, *p1SpiderNet2, *p1SpiderNet3, *p1Wood2, *p1Crowbar;
		CPolygon *p1PolyWood;
		CPolygon *p1PolyDieselCan;


		bool IsPopupTransitionActive;
		float PopupAlpha, PopVal;
		float popupCloseDelayTimer;
		void SetupPopupTransition(CHOScene *Popup, CRectangle popRect);
		void UpdatePopupTransition();	

		void SparkleUpdate();

		L1_S2_Frontyard();
		~L1_S2_Frontyard();

		//Inherit this CRenderListEntity function to render anything to scene!! 
		void Render(const CPoint* pCam);
		
		void InitPopups();
		void UpdatePopups(CPoint pos,bool ItemDropped=false);
		void ClosePopup();

		float easingQuadOut(float t,float b,float c,float d);

		void TakeInventoryItem(EHOInventory ItemID);

		void Update();
		void Required();

		void UpdateBgGraphic();
		void EndBgGraphicScaling();		

		void NavigateToScene();

	protected:

		void ReceiveMessage(int val);
		void TaskPendingUpdate();
		void ProcessNonHOHint();
};

#endif
