#ifndef DQFYLH_LEVEL3_SCENE5_POP3_ROPE_DEF
#define DQFYLH_LEVEL3_SCENE5_POP3_ROPE_DEF

#include "Popup.h"

class L3S5_Pop3Rope: public Popup
{
	public:
		L3S5_Pop3Rope(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L3S5_Pop3Rope();

		
		void Update(float ds);
		void Required();
		JSONAnimator  *JAnim_Rope;
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