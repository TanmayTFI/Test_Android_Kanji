#ifndef DQFYLH_L2_S4_POP4_STONEBRICK_DEF
#define DQFYLH_L2_S4_POP4_STONEBRICK_DEF

#include "Popup.h"
#include "SuperLog.h"

#include "JSONAnimator.h"

class L2S4_Pop4StoneBrick: public Popup
{
	public:
		JSONAnimator *JAnim_StoneMove1, *JAnim_StoneMove2;

		L2S4_Pop4StoneBrick(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L2S4_Pop4StoneBrick();	

		void HandleInventoryDrop(CPoint dropPos, int itemVal);

		void OnAnimationComplete(int animID);

		void Update(float ds );
		void Required();

		void ProcessHint();
		bool isTaskLeft();

	protected:
};

#endif