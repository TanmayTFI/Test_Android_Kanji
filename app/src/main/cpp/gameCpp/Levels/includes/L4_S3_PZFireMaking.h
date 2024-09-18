//====================================
// DQFYLH
// Hidden Object System
// Level 4 - Scene 02 : PUZZLE DOOR
//====================================

#ifndef DQFYLH_LEVEL4_SCENE3_PZ_FIRE_MAKING_DEF
#define DQFYLH_LEVEL4_SCENE3_PZ_FIRE_MAKING_DEF

#include "HiddenObjectSystem.h"
#include "JSONAnimator.h"
#include "L4S3_PZPopChart.h"

enum L4S3PZObjects
{
	e_plank,
	e_bow,
	e_coin,
	e_ember,
	e_MaxL4S3
};

enum L4S3ObjectStates
{
	ObjL4S3_Locked = 0,
	ObjL4S3_Inactive,
	ObjL4S3_Active,
	ObjL4S3_Used
};

class L4_S3_PZFireMaking: public HiddenObjectSystem, CRenderListEntity
{
	public:

		//Scene Elements
		float musicFadeOutTimer;

		//Popups
		
		L4_S3_PZFireMaking();
		~L4_S3_PZFireMaking();
		
		JSONAnimator *FireMakingAnim, *EmberDropAnim, *OilPourAnim;

		CObject *Bow;		
		CObject *BowAnim;
		CObject *BowInHand;		
		CObject *Bowl;	
		CObject *BowlPour;	
		CObject *CoinAnim;
		CObject *CoinInHand;
		CObject *CoinInScene;		
		CObject *HandL;	
		CObject *HandR;	
		CObject *Sheet;	
		CObject *OilDrop;		
		CObject *OilOnTwigs;		
		CObject *PlankAnim;	
		CObject *PlankInHand;		
		CObject *PlankInScene;	
		CObject *StickAnim;	
		CObject *Ember;	
		CObject *EmberInHand;		
		CObject *Twig1;	
		CObject *Twig2;	
		CObject *Twig3;	
		CObject *Twig4;	
		CObject *FireAnim;	

		CPoint *EmberPos;

		CObject	*ScnObjects		[e_MaxL4S3];
		CObject	*InHandObjects	[e_MaxL4S3];
		int		 ObjStates		[e_MaxL4S3];
		CPolygon *ObjPoly		[e_MaxL4S3];
		CPolygon *DestPoly		[e_MaxL4S3];

		CPolygon	*AltarPoly;
		CPolygon	*ChartPoly;

		CHOScene *ChartScene;


		bool IsAnimPlaying;
		bool IsFadingIn;
		bool IsFadingOut;
		bool IsChartActive;
		int ObjectInHandID;
		float FadeAlpha;

		//Inherit this CRenderListEntity function to render anything to scene!! 
		void Render(const CPoint* pCam);
		void Update();
		void Required();

		float easing(float t,float b,float c,float d);

		void NavigateToScene();

		
	protected:

		//Popups
		L4S3_PZPopChart *InstructionChart;
		void OnPopupClosed(Popup * popup);

		SuperFX *S3_FireBoardSmokeFX;
		std::vector<SuperFX*> superfxArray;

		void UpdateBannerMessages(CPoint pos);
		void OnAnimationComplete(int index);
		void InitAnimations();
		void TaskPendingUpdate();
		void CloseChartScene();
		void InitSceneElements();
		void ReceiveMessage(int val);
		void ProcessNonHOHint();
		void SparkleUpdate();
		void CheckForPendingTask();
};

#endif