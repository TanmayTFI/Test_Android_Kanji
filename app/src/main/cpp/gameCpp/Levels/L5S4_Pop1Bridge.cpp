//====================================
//	DQFYLH
//	HO System
//	Level 3 - Scene 02 - POPUP 2: Skull
//====================================

#include "L5_S4_Dungeon.h"
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
#include "L5S4_Pop1Bridge.h"

const CRectangle  ActionRectStone(365.0f,404.0f,200.0f,168.0f);
const CRectangle  InvRectPickAxe(558.0f,120.0f,223.0f,427.0f);
const CRectangle  InvDestRect(468.0f,119.0f,436.0f,426.0f);


const CRectangle  MemoryRectLocket(842.0f,321.0f,76.0f,115.0f);

static int No_Of_Rings = 5;
static float ANGLE_TO_ROTATE = 30;
static float RingRadious[] = {55, 90, 120, 150, 190};
static int SubRingIndex[] = {1, 4, 0, 2, 3};
static float SubRingRotateAngle[] = {-60, 30, -30, 0, 30};

L5S4_Pop1Bridge::L5S4_Pop1Bridge(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV5\\S4\\POP1\\P1.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	
	GetObjectByID("barkWithGrease")->SetEnable(false);
	GetObjectByID("greaseDrop")->SetEnable(false);
	GetObjectByID("greaseDrop1")->SetEnable(false);
	GetObjectByID("greaseDrop2")->SetEnable(false);
	GetObjectByID("greaseOnWheels")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L5_S4_InvPickaxeHeadTaken)
		GetObjectByID("PickAxeBlade")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L5_S4_Stone1Moved )
		GetObjectByID("stone1")->SetEnable(false);
	else
		GetObjectByID("stone1")->SetEnable(true);

	if(ProfileSystem::Instance->ActiveProfile.L5_S4_Stone2Moved )
		GetObjectByID("stone2")->SetEnable(false);
	else
		GetObjectByID("stone2")->SetEnable(true);

	if(ProfileSystem::Instance->ActiveProfile.L5_S4_GreaseUsed)
		GetObjectByID("rust")->SetEnable(false);

	JAnim_Grease = new JSONAnimator(0, CC_CALLBACK_1(L5S4_Pop1Bridge::OnAnimationComplete, this));
	JAnim_Grease->parseAndLoadJSON("ANIM\\L5\\S4P1GREASE.JSON", this);

	s4BridgePuzzleCompleteFXs.clear();
	s4BridgePuzzleCompleteFXs.push_back(new SuperFX("PRT\\L5\\s4BridgeRingsparks.INI", PointSystem::CreateCPoint(652,363,0), 1100)); 
	s4BridgePuzzleCompleteFXs.push_back(new SuperFX("PRT\\L5\\s4BridgeRingsparks.INI", PointSystem::CreateCPoint(631,386,0), 1100));
	s4BridgePuzzleCompleteFXs.push_back(new SuperFX("PRT\\L5\\s4BridgeRingsparks.INI", PointSystem::CreateCPoint(600,411,0), 1100)); 
	s4BridgePuzzleCompleteFXs.push_back(new SuperFX("PRT\\L5\\s4BridgeRingsparks.INI", PointSystem::CreateCPoint(581,435,0), 1100)); 
	s4BridgePuzzleCompleteFXs.push_back(new SuperFX("PRT\\L5\\s4BridgeRingsparks.INI", PointSystem::CreateCPoint(548,463,0), 1100)); 
	s4BridgePuzzleCompleteFXs.push_back(new SuperFX("PRT\\L5\\s4BridgeRingsparks.INI", PointSystem::CreateCPoint(517,487,0), 1100)); 
	 
	_isPuzzleSolved = ProfileSystem::Instance->ActiveProfile.L3_S4_PZ1BridgeCompleted;
	rotateTimer = 0;
	_mainRing.Clear();
	_subRing.Clear();
	InitRings();

	Show(false);

}

void L5S4_Pop1Bridge::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		//LampLite
		GetObjectByID("greaseOnWheels")->SetEnable(false);
		GetObjectByID("rust")->SetEnable(false);
		isAnimationPlaying = false;
		Cursor::Show(true);
	}
	
}

void L5S4_Pop1Bridge::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}

	if (itemVal == eInv_L5_S2_GreasySapBark &&  InvDestRect.Intersection(&dropPos))
	{

		if(!ProfileSystem::Instance->ActiveProfile.L5_S4_GreaseUsed && ProfileSystem::Instance->ActiveProfile.L5_S4_InvPickaxeHeadTaken )
		{
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			ProfileSystem::Instance->ActiveProfile.L5_S4_GreaseUsed= true;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S2_GreasySapBark] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L5_S2_GreasySapBark);
			JAnim_Grease->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s4p1_greaseused");
			isAnimationPlaying = true;
		}
		else 
		{
			Hud->ShowBannerText("L5axe");
		}
		return;
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
}

void L5S4_Pop1Bridge::Required()
{

}

void L5S4_Pop1Bridge::Update(float ds)
{
		Popup::Update(ds);
		if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup || Hud->IsDraggingItem() || Hud->IsComboInvPopupShowing)
		return;

		if(isAnimationPlaying)
		{
			Cursor::SetMode(CA_Normal);
			return;
		}

		if(!isMouseOverCloseButton)
			Cursor::SetMode(CA_Normal);

		CPoint pos = Control::Input->Mpos();



		switch (_pzState)
		{
		case L5S4_KState_WaitToStart:
			{
				if(!ProfileSystem::Instance->ActiveProfile.L5_S4_InvPickaxeHeadTaken && InvRectPickAxe.Intersection(&pos))
				{
					Cursor::SetMode(CA_HandTake);
					if( Control::Input->LBclicked())
					{
						GetObjectByID("PickAxeBlade")->SetEnable(false);
						ProfileSystem::Instance->ActiveProfile.L5_S4_InvPickaxeHeadTaken = true;
						Hud->TakeInventoryItem(eInv_L5_S4_PickaxeBlade, Control::Input->Mpos());

					}
				}
				else if( !ProfileSystem::Instance->ActiveProfile.L5_S4_Stone1Moved && ActionRectStone.Intersection(&pos))
				{
					Cursor::SetMode(CA_HandTake);
					if( Control::Input->LBclicked())
					{
						float tweenArray[5] = {0,998,334,1,0};
						GetObjectByID("stone1")->TweenTo(tweenArray,1,0,false);
						Control::Audio->QuickLoadAndPlaySFX("s4p1_stonemove1");
						ProfileSystem::Instance->ActiveProfile.L5_S4_Stone1Moved = true;
					}
				}
				else if( !ProfileSystem::Instance->ActiveProfile.L5_S4_Stone2Moved && ActionRectStone.Intersection(&pos))
				{
					Cursor::SetMode(CA_HandTake);
					if( Control::Input->LBclicked())
					{
						float tweenArray[5] = {0,998,334,1,0};
						GetObjectByID("stone2")->TweenTo(tweenArray,1,0,false);
						Control::Audio->QuickLoadAndPlaySFX("s4p1_stonemove2");
						ProfileSystem::Instance->ActiveProfile.L5_S4_Stone2Moved = true;
					}
				}
				else if(!ProfileSystem::Instance->ActiveProfile.L5_S4_GreaseUsed && ProfileSystem::Instance->ActiveProfile.L5_S4_InvPickaxeHeadTaken && InvDestRect.Intersection(&pos))
				{
					Cursor::SetMode(CA_Gear);
					if( Control::Input->LBclicked())
					{
						GetObjectByID("ring4")->Shake(1,0,10,0.5f);
						Control::Audio->QuickLoadAndPlaySFX("s4p1_discstuck");
						Hud->ShowBannerText("L5find");
					}
				}
				else if(ProfileSystem::Instance->ActiveProfile.L5_S4_GreaseUsed && 
					ProfileSystem::Instance->ActiveProfile.L5_S4_Stone1Moved && 
					ProfileSystem::Instance->ActiveProfile.L5_S4_Stone2Moved && 
					!ProfileSystem::Instance->ActiveProfile.L5_S4_BridgeOpened)
				{				

					Hud->InvBox_StartPuzzleMode();
					_pzState = L5S4_KState_Playing;
				}
			}
			break;

		case L5S4_KState_Playing:
			{
				for (int i = 0; i < (int)_rings.size(); i++)
			{
				_rings[i]->SetTint(1, 1, 1);
			}

			if (_mainRing._ringToRoate != NULL)
			{
				rotateTimer += Control::LogicRate * 3;

				float angle = _mainRing.startAngle + (_mainRing.endAngle - _mainRing.startAngle)*easing(rotateTimer, 0, 1, 1);
				_mainRing._ringToRoate->SetRotate(angle);

				float subangle = _subRing.startAngle + (_subRing.endAngle - _subRing.startAngle)*easing(rotateTimer, 0, 1, 1);
				_subRing._ringToRoate->SetRotate(subangle);
	
				if (rotateTimer >= 1.0f)
				{
					angle = _mainRing.endAngle;
					angle = (angle >= 360) ? angle - 360 : angle;
					angle = (angle < 0) ? 360 + angle : angle;
					_mainRing._ringToRoate->SetRotate(angle);

					subangle = _subRing.endAngle;
					subangle = (subangle >= 360) ? subangle - 360 : subangle;
					subangle = (subangle < 0) ? 360 + subangle : subangle;
					_subRing._ringToRoate->SetRotate(subangle);
					_subRing.Clear();

					_mainRing.Clear();
					_subRing.Clear();
					OnRotateComplete();
				}
			}
			else
			{
				for (int i = 0; i < (int)_rings.size(); i++)
				{
					float distance = Elixir::Distance(&_ringCenter, &pos);
					if (distance <= RingRadious[i])
					{
						_rings[i]->SetTint(2, 1.75f, 0.5f);
						Cursor::SetMode(CA_HandPointy);

						if( Control::Input->LBclicked() )
						{
							RotateRing(i);
						}

						break;
					}
				}
			} 	
		}
		break;

	case L5S4_KState_Skip:
		{
			StartAutoCompletePuzzle();
			ProfileSystem::Instance->ActiveProfile.L3_S4_PZ1BridgeCompleted = true;
			_pzState = L5S4_KState_AutoComplete;
		}
		break;

	case L5S4_KState_AutoComplete:
		{
			if (_mainRing._ringToRoate != NULL)
			{
				rotateTimer += Control::LogicRate * 2;

				float angle = _mainRing.startAngle + (_mainRing.endAngle - _mainRing.startAngle)*easing(rotateTimer, 0, 1, 1);
				_mainRing._ringToRoate->SetRotate(angle);

				if (rotateTimer >= 1.0f)
				{
					angle = _mainRing.endAngle = 0.0f;
					_mainRing._ringToRoate->SetRotate(angle);
					_mainRing.Clear();
					StartAutoCompletePuzzle();
				}
			}
		}
		break;

	case L5S4_KState_Won:
		{
			ProfileSystem::Instance->ActiveProfile.L3_S4_PZ1BridgeCompleted = true;
			_isPuzzleSolved = true;
			for (int i = 0; i < int(s4BridgePuzzleCompleteFXs.size()); i++)
			{
				s4BridgePuzzleCompleteFXs[i]->Start(i*0.2f);
			}

			_pzState++;
		}
		break;

	case L5S4_KState_Complete:
		{
			_pzState++;

			for (int i = 0; i < int(s4BridgePuzzleCompleteFXs.size()); i++)
			{
				s4BridgePuzzleCompleteFXs[i]->StopImmediate();
			}
			ClosePopup();
		}	
		break;

	default:
		_pzState++;
		break;
	}
}

float L5S4_Pop1Bridge::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L5S4_Pop1Bridge::~L5S4_Pop1Bridge()
{
	if(!_isPuzzleSolved)
		Hud->InvBox_StopPuzzleMode();

	SAFE_DELETE(JAnim_Grease);
	for (int i = 0; i < int(s4BridgePuzzleCompleteFXs.size()); i++)
	{
		SAFE_DELETE(s4BridgePuzzleCompleteFXs[i]);
	}
}

void L5S4_Pop1Bridge::TakeInventoryItem(EHOInventory ItemID)
{
	/*std::vector<int> vecInvIds;
	switch (ItemID)
	{
	
	default:
		break;
	}*/
}

void L5S4_Pop1Bridge::ReceiveMessage(int val)
{	

	
}

bool L5S4_Pop1Bridge::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	
	if(!ProfileSystem::Instance->ActiveProfile.L5_S4_InvPickaxeHeadTaken )
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S4_PZ1BridgeCompleted && ProfileSystem::Instance->ActiveProfile.L5_S2_GreaseTaken)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L5S4_Pop1Bridge::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L5_S4_InvPickaxeHeadTaken )
		Hud->InitHintTrailEmit(InvRectPickAxe.GetCenterX(), InvRectPickAxe.GetCenterY());
	else if(!ProfileSystem::Instance->ActiveProfile.L5_S4_Stone2Moved || !ProfileSystem::Instance->ActiveProfile.L5_S4_Stone1Moved)
		Hud->InitHintTrailEmit(ActionRectStone.GetCenterX(), ActionRectStone.GetCenterY());
	else if(!ProfileSystem::Instance->ActiveProfile.L5_S4_GreaseUsed && ProfileSystem::Instance->ActiveProfile.L5_S2_GreaseTaken)
		Hud->ShowHintForInvItem(eInv_L5_S2_GreasySapBark, &InvDestRect);
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S4_PZ1BridgeCompleted && ProfileSystem::Instance->ActiveProfile.L5_S4_GreaseUsed)
		Hud->InitHintTrailEmit(InvDestRect.GetCenterX(), InvDestRect.GetCenterY());
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

