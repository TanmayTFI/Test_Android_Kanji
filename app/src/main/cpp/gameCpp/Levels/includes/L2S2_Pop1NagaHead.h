#ifndef DQFYLH_LEVEL2_SCENE2_POP1_NAGAHEAD_DEF
#define DQFYLH_LEVEL2_SCENE2_POP1_NAGAHEAD_DEF

#include "Popup.h"

class L2S2_Pop1NagaHead: public Popup
{
	public:
		L2S2_Pop1NagaHead(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L2S2_Pop1NagaHead();

		void TakeInventoryItem(EHOInventory ItemID);
		void Update(float ds);
		void Required();
		JSONAnimator  *JAnim_LampLite;
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