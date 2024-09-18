//====================================
//  created by : Jithin
//  copyright  : Tuttifrutti Games
//====================================

#include "L7_S2_PZEyePuzzle.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "Util.h"
#include <math.h>

const CRectangle  StatueRect(610.0f,54.0f,229.0f,538.0f);
const CRectangle  DiscRect(617.0f,380.0f,184.0f,194.0f);

const int No_Of_EyeParts = 8;
const int AngleOffset = 45;
const CPoint Pviort(-146, 104, 0);
const CPoint AngleStartPos(688, 319, 0);
const int Radius = 300;
const int TweenSpeed = 1;

namespace Level7_Scene2
{
	EyePart::EyePart(CObject* obj)
	{
		LoadSprite(obj->Filename);
		SetPos(&obj->Pos);
		SetZ(obj->ZOrder);
		obj->SetEnable(false);

		_isSelected = false;
		_isMouseIn = false;
		_nextAngle = 0;
		_mouseOverGlow = NULL;
		_delegate = NULL;

		_exchangeWith = NULL;
		_next = NULL;
	}

	EyePart::~EyePart()
	{
		_mouseOverGlow = NULL;
		_delegate = NULL;
		_next = NULL;
	}

	void EyePart::ExchangeWith(EyePart* Part)
	{
		Swap(Part);
		_exchangeWith = Part;

		float angleDif = Part->GetRotate() - GetRotate();
		float normaize = (float)Elixir::NormalizeAngle(angleDif, -180.0, 180.0);
		float twTime = (abs(normaize) == 180) ? 1.5f : TweenSpeed;
		float valArray1[5] = {-1, -1, -1, -1, GetRotate() + normaize};
		TweenTo(valArray1, twTime, TW_EASEBACKOUT, true, CC_CALLBACK_1(EyePart::OnTweenCompleted, this));
		
		angleDif = GetRotate() - Part->GetRotate();
		normaize = (float)Elixir::NormalizeAngle(angleDif, -180.0, 180.0);
		float valArray2[5] = {-1, -1, -1, -1, Part->GetRotate() + normaize};
		Part->TweenTo(valArray2, twTime, TW_EASEBACKOUT, true);
	}

	void EyePart::UpdatePart(float angle)
	{
		if (IsTweening)
			return;

		//K_LOG("index - %d  angle - %f", _index, GetRotate());

		if (!_isSelected)
		{
			angle = (float)Elixir::NormalizeAngle(angle, 0, 360);
			if (!_isMouseIn && angle > GetRotate() && angle < GetRotate() + AngleOffset)
			{
				_isMouseIn = true;
				_mouseOverGlow->FadeIn(3);
			}

			if (_isMouseIn)
			{
				if (angle < GetRotate() || angle > GetRotate() + AngleOffset)
				{
					_isMouseIn = false;
					_mouseOverGlow->FadeOut(3);
				}

				if (Control::Input->LBclicked())
				{
					_isSelected = true;
					_delegate->onSelected(this);
				}
			}
		}
	}

	void EyePart::SetPivot( float pX, float pY )
	{
		CObject::SetPivot(pX, pY);
		_mouseOverGlow->SetPivot(pX, pY);
	}

	void EyePart::SetRotate(float rotate)
	{
		rotate = (float)Elixir::NormalizeAngle(rotate, 0, 360);

		if (rotate == 360.0f)
			rotate = 0.0f;

		CObject::SetRotate(rotate);
		_mouseOverGlow->SetRotate(rotate);
	}

	void EyePart::Reset(bool isAll)
	{
		if (!isAll && _isSelected)
			return;

		if (_isMouseIn)
				_mouseOverGlow->FadeOut(3);

		_isMouseIn = false;
		_isSelected = false;
	}

	void EyePart::OnTweenCompleted(CObject* obj)
	{
		float currentAngle = (float)Elixir::NormalizeAngle(GetRotate(), 0, 360);
		SetRotate(currentAngle);

		float exCurrentAngle = (float)Elixir::NormalizeAngle(_exchangeWith->GetRotate(), 0, 360);
		_exchangeWith->SetRotate(exCurrentAngle);

		Reset();
		_exchangeWith->Reset();
		_delegate->onRotateCompleted(this);
	}

	void EyePart::Swap(EyePart *Part)
	{
		int tmpIndex = Part->_index;
		Part->_index = _index;
		_index = tmpIndex;
	}

	EyePZController::EyePZController()
	{
		_isSolved = false;
		_resetTimer = 0;
		_finalRotation = 0;
	}

	EyePZController::~EyePZController()
	{

	}

	void EyePZController::InitPuzzle(CHOScene* scene)
	{
		_clickedObj.clear();

		for (int i = 0; i < No_Of_EyeParts; i++)
		{
			CObject* eye = scene->GetObjectByID("Eye_"+MKSTR(i+1));
			CObject* mouseOver = scene->GetObjectByID("Eye_Selection_"+MKSTR(i+1));
			mouseOver->Show(false);

			EyePart* part = new EyePart(eye);
			part->setIndex(i);
			part->setDelegate(this);
			part->setMouseOverGlow(mouseOver);
			part->SetPivot(Pviort.x, Pviort.y);
			part->SetRotate((float)i*AngleOffset);
			scene->PushObject(part);

			if (!_eyeParts.empty())
			{
				_eyeParts.back()->setNext(part);
			}

			_eyeParts.push_back(part);
		}

		_eyeParts.back()->setNext((*_eyeParts.begin()));

		LoadSavedPos();
	}

	void EyePZController::LoadSavedPos()
	{
		if (ProfileSystem::Instance->ActiveProfile.L7_S2_PZEyePartIndex[0] == -1)
		{
			int indexes[] = {4, 3, 7, 0, 5, 6, 2, 1};
			for (int i = 0; i < (int)_eyeParts.size(); i++)
			{
				_eyeParts[i]->setIndex(indexes[i]);
				_eyeParts[i]->SetRotate((float)(indexes[i] * AngleOffset));
				ProfileSystem::Instance->ActiveProfile.L7_S2_PZEyePartIndex[i] = indexes[i];
			}
		}
		else
		{
			for (int i = 0; i < (int)_eyeParts.size(); i++)
			{
				int index = ProfileSystem::Instance->ActiveProfile.L7_S2_PZEyePartIndex[i];
				_eyeParts[i]->setIndex(index);
				_eyeParts[i]->SetRotate((float)(index * AngleOffset));
			}
		}
	}

	void EyePZController::Update()
	{

		CPoint mPos = Control::Input->Mpos();

		if (Elixir::Distance(&AngleStartPos, &mPos) <= Radius)
		{
			Cursor::SetMode(CA_HandPointy);

			_resetTimer = 1;
			float angle = atan2(AngleStartPos.y - mPos.y, AngleStartPos.x - mPos.x);
			angle = 180 - (angle * 180 / PIE);

			for (int i = 0; i < (int)_eyeParts.size(); i++)
			{
				_eyeParts[i]->UpdatePart(angle);
			}
		}
		else
		{
			if (Control::Input->LBclicked())
			{
				_clickedObj.clear();
				for (int i = 0; i < (int)_eyeParts.size(); i++)
				{
					_eyeParts[i]->Reset();
				}
			}

			if (_resetTimer > 0.0f)
			{
				_resetTimer -= Control::LogicRate;
				if (!(_resetTimer <= 0.0f))
				{
					for (int i = 0; i < (int)_eyeParts.size(); i++)
					{
						_eyeParts[i]->Reset(false);
					}
				}
			}
		}
	}

	void EyePZController::FadeOutParts()
	{
		for (int i = 0; i < (int)_eyeParts.size(); i++)
		{
			_eyeParts[i]->Reset();
			_eyeParts[i]->FadeOut(3);
		}
	}

	void EyePZController::DisableParts()
	{
		for (int i = 0; i < (int)_eyeParts.size(); i++)
		{
			_eyeParts[i]->Reset();
			_eyeParts[i]->SetEnable(false);
		}
	}

	void EyePZController::onSelected(CObject *obj)
	{
		_clickedObj.push_back((EyePart*)obj);

		if (_clickedObj.size() == 2)
		{
			_clickedObj[0]->ExchangeWith(_clickedObj[1]);
			_clickedObj.clear();
			Control::Audio->QuickLoadAndPlaySFX("s2pz1_tileswap");
			return;
		}

		Control::Audio->QuickLoadAndPlaySFX("s2pz1_tileclicked");
	}

	void EyePZController::onRotateCompleted(CObject* obj)
	{
		CheckIsCompleted();

		for (int i = 0; i < (int)_eyeParts.size(); i++)
		{
			ProfileSystem::Instance->ActiveProfile.L7_S2_PZEyePartIndex[i] = _eyeParts[i]->getIndex();
		}
	}

	void EyePZController::CheckIsCompleted()
	{
		EyePart* part = NULL;
		std::vector<EyePart*>::iterator itr = _eyeParts.begin();
		while (itr != _eyeParts.end())
		{
			if ((*itr)->getIndex() == 0)
			{
				part = *itr;
				break;
			}
			itr++;
		}	

		for (int i = 1; i < (int)_eyeParts.size(); i++)
		{
			if (part->getNext()->getIndex() != i)
			{
				return;	
			}

			part = part->getNext();
		}


		_finalRotation = (int)_eyeParts[0]->GetRotate();
		_isSolved = true;
		Control::Audio->QuickLoadAndPlaySFX("s2pz1_puzzlecomplete");
	}
}


L7_S2_PZEyePuzzle::L7_S2_PZEyePuzzle()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV7\\S2\\PZ1\\PZ1.SCN");

	_eyeFull = HOScene->GetObjectByID("FinalEffectGlow");
	_eyeFull->Show(false);
	HOScene->RemoveObject(_eyeFull->UID);

	_statue = HOScene->GetObjectByID("Statue");
	_statue->Show(false);

	_device = HOScene->GetObjectByID("Device");
	_device->Show(false);

	_pzController = new Level7_Scene2::EyePZController();
	_pzController->InitPuzzle(HOScene);

	s2pzEyeDestFX = new SuperFX("PRT\\L7\\s2pzEyeDest.INI", PointSystem::CreateCPoint(629,60,0), 13); 

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL7s2LasyaPrisonAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL7s2LasyaPrisonAmb) )		
	{
		Control::Audio->PlaySample(aAmbL7s2LasyaPrisonAmb,true);
	}
	
	//MUSIC_SFX

	_pzState = ProfileSystem::Instance->ActiveProfile.L7_S2_PZEyeGameState;
	_stateMachine.push(_pzState);

	CheckOnLoad();

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L7_S2_PZEyePuzzle]  = true;
}

void L7_S2_PZEyePuzzle::CheckOnLoad()
{
	switch (_pzState)
	{
	case L7S2PZEye_Playing:
		Hud->InvBox_StartPuzzleMode();
		break;

	case L7S2PZEye_WaitForCollectStatue:
	case L7S2PZEye_DiscCollected:
		{
			_device->SetEnable(false);
		}
	case L7S2PZEye_Solved:
	case L7S2PZEye_OpenEye:
	case L7S2PZEye_OpeningEye:
	case L7S2PZEye_WaitForCollectDisc:
		{
			CPoint newPOs = _eyeFull->Pos - CPoint(_eyeFull->GetLocalRect().w, 0, 0);
			_pzController->DisableParts();
			_eyeFull->SetPos(&newPOs);
			_eyeFull->Show(true);
			_device->Show(true);
			_statue->Show(true);
		}
		break;

	default:
		break;
	}
}

void L7_S2_PZEyePuzzle::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Control::Audio->IsPlaying(aTrackL7theme) )
	{
		Control::Audio->Samples[aTrackL7theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL7theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL7theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL7puzzle) )
				  Control::Audio->PlaySample(aTrackL7puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL7puzzle))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL7puzzle, true);
		}
	}
}

