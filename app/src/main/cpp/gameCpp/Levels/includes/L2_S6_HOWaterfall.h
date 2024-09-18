//====================================
// DQFYLH
// Hidden Object System
// Level 2 - Scene 06 : Waterfall HO
//====================================

#ifndef DQFYLH_LEVEL2_SCENE6_WATERFALLHO_DEF
#define DQFYLH_LEVEL2_SCENE6_WATERFALLHO_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "SuperFX.h"
#include "Match3.h"
#include "MeshObject.h"
#include "BitmapText.h"

class CHOScene;

#define MAX_L2S6_HOOBJECTS 22

class L2S6HiddenObject : public CObject
{
	
	bool _isAnimComplete;
	float hoScaleTimer, hoFlyingTimer;
	bool _isObjectScaling;
	
	CPoint *_startPoint;
	CPoint *_endPoint;
	CPoint *_ctrlPoint;
	CPoint _objectPos;

	SuperFX *HOTrailFX;
	SuperFX *waterHOFX;
	int FXTimer;

	C_SYNTHESIZE(int, _index, Index);	
	C_SYNTHESIZE(CBitmapText*, _nameText, NameText);
	C_SYNTHESIZE(CObject*, _scnObj, SceneObject);
	C_SYNTHESIZE(CObject*, _flyObj, FlyingObject);
	C_SYNTHESIZE(CPolygon* ,_polygonArea, PolygonArea);
	C_SYNTHESIZE_READONLY(bool, _isFound, IsFound);
	C_SYNTHESIZE_READONLY(bool, _isActive, IsActive);

public:

	L2S6HiddenObject();
	~L2S6HiddenObject();

	void InitObject();
	bool MouseClicked(CPoint pos);
	void Update(float ds);
	void AlreadyFound();

	void SwitchToMatch3();
	void SwitchToHO();

	void SetActive(bool f){
		_isActive = f;
		_nameText->Show(f);
	}

private:

	void OnFound();
};

class L2_S6_HOWaterfall: public HiddenObjectSystem
{
	public:
		float musicFadeOutTimer;
		MeshObject *FgPlantMesh;

		//Scene Elements
		CPolygon *HOPolyArray[MAX_L2S6_HOOBJECTS];
		CBitmapText *HOInvboxTexts[MAX_L2S6_HOOBJECTS];
		
		L2S6HiddenObject *HOObjects[MAX_L2S6_HOOBJECTS];

		int foundItemsCount;
		bool isHOCompleted;
		
		SuperFX *waterHOFliesFX, *waterHODropsFX, *HOInvBoxFX;

		bool isHOInvTextsInitialized;
		void InitHOInventoryTexts(bool showChangeFX=false);
		void CheckHOComplete();
		void HandleHOComplete();

		float lastObjDelayTimer;

		//Match 3 code
		Match3 *match3Ctrl;
	CObject *match3Back, *match3BackText;

		CObject *BtnMatch3;
		CObject *BtnSwitchToHO;
		//Match 3

		L2_S6_HOWaterfall();
		~L2_S6_HOWaterfall();

		void Update();
		void Required();

		void NavigateToScene();

	protected:

		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif