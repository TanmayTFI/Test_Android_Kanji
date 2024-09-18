//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L5_S3_Pond.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "MeshObject.h"
#include "L5S3_Pop1BowVine.h"
#include "L5S3_Popup2Stone.h"
#include "L5S3_Pop3ToUnderWater.h"

const CRectangle  PuzzleRect(466.0f,301.0f,118.0f,128.0f);
const CRectangle  HORect(340.0f,450.0f,210.0f,102.0f);
const CRectangle  DoorRect(295.0f,214.0f,170.0f,214.0f);
const CRectangle  VineTreeRect(803.0f,215.0f,164.0f,147.0f);
const CRectangle  RockRect(591.0f,432.0f,131.0f,150.0f);

static CPoint popupHintPoints[ePopL5S3_Max] = { CPoint(VineTreeRect.GetCenterX(), VineTreeRect.GetCenterY(), 0), 
												CPoint(RockRect.GetCenterX(), RockRect.GetCenterY(), 0),
												CPoint(HORect.GetCenterX(), HORect.GetCenterY(), 0)
											  };

L5_S3_Pond::L5_S3_Pond()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV5\\S3\\S3.SCN");

	CObject* _tree1 = HOScene->GetObjectByID("c_t_1");
	_tree1->SetPivot(-59, 14);
	_tree1->Animate(0, 0, 4, 0, 4, 1.57f);

	CObject* _tree2 = HOScene->GetObjectByID("c_t_2");
	_tree2->SetPivot(-28, 24);
	_tree2->Animate(0, 0, 2, 0, 4.5f);

	holinesFX = NULL;
	hodotsFX = NULL;
	s3waterRippleFX = new SuperFX("PRT\\L5\\s3waterRipple.INI", PointSystem::CreateCPoint(589,610,0), 1100); 
	s3waterRippleFX->Start();
	s3waterRippleFX->AdvanceByTime(2.0f);

	s3waterdropsFX = new SuperFX("PRT\\L5\\s3waterdrops.INI", PointSystem::CreateCPoint(686,31,0), 1100); 
	s3waterdropsFX->Start();
	s3waterdropsFX->AdvanceByTime(2.0f);

	S3PoolWaterFallFX = new SuperFX("PRT\\L5\\S3PoolWaterFall.INI", PointSystem::CreateCPoint(835,172,0), 2); 
	S3PoolWaterFallFX->Start();
	S3PoolWaterFallFX->AdvanceByTime(2.0f);

	_startPlayingSnake = false;
	_cvdo_introSnake = NULL;
	time = 0;
	subIndex = 0;
	vidTimer = 0;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));
	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("anaconda"));

	if (!ProfileSystem::Instance->ActiveProfile.L5_S3_IntroVdoCompleted)
	{
		//CRectangle RectVidFull(171, 96, 1024, 576);
		CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
		_cvdo_introSnake = new CVideo("OGV\\L5\\S3INTROSNAKE.OGV", RectVidFull, false, eZOrder_CutsceneFull);
		//_cvdo_introSnake->SetScale(1.334f);
		_cvdo_introSnake->SetScale(GFApp->video_aspectRatio);
		_cvdo_introSnake->PlayVideo();
		Hud->EnableInput(false);
		Hud->HideHud(false);
		_startPlayingSnake = true;
	}

	Stone1 = HOScene->GetObjectByID("stone_lt");
	Bstone = HOScene->GetObjectByID("B_stone");
	vineTree_2 = HOScene->GetObjectByID("vine_tree_2");

	Bstone->SetEnable(false);
	vineTree_2->SetEnable(false);

	if (ProfileSystem::Instance->ActiveProfile.L5_S3_EmblemPlacedOnStone)
	{
		Bstone->SetEnable(true);
		Stone1->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L5_S3_VineFreed)
	{
		vineTree_2->SetEnable(true);
		vineTree_2->SetPivot(9, -159);
		vineTree_2->Animate(0, 0, 1, 0, 4);
		HOScene->GetObjectByID("vine_tree_1")->SetEnable(false);
	}

	_jAnim_DounganDoorOPen = NULL;

	if (ProfileSystem::Instance->ActiveProfile.L5_S3_LeftDoorOpened)
	{
		HOScene->GetObjectByID("door")->SetEnable(false);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L5_S3_PZCompleted)
	{
		_jAnim_DounganDoorOPen = new JSONAnimator();
		_jAnim_DounganDoorOPen->parseAndLoadJSON("ANIM\\L5\\S3_DOUNGANDOOROPEN.JSON", HOScene);
		_jAnim_DounganDoorOPen->PlayAnim();
		Control::Audio->QuickLoadAndPlaySFX("s3pz1_doorOpen");
		ProfileSystem::Instance->ActiveProfile.L5_S3_LeftDoorOpened = true;
	}

	S3PoolWaterFallBottomFX = new SuperFX("PRT\\L5\\S3PoolWaterFallBottom.INI", PointSystem::CreateCPoint(791,495,0), 3); 
	S3PoolWaterFallBottomFX->Start();
	S3PoolWaterFallBottomFX->AdvanceByTime(2.0f);

	S3CrystalParticle1FX = new SuperFX("PRT\\L5\\S3CrystalParticle1.INI", PointSystem::CreateCPoint(1191,540,0), 50); 
	S3CrystalParticle1FX->Start();
	S3CrystalParticle1FX->AdvanceByTime(2.0f);

	S3CrystalParticle2FX = new SuperFX("PRT\\L5\\S3CrystalParticle2.INI", PointSystem::CreateCPoint(251,218,0), 50); 
	S3CrystalParticle2FX->Start();
	S3CrystalParticle2FX->AdvanceByTime(2.0f);

	S3CrystalParticle3FX = new SuperFX("PRT\\L5\\S3CrystalParticle3.INI", PointSystem::CreateCPoint(148,573,0), 50); 
	S3CrystalParticle3FX->Start();
	S3CrystalParticle3FX->AdvanceByTime(2.0f);

	S3CrystalParticle4FX = new SuperFX("PRT\\L5\\S3CrystalParticle4.INI", PointSystem::CreateCPoint(1215,288,0), 50); 
	S3CrystalParticle4FX->Start();
	S3CrystalParticle4FX->AdvanceByTime(2.0f);

	MeshObject* waterObj = new MeshObject(HOScene->GetObjectByID("water"), 20, 20);
	waterObj->meshFX->SetSpeedAndDisplacement(4, 9, 5, 2);
	HOScene->InsertObject(waterObj, 1);

	InitPopups();

	SceneObjectsUpdate();

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL5s3PondAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL5s3PondAmb) )		
	{
		Control::Audio->PlaySample(aAmbL5s3PondAmb,true);
	}
	
	//MUSIC_SFX

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L5_S3_Pond]  = true;

	HOScene->GetObjectByID("Bag")->SetEnable(false);
	if (ProfileSystem::Instance->ActiveProfile.L5_S3_isHoActive && !ProfileSystem::Instance->ActiveProfile.L5_S3_PZHaveHook)
	{
		HOScene->GetObjectByID("Bag")->SetEnable(true);
		HOScene->GetObjectByID("Bag")->Animate(2, 3, 0, 0, 3);
	}

	HOScene->GetObjectByID("ChainBreakAnim")->SetEnable(false);
	if (!ProfileSystem::Instance->ActiveProfile.L5_S3_HookLostBannerShowed && ProfileSystem::Instance->ActiveProfile.L5_S3_isLostHook)
	{
		HOScene->GetObjectByID("ChainHang")->SetEnable(false);
		HOScene->GetObjectByID("ChainBreakAnim")->SetEnable(true);
		HOScene->GetObjectByID("ChainBreakAnim")->PlayAnimation(1);
		//Objective Add
		if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L5O3_OpenTheDungeon] )
		{
			ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L5O3_OpenTheDungeon] = true;
			ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
			= eObj_L5O3_OpenTheDungeon;
			//Increment Objectives
			ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
			Hud->ShowObjectiveFX(eObj_L5O3_OpenTheDungeon);

			Control::Audio->PlaySample(aSFXMagic1);
		}

		ProfileSystem::Instance->ActiveProfile.L5_S3_HookLostBannerShowed = true;
		Hud->ShowBannerText(Banner_L5_S3_HookFallsIntoTheWater);
	}

	if (!ProfileSystem::Instance->ActiveProfile.L5_S3_isLostHook || ProfileSystem::Instance->ActiveProfile.L5_S3_PZState >= 62)  //L5S3PZ_HookPlaced
	{
		HOScene->GetObjectByID("ChainHang")->SetEnable(false);
	}

	if (!ProfileSystem::Instance->ActiveProfile.L5_S3_memClothTaken)
	{
		Hud->ShowOrbGlowing();
	}

	if(!ProfileSystem::Instance->ActiveProfile.L5_S3_HOUnderWaterCompleted && ProfileSystem::Instance->ActiveProfile.L5_S3_isHoActive)
	{
		if( !ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles )
		{
			holinesFX = new SuperFX("PRT\\L5\\holines.INI", PointSystem::CreateCPoint(467,488,0), 1100); 
			holinesFX->Start();
			holinesFX->AdvanceByTime(2.0f);

			hodotsFX = new SuperFX("PRT\\L5\\hodots.INI", PointSystem::CreateCPoint(468,490,0), 1100); 
			hodotsFX->Start();
			hodotsFX->AdvanceByTime(2.0f);
		}
	}
}

