//====================================
//  created by : Jithin
//  copyright  : Tuttifrutti Games
//====================================

#include "L7_S2_HOBrokenRightHand.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"

const CRectangle  GlobRect(524.0f,163.0f,330.0f,216.0f);

L7_S2_HOBrokenRightHand::L7_S2_HOBrokenRightHand()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV7\\S2\\HO2\\HO2.SCN");

	_openDoor = HOScene->GetObjectByID("DoorOpen"); 
	_closedDoor = HOScene->GetObjectByID("DoorClosed"); 
	_globGlow = HOScene->GetObjectByID("Globe02Glow"); 

	_hoCtrl = new Level7_Scene_2::HOController();
	_hoCtrl->SetObjPolys(getPolyList());
	_hoCtrl->setDelegate(this);
	_hoCtrl->InitGame(HOScene);

	CObject *BoxGlow = HOScene->GetObjectByID("BoxGlow"); 
	BoxGlow->Animate(0.4f, 0.8f, 2);
	_globGlow->Animate(0.2f, 0.6f, 2);

	s2GlobHalfGlowFX = new SuperFX("PRT\\L7\\s2GlobHalfGlow.INI", PointSystem::CreateCPoint(662,270,0), 1100);

		//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL7s2LasyaPrisonAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL7s2LasyaPrisonAmb) )		
	{
		Control::Audio->PlaySample(aAmbL7s2LasyaPrisonAmb,true);
	}
	
	//MUSIC_SFX


	_sceneState = ProfileSystem::Instance->ActiveProfile.L7_S2_HOBrockenRightHandState;
	_stateMachine.push(_sceneState);

	if (_sceneState >= L7S2_State_GameCompleted)
	{
		_closedDoor->Show(false);
		_hoCtrl->OnComplete();
		s2GlobHalfGlowFX->Start();
	}
	else
	{
		Hud->InvBox_StartPuzzleMode();
		_openDoor->Show(false);
	}

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L7_S2_HOBrokenRightHand]  = true;
}

void L7_S2_HOBrokenRightHand::Required()
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
			 if( !Control::Audio->IsPlaying(aTrackL7ho) )
				  Control::Audio->PlaySample(aTrackL7ho, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL7ho))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL7ho, true);
		}
	}
}

void L7_S2_HOBrokenRightHand::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

#ifdef _CHEATS
	if(Control::Input->KpressedWithShift(K_VK_SPACE))
	{
		if(!ProfileSystem::Instance->ActiveProfile.L7_S2_HOBrokenRightHandCompleted)
		{
			ProfileSystem::Instance->ActiveProfile.L7_S2_HOBrokenRightHandCompleted = true;
			return;
		}
	}
#endif // _CHEATS

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);
	if (_stateMachine.top() != _sceneState)
	{
		ProfileSystem::Instance->ActiveProfile.L7_S2_HOBrockenRightHandState = _sceneState;
		_stateMachine.push(_sceneState);
	}

	switch (_sceneState)
	{
	case L7S2_State_Initialize:
		{
			_sceneState = L7S2_State_GamePlaying;
		}
		break;

	case L7S2_State_GamePlaying:
		{
			_hoCtrl->Update();

			if (_hoCtrl->getIsCompleted())
			{
				_sceneState = L7S2_State_GameCompleted;
			}
		}
		break;

	case L7S2_State_GameCompleted:
		{
			AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			if (_handleExtrasGameComplete(false))
			{
				_sceneState = L7S2_State_GameForExtrasComplete;
				return;
			}

			Hud->InvBox_StopPuzzleMode();
			_sceneState = L7S2_State_OpeningDoor;
		}
		break;

	case L7S2_State_OpeningDoor:
		{
			_openDoor->FadeIn();
			_closedDoor->FadeOut();
			s2GlobHalfGlowFX->Start();
			_sceneState = L7S2_State_WaitingForCollectGlob;
		}
		break;

	case L7S2_State_WaitingForCollectGlob:
		{
			if (GlobRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if (Control::Input->LBclicked())
				{
					_globGlow->FadeOut();
					HOScene->GetObjectByID("Globe02")->FadeOut();
					ProfileSystem::Instance->ActiveProfile.L7_S2_InvSphereHalfRightBaseTaken = true;
					Hud->TakeInventoryItem(eInv_L7_S2_SphereHalfBase);
				}
			}
		}
		break;

	case L7S2_State_GlobCollected:
		{
			ProfileSystem::Instance->ActiveProfile.L7_S2_HOBrokenRightHandCompleted = true;
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S2_LasyaPrison;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
		break;

	default:
		break;
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
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L7_S2_LasyaPrison);
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

void L7_S2_HOBrokenRightHand::ReceiveMessage(int val)
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
		_sceneState = L7S2_State_GlobCollected;
	}
}

void L7_S2_HOBrokenRightHand::ProcessSkip()
{
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	_hoCtrl->SkipGame();
}

void L7_S2_HOBrokenRightHand::ProcessNonHOHint()
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

		case 0: 
			{
				++iNonHOHint;

				if (_sceneState == L7S2_State_WaitingForCollectGlob)
				{
					Hud->InitHintTrailEmit(&GlobRect);
					return;
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

L7_S2_HOBrokenRightHand::~L7_S2_HOBrokenRightHand()
{
	Hud->InvBox_StopPuzzleMode();
	SAFE_DELETE(_hoCtrl);
	SAFE_DELETE(s2GlobHalfGlowFX);

		//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL7s2LasyaPrisonAmb);
	Control::Audio->UnloadSample(aAmbL7s2LasyaPrisonAmb);
	//MUSIC_SFX
}

vector<CPolygon*> L7_S2_HOBrokenRightHand::getPolyList()
{
	vector<CPolygon*> objPolyList;

	int ArrayPts1[] = {1175,250,1165,232,1166,209,1150,197,1164,180,1170,133,1224,134,1240,162,1219,177,1215,195,1205,201,1203,229};
	CPolygon* CPoly1=  new CPolygon(12);
	CPoly1->Init(ArrayPts1);
	objPolyList.push_back(CPoly1);

	int ArrayPts2[] = {1022,449,1018,486,1062,501,1108,493,1181,460,1182,439,1139,434,1099,429,1056,475,1046,461,1043,435};
	CPolygon* CPoly2=  new CPolygon(11);
	CPoly2->Init(ArrayPts2);
	objPolyList.push_back(CPoly2);
	
	int ArrayPts3[] = {329,367,352,367,391,355,417,379,429,347,411,327,392,326,400,313,380,312};
	CPolygon* CPoly3=  new CPolygon(9);
	CPoly3->Init(ArrayPts3);
	objPolyList.push_back(CPoly3);
	
	int ArrayPts4[] = {994,225,1012,179,989,167,990,143,1009,160,1019,138,996,123,959,152,966,168,952,189,969,193};
	CPolygon* CPoly4=  new CPolygon(11);
	CPoly4->Init(ArrayPts4);
	objPolyList.push_back(CPoly4);
	
	int ArrayPts5[] = {202,301,208,265,243,241,251,219,257,233,268,237,274,219,282,235,278,249,288,266,282,295,297,317,285,333,268,309};
	CPolygon* CPoly5=  new CPolygon(14);
	CPoly5->Init(ArrayPts5);
	objPolyList.push_back(CPoly5);
	
	int ArrayPts6[] = {359,170,385,159,399,180,411,171,426,172,437,143,419,135,401,101,363,101,332,126};
	CPolygon* CPoly6=  new CPolygon(10);
	CPoly6->Init(ArrayPts6);
	objPolyList.push_back(CPoly6);

	return objPolyList;
}

void L7_S2_HOBrokenRightHand::onSymbolComplete(int index)
{
	ProfileSystem::Instance->ActiveProfile.L7_S2_HOBrockenRightHandSymbolCompleted[index] = true;
}

bool L7_S2_HOBrokenRightHand::getIsSymbolCompleted(int index)
{
	return ProfileSystem::Instance->ActiveProfile.L7_S2_HOBrockenRightHandSymbolCompleted[index];
}
void L7_S2_HOBrokenRightHand::NavigateToScene()
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