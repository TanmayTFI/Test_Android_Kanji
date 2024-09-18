//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L5_S5_HOGanapathi.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "Util.h"
#include "MeshObject.h"

#define NUMBER_OF_HO 14

const char* L5S5ItemNames[] = {"Leather_Bag",
						   "Tea_Kettle",
						   "Tiffin_Box",
						   "Hand_Bell",
						   "Compass",
						   "Handmirror",
						   "Kerosene_Lamp",
						   "Key",
						   "Mace",
						   "Oil_Lamp",
						   "Crown",
						   "Metal_Chalice"};

const CRectangle  Match3BtnRect(550.0f,1.0f,252.0f,39.0f);

L5S5HiddenObject::L5S5HiddenObject()
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
	_isMuliHO = false;
	_isMultiHOComplete = false;
}

L5S5HiddenObject::~L5S5HiddenObject()
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

void L5S5HiddenObject::AddToMultiHO(L5S5HiddenObject *obj)
{
	_isMuliHO = true;
	_aliasList.push_back(obj);
	obj->_nameText->SetEnable(false);
}

void L5S5HiddenObject::InitObject()
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

void L5S5HiddenObject::InitInvNameText()
{
	CRectangle InvAreaRect(368.0f,670.0f,624.0f,96.0f);
	float vertGap = 30, horizontalGap = InvAreaRect.w/4.0f;
	CPoint pos(0,0,0);

	pos.x = InvAreaRect.Left() + horizontalGap*0.5f + horizontalGap*(_index%4);
	pos.y = InvAreaRect.Top() + vertGap*0.5f + vertGap*(_index/4);

	ColorRGB gold = {(233/255.0f),(204/255.0f),(131/255.0f)};
	_nameText = new CBitmapText();
	_nameText->SetTextProps(_name, pos.x, pos.y, eZOrder_INVBOXITEMS, gold, eFont_Small, Align_Center);
	_nameText->SetZ(eZOrder_INVBOXITEMS);
}


bool L5S5HiddenObject::MouseClicked(CPoint pos)
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

void L5S5HiddenObject::OnFound()
{
	_toFind->FadeOut(3.0f);
	_completeObject->SetEnable(true);
	_completeObject->FadeIn(4.0f);
	_completeObject->SetZ(eZOrder_INVBOXITEMS);
	_isFound = true;
	waterHOFX->Start();
	_isObjectScaling = true;
}

void L5S5HiddenObject::Update()
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
			if (!_isMuliHO)
				_nameText->SetStrikethrough(true);
			_completeObject->SetEnable(false);
			_isAnimComplete = true;
		}
	}
}

void L5S5HiddenObject::AlreadyFound()
{
	_isFound = true;
	_isAnimComplete = true;
	_toFind->SetEnable(false);
	_completeObject->SetEnable(false);
	InitInvNameText();
	_nameText->SetAlpha(0.5f);
}

void L5S5HiddenObject::UpdateAlias()
{
	if (_isMultiHOComplete)
		return;

	int totalCount = _isFound;
	for (int i = 0; i < (int)(_aliasList.size()); i++)
	{
		if (_aliasList[i]->_isFound)
			totalCount++;
	}

	string multiNameType = _name + "(" + MKSTR(totalCount) + "/" + MKSTR(_aliasList.size() + 1) + ")";
	_nameText->SetText(multiNameType);

	if (totalCount == (int)(_aliasList.size() + 1)) // + 1 for this object
	{
		_isMultiHOComplete = true;
		_nameText->SetStrikethrough(true);
	}
}

void L5S5HiddenObject::SwitchMatch3()
{
	_nameText->Show(false);
}

void L5S5HiddenObject::SwitchHO()
{
	_nameText->Show(true);
}

