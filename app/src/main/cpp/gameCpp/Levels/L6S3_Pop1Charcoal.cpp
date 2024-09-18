//====================================
//	DQFYLH
//	HO System
//	Level 6 - Scene 03 - POPUP 1: 
//====================================

#include "L6S3_Pop1Charcoal.h"
const CRectangle  MemObjRect(453.0f,119.0f,122.0f,49.0f);

L6S3_Pop1Charcoal::L6S3_Pop1Charcoal(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{


	//Popup load
	Load("LV6\\S3\\P1\\P1.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	GetObjectByID("club")->SetEnable(false);
	GetObjectByID("CoalPanWithCoal")->SetEnable(false);
	GetObjectByID("CoalPan")->SetEnable(false);
	GetObjectByID("LargeBurnedWoodAfterbreak")->SetEnable(false);

	/*if (ProfileSystem::Instance->ActiveProfile.L6_S3_InvHorseShoeTaken)
	{
	GetObjectByID("HorseShoe")->SetEnable(false);
	}*/

	if (ProfileSystem::Instance->ActiveProfile.L6_S3_ClubsUsed)
	{
		GetObjectByID("LargeBurnedWoodForBreakTask")->SetEnable(false);
	}

	GetObjectByID("BurnedWoodCrushedPieces")->SetEnable(false);

	GetObjectByID("club")->SetZ(GetObjectByID("club")->ZOrder + eZOrder_Popup);

	Janim_charcoalHit = new JSONAnimator(0,CC_CALLBACK_1(L6S3_Pop1Charcoal::OnAnimComplete, this));
	Janim_charcoalHit->parseAndLoadJSON("ANIM\\L6\\S3_CHARCOAL_HITING.JSON", this);

	Janim_charcoalTake = new JSONAnimator(1,CC_CALLBACK_1(L6S3_Pop1Charcoal::OnAnimComplete, this));
	Janim_charcoalTake->parseAndLoadJSON("ANIM\\L6\\S3_COAL_TAKING.JSON", this);


	if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S3PaintBrush])
	{
		GetObjectByID("MemObj")->SetEnable(false);
	}

	S3SmokeFX = new SuperFX("PRT\\L6\\S3_POP_CharcoalSmoke.INI", PointSystem::CreateCPoint(700,350,0), GetObjectByID("LargeBurnedWoodForBreakTask")->ZOrder + 1);

	SetSparkleUpdate(false);
	Show(false);
}


void L6S3_Pop1Charcoal::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (!ProfileSystem::Instance->ActiveProfile.L6_S3_InvCharcoalTaken && !S3SmokeFX->Active)
	{
		S3SmokeFX->Start();
		S3SmokeFX->AdvanceByTime(2.0f);
	}

	/*if (!ProfileSystem::Instance->ActiveProfile.L6_S3_InvHorseShoeTaken && GetObjectByID("HorseShoe")->GetWorldRect().Intersection(&pos))
	{
	Cursor::SetMode(CA_HandTake);
	if (Control::Input->LBclicked())
	{
	GetObjectByID("HorseShoe")->SetEnable(false);
	ProfileSystem::Instance->ActiveProfile.L6_S3_InvHorseShoeTaken = true;
	Hud->TakeInventoryItem(eInv_L6_S3_HorseShoe);
	if (ProfileSystem::Instance->ActiveProfile.L6_S3_InvCharcoalTaken)
	{
	ClosePopup(1.5);
	}
	}
	}
	else*/ if (!ProfileSystem::Instance->ActiveProfile.L6_S3_ClubsUsed && GetObjectByID("LargeBurnedWoodForBreakTask")->GetWorldRect().Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L6tiny");
		}
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L6_S3_InvCharcoalTaken && GetObjectByID("Coal")->GetWorldRect().Intersection(&pos))
	{
		Cursor::SetMode(CA_Gear);
	}

#ifdef _CEBUILD
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S3PaintBrush] && MemObjRect.Intersection(&pos))
	{
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("MemObj")->FadeOut();
			Hud->ShowMemoryObjectCollected_CE(eMem_L6S3PaintBrush, &(GetObjectByID("MemObj")->Pos));
			if (ProfileSystem::Instance->ActiveProfile.L6_S3_InvCharcoalTaken)
			{
				ClosePopup(1);
			}
		}
	}
#else
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S3PaintBrush] && MemObjRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("MemObj")->FadeOut();
			Hud->ShowMemoryObjectCollected_SE(eMem_L6S3PaintBrush, &GetObjectByID("MemObj")->Pos);
		}
	}
#endif 
}

void L6S3_Pop1Charcoal::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	switch (itemVal)
	{
	case eInv_L6_S1_Club:
		if (!ProfileSystem::Instance->ActiveProfile.L6_S3_ClubsUsed && GetObjectByID("LargeBurnedWoodForBreakTask")->GetWorldRect().Intersection(&dropPos))
		{
			Control::Audio->QuickLoadAndPlaySFX("s3p1_crushCharcoal");
			ProfileSystem::Instance->ActiveProfile.L6_S3_ClubsUsed = true;
			Janim_charcoalHit->PlayAnim();

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			if (ProfileSystem::Instance->ActiveProfile.L6_S4_RockBrokened)
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S1_Club] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S1_Club);
			}
		}
		break;
	case eInv_L6_S1_Mortar:
		if (!ProfileSystem::Instance->ActiveProfile.L6_S3_InvCharcoalTaken /*&& GetObjectByID("Coal")->GetWorldRect().Intersection(&dropPos)*/ && ProfileSystem::Instance->ActiveProfile.L6_S3_ClubsUsed)
		{
			Control::Audio->QuickLoadAndPlaySFX("s3p1_takeCharcoal");
			Janim_charcoalTake->PlayAnim();

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S1_Mortar] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S1_Mortar);
		}
		break; // sac: this was not there, no idea why, added it 
	default:
		GFInstance->WrongInvDropMsg(Hud);
		break;
	}
}

void L6S3_Pop1Charcoal::OnAnimComplete(int animID)
{
	if (animID == 0)
	{
		GetObjectByID("club")->SetEnable(false);
	}
	else
	{
		ProfileSystem::Instance->ActiveProfile.L6_S3_InvCharcoalTaken = true;
		Hud->TakeInventoryItem(eInv_L6_S3_Charcoal);
		GetObjectByID("CoalPanWithCoal")->SetEnable(false);
		GetObjectByID("Coal")->SetEnable(false);
		GetObjectByID("CoalPan")->SetEnable(false);

		//if (ProfileSystem::Instance->ActiveProfile.L6_S3_InvHorseShoeTaken)
		{
#ifdef _CEBUILD
			if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S3PaintBrush])
			{			
				ClosePopup(1.5);
			}
#else
			ClosePopup(1.5);
#endif 
		}

	}
}

float L6S3_Pop1Charcoal::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L6S3_Pop1Charcoal::~L6S3_Pop1Charcoal()
{
	SAFE_DELETE(Janim_charcoalHit);
	SAFE_DELETE(Janim_charcoalTake);
	SAFE_DELETE(S3SmokeFX);
}

void L6S3_Pop1Charcoal::Required()
{

}

void L6S3_Pop1Charcoal::ClosePopup(float delayTime)
{
	S3SmokeFX->StopImmediate();
	Popup::ClosePopup(delayTime);
}

bool L6S3_Pop1Charcoal::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if ((ProfileSystem::Instance->ActiveProfile.L6_S1_InvClubTaken && !ProfileSystem::Instance->ActiveProfile.L6_S3_ClubsUsed) ||
		(ProfileSystem::Instance->ActiveProfile.L6_S1_InvMortarTaken && !ProfileSystem::Instance->ActiveProfile.L6_S3_InvCharcoalTaken))
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L6S3_Pop1Charcoal::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(ProfileSystem::Instance->ActiveProfile.L6_S1_InvClubTaken && !ProfileSystem::Instance->ActiveProfile.L6_S3_ClubsUsed)
	{
		//Hud->ShowHintForInvItem(eInv_L6_S1_Club,&GetObjectByID("LargeBurnedWoodForBreakTask")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle burntWoodObjRect =GetObjectByID("LargeBurnedWoodForBreakTask")->GetWorldRect();
		Hud->ShowHintForInvItem(eInv_L6_S1_Club,&burntWoodObjRect);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S1_InvMortarTaken && !ProfileSystem::Instance->ActiveProfile.L6_S3_InvCharcoalTaken)
	{
		//Hud->ShowHintForInvItem(eInv_L6_S1_Mortar,&GetObjectByID("Coal")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle coalObjRect =GetObjectByID("Coal")->GetWorldRect();
		Hud->ShowHintForInvItem(eInv_L6_S1_Mortar,&coalObjRect);
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
