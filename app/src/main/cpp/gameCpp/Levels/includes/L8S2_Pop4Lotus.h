
#ifndef DQFYLH_LEVEL8_SCENE2_POP4_LOTUS_DEF
#define DQFYLH_LEVEL8_SCENE2_POP4_LOTUS_DEF

//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"

class L8S2_Pop4Lotus : public Popup
{
public:
	L8S2_Pop4Lotus(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L8S2_Pop4Lotus();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();

protected:

	CPolygon *LotusPoly;
	CObject *LotusClosed, *LotusClosedEffects, *LotusOpen, *LotusOpenEffects, *SwordHandle, *SwordHandleEffect;

	void InitSceneElements();

};

#endif
