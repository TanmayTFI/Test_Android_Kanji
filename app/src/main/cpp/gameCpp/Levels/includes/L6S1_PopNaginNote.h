#ifndef DQFYLH_LEVEL6_SCENE1_POP_NAGIN_NOTE_DEF
#define DQFYLH_LEVEL6_SCENE1_POP_NAGIN_NOTE_DEF

#include "Popup.h"

class L6S1_PopNaginNote: public Popup
{
	public:

		L6S1_PopNaginNote(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L6S1_PopNaginNote();

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		float easing(float t,float b,float c,float d);

		void OnAnimComplete(int animID);

		void OnPopupOpened();

		CObject *naginTalkAnim;

	protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif