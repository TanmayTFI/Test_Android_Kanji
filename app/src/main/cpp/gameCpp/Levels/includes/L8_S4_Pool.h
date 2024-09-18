

#ifndef DQFYLH_LEVEL8_SCENE4_POOL_DEF
#define DQFYLH_LEVEL8_SCENE4_POOL_DEF

//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Popup.h"
#include "JSONAnimator.h"
#include "L8S4_Pop1BronzeIdol.h"
#include "L8S4_Pop2SilverIdol.h"
#include "L8S4_Pop3GoldIdol.h"
#include "L8S4_Pop4LeafContainer.h"
#include "Bezier2D.h"

enum eL8S4PopEnums
{
	ePop1_L8S4BronzeIdol,
	ePop2_L8S4SilverIdol,
	ePop3_L8S4GoldIdol,
	ePop4_L8S4LeafContainer,
	ePopL8S4_Max
};

class L8_S4_Pool: public HiddenObjectSystem
{
public:
	L8_S4_Pool();
	~L8_S4_Pool();
	float musicFadeOutTimer;

	//Popups
	int ActivePopupID;

	Popup*		PopupArray[ePopL8S4_Max];
	int			PopStates[ePopL8S4_Max];
	CPolygon*	PopPolyArray[ePopL8S4_Max];
	CPolygon*	PoolMiniGamePoly;
	void Update();
	void Required();
	bool IsPoolMiniGameActive;
	void NavigateToScene();
protected:

	CPolygon *PillarGoldPoly, *PillarSilverPoly, *PillarBronzePoly;
	float SwanAnimThreshold, SwanAnimTimer;
	bool IsAnimPlaying;

	JSONAnimator *LeafPillarGoingDown, *S4SwanSwimming;
	JSONAnimator *MiniGameGoingDown;
	JSONAnimator *LeafPillarComingUp;
	CObject *BasePillar, *BronzeIdol, *GlowLeft, *GlowRight, *GoldIdol, *LeafBoxClosed, *LeafBoxOpen, *LeavesAnim00, *LeavesAnim01, *LeavesAnim02, *LightRays, *LotusAfter, *LotusBefore, *MinigameBase;
	CObject *PillarBack, *PillarLeft, *PillarRight, *PinkFlowerAnim00, *PinkFlowerAnim01, *PinkFlowerAnim02, *PinkFlowerAnim03, *Pool, *PuzzleCompleteState, *RabitAnim00, *RabitAnim01, *SilverIdol;
	CObject *SwanAnim00, *SwanAnim01, *YellowAnim00, *YellowAnim01, *YellowAnim02, *SwanAnim01flip;
	float	AnimTimer, AnimThreshold;

	int SwanDirection;
	SuperFX *S4LeafFallFX;
	Bezier2D *SwanPath;
	bool isPopupActive(int popId);
	void ToggleParticleEffects();
	void CheckIfBasePillarActive();
	void InitAnimations();
	void OnAnimationComplete(int index);
	void InitSceneElements();
	void SceneObjectsUpdate();
	void TaskPendingUpdate();
	void InitPopups();
	void OnPopupClosed(Popup* popup);
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
	void SparkleUpdate();
	void CheckForPendingTask();
};

#endif
