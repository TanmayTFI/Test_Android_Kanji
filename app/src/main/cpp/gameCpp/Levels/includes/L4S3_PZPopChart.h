

#ifndef DQFYLH_LEVEL4_SCENE3_FIREPUZZLE_POPUP_CHART_DEF
#define DQFYLH_LEVEL4_SCENE3_FIREPUZZLE_POPUP_CHART_DEF

//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"
#include "MultilineTextBox.h"

class L4S3_PZPopChart : public Popup
{
public:
	L4S3_PZPopChart(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L4S3_PZPopChart();

	MultilineTextBox* metalplate, *woodenboard, *bow, *coin, *twigs, *oliveoil, *embers, *embers2;

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();

protected:
	void OnPopupCloseBtnClicked();
};

#endif
