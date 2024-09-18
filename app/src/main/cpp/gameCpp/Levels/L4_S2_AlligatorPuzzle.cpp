//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 05 : MOOPAANS DEN
//====================================

#include "L4_S2_AlligatorPuzzle.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "TextureFactory.h"
#include "ParticleEffect.h"
#include "Polygon.h"
#include "Application.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "MeshObject.h"
#define NUM_AlLIGATORS 19


const CRectangle  AlligatorRect(589.0f,261.0f,188.0f,163.0f);



#pragma region ALLIGATOR
Alligator::Alligator(int id)
{
	_id = id;
	_state = eAlligatorState_Closed;
	normalSymbol = NULL;
	closedMouth = NULL;
	rightSymbol = NULL;
	wrongSymbol = NULL;
	openedMouth = NULL;
	waterSplash = NULL;
	waterRipple = NULL;
}

Alligator::~Alligator()
{

}

void Alligator::SetState(AlligatorState state)
{


	_state = state;
	//closedMouth->Show(false);
	openedMouth->Show(false);
	normalSymbol->Show(false);
	if (rightSymbol)
	{
		rightSymbol->Show(false);
	}

	if (wrongSymbol)
	{
		wrongSymbol->Show(false);
	}


	switch (state)
	{
	case eAlligatorState_Closed:
		{
			closedMouth->Show(true);

		}
		break;
	case eAlligatorState_Open:
		{
			openedMouth->Show(true);
			normalSymbol->Show(true);

		}
		break;
	case eAlligatorState_Right:
		{
			openedMouth->Show(true);
			rightSymbol->Show(true);
			closedMouth->SetIsInteractionEnabled(false);
		}

		break;
	case eAlligatorState_Wrong:
		{
			openedMouth->Show(true);
			wrongSymbol->Show(true);
		}

		break;
	default:
		break;
	} 


}

AlligatorState Alligator::GetState()
{
	return _state;
}  

int Alligator::GetId()
{
	return _id;
}

void Alligator::SetXY( float x, float y )
{
	closedMouth->SetXY(x, y);
	openedMouth->SetXY(x, y);
	waterSplash->SetXY(closedMouth->Pos.x + splashOffset.x, closedMouth->Pos.y + splashOffset.y);
	waterRipple->SetXY(closedMouth->Pos.x + rippleOffset.x, closedMouth->Pos.y + rippleOffset.y);
	if (_id == 9)//white alligator temp fix
	{
		normalSymbol->SetXY(closedMouth->Pos.x, closedMouth->Pos.y);
		openedMouth->SetXY(x - 15, y);
	}
	else
	{
		normalSymbol->SetXY(closedMouth->Pos.x + symbolOffset.x, closedMouth->Pos.y + symbolOffset.y);
	}

	if (rightSymbol)
	{
		rightSymbol->SetXY(normalSymbol->Pos.x, normalSymbol->Pos.y);
	}

	if (wrongSymbol)
	{
		wrongSymbol->SetXY(normalSymbol->Pos.x, normalSymbol->Pos.y);
	}

}

void Alligator::PlayWaterSplashAnim()
{
	waterSplash->SetEnable(true);
	waterSplash->ForcePlayAnimation(0);
}
#pragma endregion

#pragma region ALLIGATOR_PUZZLE
L4_S2_AlligatorPuzzle::L4_S2_AlligatorPuzzle()
{
	_isMouseInteractionEnabled = true;
	_prevSelectedAlligator = NULL;
	_currentSelectedAlligator = NULL;
	_whiteAlligator = NULL;
	isWaitingForPuzzleSolved = false;
	LoadHud(INVENTORY_V1);
	LoadScene("LV4\\S2\\ALLIGATORPUZZLE\\ALLIGATORPUZZLE.SCN");
	

	ResetBtn = new CObject();
	ResetBtn->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\RESETBTN.SPR", HOScene->GetObjectByID("ResetBtn")->Pos.x, HOScene->GetObjectByID("ResetBtn")->Pos.y, HOScene->GetObjectByID("ResetBtn")->ZOrder);
	ResetBtn->SetUID("resetBtn");
	HOScene->PushObject(ResetBtn);

	ResetBtn->SetEnable(false);
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		ResetBtn->SetEnable(false);
	
	timer = 10.0f;
	isFadeIn = false;
	isFadeOut = false;
	isResetDiaFadeIn = false;
	isResetDiaFadeOut = true;

	ResetDia = new CHOScene();
	ResetDia->Load("UI\\RESETDIA.SCN");

	CObject* resetText = new CObject();
	resetText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\RESETCNF.SPR", 670, 308, 2);
	resetText->SetUID("resetText");
	ResetDia->PushObject(resetText);

	CObject* mask = new CObject();
	mask->LoadInit("UI\\MASK.SPR", 683, 384, -1);
	mask->SetUID("mask");
	mask->SetScale(2);
	ResetDia->PushObject(mask);

	CObject* btnText = new CObject();
	btnText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\YES.SPR", 590, 420, 5);
	btnText->SetUID("yesBTNText");
	ResetDia->PushObject(btnText);

	btnText = new CObject();
	btnText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\NO.SPR", 780, 420, 5);
	btnText->SetUID("noBTNText");
	ResetDia->PushObject(btnText);

	ResetDia->SetZOff(eZOrder_Popup);
	ResetDia->Show(false);
	IsResetPop = false;

	_waterMeshObject = NULL;
	_waterMeshObject = new MeshObject(HOScene->GetObjectByID("Bg"), 10, 10);
	_waterMeshObject->meshFX->SetSpeedAndDisplacement(5, 5, 5, 5);
	//waterMeshObject->SetEnable(false);
	InitializeWithProfileData();
	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL4s2AlligatorPuzzleAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL4s2AlligatorPuzzleAmb) )		
	{
		Control::Audio->PlaySample(aAmbL4s2AlligatorPuzzleAmb,true);
	}
	
	//MUSIC_SFX
}
L4_S2_AlligatorPuzzle::~L4_S2_AlligatorPuzzle()
{
	for (int i = 0; i < (int)_arrayOfAlligators.size(); i++)
	{
		SAFE_DELETE(_arrayOfAlligators.at(i));
	}
	_arrayOfAlligators.clear();
	SAFE_DELETE(_waterMeshObject);
	SAFE_DELETE(ResetDia);
	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL4s2AlligatorPuzzleAmb);
	Control::Audio->UnloadSample(aAmbL4s2AlligatorPuzzleAmb);
	//MUSIC_SFX
}

void L4_S2_AlligatorPuzzle::InitializeWithProfileData()
{

	// CObject * closedMouth = HOScene->GetObjectByID("ClosedHead_0");  // Sac: Commenting Unused variable
	// CObject * normalSymbol = HOScene->GetObjectByID("NormalAnim_0");  // Sac: Commenting Unused variable
	CPoint symbolOffset;
	symbolOffset.x = -35;
	symbolOffset.y = -45;

	CPoint rippleOffset;
	rippleOffset.x = -135;
	rippleOffset.y = -105;

	CPoint splashOffset;
	splashOffset.x = -135;
	splashOffset.y = -140;

	int symbolIds[19] = {0,3,4,5,2,6,3,0,1,4,5,1,6,8,7,7,2,8,9};



	for (int i = 0; i < NUM_AlLIGATORS; i++)
	{
		if (i == 18)
		{
			_whiteAlligator = new Alligator(symbolIds[i]);
			_whiteAlligator->symbolOffset = symbolOffset;
			_whiteAlligator->rippleOffset = rippleOffset;
			_whiteAlligator->splashOffset = splashOffset;
			_whiteAlligator->openedMouth = HOScene->GetObjectByID("OpenedWhiteHead");

			_whiteAlligator->closedMouth = HOScene->GetObjectByID("ClosedWhiteHead");





			_whiteAlligator->normalSymbol = HOScene->GetObjectByID("MortarHandle");
			_whiteAlligator->closedMouth->SetMouseClickCallback(CC_CALLBACK_1(L4_S2_AlligatorPuzzle::OnMortarClick, this), _whiteAlligator->normalSymbol);
			_arrayOfInteractiveObjects.push_back(_whiteAlligator->closedMouth);
			_whiteAlligator->normalSymbol->Paused = true;
			//_whiteAlligator->normalSymbol->SetFrame(symbolIds[i]);

			_whiteAlligator->waterSplash = HOScene->GetObjectByID("WaterSplash");
			_whiteAlligator->waterSplash->SetScale(2);
			_whiteAlligator->waterSplash->SetEnable(false);
			_whiteAlligator->waterSplash->SetZ(200);

			_whiteAlligator->waterRipple = HOScene->GetObjectByID("RippleClose");
			_whiteAlligator->waterRipple->SetScale(2);
			int frame = rand() % 24;
			_whiteAlligator->waterRipple->SetFrame(frame);
			_whiteAlligator->waterRipple->PlayAnimation(0);
			_whiteAlligator->waterRipple->SetZ(1);

			_whiteAlligator->SetState(eAlligatorState_Closed);
			_whiteAlligator->SetXY(_whiteAlligator->closedMouth->Pos.x, _whiteAlligator->closedMouth->Pos.y);



			if (ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleState < eAlligatorPuzzleState_AlligatorHeadPlaced)
			{
				_whiteAlligator->closedMouth->Show(false);
				_isMouseInteractionEnabled = false;
				_whiteAlligator->waterRipple->Show(false);
				Hud->InvBox_StopPuzzleMode();
			}
			else
			{
				Hud->InvBox_StartPuzzleMode();
			}

			if (ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleState == eAlligatorPuzzleState_Solved)
			{
				_whiteAlligator->SetState(eAlligatorState_Open);
				Hud->InvBox_StopPuzzleMode();
			}
		}
		else
		{

			Alligator * alligator = new Alligator(symbolIds[i]);

			alligator->symbolOffset = symbolOffset;
			alligator->rippleOffset = rippleOffset;
			alligator->splashOffset = splashOffset;

			std::string closedMouthId = std::string("ClosedHead_")+MKSTR(i);
			alligator->closedMouth = HOScene->GetObjectByID(closedMouthId);
			alligator->closedMouth->SetMouseClickProperties(70, CC_CALLBACK_1(L4_S2_AlligatorPuzzle::OnMouseClickOnAlligator, this), alligator);
			_arrayOfInteractiveObjects.push_back(alligator->closedMouth);

			std::string openedMouthId = std::string("OpenedHead_")+MKSTR(i);
			alligator->openedMouth = HOScene->GetObjectByID(openedMouthId);

			std::string normalSymbolId = std::string("NormalAnim_")+MKSTR(i);
			alligator->normalSymbol = HOScene->GetObjectByID(normalSymbolId);
			alligator->normalSymbol->Paused = true;
			alligator->normalSymbol->SetFrame(symbolIds[i]);

			std::string rightSymbolId = std::string("BlueAnim_")+MKSTR(i);
			alligator->rightSymbol = HOScene->GetObjectByID(rightSymbolId);
			alligator->rightSymbol->Paused = true;
			alligator->rightSymbol->SetFrame(symbolIds[i]);

			std::string wrongSymbolId = std::string("OrangeAnim_")+MKSTR(i);
			alligator->wrongSymbol = HOScene->GetObjectByID(wrongSymbolId);
			alligator->wrongSymbol->Paused = true;
			alligator->wrongSymbol->SetFrame(symbolIds[i]);

			std::string waterSplashId = std::string("WaterSplash_")+MKSTR(i);
			alligator->waterSplash = HOScene->GetObjectByID(waterSplashId);
			alligator->waterSplash->SetScale(2);
			alligator->waterSplash->SetEnable(false);
			alligator->waterSplash->SetZ(200);

			std::string waterRippleId = std::string("RippleClose_")+MKSTR(i);
			alligator->waterRipple = HOScene->GetObjectByID(waterRippleId);
			alligator->waterRipple->SetScale(2);
			int frame = rand() % 24;
			alligator->waterRipple->SetFrame(frame);
			//alligator->waterRipple->PlayAnimation(0);
			alligator->waterRipple->SetZ(1);

			alligator->SetState(eAlligatorState_Closed);
			alligator->SetXY(alligator->closedMouth->Pos.x, alligator->closedMouth->Pos.y);
			if (ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleState == eAlligatorPuzzleState_Solved)
			{
				alligator->SetState(eAlligatorState_Right);
			}


			_arrayOfAlligators.push_back(alligator);
		}



	}
}
void L4_S2_AlligatorPuzzle::Update()
{
	
	NavigateToScene();
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}

	ResetPuzzle();
	if(IsResetPop)
	{
		GFHud->AllowInput = false;
		return;
	}
	else
		GFHud->AllowInput = true;

	if( isWaitingForPuzzleSolved )
	{
		Cursor::SetMode(CA_Normal);
		return;
	}


	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S2_RiverSide;
			SendMessage(ControlExit, ID, HOSM_NewArea);			
			return;			
		}	
	#endif	


	//if( IsMouseOverGoBackArea(&Control::Input->Mpos()))
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
	CPoint mousePos = Control::Input->Mpos();
	if( IsMouseOverGoBackArea(&mousePos))
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown, eArea_L4_S2_RiverSide);
#else
		Cursor::SetMode(CA_ExitDown, eArea_L4_S2_RiverSide);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L4_S2_RiverSide, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S2_RiverSide;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
			
			return;
		}	
		return;
	}
	else if(ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleState < eAlligatorPuzzleState_AlligatorHeadPlaced)
	{
		if (_whiteAlligator->closedMouth->GetWorldRect().Intersection(&mousePos))
		{
			Cursor::SetMode(CA_Gear);
		}
		else
		{
			Cursor::SetMode(CA_Normal);
		}
	}
	else
	{
		Cursor::SetMode(CA_Normal);
	}


	if(_isMouseInteractionEnabled)
	{
		if(_whiteAlligator->GetState() == eAlligatorState_Closed)
		{
			bool isCursorHandPointy = false;
			for (int i = 0; i < (int)(_arrayOfInteractiveObjects.size()); i++)
			{
				CObject * object = _arrayOfInteractiveObjects.at(i);
				if (object->GetCurrentMouseState() == eMouseState_Over || object->GetCurrentMouseState() == eMouseState_Down)
				{
					isCursorHandPointy = true;
					break;
				}
			}
			if (isCursorHandPointy)
			{
				Cursor::SetMode(CA_HandPointy);
			}
			else
			{
				Cursor::SetMode(CA_Normal);
			}
		}
		else
		{
			if (_whiteAlligator->closedMouth->GetCurrentMouseState() == eMouseState_Over)
			{
				Cursor::SetMode(CA_HandTake);
			}
			else
			{
				Cursor::SetMode(CA_Normal);
			}
		}

	}
}
void L4_S2_AlligatorPuzzle::Required()
{
	HiddenObjectSystem::Required();

	if( Control::Audio->IsPlaying(aTrackL4theme) )
	{
		Control::Audio->Samples[aTrackL4theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL4theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL4theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL4puzzle) )
				  Control::Audio->PlaySample(aTrackL4puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL4puzzle))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL4puzzle, true);
		}
	}
}

void L4_S2_AlligatorPuzzle::OnPuzzleSolved(UpdateListEntity *object)
{
	isWaitingForPuzzleSolved = false;
	AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	Control::Audio->QuickLoadAndPlaySFX("s2pz1_puzzleComplete");
	if (_handleExtrasGameComplete())
	{
		return;
	}
	ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleState = eAlligatorPuzzleState_Solved;
	ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleSolved = true;
	_whiteAlligator->SetState(eAlligatorState_Open);
	Hud->InvBox_StopPuzzleMode();
	
}

void L4_S2_AlligatorPuzzle::OnMouseClickOnAlligator(CObject * obj)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}

	if(IsResetPop)
		return;

	if (!_isMouseInteractionEnabled)
	{
		if (ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleState < eAlligatorPuzzleState_AlligatorHeadPlaced)
		{
			Hud->ShowBannerText("L4real");
		}
		return;
	}
	Alligator * alligator = (Alligator*)obj;
	if (alligator->GetState() == eAlligatorState_Closed)
	{

		_currentSelectedAlligator = alligator;
		if (_prevSelectedAlligator != NULL)
		{
			if (_prevSelectedAlligator->GetId() == _currentSelectedAlligator->GetId())
			{
				_prevSelectedAlligator->SetState(eAlligatorState_Right);
				_currentSelectedAlligator->SetState(eAlligatorState_Right);
				_prevSelectedAlligator->PlayWaterSplashAnim();
				_currentSelectedAlligator->PlayWaterSplashAnim();
				Control::Audio->QuickLoadAndPlaySFX("s2pz1_correctHead");
				_prevSelectedAlligator = NULL;
				_currentSelectedAlligator = NULL;
				if (IsPuzzleSolved())
				{
					CallFunction * callFn = new CallFunction(1, CC_CALLBACK_1(L4_S2_AlligatorPuzzle::OnPuzzleSolved, this), (UpdateListEntity*)this);
					callFn->Run();
				}
			}
			else
			{
				_prevSelectedAlligator->SetState(eAlligatorState_Wrong);
				_currentSelectedAlligator->SetState(eAlligatorState_Wrong);
				_currentSelectedAlligator->PlayWaterSplashAnim();
				Control::Audio->QuickLoadAndPlaySFX("s2pz1_wrongHead");
				CallFunction * callFn = new CallFunction(0.5, CC_CALLBACK_1(L4_S2_AlligatorPuzzle::CloseAlligatorMouths, this), (UpdateListEntity*)this);
				callFn->Run();
				_isMouseInteractionEnabled = false;
			}

		}
		else
		{
			alligator->SetState(eAlligatorState_Open);
			alligator->PlayWaterSplashAnim();
			_prevSelectedAlligator = alligator;
			Control::Audio->QuickLoadAndPlaySFX("s2pz1_firstHead");
		}

	}
}


