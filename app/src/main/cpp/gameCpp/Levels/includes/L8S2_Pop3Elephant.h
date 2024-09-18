

#ifndef DQFYLH_LEVEL8_SCENE2_POP3_ELEPHANT_DEF
#define DQFYLH_LEVEL8_SCENE2_POP3_ELEPHANT_DEF

//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"
#include "JSONAnimator.h"

class L8S2_Pop3Elephant : public Popup
{
public:
	L8S2_Pop3Elephant(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L8S2_Pop3Elephant();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();

	bool IsMammothNodAnimPlaying;
	bool IsMammothEatAnimPlaying;
	int	 EatAnimCount;
protected:

	JSONAnimator *MammothIdleAnim;

	CObject *Ear, *ElephantAnim, *EyeLid, *Head, *SugarcaneAnim, *SugarcaneInHand, *Trunk1, *Trunk2, *Trunk3, *Tusk, *Wing1, *Wing2;
	CObject *MammothEat1, *MammothEat2, *MammothNod;
	
	void OnAnimationComplete(int index);
	void InitSceneElements();
	void InitAnimations();
};

#endif
