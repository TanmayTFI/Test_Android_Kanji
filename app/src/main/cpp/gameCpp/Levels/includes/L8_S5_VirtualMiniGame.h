

#ifndef DQFYLH_L8_S5_VirtualMiniGame_DEF
#define DQFYLH_L8_S5_VirtualMiniGame_DEF

//====================================
//  created by : Jithin M
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "UIButton.h"
#include <stack>

namespace Level8_Scene5
{
	class HiddenObject
	{
		bool _isComplete;
		SuperFX *HOTrailFX;

		C_SYNTHESIZE(int, _index, Index);
		C_SYNTHESIZE(std::string, _name, Name);
		C_SYNTHESIZE(CBitmapText*, _nameText, NameText);
		C_SYNTHESIZE(CObject*, _toFindObj, ToFindObject);
		C_SYNTHESIZE(CObject*, _completeObj, CompleteObject);
		C_SYNTHESIZE(CPolygon* ,_polygonArea, PolygonArea);
		C_SYNTHESIZE_READONLY(bool, _isFound, IsFound);

	public:

		HiddenObject();
		~HiddenObject();

		void InitObject();
		void EnableHO(bool stratFromBegin = true);
		bool MouseClicked(CPoint pos);
		void AlreadyFound();
		void HideName();

	private:

		void OnFound();
		void InitInvNameText();
};


	class Shape
	{
		bool _isRotation;

		CPoint _finalPos;
		CPoint _hudPos;

		C_SYNTHESIZE(int, _index, Index);
		C_SYNTHESIZE(CObject* ,_obj, Obj);
		C_SYNTHESIZE(CObject*, _hudObj, HudObj);	
		C_SYNTHESIZE(CPolygon*, _polyAreaToPlace, PolyAreaToPlace);
		C_SYNTHESIZE_READONLY(bool, _isComplete, IsComplete);
		
	public:
		
		Shape();
		~Shape();

		void InitProp();
		void AlreadyFixed();
		void Skipping();
		CRectangle getHUDObjRect();
		CRectangle getObjRect();
		void OnSelect();
		void Relese();
		void RotateTo(float angleDiff);
		float GetRotate();
		bool PlaceAt(CPoint pos);
		void setPos(CPoint pos);
		void ShowPattern();

	private:

		void OnTweenEnd(CObject* obj);
	};

	enum GameStates
	{
		GameState_Initializing = 0,
		GameState_PlayingPuzzle,
		GameState_HoldingShape,
		GameState_ShapePlaced,
		GameState_PuzzleComplete,
		GameState_StartingHO = 120,
		GameState_PlayingHO,
		GameState_OnClicked,
		GameState_OnFoundItem,
		GameState_FoundAllItems,
		GameState_FadeOutScene = 240,
		GameState_GameComplete = 320
	};

	class GameController
	{
		int _state;
		std::stack<int> _stateMachine;
		std::vector<CPolygon*> _shapeDropPolys;
		std::vector<CPolygon*> _hiddenObjPolys;
		std::vector<HiddenObject*> _hiddenObjs;
		std::vector<HiddenObject*> _foundObjs;
		std::vector<Shape*> _shapes;
		std::vector<CObject*> _otherObjs;
		CObject* _rotateBtn;
		std::function<void ()> _gameCompleteCallback;
		Shape* _holdingShape;
		HudControl *_hud;
		SuperFX *s5VirtualGamePzCompleteFX;
		SuperFX *s5VirtualGameRoatationFX;

		CHOScene* _puzzleScene;
		CHOScene* _HOScene;

	public:

		GameController();
		~GameController();

		void CreateGame(CHOScene* pzscene, CHOScene* HOGameScene, HudControl *hud, std::function<void ()> gameCompleteCallback);
		void UpdateController(CPoint mPos);

		void SkipPuzzle();
		void ShowHint();

		bool canGoBack();

	private:

		void InitPolyList();
	};
}

using namespace Level8_Scene5;

class L8_S5_VirtualMiniGame : public HiddenObjectSystem
{
	GameController *_controller;
	bool _isGameCompleted;

	CHOScene *bgScene;

public:
	float musicFadeOutTimer;
	L8_S5_VirtualMiniGame();
	~L8_S5_VirtualMiniGame();

	void Update();
	void Required();
	void NavigateToScene();

protected:

	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
	void OnGameComplete();
};


#endif
