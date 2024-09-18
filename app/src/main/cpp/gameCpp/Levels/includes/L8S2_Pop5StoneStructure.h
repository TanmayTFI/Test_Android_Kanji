

#ifndef DQFYLH_LEVEL8_SCENE2_POP5_STONESTRUCTURE_DEF
#define DQFYLH_LEVEL8_SCENE2_POP5_STONESTRUCTURE_DEF

//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"

class L8S2_Pop5StoneStructure: public Popup
{
public:
	L8S2_Pop5StoneStructure(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L8S2_Pop5StoneStructure();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();
	CPolygon *ScrollPoly, *ToolPoly;
	JSONAnimator *KeyInsertAnim, *KeyTurnAnim;

protected:
	void OnAnimationComplete(int index);
	void InitSceneElements();

};

#endif
