#ifndef DQFYLH_LEVEL4_SCENE1_POP1_RIGHT_FOOT_DEF
#define DQFYLH_LEVEL4_SCENE1_POP1_RIGHT_FOOT_DEF

#include "Popup.h"
#include "SuperLog.h"
#include "JSONAnimator.h"


class L4S1_Pop1RightFoot: public Popup
{
	bool isThornRemoving;
	public:
		JSONAnimator *Janim_thorn1Remove,*Janim_thorn2Remove,*Janim_removeShoe,*Janim_cleanWithWater,*Janim_shoePutBack;
		CObject *MedPaste;
		L4S1_Pop1RightFoot(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L4S1_Pop1RightFoot();

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();

		float easing(float t,float b,float c,float d);

		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		void OnShoeRemoved(int animID);
		void OnShoePutBack(int animID);
		void OnThornRemoved(int animID);

	protected:

		bool isTaskLeft();
		void ProcessHint();
};

#endif