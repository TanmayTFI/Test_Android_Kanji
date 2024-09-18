//====================================
//DQFYLH
// Hidden Object System
//====================================

#ifndef DQFYLH_LEVEL1_SCENE5_TUNNELDOOR_DEF
#define DQFYLH_LEVEL1_SCENE5_TUNNELDOOR_DEF

#include "HiddenObjectSystem.h"
#include "CVideo.h"
#include "GameFlow.h"

#ifdef _PARTICLEEDITORENABLED
	#include "ParticleEditor.h"
#endif

class CHOScene;

class L1_S5_TunnelDoor: public HiddenObjectSystem, CRenderListEntity
{
	public:

		CHOScene *WallBrokenScene;

		//Scene Elements
		int	zOrderScnVideo, zOrderPopVideo;

		bool	IsVidSkippable, IsSkipMouseOver, IsSkipMouseClicked;
		float	SkipShowTimer;
		CObject *skipObj;
	
		//Videos
		CVideo *cv_DoorExplosion, *cv_TunnelRunning;
		float vidTimer, vidTimer2;

		bool isDoorExplodeVideoPlaying, isTunnelRunVideoPlaying;

		//Popups
		int ActivePopupID;

		CObject *Pop_CloseButton;

		CHOScene *Popup2Bomb;

		SuperFX *DustVFX;
		bool isDustVFXPlaying;
		float dustVFXtimer;

		std::vector<CPoint> VFXPosArray;

		//Popup Elements
		CObject *p2Bomb, *p2Thread;
		CObject *p2BombLitAnim, *p2HammerHand;
		bool isBombLitAnimPlaying;

		CObject *bomb;
		
		int Popup1_State;
		
		bool IsPopupTransitionActive;
		float PopupAlpha, PopVal;
		void SetupPopupTransition(CHOScene *Popup, CRectangle popRect);
		void UpdatePopupTransition();		

		L1_S5_TunnelDoor();
		~L1_S5_TunnelDoor();

		//Inherit this CRenderListEntity function to render anything to scene!! 
		void Render(const CPoint* pCam);

		float easingQuadOut(float t,float b,float c,float d);
		
		void InitPopups();
		void UpdatePopups(CPoint pos,bool ItemDropped=false);
		void TakeInventoryItem(EHOInventory ItemID);
		void Update();
		void Required();
		void SparkleUpdate();

		void NavigateToScene();

	protected:

		#ifdef _PARTICLEEDITORENABLED
			bool IsParticleEditorEnabled;
			ParticleEditorControl *_particleEditor;
		#endif

		void ReceiveMessage(int val);

		void TaskPendingUpdate();
		void ProcessNonHOHint();
};

#endif