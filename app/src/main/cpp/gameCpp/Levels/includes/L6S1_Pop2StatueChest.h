#ifndef DQFYLH_LEVEL6_SCENE1_POP2_STATUE_CHEST_DEF
#define DQFYLH_LEVEL6_SCENE1_POP2_STATUE_CHEST_DEF

#include "Popup.h"
#include "UIButton.h"

enum L6S1ChestSymbolState
	{
		eL6S1ChestSymbolState_Normal = 0,
		eL6S1ChestSymbolState_Selected,
		eL6S1ChestSymbolState_RightSelection,
		eL6S1ChestSymbolState_WrongSelection
	};
class L6S1ChestSymbol;
class L6S1_Pop2StatueChest: public Popup
{
private:

	std::vector<L6S1ChestSymbol*> _arrayOfSymbols;
	CObject * _needle;
	UIButton * _leftArrowBtn;
	UIButton * _rightArrowBtn;
	JSONAnimator * _jsonAnimKeyInsertion;
	JSONAnimator * _jsonAnimKeyStuck;
	CObject * _starKey;
	CObject * _starKeyHolder;
	int _currentSymbolIndex;
	int _numOfRightSymbolsVisited;
	bool _isMouseInteractionEnabled;

	public:
		
		L6S1_Pop2StatueChest(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L6S1_Pop2StatueChest();

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		float easing(float t,float b,float c,float d);
		void OnAnimComplete(int animID);

		void OnKeyInsertAnimComplete(int animId);
		void OnNeedleTweenComplete(CObject * obj);
		void OnArrowBtnClick(CObject * obj);
		void OnKeyRotationAnimComplete(CObject * obj);
		void Reset();
		void OnResetComplete(CObject * obj);
		void OnPuzzleComplete();
		bool IsPuzzleCompleted();
		L6S1ChestSymbolState GetChestSymbolStateFor(int index);
		void ProcessSkip();

	protected:
		void ProcessHint();
		bool isTaskLeft();
		void OnPopupOpened();
		
};


class L6S1ChestSymbol
{
	
private:
	L6S1ChestSymbolState _symbolState;
public:
	CObject * normalSymbol;
	CObject * selectedSymbol;
	CObject * rightSelectionGlow;
	CObject * wrongSelectionGlow;

	L6S1ChestSymbol();
	~L6S1ChestSymbol();
	void SetSymbolState(L6S1ChestSymbolState state);
	L6S1ChestSymbolState GetSymbolState();
};

#endif