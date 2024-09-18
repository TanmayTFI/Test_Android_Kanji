//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL8 - SCENE1 : SHUKRAS PLACE
//====================================

#ifndef DQFYLH_LEVEL8_SCENE3_BANYAN_TREE_DEF
#define DQFYLH_LEVEL8_SCENE3_BANYAN_TREE_DEF

#include "HiddenObjectSystem.h"
#include "ClickablePolygon.h"
#include "L8S3_Pop1SwordPopup.h"
#include "L8S3_Pop2HiddenCompartment.h"
#include "SuperFX.h"

class L8_S3_BanyanTree: public HiddenObjectSystem, CRenderListEntity
{
	CHOScene *sceneMagicGlass;
	bool isGlassInHand;

	CObject *symbols[MAX_L8_S1_MATCHING_SYMBOLS_COUNT];
	CObject *redSymbols[MAX_L8_S1_MATCHING_SYMBOLS_COUNT];
	CObject *silhouttes[(MAX_L8_S1_MATCHING_SYMBOLS_COUNT / 2) + 1];
	int selectedSymbol;
public:
	float musicFadeOutTimer;
	//popups
	std::vector<ClickablePolygon*> PopRectArray;
	std::vector<Popup*> PopupArray;

	int ActivePopUpId;
	CObject *backBTN, *backBTN_text, *hintBTN, *hintBTN_text;
	SuperFX *sfx_WaterFall1,*sfx_WaterFall2,*holinesFX, *hodotsFX;

	//--------------------------- Functions -------------------------------//
	L8_S3_BanyanTree();
	~L8_S3_BanyanTree();

	void InitPopups();
	void OnPopupClosed(Popup * popup);

	bool isPopupActive(int popId);

	float easing(float t,float b,float c,float d);

	void TakeInventoryItem(EHOInventory ItemID);
	//Inherit this CRenderListEntity function to render anything to scene!!
	void Render(const CPoint* pCam);
	void Update();
	void Required();
	void SceneObjectsUpdate();
	void TaskPendingUpdate();
	void InitParticles();

	void SkipMagicGlass();
	CObject *SkipBtn, *skipBTN_text;
	CPoint currentPos,prevPos, _pos;
	float mouseDx, mouseDy;
	bool LBPressed, isTouchScreen;
	void NavigateToScene();

protected:

	void ReceiveMessage(int val);
	void ProcessNonHOHint();
	void SparkleUpdate();
	void CheckForPendingTask();
};

#endif