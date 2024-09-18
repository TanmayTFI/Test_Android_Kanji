//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 02 - POPUP 4: Pearls
//====================================

#include "L2S2_Pop4Pearls.h"
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


const CRectangle  InvItemRectPearl1(531.0f,398.0f,75.0f,75.0f);
const CRectangle  InvItemRectPearl2(700.0f,115.0f,78.0f,62.0f);
const CRectangle  InvItemRectPearl3(498.0f,228.0f,91.0f,68.0f);
const CRectangle  InvItemRectPearl4(816.0f,379.0f,116.0f,88.0f);


L2S2_Pop4Pearls::L2S2_Pop4Pearls(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV2\\S2\\POP4\\P4.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	

	if(ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl1Taken)
	{
		
		GetObjectByID("pearl1")->SetEnable(false);
			
	}
	if(ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl3Taken)
	{
		
		GetObjectByID("pearl3")->SetEnable(false);
			
	}
	if(ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl4Taken)
	{
		
		GetObjectByID("pearl4")->SetEnable(false);
			
	}

	if(ProfileSystem::Instance->ActiveProfile.L2_S2_NagaLeaf1Taken)
	{
		
		GetObjectByID("leaf1")->SetEnable(false);
			
	}
	if(ProfileSystem::Instance->ActiveProfile.L2_S2_NagaLeaf2Taken)
	{
		
		GetObjectByID("leaf2")->SetEnable(false);
			
	}

	Show(false);

}

void L2S2_Pop4Pearls::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	
}

void L2S2_Pop4Pearls::Required()
{

}

void L2S2_Pop4Pearls::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if( InvItemRectPearl1.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl1Taken)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			GetObjectByID("pearl1")->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L2S2_Pearl1, Control::Input->Mpos());
			ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl1Taken = true;
			if (ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl4Taken && ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl3Taken  && ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl1Taken)
			{
				//Show Objective completed
				if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L2O3_NagaPearls] && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L2O3_NagaPearls] )
				{
					ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L2O3_NagaPearls] = true;
					Hud->ShowObjectiveFX(eObj_L2O3_NagaPearls,true);
				}

				ProfileSystem::Instance->ActiveProfile.L2_S2_InvNagaPearlsCollected = true;
				ClosePopup(1.0f);
			}
		}
	}
	
	else if( InvItemRectPearl3.Intersection(&pos)  && !ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl3Taken)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			if(!ProfileSystem::Instance->ActiveProfile.L2_S2_NagaLeaf1Taken)
			{
				float tweenArray[5] = {0,998,334,1,0};
				GetObjectByID("leaf1")->TweenTo(tweenArray,1,0,false);
				Control::Audio->QuickLoadAndPlaySFX("s2p4_leafmove");
				ProfileSystem::Instance->ActiveProfile.L2_S2_NagaLeaf1Taken = true;
			}
			else 
			{
				GetObjectByID("pearl3")->SetEnable(false);
				Hud->TakeInventoryItem(eInv_L2S2_Pearl3, Control::Input->Mpos());
				ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl3Taken = true;
				if (ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl4Taken && ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl3Taken  && ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl1Taken)
				{
					//Show Objective completed
					if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L2O3_NagaPearls] && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L2O3_NagaPearls] )
					{
						ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L2O3_NagaPearls] = true;
						Hud->ShowObjectiveFX(eObj_L2O3_NagaPearls,true);
					}

					ProfileSystem::Instance->ActiveProfile.L2_S2_InvNagaPearlsCollected = true;
					ClosePopup(1.0f);
				}
			}
		}
	}
	else if( InvItemRectPearl4.Intersection(&pos)  && !ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl4Taken)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			if(!ProfileSystem::Instance->ActiveProfile.L2_S2_NagaLeaf2Taken)
			{
				float tweenArray[5] = {0,998,334,1,0};
				GetObjectByID("leaf2")->TweenTo(tweenArray,1,0,false);
				Control::Audio->QuickLoadAndPlaySFX("s2p4_leafmove");
				ProfileSystem::Instance->ActiveProfile.L2_S2_NagaLeaf2Taken = true;
			}
			else 
			{
				GetObjectByID("pearl4")->SetEnable(false);
				Hud->TakeInventoryItem(eInv_L2S2_Pearl4, Control::Input->Mpos());
				ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl4Taken = true;
				if (ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl4Taken && ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl3Taken  && ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl1Taken)
				{
					//Show Objective completed
					if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L2O3_NagaPearls] && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L2O3_NagaPearls] )
					{
						ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L2O3_NagaPearls] = true;
						Hud->ShowObjectiveFX(eObj_L2O3_NagaPearls,true);
					}

					ProfileSystem::Instance->ActiveProfile.L2_S2_InvNagaPearlsCollected = true;
					ClosePopup(1.0f);
				}
			}
		}
	}
	 
}

float L2S2_Pop4Pearls::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L2S2_Pop4Pearls::~L2S2_Pop4Pearls()
{

}

void L2S2_Pop4Pearls::TakeInventoryItem(EHOInventory ItemID)
{
	/*
	std::vector<int> vecInvIds;
	switch (ItemID)
	{
	
	default:
		break;
	}
	*/
}

void L2S2_Pop4Pearls::ReceiveMessage(int val)
{	

	
}

bool L2S2_Pop4Pearls::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl1Taken )
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl3Taken )
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl4Taken )
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L2S2_Pop4Pearls::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl1Taken )
	{
		Hud->InitHintTrailEmit(InvItemRectPearl1.GetCenterX(), InvItemRectPearl1.GetCenterY());
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl3Taken )
	{
		Hud->InitHintTrailEmit(InvItemRectPearl3.GetCenterX(), InvItemRectPearl3.GetCenterY());
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl4Taken )
	{
		Hud->InitHintTrailEmit(InvItemRectPearl4.GetCenterX(), InvItemRectPearl4.GetCenterY());
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}