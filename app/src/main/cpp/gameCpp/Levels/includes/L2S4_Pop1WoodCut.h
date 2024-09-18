#ifndef DQFYLH_L2_S4_POP1_WOODCUT_DEF
#define DQFYLH_L2_S4_POP1_WOODCUT_DEF

#include "Popup.h"
#include "SuperLog.h"

#include "JSONAnimator.h"

class L2S4_Pop1WoodCut: public Popup
{
	public:

		JSONAnimator *JAnim_WoodCut1;
		JSONAnimator *JAnim_WoodCut2;

		L2S4_Pop1WoodCut(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L2S4_Pop1WoodCut();	


		void HandleInventoryDrop(CPoint dropPos, int itemVal);

		void OnAnimationComplete(int animID);

		void Update(float ds );
		void Required();

		void ProcessHint();
		bool isTaskLeft();
	protected:
};

#endif