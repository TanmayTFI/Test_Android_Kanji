//====================================
//	DQFYLH
//	HO System
//	Level 6 - Scene 01 - POPUP 2: STATUE_CHEST
//====================================

#include "L6S1_Pop2StatueChest.h"
#include "Util.h"
#include "AwardManager.h"

const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);

const int arayOfOrderedSymbols[4] = {0, 6, 2, 4}; 

L6S1_Pop2StatueChest::L6S1_Pop2StatueChest(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	_currentSymbolIndex = 1;
	_numOfRightSymbolsVisited = 0;
	_isMouseInteractionEnabled = false;
	//Popup load
	Load("LV6\\S1\\P2\\P2.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	_starKey = GetObjectByID("StarKey");
	_starKeyHolder = GetObjectByID("KeyHole");
	_needle = GetObjectByID("Needle");
	_leftArrowBtn = new	UIButton(GetObjectByID("LeftArrowBtn"), CC_CALLBACK_1(L6S1_Pop2StatueChest::OnArrowBtnClick, this));
	_rightArrowBtn = new UIButton(GetObjectByID("RightArrowBtn"), CC_CALLBACK_1(L6S1_Pop2StatueChest::OnArrowBtnClick, this));
	PushObject(_leftArrowBtn);
	PushObject(_rightArrowBtn);

	for (int i = 0; i < 8; i++)
	{
		L6S1ChestSymbol * symbol = new L6S1ChestSymbol();
		std::string symbolId = std::string("Symbol_") + MKSTR(i);
		symbol->normalSymbol = GetObjectByID(symbolId);


		std::string selectedSymbolId = std::string("SymbolSelected_") + MKSTR(i);
		symbol->selectedSymbol = GetObjectByID(selectedSymbolId);
		symbol->selectedSymbol->SetAlpha(0);
		symbol->selectedSymbol->SetXY(symbol->normalSymbol->Pos.x, symbol->normalSymbol->Pos.y);

		std::string greenGlowId = std::string("SymbolRightSelection_") + MKSTR(i);
		symbol->rightSelectionGlow = GetObjectByID(greenGlowId);
		symbol->rightSelectionGlow->SetAlpha(0);
		symbol->rightSelectionGlow->SetXY(symbol->normalSymbol->Pos.x, symbol->normalSymbol->Pos.y);

		std::string redGlowId = std::string("SymbolWrongSelection_") + MKSTR(i);
		symbol->wrongSelectionGlow = GetObjectByID(redGlowId);
		symbol->wrongSelectionGlow->SetAlpha(0);
		symbol->wrongSelectionGlow->SetXY(symbol->normalSymbol->Pos.x, symbol->normalSymbol->Pos.y);

		symbol->SetSymbolState(eL6S1ChestSymbolState_Normal);

		_arrayOfSymbols.push_back(symbol);
	}

	_jsonAnimKeyInsertion = new JSONAnimator(1,  CC_CALLBACK_1(L6S1_Pop2StatueChest::OnKeyInsertAnimComplete, this));
	_jsonAnimKeyInsertion->parseAndLoadJSON("ANIM\\L6\\S6_P2_KEYINSERTANIMATION.JSON", this);


	_jsonAnimKeyStuck = new JSONAnimator(2);
	_jsonAnimKeyStuck->parseAndLoadJSON("ANIM\\L6\\S6_P2_KEYSTUCKANIMATION.JSON", this);

	if (ProfileSystem::Instance->ActiveProfile.L6_S1_StarKeyPlaced)
	{
		_jsonAnimKeyInsertion->SetAnimTime(1.50f);
		_isMouseInteractionEnabled = true;
		_starKey->SetEnable(true);

	}
	else
	{
		_jsonAnimKeyInsertion->SetAnimTime(0.0f);
		_isMouseInteractionEnabled = false;
		_starKey->SetEnable(false);
	}

	SetSparkleUpdate(false);
	Show(false);
}


void L6S1_Pop2StatueChest::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if (GetObjectByID("StarKey")->GetWorldRect().Intersection(&pos))
	{



		if (!ProfileSystem::Instance->ActiveProfile.L6_S1_StarKeyPlaced)
		{
			Cursor::SetMode(CA_Gear);
			if (Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L6star"); 
			}
		}
		else
		{
			if (!ProfileSystem::Instance->ActiveProfile.L6_S1_SolutionFound)
			{
				Cursor::SetMode(CA_HandPointy);

				if (Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L6need");
					_jsonAnimKeyStuck->PlayAnim();
				}
			}


		}
	}
	if (_leftArrowBtn->GetCurrentMouseState() == eMouseState_Over || _leftArrowBtn->GetCurrentMouseState() == eMouseState_Down)
	{
		Cursor::SetMode(CA_HandPointy);
	}

	if (_rightArrowBtn->GetCurrentMouseState() == eMouseState_Over || _rightArrowBtn->GetCurrentMouseState() == eMouseState_Down)
	{
		Cursor::SetMode(CA_HandPointy);
	}

}

