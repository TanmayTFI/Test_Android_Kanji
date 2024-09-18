//====================================
// DQFYLH
// Hidden Object System
// Level 2 - Scene 04 : CAVE
//====================================

#ifndef DQFYLH_LEVEL3_SCENE4_FORTFOUNTAINHO
#define DQFYLH_LEVEL3_SCENE4_FORTFOUNTAINHO

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include <vector>
#include "GridPuzzle.h"
#include "Match3.h"

using namespace std;
using namespace gridpuzzle;

class CHOScene;

class L3S4HiddenObject : public CObject
{
	bool _isAnimComplete;
	float hoScaleTimer, hoFlyingTimer;
	bool _isObjectScaling;
	
	CPoint *_startPoint;
	CPoint *_endPoint;
	CPoint *_ctrlPoint1;
	CPoint *_ctrlPoint2;
	CPoint _objectPos;

	SuperFX *HOTrailFX;
	SuperFX *waterHOFX;
	int FXTimer;

	C_SYNTHESIZE(int, _index, Index);
	C_SYNTHESIZE(string, _name, Name);
	C_SYNTHESIZE(CBitmapText*, _nameText, NameText);
	C_SYNTHESIZE(CObject*, _toFind, ToFind);
	C_SYNTHESIZE(CObject*, _completeObject, CompleteObject);
	C_SYNTHESIZE(CPolygon* ,_polygonArea, PolygonArea);
	C_SYNTHESIZE_READONLY(bool, _isFound, IsFound);

public:

	L3S4HiddenObject();
	~L3S4HiddenObject();

	void InitObject();
	bool MouseClicked(CPoint pos);
	void Update();
	void AlreadyFound();

	void SwitchMatch3();
	void SwitchHO();

private:

	void OnFound();
	void InitInvNameText();
};

typedef enum 
{
	State_StartMatch3 = 0,
	State_StopMatch3,
	State_Initilizing = 1000,
	State_GameStarted,
	State_WaitForInteraction,
	State_OnInteraction,
	State_OnHint,
	State_FoundObject,
	State_Match3Completed,
	State_FoundAllObjects,
	State_HOCompleted = 1500,
	State_GameComplete,
	State_L3S4GameInExtrasComplete
}GameState;

class L3_S4FortFoundationHO: public HiddenObjectSystem, CRenderListEntity
{
	CPolygon* HOPolygonArray[16];
	vector<L3S4HiddenObject*> _hiddenObjs;
	vector<L3S4HiddenObject*> _foundObjs;
	int _gameState;
	float musicFadeOutTimer;
	CObject* _btnMatch3;
	CObject* _btnSwitchToHO;

	SuperFX *S3FortFountaionHOLightFX, *S3FortFountaionHOLeafFX;

	Match3 *_match3Ctrl;	

public:
	CObject *match3Back, *match3BackText;

	//Scene Elements

	L3_S4FortFoundationHO();
	~L3_S4FortFoundationHO();

	//Inherit this CRenderListEntity function to render anything to scene!! 
	void Render(const CPoint* pCam);
	void Update();
	void Required();

	float easing(float t,float b,float c,float d);
	void NavigateToScene();

protected:

	void ReceiveMessage(int val);
	void ProcessNonHOHint();

private:
	void InitPolygonArray();
	void InitHO();
};

#endif