
#ifndef ALLIGATOR_PUZZLE_H
#define ALLIGATOR_PUZZLE_H

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "MeshObject.h"

#include "SuperFX.h"

class ParticleEffect;

enum L4S2AlligatorPuzzleState
{
	eAlligatorPuzzleState_started = 0,
	eAlligatorPuzzleState_AlligatorHeadPlaced,
	eAlligatorPuzzleState_Solved,
	eAlligatorPuzzleState_MortarHandleTaken
};

enum AlligatorState
{
	eAlligatorState_Closed,
	eAlligatorState_Open,
	eAlligatorState_Right,
	eAlligatorState_Wrong
};

class Alligator : public CObject
{	
private:
	int _id;
	AlligatorState _state;
public:

	CObject * normalSymbol;
	CObject * closedMouth;
	CObject * rightSymbol;
	CObject * wrongSymbol;
	CObject * openedMouth;
	CObject * waterSplash;
	CObject * waterRipple;
	CPoint symbolOffset;
	CPoint rippleOffset;
	CPoint splashOffset;
	
	


	Alligator(int id);
	~Alligator();

	void SetState(AlligatorState state);
	AlligatorState GetState();
	int GetId();
	void SetXY( float x, float y);
	void PlayWaterSplashAnim();
	
};

class L4_S2_AlligatorPuzzle: public HiddenObjectSystem
{
private:

	std::vector<Alligator*> _arrayOfAlligators;
	Alligator * _prevSelectedAlligator;
	Alligator * _currentSelectedAlligator;
	MeshObject * _waterMeshObject;
	Alligator * _whiteAlligator;
	bool _isMouseInteractionEnabled;
	std::vector<CObject*> _arrayOfInteractiveObjects;

public:
	bool isWaitingForPuzzleSolved;
	float musicFadeOutTimer;
	L4_S2_AlligatorPuzzle();
	~L4_S2_AlligatorPuzzle();

	void InitializeWithProfileData();
	void Update();
	void Required();
	void OnPuzzleSolved(UpdateListEntity *object);
	void OnMouseClickOnAlligator(CObject * obj);
	void OnMortarClick(CObject * object);
	void CloseAlligatorMouths(UpdateListEntity *object);
	bool IsPuzzleSolved();
	void ProcessSkip();

	CObject *ResetBtn;
	CHOScene *ResetDia;
	bool IsResetPop;

	float timer;
	bool isFadeIn;
	bool isFadeOut;
	bool isResetDiaFadeIn;
	bool isResetDiaFadeOut;

	void ResetPuzzle();
	void NavigateToScene();
	
protected:
	void TaskPendingUpdate();
	void ReceiveMessage(int val);
	void ProcessNonHOHint();
};

#endif