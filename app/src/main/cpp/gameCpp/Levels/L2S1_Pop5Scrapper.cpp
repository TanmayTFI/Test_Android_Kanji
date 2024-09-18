//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 01 - POPUP 5: SCRAPTER
//====================================

#include "L2S1_Pop5Scrapper.h"
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

const CRectangle  InvItemRectScrapter(633.0f,207.0f,104.0f,234.0f);

L2S1_Pop5Scrapper::L2S1_Pop5Scrapper(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)

{

	//Popup load
	Load("LV2\\S1\\POP5\\P5.SCN");
	SetFrameAndCloseButton(true);
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	Show(false);
	Hud = hud;

	Scrapper = GetObjectByID("Scraper");

	if( ProfileSystem::Instance->ActiveProfile.L2_S1_InvScrapperTaken)
		Scrapper->SetEnable(false);
}

void L2S1_Pop5Scrapper::Required()
{

}

void L2S1_Pop5Scrapper::Update(float ds)
{
	Popup::Update(ds);

	CPoint pos = Control::Input->Mpos();

	if (Popup::GetPopupState() != ePopupState_Open)
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if( !ProfileSystem::Instance->ActiveProfile.L2_S1_InvScrapperTaken)
	{
		if( InvItemRectScrapter.Intersection( &pos) )
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked() && !Hud->IsDraggingItem())
			{
				Scrapper->SetEnable(false);
				TakeInventoryItem(eInv_L2S1_Scrapper);
				ClosePopup(2.5f);
				return;
			}		
		}
	}
}

void L2S1_Pop5Scrapper::TakeInventoryItem(EHOInventory ItemID)
{
	std::vector<int> vecInvIds;
	switch (ItemID)
	{
	case eInv_L2S1_Scrapper:
		Hud->InvBoxCtrl->Activate(1);			
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S1_Scrapper] = true;
		ProfileSystem::Instance->ActiveProfile.L2_S1_InvScrapperTaken = true;			
		vecInvIds.push_back(eInv_L2S1_Scrapper);
		Hud->ShowInventory(&vecInvIds);
		Hud->ShowBannerText("L2useful");
		break;
	default:
		break;
	}
}

void L2S1_Pop5Scrapper::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	GFInstance->WrongInvDropMsg(Hud);
}


bool L2S1_Pop5Scrapper::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S1_InvScrapperTaken)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L2S1_Pop5Scrapper::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S1_InvScrapperTaken)
	{
		Hud->InitHintTrailEmit(InvItemRectScrapter.GetCenterX(), InvItemRectScrapter.GetCenterY());
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
