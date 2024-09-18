//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 06 - POPUP 1: STONESANDKNIFE
//====================================

#include "L2S6_Pop1StonesNKnife.h"
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
#include "GameFlow.h"

const CRectangle  InvDestLeverRect1(588.0f,299.0f,412.0f,278.0f);
const CRectangle  InvDestLeverRect2(405.0f,106.0f,181.0f,198.0f);
const CRectangle  InvDestLeverRect3(369.0f,165.0f,148.0f,127.0f);

const CRectangle  InvItemRustedKnifeRect(815.0f,365.0f,139.0f,104.0f);

const CRectangle  InvItemRustedStone0Rect(521.0f,434.0f,70.0f,57.0f);
const CRectangle  InvItemRustedStone1Rect(705.0f,211.0f,61.0f,67.0f);
const CRectangle  InvItemRustedStone2Rect(408.0f,289.0f,73.0f,55.0f);



L2S6_Pop1StonesNKnife::L2S6_Pop1StonesNKnife(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)

{
	//Popup load
	Load("LV2\\S6\\POP1\\P1.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	Show(false);
	Hud = hud;

	for (int i = 0; i < 3; i++)
	{
		std::string str = "Rock";
		str += std::to_string(i);
		rock[i] = GetObjectByID(str);

		str = "stones";
		str += std::to_string(i);
		smallStone[i] = GetObjectByID(str);
	}

	JANIM_stoneAndCrowbar = NULL;
	JANIM_stoneAndCrowbar = new JSONAnimator(0, CC_CALLBACK_1(L2S6_Pop1StonesNKnife::OnAnimationComplete, this ));
	JANIM_stoneAndCrowbar->parseAndLoadJSON("ANIM\\L2\\S6P1ROCKMIVING.JSON", this);

	GetObjectByID("Crowbar")->SetEnable(false);
	GetObjectByID("Crowbar")->SetZ(GetObjectByID("Crowbar")->ZOrder + 100);
	rock[0]->SetZ(rock[0]->ZOrder + 100);

	if (ProfileSystem::Instance->ActiveProfile.L2_S6_LeverUsed)
	{
		rock[0]->SetEnable(false);
	}
	if (ProfileSystem::Instance->ActiveProfile.L2_S6_Rock1Moved)
	{
		rock[1]->SetEnable(false);
	}
	if (ProfileSystem::Instance->ActiveProfile.L2_S6_Rock2Moved)
	{
		rock[2]->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone1Taken)
		smallStone[0]->SetEnable(false);
	if (ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone2Taken)
		smallStone[1]->SetEnable(false);
	if (ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone3Taken)
		smallStone[2]->SetEnable(false);
	if (ProfileSystem::Instance->ActiveProfile.L2S6_InvRustedKnifeTaken)
		GetObjectByID("Knife")->SetEnable(false);
}

void L2S6_Pop1StonesNKnife::Required()
{
}

void L2S6_Pop1StonesNKnife::OnAnimationComplete(int id)
{
	ProfileSystem::Instance->ActiveProfile.L2_S6_LeverUsed = true;

	GetObjectByID("Crowbar")->SetEnable(false);
	float valarray1[5] = {0, rock[0]->Pos.x, rock[0]->Pos.y, 1,0};
	rock[0]->TweenTo(valarray1,5,0,false);


}

void L2S6_Pop1StonesNKnife::Update(float ds)
{
	Popup::Update(ds);

	CPoint pos = Control::Input->Mpos();

	if (Popup::GetPopupState() != ePopupState_Open)
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (!ProfileSystem::Instance->ActiveProfile.L2S6_Pop1Visited)
	{
		ProfileSystem::Instance->ActiveProfile.L2S6_Pop1Visited = true;
		Hud->ShowBannerText("L2hit");
	}

	if (InvItemRustedKnifeRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2S6_InvRustedKnifeTaken)
	{
		if (ProfileSystem::Instance->ActiveProfile.L2_S6_LeverUsed)
		{
			Cursor::SetMode(CA_HandTake);
			if (Control::Input->LBclicked())
			{
				GetObjectByID("Knife")->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L2S6_InvRustedKnifeTaken = true;
				Hud->TakeInventoryItem(eInv_L2S6_KnifeBlunt);
				if (ProfileSystem::Instance->ActiveProfile.l2S6_Inv3XStonesTaken)
					ClosePopup();
			}
			return;
		}
		else if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L2rock");
		}
	}

	if (InvItemRustedStone0Rect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone1Taken)
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			smallStone[0]->SetEnable(false);
			ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone1Taken = true;
			Hud->TakeInventoryItem(eInv_L2S6_SmallStone1, smallStone[0]->Pos);
			if (ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone1Taken && ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone2Taken && ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone3Taken)
				ProfileSystem::Instance->ActiveProfile.l2S6_Inv3XStonesTaken = true;
			if (ProfileSystem::Instance->ActiveProfile.L2S6_InvRustedKnifeTaken && ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone2Taken && ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone3Taken)
				ClosePopup();
		}
		return;
	}

	if (InvItemRustedStone1Rect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone2Taken)
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			smallStone[1]->SetEnable(false);
			ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone2Taken = true;
			Hud->TakeInventoryItem(eInv_L2S6_SmallStone2, smallStone[1]->Pos);
			if (ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone1Taken && ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone2Taken && ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone3Taken)
				ProfileSystem::Instance->ActiveProfile.l2S6_Inv3XStonesTaken = true;
			if (ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone1Taken && ProfileSystem::Instance->ActiveProfile.L2S6_InvRustedKnifeTaken && ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone3Taken)
				ClosePopup();
		}
		return;
	}

	if (InvItemRustedStone2Rect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone3Taken && ProfileSystem::Instance->ActiveProfile.L2_S6_Rock1Moved && ProfileSystem::Instance->ActiveProfile.L2_S6_Rock2Moved)
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			smallStone[2]->SetEnable(false);
			ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone3Taken = true;
			Hud->TakeInventoryItem(eInv_L2S6_SmallStone3, smallStone[2]->Pos);
			if (ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone1Taken && ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone2Taken && ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone3Taken)
				ProfileSystem::Instance->ActiveProfile.l2S6_Inv3XStonesTaken = true;
			if (ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone1Taken && ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone2Taken && ProfileSystem::Instance->ActiveProfile.L2S6_InvRustedKnifeTaken)
				ClosePopup();
		}
		return;
	}

	if (InvDestLeverRect1.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S6_LeverUsed)
	{

		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
		{
			rock[0]->Shake(0,2,0,0.5f);
			Hud->ShowBannerText("L2rock");
			Control::Audio->QuickLoadAndPlaySFX("s6p1_stoneslide1");
		}
	}
	else if (InvDestLeverRect2.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S6_Rock1Moved)
	{

		Cursor::SetMode(CA_HandPointy);
		if (Control::Input->LBclicked())
		{
			//rock[1]->Shake(180, 2, 0, 2);
			float valarray1[5] = {0, rock[1]->Pos.x - 100, rock[1]->Pos.y, 1,0};
			rock[1]->TweenTo(valarray1,3,0,false);
			ProfileSystem::Instance->ActiveProfile.L2_S6_Rock1Moved = true;
			Control::Audio->QuickLoadAndPlaySFX("s6p1_stoneslide2");
		}
	}
	//else if (InvDestLeverRect2.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L2_S6_RocksMoved == 0)
	//{
	//	Cursor::SetMode(CA_Gear);
	//	if (Control::Input->LBclicked())
	//		rock[1]->Shake(0,2,0,0.5f);
	//}
	else if (InvDestLeverRect3.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S6_Rock2Moved && ProfileSystem::Instance->ActiveProfile.L2_S6_Rock1Moved)
	{

		Cursor::SetMode(CA_HandPointy);
		if (Control::Input->LBclicked())
		{
			//rock[2]->Shake(180, 2, 0, 2);
			float valarray1[5] = {0, rock[2]->Pos.x - 100, rock[2]->Pos.y, 1,0};
			rock[2]->TweenTo(valarray1,3,0,false);
			ProfileSystem::Instance->ActiveProfile.L2_S6_Rock2Moved = true;
			Control::Audio->QuickLoadAndPlaySFX("s6p1_stoneslide3");
		}
	}
	else if (InvDestLeverRect3.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S6_Rock1Moved)
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
			rock[2]->Shake(0,2,0,0.5f);
	}
}


