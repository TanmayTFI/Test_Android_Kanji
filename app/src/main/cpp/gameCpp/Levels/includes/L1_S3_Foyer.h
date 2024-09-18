//====================================
//DQFYLH
// Hidden Object System
//	Level 1 - Scene 03 : Frontdoor
//====================================

#ifndef DQFYLH_LEVEL1_SCENE3_FRONTDOOR_DEF
#define DQFYLH_LEVEL1_SCENE3_FRONTDOOR_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "MeshObject.h"
#include "CustomAnimGroup.h"
#include "DialogSystem.h"

#include "FlyingInsects.h"

#include "VideoDefs.h"
#include "KVideo.h"

class CHOScene;

class L1_S3_Foyer: public HiddenObjectSystem, CRenderListEntity
{
	public:
		DialogSystem *dialogSystem;
		bool isDialogActive;

		FlyingInsects *flyingInsects;

		//Scene Elements
		CObject *Scn_Bg;
		CObject *Scn_lightning;		
		
		float lightningTimer, nextLightningThreshold, lightFrame;

		float   popupCloseDelayTimer;

		CVideo *cv_Cutscn; float vidTimer;


		//Custom Animations
		CustomAnimGroup *CAnim_RajaTakeJames;
		bool isRajaTakeJamesPlaying;

		MeshObject *FgCurtainMesh;

		int ActivePopupID;

		//Popups
		CObject *Pop_CloseButton;

		//Popup Elements		
		CHOScene *Popup1Shelf, *Popup2Table, *Popup3Photos;

		float clickDelayTimer;

		int Popup1_State;
		int Popup2_State;
		int Popup3_State;

		//Popup Elements
		CObject *P1_MovableObj1, *P1_MovableObj2;

		CustomAnimGroup *CAnim_BranchHandTake;
		bool isBranchPokeAnimPlaying;
		
		bool IsObj1MovePlaying,IsObj2MovePlaying, IsCatAnimPlaying,IsCatScaling;
		float Scalefactor;
		float catVelY;

		int zOrderScnVideo;
		int zOrderPopVideo;

		std::vector<SuperFX*>	VFXArray;
		SuperFX *MoonLightVFX, *LightVFX;
		SuperFX *HOLinesVFX, *HODotsVFX;

		L1_S3_Foyer();
		~L1_S3_Foyer();

		//Inherit this CRenderListEntity function to render anything to scene!! 
		void   Render(const CPoint* pCam);
		
		void InitPopups();
		void UpdatePopups(CPoint pos,bool ItemDropped=false);
		
		bool IsPopupTransitionActive;
		float PopupAlpha,PopVal;
		void SetupPopupTransition(CHOScene *Popup, CRectangle popRect);
		void UpdatePopupTransition();

		void SparkleUpdate();

		//Scene Scaling Render Target
		CObject *RT_BgScn;
		float 	bgScaleOffsetX;
		float	bgScaleOffsetY;
		
		float easingQuadOut(float t,float b,float c,float d);
		void TakeInventoryItem(EHOInventory ItemID);
		void Update();
		void Required();
		void EndBgGraphicScaling();
		void UpdateBgGraphic();
		void NavigateToScene();

	protected:

		void ReceiveMessage(int val);
		void TaskPendingUpdate();
		void ProcessNonHOHint();
};

#endif