#ifndef L3_S2_M1PipePopUp_
#define L3_S2_M1PipePopUp_

#include "Popup.h"
#include "MeshObject.h"

class L3_S2_M1PipePopUp : public Popup
{
	CObject *_waterMask;
	CObject *_pipeObj;
	JSONAnimator *_janim_PipePick;

private:

	void Update(float ds);
	void OnScaleUpComplete();
	void OnPopupCloseBtnClicked();

	void OnAnimationComplete(int animID);

public:
	L3_S2_M1PipePopUp(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L3_S2_M1PipePopUp(void);

	void HandleInventoryDrop(CPoint dropPos, int itemVal);
	void ProcessHint();
};

#endif
