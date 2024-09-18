

#ifndef DQFYLH_L7_S2_HORIGHTHAND_DEF
#define DQFYLH_L7_S2_HORIGHTHAND_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"

#define MAX_L7S2_HORHAND_SYMBOLS 12

class L7_S2_HORightHand : public HiddenObjectSystem
{
public:
	SuperFX *s2handhostarsFX;
	float musicFadeOutTimer;

	SuperFX *symbolShowFX, *hoClickFX;
	SuperFX *s2hocompleteFX;

	CObject* diskRotateBtn;

	CObject* hudSymbols[MAX_L7S2_HORHAND_SYMBOLS];
	CObject* symbolGlows[MAX_L7S2_HORHAND_SYMBOLS];
	CRectangle* symbolRects[MAX_L7S2_HORHAND_SYMBOLS];

	bool isHOCompleted;
	float hoWaitTimer;	

	int numSceneSymbolsFound;
	void CheckAndFindHOSymbol(int id);

	void ActivateHO();
	void DeactivateHO();

	CPoint hudPos[4];

	int hudSymbolGroupID;
	void ShowNextHUDSymbols(bool rotateNeedle=true);

	L7_S2_HORightHand();
	~L7_S2_HORightHand();

	void Update();
	void Required();

	bool isHudSymbolsSliding;
	void OnHudSymbolTweenComplete(CObject* obj);

	void ShowHintForSymbols();
	void NavigateToScene();

protected:
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
};

#endif
