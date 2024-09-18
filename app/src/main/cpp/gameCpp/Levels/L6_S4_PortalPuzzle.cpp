
//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "L6_S4_PortalPuzzle.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "Util.h"

const CRectangle  NoteRect(478.0f,56.0f,418.0f,584.0f);

#define NUM_PUZZLE_OBJECTS 7
static float angles[NUM_PUZZLE_OBJECTS] = {(360.0 / 6) * 3,  (360.0 / 6) * 1, (360.0 / 6) * 0, (360.0 / 6) *2, (360.0 / 6) * 5, (360.0 / 6) * 6, (360.0 / 6) * 3};

L6_S4_PortalPuzzle::L6_S4_PortalPuzzle()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV6\\S4\\PZ1\\PZ1.SCN");

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

	_clockBtn = NULL;
	_antiClockBtn = NULL;
	_currentDraggingObject = NULL;
	_isMouseInteractionEnabled = true;

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L6_S4_PortalPuzzle]  = true;

	_currentSkipObjectIndex = 0;
	InitializeWithProfileData();
	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL6s4ProtalMiniGameAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL6s4ProtalMiniGameAmb) )		
	{
		Control::Audio->PlaySample(aAmbL6s4ProtalMiniGameAmb,true);
	}

	//MUSIC_SFX

	int ArrayPts[] = {556,235,618,508,819,458,760,185,553,237};
	CPolygon *CPolyObjName=  new CPolygon(5);
	CPolyObjName->Init(ArrayPts);
	popNote = new L6S4_PopAnathaNote(1, Hud, NULL, eZOrder_Popup, CPolyObjName->center,CC_CALLBACK_1(L6_S4_PortalPuzzle::OnPopupClosed,this));

	CObject* temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L6\\ANANTHATEXT.SPR", 683, 440, eZOrder_Popup + 2);
	temp->SetUID("letterText");
	temp->Show(false);
	popNote->PushObject(temp);

	popNote->SetPopupBoundingBox(NoteRect);
	delete(CPolyObjName);

	int ArrayPts1[] = {558,235,618,501,823,452,760,182,555,235};
	letterRect=  new CPolygon(5);
	letterRect->Init(ArrayPts1);

	activePopUpId = -1;
}

void L6_S4_PortalPuzzle::InitializeWithProfileData()
{
	for (int i = 0; i < NUM_PUZZLE_OBJECTS; i++)
	{

		std::string holderId = std::string("Holder_") + MKSTR(i);
		CObject * holder = HOScene->GetObjectByID(holderId);
		holder->SetRotate(angles[i]);
		if (!ProfileSystem::Instance->ActiveProfile.L6_S4_PuzzlePiecesPlaced)
		{
			holder->SetIsInteractionEnabled(true);
		}
		_arrayOfHolders.push_back(holder);


		std::string puzzleObjectId = std::string("TanSolved_") + MKSTR(i);
		CObject * puzzleObject = HOScene->GetObjectByID(puzzleObjectId);
		puzzleObject->SetRotate(angles[i]);
		_arrayOfPuzzleObjects.push_back(puzzleObject);
		puzzleObject->SetMouseClickCallback(CC_CALLBACK_1(L6_S4_PortalPuzzle::OnMouseClickOnObject, this));


		_arrayOfInteractiveObjects.push_back(puzzleObject);

		_arrayOfHolderPositions.push_back(puzzleObject->Pos);

		std::string solvedPzzleObjectId = std::string("Tan_") + MKSTR(i);
		CObject * solvedPzzleObject = HOScene->GetObjectByID(solvedPzzleObjectId);
		_arrayOfSolvedPuzzleObjects.push_back(solvedPzzleObject);
		solvedPzzleObject->Show(false);

		if (ProfileSystem::Instance->ActiveProfile.L6_S4_PortalPuzzleObjectStates[i])
		{
			puzzleObject->SetAlpha(0);
			puzzleObject->SetIsInteractionEnabled(false);
			solvedPzzleObject->Show(true);
		}
		if (!ProfileSystem::Instance->ActiveProfile.L6_S4_PuzzlePiecesPlaced)
		{
			puzzleObject->Show(false);
			puzzleObject->SetIsInteractionEnabled(false);
			Hud->InvBox_StopPuzzleMode();
		}
		else
		{
			Hud->InvBox_StartPuzzleMode();
		}
	}

	_clockBtn = new	UIButton( HOScene->GetObjectByID("ClockBtn"), CC_CALLBACK_1(L6_S4_PortalPuzzle::OnRotateClockBtnClick, this)); 
	_arrayOfInteractiveObjects.push_back(_clockBtn);
	_antiClockBtn = new	UIButton( HOScene->GetObjectByID("CounterBtn"), CC_CALLBACK_1(L6_S4_PortalPuzzle::OnRotateAntiClockBtnClick, this)); 
	_arrayOfInteractiveObjects.push_back(_antiClockBtn);

	_glow = HOScene->GetObjectByID("FinalGlow");
	_glow->SetMouseClickCallback(CC_CALLBACK_1(L6_S4_PortalPuzzle::OnRotateAntiClockBtnClick, this));
	_glow->SetAlpha(0);

	if (ProfileSystem::Instance->ActiveProfile.L6_S4_PuzzlePiecesPlaced)
	{
		_glow->SetIsInteractionEnabled(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S4_LetterFromAnanthaTaken)
	{
		HOScene->GetObjectByID("Letter")->SetEnable(false);
	}
}

void L6_S4_PortalPuzzle::OnPuzzleComplete()
{
	AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	float tweenVals[5] = {1, -1, -1, -1, -1};
	_glow->TweenTo(tweenVals, 2.5, eEase_QUADIN, true, CC_CALLBACK_1(L6_S4_PortalPuzzle::OnGlowAnimComplete, this));
	Control::Audio->QuickLoadAndPlaySFX("s4pz1_puzzleComplete");

	//Show Objective completed
	if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L6O1_FindWayOutOfNetherworld] && 
		!ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L6O1_FindWayOutOfNetherworld] )
	{
		ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L6O1_FindWayOutOfNetherworld] = true;
		Hud->ShowObjectiveFX(eObj_L6O1_FindWayOutOfNetherworld,true);
	}
}

void L6_S4_PortalPuzzle::OnGlowAnimComplete(CObject * object)
{
	if (_handleExtrasGameComplete())
	{
		return;
	}
	ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S4_PortalChest;
	SendMessage(ControlExit, ID, HOSM_NewArea);
	return;
}

bool L6_S4_PortalPuzzle::CheckIfPuzzleSolved()
{
	bool isSolved = true;
	for (int i = 0; i < NUM_PUZZLE_OBJECTS; i++)
	{
		if (!_arrayOfSolvedPuzzleObjects.at(i)->Visible)
		{
			isSolved = false;
			break;
		}
	}
	return isSolved;
}

void L6_S4_PortalPuzzle::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Control::Audio->IsPlaying(aTrackL6theme) )
	{
		Control::Audio->Samples[aTrackL6theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL6theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL6theme);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL6puzzle) )
				Control::Audio->PlaySample(aTrackL6puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL6puzzle))
		{
			Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL6puzzle, true);
		}
	}
}

void L6_S4_PortalPuzzle::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive || Hud->IsDraggingItem())
		return;
	if (activePopUpId != -1)
		return;

	ResetPuzzle();
	if(IsResetPop)
	{
		GFHud->AllowInput = false;
		return;
	}
	else
		GFHud->AllowInput = true;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
		Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S4_PortalChest;
		SendMessage(ControlExit, ID, HOSM_NewArea);								
	}	
	#endif	

	if( IsMouseOverGoBackArea(&pos) )
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown, eArea_L6_S4_FloatingIsland4);
#else
		Cursor::SetMode(CA_ExitDown, eArea_L6_S4_FloatingIsland4);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L6_S4_PortalChest, 2);
			}
#else			
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S4_PortalChest;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
								
		}	
	}
	else if (letterRect->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S4_LetterFromAnanthaTaken)
	{
		Cursor::SetMode(CA_Look);
		if (Control::Input->LBclicked())
		{
			popNote->OpenPopup();
			activePopUpId = 1;
			HOScene->GetObjectByID("Letter")->SetEnable(false);
		}
		return;
	}
	else
	{
		if (_currentDraggingObject)
		{
			Cursor::SetMode(CA_Normal);
			Cursor::Show(false);
			_currentDraggingObject->SetXY(pos.x, pos.y);
		}
		else
		{
			Cursor::Show(true);
		}

		if (_isMouseInteractionEnabled)
		{
			bool isCursorHandPointy = false;
			for (int i = 0; i < (int)(_arrayOfInteractiveObjects.size()); i++)
			{
				CObject * object = _arrayOfInteractiveObjects.at(i);
				if (object->GetCurrentMouseState() == eMouseState_Over || object->GetCurrentMouseState() == eMouseState_Down)
				{
					if (object->pSprite->Alpha > 0)
					{
						isCursorHandPointy = true;
						break;
					}

				}
			}
			if (isCursorHandPointy)
			{
				Cursor::SetMode(CA_HandPointy);
			}

			if (Control::Input->LBclicked())
			{
				if (!ProfileSystem::Instance->ActiveProfile.L6_S4_PuzzlePiecesPlaced && !Hud->IsDraggingItem() && ProfileSystem::Instance->ActiveProfile.L6_S4_LetterFromAnanthaTaken)
				{
					Hud->ShowBannerText("L6puzzle");
				}
			}

			if (!ProfileSystem::Instance->ActiveProfile.L6_S4_PuzzlePiecesPlaced)
			{
				bool isCursorGear = false;
				for (int i = 0; i < (int)(_arrayOfHolders.size()); i++)
				{
					CObject * object = _arrayOfHolders.at(i);
					if (object->GetCurrentMouseState() == eMouseState_Over || object->GetCurrentMouseState() == eMouseState_Down)
					{

						isCursorGear = true;
						break;
					}
				}

				if (isCursorGear || _glow->GetCurrentMouseState() == eMouseState_Over || _glow->GetCurrentMouseState() == eMouseState_Down)
				{
					Cursor::SetMode(CA_Gear);
				}
			}
		}
	}


}

void L6_S4_PortalPuzzle::OnMouseClickOnObject(CObject * object)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	if (!_isMouseInteractionEnabled)
	{
		return;
	}
	if (_currentDraggingObject == NULL)
	{
		_currentDraggingObject = object;
		_currentDraggingObject->SetScale(1.1f);
	}
	else
	{
		float normalizedAngle = (float)Elixir::NormalizeAngle(_currentDraggingObject->GetRotate());
		if (_currentDraggingObject == _arrayOfPuzzleObjects.at(1))//neck piece
		{
			if (normalizedAngle == 180.0f)
			{
				normalizedAngle = 0.0f;
			}
		}

		if (normalizedAngle == 0.0f || normalizedAngle == 360.0f)
		{
			CObject * solvedPuzzleObject = GetSolvedPuzzleObjectForPosition(&_currentDraggingObject->Pos);
			if (solvedPuzzleObject)
			{
				solvedPuzzleObject->Show(true);
				_currentDraggingObject->SetAlpha(0);
				_currentDraggingObject->SetIsInteractionEnabled(false);
				Control::Audio->QuickLoadAndPlaySFX("s4pz1_placeTanCorrect");
				for (int i = 0; i < NUM_PUZZLE_OBJECTS; i++)
				{
					if (_currentDraggingObject == _arrayOfPuzzleObjects.at(i))
					{
						ProfileSystem::Instance->ActiveProfile.L6_S4_PortalPuzzleObjectStates[i] =  true;
						break;
					}
				}

				bool solved = CheckIfPuzzleSolved();
				if (solved)
				{
					ProfileSystem::Instance->ActiveProfile.L6_S4_PuzzleComplete = true;
					OnPuzzleComplete();
				}

			}
			else
			{
				ReturnDraggingObject();
			}
		}
		else
		{
			ReturnDraggingObject();
		}
		_currentDraggingObject = NULL;

	}
}

void L6_S4_PortalPuzzle::ReturnDraggingObject()
{
	for (int i = 0; i < NUM_PUZZLE_OBJECTS; i++)
	{
		if (_currentDraggingObject == _arrayOfPuzzleObjects.at(i))
		{
			CPoint  position = _arrayOfHolderPositions.at(i);
			float tweenVals[5] = {-1, position.x, position.y, -1, -1};
			_currentDraggingObject->TweenTo(tweenVals, 0.5, eEase_QUADIN, true, CC_CALLBACK_1(L6_S4_PortalPuzzle::OnReturnDraggingObjectComplete, this));
			_isMouseInteractionEnabled = false;
			break;
		}
	}
	_currentDraggingObject->SetScale(1);
	_currentDraggingObject = NULL;
	Control::Audio->QuickLoadAndPlaySFX("s4pz1_placeTanIncorrect");

}

void L6_S4_PortalPuzzle::OnReturnDraggingObjectComplete(CObject * object)
{
	_isMouseInteractionEnabled = true;
}

CObject * L6_S4_PortalPuzzle::GetSolvedPuzzleObjectForPosition(CPoint * position)
{
	CObject* returnObject = NULL;
	for (int i = 0; i < NUM_PUZZLE_OBJECTS; i++)
	{
		if (_currentDraggingObject == _arrayOfPuzzleObjects.at(i))
		{
			CObject * solvedPuzzleObject =  _arrayOfSolvedPuzzleObjects.at(i);
			float distance = fabsf(Elixir::Distance(&solvedPuzzleObject->Pos, position));
			if (distance < 50)
			{
				returnObject = solvedPuzzleObject;
				break;
			}
		}

	}
	return returnObject;
}

void L6_S4_PortalPuzzle::OnRotateClockBtnClick(CObject * object)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive || Hud->IsDraggingItem())
	{
		return;
	}
	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_LetterFromAnanthaTaken)
	{
		return;
	}
	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_PuzzlePiecesPlaced)
	{
		Hud->ShowBannerText("L6puzzle");
		return;
	}
	if (!_isMouseInteractionEnabled)
	{
		return;
	}
	_isMouseInteractionEnabled = false;
	for (int i = 0; i < NUM_PUZZLE_OBJECTS; i++)
	{
		CObject * puzzleObject = _arrayOfPuzzleObjects.at(i);
		CObject * holder = _arrayOfHolders.at(i);
		float rotateTo = puzzleObject->GetRotate() - 360.0f / 6;
		float tweenVals[5] = {-1, -1, -1, -1, rotateTo};
		if (i == NUM_PUZZLE_OBJECTS -1)
		{
			puzzleObject->TweenTo(tweenVals, 0.5, eEase_BACKOUT, true, CC_CALLBACK_1(L6_S4_PortalPuzzle::OnRotateComplete, this));
		}
		else
		{
			puzzleObject->TweenTo(tweenVals, 0.5, eEase_BACKOUT, true);
		}

		holder->TweenTo(tweenVals, 0.4f, eEase_BACKOUT, true);

	}
	Control::Audio->QuickLoadAndPlaySFX("s4pz1_rotateTangram");
}
void L6_S4_PortalPuzzle::OnRotateAntiClockBtnClick(CObject * object)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive || Hud->IsDraggingItem())
	{
		return;
	}
	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_LetterFromAnanthaTaken)
	{
		return;
	}
	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_PuzzlePiecesPlaced)
	{
		Hud->ShowBannerText("L6puzzle");
		return;
	}

	if (!_isMouseInteractionEnabled)
	{
		return;
	}
	_isMouseInteractionEnabled = false;
	for (int i = 0; i < NUM_PUZZLE_OBJECTS; i++)
	{
		CObject * puzzleObject = _arrayOfPuzzleObjects.at(i);
		CObject * holder = _arrayOfHolders.at(i);
		float rotateTo = puzzleObject->GetRotate() + 360.0f / 6;
		float tweenVals[5] = {-1, -1, -1, -1, rotateTo};
		if (i == NUM_PUZZLE_OBJECTS -1)
		{
			puzzleObject->TweenTo(tweenVals, 0.5, eEase_BACKOUT, true, CC_CALLBACK_1(L6_S4_PortalPuzzle::OnRotateComplete, this));
		}
		else
		{
			puzzleObject->TweenTo(tweenVals, 0.5, eEase_BACKOUT, true);
		}

		holder->TweenTo(tweenVals, 0.4f, eEase_BACKOUT, true);

	}
	Control::Audio->QuickLoadAndPlaySFX("s4pz1_rotateTangram");
}

