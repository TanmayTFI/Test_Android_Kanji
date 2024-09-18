//====================================
// Framework
// Rocket Control
//====================================

#include "InfoPanel.h"
#include "BitmapText.h"
#include "Background.h"
#include "Point.h"
#include "ProfileSystem.h"
#include "exMath.h"
#include "Rect.h"
#include "Fonts.h"
#include "GameFlow.h"
#include "Background.h"
#include "Point.h"

const CPoint WindowPos(212, 130+82.0f, 0.0f);
const CPoint B1Pos(412.0f+171.0f, 525-82-23.0f, 0.0f);
const CPoint B2Pos(612.0f+171.0f, 525-82-23.0f, 0.0f);

const CPoint CasualButtonPos(412.0f-57-27, 525-82-23.0f-2, 0.0f);
const CPoint ExpertButtonPos(612.0f-31, 525-82-23.0f-2, 0.0f);
const CPoint ChooseDiffSprPos(511,258,0);

const CPoint BCenterPos(512.0f, 525-82-23.0f, 0.0f);

const CPoint WindowLargePos(212, 130.0f, 0.0f);
const CPoint B1LargePos(412.0f, 525-23.0f, 0.0f);
const CPoint B2LargePos(612.0f, 525-23.0f, 0.0f);
const CPoint BCenterLargePos(512.0f, 525-23.0f, 0.0f);

const CPoint AuctionTextOff(40.0f, 110.0f, 0.0f);
const CPoint TextOff(14+24.0f, 110.0f, 0.0f);
const float  YSpacing = 30.0f;



InfoPanel::InfoPanel()
{
	B1Active = false;
	B2Active = false;
	IsPrevActivePopup = false;
	if(!StringTable::IsLanguageMultiByte())
		LineWidth = 530;	
	else
		LineWidth = 700;
	
	Window  = GFInstance->LoadBufferObject(INFO_WIN);//W1
	Window->SetPos(PointSystem::CreateCPoint(1366/2,384-33,0));
	Window->SetZ(25000+31);

	
	WindowLarge  = GFInstance->LoadBufferObject(INFO_WINLRG);//W2
	WindowLarge->SetPos(PointSystem::CreateCPoint(1366/2,384-33,0));
	WindowLarge->SetZ(25000+31);
	WindowLarge->Show(false);

	
	OkButton  = GFInstance->LoadBufferObject(INFO_OK);
	OkButton->SetPos(&B1Pos);
	OkButton->SetZ(25000+32); 

	
	SkipButton  = GFInstance->LoadBufferObject(INFO_SKIP);
	SkipButton->SetPos(&B1Pos);
	SkipButton->SetZ(25000+32);
	SkipButton->Show(false);

	
	
	IsButton1Skip = false;
	IsWindowLarge = false;
	IsOKCancelButtons = false;

	
	CancelButton= GFInstance->LoadBufferObject(INFO_CANCEL);
	CancelButton->SetPos(&B2Pos);
	CancelButton->SetZ(25000+32); 

	CPoint textPos;
	for(int i=0; i < INFO_TEXT_LINES; ++i)
	{
		textPos = WindowPos;
		textPos.x += TextOff.x;
		textPos.y += TextOff.y + 30+ YSpacing * i;

		Text[i] = new CBitmapText();
		Text[i]->SetZ(25000+33);
		Text[i]->SetAlignment(Align_Left);
		Text[i]->SetPos(&textPos);		
		Text[i]->LoadFont(eFont_26);
		Text[i]->SetText("Text Placeholder");
		Text[i]->SetScale(1.0f);
		Text[i]->SetColor((233/255.0f),(204/255.0f),(131/255.0f)); //gold 		
	}	
	Show(false);
}

void InfoPanel::SetTextColor( float r, float g, float b  )
{
	for(int i=0; i < INFO_TEXT_LINES; ++i)
		Text[i]->SetColor((r/255.0f),(g/255.0f),(b/255.0f)); //gold 		
}

void InfoPanel::SetTextScale( float Scale )
{
	for(int i=0; i < INFO_TEXT_LINES; ++i)
		Text[i]->SetScale(Scale);
	if( Scale != 0.0f )
	{
		if(!StringTable::IsLanguageMultiByte())
			LineWidth = (530/Scale);	
		else
			LineWidth = (700/Scale);
	}
}



