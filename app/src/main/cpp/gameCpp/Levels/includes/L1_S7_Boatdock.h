//====================================
// DQFYLH
// Hidden Object System
// Level 1 - Scene 07 : BoatDock
//====================================

#ifndef DQFYLH_LEVEL1_SCENE7_BOATDOCK_DEF
#define DQFYLH_LEVEL1_SCENE7_BOATDOCK_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "CustomAnimator.h"
#include "CustomAnimGroup.h"
#include "MeshObject.h"

#include "FlyingInsects.h"

#include "VideoDefs.h" 
#include "CustomAnimator.h"
#include "KVideo.h"

class CHOScene;

class L1_S7_Boatdock: public HiddenObjectSystem, CRenderListEntity
{
	public:		
		float 	Replay_WaitingForCollectablePickTicks;
		bool    IsReplay_WaitingForCollectablePick;
		//Scene Elements
		CustomAnimGroup *CAnim_PlankPlace;
		bool isPlankPlaceAnimPlaying;

		FlyingInsects *flyingInsects;

		//Scene Scaling Render Target
		CObject *RT_BgScn;
		float 	bgScaleOffsetX;
		float	bgScaleOffsetY;

		void UpdateBgGraphic();
		void EndBgGraphicScaling();

		int zOrderScnVideo;
		int zOrderPopVideo;
		float	popupCloseDelayTimer;
		
		bool IsPopupTransitionActive;
		float PopupAlpha, PopVal;
		void SetupPopupTransition(CHOScene *Popup, CRectangle popRect);
		void UpdatePopupTransition();

		float easingQuadOut(float t,float b,float c,float d);

		int ActivePopupID;

		CustomAnimator *custAnim_DieselFill;

		CObject	*boat;
		CObject	*boatShadow, *boatWaveSide, *boatWaveback;
		CObject	*rope;
				
		CObject	*tree1;
		CObject	*tree2;
		CObject *tree3;
		CObject	*plant1;
		CObject	*plant2;
		CObject *plant3;
		MeshObject *seaMesh, *p1Water;

		std::vector<SuperFX*>	VFXArray;
		SuperFX  *LightVFX;

		SuperFX *RainBgVFX, *RainDropsVFX, *L1S7_P1RainVFX, *L1S7_P1RaindropsVFX;
		SuperFX *RainFallLeftVFX, *RainFallLampVFX, *MapleLeafVFX;
		float	boatAngle;

		//Popups
		CObject *Pop_CloseButton;

		CHOScene *Popup1DieselTank;

		CObject *p1CuttedRope, *p1SwissKnife;
		CObject *p1Funnel;
		CObject *p1FunnelFixed;
		CObject *p1Rope;
		CObject *p1TankCap;
		CObject *p1TankCapOpend;
		//CObject *p1Water;
		//CObject *p1DieselCan;
		CObject *p1TankOpenAnim, *p1TankCloseAnim, *p1DieselFillAnim;
		bool isTankOpenAnimPlaying, isTankCloseAnimPlaying;

		CustomAnimGroup *CAnim_RopeKnifeCut;
		bool isRopeKnifeCutAnimPlaying;

		bool isDieselFillAnimPlaying;

		void UpdateDieselTankPopupItems();
		void UpdateBoatKeyPopupItems();

		CHOScene *Popup2BoatKeyStart;
		CObject *p2KeyFixed;
		CObject *p2KeyTurned;
		CObject *p2KeyTurnAnim, *p2NeedleAnim, *p2FuelRedGlow;
		bool isKeyTurnAnimPlaying;
		float p2KeyTurnAnimTimer;

		float tempTimer;

		int Popup1_State;
		int Popup2_State;

		//Popup Elements
		CObject *Popup1_Bg;
	
		L1_S7_Boatdock();
		~L1_S7_Boatdock();

		//Inherit this CRenderListEntity function to render anything to scene!! 
		void   Render(const CPoint* pCam);
		
		void InitPopups();
		void UpdatePopups(CPoint pos,bool ItemDropped=false);
		void Update();
		void Required();
		void SparkleUpdate();

		void NavigateToScene();
	protected:


		void ReceiveMessage(int val);
		void TaskPendingUpdate();
		void ProcessNonHOHint();
};

#endif