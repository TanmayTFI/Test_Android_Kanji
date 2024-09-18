//====================================
// Framework
// Rocket Control
//====================================

#ifndef DQFULP_INFOPANEL_DEF
#define DQFULP_INFOPANEL_DEF

#include "AlphaControl.h"
#include "BitmapText.h"
#include "Constants.h"


#define INFO_CLOSE		90000
#define INFO_B1			90001
#define INFO_B2			90002
#define INFO_TEXT_LINES 11

class CObject;
class CBackground;
class CBitmapText;

class InfoPanel : public AlphaControl
{
	public:
		float  LineWidth;

		InfoPanel();
		virtual ~InfoPanel();

		void ExternalClose();
		
		bool IsButton1Skip;
		bool IsWindowLarge;
		void SetInfoText(std::string bigstring, int OffsetY = 0,eTextAlignment AlignStyle = Align_Center, int OffsetX = 0);
		void SetButtons(std::string b1 = "none", std::string b2 = "none");
		void SetWindowLarge();
		void SetWindowSmall();
		bool IsOKCancelButtons;
		void SetTextColor( float r, float g, float b  );
		void SetTextScale( float Scale );
		

		
		void Show(bool f);
		CBitmapText   *Text[INFO_TEXT_LINES];
		bool B1Active, B2Active;
	protected:
		CObject 	  *Window, *WindowLarge;
		CObject		  *OkButton;
		CObject		  *SkipButton;
		CObject		  *CancelButton;

		
	


		
		bool IsPrevActivePopup;
		

		//Inherited Methods
		void SubUpdate();
		void Close();
		
		void SetAlpha(float f);
};

#endif