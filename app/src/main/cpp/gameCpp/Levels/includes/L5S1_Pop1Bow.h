

#ifndef DQFYLH_L5S1_POPUP1BOW_DEF
#define DQFYLH_L5S1_POPUP1BOW_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"
#include "MeshObject.h"

class L5S1_Pop1Bow : public Popup
{
public:
	L5S1_Pop1Bow(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L5S1_Pop1Bow();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();

	void OnPopupOpened();

protected:

	MeshObject *_leafMesh;

};

#endif
