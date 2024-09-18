
#ifndef DEITIES_PUZZLE_H
#define DEITIES_PUZZLE_H

#include "HiddenObjectSystem.h"
#include "GridPuzzle.h"
#include "GameFlow.h"

#include "SuperFX.h"

using namespace gridpuzzle;

class ParticleEffect;
class CHOScene;

enum L2S4DeitiesPuzzleState
{
	eL2S4DeitiesPuzzleState_started = 0,
	eL2S4DeitiesPuzzleState_Solved,
	eL2S4DeitiesPuzzleState_DoorOpened,
	eL2S4DeitiesPuzzleState_MetalBarTaken,
};


class DeitiesPuzzleObject: public gridpuzzle::GridPuzzleObject
{
private:

	
	CObject * _selectedGlow;
	CObject * _movableTileGlow;
	bool _isSelected;
	std::function<void (CObject*)> _puzzleCompleteAnimCallback;


public:
	
	int prevZOrder;
	CObject * _mouseOverGlow;

	bool __PopupActive;

	 DeitiesPuzzleObject(CObject * mouseOverGlow, CObject * selectedGlow, CObject * movableTileGlow, int gridIndex, bool isMovable);
	~DeitiesPuzzleObject();
	virtual void Update(float ds);
	virtual void OnMouseOver();
	virtual void OnMouseOut();
	void SetIsSelected(bool isSelected);
	bool GetIsSelected();
	void StartPuzzleCompleteAnim(std::function<void (CObject*)> puzzleCompleteAnimCallback = nullptr);
	void OnCompleteGlowUpTween(CObject * object);
	CObject * GetMovableTileGlow();


	
};

class L2_S4_DeitiesPuzzle: public HiddenObjectSystem, GridPuzzle
{
private:
	float musicFadeOutTimer;
	std::vector<DeitiesPuzzleObject*> _arrayOfPuzzleObjects;
	bool _isTweeningAnyObject;
	bool _isDoorOpenAnimPlaying;
	CObject * _metalLever;
	DeitiesPuzzleObject* _swapObject1;
	DeitiesPuzzleObject* _swapObject2;
	int _tweenCompleteCounter;

	CObject * _door;

	CObject *ResetBtn;
	CHOScene *ResetDia;
	float timer;
	bool isFadeIn;
	bool isFadeOut;
	bool isResetDiaFadeIn;
	bool isResetDiaFadeOut;

public:

	L2_S4_DeitiesPuzzle();
	~L2_S4_DeitiesPuzzle();

	void InitializeWithProfileData();
	bool IsResetPop;
	void OnMouseClick(CObject * obj);
	void OnMouseClickOnTile(CObject * obj);
	void Update();
	void UpdateChangesToProfile();
	void ProcessSkip();
	void ResetPuzzle();
	void Required();
	float easing(float t,float b,float c,float d);
	void onPuzzleSolved();
	void OnMouseOver(CObject * object);
	void OnMouseOut(CObject * object);
	void OnCompleteDoorOpenAnim(CObject * object);
	void SwapTiles(DeitiesPuzzleObject* object1, DeitiesPuzzleObject* object2);
	void onTweenComplete(CObject* object);
	void UpdateTileMouseInteraction();
	void OnPuzzleCompleteAnimFinish(CObject* obj);

	void NavigateToScene();
	
protected:

	void ReceiveMessage(int val);
	void ProcessNonHOHint();	
};

#endif