//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L3S4_Pop4Metalsheet.h"
#include "Hud.h"

const CRectangle  ActionRectLeaves(598.0f,327.0f,178.0f,100.0f);
const CRectangle  MemoryRectCat(742.0f,183.0f,68.0f,113.0f);
const CRectangle  ActionRectAmeehaDiary(555.0f,308.0f,267.0f,164.0f);
const CRectangle  InvItemRectBlanksheet(555.0f,308.0f,267.0f,164.0f);

L3S4_Pop4Metalsheet::L3S4_Pop4Metalsheet(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV3\\S4\\POP4\\P4.SCN");
	SetFrameAndCloseButton(true);

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	clickDelayTimer = -1;
	Control::Audio->LoadSample(aVOL3MaryRefS4_1,	AudioVO);
	cv_AmeehaCutscn = NULL;
	time = 0;
	subIndex = 0;
	subMax = 12;
	vidTimer = 0;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary3"));
	subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary4"));
	subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary5"));
	subtitles[5] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary6"));
	subtitles[6] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary7"));
	subtitles[7] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary8"));
	subtitles[8] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary9"));
	subtitles[9] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary10"));
	subtitles[10] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary11"));
	subtitles[11] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary12"));

	isAmeehaCutscnPlaying = false;
	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsAmeehaCutscenePlayed)
	{
		//CRectangle RectVidFull(171, 96, 1024, 576);
		CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
		cv_AmeehaCutscn = new CVideo("OGV\\L3\\S4AMEEHADIARY.OGV", RectVidFull, false, eZOrder_CutsceneFull);
		//cv_AmeehaCutscn->SetScale(1.334f);
		cv_AmeehaCutscn->SetScale(GFApp->video_aspectRatio);
		cv_AmeehaCutscn->SetSkipActive(); 
	}

	if (ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop4LeavesRemoved)
	{
		GetObjectByID("leaf1")->SetEnable(false);
		GetObjectByID("leaf2")->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L3_S4_IsAmeehaCutscenePlayed)
	{
		GetObjectByID("diarynote")->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L3_S4_InvMetalSheetBlankTaken)
	{
		GetObjectByID("metalsheet")->SetEnable(false);
	}
}

void L3S4_Pop4Metalsheet::Required()
{

}

void L3S4_Pop4Metalsheet::OnDiaryNoteTweenComplete(CObject *obj)
{
	GetObjectByID("diarynote")->SetEnable(false);
	isAnimationPlaying = false;
	//Add Diary Note

	//Journal Entry Add
	if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L3P3_AmeehaDiaryNote])
	{
		//Flag Set
		ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L3P3_AmeehaDiaryNote] = true;

		//Add to Note Array
		ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L3P3_AmeehaDiaryNote;

		ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

		//Increment Notes
		ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
	}

	Hud->ShowNewInfoTextBanner();
}

void L3S4_Pop4Metalsheet::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open)
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	if (isAmeehaCutscnPlaying)
	{
		Cursor::SetMode(CA_Normal);
		//Subtitles Code
		time = cv_AmeehaCutscn->GetTime();

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
			cv_AmeehaCutscn->isEndReached() ||
#endif
			vidTimer > 64)
		{
			isAnimationPlaying = false;
			subtitleText->SetText("");
			ProfileSystem::Instance->ActiveProfile.L3_S4_IsAmeehaCutscenePlayed = true;

			isAmeehaCutscnPlaying = false;
			cv_AmeehaCutscn->StopAndFadeOutVideo(2.0f);
			Hud->AllowInput = true;
			Hud->ShowHud();

			Hud->UnlockCharacter(eChar12_Devadeva);
			ProfileSystem::Instance->ActiveProfile.isCharacterUnlocked[eChar14_Sukra] = true;

			isAnimationPlaying = true;

			float valArr[5] = {0, 220, 667, 0.45f, 15.0f};
			GetObjectByID("diarynote")->TweenTo(valArr, 1.5f, TW_EASEQUADIN, true, CC_CALLBACK_1(L3S4_Pop4Metalsheet::OnDiaryNoteTweenComplete, this));

			Hud->ShowBannerText("L3hatred");
			Control::Audio->PlaySample(aVOL3MaryRefS4_1);
		} 
		return;
	}

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);	

	if(clickDelayTimer>0)
		clickDelayTimer -= Control::LogicRate;

	//handle interactions here

	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop4LeavesRemoved)
	{
		if(ActionRectLeaves.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked() && clickDelayTimer<0)
			{
				Control::Audio->QuickLoadAndPlaySFX("s4p4_leafmoved");

				clickDelayTimer = 0.5f;
				ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop4LeavesRemoved = true;
				GetObjectByID("leaf1")->TweenPosTo(-40, 40, -1, 1.0f, 0, true);
				GetObjectByID("leaf2")->TweenPosTo(40, 20, -1, 1.0f, 0, true);
			}
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsAmeehaCutscenePlayed)
	{
		if(ActionRectAmeehaDiary.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked() && clickDelayTimer<0)
			{
				Hud->AllowInput = false;
				Hud->HideHud();

				//GetObjectByID("diarynote")->FadeOut(2.0f);

				cv_AmeehaCutscn->PlayFadeInVideo(1.0f);
				isAmeehaCutscnPlaying = true;
				isAnimationPlaying = true;
				return;
			}
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S4_InvMetalSheetBlankTaken)
	{
		if(InvItemRectBlanksheet.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked() && clickDelayTimer<0)
			{
				ProfileSystem::Instance->ActiveProfile.L3_S4_InvMetalSheetBlankTaken = true;
				GetObjectByID("metalsheet")->SetEnable(false);
				Hud->TakeInventoryItem(eInv_L3S4_MetalSheetBlank);
				ClosePopup(1.5f);
				return;
			}
		}
	}
}

void L3S4_Pop4Metalsheet::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	GFInstance->WrongInvDropMsg(Hud);
}

void L3S4_Pop4Metalsheet::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here
	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop4LeavesRemoved)
	{
		Hud->InitHintTrailEmit(&ActionRectLeaves);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsAmeehaCutscenePlayed)
	{
		Hud->InitHintTrailEmit(&ActionRectAmeehaDiary);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S4_InvMetalSheetBlankTaken)
	{
		Hud->InitHintTrailEmit(&InvItemRectBlanksheet);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L3S4_Pop4Metalsheet::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop4LeavesRemoved)
	{
		isAnyTaskLeft = true;
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsAmeehaCutscenePlayed)
	{
		isAnyTaskLeft = true;
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S4_InvMetalSheetBlankTaken)
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

L3S4_Pop4Metalsheet::~L3S4_Pop4Metalsheet()
{
	Control::Audio->UnloadSample(aVOL3MaryRefS4_1);
	SAFE_DELETE(cv_AmeehaCutscn);

	SAFE_DELETE(subtitleText);
}
