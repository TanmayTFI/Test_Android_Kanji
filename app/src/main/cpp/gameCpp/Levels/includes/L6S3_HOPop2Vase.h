

#ifndef DQFYLH_L6S3_HO_POPUP2_VASE_DEF
#define DQFYLH_L6S3_HO_POPUP2_VASE_DEF

//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"

class L6S3_HOPop2Vase: public Popup
{
public:
	L6S3_HOPop2Vase(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L6S3_HOPop2Vase();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);
	void Update(float ds );
	void Required();
	void ProcessHint();
	bool isTaskLeft();

	bool IsSpannerActive;

protected:

	CPoint SpannerOffset;
	void OnPopupOpened();
	JSONAnimator *JAnim_Bolt1Remove, *JAnim_Bolt2Remove, *JAnim_DoorRemove;
	void OnAnimationComplete(int index);
	CObject *Spanner;
};
#endif
