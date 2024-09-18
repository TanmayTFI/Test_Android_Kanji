#ifndef DQFYLH_LEVEL4_SCENE4_POP1_STONEMILL_DEF
#define DQFYLH_LEVEL4_SCENE4_POP1_STONEMILL_DEF

#include "Popup.h"
#include "ClickablePolygon.h"
#include "L4S4_PopInstruction.h"

class L4S4_Pop1StoneMill: public Popup
{
	CPolygon *baseMachineRect;
	public:
		bool isMachineInHand,isOilAnimPlaying;
		CObject *OilMachineBase;
		const CPoint *pointToSnapMachine;

		int activePopUPID;

		ClickablePolygon *InvItemRectWood;

		L4S4_PopInstruction *popChart;

		L4S4_Pop1StoneMill(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L4S4_Pop1StoneMill();

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		void OnPopupClosed(Popup * popup);

		float easing(float t,float b,float c,float d);

	protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif