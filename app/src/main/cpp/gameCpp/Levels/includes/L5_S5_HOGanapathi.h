

#ifndef DQFYLH_L5_S5_HO_GANAPATHI_DEF
#define DQFYLH_L5_S5_HO_GANAPATHI_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Match3.h"

class L5S5HiddenObject : public CObject
{
	bool _isAnimComplete;
	float hoScaleTimer, hoFlyingTimer;
	bool _isObjectScaling;
	int _numberOfClicks;
	bool _isMultiHOComplete;

	CPoint *_startPoint;
	CPoint *_endPoint;
	CPoint *_ctrlPoint1;
	CPoint *_ctrlPoint2;
	CPoint _objectPos;

	SuperFX *HOTrailFX;
	SuperFX *waterHOFX;
	int FXTimer;

	std::vector<L5S5HiddenObject*> _aliasList;

	C_SYNTHESIZE(int, _index, Index);
	C_SYNTHESIZE(std::string, _name, Name);
	C_SYNTHESIZE(CBitmapText*, _nameText, NameText);
	C_SYNTHESIZE(CObject*, _toFind, ToFind);
	C_SYNTHESIZE(CObject*, _completeObject, CompleteObject);
	C_SYNTHESIZE(CPolygon* ,_polygonArea, PolygonArea);
	C_SYNTHESIZE(int, _clicksToFind, ClicksToFind); 
	C_SYNTHESIZE_READONLY(bool, _isFound, IsFound);
	C_SYNTHESIZE_READONLY(bool, _isMuliHO, IsMultiHO);

public:

	L5S5HiddenObject();
	~L5S5HiddenObject();

	void AddToMultiHO(L5S5HiddenObject *obj);
	void InitObject();
	bool MouseClicked(CPoint pos);
	void Update();
	void AlreadyFound();

	void SwitchMatch3();
	void SwitchHO();

	void UpdateAlias();

private:

	void OnFound();
	void InitInvNameText();
};



typedef enum 
{
	L5S5HO_State_StartMatch3 = 0,
	L5S5HO_State_StopMatch3,
	L5S5HO_State_Initilizing = 1000,
	L5S5HO_State_GameStarted,
	L5S5HO_State_WaitForInteraction,
	L5S5HO_State_OnInteraction,
	L5S5HO_State_OnHint,
	L5S5HO_State_FoundObject,
	L5S5HO_State_Match3Completed,
	L5S5HO_State_FoundAllObjects,
	L5S5HO_State_HOCompleted = 1250,
	L5S5HO_State_GameComplete,
	L5S5HO_State_GameForExtrasComplete
}L5_S5_HOGameState;

class L5_S5_HOGanapathi : public HiddenObjectSystem
{
	float musicFadeOutTimer;
	CPolygon* HOPolygonArray[14];
	std::vector<L5S5HiddenObject*> _hiddenObjs;
	std::vector<L5S5HiddenObject*> _foundObjs;
	std::vector<L5S5HiddenObject*> _multiObjs;

	int _gameState;
	int _invItemId;
	
	CObject* _btnMatch3;
	CObject* _btnSwitchToHO;
	CObject *match3Back, *match3BackText;

	Match3 *_match3Ctrl;

	SuperFX *s5HOSmokeFgFX;
	SuperFX *s5HOLightFX;

public:
	L5_S5_HOGanapathi();
	~L5_S5_HOGanapathi();

	void Update();
	void Required();
	void NavigateToScene();

protected:
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();

private:

	void InitPolygonArray();
	void InitHO();
};

#endif
