//====================================
//	DQFYLH
//	HO System
//	Level 4 - Scene 03 - POPUP 1: ANTIQUE POT
//====================================

#include "L4S3_Pop1AntiquePot.h"

enum JAnimsL4S3
{
	anim_Pot,
	anim_PaperTake
};

const CRectangle  PaptusFlyRect(628.0f,293.0f,63.0f,67.0f);

const CRectangle  MemRectHoney(911.0f,299.0f,100.0f,69.0f);

L4S3_Pop1AntiquePot::L4S3_Pop1AntiquePot(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	isVideoPlaying		= false;
	isAnimationPlaying		= false;

	int PotArray[]		= {491,314,512,250,560,200,610,183,623,243,707,288,796,373,806,410,770,462,692,436,623,396,575,334,556,318,516,326};
	PotPoly				=  new CPolygon(14);
	PotPoly->Init(PotArray);

	int PaperArray[]	= {509,284,608,237,801,329,700,431};
	PaperPoly			=  new CPolygon(4);
	PaperPoly->Init(PaperArray);

	int BowlArray[]		= {605,267,675,236,718,242,751,341,697,412,607,427,548,344,547,312};
	InvBowlPoly			=  new CPolygon(8);
	InvBowlPoly->Init(BowlArray);


	//Popup load
	Load("LV4\\S3\\POP1\\P1.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Control::Audio->LoadSample(aVOL4MaryRefS5_1,	AudioVO);
	
	SetSparkleUpdate(false);
	Show(false);

	InitSceneElements();

	PotAnim = NULL;
	PotAnim = new JSONAnimator( anim_Pot, CC_CALLBACK_1(L4S3_Pop1AntiquePot::OnAnimationComplete, this ));
	PotAnim->parseAndLoadJSON("ANIM\\L4\\S3POTANIM.JSON", this );

	PaperTakeAnim = NULL;
	PaperTakeAnim = new JSONAnimator( anim_PaperTake, CC_CALLBACK_1(L4S3_Pop1AntiquePot::OnAnimationComplete, this ));
	PaperTakeAnim->parseAndLoadJSON("ANIM\\L4\\S3PAPERTAKEANIM.JSON", this );

	cv_Cutscn = NULL;
	time = 0;
	subIndex = 0;
	subMax = 16;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));
	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote3"));
	subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote4"));
	subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote5"));
	subtitles[5] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote6"));
	subtitles[6] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote7"));
	subtitles[7] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote8"));
	subtitles[8] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote9"));
	subtitles[9] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote10"));
	subtitles[10] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote11"));
	subtitles[11] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote12"));
	subtitles[12] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote13"));
	subtitles[13] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote14"));
	subtitles[14] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote15"));
	subtitles[15] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote16"));
	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	cv_Cutscn = new CVideo("OGV\\L4\\S3AMEEHADIARYNOTE.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	//cv_Cutscn->SetScale(1.334f);
	cv_Cutscn->SetScale(GFApp->video_aspectRatio);
	cv_Cutscn->SetSkipActive(1.0f);
	vidTimer = 0;

	GetObjectByID("paptus")->SetBlendMode(eBlend_AlphaAdditive);
	if(ProfileSystem::Instance->ActiveProfile.IsPaptusL4S3Collected)
	{
		GetObjectByID("paptus")->SetEnable(false);
	}


	if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S3Honey])
	{
		GetObjectByID("memhoney")->SetEnable(false);
	}

}

void L4S3_Pop1AntiquePot::Required()
{

}

void L4S3_Pop1AntiquePot::OnAnimationComplete(int id)
{
	isAnimationPlaying = false;
	switch (id)
	{
	case anim_Pot:	PotAnim->SetEnableObjects(false);
		break;
	case anim_PaperTake:	PaperTakeAnim->SetEnableObjects(false);
		cv_Cutscn->PlayVideo();
		Hud->EnableInput(false);
		Hud->HideHud(false);
		isVideoPlaying = true;
		isAnimationPlaying = true;
		return;
		break;
	default:
		break;
	}
}

void L4S3_Pop1AntiquePot::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}

void L4S3_Pop1AntiquePot::OnDiaryNoteTweenComplete(CObject *obj)
{
	MetalPaper->SetEnable(false);
	isAnimationPlaying = false;
	//Add New Info Text Add
	Hud->ShowNewInfoTextBanner();

	//Journal Entry Add
	if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L4P1_AmeehaDiaryNoteEnds])
	{
		//Flag Set
		ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L4P1_AmeehaDiaryNoteEnds] = true;

		//Add to Note Array
		ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L4P1_AmeehaDiaryNoteEnds;

		ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

		//Increment Notes
		ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
	}
}

void L4S3_Pop1AntiquePot::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open)
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	if (isVideoPlaying )
	{
		//Subtitles Code
		time = cv_Cutscn->GetTime();

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
			cv_Cutscn->isEndReached() ||
#endif
			vidTimer > 90)
		{
			subtitleText->SetText("");
			isAnimationPlaying = true;
			isVideoPlaying = false;
			cv_Cutscn->StopAndFadeOutVideo(2.0f);					
			Hud->AllowInput = true;
			Hud->ShowHud();

			Hud->UnlockCharacter(eChar11_Ameeha);
			ProfileSystem::Instance->ActiveProfile.isCharacterUnlocked[eChar8_AnanthaYoung] = true;

			Hud->ShowBannerText("L4together");
			MetalPaper->SetEnable(true);
			MetalPaper->SetAlpha(1.0f);
			float valArr[5] = {0, 220, 667, 0.45f, 15.0f};
			MetalPaper->TweenTo(valArr, 1.85f, TW_EASEQUADIN, true, CC_CALLBACK_1(L4S3_Pop1AntiquePot::OnDiaryNoteTweenComplete, this));
			Control::Audio->PlaySample(aVOL4MaryRefS5_1);
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

	if(ProfileSystem::Instance->ActiveProfile.L4_S3_IsMetalPaperTaken && !ProfileSystem::Instance->ActiveProfile.IsPaptusL4S3Collected && PaptusFlyRect.Intersection(&pos))
	{
		if(Control::Input->LBclicked())
		{
			Control::Audio->PlaySample(aSFXMagic2);
			GetObjectByID("paptus")->FadeOut(1.25f);
			Hud->CollectMysteryFlower(eArea_L4_S3_Cave, GetObjectByID("paptus")->Pos);
			return;
		}
	}

	if( PotPoly->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S3_IsPotRemoved )
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked() )
		{
			Cursor::SetMode(CA_Normal);
			PotAnim->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s3p1_vaseRemove");
			//Pot->SetEnable(false);
			isAnimationPlaying =true;
			ProfileSystem::Instance->ActiveProfile.L4_S3_IsPotRemoved  = true;
		}
		return;
	}
	else if( PaperPoly->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S3_IsMetalPaperTaken )
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked() )
		{
			Cursor::SetMode(CA_Normal);
			MetalPaper->SetEnable(false);
			ProfileSystem::Instance->ActiveProfile.L4_S3_IsMetalPaperTaken = true;
			PaperTakeAnim->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s3p1_MetalSheetRemove");
			isAnimationPlaying =true;
		}
		return;
	}
	else if( ProfileSystem::Instance->ActiveProfile.L4_S3_IsMetalPaperTaken && InvBowlPoly->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S3_IsInvBowlTaken )
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked() )
		{
			Cursor::SetMode(CA_Normal);
			Bowl->SetEnable(false);
			Hud->TakeInventoryItem( eInv_L4_S3_Pot );
			ProfileSystem::Instance->ActiveProfile.L4_S3_IsInvBowlTaken = true;
			ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S4_HillTop] = true;
			#ifdef _CEBUILD
				if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S3Honey])
					ClosePopup(1.5f);
			#else
				ClosePopup(1.5f);
			#endif 
			
		}
	}


#ifdef _CEBUILD
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S3Honey] && MemRectHoney.Intersection(&pos))
	{
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("memhoney")->FadeOut();
			Hud->ShowMemoryObjectCollected_CE(eMem_L4S3Honey, &GetObjectByID("memhoney")->Pos);
			if(ProfileSystem::Instance->ActiveProfile.L4_S3_IsInvBowlTaken)
				ClosePopup(1.5f);
		}
	}
#else
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S3Honey] && MemRectHoney.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("memhoney")->FadeOut();
			Hud->ShowMemoryObjectCollected_SE(eMem_L4S3Honey, &GetObjectByID("memhoney")->Pos);
		}
	}
#endif 
}

float L4S3_Pop1AntiquePot::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L4S3_Pop1AntiquePot::~L4S3_Pop1AntiquePot()
{
	Control::Audio->UnloadSample(aVOL4MaryRefS5_1);
	SAFE_DELETE(cv_Cutscn);
	SAFE_DELETE(PaperTakeAnim);
	SAFE_DELETE(PotAnim);

	SAFE_DELETE(subtitleText);
}

void L4S3_Pop1AntiquePot::InitSceneElements()
{
	PaperInHand		= GetObjectByID("PaperInHand");
	Pot			    = GetObjectByID("Pot");
	MetalPaper	    = GetObjectByID("MetalPaper");
	Bowl		    = GetObjectByID("Bowl");
	
	PaperInHand->SetEnable(false);

	if( ProfileSystem::Instance->ActiveProfile.L4_S3_IsMetalPaperTaken )
	{
		MetalPaper->SetEnable(false);
		Pot->SetEnable(false);
	}
	else if( ProfileSystem::Instance->ActiveProfile.L4_S3_IsPotRemoved )
		Pot->SetEnable(false);


}				  

bool L4S3_Pop1AntiquePot::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsPotRemoved )
		isAnyTaskLeft = true;
	if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsMetalPaperTaken )
		isAnyTaskLeft = true;
	if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsInvBowlTaken )
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L4S3_Pop1AntiquePot::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsPotRemoved )
	{
		Hud->InitHintTrailEmit(Pot->GetCenterX(),Pot->GetCenterY(), false );
	}
	if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsMetalPaperTaken )
	{
		Hud->InitHintTrailEmit(MetalPaper->GetCenterX(),MetalPaper->GetCenterY(), false );
	}
	if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsInvBowlTaken )
	{
		Hud->InitHintTrailEmit(Bowl->GetCenterX(),Bowl->GetCenterY(), false );
	}
	else
	{
	/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
