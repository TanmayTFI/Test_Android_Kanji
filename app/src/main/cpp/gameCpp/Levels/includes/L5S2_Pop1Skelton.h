#ifndef DQFYLH_LEVEL5_SCENE2_POP1_SKELTON_DEF
#define DQFYLH_LEVEL5_SCENE2_POP1_SKELTON_DEF

#include "Popup.h"

class L5S2_Pop1Skelton: public Popup
{
	public:
		L5S2_Pop1Skelton(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L5S2_Pop1Skelton();

		void TakeInventoryItem(EHOInventory ItemID);
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