void L7_S2_PZEyePuzzle::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	if (_stateMachine.top() != _pzState)
	{
		ProfileSystem::Instance->ActiveProfile.L7_S2_PZEyeGameState = _pzState;
		_stateMachine.push(_pzState);
	}

	switch (_pzState)
	{
	case L7S2PZEye_Start:
		{
			Hud->InvBox_StartPuzzleMode();
			_pzState = L7S2PZEye_Playing;
		}
		break;

	case L7S2PZEye_Playing:
		{
			_pzController->Update();

			if (_pzController->getIsSolved())
			{
				_pzState++;
			}
		}
		break;

	case L7S2PZEye_Skipping:
		{
			_pzState = L7S2PZEye_Solved;
		}
		break;

	case L7S2PZEye_Solved:
		{
			AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			if (_handleExtrasGameComplete())
			{
				_pzState = L7S2PZEye_ExtrasModeComplete;
				return;
			}

			Hud->InvBox_StopPuzzleMode();
			_pzController->FadeOutParts();
			_eyeFull->FadeIn(3);
			_eyeFull->SetRotate((float)_pzController->getFinalRotation());
			ProfileSystem::Instance->ActiveProfile.L7_S2_PZEyePuzzleCompleted = true;
			_pzState++;
		}
		break;

	case L7S2PZEye_OpenEye:
		{
			s2pzEyeDestFX->Start();
			_device->Show(true);
			_statue->Show(true);
			float valArr[5] = {-1, _eyeFull->Pos.x - _eyeFull->GetLocalRect().w, -1, -1, _eyeFull->GetRotate() + 90};
			_eyeFull->TweenTo(valArr, 3, 0, true);
			HOScene->Shake(5, 5, 3);
			_pzState = L7S2PZEye_OpeningEye;
		}
		break;

	case L7S2PZEye_OpeningEye:
		{
			HOScene->update(Control::LogicRate);

			if (!_eyeFull->IsTweening)
			{
				s2pzEyeDestFX->StopEmit();
				_pzState = L7S2PZEye_WaitForCollectDisc;
			}
		}
		break;

	case L7S2PZEye_WaitForCollectDisc:
		{
			if (DiscRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if (Control::Input->LBclicked())
				{
					Hud->TakeInventoryItem(eInv_L7_S2_SymbolDisk);
					_device->FadeOut(1);
					_pzState = L7S2PZEye_DiscCollected;
				}
			}
		}
		break;

	case L7S2PZEye_DiscCollected:
		{
			ProfileSystem::Instance->ActiveProfile.L7_S2_InvSymbolDiscTaken = true;
		}
		break;

	case L7S2PZEye_WaitForCollectStatue:
		{
			if (StatueRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if (Control::Input->LBclicked())
				{
					Hud->TakeInventoryItem(eInv_L7_S2_LasyaStatue);
					_statue->FadeOut(1);
					_pzState = L7S2PZEye_StatueCollected;
				}
			}
			
		}
		break;

	case L7S2PZEye_StatueCollected:
		{
			ProfileSystem::Instance->ActiveProfile.L7_S2_InvStatueTaken = true;
		}
		break;

	case L7S2PZEye_Completed:
		{
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S2_LasyaPrison;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
			_pzState++;
		}
		break;

	case L7S2PZEye_ExtrasModeComplete:
		break;

	default:
		_pzState++;
		return;
	}

	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
		//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S2_LasyaPrison;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif
	if( IsMouseOverGoBackArea(&pos) )
	{
		
#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitDown,eArea_L7_S2_LasyaPrison);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L7_S2_LasyaPrison);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S2_LasyaPrison, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S2_LasyaPrison;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
									
		}	
	}
}

void L7_S2_PZEyePuzzle::ReceiveMessage(int val)
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
			GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		
		if (_pzState == L7S2PZEye_StatueCollected)
		{
			_pzState = L7S2PZEye_Completed;
		}
		else
		{
			_pzState = L7S2PZEye_WaitForCollectStatue;
		}
	}
}

void L7_S2_PZEyePuzzle::ProcessSkip()
{
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	_pzState = L7S2PZEye_Skipping;
}

void L7_S2_PZEyePuzzle::ProcessNonHOHint()
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
				{
					if(!ProfileSystem::Instance->ActiveProfile.L7_S2_InvSymbolDiscTaken)
						Hud->InitHintTrailEmit(&DiscRect);
					else
						Hud->InitHintTrailEmit(&StatueRect);
					return;
				}


			break;
		case 1: ++iNonHOHint;
			/*if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S3_CaveMouth])
			{
			InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
			return;
			}*/
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

L7_S2_PZEyePuzzle::~L7_S2_PZEyePuzzle()
{
	Hud->InvBox_StopPuzzleMode();
	SAFE_DELETE(_pzController);
	SAFE_DELETE(_eyeFull);
	SAFE_DELETE(s2pzEyeDestFX);

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL7s2LasyaPrisonAmb);
	Control::Audio->UnloadSample(aAmbL7s2LasyaPrisonAmb);
	//MUSIC_SFX
}
void L7_S2_PZEyePuzzle::NavigateToScene()
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