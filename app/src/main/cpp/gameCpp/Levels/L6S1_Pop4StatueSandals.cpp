//====================================
//	DQFYLH
//	HO System
//	Level 6 - Scene 01 - POPUP 4: 
//====================================

#include "L6S1_Pop4StatueSandals.h"

const CRectangle  InvItemRectMetalContainer(759.0f,102.0f,201.0f,130.0f);
const CRectangle  ScrewRect(373.0f,342.0f,82.0f,81.0f);

const CRectangle  PopAreaRect(479.0f,96.0f,494.0f,631.0f);

const CRectangle  PaptusFlyRect(492.0f,371.0f,63.0f,71.0f);

L6S1_Pop4StatueSandals::L6S1_Pop4StatueSandals(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV6\\S1\\P4\\P4.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	/*if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvMetalContainerTaken)
	{
	GetObjectByID("PanINV")->SetEnable(false);
	}*/

	if (ProfileSystem::Instance->ActiveProfile.L6_S1_ScrewUnsealed)
	{
		GetObjectByID("Button")->SetRelativePos(PointSystem::CreateCPoint(10,-10,0));
		GetObjectByID("chappal_slide-_open")->SetRelativePos(PointSystem::CreateCPoint(40,50,0));
		GetObjectByID("ClipClosed")->SetEnable(false);
		GetObjectByID("ClipOpen")->SetRelativePos(PointSystem::CreateCPoint(40,50,0));
		GetObjectByID("Emblum-OnChappal'")->SetRelativePos(PointSystem::CreateCPoint(40,50,0));
	}
	else
	{
		GetObjectByID("ClipOpen")->SetEnable(false);
	}

	/*if (ProfileSystem::Instance->ActiveProfile.L6_S1_ChappalKnobRemoved)
	{
	GetObjectByID("chappal_golder")->SetEnable(false);
	}
	*/
	if (ProfileSystem::Instance->ActiveProfile.L6_S1_LetterOpened)
	{
		GetObjectByID("Letter")->SetEnable(false);			
	}

	GetObjectByID("paptus")->SetBlendMode(eBlend_AlphaAdditive);
	if(ProfileSystem::Instance->ActiveProfile.IsPaptusL6S1popCollected)
	{
		GetObjectByID("paptus")->SetEnable(false);
	}

	isAmeehaNotePopupActive = false;

	int ArrayPts[] = {549,314,538,321,585,374,603,369,550,310};
	CPolygon *CPolyObjName=  new CPolygon(5);
	CPolyObjName->Init(ArrayPts);
	popUp = new L6S1_PopAnanthaNote(1, Hud, NULL, eZOrder_Popup + 200, CPolyObjName->center,CC_CALLBACK_1(L6S1_Pop4StatueSandals::OnPopupClosed,this));

	//Localized
	CObject* temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L6\\AnanthaOurSavior.SPR", 680, 440, eZOrder_Popup + 210);
	temp->SetUID("text");
	temp->Show(false);
	popUp->PushObject(temp);

	popUp->SetPopupBoundingBox(PopAreaRect);
	delete(CPolyObjName);

	/*isVideoPlaying = false;
	lpVidVaruniCutScn = NULL;

	CRectangle RectVidFull(171, 96, 1024, 576);
	lpVidVaruniCutScn = new CVideo("OGV\\L6\\S1VARUNICURSE.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	lpVidVaruniCutScn->SetScale(1.334f);*/
	Control::Audio->LoadSample(aVOL6MaryRefS1_2,	AudioVO);
	Control::Audio->LoadSample(aVOL6MaryRefS1_3,	AudioVO);
	int ArrayPts1[] = {446,247,433,353,360,444,518,526,581,530,618,441,637,329,448,248};
	notePoly =  new CPolygon(8);
	notePoly->Init(ArrayPts1);
	
	SetSparkleUpdate(false);
	Show(false);
}


