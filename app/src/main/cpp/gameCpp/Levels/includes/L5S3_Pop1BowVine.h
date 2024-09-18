

#ifndef DQFYLH_L5S3_Pop1BowVine_DEF
#define DQFYLH_L5S3_Pop1BowVine_DEF

//====================================
//  created by : Jithin M
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"
#include "PopupInterface.h"
#include "MeshObject.h"

class L5S3_Pop1BowVine : public Popup
{
	SuperFX *PoolWaterFallFX, *PoolWaterFallBottomFX;
	JSONAnimator* _janim_ShootArrow;
	bool _isArrowDroped;
	
	C_SYNTHESIZE(PopupInterface*, _interface, Interface);

public:
	MeshObject* waterObj;
	L5S3_Pop1BowVine(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L5S3_Pop1BowVine();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();
	bool isTaskLeft2();

	void OpenPopup(bool withArrow);

protected:

	void OnAnimationComplete(int animID);
	void OnPopupOpened();
	void OnPopupCloseBtnClicked();
};

#endif
