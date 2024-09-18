
//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "L6S3_HOPop1Code.h"
#include "Hud.h"
#include "Util.h"

L6S3_HOPop1Code::L6S3_HOPop1Code(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	IndexCount = 0;

	for (int i = 0; i < 4; i++)
	{
		IndexArray[i] = -1; 
	}
	//Popup load
	Load("LV6\\S3\\HO_P1\\HO_P1.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	if( !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodeApplied )
	{
		GetObjectByID("Paper")->SetEnable(false);
	}
	else
	{
		GetObjectByID("Paper")->SetEnable(true);
	}
	for (int i = 0; i < 4; i++)
	{
		PuzzleSymbols[i] = GetObjectByID("Symbol" + MKSTR(i+1));	
		PuzzleSymbols[i]->SetEnable(false); 
	}
	for (int i = 0; i < 9; i++)
	{
		PuzzleButtons[i] = GetObjectByID("Button0" + MKSTR(i));
		PuzzleButtons[i]->SetEnable(false); 
	}

	PuzzleRect[0] = new CRectangle(635.0f,285.0f,50.0f,50.0f);
	PuzzleRect[1] = new CRectangle(685.0f,285.0f,50.0f,50.0f);
	PuzzleRect[2] = new CRectangle(735.0f,285.0f,50.0f,50.0f);
	PuzzleRect[3] = new CRectangle(635.0f,335.0f,50.0f,50.0f);
	PuzzleRect[4] = new CRectangle(685.0f,335.0f,50.0f,50.0f);
	PuzzleRect[5] = new CRectangle(735.0f,335.0f,50.0f,50.0f);
	PuzzleRect[6] = new CRectangle(635.0f,385.0f,50.0f,50.0f);
	PuzzleRect[7] = new CRectangle(685.0f,385.0f,50.0f,50.0f);
	PuzzleRect[8] = new CRectangle(735.0f,385.0f,50.0f,50.0f);

	S3_HO_PuzzleEffectFX = new SuperFX("PRT\\L6\\S3_HO_PuzzleEffect.INI", PointSystem::CreateCPoint(683,332,0), 1100); 

}




void L6S3_HOPop1Code::OnPopupOpened()
{
	if( !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodeApplied )
	{
		GetObjectByID("Paper")->SetEnable(false);
	}
	else
	{
		Hud->InvBox_StartPuzzleMode();
		GetObjectByID("Paper")->SetEnable(true);
	}

	
}

void L6S3_HOPop1Code::Required()
{

}

void L6S3_HOPop1Code ::ProcessSkip()
{
	int PuzzleSolution[4] = {2,7,3,8};
	for (int i = 0; i < 9; i++)
	{
		PuzzleButtons[i]->SetEnable(false);
	}
	for (int i = 0; i < 4; i++)
	{
		PuzzleButtons[PuzzleSolution[i]]->SetEnable(true);
		PuzzleSymbols[i]->SetEnable(true);
	}
	if( !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodePuzzleSolved )
	{
		ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodeTaken = true;
		ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodePuzzleSolved = true;
	}
	S3_HO_PuzzleEffectFX->Start();
	S3_HO_PuzzleEffectFX->AdvanceByTime(2.0f);
	Hud->InvBox_StopPuzzleMode();
	ClosePopup(1.5f);
}

void L6S3_HOPop1Code::OnPopupCloseBtnClicked()
{
	Hud->InvBox_StopPuzzleMode();
}

void L6S3_HOPop1Code::HandleInventoryDrop(CPoint dropPos,int itemVal )
{
	if( itemVal == eInv_L6_S3_PuzzleCode )
	{
		if( !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodeApplied )
		{
			ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodeApplied = true;
			GetObjectByID("Paper")->SetEnable(true);
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			SetSparkleUpdate(true);
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S3_PuzzleCode] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S3_PuzzleCode);
			Hud->InvBox_StartPuzzleMode();
		}
	}
}

void L6S3_HOPop1Code::Update(float ds)
{
	
	Popup::Update(ds);

	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup)
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	//handle interactions here
	if( !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodeTaken )
	{
		if( Control::Input->LBclicked() )
			Hud->ShowBannerText("L6take");
		return;
	}

	for (int i = 0; i < 9; i++)
	{
		PuzzleButtons[i]->SetEnable(false);
		if(PuzzleRect[i].Intersection(&pos) ) 
		{
			Cursor::SetMode(CA_HandPointy);
			PuzzleButtons[i]->SetEnable(true);
			
			if(Control::Input->LBdown())
				PuzzleButtons[i]->PlayAnimation(0);
			else
				PuzzleButtons[i]->PlayAnimation(1);
			if( Control::Input->LBclicked() )
			{
				Cursor::SetMode(CA_Normal);
				IndexArray[IndexCount] = i;
				if( CheckIfValid() )
				{
					Control::Audio->QuickLoadAndPlaySFX("s3hop1_correctEntry");
					PuzzleSymbols[IndexCount]->SetEnable(true);
					if( CheckIfPuzzleSolved() )
					{
						Control::Audio->QuickLoadAndPlaySFX("s3hop1_puzzleComplete");
						S3_HO_PuzzleEffectFX->Start();
						S3_HO_PuzzleEffectFX->AdvanceByTime(2.0f);
						ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodePuzzleSolved = true;
						Hud->InvBox_StopPuzzleMode();
						ClosePopup(1.5f);
					}
					IndexCount++;
				}
				else
					ResetPuzzle();
			}
		}
	}

}

bool L6S3_HOPop1Code::CheckIfPuzzleSolved()
{
	if( IndexCount >= 3 )
		return (true);

	return(false);
}

bool L6S3_HOPop1Code::CheckIfValid()
{
	int PuzzleSolution[4] = {2,7,3,8};
	bool flag = true;
	for (int i = 0; i <= IndexCount; i++)
	{
		if( IndexArray[i] != PuzzleSolution[i] ) 
		{
			flag = false;
		}
	}
	return(flag);
}

void L6S3_HOPop1Code::ResetPuzzle()
{
	Control::Audio->QuickLoadAndPlaySFX("s3hop1_incorrectEntry");
	IndexCount = 0;
	for (int i = 0; i < 4; i++)
	{
		PuzzleSymbols[i]->SetEnable(false); 
	}
	for (int i = 0; i < 9; i++)
	{
		PuzzleButtons[i]->SetEnable(false); 
	}
}


//void L6S3_HOPop1Code::HandleInventoryDrop(CPoint dropPos, int itemVal)
//{
//	GFInstance->WrongInvDropMsg(Hud);
//}

void L6S3_HOPop1Code::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here
	if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodeTaken && !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodeApplied)
	{
		Hud->ShowHintForInvItem(  eInv_L6_S3_PuzzleCode, &PuzzleRect[4]);
		return;
	}

	//else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L6S3_HOPop1Code::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if(ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodeTaken && !ProfileSystem::Instance->ActiveProfile.L6_S3_HO_CodeApplied)
		isAnyTaskLeft = true;
	
	return isAnyTaskLeft;
}

L6S3_HOPop1Code::~L6S3_HOPop1Code()
{
	Hud->InvBoxCtrl->ResetToInvBox();
	SAFE_DELETE(S3_HO_PuzzleEffectFX);
}
