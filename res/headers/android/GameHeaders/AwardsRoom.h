//====================================
// DQFYLH
// Hidden Object System
// AWARDS ROOM
//====================================

#ifndef DQFYLH_AWARDSSROOM_DEF
#define DQFYLH_AWARDSSROOM_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Polygon.h"
#include "SuperFX.h"
#include "MeshObject.h"

#include "MultilineText.h"

#include "Award.h"

class SuperFX;
class CHOScene;

#define NUM_PAGE1_AWARDS 15
#define NUM_PAGE2_AWARDS 21

class AwardsRoom: public HiddenObjectSystem, CRenderListEntity
{
	public:
		//Scene Elements
		CObject *BackBtnGlow, *BackBtnText;

		bool isAwardCollected[kAward_Max];

		//Page One Awards
		CHOScene *SCN_Page1;
		CObject *pg1AwardGlows[NUM_PAGE1_AWARDS], *pg1AwardIcons[NUM_PAGE1_AWARDS], *pg1AwardActiveGlows[NUM_PAGE1_AWARDS];

		//special cases
		CObject *pg1Subicon5[3], *pg1Subicon13[2], *pg1Subicon3[3];

		//Page Two Awards
		CHOScene *SCN_Page2;
		CObject *pg2AwardGlows[NUM_PAGE2_AWARDS], *pg2AwardIcons[NUM_PAGE2_AWARDS], *pg2AwardActiveGlows[NUM_PAGE2_AWARDS];

		CObject *subIconGlow;

		CObject *pageTab1, *pageTab2;
		CObject *btnNextPageActiveIcon, *btnNextPageMouseOver;
		CObject *btnPrevPageActiveIcon, *btnPrevPageMouseOver;
		int currentPage;

		void setupCurrentPage();

		CObject *TextFrameBG, *TextTitle;
		MultilineText *TextAwardDescription;

		CObject *PopMaskBg, *PopCloseBtn;

		SuperFX *WindFX, *GlareFX, *LeftCrystalFX, *RightCrystalFX;

		MeshObject *leftPlantMesh, *rightPlantMesh;

		int Popup_State;

		bool IsPopupActive;
		bool IsPopupTransitionActive;
		float PopupAlpha, PopVal;
		void SetupPopupTransition(CHOScene *Popup, int popx, int popy);
		void UpdatePopupTransition();

		float easingQuadOut(float t,float b,float c,float d);

		AwardsRoom();
		~AwardsRoom();

		//Inherit this CRenderListEntity function to render anything to scene!! 
		void   Render(const CPoint* pCam);

		//Message Banner
		void UpdateBanner();
		void InitMessageBanner();		
		void TransOutBanner();
		void ShowMessageBanner(const std::string& message, int SFXId = SFX_SOFT_NOTIFY);
		void ShowBannerText(const std::string& keyString, int SFXId = SFX_SOFT_NOTIFY);
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
		void UpdateIcons();
		void Required(){};

	protected:

		void ReceiveMessage(int val){};
};

#endif
