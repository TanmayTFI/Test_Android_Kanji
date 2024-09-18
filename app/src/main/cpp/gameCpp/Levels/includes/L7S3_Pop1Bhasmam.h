

#ifndef DQFYLH_L7_S3_POP1_BHASMAM_TWO_DEF
#define DQFYLH_L7_S3_POP1_BHASMAM_TWO_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"
#include "JSONAnimator.h"

class L7S3_Pop1Bhasmam : public Popup
{
public:
	L7S3_Pop1Bhasmam(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L7S3_Pop1Bhasmam();

	SuperFX* dustFX;
	JSONAnimator *JAnim_BroomCollect;

	void OnAnimationComplete(int animID);

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	//UI sliding handling
	CObject *UIBtnUp, *UIBtnDown;

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