void L6_S4_PortalPuzzle::OnRotateComplete(CObject * object)
{
	_isMouseInteractionEnabled = true;
}

void L6_S4_PortalPuzzle::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		Hud->ShowMap();
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			if (itemVal == eInv_L6_S2_PuzzlePices && ProfileSystem::Instance->ActiveProfile.L6_S4_LetterFromAnanthaTaken)
			{				
				Hud->ReleaseDrag();
				Hud->CloseInventory(); 

				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S2_PuzzlePices] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S2_PuzzlePices);
				ProfileSystem::Instance->ActiveProfile.L6_S4_PuzzlePiecesPlaced = true;
				_glow->SetIsInteractionEnabled(false);
				_isMouseInteractionEnabled = true;
				for (int i = 0; i < (int)_arrayOfPuzzleObjects.size(); i++)
				{
					CObject * puzzleObject = _arrayOfPuzzleObjects.at(i);
					puzzleObject->Show(true);
					puzzleObject->SetIsInteractionEnabled(true);
				}
				Hud->InvBox_StartPuzzleMode();
			}
			else
			{
				GFInstance->WrongInvDropMsg(Hud);
			}
		}
	}
	else if( val == INV_CLOSE )
	{		


	}
}

void L6_S4_PortalPuzzle::ProcessSkip()
{
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	for (int i = 0; i < NUM_PUZZLE_OBJECTS; i++)
	{
		_currentSkipObjectIndex = i;
		CObject * puzzleObject = _arrayOfPuzzleObjects.at(_currentSkipObjectIndex);
        //	CObject * holder = _arrayOfHolders.at(_currentSkipObjectIndex); //Sac: Unused variable, so commented it
		CObject * solvedPuzzleObject = _arrayOfSolvedPuzzleObjects.at(_currentSkipObjectIndex);
		if (!solvedPuzzleObject->Visible)
		{
			float tweenVals[5] = {-1, solvedPuzzleObject->Pos.x, solvedPuzzleObject->Pos.y, -1, 0};
			puzzleObject->TweenTo(tweenVals, 0.3f, eEase_QUADIN, true, CC_CALLBACK_1(L6_S4_PortalPuzzle::OnSkipTweenComplete, this));
			break;
		}
	}

}

void L6_S4_PortalPuzzle::OnPopupClosed(Popup * popup)
{
	ProfileSystem::Instance->ActiveProfile.L6_S4_LetterFromAnanthaTaken = true;

	//Journal Entry Add
	if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P7_AnanthaNote])
	{
		//Flag Set
		ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P7_AnanthaNote] = true;

		//Add to Note Array
		ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L6P7_AnanthaNote;

		ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

		//Increment Notes
		ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
		Hud->ShowJrnEntryFX();
	}

	Hud->ShowBannerText("L6spell");
	activePopUpId = -1;
}