InfoPanel::~InfoPanel()
{
	//unbuffered objects killed!!
	for(int i=0; i < INFO_TEXT_LINES; ++i)	{
			if(Text[i]!=NULL)
				delete Text[i];
			Text[i] = NULL;
	}

	//buffered objects need to be just turned off!!
	Window->Show(false);Window = NULL;
	OkButton->Show(false);OkButton = NULL;
	SkipButton->Show(false);SkipButton = NULL;
	CancelButton->Show(false);CancelButton = NULL;
	

	//Never unload buffered object since gameflow contains a GFInfopanel whose objects is accessed 
	//from Hud, HiddenObjectsystem and Mainmenucontrol option popups!!
	
}

void InfoPanel::SetWindowLarge()
{
	IsWindowLarge = true;
	CancelButton->SetPos(&B2LargePos);
	Window->Show(false);
	WindowLarge->Show(true);
}

void InfoPanel::SetWindowSmall()
{
	IsWindowLarge = false;
	CancelButton->SetPos(&B2Pos);
	Window->Show(true);
	WindowLarge->Show(false);
}



void InfoPanel::SubUpdate()
{
	CPoint mosPos = Control::Input->Mpos();
	bool click = Control::Input->LBclicked();

	if( !B1Active && !B2Active )//First info window with no buttons
	{
		Cursor::SetMode(CA_Normal);
		bool IsHover = false;
		if( IsWindowLarge )
			IsHover = WindowLarge->Collide(&mosPos);
		else
			IsHover = Window->Collide(&mosPos);

		if( IsHover )//for click no need for cursor as user maybe outside window region!!		
			Cursor::SetMode(CA_HandPointy);

		//Help user to click the popup faster - as his intention is clear!!
		if( click )
		{
			Control::Audio->PlaySample(aSFXClick);	
			SendMessage(ActionProcessed, ID, INFO_B1);
		}
		
		return;
	}
	
	
	
	if( IsOKCancelButtons )
	{
		Cursor::SetMode(CA_Normal);

		if(OkButton->Collide(&mosPos))
		{
			Cursor::SetMode(CA_HandPointy);
			if(click)
			{
				OkButton->PlayAnimation(2);
				Control::Audio->PlaySample(aSFXClick);	
				SendMessage(ActionProcessed, ID, INFO_B1);
				return;
			}
			else
			{
				if( OkButton->GetAnimation() != 1)
				{
					Control::Audio->PlaySample(aSFXMouseover);
					OkButton->PlayAnimation(1);
				}				
			}
		}
		else
			OkButton->PlayAnimation(0);

		if(CancelButton->Collide(&mosPos))
		{
			Cursor::SetMode(CA_HandPointy);
			if(click)
			{
				CancelButton->PlayAnimation(2);
				Control::Audio->PlaySample(aSFXClick);	
				SendMessage(ActionProcessed, ID, INFO_B2);
				return;
			}
			else
			{
				if( CancelButton->GetAnimation() != 1)
				{
					Control::Audio->PlaySample(aSFXMouseover);
					CancelButton->PlayAnimation(1);
				}
			}
		}
		else
			CancelButton->PlayAnimation(0);		
		
		return;
	}

	if( IsButton1Skip )
		SkipButton->PlayAnimation(0);
	else
		OkButton->PlayAnimation(0);
	CancelButton->PlayAnimation(0);

	Cursor::SetMode(CA_Normal);
	if( IsButton1Skip )
	{
		if(SkipButton->Collide(&mosPos))
		{
			Cursor::SetMode(CA_HandPointy);
			if(click)
			{
				SkipButton->PlayAnimation(2);
				Control::Audio->PlaySample(aSFXClick);	
				SendMessage(ActionProcessed, ID, INFO_B1);
				return;
			}
			else
			{
				if( SkipButton->GetAnimation() != 1)
				{
					Control::Audio->PlaySample(aSFXMouseover);
					SkipButton->PlayAnimation(1);
				}				
			}
		}
		else
			SkipButton->PlayAnimation(0);
	}
	else if( B1Active )
	{
		if( OkButton->Collide(&mosPos) )
		{
			Cursor::SetMode(CA_HandPointy);
			if(click)
			{
				OkButton->PlayAnimation(2);
				Control::Audio->PlaySample(aSFXClick);	
				SendMessage(ActionProcessed, ID, INFO_B1);
				return;
			}
			else
			{
				if( OkButton->GetAnimation() != 1)
				{
					Control::Audio->PlaySample(aSFXMouseover);
					OkButton->PlayAnimation(1);
				}				
			}
		}
		else
			OkButton->PlayAnimation(0);
	}
	else //B1 not active
	{
		Cursor::SetMode(CA_HandPointy);				

		if( click )
		{			
			Control::Audio->PlaySample(aSFXClick);	
			SendMessage(ActionProcessed, ID, INFO_B1);
			return;
		}
		
	}

	if(B2Active && CancelButton->Collide(&mosPos))
	{		
		Cursor::SetMode(CA_HandPointy);
		if(click)
		{
			CancelButton->PlayAnimation(2);
			Control::Audio->PlaySample(aSFXClick);	
			SendMessage(ActionProcessed, ID, INFO_B2);
			return;
		}
		else
		{
			if( CancelButton->GetAnimation() != 1)
			{
				Control::Audio->PlaySample(aSFXMouseover);
				CancelButton->PlayAnimation(1);
			}	
		}
	}
	else
		CancelButton->PlayAnimation(0);
}


void InfoPanel::SetButtons(std::string b1, std::string b2) //b2 = "none" by default
{
	IsPrevActivePopup = GFInstance->IsActivePopup;
	GFInstance->IsActivePopup = true;
	B1Active = false;
	B2Active = false;
	IsButton1Skip = false;
	IsOKCancelButtons = false;
	if( b1 == "Ok" && b2 == "Cancel" )
	{
		B1Active = true;
		B2Active = true;
		IsOKCancelButtons = true;	
		SkipButton->Show(false);

		OkButton->SetPos(&B1Pos);
		CancelButton->SetPos(&B2Pos);
		OkButton->SetAlpha(0.0f);
		CancelButton->SetAlpha(0.0f);		
		OkButton->Show(true);
		CancelButton->Show(true);		
		return;
	}

	
	if( b1 == "Skip" )
	{
		OkButton->Show(false);
		SkipButton->Show(true);
		CancelButton->Show(true);
		IsButton1Skip = true;
	}
	else
		SkipButton->Show(false);

	if( b2 != "none" ) 
	{
		B1Active = true;
		B2Active = true;
		if( IsButton1Skip )
		{
			if( IsWindowLarge )
				SkipButton->SetPos(&B1LargePos);
			else
				SkipButton->SetPos(&B1Pos);
		}
		else
		{
			if( IsWindowLarge )
				OkButton->SetPos(&B1LargePos);
			else
				OkButton->SetPos(&B1Pos);
		}
	}
	else // b2 == none
	{
		B2Active = false;
		CancelButton->Show(false);
		if( IsButton1Skip )
		{
			B1Active = true;
			if( IsWindowLarge )
				SkipButton->SetPos(&BCenterLargePos);
			else
				SkipButton->SetPos(&BCenterPos);
		}
		else
		{
			if( b1 == "none" ) 
			{
				B1Active = false;
				OkButton->Show(false);				
			}
			else
			{
				B1Active = true;
				if( IsWindowLarge )
					OkButton->SetPos(&BCenterLargePos);			
				else
					OkButton->SetPos(&BCenterPos);			
			}
		}
	}
}
void InfoPanel::Show(bool f)
{
	if( IsWindowLarge )
		WindowLarge->Show(f);
	else
		Window->Show(f);

	if( IsOKCancelButtons )
	{
		OkButton->Show(f);
		CancelButton->Show(f);			
	}
	else
	{
		if( IsButton1Skip )
			SkipButton->Show(f);
		else
		{
			if( B1Active )
				OkButton->Show(f);
			else
				OkButton->Show(false);
		}
		if( B2Active )
			CancelButton->Show(f);
		else
			CancelButton->Show(false);
	}

	for(int i=0; i < INFO_TEXT_LINES; ++i)
	{
		Text[i]->Show(f);
	}
}

void InfoPanel::ExternalClose()
{
	GFInstance->IsActivePopup = IsPrevActivePopup;
	ACState = ACS_TransOut;
}

void InfoPanel::Close()
{
	GFInstance->IsActivePopup = IsPrevActivePopup;
	SetModal(false);
	
	SendMessage(ActionProcessed, ID, INFO_CLOSE);
	//for(int i=0; i < INFO_TEXT_LINES; ++i)
	//	Text[i]->SetScale(1.0f);
}

void InfoPanel::SetAlpha(float f)
{
	if( IsWindowLarge )
		WindowLarge->SetAlpha(f);
	else
		Window->SetAlpha(f);

	if( IsOKCancelButtons )
	{
		OkButton->SetAlpha(f);
		CancelButton->SetAlpha(f);		
	}
	else
	{
		if( IsButton1Skip )
			SkipButton->SetAlpha(f);
		else if( B1Active )		
			OkButton->SetAlpha(f);

		if( B2Active )
			CancelButton->SetAlpha(f);
	}

	for(int i=0; i < INFO_TEXT_LINES; ++i)
	{
		Text[i]->SetAlpha(f);
	}

}



void InfoPanel::SetInfoText(std::string bigstring, int OffsetY, eTextAlignment AlignStyle, int OffsetX)
{
	if(!bigstring.empty()) bigstring = StringTable::getStr(bigstring);
	std::vector<std::string> Strings;
	std::string CurString = "";
	std::string CurWord = "";
	int CurWidth = 0;
	int WordLength = 0;
//#ifdef JAPANESE_BUILD 
	unsigned int Char; // Everything is treated as Unicode
//#else
//	unsigned char Char;
//#endif
	//Hide all current strings
	CPoint textPos;
	for(int i=0; i < INFO_TEXT_LINES; ++i)
	{
		Text[i]->SetText(" ");

		if( IsWindowLarge )
			textPos = WindowLargePos;
		else
			textPos = WindowPos;
		textPos.x += OffsetX + TextOff.x + 265+171;

		if( AlignStyle == Align_Left )
			textPos.x -= 260;

		if( IsWindowLarge )
			textPos.y += TextOff.y + OffsetY -15 + YSpacing * i;
		else
			textPos.y += TextOff.y + OffsetY -50 + YSpacing * i;
		Text[i]->SetPos(&textPos);		
		Text[i]->SetAlignment(AlignStyle);
	}

	//Split String
	int length = (int)bigstring.length();
	int CharWidth = 0;
	for(int i=0; i < length; ++i)
	{
		Char = bigstring.at(i);//japanese will have multibyte characters!!
//#ifdef JAPANESE_BUILD
	if(!StringTable::IsMyLangaugeLatin())
		CharWidth = 0;//We split words based on $ sign only!! :) 
	else
//#else
		CharWidth = Text[0]->charWidth(Char);
//#endif
		if( Char == '\\'  || Char == '$') //for japanese $ and for lattin 
		{
			CurWidth = 0;
			Strings.push_back(CurString);

			CurString = "";
		}
		//If current word cannot fit on the line
		//add line, clear line, and add char to current word
		else if( (CharWidth + CurWidth + WordLength) > LineWidth )
		{
			CurWidth = 0;
			Strings.push_back(CurString);

			CurString = "";

			CurWord += Char;
			WordLength += CharWidth;			
		}
		else
		{
			WordLength += CharWidth;
			CurWord += Char;

			//Hit a space or end of string, add a word
			if( Char == 32 || i == (length - 1))
			{
				CurWidth += WordLength;
				WordLength = 0;

				CurString += CurWord;
				CurWord = "";

				//Add last string
				if( i == (length - 1) )
				{
					Strings.push_back(CurString);
				}
			}
		}
	}
	int size = (int)Strings.size();  
	if(size >= INFO_TEXT_LINES)
	{
		Text[0]->SetText("String is too big.");
	}
	else
	{
		int size = (int)Strings.size();
		for(int i=0; i < size; ++i)
		{
			if(size == 1)  // Sac: if we have only 1 or 2 lines, then push the line to center of the display box
				Text[i+(INFO_TEXT_LINES/2)]->SetText(Strings[i]);
			else	
				Text[i]->SetText(Strings[i]);			
		}
	}
}