float L2S6_Pop1StonesNKnife::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L2S6_Pop1StonesNKnife::~L2S6_Pop1StonesNKnife()
{
	SAFE_DELETE(JANIM_stoneAndCrowbar);
}


void L2S6_Pop1StonesNKnife::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (InvDestLeverRect1.Intersection(&dropPos) && !ProfileSystem::Instance->ActiveProfile.L2_S6_LeverUsed)
	{
		if (itemVal == eInv_L2S4_Lever)
		{
			GetObjectByID("Crowbar")->SetEnable(true);
			JANIM_stoneAndCrowbar->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s6p1_stonelever");

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S4_Lever] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S4_Lever); 
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
	}
}

bool L2S6_Pop1StonesNKnife::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone2Taken)
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone1Taken)
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S6_Rock1Moved)
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S6_Rock2Moved)
		isAnyTaskLeft = true;
	else if(ProfileSystem::Instance->ActiveProfile.L2_S6_Rock2Moved && !ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone3Taken)
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S6_LeverUsed && ProfileSystem::Instance->ActiveProfile.L2_S4_InvMetalLeverTaken)
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L2S6_InvRustedKnifeTaken && ProfileSystem::Instance->ActiveProfile.L2_S6_LeverUsed)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L2S6_Pop1StonesNKnife::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone2Taken)
	{
		Hud->InitHintTrailEmit(InvItemRustedStone1Rect.GetCenterX(), InvItemRustedStone1Rect.GetCenterY());
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone1Taken)
	{
		Hud->InitHintTrailEmit(InvItemRustedStone0Rect.GetCenterX(), InvItemRustedStone0Rect.GetCenterY());
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S6_Rock1Moved)
	{
		Hud->InitHintTrailEmit(InvDestLeverRect2.GetCenterX(), InvDestLeverRect2.GetCenterY());
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S6_Rock2Moved)
	{
		Hud->InitHintTrailEmit(InvDestLeverRect3.GetCenterX(), InvDestLeverRect3.GetCenterY());
	}
	else if(ProfileSystem::Instance->ActiveProfile.L2_S6_Rock2Moved && !ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone3Taken)
	{
		Hud->InitHintTrailEmit(InvItemRustedStone2Rect.GetCenterX(), InvItemRustedStone2Rect.GetCenterY());
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S6_LeverUsed && ProfileSystem::Instance->ActiveProfile.L2_S4_InvMetalLeverTaken)
	{
		Hud->ShowHintForInvItem(eInv_L2S4_Lever, &InvDestLeverRect1);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L2S6_InvRustedKnifeTaken && ProfileSystem::Instance->ActiveProfile.L2_S6_LeverUsed)
	{
		Hud->InitHintTrailEmit(InvItemRustedKnifeRect.GetCenterX(), InvItemRustedKnifeRect.GetCenterY());
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
