
#ifndef FAN_PUZZLE_H
#define FAN_PUZZLE_H

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "ClickablePolygon.h"
#include "JSONAnimator.h"


class ParticleEffect;

enum L4S2FanPuzzleState
{
	eFanPuzzleState_started = 0,
	eFanPuzzleState_KeyInserted,
	eFanPuzzleState_Solved,
	eFanPuzzleState_DoorOpened
};


enum FanState
{
	eFanState_Default,
	eFanState_Selected,
	eFanState_MouseOver,
	eFanState_Locked
};

class Fan : public CObject
{	
private:
	int _id;
	FanState _state;
public:
	//int index;
	CObject * normal;
	CObject * selectedGlow;
	CObject * greyImage;
	float solvedRotation;
	float solvedZorder;
	Fan(int id);
	~Fan();

	void SetState(FanState state);
	FanState GetState();
	int GetId();
	void Update(float ds);
};

class L4_S2_FanPuzzle: public HiddenObjectSystem
{
private:

	std::vector<Fan*> _arrayOfFans;
	ClickablePolygon * _fanAreaPolygon;
	CObject * _doorLock;
	Fan * _prevSelectedFan;
	Fan * _currentSelectedFan;
	bool _isMouseInteractionOnFanEnabled;
	std::vector<Fan*>::iterator _currentFanIterator;
	std::vector<Fan*>::iterator _prevFanIterator;
	JSONAnimator * _jsonAnimInsertKey;
	JSONAnimator * _jsonAnimRotateKey;
	JSONAnimator * _jsonAnimStuckKey;
	JSONAnimator * _jsonAnimOpenDoor;
	CPolygon * _keyDropArea;
	int _firstFanZOrder;

public:
	float musicFadeOutTimer;
	L4_S2_FanPuzzle();
	~L4_S2_FanPuzzle();

	CObject *ResetBtn;
	CHOScene *ResetDia;
	bool IsResetPop;

	float timer;
	bool isFadeIn;
	bool isFadeOut;
	bool isResetDiaFadeIn;
	bool isResetDiaFadeOut;

	void InitializeWithProfileData();
	void Update();
	void Required();
	void OnPuzzleSolved(CObject * obj);
	void OnMouseClickOnFan(int polyId);
	void OnKeyInsertAnimComplete(int id);
	void OnKeyRotateAnimComplete(int id);
	void OnOpenDoorAnimComplete(int id);
	void OnKeyStuckAnimComplete(int id);
	void OnKeyClick(CObject * object);

	bool IsPuzzleSolved();
	void OnTweenComplete(CObject * object);
	Fan* GetFan(CPoint mousePos);
	void OpenFan(CObject * obj);
	void CloseFan();
	void FadeOutFanGlow(UpdateListEntity* obj);
	void OnFadeOutFanComplete(CObject * object);
	void Shuffle(CObject * object);
	void OnShuffleCompleted(CObject * obj);
	void SkipPuzzle();
	void ResetPuzzle();
	void ProcessSkip();
	void SortZOrders();
	void UpdateFanStates();
	//void OnSkipAnimComplete(CObject * object);
	void NavigateToScene();
	
protected:
	void TaskPendingUpdate();
	void ReceiveMessage(int val);
	void ProcessNonHOHint();
};

#endif