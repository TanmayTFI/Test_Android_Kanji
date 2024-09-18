

#ifndef DQFYLH_L7_S2_PUZZLE_LASYASTATUE_DEF
#define DQFYLH_L7_S2_PUZZLE_LASYASTATUE_DEF

//====================================
//  created by : Jithin
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "MultilineText.h"
#include "CVideo.h"

enum L7S2Offset
{
	eOffset_DOWN,
	eOffset_MIDDLE,
	eOffset_TOP,

	eOffset_DOWNMID,
	eOffset_MIDTOP
};

class L7_S2_PZLasyaStatue : public HiddenObjectSystem
{
	
	CVideo* _vdo_teleport; 
	float vidTimer;
	SuperFX *s1StatuePzClickFX;

public:

	CObject* DiffItemsArray[MAX_L7S2_LASYAITEMS];
	CPolygon* itemPoly[MAX_L7S2_LASYAITEMS];

	CHOScene *TopScene;
	float musicFadeOutTimer;

	int offsetID;
	int startPosY;
	int endPosY;

	int numDifferencesFound;

	MultilineText *txtInfo;

	CObject *statueOnHand, *arrowUp, *arrowDown;

	SuperFX *s2difffoundFX;

	L7_S2_PZLasyaStatue();
	~L7_S2_PZLasyaStatue();

	bool IsScreenScrolling;

	void ShowHintForItems();

	float ScrollTimer;
	float ScrollTime;
	bool _teleportVdoPlaying;

	float easing(float t,float b,float c,float d);

	bool HandleItemClick(CPoint pos); 

	void updateScrolling();

	void ActivateHO();

	void Update();
	void Required();
	void NavigateToScene();

protected:
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
};

#endif
