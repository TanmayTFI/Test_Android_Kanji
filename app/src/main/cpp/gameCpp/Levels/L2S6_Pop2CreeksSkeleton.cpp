//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 06 - POPUP 2: CREEK'S SKELETON
//====================================

#include "L2S6_Pop2CreeksSkeleton.h"
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

const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  PopCircleAreaRect(502.0f,153.0f,364.0f,360.0f);
const CRectangle  SkeletonHeadRect(368.0f,288.0f,248.0f,162.0f);

const CRectangle  MemoryRectSpecs(434.0f-60.0f, 413.0f-60.0f, 190.0f+110.0f, 105.0f+110.0f);

const CRectangle  PaptusFlyRect(655.0f,177.0f,59.0f,57.0f);

L2S6_Pop2CreeksSkeleton::L2S6_Pop2CreeksSkeleton(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV2\\S6\\POP2\\P2.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	Show(false);
	Hud = hud;

	for (int i = 0; i < 10; i++)
	{
		std::string str = "_f";
		str = std::to_string(i) + str;
		GetObjectByID(str)->SetPos(GetObjectByID("0_f")->GetPosPointer());
		
		if(i != ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[0])
			GetObjectByID(str)->SetEnable(false);

		str = "_s";
		str = std::to_string(i) + str;
		GetObjectByID(str)->SetPos(GetObjectByID("0_s")->GetPosPointer());

		if(i != ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[1])
			GetObjectByID(str)->SetEnable(false);

		str = "_t";
		str = std::to_string(i) + str;
		GetObjectByID(str)->SetPos(GetObjectByID("0_t")->GetPosPointer());

		if(i != ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[2])
			GetObjectByID(str)->SetEnable(false);

	}

	int pop3Rect[] = {680,336,614,407,663,467,739,402,678,332};
	pop3ClickPoly =  new ClickablePolygon(2, 5, pop3Rect, CC_CALLBACK_1(L2S6_Pop2CreeksSkeleton::onPopAreaClicked,this));
	pop3ClickPoly->SetMouseOverCallback(CC_CALLBACK_1(L2S6_Pop2CreeksSkeleton::onPopAreaMouseOver,this));
	pop3ClickPoly->SetMouseOutCallback(CC_CALLBACK_1(L2S6_Pop2CreeksSkeleton::onPopAreaMouseOut,this));

	pop3Locket = new L2S6_Pop3Locket(2, Hud, NULL, eZOrder_Popup + 200, pop3ClickPoly->center, CC_CALLBACK_1(L2S6_Pop2CreeksSkeleton::onPopupClosed, this));
	pop3Locket->SetPopupBoundingBox(PopCircleAreaRect);
	PopUpArray[0] = pop3Locket;

	int pop4Rect[] = {641,170,647,238,726,240,715,166,638,167};
	pop4ClickPoly =  new ClickablePolygon(3, 5, pop4Rect, CC_CALLBACK_1(L2S6_Pop2CreeksSkeleton::onPopAreaClicked,this));
	pop4ClickPoly->SetMouseOverCallback(CC_CALLBACK_1(L2S6_Pop2CreeksSkeleton::onPopAreaMouseOver,this));
	pop4ClickPoly->SetMouseOutCallback(CC_CALLBACK_1(L2S6_Pop2CreeksSkeleton::onPopAreaMouseOut,this));

	pop4Holder = new L2S6_Pop4Holder(3, Hud, NULL, eZOrder_Popup + 200, pop4ClickPoly->center, CC_CALLBACK_1(L2S6_Pop2CreeksSkeleton::onPopupClosed, this));
	pop4Holder->SetPopupBoundingBox(PopCircleAreaRect);
	PopUpArray[1] = pop4Holder;

	GetObjectByID("Popup1_CreeksSkeletonpaper")->SetEnable(false);

	GetObjectByID("paptus")->SetEnable(false);

	if (ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3HolderOpen)
	{
		GetObjectByID("closed-holder")->SetEnable(false);
		for (int i = 0; i < 10; i++)
		{
			std::string str = "_f";
			str = std::to_string(i) + str;
			GetObjectByID(str)->SetEnable(false);

			str = "_s";
			str = std::to_string(i) + str;
			GetObjectByID(str)->SetEnable(false);

			str = "_t";
			str = std::to_string(i) + str;
			GetObjectByID(str)->SetEnable(false);

		}
		if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3PaperPuzClueTaken)
		{
			GetObjectByID("Popup1_CreeksSkeletonpaper")->SetEnable(true);
		}
		else
		{
			GetObjectByID("paptus")->SetEnable(true);
		}
	}
	else
	{
		GetObjectByID("OpenLid")->SetEnable(false);
	}

	if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP2LocketOpen)
	{
		GetObjectByID("LocketOpened")->SetEnable(false);
	}

	MemSpectacles = GetObjectByID("Specs");

	GetObjectByID("paptus")->SetBlendMode(eBlend_AlphaAdditive);
	if(ProfileSystem::Instance->ActiveProfile.IsPaptusL2S6Collected)
	{
		GetObjectByID("paptus")->SetEnable(false);
	}


	if( ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S6Spectacles] )
	{
		MemSpectacles->SetEnable(false);
	}

	ActivePopUpID = popupId;
}

void L2S6_Pop2CreeksSkeleton::onPopAreaMouseOut(int popId)
{
	isMouseOverPopup = false;
}

void L2S6_Pop2CreeksSkeleton::onPopAreaMouseOver(int popId)
{
	if (ActivePopUpID != 1)
		return;

	switch (popId)
	{
	case 2: isMouseOverPopup = true;	break;
	case 3:	if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3PaperPuzClueTaken)
										isMouseOverPopup = true;
										break;
	default:
		break;
	}

}

void L2S6_Pop2CreeksSkeleton::onPopAreaClicked(int popId)
{

	if (ActivePopUpID != 1)
		return;
	if (Popup::GetPopupState() != ePopupState_Open)
		return;

	switch (popId)
	{
	case 2:
		isAnimationPlaying = true; //To avoid closing this popup on click outside
		ActivePopUpID = 2;
		pop3Locket->OpenPopup();
		break;
	case 3:
		if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3PaperPuzClueTaken)
		{
			isAnimationPlaying = true; //To avoid closing this popup on click outside
			ActivePopUpID = 3;
			pop4Holder->OpenPopup();
		}
		break;
	default:
		break;
	}
}

void L2S6_Pop2CreeksSkeleton::onPopupClosed(Popup * popup)
{
	//K_LOG("popup closed %d", popup->GetPopupId());

	ActivePopUpID = 1;

	isAnimationPlaying = false;

	switch (popup->GetPopupId())
	{
	case 2:
		if (ProfileSystem::Instance->ActiveProfile.L2_S6_IsP2LocketOpen)
			GetObjectByID("LocketOpened")->SetEnable(true);
		break;
	case 3:
		for (int i = 0; i < 10; i++)
		{
			std::string str = "_f";
			str = std::to_string(i) + str;
			GetObjectByID(str)->SetEnable(false);

			str = "_s";
			str = std::to_string(i) + str;
			GetObjectByID(str)->SetEnable(false);

			str = "_t";
			str = std::to_string(i) + str;
			GetObjectByID(str)->SetEnable(false);

		}

		if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3HolderOpen)
		{
			std::string str = "_f";
			str = std::to_string(ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[0]) + str;
			GetObjectByID(str)->SetEnable(true);
			str = "_s";
			str = std::to_string(ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[1]) + str;
			GetObjectByID(str)->SetEnable(true);
			str = "_t";
			str = std::to_string(ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[2]) + str;
			GetObjectByID(str)->SetEnable(true);
		}

		GetObjectByID("Popup1_CreeksSkeletonpaper")->SetEnable(false);

		if (ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3HolderOpen)
		{
			GetObjectByID("closed-holder")->SetEnable(false);
			GetObjectByID("OpenLid")->SetEnable(true);

			if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3PaperPuzClueTaken)
			{
				GetObjectByID("Popup1_CreeksSkeletonpaper")->SetEnable(true);
			}
			else
			{
				GetObjectByID("paptus")->SetEnable(true);
			}
		}
		else
		{
			GetObjectByID("OpenLid")->SetEnable(false);
		}
		break;
	default:
		break;
	}

}

void L2S6_Pop2CreeksSkeleton::Required()
{

}

void L2S6_Pop2CreeksSkeleton::Update(float ds)
{
	Popup::Update(ds);

	CPoint pos = Control::Input->Mpos();

	if (Popup::GetPopupState() != ePopupState_Open)
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	if (ActivePopUpID != 1)
		return;

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (isMouseOverPopup)
	{
		Cursor::SetMode(CA_Look);
		return;
	}

	if(ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3PaperPuzClueTaken && !ProfileSystem::Instance->ActiveProfile.IsPaptusL2S6Collected && PaptusFlyRect.Intersection(&pos))
	{
		if(Control::Input->LBclicked())
		{
			Control::Audio->PlaySample(aSFXMagic2);
			GetObjectByID("paptus")->FadeOut(1.25f);
			Hud->CollectMysteryFlower(eArea_L2_S6_TunnelEnd, GetObjectByID("paptus")->Pos);
		}
	}

	if (SkeletonHeadRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);

		if (Control::Input->LBclicked())
		{
			if (ProfileSystem::Instance->ActiveProfile.L2_S6_IsP2LocketOpen)
			{
				Hud->ShowBannerText("L2creek");
			}
			else if (ProfileSystem::Instance->ActiveProfile.L2_S6_IsWoodCutDown)
			{
				Hud->ShowBannerText("L2skeleton");
			}
		}
	}

#ifdef _CEBUILD
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S6Spectacles] && MemoryRectSpecs.Intersection(&pos))
	{			
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("Specs")->FadeOut(2.0f);
			Hud->ShowMemoryObjectCollected_CE(eMem_L2S6Spectacles, &GetObjectByID("Specs")->Pos);

			if(ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3PaperPuzClueTaken && ProfileSystem::Instance->ActiveProfile.IsPaptusL2S6Collected)
			{
				GetObjectByID("Specs")->SetEnable(false);
				ClosePopup(1.25f);
			}
		}
	}
#else
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S6Spectacles] && MemoryRectSpecs.Intersection(&pos))
	{	
		Cursor::SetMode(CA_Help);
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("Specs")->FadeOut(2.0f);
			Hud->ShowMemoryObjectCollected_SE(eMem_L2S6Spectacles, &GetObjectByID("Specs")->Pos);
		}
	}
#endif 

}

float L2S6_Pop2CreeksSkeleton::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L2S6_Pop2CreeksSkeleton::~L2S6_Pop2CreeksSkeleton()
{
	SAFE_DELETE(pop3ClickPoly);
	SAFE_DELETE(pop4ClickPoly);	

	SAFE_DELETE(pop3Locket);
	SAFE_DELETE(pop4Holder);
}

void L2S6_Pop2CreeksSkeleton::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	GFInstance->WrongInvDropMsg(Hud);
}

bool L2S6_Pop2CreeksSkeleton::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP2LocketOpen)
		isAnyTaskLeft = true;
	else if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3PaperPuzClueTaken)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L2S6_Pop2CreeksSkeleton::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	if (ActivePopUpID != 1)
	{
		PopUpArray[ActivePopUpID - 2]->ProcessHint();
		return;
	}

	if (!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP2LocketOpen)
	{
		Hud->InitHintTrailEmit(pop3ClickPoly->center.x, pop3ClickPoly->center.y);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3PaperPuzClueTaken)
	{
		Hud->InitHintTrailEmit(pop4ClickPoly->center.x, pop4ClickPoly->center.y);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
