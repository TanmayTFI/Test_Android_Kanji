#pragma once
#include "Popup.h"
class L3_S2_M2PipePopUp : public Popup
{

	int _noOfClicks;

	CObject *_pipe;
	std::vector<CObject*> _mudOnPipe;

private:

	void Update(float ds);
	void OnPopupOpened();

public:

	L3_S2_M2PipePopUp(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	virtual ~L3_S2_M2PipePopUp(void);

	void ProcessHint();
};

