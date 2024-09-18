//====================================
// DQFULP
// Hidden Object System
// Author : Sreeraj
// LEVEL3 - SCENE3 : P1Room6Pillar
//====================================

#ifndef DQFYLH_LEVEL3_SCENE3_P1ROOM6PILLAR_DEF
#define DQFYLH_LEVEL3_SCENE3_P1ROOM6PILLAR_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include <queue>
#include <stack>
#include "CVideo.h"
#include "AssetsStringTable.h"
class CHOScene;

using namespace std;

enum PillerState
{
	Piller_Top = 398,
	Piller_Middle = 505,
	Piller_Bottom = 587
};

enum BoxToPick
{
	NO_BOX = -1,
	LEFT_BOX = 0,
	RIGHT_BOX = 1
};

enum PuzzleState
{
	M6_Scene_Enter = 0,
	M6_Scene_LasiaAttack,
	M6_Scene_AfterLasiaAttack,
	M6_Scene_BrakingCrystal,
	M6_Scene_BreakingCrystalComplete,
	M6_Puzzle_Start,
	M6_Puzzle_Playing,
	M6_Puzzle_Onvdo,
	M6_Puzzle_Onvdo_End,
	M6_Puzzle_Scene_Left,
	M6_Puzzle_Scene_Right,
	M6_Puzzle_WaitForCollectInv,
	M6_Puzzle_OnBoxCollected = 60,
	M6_Puzzle_Complete
};

static PillerState sol[2][3] = {{Piller_Top, Piller_Middle, Piller_Bottom}, {Piller_Bottom, Piller_Middle, Piller_Top}};

class PuzzleSolution
{
public:
	
	static BoxToPick getSolution(vector<PillerState> pillerPos)
	{
		BoxToPick retVal = NO_BOX;

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 3; j++)	
			{
				if (sol[i][j] != pillerPos[j])
				{
					retVal = NO_BOX;
					break;
				}

				retVal = BoxToPick(i);
			}

			if (retVal != NO_BOX)
				break;
		}

		return retVal;
	}

	static int hint_GetNearestBoxIndex(vector<PillerState> pillerPos)
	{
		for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 3; j+=2)
				{
					if (sol[i][j] == pillerPos[j])
					{
						return i;
					}
				}
			}

		return 0;
	}

	static int hint_GetWheelIndex(vector<PillerState> pillerPos, vector<BoxToPick> collectedBoxes)
	{
		if (collectedBoxes.size() != 0)
		{
			for (int k = 0; k  < 3; k ++)
			{
				if (sol[!collectedBoxes[0]][k] != pillerPos[k])
				{
					return k;
				}
			}
		}
		else
		{
			int i = hint_GetNearestBoxIndex(pillerPos);
			
			for (int j = 0; j < 3; j++)
			{
				if (sol[i][j] != pillerPos[j])
				{
					return j;
				}
			}
		}

		return 0;
	}
};

class PillarWheelInterface
{
public:

	virtual void OnWheelStartRotate(CObject *obj) = 0;
	virtual void OnWheelEndRotate(CObject *obj) = 0;
};


class PillarWheel : public CObject
{
	bool _isWheelRotating;
	float _currentAngle;
	float _animTimer;

	PillerState _pillerNextState;

	C_SYNTHESIZE_READONLY(PillerState, _pillerState, CurrentPillerPos);
	C_SYNTHESIZE(SuperFX *, _pillerDustFX, PillerDustFX);
	C_SYNTHESIZE(PillarWheelInterface*, _interface, Interface);
	C_SYNTHESIZE(CObject*, _piller, Piller);
	C_SYNTHESIZE(queue<PillerState>, _pillerStates, PillerPossibleStates);
	C_SYNTHESIZE_READONLY(bool, _cursorOnWheel, CursorOnWheel);
	C_SYNTHESIZE(bool, _isWheelDisabled, WheelDisabled);

protected:

	void OnMouseOver();
	void OnMouseClick();
	void OnMouseOut();

	void Update(float ds);

public:

	PillarWheel ();
	~PillarWheel ();

	void SetPillerState(PillerState state);

private:

	void Init();
};

class L3_S2_P1Room6Pillar: public HiddenObjectSystem, CRenderListEntity, public PillarWheelInterface
{
	PillarWheel *pillarWheels[3];
	int _puzzleState;
	std::stack<int> _stateMachine;
	float musicFadeOutTimer;
	PuzzleState _stateOnVdoEnd[2];

	CVideo *_vdo_LasiaAttack, *_vdo_CrystalBreak;
	float vidTimer;
	float vidTimer2;

	float time;
	int subIndex;
	subText subtitles[3];
	CBitmapText* subtitleText; int subMax;

	vector<PillarWheel*> _PillarWheels;
	SuperFX *M6LighitParticleFX;

	BoxToPick _enabledBox;
	vector<BoxToPick> _collectedBoxes;
	vector<CVideo*> _moveToBoxVdos;
	vector<CObject*> _boxObjects;
	vector<CObject*> _boxGlowObjects;
	vector<string> _boxPickSceneFile;

	CHOScene *boxPickScene;
	CObject	 *_boxOnScene; 

	CObject *ResetBtn;
	CHOScene *ResetDia;
	bool IsResetPop;

	float timer;
	bool isFadeIn;
	bool isFadeOut;
	bool isResetDiaFadeIn;
	bool isResetDiaFadeOut;

	CObject *_overlap;

	public:
		//Scene Elements
		
		//--------------------------- Functions -------------------------------//
		L3_S2_P1Room6Pillar();
		~L3_S2_P1Room6Pillar();
		
		//_PUZZLE
		void InitPuzzle();
		
		float easing(float t,float b,float c,float d);
		
		//Inherit this CRenderListEntity function to render anything to scene!!
		void Render(const CPoint* pCam);
		//_PUZZLE
		void ProcessSkip();
		void Update();
		void Required();

		void TaskPendingUpdate();

		void NavigateToScene();

	protected:

		void ReceiveMessage(int val);
		void ProcessNonHOHint();

		void OnWheelStartRotate(CObject *obj);
		void OnWheelEndRotate(CObject *obj);

	private:

		void SparkleUpdate();
		void InitSceneProperties();
		void CheckCursorMode();
		void CollectBox();
		void OnVideoEnd();
		void OnBoxCollect();
		void ResetPuzzle();
};

#endif