//====================================
// Framework
// EndCutscene Series Control
//====================================

#ifndef FRMWRK_ENDCUTSCENE_DEF
#define FRMWRK_ENDCUTSCENE_DEF

#include "VideoDefs.h"
#include "KVideo.h"

#include "Control.h"

#include "RenderListEntity.h"
#include "LangDefs.h" 

class CObject;
class CBitmapText;

class EndCutsceneSeries : public Control, CRenderListEntity
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

		
		void Render(const CPoint* pCam);


		EndCutsceneSeries();
		~EndCutsceneSeries();

		//Inherited Method
		void Update();
};

#endif