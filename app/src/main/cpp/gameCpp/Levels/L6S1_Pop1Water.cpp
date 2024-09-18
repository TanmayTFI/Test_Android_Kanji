//====================================
//	DQFYLH
//	HO System
//	Level 6 - Scene 01 - POPUP 1: 
//====================================

#include "L6S1_Pop1Water.h"
#include "MeshObject.h"

const CRectangle  InvDestRectHorseShoe(640.0f,266.0f,219.0f,154.0f);
const CRectangle  MemObjRectFootPrint(904.0f,340.0f,82.0f,95.0f);

const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);

L6S1_Pop1Water::L6S1_Pop1Water(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{


	//Popup load
	Load("LV6\\S1\\P1\\P1.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	mo_water = NULL;
	mo_water = new MeshObject(GetObjectByID("WaterAnim"), 5, 5);
	mo_water->meshFX->SetSpeedAndDisplacement(4, 3, 2, 3);
	PushObject(mo_water);

	Janim_steelPick = new JSONAnimator(0, CC_CALLBACK_1(L6S1_Pop1Water::OnAnimComplete, this));
	Janim_steelPick->parseAndLoadJSON("ANIM\\L6\\S1_STEELPICKINGWITHROPE.JSON", this);

	GetObjectByID("water_mask")->SetAlpha(0.0);
	GetObjectByID("magnet")->SetAlpha(0);

	if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvMortarTaken)
	{
		GetObjectByID("Mortar")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvSteelTaken)
	{
		GetObjectByID("Steel-fire-strikerUnderWater")->SetEnable(false);
	}

	//GetObjectByID("magnet")->SetMaskRect(PopAreaRect);

	if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S1LocketChain])
	{
		GetObjectByID("KidsFootsteps")->SetEnable(false);
	}

	isVideoPlaying = false;
	lpVidMemCutScn = NULL;
	time = 0;
	subIndex = 0;
	subMax = 5;
	vidTimer = 0;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));
	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("sophiamemory1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("sophiamemory2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("sophiamemory3"));
	subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("sophiamemory4"));
	subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("sophiamemory5"));

	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	lpVidMemCutScn = new CVideo("OGV\\L6\\S1SOPHIAMEMORY.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	//lpVidMemCutScn->SetScale(1.334f);
	lpVidMemCutScn->SetScale(GFApp->video_aspectRatio);

	isOrbGlowing = false;
	isOrbFXFlying = false;

	isCollectingInventory = false;

	SetSparkleUpdate(false);
	Show(false);
}

void L6S1_Pop1Water::StartMemoryCutscene()
{
	Hud->AllowInput = false;
	Hud->HideHud();

	lpVidMemCutScn->SetSkipActive();
	isVideoPlaying = true;
	lpVidMemCutScn->PlayFadeInVideo(1.5f);
}

void L6S1_Pop1Water::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	#ifdef _CEBUILD

	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S1LocketChain] && !isOrbGlowing && !isVideoPlaying)
	{
		Hud->ShowOrbGlowing();
		isOrbGlowing = true;
	}

	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S1LocketChain] && MemObjRectFootPrint.Intersection(&pos) && !isVideoPlaying)
	{
		if( Control::Input->LBclicked() )
		{
			isAnimationPlaying = true;

			isOrbFXFlying = true;
			Hud->HideOrbGlowing();
			isOrbGlowing = false;
			Hud->SendOrbFXToObjectPos(GetObjectByID("KidsFootsteps")->Pos);
			
		}
	}
	else if (isVideoPlaying )
	{
		//Subtitles Code
		time = lpVidMemCutScn->GetTime();

		if (time >= subtitles[subIndex].begin && time < subtitles[subIndex].end)
		{
			subtitleText->SetText(subtitles[subIndex].text);
		}
		else
			subtitleText->SetText("");

		if (time > subtitles[subIndex].end)
		{
			if (subMax - 1 > subIndex)
				subIndex++;
		}
#ifdef FORCE_CLOSE_VIDEO
		vidTimer += Control::LogicRate;
#endif
		if(
#ifndef FORCE_CLOSE_VID_END
			lpVidMemCutScn->isEndReached() ||
#endif
			vidTimer > 32)
		{
			subtitleText->SetText("");
			isVideoPlaying = false;
			lpVidMemCutScn->StopAndFadeOutVideo(2.0f);					
			Hud->AllowInput = true;
			Hud->ShowHud();
			Hud->SendUpdateSceneMessage();
			GetObjectByID("KidsFootsteps")->FadeOut();

			isAnimationPlaying = false;

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P10_CrystalBallVideo])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P10_CrystalBallVideo] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L6P10_CrystalBallVideo;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
				Hud->ShowJrnEntryFX();
			}

			Hud->ShowMemoryObjectCollected_CE(eMem_L6S1LocketChain, &GetObjectByID("KidsFootsteps")->Pos);
			if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvMortarTaken && ProfileSystem::Instance->ActiveProfile.L6_S1_InvSteelTaken)
			{
				ClosePopup(1.0f);
			}
			Hud->ShowBannerText("L6mama");
		} 
		return;
	}
