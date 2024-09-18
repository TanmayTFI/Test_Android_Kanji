//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L5_S3_HOUnderwater.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "MeshObject.h"
#include "Util.h"

#define NUMBER_OF_HO 12

const char* L5S3ItemNames[] = {"","","","","","","","","","","",""};
const int NumClicks[] = {1,1,1,1,1,1,1,1,1,1,1,2};
const string InvBoxText = "Exhook";
const CRectangle  Match3BtnRect(550.0f,1.0f,252.0f,39.0f);

L5S3HiddenObject::L5S3HiddenObject()
{
	_isAnimComplete = false;
	_startPoint = NULL;
	_endPoint = NULL;
	_ctrlPoint1 = NULL;
	_ctrlPoint2 = NULL;
	_objectPos = CPoint();

	_name = "";
	_nameText = NULL;
	_toFind = NULL;
	_completeObject = NULL;
	_polygonArea = NULL;
	_isFound = false;
	hoScaleTimer = hoFlyingTimer = 0;
	_isObjectScaling = false;
	_numberOfClicks = 0;

	HOTrailFX = NULL;
	waterHOFX = NULL;

	_clicksToFind = 1;
}

L5S3HiddenObject::~L5S3HiddenObject()
{
	_startPoint = NULL;
	_endPoint = NULL;
	SAFE_DELETE(_ctrlPoint1);
	SAFE_DELETE(_ctrlPoint2);
	_name = "";
	SAFE_DELETE(_nameText);
	_toFind = NULL;
	_completeObject = NULL;
	SAFE_DELETE(_polygonArea);
	_isFound = false;
	SAFE_DELETE(HOTrailFX);
	SAFE_DELETE(waterHOFX);
}

void L5S3HiddenObject::InitObject()
{
	InitInvNameText();
	_objectPos = _toFind->Pos;
	_startPoint = &_toFind->Pos;
	_endPoint = &_nameText->Pos;
	_ctrlPoint1 = new CPoint(_startPoint->x, (_startPoint->y + _endPoint->y)/3.0f, 0);
	_ctrlPoint2 = new CPoint(_endPoint->x, (_ctrlPoint1->y + _endPoint->y)*0.5f, 0);

	_ctrlPoint1->y += (200 + rand() % 200);
	_ctrlPoint2->y -= (200 + rand() % 200);

	_completeObject->SetEnable(false);

	HOTrailFX = NULL;
	HOTrailFX = new SuperFX("PRT\\L2\\HOTrailFX.INI", PointSystem::CreateCPoint(1065,122,0), 50);

	waterHOFX = new SuperFX("PRT\\L2\\waterHO.INI", PointSystem::CreateCPoint(564,368,0), 1100); 
	waterHOFX->SetPos(_startPoint);
}

void L5S3HiddenObject::InitInvNameText()
{
	CRectangle InvAreaRect(368.0f,664.0f,624.0f,96.0f);
	CPoint pos(InvAreaRect.GetCenterX(), InvAreaRect.GetCenterY(), 0);

	ColorRGB gold = {(233/255.0f),(204/255.0f),(131/255.0f)};
	_nameText = new CBitmapText();
	_nameText->SetTextProps(_name, pos.x, pos.y, eZOrder_INVBOXITEMS, gold, eFont_Small, Align_Center);
	_nameText->SetZ(eZOrder_INVBOXITEMS);
}


bool L5S3HiddenObject::MouseClicked(CPoint pos)
{
	if (_isFound)
	{
		return false;
	}

	if (_polygonArea->Intersection(&pos) && 
		++_numberOfClicks == _clicksToFind)
	{
		OnFound();
		return true;
	}

	return false;
}

void L5S3HiddenObject::OnFound()
{
	_toFind->FadeOut(3.0f);
	_completeObject->SetEnable(true);
	_completeObject->FadeIn(4.0f);
	_completeObject->SetZ(eZOrder_INVBOXITEMS);
	_isFound = true;
	waterHOFX->Start();
	_isObjectScaling = true;
}

