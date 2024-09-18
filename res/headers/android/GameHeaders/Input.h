//===========================================================
// 
// Framework
//===========================================================

#ifndef INPUT_H
#define INPUT_H

#include "Point.h"
#include "Framework.h"
#include "Cheats.h"

class CInput
{
	public: 
		bool IsCheats; 
		CInput();
		~CInput();

		void LBtnUpdate();
		void Update();

		const CPoint Mpos();

		int   Mwheel();	

		bool prevLBjustdown;
		bool LBdown();
		bool LBjustdown();
		bool RBdown();
		bool RBjustdown();
		bool LBclicked( );
		bool RBclicked( );
		
		// forcefully set the key down & up, currently only used in Android for forcing BACK Button sensing
		void SetKeydown(EKeyboardLayout k );
		void SetKeyup(EKeyboardLayout k );
		bool isKeyBackSensed(); // tells if the Back key is pressed or not


		//Modified key presses
		bool KpressedWithShift	( EKeyboardLayout k );
		bool KpressedWithCtrl	( EKeyboardLayout k );
		#ifdef K_MACOS
        bool KpressedWithCmmd    ( EKeyboardLayout k ); // for mac os, where we need to dedect COMMAND key pressed also
		#endif
		bool Kpressed	( EKeyboardLayout k, bool peek = false );
		int  Kdown		( EKeyboardLayout k );

		void SetProjectComp(int width, int height);
		
		void ClearMouseLeftClick();
		void ClearMouse();
		void ClearAll();
		float ProjComp;
		float XOffset, YOffset;
		
		short LClkCurX, LClkCurY, RClkCurX, RClkCurY;
		int   getLBD(){return LBd;}
	private:
		int LBd;
		int RBd;
		bool LBc;
		bool RBc;
		bool tapped;
		
    // for mac, even 102 is mapped so we should have +1 array limit for mac
    #ifndef K_MACOS
	    int  Kd[ K_VK_ERROR ]; 
		bool Kp[ K_VK_ERROR ];
    #else
        int  Kd[ K_VK_ERROR+1 ];
        bool Kp[ K_VK_ERROR+1 ];
    #endif
};

#endif