void L4_S2_AlligatorPuzzle::OnMortarClick(CObject * object)
{

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	if (_whiteAlligator->GetState() != eAlligatorState_Closed && !Hud->IsDraggingItem())
	{
		Hud->TakeInventoryItem(eInv_L4_S2_GrinderHandle);
		_whiteAlligator->normalSymbol->Show(false);
		ProfileSystem::Instance->ActiveProfile.L4_S2_InvGrinderHandleTaken = true;
		_whiteAlligator->closedMouth->SetIsInteractionEnabled(false);
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S4_HillTop] = true;
		//ProfileSystem::Instance->ActiveProfile.L2_S5_EagleBlockPuzzleState
	}
	else if (ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleState == eAlligatorPuzzleState_AlligatorHeadPlaced)
	{
		Hud->ShowBannerText("L4shut");
	}
	else if (!Hud->IsDraggingItem() && ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleState < eAlligatorPuzzleState_AlligatorHeadPlaced)
	{
		Hud->ShowBannerText("L4slab");
	}

}

void L4_S2_AlligatorPuzzle::ReceiveMessage(int val)
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

			if (itemVal == eInv_L4_S5_AlligatorFace)
			{	
				Hud->ReleaseDrag();
				Hud->CloseInventory(); 

				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S5_AlligatorFace] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S5_AlligatorFace);
				ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleState = eAlligatorPuzzleState_AlligatorHeadPlaced;
				_isMouseInteractionEnabled = true;
				_whiteAlligator->closedMouth->Show(true);
				_whiteAlligator->waterRipple->Show(true);
				_isMouseInteractionEnabled = true;
				Hud->InvBox_StartPuzzleMode();
				Control::Audio->QuickLoadAndPlaySFX("s2pz1_placeAlligatorHead");

			}
			else
			{
				GFInstance->WrongInvDropMsg(Hud);
			}

		}
	}

	if( val == INV_CLOSE )
	{		
		if (ProfileSystem::Instance->ActiveProfile.L4_S2_InvGrinderHandleTaken)
		{		
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S2_RiverSide;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
	}	
}

void L4_S2_AlligatorPuzzle::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	if(ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S5_AlligatorFace] &&
		ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleState < eAlligatorPuzzleState_AlligatorHeadPlaced )
		isAnyTaskLeft = true;
	else if(ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleState >= eAlligatorPuzzleState_AlligatorHeadPlaced && 
		!ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleSolved ) 
		isAnyTaskLeft = true;
	else if (ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L4_S2_InvGrinderHandleTaken)
	{
		isAnyTaskLeft = true;
	}

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S1_SwampEntrance] = isAnyTaskLeft;
}


void L4_S2_AlligatorPuzzle::ProcessNonHOHint()
{
	int iNonHOHint = 0; 
	int MaxNonHOHints = 5;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
		case 0: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleState < eAlligatorPuzzleState_AlligatorHeadPlaced && 
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S5_AlligatorFace] )
			{
				Hud->ShowHintForInvItem( eInv_L4_S5_AlligatorFace, &AlligatorRect );
				return;
			}
			break;
		case 1:
			{
				++iNonHOHint;
				if (ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L4_S2_InvGrinderHandleTaken)
				{
					//InitHintTrailEmit(&_whiteAlligator->normalSymbol->GetWorldRect());
					// Linux: gives error of taking address of temporary also potential crash factor
					// below is fix
					CRectangle tempwhiteAlligatorRect = _whiteAlligator->normalSymbol->GetWorldRect();
					InitHintTrailEmit(&tempwhiteAlligatorRect);
					return;
				}
			}
			break;
		case 2: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S2_RiverSide])
			{
				InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
				return;
			}
			break;
		case 3: ++iNonHOHint;
			if(Hud->ShowHintForComboInvItems())
				return;
			break;
		case 4: ++iNonHOHint;
			Hud->teleportSystem->ShowTeleport();
			return;
			break;
		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();
}

