#ifndef DQFYLH_LEVEL2_SCENE1_POP4_CLEAR_WATER_DEF
#define DQFYLH_LEVEL2_SCENE1_POP4_CLEAR_WATER_DEF

#include "Popup.h"
#include "MeshObject.h"

#define NUM_OF_POTS	5

class L2S1_Pop4ClearWater: public Popup
{
	public:
		bool IsCustomAnimPlaying; 

		//scene objects
		CObject *emptyPot, *filledPot;
		CObject *leavesSet1;
		CObject *leavesSet2;
		CObject *leavesSet3;

		MeshObject *pondWater;

		CustomAnimator *CAnim_EmptyPot, *CAnim_FilledPot, *CAnim_WaterMask;

		L2S1_Pop4ClearWater(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L2S1_Pop4ClearWater();

		void TakeInventoryItem(EHOInventory ItemID);
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		void Update(float ds);
		void Required();
		void OpenPopup();
		void ClosePopup(float delayTime);
		void OnScaleUpComplete();

		protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif