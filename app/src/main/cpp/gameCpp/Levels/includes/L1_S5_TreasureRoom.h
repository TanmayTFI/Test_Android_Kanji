//====================================
// DQFYLH
// Hidden Object System
// Level 1 - Scene 05 : Treasure Room
//====================================

#ifndef DQFYLH_LEVEL1_SCENE5_TREASUREROOM_DEF
#define DQFYLH_LEVEL1_SCENE5_TREASUREROOM_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "SuperFX.h"

#ifdef _PARTICLEEDITORENABLED
	#include "ParticleEditor.h"
#endif

#include "VideoDefs.h" 
#include "KVideo.h"
#include "CVideo.h"


class CHOScene;

class L1_S5_TreasureRoom: public HiddenObjectSystem, CRenderListEntity
{
	public:

		//Scene Elements
		SuperFX *DarkLightVFX, *GoldLeftVFX, *GoldRightVFX;

		SuperFX *MoonFliesVFX;
		std::vector<SuperFX*>	VFXArray;
		
		// -- Popups -- //
		int ActivePopupID;

		CObject *Pop_CloseButton;
		
		CHOScene *Pop1WallSide;
		CHOScene *Pop2FireLamp;

		CObject *p1HammerHead;
		CObject *p1Wood;
		CObject *p1FireImage;
		CObject *p1Spoon;

		CObject *p1WoodDropAnim;
		bool isWoodDropAnimPlaying;

		CObject *p2key;
		CObject *p2TorchLightAnim;
		bool isTorchLightAnimPlaying;

		CObject *p2MatchBoxAnim;
		bool isMatchBoxAnimPlaying;

		int Popup2_State;

		float popupCloseDelayTimer;
		// -- Popups -- //

		//Scene Scaling Render Target
		CObject *RT_BgScn;
		float 	bgScaleOffsetX;
		float	bgScaleOffsetY;

		L1_S5_TreasureRoom();
		~L1_S5_TreasureRoom();

		CObject *amoniBottle;
		CObject *amoniBottleCork;
		CObject *bottle;
		CObject *closedBoxDoor;
		CObject *crystalInGrinder1;
		CObject *crystalInGrinder3;
		CObject *greenPowderOnPlate;
		CObject *bluePowderOnPlate;
		CObject *map1;
		CObject *map2;
		CObject *openedBoxDoor;
		CObject *thread;
		CObject *threadOnBottle;
		CObject *threadWaxOnBottle;
		CObject *waxCylinder;
		CObject *dark;
		CObject *brockenDoor;
		CObject *woodDown;
		CObject *hammer;
		CObject *woodNhammer;
		CObject *key;
		CObject *Fire[2];
		CObject *Smoke;

		CPolygon *polyWood;

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

		float easing(float t,float b,float c,float d);
		float easingQuadOut(float t,float b,float c,float d);

		void UpdateBgGraphic();
		void EndBgGraphicScaling();

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