void L5S4_Pop1Bridge::ProcessSkip()
{
	_pzState = L5S4_KState_Skip;
}

void L5S4_Pop1Bridge::InitRings()
{
	_ringCenter = GetObjectByID("ring1")->Pos;

	for (int i = 1; i <= No_Of_Rings; i++)
	{
		CObject* ringObj = GetObjectByID(std::string("ring")+MKSTR(i));
		_rings.push_back(ringObj);
	}

	// handle saved data,
	if (!_isPuzzleSolved)
	{
		if (ProfileSystem::Instance->ActiveProfile.L3_S4_PZ1BridgeRingAngles[0] == -1)
		{
			ShuffleRings();
		}
		else
		{
			for (int i = 0; i < (int)_rings.size(); i++)
			{
				_rings[i]->SetRotate((float)(ProfileSystem::Instance->ActiveProfile.L3_S4_PZ1BridgeRingAngles[i]));
			}
		}

		_pzState = L5S4_KState_WaitToStart;
	}
}

void L5S4_Pop1Bridge::ShuffleRings()
{
	int angleArray[] = {300, 240, 120, 180, 120, 180, 60, 240, 300, 60, 300};
	int ranAngle = 0;

	for (int i = 0; i < (int)_rings.size(); i++)
	{
		ranAngle = rand()%11;
		_rings[i]->SetRotate((float)(angleArray[ranAngle]));
		ProfileSystem::Instance->ActiveProfile.L3_S4_PZ1BridgeRingAngles[i] = (int)(angleArray[ranAngle]);
	}
}

void L5S4_Pop1Bridge::RotateRing(int index)
{
	int direction = (int)(pow(-1, ((index % 2) + 1)));
	float angleToRotate = (float)(ANGLE_TO_ROTATE * direction);

	float currentAngle = _rings[index]->GetRotate();
	float newAngle = currentAngle + angleToRotate;

	Control::Audio->QuickLoadAndPlaySFX("s4p1_discrotate");

	rotateTimer = 0;
	_mainRing._ringToRoate = _rings[index];
	_mainRing.startAngle = currentAngle;
	_mainRing.endAngle = newAngle;

	_subRing._ringToRoate = _rings[SubRingIndex[index]];
	_subRing.startAngle = _subRing._ringToRoate->GetRotate();
	_subRing.endAngle = _subRing._ringToRoate->GetRotate() + (SubRingRotateAngle[index] * direction);
}

void L5S4_Pop1Bridge::OnRotateComplete()
{
	for (int i = 0; i < (int)_rings.size(); i++)
	{
		ProfileSystem::Instance->ActiveProfile.L3_S4_PZ1BridgeRingAngles[i] = (int)_rings[i]->GetRotate();
	}

	for (int i = 0; i < (int)_rings.size(); i++)
	{
		if (_rings[i]->GetRotate() != 0)
		{
			return;
		}
	}

	OnPuzzleComplete();
}

void L5S4_Pop1Bridge::OnPuzzleComplete()
{
	_pzState = L5S4_KState_Won;
	Control::Audio->PlayCollectFX();
}

void L5S4_Pop1Bridge::StartAutoCompletePuzzle()
{
	for (int i = 0; i < (int)_rings.size(); i++)
	{
		if (_rings[i]->GetRotate() != 0)
		{
			Control::Audio->QuickLoadAndPlaySFX("s4p1_discrotate");
			RotateRingToZero(i);
			return;
		}
	}

	OnPuzzleComplete();
}

void L5S4_Pop1Bridge::RotateRingToZero(int index)
{
	float currentAngle = _rings[index]->GetRotate();

	int direction = (int)(pow(-1, ((index % 2) + 1)));
	float angleToRotate = 0;
	
	if (direction == 1)
	{
		angleToRotate = 360 - currentAngle;
	}
	else
	{
		angleToRotate = -currentAngle;
	}

	float newAngle = currentAngle + angleToRotate;

	rotateTimer = 0;
	_mainRing._ringToRoate = _rings[index];
	_mainRing.startAngle = currentAngle;
	_mainRing.endAngle = newAngle;
}

void L5S4_Pop1Bridge::OnPopupOpened()
{
	Popup::OnPopupOpened();
	if(_pzState == L5S4_KState_Playing)
		Hud->InvBox_StartPuzzleMode();
}

void L5S4_Pop1Bridge::OnPopupCloseBtnClicked()
{
	Popup::OnPopupCloseBtnClicked();
	if(_pzState != L5S4_KState_WaitToStart)
		Hud->InvBox_StopPuzzleMode();
}

