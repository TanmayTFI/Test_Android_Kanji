//====================================
//DQFULP
// Hidden Object System
// Strategy Guide
//====================================

#ifndef DQFULP_STRATEGYGUIDE_DEF
#define DQFULP_STRATEGYGUIDE_DEF

#include "Control.h"
#include "HOObject.h"
#include "BitmapText.h"
#include <string>
#include "MultilineTextBox.h"
#include "AssetsStringTable.h"

class CObject;
class CHOScene;
class HudControl;
class CBitmapText;

#define MAX_GUIDE_LV1_PAGES 24 //always plus two - first page always table of contents, 2nd Page General Tips
#define MAX_GUIDE_LV2_PAGES 11
#define MAX_GUIDE_LV3_PAGES 13
#define MAX_GUIDE_LV4_PAGES 9
#define MAX_GUIDE_LV5_PAGES 8
#define MAX_GUIDE_LV6_PAGES 7
#define MAX_GUIDE_LV7_PAGES 4
#define MAX_GUIDE_LV8_PAGES 5

class StrategyGuide : public Control
{
	public:
		int	BgZOrder;
		int StrategyGuideState;

		int activePage, currentPageIndex, maxGuidePages;
		int activeLevel;
		void setPageFilePath();
		
		//Scn Elements
		CObject *BtnClose, *BtnClosePreview;
		CObject *BtnPrevPage, *BtnNextPage;
		CObject *BtnContents;

		CObject *ChapterGlow;

		CObject *PageTextLeft, *PageTextRight;
		CObject *Thumbs[6], *ThumbPreview, *previewMaskBg;

		MultilineTextBox* PageTEXT;
		CBitmapText* PageNo;

		bool isPreviewActive, isPreviewTransitioning, isPreviewZoomIn;

		CHOScene *guideScene;

		std::string pageFilePath;
		int numThumbsInPage;
		
		StrategyGuide();
		~StrategyGuide();

		void  LoadCurrentPage();
		void  Dispose();

		void  SetGuideAlpha(float alpha);

		void  SetActiveGuidePageToCurrentScene();
		void  ShowStrategyGuide();
		void  HideStrategyGuide();

		float easingQuadOut(float t,float b,float c,float d);

		float btnDownTimer;
		float PopupAlpha, PopVal;

		//Inherited method
		void Update();
		void Required();

	protected:
};

#endif