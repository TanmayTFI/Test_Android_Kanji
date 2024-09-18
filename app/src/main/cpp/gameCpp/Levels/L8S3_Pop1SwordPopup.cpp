//====================================
//	DQFYLH
//	HO System
//	Level 8 - Scene 03 - POPUP 1: 
//====================================

#include "L8S3_Pop1SwordPopup.h"
#include "Util.h"

const CRectangle  PuzzleRect(805.0f,197.0f,90.0f,303.0f);
const CRectangle  PopAreaRect(479.0f,35.0f,413.0f,598.0f);
const CRectangle  InvDestRectSwordHandle  (796.0f,201.0f,97.0f,149.0f);
const CRectangle  SceneRectPopup(467.0f,156.0f,268.0f,341.0f);

float shufflePosX[L8S3_SWORDPIECES] = {784,464,783,785,927};
float shufflePosY[L8S3_SWORDPIECES] = {530,534,337,423,269};

L8S3_Pop1SwordPopup::L8S3_Pop1SwordPopup(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{


	//Popup load
	Load("LV8\\S3\\P1\\P1.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	GetObjectByID("greenglow")->SetEnable(false);
	GetObjectByID("effect")->SetEnable(false);

	if (!ProfileSystem::Instance->ActiveProfile.L8_S3_SwordHandlePlaced)
	{
		GetObjectByID("Sword-handle")->SetEnable(false);
	}

	if (!ProfileSystem::Instance->ActiveProfile.L8_S3_SwordPuzzleSolved)
	{
		GetObjectByID("Sword")->SetEnable(false);
	}
	else
	{
		GetObjectByID("GlassCover")->SetEnable(false);
		GetObjectByID("swordInGlassBox-")->SetEnable(false);
	}

	for (int i = 0; i < L8S3_SWORDPIECES; i++)
	{
		if (!ProfileSystem::Instance->ActiveProfile.L8_S3_SwordPiecePlaced[i])
		{

			string id = "SwordOnSlot_0" + MKSTR(i + 1);
			GetObjectByID(id)->SetEnable(false);
		}
		else
		{
			string id = "SwordPiece_0" + MKSTR(i + 1);
			GetObjectByID(id)->SetEnable(false);
		}

		if(ProfileSystem::Instance->ActiveProfile.L8_S3_SwordPuzzleSolved)
		{
			string id = "SwordOnSlot_0" + MKSTR(i + 1);
			GetObjectByID(id)->SetEnable(false);
		}
	}

	if (ProfileSystem::Instance->ActiveProfile.L8_S3_InvSwordTaken)
	{
		GetObjectByID("inner-sword")->SetEnable(false);
	}

	Janim_sword = new JSONAnimator(0, CC_CALLBACK_1(L8S3_Pop1SwordPopup::OnAnimComplete,this));
	Janim_sword->parseAndLoadJSON("ANIM\\L8\\L8_S3_P1_SWORDANIM.JSON",this);

	int ArrayPts[] = {468,155,461,496,730,495,737,151,471,155};
	CPolygon *CPolyObjName=  new CPolygon(5);
	CPolyObjName->Init(ArrayPts);
	popPuzzle = new L8S3_Pop1CrystalKey(1, Hud, NULL, zOrder + 200, CPolyObjName->center,CC_CALLBACK_1(L8S3_Pop1SwordPopup::OnPopupClosed,this));
	delete(CPolyObjName);
	popPuzzle->SetPopupBoundingBox(PopAreaRect);

	if (!ProfileSystem::Instance->ActiveProfile.L8_S3_LockKeyPlaced)
	{
		GetObjectByID("Solution")->SetEnable(false);
	}
	int ArrayPts1[] = {709,209,679,258,666,251,647,267,650,273,656,269,666,277,595,372,536,485,539,495,550,489,636,371,701,280,696,272,724,213,710,205};
	SwordPoly =  new CPolygon(16);
	SwordPoly->Init(ArrayPts1);

	activePopupId = ePopInactive;

	isSwordPieceInHand = false;
	currentItem = -1;

	SetSparkleUpdate(false);
	Show(false);
}


void L8S3_Pop1SwordPopup::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;
	if (activePopupId != ePopInactive)
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);
	if (SceneRectPopup.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L8_S3_InvSwordTaken && !ProfileSystem::Instance->ActiveProfile.L8_S3_InvCrystalKeyTaken)
	{
		Cursor::SetMode(CA_Look);
		if (Control::Input->LBclicked())
		{
			popPuzzle->OpenPopup();
			activePopupId = 1;
		}
	}
	else if (GetObjectByID("Sword-handle")->GetWorldRect().Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L8_S3_SwordHandlePlaced && !isSwordPieceInHand)
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L8dear");
		}
	}
	else if (GetObjectByID("swordInGlassBox-")->GetWorldRect().Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L8_S3_SwordPuzzleSolved)
	{
		Cursor::SetMode(CA_Help);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L8wars");
		}
	}
	else if (/*ProfileSystem::Instance->ActiveProfile.L8_S3_SwordHandlePlaced && */!ProfileSystem::Instance->ActiveProfile.L8_S3_SwordPuzzleSolved)
	{
		if (!isSwordPieceInHand)
		{
			for (int i = 0; i < L8S3_SWORDPIECES; i++)
			{
				string id = "SwordPiece_0" + MKSTR(i + 1);
				if (GetObjectByID(id)->GetWorldRect().Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L8_S3_SwordPiecePlaced[i])
				{
					Cursor::SetMode(CA_HandPointy);
					if (Control::Input->LBclicked())
					{
						isSwordPieceInHand = true;
						currentItem = i;
					}
				}
			}
		}
		else
		{
			string id = "SwordPiece_0" + MKSTR(currentItem + 1);
			CObject *itemInHand = GetObjectByID(id);
			if (Control::Input->LBclicked())
			{
				id = "SwordOnSlot_0" + MKSTR(currentItem + 1);

				CRectangle temp = GetObjectByID(id)->GetWorldRect();
				temp.w += 10; temp.h += 10; temp.topleft = CPoint(temp.topleft.x - 5, temp.topleft.y - 5, 0);

				if (temp.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L8_S3_SwordPiecePlaced[currentItem])
				{
					Control::Audio->QuickLoadAndPlaySFX("s3p1_swordBladePlacedCorrect");
					GetObjectByID(id)->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L8_S3_SwordPiecePlaced[currentItem] = true;
					itemInHand->SetEnable(false);

					bool isSwordComplete = true;
					for (int i = 0; i < L8S3_SWORDPIECES; i++)
					{
						if (!ProfileSystem::Instance->ActiveProfile.L8_S3_SwordPiecePlaced[i])
						{
							isSwordComplete = false;
							break;
						}
					}

					if (isSwordComplete)
					{
						if (ProfileSystem::Instance->ActiveProfile.L8_S3_SwordHandlePlaced)
						{
							ProfileSystem::Instance->ActiveProfile.L8_S3_SwordPuzzleSolved = true;
							GetObjectByID("Sword")->SetEnable(true);
							Control::Audio->QuickLoadAndPlaySFX("s3p1_swordPuzzleComplete");
							Janim_sword->PlayAnim();
						}
					}
				}
				else
				{
					Control::Audio->QuickLoadAndPlaySFX("s3p1_swordBladePlacedIncorrect");
					itemInHand->SetPos(PointSystem::CreateCPoint(shufflePosX[currentItem],shufflePosY[currentItem],0));
				}

				isSwordPieceInHand = false;
				currentItem = -1;
			}
			else
			{
				itemInHand->SetPos(&pos);
			}
		}
	}
	else if (ProfileSystem::Instance->ActiveProfile.L8_S3_SwordPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L8_S3_InvSwordTaken)
	{
		if (SwordPoly->Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if (Control::Input->LBclicked())
			{
				GetObjectByID("inner-sword")->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L8_S3_InvSwordTaken = true;
				Hud->TakeInventoryItem(eInv_L8_S3_Sword);
			}
		}
	}
}

void L8S3_Pop1SwordPopup::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(activePopupId == 1)
	{
		popPuzzle->HandleInventoryDrop(dropPos, itemVal);
	}
	else
	{
		switch (itemVal)
		{
		case eInv_L8_S2_SwordHandle:
			if (InvDestRectSwordHandle.Intersection(&dropPos) && !ProfileSystem::Instance->ActiveProfile.L8_S3_SwordHandlePlaced)
			{
				Control::Audio->QuickLoadAndPlaySFX("s3p1_placeSwordHandle");
				GetObjectByID("Sword-handle")->SetEnable(true);
				ProfileSystem::Instance->ActiveProfile.L8_S3_SwordHandlePlaced = true;

				Hud->ReleaseDrag();
				Hud->CloseInventory(); 

				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S2_SwordHandle] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_S2_SwordHandle);

				bool isSwordComplete = true;
				for (int i = 0; i < L8S3_SWORDPIECES; i++)
				{
					if (!ProfileSystem::Instance->ActiveProfile.L8_S3_SwordPiecePlaced[i])
					{
						isSwordComplete = false;
						break;
					}
				}

				if (isSwordComplete)
				{
					ProfileSystem::Instance->ActiveProfile.L8_S3_SwordPuzzleSolved = true;
					GetObjectByID("Sword")->SetEnable(true);
					Control::Audio->QuickLoadAndPlaySFX("s3p1_swordPuzzleComplete");
					Janim_sword->PlayAnim();
				}
			}
			else 
				Hud->CancelDrag();
			break;
		case eInv_L8_S2_SymbolCombination:
			if (ProfileSystem::Instance->ActiveProfile.L8_S3_InvSwordTaken && !ProfileSystem::Instance->ActiveProfile.L8_S3_InvCrystalKeyTaken)
			{
				popPuzzle->OpenPopup();
				activePopupId = 1;
			}
			else
				GFInstance->WrongInvDropMsg(Hud);
			break;
		default:
			GFInstance->WrongInvDropMsg(Hud);
			break;
		}
	}

}