void L6_S4_PortalPuzzle::OnSkipTweenComplete(CObject * object)
{
	ProfileSystem::Instance->ActiveProfile.L6_S4_PortalPuzzleObjectStates[_currentSkipObjectIndex] =  true;
	object->Visible = false;
	_arrayOfSolvedPuzzleObjects.at(_currentSkipObjectIndex)->Visible = true;
	_currentSkipObjectIndex++;

	bool solved = CheckIfPuzzleSolved();
	if (solved)
	{
		ProfileSystem::Instance->ActiveProfile.L6_S4_PuzzleComplete = true;
		OnPuzzleComplete();
	}
	else
	{
		for (; _currentSkipObjectIndex < NUM_PUZZLE_OBJECTS; _currentSkipObjectIndex++)
		{
			CObject * puzzleObject = _arrayOfPuzzleObjects.at(_currentSkipObjectIndex);
            //	CObject * holder = _arrayOfHolders.at(_currentSkipObjectIndex); // Sac: Commenting Unused variable
			CObject * solvedPuzzleObject = _arrayOfSolvedPuzzleObjects.at(_currentSkipObjectIndex);
			if (!solvedPuzzleObject->Visible)
			{
				float tweenVals[5] = {-1, solvedPuzzleObject->Pos.x, solvedPuzzleObject->Pos.y, -1, 0};
				puzzleObject->TweenTo(tweenVals, 0.3f, eEase_QUADIN, true, CC_CALLBACK_1(L6_S4_PortalPuzzle::OnSkipTweenComplete, this));
				break;
			}
		}
	}


}

void L6_S4_PortalPuzzle::ProcessNonHOHint()
{
	int iNonHOHint = 0; 
	int MaxNonHOHints = 8;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
			break;

		case 0: ++iNonHOHint;
			if (!ProfileSystem::Instance->ActiveProfile.L6_S4_LetterFromAnanthaTaken)
			{
				//InitHintTrailEmit(&HOScene->GetObjectByID("Letter")->GetWorldRect());
				// Linux: gives error of taking address of temporary also potential crash factor
				// below is fix
				CRectangle letterObjRect =HOScene->GetObjectByID("Letter")->GetWorldRect();
				InitHintTrailEmit(&letterObjRect);
				return;
			}
			break;
		case 1: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L6_S2_InvPuzzlePiecesCollected && !ProfileSystem::Instance->ActiveProfile.L6_S4_PuzzlePiecesPlaced)
			{
				//Hud->ShowHintForInvItem(eInv_L6_S2_PuzzlePices,&HOScene->GetObjectByID("Letter")->GetWorldRect());
				// Linux: gives error of taking address of temporary also potential crash factor
				// below is fix
				CRectangle letterObjRect =HOScene->GetObjectByID("Letter")->GetWorldRect();
				Hud->ShowHintForInvItem(eInv_L6_S2_PuzzlePices,&letterObjRect);
				return;
			}
			break;
		case 2: ++iNonHOHint;

			break;
		case 3: ++iNonHOHint;

			break;
		case 4: ++iNonHOHint;

			break;
		case 5: ++iNonHOHint;

			break;
		case 6: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
				break;
		case 7: ++iNonHOHint;
				{
					Hud->teleportSystem->ShowTeleport();
					return;
				}
				break;

		}
	}
	while( iNonHOHint < MaxNonHOHints );

	Hud->ShowNothingToDoMsgBanner();
}

L6_S4_PortalPuzzle::~L6_S4_PortalPuzzle()
{
	Hud->InvBox_StopPuzzleMode();
	_arrayOfHolderPositions.clear();
	_arrayOfInteractiveObjects.clear();
	_arrayOfPuzzleObjects.clear();
	_arrayOfSolvedPuzzleObjects.clear();
	_arrayOfHolders.clear();
	_currentDraggingObject = NULL;
	SAFE_DELETE(_clockBtn);
	SAFE_DELETE(_antiClockBtn);
	SAFE_DELETE(popNote);
	SAFE_DELETE(letterRect);
	SAFE_DELETE(ResetDia);
	//MUSIC_SFX

	Control::Audio->StopSample(aAmbL6s4ProtalMiniGameAmb);
	Control::Audio->UnloadSample(aAmbL6s4ProtalMiniGameAmb);
	//MUSIC_SFX
}

void L6_S4_PortalPuzzle::ResetPuzzle()
{
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L6_S4_PuzzlePiecesPlaced)
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
			for (int i = 0; i < 7; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L6_S4_PortalPuzzleObjectStates[i] = false;
			}
			ProfileSystem::Instance->ActiveProfile.L6_S4_PuzzleComplete = false;

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S4_PortalPuzzle;
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
void L6_S4_PortalPuzzle::NavigateToScene()
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