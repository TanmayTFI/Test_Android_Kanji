//====================================
// DQFYLH
// Have you cutscene
// backstory
//====================================

#ifndef DQFYLH_HAVEYOU_DEF
#define DQFYLH_HAVEYOU_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "HOScene.h"
#include "MeshObject.h"

#include "VideoDefs.h" 
#include "KVideo.h"
#include "AssetsStringTable.h"
#include "BitmapText.h"

#ifdef _PARTICLEEDITORENABLED
	#include "ParticleEditor.h"
#endif

class HaveYouCutScene: public Control, CRenderListEntity
{
	public:
		#ifdef _PARTICLEEDITORENABLED
			bool IsParticleEditorEnabled;
			ParticleEditorControl *_particleEditor;
		#endif

		//Scene Elements
		bool IsVideo1Loaded;
		bool IsHaveYouVideoPlaying;

		CHOScene *SCNSndExp;
		CObject* SndExpTxt;
		MeshObject *FgFeatherMesh;

		KGraphic* templeOdoomTxt;

		SuperFX *vfxRain, *vfxLeaves, *vfxRainDrops;
		bool isSndSceneFadingIn, isSndSceneFadingOut;
		float sndSceneFadeInTime, sndSceneFadeOutTime;
		
		float sndSceneIdleTime;

		subText subtitles[10];
		float time;
		int subIndex;
		CBitmapText* subtitleText; int subMax;

		//Benchmarking the system for stable FPS to check for Low System Specifications
		bool isFPSCheckingActive;
		int Totalfps, fpsCount;

		CObject	*MaskBg;

		KGraphic *CoreImage;

		bool isMouseHover;
		CPoint mosPos;
		bool isSkipped;
		bool isSkipShowing;


		KVideo *lpHaveYouCutsceneVideo;
		KSound *lpHaveYouCutsceneSndStream;

		HaveYouCutScene();
		~HaveYouCutScene();

		//Inherit this CRenderListEntity function to render anything to scene!! 
		void   Render(const CPoint* pCam);

		void Update();
		void Required(){};

	protected:

		void ReceiveMessage(int val){};

};

#endif