

#ifndef DQFYLH_L6S3_PATHWAY_PZ_DEF
#define DQFYLH_L6S3_PATHWAY_PZ_DEF

//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include <vector>

typedef enum 
{
	L6S3_KState_WaitToStart = 100,
	L6S3_KState_Playing,
	L6S3_KState_Snapping,
	L6S3_KState_Skip,
	L6S3_KState_AutoComplete,
	L6S3_KState_Won,
	L6S3_KState_Complete,
	L6S3_KState_ExtrasComplete
}L6S3_BridgePuzzleState;


class L6_S3_PZPathway : public HiddenObjectSystem
{
	int _pzState;
	CPoint _ringCenter;
	std::vector<CObject*> _rings;
	bool _isPuzzleSolved;
	float musicFadeOutTimer;
	void ShuffleRings();

	CObject *ResetBtn;
	CHOScene *ResetDia;
	bool IsResetPop;

	float timer;
	bool isFadeIn;
	bool isFadeOut;
	bool isResetDiaFadeIn;
	bool isResetDiaFadeOut;

	
	// Rotate animation
	float rotateTimer;
	float startAngle;
	float endAngle;
	CObject *_ringToRoate;
	CPoint _angleStartPos;
	float _startAngle, _ringAngle;

private:
	//Normal Game play
	void InitRings();
	void RotateRing(int index);
	void OnRotateComplete();
	void OnPuzzleComplete();
	void ResetPuzzle();
	// On skip
	void StartAutoCompletePuzzle();
	void RotateRingToZero(int index);
	float easing(float t,float b,float c,float d);

public:
	L6_S3_PZPathway();
	~L6_S3_PZPathway();

	void Update();
	void Required();
	void NavigateToScene();

protected:

	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
};
#endif
