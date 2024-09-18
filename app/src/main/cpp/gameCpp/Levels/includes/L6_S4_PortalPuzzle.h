

#ifndef DQFYLH_L6_S4_PORTAL_PUZZLE_DEF
#define DQFYLH_L6_S4_PORTAL_PUZZLE_DEF

//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "UIButton.h"
#include "L6S4_PopAnathaNote.h"

class L6_S4_PortalPuzzle: public HiddenObjectSystem
{
	std::vector<CObject*> _arrayOfPuzzleObjects;
	std::vector<CObject*> _arrayOfSolvedPuzzleObjects;
	std::vector<CPoint> _arrayOfHolderPositions;
	std::vector<CObject*> _arrayOfInteractiveObjects;
	std::vector<CObject*> _arrayOfHolders;
	UIButton * _clockBtn;
	UIButton * _antiClockBtn;
	CObject * _currentDraggingObject;
	CObject * _glow;
	bool _isMouseInteractionEnabled;
	int _currentSkipObjectIndex;
	float musicFadeOutTimer;
	CPolygon *letterRect;
	int activePopUpId;

	CObject *ResetBtn;
	CHOScene *ResetDia;
	bool IsResetPop;

	float timer;
	bool isFadeIn;
	bool isFadeOut;
	bool isResetDiaFadeIn;
	bool isResetDiaFadeOut;


	L6S4_PopAnathaNote *popNote;
public:
	L6_S4_PortalPuzzle();
	~L6_S4_PortalPuzzle();

	void Update();
	void Required();
	void NavigateToScene();
	void ResetPuzzle();

protected:
	void InitializeWithProfileData();
	void OnPuzzleComplete();
	void OnGlowAnimComplete(CObject * object);
	bool CheckIfPuzzleSolved();
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
	void OnMouseClickOnObject(CObject * object);
	CObject * GetSolvedPuzzleObjectForPosition(CPoint * position);
	void OnRotateClockBtnClick(CObject * object);
	void OnRotateAntiClockBtnClick(CObject * object);
	void OnRotateComplete(CObject * object);
	void OnSkipTweenComplete(CObject * object);
	void ReturnDraggingObject();
	void OnReturnDraggingObjectComplete(CObject * object);
	void OnPopupClosed(Popup * popup);
};

#endif
