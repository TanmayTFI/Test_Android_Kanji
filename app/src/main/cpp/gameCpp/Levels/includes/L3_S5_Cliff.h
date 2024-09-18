//====================================
// DQFULP
// Hidden Object System
// Author : Amal
// LEVEL3 - SCENE3 : Cliff
//====================================

#ifndef DQFYLH_LEVEL3_SCENE3_CLIFF_DEF
#define DQFYLH_LEVEL3_SCENE3_CLIFF_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "MeshObject.h"
#include "JSONAnimator.h"
#include "VideoDefs.h" 
#include "L3S5_Pop1Grass.h"
#include "L3S5_Pop2Deer.h"
#include "L3S5_Pop3Rope.h"
#include "L3S5_Pop4Clip.h"
#include "L3S5_Pop5Bucket.h"
#include "KVideo.h"

class CHOScene;

enum eL3S5PopEnums
{
	ePop1_Grass,
	ePop2_Deer,
	ePop3_Rope,
	ePop4_Clip,
	ePop5_Bucket,
	ePopL3S5_Max
};

class L3_S5_Cliff: public HiddenObjectSystem
{
	public:
		float ExploreAreaUnlockTimer;
		SuperFX *S5crystalprt1FX, *S5crystalprt2FX, *S5crystalprt3FX, *S5crystalprt4FX, *S5crystalprt5FX, *S5crystalprt6FX, *S5FogPrtFX;
		MeshObject *meshCloth;
		JSONAnimator  *JAnim_DeerLoop,  *JAnim_DeerFadeOut;
		float musicFadeOutTimer;
		bool isMaryFallCutscnPlaying;
		CVideo *cv_MaryFallCutscn;
		float vidTimer;

		Popup*		PopupArray[ePopL3S5_Max];
		int			PopStates[ePopL3S5_Max];
		CPolygon*	PopPolyArray[ePopL3S5_Max];
		L3S5_Pop5Bucket*  PopupBucket;
		DialogSystem *dialogSystem;
		void onPopupClosed(Popup * popup);

		//Scene Elements
		CObject *Scn_Bg;
		CObject *Scn_Obj1, *Scn_Obj2, *Scn_Obj3;
		
		//--------------------------- VIDEOS -------------------------------//
		//SKIP_VIDEO
		bool	IsVidSkippable, IsSkipMouseOver, IsSkipMouseClicked;
		float	SkipShowTimer;
		KGraphic *SkipImage;
		
		bool IsZOrderChangeToScene;
		int  zOrderScnVideo;
		int  zOrderPopVideo;

		bool IsVideoFadingIn;
		bool IsVideoFadingOut;

		float videoFadeInAlpha;
		float videoFadeOutAlpha;
		
		KVideo *lpVid1BgLoop;
		
		KVideo *lpVid2BgCliffCutscn;
		KSound *lpSnd2BgCliffCutscn;

		KVideo *lpPop1Vid1GrassCutting;
		KVideo *lpPop2Vid1DeerLoop, *lpPop2Vid2GrassToDeer;
		KVideo *lpPop3Vid1NailHammer, *lpPop3Vid2RopeFix;
		
		bool IsVid1BgLoopLoaded;
			
		bool IsVid2BgCliffCutscnLoaded;
		bool IsVid2BgCliffCutscnPlaying;

		bool IsPop1Vid1GrassCuttingLoaded;
		bool IsPop1Vid1GrassCuttingPlaying;
		
		bool IsPop2Vid1DeerLoopLoaded, IsPop2Vid2GrassToDeerLoaded;
		bool IsPop2Vid1DeerLoopPlaying, IsPop2Vid2GrassToDeerPlaying;
		
		bool IsPop3Vid1NailHammerLoaded, IsPop3Vid2RopeFixLoaded;
		bool IsPop3Vid1NailHammerPlaying, IsPop3Vid2RopeFixPlaying;
		
		//--------------------------- POPUPS -------------------------------//
		CObject *PopRectCloseBtn;

		int ActivePopupID;
		
		float popupCloseDelayTimer;
		
		//Popups		
		CHOScene *Popup1Grass;
		CHOScene *Popup2Deer;
		CHOScene *Popup3CliffSide;
		CHOScene *Popup4Rope;		

		int Popup1_State;
		int Popup2_State;
		int Popup3_State;
		int Popup4_State;

		//Popup Elements
		CObject *Pop1_obj1,*Pop1_obj2,*Pop1_obj3,*Pop1_obj4;
		CObject *Pop2_obj1,*Pop2_obj2,*Pop2_obj3,*Pop2_obj4;
		CObject *Pop3_obj1,*Pop3_obj2,*Pop3_obj3,*Pop3_obj4;
		CObject *Pop4_obj1,*Pop4_obj2,*Pop4_obj3,*Pop4_obj4;
		
		
		//--------------------------- Functions -------------------------------//
		L3_S5_Cliff();
		~L3_S5_Cliff();

		void InitPopups();
			
		float easing(float t,float b,float c,float d);

		void Update();
		void Required();
		void SparkleUpdate();
		void TaskPendingUpdate();
		void SceneObjectsUpdate();
		void OnAnimationComplete(int AnimID);
		
		void NavigateToScene();
	protected:

		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif