
//====================================
//  created by : Jithin	
//  copyright  : Tuttifrutti Games
//====================================

#include "L8_S1_HODoor.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "Util.h"
#include "AwardManager.h"

const CRectangle  Match3BtnRect(550.0f,1.0f,252.0f,39.0f);
const CRectangle  BackDisableRect(510.0f,576.0f,310.0f,84.0f);

namespace Level8_Scene1
{
	HiddenObject::HiddenObject()
	{
		_isActive = false;
		_isFound = false;
		_isCompleted = false;
		_HUDhideDelay = 0.0f;
		_completeTimer = 0.0f;
		_polyArea = NULL;
		_nameText = NULL;
		_completeParticle = NULL;
	}

	HiddenObject::~HiddenObject()
	{
		SAFE_DELETE(_polyArea);
		SAFE_DELETE(_nameText);
		_completeParticle = NULL;
	}

	void HiddenObject::Init()
	{
		CRectangle InvAreaRect(368.0f,670.0f,624.0f,96.0f);
		float vertGap = InvAreaRect.h/3.0f, horizontalGap = InvAreaRect.w/3.0f;
		CPoint pos(0,0,0);

		pos.x = InvAreaRect.Left() + horizontalGap*0.5f + horizontalGap*(_index%3);
		pos.y = InvAreaRect.Top() + vertGap*0.5f + vertGap*(_index/3);

		ColorRGB gold = {(233/255.0f),(204/255.0f),(131/255.0f)};
		_nameText = new CBitmapText();
		_nameText->SetTextProps(_name, pos.x, pos.y, eZOrder_INVBOXITEMS, gold, eFont_Small, Align_Center);
		_nameText->SetZ(eZOrder_INVBOXITEMS);
		_nameText->SetAlpha(0.0f);
	}

	UserClick HiddenObject::OnClickAtPoint(CPoint pos)
	{
		if (_isFound)
			return Click_None;

		if (_isActive && _polyArea->Intersection(&pos))
		{
			OnFound();
			return Click_HiddenObj;
		}

		return Click_None;
	}

	void HiddenObject::OnFound()
	{
		_isFound = true;

		_nameText->SetStrikethrough(true);
		_completeParticle->SetPos(&_hiddenObj->Pos);
		_completeParticle->Start();
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
				HideHUDName();
				_HUDhideDelay = 0.0f;
			}
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
		_hiddenObj->SetEnable(false);
	}

	void HiddenObject::setActive(bool active)
	{
		_isActive = active;

		if (_isActive)
		{
			_isFound ? _nameText->SetAlpha(0.5f) : _nameText->SetAlpha(1.0f);
		}
		else
		{
			_HUDhideDelay = 1.0f;
		}

	}

	void HiddenObject::HideHUDName()
	{
		_nameText->SetAlpha(0.0f);
	}

	void  HiddenObject::SwitchHO()
	{
		_nameText->SetEnable(true);
	}

	void  HiddenObject::SwitchToMatch3()
	{
		_nameText->SetEnable(false);
	}
}


L8_S1_DoorHO::L8_S1_DoorHO()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV8\\S1\\HO1\\HO1.SCN");

	_hoCompleteDelay = 0.0f;

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

	s1DoorHOFX = new SuperFX("PRT\\L8\\s1DoorHOComplete.INI", PointSystem::CreateCPoint(614,351,0), 308); 
	s1DoorHOInvBoxFXFX = new SuperFX("PRT\\L8\\s1DoorHOInvBoxFX.INI", PointSystem::CreateCPoint(693,716,0), 1787);

	_hoState = L8S1_HOState_Initializing;
	InitPolyAndNameList();
	InitHOGame();

	s1hoglaresFX = new SuperFX("PRT\\L8\\s1hoglares.INI", PointSystem::CreateCPoint(914,172,0), 100); 
	s1hoglaresFX->Start();
	s1hoglaresFX->AdvanceByTime(2.0f);

	s1hofliesFX = new SuperFX("PRT\\L8\\s1hoflies.INI", PointSystem::CreateCPoint(885,240,0), 100); 
	s1hofliesFX->Start();
	s1hofliesFX->AdvanceByTime(2.0f);

	s1hoglitrightFX = new SuperFX("PRT\\L8\\s1hoglitright.INI", PointSystem::CreateCPoint(920,317,0), 100); 
	s1hoglitrightFX->Start();
	s1hoglitrightFX->AdvanceByTime(2.0f);

	s1hoglitleftFX = new SuperFX("PRT\\L8\\s1hoglitleft.INI", PointSystem::CreateCPoint(300,375,0), 100); 
	s1hoglitleftFX->Start();
	s1hoglitleftFX->AdvanceByTime(2.0f);

	ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

	AwardManager::GetInstance()->StartHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
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
	GFInstance->IsScnElmntsPreloaded[eArea_L8_S1_DoorHO]  = true;
}

void L8_S1_DoorHO::InitHOGame()
{ 
	_currentLevel = 0; // for initial state. once OnListUpdate() called it changed to 1.

	CObject* hoObj = NULL;
	int i = 0;
	do
	{
		vector<HiddenObject*> hoList;
		int j = 0;
		hoObj = HOScene->GetObjectByID("HO_"+ MKSTR(i) +"_"+ MKSTR(j));

		if (!hoObj)
			break;

		std::vector<CPolygon*> polyVect = _hoPolyList[i];
		std::vector<string> nameList = _hoNameList[i];

		do
		{
			hoObj = HOScene->GetObjectByID("HO_"+ MKSTR(i) +"_"+ MKSTR(j));

			if (!hoObj)
				break;

			HiddenObject* obj = new HiddenObject();
			obj->setIndex(j);
			obj->setHiddenObj(hoObj);
			obj->setPolyArea(polyVect[j]);
			obj->setName(StringTable::getStr(nameList[j]));
			obj->setCompleteParticle(s1DoorHOFX);
			obj->Init();
			hoList.push_back(obj);
			bool isFound = ProfileSystem::Instance->ActiveProfile.l8_s1_DoorHOFoundObjects[i][j];
			if (isFound)
			{
				obj->setAlreadyFound();
				_foundObjects.push_back(obj);
			}

			j++;
		} while (true);

		_hoMapByLevel.insert(std::make_pair(++i, hoList)); // i+1 for level start from 1
		hoList.clear();
	} while (true);

	OnListUpdate();
}

void L8_S1_DoorHO::Required()
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

void L8_S1_DoorHO::Update()
{
	NavigateToScene();

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);
	if(!_btnMatch3->Visible && !ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicGlassTaken)
	{
		match3Back->Show(true);
		match3BackText->Show(true);
	}
	else
	{
		match3Back->Show(false);
		match3BackText->Show(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicGlassTaken)
	{
		match3Back->Show(false);
		match3BackText->Show(false);
		_btnMatch3->Show(false);
		_btnSwitchToHO->Show(false);
	}

	for (int i = 0; i < (int)_foundObjects.size(); i++)
	{
		_foundObjects[i]->UpdateFoundObj();
	}

	if (_hoState >= L8S1_HOState_Playing && _hoState < L8S1_HOState_Match3Completed && Match3BtnRect.Intersection(&pos))
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
						itr->second[j]->SwitchHO();
					}
				}

				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
				Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
				_hoState = L8S1_HOState_StopMatch3;
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
					_match3Ctrl = new Match3(16);
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
						itr->second[j]->SwitchToMatch3();
					}
				}

				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = true;
				Hud->InvBoxCtrl->SetLockBase(MATCH3_FRAME);
				_hoState = L8S1_HOState_StartMatch3;
			}
		}

	}

	switch (_hoState)
	{
	case L8S1_HOState_Initializing:
		{
			_changeLevelDelay = 0.1f;
			_hoState = L8S1_HOState_ChangeLevel;
		}
		break;

	case L8S1_HOState_Playing:
		{
			if (Control::Input->LBclicked())
			{
				_hoState = L8S1_HOState_OnClickRegister;
			}
		}
		break;

	case L8S1_HOState_OnClickRegister:
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
						Control::Audio->QuickLoadAndPlaySFX("s1HO1_ObjFound");
						ProfileSystem::Instance->ActiveProfile.l8_s1_DoorHOFoundObjects[i][j] = true;
						_hoState = L8S1_HOState_OnFoundItem;
						return;
					}
				}
			}

			_hoState = L8S1_HOState_Playing;
		}
		break;

	case L8S1_HOState_OnFoundItem:
		{
			_hoState = L8S1_HOState_Playing;
			OnListUpdate();
		}
		break;
	case L8S1_HOState_ChangeLevel:
		{
			if (_changeLevelDelay > 0.0f)
			{
				_changeLevelDelay -= Control::LogicRate;
				if (_changeLevelDelay <= 0.0f)
				{
					ChangeLevel(_currentLevel);
					_changeLevelDelay = 0.0f;
					_hoState = L8S1_HOState_Playing;
				}
			}
		}
		break;

	case L8S1_HOState_StartMatch3:
		{
			if (_match3Ctrl->getIsCompleted())
			{
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
				Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
				_hoState = L8S1_HOState_Match3Completed;
				Hud->InvBoxCtrl->ResetToInvBox();
				_hoCompleteDelay = 1.0f;
			}
		}

		break;

	case L8S1_HOState_StopMatch3:
		_hoState = L8S1_HOState_Playing;
		break;

	
	case L8S1_HOState_FoundAllItems:
	case L8S1_HOState_Match3Completed:
		{
			Control::Audio->QuickLoadAndPlaySFX("s3ho_hoComplete");

			if (_handleExtrasGameComplete(false))
			{
				_hoState = L8S1_HOState_InExtrasComplete;
				return;
			}

			if (_hoCompleteDelay > 0)
			{
				_hoCompleteDelay -= Control::LogicRate;
				if (_hoCompleteDelay <= 0)
				{
					Hud->TakeInventoryItem(eInv_L8_S1_MagicGlass);
					Hud->ShowBannerText(Banner_L8_S1_TakeMagicGlass);
					ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicGlassTaken = true;
					Hud->InvBoxCtrl->ResetToInvBox();
					_hoState = L8S1_HOState_Idle;
				}
			}
		}
		break;

	case L8S1_HOState_HOCompleted:
		{
			ProfileSystem::Instance->ActiveProfile.l8_s1_DoorHOCompleted = true;
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S1_ShukrasPlace;
			SendMessage(ControlExit, ID, HOSM_NewArea);
			_hoState = L8S1_HOState_Idle;
		}
		break;

	case L8S1_HOState_InExtrasComplete:
		break;

	default:
		break;
	}

	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (!BackDisableRect.Intersection(&pos) && Control::Input->isKeyBackSensed()) // need to know if BackDisaleRect has to be checked or not here
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
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S1_ShukrasPlace;
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
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S1_ShukrasPlace;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
	}
	if(!BackDisableRect.Intersection(&pos) && IsMouseOverGoBackArea(&pos)
											  #ifdef M3_BUTTON
											  && _btnMatch3->Visible
                                              #endif
                                              )
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L8_S1_ShukrasPlace);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L8_S1_ShukrasPlace);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L8_S1_ShukrasPlace, 2);
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
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S1_ShukrasPlace;
			SendMessage(ControlExit, ID, HOSM_NewArea);		
#endif
							
		}	
	}
}

void L8_S1_DoorHO::ReceiveMessage(int val)
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
		if (ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicGlassTaken)
		{
			_hoState = L8S1_HOState_HOCompleted;
		}

	}
}

void L8_S1_DoorHO::ProcessSkip()
{
}

void L8_S1_DoorHO::ProcessNonHOHint()
{
	if (_hoState == L8S1_HOState_StartMatch3)
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
				AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
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
							CRectangle ObjRect =it->second[i]->getHiddenObj()->GetWorldRect();
							Hud->InitHintTrailEmit(&ObjRect);
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

L8_S1_DoorHO::~L8_S1_DoorHO()
{
	Hud->InvBoxCtrl->ResetToInvBox();

	SAFE_DELETE(s1DoorHOFX);
	SAFE_DELETE(s1DoorHOInvBoxFXFX);

	std::map<int, std::vector<HiddenObject*> >::iterator it;
	for (it = _hoMapByLevel.begin(); it != _hoMapByLevel.end(); it++)
	{
		for (int i = 0; i < (int)(it->second.size()); i++)
		{
			SAFE_DELETE(it->second[i]);
		}
	}

	SAFE_DELETE(s1hoglaresFX);
	SAFE_DELETE(s1hofliesFX);
	SAFE_DELETE(s1hoglitrightFX);
	SAFE_DELETE(s1hoglitleftFX);

	_hoMapByLevel.clear();
	_hoNameList.clear();
	_hoPolyList.clear();

	//MUSIC_SFX

	Control::Audio->StopSample(aAmbL8s3TreeHOAmb);
	Control::Audio->UnloadSample(aAmbL8s3TreeHOAmb);
	//MUSIC_SFX
}

void L8_S1_DoorHO::InitPolyAndNameList()
{
	std::vector<string> names;
	std::vector<CPolygon*> polys;
	{
		names.push_back("Gauntlet");
		names.push_back("Trident");
		names.push_back("Sandal");
		names.push_back("Amulets_and_Charms");
		names.push_back("Plant");
		names.push_back("Cauldron");
		names.push_back("Gold_Chain");
		names.push_back("Horn");
		names.push_back("Gemstones");

		int ArrayPts0[] = {550,639,602,626,634,602,666,607,731,584,748,595,756,624,683,633,670,648,629,652,617,642,565,651};
		CPolygon* PolyObj_0=  new CPolygon(12);
		PolyObj_0->Init(ArrayPts0);
		polys.push_back(PolyObj_0);

		int ArrayPts1[] = {169,197,172,254,211,267,228,365,237,370,225,269,255,261,263,232,246,185,229,198,243,244,227,246,206,165,197,166,204,244,189,244,180,187};
		CPolygon* PolyObj_1=  new CPolygon(17);
		PolyObj_1->Init(ArrayPts1);
		polys.push_back(PolyObj_1);

		int ArrayPts2[] = {703,581,725,584,779,559,775,525,728,544,723,558,684,555};
		CPolygon* PolyObj_2=  new CPolygon(7);
		PolyObj_2->Init(ArrayPts2);
		polys.push_back(PolyObj_2);

		int ArrayPts3[] = {449,333,436,365,434,407,456,430,485,399,465,337};
		CPolygon* PolyObj_3=  new CPolygon(6);
		PolyObj_3->Init(ArrayPts3);
		polys.push_back(PolyObj_3);

		int ArrayPts4[] = {997,655,994,615,1159,563,1213,673,1364,663,1364,542,1265,541,1267,525,1324,480,1270,358,1162,416,1135,438,1133,501,1080,400,1063,423,1087,508,1014,508,1018,556,1104,541,1107,560,935,617,937,649};
		CPolygon* PolyObj_4=  new CPolygon(22);
		PolyObj_4->Init(ArrayPts4);
		polys.push_back(PolyObj_4);

		int ArrayPts5[] = {557,289,579,289,580,251,600,248,596,269,619,268,612,251,629,251,633,287,648,284,646,224,657,224,661,211,660,156,647,156,645,178,558,181,555,155,543,155,553,238};
		CPolygon* PolyObj_5=  new CPolygon(20);
		PolyObj_5->Init(ArrayPts5);
		polys.push_back(PolyObj_5);

		int ArrayPts6[] = {1004,483,1004,546,994,585,1018,594,1038,557,1020,521,1059,500,1038,476};
		CPolygon* PolyObj_6=  new CPolygon(8);
		PolyObj_6->Init(ArrayPts6);
		polys.push_back(PolyObj_6);

		int ArrayPts7[] = {769,145,816,148,843,159,862,146,854,123,827,118,818,146,767,143,759,127,708,138,683,111,690,75,675,70,678,120,711,155};
		CPolygon* PolyObj_7=  new CPolygon(15);
		PolyObj_7->Init(ArrayPts7);
		polys.push_back(PolyObj_7);

		int ArrayPts8[] = {521,575,560,590,588,569,564,541,533,542};
		CPolygon* PolyObj_8=  new CPolygon(5);
		PolyObj_8->Init(ArrayPts8);
		polys.push_back(PolyObj_8);
	}

	_hoNameList.insert(std::make_pair(0, names));
	_hoPolyList.insert(std::make_pair(0, polys));

	names.clear();
	polys.clear();

	{
		names.push_back("Feather");
		names.push_back("Lantern");
		names.push_back("Shield");
		names.push_back("Cups_and_Chalices");
		names.push_back("Sculpture");
		names.push_back("Book");
		names.push_back("Robe");
		names.push_back("Marked_Stone");
		names.push_back("Flute");

		int ArrayPts0[] = {332,597,342,622,362,628,385,605,407,596,411,579,391,564,387,552,413,524,423,528,577,469,554,429,476,457,478,504,423,525,429,482,400,504,408,518,376,549,357,564};
		CPolygon* PolyObj_0=  new CPolygon(20);
		PolyObj_0->Init(ArrayPts0);
		polys.push_back(PolyObj_0);

		int ArrayPts1[] = {345,318,379,338,438,340,481,315,469,299,467,201,481,188,465,177,462,151,449,137,449,120,422,97,415,3,389,1,386,43,386,101,355,156,343,198,352,209,360,302};
		CPolygon* PolyObj_1=  new CPolygon(20);
		PolyObj_1->Init(ArrayPts1);
		polys.push_back(PolyObj_1);

		int ArrayPts2[] = {516,273,551,273,590,244,600,196,577,153,462,156,465,179,479,187,467,200,469,243,479,227,489,176,517,180,508,232};
		CPolygon* PolyObj_2=  new CPolygon(14);
		PolyObj_2->Init(ArrayPts2);
		polys.push_back(PolyObj_2);

		int ArrayPts3[] = {293,414,343,408,353,446,337,472,313,537,343,552,354,507,369,525,399,499,407,519,425,526,410,578,391,566,387,553,370,552,340,580,341,554,312,531,295,506,258,483,257,458,278,450};
		CPolygon* PolyObj_3=  new CPolygon(22);
		PolyObj_3->Init(ArrayPts3);
		polys.push_back(PolyObj_3);

		int ArrayPts4[] = {223,274,226,231,193,164,209,100,191,57,226,22,248,41,253,73,272,101,264,71,286,74,285,114,299,130,290,145,262,147,288,262,265,280};
		CPolygon* PolyObj_4=  new CPolygon(17);
		PolyObj_4->Init(ArrayPts4);
		polys.push_back(PolyObj_4);

		int ArrayPts5[] = {154,187,260,172,299,199,301,235,178,255,153,224};
		CPolygon* PolyObj_5=  new CPolygon(6);
		PolyObj_5->Init(ArrayPts5);
		polys.push_back(PolyObj_5);

		int ArrayPts6[] = {237,133,273,126,269,217,361,280,389,274,376,124,405,123,378,14,329,0,268,7};
		CPolygon* PolyObj_6=  new CPolygon(10);
		PolyObj_6->Init(ArrayPts6);
		polys.push_back(PolyObj_6);

		int ArrayPts7[] = {451,481,485,537,534,545,569,477,557,460,508,464,477,447};
		CPolygon* PolyObj_7=  new CPolygon(7);
		PolyObj_7->Init(ArrayPts7);
		polys.push_back(PolyObj_7);

		int ArrayPts8[] = {856,433,902,358,913,366,867,443};
		CPolygon* PolyObj_8=  new CPolygon(4);
		PolyObj_8->Init(ArrayPts8);
		polys.push_back(PolyObj_8);
	}

	_hoNameList.insert(std::make_pair(1, names));
	_hoPolyList.insert(std::make_pair(1, polys));

	names.clear();
	polys.clear();

	{
		names.push_back("Organ");
		names.push_back("Weighing_Scale");
		names.push_back("Dagger");
		names.push_back("Chariot");
		names.push_back("Magic_Glass");
		names.push_back("Mirror");

		int ArrayPts0[] = {176,103,304,104,300,18,171,13};
		CPolygon* PolyObj_0=  new CPolygon(4);
		PolyObj_0->Init(ArrayPts0);
		polys.push_back(PolyObj_0);

		int ArrayPts1[] = {809,545,789,567,776,555,802,538,817,393,887,394,854,373,891,338,937,367,909,393,940,387,978,395,999,541,1014,555,1000,574,941,573,924,552,971,409,908,409,909,566,934,590,909,612,881,609,883,592,863,579,888,567,887,406,826,415,866,551,832,544};
		CPolygon* PolyObj_1=  new CPolygon(30);
		PolyObj_1->Init(ArrayPts1);
		polys.push_back(PolyObj_1);

		int ArrayPts2[] = {817,351,835,289,838,224,829,174,800,163,789,193,809,188};
		CPolygon* PolyObj_2=  new CPolygon(7);
		PolyObj_2->Init(ArrayPts2);
		polys.push_back(PolyObj_2);

		int ArrayPts3[] = {902,163,921,95,927,112,958,101,965,45,988,50,1000,67,978,127,980,147,941,159};
		CPolygon* PolyObj_3=  new CPolygon(10);
		PolyObj_3->Init(ArrayPts3);
		polys.push_back(PolyObj_3);

		int ArrayPts4[] = {658,495,658,479,673,466,670,454,703,421,735,449,753,490,735,498,730,509,683,502,683,492};
		CPolygon* PolyObj_4=  new CPolygon(11);
		PolyObj_4->Init(ArrayPts4);
		polys.push_back(PolyObj_4);

		int ArrayPts5[] = {954,421,980,424,983,371,949,379};
		CPolygon* PolyObj_5=  new CPolygon(4);
		PolyObj_5->Init(ArrayPts5);
		polys.push_back(PolyObj_5);
	}

	_hoNameList.insert(std::make_pair(2, names));
	_hoPolyList.insert(std::make_pair(2, polys));

	names.clear();
	polys.clear();
}

void L8_S1_DoorHO::OnListUpdate()
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
					_changeLevelDelay = 1.0f;
					_hoState = L8S1_HOState_ChangeLevel;
				}
				return;
			}
		}

		OnLevelComplete(level);
	}

	AwardManager::GetInstance()->CompleteHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	_hoState = L8S1_HOState_FoundAllItems;
	_hoCompleteDelay = 2.0f;
}

void L8_S1_DoorHO::ChangeLevel(int level)
{
	std::vector<HiddenObject*> currentList = _hoMapByLevel[_currentLevel];
	for (int j = 0; j < (int)(currentList.size()); j++)
	{
		currentList[j]->setActive(true);
	}

	s1DoorHOInvBoxFXFX->Start();
}

void L8_S1_DoorHO::OnLevelComplete(int level)
{
	std::vector<HiddenObject*> currentList = _hoMapByLevel[level];
	for (int j = 0; j < (int)(currentList.size()); j++)
	{
		currentList[j]->setActive(false);
	}
}


void L8_S1_DoorHO::NavigateToScene()
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