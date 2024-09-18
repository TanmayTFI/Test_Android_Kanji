//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL6 - SCENE4 : FLOATING ISLAND_4
//====================================

#ifndef DQFYLH_LEVEL6_SCENE4_FLOATING_ISLAND_4_DEF
#define DQFYLH_LEVEL6_SCENE4_FLOATING_ISLAND_4_DEF

#include "HiddenObjectSystem.h"
#include "ClickablePolygon.h"
#include "L6S4_Pop1PineTree.h"
#include "L6S4_Pop2HotSpring.h"
#include "L6S4_Pop3MagnetizingRocks.h"
#include "SuperFX.h"

class L6_S4_FloatingIsland_4: public HiddenObjectSystem, CRenderListEntity
{
	public:

		//popups
		std::vector<ClickablePolygon*> PopRectArray;
		std::vector<Popup*> PopupArray;

		int ActivePopUpId;
		SuperFX *SnowFX_L;
		SuperFX *s4_bubblesFX;
		SuperFX *s4_PondSmokeFX;

		//--------------------------- Functions -------------------------------//
		L6_S4_FloatingIsland_4();
		~L6_S4_FloatingIsland_4();
		float musicFadeOutTimer;
		L6S4_Pop1PineTree *pop1;

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
		void NavigateToScene();
	protected:
		SuperFX *S4_StarsFX;
		void ReceiveMessage(int val);
		void ProcessNonHOHint();
		void SparkleUpdate();
		void CheckForPendingTask();
};

#endif