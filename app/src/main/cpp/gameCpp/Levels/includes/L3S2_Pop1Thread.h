#ifndef DQFYLH_LEVEL3_SCENE2_POP1_THREAD_DEF
#define DQFYLH_LEVEL3_SCENE2_POP1_THREAD_DEF

#include "Popup.h"

class L3S2_Pop1Thread: public Popup
{
	public:
		bool IsObjFading;
		CObject *tempObj;
		CPolygon *Thread5Poly,*Thread4Poly,*Thread3Poly,*Thread2Poly,*Thread1Poly;
		L3S2_Pop1Thread(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L3S2_Pop1Thread();

		void Update(float ds);
		void Required();
		JSONAnimator  *JAnim_LampLite;
		void OnAnimationComplete(int animID);
		void puzzleSkip();

		CObject *popCloseBtn;
		float easing(float t,float b,float c,float d);
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		bool isTaskLeft();
		void ResetItems();
	protected:

		void ReceiveMessage(int val);
		void ProcessHint();

};

#endif
