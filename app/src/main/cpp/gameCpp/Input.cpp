//===========================================================
// 
// Framework
//===========================================================

#include "Input.h"
#include "GameFlow.h"
#include "Hud.h"
#include "Util.h"
#include "Application.h"

CInput::CInput()
{
	ProjComp = 1.0f;
	XOffset = 0;
	YOffset = 0;	
	ClearAll();
	
	#ifdef _CHEATS
	{
		IsCheats = true;
	}
	#else
	{
		IsCheats = false;
	}
	#endif 
}

CInput::~CInput()
{
}

void CInput::SetProjectComp(int width, int height)
{
	//same scale - clip
	XOffset = 0;
	YOffset = 0;
	ProjComp = 1.0f;	
/*	
	if( width == 1366 || width == 1360 || width == 1280 || width == 1024 || width == 1152 )
	{
		ProjComp = 1.0f;	
		if( width == 1366 )
		{
			if( height == 768 )
            {
				XOffset = 0;
                YOffset = 0;
            }
            else //height == 640
            {
                XOffset = 0;
                YOffset = 0;
                ProjComp = 1.2f;
            }
		}
		else if( width == 1360 )
		{
			XOffset = (1366-1360)/2;
			if( height == 768 )
            {
                YOffset = -20;
				ProjComp = 1.0f;
            }
		}
		else if( width == 1280 )
		{
			if( height == 768 )
			{
				XOffset = (1366-1280)/2;
				ProjComp = 0.98f;
				YOffset = 0;
			}
            else if( height == 800 )
            {
                XOffset = (1366-1280)/2;
                YOffset = -24;
                ProjComp = 0.96f;
            }
			else if( height == 720 )  //height == 720
			{
			//	XOffset = (1366-1280)/2;
				XOffset = 0;
				//YOffset = -24;
			//	ProjComp = 1.067f;
				ProjComp = 1.067f;
				YOffset = -24;
			}
            else //height == 640
            {
                XOffset = 0;
                YOffset = 0;
                ProjComp = 1.2f;
            }
		}
		else if( width == 1024 )
		{
			//XOffset = (1366-1024)/2;
			//XOffset = 270;
			XOffset = 170;
            if( height == 768 )
            {
				//YOffset = 0;
				ProjComp = 1.0f;
                YOffset = 0;
            }
            else //height == 640
            {
                XOffset = 60;
                YOffset = 0;
                ProjComp = 1.2f;
            }
		}		
		else if( width == 1152 )
		{
			//XOffset = (1366-1024)/2;
			XOffset = 40;
         	 if( height == 720 )  //height == 720
			{
				ProjComp = 1.067f;
				YOffset = -24;
			}
		}		
	
	}
	else if( width == 1920 )
	{
		if(height==1080)
		{
			ProjComp = 0.71111111f;
			//	XOffset = (1366-1920)/2;
			//YOffset = 4;
            YOffset = 0;
		}
		if(height==1200)
		{
			//XOffset = 100;
            XOffset = 80;
			ProjComp = 0.64f;
			//	XOffset = (1366-1920)/2;
			YOffset = 0;
		}
	}
	else if( width == 2560 )
	{
		ProjComp = 0.533f;
		XOffset = 10;
		YOffset = -58;
	}
	else if( width == 3840 )
	{
		ProjComp = 0.355f;
		XOffset = 10;
		YOffset = -60;
	}
	else if( width == 7680 )
	{
		ProjComp = 0.177f;
		XOffset = 24;
		YOffset = -70;
	}

	//downscale
	else if( width == 800 )
	{
		//ProjComp = 1.28f;
		//XOffset = (1366-1024)/2;
		//XOffset = 270;
		XOffset = 180;
		YOffset = 10;
		//ProjComp = 1.24f;
		ProjComp = 1.28f;
	}
	else if( width == 640 )
	{
		//ProjComp = 1.6f;
		//XOffset = (1366-1024)/2;
		//XOffset = 270;
		XOffset = 180;
		YOffset = 10;
		ProjComp = 1.56f;
	}
*/

	ProjComp = MAX_DISPLAY_HEIGHT/height;
	XOffset = (MAX_DISPLAY_WIDTH - (ProjComp*width))/2;
	YOffset = (MAX_DISPLAY_HEIGHT - (ProjComp*height))/2;

}

void CInput::ClearMouseLeftClick()
{
	LBd = 0;
	LBc = false;
	prevLBjustdown = false;	
}

void CInput::ClearMouse()
{
	LBd = 0;
	RBd = 0;
	LBc = false;
	RBc = false;
	prevLBjustdown = false;
	LClkCurX = -1;
	LClkCurY = -1;
	RClkCurX = -1;
	RClkCurY = -1;
       
}

void CInput::ClearAll()
{
	//Clear mouse
	LBd = 0;
	RBd = 0;
	LBc = false;
	RBc = false;
	prevLBjustdown = false;
	LClkCurX = -1;
	LClkCurY = -1;
	RClkCurX = -1;
	RClkCurY = -1;

   int totalKeys = (int)K_VK_ERROR;
#ifdef K_MACOS
    totalKeys +=1;
#endif
    
	//Clear keyboard
	for(int i=0; i < totalKeys; ++i)
	{
		Kd[i] = 0;
		Kp[i] = false;
	}
}

#if ( defined  K_iOS) || ( defined  K_ANDROID)
    #define CLK_GAP 96
#else
    #define CLK_GAP 8
#endif

void CInput::Update()
{
	//================================
	// Clear clicks and presses
	//================================
	LBc = false;
	RBc = false;
	prevLBjustdown = (LBd == 0)?false:true;

#ifdef TOUCH_UI
    if(GFApp->touched)
    {
        LBd++;
        tapped = true;
    }
    else
    {
        if(LBd)
        {
            LBc = true;
            //LBc = GFApp->_tapped;
        }
        tapped = false;
        LBd = 0;
    }
    //LBc = GFApp->_tapped;
    
    GFApp->debugMsg = "tapped=" + to_string(GFApp->_tapped) + "touched" + to_string(GFApp->touched) +"time=" + to_string(GFApp->touchTime) +"LBd=" + to_string(LBd) + "LBc=" + to_string(LBc);
#else
	//================================
	// Check for clicks
	//================================
	//Left mouse button
	if( KInput::getLeftButtonState() == ISDOWN )
	{
		LBd++;
		if( LBd == 1 )//First click save cursor coordinates
		{
			LClkCurX = KInput::getMouseX();
			LClkCurY = KInput::getMouseY();
		}
	}
	else
	{
		if( LBd )//CLK_GAP pixel offset allowed!!
		{
			if( abs(LClkCurX - KInput::getMouseX()) < CLK_GAP &&
				abs(LClkCurY - KInput::getMouseY()) < CLK_GAP )
				LBc = true;
		}

		LBd = 0;
	}
#endif
	//Right mouse button
	if( KInput::getRightButtonState() == ISDOWN )
	{
		RBd++;
		if( RBd == 1 )//First click save cursor coordinates
		{
			RClkCurX = KInput::getMouseX();
			RClkCurY = KInput::getMouseY();
		}
	}
	else
	{
		if( RBd )//CLK_GAP pixel offset allowed!!
		{
			if( abs(RClkCurX - KInput::getMouseX()) < CLK_GAP &&
				abs(RClkCurY - KInput::getMouseY()) < CLK_GAP )
				RBc = true;
		}
		RBd = 0;
	}

	//Keyboard textentry support + cheats section!!
	if( GFInstance != NULL )
		if( GFInstance->Flow == fRootMenu || IsCheats )
		{
            int totalKeys = (int)K_VK_ERROR;
            #ifdef K_MACOS
                totalKeys +=1;
            #endif
			for(int i=0; i < totalKeys; ++i)
				Kp[i] = false;

			//================================
			// Check for presses
			//================================
			for(int i=0; i < totalKeys; ++i)
			{
				if( KInput::isPressed( EKeyboardLayout(i) ) == ISDOWN)
					{
						/*
						if(GFHud !=NULL) {
							GFHud->ShowBannerDirectText(
									"Key Pressed -" + MKSTR(EKeyboardLayout(i)));
						}
						*/
						Kd[i] = true;
					}
				else
				{
					if( Kd[i] )
						Kp[i] = true;

					Kd[i] = false;
				}
			}
		}
}

