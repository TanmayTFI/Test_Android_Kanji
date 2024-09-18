//====================================
//	DQFYLH
//	HO System
//	Level 8 - Scene 03 - POPUP: 
//====================================

#include "L8S3_Pop1CrystalKey.h"
#include "Util.h"

const CRectangle  LockRect(509.0f,185.0f,358.0f,91.0f);

#define	NUM_SYMBOLS_ON_STRIP	6

L8S3_Pop1CrystalKey::L8S3_Pop1CrystalKey(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV8\\S3\\P1\\PZ\\P1.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	if (!ProfileSystem::Instance->ActiveProfile.L8_S3_StripPuzzleSolved)
	{
		GetObjectByID("Solution_Final-Effect")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L8_S3_PZDoorOpened)
	{
		GetObjectByID("Door01")->SetRelativePos(PointSystem::CreateCPoint(-55,0,0));
		GetObjectByID("Door02")->SetRelativePos(PointSystem::CreateCPoint(55,0,0));
	}

	if (!ProfileSystem::Instance->ActiveProfile.L8_S3_LockKeyPlaced)
	{
		GetObjectByID("scroll")->SetEnable(false);
	}

	for (int i = 0; i < MAX_L8S3_STRIPS; i++)
	{
		string id = "Strip0" + MKSTR(i + 1) + "_Symbol01";
		GetObjectByID(id)->PlayAnimation(ProfileSystem::Instance->ActiveProfile.L8_S3_SymboOnStrip[i]);
	}

	Janim_DoorOpen = new JSONAnimator(0, CC_CALLBACK_1(L8S3_Pop1CrystalKey::OnAnimComplete,this));
	Janim_DoorOpen->parseAndLoadJSON("ANIM\\L8\\L8_S3_P1_DOOROPENANIM.JSON",this);

	SetSparkleUpdate(false);
	Show(false);
}


void L8S3_Pop1CrystalKey::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (ProfileSystem::Instance->ActiveProfile.L8_S3_LockKeyPlaced)
	{
		if (!ProfileSystem::Instance->ActiveProfile.L8_S3_StripPuzzleSolved)
		{
			for (int i = 0; i < MAX_L8S3_STRIPS; i++)
			{
				string id = "Wheel0" + MKSTR(i + 1);
				if (GetObjectByID(id)->GetWorldRect().Intersection(&pos))
				{
					Cursor::SetMode(CA_HandPointy);
					if (Control::Input->LBclicked())
					{
						if (ProfileSystem::Instance->ActiveProfile.L8_S3_SymboOnStrip[i] < NUM_SYMBOLS_ON_STRIP - 1)
						{
							ProfileSystem::Instance->ActiveProfile.L8_S3_SymboOnStrip[i]++;
						}
						else
						{
							ProfileSystem::Instance->ActiveProfile.L8_S3_SymboOnStrip[i] = 0;
						}

						id = "Strip0" + MKSTR(i + 1) + "_Symbol01";
						GetObjectByID(id)->PlayAnimation(ProfileSystem::Instance->ActiveProfile.L8_S3_SymboOnStrip[i]);
						Control::Audio->QuickLoadAndPlaySFX("s3p1_rotateKeys");

						bool isPuzzleComplete = true;
						for (int i = 0; i < MAX_L8S3_STRIPS; i++)
						{
							if (ProfileSystem::Instance->ActiveProfile.L8_S3_SymboOnStrip[i])
							{
								isPuzzleComplete = false;
								break;
							}
						}

						if (isPuzzleComplete)
						{
							Control::Audio->QuickLoadAndPlaySFX("s3p1_combinationPuzzleComplete");
							ProfileSystem::Instance->ActiveProfile.L8_S3_StripPuzzleSolved = true;
							GetObjectByID("Solution_Final-Effect")->SetEnable(true);
							Control::Audio->QuickLoadAndPlaySFX("s3p1_keyDoorOpen");
							Janim_DoorOpen->PlayAnim();
						}
					}
				}
			}
		}
		else if (ProfileSystem::Instance->ActiveProfile.L8_S3_PZDoorOpened && !ProfileSystem::Instance->ActiveProfile.L8_S3_InvCrystalKeyTaken)
		{
			if (GetObjectByID("InnerKey")->GetWorldRect().Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if (Control::Input->LBclicked())
				{
					GetObjectByID("InnerKey")->SetEnable(false);
					ProfileSystem::Instance->ActiveProfile.L8_S3_InvCrystalKeyTaken = true;
					Hud->TakeInventoryItem(eInv_L8_S3_CrystalKey);
					ClosePopup(1.5f);
				}
			}
		}
	}
	else
	{
		if (LockRect.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if (Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L8tend");
			}
		}
	}
}

