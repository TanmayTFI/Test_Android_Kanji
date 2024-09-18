#ifndef DQFYLH_LEVEL3_SCENE5_POP2_DEER_DEF
#define DQFYLH_LEVEL3_SCENE5_POP2_DEER_DEF

#include "Popup.h"

class L3S5_Pop2Deer: public Popup
{
	public:
		L3S5_Pop2Deer(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L3S5_Pop2Deer();

		JSONAnimator  *JAnim_DeerLoop, *JAnim_DeerRun, *JAnim_DeerAngry;
		
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