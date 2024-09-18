#ifndef DQFYLH_LEVEL2_SCENE2_POP2_FRANKINSCENCE_DEF
#define DQFYLH_LEVEL2_SCENE2_POP2_FRANKINSCENCE_DEF

#include "Popup.h"

class L2S2_Pop2Frankinscence: public Popup
{
	public:
		L2S2_Pop2Frankinscence(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L2S2_Pop2Frankinscence();

		void TakeInventoryItem(EHOInventory ItemID);
		void Update(float ds);
		void Required();

		CObject *popCloseBtn;
		float easing(float t,float b,float c,float d);
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		JSONAnimator  *JAnim_Frank;
		void OnAnimationComplete(int animID);
		void ProcessHint();
		bool isTaskLeft();

	protected:

		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif