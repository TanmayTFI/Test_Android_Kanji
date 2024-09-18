#ifndef DQFYLH_L7_S2_HANDHOBASICS_DEF
#define DQFYLH_L7_S2_HANDHOBASICS_DEF

//====================================
//  created by : Jithin
//  copyright  : Tuttifrutti Games
//====================================

#include "GameFlow.h"
#include <map>
#include <vector>
#include <list>
#include <stack>
#include "UIButton.h"

namespace Level7_Scene_2
{
	enum SymbolSide
	{
		None = -1,
		Front_Left = 0,
		Front_Center,
		Front_Right,
		Back_Left,
		Back_Center,
		Back_Right
	};


	class HODelegate
	{
	public:

		CObject* _selectedObj;
		virtual void onSelection(CObject *obj) = 0;
	};


	class HiddenObject : public CObject
	{

		bool _isCompleted;
		bool _isSelected;

		CObject* _fullVersion;

		C_SYNTHESIZE(int, _id, ID);
		C_SYNTHESIZE(CPolygon*, _areaPoly, AreaPoly);
		C_SYNTHESIZE(CPoint, _actualPos, ActualPos);
		C_SYNTHESIZE(HODelegate*, _delegate, Delegate);

	public:
		HiddenObject (CObject* obj, CObject *fullObj);
		~HiddenObject ();

		void CustomUpdate();
		void setCompleted();
		void OnComplete();
		void BackToPosition();

	private:

		void BackToPosComplete(CObject* obj);
	};

	class Symbol
	{
		std::map<SymbolSide, CObject*> _symbolObjs;
		std::map<SymbolSide, CObject*> _doorUnlockGlowObjs;
		
		HiddenObject* _matchObject;
		
		C_SYNTHESIZE(int, _index, Index);
		C_SYNTHESIZE(bool, _isActive, IsActive);
		C_SYNTHESIZE_READONLY(bool, _isCompleted, IsCompleted);
		C_SYNTHESIZE_READONLY(SymbolSide, _currentSide, CurrentSide);

	public:

		Symbol(int index, bool isComplete);
		~Symbol();

		void UpdateSymbol(CPoint mPos);
		void setMatchObject(HiddenObject* ho);
		void SetSymbolForSide(CObject* synmol, CObject* doorGlow, SymbolSide side);
		void SetCurrentSide(SymbolSide side);
		bool CopyObject(HiddenObject* obj);
		void HideDoorUnlockGlows();
		void OnMouseOver();
		void OnMouseOut();

	private:

		void OnMatchFound();
	};

	enum HOState
	{
		State_GameStart = 0,
		State_WaitForInteraction,
		State_RotatingBox = 20,
		State_HoldingObject,
		State_PlacingOnSymbol,
		State_OnSymbolMatch,
		State_OnSymbolMismatch,
		State_SkipGame,
		State_CheckIsCompleted,
		State_OnComplete = 100,

	};

	class HandHOControllerDelegate
	{
	public:

		virtual void onSymbolComplete(int index) = 0;
		virtual bool getIsSymbolCompleted(int index) = 0;
	};

	class HOController : public HODelegate
	{
		std::vector<CPolygon*> _objPolyList; 
		std::vector<HiddenObject*> _hiddenObj;
		std::vector<Symbol*> _symbols;
		std::list<int> _shiftList;
		
		int _hoState;
		Symbol *_activeSymbol;
		CObject* _blurBox;
		UIButton *_btnLeftRotate,*_btnRightRotate;
		float _symbChangeDelay;
			
		C_SYNTHESIZE_READONLY(bool, _isCompleted, IsCompleted);
		C_SYNTHESIZE(HandHOControllerDelegate*, _delegate, Delegate);

	public:

		HOController();
		~HOController();

		void SetObjPolys(std::vector<CPolygon*> polyList);
		void InitGame(CHOScene* scene);
		void Update();
		void OnComplete();
		void SkipGame();

	private:

		void onSelection(CObject *obj);
		void ShiftLeftAction();
		void ShiftRightAction();
	};

	enum L7S2_GameSceneState
	{
		L7S2_State_Initialize = 0,
		L7S2_State_GamePlaying,
		L7S2_State_GameCompleted,
		L7S2_State_OpeningDoor,
		L7S2_State_WaitingForCollectGlob,
		L7S2_State_GlobCollected,
		L7S2_State_GameForExtrasComplete
	};

}

#endif