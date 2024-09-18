#ifndef DQFYLH_LEVEL2_SCENE3_POP1_TURMERIC_DEF
#define DQFYLH_LEVEL2_SCENE3_POP1_TURMERIC_DEF

#include "Popup.h"
#include "JSONAnimator.h"

class L2S3_Pop1Turmeric: public Popup
{
	public:
		CPolygon *polyTermericPluck;

		//scene objects

		CObject *RakeR, *RakeL, *SoilL, *SoilR, *plantOnGround, *turmericPlant, *Frog;
		JSONAnimator *Janim_rakeR, *Janim_rakeL, *Janim_plantPluck;

		L2S3_Pop1Turmeric(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L2S3_Pop1Turmeric();

		void TakeInventoryItem(EHOInventory ItemID);
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		void Update(float ds);
		void Required();
		void OnRakeRightAnimationComplete(int animID);
		void OnRakeLeftAnimationComplete(int animID);

		protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif