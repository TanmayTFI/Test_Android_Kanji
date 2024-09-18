//====================================
// DQFULP
// Hidden Object System
// Author : Amal
// LEVEL3 - SCENE3 : FortEntrance
//====================================

#ifndef DQFYLH_LEVEL3_SCENE3_FORTENTRANCE_DEF
#define DQFYLH_LEVEL3_SCENE3_FORTENTRANCE_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Map.h"
#include "PopupInterface.h"

class CHOScene;

using namespace std;

enum SceneProperties
{
	S1_Eye = 0,
	S1_OpenDoor,
	S1_Ladder,
	S1_RingOpen,
	S1_RingClose,
	S1_RingCharcol,
	S1_RingFire,
	S1_HoAfterMask,
	S1_PlantAnim,
	S1_Leaf1,
	S1_Leaf2,
	S1_Leaf3,
	S1_CrystelGlow,
	S1_Fire1,
	S1_Fire2,
	S1_Fire3,
	S1_Flag,
	S1_MouthOpen,
	S1_CrystalOnMouth
};

enum SceneObjectives
{
	S1_Objctive_FindLadder = 0,
	S1_Objctive_PickFlag,
	S1_Objctive_FindLadderStick,
	S1_Objctive_PlaceLadder,
	S1_Objctive_OpenDoor,
	S1_Objctive_OffTheFlames,
	S1_Objctive_PickCharcol,
	S1_Objctive_RemoveCharcols,
	S1_Objctive_OpenPuzzleDoor,
	S1_Objctive_CollectGemEye,
	S1_Objctive_PlaceGemEye,
	S1_Objctive_CollectCrystelkFromMouth
};

enum L3_Popup_Types
{
	ePopL3S1_Inactive = -1,
	ePopL3S1_Ember = 0,
	ePopL3S1_Flag,
	ePopL3S1_SkullFace,
	ePopL3S1_Max
};

class L3_S1_FortEntrance: public HiddenObjectSystem, CRenderListEntity, public PopupInterface
{
	CPolygon *PopAreaPolyRing;
	SuperFX *WaterFallLeftFX1, *WaterFallLeftFX2, *WaterFallbotomFX, *WaterFallRightFX;
	SuperFX *fireflamesFX, *firefogFX;
	SuperFX *hodotsFX, *holinesFX;
	map<SceneProperties, CObject*> _sceneProperties;
	vector<bool> _userCompletedObjectives;
	float musicFadeOutTimer;
	// Popups
	L3_Popup_Types ActivePopupID;
	map<L3_Popup_Types, Popup*> _popupsMap; 

public:

	L3_S1_FortEntrance();
	~L3_S1_FortEntrance();

	void SaveChange(SceneObjectives objective, bool status);
	void TaskPendingUpdate();
	void NavigateToScene();

protected:

	float easing(float t,float b,float c,float d);
	void Render(const CPoint* pCam);
	void Update();
	void Required();

	void ReceiveMessage(int val);
	void ProcessNonHOHint();
	void SparkleUpdate();

private:

	void InitPopups();
	void InitSceneProperties();
	void SceneObjectsUpdate();
	void CheckForPendingTask();
	void OnOpenPopup(); 

	void OnClosePopup(Popup *popup);
	void UpdateHOSystem(Popup *popup, int objectiveVal);
	void NewArea();
};

#endif