void L4_S2_AlligatorPuzzle::CloseAlligatorMouths(UpdateListEntity * object)
{
	//TODO:sujin remove call after delay scheduler when exit
	_prevSelectedAlligator->SetState(eAlligatorState_Closed);
	_currentSelectedAlligator->SetState(eAlligatorState_Closed);
	_prevSelectedAlligator->PlayWaterSplashAnim();
	_currentSelectedAlligator->PlayWaterSplashAnim();
	_prevSelectedAlligator = NULL;
	_currentSelectedAlligator = NULL;
	_isMouseInteractionEnabled = true;
}

bool L4_S2_AlligatorPuzzle::IsPuzzleSolved()
{
	bool isSolved = true;
	for (int i = 0; i < (int)(_arrayOfAlligators.size()); i++)
	{
		Alligator * alligator = _arrayOfAlligators.at(i);
		if (alligator->GetState() != eAlligatorState_Right)
		{
			isSolved = false;
			break;
		}
	}

	return isSolved;
}

void L4_S2_AlligatorPuzzle::ProcessSkip()
{
	isWaitingForPuzzleSolved = true;
	for (int i = 0; i < (int)(_arrayOfAlligators.size()); i++)
	{
		Alligator * alligator = _arrayOfAlligators.at(i);
		if (alligator->GetState() != eAlligatorState_Right)
		{
			alligator->SetState(eAlligatorState_Right);
			alligator->PlayWaterSplashAnim();
		}
	}
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	CallFunction * callFn = new CallFunction(1, CC_CALLBACK_1(L4_S2_AlligatorPuzzle::OnPuzzleSolved, this), (UpdateListEntity*)this);
	callFn->Run();
}

void L4_S2_AlligatorPuzzle::ResetPuzzle()
{
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		return;
	if(ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleState != eAlligatorPuzzleState_AlligatorHeadPlaced)
	{
		ResetBtn->SetEnable(false);
		return;
	}
	ResetBtn->SetEnable(true);
	if(timer > 0)
	{
		timer -= (float(Control::MasterTmr->RealDMS) / 1000.0f);
		ResetBtn->SetEnable(false);
		if(!IsResetPop)
		{
			if(!isResetDiaFadeOut)
				ResetDia->FadeOut(2);
			isResetDiaFadeOut = true;
		}
		return;
	}
	else
	{
		ResetBtn->SetEnable(true);	
		if(!isFadeIn)
			ResetBtn->FadeIn(1);
		isFadeIn = true;
	}
	if(ResetBtn->IsIntersect(Control::Input->Mpos()))
	{
		ResetBtn->SetAnimation(1);
		if(!IsResetPop)
			Cursor::SetMode(CA_HandPointy);
		if(KInput::getLeftButtonState() == ISDOWN)
		{
			IsResetPop = true;
		}
	}
	else
	{
		ResetBtn->SetAnimation(2);
		Cursor::SetMode(CA_Normal);
	}

	if(IsResetPop)
	{
		if(!isResetDiaFadeIn)
		{
			ResetDia->FadeIn(2);
		}
		if(!isFadeOut)
			ResetBtn->FadeOut(1);
		isFadeOut = true;
		isResetDiaFadeIn = true;
		CObject *resetYes = ResetDia->GetObjectByID("BtnYes");
		CObject *resetNo = ResetDia->GetObjectByID("BtnNo");

		if(Hud->CheckPopupButton(resetYes) == Popup_Click)
		{
			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S2_PZAlligatorHead;
			SendMessage(ControlExit, ID, HOSM_NewArea);
			IsResetPop = false;
		}
		if(Hud->CheckPopupButton(resetNo) == Popup_Click)
		{
			IsResetPop = false;
			timer = 10.0f;
			isFadeIn = false;
			isFadeOut = false;
			isResetDiaFadeOut = false;
			isResetDiaFadeIn = false;
		}
	}
	else
	{
		ResetBtn->SetEnable(true);
	}
}


void L4_S2_AlligatorPuzzle::NavigateToScene()
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
#pragma endregion





