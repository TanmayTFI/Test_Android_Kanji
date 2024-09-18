//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 01 - POPUP 5: SCRAPTER
//====================================

#include "L2S3_Pop1Turmeric.h"
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

const CRectangle  InvDestRectRake(436.0f,218.0f,465.0f,361.0f);
const CRectangle  InvItemRectTermeric(367.0f,435.0f,206.0f,116.0f);
const CRectangle  pop1AreaRectFrog(817.0f,420.0f,144.0f,113.0f);

const CRectangle  TurmericPlantsRect(456.0f,93.0f,433.0f,470.0f);


L2S3_Pop1Turmeric::L2S3_Pop1Turmeric(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)

{
	//Popup load
	Load("LV2\\S3\\POP1\\P1.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	Show(false);
	Hud = hud;

	RakeR = GetObjectByID("rake1");
	RakeL = GetObjectByID("rake2");
	SoilL = GetObjectByID("Soil02");
	SoilR = GetObjectByID("Soil01");
	plantOnGround = GetObjectByID("PlantDown");
	turmericPlant = GetObjectByID("TurmericPlant");
	Frog = GetObjectByID("FrogJump");

	CPoint *posPointer = new CPoint(663,499,0);
	SoilL->SetPos(posPointer);
	delete(posPointer);

	Janim_rakeR = new JSONAnimator(0, CC_CALLBACK_1(L2S3_Pop1Turmeric::OnRakeRightAnimationComplete, this));
	Janim_rakeR->parseAndLoadJSON("ANIM\\L2\\S3_POP1RAKER.JSON", this);

	Janim_rakeL = new JSONAnimator(0, CC_CALLBACK_1(L2S3_Pop1Turmeric::OnRakeLeftAnimationComplete, this));
	Janim_rakeL->parseAndLoadJSON("ANIM\\L2\\S3_POP1RAKEL.JSON", this);

	Janim_plantPluck = new JSONAnimator(0);
	Janim_plantPluck->parseAndLoadJSON("ANIM\\L2\\S3_POP1PLUCK.JSON", this);

	int ArrayPts[] = {642,557,573,506,635,448,620,258,698,250,683,442,754,488,712,556};
	polyTermericPluck =  new CPolygon(8);
	polyTermericPluck->Init(ArrayPts);

	RakeR->SetEnable(false);
	RakeL->SetEnable(false);
	plantOnGround->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_PluckingTurmericPlayed && (!ProfileSystem::Instance->ActiveProfile.L2_S3_InvTurmericPicked) )
	{
		plantOnGround->SetEnable(true);
		turmericPlant->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_FrogJumpPlayed)
	{
		Frog->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_FirstDigPlayed)
	{
		SoilR->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_SecondDigPlayed)
	{
		SoilL->SetEnable(false);
	}
}

void L2S3_Pop1Turmeric::Required()
{

}

L2S3_Pop1Turmeric::~L2S3_Pop1Turmeric()
{
	SAFE_DELETE(Janim_rakeR);
	SAFE_DELETE(Janim_rakeL);
	SAFE_DELETE(Janim_plantPluck);
	SAFE_DELETE(polyTermericPluck);
}
void L2S3_Pop1Turmeric:: OnRakeRightAnimationComplete(int animID)
{

}

void L2S3_Pop1Turmeric::OnRakeLeftAnimationComplete(int animID)
{

}

void L2S3_Pop1Turmeric::Update(float ds)
{
	Popup::Update(ds);

	CPoint pos = Control::Input->Mpos();

	if (Popup::GetPopupState() != ePopupState_Open)
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if(ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_FrogJumpPlayed)
	{
		// Do the digging code here
		if(ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_SecondDigPlayed && (!ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_PluckingTurmericPlayed))
		{
			if(polyTermericPluck->Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if( Control::Input->LBclicked())
				{
					Control::Audio->QuickLoadAndPlaySFX("s3p1_turmerictake");
					turmericPlant->Shake(20,5,0,0.25f);
					Janim_plantPluck->PlayAnim();
					ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_PluckingTurmericPlayed = true;
					return;
				}
			}
		}

		if(ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_PluckingTurmericPlayed && (!ProfileSystem::Instance->ActiveProfile.L2_S3_InvTurmericPicked) )
		{
			if(InvItemRectTermeric.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if( Control::Input->LBclicked())
				{
					TakeInventoryItem(eInv_L2S3_Turmeric);
					ClosePopup();
					return;
				}
			}
		}

	}

	if( pop1AreaRectFrog.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_FrogJumpPlayed)
	{
		Cursor::SetMode(CA_HandPointy);
		if( Control::Input->LBclicked())
		{
			Frog->PlayAnimation(1);
			ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_FrogJumpPlayed = true;
			Control::Audio->QuickLoadAndPlaySFX("s3p1_frogjump");
		}
		return;
	}

	if (TurmericPlantsRect.Intersection (&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_FirstDigPlayed && !Hud->IsDraggingItem())
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
		{
			if(ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L2_S5_PZMedicine_Making])
				Hud->ShowBannerText("L2turmeric");
			else
				Hud->ShowBannerText("L2grow");
			return;
		}
	}
}

