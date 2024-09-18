

#ifndef DQFYLH_L7_S2_PUZZLE_EYE_DEF
#define DQFYLH_L7_S2_PUZZLE_EYE_DEF

//====================================
//  created by : Jithin
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include <vector>
#include <stack>

namespace Level7_Scene2
{
	class EyePartDelegate
	{
	public:

		virtual void onSelected(CObject *obj) = 0;
		virtual void onRotateCompleted(CObject* obj) = 0;
	};

	class EyePart : public CObject
	{
		int _nextAngle;
		bool _isSelected;
		bool _isMouseIn;
		EyePart* _exchangeWith;

		C_SYNTHESIZE(int, _index, Index);
		C_SYNTHESIZE(CObject*, _mouseOverGlow, MouseOverGlow);
		C_SYNTHESIZE(EyePartDelegate*, _delegate, Delegate);

		C_SYNTHESIZE(EyePart*, _next, Next);

	public:

		EyePart(CObject* obj);
		~EyePart();

		void ExchangeWith(EyePart* Part);
		void UpdatePart(float angle);
		void SetRotate(float rotate);
		void SetPivot( float pX, float pY );
		void Reset(bool isAll = true);

	private:
		
		void Swap(EyePart *y);
		void OnTweenCompleted(CObject* obj);
	};

	class EyePZController : public EyePartDelegate
	{
		float _resetTimer;

		std::vector<EyePart*> _eyeParts;
		std::vector<EyePart*> _clickedObj;
		
		C_SYNTHESIZE_READONLY(bool, _isSolved, IsSolved);
		C_SYNTHESIZE_READONLY(int, _finalRotation, FinalRotation);

	public:

		EyePZController();
		~EyePZController();

		void InitPuzzle(CHOScene* scene);
		void Update();
		void FadeOutParts();
		void DisableParts();

	private:

		void LoadSavedPos();
		void onSelected(CObject *obj);
		void onRotateCompleted(CObject* obj);
		void CheckIsCompleted();
	};

}

enum L7S2_EyePZState
{
	L7S2PZEye_Start = 0,
	L7S2PZEye_Playing,
	L7S2PZEye_Skipping,
	L7S2PZEye_Solved = 60,
	L7S2PZEye_OpenEye = 120,
	L7S2PZEye_OpeningEye,
	L7S2PZEye_WaitForCollectDisc,
	L7S2PZEye_DiscCollected,
	L7S2PZEye_WaitForCollectStatue,
	L7S2PZEye_StatueCollected,
	L7S2PZEye_Completed,
	L7S2PZEye_ExtrasModeComplete
};

class L7_S2_PZEyePuzzle : public HiddenObjectSystem
{
	CObject* _eyeFull;
	CObject* _statue;
	CObject* _device;

	int _pzState;
	std::stack<int> _stateMachine;
	Level7_Scene2::EyePZController* _pzController;
	
	SuperFX *s2pzEyeDestFX;


public:
	L7_S2_PZEyePuzzle();
	~L7_S2_PZEyePuzzle();
	float musicFadeOutTimer;

	void Update();
	void Required();
	void NavigateToScene();

protected:
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
	void CheckOnLoad();
};

#endif
