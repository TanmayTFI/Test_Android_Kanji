//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L3S4_Pop1BambooDoor.h"
#include "Hud.h"

const CRectangle  InvDestRectBambooStick(458.0f,91.0f,376.0f,484.0f);
const CRectangle  InvItemRectSharpRod(422.0f,376.0f,133.0f,198.0f);

const CRectangle  MemoryRectCat(903.0f,454.0f,85.0f,113.0f);

L3S4_Pop1BambooDoor::L3S4_Pop1BambooDoor(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV3\\S4\\POP1\\P1.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	GetObjectByID("sickle")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L3_S4_InvSharpRodTaken)
		GetObjectByID("sharprod")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L3_S4_InvBambooStickTaken)
	{
		GetObjectByID("bamboostick")->SetEnable(false);
		GetObjectByID("ropetop")->SetEnable(false);
		GetObjectByID("ropedown")->SetEnable(false);
	}

	JAnim_BambooCut = new JSONAnimator(0, CC_CALLBACK_1(L3S4_Pop1BambooDoor::OnAnimationComplete, this));
	JAnim_BambooCut->parseAndLoadJSON("ANIM\\L3\\S4BAMBOOCUT.JSON", this);

	if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S4CatStatue])
	{
		GetObjectByID("memcat")->SetEnable(false);
	}
}

void L3S4_Pop1BambooDoor::Required()
{

}

void L3S4_Pop1BambooDoor::OnAnimationComplete(int animID)
{
	isAnimationPlaying = false;

	ProfileSystem::Instance->ActiveProfile.L3_S4_InvBambooStickTaken = true;

	GetObjectByID("bamboostick")->SetEnable(false);
	GetObjectByID("ropetop")->SetEnable(false);
	GetObjectByID("ropedown")->SetEnable(false);

	Hud->TakeInventoryItem(eInv_L3S4_BambooStick);

	if(ProfileSystem::Instance->ActiveProfile.L3_S4_InvSharpRodTaken && ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S4CatStatue])
		ClosePopup(1.5f);

}


void L3S4_Pop1BambooDoor::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open)
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

	//handle interactions here
	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_InvSharpRodTaken)
	{
		if(InvItemRectSharpRod.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				GetObjectByID("sharprod")->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L3_S4_InvSharpRodTaken = true;

				Hud->ShowBannerText("L3tear");

				Hud->TakeInventoryItem(eInv_L3S4_SharpTool);

				if(ProfileSystem::Instance->ActiveProfile.L3_S4_InvBambooStickTaken && ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S4CatStatue])
					ClosePopup(1.5f);
			}
			return;
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_InvBambooStickTaken)
	{
		if(InvDestRectBambooStick.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if(Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L3bamboo");
			}
		}
	}

#ifdef _CEBUILD
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S4CatStatue] && MemoryRectCat.Intersection(&pos))
	{
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("memcat")->FadeOut();
			Hud->ShowMemoryObjectCollected_CE(eMem_L3S4CatStatue, &GetObjectByID("memcat")->Pos);
			if(ProfileSystem::Instance->ActiveProfile.L3_S4_InvBambooStickTaken && ProfileSystem::Instance->ActiveProfile.L3_S4_InvSharpRodTaken)
			{
				GetObjectByID("memcat")->SetEnable(false);
				ClosePopup(1.5f);
			}
		}
	}
#else
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S4CatStatue] && MemoryRectCat.Intersection(&pos))
	{	
		Cursor::SetMode(CA_Help);
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("memcat")->FadeOut();
			Hud->ShowMemoryObjectCollected_SE(eMem_L3S4CatStatue, &GetObjectByID("memcat")->Pos);
		}
	}
#endif 
}

void L3S4_Pop1BambooDoor::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(itemVal == eInv_L3S2_Sickle)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S4_InvBambooStickTaken && InvDestRectBambooStick.Intersection(&dropPos))
		{
			isAnimationPlaying = true;

			//Inbetween user alt+f4 close - handle starts....
			ProfileSystem::Instance->ActiveProfile.L3_S4_InvBambooStickTaken = true;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S4_BambooStick] = true;
			////////////////

			Control::Audio->QuickLoadAndPlaySFX("s4p1_bambooropecut");

			JAnim_BambooCut->PlayAnim();

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			if (ProfileSystem::Instance->ActiveProfile.L3_S2_M6_PillerPuzzleComplete)
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_Sickle] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S2_Sickle);
			}
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
}

void L3S4_Pop1BambooDoor::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here
	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_InvSharpRodTaken)
	{
		Hud->InitHintTrailEmit(&InvItemRectSharpRod);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvSickleTaken && !ProfileSystem::Instance->ActiveProfile.L3_S4_InvBambooStickTaken)
	{
		Hud->ShowHintForInvItem(eInv_L3S2_Sickle, &InvDestRectBambooStick);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L3S4_Pop1BambooDoor::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L3_S4_InvSharpRodTaken)
	{
		isAnyTaskLeft = true;
	}
	else if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvSickleTaken && !ProfileSystem::Instance->ActiveProfile.L3_S4_InvBambooStickTaken)
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

L3S4_Pop1BambooDoor::~L3S4_Pop1BambooDoor()
{
	SAFE_DELETE(JAnim_BambooCut);
}
