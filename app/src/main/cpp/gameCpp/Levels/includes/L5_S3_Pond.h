

#ifndef DQFYLH_L5_S3_POND_DEF
#define DQFYLH_L5_S3_POND_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Popup.h"
#include "PopupInterface.h"
#include <map>
#include "CVideo.h"
#include "L5S3_Popup2Stone.h"

enum eL5S3PopEnums
{
	ePop1_L5S3Pop1_BowVine,
	ePop2_L5S3Pop2_Stone,
	ePop3_L5S3_ToUnderWater,
	ePopL5S3_Max
};

class L5_S3_Pond : public HiddenObjectSystem, public PopupInterface
{
	float musicFadeOutTimer;
	SuperFX *S3PoolWaterFallFX;
	SuperFX *S3PoolWaterFallBottomFX;
	SuperFX *S3CrystalParticle1FX, *S3CrystalParticle2FX, *S3CrystalParticle3FX, *S3CrystalParticle4FX;
	L5S3_PopStone *stonePopup;
	CObject* Stone1;
	CObject* Bstone;
	CObject* vineTree_2;

	CVideo* _cvdo_introSnake;
	float time;
	float vidTimer;
	int subIndex;
	subText subtitles[3];
	CBitmapText* subtitleText; int subMax;
	bool _startPlayingSnake;
	
	SuperFX *s3waterRippleFX, *s3waterdropsFX;
	JSONAnimator* _jAnim_DounganDoorOPen;
	map<eL5S3PopEnums, Popup*>	PopupArray;

	SuperFX *hodotsFX, *holinesFX;

public:
	//Popups
	int ActivePopupID;

	void OnPopupClosed(Popup* popup);

	L5_S3_Pond();
	~L5_S3_Pond();

	void SparkleUpdate();
	void TaskPendingUpdate();

	void InitPopups();
	void Update();
	void Required();

	void SceneObjectsUpdate();
	void NavigateToScene();

protected:
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();

	void UpdateHOSystem(Popup *popup, int UpdateVal = -1);
	void NewArea();
};

#endif
