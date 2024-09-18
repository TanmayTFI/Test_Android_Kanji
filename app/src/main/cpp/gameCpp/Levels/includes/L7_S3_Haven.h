

#ifndef DQFYLH_L7_S3_HAVEN_DEF
#define DQFYLH_L7_S3_HAVEN_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Popup.h"
#include "MeshObject.h"
#include "CVideo.h"
#include "DialogSystem.h"

#include "L7S3_Pop1Bhasmam.h"
#include "L7S3_Pop2Bull.h"
#include "L7S3_Pop3Chain.h"
#include "L7S3_Pop4Ameeha.h"
#include "AssetsStringTable.h"

enum eL7S3PopEnums
{
	ePop1_Bhasmam,
	ePop2_BullStatue,
	ePop3_Chain,
	ePop4_Ameeha,
	ePopL7S3_Max
};

class L7_S3_Haven : public HiddenObjectSystem
{


public:
	MeshObject* meshwater;
	float musicFadeOutTimer;

	
	bool isLasyaAttackVideoPlaying;
	CVideo *cv_LasyaAttack;
	float time;
	float vidTimer;
	int subIndex;
	int subMax;
	subText subtitles[15];
	CBitmapText* subtitleText;

	bool isDialogActive;
	DialogSystem *dialogSystem;

	//Popups
	int ActivePopupID;

	L7S3_Pop1Bhasmam	*pop1Bhasmam;
	L7S3_Pop2Bull		*pop2BullStatue;
	L7S3_Pop3Chain		*pop3Chain;

	Popup*		PopupArray[ePopL7S3_Max];
	int			PopStates[ePopL7S3_Max];
	CPolygon*	PopPolyArray[ePopL7S3_Max];

	void OnPopupClosed(Popup* popup);
	void LoadNextPopup(Popup* popup, bool loadPrevious);

	SuperFX *s3lavadrops2FX, *s3lavadrops3FX, *s3lavadrops4FX, *s3lavafallsFX;
	SuperFX *s3lavadropsFX, *s3rightsmokeFX, *s3leftsmokeFX;
	SuperFX *s3fireprtFX, *s3fireprt2FX, *s3lavafall2FX, *s3lavafall3FX, *s3lavafall4FX, *s3smokefrontFX, *s3smokegroundFX, *s3rightsmokedownFX, *s3starsFX;

	L7_S3_Haven();
	~L7_S3_Haven();

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
};

#endif
