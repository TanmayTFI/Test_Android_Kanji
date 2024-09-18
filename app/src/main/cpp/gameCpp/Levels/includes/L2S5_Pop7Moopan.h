#ifndef DQFYLH_LEVEL2_SCENE1_POP7_MOOPAN_DEF
#define DQFYLH_LEVEL2_SCENE1_POP7_MOOPAN_DEF

#include "Popup.h"
#include "CVideo.h"
#include "DialogSystem.h"
#include "AssetsStringTable.h"

class L2_S5_MoopansDen;

class L2S5_Pop7Moopan: public Popup
{
	public:
		L2S5_Pop7Moopan(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		void SetDialogSystem(DialogSystem *dialogSystem);
		~L2S5_Pop7Moopan();

		void Update(float ds );
		void Required();
		void HandleInventoryDrop(CPoint pos,int val);


		bool isTalkingDialogComplete, isDrinkingDialogComplete;
		JSONAnimator *JAnim_MoopanTired, *JAnim_MoopanTalk, *JAnim_MoopanDrinking;

		bool	isVideoPlaying, isDialogActive, isDialogComplete;
		CVideo *cv_Cutscn;
		float time; float vidTimer;

		int subIndex;
		subText subtitles[5];
		CBitmapText* subtitleText; int subMax;

		CObject *Moopan, *MoopanTalk;
		CPolygon *Pop7MoopanPoly;
		void ProcessHint();
		DialogSystem *dialogSystem;
		float easing(float t,float b,float c,float d);

		bool isSceneUpdated;
		CObject *MoopanTiredArray[8];
		CObject *MoopanDrinkingArray[11];
		void OnDialogComplete(int index );

	protected:
		
		void ReceiveMessage(int val);
		void UpdateSceneElements();
		void InitSceneElements();
		void OnAnimationComplete(int id);
		bool isTaskLeft();
};

#endif