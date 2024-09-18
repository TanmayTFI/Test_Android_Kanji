#ifndef DQFYLH_LEVEL7_SCENE4_POP2_FLOWER_DEF
#define DQFYLH_LEVEL7_SCENE4_POP2_FLOWER_DEF

#include "Popup.h"

class L7S4_Pop2Flower: public Popup
{
	public:
		L7S4_Pop2Flower(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L7S4_Pop2Flower();

		void TakeInventoryItem(EHOInventory ItemID);
		void Update(float ds);
		void Required();
		JSONAnimator  *JAnim_Star, *JAnim_Flower;
		SuperFX *S7P1GliterprtFX;
		void OnAnimationComplete(int animID);

		CObject *popCloseBtn;
		float easing(float t,float b,float c,float d);
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		bool isTaskLeft();

	protected:

		void ReceiveMessage(int val);
		void ProcessHint();

};

#endif