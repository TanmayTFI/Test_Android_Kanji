//====================================
// DQFULP
// Hidden Object System
// Author : Sreeraj
// LEVEL3 - SCENE3 : P1Room5Pipe
//====================================

#include "L3_S2_P1Room5Pipe.h"
#include "Banners.h"
#include "HiddenObjectEnums.h"
#include "Rect.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"

const CPoint		CenterPoint(841,502,0);

const CRectangle  DoorRectLeft(334.0f,125.0f,121.0f,221.0f);
const CRectangle  DoorRectCenter(771.0f,161.0f,99.0f,174.0f);
const CRectangle  DoorRectRight(979.0f,158.0f,90.0f,188.0f);

const CRectangle DoorRects[] = {DoorRectLeft, DoorRectCenter, DoorRectRight};
const eCursorAnim NextAreaPoint[] = {CA_ExitLeft, CA_ExitUp, CA_ExitRight};

const int SolutionDoor = 2;

L3_S2_P1Room5Pipe::L3_S2_P1Room5Pipe()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV3\\S2\\MAZE5\\PIPEMAZESCENE.SCN");
	musicFadeOutTimer = 0;
	Visible = true;	//To render anything with renderlistentity

	pipePuzzlePopup = NULL;
	_isPipePuzzleSolved = false;

	InitSceneProperties();
	InitPopups();

	if ( !ProfileSystem::Instance->ActiveProfile.L3_S2_M5_PipePuzzleCompleted)
	{
		_solvedMask = HOScene->GetObjectByID("PipeFixed");
		_solvedMask->SetEnable(false);
	}
	else
	{
		_isPipePuzzleSolved = true;
	}

	for (int i = 0; i < 3; i++)
	{
		CObject *doorlt = HOScene->GetObjectByID(string("door_")+MKSTR(i + 1)+string("_lt"));
		CObject *doorrt = HOScene->GetObjectByID(string("door_")+MKSTR(i + 1)+string("_rt"));
		
		if (!ProfileSystem::Instance->ActiveProfile.L3_S2_M5_PipeSceneDoorsOpend)
		{
			_doors.push_back(doorlt);
			_doors.push_back(doorrt);
		}
		else
		{
			doorlt->SetEnable(false);
			doorrt->SetEnable(false);
		}
	}

	ProfileSystem::Instance->ActiveProfile.L3_S2_MazeCurrentRoom = eArea_L3_S2_P1Room5Pipe;

	if(ProfileSystem::Instance->ActiveProfile.L3_S2_MazeMapUsed)
	{
		HOScene->Load("LV3\\S2\\MAZECOMMON\\MAP\\MAPHAND.SCN");

		for (int i = 0; i < 5; i++)
		{
			if(i!=4)
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

void L3_S2_P1Room5Pipe::InitSceneProperties()
{
	int ArrayPts[] = {592,424,680,610,1308,564,903,412};
	PuzzleAreaRect =  new CPolygon(4);
	PuzzleAreaRect->Init(ArrayPts);

	int LiverPts[] = {519,259,585,279,600,387,456,504,388,447};
	_liverPolygon=  new CPolygon(5);
	_liverPolygon->Init(LiverPts);

	_janim_liverAnim = new JSONAnimator(0, CC_CALLBACK_1(L3_S2_P1Room5Pipe::OnAnimatonComplete, this));
	_janim_liverAnim->parseAndLoadJSON("ANIM\\L3\\MAZE5LIVERANIM.JSON",HOScene);

	CObject *popfireanim1 = HOScene->GetObjectByID("popfireanim1");
	CObject *popfireanim2 = HOScene->GetObjectByID("popfireanim2");
	CObject *popfireanim3 = HOScene->GetObjectByID("popfireanim3");
	CObject *popfireanim4 = HOScene->GetObjectByID("popfireanim4");
	CObject *popfireanim5 = HOScene->GetObjectByID("popfireanim5");

	popfireanim2->SetScale(0.8f);
	popfireanim3->SetScale(0.6f);
	popfireanim4->SetScale(0.5f);
	popfireanim5->SetScale(0.6f);

	popfireanim1->SetFrame(42);
	popfireanim2->SetFrame(75);
	popfireanim3->SetFrame(5);
	popfireanim4->SetFrame(34);
	popfireanim5->SetFrame(50);

	CObject *vine_cr = HOScene->GetObjectByID("vine_cr");
	CObject *vine_rt_1 = HOScene->GetObjectByID("vine_rt_1");
	CObject *vine_rt_2 = HOScene->GetObjectByID("vine_rt_2");
	CObject *plant_rt = HOScene->GetObjectByID("plant_rt");
	CObject *plant_lt = HOScene->GetObjectByID("plant_lt");
	CObject *light_ray = HOScene->GetObjectByID("light_ray");

	light_ray->Animate(0.65f, 0.8f, 4);
	light_ray->SetBlendMode(eBlend_AlphaAdditive);

	vine_cr->Animate(0, 7, 0, 0, 8);

	vine_rt_1->Animate(0, 0, 6, 0, 7);
	vine_rt_1->SetPivot(0, 0 - 250);
	vine_rt_2->Animate(0, 5, 4, 0, 6);
	vine_rt_2->SetPivot(0, 0 - 150);

	plant_lt->Animate(0, 0, 5, 0, 4);
	plant_rt->Animate(0, 0, 7, 0, 7);

	pipepumpFX = new SuperFX("PRT\\L3\\pipepump.INI", PointSystem::CreateCPoint(744,490,0), 1100); 

	HOScene->GetObjectByID("WaterSpray")->SetBlendMode(eBlend_AlphaAdditive);

	M5LightParticleFX = new SuperFX("PRT\\L3\\M5LightParticle.INI", PointSystem::CreateCPoint(739,260,0), 1100); 
	M5LightParticleFX->Start();
	M5LightParticleFX->AdvanceByTime(2.0f);
	M5LightParticleFX->ZOrder = (pipePuzzlePopup) ? pipePuzzlePopup->ZOrder - 1 : M5LightParticleFX->ZOrder;
}

void L3_S2_P1Room5Pipe::InitPopups()
{	
	ActivePopupID = ePopInactive;

	if ( !ProfileSystem::Instance->ActiveProfile.L3_S2_M5_PipePuzzleCompleted)
	{
		pipePuzzlePopup = new L3_S2_P1_MazePipePopup(1, Hud, HOScene, eZOrder_Popup, PuzzleAreaRect->center, CC_CALLBACK_1(L3_S2_P1Room5Pipe::OnClosePopup, this));
	}
}

L3_S2_P1Room5Pipe::~L3_S2_P1Room5Pipe()
{
	Hud->InvBoxCtrl->ResetToInvBox();

	SAFE_DELETE(pipePuzzlePopup);
	SAFE_DELETE(PuzzleAreaRect);
	SAFE_DELETE(pipepumpFX);
	SAFE_DELETE(M5LightParticleFX);
	SAFE_DELETE(_janim_liverAnim);
	SAFE_DELETE(_liverPolygon);
	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL3s2Maze);
	Control::Audio->UnloadSample(aAmbL3s2Maze);
	//MUSIC_SFX
}

void L3_S2_P1Room5Pipe::Required()
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

void L3_S2_P1Room5Pipe::Update()
{
#ifdef _PARTICLEEDITORENABLED
	if (_isParticleEditorActive)
	{
		return;
	}
#endif

	NavigateToScene();
	HiddenObjectSystem::HOUpdate();

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing)
		return;

	if (ActivePopupID != ePopInactive)
		return;
	
	CPoint mPos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
			//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_P1Room4Match3;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif
	if( IsMouseOverGoBackArea(&mPos) )
	{

#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L3_S2_P1Room4Match3);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L3_S2_P1Room4Match3);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L3_S2_P1Room4Match3, 2);
			}
#else
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_P1Room4Match3;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}	

		return;
	} 

	if (ProfileSystem::Instance->ActiveProfile.L3_S2_M5_PipeSceneDoorsOpend)
	{
		for (int i = 0; i < 3; i++)
		{
			if (DoorRects[i].Intersection(&mPos))
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
								GFHud->InvBoxCtrl->GoToScene(eArea_L3_S2_P1Room6Pillar, 0, DoorRects[i]);
							}
							else
							{
								GFHud->InvBoxCtrl->GoToScene(eArea_L3_S2_P1DeadEnd, 0, DoorRects[i]);
							}
						}
#else
						if (i == SolutionDoor)
						{
							ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_P1Room6Pillar;
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
		}

		return;
	}
	else
	{
		Cursor::SetMode(CA_Gear);

		for (int i = 0; i < 3; i++)
		{
			if (DoorRects[i].Intersection(&mPos))
			{
				if (Control::Input->LBclicked())
				{
					Hud->ShowBannerText(Banner_L3_S2_M5_ClickOnLockeDoor);
				}
			}
		}
	}

	if (pipePuzzlePopup && 
		PuzzleAreaRect->Intersection(&mPos) && 
		!pipePuzzlePopup->getIsPuzzleSolved())
	{
		Cursor::SetMode(CA_Look);

		if (Control::Input->LBclicked())
		{
			OpenPuzzlePopup();
		}
	}
	else
	{
		Cursor::SetMode(CA_Normal);
	}

	if (_liverPolygon->Intersection(&mPos))
	{
		Cursor::SetMode(CA_HandPointy);

		if (Control::Input->LBclicked())
		{
			_janim_liverAnim->PlayAnim(false);
			Control::Audio->QuickLoadAndPlaySFX("s2m5PipePump");
		}
	}

}

void L3_S2_P1Room5Pipe::Render(const CPoint* pCam)
{  
	
}

void L3_S2_P1Room5Pipe::ReceiveMessage(int val)
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
			if (ActivePopupID == ePopL3S2_PZP1PipePuzzle)
			{
				pipePuzzlePopup->HandleInventoryDrop(dropPos, itemVal);
			}
			else if (itemVal == eInv_L3S2_PipePiece1Room1 && PuzzleAreaRect->Intersection(&dropPos))
			{
				OpenPuzzlePopup();
				//pipePuzzlePopup->HandleInventoryDrop(dropPos, itemVal);
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

void L3_S2_P1Room5Pipe::TaskPendingUpdate()
{
	if (!ProfileSystem::Instance->ActiveProfile.L3_S2_M6_PillerPuzzleComplete)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_P1Room1Start] = true;
}

float L3_S2_P1Room5Pipe::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L3_S2_P1Room5Pipe::ProcessNonHOHint()
{
	if (!ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe1R1Taken ||
		!ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe2R2Taken ||
		!ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe3R3Taken ||
		!ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe4R4Taken)
	{
		Hud->InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
		return;
	}
	else
	{
		if (ActivePopupID == ePopL3S2_PZP1PipePuzzle)
		{
			pipePuzzlePopup->ProcessHint();
			return;
		}
	}

	if (ProfileSystem::Instance->ActiveProfile.L3_S2_M5_PipeSceneDoorsOpend)
	{
		Hud->InitHintTrailEmit(&DoorRects[SolutionDoor], false, CA_ExitRight);
		return;
	}

	if (!ProfileSystem::Instance->ActiveProfile.L3_S2_M5_PipePuzzleCompleted)
	{
		Hud->InitHintTrailEmit(PuzzleAreaRect->center.x - 100, PuzzleAreaRect->center.y);
	}
	else
	{
		Hud->InitHintTrailEmit(_liverPolygon->center.x, _liverPolygon->center.y);
	}
}

void L3_S2_P1Room5Pipe::SparkleUpdate()
{
	isSparkleUpdatePending = false;

	if (ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		return;

	if (ActivePopupID != ePopInactive)
		return;

	if (!ProfileSystem::Instance->ActiveProfile.L3_S2_M5_PipePuzzleCompleted)
	{
		sparklePointsVec.push_back(PuzzleAreaRect->center - CPoint(100, 0, 0));
		isSparkleUpdatePending = true;
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S2_M5_PipeSceneDoorsOpend)
	{
		sparklePointsVec.push_back(_liverPolygon->center);
		isSparkleUpdatePending = true;
	}

	if (!isSparkleUpdatePending)
		return;

	HiddenObjectSystem::SparkleUpdate();
}

void L3_S2_P1Room5Pipe::ProcessSkip()
{
	if (ActivePopupID == ePopL3S2_PZP1PipePuzzle)
	{
		pipePuzzlePopup->ProcessSkip();
	}
}

void L3_S2_P1Room5Pipe::OnClosePopup(Popup *popup)
{
	if (ActivePopupID == ePopL3S2_PZP1PipePuzzle)
	{
		if (pipePuzzlePopup->getIsPuzzleSolved())
		{
			_solvedMask->SetEnable(true);
			_isPipePuzzleSolved = true;
		}
	}
	HOScene->GetObjectByID("handMazeMap")->Show(true);
	HOScene->GetObjectByID("posindicator4")->Show(true);
	ActivePopupID = ePopInactive;
}

void L3_S2_P1Room5Pipe::OpenPuzzlePopup()
{
	if (_isPipePuzzleSolved)
		return;

	HOScene->GetObjectByID("handMazeMap")->Show(false);
	HOScene->GetObjectByID("posindicator4")->Show(false);
	pipePuzzlePopup->OpenPopup();
	ActivePopupID = ePopL3S2_PZP1PipePuzzle;
	Cursor::SetMode(CA_Normal);
}

void L3_S2_P1Room5Pipe::OnAnimatonComplete(int animID)
{
	if (animID == 0)
	{
		if (!_isPipePuzzleSolved)
		{
			Hud->ShowBannerText(Banner_L3_S2_M5_ClickOnPumb);
			pipepumpFX->Start();
			Control::Audio->QuickLoadAndPlaySFX("s2m5DisorderPumping");
			return;
		}

		ProfileSystem::Instance->ActiveProfile.L3_S2_M5_PipeSceneDoorsOpend = true;

		Control::Audio->QuickLoadAndPlaySFX("s2m5_dooropen");

		HOScene->GetObjectByID("WaterSpray")->PlayAnimation(1);

		for (int i = 0; i < (int)(_doors.size()); i++)
		{
			float valArrLeft[5] = {0, _doors[i]->Pos.x - 40, _doors[i]->Pos.y, -1, -1};
			float valArrRight[5] = {0, _doors[i]->Pos.x + 40, _doors[i]->Pos.y, -1, -1};
			(i%2) ? _doors[i]->TweenTo(valArrRight, 1.0f, 0, true) : _doors[i]->TweenTo(valArrLeft, 1.0f, 0, true);
			_doors[i] = NULL;
		}

		_doors.clear();
	}
}


void L3_S2_P1Room5Pipe::NavigateToScene()
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
