#ifndef DQFYLH_LEVEL5_SCENE6_POP2_MOON_DEF
#define DQFYLH_LEVEL5_SCENE6_POP2_MOON_DEF

#include "Popup.h"

class L5S6_Pop2Moon: public Popup
{
	public:
		L5S6_Pop2Moon(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L5S6_Pop2Moon();

		SuperFX *S6P2LocketFX;
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