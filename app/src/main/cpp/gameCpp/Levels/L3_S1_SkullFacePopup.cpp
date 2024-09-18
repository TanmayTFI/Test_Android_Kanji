#include "L3_S1_SkullFacePopup.h"
#include "L3_S1_FortEntrance.h"

const CRectangle  GemDropRect(407.0f,103.0f,556.0f,450.0f);
const CRectangle  CrystalRect(565.0f,381.0f,186.0f,111.0f);

L3_S1_SkullFacePopup::L3_S1_SkullFacePopup(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback)
	: Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	_interface = NULL;
	Load("LV3\\S1\\POP\\POP2\\S1SKULLFACEPOPUP.SCN");
	SetFrameAndCloseButton();
	SetPosOff(683.0f, 334.0f);
	SetZOff(zOrder);
	Show(false);
	SetPopupBoundingBox(GetObjectByID("PopupBg"));

	_janim_PlaceEye = new JSONAnimator(1, CC_CALLBACK_1(L3_S1_SkullFacePopup::OnAnimationComplete, this));
	_janim_PlaceEye->parseAndLoadJSON("ANIM\\L3\\S1POP2_SKULLPLACEEYE.JSON",this);

	string uids[] ={"GemDrop","MouthOpen","EyeFixed","CrystalInMouth"};
	int lengthOfUids = sizeof uids / sizeof uids[0];
	
	for (int i = 0; i < lengthOfUids; i++)
	{
		L3_SkullPopupProperties p = L3_SkullPopupProperties(i);
		_popupProperties[p] =  GetObjectByID(uids[i]);
	}

	_state = L3_Skull_Initial;
	_nextState = L3_Skull_Initial;
	_stateMachine.push(L3_Skull_Initial);
	HandleProperties();
}

L3_S1_SkullFacePopup::~L3_S1_SkullFacePopup(void)
{
	_interface = NULL;
	SAFE_DELETE(_janim_PlaceEye);
}

void L3_S1_SkullFacePopup::SavedState(bool eyePlaced)
{
	if (eyePlaced)
	{
		_state = L3_Skull_EyeFixed;
		HandleProperties();
	}
}

void L3_S1_SkullFacePopup::HandleProperties()
{
	for (int i = 0; i < (int)(_popupProperties.size()); i++)
	{
		L3_SkullPopupProperties p = L3_SkullPopupProperties(i);
		_popupProperties[p]->SetAlpha(1);
	}

	vector<L3_SkullPopupProperties> disabledObjsIndex;

	switch (_state)
	{
	case L3_Skull_Initial:
		{
			L3_SkullPopupProperties arrItms[] = {S1_P_GemDrop,S1_P_MouthOpen,S1_P_EyeFixed,S1_P_CrystalInMouth}; 
			vector<L3_SkullPopupProperties> vectorArry(arrItms, arrItms + sizeof arrItms / sizeof arrItms[0]);
			disabledObjsIndex = vectorArry;
		}
		break;

	case L3_Skull_EyeFixed:
		{
			L3_SkullPopupProperties arrItms[] = {S1_P_GemDrop}; 
			vector<L3_SkullPopupProperties> vectorArry(arrItms, arrItms + sizeof arrItms / sizeof arrItms[0]);
			disabledObjsIndex = vectorArry;
		}
		break;

	case L3_Skull_CrystalCollected:
		{
			L3_SkullPopupProperties arrItms[] = {S1_P_GemDrop,S1_P_CrystalInMouth}; 
			vector<L3_SkullPopupProperties> vectorArry(arrItms, arrItms + sizeof arrItms / sizeof arrItms[0]);
			disabledObjsIndex = vectorArry;
		}
		break;

	case L3_Skull_OnAnimate:
		break;

	case L3_Skull_AnimComplete:
		break;
	default:
		break;
	}

	for (int i = 0; i < (int)(disabledObjsIndex.size()); i++)
	{
		_popupProperties[disabledObjsIndex[i]]->SetAlpha(0);
	}
}

void L3_S1_SkullFacePopup::OpenPopup()
{
	Popup::OpenPopup();
}

void L3_S1_SkullFacePopup::OnPopupCloseBtnClicked()
{
	Popup::OnPopupCloseBtnClicked();
}

void L3_S1_SkullFacePopup::Update(float ds)
{
	Popup::Update(ds);

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;
	
	if(GetPopupState()!= ePopupState_Open )
		return;
	
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	CPoint mPos = Control::Input->Mpos();

	if (_state != _stateMachine.top())
	{
		_stateMachine.push(_state);
	}

	switch (_state)
	{
	case L3_Skull_Initial:
		{
			if (GemDropRect.Intersection(&mPos))
			{
				Cursor::SetMode(CA_Gear);

				if (Control::Input->LBclicked())
				{
					Hud->ShowBannerText(Banner_L3_S1_ClickOnSkullFace);
				}
			}
		}
		break;

	case L3_Skull_EyeFixed:
		{
			if (CrystalRect.Intersection(&mPos))
			{
				Cursor::SetMode(CA_HandTake);

				if (Control::Input->LBclicked())
				{
					_interface->UpdateHOSystem(this, S1_Objctive_CollectCrystelkFromMouth);
					Hud->TakeInventoryItem(eInv_L3S1_GreenCrystal);
					_popupProperties[S1_P_CrystalInMouth]->FadeOut(1.5f);
					ProfileSystem::Instance->ActiveProfile.L3_S1_InvGreenCrystalTaken = true;
					if (ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L3_S5_Cliff])
					{
						ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S5_Cliff] = true;
					}
					_state = L3_Skull_CrystalCollected;
				}
			}
		}
		break;

	case L3_Skull_CrystalCollected:
		{
		}
		break;

	case L3_Skull_OnAnimate:
		break;

	case L3_Skull_AnimComplete:
		_state = _nextState;
		break;

	default:
		break;
	}
}

void L3_S1_SkullFacePopup::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (_state == L3_Skull_Initial && GemDropRect.Intersection(&dropPos) && itemVal == eInv_L3S1_GreenEye)
	{
		Control::Audio->QuickLoadAndPlaySFX("s1p3_eyeplaced");

		_janim_PlaceEye->PlayAnim(false);
		isAnimationPlaying = true;
		_interface->UpdateHOSystem(this, S1_Objctive_PlaceGemEye);
		_state = L3_Skull_OnAnimate;
		_nextState = L3_Skull_EyeFixed;

		Hud->ReleaseDrag();
		Hud->CloseInventory(); 
		//Clear Inventory. One Time Use Items.
		ProfileSystem::Instance->ActiveProfile.Inventory[itemVal] = false;
		Hud->InvBoxCtrl->ClearItemFromInvBox(itemVal);
	}
	else
	{
		GFInstance->WrongInvDropMsg(Hud);
	}
}

void L3_S1_SkullFacePopup::OnAnimationComplete(int animID)
{
	isAnimationPlaying = false;
	_state = L3_Skull_AnimComplete;
}

void L3_S1_SkullFacePopup::ProcessHint()
{
	switch (_state)
	{
	case L3_Skull_OnAnimate:
	case L3_Skull_AnimComplete:
		break;

	case L3_Skull_EyeFixed:
		Hud->InitHintTrailEmit(&CrystalRect, false);
		break;

	case L3_Skull_Initial:
		if (ProfileSystem::Instance->ActiveProfile.L3_S1_invGreenEyeCollected)
		{
			Hud->ShowHintForInvItem(eInv_L3S1_GreenEye, &_popupBoundingBox);
			break;
		}

	case L3_Skull_CrystalCollected:
	default:
		Popup::ProcessHint();
		break;
	}
}

bool L3_S1_SkullFacePopup::isTaskLeft()
{
	switch (_state)
	{
	case L3_Skull_EyeFixed:
		return true;

	case L3_Skull_Initial:
		if (ProfileSystem::Instance->ActiveProfile.L3_S1_invGreenEyeCollected)
			return true;
	default: break; //Sac added this as there are more case possibilites as per SkullPopup_State enum listed
	}

	return false;
}