L5_S5_HOGanapathi::L5_S5_HOGanapathi()
{
	AwardManager::GetInstance()->StartHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV5\\S5\\HO\\HO.SCN");

		//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL5s5MoutainBelowAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL5s5MoutainBelowAmb) )		
	{
		Control::Audio->PlaySample(aAmbL5s5MoutainBelowAmb,true);
	}
	
	//MUSIC_SFX
	
	

	CObject* ray1 = HOScene->GetObjectByID("ray1");
	ray1->SetScale(3.70f);
	ray1->Animate(0.4f,0.99f, 5, 0, eBlend_AlphaAdditive);

	CObject* ray2 = HOScene->GetObjectByID("ray2");
	ray2->SetScale(3.30f);
	ray2->Animate(0.4f,0.99f, 5, 6, eBlend_AlphaAdditive);

	CObject* plant1 = HOScene->GetObjectByID("plant_overlap_rt_1");
	plant1->SetPivot(0, 125);
	plant1->Animate(0,0,5,0,6);

	MeshObject* plant2 = new MeshObject(HOScene->GetObjectByID("plant_overlap"), 3, 3);
	plant2->meshFX->SetSpeedAndDisplacement(3, 6, 6, 2);
	HOScene->PushObject(plant2);

	MeshObject* plant3 = new MeshObject(HOScene->GetObjectByID("plant_overlap_rt_2_Mesh"), 3, 3);
	plant3->meshFX->SetSpeedAndDisplacement(3, 5, 4, 1);
	HOScene->PushObject(plant3);

	s5HOSmokeFgFX = new SuperFX("PRT\\L5\\s5HOSmokeFg.INI", PointSystem::CreateCPoint(443,68,0), 1100); 
	s5HOSmokeFgFX->Start();
	s5HOSmokeFgFX->AdvanceByTime(2.0f);

	s5HOLightFX = new SuperFX("PRT\\L5\\s5HOLight.INI", PointSystem::CreateCPoint(657,116,0), 1100); 
	s5HOLightFX->Start();
	s5HOLightFX->AdvanceByTime(2.0f);

	ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
	_match3Ctrl = NULL;

	match3Back = new CObject();
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
	_btnMatch3->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNMATCH3.SPR", 680, 21, 34);
	_btnMatch3->SetUID("btnMatch3");
	HOScene->PushObject(_btnMatch3);

	_btnSwitchToHO = new CObject();
	_btnSwitchToHO->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNSWITCHTOHO.SPR", 680, 21, 35);
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

	_gameState = L5S5HO_State_Initilizing;


	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L5_S5_HOGanapathi]  = true;
}

void L5_S5_HOGanapathi::Required()
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

void L5_S5_HOGanapathi::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);
	if(!_btnMatch3->Visible && !ProfileSystem::Instance->ActiveProfile.L5_S5_HOGanpathiCompleted)
	{
		match3Back->Show(true);
		match3BackText->Show(true);
	}
	else
	{
		match3Back->Show(false);
		match3BackText->Show(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L5_S5_HOGanpathiCompleted)
	{
		match3Back->Show(false);
		match3BackText->Show(false);
		_btnMatch3->Show(false);
		_btnSwitchToHO->Show(false);
	}

	for (int i = 0; i < (int)(_foundObjs.size()); i++)
	{
		_foundObjs[i]->Update();
	}

	_btnSwitchToHO->PlayAnimation(0);
	_btnMatch3->PlayAnimation(0);

	if (_gameState < L5S5HO_State_Match3Completed && Match3BtnRect.Intersection(&pos))
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

				for (int i = 0; i < (int)(_hiddenObjs.size()); i++)
				{
					_hiddenObjs[i]->SwitchHO();
				}

				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
				Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
				_gameState = L5S5HO_State_StopMatch3;
			}
		}
		else
		{
			_btnMatch3->PlayAnimation(1);

			if (Control::Input->LBclicked())
			{
				_btnMatch3->Show(false);
				_btnSwitchToHO->Show(true);

				if (!_match3Ctrl)
				{
					_match3Ctrl = new Match3(15);
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
				_gameState = L5S5HO_State_StartMatch3;
			}
		}

	}

	switch (_gameState)
	{
	case L5S5HO_State_Initilizing:
		{
			for (int i = 0; i < (int)(_multiObjs.size()); i++)
			{
				_multiObjs[i]->UpdateAlias();
			}

			if (_hiddenObjs.size() == _foundObjs.size())
			{
				_gameState = L5S5HO_State_HOCompleted;
			}
			else
			{
				_gameState = L5S5HO_State_GameStarted;
			}
		}
		break;

	case L5S5HO_State_GameStarted:
		{
			_gameState = L5S5HO_State_WaitForInteraction;
		}
		break;

	case L5S5HO_State_WaitForInteraction:
		{
			if( Control::Input->LBclicked() )
			{
				_gameState = L5S5HO_State_OnInteraction;			
			}
		}
		break;

	case L5S5HO_State_OnInteraction:
		{
			bool stateChange = false;

			for (int i = 0; i < NUMBER_OF_HO; i++)
			{
				if (_hiddenObjs[i]->MouseClicked(pos))
				{
					Control::Audio->QuickLoadAndPlaySFX("s5ho_itemtake");

					_foundObjs.push_back(_hiddenObjs[i]);
					ProfileSystem::Instance->ActiveProfile.L5_S5_HOGanpathiFoundObjs[i] = true;
					_gameState = L5S5HO_State_FoundObject;
					stateChange = true;
					break;
				}
			}

			if (!stateChange)
			{
				_gameState = L5S5HO_State_WaitForInteraction;
			}
		}
		break;

	case L5S5HO_State_OnHint:
		{
			_gameState = L5S5HO_State_WaitForInteraction;
		}
		break;

	case L5S5HO_State_FoundObject:
		{
			for (int i = 0; i < (int)(_multiObjs.size()); i++)
			{
				_multiObjs[i]->UpdateAlias();
			}

			if (_foundObjs.size() == _hiddenObjs.size())
			{
				_gameState = L5S5HO_State_FoundAllObjects;
			}
			else
			{
				_gameState = L5S5HO_State_WaitForInteraction;
			}
		}
		break;

	case L5S5HO_State_FoundAllObjects:
		{
			_gameState++; // waiting to complete the animation
		}
		break;

	case L5S5HO_State_Match3Completed:
	case L5S5HO_State_HOCompleted:
		{
			AwardManager::GetInstance()->CompleteHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			if (_handleExtrasGameComplete(false))
			{
				_gameState = L5S5HO_State_GameForExtrasComplete;
				return;
			}

			Hud->InvBoxCtrl->ResetToInvBox();
			Hud->IsHOScene = false;
			Hud->TakeInventoryItem(eInv_L5_S5_DivingBag);
			Hud->ShowBannerText(Banner_L5_S3_waterproofBagCollect);
			_invItemId = eInv_L5_S5_DivingBag;
			for (int i = 0; i < (int)(_hiddenObjs.size()); i++)
			{
				_hiddenObjs[i]->SwitchMatch3();
			}

			if(ProfileSystem::Instance->ActiveProfile.L5_S3_PZState == 61) // waiting for hook
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S3_Pond] = true;
				
			ProfileSystem::Instance->ActiveProfile.L5_S5_InvWaterproofBagTaken = true;
			ProfileSystem::Instance->ActiveProfile.L5_S5_HOGanpathiCompleted = true;
			_gameState = L5S5HO_State_GameComplete;
		}
		break;

	case L5S5HO_State_StartMatch3:
		{
			if (_match3Ctrl->getIsCompleted())
			{
				for (int i = 0; i < (int)(_hiddenObjs.size()); i++)
				{
					_hiddenObjs[i]->SwitchHO();
				}

				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
				Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
				_gameState = L5S5HO_State_Match3Completed;
			}
		}

		break;

	case L5S5HO_State_StopMatch3:
		_gameState = L5S5HO_State_WaitForInteraction;
		break;

	case L5S5HO_State_GameComplete:
		break;

	case L5S5HO_State_GameForExtrasComplete:
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

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S5_MountainBelow;
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

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S5_MountainBelow;
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
	//		Cursor::SetMode(CA_ExitDown,eArea_L5_S5_MountainBelow);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L5_S5_MountainBelow);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L5_S5_MountainBelow, 2);
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

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S5_MountainBelow;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
			
		}	
	}
}

void L5_S5_HOGanapathi::ReceiveMessage(int val)
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
		if (_invItemId == eInv_L5_S5_DivingBag)
		{
			Hud->TakeInventoryItem(eInv_L5_S5_Keyhandcuff);
			ProfileSystem::Instance->ActiveProfile.L5_S5_InvHandcuffKeyTaken = true;
			_invItemId = eInv_L5_S5_Keyhandcuff;
		}
		else
		{
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S5_MountainBelow;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
	}
}

void L5_S5_HOGanapathi::ProcessSkip()
{
}

void L5_S5_HOGanapathi::ProcessNonHOHint()
{
	if (_gameState == L5S5HO_State_StartMatch3)
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
						_gameState = L5S5HO_State_OnHint;
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

L5_S5_HOGanapathi::~L5_S5_HOGanapathi()
{
	for (int i = 0; i < NUMBER_OF_HO; i++)
	{
		SAFE_DELETE(_hiddenObjs[i]);
	}

	_hiddenObjs.clear();
	_foundObjs.clear();

	Hud->InvBoxCtrl->ResetToInvBox();
	SAFE_DELETE(s5HOSmokeFgFX);
	SAFE_DELETE(s5HOLightFX);

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL5s5MoutainBelowAmb);
	Control::Audio->UnloadSample(aAmbL5s5MoutainBelowAmb);
	//MUSIC_SFX
}

void L5_S5_HOGanapathi::InitHO()
{
	int j = 0;
	int nameTxtIndex = 0;
	L5S5HiddenObject *original = NULL;;

	for (int i = 0; i < NUMBER_OF_HO; i+=j)
	{
		j = 0;
		do
		{
			j++;
			CObject* toFind = HOScene->GetObjectByID(std::string("hidden_object_")+MKSTR(i+1)+"_"+MKSTR(j));
			CObject* completed = HOScene->GetObjectByID(std::string("complete_object_")+MKSTR(i+1)+"_"+MKSTR(j));
			
			if (!toFind)
			{
				j--;
				break;
			}

			L5S5HiddenObject *obj = new L5S5HiddenObject();
			obj->setIndex(i);
			obj->setName(StringTable::getStr(L5S5ItemNames[nameTxtIndex]));
			obj->setToFind(toFind);
			obj->setCompleteObject(completed);
			obj->setPolygonArea(HOPolygonArray[i + j - 1]);

			if (ProfileSystem::Instance->ActiveProfile.L5_S5_HOGanpathiFoundObjs[i + j - 1])
			{
				obj->AlreadyFound();
				_foundObjs.push_back(obj);
			}
			else
			{
				obj->InitObject();
			}

			_hiddenObjs.push_back(obj);

			if (original)
			{
				if (!original->getIsMultiHO())
					_multiObjs.push_back(original);

				original->AddToMultiHO(obj);
			}

			if (j == 1)
				original = obj;

		} while (true);

		nameTxtIndex++;
		original = NULL;
	}
}

void L5_S5_HOGanapathi::InitPolygonArray()
{
int PtsAr1[] = {165,589,397,474,706,551,643,549,541,583,498,676};
CPolygon *CPolyObj1=  new CPolygon(6);
CPolyObj1->Init(PtsAr1);
HOPolygonArray[0] = CPolyObj1;

int PtsAr2[] = {867,421,783,439,753,426,735,385,748,379,761,391,780,373,805,353,814,336,830,356,852,367,860,378,880,375,894,408};
CPolygon* CPolyObj2=  new CPolygon(14);
CPolyObj2->Init(PtsAr2);
HOPolygonArray[1] = CPolyObj2;

int PtsAr3[] = {816,239,818,282,860,295,907,281,902,237};
CPolygon* CPolyObj3=  new CPolygon(5);
CPolyObj3->Init(PtsAr3);
HOPolygonArray[2] = CPolyObj3;

int PtsAr4[] = {546,546,567,590,598,568,623,567,631,555,685,550,687,535,632,533,623,520,594,514,568,490};
CPolygon* CPolyObj4=  new CPolygon(11);
CPolyObj4->Init(PtsAr4);
HOPolygonArray[3] = CPolyObj4;

int PtsAr5[] = {824,580,856,591,857,601,886,598,886,582,898,568,892,541,855,528,822,551};
CPolygon* CPolyObj5=  new CPolygon(9);
CPolyObj5->Init(PtsAr5);
HOPolygonArray[4] = CPolyObj5;

int PtsAr6[] = {650,229,649,193,670,158,702,187,707,241,695,261,697,306,683,312,677,265};
CPolygon* CPolyObj6=  new CPolygon(9);
CPolyObj6->Init(PtsAr6);
HOPolygonArray[5] = CPolyObj6;

int PtsAr7[] = {732,515,800,584,820,567,809,512,762,431,736,430,727,413,719,419,726,428,709,451};
CPolygon* CPolyObj7=  new CPolygon(10);
CPolyObj7->Init(PtsAr7);
HOPolygonArray[6] = CPolyObj7;

int PtsAr8[] = {845,272,830,235,848,215,874,262,866,283};
CPolygon* CPolyObj8=  new CPolygon(5);
CPolyObj8->Init(PtsAr8);
HOPolygonArray[7] = CPolyObj8;

int PtsAr9[] = {313,629,349,579,464,511,458,492,332,531,275,514,216,548,215,602,268,603};
CPolygon* CPolyObj9=  new CPolygon(9);
CPolyObj9->Init(PtsAr9);
HOPolygonArray[8] = CPolyObj9;

int PtsAr10[] = {173,408,249,432,347,404,357,370,288,347,283,333,331,318,330,295,278,290,276,271,265,261,270,225,247,189,224,219,232,238,205,266,203,291,176,306,154,389,127,414,123,426,37,433,37,445,127,446,165,430};
CPolygon* CPolyObj10=  new CPolygon(25);
CPolyObj10->Init(PtsAr10);
HOPolygonArray[9] = CPolyObj10;

int PtsAr11[] = {589,223,592,191,609,168,642,153,661,152,676,172,688,175,695,184,685,192,715,229,686,239,655,242,654,227,633,217};
CPolygon* CPolyObj11=  new CPolygon(14);
CPolyObj11->Init(PtsAr11);
HOPolygonArray[10] = CPolyObj11;

int PtsAr12[] = {641,370,650,357,653,342,640,332,642,294,634,288,656,276,686,287,675,299,679,326,668,344,665,357,679,369,662,381};
CPolygon* CPolyObj12=  new CPolygon(14);
CPolyObj12->Init(PtsAr12);
HOPolygonArray[11] = CPolyObj12;

int PtsAr13[] = {1054,418,1064,467,1089,483,1089,502,1079,506,1079,515,1111,502,1105,486,1124,456,1116,405};
CPolygon* CPolyObj13=  new CPolygon(10);
CPolyObj13->Init(PtsAr13);
HOPolygonArray[12] = CPolyObj13;

int PtsAr14[] = {400,312,409,324,410,364,430,374,430,394,423,406,468,403,455,385,451,367,469,356,464,302};
CPolygon* CPolyObj14=  new CPolygon(11);
CPolyObj14->Init(PtsAr14);
HOPolygonArray[13] = CPolyObj14;
}


void L5_S5_HOGanapathi::NavigateToScene()
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