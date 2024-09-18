//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 06 - POPUP 3: LOCKET
//====================================

#include "L2S6_Pop3Locket.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "TextureFactory.h"
#include "ParticleEffect.h"
#include "Rect.h"
#include "Polygon.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "InteractiveObjectList.h"
#include "exMath.h"

const CRectangle  InvItemPoojaLampRect(618.0f,320.0f,234.0f,95.0f);

const CRectangle  LightRect(552.0f,232.0f,47.0f,97.0f);


L2S6_Pop3Locket::L2S6_Pop3Locket(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)

{
	//Popup load
	Load("LV2\\S6\\POP3\\P3.SCN");
	SetFrameAndCloseButton(true);
	SetZOff(zOrder + 100);
	SetPosOff(683.0f, 334.0f);
	Show(false);
	Hud = hud;

	closedLocket = GetObjectByID("LocketPopup_closed");
	openedLocket = GetObjectByID("LocketPopup_opend");

	if (ProfileSystem::Instance->ActiveProfile.L2_S6_IsP2LocketOpen)
	{
		closedLocket->SetEnable(false);
	}
	else
	{
		openedLocket->SetEnable(false);
	}

	int ArrayPts1[] = {576,303,565,355,581,397,614,432,659,451,710,447,719,406,704,355,685,323,650,298,604,287,576,300};
	grandma =  new CPolygon(12);
	grandma->Init(ArrayPts1);

	int ArrayPts2[] = {660,216,649,273,673,308,709,343,760,355,802,329,809,290,790,247,750,214,702,201,662,214};
	grandpa =  new CPolygon(11);
	grandpa->Init(ArrayPts2);

}
L2S6_Pop3Locket::~L2S6_Pop3Locket()
{
	SAFE_DELETE(grandma);
	SAFE_DELETE(grandpa);	
}


void L2S6_Pop3Locket::Required()
{

}

void L2S6_Pop3Locket::Update(float ds)
{
	Popup::Update(ds);

	CPoint pos = Control::Input->Mpos();

	if (Popup::GetPopupState() != ePopupState_Open)
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP2LocketOpen)
	{
		if( closedLocket->GetWorldRect().Intersection(&pos))
		{
			Cursor::SetMode(CA_HandPointy);
			if( Control::Input->LBclicked() )
			{
				closedLocket->SetEnable(false);			
				openedLocket->SetEnable(true);
				openedLocket->Shake(270,2,0,0.5);
				ProfileSystem::Instance->ActiveProfile.L2_S6_IsP2LocketOpen = true;
				Hud->ShowBannerText("L2creek");

				//Journal Entry Add
				if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L2P8_GrandpaMurdered])
				{
					//Flag Set
					ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L2P8_GrandpaMurdered] = true;

					//Add to Note Array
					ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L2P8_GrandpaMurdered;

					ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

					//Increment Notes
					ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
					Hud->ShowJrnEntryFX();
				}

				Control::Audio->QuickLoadAndPlaySFX("s6p3_locketopen");
			}			
			return;		
		}
		/*else if( GetObjectByID("LocketPopup_Bg")->GetWorldRect().Intersection(&pos))
		{
		Cursor::SetMode(CA_Help);

		if (Control::Input->LBclicked())
		{
		Hud->ShowBannerText("I can't waste time trying to figure out the right code. Is there a clue");//Sac: where is the external text for this ?
		}
		}*/
	}
	else
	{
		/*if (openedLocket->GetWorldRect().Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3HolderOpen)
		{
		Cursor::SetMode(CA_Help);

		if (Control::Input->LBclicked())
		{
		Hud->ShowBannerText("I think I know the code");//Sac: where is the external text for this ?
		}
		}*/
		if (grandma->Intersection(&pos))
		{
			Cursor::SetMode(CA_Help);
			if (Control::Input->LBclicked())
				Hud->ShowBannerText("L2grandma");
		}
		else if (grandpa->Intersection(&pos))
		{
			Cursor::SetMode(CA_Help);
			if (Control::Input->LBclicked())
				Hud->ShowBannerText("L2pic");
		}
	}

}

//void L2S6_Pop3Locket::HandleMouseInput(CInput * mouseInput)
//{
//	if(_currentPopupState != ePopupState_Open)
//	{
//		return;
//	}
//
//	if (mouseInput->LBclicked())
//	{
//		if(_allowCloseOutsideArea && !GetObjectByID("LocketPopup_Bg")->GetWorldRect().Intersection(&mouseInput->Mpos()))
//		{
//			ClosePopup();
//		}
//	}
//}

bool L2S6_Pop3Locket::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP2LocketOpen && !ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3HolderOpen )
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L2S6_Pop3Locket::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP2LocketOpen && !ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3HolderOpen )
	{
		Hud->InitHintTrailEmit(closedLocket->GetCenterX(), closedLocket->GetCenterY());
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
