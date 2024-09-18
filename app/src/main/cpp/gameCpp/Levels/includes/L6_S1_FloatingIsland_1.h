//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL6 - SCENE1 : FLOATING ISLAND_1
//====================================

#ifndef DQFYLH_LEVEL6_SCENE1_FLOATING_ISLAND_1_DEF
#define DQFYLH_LEVEL6_SCENE1_FLOATING_ISLAND_1_DEF

#include "HiddenObjectSystem.h"
#include "ClickablePolygon.h"
#include "L6S1_Pop1Water.h"
#include "L6S1_Pop2StatueChest.h"
#include "L6S1_Pop3StatueHand.h"
#include "L6S1_Pop4StatueSandals.h"
#include "MeshObject.h"
class L6_S1_FloatingIsland_1: public HiddenObjectSystem, CRenderListEntity
{
	public:
		float musicFadeOutTimer;
		//popups
		std::vector<ClickablePolygon*> PopRectArray;
		std::vector<Popup*> PopupArray;
		CPolygon * ClubPolygon;
		SuperFX * StatueRayFX;
		L6S1_Pop1Water *pop1;
		L6S1_Pop4StatueSandals *pop4;
		int ActivePopUpId;
		MeshObject *mo_water;
		//--------------------------- Functions -------------------------------//
		L6_S1_FloatingIsland_1();
		~L6_S1_FloatingIsland_1();

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
		SuperFX *S1FireFlyFX, *S1UnderFogFX;// *S1SkyStarsFX;

		void ReceiveMessage(int val);
		void ProcessNonHOHint();
		void ProcessSkip();
		void SparkleUpdate();
		void CheckForPendingTask();
};

#endif