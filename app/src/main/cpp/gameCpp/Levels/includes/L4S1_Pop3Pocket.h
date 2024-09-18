#ifndef DQFYLH_LEVEL4_SCENE1_POP3_POCKET_DEF
#define DQFYLH_LEVEL4_SCENE1_POP3_POCKET_DEF

#include "Popup.h"
#include "SuperLog.h"
#include "JSONAnimator.h"


class L4S1_Pop3Pocket: public Popup
{
	public:
		CObject	*Kerchief, *Open_state, *Closed_state;

		JSONAnimator *Janim_pocketopen;

		L4S1_Pop3Pocket(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L4S1_Pop3Pocket();

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();

		float easing(float t,float b,float c,float d);

	protected:

		void ReceiveMessage(int val);
		void ProcessHint();
		bool isTaskLeft();
};

#endif