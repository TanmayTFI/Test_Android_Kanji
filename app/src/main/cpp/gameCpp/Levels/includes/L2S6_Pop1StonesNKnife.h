#ifndef DQFYLH_LEVEL2_SCENE6_POP1_STONESANDKNIFE_DEF
#define DQFYLH_LEVEL2_SCENE6_POP1_STONESANDKNIFE_DEF

#include "Popup.h"
#include "JSONAnimator.h"

class L2S6_Pop1StonesNKnife: public Popup
{
	public:

		//scene objects

		CObject *rock[3], *smallStone[3];

		JSONAnimator *JANIM_stoneAndCrowbar;

		L2S6_Pop1StonesNKnife(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L2S6_Pop1StonesNKnife();

		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		void Update(float ds);
		void Required();
		void OnAnimationComplete(int id);

		float easing(float t,float b,float c,float d);

		protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif