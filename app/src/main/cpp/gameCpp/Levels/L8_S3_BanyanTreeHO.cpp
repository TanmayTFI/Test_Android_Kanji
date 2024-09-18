
//====================================
//  created by : Jithin	
//  copyright  : Tuttifrutti Games
//====================================

#include "L8_S3_BanyanTreeHO.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "Util.h"

const CRectangle  Match3BtnRect(550.0f,1.0f,252.0f,39.0f);

const CRectangle  BackRect1(354.0f,584.0f,96.0f,30.0f);
const CRectangle  BackRect2(606.0f,596.0f,102.0f,38.0f);
const CRectangle  BackRect3(852.0f,596.0f,83.0f,63.0f);
const CRectangle  BackDissableRects[] = { BackRect1, BackRect2, BackRect3};

namespace Level8_Scene3					  
{										  
	HiddenObject::HiddenObject()
	{
		_isActive = false;
		_isFound = false;
		_isCompleted = false;
		_overlappedObj.clear();
		_HUDhideDelay = 0.0f;
		_completeTimer = 0.0f;
	}

	HiddenObject::~HiddenObject()
	{
		SAFE_DELETE(_polyArea);
	}

	void HiddenObject::setHUDObj(CObject* obj, SuperFX* completeParticleHUD)
	{
		_hudObj = obj;
		_hudObj->ZOrder = eZOrder_INVBOXITEMS;

		_completeParticleHUD = completeParticleHUD;
		_completeParticleHUD->ZOrder = eZOrder_INVBOXITEMS+1;
	}

	void HiddenObject::AddOverlapObj(CObject* obj)
	{
		_overlappedObj.push_back(obj);
	}

	UserClick HiddenObject::OnClickAtPoint(CPoint pos)
	{
		if (_isFound)
			return Click_None;

		if (!_overlappedObj.size())
		{
			if (_isActive && _polyArea->Intersection(&pos))
			{
				OnFound();
				return Click_HiddenObj;
			}
		}
		else
		{
			if (_overlapedArea.Intersection(&pos))
			{
				float xoff = 30.0f * (float)pow(-1, (rand()%2 + 1));
				float yoff = 30.0f;
				_overlappedObj.back()->TweenPosTo(xoff, yoff, -1, 1, 0, true);
				_overlappedObj.pop_back();
				return Click_OverlapObj;
			}
		}

		return Click_None;
	}

	void HiddenObject::OnFound()
	{
		_isFound = true;
		float arr1[] = {0.45f, -1, -1, -1, -1};
		_hudObj->TweenTo(arr1, 1, 0, true);
		
		_completeParticle->SetPos(&_hiddenObj->Pos);
		_completeParticle->Start();
		_completeParticleHUD->SetPos(&_hudObj->Pos);
		_completeParticleHUD->Start();
		_completeTimer = 1.0f;

		//particle
	}

	void HiddenObject::Update(float ds)
	{
		if (_HUDhideDelay > 0)
		{
			_HUDhideDelay -= ds;
			if (_HUDhideDelay <= 0.0f)
			{
				HideHUDLeaf();
				_HUDhideDelay = 0.0f;
			}
		}

		if(_isFound)
			return;

		CPoint mPos = Control::Input->Mpos();
		if (_hudObj->GetWorldRect().Intersection(&mPos))
		{
			_hudObj->SetScale(1.0);
		}
		else
		{
			_hudObj->SetScale(0.8f);
		}
	}

	void HiddenObject::UpdateFoundObj()
	{
		if (_isCompleted)
			return;

		if (_completeTimer > 0.0f)
		{
			_completeTimer -= Control::LogicRate;
			
			if (_completeTimer <= 0.0f)
			{
				_hiddenObj->TweenPosTo(0, -30, -1, 1, 0, true);
				_isCompleted = true;
				_completeTimer = 0.0f;
			}
		}
	}
	
	void HiddenObject::setAlreadyFound()
	{
		_isFound = true;
		_isCompleted = true;

		for (int i = 0; i < (int)(_overlappedObj.size()); i++)
		{
			_overlappedObj[i]->SetEnable(false);
		}

		_hudObj->SetAlpha(0.45f);
		_hudObj->SetScale(0.8f);
		_hiddenObj->SetEnable(false);
	}

	void HiddenObject::setActive(bool active)
	{
		_isActive = active;

		if (_isActive)
		{
			if(!_isFound)
				_hudObj->FadeIn(2);
		}
		else
		{
			_HUDhideDelay = 1.0f;
		}

	}

	void HiddenObject::HideHUDLeaf()
	{
		float arr[] = {0, -1, -1, -1, -1};
		_hudObj->TweenTo(arr, 1, 0, true);
	}

	void  HiddenObject::SetEnable(bool enable)
	{
		_hudObj->SetEnable(enable);
	}


}


L8_S3_BanyanTreeHO::L8_S3_BanyanTreeHO()
{
	//Main Scene
	AwardManager::GetInstance()->StartHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	LoadHud(INVENTORY_V1);
	LoadScene("LV8\\S3\\HO1\\HO1.SCN");

	_hoCompleteDelay = 0.0f;
	s3BanyanTreeHOFX = new SuperFX("PRT\\L8\\s3BanyanTreeHO.INI", PointSystem::CreateCPoint(614,351,0), 308); 
	s3BanyanTreeHOHudLeafFX = new SuperFX("PRT\\L8\\s3BanyanTreeHOHudLeaf.INI", PointSystem::CreateCPoint(472,668,0), 1100); 

	s3BanyanTreeHOLeafFallFX = new SuperFX("PRT\\L8\\s3BanyanTreeHOLeafFall.INI", PointSystem::CreateCPoint(649,-20,0), 1100); 
	s3BanyanTreeHOLeafFallFX->Start();
	s3BanyanTreeHOLeafFallFX->AdvanceByTime(2.0f);

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
	_btnMatch3->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNMATCH3.SPR", 680, 20, 44);
	_btnMatch3->SetUID("btnMatch3");
	HOScene->PushObject(_btnMatch3);

	_btnSwitchToHO = new CObject();
	_btnSwitchToHO->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNSWITCHTOHO.SPR", 680, 20, 45);
	_btnSwitchToHO->SetUID("btnSwitchToHO");
	HOScene->PushObject(_btnSwitchToHO);

	//_btnMatch3 = HOScene->GetObjectByID("btnMatch3");
	_btnMatch3->PlayAnimation(0);
	//_btnSwitchToHO = HOScene->GetObjectByID("btnSwitchToHO");
	_btnSwitchToHO->Show(false);
	_btnSwitchToHO->SetZ(eZOrder_PopupOverPopup + 300);
	
	_hoState = L8S3_HOState_Initializing;
	InitPolyAndRectList();
	InitHOGame();

	ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

	Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);

		//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL8s3TreeHOAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL8s3TreeHOAmb) )		
	{
		Control::Audio->PlaySample(aAmbL8s3TreeHOAmb,true);
	}
	
	//MUSIC_SFX
	

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L8_S3_BanyanTreeHO]  = true;
}

void L8_S3_BanyanTreeHO::InitHOGame()
{ 
	_currentLevel = 0; // for initial state. once OnListUpdate() called it changed to 1.

	CObject* hoObj = NULL;
	int i = 0;
	do
	{
		vector<HiddenObject*> hoList;
		int j = 0;
		hoObj = HOScene->GetObjectByID("HO_Leaf_"+ MKSTR(i) +"_"+ MKSTR(j));
		
		if (!hoObj)
			break;

		std::vector<CRectangle> rectVect = _hoRectList[i];
		std::vector<CPolygon*> polyVect = _hoPolyList[i];

		do
		{
			hoObj = HOScene->GetObjectByID("HO_Leaf_"+ MKSTR(i) +"_"+ MKSTR(j));
			CObject* hudObj = HOScene->GetObjectByID("Hud_Leaf_"+ MKSTR(i) +"_"+ MKSTR(j));
		
			if (!hoObj)
				break;
		
			hudObj->SetAlpha(0);

			HiddenObject* obj = new HiddenObject();
			obj->setHiddenObj(hoObj);
			obj->setHUDObj(hudObj, s3BanyanTreeHOHudLeafFX);
			obj->setOverlapRect(rectVect[j]);
			obj->setPolyArea(polyVect[j]);
			obj->setCompleteParticle(s3BanyanTreeHOFX);
			hoList.push_back(obj);

			CObject* ovverlapObj = NULL;
			int k = 0;
			do
			{
				ovverlapObj = HOScene->GetObjectByID("GreenLeaf_" + MKSTR(i) + "_" + MKSTR(j) +"_" + "Overlap_" + MKSTR(k));
				if (!ovverlapObj)
					break;
				
				obj->AddOverlapObj(ovverlapObj);
				
				k++;
			} while (true);

			bool isFound = ProfileSystem::Instance->ActiveProfile.l8_s3_LeafHOFoundObjects[i][j];
			if (isFound)
			{
				obj->setAlreadyFound();
				_foundObjects.push_back(obj);
			}

			j++;
		} while (true);
		
		_hoMapByLevel.insert(std::make_pair(i+1, hoList)); // i+1 for level start from 1
		hoList.clear();

		i++;
	} while (true);

	OnListUpdate();
}

void L8_S3_BanyanTreeHO::Required()
{
	HiddenObjectSystem::Required();	//forced update
	if( Control::Audio->IsPlaying(aTrackL8theme) )
	{
		Control::Audio->Samples[aTrackL8theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL8theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL8theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL8ho) )
				  Control::Audio->PlaySample(aTrackL8ho, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL8ho))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL8ho, true);
		}
	}
}

void L8_S3_BanyanTreeHO::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);
	if(!_btnMatch3->Visible && !ProfileSystem::Instance->ActiveProfile.l8_s3_invMaryLeafTaken && ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{
		match3Back->Show(true);
		match3BackText->Show(true);
	}
	else
	{
		match3Back->Show(false);
		match3BackText->Show(false);
	}
	if(ProfileSystem::Instance->ActiveProfile.l8_s3_MaryLeafHOCompleted)
	{
		match3Back->Show(false);
		match3BackText->Show(false);
		_btnMatch3->Show(false);
		_btnSwitchToHO->Show(false);
	}
	if (!ProfileSystem::Instance->ActiveProfile.L8_S3_LeafTextShown)
	{
		Hud->ShowBannerText("L8mary");
		ProfileSystem::Instance->ActiveProfile.L8_S3_LeafTextShown = true;
	}
	for (int i = 0; i < (int)_foundObjects.size(); i++)
	{
		_foundObjects[i]->UpdateFoundObj();
	}

	if (_hoState >= L8S3_HOState_Playing && _hoState < L8S3_HOState_Match3Completed && Match3BtnRect.Intersection(&pos))
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

				std::map<int, std::vector<HiddenObject*> >::iterator itr;
				for (itr = _hoMapByLevel.begin(); itr != _hoMapByLevel.end(); itr++)
				{
					for (int j = 0; j < (int)(itr->second.size());j++)
					{
						itr->second[j]->SetEnable(true);
					}
				}

				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
				Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
				_hoState = L8S3_HOState_StopMatch3;
			}
		}
		else
		{
			_btnMatch3->PlayAnimation(1);

			if (Control::Input->LBclicked() && _btnMatch3->Visible)
			{
				_btnMatch3->Show(false);
				_btnSwitchToHO->Show(true);

				if (!_match3Ctrl)
				{
					_match3Ctrl = new Match3(17);
					AddControl(_match3Ctrl);
				}
				else
				{
					_match3Ctrl->ShowBoard();
				}

				std::map<int, std::vector<HiddenObject*> >::iterator itr;
				for (itr = _hoMapByLevel.begin(); itr != _hoMapByLevel.end(); itr++)
				{
					for (int j = 0; j < (int)(itr->second.size());j++)
					{
						itr->second[j]->SetEnable(false);
					}
				}
				
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = true;
				Hud->InvBoxCtrl->SetLockBase(MATCH3_FRAME);
				_hoState = L8S3_HOState_StartMatch3;
			}
		}

	}

	switch (_hoState)
	{
	case L8S3_HOState_Initializing:
		{
			_changeLevelDelay = 0.1f;
			_hoState = L8S3_HOState_ChangeLevel;
		}
		break;
	
	case L8S3_HOState_Playing:
		{
			if (Control::Input->LBclicked())
			{
				_hoState = L8S3_HOState_OnClickRegister;
			}
		}
		break;

	case L8S3_HOState_OnClickRegister:
		{
			int i = 0;
			std::map<int, std::vector<HiddenObject*> >::iterator itr;
			for (itr = _hoMapByLevel.begin(); itr != _hoMapByLevel.end(); itr++, i++)
			{
				for (int j = 0; j < (int)(itr->second.size());j++)
				{
					UserClick what = itr->second[j]->OnClickAtPoint(pos);

					if(what == Click_HiddenObj)
					{
						_foundObjects.push_back(itr->second[j]);
						Control::Audio->QuickLoadAndPlaySFX("s3ho_clickOnActiveLeaves");
						ProfileSystem::Instance->ActiveProfile.l8_s3_LeafHOFoundObjects[i][j] = true;
						_hoState = L8S3_HOState_OnFoundItem;
						return;
					}
					else if(what == Click_OverlapObj)
					{
						Control::Audio->QuickLoadAndPlaySFX("s3ho_clickOnNormalLeaves");
					}
				}
			}
					
			_hoState = L8S3_HOState_Playing;
		}
		break;

	case L8S3_HOState_OnFoundItem:
		{
			_hoState = L8S3_HOState_Playing;
			OnListUpdate();
		}
		break;
	case L8S3_HOState_ChangeLevel:
		{
			if (_changeLevelDelay > 0.0f)
			{
				_btnMatch3->Show(false);
				_changeLevelDelay -= Control::LogicRate;
				if (_changeLevelDelay <= 0.0f)
				{
					ChangeLevel(_currentLevel);
					_changeLevelDelay = 0.0f;
					_hoState = L8S3_HOState_Playing;
					_btnMatch3->Show(true);
				}
			}
		}
		break;

	case L8S3_HOState_StartMatch3:
		{
			if (_match3Ctrl->getIsCompleted())
			{
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
				Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
				_hoState = L8S3_HOState_Match3Completed;
				Hud->InvBoxCtrl->ResetToInvBox();
				_hoCompleteDelay = 1.0f;
			}
		}

		break;

	case L8S3_HOState_StopMatch3:
		_hoState = L8S3_HOState_Playing;
		break;

	case L8S3_HOState_Match3Completed:
	case L8S3_HOState_FoundAllItems:
		{
			Control::Audio->QuickLoadAndPlaySFX("s3ho_hoComplete");

			if (_handleExtrasGameComplete(false))
			{
				_hoState = L8S3_HOState_InExtrasComplete;
				return;
			}

			if (_hoCompleteDelay > 0)
			{
				_hoCompleteDelay -= Control::LogicRate;
				if (_hoCompleteDelay <= 0)
				{
					std::map<int, std::vector<HiddenObject*> >::iterator itr;
					for (itr = _hoMapByLevel.begin(); itr != _hoMapByLevel.end(); itr++)
					{
						for (int j = 0; j < (int)(itr->second.size());j++)
						{
							itr->second[j]->SetEnable(false);
						}
					}
			
					Hud->TakeInventoryItem(eInv_L8_S3_MaryLeaf);
					ProfileSystem::Instance->ActiveProfile.l8_s3_invMaryLeafTaken = true;
					Hud->InvBoxCtrl->ResetToInvBox();
					_hoState = L8S3_HOState_Idle;

					//Objective Add
					if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L8O3_PurifyLeaf] )
					{
						ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L8O3_PurifyLeaf] = true;
						ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
						= eObj_L8O3_PurifyLeaf;
						//Increment Objectives
						ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
						Hud->ShowObjectiveFX(eObj_L8O3_PurifyLeaf);

						Control::Audio->PlaySample(aSFXMagic1);
					}

				}
			}
		}
		break;
	
	case L8S3_HOState_HOCompleted:
		{
			ProfileSystem::Instance->ActiveProfile.l8_s3_MaryLeafHOCompleted = true;
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S3_BanyanTree;
			SendMessage(ControlExit, ID, HOSM_NewArea);
			_hoState = L8S3_HOState_Idle;
		}
		break;

	case L8S3_HOState_InExtrasComplete:
		break;
	
	default:
		break;
	}

	
	for (int i = 0; i < 3; i++)
	{
		if (BackDissableRects[i].Intersection(&pos))
		{
			return;
		}
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
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S3_BanyanTree;
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
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S3_BanyanTree;
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
	//		Cursor::SetMode(CA_ExitDown,eArea_L8_S3_BanyanTree);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L8_S3_BanyanTree);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L8_S3_BanyanTree, 2);
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
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S3_BanyanTree;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
								
		}	
	}
}

void L8_S3_BanyanTreeHO::ReceiveMessage(int val)
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
		if (ProfileSystem::Instance->ActiveProfile.l8_s3_invMaryLeafTaken)
		{
			_hoState = L8S3_HOState_HOCompleted;
		}

	}
}

void L8_S3_BanyanTreeHO::ProcessSkip()
{
}

void L8_S3_BanyanTreeHO::ProcessNonHOHint()
{
	if (_hoState == L8S3_HOState_StartMatch3)
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
				std::map<int, std::vector<HiddenObject*> >::iterator it;
				for (it = _hoMapByLevel.begin(); it != _hoMapByLevel.end(); it++)
				{
					for (int i = 0; i < (int)(it->second.size()); i++)
					{
						if (!it->second[i]->getisFound())
						{
							//Hud->InitHintTrailEmit(&it->second[i]->getHiddenObj()->GetWorldRect());
							// Linux: gives error of taking address of temporary also potential crash factor
							// below is fix
							CRectangle selectObjRect =it->second[i]->getHiddenObj()->GetWorldRect();
							Hud->InitHintTrailEmit(&selectObjRect);
							AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
							return;
						}
					}
				}
			}
			break;
		
		case 1: ++iNonHOHint;
			
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

L8_S3_BanyanTreeHO::~L8_S3_BanyanTreeHO()
{
	Hud->InvBoxCtrl->ResetToInvBox();

	SAFE_DELETE(s3BanyanTreeHOFX);
	SAFE_DELETE(s3BanyanTreeHOHudLeafFX);
	SAFE_DELETE(s3BanyanTreeHOLeafFallFX);

	std::map<int, std::vector<HiddenObject*> >::iterator it;
	for (it = _hoMapByLevel.begin(); it != _hoMapByLevel.end(); it++)
	{
		for (int i = 0; i < (int)(it->second.size()); i++)
		{
			SAFE_DELETE(it->second[i]);
		}
	}

	_hoMapByLevel.clear();
	_hoPolyList.clear();
	_hoRectList.clear();

		//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL8s3TreeHOAmb);
	Control::Audio->UnloadSample(aAmbL8s3TreeHOAmb);
	//MUSIC_SFX
}

void L8_S3_BanyanTreeHO::InitPolyAndRectList()
{
	std::vector<CRectangle> rects;
	std::vector<CPolygon*> polys;
	{
		rects.push_back(CRectangle(177.0f,126.0f,168.0f,169.0f));
		rects.push_back(CRectangle(605.0f,101.0f,171.0f,160.0f));
		rects.push_back(CRectangle(546.0f,355.0f,176.0f,221.0f));
		rects.push_back(CRectangle(348.0f,259.0f,178.0f,238.0f));

		int ArrayPts0[] = {248,277,273,269,296,281,320,242,279,164,232,228,234,270};
		CPolygon* PolyLeaf_0=  new CPolygon(7);
		PolyLeaf_0->Init(ArrayPts0);
		polys.push_back(PolyLeaf_0);

		int ArrayPts1[] = {645,218,677,230,761,228,730,163,684,129,659,150,635,196};
		CPolygon* PolyLeaf_1=  new CPolygon(7);
		PolyLeaf_1->Init(ArrayPts1);
		polys.push_back(PolyLeaf_1);

		int ArrayPts2[] = {553,439,611,459,621,491,689,524,719,462,699,438,640,429,610,445};
		CPolygon* PolyLeaf_2=  new CPolygon(8);
		PolyLeaf_2->Init(ArrayPts2);
		polys.push_back(PolyLeaf_2);

		int ArrayPts3[] = {321,433,378,397,396,406,443,399,477,369,452,340,459,320,440,327,421,309,378,340,370,387};
		CPolygon* PolyLeaf_3=  new CPolygon(11);
		PolyLeaf_3->Init(ArrayPts3);
		polys.push_back(PolyLeaf_3);
	}
	
	_hoRectList.insert(std::make_pair(0, rects));
	_hoPolyList.insert(std::make_pair(0, polys));

	rects.clear();
	polys.clear();

	{
		rects.push_back(CRectangle(721.0f,265.0f,201.0f,190.0f));
		rects.push_back(CRectangle(728.0f,459.0f,258.0f,182.0f));
		rects.push_back(CRectangle(1015.0f,418.0f,212.0f,188.0f));
		rects.push_back(CRectangle(303.0f,502.0f,222.0f,158.0f));

		int ArrayPts4[] = {783,401,843,382,848,357,791,285,784,256,782,290,760,317,760,370};
		CPolygon* PolyLeaf_4=  new CPolygon(8);
		PolyLeaf_4->Init(ArrayPts4);
		polys.push_back(PolyLeaf_4);
		
		int ArrayPts5[] = {790,624,825,594,867,587,908,547,899,518,861,496,833,503,815,545,816,585};
		CPolygon* PolyLeaf_5=  new CPolygon(9);
		PolyLeaf_5->Init(ArrayPts5);
		polys.push_back(PolyLeaf_5);

		int ArrayPts6[] = {1092,579,1108,557,1160,497,1149,471,1128,471,1106,453,1079,486,1087,544};
		CPolygon* PolyLeaf_6=  new CPolygon(8);
		PolyLeaf_6->Init(ArrayPts6);
		polys.push_back(PolyLeaf_6);

		int ArrayPts7[] = {467,603,445,596,423,610,380,611,356,588,374,559,376,527,440,544,447,583};
		CPolygon* PolyLeaf_7=  new CPolygon(9);
		PolyLeaf_7->Init(ArrayPts7);
		polys.push_back(PolyLeaf_7);
	}
	
	_hoRectList.insert(std::make_pair(1, rects));
	_hoPolyList.insert(std::make_pair(1, polys));

	rects.clear();
	polys.clear();

	{
		rects.push_back(CRectangle(782.0f,55.0f,259.0f,194.0f));

		int ArrayPts8[] = {818,186,846,164,834,126,848,108,929,108,943,117,993,112,992,120,965,117,948,144,905,202,874,202,850,169,822,191};
		CPolygon* PolyLeaf_8=  new CPolygon(14);
		PolyLeaf_8->Init(ArrayPts8);
		polys.push_back(PolyLeaf_8);
	}

	_hoRectList.insert(std::make_pair(2, rects));
	_hoPolyList.insert(std::make_pair(2, polys));

	rects.clear();
	polys.clear();
}

void L8_S3_BanyanTreeHO::OnListUpdate()
{
	int level = 1;
	std::map<int, std::vector<HiddenObject*> >::iterator it;
	for (it = _hoMapByLevel.begin(); it != _hoMapByLevel.end(); it++, level++)
	{
		for (int i = 0; i < (int)(it->second.size()); i++)
		{
			if (!it->second[i]->getisFound())
			{
				if (_currentLevel != level)
				{
					_currentLevel = level;
					_changeLevelDelay = 2.0f;
					_hoState = L8S3_HOState_ChangeLevel;
				}
				return;
			}
		}

		OnLevelComplete(level);
	}
	AwardManager::GetInstance()->CompleteHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	_hoState = L8S3_HOState_FoundAllItems;
	_hoCompleteDelay = 2.0f;
}

void L8_S3_BanyanTreeHO::ChangeLevel(int level)
{
	std::vector<HiddenObject*> currentList = _hoMapByLevel[_currentLevel];
	for (int j = 0; j < (int)(currentList.size()); j++)
	{
		currentList[j]->setActive(true);
	}
}

void L8_S3_BanyanTreeHO::OnLevelComplete(int level)
{
	std::vector<HiddenObject*> currentList = _hoMapByLevel[level];
	for (int j = 0; j < (int)(currentList.size()); j++)
	{
		currentList[j]->setActive(false);
	}
}


void L8_S3_BanyanTreeHO::NavigateToScene()
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