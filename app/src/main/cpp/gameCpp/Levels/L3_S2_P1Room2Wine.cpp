//====================================
// DQFULP
// Hidden Object System
// Author : Sreeraj
// LEVEL3 - SCENE3 : P1Room2Wine
//====================================

#include "L3_S2_P1Room2Wine.h"
#include "Hud.h"
#include "Rect.h"
#include "Polygon.h"
#include "Util.h"

const CRectangle  PipePickRect(432.0f,514.0f,223.0f,179.0f);
const CRectangle  CreeperPuzzleRect(786.0f,24.0f,159.0f,460.0f);

const CRectangle  InvDropDoor1(173.0f,139.0f,206.0f,397.0f);
const CRectangle  InvDropDoor2(598.0f,256.0f,160.0f,240.0f);
const CRectangle  InvDropDoor3(996.0f,134.0f,215.0f,396.0f);

const CRectangle DoorRectsArray[] = {InvDropDoor1, InvDropDoor2, InvDropDoor3};
const eCursorAnim NextAreaPoint[] = {CA_ExitLeft, CA_ExitUp, CA_ExitRight};
static std::string VinePos[3] = {"Left", "Center", "Right"};

static int SolutionDoor = 1;

L3_S2_P1Room2Wine::L3_S2_P1Room2Wine()
{
	LoadHud(INVENTORY_V1);
	LoadScene("LV3\\S2\\MAZE2\\MAZE2.SCN");
	musicFadeOutTimer = 0;
	HOScene->GetObjectByID("sickle")->SetAlpha(0);
	HOScene->GetObjectByID("popfireanim2")->SetScale(0.80f);
	HOScene->GetObjectByID("popfireanim3")->SetScale(0.50f);
	HOScene->GetObjectByID("popfireanim4")->SetScale(0.60f);
	HOScene->GetObjectByID("popfireanim5")->SetScale(0.80f);
	HOScene->GetObjectByID("popfireanim6")->SetScale(0.90f);
	HOScene->GetObjectByID("top_light")->SetScale(1.90f);
	HOScene->GetObjectByID("glow")->Animate(0.4f, 1.0f, 3.0); 

	for (int i = 1; i <= 6; i++)
	{
		HOScene->GetObjectByID("popfireanim"+MKSTR(i))->SetFrame(rand()%80);  // total 96 frames for fireanim
	}

	S2M2LightFX = new SuperFX("PRT\\L3\\S2M2Light.INI", PointSystem::CreateCPoint(435,152,0), 25); 
	S2M2LightFX->Start();
	S2M2LightFX->AdvanceByTime(2.0f);

	S2M1SmokeFX = new SuperFX("PRT\\L3\\S2M1Smoke.INI", PointSystem::CreateCPoint(265,494,0), 20); 
	S2M1SmokeFX->Start();
	S2M1SmokeFX->AdvanceByTime(2.0f);

	S2M1Smoke2FX = new SuperFX("PRT\\L3\\S2M1Smoke2.INI", PointSystem::CreateCPoint(694,413,0), 21); 
	S2M1Smoke2FX->Start();
	S2M1Smoke2FX->AdvanceByTime(2.0f);

	S2M1Smoke3FX = new SuperFX("PRT\\L3\\S2M1Smoke3.INI", PointSystem::CreateCPoint(1143,415,0), 22); 
	S2M1Smoke3FX->Start();
	S2M1Smoke3FX->AdvanceByTime(2.0f);

	JSONAnimator *janim_RemoveLeftVine = new JSONAnimator(0, CC_CALLBACK_1(L3_S2_P1Room2Wine::OnAnimatonComplete, this));
	janim_RemoveLeftVine->parseAndLoadJSON("ANIM\\L3\\S2MAZE2_LEFTVINE.JSON",HOScene);

	JSONAnimator *janim_RemoveCenterVine = new JSONAnimator(1, CC_CALLBACK_1(L3_S2_P1Room2Wine::OnAnimatonComplete, this));
	janim_RemoveCenterVine->parseAndLoadJSON("ANIM\\L3\\S2MAZE2_CENTERVINE.JSON",HOScene);
	
	JSONAnimator *janim_RemoveRightVine = new JSONAnimator(2, CC_CALLBACK_1(L3_S2_P1Room2Wine::OnAnimatonComplete, this));
	janim_RemoveRightVine->parseAndLoadJSON("ANIM\\L3\\S2MAZE2_RIGHTVINE.JSON",HOScene);

	_janim_RemoveVine.push_back(janim_RemoveLeftVine);
	_janim_RemoveVine.push_back(janim_RemoveCenterVine);
	_janim_RemoveVine.push_back(janim_RemoveRightVine);

	for (int i = 0; i < 3; i++)
	{
		if (ProfileSystem::Instance->ActiveProfile.L3_S2_vineRemovedR2Status[i])
		{
			HOScene->GetObjectByID("vine_"+ VinePos[i] +"_1")->SetEnable(false);
			HOScene->GetObjectByID("vine_"+ VinePos[i] +"_2")->SetEnable(false);
		}
	}

	if (ProfileSystem::Instance->ActiveProfile.L3_S2_InvToothR2Taken)
	{
		HOScene->GetObjectByID("teeth")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe2R2Taken)
	{
		HOScene->GetObjectByID("pipeInMud")->SetEnable(false);
	}

	_activePopup = R2_NoPopup;

	CPoint zoomOrigin = CPoint(PipePickRect.GetCenterX(), PipePickRect.GetCenterY(), 0);
	_pipePopup = new L3_S2_M2PipePopUp(0, Hud, HOScene, eZOrder_Popup, zoomOrigin, CC_CALLBACK_1(L3_S2_P1Room2Wine::OnClosePopup, this));

	zoomOrigin = CPoint(CreeperPuzzleRect.GetCenterX(), CreeperPuzzleRect.GetCenterY(), 0); 
	_puzzlePopup = new L3_S2_P1R2CreeperPuzzle(1, Hud, HOScene, eZOrder_Popup, zoomOrigin, CC_CALLBACK_1(L3_S2_P1Room2Wine::OnClosePopup, this));

	ProfileSystem::Instance->ActiveProfile.L3_S2_MazeCurrentRoom = eArea_L3_S2_P1Room2Wine;

	if(IsVisitedForFirstTime)
	{
		Hud->ShowBannerText(Banner_L3_S2_M2_EnterFirstTime);
	}

	if(ProfileSystem::Instance->ActiveProfile.L3_S2_MazeMapUsed)
	{
		HOScene->Load("LV3\\S2\\MAZECOMMON\\MAP\\MAPHAND.SCN");
		for (int i = 0; i < 5; i++)
		{
			if(i!=1)
				HOScene->GetObjectByID(std::string("posindicator" + MKSTR(i)).c_str())->SetEnable(false);
			else
				HOScene->GetObjectByID(std::string("posindicator" + MKSTR(i)).c_str())->Animate(0.85f, 1.0f, 1.25f);
		}
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

L3_S2_P1Room2Wine::~L3_S2_P1Room2Wine()
{
	SAFE_DELETE(_pipePopup);
	SAFE_DELETE(_puzzlePopup);
	SAFE_DELETE(S2M1SmokeFX);
	SAFE_DELETE(S2M1Smoke2FX);
	SAFE_DELETE(S2M1Smoke3FX);
	SAFE_DELETE(S2M2LightFX);
	
	for (int i = 0; i < (int)(_janim_RemoveVine.size()); i++)
	{
		SAFE_DELETE(_janim_RemoveVine[i]);
	}

	_janim_RemoveVine.clear();

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL3s2Maze);
	Control::Audio->UnloadSample(aAmbL3s2Maze);
	//MUSIC_SFX
}

float L3_S2_P1Room2Wine::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L3_S2_P1Room2Wine::Render(const CPoint* pCam)
{

}

void L3_S2_P1Room2Wine::Update()
{
	NavigateToScene();
	HiddenObjectSystem::HOUpdate();

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing)
		return;

	if (_activePopup != R2_NoPopup)
		return;

	CPoint mPos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	for (int i = 0; i < (int)(_janim_RemoveVine.size()); i++)
	{
		if (ProfileSystem::Instance->ActiveProfile.L3_S2_vineRemovedR2Status[i] &&
			DoorRectsArray[i].Intersection(&mPos))
		{
			if (ProfileSystem::Instance->ActiveProfile.L3_S2_MazeMapUsed)
			{


#ifdef TOUCH_UI
				//if(!GFHud->InvBoxCtrl->navTapped)
				//	Cursor::SetMode(NextAreaPoint[i]);
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
							GFHud->InvBoxCtrl->GoToScene(eArea_L3_S2_P1Room3Lock, 0, DoorRectsArray[i]);
						}
						else
						{
							GFHud->InvBoxCtrl->GoToScene(eArea_L3_S2_P1DeadEnd, 0, DoorRectsArray[i]);
						}
					}
#else
					if (i == SolutionDoor)
					{
						ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_P1Room3Lock;
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
				Cursor::SetMode(CA_Gear);
		}
		else if (DoorRectsArray[i].Intersection(&mPos))
		{
			Cursor::SetMode(CA_Gear);
			
			if (Control::Input->LBclicked())
			{
				Hud->ShowBannerText(Banner_L3_S2_M2_ClickCreeper);
			}
		}
	}

	if (!ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe2R2Taken &&
		PipePickRect.Intersection(&mPos))
	{
		Cursor::SetMode(CA_Look);

		if (Control::Input->LBclicked())
		{
			_pipePopup->OpenPopup();
			_activePopup = R2_PipePickPopup;
			HOScene->GetObjectByID("handMazeMap")->Show(false);
			HOScene->GetObjectByID("posindicator1")->Show(false);
		}

		return;
	}

	if (!ProfileSystem::Instance->ActiveProfile.L3_S2_InvToothR2Taken &&
		CreeperPuzzleRect.Intersection(&mPos))
	{
		Cursor::SetMode(CA_Look);

		if (Control::Input->LBclicked())
		{
			_puzzlePopup->OpenPopup();
			_activePopup = R2_CreeperPuzzle;
			HOScene->GetObjectByID("handMazeMap")->Show(false);
			HOScene->GetObjectByID("posindicator1")->Show(false);
		}
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
			//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_P1Room1Start;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif
	if( IsMouseOverGoBackArea(&mPos) )
	{

#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L3_S2_P1Room1Start);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L3_S2_P1Room1Start);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L3_S2_P1Room1Start, 2);
			}
#else
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_P1Room1Start;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}	
	}
}

void L3_S2_P1Room2Wine::Required()
{
	HiddenObjectSystem::Required();

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

void L3_S2_P1Room2Wine::ReceiveMessage(int val)
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
			HandleInventoryDrop(dropPos, itemVal);
	}
	else if( val == INV_CLOSE )
	{		
		if (_activePopup == R2_PipePickPopup &&
			ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe2R2Taken)
		{
			_pipePopup->ClosePopup();
		}

		if (_activePopup == R2_CreeperPuzzle && 
			ProfileSystem::Instance->ActiveProfile.L3_S2_InvToothR2Taken)
		{
			_puzzlePopup->ClosePopup();
		}
	}	
}

void L3_S2_P1Room2Wine::TaskPendingUpdate()
{
	if (!ProfileSystem::Instance->ActiveProfile.L3_S2_M6_PillerPuzzleComplete)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_P1Room1Start] = true;
}

void L3_S2_P1Room2Wine::ProcessSkip()
{
	if (_activePopup == R2_CreeperPuzzle)
	{
		_puzzlePopup->ProcessSkip();
	}
}

void L3_S2_P1Room2Wine::ProcessNonHOHint()
{
	if (_activePopup == R2_PipePickPopup)
	{
		_pipePopup->ProcessHint();
	}
	else if(_activePopup == R2_CreeperPuzzle)
	{
		_puzzlePopup->ProcessHint();
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe2R2Taken)
	{
		Hud->InitHintTrailEmit(&PipePickRect);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvToothR2Taken)
	{
		Hud->InitHintTrailEmit(&CreeperPuzzleRect);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe1R1Taken)
	{
		Hud->InitHintTrailEmit(&HoveredBackBtnRectMax, true, CA_ExitDown);
		return;
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L3_S2_vineRemovedR2Status[SolutionDoor])
	{
		Hud->ShowHintForInvItem(eInv_L3S2_Sickle, &DoorRectsArray[SolutionDoor]);
	}
	else
	{
		Hud->InitHintTrailEmit(&DoorRectsArray[SolutionDoor],false,CA_ExitUp);
	}
}

void L3_S2_P1Room2Wine::OnClosePopup(Popup *popup)
{
	_activePopup = R2_NoPopup;

	if (ProfileSystem::Instance->ActiveProfile.L3_S2_InvToothR2Taken)
	{
		HOScene->GetObjectByID("teeth")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe2R2Taken)
	{
		HOScene->GetObjectByID("pipeInMud")->SetEnable(false);
	}
	HOScene->GetObjectByID("handMazeMap")->Show(true);
	HOScene->GetObjectByID("posindicator1")->Show(true);
}

void L3_S2_P1Room2Wine::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (_activePopup == R2_NoPopup)
	{
		if (itemVal == eInv_L3S2_Sickle)
		{
			for (int i = 0; i < (int)(_janim_RemoveVine.size()); i++)
			{
				if (!ProfileSystem::Instance->ActiveProfile.L3_S2_vineRemovedR2Status[i] && 
					DoorRectsArray[i].Intersection(&dropPos))
				{
					if(i==0)
						Control::Audio->QuickLoadAndPlaySFX("s2m2_vinecutL");
					else if(i==1)
						Control::Audio->QuickLoadAndPlaySFX("s2m2_vinecutC");
					else if(i==2)
						Control::Audio->QuickLoadAndPlaySFX("s2m2_vinecut");

					_janim_RemoveVine[i]->PlayAnim(false);
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 
					return;
				}
			}
		}
	}

	GFInstance->WrongInvDropMsg(Hud);
}

void L3_S2_P1Room2Wine::OnAnimatonComplete(int animID)
{
	ProfileSystem::Instance->ActiveProfile.L3_S2_vineRemovedR2Status[animID] = true;
}

void L3_S2_P1Room2Wine::SparkleUpdate()
{
	isSparkleUpdatePending = false;

	if (ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		return;

	if (_activePopup != R2_NoPopup)
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe2R2Taken)
	{
		sparklePointsVec.push_back(CPoint(PipePickRect.GetCenterX(), PipePickRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}
	
	if(!ProfileSystem::Instance->ActiveProfile.L3_S2_InvToothR2Taken)
	{
		sparklePointsVec.push_back(CPoint(CreeperPuzzleRect.GetCenterX(), CreeperPuzzleRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}

	if (!isSparkleUpdatePending)
		return;

	HiddenObjectSystem::SparkleUpdate();
}
void L3_S2_P1Room2Wine::NavigateToScene()
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