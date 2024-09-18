//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 05 : MOOPAANS DEN
//====================================

#include "L8_S2_GearPuzzle.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "ParticleEffect.h"
#include "Application.h"
#include "Util.h"
#include "exMath.h"


#define Gear_Default_Z_Order 40
#define Gear_Slot_Z_Order 4

static int rotationDirection[L8_S2_NUM_GEARS] = {-1, 1, -1, 1, -1, 1, -1, 1, -1, 1, 1, -1, 1};
static float firstGearRadius = 15;
static float firstGearAngularVelocity = 1.0f;
const CRectangle  keyRect(550.0f,420.0f,266.0f,232.0f);

const CRectangle  GearPuzzleRect(336.0f,58.0f,692.0f,524.0f);


L8_S2_GearPuzzle::L8_S2_GearPuzzle()
{
	_isDoorMoving = false;
	_currentDraggingGear = NULL;
	//Control::Audio->QuickLoadAndPlaySFX("s2pz1_GearTurning", true);
	InitializeWithProfileData();

	//MUSIC_SFX
	/*Control::Audio->LoadSample(aAmbL4s2GearPuzzleAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL4s2GearPuzzleAmb) )		
	{
	Control::Audio->PlaySample(aAmbL4s2GearPuzzleAmb,true);
	}*/

	//MUSIC_SFX
}

L8_S2_GearPuzzle::~L8_S2_GearPuzzle()
{
	Hud->InvBox_StopPuzzleMode();
	for (int i = 0; i < (int)(_arrayOfGears.size()); i++)
	{
		Gear * gear = _arrayOfGears.at(i);
		SAFE_DELETE(gear);
	}
	_arrayOfGears.clear();

	for (int i = 0; i < (int)(_arrayOfGearStots.size()); i++)
	{
		GearSlot * gearSlot = _arrayOfGearStots.at(i);
		SAFE_DELETE(gearSlot);
	}
	_arrayOfGearStots.clear();

	SAFE_DELETE(_jsonAnimDoorOpen);
	SAFE_DELETE(_jsonAnimKeyInsertion);


	//MUSIC_SFX

	/*Control::Audio->StopSample(aAmbL4s2GearPuzzleAmb);
	Control::Audio->UnloadSample(aAmbL4s2GearPuzzleAmb);*/
	//MUSIC_SFX
}

void L8_S2_GearPuzzle::InitializeWithProfileData()
{
	//LoadHud();

	LoadHud(INVENTORY_V1);



	Hud->InvBox_StopPuzzleMode();
	//Hud->InvBox_StartPuzzleMode();
	LoadScene("LV8\\S2\\GEARPUZZLE\\GEARPUZZLE.SCN");

	_key = HOScene->GetObjectByID("Key1");


	int ArrayPts1[] = {650,533,701,500,781,549,779,584,688,599};
	CPolygon * keyPoly =  new CPolygon(5);
	keyPoly->Init(ArrayPts1);
	_key->SetMouseClickProperties(keyPoly, CC_CALLBACK_1(L8_S2_GearPuzzle::OnMouseClickOnObject, this));
	_key->SetIsInteractionEnabled(false);


	int ArrayPts2[] = {442,503,518,541,565,494,523,480,443,485};
	CPolygon * toolPoly =  new CPolygon(5);
	toolPoly->Init(ArrayPts2);
	_tool = HOScene->GetObjectByID("Tool");
	_tool->SetMouseClickProperties(toolPoly, CC_CALLBACK_1(L8_S2_GearPuzzle::OnMouseClickOnObject, this));
	_tool->SetIsInteractionEnabled(false);
	_tool->SetEnable(false);

	_code = HOScene->GetObjectByID("Code");
	_code->SetMouseClickCallback(CC_CALLBACK_1(L8_S2_GearPuzzle::OnMouseClickOnObject, this));
	_code->SetIsInteractionEnabled(false);
	_code->SetEnable(false);

	_door = HOScene->GetObjectByID("Bg1");
	_jsonAnimKeyInsertion = new JSONAnimator(2,  CC_CALLBACK_1(L8_S2_GearPuzzle::OnKeyInsertionAnimComplete, this));
	_jsonAnimKeyInsertion->parseAndLoadJSON("ANIM\\L8\\S2GEARKEYINSERTION.JSON", HOScene);

	_jsonAnimDoorOpen = new JSONAnimator(1,  CC_CALLBACK_1(L8_S2_GearPuzzle::OnDoorOpenAnimComplete, this));
	_jsonAnimDoorOpen->parseAndLoadJSON("ANIM\\L8\\S2GEARDOOROPEN.JSON", HOScene);



	bool isMovable[L8_S2_NUM_GEARS] = {false, true, true, true, true, true, true, false, false, true, true, true, false};

	for (int i = 0; i < L8_S2_NUM_GEARS; i++)
	{
		Gear * gear = new Gear(i);
		std::string gearId = std::string("Gear_") + MKSTR(i);
		std::string shadowId = std::string("GearShadow_") + MKSTR(i);
		gear->normal = HOScene->GetObjectByID(gearId);
		gear->shadow = HOScene->GetObjectByID(shadowId);
		gear->shadow->SetXY(gear->normal->Pos.x, gear->normal->Pos.y);
		//gear->radius =  radius[i];
		if (i == 0)
		{
			firstGearRadius = gear->normal->GetWorldRect().w / 2;
			gear->angularVelocity = firstGearAngularVelocity * rotationDirection[0];
		}
		gear->radius =  gear->normal->GetWorldRect().w / 2;		gear->angularSpeed = (firstGearRadius / gear->radius) * firstGearAngularVelocity;
		if (i == 0)
		{
			gear->SetState(eGearState_Rotating);
		}
		else
		{
			gear->SetState(eGearState_Default);
		}


		_arrayOfGears.push_back(gear);

		GearSlot * gearSlot = new GearSlot();
		gearSlot->position = gear->normal->Pos;
		gearSlot->radius = gear->radius;
		gearSlot->rotationDirection = rotationDirection[i];
		//	gearSlot->maxRadius = gearSlotMaxRadius[i];
		_arrayOfGearStots.push_back(gearSlot);



		if (isMovable[i])
		{
			std::string invGearId = std::string("InvGear_") + MKSTR(i);
			Gear * invGear = new Gear(i);
			invGear->normal = HOScene->GetObjectByID(invGearId);
			gear->normal->SetPos(invGear->normal->GetPosPointer());
			gear->shadow->SetXY(gear->normal->Pos.x, gear->normal->Pos.y);
			_arrayOfInteractiveGears.push_back(gear);
			invGear->normal->Show(false);
			gear->invPosition = invGear->normal->Pos;
			gear->normal->SetMouseInteractionArea(gear->normal->GetWorldRect().w / 2.0f);


		}
	}

	for (int i = 0; i < L8_S2_NUM_GEARS; i++)
	{
		GearSlot * gearSlot = _arrayOfGearStots.at(i);
		switch (i)
		{
		case 0:
			{
				gearSlot->SetGear(_arrayOfGears.at(i));
				gearSlot->isSolved = true;
				if (ProfileSystem::Instance->ActiveProfile.L8_S2_GearPuzzleState >= eGearPuzzleState_KeyInserted)
				{
					gearSlot->isRotating = true;
				}
				_arrayOfGears.at(i)->gearSlot = gearSlot;
				_arrayOfGears.at(i)->SetState(eGearState_Fixed);
				gearSlot->drivingNeighbours.push_back(gearSlot);
			}
			break;

		case 1:
		case 2:
		case 3:
		case 5:
		case 6:
			{
				gearSlot->drivingNeighbours.push_back(_arrayOfGearStots.at(i - 1));
			}
			break;
		case 4:
			{
				gearSlot->drivingNeighbours.push_back(_arrayOfGearStots.at(3));
				gearSlot->drivingNeighbours.push_back(_arrayOfGearStots.at(11));
			}
			break;
		case 7:
			{
				gearSlot->drivingNeighbours.push_back(_arrayOfGearStots.at(i - 1));
				gearSlot->SetGear(_arrayOfGears.at(i));
				_arrayOfGears.at(i)->gearSlot = gearSlot;
				_arrayOfGears.at(i)->SetState(eGearState_Fixed);
				gearSlot->isSolved = true;
			}
			break;

		case 8:
			{
				gearSlot->drivingNeighbours.push_back(_arrayOfGearStots.at(9));
				gearSlot->SetGear(_arrayOfGears.at(i));
				_arrayOfGears.at(i)->gearSlot = gearSlot;
				_arrayOfGears.at(i)->SetState(eGearState_Fixed);
				gearSlot->isSolved = true;
			}
			break;
		case 9:
			{
				gearSlot->drivingNeighbours.push_back(_arrayOfGearStots.at(2));
			}
			break;
		case 10:
			{
				gearSlot->drivingNeighbours.push_back(_arrayOfGearStots.at(4));

			}
			break;
		case 11:
			{
				gearSlot->drivingNeighbours.push_back(_arrayOfGearStots.at(10));
			}
			break;
		case 12:
			{
				gearSlot->drivingNeighbours.push_back(_arrayOfGearStots.at(11));
				gearSlot->SetGear(_arrayOfGears.at(i));
				gearSlot->isSolved = true;
				_arrayOfGears.at(i)->SetState(eGearState_Fixed);
				_arrayOfGears.at(i)->gearSlot = gearSlot;

			}
			break;

		default:
			break;
		}
		int gearId = ProfileSystem::Instance->ActiveProfile.L8_S2_GearSlotGearId[i];
		if (gearId != -1)
		{
			gearSlot->SetGear(_arrayOfGears.at(gearId));
			gearSlot->isSolved = true;
			_arrayOfGears.at(gearId)->SetState(eGearState_Fixed);
			_arrayOfGears.at(gearId)->gearSlot = gearSlot;
			gearSlot->GetGear()->normal->SetXY(gearSlot->position.x, gearSlot->position.y);
			gearSlot->GetGear()->shadow->SetXY(gearSlot->GetGear()->normal->Pos.x, gearSlot->GetGear()->normal->Pos.y);
		}

	}

	UpdateGearStates();


	switch ((L8S2GearPuzzleState)ProfileSystem::Instance->ActiveProfile.L8_S2_GearPuzzleState)
	{
	case eGearPuzzleState_started:
		{
			_jsonAnimDoorOpen->SetAnimTime(0);
			_jsonAnimKeyInsertion->SetAnimTime(0);
		}
		break;
	case eGearPuzzleState_KeyInserted:
		{
			_jsonAnimDoorOpen->SetAnimTime(1);
			Hud->InvBox_StartPuzzleMode();
			_key->SetIsInteractionEnabled(true);
		}
		break;
	case eGearPuzzleState_Solved:
		{
			_jsonAnimDoorOpen->SetAnimTime(1);
			_tool->SetEnable(true);
			_code->SetEnable(true);
			_key->SetIsInteractionEnabled(true);
		}
		break;
	case eGearPuzzleState_DoorOpened:
		{
			_jsonAnimKeyInsertion->SetAnimTime(1);
			_jsonAnimDoorOpen->SetAnimTime(3);
			if (!ProfileSystem::Instance->ActiveProfile.L8_S2_InvToolTaken)
			{
				_tool->SetEnable(true);
				_tool->SetIsInteractionEnabled(true);
			}
			if (!ProfileSystem::Instance->ActiveProfile.L8_S2_InvScrollTaken)
			{
				_code->SetEnable(true);
				_code->SetIsInteractionEnabled(true);
			}
			for (int i = 0; i < L8_S2_NUM_GEARS; i++)
			{
				Gear * gear = _arrayOfGears.at(i);
				gear->normal->SetEnable(false);
				gear->shadow->SetEnable(false);
			}
		}
		break;
	default:
		break;
	}

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L8_S2_PZGears]  = true;
}

void L8_S2_GearPuzzle::Update()
{
	NavigateToScene();
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}

	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
		//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S2_Garden;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif	

	//if( IsMouseOverGoBackArea(&Control::Input->Mpos()))
	// Linux: gives error of taking address of temporary also potential crash factor
	// below is fix
	CPoint mPos =Control::Input->Mpos();
	if( IsMouseOverGoBackArea(&mPos))
	{
		
#ifdef TOUCH_UI
//		if(!GFHud->InvBoxCtrl->navTapped)
//			Cursor::SetMode(CA_ExitDown, eArea_L8_S2_Garden);
#else
		Cursor::SetMode(CA_ExitDown, eArea_L8_S2_Garden);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L8_S2_Garden, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S2_Garden;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
			
			return;
		}	
		return;
	}


	HandleMouseClickOnGears();

	Cursor::SetMode(CA_Normal);
	// Gear * gear = NULL; //Sac: Unused variable, so commented it
	CPoint mousePos = Control::Input->Mpos();

	if (_currentDraggingGear)
	{
		_currentDraggingGear->normal->SetPos(&mousePos);
	}
	
	if (ProfileSystem::Instance->ActiveProfile.L8_S2_GearPuzzleState == eGearPuzzleState_started)
	{
		if (keyRect.Intersection(&mousePos))
		{
			Cursor::SetMode(CA_Gear);
		}
	}

	if (ProfileSystem::Instance->ActiveProfile.L8_S2_GearPuzzleState == eGearPuzzleState_KeyInserted)
	{
		for (int i = 0; i < (int)(_arrayOfGears.size()); i++)
		{
			Gear * gear = _arrayOfGears.at(i);
			if (gear->normal->GetCurrentMouseState() == eMouseState_Over || gear->normal->GetCurrentMouseState() == eMouseState_Down)
			{
				Cursor::SetMode(CA_HandPointy);
				break;
			}
		}
	}
	if (ProfileSystem::Instance->ActiveProfile.L8_S2_GearPuzzleState == eGearPuzzleState_Solved)
	{
		if (_key->GetCurrentMouseState() == eMouseState_Over || _key->GetCurrentMouseState() == eMouseState_Down)
		{
			Cursor::SetMode(CA_HandPointy);
		}
	}

	if (_tool->GetCurrentMouseState() == eMouseState_Over || _tool->GetCurrentMouseState() == eMouseState_Down)
	{
		Cursor::SetMode(CA_HandTake);
	}

	if (_code->GetCurrentMouseState() == eMouseState_Over || _code->GetCurrentMouseState() == eMouseState_Down)
	{
		Cursor::SetMode(CA_HandTake);
	}

	
	if(GearPuzzleRect.Intersection(&mousePos) && !(ProfileSystem::Instance->ActiveProfile.L8_S2_GearPuzzleState == eGearPuzzleState_KeyInserted))
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked() )
		{
			Cursor::SetMode(CA_Normal);
			Hud->ShowBannerText("L8order");
		}
		return;
	}

}

void L8_S2_GearPuzzle::Required()
{
	HiddenObjectSystem::Required();
	if (_isDoorMoving)
	{
		for (int i = 0; i < (int)(_arrayOfGears.size()); i++)
		{
			Gear * gear = _arrayOfGears.at(i);
			gear->normal->SetY(_door->Pos.y + gear->offsetY);
		}
	}
}

void L8_S2_GearPuzzle::OnPuzzleSolved()
{
	
	Control::Audio->QuickLoadAndPlaySFX("s2pz1_GearTurning", true);
	Hud->InvBox_StopPuzzleMode();
	ProfileSystem::Instance->ActiveProfile.L8_S2_GearPuzzleState = eGearPuzzleState_Solved;
	_tool->SetEnable(true);

	_code->SetEnable(true);
	AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
}

void L8_S2_GearPuzzle::OnMouseClickOnObject(CObject * object)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	if (object == _key)
	{
		if (ProfileSystem::Instance->ActiveProfile.L8_S2_GearPuzzleState == eGearPuzzleState_Solved)
		{
			_key->SetIsInteractionEnabled(false);
			_jsonAnimDoorOpen->PlayAnim(false);

			Control::Audio->StopSFX("s2pz1_GearTurning");
			Control::Audio->QuickLoadAndPlaySFX("s2pz1_DoorOpening");
			for (int i = 0; i < (int)(_arrayOfGears.size()); i++)
			{
				Gear * gear = _arrayOfGears.at(i);
				gear->offsetY =  gear->normal->Pos.y - _door->Pos.y;
			}
			_isDoorMoving = true;
		}
		if (ProfileSystem::Instance->ActiveProfile.L8_S2_GearPuzzleState == eGearPuzzleState_KeyInserted)
		{
		//	Hud->ShowMessageBanner("TODO: Solve the puzzle");
		}
		if (ProfileSystem::Instance->ActiveProfile.L8_S2_GearPuzzleState == eGearPuzzleState_started)
		{
			Hud->ShowBannerText("L8key");
		}
	}
	if (object == _tool)
	{
		_tool->SetEnable(false);
		ProfileSystem::Instance->ActiveProfile.L8_S2_InvToolTaken = true;
		Hud->TakeInventoryItem(eInv_L8_S2_Tool);
		_tool->SetIsInteractionEnabled(false);
	}
	if (object == _code)
	{
		_code->SetEnable(false);
		ProfileSystem::Instance->ActiveProfile.L8_S2_InvScrollTaken = true;
		Hud->TakeInventoryItem(eInv_L8_S2_SymbolCombination);
		_code->SetIsInteractionEnabled(false);
	}
}


bool L8_S2_GearPuzzle::IsOverlapWithGears(Gear * gear)
{
	for (int i = 0; i < (int)(_arrayOfGears.size()); i++)
	{
		Gear * tempGear = _arrayOfGears.at(i);
		if (tempGear != gear)
		{
			float dist = Elixir::Distance((int)tempGear->normal->Pos.x, (int)tempGear->normal->Pos.y, (int)gear->normal->Pos.x, (int)gear->normal->Pos.y);
			float minimumDistanceToAvoidOverlapping = tempGear->radius + gear->radius;
			// float offset = minimumDistanceToAvoidOverlapping / 4; //Sac: Unused variable, so commented it
			if (dist < (minimumDistanceToAvoidOverlapping - 10))
			{
				return true;
			}
		}
	}
	return false;
}

bool L8_S2_GearPuzzle::IsTouchWithGears(Gear * gear1, Gear * gear2)
{
	float dist = Elixir::Distance((int)gear1->normal->Pos.x, (int)gear1->normal->Pos.y, (int)gear2->normal->Pos.x, (int)gear2->normal->Pos.y);
	float minimumDistanceToAvoidOverlapping = gear1->radius + gear2->radius;
	if (dist < minimumDistanceToAvoidOverlapping - 3)
	{
		return true;
	}
	else
	{
		return false;
	}
}

GearSlot * L8_S2_GearPuzzle::GetGearSlot(Gear * gear)
{
	GearSlot * gearSlot = NULL;
	CPoint snappingPos;
	for (int i = 0; i < (int)(_arrayOfGearStots.size()); i++)
	{
		gearSlot = _arrayOfGearStots.at(i);
		CPoint snappingPos = gearSlot->position;
		float dist = Elixir::Distance((int)snappingPos.x, (int)snappingPos.y, (int)gear->normal->Pos.x, (int)gear->normal->Pos.y);
		if (dist < 50)
		{
			return gearSlot;
		}
	}
	snappingPos.x = 0;
	snappingPos.y = 0;
	return NULL;
}

void L8_S2_GearPuzzle::OnMouseClickOnGear(Gear * gear)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	if (Hud->IsDraggingItem())
	{
		return;
	}
	if (ProfileSystem::Instance->ActiveProfile.L8_S2_GearPuzzleState == eGearPuzzleState_started)
	{
		Hud->ShowBannerText("L8key");
	}
	else if (ProfileSystem::Instance->ActiveProfile.L8_S2_GearPuzzleState == eGearPuzzleState_KeyInserted)
	{
		//SLOG::Log("id = ", gear->GetId());
		
		if (_currentDraggingGear)
		{
			if (_currentDraggingGear->GetId() != gear->GetId())
			{
				TweenGearToInv(_currentDraggingGear);
				_currentDraggingGear->SetState(eGearState_Default);
				_currentDraggingGear = NULL;
				return;

			}
		}

		GearState gearState = gear->GetState();
		switch (gearState)
		{
		case eGearState_Default:
			{
				_currentDraggingGear = gear;
				gear->SetState(eGearState_Dragging);
			}
			break;
		case eGearState_Dragging:
			{
				_currentDraggingGear = NULL;
				GearSlot * gearSlot = GetGearSlot(gear);

				if (gearSlot == NULL)
				{
					gear->SetState(eGearState_Default);
					TweenGearToInv(gear);
				}
				else
				{
					gear->SetState(eGearState_Default);
					gear->normal->SetXY(gearSlot->position.x, gearSlot->position.y);
					if (IsOverlapWithGears(gear))
					{
						TweenGearToInv(gear);
					}
					else
					{
						gearSlot->SetGear(gear);
						gear->gearSlot = gearSlot;
						gear->SetState(eGearState_Fixed);
						Control::Audio->QuickLoadAndPlaySFX("s2pz1_GearPlacing");
						UpdateGearStates();
						if (IsPuzzleSolved())
						{
							OnPuzzleSolved();
						}
					}
				}
			}
			break;

		case eGearState_Fixed:
		case eGearState_Rotating:
			{
				_currentDraggingGear = gear;
				gear->SetState(eGearState_Dragging);
				gear->gearSlot->SetGear(NULL);
				gear->gearSlot = NULL;
				UpdateGearStates();
			}
			break;
		default:
			break;
		}
	}



}

void L8_S2_GearPuzzle::SkipPuzzle()
{

	for (int i = 0; i < (int)(_arrayOfGearStots.size()); i++)
	{
		GearSlot * gearSlot  = _arrayOfGearStots.at(i);
		Gear * gear = _arrayOfGears.at(i);
		_currentDraggingGear = NULL;
		gearSlot->SetGear(gear);
		gearSlot->isSolved = true;
		gear->SetState(eGearState_Fixed);
		gear->gearSlot = gearSlot;

		float tweenVals[5] = {-1, gearSlot->position.x, gearSlot->position.y, -1, -1};
		if (i == (int)(_arrayOfGearStots.size() - 1))
		{
			gear->normal->TweenTo(tweenVals, 0.5, eEase_COSINE, true, CC_CALLBACK_1(L8_S2_GearPuzzle::OnSkipAnimComplete, this));
		}
		else
		{
			gear->normal->TweenTo(tweenVals, 0.5, eEase_COSINE, true);
		}
	}



}
void L8_S2_GearPuzzle::OnSkipAnimComplete(CObject * object)
{
	for (int i = 0; i < (int)(_arrayOfGearStots.size()); i++)
	{
		Gear * gear = _arrayOfGears.at(i);
		gear->SetState(eGearState_Rotating);
		ProfileSystem::Instance->ActiveProfile.L8_S2_GearSlotGearId[i] = gear->GetId();
	}
	OnPuzzleSolved();
}

void L8_S2_GearPuzzle::ProcessSkip()
{
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	SkipPuzzle();
}

void L8_S2_GearPuzzle::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		//TaskPendingUpdate();
		Hud->ShowMap();
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;

		if( Hud->GetDropInfo( dropPos, itemVal ) )
		{

			if (itemVal == eInv_L8_CB_FullKey)
			{

				if (keyRect.Intersection(&dropPos))
				{
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 

					//Clear Inventory. One Time Use Items.
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_CB_FullKey] = false;
					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_CB_FullKey);
					_jsonAnimKeyInsertion->PlayAnim(false);
					Control::Audio->QuickLoadAndPlaySFX("s2pz1_KeyInserting");
					//Control::Audio->QuickLoadAndPlaySFX("s2pz2_keyInsert");
				}
				else
				{
					GFInstance->WrongInvDropMsg(Hud);
				}
			}
			else
			{
				GFInstance->WrongInvDropMsg(Hud);
			}

		}
	}
	else if ( val == INV_CLOSE)
	{
		if (ProfileSystem::Instance->ActiveProfile.L8_S2_InvToolTaken && ProfileSystem::Instance->ActiveProfile.L8_S2_InvScrollTaken)
		{		

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S2_Garden;
			SendMessage(ControlExit, ID, HOSM_NewArea);

		}
	}
}

void L8_S2_GearPuzzle::ProcessNonHOHint()
{
	int iNonHOHint = 0; 
	int MaxNonHOHints = 24;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
		case 0: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L8_S2_GearPuzzleState <= eGearPuzzleState_KeyInserted && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_CB_FullKey])
			{
				Hud->ShowHintForInvItem( eInv_L8_CB_FullKey, PointSystem::CreateCPoint(keyRect.GetCenterX(),keyRect.GetCenterY(),0));
				return;
			}
			break;
		case 1: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L8_S2_GearPuzzleState == eGearPuzzleState_Solved)
			{
				//InitHintTrailEmit(&_key->GetWorldRect(), false, CA_HandPointy);
				// Linux: gives error of taking address of temporary also potential crash factor
				// below is fix
				CRectangle ObjRect =_key->GetWorldRect();
				InitHintTrailEmit(&ObjRect, false, CA_HandPointy);
				return;
			}
			break;
		case 2: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L8_S2_GearPuzzleState == eGearPuzzleState_DoorOpened && !ProfileSystem::Instance->ActiveProfile.L8_S2_InvToolTaken)
			{
				//InitHintTrailEmit(&_tool->GetWorldRect(), false, CA_HandPointy);
				// Linux: gives error of taking address of temporary also potential crash factor
				// below is fix
				CRectangle ObjRect =_tool->GetWorldRect();
				InitHintTrailEmit(&ObjRect, false, CA_HandPointy);
				return;
			}
			break;
		case 3: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L8_S2_GearPuzzleState == eGearPuzzleState_DoorOpened && !ProfileSystem::Instance->ActiveProfile.L8_S2_InvScrollTaken)
			{
				//InitHintTrailEmit(&_code->GetWorldRect(), false, CA_HandPointy);
				// Linux: gives error of taking address of temporary also potential crash factor
				// below is fix
				CRectangle ObjRect =_code->GetWorldRect();
				InitHintTrailEmit(&ObjRect, false, CA_HandPointy);
				return;
			}
			break;
		case 4: ++iNonHOHint;
			if(  ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S2_Garden] )
			{
				InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
				return;
			}
			break;		
		case 5: ++iNonHOHint;
			if(Hud->ShowHintForComboInvItems())
				return;
		case 6: ++iNonHOHint;
				{
					Hud->teleportSystem->ShowTeleport();
					return;
				}
				break;
		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();
}


void L8_S2_GearPuzzle::TaskPendingUpdate()
{
	/*bool isAnyTaskLeft = false;

	if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S5_CaveDoorKey] &&
	ProfileSystem::Instance->ActiveProfile.L8_S2_GearPuzzleState <= eGearPuzzleState_KeyInserted )
	isAnyTaskLeft = true;
	else if(ProfileSystem::Instance->ActiveProfile.L8_S2_GearPuzzleState >= eGearPuzzleState_KeyInserted && 
	!ProfileSystem::Instance->ActiveProfile.L4_S2_DoorToCaveOpened ) 
	isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S2_PZDoor] = isAnyTaskLeft;*/

}

bool L8_S2_GearPuzzle::IsPuzzleSolved()
{
	bool isSolved = true;
	for (int i = 0; i < (int)_arrayOfGears.size(); i++)
	{
		Gear * gear = _arrayOfGears.at(i);
		if (gear->GetState() != eGearState_Rotating)
		{
			isSolved = false;
			break;
		}
	}

	return isSolved;
}

void L8_S2_GearPuzzle::TweenGearToInv(Gear * gear)
{
	if (gear->gearSlot != NULL)
	{
		gear->gearSlot->isRotating = false;
		gear->gearSlot->isSolved = false;
		gear->gearSlot = NULL;
	}
	float tweenVals[5] = {-1, gear->invPosition.x, gear->invPosition.y, -1, -1};
	gear->normal->TweenTo(tweenVals, 0.5, eEase_BACKOUT, true, CC_CALLBACK_1(L8_S2_GearPuzzle::OnTweenGearComplete, this));
}
void L8_S2_GearPuzzle::OnTweenGearComplete(CObject * gear)
{
	//_currentDraggingGear = NULL;
}

void L8_S2_GearPuzzle::UpdateGearStates()
{
	for (int i = 1; i < (int)_arrayOfGearStots.size(); i++)
	{
		GearSlot * gearSlot = _arrayOfGearStots.at(i);
		gearSlot->isRotating = false;
	}
	for (int i = 0; i < (int)_arrayOfGearStots.size(); i++)
	{
		GearSlot * gearSlot = _arrayOfGearStots.at(i);
		UpdateGearState(gearSlot);
	}
	for (int i = 0; i < (int)_arrayOfGearStots.size(); i++)
	{
		GearSlot * gearSlot = _arrayOfGearStots.at(i);
		UpdateGearState(gearSlot);
		if (gearSlot->GetGear() != NULL)
		{
			ProfileSystem::Instance->ActiveProfile.L8_S2_GearSlotGearId[i] = gearSlot->GetGear()->GetId();      
		}
		else
		{
			ProfileSystem::Instance->ActiveProfile.L8_S2_GearSlotGearId[i] = -1;
		}

	}
	/*UpdateGearState(_arrayOfGearStots.at(4));
	UpdateGearState(_arrayOfGearStots.at(10));*/
}

void L8_S2_GearPuzzle::UpdateGearState(GearSlot * gearSlot)
{
	if (gearSlot->GetGear() != NULL)
	{
		for (int j = 0; j < (int)gearSlot->drivingNeighbours.size(); j++)
		{
			GearSlot * drivingGearSlot = gearSlot->drivingNeighbours.at(j);
			bool isTouchingNeighbour = false;
			if (gearSlot->GetGear() != NULL && drivingGearSlot->GetGear() != NULL)
			{
				isTouchingNeighbour = IsTouchWithGears(gearSlot->GetGear(), drivingGearSlot->GetGear());
			}

			if (isTouchingNeighbour)
			{
				if (drivingGearSlot->isRotating)
				{

					gearSlot->GetGear()->SetState(eGearState_Rotating);
					gearSlot->isRotating = true;
					break;
				}
				else
				{
					gearSlot->GetGear()->SetState(eGearState_Fixed);
					gearSlot->isRotating = false;
				}
			}
			else
			{
				gearSlot->GetGear()->SetState(eGearState_Fixed);
				gearSlot->isRotating = false;
			}
		}
	}
	else
	{
		gearSlot->isRotating = false;
		gearSlot->isSolved = false;
	}
}

void L8_S2_GearPuzzle::HandleMouseClickOnGears()
{
	if (Control::Input->LBclicked())
	{
		if (_currentDraggingGear)
		{
			if (_currentDraggingGear->normal->GetCurrentMouseState() == eMouseState_Click)
			{
				OnMouseClickOnGear(_currentDraggingGear);
			}
		}
		else
		{
			for (int i = 0; i < (int)(_arrayOfGears.size()); i++)
			{
				Gear * gear = _arrayOfGears.at(i);

				if (gear->normal->GetCurrentMouseState() == eMouseState_Click)
				{
					OnMouseClickOnGear(gear);
					break;
				}
			}
		}
	}

}

