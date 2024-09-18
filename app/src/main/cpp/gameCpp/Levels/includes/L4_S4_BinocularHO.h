//====================================
// DQFYLH
// Hidden Object System
// Level 4 - Scene 04 : HILL
//====================================

#ifndef DQFYLH_LEVEL4_SCENE4_BINOCULAR_HO
#define DQFYLH_LEVEL4_SCENE4_BINOCULAR_HO

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include <vector>
#include "GridPuzzle.h"
#include "ClickablePolygon.h"

using namespace std;
class CHOScene;

#define NUMBER_OF_HO 6


class L4_S4_BinocularHO: public HiddenObjectSystem, CRenderListEntity
{
	CPolygon* HOPolygonArray[NUMBER_OF_HO];
	vector<CObject*> _hiddenObjs;
	int isFound[NUMBER_OF_HO];

	CBitmapText *_hintText;

	CObject * _compassNeedle;
	CObject * _compassBase;
	//int _counter;


public:

	int X_LOWER_LIMIT;
	int X_UPPER_LIMIT;
	int Y_LOWER_LIMIT;
	int Y_UPPER_LIMIT;

	float musicFadeOutTimer;
	//Scene Elements
	CHOScene *BgScn[9];
	bool isMovingBinocular;
	ClickablePolygon * sophiaClickArea;
	int numObjectsFound;
	bool wasPrevMouseDown;
	bool isFoundSophia;
	CObject *OutLineSymbols[NUMBER_OF_HO];
	CObject *BlueOutLineSymbols[NUMBER_OF_HO];
    KEvent *eventHandler;
	float FoundSophiaTimer;

	CPoint currentPos,prevPos, originPos, _pos;
	ClickablePolygon *BinocularClickPoly;
	float mouseDx, mouseDy;
	float targetX, targetY;
	bool LBPressed, isTouchScreen;

	L4_S4_BinocularHO();
	~L4_S4_BinocularHO();

	//Inherit this CRenderListEntity function to render anything to scene!! 
	void Render(const CPoint* pCam);
	void Update();
	void Required();

	float easing(float t,float b,float c,float d);

	void OnObjectFadeInComplete(CObject * obj);
	void OnObjectFadeOutComplete(CObject * obj);
	void OnSophiaPolygonClick(int id);
	void OnFoundSophia();

	void NavigateToScene();

protected:

	void ReceiveMessage(int val);
	void ProcessNonHOHint();

private:
	void InitPolygonArray();
};

#endif