#ifndef DQFYLH_LEVEL3_SCENE2_POP2_SKULL_DEF
#define DQFYLH_LEVEL3_SCENE2_POP2_SKULL_DEF

#include "Popup.h"

class L3S2_Pop2Skull: public Popup
{
	public:
		L3S2_Pop2Skull(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L3S2_Pop2Skull();

		void Update(float ds);
		void Required();
		JSONAnimator  *JAnim_Skull;
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