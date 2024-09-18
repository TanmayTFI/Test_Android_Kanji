#ifndef DQFYLH_LEVEL2_SCENE1_POP1_HONEYCOMB_DEF
#define DQFYLH_LEVEL2_SCENE1_POP1_HONEYCOMB_DEF

#include "Popup.h"
#include "SuperFX.h"

#define NUM_OF_BEES	9
#define NUM_OF_LEAVES 8

class L2S1_Pop1HoneyComb: public Popup
{
	public:
		int showfrankinscene;

		JSONAnimator *JAnim_HoneyCombCut;

		//scene objects
		CObject *frankinscence;
		CObject *KnifeL;
		CObject *KnifeR;
		CObject *Leaf[NUM_OF_LEAVES];
		CObject *HoneyBee[NUM_OF_BEES];
		CObject *HoneycombCut;
		CObject *HoneycombMask;
		CObject *HoneybeeSetL;
		CObject *HoneybeeSetR;

		HudControl *Hud;
		SuperFX *smokeVFX;

		L2S1_Pop1HoneyComb(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L2S1_Pop1HoneyComb();

		void HandleInventoryDrop(CPoint dropPos, int itemVal);

		void Update(float ds);
		void Required();

		void OnAnimationComplete(int animID);

		protected:

		void ProcessHint();
		bool isTaskLeft();
};

#endif