//====================================
// DQFYLH
// Hidden Object System
// Level 2 - Scene 05 :  MOOPAANS DEN
//====================================

#ifndef DQFYLH_LEVEL2_SCENE5_MOOPANSDEN_DEF
#define DQFYLH_LEVEL2_SCENE5_MOOPANSDEN_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "SuperFX.h"
#include "L2S5_Pop1Vinegar.h"
#include "L2S5_Pop3Ember.h"
#include "L2S5_Pop5Basket.h"
#include "L2S5_Pop7Moopan.h"

class CHOScene;

enum eL2S5HOPopEnums
{
	ePop1_Vinegar,
	ePop2_MedicineMaking,
	ePop3_Amber,
	ePop4_EagleBlock,
	ePop5_BambooBasket,
	ePop6_MedicineRecipe,
	ePop7_Moopan,
	ePopS5_Max
};

class L2_S5_MoopansDen: public HiddenObjectSystem
{
	public:

		//Scene Elements
		CObject *Scn_Bg;
		float musicFadeOutTimer;		
		//Popups
		int ActivePopupID;

		Popup*		PopupArray[ePopS5_Max];
		int			PopStates[ePopS5_Max];
		CPolygon*	PopPolyArray[ePopS5_Max];
		
		//particles

		SuperFX *DustWindowLeft, *DustWindowRight;

		void OnPopupClosed(Popup * popup);

		CObject *Scn_OrbTaking;
		bool IsOrbFadingIn, IsOrbTaken, IsOrbAnimatingToHUD;
		CPoint orbControlPos;
		CPoint orbInitPos;
		CPoint orbEndPos;		
		float orbInitScale,orbFinalScale;
		float orbTweenTimer, orbTweenMaxTime;

		CObject *InvAmber;
		CObject *InvPot;
		CObject *StrawBasket;

		L2_S5_MoopansDen();
		~L2_S5_MoopansDen();

		void InitPopups();
		
		void Update();
		void Required();

		void SceneObjectsUpdate();

		L2S5_Pop7Moopan* MoopanPopup;
		DialogSystem *dialogSystem;
		int dialogIndex;

		void NavigateToScene();

	protected:

		CObject *Alovera, *AloveraCut, *Chart, *Fire, *FireFx, *Fog, *HoneyPot, *Kindi, *Mango, *MangoCut, *MiniGrinderHadle, *MortarWithPestle, *PestleOnGround, *Pot, *TurmericCrusher;
		CObject *TurmericFull, *TurmericPices, *TurmericPot, *Wood;

		void InitSceneElements();
		bool CheckIfPopupActive(int polyIndex);
		void SparkleUpdate();
		void TaskPendingUpdate();
		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif