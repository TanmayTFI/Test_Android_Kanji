//====================================
//	DQFYLH
//	HO System
//	Level 6 - Scene 03 - POPUP 2: 
//====================================

#include "L6S3_Pop2Fire.h"

const CRectangle  InDestRect(571.0f,244.0f,227.0f,222.0f);


L6S3_Pop2Fire::L6S3_Pop2Fire(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV6\\S3\\P2\\P2.SCN");
	SetFrameAndCloseButton(true);
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	if (!ProfileSystem::Instance->ActiveProfile.L6_S3_SteelPlaced)
	{
		GetObjectByID("Metal")->SetEnable(false); 
	}
	GetObjectByID("FlintStone")->SetEnable(false);
	GetObjectByID("FlintStone")->SetZ(GetObjectByID("FlintStone")->ZOrder + 200);

	std::vector<SuperFX*> superfxArray;
	SuperFX *SparklesFX = new SuperFX("PRT\\L6\\S3_FireSparkle.INI", PointSystem::CreateCPoint(660,412,0), eZOrder_PopupCloseButton);
	superfxArray.push_back(SparklesFX);

	Janim_RubForFire = new JSONAnimator(0,CC_CALLBACK_1(L6S3_Pop2Fire::OnAnimComplete, this));
	Janim_RubForFire->parseAndLoadJSON("ANIM\\L6\\S3_FIREMAKING.JSON",this,&superfxArray);

	isVideoPlaying = false;
	lpVidroomlighteningCutScn = NULL;
	vidTimer = 0;

	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	lpVidroomlighteningCutScn = new CVideo("OGV\\L6\\S3ROOMLIGHTING.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	//lpVidroomlighteningCutScn->SetScale(1.334f);
	lpVidroomlighteningCutScn->SetScale(GFApp->video_aspectRatio);

	SetSparkleUpdate(false);
	Show(false);
}


void L6S3_Pop2Fire::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (InDestRect.Intersection(&pos) && (!ProfileSystem::Instance->ActiveProfile.L6_S3_SteelPlaced || !ProfileSystem::Instance->ActiveProfile.L6_S3_FlintStoneUsed))
	{
		Cursor::SetMode(CA_Gear);
	}

	if (Control::Input->LBclicked() && (!ProfileSystem::Instance->ActiveProfile.L6_S3_SteelPlaced || !ProfileSystem::Instance->ActiveProfile.L6_S3_FlintStoneUsed))
	{
		Hud->ShowBannerText("L6fire");
	}

	if (isVideoPlaying )
	{
		Cursor::SetMode(CA_Normal);
#ifdef FORCE_CLOSE_VIDEO
		vidTimer += Control::LogicRate;
#endif
		if(
#ifndef FORCE_CLOSE_VID_END
			lpVidroomlighteningCutScn->isEndReached() ||
#endif
			vidTimer > 12)
		{
			isVideoPlaying = false;
			lpVidroomlighteningCutScn->StopAndFadeOutVideo(2.0f);					
			Hud->AllowInput = true;
			Hud->ShowHud();
			Hud->SendUpdateSceneMessage();
			ClosePopup();
		} 
	}
}

void L6S3_Pop2Fire::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	switch (itemVal)
	{
	case eInv_L6_S1_Steel:
		if (InDestRect.Intersection(&dropPos) && !ProfileSystem::Instance->ActiveProfile.L6_S3_SteelPlaced)
		{
			Control::Audio->QuickLoadAndPlaySFX("s3p2_placeSteel");
			GetObjectByID("Metal")->SetEnable(true);
			ProfileSystem::Instance->ActiveProfile.L6_S3_SteelPlaced = true;

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S1_Steel] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S1_Steel);
		}
		break;
	case eInv_L6_S4_FlintStones:
		if (InDestRect.Intersection(&dropPos))
		{
			if (!ProfileSystem::Instance->ActiveProfile.L6_S3_SteelPlaced)
			{
				Hud->ShowBannerText("L6rough");

				Hud->ReleaseDrag();
				Hud->CloseInventory(); 
			}
			else if (!ProfileSystem::Instance->ActiveProfile.L6_S3_FlintStoneUsed)
			{
				Control::Audio->QuickLoadAndPlaySFX("s3p2_createFire");
				GetObjectByID("FlintStone")->SetEnable(true);
				Janim_RubForFire->PlayAnim();
				ProfileSystem::Instance->ActiveProfile.L6_S3_FlintStoneUsed = true;

				Hud->ReleaseDrag();
				Hud->CloseInventory(); 

			/*	ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S4_FlintStones] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S4_FlintStones);*/
			}
		}
		break;
	default:
		GFInstance->WrongInvDropMsg(Hud);
		break;
	}
}

void L6S3_Pop2Fire::OnAnimComplete(int animID)
{
	ProfileSystem::Instance->ActiveProfile.L6_S3_RoomLightened = true;
	GetObjectByID("FlintStone")->FadeOut();
	GetObjectByID("Metal")->FadeOut();
	//GetObjectByID("Fire")->FadeIn();
	Hud->TakeInventoryItem(eInv_L6_S1_Steel,GetObjectByID("Metal")->Pos);
	Hud->AllowInput = false;
	Hud->HideHud();
	lpVidroomlighteningCutScn->SetSkipActive();
	isVideoPlaying = true;
	lpVidroomlighteningCutScn->PlayVideo();
	//ClosePopup(1.5f);
}

float L6S3_Pop2Fire::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L6S3_Pop2Fire::~L6S3_Pop2Fire()
{
	SAFE_DELETE(Janim_RubForFire);
	SAFE_DELETE(lpVidroomlighteningCutScn);
}

void L6S3_Pop2Fire::Required()
{

}

void L6S3_Pop2Fire::ClosePopup(float delayTime)
{
	if (isVideoPlaying)
	{
		return;
	}

	Popup::ClosePopup(delayTime);
}

bool L6S3_Pop2Fire::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if ((!ProfileSystem::Instance->ActiveProfile.L6_S3_SteelPlaced && ProfileSystem::Instance->ActiveProfile.L6_S1_InvSteelTaken) ||
		(!ProfileSystem::Instance->ActiveProfile.L6_S3_FlintStoneUsed && ProfileSystem::Instance->ActiveProfile.L6_S4_InvFlintStoneCollected && ProfileSystem::Instance->ActiveProfile.L6_S3_SteelPlaced))
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L6S3_Pop2Fire::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L6_S3_SteelPlaced && ProfileSystem::Instance->ActiveProfile.L6_S1_InvSteelTaken)
	{
		Hud->ShowHintForInvItem(eInv_L6_S1_Steel,&InDestRect);
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L6_S3_FlintStoneUsed && ProfileSystem::Instance->ActiveProfile.L6_S4_InvFlintStoneCollected && ProfileSystem::Instance->ActiveProfile.L6_S3_SteelPlaced)
	{
		Hud->ShowHintForInvItem(eInv_L6_S4_FlintStones,&InDestRect);
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
