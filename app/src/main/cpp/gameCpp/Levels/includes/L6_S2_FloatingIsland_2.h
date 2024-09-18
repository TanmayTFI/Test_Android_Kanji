//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL6 - SCENE2 : FLOATING ISLAND_2
//====================================

#ifndef DQFYLH_LEVEL6_SCENE2_FLOATING_ISLAND_2_DEF
#define DQFYLH_LEVEL6_SCENE2_FLOATING_ISLAND_2_DEF

#include "HiddenObjectSystem.h"
#include "ClickablePolygon.h"
#include "L6S2_Pop1Ice.h"
#include "L6S2_Pop2Door.h"
#include "SuperFX.h"

class L6_S2_FloatingIsland_2: public HiddenObjectSystem, CRenderListEntity
{
	L6S2_Pop1Ice *pop1;

	public:
		float musicFadeOutTimer;
		//popups
		std::vector<ClickablePolygon*> PopRectArray;
		std::vector<Popup*> PopupArray;
		SuperFX *StatueRayFX;
		SuperFX *_magicPowderVFX;
		CObject * _powderBottle;
		CObject * _sphereGlow;

		int ActivePopUpId;
		SuperFX *PlantGlowFX_L, *PlantGlowFX_R, *SnowFX_L, *SnowFX_R, *DoorToS3RayFX;
		JSONAnimator *Janim_doorOpen;
		JSONAnimator *Janim_SphereReveal;
		//--------------------------- Functions -------------------------------//
		L6_S2_FloatingIsland_2();
		~L6_S2_FloatingIsland_2();

		void InitPopups();
		void OnPopupClosed(Popup * popup);

		bool isPopupActive(int popId);
		
		float easing(float t,float b,float c,float d);
		
		void TakeInventoryItem(EHOInventory ItemID);
		//Inherit this CRenderListEntity function to render anything to scene!!
		void Render(const CPoint* pCam);
		void Update();
		void Required();
		void SceneObjectsUpdate();
		void TaskPendingUpdate();
		void InitParticles();
		void OnSphereRevealAnimComplete(int id);
		void NavigateToScene();
	protected:
		SuperFX *S2_LeafFX;
		void ReceiveMessage(int val);
		void ProcessNonHOHint();
		void SparkleUpdate();
		void CheckForPendingTask();
};

#endif