void L6S1_Pop4StatueSandals::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open)
		return;
	if (popUp->GetPopupState() != ePopupState_Inactive && popUp->GetPopupState() != ePopupState_Closed)
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if(!isAmeehaNotePopupActive && ProfileSystem::Instance->ActiveProfile.L6_S1_LetterOpened && !ProfileSystem::Instance->ActiveProfile.IsPaptusL6S1popCollected && PaptusFlyRect.Intersection(&pos))
	{
		if(Control::Input->LBclicked())
		{
			Control::Audio->PlaySample(aSFXMagic2);
			GetObjectByID("paptus")->FadeOut(1.25f);
			Hud->CollectMysteryFlower(eArea_L6_S1_FloatingIsland1, GetObjectByID("paptus")->Pos);
			return;
		}
	}

	/* if (isVideoPlaying )
	{
	if(lpVidVaruniCutScn->isEndReached())
	{
	ProfileSystem::Instance->ActiveProfile.L6_S1_VaruniCutscenePlayed = true;
	isVideoPlaying = false;
	lpVidVaruniCutScn->StopAndFadeOutVideo(2.0f);					
	Hud->AllowInput = true;
	Hud->ShowHud();
	Hud->SendUpdateSceneMessage();
	} 
	return;
	}*/
	/*
	if (!ProfileSystem::Instance->ActiveProfile.L6_S1_InvMetalContainerTaken && InvItemRectMetalContainer.Intersection(&pos))
	{
	Cursor::SetMode(CA_HandTake);
	if (Control::Input->LBclicked())
	{
	Hud->TakeInventoryItem(eInv_L6_S1_MetalContainer);
	GetObjectByID("PanINV")->SetEnable(false);
	ProfileSystem::Instance->ActiveProfile.L6_S1_InvMetalContainerTaken = true;
	if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvEmblemTaken)
	{
	ClosePopup(1);
	}
	}
	}
	else *//*if (GetObjectByID("chappal_golder")->GetWorldRect().Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S1_ChappalKnobRemoved)
	{
	Cursor::SetMode(CA_HandTake);
	if (Control::Input->LBclicked())
	{
	GetObjectByID("chappal_golder")->SetEnable(false);
	ProfileSystem::Instance->ActiveProfile.L6_S1_ChappalKnobRemoved = true;
	}
	}
	else */if (notePoly->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S1_LetterOpened)
	{
		Cursor::SetMode(CA_Look);

		if (Control::Input->LBclicked())
		{
			GetObjectByID("Letter")->SetEnable(false);
			popUp->OpenPopup();
			isAmeehaNotePopupActive = true;
			ProfileSystem::Instance->ActiveProfile.L6_S1_LetterOpened = true;
		}
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L6_S1_ScrewUnsealed && ScrewRect.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L6_S1_LetterOpened)
	{
		Cursor::SetMode(CA_HandPointy);
		if (Control::Input->LBclicked()/* && ProfileSystem::Instance->ActiveProfile.L6_S1_ChappalKnobRemoved*/)
		{
			Control::Audio->QuickLoadAndPlaySFX("s1p4_sandalSlide");
			GetObjectByID("Button")->SetRelativePos(PointSystem::CreateCPoint(10,-10,0));
			GetObjectByID("chappal_slide-_open")->SetRelativePos(PointSystem::CreateCPoint(40,50,0));
			GetObjectByID("ClipOpen")->SetEnable(true);
			GetObjectByID("ClipClosed")->SetEnable(false);
			GetObjectByID("ClipOpen")->SetRelativePos(PointSystem::CreateCPoint(40,50,0));
			GetObjectByID("Emblum-OnChappal'")->SetRelativePos(PointSystem::CreateCPoint(40,50,0));
			ProfileSystem::Instance->ActiveProfile.L6_S1_ScrewUnsealed = true;
		}
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S1_ScrewUnsealed && GetObjectByID("Emblum-OnChappal'")->GetWorldRect().Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S1_InvEmblemTaken)
	{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
			GetObjectByID("Emblum-OnChappal'")->SetEnable(false);
			ProfileSystem::Instance->ActiveProfile.L6_S1_InvEmblemTaken = true;
			Hud->TakeInventoryItem(eInv_L6_S1_Emblem);
			//if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvMetalContainerTaken)
			{
				ClosePopup(1);
			}
		}

	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S1_VaruniCutscenePlayed && !ProfileSystem::Instance->ActiveProfile.L6_S1_MaryFirstBannerTextShown)
	{
		Hud->ShowBannerText("L6cruel");
		Control::Audio->PlaySample(aVOL6MaryRefS1_2);
		ProfileSystem::Instance->ActiveProfile.L6_S1_MaryFirstBannerTextShown = true;
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S1_MaryFirstBannerTextShown && !ProfileSystem::Instance->ActiveProfile.L6_S1_MarySecondBannerTextShown)
	{
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L6ooh");
			Control::Audio->PlaySample(aVOL6MaryRefS1_3);
			ProfileSystem::Instance->ActiveProfile.L6_S1_MarySecondBannerTextShown = true;
		}
	}

}

