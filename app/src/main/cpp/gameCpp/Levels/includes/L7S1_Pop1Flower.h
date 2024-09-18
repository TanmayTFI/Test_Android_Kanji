#ifndef DQFYLH_LEVEL7_SCENE1_POP1_FLOWER_DEF
#define DQFYLH_LEVEL7_SCENE1_POP1_FLOWER_DEF

#include "Popup.h"

class L7S1_Pop1Flower: public Popup
{
	public:
		L7S1_Pop1Flower(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L7S1_Pop1Flower();

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