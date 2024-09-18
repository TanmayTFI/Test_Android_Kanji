//====================================
//	DQFYLH
//	HO System
//	Level 4 - Scene 04 - POPUP 1: STONE MILL
//====================================

#include "L4S4_Pop1StoneMill.h"

const CRectangle  OilMachineBaseRect(549.0f,219.0f,173.0f,139.0f);
const CRectangle  InvDestOliveRect(543.0f,179.0f,170.0f,130.0f);

const CRectangle  InvDestHandleRect(450.0f,155.0f,130.0f,200.0f);
const CRectangle  InvDestPotRect(600.0f,380.0f,345.0f,200.0f);

const CRectangle  OilMakingChartRect(362.0f,405.0f,176.0f,137.0f);

const CRectangle  PopAreaRect(270.0f,74.0f,825.0f,539.0f);
const CRectangle  GrinderRect1(465.0f,174.0f,180.0f,207.0f);
const CRectangle  GrinderRect2(628.0f,212.0f,102.0f,162.0f);

const CRectangle  LeafMemRect(887.0f,153.0f,77.0f,73.0f);

#define MILL_ANIM_SPEED 2

L4S4_Pop1StoneMill::L4S4_Pop1StoneMill(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV4\\S4\\POP1\\P1.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	int ArrayPts[] = {358,421,381,400,472,400,507,435,537,537,458,528,369,541,355,419};
	CPolygon *CPolyObjName=  new CPolygon(8);
	CPolyObjName->Init(ArrayPts);
	popChart = new L4S4_PopInstruction(3, Hud, NULL, zOrder + 200, CPolyObjName->center,CC_CALLBACK_1(L4S4_Pop1StoneMill::OnPopupClosed,this));
	popChart->SetPopupBoundingBox(PopAreaRect);
	SAFE_DELETE(CPolyObjName);

	OilMachineBase = GetObjectByID("WithoutHandle");

	if (!ProfileSystem::Instance->ActiveProfile.L4_S4_GrinderHandlePlaced)
	{
		GetObjectByID("Handle")->SetEnable(false);
	}
	GetObjectByID("Olive")->SetEnable(false);
	GetObjectByID("Oil01")->SetEnable(false);
	GetObjectByID("MillA")->SetEnable(false);
	GetObjectByID("Oil01")->SetAnimSpeed(MILL_ANIM_SPEED);
	GetObjectByID("MillA")->SetAnimSpeed(MILL_ANIM_SPEED);
	if (ProfileSystem::Instance->ActiveProfile.L4_S4_OlivePlaced && !ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachineRotated)
		GetObjectByID("Olive")->SetEnable(true);
	if (!ProfileSystem::Instance->ActiveProfile.L4_S4_PotPlaced)
		GetObjectByID("Bowl")->SetEnable(false);
	if (!ProfileSystem::Instance->ActiveProfile.L4_S4_StrainerPlaced)
		GetObjectByID("Net")->SetEnable(false);
	if (ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachineRotated)
	{
		GetObjectByID("Olive")->SetEnable(false);
		GetObjectByID("Bowl")->SetEnable(false);
		GetObjectByID("Net")->SetEnable(false);
	}
	GetObjectByID("OliveOil")->SetEnable(false);
	if (!ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachinePlaced)
	{
		OilMachineBase->SetEnable(false);
		GetObjectByID("Shadow")->SetEnable(false);
	}
	/*else*/
	GetObjectByID("Inglined")->SetEnable(false);

	/*if (!ProfileSystem::Instance->ActiveProfile.L4_S4_InstructionsPlaced)
	GetObjectByID("Chart")->SetEnable(false);*/
	if (ProfileSystem::Instance->ActiveProfile.L4_S4_InvTwig2Taken)
		GetObjectByID("WoodenBranch")->SetEnable(false);
	if (ProfileSystem::Instance->ActiveProfile.L4_S4_PotWithOilTaken)
	{
		GetObjectByID("Net")->SetEnable(false);
		GetObjectByID("Bowl")->SetEnable(false);
		//GetObjectByID("Handle")->SetEnable(false);
		GetObjectByID("Olive")->SetEnable(false);
		GetObjectByID("OliveOil")->SetEnable(false);
	}
	isMachineInHand = false;
	isOilAnimPlaying = false;
	pointToSnapMachine = OilMachineBase->GetPosPointer();

	int ArrayPts1[] = {638,96,644,141,631,140,629,180,660,181,701,192,724,199,761,226,811,265,820,271,833,245,778,205,728,170,680,148,661,109,658,88,636,92};
	InvItemRectWood =  new ClickablePolygon(0, 17, ArrayPts1);

	int ArrayPts2[] = {531,278,533,325,593,353,648,353,764,381,790,357,786,324,721,290,721,266,666,245,598,245,544,257,531,274};
	baseMachineRect=  new CPolygon(13);
	baseMachineRect->Init(ArrayPts2);

	SetSparkleUpdate(false);
	
	
	if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S4Colorleaf])
	{
		GetObjectByID("Rainbow")->SetEnable(false);
	}

	Show(false);

	activePopUPID = ePopInactive;
}

void L4S4_Pop1StoneMill::OnPopupClosed(Popup * popup)
{
	activePopUPID = ePopInactive;
}

void L4S4_Pop1StoneMill::Update(float ds)
{
	Popup::Update(ds);

	if(GetPopupState()!= ePopupState_Open  || activePopUPID != ePopInactive)
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
	{
		Cursor::Show(true);
		Cursor::SetMode(CA_Normal);
	}
	
	#ifdef _CEBUILD
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S4Colorleaf] && LeafMemRect.Intersection(&pos))
	{
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("Rainbow")->FadeOut();
			Hud->ShowMemoryObjectCollected_CE(eMem_L4S4Colorleaf, &GetObjectByID("Rainbow")->Pos);
			if (ProfileSystem::Instance->ActiveProfile.L4_S4_InvTwig2Taken && ProfileSystem::Instance->ActiveProfile.L4_S4_PotWithOilTaken)
				ClosePopup(1.5f);
		}
	}
#else
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S4Colorleaf] && LeafMemRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("Rainbow")->FadeOut();
			Hud->ShowMemoryObjectCollected_SE(eMem_L4S4Colorleaf, &GetObjectByID("Rainbow")->Pos);
		}
	}
#endif 
	if (!ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachinePlaced)
	{
		/*if (isMachineInHand)
		{	
			Cursor::Show(false);

			if (OilMachineBaseRect.Intersection(OilMachineBase->GetPosPointer()) && Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX("s4p1_placeGrindingStone");
				isMachineInHand = false;
				OilMachineBase->SetZ(OilMachineBase->ZOrder - 200);
				OilMachineBase->SetPos(new CPoint(594, 265, 0));
				GetObjectByID("Shadow")->SetEnable(true);
				ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachinePlaced = true;
				Cursor::Show(true);
				return;
			}
			OilMachineBase->SetPos(&pos);
		}
		else if (OilMachineBaseRect.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
		}*/
		/*else if (GetObjectByID("Inglined")->GetWorldRect().Intersection(&pos))
		{
		Cursor::SetMode(CA_HandTake);
		if (Control::Input->LBclicked())
		{
		isMachineInHand = true;
		OilMachineBase->SetZ(OilMachineBase->ZOrder + 200);
		OilMachineBase->SetEnable(true);
		GetObjectByID("Inglined")->SetEnable(false);
		}
		}*/
	}
	else 
	{
		if (InvDestOliveRect.Intersection(&pos))
		{
			if (!ProfileSystem::Instance->ActiveProfile.L4_S4_OlivePlaced)
			{
				Cursor::SetMode(CA_Gear);
			} 
		}
		else if (InvDestHandleRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachineRotated)
		{
			if (ProfileSystem::Instance->ActiveProfile.L4_S4_GrinderHandlePlaced)
			{
				Cursor::SetMode(CA_HandTake);
				if (Control::Input->LBclicked())
				{
					if (ProfileSystem::Instance->ActiveProfile.L4_S4_PotPlaced)
					{
						if (ProfileSystem::Instance->ActiveProfile.L4_S4_StrainerPlaced)
						{
							if (/*ProfileSystem::Instance->ActiveProfile.L4_S4_InstructionsPlaced*/ProfileSystem::Instance->ActiveProfile.L4_S4_OlivePlaced)
							{
								Control::Audio->QuickLoadAndPlaySFX("s4p1_grindingOlives");
								GetObjectByID("Oil01")->SetEnable(true);
								GetObjectByID("Oil01")->PlayAnimation(1);
								isOilAnimPlaying = true;
								GetObjectByID("MillA")->SetEnable(true);
								GetObjectByID("MillA")->PlayAnimation(1);
								OilMachineBase->SetEnable(false);
								GetObjectByID("Handle")->SetEnable(false);
								GetObjectByID("Olive")->SetEnable(false);
								GetObjectByID("OliveOil")->SetEnable(false);
								ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachineRotated = true;
							}
						}
					}
				}
			}
		}
	}

	if (InvDestPotRect.Intersection(&pos))
	{
		if (!ProfileSystem::Instance->ActiveProfile.L4_S4_PotPlaced)
		{
			Cursor::SetMode(CA_Gear);
		}
		else if (!ProfileSystem::Instance->ActiveProfile.L4_S4_StrainerPlaced)
		{
			Cursor::SetMode(CA_Gear);
		}
	}

	else if (InvItemRectWood->Intersection(&pos))
	{
		if (!ProfileSystem::Instance->ActiveProfile.L4_S4_InvTwig2Taken && !isMachineInHand)
		{
			Cursor::SetMode(CA_HandTake);
			if (Control::Input->LBclicked())
			{
				ProfileSystem::Instance->ActiveProfile.L4_S4_InvTwig2Taken = true;
				Hud->TakeInventoryItem(eInv_L4_S4_Twig2);
				ProfileSystem::Instance->ActiveProfile.L4_S4_InvTwig2Taken = true;
				GetObjectByID("WoodenBranch")->SetEnable(false);

				if (ProfileSystem::Instance->ActiveProfile.L4_S4_PotWithOilTaken )
				{
#ifdef _CEBUILD
					if(ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S4Colorleaf])
						ClosePopup();
#else
					ClosePopup();
#endif
				}
			}
		}
	}
	else if (OilMakingChartRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachineRotated)
	{
		/*if (!ProfileSystem::Instance->ActiveProfile.L4_S4_InstructionsPlaced)
		{
		Cursor::SetMode(CA_Gear);
		}
		else*/
		{
			Cursor::SetMode(CA_Look);

			if (Control::Input->LBclicked())
			{
				popChart->OpenPopup();
				activePopUPID = 1;
				
				//Objective Add
				if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L4O4_FindWayToMakeOil] )
				{
					ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L4O4_FindWayToMakeOil] = true;
					ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
					= eObj_L4O4_FindWayToMakeOil;
					//Increment Objectives
					ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
					Hud->ShowObjectiveFX(eObj_L4O4_FindWayToMakeOil);

					Control::Audio->PlaySample(aSFXMagic1);
				}

			}
		}
	}
	else if (baseMachineRect->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachinePlaced)
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L4grinder");
		}
	}
	else if((GrinderRect1.Intersection(&pos) || GrinderRect2.Intersection(&pos)) && ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachinePlaced)
	{
		if (!ProfileSystem::Instance->ActiveProfile.L4_S4_OlivePlaced && ProfileSystem::Instance->ActiveProfile.L4_S4_GrinderHandlePlaced /*&& ProfileSystem::Instance->ActiveProfile.L4_S4_InstructionsPlaced*/ && ProfileSystem::Instance->ActiveProfile.L4_S4_StrainerPlaced && ProfileSystem::Instance->ActiveProfile.L4_S4_PotPlaced)
		{
			Cursor::SetMode(CA_Gear);
			if (Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L4oil");
			}
		}
		else if (!ProfileSystem::Instance->ActiveProfile.L4_S4_GrinderHandlePlaced)
		{
			Cursor::SetMode(CA_Gear);
			if (Control::Input->LBclicked())
			{

				Hud->ShowBannerText("L1miss");
			}
		}
		/*else if (!ProfileSystem::Instance->ActiveProfile.L4_S4_InstructionsPlaced)
		{
			Cursor::SetMode(CA_Gear);
			if (Control::Input->LBclicked())
			{

				Hud->ShowBannerText("I can prepare oil here. If only I had some guidance."); //Sac: where is the external text for this ?
			}
		}*/
		else if (!ProfileSystem::Instance->ActiveProfile.L4_S4_StrainerPlaced)
		{
			Cursor::SetMode(CA_Gear);
			if (Control::Input->LBclicked())
			{

				Hud->ShowBannerText("L4pulp");
			}
		}
		else if (!ProfileSystem::Instance->ActiveProfile.L4_S4_PotPlaced)
		{
			Cursor::SetMode(CA_Gear);
			if (Control::Input->LBclicked())
			{

				Hud->ShowBannerText("L4collect");
			}
		}
	}


	if (GetObjectByID("MillA")->IsAnimationFinished() && isOilAnimPlaying)
	{
		isOilAnimPlaying = false;
		GetObjectByID("Oil01")->SetEnable(false);
		ProfileSystem::Instance->ActiveProfile.L4_S4_PotWithOilTaken = true;
		Hud->TakeInventoryItem(eInv_L4_S3_PotWithOlive);
		GetObjectByID("Net")->SetEnable(false);
		GetObjectByID("Bowl")->SetEnable(false);

		//Show Objective completed
		if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L4O4_FindWayToMakeOil] && 
			!ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L4O4_FindWayToMakeOil] )
		{
			ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L4O4_FindWayToMakeOil] = true;
			Hud->ShowObjectiveFX(eObj_L4O4_FindWayToMakeOil,true);
		}

		if (ProfileSystem::Instance->ActiveProfile.L4_S4_InvTwig2Taken )
		{
#ifdef _CEBUILD
			if(ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S4Colorleaf])
				ClosePopup();
