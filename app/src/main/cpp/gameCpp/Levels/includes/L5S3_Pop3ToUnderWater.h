

#ifndef DQFYLH_L5S3_Pop3ToUnderWater_DEF
#define DQFYLH_L5S3_Pop3ToUnderWater_DEF

//====================================
//  created by : JIthin M
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"
#include "PopupInterface.h"
#include "MeshObject.h"

class L5S3_Pop3ToUnderWater : public Popup
{
	JSONAnimator *JAnim_BagPlacing;
	C_SYNTHESIZE(PopupInterface*, _interface, Interface);

public:
	MeshObject* water;
	MeshObject* waterEffect;
	L5S3_Pop3ToUnderWater(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L5S3_Pop3ToUnderWater();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();

protected:

	void OnAnimationComplete(int animID);

};

#endif
