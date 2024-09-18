#ifndef DQFYLH_LEVEL4_SCENE5_POP4_BRIDGE_DEF
#define DQFYLH_LEVEL4_SCENE5_POP4_BRIDGE_DEF

#include "Popup.h"
#include "JSONAnimator.h"

class L4S5_Pop4NetSquare: public Popup
{
	JSONAnimator *Janim_cutNet;
	public:

		L4S5_Pop4NetSquare(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L4S5_Pop4NetSquare();

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();

		float easing(float t,float b,float c,float d);

		void OnAnimComplete(int animID);
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
	protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif