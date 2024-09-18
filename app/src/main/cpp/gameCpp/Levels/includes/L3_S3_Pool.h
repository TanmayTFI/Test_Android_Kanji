//====================================
// DQFULP
// Hidden Object System
// Author : Faisal
// LEVEL3 - SCENE3 : Pool
//====================================

#ifndef DQFYLH_LEVEL3_SCENE3_POOL_DEF
#define DQFYLH_LEVEL3_SCENE3_POOL_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "MeshObject.h"

#include "Popup.h"

class CHOScene;

enum eL3S3PopEnums
{
	ePop1_Saw,
	ePop2_Showel,
	ePop3_Pool,
	ePop4_Lamp,
	ePop5_Door,
	ePopL3S3_Max
};

class L3_S3_Pool: public HiddenObjectSystem
{
	public:

		bool isLampRisingFromWater;
		void OnLampRisingCompleted();
		CObject *LampRising;
		int LampRisingEndY;
		float musicFadeOutTimer;
		SuperFX *s3riseupraysFX, *s3riseupspreadfxFX;

		std::vector<SuperFX*>	VFXArray;

		SuperFX *s3crystal1FX, *s3crystal2FX, *s3crystal3FX, *s3crystal4FX, *s3smokeFX;

		MeshObject* meshWater;

		//Popups
		int ActivePopupID;

		Popup*		PopupArray[ePopL3S3_Max];
		int			PopStates[ePopL3S3_Max];
		CPolygon*	PopPolyArray[ePopL3S3_Max];

		void InitPopups();
		void OnPopupClosed(Popup* popup);
		
		L3_S3_Pool();
		~L3_S3_Pool();
				
		void SparkleUpdate();
		void TaskPendingUpdate();
		void SceneObjectsUpdate();

		void Update();
		void Required();
		void NavigateToScene();

	protected:
		void ReceiveMessage(int val);		
		void ProcessNonHOHint();
};

#endif