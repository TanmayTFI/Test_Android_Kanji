#ifndef DQFYLH_LEVEL5_SCENE2_POP3_GREASE_DEF
#define DQFYLH_LEVEL5_SCENE2_POP3_GREASE_DEF

#include "Popup.h"

class L5S2_Pop3Grease: public Popup
{
	public:
		L5S2_Pop3Grease(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L5S2_Pop3Grease();

		
		void Update(float ds);
		void Required();
		JSONAnimator  *JAnim_Grease;
		void OnAnimationComplete(int animID);
		
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		bool isTaskLeft();

	protected:

		void ReceiveMessage(int val);
		void ProcessHint();

};

#endif