

#ifndef DQFYLH_L3_S4_POP2_MAPWALL_TWO_DEF
#define DQFYLH_L3_S4_POP2_MAPWALL_TWO_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"

class L3S4_Pop2MapWallTwo : public Popup
{
public:
	L3S4_Pop2MapWallTwo(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L3S4_Pop2MapWallTwo();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	//UI sliding handling
	CObject *UINode0, *UINode1, *UINode2, *UINode3, *UINodeGlow;
	CObject *UIBtnNext, *UIBtnPrev;

	void OnPopupOpened();
	void OnPopupCloseBtnClicked();

	float clickDelayTimer;

	void SetupAndShowUI();
	void HideUI();

	bool isSliding, isFadingIn;
	float slideTimer, slideDisplacement, maxDisplacement, maxVelocity;

	void StartSceneSlideIn(bool fromLeft=false);
	void StartSceneSlideOut(bool toLeft=false);

	void resetScene();

	//Popup ID and LoadPrevious if true, next if false
	void setLoadNextCallback(std::function<void (Popup*, bool)> _loadNextPopupCB);
	std::function<void (Popup*, bool)> LoadNextPopup;
	//UI sliding handling

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();

protected:

};

#endif
