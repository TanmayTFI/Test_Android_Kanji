//====================================
// DQFYLH
// Hidden Object System
// EXTRAS ROOM
//====================================

#ifndef DQFYLH_EXTRASROOM_DEF
#define DQFYLH_EXTRASROOM_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"

#include "VideoDefs.h"
#include "KVideo.h"

#include "Polygon.h"
#include "SuperFX.h"
#include "MeshObject.h"

class SuperFX;
class CHOScene;

class ExtrasRoom: public HiddenObjectSystem, CRenderListEntity
{
	public:
		//Scene Elements
		CObject *BackBtn;

		CObject *MemoryOver, *AwardsOver, *MorphOver, *BackBtnText;
		CBitmapText* MemoryText, * AwardsText, * MorphText;
		CObject *PopMaskBg, *PopCloseBtn;

		SuperFX *KidMemOverFX, *AwardFX, *EagleGlitterFX, *CrystalFX;

		MeshObject *memoryMesh, *awardMesh;

		int Popup_State;

		bool IsPopupActive;
		bool IsPopupTransitionActive;
		float PopupAlpha, PopVal;
		void SetupPopupTransition(CHOScene *Popup, int popx, int popy);
		void UpdatePopupTransition();

		float easingQuadOut(float t,float b,float c,float d);

		ExtrasRoom();
		~ExtrasRoom();

		//Inherit this CRenderListEntity function to render anything to scene!! 
		void   Render(const CPoint* pCam);

		//Message Banner
		void UpdateBanner();
		void InitMessageBanner();		
		void TransOutBanner();
		void ShowMessageBanner(const std::string& message, int SFXId = SFX_SOFT_NOTIFY);
		bool HideMessageBanner(void);
		virtual void BannerCloseNotify(){};
		CBitmapText *MessageText;
		float BannerTimer;
		float BannerAlpha;
		float TransOutFactor;
		bool  IsBannerStandStill;
		eMessageBannerState BannerState;
		CObject	  *MessageElement;

		void Update();
		void Required(){};

	protected:

		void ReceiveMessage(int val){};
};

#endif