void L5S3HiddenObject::Update()
{
	if (_isAnimComplete || !_isFound)
		return;

	if(_isObjectScaling)
	{
		//scaling anim
		hoScaleTimer += Control::LogicRate;
		_completeObject->SetScale(0.85f+Elixir::Sin(90+hoScaleTimer*360.0f)*0.15f);
		if(hoScaleTimer > 1.0f)
		{
			//start flying
			_isObjectScaling = false;
		}
	}
	else
	{
		if(!HOTrailFX->IsEmitting)
			HOTrailFX->Start();

		FXTimer += 1;
		if(FXTimer%3==0)
			HOTrailFX->Emit();

		//flying to HUD
		hoFlyingTimer += Control::LogicRate;

		CPoint temp = Elixir::GetCubicBezierPoint(_startPoint, _ctrlPoint1, _ctrlPoint2, _endPoint, hoFlyingTimer);
		_completeObject->SetPos(&temp);
		_completeObject->SetScale(1.0f-hoFlyingTimer*0.75f);
		HOTrailFX->SetPos(&temp);

		if(hoFlyingTimer>1.0f)
		{
			HOTrailFX->StopEmit();
			_completeObject->SetEnable(false);
			_isAnimComplete = true;
		}
	}
}

void L5S3HiddenObject::AlreadyFound()
{
	_isFound = true;
	_isAnimComplete = true;
	_toFind->SetEnable(false);
	_completeObject->SetEnable(false);
	InitInvNameText();
	_nameText->SetAlpha(0.5f);
}

void L5S3HiddenObject::SwitchMatch3()
{
	_nameText->Show(false);
}

void L5S3HiddenObject::SwitchHO()
{
	_nameText->Show(true);
}


L5_S3_HOUnderwater::L5_S3_HOUnderwater()
{
	AwardManager::GetInstance()->StartHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV5\\S3\\HO\\HO.SCN");

	_starFish = HOScene->GetObjectByID("star_fish_1");
	if (ProfileSystem::Instance->ActiveProfile.L5_S3_HOUnderWaterStarRemoved)
		_starFish->SetAlpha(0);

	MeshObject* underWaterMesh = new MeshObject(HOScene->GetObjectByID("UnderWater_Effect"), 10, 10);
	underWaterMesh->meshFX->SetSpeedAndDisplacement(2, 4, 8, 10);
	HOScene->PushObject(underWaterMesh);

	MeshObject* topWaterMesh = new MeshObject(HOScene->GetObjectByID("water_top"), 5, 8);
	topWaterMesh->meshFX->SetSpeedAndDisplacement(5, 4, 5, 6);
	HOScene->PushObject(topWaterMesh);

	MeshObject* plant1 = new MeshObject(HOScene->GetObjectByID("planet_lt"), 5, 5);
	plant1->meshFX->SetSpeedAndDisplacement(2, 1, 5, 2);
	HOScene->PushObject(plant1);

	MeshObject* plant2 = new MeshObject(HOScene->GetObjectByID("planet_rt"), 8, 8);
	plant2->meshFX->SetSpeedAndDisplacement(4, 1, 7, 2);
	HOScene->PushObject(plant2);

	MeshObject* plant3 = new MeshObject(HOScene->GetObjectByID("planet_back"), 4, 4);
	plant3->meshFX->SetSpeedAndDisplacement(3, 1, 4, 2);
	HOScene->PushObject(plant3);

	CObject* light = HOScene->GetObjectByID("top_light");
	light->Animate(0.5f, 0.7f, 4);
	light->SetBlendMode(eBlend_AlphaAdditive);

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL5s3UnderwaterHOAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL5s3UnderwaterHOAmb) )		
	{
		Control::Audio->PlaySample(aAmbL5s3UnderwaterHOAmb,true);
	}
	
	//MUSIC_SFX


	S3Bubble1FX = new SuperFX("PRT\\L5\\S3Bubble1.INI", PointSystem::CreateCPoint(697,556,0), 1100); 
	S3Bubble1FX->Start();
	S3Bubble1FX->AdvanceByTime(2.0f);

	S3Bubble2FX = new SuperFX("PRT\\L5\\S3Bubble2.INI", PointSystem::CreateCPoint(365,459,0), 1100); 
	S3Bubble2FX->Start();
	S3Bubble2FX->AdvanceByTime(2.0f);

	S3Bubble3FX = new SuperFX("PRT\\L5\\S3Bubble3.INI", PointSystem::CreateCPoint(1157,674,0), 1100); 
	S3Bubble3FX->Start();
	S3Bubble3FX->AdvanceByTime(2.0f);


	ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
	_match3Ctrl = NULL;
	match3Back = new CObject();
	match3Back->LoadInit("MNU\\EXTRAS\\BACKBUTTON.SPR", 1266, 30, eZOrder_Popup + 300);
	match3Back->SetUID("match3Back");
	match3Back->Show(false);
	HOScene->PushObject(match3Back);

	match3BackText = new CObject();
	match3BackText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\BACK.SPR", 1250, 27, eZOrder_Popup + 301);
	match3BackText->SetUID("match3BackText");
	match3BackText->Show(false);
	HOScene->PushObject(match3BackText);
#ifndef M3_BUTTON
	match3Back->SetEnable(false);
    match3BackText->SetEnable(false);
#endif
	if (ProfileSystem::Instance->_isExtrasGamePlay)
	{
		match3Back->SetEnable(false);
		match3BackText->SetEnable(false);
	}
	_btnMatch3 = new CObject();
	_btnMatch3->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNMATCH3.SPR", 673, 20, 38);
	_btnMatch3->SetUID("btnMatch3");
	HOScene->PushObject(_btnMatch3);

	_btnSwitchToHO = new CObject();
	_btnSwitchToHO->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNSWITCHTOHO.SPR", 673, 20, 38);
	_btnSwitchToHO->SetUID("btnSwitchToHO");
	HOScene->PushObject(_btnSwitchToHO);

	//_btnMatch3 = HOScene->GetObjectByID("btnMatch3");
	_btnMatch3->PlayAnimation(0);
	//_btnSwitchToHO = HOScene->GetObjectByID("btnSwitchToHO");
	_btnSwitchToHO->Show(false);
	_btnSwitchToHO->SetZ(eZOrder_PopupOverPopup + 300);

	Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);//lock in HO anim
	Hud->IsHOScene = true;
	Hud->HOState = HOActive;

	InitPolygonArray();
	InitHO();

	_gameState = L5S3HO_State_FoundObject;

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L5_S3_HOUnderwater]  = true;
}

void L5_S3_HOUnderwater::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Control::Audio->IsPlaying(aTrackL5theme) )
	{
		Control::Audio->Samples[aTrackL5theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL5theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL5theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL5ho) )
				  Control::Audio->PlaySample(aTrackL5ho, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL5ho))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL5ho, true);
		}
	}
}

void L5_S3_HOUnderwater::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);
	if(!_btnMatch3->Visible && !ProfileSystem::Instance->ActiveProfile.L5_S3_HOUnderWaterCompleted)
	{
		match3Back->Show(true);
		match3BackText->Show(true);
	}
	else
	{
		match3Back->Show(false);
		match3BackText->Show(false);
	}
	if(ProfileSystem::Instance->ActiveProfile.L5_S3_HOUnderWaterCompleted)
	{
		match3Back->Show(false);
		match3BackText->Show(false);
		_btnMatch3->Show(false);
		_btnSwitchToHO->Show(false);
	}
	int size = (int)_foundObjs.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		_foundObjs[i]->Update();
	}

	_btnSwitchToHO->PlayAnimation(0);
	_btnMatch3->PlayAnimation(0);

	if (_gameState < L5S3HO_State_Match3Completed && Match3BtnRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_HandPointy);

		if (ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
		{
			_btnSwitchToHO->PlayAnimation(1);

			if (Control::Input->LBclicked())
			{
				_match3Ctrl->clearBoard();
				_btnMatch3->Show(true);
				_btnSwitchToHO->Show(false);
				_countText->SetEnable(true);

				for (int i = 0; i < (int)(_hiddenObjs.size()); i++)
				{
					_hiddenObjs[i]->SwitchHO();
				}

				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
				Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
				_gameState = L5S3HO_State_StopMatch3;
			}
		}
		else
		{
			_btnMatch3->PlayAnimation(1);

			if (Control::Input->LBclicked())
			{
				_btnMatch3->Show(false);
				_btnSwitchToHO->Show(true);
				_countText->SetEnable(false);

				if (!_match3Ctrl)
				{
					_match3Ctrl = new Match3(8);
					AddControl(_match3Ctrl);
				}
				else
				{
					_match3Ctrl->ShowBoard();
				}

				for (int i = 0; i < (int)(_hiddenObjs.size()); i++)
				{
					_hiddenObjs[i]->SwitchMatch3();
				}
				
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = true;
				Hud->InvBoxCtrl->SetLockBase(MATCH3_FRAME);
				_gameState = L5S3HO_State_StartMatch3;
			}
		}

	}

	switch (_gameState)
	{
	case L5S3HO_State_Initilizing:
		{
			_gameState = L5S3HO_State_GameStarted;
		}
		break;

	case L5S3HO_State_GameStarted:
		{
			_gameState = L5S3HO_State_WaitForInteraction;
		}
		break;

	case L5S3HO_State_WaitForInteraction:
		{
			if (_starFish->pSprite->Alpha == 1.0f && _startFishPoly->Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if( Control::Input->LBclicked() )
				{
					_starFish->TweenPosTo(-50, 65, -1, 1, 0, true);
					ProfileSystem::Instance->ActiveProfile.L5_S3_HOUnderWaterStarRemoved = true;
				}
			}

			if( Control::Input->LBclicked() )
			{
				_gameState = L5S3HO_State_OnInteraction;			
			}
		}
		break;

	case L5S3HO_State_OnInteraction:
		{
			bool stateChange = false;

			for (int i = 0; i < NUMBER_OF_HO; i++)
			{
				if (_hiddenObjs[i]->MouseClicked(pos))
				{
					_foundObjs.push_back(_hiddenObjs[i]);
					ProfileSystem::Instance->ActiveProfile.L5_S3_HOUnderWaterFoundObjs[i] = true;
					_gameState = L5S3HO_State_FoundObject;
					stateChange = true;

					Control::Audio->QuickLoadAndPlaySFX("s3ho_itemtake");
					break;
				}
			}

			if (!stateChange)
			{
				_gameState = L5S3HO_State_WaitForInteraction;
			}
		}
		break;

	case L5S3HO_State_OnHint:
		{
			_gameState = L5S3HO_State_WaitForInteraction;
		}
		break;

	case L5S3HO_State_FoundObject:
		{
			string txt = StringTable::getStr(InvBoxText) + MKSTR((int)_foundObjs.size()) + "/" + MKSTR((int)_hiddenObjs.size());
			_countText->SetText(txt);

			if (_foundObjs.size() == _hiddenObjs.size())
			{
				_gameState = L5S3HO_State_FoundAllObjects;
			}
			else
			{
				_gameState = L5S3HO_State_WaitForInteraction;
			}
		}
		break;

	case L5S3HO_State_FoundAllObjects:
		{
			_gameState++; // waiting to complete the animation
		}
		break;

	case L5S3HO_State_Match3Completed:
	case L5S3HO_State_HOCompleted:
		{
			AwardManager::GetInstance()->CompleteHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			if (_handleExtrasGameComplete(false))
			{
				_gameState = State_L5S3GameInExtrasComplete;
				return;
			}

			_countText->Show(false);
			Hud->InvBoxCtrl->ResetToInvBox();
			Hud->IsHOScene = false;
			Hud->TakeInventoryItem(eInv_L5_S3_Hook);
			for (int i = 0; i < (int)(_hiddenObjs.size()); i++)
			{
				_hiddenObjs[i]->SwitchMatch3();
			}

			ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S3_Pond] = true;
			ProfileSystem::Instance->ActiveProfile.L5_S3_HOUnderWaterCompleted = true;
			ProfileSystem::Instance->ActiveProfile.L5_S3_PZHaveHook = true;
			_gameState = L5S3HO_State_GameComplete;
		}
		break;

	case L5S3HO_State_StartMatch3:
		{
			if (_match3Ctrl->getIsCompleted())
			{
				for (int i = 0; i < (int)(_hiddenObjs.size()); i++)
				{
					_hiddenObjs[i]->SwitchHO();
				}

				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
				Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
				_gameState = L5S3HO_State_Match3Completed;
			}
		}

		break;

	case L5S3HO_State_StopMatch3:
		_gameState = L5S3HO_State_WaitForInteraction;
		break;

	case L5S3HO_State_GameComplete:
		break;

	case State_L5S3GameInExtrasComplete:
		break;

	default:
		_gameState++;
		break;
	}
		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
			{
				if(_match3Ctrl!=NULL)
					_match3Ctrl->clearBoard();

				_btnMatch3->Show(false);
				_btnSwitchToHO->Show(false);
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
			}
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S3_Pond;
			SendMessage(ControlExit, ID, HOSM_NewArea);										
		}	
		#endif
	if(match3Back->IsIntersect(pos) && !ProfileSystem::Instance->_isExtrasGamePlay)
	{
		if( Control::Input->LBclicked() )
		{
			if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
			{
				if(_match3Ctrl!=NULL)
					_match3Ctrl->clearBoard();

				_btnMatch3->Show(false);
				_btnSwitchToHO->Show(false);
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
			}
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S3_Pond;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
	}
	if( IsMouseOverGoBackArea(&pos)
		#ifdef M3_BUTTON
		&& _btnMatch3->Visible
        #endif
        )
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L5_S3_Pond);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L5_S3_Pond);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L5_S3_Pond, 2);
			}