#else
			ClosePopup();
#endif
		}
	}
}

void L4S4_Pop1StoneMill::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	switch (itemVal)
	{
	case eInv_L4_S5_Olives:
		if (!ProfileSystem::Instance->ActiveProfile.L4_S4_OlivePlaced && ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachinePlaced && InvDestOliveRect.Intersection(&dropPos))
		{
			Control::Audio->QuickLoadAndPlaySFX("s4p1_placingOlives");
			GetObjectByID("Olive")->SetEnable(true);

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.L4_S4_OlivePlaced = true;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S5_Olives] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S5_Olives);
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		break;
	case eInv_L4_S2_GrinderHandle:
		if (!ProfileSystem::Instance->ActiveProfile.L4_S4_GrinderHandlePlaced && ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachinePlaced && InvDestHandleRect.Intersection(&dropPos))
		{
			Control::Audio->QuickLoadAndPlaySFX("s4p1_placingGrinderhandle");
			GetObjectByID("Handle")->SetEnable(true);

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.L4_S4_GrinderHandlePlaced = true;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S2_GrinderHandle] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S2_GrinderHandle);
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		break;
	case eInv_L4_S3_Pot:
		if (!ProfileSystem::Instance->ActiveProfile.L4_S4_PotPlaced && InvDestPotRect.Intersection(&dropPos))
		{
			Control::Audio->QuickLoadAndPlaySFX("s4p1_placingContainer");
			GetObjectByID("Bowl")->SetEnable(true);

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.L4_S4_PotPlaced = true;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S3_Pot] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S3_Pot);
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		break;
	case eInv_L4_S2_Strainer:
		if (!ProfileSystem::Instance->ActiveProfile.L4_S4_StrainerPlaced && ProfileSystem::Instance->ActiveProfile.L4_S4_PotPlaced && InvDestPotRect.Intersection(&dropPos))
		{
			Control::Audio->QuickLoadAndPlaySFX("s4p1_placingInstructions");
			GetObjectByID("Net")->SetEnable(true);

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.L4_S4_StrainerPlaced = true;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S2_Strainer] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S2_Strainer);
		}
		else if (!ProfileSystem::Instance->ActiveProfile.L4_S4_PotPlaced && !ProfileSystem::Instance->ActiveProfile.L4_S4_StrainerPlaced)
		{
			Hud->ShowBannerText("L4place");
			Hud->CancelDrag();
		}
		else
			GFInstance->WrongInvDropMsg(Hud);
		break;		
	case eInv_L4_S5_GrinderTop:
		if (!ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachinePlaced)
		{
			OilMachineBase->SetEnable(true);
			GetObjectByID("Shadow")->SetEnable(true);

			Control::Audio->QuickLoadAndPlaySFX("s4p1_placeGrindingStone");

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachinePlaced = true;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S5_GrinderTop] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S5_GrinderTop);
		}
		else 
			GFInstance->WrongInvDropMsg(Hud);
		break;
	default:
		GFInstance->WrongInvDropMsg(Hud);
		break;
	}
}


