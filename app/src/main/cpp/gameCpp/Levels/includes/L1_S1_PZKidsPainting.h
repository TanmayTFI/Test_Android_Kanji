//====================================
// DQFYLH
// Hidden Object System
// Level 1 - Scene 01 :   KIDS PAINTING
//====================================

#ifndef DQFYLH_LEVEL1_SCENE1_KIDSPAINTING_DEF
#define DQFYLH_LEVEL1_SCENE1_KIDSPAINTING_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "SuperFX.h"
#include "CustomAnimator.h"
#include "MeshObject.h"
#include "CVideo.h"

#include "VideoDefs.h"
#include "KVideo.h"
#include "AssetsStringTable.h"

class CHOScene;

class L1_S1_PZKidsPainting: public HiddenObjectSystem, CRenderListEntity
{
	public:

		CObject *mom;
		CObject *me;
		CObject *king;
		CObject *buffalo;
		CObject *castle;
		CObject *stars;
		CObject *moon;
		CObject *flower1;
		CObject *flower2;

		CObject *momred;
		CObject *mered;
		CObject *kingred;
		CObject *buffalored;
		CObject *castlered;
		CObject *starsred;
		CObject *moonred;
		CObject *flower1red;
		CObject *flower2red;
		
		CObject *momyellow;
		CObject *meyellow;
		CObject *kingyellow;
		CObject *buffaloyellow;
		CObject *castleyellow;
		CObject *starsyellow;
		CObject *moonyellow;
		CObject *flower1yellow;
		CObject *flower2yellow;		

		L1_S1_PZKidsPainting();
		~L1_S1_PZKidsPainting();

		CVideo *cv_Cutscn; 
		float time;
		float vidTimer;
		int subIndex;
		subText subtitles[2];
		CBitmapText* subtitleText; int subMax;
		
		float musicFadeOutTimer;
		bool	IsVidSkippable, IsSkipMouseOver, IsSkipMouseClicked;
		float	SkipShowTimer;
		CObject *skipObj;

		SuperFX *endGameVFX;
		bool IsEndAnimPlaying,IsCutScenePlaying;
		bool IsMiniGameSkiped;
		float endAnimTimer;
		//Inherit this CRenderListEntity function to render anything to scene!! 
		void Render(const CPoint* pCam);

		void ResetItems(CObject *Obj, CObject *txtObj);
		void DisableItems();

		void Update();
		void Required();	
		void ProcessSkip();

		CObject *txtMama,*txtMoon, *txtKing, *txtMe, *txtBuffalo, *txtStars, *txtFlower, *txtCastle;
		CObject *tempObj,*tempTxtObj;	

		MeshObject *FgBgMesh;

		bool IsObjFading ;
		float easing(float t,float b,float c,float d);
	void NavigateToScene();

	protected:
		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif
