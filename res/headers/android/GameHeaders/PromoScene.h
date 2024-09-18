//====================================
// DQFYLH
// Hidden Object System
// Level 1 - PROMO
//====================================

#ifndef DQFYLH_LEVEL1_PROMO_DEF
#define DQFYLH_LEVEL1_PROMO_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"

#include "VideoDefs.h"
#include "KVideo.h"

class CHOScene;

class PromoScene: public HiddenObjectSystem, CRenderListEntity
{
	public:

		//Scene Elements
		bool IsVideo1Loaded;
		bool IsPromoVideoPlaying;

		bool	IsSkipMouseOver;
		float	SkipShowTimer;
		KGraphic *SkipImage;

		KVideo *lpPromoCutsceneVideo;
		KSound *lpPromoCutsceneSndStream;

		PromoScene();
		~PromoScene();

		//Inherit this CRenderListEntity function to render anything to scene!! 
		void   Render(const CPoint* pCam);
		void ExitGame();
		void Update();
		void Required(){};

	protected:


		void ReceiveMessage(int val){};
		void ProcessNonHOHint(){};
};

#endif