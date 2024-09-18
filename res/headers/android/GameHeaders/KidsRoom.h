//====================================
// DQFYLH
// Hidden Object System
// KIDS ROOM
//====================================

#ifndef DQFYLH_KIDSROOM_DEF
#define DQFYLH_KIDSROOM_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"

#include "VideoDefs.h"
#include "KVideo.h"

#include "Polygon.h"
#include "SuperFX.h"

class SuperFX;
class CHOScene;

class KidsRoom: public HiddenObjectSystem, CRenderListEntity
{
	public:

		//Scene Elements
		CObject *BackBtn;

		CObject *HangBellBody;
		CObject *HangBellParts[5];

		CBitmapText *txtMemoryCollectedCount;

		float maxDeg;
		bool IsMaxDegRising;
		float HangBellAngles[5];

		float BellTopAngle;

		CHOScene *MemPopScene;
		CObject *PopMaskBg, *PopCloseBtn;

		CObject *MemObjects[eMem_MAX];

		CPolygon *MemPolyArray[eMem_MAX];

		bool isMemCollected[eMem_MAX];

		int NumMemObjActive;

		SuperFX *windowFX, *LampFX, *ObjMouseOverFX, *lastObjFX;

		int Popup_State;

		bool IsPopupActive;
		bool IsPopupTransitionActive;
		float PopupAlpha, PopVal;
		void SetupPopupTransition(CHOScene *Popup, int popx, int popy);
		void UpdatePopupTransition();

		float easingQuadOut(float t,float b,float c,float d);

		KidsRoom();
		~KidsRoom();

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

		int	 PrevRandomId;
		void PlayRandomBellSFX();
		void Update();
		void Required(){};

	protected:

		void ReceiveMessage(int val){};

};

#endif
