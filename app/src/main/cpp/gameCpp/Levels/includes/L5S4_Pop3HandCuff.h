#ifndef DQFYLH_LEVEL5_SCENE4_POP3_HANDCUFF_DEF
#define DQFYLH_LEVEL5_SCENE4_POP3_HANDCUFF_DEF

#include "Popup.h"

class L5S4_Pop3HandCuff: public Popup
{
	public:
		L5S4_Pop3HandCuff(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L5S4_Pop3HandCuff();

		void TakeInventoryItem(EHOInventory ItemID);
		void Update(float ds);
		void Required();
		JSONAnimator  *JAnim_PickAxe;
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