void CInput::SetKeydown(EKeyboardLayout k )
{
	Kd[(int)k] = true;
}
void CInput::SetKeyup(EKeyboardLayout k )
{
	Kd[(int)k] = false;
}
bool CInput::isKeyBackSensed()
{
	return Kd[(int)K_VK_PRIOR] || Kp[(int)K_VK_PRIOR];

}


void CInput::LBtnUpdate()
{
	//================================
	// Clear clicks and presses
	//================================
	LBc = false;
	prevLBjustdown = (LBd == 0)?false:true;

#ifdef TOUCH_UI
    if(GFApp->touched)
    {
        LBd++;
        tapped = true;
    }
    else
    {
        tapped = false;
        LBd = 0;
    }
    LBc = GFApp->_tapped;
#else
	//================================
	// Check for clicks
	//================================
	//Left mouse button
	if( KInput::getLeftButtonState() == ISDOWN )
	{
		LBd++;
		if( LBd == 1 )//First click save cursor coordinates
		{
			LClkCurX = KInput::getMouseX();
			LClkCurY = KInput::getMouseY();
		}
	}
	else
	{
		if( LBd )//CLK_GAP pixel offset allowed!!
		{
			if( abs(LClkCurX - KInput::getMouseX()) < CLK_GAP &&
				abs(LClkCurY - KInput::getMouseY()) < CLK_GAP )
				LBc = true;
		}

		LBd = 0;
	}
#endif
}

//Modified key presses
bool CInput::KpressedWithShift	( EKeyboardLayout k )
{
	if(Kd[K_VK_L_SHIFT] || Kd[K_VK_R_SHIFT])
		return Kp[k];
	else
		return false;
}

bool CInput::KpressedWithCtrl	( EKeyboardLayout k )
{
	if(Kd[K_VK_L_CONTROL] || Kd[K_VK_R_CONTROL])
		return Kp[k];
	else
		return false;
}
#ifdef K_MACOS
bool CInput::KpressedWithCmmd    ( EKeyboardLayout k ) // Command key in MACOS
{
    if(Kd[K_VK_MENU] || Kd[K_VK_ERROR]) // Somehow kanji maps Left command to K_VK_MENU which is 77 and Right Command as K_VK_ERROR which is 102
        return Kp[k];
    else
        return false;
}
#endif
bool CInput::Kpressed( EKeyboardLayout k, bool peek)
{
	return Kp[k];
}

int CInput::Kdown( EKeyboardLayout k )
{
	return Kd[k];
}

bool CInput::LBdown()
{
	if(KInput::getMouseY() < 0)
		return false;

	return (LBd > 0);
}

bool CInput::RBdown()
{
	if(KInput::getMouseY() < 0)
		return false;

	return (RBd > 0);
}

bool CInput::LBclicked( )
{
#ifndef TOUCH_UI
	if(KInput::getMouseY() < 0)
		return false;
#endif
	return LBc;
}

bool CInput::RBclicked( )
{
	if(KInput::getMouseY() < 0)
		return false;

	return RBc;	
}

bool CInput::LBjustdown()
{
	if(KInput::getMouseY() < 0)
		return false;

	return (LBd == 1);
}

bool CInput::RBjustdown()
{
	if(KInput::getMouseY() < 0)
		return false;

	return (RBd == 1);
}

const CPoint CInput::Mpos()
{
	float x = ProjComp*(KInput::getMouseX()-5);
	float y = ProjComp*(KInput::getMouseY()-3);
	x += XOffset;
	y += YOffset;

#ifdef TOUCH_UI
    return CPoint(GFApp->touchPosX, GFApp->touchPosY, 0.0f);
#else
    return CPoint(x, y, 0.0f);
#endif
}

int CInput::Mwheel()
{
	return int(KInput::getWheelValue());
}