void L5_S3_Pond::InitPopups()
{
	ActivePopupID = ePopInactive;

	CPoint zoomOrigin = CPoint(VineTreeRect.GetCenterX(), VineTreeRect.GetCenterY(), 0);
	L5S3_Pop1BowVine *bowPopup = new L5S3_Pop1BowVine(ePop1_L5S3Pop1_BowVine, Hud, HOScene,  eZOrder_Popup, zoomOrigin, CC_CALLBACK_1(L5_S3_Pond::OnPopupClosed, this));
	PopupArray[ePop1_L5S3Pop1_BowVine] = bowPopup;
	bowPopup->setInterface(this);

	if (!ProfileSystem::Instance->ActiveProfile.L5_S3_InvMoonSolutionTaken)
	{
		CPoint zoomOrigin = CPoint(RockRect.GetCenterX(), RockRect.GetCenterY(), 0);
		stonePopup = new L5S3_PopStone(ePop2_L5S3Pop2_Stone, Hud, HOScene,  eZOrder_Popup, zoomOrigin, CC_CALLBACK_1(L5_S3_Pond::OnPopupClosed, this));
		PopupArray[ePop2_L5S3Pop2_Stone] = stonePopup;
		stonePopup->setInterface(this);
	}

	if (!ProfileSystem::Instance->ActiveProfile.L5_S3_isHoActive)
	{
		CPoint zoomOrigin = CPoint(HORect.GetCenterX(), HORect.GetCenterY(), 0);
		L5S3_Pop3ToUnderWater *popup = new L5S3_Pop3ToUnderWater(ePop3_L5S3_ToUnderWater, Hud, HOScene,  eZOrder_Popup, zoomOrigin, CC_CALLBACK_1(L5_S3_Pond::OnPopupClosed, this));
		PopupArray[ePop3_L5S3_ToUnderWater] = popup;
		popup->setInterface(this);
	}
}

void L5_S3_Pond::OnPopupClosed(Popup* popup)
{
	ActivePopupID = ePopInactive;
	//int popID = popup->GetPopupId();  //Sac: Unused variable, so commented it
	SceneObjectsUpdate();
}

void L5_S3_Pond::SceneObjectsUpdate()
{
	TaskPendingUpdate();

	if (ProfileSystem::Instance->ActiveProfile.L5_S3_memClothTaken)
	{
		HOScene->GetObjectByID("cloth")->SetEnable(false);
	}
}

void L5_S3_Pond::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if(_startPlayingSnake)
		return;

	if (!ProfileSystem::Instance->ActiveProfile.L5_S3_InvMoonSolutionTaken)
		if(stonePopup->_state  == L5S3_Pop2_PlayingMemVdo)
			return;

	if( Control::Audio->IsPlaying(aTrackL5ho) )
		{
			Control::Audio->Samples[aTrackL5ho]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL5ho]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL5ho);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL5theme) )
					  Control::Audio->PlaySample(aTrackL5theme, true);
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
	else if( Control::Audio->IsPlaying(aTrackL5puzzle) )
		{
			Control::Audio->Samples[aTrackL5puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL5puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL5puzzle);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL5theme) )
					  Control::Audio->PlaySample(aTrackL5theme, true);
					  
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
		else
		{
			if( !Control::Audio->IsPlaying(aTrackL5theme))
			{
				Control::Audio->TurnOffLvlMscTracks();
				Control::Audio->PlaySample(aTrackL5theme, true);
			}
		}
}

void L5_S3_Pond::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	#ifdef _PARTICLEEDITORENABLED
		if (_isParticleEditorActive)
		{
			return;
		}
	#endif

	CPoint mPos = Control::Input->Mpos();

	if( ActivePopupID != ePopInactive )
		return;

	Cursor::SetMode(CA_Normal);
	if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		SparkleUpdate();

	if (_startPlayingSnake && _cvdo_introSnake)
	{
		//Subtitles Code
		time = _cvdo_introSnake->GetTime();

		if (time >= subtitles[subIndex].begin && time < subtitles[subIndex].end)
		{
			subtitleText->SetText(subtitles[subIndex].text);
		}
		else
			subtitleText->SetText("");
#ifdef FORCE_CLOSE_VIDEO
		vidTimer += Control::LogicRate;
#endif
		if(
#ifndef FORCE_CLOSE_VID_END
			_cvdo_introSnake->isEndReached() ||
#endif
			vidTimer > 16)
		{
			_cvdo_introSnake->StopAndFadeOutVideo(1);
			_startPlayingSnake = false;
			subtitleText->SetText("");
			Hud->AllowInput = true;
			Hud->ShowHud(false);

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L5P2_SnakeVideoEnds])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L5P2_SnakeVideoEnds] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L5P2_SnakeVideoEnds;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
				Hud->ShowJrnEntryFX();
			}

			ProfileSystem::Instance->ActiveProfile.L5_S3_IntroVdoCompleted = true;
			Hud->ShowBannerText(Banner_L5_S3_AfterCutScene);
		}

		return;
	}


	if (VineTreeRect.Intersection(&mPos))
	{
		if (((L5S3_Pop1BowVine*)PopupArray[ePop1_L5S3Pop1_BowVine])->isTaskLeft2())
		{
			Cursor::SetMode(CA_Look);
			if (Control::Input->LBclicked())
			{
				ActivePopupID = ePop1_L5S3Pop1_BowVine;
				PopupArray[ePop1_L5S3Pop1_BowVine]->OpenPopup();
			}
		}
		else
		{
			
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitUp, eArea_L5_S5_MountainBelow);
#else
		Cursor::SetMode(CA_ExitUp, eArea_L5_S5_MountainBelow);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L5_S5_MountainBelow, 0, VineTreeRect);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S5_MountainBelow;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
				
			}
		}
	}
	
	if (!ProfileSystem::Instance->ActiveProfile.L5_S3_InvMoonSolutionTaken &&
		RockRect.Intersection(&mPos))
	{
		Cursor::SetMode(CA_Look);
		if (Control::Input->LBclicked())
		{
			ActivePopupID = ePop2_L5S3Pop2_Stone;
			PopupArray[ePop2_L5S3Pop2_Stone]->OpenPopup();
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L5_S3_HOUnderWaterCompleted && 
		HORect.Intersection(&mPos))
	{
		if (ProfileSystem::Instance->ActiveProfile.L5_S3_isHoActive)
		{
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_Look);
#else
		Cursor::SetMode(CA_Look);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L5_S3_HOUnderwater, 0, HORect);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S3_HOUnderwater;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
				
			}
		}
		else
		{
			if(ProfileSystem::Instance->ActiveProfile.L5_S3_PZState == 61)// waiting for hook
			{
				Cursor::SetMode(CA_Look);
				if (Control::Input->LBclicked())
				{
					ActivePopupID = ePop3_L5S3_ToUnderWater;
					PopupArray[ePop3_L5S3_ToUnderWater]->OpenPopup();
				}
			}
		}
	}

	if (!ProfileSystem::Instance->ActiveProfile.L5_S3_LeftDoorOpened &&
		PuzzleRect.Intersection(&mPos))
	{
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_Look);
#else
		Cursor::SetMode(CA_Look);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L5_S3_PZDungeonDoor, 0, PuzzleRect);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S3_PZDungeonDoor;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
			
		}
	}

	if (DoorRect.Intersection(&mPos))
	{
		if (ProfileSystem::Instance->ActiveProfile.L5_S3_LeftDoorOpened)
		{
			
#ifdef TOUCH_UI
	///	if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitLeft, eArea_L5_S4_Dungeon);
#else
		Cursor::SetMode(CA_ExitLeft, eArea_L5_S4_Dungeon);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				ProfileSystem::Instance->ActiveProfile.L5_S3_EnteredLeftDoor = true;
				GFHud->InvBoxCtrl->GoToScene(eArea_L5_S4_Dungeon, 3, DoorRect);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.L5_S3_EnteredLeftDoor = true;
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S4_Dungeon;
				SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
					
			}
		}
		else
		{
			if (Control::Input->LBclicked())
			{
				Hud->ShowBannerText(Banner_L5_S3_ClickOnDungonDoor);
			}
		}
	}
	
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
		//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S2_TigerCloseup;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif	



	if( IsMouseOverGoBackArea(&mPos ) )
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitDown,eArea_L5_S2_TigerCloseup);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L5_S2_TigerCloseup);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L5_S2_TigerCloseup, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S2_TigerCloseup;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
									
		}	
	}

}

