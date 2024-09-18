//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 05 : MOOPAANS DEN
//====================================

#include "L2_S4_DeitiesPuzzle.h"
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
#include "exMath.h"
#include "InteractiveObjectList.h"

using namespace gridpuzzle;
#define NUM_ROWS 3
#define NUM_COLS 3

#pragma region DEITIES_PUZZLE_OBJECT



DeitiesPuzzleObject::DeitiesPuzzleObject(CObject * mouseOverGlow, CObject * selectedGlow, CObject * movableTileGlow, int gridIndex, bool isMovable): gridpuzzle::GridPuzzleObject(gridIndex, isMovable)
{
	_mouseOverGlow = mouseOverGlow;
	_mouseOverGlow->Show(false);

	_movableTileGlow = movableTileGlow;
	_movableTileGlow->Show(false);

	_selectedGlow = selectedGlow;
	_selectedGlow->Show(false);
	_puzzleCompleteAnimCallback = nullptr;
}

DeitiesPuzzleObject::~DeitiesPuzzleObject()
{
	SAFE_DELETE(_mouseOverGlow);
	SAFE_DELETE(_selectedGlow);
	SAFE_DELETE(_movableTileGlow);
	_puzzleCompleteAnimCallback = nullptr;
}

void DeitiesPuzzleObject::Update(float ds)
{
	CObject::Update(ds);
	_mouseOverGlow->SetXY(Pos.x, Pos.y);
	_selectedGlow->SetXY(Pos.x, Pos.y);
	_movableTileGlow->SetXY(Pos.x, Pos.y);
}

void DeitiesPuzzleObject::OnMouseOver()
{
	if (!_isMovable)
	{
		return;
	}
	CObject::OnMouseOver();
	_mouseOverGlow->Show(true);
}
 void DeitiesPuzzleObject::OnMouseOut()
{
	if (!_isMovable)
	{
		return;
	}
	CObject::OnMouseOut();
	_mouseOverGlow->Show(false);
}

 void DeitiesPuzzleObject::SetIsSelected(bool isSelected)
 {
	 _isSelected = isSelected;
	 if (_isSelected)
	 {
		 _selectedGlow->Show(true);
		//SetScale(1.05);
	 }
	 else
	 {
		 //SetScale(1);
		 _selectedGlow->Show(false);
	 }
 }

 bool DeitiesPuzzleObject::GetIsSelected()
 {
	 return _isSelected;
 }

 void DeitiesPuzzleObject::StartPuzzleCompleteAnim(std::function<void (CObject*)> puzzleCompleteAnimCallback)
 {
		 _puzzleCompleteAnimCallback = puzzleCompleteAnimCallback;
		float valArr[5] = {2, -1, -1, -1, -1};
		_selectedGlow->Show(true);
		_selectedGlow->SetAlpha(0);
		_selectedGlow->TweenTo(valArr, 2, TW_EASEQUADOUT, true, CC_CALLBACK_1(DeitiesPuzzleObject::OnCompleteGlowUpTween, this));	
 }

 void DeitiesPuzzleObject::OnCompleteGlowUpTween(CObject * obj)
 {
	 _selectedGlow->Show(true);
	_selectedGlow->SetAlpha(2);
	 float valArr[5] = {0, -1, -1, -1, -1};
		_selectedGlow->TweenTo(valArr, 1, TW_EASEQUADOUT, true, _puzzleCompleteAnimCallback);
 }

 CObject * DeitiesPuzzleObject::GetMovableTileGlow()
 {
	 return _movableTileGlow;
 }
 

#pragma endregion



#pragma region L2_S4_DEITIES_PUZZLE

 const CRectangle  InvRect(541.0f,587.0f,358.0f,66.0f);

L2_S4_DeitiesPuzzle::L2_S4_DeitiesPuzzle()
{
	musicFadeOutTimer = 0;
	_isTweeningAnyObject = false;
	_isDoorOpenAnimPlaying = false;
	_swapObject1 = NULL;
	_swapObject2 = NULL;
	_tweenCompleteCounter = -1;
	LoadHud(INVENTORY_V1);
	LoadScene("LV2\\S4\\DEITIESPUZZLE\\DEITIESPUZZLE.SCN");
	if(ProfileSystem::Instance->ActiveProfile.L2_S4_DeitiesPuzzleState < eL2S4DeitiesPuzzleState_Solved)
	{
		Hud->InvBox_StartPuzzleMode();
	}
	InitializeWithProfileData();

	Control::Audio->LoadSample(aAmbs4Cave,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbs4Cave) )		
	{
		Control::Audio->PlaySample(aAmbs4Cave,true);
	}

	ResetBtn = new CObject();
	ResetBtn->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\RESETBTN.SPR", HOScene->GetObjectByID("ResetBtn")->Pos.x, HOScene->GetObjectByID("ResetBtn")->Pos.y, HOScene->GetObjectByID("ResetBtn")->ZOrder);
	ResetBtn->SetUID("resetBtn");
	HOScene->PushObject(ResetBtn);

	ResetBtn->SetEnable(false);
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		ResetBtn->SetEnable(false);

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

	timer = 10.0f;
	isFadeIn = false;
	isFadeOut = false;
	isResetDiaFadeIn = false;
	isResetDiaFadeOut = true;
}


L2_S4_DeitiesPuzzle::~L2_S4_DeitiesPuzzle()
{
	Hud->InvBox_StopPuzzleMode();
	_arrayOfPuzzleObjects.clear();

	SAFE_DELETE(ResetDia);

	//MUSIC_SFX

	Control::Audio->StopSample(aAmbs2MangoPuzzle);
	Control::Audio->UnloadSample(aAmbs2MangoPuzzle);
	//MUSIC_SFX
}

void L2_S4_DeitiesPuzzle::ProcessSkip()
{
	//SOLVED STATE SIMULATING STARTS....
	int bgZOrder = HOScene->GetObjectByID("Bg")->ZOrder;
	int solvedArray[MAX_L2_S4_DEITIESTILES] = {0,1,2,3,4,5,6,7,8};
	for (int i = 0; i < MAX_L2_S4_DEITIESTILES; i++)
	{
		ProfileSystem::Instance->ActiveProfile.L2_S4_DeitiesTileIndices[i] = solvedArray[i];
		CPoint  position = getPositionOfGrid(solvedArray[i]);
			position.z = (float)(bgZOrder + 1);
		
		DeitiesPuzzleObject* puzzleObject = _arrayOfPuzzleObjects.at(i);
		puzzleObject->InitPos = position;
		puzzleObject->Pos = position;
	}
	//SOLVED STATE SIMULATING ENDS....
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	onPuzzleSolved();
}

void L2_S4_DeitiesPuzzle::InitializeWithProfileData()
{
	_metalLever = HOScene->GetObjectByID("MetalLever");
	_metalLever->SetMouseClickCallback(CC_CALLBACK_1(L2_S4_DeitiesPuzzle::OnMouseClick, this));
	_metalLever->SetIsInteractionEnabled(false);
	_metalLever->Show(false);

	_door = HOScene->GetObjectByID("Door");
	_door->Show(false);


	CPoint origin;
	CPoint gridSize;
	origin.x = 330;
	origin.y = 32;
	gridSize.x = 238;
	gridSize.y = 183;

	init(NUM_ROWS, NUM_COLS, origin, gridSize);


	if (ProfileSystem::Instance->ActiveProfile.L2_S4_DeitiesTileIndices[0] == -1)
	{
		int tempArray[MAX_L2_S4_DEITIESTILES] = {6, 7, 8, 5, 0, 3, 2, 1, 4};
		for (int i = 0; i < MAX_L2_S4_DEITIESTILES; i++)
		{
			ProfileSystem::Instance->ActiveProfile.L2_S4_DeitiesTileIndices[i] = tempArray[i];
		}
	}

	int bgZOrder = HOScene->GetObjectByID("Bg")->ZOrder;
	for (int i = 0; i < MAX_L2_S4_DEITIESTILES; i++)
	{

		CObject * mouseOverGlow = new CObject();
		mouseOverGlow->LoadInit("LV2\\S4\\DEITIESPUZZLE\\MouseOver.SPR", 0, 0, bgZOrder + 2);
		mouseOverGlow->SetUID(std::string("MouseOver_") + MKSTR(i));

		CObject * selectedGlow = new CObject();
		selectedGlow->LoadInit("LV2\\S4\\DEITIESPUZZLE\\SelectedGlow.SPR", 0, 0, bgZOrder + 3);
		selectedGlow->SetUID(std::string("SelectedGlow_") + MKSTR(i));

		CObject * movableTileGlow = new CObject();
		movableTileGlow->LoadInit("LV2\\S4\\DEITIESPUZZLE\\MovableTileGlow.SPR", 0, 0, bgZOrder + 3);
		movableTileGlow->SetUID(std::string("MovableTileGlow_") + MKSTR(i));
		
		DeitiesPuzzleObject * puzzleObject = new DeitiesPuzzleObject(mouseOverGlow, selectedGlow, movableTileGlow, ProfileSystem::Instance->ActiveProfile.L2_S4_DeitiesTileIndices[i], true);
		puzzleObject->addSolution(i);
		CPoint gridOrigin(229, 86, 0);
		CPoint gridSize(72, 72, 0);

		
		std::string  str = std::string("LV2\\S4\\DEITIESPUZZLE\\Tile_") + MKSTR(i) + std::string(".SPR");
		CPoint position = getPositionOfGrid(ProfileSystem::Instance->ActiveProfile.L2_S4_DeitiesTileIndices[i]);
		position.z = (float)(bgZOrder + 1);
		puzzleObject->LoadInit(str, position.x, position.y, (int)(position.z));
		puzzleObject->SetUID(std::string("Tile_") + MKSTR(i));
		puzzleObject->setCurrentGridIndex(ProfileSystem::Instance->ActiveProfile.L2_S4_DeitiesTileIndices[i]);

		puzzleObject->SetMouseClickCallback(CC_CALLBACK_1(L2_S4_DeitiesPuzzle::OnMouseClickOnTile, this));
		puzzleObject->setIsMovable(false);	

		HOScene->PushObject(puzzleObject);
		_arrayOfPuzzleObjects.push_back(puzzleObject);
		addObject(puzzleObject);
	}


	switch (ProfileSystem::Instance->ActiveProfile.L2_S4_DeitiesPuzzleState)
	{

	case eL2S4DeitiesPuzzleState_started:
		{
			for (int i = 0; i < (int)(_arrayOfPuzzleObjects.size()); i++)
			{
				DeitiesPuzzleObject* puzzleObject = _arrayOfPuzzleObjects.at(i);
				puzzleObject->setIsMovable(true);
			}
			_door->Show(true);
			_metalLever->Show(true);
		}
		break;
	case eL2S4DeitiesPuzzleState_Solved:
		{
			_metalLever->Show(true);
			_metalLever->SetIsInteractionEnabled(true);
		}
		break;
	case eL2S4DeitiesPuzzleState_MetalBarTaken:
		{

		}
	default:
		break;
	}

}

void L2_S4_DeitiesPuzzle::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Control::Audio->IsPlaying(aTrackL2theme) )
	{
		Control::Audio->Samples[aTrackL2theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL2theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL2theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL2puzzle) )
				  Control::Audio->PlaySample(aTrackL2puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL2puzzle))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL2puzzle, true);
		}
	}
}

void L2_S4_DeitiesPuzzle::Update()
{
	NavigateToScene();
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;
	//Cursor::SetMode(CA_Normal);

	ResetPuzzle();
	if(IsResetPop)
	{
		GFHud->AllowInput = false;
		return;
	}
	else
		GFHud->AllowInput = true;

	bool isMouseOverTile =  false;
	_isTweeningAnyObject = false;
	for (int i = 0; i < (int)(_arrayOfPuzzleObjects.size()); i++)
	{
		DeitiesPuzzleObject* puzzleObject = _arrayOfPuzzleObjects.at(i);
		if (puzzleObject->IsTweening)
		{
			_isTweeningAnyObject = true;
		}
		if (puzzleObject->_mouseOverGlow->Visible)
		{
			isMouseOverTile = true;
		}
	}

	if(isMouseOverTile)
	{
		Cursor::SetMode(CA_HandPointy);
	}
	else 
	{
		Cursor::SetMode(CA_Normal);
	}
	CPoint mousePos = Control::Input->Mpos();
	if(_metalLever->GetCurrentMouseState() == eMouseState_Over)
	{
		Cursor::SetMode(CA_HandTake);
	}
	//else if( !_isTweeningAnyObject && (ProfileSystem::Instance->ActiveProfile.L2_S4_DeitiesPuzzleState != eL2S4DeitiesPuzzleState_Solved) && IsMouseOverGoBackArea(&Control::Input->Mpos()) )
	// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
	else if( !_isTweeningAnyObject && (ProfileSystem::Instance->ActiveProfile.L2_S4_DeitiesPuzzleState != eL2S4DeitiesPuzzleState_Solved) && IsMouseOverGoBackArea(&mousePos) )
	{


#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitDown,eArea_L2_S4_Cave);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L2_S4_Cave);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L2_S4_Cave, 2);
			}
#else
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S4_Cave;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}	
	}

	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed() && !_isTweeningAnyObject && (ProfileSystem::Instance->ActiveProfile.L2_S4_DeitiesPuzzleState != eL2S4DeitiesPuzzleState_Solved))
		{	
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S4_Cave;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}
	#endif
}

void L2_S4_DeitiesPuzzle::UpdateChangesToProfile()
{
	for (int i = 0; i < (int)(_arrayOfPuzzleObjects.size()); i++)
	{
		DeitiesPuzzleObject * puzzleObject = _arrayOfPuzzleObjects.at(i);
		ProfileSystem::Instance->ActiveProfile.L2_S4_DeitiesTileIndices[i] = puzzleObject->getCurrentGridIndex();
	}

}

float L2_S4_DeitiesPuzzle::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L2_S4_DeitiesPuzzle::onPuzzleSolved()
{

	GridPuzzle::onPuzzleSolved();
	Control::Audio->QuickLoadAndPlaySFX("s4pz1_puzzlecomplete");
	Hud->ShowBannerText("L2wasnt");
	
	ProfileSystem::Instance->ActiveProfile.L2_S4_DeitiesPuzzleState = eL2S4DeitiesPuzzleState_Solved;
	Hud->InvBox_StopPuzzleMode();

	for (int i = 0; i < (int)(_arrayOfPuzzleObjects.size()); i++)
	{
		DeitiesPuzzleObject* puzzleObject = _arrayOfPuzzleObjects.at(i);
		{
			puzzleObject->setIsMovable(false);
			if (i == 0)
			{
				puzzleObject->StartPuzzleCompleteAnim(CC_CALLBACK_1(L2_S4_DeitiesPuzzle::OnPuzzleCompleteAnimFinish, this));
			}
			else
			{
				puzzleObject->StartPuzzleCompleteAnim();
			}
		}
	}

	AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
		
	
}

