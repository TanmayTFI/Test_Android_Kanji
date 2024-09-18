//====================================
//	DQFYLH
//	HO System
//	Level 4 - Scene 04 - POPUP 3: SKELETON
//====================================

#include "L4S4_Pop3Skeleton.h"

const CRectangle  InvDestLocketRect(661.0f,400.0f,200.0f,200.0f);
const CRectangle  BinocularHandleRect(450.0f,225.0f,250.0f,250.0f);

L4S4_Pop3Skeleton::L4S4_Pop3Skeleton(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{


	//Popup load
	Load("LV4\\S4\\POP3\\P3.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	Control::Audio->LoadSample(aVOL4MaryRefS4_1,	AudioVO);
	if (!ProfileSystem::Instance->ActiveProfile.L4_S4_SkeletonHandOpened)
	{
		GetObjectByID("locket_InPosition")->SetEnable(false);
		GetObjectByID("Hand2")->SetAlpha(0);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S4_FlowerCutscenePlayed)
	{
		GetObjectByID("MemFlower")->SetEnable(false);
	}
	Janim_handOpen = new JSONAnimator(0,CC_CALLBACK_1(L4S4_Pop3Skeleton::OnAnimComplete,this));
	Janim_handOpen->parseAndLoadJSON("ANIM\\L4\\L4_S4_POP3HANDOPENANIM.JSON", this);

	isVideoPlaying = false;
	lpVidFlowerCutScn = NULL;
	time = 0;
	subIndex = 0;
	subMax = 4;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("s4memory1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("s4memory2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("s4memory3"));
	subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("s4memory4"));

	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	lpVidFlowerCutScn = new CVideo("OGV\\L4\\S4MEMORY.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	//lpVidFlowerCutScn->SetScale(1.334f);
	lpVidFlowerCutScn->SetScale(GFApp->video_aspectRatio);
	vidTimer = 0;
	isOrbGlowing = false;
	isOrbFXFlying = false;

	SetSparkleUpdate(false);
	Show(false);
}


void L4S4_Pop3Skeleton::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (!ProfileSystem::Instance->ActiveProfile.L4_S4_FlowerCutscenePlayed && !isOrbGlowing && !isVideoPlaying)
	{
		Hud->ShowOrbGlowing();
		isOrbGlowing = true;
	}

	if(isOrbFXFlying)
		return;

	if (isVideoPlaying )
	{
		//Subtitles Code
		time = lpVidFlowerCutScn->GetTime();

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
			lpVidFlowerCutScn->isEndReached() ||
#endif
			vidTimer > 27)
		{
			subtitleText->SetText("");
			Hud->ShowBannerText("L4nice");
			Control::Audio->PlaySample(aVOL4MaryRefS4_1);
			ProfileSystem::Instance->ActiveProfile.L4_S4_FlowerCutscenePlayed = true;
			isVideoPlaying = false;
			lpVidFlowerCutScn->StopAndFadeOutVideo(2.0f);					
			Hud->AllowInput = true;
			Hud->ShowHud();
			Hud->SendUpdateSceneMessage();

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L4P3_CrystalVideo])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L4P3_CrystalVideo] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L4P3_CrystalVideo;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
				Hud->ShowJrnEntryFX();
			}
		} 
		return;
	}
	if (InvDestLocketRect.Intersection(&pos))
	{
		if (!ProfileSystem::Instance->ActiveProfile.L4_S4_FlowerCutscenePlayed)
		{
			Cursor::SetMode(CA_HandPointy);
			if (Control::Input->LBclicked())
			{
				Hud->HideOrbGlowing();
				isOrbGlowing = false;

				isAnimationPlaying = true;
				isOrbFXFlying = true;
				Hud->SendOrbFXToObjectPos(GetObjectByID("MemFlower")->Pos);
			}
		}
		else if (!ProfileSystem::Instance->ActiveProfile.L4_S4_SkeletonHandOpened)
		{
			Cursor::SetMode(CA_Gear);
			if (Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L4maybe");
			}
		}
	}
	else if (BinocularHandleRect.Intersection(&pos))
	{
		if (!ProfileSystem::Instance->ActiveProfile.L4_S4_SkeletonHandOpened)
		{
			Cursor::SetMode(CA_Help);
			if (Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L4grip");
			}
		}
		else if (!ProfileSystem::Instance->ActiveProfile.L4_S4_InvBinocularTaken)
		{
			Cursor::SetMode(CA_HandTake);
			if (Control::Input->LBclicked())
			{
				Hud->TakeInventoryItem(eInv_L4_S4_BinocularWithoutLens);
				//Hud->ShowBannerText("L4lens");
				ProfileSystem::Instance->ActiveProfile.L4_S4_InvBinocularTaken = true;
				GetObjectByID("Skeleton_0007_Binocular")->SetEnable(false);
				ClosePopup(2.0f);
			}
		}
	}
}

