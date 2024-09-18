

#ifndef DQFYLH_LEVEL8_SCENE2_POP1_GATE_DEF
#define DQFYLH_LEVEL8_SCENE2_POP1_GATE_DEF

//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"

class L8S2_Pop1Gate : public Popup
{
public:
	L8S2_Pop1Gate(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L8S2_Pop1Gate();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	JSONAnimator *KeyInsertAnim, *KeyTurnAnim;

	void ProcessHint();
	bool isTaskLeft();

protected:
	void OnAnimationComplete(int index);
	void InitSceneElements();

};

#endif
