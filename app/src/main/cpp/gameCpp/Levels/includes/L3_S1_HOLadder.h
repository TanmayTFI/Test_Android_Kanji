//====================================
// DQFULP
// Hidden Object System
// Author : Faisal
// LEVEL3 - SCENE1 : HOLadder
//====================================

#ifndef DQFYLH_LEVEL3_SCENE1_HOLADDER_DEF
#define DQFYLH_LEVEL3_SCENE1_HOLADDER_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Match3.h"

class CHOScene;

#define MAX_L3S1_HOOBJECTS 12
#define MAX_HUDL3S1_SYMBOLS 7

class L3S1HiddenObject : public CObject
{
	bool _isAnimComplete;
	float hoFlyingTimer;
	bool _isObjFlying;
	
	CPoint *_startPoint;
	CPoint *_endPoint;
	CPoint *_ctrlPoint;	

	SuperFX *HOTrailFX;
	SuperFX *hoClickFX;
	SuperFX *symbolShowFX;
	int FXTimer;

	std::function<void (int)> _flownToHUDCallback;
	
	C_SYNTHESIZE(int, _objID, ObjectID);
	C_SYNTHESIZE(CObject*, _hudSymbol, HudSymbol);
	C_SYNTHESIZE(CObject*, _symbolGlow, SymbolGlow);
	C_SYNTHESIZE(CObject*, _scnObj, SceneObject);
	C_SYNTHESIZE(CPolygon* ,_polygonArea, PolygonArea);
	C_SYNTHESIZE_READONLY(bool, _isFound, IsFound);
	C_SYNTHESIZE_READONLY(bool, _isActive, IsActive);

public:

	L3S1HiddenObject();
	~L3S1HiddenObject();

	void InitObject(std::function<void (int)> flownToHUDCallback);
	bool MouseClicked(CPoint pos);
	void Update(float ds);
	void AlreadyFound();

	void SwitchToMatch3();
	void SwitchToHO();

	void SetActive(bool f){
		_isActive = f;
		_hudSymbol->Show(f);
	}

private:

	void OnFound();
};

class L3_S1_HOLadder: public HiddenObjectSystem
{
	public:
		float musicFadeOutTimer;
		int nextSymbolID;

		bool isHOComplete;
		float hoEndDelayTimer;

		void HandleHOComplete();
		bool CheckHOComplete();

		L3S1HiddenObject *HObjects[MAX_L3S1_HOOBJECTS];
	CObject *match3Back, *match3BackText;

	CPolygon	*HOPolyArray[MAX_L3S1_HOOBJECTS];
		CPoint		*HudSymbolPoints[MAX_HUDL3S1_SYMBOLS];

		void InitHO();
		void TakenHOItem(int objID);
		bool CheckIfSymbolShownInHUD(int objID);		

		void OnObjectReachedHUD(int objectID);
		
		L3_S1_HOLadder();
		~L3_S1_HOLadder();

		//Match 3 code
		Match3 *match3Ctrl;

		CObject *BtnMatch3;
		CObject *BtnSwitchToHO;
		//Match 3

		void Update();
		void Required();	
		void NavigateToScene();		

	protected:
		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif