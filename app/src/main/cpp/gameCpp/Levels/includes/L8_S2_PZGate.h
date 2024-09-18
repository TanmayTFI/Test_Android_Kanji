

#ifndef DQFYLH_LEVEL8_SCENE2_PZ_GATE_DEF
#define DQFYLH_LEVEL8_SCENE2_PZ_GATE_DEF

//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "JSONAnimator.h"

enum L8S2GateSymbolState
{
	eGateSymbolState_Idle = 0,
	eGateSymbolState_Clicked,
	eGateSymbolState_Swapping,
	eGateSymbolState_Skipping
};

class GateSymbol: public  CObject
{
private :

	int _id;
	int _pos;
	int _nextPos;
	bool _isSelected;
	L8S2GateSymbolState _state;
	CObject *_swapSymbol;
	CObject *_swapBgObject;
	
	int _angleIncrement;
	int _angleDiff;
	int _direction;
	std::function<void (int)> _SwapCompleteCallback;

public:

	CObject *glowObject;
	CObject *gateSymObj;
	CObject *bgObject;
	GateSymbol(int id, int pos, CObject *symbol, CObject *bgSymbol, CObject *glow);
	~GateSymbol();

	void SetHoverEffect(bool glowFlag);
	void SetGlow(bool glowFlag);
	void SwapSymbol(int swapId, int swapPos, CObject *swapSymbol, CObject *swapBgObject);
	void SwapSymbol(int swapId, int swapPos, CObject *swapSymbol, CObject *swapBgObject, std::function<void (int)> SwapCompleteCallback);
	void SetSymbolRotate( float rotation);
	void OnSkipActive(int swapId, int swapPos, std::function<void (int)> SwapCompleteCallback);
	int GetPositionIndex();
	CRectangle GetIntersectArea();
	int  GetId();
	bool CheckIfSelected();
	void SetEnable(bool enableFlag);
	void SetId(int Id);
	void UpdateSymbol();
	void DisableSymbol();
	void OnSwapComplete();
	void UpdateSymbolPos(CPoint symbolPos);
	CPoint GetCenter();
	void SetPositionIndex(int pos);
	CPoint GetOrigin();
	void SetOrigin(float x, float y);
	CObject GetSymbolObject();
	void SetPosition(int posIndex, CPoint pos, CPoint symbolPos);
	L8S2GateSymbolState GetState();
	float GetRotate();
	void SetState(L8S2GateSymbolState state );
};

class GateSymbolManager: public CObject
{
private :
	int symbolSelectionCount;
	int Selectedsymbols[2];
	int SkipCount;
	CObject *_glowSmall[10];
	CObject *_glowBig[10];
	CObject	*_lock;
	CObject *_mergedObject;
	CObject *_frame;
	bool	_isSwapping;
public :
	bool IsSwapping();
	int	 RandomArray[10];
	bool isSolved;
	std::vector<GateSymbol*> _SymbolArray;
	GateSymbolManager(CHOScene* scene);
	~GateSymbolManager();
	void InitGateSymbols();
	void SetGateSymbolsActive();
	void UpdateSymbols();
	void ToggleHoverEffect(int id,bool glowFlag);
	void OnSymbolSelected(int id);
	void ShuffleGateSymbols();
	void AutoSolvePuzzle();
	void OnSwapComplete(int id);
	void ShuffleSymbols(int shuffleIndex1, int shuffleIndex2);
	bool CheckIfPuzzleComplete();
};


class L8_S2_PZGate : public HiddenObjectSystem
{
public:
	L8_S2_PZGate();
	~L8_S2_PZGate();
	
	void Update();
	void Required();
	//std::vector<GateSymbol*> _SymbolArray;
	GateSymbolManager *SymbolManager;
	void NavigateToScene();

protected:
	SuperFX *s2_PzGateCompleteFX;
	SuperFX *S2_PzGateGlowFX;
	CObject *Key;
	bool isAnimationPlaying;
	JSONAnimator *GateOpenAnim, *KeyTurnAnim;
	int		SelectedSymbol;
	int		PrevSymbol;
	float	angleDiff;
	bool	isSkipActive;
	bool	isPuzzleSolved;
	float	angleIncrement;
	CPolygon  *SymbolPoly[10];
	int		SwapIndex[2], RandomArray[10];
	int		SwapCount;
	void SwapSymbols(int index1, int index2);
	void CheckForPendingTask();
	void SparkleUpdate();
	void TaskPendingUpdate();
	void InitSceneElements();
	void OnAnimationComplete(int index);
	void InitControlPolys();
	void OnSwapComplete(CObject* obj);
	void UpdateSceneElements(CPoint pos);
	void UpdateSymbolState(int id);
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
	bool CheckIfPuzzleComplete();
	void OnPuzzleComplete();
	void OpenGate();
};

#endif