void L5_S3_Pond::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	map<eL5S3PopEnums, Popup*>::iterator it;
	int i = 0;
	for (it = PopupArray.begin(); it != PopupArray.end(); ++it, ++i)
		if(it->second->isTaskLeft())
			isAnyTaskLeft = true;


	if (!ProfileSystem::Instance->ActiveProfile.L5_S3_LeftDoorOpened && 
		ProfileSystem::Instance->ActiveProfile.L5_S5_InvWaterproofBagTaken)
		isAnyTaskLeft = true;

	if (ProfileSystem::Instance->ActiveProfile.L5_S3_LeftDoorOpened &&
		!ProfileSystem::Instance->ActiveProfile.L5_S3_EnteredLeftDoor)
	{
		isAnyTaskLeft = true;
	}


	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S3_Pond] = isAnyTaskLeft;
}

void L5_S3_Pond::SparkleUpdate()
{
	if (ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		return;

	if(isSparkleUpdatePending)
	{
		isSparkleUpdatePending = false;
		if(SparkleFX==NULL)
		{
			SparkleFX = new SuperFX("PRT\\glint.INI",PointSystem::CreateCPoint(0,0,0),120);		
		}
		//Game Logic Goes here
		numSparkles = 0;
		currentSparkleID = 0;

		//Check for Popup Tasks
		map<eL5S3PopEnums, Popup*>::iterator it;
		for (it = PopupArray.begin(); it != PopupArray.end(); ++it)
		{
			if(it->second->isTaskLeft())
			{
				sparklePoints[numSparkles].SetXY(popupHintPoints[int(it->first)].x, popupHintPoints[int(it->first)].y);
				numSparkles++;
				isSparkleUpdatePending = true;
			}
		}

		if (ProfileSystem::Instance->ActiveProfile.L5_S3_isHoActive &&
			!ProfileSystem::Instance->ActiveProfile.L5_S3_HOUnderWaterCompleted)
		{
			sparklePoints[numSparkles].SetXY(HORect.GetCenterX(), HORect.GetCenterY());
			numSparkles++;
			isSparkleUpdatePending = true;
		}

		if (!ProfileSystem::Instance->ActiveProfile.L5_S3_isLostHook ||
			(!ProfileSystem::Instance->ActiveProfile.L5_S3_LeftDoorOpened &&
			ProfileSystem::Instance->ActiveProfile.L5_S3_PZHaveHook))
		{
			sparklePoints[numSparkles].SetXY(PuzzleRect.GetCenterX(), PuzzleRect.GetCenterY());
			numSparkles++;
			isSparkleUpdatePending = true;
		}

		if ( ProfileSystem::Instance->ActiveProfile.L5_S3_VineFreed )
		{
			sparklePoints[numSparkles].SetXY(VineTreeRect.GetCenterX(), VineTreeRect.GetCenterY());
			numSparkles++;
			isSparkleUpdatePending = true;
		}

	}
	sparkleTimer += Control::LogicRate;
	if(sparkleTimer>sparkleThreshold)
	{
		sparkleTimer = 0;
		if(numSparkles>0)
		{			
			SparkleFX->SetPos(&sparklePoints[currentSparkleID]);
			SparkleFX->Start();
			currentSparkleID += 1;
			if(currentSparkleID>=numSparkles)
				currentSparkleID = 0;
		}
	}
}

void L5_S3_Pond::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == UPDATE_SCENE_OBJECTS )
	{
		SceneObjectsUpdate();
		if(ActivePopupID != ePopInactive)
		{
			if(PopupArray[eL5S3PopEnums(ActivePopupID)])
				PopupArray[eL5S3PopEnums(ActivePopupID)]->UpdateBgGraphic();
		}
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		TaskPendingUpdate();
		Hud->ShowMap();
	}
	else if( val == MEMORY_OBJFX_REACHED )
	{
		if(stonePopup)
			stonePopup->StartMemoryCutscene();
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			if(ActivePopupID == ePopInactive)
			{
				//check for popup actions
				//UpdatePopups(dropPos,true);
				if(itemVal == eInv_L5_S1_BowArrowFixed)
				{
					ActivePopupID = ePop1_L5S3Pop1_BowVine;
					((L5S3_Pop1BowVine*)PopupArray[ePop1_L5S3Pop1_BowVine])->OpenPopup(true);
				}
				else
				{
					//If No popups responded
					GFInstance->WrongInvDropMsg(Hud);
				}
				return;
			}
			else
			{
				if(PopupArray[eL5S3PopEnums(ActivePopupID)])
					PopupArray[eL5S3PopEnums(ActivePopupID)]->HandleInventoryDrop(dropPos, itemVal);
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
		}
	}
	else if( val == INV_CLOSE )
	{		
		if (ActivePopupID == ePop2_L5S3Pop2_Stone)
		{
			if (ProfileSystem::Instance->ActiveProfile.L5_S3_InvMoonSolutionTaken)
			{
				PopupArray[eL5S3PopEnums(ActivePopupID)]->ClosePopup();
			}
		}

	}
}

void L5_S3_Pond::ProcessSkip()
{
}

void L5_S3_Pond::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S3AngelStatue])
		{
			if (ActivePopupID != ePopInactive)
				InitHintTrailEmit(950, 200);
			else
				InitHintTrailEmit(920, 220);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}
	int iNonHOHint = 0; 
	int MaxNonHOHints = 9;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
			break;

		case 0:  ++iNonHOHint;
			if( ActivePopupID != ePopInactive )
			{
				PopupArray[eL5S3PopEnums(ActivePopupID)]->ProcessHint();
				return;
			}
			break;
		case 1:
			{
				++iNonHOHint;
				map<eL5S3PopEnums, Popup*>::iterator it;

				if (ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S1_BowArrowFixed])
				{
					for (it = PopupArray.begin(); it != PopupArray.end(); ++it)
					{
						if (it->second->isTaskLeft())
						{
							if (it->first == ePop1_L5S3Pop1_BowVine)
								Hud->ShowHintForInvItem(eInv_L5_S1_BowArrowFixed, &VineTreeRect);
							else
								InitHintTrailEmit(&popupHintPoints[int(it->first)], false);

							return;
						}
					}
				}
				else if (ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S1_Bow])
				{
					Hud->ShowHintForInvItem(eInv_L5_S1_Bow);
				}
			}
		break;
		case 2: ++iNonHOHint;
				{
					if (ProfileSystem::Instance->ActiveProfile.L5_S3_isHoActive &&
						!ProfileSystem::Instance->ActiveProfile.L5_S3_HOUnderWaterCompleted
						 && ProfileSystem::Instance->ActiveProfile.L5_S3_isLostHook)
					{
						InitHintTrailEmit(&HORect, false);
						return;
					}

					if (!ProfileSystem::Instance->ActiveProfile.L5_S3_HOUnderWaterCompleted && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S5_DivingBag] && ProfileSystem::Instance->ActiveProfile.L5_S3_isLostHook)
					{
						InitHintTrailEmit(&HORect, false);
						return;
					}
				}
			break;
		case 3: ++iNonHOHint;
				{
					if (!ProfileSystem::Instance->ActiveProfile.L5_S3_LeftDoorOpened &&
						ProfileSystem::Instance->ActiveProfile.L5_S3_PZHaveHook)
					{
						InitHintTrailEmit(&PuzzleRect, false);
						return;
					}
				}
			break;
		case 4: ++iNonHOHint;
				{
					if (ProfileSystem::Instance->ActiveProfile.L5_S3_LeftDoorOpened && !ProfileSystem::Instance->ActiveProfile.L5_S3_EnteredLeftDoor)
					{
						InitHintTrailEmit(&DoorRect, true, CA_ExitLeft);
						return;
					}
					else if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S4_Dungeon])
					{
						InitHintTrailEmit(&DoorRect, true, CA_ExitLeft);
						return;
					}
					else if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S2_TigerCloseup])
					{
						InitHintTrailEmit(&HoveredBackBtnRectMax, true, CA_ExitDown);
						return;
					}
				}
			break;
		case 5: ++iNonHOHint;
			if (!ProfileSystem::Instance->ActiveProfile.L5_S3_InvMoonSolutionTaken && ProfileSystem::Instance->ActiveProfile.L5_S5_InvStoneEmblemTaken)
			{
				InitHintTrailEmit(&RockRect, true, CA_ExitDown);
				return;
			}
			break;
		case 6: ++iNonHOHint;
				{
					if ( ProfileSystem::Instance->ActiveProfile.L5_S3_VineFreed &&
						ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S5_MountainBelow])
					{
						InitHintTrailEmit(&VineTreeRect, true, CA_ExitUp);
						return;
					}
				}
			break;
		case 7:  ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
		case 8: ++iNonHOHint;
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

