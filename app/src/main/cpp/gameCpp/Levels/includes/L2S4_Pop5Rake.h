#ifndef DQFYLH_L2_S4_POP5_RAKE_DEF
#define DQFYLH_L2_S4_POP5_RAKE_DEF

#include "Popup.h"
#include "SuperLog.h"

#include "JSONAnimator.h"

class L2S4_Pop5Rake: public Popup
{
	public:

		L2S4_Pop5Rake(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L2S4_Pop5Rake();

		void HandleInventoryDrop(CPoint dropPos, int itemVal);

		void OnAnimationComplete(int animID);

		void Update(float ds);
		void Required();

		void ProcessHint();
		bool isTaskLeft();

	protected:
};

#endif