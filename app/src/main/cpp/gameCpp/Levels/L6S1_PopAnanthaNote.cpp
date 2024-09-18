//====================================
//	DQFYLH
//	HO System
//	Level 6 - Scene 01 - POPUP: 
//====================================

#include "L6S1_PopAnanthaNote.h"


L6S1_PopAnanthaNote::L6S1_PopAnanthaNote(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{


	//Popup load
	Load("LV6\\S1\\P4\\POP\\P4.SCN");
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	GetObjectByID("Mask")->SetScale(2.3f);
	GetObjectByID("Mask")->SetEnable(false);

	isVideoPlaying = false;
	lpVidVaruniCutScn = NULL;
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

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("varunicurse1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("varunicurse2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("varunicurse3"));
	subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("varunicurse4"));
	subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("varunicurse5"));

	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	lpVidVaruniCutScn = new CVideo("OGV\\L6\\S1VARUNICURSE.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	//lpVidVaruniCutScn->SetScale(1.334f);
	lpVidVaruniCutScn->SetScale(GFApp->video_aspectRatio);

	SetSparkleUpdate(false);
	Show(false);
}


void L6S1_PopAnanthaNote::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);
	if (isVideoPlaying )
	{
		//Subtitles Code
		time = lpVidVaruniCutScn->GetTime();

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
			lpVidVaruniCutScn->isEndReached() ||
#endif
			vidTimer > 36)
		{
			ProfileSystem::Instance->ActiveProfile.L6_S1_VaruniCutscenePlayed = true;
			isVideoPlaying = false;
			lpVidVaruniCutScn->StopAndFadeOutVideo(2.0f);					
			Hud->AllowInput = true;
			Hud->ShowHud();
			Hud->SendUpdateSceneMessage();
			subtitleText->SetText("");
			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P1_VaruniVideoEnd1])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P1_VaruniVideoEnd1] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L6P1_VaruniVideoEnd1;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
			}

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P2_VaruniVideoEnd2])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P2_VaruniVideoEnd2] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L6P2_VaruniVideoEnd2;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
				Hud->ShowJrnEntryFX();
			}

			Hud->UnlockCharacter(eChar9_Varuni);


			ClosePopup(2.0f);
		} 
		return;
	}
	else if (Control::Input->LBclicked())
	{
		Hud->AllowInput = false;
		Hud->HideHud();
		lpVidVaruniCutScn->SetSkipActive();
		isVideoPlaying = true;
		lpVidVaruniCutScn->PlayVideo();
	}
}

void L6S1_PopAnanthaNote::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	/* switch (itemVal)
	{

	default:
	*/
	GFInstance->WrongInvDropMsg(Hud);
	/*break;
	}*/
}

void L6S1_PopAnanthaNote::OnAnimComplete(int animID)
{

}

float L6S1_PopAnanthaNote::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L6S1_PopAnanthaNote::~L6S1_PopAnanthaNote()
{
	SAFE_DELETE(lpVidVaruniCutScn);
	SAFE_DELETE(subtitleText);
}

void L6S1_PopAnanthaNote::Required()
{

}

void L6S1_PopAnanthaNote::ClosePopup(float delayTime)
{
	if (isVideoPlaying || !ProfileSystem::Instance->ActiveProfile.L6_S1_VaruniCutscenePlayed)
	{
		return;
	}
	GetObjectByID("Mask")->SetEnable(false);
	Popup::ClosePopup(delayTime);
}

void L6S1_PopAnanthaNote::OnPopupOpened()
{
	GetObjectByID("Mask")->SetEnable(true);
	Popup::OnPopupOpened();
}

bool L6S1_PopAnanthaNote::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	/*if (true)
	{
	isAnyTaskLeft = true;
	}*/

	return isAnyTaskLeft;
}

void L6S1_PopAnanthaNote::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	//if(true)
	//{
	//	//
	//}
	//else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