void L8S3_Pop1SwordPopup::OnPopupClosed(Popup * popup)
{
	activePopupId = ePopInactive;

	if (ProfileSystem::Instance->ActiveProfile.L8_S3_LockKeyPlaced)
	{
		GetObjectByID("Solution")->SetEnable(true);
	}

	if (ProfileSystem::Instance->ActiveProfile.L8_S3_InvCrystalKeyTaken)
	{
		ClosePopup();
	}
}


void L8S3_Pop1SwordPopup::OnAnimComplete(int animID)
{

}

float L8S3_Pop1SwordPopup::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L8S3_Pop1SwordPopup::~L8S3_Pop1SwordPopup()
{
	SAFE_DELETE(Janim_sword);
	SAFE_DELETE(popPuzzle);
	SAFE_DELETE(SwordPoly);
}

void L8S3_Pop1SwordPopup::Required()
{

}

void L8S3_Pop1SwordPopup::ClosePopup(float delayTime)
{
	if (activePopupId != ePopInactive)
		return;

	Popup::ClosePopup(delayTime);
}

bool L8S3_Pop1SwordPopup::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if ((!ProfileSystem::Instance->ActiveProfile.L8_S3_SwordHandlePlaced &&  ProfileSystem::Instance->ActiveProfile.L8_S2_InvSwordHandleTaken)|| 
		(ProfileSystem::Instance->ActiveProfile.L8_S3_SwordHandlePlaced && !ProfileSystem::Instance->ActiveProfile.L8_S3_SwordPuzzleSolved) ||
		(ProfileSystem::Instance->ActiveProfile.L8_S3_SwordPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L8_S3_InvSwordTaken) )
	{
		isAnyTaskLeft = true;
	}

	if (ProfileSystem::Instance->ActiveProfile.L8_S3_InvSwordTaken /*&& !ProfileSystem::Instance->ActiveProfile.L8_S3_InvCrystalKeyTaken*/)
	{
		if (popPuzzle->isTaskLeft())
		{
			isAnyTaskLeft = true;
		}
	}

	return isAnyTaskLeft;
}

void L8S3_Pop1SwordPopup::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;
	/*if (activePopupId == 1)
	{
	if (popPuzzle->isTaskLeft())
	popPuzzle->ProcessHint();
	else
	Popup::ProcessHint();
	}
	*/
	if(!ProfileSystem::Instance->ActiveProfile.L8_S3_SwordHandlePlaced &&  ProfileSystem::Instance->ActiveProfile.L8_S2_InvSwordHandleTaken)
	{
		Hud->ShowHintForInvItem(eInv_L8_S2_SwordHandle,&InvDestRectSwordHandle);
	}
	else if (/*ProfileSystem::Instance->ActiveProfile.L8_S3_SwordHandlePlaced &&*/ !ProfileSystem::Instance->ActiveProfile.L8_S3_SwordPuzzleSolved)
	{
		int itemID;
		//Sac: possible that itemID could be used above L8S3_SWORDPIECES ??
		// Then MKSTR will crash so initialising it to - anyway. 
		itemID = 0;
		for (int i = 0; i < L8S3_SWORDPIECES; i++)
		{
			if (!ProfileSystem::Instance->ActiveProfile.L8_S3_SwordPiecePlaced[i])
			{
				itemID = i;
				break;
			}
		}
		//Hud->InitHintTrailEmit(&GetObjectByID("SwordPiece_0" + MKSTR(itemID + 1))->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle ObjRect =GetObjectByID("SwordPiece_0" + MKSTR(itemID + 1))->GetWorldRect();
		if (ProfileSystem::Instance->ActiveProfile.L8_S3_SwordHandlePlaced)
		{
			Hud->InitHintTrailEmit(&ObjRect);
			Hud->ShowHintForSceneItem(PointSystem::CreateCPoint(GetObjectByID("SwordOnSlot_0" + MKSTR(itemID + 1))->GetCenterX(), GetObjectByID("SwordOnSlot_0" + MKSTR(itemID + 1))->GetCenterY(), 0));
		}
		else
		{
			Hud->InitHintTrailEmit(1005,90);
		}
	}
	else if (ProfileSystem::Instance->ActiveProfile.L8_S3_SwordPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L8_S3_InvSwordTaken)
	{
		//Hud->InitHintTrailEmit(&GetObjectByID("inner-sword")->GetWorldRect());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CRectangle ObjRect =GetObjectByID("inner-sword")->GetWorldRect();
		Hud->InitHintTrailEmit(&ObjRect);
	}
	else if (ProfileSystem::Instance->ActiveProfile.L8_S3_InvSwordTaken && !ProfileSystem::Instance->ActiveProfile.L8_S3_InvCrystalKeyTaken)
	{
		if (activePopupId == ePopInactive)
		{
			if (popPuzzle->isTaskLeft())
				Hud->InitHintTrailEmit(&SceneRectPopup);
			else
				Popup::ProcessHint();
		}
		else if(popPuzzle->isTaskLeft())
		{
			popPuzzle->ProcessHint();
		}
		else
			Popup::ProcessHint();
	}
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
