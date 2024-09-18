//====================================
// DQFYLH
// Hidden Object System
// Level 4 - Scene 01 : SWAMP ENTRANCE
//====================================

#ifndef DQFYLH_LEVEL4_SCENE1_SHOE_HO
#define DQFYLH_LEVEL4_SCENE1_SHOE_HO

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include <vector>
#include "GridPuzzle.h"
#include "MeshObject.h"
#include "Match3.h"

using namespace std;
class CHOScene;

#define NUMBER_OF_HO_SHOE 12

class L4_S1_HOObject
{
	CBitmapText *_hintText;

	C_SYNTHESIZE(CObject*, _hiddenObject, HiddenObject);
	C_SYNTHESIZE(CObject*, _mouseOverObject, MouseOverObject);
	C_SYNTHESIZE(int, _id, ID);
	C_SYNTHESIZE(string, _nameText, NameText);
	C_SYNTHESIZE(string, _descText, DescriptionText);
	//C_SYNTHESIZE(CPolygon* ,_polygonArea, PolygonArea);
	C_SYNTHESIZE(bool, _isFound, IsFound);
public:
	L4_S1_HOObject();
	~L4_S1_HOObject();

	void InitObject();
	void ShowHintText();
	void HideHintText();
	void OnMouseOverHOObject(CObject *obj);
	void OnMouseOutHOObject(CObject *obj);
	bool IsMouseClicked(CPoint pos);
};

class L4_S1_ShoeHO: public HiddenObjectSystem, CRenderListEntity
{
	vector<L4_S1_HOObject*> _hiddenObjs;
	vector<L4_S1_HOObject*> _foundObjs;

	MeshObject *mo_Water;

	int ObjToFind;
	bool isHandleInHand;

public:
	float musicFadeOutTimer;
	//Scene Elements
	CObject *HiddenObjects[NUMBER_OF_HO_SHOE];
	CObject *HO_MouseOver[NUMBER_OF_HO_SHOE];
	CObject *match3Back, *match3BackText;

	L4_S1_ShoeHO();
	~L4_S1_ShoeHO();

	//Inherit this CRenderListEntity function to render anything to scene!! 
	void Render(const CPoint* pCam);
	void Update();
	void Required();

	float easing(float t,float b,float c,float d);
	void NavigateToScene();

protected:

	//Match 3 code
	Match3 *match3Ctrl;
	CObject *BtnMatch3;
	CObject *BtnSwitchToHO;
	//Match 3

	void ReceiveMessage(int val);
	void ProcessNonHOHint();

private:
};

#endif