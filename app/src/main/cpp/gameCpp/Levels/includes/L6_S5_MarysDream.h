//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL6 - SCENE5 : MARY'S DREAM
//====================================

#ifndef DQFYLH_LEVEL6_SCENE5_MARYSDREAM_DEF
#define DQFYLH_LEVEL6_SCENE5_MARYSDREAM_DEF

#include "HiddenObjectSystem.h"
#include "L6S5_Pop1Sophia.h"
#include "SuperFX.h"
#include "DialogSystem.h"
#include "MeshObject.h"

#define NUMBER_OF_HOs 10

class L6_S5_MarysDream: public HiddenObjectSystem, CRenderListEntity
{
	CBitmapText *_nameText[NUMBER_OF_HOs];
public:
	bool isCutscnPlaying;
	CVideo * _cVideoCutScene;
	float vidTimer;
	bool _hoModeActive;
	int _objectInHand;
	//popups
	std::vector<ClickablePolygon*> PopRectArray;
	std::vector<Popup*> PopupArray;
	float musicFadeOutTimer;
	std::vector<CObject*> MisplacedObjectsArray;
	//	std::vector<CObject*> OrderObjectsArray;
	CPolygon *OrderObjectPos[NUMBER_OF_HOs];
	CPolygon *ObjectRect[NUMBER_OF_HOs];
	float InitialPosX[NUMBER_OF_HOs],InitialPosY[NUMBER_OF_HOs];
	int InitialZorder[NUMBER_OF_HOs];
	SuperFX *DreamparticlesFX;
	SuperFX *S5_WindowParticlesFX;
	SuperFX *S5_LampParticlesFX;
	SuperFX *S5_HOCompleteFxFX;
	CObject *_cloth;
	CObject *_sophiaUnderBed;
	CObject * _tabletLight;
	CObject * blurBg;
	bool isNeedToBlur;
	int tablightCounter;

	int ActivePopUpId;
	//SuperFX *PlantGlowFX_L, *PlantGlowFX_R, *SnowFX_L, *SnowFX_R;

	//--------------------------- Functions -------------------------------//
	L6_S5_MarysDream();
	~L6_S5_MarysDream();


	DialogSystem *dialogSystem;
	bool isDialogActive;

	void InitInvNames();
	void OnPopupClosed(Popup * popup);

	bool isPopupActive(int popId);
	bool isSophiaPositionSet;

	float easing(float t,float b,float c,float d);

	void TakeInventoryItem(EHOInventory ItemID);
	//Inherit this CRenderListEntity function to render anything to scene!!
	void Render(const CPoint* pCam);
	void Update();
	void Required();
	void SceneObjectsUpdate();
	void TaskPendingUpdate();
	void InitParticles();
	void InitFinalPolygon();
	void InitRectPolygon();
	void ShowSophia(int _id);
	void OnFirstTweenComplete(CObject *obj);
	void OnSecondTweenComplete(CObject *obj);
	void NavigateToScene();
protected:
	void CheckForPendingTask();
	void SparkleUpdate();
	void ReceiveMessage(int val);
	void ProcessNonHOHint();
};

#endif