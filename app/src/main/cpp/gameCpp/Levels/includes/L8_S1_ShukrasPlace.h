//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL8 - SCENE1 : SHUKRAS PLACE
//====================================

#ifndef DQFYLH_LEVEL8_SCENE1_SHUKRAS_PLACE_DEF
#define DQFYLH_LEVEL8_SCENE1_SHUKRAS_PLACE_DEF

#include "HiddenObjectSystem.h"
#include "ClickablePolygon.h"
#include "L8S1_Pop1MagicWand.h"
#include "L8S1_Pop2HiddenCompartment.h"
#include "L8S1_Pop3DoorPopup.h"
#include "MeshObject.h"
#include "SuperFX.h"
#include "CVideo.h"
#include "AssetsStringTable.h"

class FlyingDot
{
private:
	float _radius, _angleDeg, _speed;
	SuperFX *_trailsFX;

	float _minHideAngle, _maxHideAngle;
	bool _isClockwise;
	CPoint *_centerPt;

	C_SYNTHESIZE(CObject*, _dotObj, dotObject);
public:

	FlyingDot();
	~FlyingDot();

	void SetProperties(float centerX, float centerY, float radius, float speed, bool isClockwise, float initAngle = 0);
	void update(float ds);
};

#define MAX_L8S1_FLYING_DOTS 6

class L8_S1_ShukrasPlace: public HiddenObjectSystem, CRenderListEntity
{
	CHOScene *sceneMagicGlass;
	bool isGlassInHand;
	CVideo *lpIntroCutscene;
	float vidTimer;
	float time;
	int subIndex;
	int subMax;
	subText subtitles[5];
	CBitmapText* subtitleText;
	bool isVideoPlaying;
	float musicFadeOutTimer;
	CPolygon *globPoly;
	SuperFX *holinesFX, *hodotsFX;

	CObject *symbols[MAX_L8_S1_MATCHING_SYMBOLS_COUNT];
	CObject *redSymbols[MAX_L8_S1_MATCHING_SYMBOLS_COUNT];
	CObject *silhouttes[(MAX_L8_S1_MATCHING_SYMBOLS_COUNT / 2) + 1];
	int selectedSymbol;
	public:

		FlyingDot *flyingDots[MAX_L8S1_FLYING_DOTS];

		//popups
		std::vector<ClickablePolygon*> PopRectArray;
		std::vector<Popup*> PopupArray;

		int ActivePopUpId;
		//SuperFX *GlobeGlowParticle;
		MeshObject *mo_flag1, *mo_flag2, *mo_flag3, *mo_flag4;

		SuperFX *s1sidewindowraysFX, *s1doorraysFX, *s1lightflyFX, *s1lightfly2FX, *s1goldglitterFX;

		//--------------------------- Functions -------------------------------//
		L8_S1_ShukrasPlace();
		~L8_S1_ShukrasPlace();

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
		L8S1_Pop3DoorPopup *pop3;
	protected:
		void ProcessSkip();
		void ReceiveMessage(int val);
		void ProcessNonHOHint();
		void SparkleUpdate();
		void CheckForPendingTask();
};

#endif