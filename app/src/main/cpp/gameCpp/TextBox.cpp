//===========================================================
// 
// Framework
//===========================================================



#include "Application.h"
#include "TextBox.h"
#include "ProfileSystem.h"
#include "LangDefs.h"

#define ENTRY_OFFX 50

#ifdef TOUCH_UI
    #define ENTRY_OFFY -85
#else
    #define ENTRY_OFFY 17
#endif

#define LOWERCASE_OFFSET (97 - 3)
#define UPPERCASE_OFFSET (65 - 3)
#define NUMERAL_OFFSET (48 - 29)
#define MAX_LETTERS 40

extern bool isTextBoxEnterKeyPressed;


static int TextBoxKeys[TEXTBOX_KEYS] =
{
	K_VK_BACK,
	K_VK_DEL,
	K_VK_SPACE, 
	K_VK_A,
	K_VK_B,
	K_VK_C,
	K_VK_D,
	K_VK_E,
	K_VK_F,
	K_VK_G,
	K_VK_H,
	K_VK_I,
	K_VK_J,
	K_VK_K,
	K_VK_L,
	K_VK_M,
	K_VK_N,
	K_VK_O,
	K_VK_P,
	K_VK_Q,
	K_VK_R,
	K_VK_S,
	K_VK_T,
	K_VK_U,
	K_VK_V,
	K_VK_W,
	K_VK_X,
	K_VK_Y,
	K_VK_Z,
	K_VK_0,
	K_VK_1,
	K_VK_2,
	K_VK_3,
	K_VK_4,
	K_VK_5,
	K_VK_6,
	K_VK_7,
	K_VK_8,
	K_VK_9,
	K_VK_OEM_MINUS,
	K_VK_OEM_PERIOD
};

void TextBox::Clear()
{
	String.clear();//String = "";
}

TextBox::TextBox( MainMenuControl *Prnt )
{
	Parent = Prnt;
	IsTextBoxShowing = false;
	
	//String = "";
	//String.clear();

	CurrentKey = -1;
	TimeDown = 0.0f;
	ShiftDown = false;
	FirstPress = false;

	for(int i=0; i < TEXTBOX_KEYS; ++i)
	{
		KeysReleased[i] = true;
	}

	BitmapText = NULL;
	IBeam = NULL;

	BitmapText = new CBitmapText();
	BitmapText->LoadFont(eFont_26);
	BitmapText->SetColor((233/255.0f),(204/255.0f),(131/255.0f));
	BitmapText->SetAlignment(Align_Center);

	IBeam = new CObject();
	IBeam->LoadSprite("UI\\IB.SPR");
	IBeam->PlayAnimation(0);
	IsTextBoxShowing = true;
}

TextBox::~TextBox()
{
	delete BitmapText; BitmapText = NULL;
	delete IBeam; IBeam = NULL;
}


void TextBox::SetXY(float x, float y)
{
	CPoint loc = CPoint(x,y,0);
    BGPos = loc;
	
	loc.x += ENTRY_OFFX;
	loc.y += ENTRY_OFFY;
	BitmapText->SetPos(&loc);

	loc.x += BitmapText->StringWidth()/2 + 4;
	loc.y -= 11.0f;
	IBeam->SetPos(&loc);
}

void TextBox::SetZ(int z)
{
	BitmapText->SetZ(z+1);
	IBeam->SetZ(z+2);
}

void TextBox::Update()
{
	if( !IsTextBoxShowing )
		return;
	if( BitmapText == NULL || IBeam == NULL )
		return;

	TimeDown += Control::LogicRate;

	
	if( String.length() > 1 )
		if( CheckForEnter() ) //There must be some characters in a profile!!
		{				
			IsTextBoxShowing = false;
			//SendMessage(ControlExit, ID, 1);
			isTextBoxEnterKeyPressed = true;
			return;
		}

	CheckForShift();
	
	bool UsePrevKey = false;
	int PrevKey = CurrentKey;
	CurrentKey = -1;

	//Check keys
	for(int i=0; i < TEXTBOX_KEYS; ++i)
	{
		if( Control::Input->Kdown( (EKeyboardLayout)TextBoxKeys[i] ) )
		{
			#ifdef K_ANDROID
				char szLogMessage[1024];
   		         KStringFmt (szLogMessage, 1024,"is %d", TextBoxKeys[i]);
				KMiscTools::alertBox ("Key Pressed!!", szLogMessage, "OK", NULL);
			#endif
			//Allow key to stomp current key
			if( i == PrevKey )
			{
				UsePrevKey = true;
			}
			//Newkey must've been released prior to recalling
			else if( KeysReleased[i]  )
			{
				TimeDown = 0.0f;
				CurrentKey = i;
				FirstPress = true;
				KeysReleased[i] = false;

				HandleCurrentKey();
			}
		}
		else
			KeysReleased[i] = true;
	}

	//If we have a valid current key, that is still held, 
	//and wasn't stomped. Handle repeating.
	if( UsePrevKey && (CurrentKey == -1) )
	{
		CurrentKey = PrevKey;

		if( (FirstPress && (TimeDown >= 0.5f)) || 
			(!FirstPress && (TimeDown >= 0.05f)) )
		{
			FirstPress = false;
			TimeDown = 0.0f;
			HandleCurrentKey();
		}
	}

	CPoint pos(BGPos.x + ENTRY_OFFX, BGPos.y + ENTRY_OFFY, 0);
	
	pos.x += BitmapText->StringWidth()/2 + 8;
	pos.y -= 0;//11.0f;

    //BitmapText->SetText(String);
	#ifdef K_ANDROID
		BitmapText->SetText(GFApp->lpUIEditField->getText());
		CPoint mosPos = Control::Input->Mpos();
        if(mosPos.x >= 520 && mosPos.x <= 920 && mosPos.y >= 230 && mosPos.y <= 300)
        {
            if(Control::Input->LBclicked())
                GFApp->lpUIEditField->setFocus();
        }
    #endif

    #ifdef K_iOS
        if(GFApp->lpTextField != NULL)
        {
            BitmapText->SetText(GFApp->lpTextField->getText());
        }
        CPoint mosPos = Control::Input->Mpos();
        if(mosPos.x >= 520 && mosPos.x <= 920 && mosPos.y >= 230 && mosPos.y <= 300)
        {
            if(Control::Input->LBclicked())
                GFApp->lpTextField->setFocus();
        }
    #endif
    
	IBeam->SetPos(&pos);
}

void TextBox::CheckForShift()
{
	if( Control::Input->Kdown(K_VK_R_SHIFT) || 
		Control::Input->Kdown(K_VK_L_SHIFT) )
	{
		ShiftDown = true;
	}
	else
	{
		ShiftDown = false;
	}
}

void TextBox::HandleCurrentKey()
{
	if( CurrentKey == 0 ||
		CurrentKey == 1)
	{
		if( String.length() < 1 )
			return;

		if( !Control::Audio->IsPlaying(aSFXClick) )
			Control::Audio->PlaySample(aSFXClick);
		String.erase(--String.end());
	}
	else
	{
		int strlen = (int)String.length(); //Sac: converted Implicit to explicit
		if( (strlen < MAX_LETTERS) && (BitmapText->StringWidth() < (155/Control::Input->ProjComp)) )
		{
			if(CurrentKey == 2)
			{
				if( strlen == 0  )//first character must not be space!!
					Control::Audio->PlaySample(aSFXNotifySoft);
				else
				{
					String += ' ';
					Control::Audio->QuickLoadAndPlaySFX(aSFXMnuProfNameType);						
				}
			}
			else if(CurrentKey > 28)
			{
				
				Control::Audio->QuickLoadAndPlaySFX(aSFXMnuProfNameType);

				int ShiftModifier = NUMERAL_OFFSET;
				String += (unsigned char)(CurrentKey + ShiftModifier);
			}
			else
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXMnuProfNameType);

				int ShiftModifier = (ShiftDown ) ? UPPERCASE_OFFSET : LOWERCASE_OFFSET;
				String += (unsigned char)(CurrentKey + ShiftModifier);
			}
		}
	}

	
	BitmapText->SetText(String);
}

bool TextBox::CheckForEnter()
{
	if( Control::Input->Kdown(K_VK_RETURN) )
	{
		for( int i = 0; i < MAX_PROFILES; i++ )
			if( i != ProfileSystem::Instance->ActiveProfile._iActiveProfileSlot )//no need to compare with ourself!!
				if( ProfileSystem::Instance->ProfileName[i] == String && Parent != NULL)//Name already present
				{
						Parent->ShowBannerText("Exname");				
					//KMiscTools::messageBox((const char*)L"Invalid profile name",(const char*)L"Name already being in use!");
					//KInput::hidePointer();//since user may see this before window creation - the system cursor will follow to game!!
					Control::Audio->PlaySample(aSFXNotifySoft);
					return false;
				}
				
		Control::Audio->QuickLoadAndPlaySFX(aSFXMnuProfNameType);
		return true;
	}

	return false;
}

void TextBox::Show(bool f)
{
	IsTextBoxShowing = f;
	BitmapText->Show(f);
	IBeam->Show(f);
}

void TextBox::SetAlpha(float a)
{
	BitmapText->SetAlpha(a);
	IBeam->SetAlpha(a);
}

