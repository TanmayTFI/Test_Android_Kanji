

#ifndef DQFYLH_LEVEL6_SCENE2_PINEGUMPUZZLE_POPUP_CHART_DEF
#define DQFYLH_LEVEL6_SCENE2_PINEGUMPUZZLE_POPUP_CHART_DEF

//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"
#include "MultilineTextBox.h"

class L6S2_PZPop1Chart : public Popup
{
public:
	L6S2_PZPop1Chart(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L6S2_PZPop1Chart();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	void ProcessHint();

	MultilineTextBox * stonesTxt, * leavesTxt, * pineTxt, * stickTxt, * pestleTxt, * coalTxt;
	
protected:
	void OnPopupCloseBtnClicked();
};

#endif