#else			
			if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
			{
				if(_match3Ctrl!=NULL)
					_match3Ctrl->clearBoard();

				_btnMatch3->Show(false);
				_btnSwitchToHO->Show(false);
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
			}
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S3_Pond;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
								
		}	
	}
}

void L5_S3_HOUnderwater::ReceiveMessage(int val)
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
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S3_Pond;
		SendMessage(ControlExit, ID, HOSM_NewArea);
	}
}

void L5_S3_HOUnderwater::ProcessSkip()
{
}

void L5_S3_HOUnderwater::ProcessNonHOHint()
{
	if (_gameState == L5S3HO_State_StartMatch3)
	{
		return;
	}

	
	int iNonHOHint = 0; 
	int MaxNonHOHints = 8;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
			break;

		case 0: 
			{
				++iNonHOHint;

				int size = (int)_hiddenObjs.size();  //Sac: converted Implicit to explicit
				for (int i = 0; i < size; i++)
				{
					if (!_hiddenObjs[i]->getIsFound())
					{
						_gameState = L5S3HO_State_OnHint;
						InitHintTrailEmit(_hiddenObjs[i]->getToFind()->Pos.x, _hiddenObjs[i]->getToFind()->Pos.y);
						AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
						return;
					}
				}
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

L5_S3_HOUnderwater::~L5_S3_HOUnderwater()
{
	for (int i = 0; i < NUMBER_OF_HO; i++)
	{
		SAFE_DELETE(_hiddenObjs[i]);
	}

	_hiddenObjs.clear();
	_foundObjs.clear();

	SAFE_DELETE(S3Bubble1FX);
	SAFE_DELETE(S3Bubble2FX);
	SAFE_DELETE(S3Bubble3FX);

	Hud->InvBoxCtrl->ResetToInvBox();
	SAFE_DELETE(_countText);

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL5s3UnderwaterHOAmb);
	Control::Audio->UnloadSample(aAmbL5s3UnderwaterHOAmb);
	//MUSIC_SFX
}

void L5_S3_HOUnderwater::InitHO()
{
	for (int i = 0; i < NUMBER_OF_HO; i++)
	{
		CObject* toFind = HOScene->GetObjectByID(std::string("hidden_object_")+MKSTR(i+1));
		CObject* completed = HOScene->GetObjectByID(std::string("complete_object_")+MKSTR(i+1));

		L5S3HiddenObject *obj = new L5S3HiddenObject();
		obj->setIndex(i);
		obj->setName(L5S3ItemNames[i]);
		obj->setToFind(toFind);
		obj->setCompleteObject(completed);
		obj->setPolygonArea(HOPolygonArray[i]);
		_hiddenObjs.push_back(obj);

		if (ProfileSystem::Instance->ActiveProfile.L5_S3_HOUnderWaterFoundObjs[i])
		{
			obj->AlreadyFound();
			_foundObjs.push_back(obj);
			continue;
		}
		
		obj->setClicksToFind(NumClicks[i]);
		obj->InitObject();
	}

	CRectangle InvAreaRect(368.0f,664.0f,624.0f,96.0f);
	CPoint pos(InvAreaRect.GetCenterX(), InvAreaRect.GetCenterY(), 0);

	ColorRGB gold = {(233/255.0f),(204/255.0f),(131/255.0f)};
	_countText = new CBitmapText();
	_countText->SetTextProps("", pos.x, pos.y, eZOrder_INVBOXITEMS, gold, eFont_25, Align_Center);
	_countText->SetZ(eZOrder_INVBOXITEMS);
}

void L5_S3_HOUnderwater::InitPolygonArray()
{
	int PtsAr1[] = {680,429,669,411,675,410,690,386,735,386,751,371,746,365,760,354,795,379,784,392,775,388,762,397,755,391,739,402,731,393,705,394,689,410,696,412,683,431,689,437,707,430,706,444,692,438};
	CPolygon *CPolyObj1=  new CPolygon(23);
	CPolyObj1->Init(PtsAr1);
	HOPolygonArray[0] = CPolyObj1;

	int PtsAr2[] = {781,216,791,212,794,194,812,182,819,188,829,193,839,182,856,171,867,157,854,151,852,153,830,137,819,150,827,154,820,167,825,174,803,177,789,186,780,195};
	CPolygon *CPolyObj2=  new CPolygon(19);
	CPolyObj2->Init(PtsAr2);
	HOPolygonArray[1] = CPolyObj2;

	int PtsAr3[] = {917,606,906,615,867,605,868,589,851,578,851,569,841,562,832,537,803,526,783,537,780,528,796,515,824,519,843,539,851,553,864,563,869,557,875,563,883,548,902,554,896,568,884,590};
	CPolygon *CPolyObj3=  new CPolygon(22);
	CPolyObj3->Init(PtsAr3);
	HOPolygonArray[2] = CPolyObj3;

	int PtsAr4[] = {1052,443,1057,455,1089,467,1094,455,1128,464,1146,449,1162,443,1183,459,1184,476,1171,482,1145,497,1171,503,1182,493,1194,477,1192,440,1164,429,1137,440,1101,427,1084,447};
	CPolygon *CPolyObj4=  new CPolygon(19);
	CPolyObj4->Init(PtsAr4);
	HOPolygonArray[3] = CPolyObj4;

	int PtsAr5[] = {389,586,359,577,359,582,338,576,333,548,346,533,364,529,382,534,386,546,398,548,409,540,434,530,414,506,398,514,403,522,396,526,378,516,348,520,329,532,321,547,322,571,330,585,353,596,386,586};
	CPolygon *CPolyObj5=  new CPolygon(24);
	CPolyObj5->Init(PtsAr5);
	HOPolygonArray[4] = CPolyObj5;

	int PtsAr6[] = {257,489,253,474,261,474,253,456,230,442,231,414,224,392,218,393,205,369,215,354,222,378,244,371,249,384,251,412,239,425,254,444,273,464,269,478,273,481};
	CPolygon *CPolyObj6=  new CPolygon(19);
	CPolyObj6->Init(PtsAr6);
	HOPolygonArray[5] = CPolyObj6;

	int PtsAr7[] = {204,252,210,236,225,236,220,255,232,257,238,228,232,226,232,205,213,194,218,171,230,151,234,152,251,143,231,132,207,156,211,165,203,173,200,193,210,202,215,209,210,222,198,222};
	CPolygon *CPolyObj7=  new CPolygon(22);
	CPolyObj7->Init(PtsAr7);
	HOPolygonArray[6] = CPolyObj7;

	int PtsAr8[] = {496,239,515,244,532,232,519,224,508,227,501,219,494,207,481,210,487,188,467,185,479,170,462,157,454,166,463,170,455,188,478,196,467,210,476,221};
	CPolygon *CPolyObj8=  new CPolygon(18);
	CPolyObj8->Init(PtsAr8);
	HOPolygonArray[7] = CPolyObj8;

	int PtsAr9[] = {542,146,546,157,548,185,568,197,595,199,608,176,596,142,587,154,591,176,575,185,561,176,562,157,549,134};
	CPolygon *CPolyObj9=  new CPolygon(13);
	CPolyObj9->Init(PtsAr9);
	HOPolygonArray[8] = CPolyObj9;

	int PtsAr10[] = {1096,147,1085,146,1102,134,1120,136,1121,123,1115,104,1130,68,1144,68,1134,84,1154,99,1164,85,1176,92,1162,125,1151,121,1137,144,1128,149};
	CPolygon *CPolyObj10=  new CPolygon(16);
	CPolyObj10->Init(PtsAr10);
	HOPolygonArray[9] = CPolyObj10;

	int PtsAr11[] = {428,341,443,339,459,331,480,325,497,325,533,315,544,301,531,300,512,309,492,315,472,315,459,323,429,320,431,305,400,315,399,324,407,323,413,331};
	CPolygon *CPolyObj11=  new CPolygon(18);
	CPolyObj11->Init(PtsAr11);
	HOPolygonArray[10] = CPolyObj11;

	int PtsAr12[] = {920,338,909,349,888,353,870,338,870,310,885,290,900,293,902,277,921,252,930,259,921,268,930,273,935,261,947,265,934,292,925,286,921,300,912,309,896,308,885,313,884,326,894,336,917,329};
	CPolygon *CPolyObj12=  new CPolygon(23);
	CPolyObj12->Init(PtsAr12);
	HOPolygonArray[11] = CPolyObj12;

	_startFishPoly = CPolyObj12;
}
void L5_S3_HOUnderwater::NavigateToScene()
{
	if(GFHud->InvBoxCtrl->navTapped)
	{
		if( Control::Input->LBclicked() )
		{
			if(Control::Input->Mpos().x > GFHud->InvBoxCtrl->navRectX2 && Control::Input->Mpos().x < GFHud->InvBoxCtrl->navRectX1 && Control::Input->Mpos().y > GFHud->InvBoxCtrl->navRectY2 && Control::Input->Mpos().y < GFHud->InvBoxCtrl->navRectY1)
			{	
				//Cursor::SetMode(CA_ExitUp);
				if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
				{
					if(_match3Ctrl!=NULL)
						_match3Ctrl->clearBoard();

					_btnMatch3->Show(false);
					_btnSwitchToHO->Show(false);
					ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
				}
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