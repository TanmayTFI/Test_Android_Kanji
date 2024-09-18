#ifndef DQFYLH_LEVEL4_SCENE5_POP3_SKELETON_DEF
#define DQFYLH_LEVEL4_SCENE5_POP3_SKELETON_DEF

#include "Popup.h"
#include "L4S4_PopInstruction.h"

class L4S5_Pop3Skeleton: public Popup
{
	int activePopUPID;
	L4S4_PopInstruction *popChart;

	public:

		L4S5_Pop3Skeleton(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L4S5_Pop3Skeleton();

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();
		void OnPopupClosed(Popup * popup);

		float easing(float t,float b,float c,float d);

	protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif