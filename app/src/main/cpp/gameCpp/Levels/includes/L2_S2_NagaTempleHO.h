//====================================
// DQFYLH
// Hidden Object System
// Level 2 - Scene 02 : Naga Temple HO
//====================================

#ifndef DQFYLH_LEVEL2_SCENE2_NAGATEMPLEHO_DEF
#define DQFYLH_LEVEL2_SCENE2_NAGATEMPLEHO_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "SuperFX.h"
#include "Match3.h"
#include "MeshObject.h"

class CHOScene;

#define MAX_L2S2_HOOBJECTS 10

class L2S2HiddenObject : public CObject
{
	
	bool _isAnimComplete;
	float hoFlyingTimer, hoScaleTimer;
	bool _isObjFlying;
	
	CPoint *_startPoint;
	CPoint *_endPoint;
	CPoint *_ctrlPoint;	

	SuperFX *HOTrailFX;
	SuperFX *hoClickFX;
	bool _isObjectScaling;
	int FXTimer;

	std::function<void (CObject*)> _flownToHUDCallback;
	
	C_SYNTHESIZE(int, _objID, ObjectID);
	C_SYNTHESIZE(CObject*, _inHudObj, InHudObj);
	C_SYNTHESIZE(CObject*, _flyingObj, FlyingObj);
	C_SYNTHESIZE(CObject*, _scnObj, SceneObject);
	C_SYNTHESIZE(CPolygon* ,_polygonArea, PolygonArea);
	C_SYNTHESIZE_READONLY(bool, _isFound, IsFound);
	C_SYNTHESIZE_READONLY(bool, _isActive, IsActive);

public:

	L2S2HiddenObject();
	~L2S2HiddenObject();

	void InitObject(std::function<void (CObject*)> flownToHUDCallback);
	bool MouseClicked(CPoint pos);
	void Update(float ds);
	void AlreadyFound();

	void SwitchToMatch3();
	void SwitchToHO();

private:

	void OnFound();
};


class L2_S2_NagaTempleHO: public HiddenObjectSystem
{
	public:
		CPolygon *polyStatueInfo;
		float musicFadeOutTimer;
		MeshObject *FgPlantMesh;

		SuperFX *HORightVFX, *HOLeftVFX, *HOEndVFX;

		//Scene Elements
		CPolygon *HOPolyArray[MAX_L2S2_HOOBJECTS];
		std::vector<L2S2HiddenObject*> _hoList;
		std::vector<L2S2HiddenObject*> _founfObjs;

		float animPartsFadeInTime;
		float animPartJoinTimer;
		bool IsAnimPartsFadingIn;
		bool IsAnimPartsJoining;
		
		bool IsHeadMovingToFix;
		float headMovingTimer;
	CObject *match3Back, *match3BackText;


	CObject *NagaHeadFull;
		CObject *NagaHeadFixed;

		CObject *NagaHeadParts[8];
		CPoint  *NagaPartStartPt[8];
		CPoint  *NagaPartCtrlPt[8];
		CPoint  *NagaPartEndPt[8];
		float NagaPartStartScale[8];

		CPoint *BzStartPt, *BzEndPt, *BzCtrlPt;
		bool isHOCompleted ;
		void HandleHOComplete();

		//Match 3 code
		Match3 *match3Ctrl;		

		CObject *BtnMatch3;
		CObject *BtnSwitchToHO;
		//Match 3

		CObject *ResetBtn;
		CHOScene *ResetDia;
		bool IsResetPop;

		float timer;
		bool isFadeIn;
		bool isFadeOut;
		bool isResetDiaFadeIn;
		bool isResetDiaFadeOut;

		float easing(float t,float b,float c,float d);

		L2_S2_NagaTempleHO();
		~L2_S2_NagaTempleHO();

		void Update();
		void Required();
		void ResetHO();

		void NavigateToScene();

	protected:

		void ReceiveMessage(int val);
		void ProcessNonHOHint();
		void OnObjFlyComplete(CObject *obj);
};

#endif