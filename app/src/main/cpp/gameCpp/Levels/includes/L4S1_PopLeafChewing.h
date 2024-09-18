#ifndef DQFYLH_LEVEL4_SCENE1_POP_LEAF_CHEW_DEF
#define DQFYLH_LEVEL4_SCENE1_POP_LEAF_CHEW_DEF

#include "Popup.h"
#include "JSONAnimator.h"

class L4S1_PopLeafChewing: public Popup
{
	public:
		JSONAnimator *Janim_chew;
		bool isAnimPlaying;
		
		L4S1_PopLeafChewing(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);

		~L4S1_PopLeafChewing();

		void TakeInventoryItem(EHOInventory ItemID);
		void Update(float ds);
		void Required();

		float easing(float t,float b,float c,float d);
		void OnChewComplete(int animID);

	protected:

		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif