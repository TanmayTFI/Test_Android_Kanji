//====================================
// Framework
// StartCutscene Series Control
//====================================

#ifndef FRMWRK_STARTCUTSCENE_DEF
#define FRMWRK_STARTCUTSCENE_DEF

#include "Control.h"
#include "RenderListEntity.h"
#include "LangDefs.h" 
#include "AssetsStringTable.h"

class CObject;
class CBitmapText;

class StartCutsceneSeries : public Control, CRenderListEntity
{
	public:	
	#ifndef ENGLISH_BUILD
		int iSlide;
		int SlideState;		
		void UpdateSlideTxts();
		CBitmapText *SlideTxts[2];
		void CreateSlideTxts();
		#ifdef JAPANESE_BUILD
			const char* JapCineStr( int iSlide, int iRow );
		#endif
	#endif

		void Render(const CPoint* pCam){};
		

		KGraphic *CoreImage, *BG;
		KTrueText* sub;
		
		KGraphic* letterBase, *letterTxt, *gulfTxt;

		float time;
		int subIndex;
		int subMax;
		subText subtitles[6];
		std::string subtitleText;

		StartCutsceneSeries();
		~StartCutsceneSeries();

		//Inherited Method
		void Update();
};

#endif