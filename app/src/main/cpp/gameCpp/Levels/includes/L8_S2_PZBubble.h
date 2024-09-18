
#ifndef PZBubble_H
#define PZBubble_H

#include "HOScene.h"
#include <vector>
#include "GameFlow.h"

namespace L8_S1_BubblePuzzle
{
	enum BubbleState
	{
		State_Hide = 0,
		State_ReadyToActivate,
		State_Activating,
		State_Active,
		State_Selected,
		State_Match,
		State_NotMatch,
		State_Shuffling,
		State_Complete
	};

	enum PZLevel
	{
		PZLevel_None = 0,
		PZLevel_1,
		PZLevel_2,
		PZLevel_3
	};

	class Symbol : public CObject
	{

		C_SYNTHESIZE_READONLY(int, _id, ID);

	public:

		Symbol(int id){ _id = id; };
		~Symbol(){};
	};


	class Bubble : public CObject
	{
		int _index;
		CPoint _newPos;
		float _delay;

		C_SYNTHESIZE(SuperFX*, _breakfx1, Breakfx1);
		C_SYNTHESIZE(SuperFX*, _breakfx2, Breakfx2);

		C_SYNTHESIZE_READONLY(BubbleState, _state, State);
		C_SYNTHESIZE_READONLY(Symbol*, _symbol, Symbol);
		C_SYNTHESIZE(float, _symbolMinAlpha, SymbolMinAlpha);
		C_SYNTHESIZE(float, _symbolMaxAlpha, SymbolMaxAlpha);

	public:

		Bubble(int index);
		~Bubble();

		void CreateSymbol(SceneData data, int id);
		void ActivateWith(float Delay);
		void Distroy();
		bool OnClick();
		void OnMousOver();
		void setIsMatched(bool m);
		void Reset();
		void InitForShuffling(CPoint newPos);
		bool getIsComplete();
		void setBackToActive();
		void SetAlpha(float alpha);
	private:

		void Update(float ds);

	};

	enum Result
	{
		Result_NotMatch = 0,
		Result_Match,
		Result_Nothing
	};

	class PZSceneInterface
	{
	public:

		virtual void OnLevelComplete(int currentLevel) = 0;
	};

	class PZBubbleScene : public CHOScene
	{
		CObject* _mask;
		float _shuffleDelay;

		std::vector<SceneData> _symbolsData;
		std::vector<Bubble*> _bubbles;
		std::vector<CPoint> _bubblePosList;
		std::vector<Bubble*> _selectedBubbles;
		std::vector<Bubble*> _prevSelectedBubbles;

		SuperFX *s2BubbleBreakfx1FX, *s2BubbleBreakfx2FX;

		PZLevel _currentLevel; 
		C_SYNTHESIZE(PZSceneInterface*, _delegate, Delegate);

	public:

		PZBubbleScene();
		~PZBubbleScene();

		void Load(const std::string& sceneStr);
		void update(float ds);
		void LoadLevel(int level);

	private:

		CObject* CreateObject(SceneData data);
		void ShuffleBubbles(bool tween);
		void HandleComplete();
	};
}
#endif