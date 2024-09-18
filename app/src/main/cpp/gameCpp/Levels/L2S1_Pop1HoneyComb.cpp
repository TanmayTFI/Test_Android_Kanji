//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 01 - POPUP 1: Honey COMB
//====================================

#include "L2S1_Pop1HoneyComb.h"
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


const CRectangle  InvDestRectFrankinsmoke(401.0f,140.0f,537.0f,392.0f);
const CRectangle  InvItemRectHoneyComb(542.0f,356.0f,147.0f,88.0f);
const CRectangle  HoneyCombRect(528.0f,237.0f,225.0f,180.0f);
const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);


L2S1_Pop1HoneyComb::L2S1_Pop1HoneyComb(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV2\\S1\\POP1\\P1.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	Show(false);
	showfrankinscene = 0;

	Hud = hud;

	frankinscence = GetObjectByID("smoke_inv");
	frankinscence->SetMaskRect(PopAreaRect);
	frankinscence->SetEnable(false);
	KnifeL = GetObjectByID("KnifeL");
	KnifeL->SetEnable(false);
	KnifeR = GetObjectByID("KnifeR");
	KnifeR->SetEnable(false);
	HoneycombCut = GetObjectByID("HoneyComp_CutPiece");
	HoneycombMask = GetObjectByID("HoneyComp_Cut");
	HoneybeeSetL = GetObjectByID("HoneyBeeSet03");
	HoneybeeSetR = GetObjectByID("HoneyBeeSet02");

	JAnim_HoneyCombCut = new JSONAnimator(0, CC_CALLBACK_1(L2S1_Pop1HoneyComb::OnAnimationComplete, this));
	JAnim_HoneyCombCut->parseAndLoadJSON("ANIM\\L2\\S1HONEYCOMBCUT.JSON", this);

	if (ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked)
	{
		HoneybeeSetL->SetEnable(false);
		HoneybeeSetR->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSliced)
	{
		HoneycombCut->SetEnable(false);

		if (ProfileSystem::Instance->ActiveProfile.L2_S1_InvHoneyCombPicked)
		{
			HoneycombMask->SetEnable(false);
		}
	}


	for (int i = 0; i < NUM_OF_LEAVES; i++)
	{
		std::string str = "L";
		str = str + std::to_string(i);

		Leaf[i] = GetObjectByID(str);
		if (i == 2)
			Leaf[i]->SetPivot(83,46);
		else
			Leaf[i]->SetPivot(27,27);
		Leaf[i]->Animate(0,0,3,0,3);
	}

	for (int i = 0; i < NUM_OF_BEES; i++)
	{
		std::string str = "HoneyBee0";
		str = str + std::to_string(i + 1);

		HoneyBee[i] = GetObjectByID(str);

		switch (i % 4)
		{
		case 0:
			HoneyBee[i]->Animate(0,2,0,0,10,2);
			break;
		case 1:
			HoneyBee[i]->Animate(5,5,0,0,10,2);
			break;
		case 2:
			HoneyBee[i]->Animate(4,0,0,0,6,3);
			break;
		case 3:
			HoneyBee[i]->Animate(2,0,0,0,7,4);
			break;
		default:
			break;
		}

		if (ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked)
			HoneyBee[i]->SetEnable(false);
	}

	smokeVFX = new SuperFX("PRT\\smoke_test2.INI", PointSystem::CreateCPoint(615,430,0), zOrder + 10);
}

L2S1_Pop1HoneyComb::~L2S1_Pop1HoneyComb()
{	
	SAFE_DELETE(JAnim_HoneyCombCut);

	if (smokeVFX)
	{
		SAFE_DELETE(smokeVFX);
	}
}

void L2S1_Pop1HoneyComb::Required()
{

}

void L2S1_Pop1HoneyComb::Update(float ds)
{
	Popup::Update(ds);

	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (frankinscence->IsActive)
	{
		showfrankinscene++;
	}

	if (showfrankinscene == 250)
	{
		frankinscence->SetEnable(false);
		showfrankinscene = 0;
	}

	if (isAnimationPlaying)
	{
		return;
	}

	if( !ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked && HoneyCombRect.Intersection(&pos) && !Hud->IsDraggingItem())
	{
		Cursor::SetMode(CA_Gear);
		if(Control::Input->LBclicked())
			Hud->ShowBannerText("L2bees");
	}
	else if( !ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSliced && ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked && HoneyCombRect.Intersection(&pos) && !Hud->IsDraggingItem())
	{
		Cursor::SetMode(CA_Gear);
		if(Control::Input->LBclicked())
			Hud->ShowBannerText("L2just");
	}

	if( !ProfileSystem::Instance->ActiveProfile.L2_S1_InvHoneyCombPicked )
	{
		if( ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSliced )
		{
			if( InvItemRectHoneyComb.Intersection( &pos) )
			{
				Cursor::SetMode(CA_HandTake);
				if(Control::Input->LBclicked())
				{
					Control::Audio->QuickLoadAndPlaySFX("s1p1_honeypick");
					HoneycombCut->SetEnable(false);
					HoneycombMask->SetEnable(false);

					ProfileSystem::Instance->ActiveProfile.L2_S1_InvHoneyCombPicked = true;
					Hud->TakeInventoryItem(eInv_L2S1_HoneyComb);

					ClosePopup(1.5f);
					return;
				}
			}
		}
	}

}

