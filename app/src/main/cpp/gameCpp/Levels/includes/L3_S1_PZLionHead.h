//====================================
// DQFYLH
// Hidden Object System
// Level 2 - Scene 04 : CAVE
//====================================

#ifndef DQFYLH_LEVEL3_SCENE1_LIONHEAD_PZ
#define DQFYLH_LEVEL3_SCENE1_LIONHEAD_PZ

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include <vector>

using namespace std;

class CHOScene;

typedef enum 
{
	KState_Initialize = 1000,
	KState_WaitToStart,
	KState_Playing,
	KState_Snapping,
	KState_Skip,
	KState_AutoComplete,
	KState_Won,
	KState_OpenDoor = 1120,
	KState_Complete,
	KState_GemCollected,
	KState_L3S1CompleteInExtras
}L3_S1_PuzzleState;

class L3_S1_PZLionHead: public HiddenObjectSystem, CRenderListEntity
{
	int _pzState;
	CPoint _ringCenter;
	vector<CObject*> _rings;
	vector<float> _ringRadious;
	CObject *_ringOpen;
	CObject *_gem;
	CObject *_gemGlow;
	SuperFX *GemParticlesFX;
	SuperFX *P1LeafsFX;
	SuperFX *s1RoundPZCompleteFX;
	float musicFadeOutTimer;
	bool _isPuzzleSolved;
	CPoint _angleStartPos;
	float _startAngle, _ringAngle;

	void ShuffleRings();
	
	// Rotate animation
	float rotateTimer;
	float startAngle;
	float endAngle;
	CObject *_ringToRoate;

	//Normal Game play
	void InitRings();
	void RotateRing(int index);
	void OnRotateComplete();
	void OnPuzzleComplete();

	// On skip
	void StartAutoCompletePuzzle();
	void RotateRingToZero(int index);

	public:

		//Scene Elements
	

		L3_S1_PZLionHead();
		~L3_S1_PZLionHead();

		//Inherit this CRenderListEntity function to render anything to scene!! 
		void Render(const CPoint* pCam);
		void Update();
		void Required();

		float easing(float t,float b,float c,float d);
		void NavigateToScene();

	protected:

		void ReceiveMessage(int val);
		void ProcessSkip();
		void ProcessNonHOHint();
};

#endif