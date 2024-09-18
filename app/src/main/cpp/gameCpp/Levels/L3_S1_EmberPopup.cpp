#include "L3_S1_EmberPopup.h"
#include "L3_S1_FortEntrance.h"

const CRectangle  PopRect(399.0f,109.0f,476.0f,461.0f);
const CRectangle  CharcolRect(399.0f,109.0f,476.0f,461.0f);
const CRectangle  StickRect(775.0f,110.0f,158.0f,117.0f);
const CRectangle  MirrorRect(878.0f,328.0f,125.0f,232.0f);

L3_S1_EmberPopup::L3_S1_EmberPopup(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback)
	: Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	_interface = NULL;
	Load("LV3\\S1\\POP\\POP1\\S1EMBERPOPUP.SCN");
	SetFrameAndCloseButton();
	SetPosOff(683.0f, 334.0f);
	SetZOff(zOrder);
	Show(false);
	SetPopupBoundingBox(GetObjectByID("PopupBg"));
	FireFogOnPopupFX = new SuperFX("PRT\\L3\\FireFogOnPopup.INI", PointSystem::CreateCPoint(635,347,0), 1050); 

	SuperFX *water =  new SuperFX("PRT\\L3\\MugWater.INI", PointSystem::CreateCPoint(635,347,0), 1050);
	fxVector.push_back(water);

	SuperFX *smoke =  new SuperFX("PRT\\L3\\S1_FlameSmoke.INI", PointSystem::CreateCPoint(635,347,0), 1050);
	fxVector.push_back(smoke);

	_janim_OffTheFlame = new JSONAnimator(0, CC_CALLBACK_1(L3_S1_EmberPopup::OnAnimatonComplete, this));
	_janim_OffTheFlame->parseAndLoadJSON("ANIM\\L3\\S1POP1_OFFTHEFLAMESANIM.JSON", this, &fxVector);

	_janim_RemoveCharcol = new JSONAnimator(1, CC_CALLBACK_1(L3_S1_EmberPopup::OnAnimatonComplete, this));
	_janim_RemoveCharcol->parseAndLoadJSON("ANIM\\L3\\S1POP1_CHARCOLREMOVEANIM.JSON",this);

	string uids[] ={"FlameFull","FlameGlowMask","LadderStick","CharcolRound_1","CharcolRound_2","CharcolRound_3",
					 "Charcol","CharcolShadow","PuzzleDoorClose","PuzzleDoorOpen","Mug","shovel","Mirror"};

	int lengthOfUids = sizeof uids / sizeof uids[0];
	
	for (int i = 0; i < lengthOfUids; i++)
	{
		L3_EmberPopupProperties p = L3_EmberPopupProperties(i);
		_popupProperties[p] =  GetObjectByID(uids[i]);
	}

	_popupProperties[L3_P_PuzzleDoorOpen]->SetZ(zOrder + eZOrder_PopCloseBtn + 1);

	_nextState = L3_Ember_NotStart;
	_state = (L3_EmberState)ProfileSystem::Instance->ActiveProfile.L3_S1_EmberPopupState;

	if (ProfileSystem::Instance->ActiveProfile.L3_S1_invGreenEyeCollected ||
		ProfileSystem::Instance->ActiveProfile.L3_S1_P1LionHeadPuzzleCompleted)
	{
		_state = L3_Ember_PuzzleDoorOpened;
		ProfileSystem::Instance->ActiveProfile.L3_S1_EmberPopupState = _state;
	}

	_stateMachine.push(L3_Ember_NotStart);
	HandleProperties();
	_sceneObjective = -1;

	L3_InvDropItem item = {false, CPoint(0, 0, 0), 0};
	_invDropItem = item;
}

L3_S1_EmberPopup::~L3_S1_EmberPopup(void)
{
	_interface = NULL;
	SAFE_DELETE(FireFogOnPopupFX);
	SAFE_DELETE(_janim_OffTheFlame);
	SAFE_DELETE(_janim_RemoveCharcol);

	for (int i = 0; i < int(fxVector.size()); i++)
	{
		SAFE_DELETE(fxVector[i]);
	}
}

void L3_S1_EmberPopup::HandleProperties()
{
	if (GetPopupState() == ePopupState_Closed)
		return;

	for (int i = 0; i < (int)(_popupProperties.size()); i++)
	{
		L3_EmberPopupProperties p = L3_EmberPopupProperties(i);
		_popupProperties[p]->SetAlpha(1);
	}

	vector<L3_EmberPopupProperties> disabledObjsIndex;

	switch (_state)
	{
	case L3_Ember_NotStart:
		{
			L3_EmberPopupProperties arrItms[] = {L3_P_CharcolPart1,L3_P_CharcolPart2,L3_P_CharcolPart3,L3_P_CharcolCollect,
													L3_P_CharcolShadow,L3_P_PuzzleDoorOpen,L3_P_Mug,L3_P_Shovel}; 
			vector<L3_EmberPopupProperties> vectorArry(arrItms, arrItms + sizeof arrItms / sizeof arrItms[0]);
			disabledObjsIndex = vectorArry;
		}
		break;

	case L3_Ember_OffTheFlames:
		{
			L3_EmberPopupProperties arrItms[] = {L3_P_Flame, L3_P_FlameGlowMask ,L3_P_PuzzleDoorOpen,L3_P_Mug,L3_P_Shovel}; 
			vector<L3_EmberPopupProperties> vectorArry(arrItms, arrItms + sizeof arrItms / sizeof arrItms[0]);
			disabledObjsIndex = vectorArry;
		}
		break;

	case L3_Ember_CollectedCharcol:
		{
			L3_EmberPopupProperties arrItms[] = {L3_P_Flame, L3_P_FlameGlowMask,L3_P_CharcolCollect,
													L3_P_CharcolShadow,L3_P_PuzzleDoorOpen,L3_P_Mug,L3_P_Shovel}; 
			vector<L3_EmberPopupProperties> vectorArry(arrItms, arrItms + sizeof arrItms / sizeof arrItms[0]);
			disabledObjsIndex = vectorArry;
		}
		break;

	case L3_Ember_RemovedCharcols:
		{
			L3_EmberPopupProperties arrItms[] = {L3_P_Flame, L3_P_FlameGlowMask,L3_P_CharcolPart1,L3_P_CharcolPart2,L3_P_CharcolPart3,L3_P_CharcolCollect,
													L3_P_CharcolShadow,L3_P_PuzzleDoorOpen,L3_P_Mug,L3_P_Shovel}; 
			vector<L3_EmberPopupProperties> vectorArry(arrItms, arrItms + sizeof arrItms / sizeof arrItms[0]);
			disabledObjsIndex = vectorArry;
		}
		break;

	case L3_Ember_PuzzleDoorOpened:
		{
			L3_EmberPopupProperties arrItms[] = {L3_P_Flame, L3_P_FlameGlowMask,L3_P_CharcolPart1,L3_P_CharcolPart2,L3_P_CharcolPart3,L3_P_CharcolCollect,
													L3_P_CharcolShadow,L3_P_PuzzleDoorClose,L3_P_Mug,L3_P_Shovel}; 
			vector<L3_EmberPopupProperties> vectorArry(arrItms, arrItms + sizeof arrItms / sizeof arrItms[0]);
			disabledObjsIndex = vectorArry;
		}
		break;

	case L3_Ember_OnAnimation:
		break;

	case L3_Ember_Closed:
		{
			L3_EmberPopupProperties arrItms[] = {L3_P_Flame,L3_P_FlameGlowMask,L3_P_LaddedStick,L3_P_CharcolPart1,L3_P_CharcolPart2,L3_P_CharcolPart3,
												L3_P_CharcolCollect,L3_P_CharcolShadow,L3_P_PuzzleDoorClose,L3_P_PuzzleDoorOpen,L3_P_Mug,L3_P_Shovel}; 
			vector<L3_EmberPopupProperties> vectorArry(arrItms, arrItms + sizeof arrItms / sizeof arrItms[0]);
			disabledObjsIndex = vectorArry;
		}
		break;

	default:
		break;
	}

	if (ProfileSystem::Instance->ActiveProfile.L3_S1_InvLadderStep1Taken)
		disabledObjsIndex.push_back(L3_P_LaddedStick);

	if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S1MemMirror])
		disabledObjsIndex.push_back(L3_P_Mirror);

	for (int i = 0; i < (int)(disabledObjsIndex.size()); i++)
	{
		_popupProperties[disabledObjsIndex[i]]->SetAlpha(0.0f);
	}
}

void L3_S1_EmberPopup::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (itemVal != eInv_L3S3_Showel && itemVal != eInv_L3S3_CupWaterFilled)
	{	
		GFInstance->WrongInvDropMsg(Hud);
		return;
	}

	if (PopRect.Intersection(&dropPos))
	{
		L3_InvDropItem item = {true, dropPos, itemVal};
		_invDropItem = item;
	}
}

void L3_S1_EmberPopup::Required()
{

}

void L3_S1_EmberPopup::Update(float ds)
{
	Popup::Update(ds);

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing)
		return;

	if(GetPopupState()!= ePopupState_Open )
		return;

	if (isMouseOverCloseButton)
		return;
	
	CPoint mPos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	if (_stateMachine.top() != _state)
	{
		_stateMachine.push(_state);

		if (_state < L3_Ember_OnAnimation)
			OnStateChange();
	}

	if (!Hud->IsDraggingItem()) 
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S1_InvLadderStep1Taken && 
			StickRect.Intersection(&mPos))
		{
			Cursor::SetMode(CA_HandTake);

			if (Control::Input->LBclicked())
			{
				ProfileSystem::Instance->ActiveProfile.L3_S1_InvLadderStep1Taken = true;
				Hud->TakeInventoryItem(eInv_L3S1_LadderPiece1, CPoint(StickRect.GetCenterX(), StickRect.GetCenterY(), 0));
				_interface->UpdateHOSystem(this, S1_Objctive_FindLadderStick);
				_popupProperties[L3_P_LaddedStick]->FadeOut(1.5f);
			}

			return;
		}

		if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S1MemMirror] && 
			MirrorRect.Intersection(&mPos))
		{
			Cursor::SetMode(CA_Normal);

			if (Control::Input->LBclicked())
			{
#ifdef _CEBUILD
				ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S1MemMirror] = true;
				_popupProperties[L3_P_Mirror]->FadeOut(1.5f);
				Hud->ShowMemoryObjectCollected_CE(eMem_L3S1MemMirror, &_popupProperties[L3_P_Mirror]->Pos);
#else
				_popupProperties[L3_P_Mirror]->FadeOut(1.5f);
				Hud->ShowMemoryObjectCollected_SE(eMem_L3S1MemMirror, &_popupProperties[L3_P_Mirror]->Pos);
#endif
			}

			return;
		}
	}

	switch (_state)
	{
	case L3_Ember_NotStart:
		{
			if(PopRect.Intersection(&mPos))
			{
				Cursor::SetMode(CA_Gear);

				if (!Hud->IsDraggingItem() && Control::Input->LBclicked())
				{
					Hud->ShowBannerText(Banner_L3_S1_ClickingOnBurningCoil);
				}
			}

			if (_invDropItem.haveItem)
			{
				if (_invDropItem.itemVal== eInv_L3S3_CupWaterFilled)
				{
					Control::Audio->QuickLoadAndPlaySFX("s1p1_waterpour");
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 
					//Clear Inventory. One Time Use Items.
					ProfileSystem::Instance->ActiveProfile.Inventory[_invDropItem.itemVal] = false;
					Hud->InvBoxCtrl->ClearItemFromInvBox(_invDropItem.itemVal);

					_janim_OffTheFlame->PlayAnim(false);
					isAnimationPlaying = true;
					_sceneObjective = S1_Objctive_OffTheFlames;
					_nextState = L3_Ember_OffTheFlames;
					_state = L3_Ember_OnAnimation;
				}
				else if(_invDropItem.itemVal== eInv_L3S3_Showel)
				{
					Hud->ShowBannerText(Banner_L3_S1_SpadeOnBurningCoil);
					Hud->CancelDrag();
				}
				else
				{
					GFInstance->WrongInvDropMsg(Hud);
				}

				L3_InvDropItem item = {false, CPoint(0, 0, 0), 0};
				_invDropItem = item;
			}
		}
		break;

	case L3_Ember_OffTheFlames:
		{
			if (PopRect.Intersection(&mPos))
			{
				Cursor::SetMode(CA_Gear);
			}

			if (_invDropItem.haveItem)
			{
				L3_InvDropItem item = {false, CPoint(0, 0, 0), 0};
				_invDropItem = item;
				GFInstance->WrongInvDropMsg(Hud);
			}
			
			if (CharcolRect.Intersection(&mPos) && !Hud->IsDraggingItem())
			{
				Cursor::SetMode(CA_HandTake);

				if (Control::Input->LBclicked())
				{
					_popupProperties[L3_P_CharcolCollect]->FadeOut(1.5f);
					_popupProperties[L3_P_CharcolShadow]->FadeOut(1.5f);
					Hud->TakeInventoryItem(eInv_L3S1_Charcoal);
					_sceneObjective = S1_Objctive_PickCharcol;
					_state = L3_Ember_CollectedCharcol;

					if (ProfileSystem::Instance->ActiveProfile.L3_S4_IsPop2ABlankSheetPlaced)
					{
						ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S4_FortFountain] = true;
					}
				}
			}
		}
		break;

	case L3_Ember_CollectedCharcol:
		{
			if (PopRect.Intersection(&mPos))
			{
				Cursor::SetMode(CA_Gear);
				if (!Hud->IsDraggingItem() && Control::Input->LBclicked())
				{
					Hud->ShowBannerText(Banner_L3_S1_AddingWaterOnBurningCoil);
				}
			}

			if (_invDropItem.haveItem)
			{
				if (_invDropItem.itemVal == eInv_L3S3_Showel)
				{
					Control::Audio->QuickLoadAndPlaySFX("s1p1_shoveluse");

					Hud->ReleaseDrag();
					Hud->CloseInventory(); 

					_popupProperties[L3_P_PuzzleDoorClose]->SetAlpha(1.0f);
					_janim_RemoveCharcol->PlayAnim(false);
					isAnimationPlaying = true;

					if (ProfileSystem::Instance->ActiveProfile.L3_S2_R3StonesRemoved)
					{
						ProfileSystem::Instance->ActiveProfile.Inventory[_invDropItem.itemVal] = false;
						Hud->InvBoxCtrl->ClearItemFromInvBox(_invDropItem.itemVal);
					}

					_sceneObjective = S1_Objctive_RemoveCharcols;
					_nextState = L3_Ember_RemovedCharcols;
					_state = L3_Ember_OnAnimation;
				}
				else
				{
					GFInstance->WrongInvDropMsg(Hud);
				}

				L3_InvDropItem item = {false, CPoint(0, 0, 0), 0};
				_invDropItem = item;
			}
		}
		break;

	case L3_Ember_RemovedCharcols:
		{
			if (PopRect.Intersection(&mPos))
			{
				Cursor::SetMode(CA_HandPointy);

				if (Control::Input->LBclicked())
				{
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S1_PZLionHead;
					_interface->NewArea();
				}
			}
		}
		break;

	case L3_Ember_PuzzleDoorOpened:
		break;

	case L3_Ember_OnAnimation:
		break;

	case L3_Ember_OnAnimationEnd:
		{
			_state = _nextState;
		}
		break;

	case L3_Ember_Closed:
		break;

	default:
		break;
	}
}

void L3_S1_EmberPopup::OpenPopup()
{
	_state = (L3_EmberState)ProfileSystem::Instance->ActiveProfile.L3_S1_EmberPopupState;
	_stateMachine.push(_state);

	if (_state == L3_Ember_NotStart)
	{
		_popupProperties[L3_P_FlameGlowMask]->SetBlendMode(eBlend_AlphaAdditive);
		_popupProperties[L3_P_FlameGlowMask]->Animate(0.3f, 0.7f, 2);
	}

	Popup::OpenPopup();
	HandleProperties();
}

void L3_S1_EmberPopup::OnPopupOpened()
{
	if (_state == L3_Ember_NotStart)
	{
		FireFogOnPopupFX->Start();
		FireFogOnPopupFX->AdvanceByTime(2.0f);
	}

	Popup::OnPopupOpened();
}

void L3_S1_EmberPopup::OnPopupCloseBtnClicked()
{
	Popup::OnPopupCloseBtnClicked();
	FireFogOnPopupFX->StopImmediate();
}

void L3_S1_EmberPopup::OnScaleDownComplete()
{
	Popup::OnScaleDownComplete();
}

float L3_S1_EmberPopup::easing(float t,float b,float c,float d)
{
	t /= d/2;
	if (t < 1) return c/2*t*t + b;
	t--;
	return -c/2 * (t*(t-2) - 1) + b;
}