void L6S1_Pop4StatueSandals::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	/*switch (itemVal)
	{

	default:*/
	GFInstance->WrongInvDropMsg(Hud);
	/*break;
	}*/
}


float L6S1_Pop4StatueSandals::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L6S1_Pop4StatueSandals::~L6S1_Pop4StatueSandals()
{
	/*SAFE_DELETE(lpVidVaruniCutScn);*/
	SAFE_DELETE(popUp);
	SAFE_DELETE(notePoly);

	Control::Audio->UnloadSample(aVOL6MaryRefS1_2);
	Control::Audio->UnloadSample(aVOL6MaryRefS1_3);
}

void L6S1_Pop4StatueSandals::Required()
{

}

void L6S1_Pop4StatueSandals::OnPopupClosed(Popup * popup)
{
	isAmeehaNotePopupActive = false;

	Hud->ShowNewInfoTextBanner();
	//Hud->AllowInput = false;
	//Hud->HideHud();
	//lpVidVaruniCutScn->SetSkipActive();
	//isVideoPlaying = true;
	//lpVidVaruniCutScn->PlayVideo();
}


void L6S1_Pop4StatueSandals::ClosePopup(float delayTime)
{
	if (popUp->GetPopupState() == ePopupState_Open/* || isVideoPlaying*/)
	{
		return;
	}

	Popup::ClosePopup(delayTime);
}

bool L6S1_Pop4StatueSandals::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if (/*!ProfileSystem::Instance->ActiveProfile.L6_S1_ChappalKnobRemoved ||
		*/
		!ProfileSystem::Instance->ActiveProfile.L6_S1_LetterOpened||
		!ProfileSystem::Instance->ActiveProfile.L6_S1_ScrewUnsealed||
		(ProfileSystem::Instance->ActiveProfile.L6_S1_ScrewUnsealed && !ProfileSystem::Instance->ActiveProfile.L6_S1_InvEmblemTaken))
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L6S1_Pop4StatueSandals::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	if (isAmeehaNotePopupActive)
		return;
	/*if(!ProfileSystem::Instance->ActiveProfile.L6_S1_ChappalKnobRemoved)
	{
	Hud->InitHintTrailEmit(&GetObjectByID("chappal_golder")->GetWorldRect());
	}
	else */if (!ProfileSystem::Instance->ActiveProfile.L6_S1_LetterOpened)
	{
		//Hud->InitHintTrailEmit(&GetObjectByID("Letter")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle letterObjRect =GetObjectByID("Letter")->GetWorldRect();
		Hud->InitHintTrailEmit(&letterObjRect);
		
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L6_S1_ScrewUnsealed)
	{
		Hud->InitHintTrailEmit(&ScrewRect);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S1_ScrewUnsealed && !ProfileSystem::Instance->ActiveProfile.L6_S1_InvEmblemTaken)
	{
		//Hud->InitHintTrailEmit(&GetObjectByID("Emblum-OnChappal'")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle chappalObjRect =GetObjectByID("Emblum-OnChappal'")->GetWorldRect();
		Hud->InitHintTrailEmit(&chappalObjRect);
		
		
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
