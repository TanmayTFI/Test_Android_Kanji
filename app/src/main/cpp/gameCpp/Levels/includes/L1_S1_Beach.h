//====================================
// DQFYLH
// Hidden Object System
// Level 1 - Scene 01 : Beach
//====================================

#ifndef DQFYLH_LEVEL1_SCENE1_BEACH_DEF
#define DQFYLH_LEVEL1_SCENE1_BEACH_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Tutorial.h"
#include "MeshObject.h"
#include "SuperFX.h"
#include "CustomAnimator.h"
#include "CustomAnimGroup.h"
#include "DialogSystem.h"

#include "CVideo.h"


#include "VideoDefs.h" 
#include "KVideo.h"

class CHOScene;

class L1_S1_Beach: public HiddenObjectSystem, CRenderListEntity
{
	public:
		DialogSystem *dialogSystem;
		bool isDialogActive;

		//Scene Elements
		CObject *Scn_lightning, *memObj_Teddy;
		
		CObject *Scn_TopLeaves;
		CObject *Wave1, *Wave2;
		float waveTimer;

		CHOScene *RajaJamesSCN;		
		CustomAnimGroup *CAnim_RajaTakeJames;
		bool isRajaTakeJamesPlaying;

		SuperFX *RainBgVFX, *RainDropsVFX, *RainSandDropsVFX, *MoonFliesVFX;
		std::vector<SuperFX*>	VFXArray;

		float lightningTimer, nextLightningThreshold, lightFrame;

		MeshObject *FgPlantMesh;

		bool IsZOrderChangeToScene;
		int zOrderScnVideo;
		int zOrderPopVideo;

		// -- Popups -- //
		int ActivePopupID;
		
		CObject *Pop_CloseButton;
		
		CHOScene *Pop1Roses;
		CHOScene *Pop2CreekBoard;

		SuperFX *P1RainVFX, *P1RaindropsVFX;

		bool IsAllRubyPiecesTaken;

		CObject *P1_SwissKnife, *P1_Branch1, *P1_Branch2, *P1_Branch3;
		CObject *P1_Ruby1branch2, *P1_Ruby2Branch3, *P1_Ruby3UnderLeaf1, *P1_Ruby4UnderLeaves;
		CObject *P1_Leaf1, *P1_Leaf2, *P1_Leaf3;

		CustomAnimator *CAnim_KnifeCut1, *CAnim_KnifeCut2, *CAnim_KnifeCut3;
		CustomAnimator *CAnim_BranchCut1, *CAnim_BranchCut2, *CAnim_BranchCut3;

		std::vector<CustomAnimator*>	custAnimArray;

		bool IsKnifeCut1Playing, IsKnifeCut2Playing, IsKnifeCut3Playing;

		int Popup1_State;
		int Popup2_State;

		CObject *P2_Lightning;
		float p2lightTimer, p2lightFrame;

		float popupCloseDelayTimer;
		// -- Popups -- //

		//Scene Scaling Render Target
		CObject *RT_BgScn;
		float 	bgScaleOffsetX;
		float	bgScaleOffsetY;

		void TaskPendingUpdate();

		L1_S1_Beach();
		~L1_S1_Beach();

		//Inherit this CRenderListEntity function to render anything to scene
		void Render(const CPoint* pCam);
		void Update();
		void Required();
		void SparkleUpdate();

		void InitPopups();
		void UpdatePopups(CPoint pos,bool ItemDropped=false);
		void CloseActivePopup(bool isDelayed=false);
		
		bool IsPopupTransitionActive;
		float PopupAlpha, PopVal;
		void SetupPopupTransition(CHOScene *Popup, CRectangle popRect);
		void UpdatePopupTransition();

		void TakeInventoryItem(EHOInventory ItemID);

		float easingQuadOut(float t,float b,float c,float d);

		void UpdateBgGraphic();
		void EndBgGraphicScaling();

		void NavigateToScene();

	protected:

		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif