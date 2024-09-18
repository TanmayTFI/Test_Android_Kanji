//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL6 - SCENE4 : FLOATING ISLAND4
//====================================

#ifndef DQFYLH_LEVEL6_SCENE4_PORTAL_CHEST_DEF
#define DQFYLH_LEVEL6_SCENE4_PORTAL_CHEST_DEF

#include "HiddenObjectSystem.h"
#include "L6S4_PopSecretLetter.h"
#include "JSONAnimator.h"
#include "DialogSystem.h"
#include "CVideo.h"

class L6_S4_PortalChest: public HiddenObjectSystem, CRenderListEntity
{
	L6S4_PopSecretLetter *popUp;
	JSONAnimator *Janim_Glue;
	CVideo *lpVidMaryWalking;
	float vidTimer;
	bool isVideoPlaying;
	int activePopupId;
public:
	//--------------------------- Functions -------------------------------//
	L6_S4_PortalChest();
	~L6_S4_PortalChest();
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
	void NavigateToScene();

protected:

	void ReceiveMessage(int val);
	void ProcessNonHOHint();
};

#endif