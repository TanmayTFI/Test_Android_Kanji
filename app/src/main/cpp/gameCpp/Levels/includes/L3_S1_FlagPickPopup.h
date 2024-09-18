
#ifndef L3_S1_FlagPickPopup_
#define L3_S1_FlagPickPopup_

#include "Popup.h"
#include "PopupInterface.h"
#include "MeshObject.h"

class L3_S1_FlagPickPopup : public Popup
{
	CObject* flagObj;
	JSONAnimator *_janim_PickFlag;
	MeshObject *_flagMesh;
	C_SYNTHESIZE(PopupInterface*, _interface, Interface);

public:
	L3_S1_FlagPickPopup(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L3_S1_FlagPickPopup(void);

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

private:

	void Update(float ds);
	void OnAnimationComplete(int animId);
	void ProcessHint();
	bool isTaskLeft();

	void OnScaleUpComplete();
	void OnPopupCloseBtnClicked();
};

#endif