#else
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S1LocketChain] && MemObjRectFootPrint.Intersection(&pos) )
	{
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("KidsFootsteps")->FadeOut();
			Hud->ShowMemoryObjectCollected_SE(eMem_L6S1LocketChain, &(GetObjectByID("KidsFootsteps")->Pos));
		}
	}
#endif 
	if (GetObjectByID("Mortar")->GetWorldRect().Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S1_InvMortarTaken && !isCollectingInventory)
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			GetObjectByID("Mortar")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L6_S1_Mortar);
			ProfileSystem::Instance->ActiveProfile.L6_S1_InvMortarTaken = true;
			if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvSteelTaken)
			{
#ifdef _CEBUILD
				if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S1LocketChain])
				{			
					ClosePopup(1.5);
				}
#else
				ClosePopup(1.5);
#endif 
			}
		}
	}
	else if(GetObjectByID("Steel-fire-strikerUnderWater")->GetWorldRect().Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S1_InvSteelTaken)
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L6far");
		}
	}

}

void L6S1_Pop1Water::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	switch (itemVal)
	{
	case eInv_L6_CB_RopeWithMagnetisedHorseShoe:
		if (InvDestRectHorseShoe.Intersection(&dropPos) && !ProfileSystem::Instance->ActiveProfile.L6_S1_InvSteelTaken)
		{
			Control::Audio->QuickLoadAndPlaySFX("s1p1_steelTake");
			isCollectingInventory = true;
			Janim_steelPick->PlayAnim();

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_CB_RopeWithMagnetisedHorseShoe] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_CB_RopeWithMagnetisedHorseShoe);
		}
		break;
	case eInv_L6_S4_Rope:
		if (InvDestRectHorseShoe.Intersection(&dropPos) && !ProfileSystem::Instance->ActiveProfile.L6_S1_InvSteelTaken)
		{
			Hud->ShowBannerText("L6can");

			Hud->CancelDrag();
		}
		break;
	default:
		GFInstance->WrongInvDropMsg(Hud);
		break;
	}
}

void L6S1_Pop1Water::OnAnimComplete(int animID)
{
	GetObjectByID("water_mask")->SetAlpha(0.0);
	GetObjectByID("Steel-fire-strikerUnderWater")->SetEnable(false);
	GetObjectByID("magnet")->SetEnable(false);
	ProfileSystem::Instance->ActiveProfile.L6_S1_InvSteelTaken = true;
	isCollectingInventory = false;

	Hud->TakeInventoryItem(eInv_L6_S1_Steel);

	if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvMortarTaken)
	{
#ifdef _CEBUILD
		if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S1LocketChain])
		{			
			ClosePopup(1.5);
		}
#else
		ClosePopup(1.5);
#endif 
	}
}

float L6S1_Pop1Water::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L6S1_Pop1Water::~L6S1_Pop1Water()
{
	SAFE_DELETE(Janim_steelPick);
	SAFE_DELETE(lpVidMemCutScn);
	SAFE_DELETE(subtitleText);
}

void L6S1_Pop1Water::Required()
{

}

void L6S1_Pop1Water::ClosePopup(float delayTime)
{
	if (isVideoPlaying)
	{
		return;
	}
	Popup::ClosePopup(delayTime);
}

bool L6S1_Pop1Water::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if (!ProfileSystem::Instance->ActiveProfile.L6_S1_InvMortarTaken ||
		(ProfileSystem::Instance->ActiveProfile.L6_S4_InvRopeHorseShoeComboTaken && !ProfileSystem::Instance->ActiveProfile.L6_S1_InvSteelTaken))
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L6S1_Pop1Water::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L6_S1_InvMortarTaken)
	{
		//Hud->InitHintTrailEmit(&GetObjectByID("Mortar")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle mortarObjRect =GetObjectByID("Mortar")->GetWorldRect();
		Hud->InitHintTrailEmit(&mortarObjRect);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvRopeHorseShoeComboTaken && !ProfileSystem::Instance->ActiveProfile.L6_S1_InvSteelTaken)
	{
		Hud->ShowHintForInvItem(eInv_L6_CB_RopeWithMagnetisedHorseShoe,&InvDestRectHorseShoe);
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
