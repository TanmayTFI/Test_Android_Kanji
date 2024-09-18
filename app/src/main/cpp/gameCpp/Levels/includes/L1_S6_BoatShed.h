//====================================
// DQFYLH
// Hidden Object System
// Level 1 - Scene 08 : BoatShed
//====================================

#ifndef DQFYLH_LEVEL1_SCENE8_BOATSHED_DEF
#define DQFYLH_LEVEL1_SCENE8_BOATSHED_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "MeshObject.h"
#include "CustomAnimator.h"
#include "BuildConstants.h"
#include "VideoDefs.h" 
#include "KVideo.h"

class CHOScene;

class L1_S6_Boatshed: public HiddenObjectSystem, CRenderListEntity
{
	public:

		//Scene Elements
		CObject *Scn_BGDark;

		CObject *BroomTaken;
		

		CObject *scn_dogbarking;
		int dogBarkSFXID;
		float dogWhineTimer, dogWhineThreshold;

		//Popups
		CObject *Pop_CloseButton;

		CHOScene *Popup2Switch, *Popup1Pumper;

		MeshObject *FgWebMesh, *FgWebBoxMesh;

		SuperFX *MoonLightVFX, *SparkVFX, *LightVFX;

		SuperFX *HOLinesVFX, *HODotsVFX;

		CustomAnimator *CAnim_Pumper, *CAnim_Handle, *CAnim_PumperPlaced, *CAnim_Box, *CAnim_BoxMoved, *CAnim_WebBox, *CAnim_Broom, *CAnim_Pumpershadow;

		bool IsDogBarking, IsbroomTaken, IsPumpingAnimPlaying, IsBroomAnimPlaying, IsBoxMovingAnimPlaying;

		int	zOrderScnVideo, zOrderPopVideo;
		bool IsZOrderChangeToScene;

		float PumpAnimTimer;
		bool isFlowStarted, isFillStarted;

		float lightFrameTimer;
		bool isLightFlickering;

		int ActivePopupID;

		bool IsPopupTransitionActive;
		float PopupAlpha,PopVal;
		void SetupPopupTransition(CHOScene *Popup, CRectangle popRect);
		void UpdatePopupTransition();

		//Scene Scaling Render Target
		CObject *RT_BgScn;

		float 	bgScaleOffsetX;
		float	bgScaleOffsetY;

		float easingQuadOut(float t,float b,float c,float d);

		void InitPopupRenderTarget(CHOScene *Popup);
		void TakeInventoryItem(EHOInventory ItemID);

		void EndBgGraphicScaling();
		void UpdateBgGraphic();

		float	popupCloseDelayTimer;

		int Popup1_State;
		int Popup2_State;
		int Popup3_State;

		CObject *PopCircCloseBtn;

		L1_S6_Boatshed();
		~L1_S6_Boatshed();

		float tweenTime, SlideTime;
		CPoint *startPos,*endPos;
		bool  isToolSliding;
		int slideToolId;

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