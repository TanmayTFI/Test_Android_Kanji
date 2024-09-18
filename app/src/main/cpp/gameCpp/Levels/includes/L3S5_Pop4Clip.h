#ifndef DQFYLH_LEVEL3_SCENE5_POP4_CLIP_DEF
#define DQFYLH_LEVEL3_SCENE5_POP4_CLIP_DEF

#include "Popup.h"

class L3S5_Pop4Clip: public Popup
{
	public:
		L3S5_Pop4Clip(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L3S5_Pop4Clip();

		
		void Update(float ds);
		void Required();
		JSONAnimator  *JAnim_Clip,*JAnim_Knot;
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