void L3_S1_EmberPopup::OnStateChange()
{
	ProfileSystem::Instance->ActiveProfile.L3_S1_EmberPopupState = _state;

	if (_sceneObjective != -1)
	{
		_interface->UpdateHOSystem(this, _sceneObjective);
		_sceneObjective = -1;
	}
}

void L3_S1_EmberPopup::OnAnimatonComplete(int animID)
{
	isAnimationPlaying = false;

	if (animID == 0)
	{
		Hud->ShowBannerText(Banner_L3_S1_ClickOnCooledCoil);
		_popupProperties[L3_P_FlameGlowMask]->SetEnable(false);
		FireFogOnPopupFX->StopEmit();
	}
	else if (animID == 1)
	{
		_popupProperties[L3_P_Shovel]->FadeOut(2.0f);
	}

	_state = L3_Ember_OnAnimationEnd;
}

void L3_S1_EmberPopup::ProcessHint()
{
	switch (_state)
	{
	case L3_Ember_OnAnimation:
	case L3_Ember_OnAnimationEnd:
		return;

	case L3_Ember_NotStart:
		if (ProfileSystem::Instance->ActiveProfile.L3_S3_InvCupFilledTaken)
		{
			Hud->ShowHintForInvItem(eInv_L3S3_CupWaterFilled, &_popupBoundingBox);
			return;
		}
		break;

	case L3_Ember_CollectedCharcol:
		if (ProfileSystem::Instance->ActiveProfile.L3_S3_InvShowelTaken)
		{
			Hud->ShowHintForInvItem(eInv_L3S3_Showel, &_popupBoundingBox);
			return;
		}
	
		break;

	case L3_Ember_OffTheFlames:
		Hud->InitHintTrailEmit(&CharcolRect);
		return;
	
	case L3_Ember_RemovedCharcols:
		Hud->InitHintTrailEmit(&_popupBoundingBox);
		return;
	
	case L3_Ember_PuzzleDoorOpened:
		break;

	case L3_Ember_Closed:
		break;
	
	default:
		break;
	}

	if (!ProfileSystem::Instance->ActiveProfile.L3_S1_InvLadderStep1Taken)
	{
		Hud->InitHintTrailEmit(&StickRect);
		return;
	}

	/*if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S1MemMirror])
	{
		Hud->InitHintTrailEmit(&MirrorRect);
		return;
	}*/

	Popup::ProcessHint();
}

bool L3_S1_EmberPopup::isTaskLeft()
{
	bool haveTask = false;

	if (!ProfileSystem::Instance->ActiveProfile.L3_S1_InvLadderStep1Taken)
		return true;

	/*if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S1MemMirror])
		haveTask = true;*/

	if (_state != L3_Ember_PuzzleDoorOpened)
	{
		haveTask = true;

		if ((_state == L3_Ember_NotStart && !ProfileSystem::Instance->ActiveProfile.L3_S3_InvCupFilledTaken) ||
			(_state == L3_Ember_CollectedCharcol && !ProfileSystem::Instance->ActiveProfile.L3_S3_InvShowelTaken))
			return false;
	}

	if (!ProfileSystem::Instance->ActiveProfile.L3_S1_invGreenEyeCollected)
		haveTask = true;

	return haveTask;
}