void L4S4_Pop3Skeleton::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	switch (itemVal)
	{
	case eInv_L4_S1_Bracelet:
		if (!ProfileSystem::Instance->ActiveProfile.L4_S4_SkeletonHandOpened && InvDestLocketRect.Intersection(&dropPos) && ProfileSystem::Instance->ActiveProfile.L4_S4_FlowerCutscenePlayed)
		{
			Control::Audio->QuickLoadAndPlaySFX("s4p2_placingMedallion");
			GetObjectByID("locket_InPosition")->SetEnable(true);
			Janim_handOpen->PlayAnim();

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_Bracelet] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S1_Bracelet);
		}
		else
			Hud->CancelDrag();
		break;
	default:
		GFInstance->WrongInvDropMsg(Hud);
		break;
	}
}

void L4S4_Pop3Skeleton::OnAnimComplete(int animID)
{
	ProfileSystem::Instance->ActiveProfile.L4_S4_SkeletonHandOpened = true;
}

void L4S4_Pop3Skeleton::startMemoryCutscene()
{
	isAnimationPlaying = false;
	isOrbFXFlying = false;

	GetObjectByID("MemFlower")->FadeOut(2.0f);

	Hud->AllowInput = false;
	Hud->HideHud();
	lpVidFlowerCutScn->SetSkipActive();
	isVideoPlaying = true;
	lpVidFlowerCutScn->PlayFadeInVideo(1.5f);
}


float L4S4_Pop3Skeleton::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L4S4_Pop3Skeleton::~L4S4_Pop3Skeleton()
{
	Control::Audio->UnloadSample(aVOL4MaryRefS4_1);
	SAFE_DELETE(lpVidFlowerCutScn);
	SAFE_DELETE(Janim_handOpen);
	SAFE_DELETE(subtitleText);
}

void L4S4_Pop3Skeleton::Required()
{

}

void L4S4_Pop3Skeleton::ClosePopup(float delayTime)
{
	if (isVideoPlaying)
	{
		return;
	}

	Popup::ClosePopup(delayTime);
}

bool L4S4_Pop3Skeleton::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if (!ProfileSystem::Instance->ActiveProfile.L4_S4_FlowerCutscenePlayed)
		isAnyTaskLeft = true;
	if ((ProfileSystem::Instance->ActiveProfile.L4_S1_InvBraceletTaken && !ProfileSystem::Instance->ActiveProfile.L4_S4_SkeletonHandOpened) ||
		(ProfileSystem::Instance->ActiveProfile.L4_S4_SkeletonHandOpened && !ProfileSystem::Instance->ActiveProfile.L4_S4_InvBinocularTaken))
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L4S4_Pop3Skeleton::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	if (!ProfileSystem::Instance->ActiveProfile.L4_S4_FlowerCutscenePlayed)
	{
		Hud->InitHintTrailEmit(&InvDestLocketRect);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L4_S1_InvBraceletTaken && !ProfileSystem::Instance->ActiveProfile.L4_S4_SkeletonHandOpened)
	{
		Hud->ShowHintForInvItem(eInv_L4_S1_Bracelet, &InvDestLocketRect);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L4_S4_SkeletonHandOpened && !ProfileSystem::Instance->ActiveProfile.L4_S4_InvBinocularTaken)
	{
		Hud->InitHintTrailEmit(BinocularHandleRect.GetCenterX(), BinocularHandleRect.GetCenterY());
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
