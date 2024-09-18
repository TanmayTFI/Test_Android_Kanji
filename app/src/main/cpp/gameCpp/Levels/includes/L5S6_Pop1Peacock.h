#ifndef DQFYLH_LEVEL5_SCENE6_POP1_PEACOCK_DEF
#define DQFYLH_LEVEL5_SCENE6_POP1_PEACOCK_DEF

#include "Popup.h"

class L5S6_Pop1Peacock: public Popup
{
	public:
		L5S6_Pop1Peacock(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L5S6_Pop1Peacock();

		SuperFX *S6P1LocketprtFX;
		void Update(float ds);
		void Required();
		JSONAnimator  *JAnim_Locket;
		void OnAnimationComplete(int animID);
		
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		bool isTaskLeft();

	protected:

		void ReceiveMessage(int val);
		void ProcessHint();

};

#endif