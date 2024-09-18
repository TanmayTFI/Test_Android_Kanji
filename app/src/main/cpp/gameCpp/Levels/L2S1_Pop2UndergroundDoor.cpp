//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 01 - POPUP 2: UNDERGROUND DOOR
//====================================

#include "L2S1_Pop2UndergroundDoor.h"
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

const CRectangle  InvDestSnakeStatueRect(622.0f,237.0f,243.0f,148.0f);
const CRectangle  ScnRectUndergroundHO(528.0f,211.0f,405.0f,268.0f);


L2S1_Pop2UndergroundDoor::L2S1_Pop2UndergroundDoor(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV2\\S1\\POP2\\P2.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	Show(false);
	Hud = hud;

	doorClosed = GetObjectByID("Door01");
	doorHalfOpen = GetObjectByID("door02");
	doorFullOpen = GetObjectByID("door03");
	nagaSculpture1 = GetObjectByID("S-1");
	nagaSculpture2 = GetObjectByID("S-2");

	isToUndergroundAreaClicked = false;

	doorHalfOpen->SetEnable(false);
	nagaSculpture2->SetAlpha(0);
	nagaSculpture1->SetEnable(false);

	fireVFX = new SuperFX("PRT\\L2\\Fire3.INI", PointSystem::CreateCPoint(630,290,0), zOrder + 1);

	if (!ProfileSystem::Instance->ActiveProfile.L2_S1_IsNagaSculpturePlaced)
	{
		doorFullOpen->SetEnable(false);
	} 
	else
	{
		doorClosed->SetEnable(false);
	}
	
}

L2S1_Pop2UndergroundDoor::~L2S1_Pop2UndergroundDoor()
{
	SAFE_DELETE(fireVFX);
}

void L2S1_Pop2UndergroundDoor::Required()
{

}

void L2S1_Pop2UndergroundDoor::Update(float ds)
{
	Popup::Update(ds);

	CPoint pos = Control::Input->Mpos();

	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if(isAnimationPlaying)
		return;
	
	if (!ProfileSystem::Instance->ActiveProfile.L2_S1_IsNagaSculpturePlaced)
	{
		if (InvDestSnakeStatueRect.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if (Control::Input->LBclicked() && !Hud->IsDraggingItem())
			{
				Hud->ShowBannerText("L2door");
			} 
		}
		/*if (ProfileSystem::Instance->ActiveProfile.L2_S1_IsNagaSculpturePlaced && !fireVFX->Active)
		{
			fireVFX->Start();
		}*/

	}
	//else if(ScnRectUndergroundHO.Intersection(&pos))
	//{
	//	Cursor::SetMode(CA_ExitUp, eArea_L2_S1_UndergroundHO);
	//	if (Control::Input->LBclicked())
	//	{
	//		isToUndergroundAreaClicked = true;
	//		ClosePopup();
	//		//ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S2_NagaTemple;
	//		//Hud->SendNewAreaMessage();
	//	}
	//}
}

void L2S1_Pop2UndergroundDoor::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (InvDestSnakeStatueRect.Intersection(&dropPos))
	{
		if (itemVal == eInv_L2S5_NagaSculpture)
		{
			if (!ProfileSystem::Instance->ActiveProfile.L2_S1_IsNagaSculpturePlaced)
			{
				isAnimationPlaying = true;
				Control::Audio->QuickLoadAndPlaySFX("s1p2_dooropen");

				nagaSculpture1->SetEnable(true);
				float valArr1[5] = {1,nagaSculpture2->Pos.x, nagaSculpture2->Pos.y, 1,0};
				nagaSculpture2->TweenTo(valArr1,1.25f,0,false);
				float valArr[5] = {0, nagaSculpture1->Pos.x, nagaSculpture1->Pos.y, 1,0};
				nagaSculpture1->TweenTo(valArr,1.25f,0,true,CC_CALLBACK_1(L2S1_Pop2UndergroundDoor::OnFirstTweenComplete,this));
				ProfileSystem::Instance->ActiveProfile.L2_S1_IsNagaSculpturePlaced = true;

				Hud->ReleaseDrag();
				Hud->CloseInventory(); 

				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S5_NagaSculpture] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S5_NagaSculpture);
			} 
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		return;
	}
}

void L2S1_Pop2UndergroundDoor::OnFirstTweenComplete(CObject *obj)
{
	float valArr1[5] = {0,doorClosed->Pos.x, doorClosed->Pos.y, 1,0};
	doorClosed->TweenTo(valArr1,1.0f,0,false);
	nagaSculpture2->SetEnable(false);
	doorHalfOpen->SetEnable(true);
	doorHalfOpen->SetAlpha(0);
	float valArr[5] = {1,doorHalfOpen->Pos.x, doorHalfOpen->Pos.y, 1,0};
	doorHalfOpen->TweenTo(valArr,0.75f,0,true,CC_CALLBACK_1(L2S1_Pop2UndergroundDoor::OnSecondTweenComplete,this));
}

void L2S1_Pop2UndergroundDoor::OnSecondTweenComplete(CObject *obj)
{
	doorHalfOpen->SetAlpha(0.5f);
	float valArr1[5] = {0,doorHalfOpen->Pos.x, doorHalfOpen->Pos.y, 1,0};
	doorHalfOpen->TweenTo(valArr1,0.75f,0,false);
	doorFullOpen->SetEnable(true);
	doorFullOpen->SetAlpha(0.5);
	float valArr[5] = {1,doorFullOpen->Pos.x, doorFullOpen->Pos.y, 1,0};
	doorFullOpen->TweenTo(valArr,0.75f,0,true,CC_CALLBACK_1(L2S1_Pop2UndergroundDoor::OnThirdTweenComplete,this));
	
	fireVFX->Start();
}

void L2S1_Pop2UndergroundDoor::OnThirdTweenComplete(CObject *obj)
{
	isAnimationPlaying = false;
	ProfileSystem::Instance->ActiveProfile.L2_S1_IsUndergroundHatchOpened = true;
	ClosePopup();
}

void L2S1_Pop2UndergroundDoor::OnPopupOpened()
{
	fireVFX->StopImmediate();
	if (ProfileSystem::Instance->ActiveProfile.L2_S1_IsNagaSculpturePlaced)
	{
		fireVFX->Start();
	}
}

void L2S1_Pop2UndergroundDoor::OnPopupCloseBtnClicked()
{
	fireVFX->StopImmediate();
}

bool L2S1_Pop2UndergroundDoor::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S1_IsNagaSculpturePlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_InvNagaSculptureTaken)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L2S1_Pop2UndergroundDoor::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	
	if(!ProfileSystem::Instance->ActiveProfile.L2_S1_IsNagaSculpturePlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_InvNagaSculptureTaken)
	{
		Hud->ShowHintForInvItem(eInv_L2S5_NagaSculpture, &InvDestSnakeStatueRect);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}