void L6S1_Pop2StatueChest::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	switch (itemVal)
	{
	case eInv_L6_S4_StarShapedKey:
		{
			Control::Audio->QuickLoadAndPlaySFX("s1p2_keyFix");
			ProfileSystem::Instance->ActiveProfile.L6_S1_StarKeyPlaced = true;

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S4_StarShapedKey] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S4_StarShapedKey);
			_jsonAnimKeyInsertion->SetEnableObjects(true);
			_jsonAnimKeyInsertion->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s1p2_keyFix");

			if (ProfileSystem::Instance->ActiveProfile.L6_S1_SolutionFound)
			{
				Hud->InvBox_StartPuzzleMode();
				_isMouseInteractionEnabled = true;

			}
			//Hud->InvBox_StartPuzzleMode();
		}
		break;
	default:
		GFInstance->WrongInvDropMsg(Hud);
		break;
	}
}

void L6S1_Pop2StatueChest::OnAnimComplete(int animID)
{

}


float L6S1_Pop2StatueChest::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L6S1_Pop2StatueChest::~L6S1_Pop2StatueChest()
{
	for (int i = 0; i < (int)(_arrayOfSymbols.size()); i++)
	{
		SAFE_DELETE(_arrayOfSymbols.at(i));
	}
	_arrayOfSymbols.clear();
	SAFE_DELETE(_jsonAnimKeyInsertion);
	SAFE_DELETE(_jsonAnimKeyStuck);
}

void L6S1_Pop2StatueChest::Required()
{

}


void L6S1_Pop2StatueChest::ClosePopup(float delayTime)
{
	Popup::ClosePopup(delayTime);
}

bool L6S1_Pop2StatueChest::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if ((ProfileSystem::Instance->ActiveProfile.L6_S4_InvStarShapedKeyTaken && (!ProfileSystem::Instance->ActiveProfile.L6_S1_StarKeyPlaced)) ||
		ProfileSystem::Instance->ActiveProfile.L6_S1_SolutionFound && (!ProfileSystem::Instance->ActiveProfile.L6_S1_ChestPuzzleSolved) && ProfileSystem::Instance->ActiveProfile.L6_S1_StarKeyPlaced)
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

void L6S1_Pop2StatueChest::OnPopupOpened()
{
	if (ProfileSystem::Instance->ActiveProfile.L6_S1_StarKeyPlaced)
	{
		_jsonAnimKeyInsertion->SetAnimTime(1.50f);
		if(ProfileSystem::Instance->ActiveProfile.L6_S1_SolutionFound)
		{
			_isMouseInteractionEnabled = true;
			Hud->InvBox_StartPuzzleMode();
		}
		else
		{
			_isMouseInteractionEnabled = false;
		}

	}
	else
	{
		_jsonAnimKeyInsertion->SetAnimTime(0.0f);
		_isMouseInteractionEnabled = false;
		Hud->InvBox_StopPuzzleMode();
	}
}

void L6S1_Pop2StatueChest::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if (ProfileSystem::Instance->ActiveProfile.L6_S4_InvStarShapedKeyTaken && !ProfileSystem::Instance->ActiveProfile.L6_S1_StarKeyPlaced)
	{
		Hud->ShowHintForInvItem(eInv_L6_S4_StarShapedKey,&PopAreaRect);
	}
	else
	{
		Popup::ProcessHint();
	}
}

void L6S1_Pop2StatueChest::ProcessSkip()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	for (int i = 0; i < 4; i++)
	{
		_arrayOfSymbols.at(arayOfOrderedSymbols[i])->SetSymbolState(eL6S1ChestSymbolState_RightSelection);
	}
	
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);

	OnPuzzleComplete();
}

void L6S1_Pop2StatueChest::OnKeyInsertAnimComplete(int animId)
{

}

