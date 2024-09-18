//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L5S1_Pop3Tiger.h"
#include "Hud.h"

const CRectangle  InvDestRectBowArrow(366.0f,112.0f,628.0f,440.0f);
const CRectangle  PopMaskRect(361.0f,94.0f,649.0f,484.0f);


L5S1_Pop3Tiger::L5S1_Pop3Tiger(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV5\\S1\\POP3\\P3.SCN");
	SetFrameAndCloseButton();
	Control::Audio->LoadSample(aVOL5MaryRefS1_1,	AudioVO);
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	GetObjectByID("handarrow")->SetEnable(false);
	GetObjectByID("handbow")->SetEnable(false);

	_JAnim_TigerLoop = new JSONAnimator(0);
	_JAnim_TigerLoop->parseAndLoadJSON("ANIM\\L5\\S1TIGERLOOP.JSON", this);
	_JAnim_TigerLoop->PlayAnim(true);

	_JAnim_BowAim = new JSONAnimator(1, CC_CALLBACK_1(L5S1_Pop3Tiger::OnAnimationComplete, this));
	_JAnim_BowAim->parseAndLoadJSON("ANIM\\L5\\S1BOWAIM.JSON", this);

	
	cv_TigerShootCutscn = NULL;
	time = 0;
	subIndex = 0;
	vidTimer = 0;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("tigershoot"));

	isTigerShootCutscnPlaying = false;
	//if(!ProfileSystem::Instance->ActiveProfile.L3_S4_IsAmeehaCutscenePlayed)
	{
		//CRectangle RectVidFull(171, 96, 1024, 576);
		CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
		cv_TigerShootCutscn = new CVideo("OGV\\L5\\S1TIGERSHOOT.OGV", RectVidFull, false, eZOrder_CutsceneFull + 2000);
		//cv_TigerShootCutscn->SetScale(1.334f);
		cv_TigerShootCutscn->SetScale(GFApp->video_aspectRatio);
		cv_TigerShootCutscn->SetSkipActive(); 
	}
}

void L5S1_Pop3Tiger::OnPopupOpened()
{
	//Objective Add
	if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L5O1_GetRidOfTiger] )
	{
		ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L5O1_GetRidOfTiger] = true;
		ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
		= eObj_L5O1_GetRidOfTiger;
		//Increment Objectives
		ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
		Hud->ShowObjectiveFX(eObj_L5O1_GetRidOfTiger);

		Control::Audio->PlaySample(aSFXMagic1);
	}
}

void L5S1_Pop3Tiger::OnAnimationComplete(int animID)
{
	if(animID == 1)
	{
		Hud->AllowInput = false;
		Hud->HideHud();		

		//Bow and Arrow is ready
		//ProfileSystem::Instance->ActiveProfile.L5_S1_BowToTigerCutscenePlayed = true;
		cv_TigerShootCutscn->PlayFadeInVideo(2.0f);

		Hud->SendUpdateSceneMessage(); //hide tiger on scene

		isTigerShootCutscnPlaying = true;
		isAnimationPlaying = true;
	}
}

void L5S1_Pop3Tiger::Required()
{

}

void L5S1_Pop3Tiger::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || Hud->IsDraggingItem() || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing)
		return;

	if (isTigerShootCutscnPlaying)
	{		
		Cursor::SetMode(CA_Normal);

		//Subtitles Code
		time = cv_TigerShootCutscn->GetTime();

		if (time >= subtitles[subIndex].begin && time < subtitles[subIndex].end)
		{
			subtitleText->SetText(subtitles[subIndex].text);
		}
		else
			subtitleText->SetText("");

#ifdef FORCE_CLOSE_VIDEO
		vidTimer += Control::LogicRate;
#endif
		if(
#ifndef FORCE_CLOSE_VID_END
			cv_TigerShootCutscn->isEndReached() ||
#endif
			vidTimer > 14)
		{
			isAnimationPlaying = false;
			subtitleText->SetText("");
			ProfileSystem::Instance->ActiveProfile.L5_S1_BowToTigerCutscenePlayed = true;

			isTigerShootCutscnPlaying = false;
			cv_TigerShootCutscn->StopAndFadeOutVideo(2.0f);
			Hud->AllowInput = true;
			Hud->ShowHud();
			
			Hud->ShowBannerText("L5mistake");
			Control::Audio->PlaySample(aVOL5MaryRefS1_1);
			ClosePopup();
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

	//handle interactions here
	if(!ProfileSystem::Instance->ActiveProfile.L5_S1_BowToTigerCutscenePlayed)
	{
		if(InvDestRectBowArrow.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if(Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L5scare");
			}
		}
	}
}

void L5S1_Pop3Tiger::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		GFInstance->WrongInvDropMsg(Hud);
		return;
	}

	if (itemVal == eInv_L5_S1_BowArrowFixed && InvDestRectBowArrow.Intersection(&dropPos))
	{
		if(!ProfileSystem::Instance->ActiveProfile.L5_S1_BowToTigerCutscenePlayed)
		{
			GetObjectByID("handarrow")->SetMaskRect(PopMaskRect);
			GetObjectByID("handbow")->SetMaskRect(PopMaskRect);
			isAnimationPlaying = true;

			Control::Audio->QuickLoadAndPlaySFX("s1p3_arrowloading");

			_JAnim_BowAim->PlayAnim();

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
		}
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
}

void L5S1_Pop3Tiger::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here
	if(!ProfileSystem::Instance->ActiveProfile.L5_S1_BowToTigerCutscenePlayed && ProfileSystem::Instance->ActiveProfile.L5_S1_InvBowWithArrowFirstTaken)
	{
		Hud->ShowHintForInvItem(eInv_L5_S1_BowArrowFixed, &InvDestRectBowArrow);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L5S1_Pop3Tiger::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if(!ProfileSystem::Instance->ActiveProfile.L5_S1_BowToTigerCutscenePlayed && ProfileSystem::Instance->ActiveProfile.L5_S1_InvBowWithArrowFirstTaken)
	{
		isAnyTaskLeft = true;
	}
	return isAnyTaskLeft;
}

L5S1_Pop3Tiger::~L5S1_Pop3Tiger()
{
	
	Control::Audio->UnloadSample(aVOL5MaryRefS1_1);
	SAFE_DELETE(_JAnim_TigerLoop);
	SAFE_DELETE(_JAnim_BowAim);
	SAFE_DELETE(cv_TigerShootCutscn);
	SAFE_DELETE(subtitleText);
}