void L2S3_Pop1Turmeric::TakeInventoryItem(EHOInventory ItemID)
{
	std::vector<int> vecInvIds;
	switch (ItemID)
	{
	case eInv_L2S3_Turmeric:
		Hud->InvBoxCtrl->Activate(1);			
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S3_Turmeric] = true;
		ProfileSystem::Instance->ActiveProfile.L2_S3_InvTurmericPicked = true;			
		vecInvIds.push_back(eInv_L2S3_Turmeric);
		Hud->ShowInventory(&vecInvIds);
		break;
	default:
		break;
	}
}

void L2S3_Pop1Turmeric::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (InvDestRectRake.Intersection(&dropPos))
	{
		if (itemVal == eInv_L2S4_Rake)
		{
			if( !ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_FrogJumpPlayed )
			{
				Hud->ShowBannerText("L2eww");

				Hud->ReleaseDrag();
				Hud->CloseInventory(); 
			}
			else if( !ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_FirstDigPlayed)
			{
				Janim_rakeR->PlayAnim();
				ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_FirstDigPlayed = true;
				Control::Audio->QuickLoadAndPlaySFX("s3p1_rakeuse1");
			}
			else if( ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_FirstDigPlayed && !ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_SecondDigPlayed)
			{
				Janim_rakeL->PlayAnim();
				ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_SecondDigPlayed = true;
				Control::Audio->QuickLoadAndPlaySFX("s3p1_rakeuse2");

				Hud->ReleaseDrag();
				Hud->CloseInventory(); 


				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S4_Rake] = false;

				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S4_Rake);
			} 
		}
		else
			GFInstance->WrongInvDropMsg(Hud);

		return;
	}
}

bool L2S3_Pop1Turmeric::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	
	if(!ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_FrogJumpPlayed)
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_FirstDigPlayed && ProfileSystem::Instance->ActiveProfile.L2_S4_InvRakeTaken)
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_SecondDigPlayed && ProfileSystem::Instance->ActiveProfile.L2_S4_InvRakeTaken)
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_PluckingTurmericPlayed && ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_SecondDigPlayed)
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S3_InvTurmericPicked && ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_SecondDigPlayed)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L2S3_Pop1Turmeric::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_FrogJumpPlayed)
	{
		Hud->InitHintTrailEmit(pop1AreaRectFrog.GetCenterX(), pop1AreaRectFrog.GetCenterY());
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_FirstDigPlayed && ProfileSystem::Instance->ActiveProfile.L2_S4_InvRakeTaken)
	{
		Hud->ShowHintForInvItem(eInv_L2S4_Rake, &InvDestRectRake);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_SecondDigPlayed && ProfileSystem::Instance->ActiveProfile.L2_S4_InvRakeTaken)
	{
		Hud->ShowHintForInvItem(eInv_L2S4_Rake, &InvDestRectRake);		
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_PluckingTurmericPlayed && ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_SecondDigPlayed)
	{
		Hud->InitHintTrailEmit(TurmericPlantsRect.GetCenterX(), TurmericPlantsRect.GetCenterY());	
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S3_InvTurmericPicked && ProfileSystem::Instance->ActiveProfile.L2_S3_Pop1_PluckingTurmericPlayed)
	{
		Hud->InitHintTrailEmit(InvItemRectTermeric.GetCenterX(), InvItemRectTermeric.GetCenterY());	
	}
	else
	{
	/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
