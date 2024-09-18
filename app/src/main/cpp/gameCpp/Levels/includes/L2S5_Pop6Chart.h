#ifndef DQFYLH_LEVEL2_SCENE5_POP6_CHART_DEF
#define DQFYLH_LEVEL2_SCENE5_POP6_CHART_DEF

#include "Popup.h"
#include "MultilineTextBox.h"

class L2_S5_PZMedicineMaking;

class L2S5_Pop6Chart: public Popup
{
	public:
		L2S5_Pop6Chart(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);

		~L2S5_Pop6Chart();

		void TakeInventoryItem(EHOInventory ItemID);
		void Update(float ds);
		void Required();

		float easing(float t,float b,float c,float d);
		void OnPopupCloseBtnClicked();

		MultilineTextBox* mangoTxt, *waxTxt, *grinderTxt, *liquidTxt, *aleoveraTxt, *aleopiecesTxt, *turmericTxt, *minigrinderTxt, *turmericmixTxt, *boiledwaterTxt;

	protected:

		L2_S5_PZMedicineMaking *parentScene;
		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif