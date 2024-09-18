

#ifndef DQFYLH_L5_S6_HO_TREE_DEF
#define DQFYLH_L5_S6_HO_TREE_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "JSONAnimator.h"
#include "MeshObject.h"
#include "MultilineText.h"
#include "CVideo.h"

#include "DialogSystem.h"

#define MAX_L5S6_HOOBJECTS 14

class L5_S6_HOTree : public HiddenObjectSystem
{
public:
	DialogSystem *dialogSystem;
	bool isDialogActive;

	bool isTreeReleaseCutscnPlaying;
	CVideo *cv_TreeReleaseCutscn;
	float vidTimer;

	float dialogDelayTimer;
	float musicFadeOutTimer;
	L5_S6_HOTree();
	~L5_S6_HOTree();

	MeshObject *_faceMesh;

	CObject		*cloudBG1, *cloudBG2;
	float cloudbgSpeed1, cloudbgSpeed2;
	CObject		*cloudFG1, *cloudFG2;
	float cloudfgSpeed1, cloudfgSpeed2;

	float treeEyeTimer, treeEyeTimerThreshold;

	SuperFX *s6hofgblueplantFX, *s6horedplantFX, *s6hoeyefxFX, *s6hoeyefx2, *s6holeafFX, *s6homanjaadifxFX;
	SuperFX *s6hosnowFX, *s6hofogFX;

	CPolygon	*HOPolyArray[MAX_L5S6_HOOBJECTS];

	CObject		*HOScnObjects[MAX_L5S6_HOOBJECTS];
	CObject		*HOGlowObjects[MAX_L5S6_HOOBJECTS];
	CObject		*HOManjadiObjects[MAX_L5S6_HOOBJECTS];

	CObject		*ManjadiFull, *ManjadiHalf, *ManjadiQuarter;
	CObject		*manjaadiRiseup;

	CObject		*boxGlow1Butterfly, *boxGlow2Nest, *boxGlow3Bird, *boxGlow4Snake, *boxGlow5Nuts;
	void updateBoxSymbolGlow();

	int numObjCollected;

	bool isHOCompleted;
	float hoCompleteTimer;

	void HandleHOComplete();

	SuperFX *boxOpenVFX, *HOFoundFX;
	JSONAnimator *JAnim_SoldierBadgePlace;
	void onAnimationComplete(int id);
	bool isSoldierBadgePlacePlaying;

	MultilineText *hoHelpText;

	void Update();
	void Required();

	void InitHO();

	void SetLockLevels(bool lockLevel);

protected:
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
};

#endif