void L6S1_Pop2StatueChest::OnNeedleTweenComplete(CObject * obj)
{
	_leftArrowBtn->SetIsInteractionEnabled(true);
	_rightArrowBtn->SetIsInteractionEnabled(true);

	L6S1ChestSymbolState state = GetChestSymbolStateFor(_currentSymbolIndex);
	if (state == eL6S1ChestSymbolState_WrongSelection)
	{
		Reset();
	}
	else if(state == eL6S1ChestSymbolState_RightSelection)
	{
		if (IsPuzzleCompleted())
		{
			OnPuzzleComplete();
		}
	}
}



void L6S1_Pop2StatueChest::OnArrowBtnClick(CObject * obj)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	if (!ProfileSystem::Instance->ActiveProfile.L6_S1_StarKeyPlaced)
	{
		Hud->ShowBannerText("L6star");
		return;
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L6_S1_SolutionFound)
	{
		Hud->ShowBannerText("L6need");
		return;
	}
	L6S1ChestSymbol * currentSymbol = _arrayOfSymbols.at(_currentSymbolIndex);
	L6S1ChestSymbol * nextSymbol = NULL;
	float rotateTo = 0;

	if (obj == _leftArrowBtn)
	{
		rotateTo = _needle->GetRotate() + (360.0f / 8);
		_currentSymbolIndex += 1;
		if (_currentSymbolIndex == 8)
		{
			_currentSymbolIndex = 0;
		}

		Control::Audio->QuickLoadAndPlaySFX("s1pz1_buttonClick");

	}

	else
	{
		rotateTo = _needle->GetRotate() - (360.0f / 8);
		_currentSymbolIndex -= 1;
		if (_currentSymbolIndex == -1)
		{
			_currentSymbolIndex = 7;
		}

		Control::Audio->QuickLoadAndPlaySFX("s1pz1_buttonClick");

	}

	nextSymbol = _arrayOfSymbols.at(_currentSymbolIndex);

	if (currentSymbol->GetSymbolState() != eL6S1ChestSymbolState_RightSelection)
	{
		currentSymbol->SetSymbolState(eL6S1ChestSymbolState_Normal);
	}


	L6S1ChestSymbolState state = GetChestSymbolStateFor(_currentSymbolIndex);
	switch (state)
	{
	case eL6S1ChestSymbolState_Normal:
		break;
	case eL6S1ChestSymbolState_Selected:
		{
			nextSymbol->SetSymbolState(state);
		}
		break;
	case eL6S1ChestSymbolState_RightSelection:
		{
			nextSymbol->SetSymbolState(state);
			Control::Audio->QuickLoadAndPlaySFX("s1pz1_correctTraversal");
		}
		break;
	case eL6S1ChestSymbolState_WrongSelection:
		{
			nextSymbol->SetSymbolState(state);
			Control::Audio->QuickLoadAndPlaySFX("s1pz1_incorrectTraversal");
		}
		break;
	default:
		break;
	}




	float tweenVals[5] = {-1, -1, -1, -1, rotateTo};
	_needle->TweenTo(tweenVals, 0.5, eEase_BACKOUT, true, CC_CALLBACK_1(L6S1_Pop2StatueChest::OnNeedleTweenComplete, this));
	_leftArrowBtn->SetIsInteractionEnabled(false);
	_rightArrowBtn->SetIsInteractionEnabled(false);
	_isMouseInteractionEnabled = false;
}

void L6S1_Pop2StatueChest::Reset()
{
	_leftArrowBtn->SetIsInteractionEnabled(false);
	_rightArrowBtn->SetIsInteractionEnabled(false);
	_numOfRightSymbolsVisited = 0;
	_currentSymbolIndex = 1;

	float tweenVals[5] = {-1, -1, -1, -1, 0};
	_needle->TweenTo(tweenVals, 0.5, eEase_BACKOUT, true, CC_CALLBACK_1(L6S1_Pop2StatueChest::OnResetComplete, this));
	for (int i = 0; i < (int)_arrayOfSymbols.size(); i++)
	{
		_arrayOfSymbols.at(i)->SetSymbolState(eL6S1ChestSymbolState_Normal);
	}
}

void L6S1_Pop2StatueChest::OnPuzzleComplete()
{
	AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	float tweenVals1[5] = {-1, -1, -1, -1, -180};
	_starKey->TweenTo(tweenVals1, 1, eEase_LINEAR, true, CC_CALLBACK_1(L6S1_Pop2StatueChest::OnKeyRotationAnimComplete, this));

	float tweenVals2[5] = {-1, -1, -1, -1, -180};
	_starKeyHolder->TweenTo(tweenVals2, 1, eEase_LINEAR, true);

	_leftArrowBtn->SetIsInteractionEnabled(false);
	_rightArrowBtn->SetIsInteractionEnabled(false);

	Control::Audio->QuickLoadAndPlaySFX("s1pz1_puzzleComplete");
}

