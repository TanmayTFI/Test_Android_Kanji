

#ifndef DQFYLH_L7_S1_STARPUZZLE_DEF
#define DQFYLH_L7_S1_STARPUZZLE_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"

namespace Level7_Scene_1
{
	enum SymbolColor
	{
		Color_None = -1,
		Color_Yellow = 0,
		Color_Red,
		Color_Blue
	};

	class SymbolForSelect : public CObject
	{

		C_SYNTHESIZE(int, _index, Index);
		C_SYNTHESIZE(bool, _selected, Selected);
		C_SYNTHESIZE(CPolygon*, _boundArea, BoundArea);

	public:
		
		SymbolForSelect(CObject* obj);
		~SymbolForSelect();

		void OnCorrectSelection();
		void OnWrongSelection();
		void Reset();

	private:

	};


	class StarColor : public CObject
	{
		std::vector<SymbolForSelect*> _symbolList;
		std::vector<CObject*> _doors;
		SymbolColor _color;
		int _selectedCount;
		bool _isSymbolComplete;
		int _doorOpenIndex;

		C_SYNTHESIZE(bool, _isActive, IsActive);
		C_SYNTHESIZE(int, _index, Index);
		C_SYNTHESIZE(int, _isDoorsOpened, IsDoorsOpened);
		C_SYNTHESIZE(bool, _isCollected, IsCollected);

	public:
		
		StarColor(CObject *obj, SymbolColor color);
		~StarColor();

		void AddSymbol(SymbolForSelect* symbol);
		void setDoorObject(CObject* obj);
		void Update();
		void ProcessHint(HudControl* Hud);

	private:

		void OnClickRegistered(CPoint mPos);
		void Reset();
		void OpenDoor();
		void OpenDoorTweenComplete(CObject* obj);
	};

}

using namespace Level7_Scene_1;

class L7_S1_PZStarPuzzle : public HiddenObjectSystem
{
	int _collectedStarsCount;
	std::vector<StarColor*> _stars;
	CPolygon* polyArray[3][4];
	bool _isAnyActivePZ;
	bool _isAnyPZCompleted;

public:
	L7_S1_PZStarPuzzle();
	~L7_S1_PZStarPuzzle();
	float musicFadeOutTimer;

	void Update();
	void Required();
	void NavigateToScene();

protected:

	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();

private:
	
	void InitPuzzleObj();
	void setPolyArray();
	void CheckAndUpdateStatus();
};

#endif
