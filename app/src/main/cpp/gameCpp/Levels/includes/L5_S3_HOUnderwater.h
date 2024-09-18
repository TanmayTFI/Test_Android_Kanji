

#ifndef DQFYLH_L5_S3_HO_UNDERWATER_DEF
#define DQFYLH_L5_S3_HO_UNDERWATER_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Match3.h"

class L5S3HiddenObject : public CObject
{
	bool _isAnimComplete;
	float hoScaleTimer, hoFlyingTimer;
	bool _isObjectScaling;
	int _numberOfClicks;

	CPoint *_startPoint;
	CPoint *_endPoint;
	CPoint *_ctrlPoint1;
	CPoint *_ctrlPoint2;
	CPoint _objectPos;

	SuperFX *HOTrailFX;
	SuperFX *waterHOFX;
	int FXTimer;

	C_SYNTHESIZE(int, _index, Index);
	C_SYNTHESIZE(std::string, _name, Name);
	C_SYNTHESIZE(CBitmapText*, _nameText, NameText);
	C_SYNTHESIZE(CObject*, _toFind, ToFind);
	C_SYNTHESIZE(CObject*, _completeObject, CompleteObject);
	C_SYNTHESIZE(CPolygon* ,_polygonArea, PolygonArea);
	C_SYNTHESIZE(int, _clicksToFind, ClicksToFind); 
	C_SYNTHESIZE_READONLY(bool, _isFound, IsFound);

public:

	L5S3HiddenObject();
	~L5S3HiddenObject();

	void InitObject();
	bool MouseClicked(CPoint pos);
	void Update();
	void AlreadyFound();

	void SwitchMatch3();
	void SwitchHO();

private:

	void OnFound();
	void InitInvNameText();
};

typedef enum 
{
	L5S3HO_State_StartMatch3 = 0,
	L5S3HO_State_StopMatch3,
	L5S3HO_State_Initilizing = 1000,
	L5S3HO_State_GameStarted,
	L5S3HO_State_WaitForInteraction,
	L5S3HO_State_OnInteraction,
	L5S3HO_State_OnHint,
	L5S3HO_State_FoundObject,
	L5S3HO_State_Match3Completed,
	L5S3HO_State_FoundAllObjects,
	L5S3HO_State_HOCompleted = 1300,
	L5S3HO_State_GameComplete,
	State_L5S3GameInExtrasComplete
}L5_S3_HOGameState;

class L5_S3_HOUnderwater : public HiddenObjectSystem
{
	float musicFadeOutTimer;
	CPolygon* HOPolygonArray[14];
	std::vector<L5S3HiddenObject*> _hiddenObjs;
	std::vector<L5S3HiddenObject*> _foundObjs;

	int _gameState;
	CBitmapText* _countText;
	
	SuperFX *S3Bubble1FX, *S3Bubble2FX, *S3Bubble3FX;
	
	CObject* _btnMatch3;
	CObject* _btnSwitchToHO;
	CObject *match3Back, *match3BackText;

	Match3 *_match3Ctrl;

	CObject* _starFish;
	CPolygon *_startFishPoly;
	
public:

	L5_S3_HOUnderwater();
	~L5_S3_HOUnderwater();

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