void L2_S4_DeitiesPuzzle::OnMouseClick(CObject * object)
{
	if(IsResetPop)
		return;
	if(GFInstance->IsActivePopup)
	{
		return;
	}

	if (object->UID.compare("MetalLever") == 0)
	{
		Hud->TakeInventoryItem(eInv_L2S4_Lever);
		object->Show(false);
		ProfileSystem::Instance->ActiveProfile.L2_S4_InvMetalLeverTaken = true;
		_metalLever->SetIsInteractionEnabled(false);
		ProfileSystem::Instance->ActiveProfile.L2_S4_DeitiesPuzzleState = eL2S4DeitiesPuzzleState_MetalBarTaken;
		
	}

}

void L2_S4_DeitiesPuzzle::ReceiveMessage(int val)
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

	}
	else if( val == INV_CLOSE )
	{		
		if(ProfileSystem::Instance->ActiveProfile.L2_S4_DeitiesPuzzleState == eL2S4DeitiesPuzzleState_MetalBarTaken)
		{
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S4_Cave;
			SendMessage(ControlExit, ID, HOSM_NewArea);		
		}

	}	
}

void L2_S4_DeitiesPuzzle::ProcessNonHOHint()
{
	if(ProfileSystem::Instance->ActiveProfile.L2_S4_DeitiesPuzzleState == eL2S4DeitiesPuzzleState_Solved)
	{
		InitHintTrailEmit(&InvRect);
		return;
	}
}  

void L2_S4_DeitiesPuzzle::OnMouseOver(CObject * obj)
{
	
}

void L2_S4_DeitiesPuzzle::OnMouseOut(CObject * obj)
{
	
}

void L2_S4_DeitiesPuzzle::OnMouseClickOnTile(CObject * obj)
{
	if(IsResetPop)
		return;

	DeitiesPuzzleObject * puzzleObject = (DeitiesPuzzleObject*)obj;
	if (!puzzleObject->getISMovable())
	{
		return;
	}
	if (_swapObject1 == NULL)
	{
		Control::Audio->QuickLoadAndPlaySFX("s3pz1_stoneselect");
		_swapObject1 = puzzleObject;
		_swapObject1->SetIsSelected(true);
		UpdateTileMouseInteraction();
	}
	else if (puzzleObject == _swapObject1)// cancel selected tile
	{
		_swapObject1->SetIsSelected(false);
		_swapObject1 = NULL;
		_swapObject2 = NULL;
		UpdateTileMouseInteraction();
	}
	else
	{
		_swapObject1->SetIsSelected(false);
		_swapObject2 = puzzleObject;
		SwapTiles(_swapObject1, _swapObject2);
		_swapObject1 = NULL;
		_swapObject2 = NULL;
		UpdateTileMouseInteraction();
	}
	
}

void L2_S4_DeitiesPuzzle::OnCompleteDoorOpenAnim(CObject * object)
{
	_isDoorOpenAnimPlaying = false;
	_metalLever->SetIsInteractionEnabled(true);
	

}

void L2_S4_DeitiesPuzzle::SwapTiles(DeitiesPuzzleObject* object1, DeitiesPuzzleObject* object2)
{
	_tweenCompleteCounter = 0;
	object1->prevZOrder = object1->ZOrder;
	object2->prevZOrder = object2->ZOrder;
	object1->SetZ(20);
	object2->SetZ(20);
	tweenObjectToGrid(object1, object2->getCurrentGridIndex());
	tweenObjectToGrid(object2, object1->getCurrentGridIndex());
	Control::Audio->QuickLoadAndPlaySFX("s4pz1_tileswap");
}

void L2_S4_DeitiesPuzzle::onTweenComplete(CObject * object)
{
	GridPuzzle::onTweenComplete(object);
	object->SetZ(((DeitiesPuzzleObject*)object)->prevZOrder);
	_tweenCompleteCounter++;
	if (_tweenCompleteCounter == 2)
	{
		UpdateTileMouseInteraction();
		UpdateChangesToProfile();
	}
}