void L2S1_Pop1HoneyComb::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (InvDestRectFrankinsmoke.Intersection(&dropPos))
	{

		if (itemVal == eInv_L2S2_Frankinscence)
		{
			smokeVFX->Start();

			if( !ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked )
			{
				Hud->ShowBannerText("L2rid");

				frankinscence->SetEnable(true);
				/*	frankinscence->SetPivot(0, -140);
				frankinscence->Animate(0, 0, 40, 0, 10, 1);
				*/
				ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked = true;
				Control::Audio->QuickLoadAndPlaySFX("s1p1_smoke");

				Hud->ReleaseDrag();
				Hud->CloseInventory(); 

				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S2_Frankinscence] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S2_Frankinscence);

				float valArray3[5] = {0, HoneybeeSetL->Pos.x, HoneybeeSetL->Pos.y, 1, 0};
				HoneybeeSetL->TweenTo(valArray3, 4.0f, 0, false);
				float valArray4[5] = {0, HoneybeeSetR->Pos.x, HoneybeeSetR->Pos.y, 1, 0};
				HoneybeeSetR->TweenTo(valArray4, 4.0f, 0, false);

				for (int i = 0; i < NUM_OF_BEES; i++)
				{
					HoneyBee[i]->StopAnim();
					float valArray00[5] = {0, HoneyBee[i]->Pos.x, HoneyBee[i]->Pos.y - 300, 1, 0};
					float valArray01[5] = {0, HoneyBee[i]->Pos.x - 200, HoneyBee[i]->Pos.y - 200, 1, 0};
					float valArray02[5] = {0, HoneyBee[i]->Pos.x + 250, HoneyBee[i]->Pos.y, 1, 0};

					switch (i % 4)
					{
					case 0:
						HoneyBee[i]->TweenTo(valArray00, 2.0f, 0, false);
						break;
					case 1:
						HoneyBee[i]->TweenTo(valArray01, 4.0f, 0, false);
						break;
					case 2:
					case 3:
						HoneyBee[i]->TweenTo(valArray02, 3.0f, 0, false);
						break;
					default:
						break;
					}
				} 
			}

		}
		else if (itemVal == eInv_L2S5_Ember)
		{
			if( !ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked )
			{
				Hud->ShowBannerText("L2smoke");
				Hud->ReleaseDrag();
				Hud->CloseInventory(); 
			}
			else
				GFInstance->WrongInvDropMsg(Hud);
		}
		else if (itemVal == eInv_L2S4_LitTorch)
		{
			if( !ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked )
			{
				Hud->ShowBannerText("L2flame");
				Hud->ReleaseDrag();
				Hud->CloseInventory(); 
			}
			else
				GFInstance->WrongInvDropMsg(Hud);
		}
		else if (itemVal == eInv_L2S6_KnifeSharpen)
		{
			if (ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked)
			{
				if(!ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSliced)
				{
					JAnim_HoneyCombCut->PlayAnim();
					isAnimationPlaying = true;
					Control::Audio->QuickLoadAndPlaySFX("s1p1_knifecut");

					Hud->ReleaseDrag();
					Hud->CloseInventory(); 

					if (ProfileSystem::Instance->ActiveProfile.L2_S1_IsAloveraSliced && ProfileSystem::Instance->ActiveProfile.L2_S5_MangoSliced && ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraSliced && ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxSliced && ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericSliced)
					{
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S6_KnifeSharpen] = false;
						Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S6_KnifeSharpen);
					}
				}
			}
			else
			{
				Hud->ShowBannerText("L2sting");

				Hud->ReleaseDrag();
				Hud->CloseInventory(); 
			}
		}
		else if (itemVal == eInv_L2S6_KnifeBlunt)
		{
			if( ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked )
			{
				Hud->ShowBannerText("L2rust");
				Hud->ReleaseDrag();
				Hud->CloseInventory(); 
			}
			else 
			{
				Hud->ShowBannerText("L2sting");
				Hud->ReleaseDrag();
				Hud->CloseInventory(); 
			}
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
	}
}

void L2S1_Pop1HoneyComb::OnAnimationComplete(int animID)
{
	isAnimationPlaying = false;
	ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSliced = true;
}

bool L2S1_Pop1HoneyComb::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked && ProfileSystem::Instance->ActiveProfile.L2_S2_InvFrankinscenceTaken)
		isAnyTaskLeft = true;
	else if(ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked && !ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSliced && ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken)
		isAnyTaskLeft = true;
	else if(ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSliced && !ProfileSystem::Instance->ActiveProfile.L2_S1_InvHoneyCombPicked)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L2S1_Pop1HoneyComb::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked && ProfileSystem::Instance->ActiveProfile.L2_S2_InvFrankinscenceTaken)
	{
		Hud->ShowHintForInvItem(eInv_L2S2_Frankinscence,  &InvItemRectHoneyComb);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked && !ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSliced && ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken)
	{
		Hud->ShowHintForInvItem(eInv_L2S6_KnifeSharpen, &InvItemRectHoneyComb);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSliced && !ProfileSystem::Instance->ActiveProfile.L2_S1_InvHoneyCombPicked)
	{
		Hud->InitHintTrailEmit(InvItemRectHoneyComb.GetCenterX(), InvItemRectHoneyComb.GetCenterY());		
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
