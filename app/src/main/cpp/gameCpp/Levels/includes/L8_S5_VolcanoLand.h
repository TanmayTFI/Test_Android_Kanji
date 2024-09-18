//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL8 - SCENE5 : VOLCANO_LAND
//====================================

#ifndef DQFYLH_LEVEL8_SCENE5_VOLCANO_LAND_DEF
#define DQFYLH_LEVEL8_SCENE5_VOLCANO_LAND_DEF

#include "HiddenObjectSystem.h"
//#include "L6S4_PopSecretLetter.h"
#include "JSONAnimator.h"
#include "DialogSystem.h"
#include "CVideo.h"
#include "Popup.h"
#include "MeshObject.h"
#include "AssetsStringTable.h"

class L8_S5_VolcanoLand: public HiddenObjectSystem, CRenderListEntity
{
	CVideo *lpVidEndCutscene;
	float vidTimer;
	float time;
	int subIndex;
	int subMax;
	subText subtitles[10];
	CBitmapText* subtitleText;
	bool isVideoPlaying;
	CObject * lava;
	CObject * boxGlow;
	SuperFX *volcanoSmokeFX;
	SuperFX *lavaParticleFX;
	CPolygon *MaryRect;
	CPolygon * leafArea;
	MeshObject *waterMesh;
	CObject * mary;
public:
	//--------------------------- Functions -------------------------------//
	L8_S5_VolcanoLand();
	~L8_S5_VolcanoLand();
	float musicFadeOutTimer;
	DialogSystem *dialogSystem;
	bool isDialogActive;

	float easing(float t,float b,float c,float d);

	//Inherit this CRenderListEntity function to render anything to scene!!
	void Render(const CPoint* pCam);
	void Update();
	void Required();
	void SceneObjectsUpdate();
	void TaskPendingUpdate();
	void InitParticles();
	void OnPopupClosed(Popup * popup);
	void OnClickLeafBox(CObject * obj);

protected:

	void ReceiveMessage(int val);
	void ProcessNonHOHint();
	void SparkleUpdate();
	void CheckForPendingTask();
};

#endif