L5_S3_Pond::~L5_S3_Pond()
{
	TaskPendingUpdate();
	SAFE_DELETE(s3waterRippleFX);
	SAFE_DELETE(s3waterdropsFX);
	SAFE_DELETE(_cvdo_introSnake);
	SAFE_DELETE(S3PoolWaterFallFX);
	SAFE_DELETE(S3PoolWaterFallBottomFX);
	SAFE_DELETE(S3CrystalParticle1FX);
	SAFE_DELETE(S3CrystalParticle2FX);
	SAFE_DELETE(S3CrystalParticle3FX);
	SAFE_DELETE(S3CrystalParticle4FX);
	SAFE_DELETE(_jAnim_DounganDoorOPen);
	SAFE_DELETE(holinesFX);
	SAFE_DELETE(hodotsFX);
	SAFE_DELETE(subtitleText);

	map<eL5S3PopEnums, Popup*>::iterator itr;
	for (itr = PopupArray.begin(); itr != PopupArray.end(); ++itr)
	{
		delete itr->second;
	}

	PopupArray.clear();

	if( !Control::Audio->IsToLevelTrackRoom() )
		if( Control::Audio->IsPlaying(aTrackLevel5) )
			Control::Audio->StopSample(aTrackLevel5);
	/*if( ProfileSystem::Instance->ActiveProfile.CurrentArea != eArea_L1_S2_Frontyard )
	{
	Control::Audio->StopSample(aLightRainAmb);
	Control::Audio->Samples[aLightRainAmb]->SetVolume(100);			
	}*/
}

void L5_S3_Pond::UpdateHOSystem(Popup *popup, int UpdateVal)
{
	if (ProfileSystem::Instance->ActiveProfile.L5_S3_EmblemPlacedOnStone)
	{
		Bstone->SetEnable(true);
		Stone1->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L5_S3_VineFreed)
	{
		vineTree_2->SetEnable(true);
		vineTree_2->SetPivot(9, -159);
		vineTree_2->Animate(0, 0, 1, 0, 4);
		HOScene->GetObjectByID("vine_tree_1")->SetEnable(false);
	}

}

void L5_S3_Pond::NewArea()
{
	SendMessage(ControlExit, ID, HOSM_NewArea);
}
void L5_S3_Pond::NavigateToScene()
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