float L4S4_Pop1StoneMill::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L4S4_Pop1StoneMill::~L4S4_Pop1StoneMill()
{
	SAFE_DELETE(popChart);
	SAFE_DELETE(InvItemRectWood);
	SAFE_DELETE(baseMachineRect);
}

void L4S4_Pop1StoneMill::Required()
{

}

void L4S4_Pop1StoneMill::ClosePopup(float delayTime)
{
	if (activePopUPID != ePopInactive || isMachineInHand)
		return;
	Popup::ClosePopup(delayTime);
}



bool L4S4_Pop1StoneMill::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if (!ProfileSystem::Instance->ActiveProfile.L4_S4_InvTwig2Taken ||
		(!ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachinePlaced && ProfileSystem::Instance->ActiveProfile.L4_S5_OilMachineBaseCollected)/*||
																	   (ProfileSystem::Instance->ActiveProfile.L4_S5_OilMachineBaseCollected && !ProfileSystem::Instance->ActiveProfile.L4_S4_InstructionsPlaced)*/ ||
																	   (ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachinePlaced && ProfileSystem::Instance->ActiveProfile.L4_S5_InvOlivesCollected && !ProfileSystem::Instance->ActiveProfile.L4_S4_OlivePlaced) ||
																	   (ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachinePlaced && ProfileSystem::Instance->ActiveProfile.L4_S2_InvGrinderHandleTaken && !ProfileSystem::Instance->ActiveProfile.L4_S4_GrinderHandlePlaced) ||
																	   (ProfileSystem::Instance->ActiveProfile.L4_S3_IsInvBowlTaken && !ProfileSystem::Instance->ActiveProfile.L4_S4_PotPlaced) || 
																	   (ProfileSystem::Instance->ActiveProfile.L4_S5_InvBambooStrainerTaken && !ProfileSystem::Instance->ActiveProfile.L4_S4_StrainerPlaced  && ProfileSystem::Instance->ActiveProfile.L4_S4_PotPlaced) ||
																	   (ProfileSystem::Instance->ActiveProfile.L4_S4_OlivePlaced && ProfileSystem::Instance->ActiveProfile.L4_S4_GrinderHandlePlaced && ProfileSystem::Instance->ActiveProfile.L4_S4_PotPlaced && ProfileSystem::Instance->ActiveProfile.L4_S4_StrainerPlaced/* && ProfileSystem::Instance->ActiveProfile.L4_S4_InstructionsPlaced*/ && !ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachineRotated) ||
																	   (ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachineRotated && !ProfileSystem::Instance->ActiveProfile.L4_S4_PotWithOilTaken))
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L4S4_Pop1StoneMill::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L4_S4_InvTwig2Taken)
	{
		Hud->InitHintTrailEmit(InvItemRectWood->center.x, InvItemRectWood->center.y);
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachinePlaced && ProfileSystem::Instance->ActiveProfile.L4_S5_OilMachineBaseCollected)
	{
		/*	Hud->InitHintTrailEmit(GetObjectByID("Inglined")->GetWorldRect().GetCenterX(), GetObjectByID("Inglined")->GetWorldRect().GetCenterY());
		Hud->ShowHintForSceneItem(new CPoint(OilMachineBaseRect.GetCenterX(),OilMachineBaseRect.GetCenterY(),0));*/
		Hud->ShowHintForInvItem(eInv_L4_S5_GrinderTop,&OilMachineBaseRect);
	}
	/*else if (ProfileSystem::Instance->ActiveProfile.L4_S5_OilMachineBaseCollected && !ProfileSystem::Instance->ActiveProfile.L4_S4_InstructionsPlaced)
	{
	Hud->ShowHintForInvItem(eInv_L4_S5_GrinderTop, &PopAreaRect);
	}*/
	else if (ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachinePlaced && ProfileSystem::Instance->ActiveProfile.L4_S5_InvOlivesCollected && !ProfileSystem::Instance->ActiveProfile.L4_S4_OlivePlaced)
	{
		Hud->ShowHintForInvItem(eInv_L4_S5_Olives, &InvDestOliveRect);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachinePlaced && ProfileSystem::Instance->ActiveProfile.L4_S2_InvGrinderHandleTaken && !ProfileSystem::Instance->ActiveProfile.L4_S4_GrinderHandlePlaced)
	{
		Hud->ShowHintForInvItem(eInv_L4_S2_GrinderHandle, &InvDestHandleRect);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L4_S3_IsInvBowlTaken && !ProfileSystem::Instance->ActiveProfile.L4_S4_PotPlaced)
	{
		Hud->ShowHintForInvItem(eInv_L4_S3_Pot, &InvDestPotRect);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L4_S5_InvBambooStrainerTaken && !ProfileSystem::Instance->ActiveProfile.L4_S4_StrainerPlaced && ProfileSystem::Instance->ActiveProfile.L4_S4_PotPlaced)
	{
		Hud->ShowHintForInvItem(eInv_L4_S2_Strainer, &InvDestPotRect);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L4_S4_OlivePlaced && ProfileSystem::Instance->ActiveProfile.L4_S4_GrinderHandlePlaced && ProfileSystem::Instance->ActiveProfile.L4_S4_PotPlaced && ProfileSystem::Instance->ActiveProfile.L4_S4_StrainerPlaced /*&& ProfileSystem::Instance->ActiveProfile.L4_S4_InstructionsPlaced*/ && !ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachineRotated)
	{
		Hud->InitHintTrailEmit(GetObjectByID("Handle")->GetWorldRect().GetCenterX(), GetObjectByID("Handle")->GetWorldRect().GetCenterY());
	}
	else if (ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachineRotated && !ProfileSystem::Instance->ActiveProfile.L4_S4_PotWithOilTaken)
	{
		Hud->InitHintTrailEmit(GetObjectByID("Bowl")->GetWorldRect().GetCenterX(), GetObjectByID("Bowl")->GetWorldRect().GetCenterY());
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
