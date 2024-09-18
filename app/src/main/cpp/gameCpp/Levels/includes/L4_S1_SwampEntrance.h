//====================================
// DQFULP
// Hidden Object System
// Author : Sajith
// LEVEL4 - SCENE1 : Swamp Entrance
//====================================

#ifndef DQFYLH_LEVEL4_SCENE1_SWAMP_ENTRANCE_DEF
#define DQFYLH_LEVEL4_SCENE1_SWAMP_ENTRANCE_DEF
#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "ClickablePolygon.h"
#include "SuperFX.h"
#include "L4S1_Pop1RightFoot.h"
#include "L4S1_Pop2HealingObjects.h"
#include "L4S1_Pop3Pocket.h"
#include "L4_S1_PZMushroom.h"

class ParticleEffect;
class CHOScene;

class L4_S1_SwampEntrance: public HiddenObjectSystem, CRenderListEntity
{
	CPolygon *waterAreaPoly;
	float BannerTimer;
	public:
		MeshObject *mo_water;

		float musicFadeOutTimer;
		//popups
		std::vector<CRectangle*> PopRectArray;
		std::vector<Popup*> PopupArray;

		int activePopUPId;

		SuperFX *WaterFallFX, *WaterFallBottomFX, *GlowPlant_FX, *GlowPlant_FX1, *holinesFX, *hodotsFX;

		L4_S1_PZMushroom *PZ_Mushroom;
		CObject *resetBtn, *resetBTN_text;
		//--------------------------- Functions -------------------------------//
		L4_S1_SwampEntrance();
		~L4_S1_SwampEntrance();

		void InitPopups();
		void OnPopupClosed(Popup * popup);
		
		float easing(float t,float b,float c,float d);
		
		bool isPopupActive(int popId);
		//Inherit this CRenderListEntity function to render anything to scene!!
		void Render(const CPoint* pCam);
		void Update();
		void Required();
		void SceneObjectsUpdate();
		void TaskPendingUpdate();

		void ProcessSkip();
		void NavigateToScene();
	protected:

		void ReceiveMessage(int val);
		void ProcessNonHOHint();
		void SparkleUpdate();
		void CheckForPendingTask();
};

#endif