void L8S3_Pop1CrystalKey::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	switch (itemVal)
	{
	case eInv_L8_S2_SymbolCombination:
		if (!ProfileSystem::Instance->ActiveProfile.L8_S3_LockKeyPlaced)
		{
			Control::Audio->QuickLoadAndPlaySFX("s3p1_placeInstructions");
			GetObjectByID("scroll")->SetEnable(true);
			ProfileSystem::Instance->ActiveProfile.L8_S3_LockKeyPlaced = true;

			Hud->ReleaseDrag();
			Hud->CloseInventory();

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S2_SymbolCombination] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_S2_SymbolCombination);
		}
		break;
	default:
		GFInstance->WrongInvDropMsg(Hud);
		break;
	}
}

void L8S3_Pop1CrystalKey::OnAnimComplete(int animID)
{
	ProfileSystem::Instance->ActiveProfile.L8_S3_PZDoorOpened = true;
}

float L8S3_Pop1CrystalKey::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L8S3_Pop1CrystalKey::~L8S3_Pop1CrystalKey()
{
	SAFE_DELETE(Janim_DoorOpen);
}

void L8S3_Pop1CrystalKey::Required()
{

}

void L8S3_Pop1CrystalKey::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}

bool L8S3_Pop1CrystalKey::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if ((!ProfileSystem::Instance->ActiveProfile.L8_S3_LockKeyPlaced && ProfileSystem::Instance->ActiveProfile.L8_S2_InvScrollTaken) ||
		(ProfileSystem::Instance->ActiveProfile.L8_S3_LockKeyPlaced && !ProfileSystem::Instance->ActiveProfile.L8_S3_StripPuzzleSolved) ||
		(ProfileSystem::Instance->ActiveProfile.L8_S3_StripPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L8_S3_InvCrystalKeyTaken))
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L8S3_Pop1CrystalKey::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L8_S3_LockKeyPlaced && ProfileSystem::Instance->ActiveProfile.L8_S2_InvScrollTaken)
	{
		//Hud->ShowHintForInvItem(eInv_L8_S2_SymbolCombination,&GetObjectByID("scroll")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle ObjRect =GetObjectByID("scroll")->GetWorldRect();
		Hud->ShowHintForInvItem(eInv_L8_S2_SymbolCombination, &ObjRect);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L8_S3_LockKeyPlaced && !ProfileSystem::Instance->ActiveProfile.L8_S3_StripPuzzleSolved)
	{
		//Hud->InitHintTrailEmit(&GetObjectByID("scroll")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle ObjRect =GetObjectByID("scroll")->GetWorldRect();
		Hud->InitHintTrailEmit(&ObjRect);
		Hud->ShowHintForSceneItem(PointSystem::CreateCPoint(LockRect.GetCenterX(),LockRect.GetCenterY(),0));
	}
	else if (ProfileSystem::Instance->ActiveProfile.L8_S3_StripPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L8_S3_InvCrystalKeyTaken)
	{
		//Hud->InitHintTrailEmit(&GetObjectByID("InnerKey")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle ObjRect =GetObjectByID("InnerKey")->GetWorldRect();
		Hud->InitHintTrailEmit(&ObjRect);
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
