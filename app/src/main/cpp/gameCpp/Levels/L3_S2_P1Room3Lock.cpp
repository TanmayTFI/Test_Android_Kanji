//====================================
// DQFULP
// Hidden Object System
// Author : Sreeraj
// LEVEL3 - SCENE3 : P1Room3Lock
//====================================

#include "L3_S2_P1Room3Lock.h"
#include "Hud.h"
#include "Rect.h"
#include "Polygon.h"
#include "Util.h"
#include "HiddenObjectEnums.h"
#include "L3_S2_P1R3HammerPopup.h"
#include "L3_S2_P1R3PipePopup.h"
#include "L3_S2_P1R3LockOpen.h"

const CRectangle  DoorRect[] = {CRectangle(183.0f,274.0f,118.0f,201.0f), 
								CRectangle(522.0f,296.0f,126.0f,196.0f),
								CRectangle(1002.0f,147.0f,185.0f,356.0f)};

const CRectangle  PipePopupRect(342.0f,540.0f,127.0f,66.0f);
//const CRectangle  HammerPopupRect(501.0f,586.0f,312.0f,91.0f);
const CRectangle  PopupOrigins[] = {PipePopupRect, DoorRect[0], DoorRect[1], DoorRect[2]};

const eCursorAnim NextAreaPoint[] = {CA_ExitLeft, CA_ExitUp, CA_ExitRight};

const int SolutionDoor = 0;

L3_S2_P1Room3Lock::L3_S2_P1Room3Lock()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV3\\S2\\MAZE3\\MAZE3.SCN");
	musicFadeOutTimer = 0;
	for (int i = 0; i < 3; i++)
	{
		CObject* doorObj = HOScene->GetObjectByID("Door_"+MKSTR(i));
		_doors.push_back(doorObj);

		if (ProfileSystem::Instance->ActiveProfile.L3_S2_R3DoorOpen[i])
			doorObj->SetEnable(false);
	}

	HOScene->GetObjectByID("popfireanim1")->SetScale(0.70f);
	HOScene->GetObjectByID("popfireanim2")->SetScale(0.70f);

	_stonesClosed = HOScene->GetObjectByID("BeforePipePopup");
	_stonesCleared = HOScene->GetObjectByID("AfterPipePopup");
	_stonesCleared->SetEnable(false);

	if (ProfileSystem::Instance->ActiveProfile.L3_S2_R3StonesRemoved)
	{
		_stonesClosed->SetEnable(false);
		_stonesCleared->SetEnable(true);
	}

	/*if (!ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHeadTaken)
	{

	}*/

	if (!ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe3R3Taken ||
		!ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHeadTaken)
	{
		CPoint zoomOrigin = CPoint(PipePopupRect.GetCenterX(), PipePopupRect.GetCenterY(), 0); 
		L3_S2_P1R3PipePopup *popup = new L3_S2_P1R3PipePopup(P1R3_PipePickPopup, Hud, HOScene, eZOrder_Popup, zoomOrigin, CC_CALLBACK_1(L3_S2_P1Room3Lock::OnClosePopup, this));
		_scenePopups[P1R3_PipePickPopup] = popup;
	}

	for (int i = 0; i < 3; i++)
	{
		if (!ProfileSystem::Instance->ActiveProfile.L3_S2_R3DoorOpen[i])
		{
			L3_S2_P1R3Popups popupID = L3_S2_P1R3Popups(int(P1R3_Door1Popup) + i);
			CPoint zoomOrigin = CPoint(DoorRect[i].GetCenterX(), DoorRect[i].GetCenterY(), 0); 
			L3_S2_P1R3LockOpen *popup = new L3_S2_P1R3LockOpen(popupID, Hud, HOScene, eZOrder_Popup, zoomOrigin, CC_CALLBACK_1(L3_S2_P1Room3Lock::OnClosePopup, this));
			popup->setDoorIndex(i);
			_scenePopups[popupID] = popup;
		}
	}

	_activePopup = P1R3_NoPopup;

	ProfileSystem::Instance->ActiveProfile.L3_S2_MazeCurrentRoom = eArea_L3_S2_P1Room3Lock;

	if(ProfileSystem::Instance->ActiveProfile.L3_S2_MazeMapUsed)
	{
		HOScene->Load("LV3\\S2\\MAZECOMMON\\MAP\\MAPHAND.SCN");
		for (int i = 0; i < 5; i++)
		{
			if(i!=2)
				HOScene->GetObjectByID(std::string("posindicator" + MKSTR(i)).c_str())->SetEnable(false);
			else
				HOScene->GetObjectByID(std::string("posindicator" + MKSTR(i)).c_str())->Animate(0.85f, 1.0f, 1.25f);
		}
	}

	if(IsVisitedForFirstTime)
	{
		Hud->ShowBannerText(Banner_L3_S2_M3_EnterFirstTime);
	}


	//MUSIC_SFX
	Control::Audio->LoadSample(aAmbL3s2Maze,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL3s2Maze) )		
	{
		Control::Audio->PlaySample(aAmbL3s2Maze,true);
	}
	
	//MUSIC_SFX
	

	TaskPendingUpdate();
}

L3_S2_P1Room3Lock::~L3_S2_P1Room3Lock()
{
	std::map<L3_S2_P1R3Popups, Popup*>::iterator it;
	for (it = _scenePopups.begin(); it != _scenePopups.end(); it++)
	{
		SAFE_DELETE(it->second);
	}

	_scenePopups.clear();

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL3s2Maze);
	Control::Audio->UnloadSample(aAmbL3s2Maze);
	//MUSIC_SFX
}

void L3_S2_P1Room3Lock::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Control::Audio->IsPlaying(aTrackL3ho) )
		{
			Control::Audio->Samples[aTrackL3ho]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL3ho]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL3ho);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL3theme) )
					  Control::Audio->PlaySample(aTrackL3theme, true);
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
	else if( Control::Audio->IsPlaying(aTrackL3puzzle) )
		{
			Control::Audio->Samples[aTrackL3puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL3puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL3puzzle);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL3theme) )
					  Control::Audio->PlaySample(aTrackL3theme, true);
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
		else
		{
			if( !Control::Audio->IsPlaying(aTrackL3theme))
			{
				Control::Audio->TurnOffLvlMscTracks();
				Control::Audio->PlaySample(aTrackL3theme, true);
			}
		}
}

void L3_S2_P1Room3Lock::Update()
{
	NavigateToScene();
	HiddenObjectSystem::HOUpdate();

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing)
		return;

	if (_activePopup != P1R3_NoPopup)
		return;

	CPoint mPos = Control::Input->Mpos();	
	Cursor::SetMode(CA_Normal);

	for (int i = 0; i < 3; i++)
	{
		if (DoorRect[i].Intersection(&mPos))
		{
			if (!ProfileSystem::Instance->ActiveProfile.L3_S2_R3DoorOpen[i])
			{
				Cursor::SetMode(CA_Look);
				if (Control::Input->LBclicked())
				{
					_activePopup = L3_S2_P1R3Popups(int(P1R3_Door1Popup) + i);
					_scenePopups[_activePopup]->OpenPopup();
					HOScene->GetObjectByID("handMazeMap")->Show(false);
					HOScene->GetObjectByID("posindicator2")->Show(false);
				}
			}
			else
			{
				if (ProfileSystem::Instance->ActiveProfile.L3_S2_MazeMapUsed)
				{

#ifdef TOUCH_UI
				//	if(!GFHud->InvBoxCtrl->navTapped)
				//		Cursor::SetMode(NextAreaPoint[i]);
#else
					Cursor::SetMode(NextAreaPoint[i]);
#endif

					if( Control::Input->LBclicked() )
					{
#ifdef TOUCH_UI
						if(!GFHud->InvBoxCtrl->navTapped)
						{
							if (i == SolutionDoor)
							{
								GFHud->InvBoxCtrl->GoToScene(eArea_L3_S2_P1Room4Match3, 0, DoorRect[i]);
							}
							else
							{
								GFHud->InvBoxCtrl->GoToScene(eArea_L3_S2_P1DeadEnd, 0, DoorRect[i]);
							}
						}
#else
						if (i == SolutionDoor)
						{
							ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_P1Room4Match3;
							SendMessage(ControlExit, ID, HOSM_NewArea);
						}
						else
						{
							ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_P1DeadEnd;
							SendMessage(ControlExit, ID, HOSM_NewArea);
						}
#endif

					}
				}
				else
				{
					Cursor::SetMode(CA_Gear);
				}
			}
		}
	}

	if ((!ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe3R3Taken ||
		!ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHeadTaken) && 
		PipePopupRect.Intersection(&mPos))
	{
		Cursor::SetMode(CA_Look);
		if (Control::Input->LBclicked())
		{
			_activePopup = P1R3_PipePickPopup;
			_scenePopups[_activePopup]->OpenPopup();
			HOScene->GetObjectByID("handMazeMap")->Show(false);
			HOScene->GetObjectByID("posindicator2")->Show(false);
		}
		return;
	}

	//if (!ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHeadTaken &&
	//	HammerPopupRect.Intersection(&mPos))
	//{
	//	Cursor::SetMode(CA_Look);
	//	if (Control::Input->LBclicked())
	//	{
	//		_activePopup = P1R3_HammerPopup;
	//		_scenePopups[_activePopup]->OpenPopup();
	//	}
	//	return;
	//}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
			//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_P1Room2Wine;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif
	if( IsMouseOverGoBackArea(&mPos) )
	{

#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitDown,eArea_L3_S2_P1Room2Wine);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L3_S2_P1Room2Wine);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L3_S2_P1Room2Wine, 2);
			}
#else
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_P1Room2Wine;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}	
	}
}

void L3_S2_P1Room3Lock::ReceiveMessage(int val)
{	
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		TaskPendingUpdate();
		Hud->ShowMap();
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )
		{
			if (_activePopup != P1R3_NoPopup)
			{
				_scenePopups[_activePopup]->HandleInventoryDrop(dropPos, itemVal);
			}
			else
			{
				GFInstance->WrongInvDropMsg(Hud);
			}
		}
	}
	else if( val == INV_CLOSE )
	{		
		if (_activePopup == P1R3_PipePickPopup &&
			ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe3R3Taken &&
			ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHeadTaken)
		{
			_scenePopups[_activePopup]->ClosePopup();
		}
		/*else if (_activePopup == P1R3_HammerPopup &&
			ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerHeadTaken)
		{
			_scenePopups[_activePopup]->ClosePopup();
		}*/
		else if (_activePopup != P1R3_NoPopup)
		{

		}
	}	
}

void L3_S2_P1Room3Lock::TaskPendingUpdate()
{
	if (!ProfileSystem::Instance->ActiveProfile.L3_S2_M6_PillerPuzzleComplete)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_P1Room1Start] = true;
}

float L3_S2_P1Room3Lock::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L3_S2_P1Room3Lock::ProcessNonHOHint()
{
	if (_activePopup != P1R3_NoPopup)
	{
		if (_activePopup > 0)
		{
			if (ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S2_Hammer])
				_scenePopups[_activePopup]->ProcessHint();
			else
				Hud->InitHintTrailEmit(830, 170);

			return;
		}
		else
		{
			_scenePopups[_activePopup]->ProcessHint();
			return;
		}
	}

	map<L3_S2_P1R3Popups, Popup*>::iterator itr;
	for (itr = _scenePopups.begin(); itr != _scenePopups.end(); itr++)
	{
		if (itr->second->isTaskLeft())
		{
			Hud->InitHintTrailEmit(&PopupOrigins[itr->first]);
			return;
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe1R1Taken 
	|| !ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe2R2Taken)
	{
		Hud->InitHintTrailEmit(&HoveredBackBtnRectMax, true, CA_ExitDown);
		return;
	}

	if (ProfileSystem::Instance->ActiveProfile.L3_S2_R3DoorOpen[SolutionDoor])
	{
		Hud->InitHintTrailEmit(&DoorRect[SolutionDoor], false, CA_ExitLeft);
		return;
	}
	else if (ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerTaken)
	{
		Hud->InitHintTrailEmit(&DoorRect[SolutionDoor]);
		return;
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvToothR2Taken)
	{
		Hud->InitHintTrailEmit(&HoveredBackBtnRectMax, true, CA_ExitDown);
		return;
	}

	if(Hud->ShowHintForComboInvItems())
		return;

	Hud->teleportSystem->ShowTeleport();
}

void L3_S2_P1Room3Lock::SparkleUpdate()
{
	isSparkleUpdatePending = false;

	if (ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		return;

	if (_activePopup != P1R3_NoPopup)
		return;

	map<L3_S2_P1R3Popups, Popup*>::iterator itr;
	for (itr = _scenePopups.begin(); itr != _scenePopups.end(); itr++)
	{
		if (itr->second->isTaskLeft())
		{
			sparklePointsVec.push_back(CPoint(PopupOrigins[itr->first].GetCenterX(), PopupOrigins[itr->first].GetCenterY(), 0));
			isSparkleUpdatePending = true;
		}
	}

	if (!isSparkleUpdatePending)
		return;

	HiddenObjectSystem::SparkleUpdate();
}

void L3_S2_P1Room3Lock::OnClosePopup(Popup *popup)
{
	switch (_activePopup)
	{
	case P1R3_NoPopup:
		break;

	case P1R3_PipePickPopup:
		if (ProfileSystem::Instance->ActiveProfile.L3_S2_R3StonesRemoved)
		{
			_stonesClosed->SetEnable(false);
			_stonesCleared->SetEnable(true);
		}
		break;
	case P1R3_Door1Popup:
	case P1R3_Door2Popup:
	case P1R3_Door3Popup:
			if (ProfileSystem::Instance->ActiveProfile.L3_S2_R3DoorOpen[((L3_S2_P1R3LockOpen*) popup)->getDoorIndex()])
			{
				_doors[((L3_S2_P1R3LockOpen*) popup)->getDoorIndex()]->FadeOut(1.5f); 
			}

		break;

	default:
		break;
	}
	HOScene->GetObjectByID("handMazeMap")->Show(true);
	HOScene->GetObjectByID("posindicator2")->Show(true);
	_activePopup = P1R3_NoPopup;
}

void L3_S2_P1Room3Lock::NavigateToScene()
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