bool L6S1_Pop2StatueChest::IsPuzzleCompleted()
{
	if (_numOfRightSymbolsVisited == 4)
	{
		return true;
	}
	return false;
}

void L6S1_Pop2StatueChest::OnResetComplete(CObject * obj)
{
	_leftArrowBtn->SetIsInteractionEnabled(true);
	_rightArrowBtn->SetIsInteractionEnabled(true);
}

void L6S1_Pop2StatueChest::OnKeyRotationAnimComplete(CObject * obj)
{
	ProfileSystem::Instance->ActiveProfile.L6_S1_PathToS3Activated = true;
	ProfileSystem::Instance->ActiveProfile.L6_S1_ChestPuzzleSolved = true;
	ClosePopup();
}

L6S1ChestSymbolState L6S1_Pop2StatueChest::GetChestSymbolStateFor(int index)
{

	L6S1ChestSymbolState state = eL6S1ChestSymbolState_Normal;
	bool isSymbolInArrayOfOrderedSymbol = false;
	for (int i = 0; i < 4; i++)
	{
		if (index == arayOfOrderedSymbols[i])
		{
			isSymbolInArrayOfOrderedSymbol = true;
			if (i < _numOfRightSymbolsVisited || arayOfOrderedSymbols[_numOfRightSymbolsVisited] == index)
			{
				state = eL6S1ChestSymbolState_RightSelection;
				if (i >= _numOfRightSymbolsVisited)
				{
					_numOfRightSymbolsVisited++;
				}

			}
			else
			{
				state = eL6S1ChestSymbolState_WrongSelection;
			}
			break;
		}
	}

	if (!isSymbolInArrayOfOrderedSymbol)
	{
		state = eL6S1ChestSymbolState_Selected;
	}
	return state;
}


#pragma region SYMBOL


L6S1ChestSymbol::L6S1ChestSymbol()
{
	_symbolState = eL6S1ChestSymbolState_Normal;
	normalSymbol = NULL;
	selectedSymbol = NULL;
	rightSelectionGlow = NULL;
	wrongSelectionGlow = NULL;

}
L6S1ChestSymbol::~L6S1ChestSymbol()
{
	normalSymbol = NULL;
	selectedSymbol = NULL;
	rightSelectionGlow = NULL;
	wrongSelectionGlow = NULL;
}

void L6S1ChestSymbol::SetSymbolState(L6S1ChestSymbolState state)
{


	switch (state)
	{
	case eL6S1ChestSymbolState_Normal:
		{

			if (selectedSymbol->pSprite->Alpha > 0.0f)
			{
				selectedSymbol->FadeOut(2);
			}
			if (wrongSelectionGlow->pSprite->Alpha > 0.0f)
			{
				wrongSelectionGlow->FadeOut(2);
			}
			if (rightSelectionGlow->pSprite->Alpha > 0.0f)
			{
				rightSelectionGlow->FadeOut(2);
			}
		}
		break;
	case eL6S1ChestSymbolState_Selected:
		{
			if (selectedSymbol->pSprite->Alpha < 1.0f)
			{
				selectedSymbol->FadeIn(2);
			}

		}
		break;
	case eL6S1ChestSymbolState_RightSelection:
		{
			if (rightSelectionGlow->pSprite->Alpha < 1.0f)
			{
				rightSelectionGlow->FadeIn(2);
			}
			if (selectedSymbol->pSprite->Alpha < 1.0f)
			{
				selectedSymbol->FadeIn(2);
			}
		}
		break;
	case eL6S1ChestSymbolState_WrongSelection:
		{
			if (selectedSymbol->pSprite->Alpha < 1.0f)
			{
				selectedSymbol->FadeIn(2);
			}
			if (wrongSelectionGlow->pSprite->Alpha < 1.0f)
			{
				wrongSelectionGlow->FadeIn(2);
			}
			if (rightSelectionGlow->pSprite->Alpha > 0.0f)
			{
				rightSelectionGlow->FadeOut(2);
			}

		}
		break;
	default:
		break;
	}

	_symbolState = state;
}

L6S1ChestSymbolState L6S1ChestSymbol::GetSymbolState()
{
	return _symbolState;
}

#pragma endregion
