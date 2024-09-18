#ifndef DQFYLH_LEVEL6_SCENE5_POP1_SOPHIA_DEF
#define DQFYLH_LEVEL6_SCENE5_POP1_SOPHIA_DEF

#include "DialogSystem.h"
#include "Popup.h"

class L6S5_Pop1Sophia: public Popup
{
	public:		
		DialogSystem *dialogSystem;
		bool isDialogActive;
		L6S5_Pop1Sophia(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L6S5_Pop1Sophia();

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		float easing(float t,float b,float c,float d);
		void SetDialogSystem(DialogSystem *l_dialogSystem);
		void OnAnimComplete(int animID);
		void OnPopupOpened();
	protected:
		
		void OnDialogComplete(int index );
		void ProcessHint();
		bool isTaskLeft();
};

#endif