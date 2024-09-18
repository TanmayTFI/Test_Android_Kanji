#ifndef DQFYLH_LEVEL6_SCENE4_POP1_PINETREE_DEF
#define DQFYLH_LEVEL6_SCENE4_POP1_PINETREE_DEF

#include "Popup.h"
#include "JSONAnimator.h"

class L6S4_Pop1PineTree: public Popup
{
	JSONAnimator *Janim_stickCut, *Janim_removeGum;
	bool isCollectingInventory;
	CPolygon *stickPoly;
	public:

		L6S4_Pop1PineTree(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L6S4_Pop1PineTree();

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		float easing(float t,float b,float c,float d);

		void OnAnimComplete(int animID);
		void ReceiveMessage(int val);

	protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif