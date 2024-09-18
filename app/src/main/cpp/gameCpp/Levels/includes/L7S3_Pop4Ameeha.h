

#ifndef DQFYLH_L7_S3_AMEEHAPOP_DEF
#define DQFYLH_L7_S3_AMEEHAPOP_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"
#include "MeshObject.h"

class L7S3_Pop4Ameeha : public Popup
{
public:
	L7S3_Pop4Ameeha(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L7S3_Pop4Ameeha();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();

	MeshObject *meshAmeeha;

protected:

};

#endif
