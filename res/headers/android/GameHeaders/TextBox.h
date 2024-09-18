//===========================================================
// 
// Framework
//===========================================================

#ifndef UI_TEXTBOX_H
#define UI_TEXTBOX_H

#define TEXTBOX_KEYS 41

#include "Control.h"
#include "Background.h"
#include "Fonts.h"
#include "BitmapText.h"
#include "MainMenuControl.h"

class MainMenuControl;
class TextBox : public Control
{
	protected:
		MainMenuControl *Parent;
		CBitmapText *BitmapText;
		CObject		*IBeam;

		
		bool IsTextBoxShowing;
		int  CurrentKey;
		float TimeDown;
		bool ShiftDown;
		bool FirstPress;

		#ifdef K_ANDROID
			bool isVirtualKeyBoardShown;
		#endif

		CPoint BGPos;

		bool KeysReleased[TEXTBOX_KEYS];

		void CheckForShift();
		void HandleCurrentKey();
	
	public:
		bool CheckForEnter();

		std::string	String;
		void Clear();		
		TextBox( MainMenuControl *Prnt = NULL );
		virtual ~TextBox();

		//Inherited
		void Update();

		void SetXY(float x, float y);
		void SetZ(int z);

		void Show(bool f);
		void SetAlpha(float a);

};

#endif
