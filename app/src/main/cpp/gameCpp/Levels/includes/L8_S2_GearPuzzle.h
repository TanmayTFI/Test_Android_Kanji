
#ifndef GEAR_PUZZLE_H
#define GEAR_PUZZLE_H

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "ClickablePolygon.h"
#include "JSONAnimator.h"


class ParticleEffect;
class GearSlot;

enum L8S2GearPuzzleState
{
	eGearPuzzleState_started = 0,
	eGearPuzzleState_KeyInserted,
	eGearPuzzleState_Solved,
	eGearPuzzleState_DoorOpened
};


enum GearState
{
	eGearState_Default,
	eGearState_Dragging,
	eGearState_Fixed,
	eGearState_Rotating
};

class Gear : public CObject
{	
private:
	int _id;
	GearState _state;
public:
	GearSlot* gearSlot;
	CObject * normal;
	CObject * shadow;
	float solvedPosition;
	CPoint invPosition;
	float solvedZorder;
	float angularVelocity;
	float angularSpeed;
	float radius;
	float offsetY;
	Gear(int id);
	~Gear();

	void SetState(GearState state);

	GearState GetState();
	int GetId();
	void Update(float ds);
};

class GearSlot
{
private:
	Gear * _gear;

public:
	
	CPoint  position;
	float maxRadius;
	float radius;
	int rotationDirection;
	bool isSolved;
	bool isRotating;
	std::vector<GearSlot*> drivingNeighbours;

	GearSlot();
	~GearSlot();
	void SetGear(Gear* gear);
	Gear * GetGear();
	

};

class L8_S2_GearPuzzle: public HiddenObjectSystem
{
private:

	std::vector<Gear*> _arrayOfGears;
	std::vector<GearSlot *> _arrayOfGearStots;
	std::vector<Gear*> _arrayOfInteractiveGears;
	Gear * _currentDraggingGear;
	JSONAnimator * _jsonAnimDoorOpen;
	JSONAnimator * _jsonAnimKeyInsertion;
	CObject * _door;
	CObject * _key;
	CObject * _tool;
	CObject * _code;
	int maskZorder;
	bool _isDoorMoving;


public:
	L8_S2_GearPuzzle();
	~L8_S2_GearPuzzle();

	void InitializeWithProfileData();
	void Update();
	void Required();
	void OnPuzzleSolved();
	void OnMouseClickOnGear(Gear * gear);
	bool IsOverlapWithGears(Gear * gear);
	bool IsTouchWithGears(Gear * gear1, Gear * gear2);
	void UpdateGearStates();
	void UpdateGearState(GearSlot * gearSlot);
	void HandleMouseClickOnGears();
	void OnMouseClickOnObject(CObject * object);
	GearSlot* GetGearSlot(Gear * gear); 

	bool IsPuzzleSolved();
	void SkipPuzzle();
	void ProcessSkip();
	void TweenGearToInv(Gear * gear);
	void OnTweenGearComplete(CObject * gear);
	void OnDoorOpenAnimComplete(int id);
	void OnKeyInsertionAnimComplete(int id);
	void OnSkipAnimComplete(CObject * gear);

	void NavigateToScene();

protected:
	void TaskPendingUpdate();
	void ReceiveMessage(int val);
	void ProcessNonHOHint();
};

#endif