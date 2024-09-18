//====================================
// DQFULP
// Hidden Object System
// Author : Sreeraj
// LEVEL3 - SCENE3 : P1Room1Start
//====================================

#ifndef DQFYLH_LEVEL3_SCENE3_P1Room1Start_DEF
#define DQFYLH_LEVEL3_SCENE3_P1Room1Start_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "MeshObject.h"
#include "L3_S2_M1PipePopUp.h"
#include "L3_S2_MazeDeadEnd.h"

using namespace std;

class CHOScene;

enum M1Room1_Popups
{
	R1_NoPopup = 0,
	R1_PipePickPopup
};

class L3_S2_P1Room1Start: public HiddenObjectSystem, CRenderListEntity
{
	CObject* _waterMask;
	CObject* _pipeInWater;
	SuperFX *S2M1LightFX;
	M1Room1_Popups _activePopup;
	float musicFadeOutTimer;
	vector<JSONAnimator*> _janim_RemoveVine;

	L3_S2_M1PipePopUp *_pipePopup;
	L3_S2_MazeDeadEnd *_wrongScene;

	JSONAnimator* _jAnim_PlaceMap;
	
public:

		//--------------------------- Functions -------------------------------//
		L3_S2_P1Room1Start();
		~L3_S2_P1Room1Start();
		
		float easing(float t,float b,float c,float d);
		
		//Inherit this CRenderListEntity function to render anything to scene!!
		void Render(const CPoint* pCam);
		void Update();
		void Required();

		void TaskPendingUpdate();
		void NavigateToScene();

	protected:

		void ReceiveMessage(int val);
		void SparkleUpdate();
		void ProcessNonHOHint();
		void OnClosePopup(Popup *popup);
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		void OnAnimatonComplete(int animID);
};

#endif