void L8_S2_GearPuzzle::OnDoorOpenAnimComplete(int id)
{
	_tool->SetIsInteractionEnabled(true);
	_code->SetIsInteractionEnabled(true);
	ProfileSystem::Instance->ActiveProfile.L8_S2_GearPuzzleState = eGearPuzzleState_DoorOpened;
	_isDoorMoving = false;

}

void L8_S2_GearPuzzle::OnKeyInsertionAnimComplete(int id)
{
	ProfileSystem::Instance->ActiveProfile.L8_S2_GearPuzzleState = eGearPuzzleState_KeyInserted;
	_key->SetIsInteractionEnabled(true);
	_jsonAnimDoorOpen->SetAnimTime(0);
	Hud->InvBox_StartPuzzleMode();
	_arrayOfGearStots.at(0)->isRotating = true;
	_arrayOfGearStots.at(0)->GetGear()->SetState(eGearState_Rotating);
}

void L8_S2_GearPuzzle::NavigateToScene()
{
	if(GFHud->InvBoxCtrl->navTapped)
	{
		if( Control::Input->LBclicked() )
		{
			if(Control::Input->Mpos().x > GFHud->InvBoxCtrl->navRectX2 && Control::Input->Mpos().x < GFHud->InvBoxCtrl->navRectX1 && Control::Input->Mpos().y > GFHud->InvBoxCtrl->navRectY2 && Control::Input->Mpos().y < GFHud->InvBoxCtrl->navRectY1)
			{	
				//Cursor::SetMode(CA_ExitUp);
				
				//GFHud->InvBoxCtrl->NavigateText->Show(false);
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = GFHud->InvBoxCtrl->travelArea;
				SendMessage(ControlExit, ID, HOSM_NewArea);
			}
				
			GFHud->InvBoxCtrl->NavigateText->Text = "";
			GFHud->InvBoxCtrl->travelArea = -1;
			GFHud->InvBoxCtrl->navTapped = false;
			GFHud->InvBoxCtrl->Navigate->Show(false);					
			GFHud->InvBoxCtrl->NavArrow->Show(false);
		}
	}
}

#pragma region GEAR
Gear::Gear(int gearId)
{
	normal = NULL;
	shadow = NULL;
	_id = gearId;
	_state = eGearState_Default;
	angularVelocity = 0.0;
	gearSlot = NULL;

}

Gear::~Gear()
{
	normal = NULL;
	shadow = NULL;
	gearSlot = NULL;
}

GearState Gear::GetState()
{
	return _state;
}

void Gear::SetState(GearState gearState)
{

	_state = gearState;
	switch (gearState)
	{
	case eGearState_Default:
		{
			normal->SetZ(Gear_Default_Z_Order);
			shadow->SetZ(Gear_Default_Z_Order - 1);
		}
		break;

	case eGearState_Dragging:
		{
			normal->SetZ(Gear_Default_Z_Order + 2);
			shadow->SetZ(Gear_Default_Z_Order + 1);
			Control::Audio->QuickLoadAndPlaySFX("s2pz1_GearTaking");
		}
		break;

	case eGearState_Fixed:
		{
			normal->SetZ(Gear_Slot_Z_Order);
			shadow->SetZ(Gear_Slot_Z_Order - 1);
			
		}
		break;

	case eGearState_Rotating:
		{
			normal->SetZ(Gear_Slot_Z_Order);
			shadow->SetZ(Gear_Slot_Z_Order - 1);
			if (gearSlot != NULL)
			{
				angularVelocity = angularSpeed * gearSlot->rotationDirection;
			}
		}
		break;

	default:
		break;
	}
}

int Gear::GetId()
{
	return _id;
}

void Gear::Update(float ds)
{
	if (shadow != NULL)
	{
		shadow->SetXY(normal->Pos.x, normal->Pos.y);
	}

	if (_state == eGearState_Rotating)
	{
		normal->SetRotate(normal->GetRotate() + angularVelocity);
		shadow->SetRotate(normal->GetRotate());
	}
}

#pragma endregion



#pragma region GEARSLOT

GearSlot::GearSlot()
{
	_gear = NULL;
	isSolved = false;
	isRotating = false;
}

GearSlot::~GearSlot()
{
	_gear = NULL;
}

void GearSlot::SetGear(Gear* gear)
{
	_gear = gear;
	if (gear != NULL)
	{
		if (gear->radius == radius)
		{
			isSolved = true;
		}
		else
		{
			isSolved = false;
		}
	}
	else
	{
		isSolved = false;
		isRotating = false;
	}

}

Gear * GearSlot::GetGear()
{
	return _gear;
}  

#pragma endregion