void L2_S4_DeitiesPuzzle::OnPuzzleCompleteAnimFinish(CObject * object)
{
	if (_handleExtrasGameComplete())
	{
		return;
	}
	float valArr[5] = {-1, _door->Pos.x, _door->Pos.y + 50, -1, -1};
	_door->TweenTo(valArr, 1.0f, TW_EASEBACKOUT, true, CC_CALLBACK_1(L2_S4_DeitiesPuzzle::OnCompleteDoorOpenAnim, this));
}

void L2_S4_DeitiesPuzzle::UpdateTileMouseInteraction()
{

	if(IsResetPop)
		return;

	for (int i = 0; i < (int)(_arrayOfPuzzleObjects.size()); i++)
	{
		DeitiesPuzzleObject * obj = _arrayOfPuzzleObjects.at(i);
		obj->OnMouseOut();
		obj->SetIsInteractionEnabled(false);
		obj->GetMovableTileGlow()->Show(false);
		
	}

	if ((_tweenCompleteCounter == 2  || _tweenCompleteCounter == -1) &&  ( _swapObject1 == NULL && _swapObject2 == NULL))
	{
		_tweenCompleteCounter = -1;
		for (int i = 0; i < (int)(_arrayOfPuzzleObjects.size()); i++)
		{
			DeitiesPuzzleObject * obj = _arrayOfPuzzleObjects.at(i);
			obj->SetIsInteractionEnabled(true);
		}
	}

	if (_swapObject1 != NULL && _swapObject2 == NULL)
	{
		int columnNumber = _swapObject1->getCurrentGridIndex() % _numOfColumns;
		int rowNumber =  _swapObject1->getCurrentGridIndex() / _numOfColumns;
		
		int topLeft = _swapObject1->getCurrentGridIndex() - (_numOfColumns + 1); //top left
		int topRight = _swapObject1->getCurrentGridIndex() - (_numOfColumns - 1); //top right
		int bottomLeft = _swapObject1->getCurrentGridIndex() + (_numOfColumns - 1); //bottom left
		int bottomRight = _swapObject1->getCurrentGridIndex() + (_numOfColumns + 1); //bottom right

		if (columnNumber == 0)
		{
			topLeft = -1;
			bottomLeft = -1;
		}
		if (columnNumber == (_numOfColumns - 1))
		{
			topRight = -1;
			bottomRight = -1;
		}
		if (rowNumber == 0)
		{
			topLeft = -1;
			topRight = -1;
		}
		if (rowNumber == (_numOfRows - 1))
		{
			bottomLeft = -1;
			bottomRight = -1;
		}

		int adjucentDiagonalIndices[4] = {topLeft, topRight, bottomLeft, bottomRight};

		_swapObject1->SetIsInteractionEnabled(true);
		for (int i = 0; i < 4; i++)
		{
			if (adjucentDiagonalIndices[i] != -1)
			{
				GridPuzzleObject * obj = getObjectAtGridIndex(adjucentDiagonalIndices[i]);
				obj->SetIsInteractionEnabled(true);
				((DeitiesPuzzleObject*)obj)->GetMovableTileGlow()->Show(true);
			}
		}

	}
}
void L2_S4_DeitiesPuzzle::ResetPuzzle()
{
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		return;
	
	if(ProfileSystem::Instance->ActiveProfile.L2_S4_DeitiesPuzzleState == eL2S4DeitiesPuzzleState_Solved)
	{
		ResetBtn->SetEnable(false);
		return;
	}
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
			ProfileSystem::Instance->ActiveProfile.L2_S4_DeitiesPuzzleState = 0;
			ProfileSystem::Instance->ActiveProfile.L2_S4_InvMetalLeverTaken = false;
			for (int i = 0; i < MAX_L2_S4_DEITIESTILES; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L2_S4_DeitiesTileIndices[i] = -1;
			}

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S4_PZDeities;
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

void L2_S4_DeitiesPuzzle::NavigateToScene()
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



