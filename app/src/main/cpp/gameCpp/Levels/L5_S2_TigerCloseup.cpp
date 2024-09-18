
//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L5_S2_TigerCloseup.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "MeshObject.h"

const CRectangle  	PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  SceneRectPond(970.0f,247.0f,232.0f,255.0f);
const CRectangle  SceneRectPZBlocks(686.0f,490.0f,198.0f,110.0f);
const CRectangle  InfoRectFace(1025.0f,2.0f,136.0f,172.0f);


L5_S2_TigerCloseup::L5_S2_TigerCloseup()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV5\\S2\\S2.SCN");

	HOScene->GetObjectByID("light")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("light")->Animate(0.15f,1.0f,7.0f);

	HOScene->GetObjectByID("plant1")->SetPivot(-10, 43);
	HOScene->GetObjectByID("plant1")->Animate(0,0,4,0,8.0f);

	HOScene->GetObjectByID("plant2")->SetPivot(0, 79);
	HOScene->GetObjectByID("plant2")->Animate(0,0,2,0,10.0f);

	HOScene->GetObjectByID("plant3")->SetPivot(129, 0);
	HOScene->GetObjectByID("plant3")->Animate(0,0,2,0,10.0f);

	HOScene->GetObjectByID("plant4")->SetPivot(49, 52);
	HOScene->GetObjectByID("plant4")->Animate(0,0,2,0,15.0f);

	HOScene->GetObjectByID("plant5")->SetPivot(0, 91);
	HOScene->GetObjectByID("plant5")->Animate(0,0,2,0,10.0f);

	HOScene->GetObjectByID("plant6")->SetPivot(-79, 76);
	HOScene->GetObjectByID("plant6")->Animate(0,0,2,0,10.0f);

	HOScene->GetObjectByID("branch")->SetPivot(-42, 0);
	HOScene->GetObjectByID("branch")->Animate(0,0,4,0,8.0f);

	HOScene->GetObjectByID("TreeLogOnWater")->SetPivot(-10, 43);
	HOScene->GetObjectByID("TreeLogOnWater")->Animate(0,1,0,0,3.0f);

	S2MoonFXFX = new SuperFX("PRT\\L5\\S2MoonFX.INI", PointSystem::CreateCPoint(313,167,0), 100); 
	S2MoonFXFX->Start();
	S2MoonFXFX->AdvanceByTime(2.0f);

	MeshObject* waterObj1 = new MeshObject(HOScene->GetObjectByID("PondWaterAnim"), 10, 10);
	waterObj1->meshFX->SetSpeedAndDisplacement(4, 4, 3, 3);
	HOScene->InsertObject(waterObj1, 1);
		

	InitPopups();
	#ifdef _CEBUILD
	//Morph Object Add
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S2Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("morphtarget"), HOScene->GetObjectByID("morphbase"));
		const CRectangle  MorphRect(328.0f,256.0f,151.0f,136.0f);

		Hud->setMorphObjRect(MorphRect);
	}
	else
	{
		HOScene->GetObjectByID("morphtarget")->SetEnable(false);
	}
#else
	HOScene->GetObjectByID("morphtarget")->SetEnable(false);
#endif 

	SceneObjectsUpdate();

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L5_S2_TigerCloseup]  = true;

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL5s2TigerCloseUpAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL5s2TigerCloseUpAmb) )		
	{
		Control::Audio->PlaySample(aAmbL5s2TigerCloseUpAmb,true);
	}

	//Objective Add
	if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L5O2_FindWayOutOfAtharwaSwamps] )
	{
		ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L5O2_FindWayOutOfAtharwaSwamps] = true;
		ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
		= eObj_L5O2_FindWayOutOfAtharwaSwamps;
		//Increment Objectives
		ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
		Hud->ShowObjectiveFX(eObj_L5O2_FindWayOutOfAtharwaSwamps);

		Control::Audio->PlaySample(aSFXMagic1);
	}
	
	//MUSIC_SFX
}

void L5_S2_TigerCloseup::InitPopups()
{
	ActivePopupID = ePopInactive;

	for (int i = 0; i < ePopL5S2_Max; i++)
	{
		PopupArray[i] = NULL;
		PopStates[i] = ePopupState_Inactive;
		PopPolyArray[i] = NULL;
	}

	//Popup 1
	if(!ProfileSystem::Instance->ActiveProfile.L5_S2_InvBadgeTaken || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S2FootStone] )
	{
		int p1rect[] = {440,432,549,428,606,459,619,515,473,527};
		PopPolyArray[ePop1_L5S2Skelton] =  new CPolygon(5);
		PopPolyArray[ePop1_L5S2Skelton]->Init(p1rect);

		PopupArray[ePop1_L5S2Skelton] = new L5S2_Pop1Skelton(ePop1_L5S2Skelton,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop1_L5S2Skelton]->center, CC_CALLBACK_1(L5_S2_TigerCloseup::OnPopupClosed, this));
		PopupArray[ePop1_L5S2Skelton]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePop1_L5S2Skelton] = ePopupState_Closed;
	}

	//Popup 2
	if(!ProfileSystem::Instance->ActiveProfile.L5_S2_LogFell)
	{
		int p2rect[] = {972,314,1142,329,1140,455,977,486,903,429};
		PopPolyArray[ePop2_L5S2Log] =  new CPolygon(5);
		PopPolyArray[ePop2_L5S2Log]->Init(p2rect);

		PopupArray[ePop2_L5S2Log] = new L5S2_Pop2Log(ePop2_L5S2Log,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop2_L5S2Log]->center, CC_CALLBACK_1(L5_S2_TigerCloseup::OnPopupClosed, this));
		PopupArray[ePop2_L5S2Log]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePop2_L5S2Log] = ePopupState_Closed;
	}

	////Popup 3
	if( !ProfileSystem::Instance->ActiveProfile.L5_S2_GreaseTaken || 
		!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S2DogStatue] )
	{
		int p3rect[] = {603,319,698,314,699,440,623,459,575,407};
		PopPolyArray[ePop3_L5S2Grease] = new CPolygon(5);
		PopPolyArray[ePop3_L5S2Grease]->Init(p3rect);

		PopupArray[ePop3_L5S2Grease] = new L5S2_Pop3Grease(ePop3_L5S2Grease,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop3_L5S2Grease]->center, CC_CALLBACK_1(L5_S2_TigerCloseup::OnPopupClosed, this));
		PopupArray[ePop3_L5S2Grease]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePop3_L5S2Grease] = ePopupState_Closed;
	}
}

void L5_S2_TigerCloseup::OnPopupClosed(Popup* popup)
{
	ActivePopupID = ePopInactive;
	isSparkleUpdatePending = true;
	SceneObjectsUpdate();
	switch (popup->GetPopupId())
	{
	case 0:
	#ifdef _CEBUILD
		if (ProfileSystem::Instance->ActiveProfile.L5_S2_InvBadgeTaken && ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S2FootStone])
	#else
		if(ProfileSystem::Instance->ActiveProfile.L5_S2_InvBadgeTaken && ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S2FootStone])
	#endif 		
		{
			//disable popup
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}
		break;
	case 1:
		if(ProfileSystem::Instance->ActiveProfile.L5_S2_LogFell)
		{
			//disable popup
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}
		break;
	case 2:
		#ifdef _CEBUILD
		if(ProfileSystem::Instance->ActiveProfile.L5_S2_GreaseTaken  && ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S2DogStatue])
	#else
		if(ProfileSystem::Instance->ActiveProfile.L5_S2_GreaseTaken)
	#endif 	
		{
			//disable popup
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}
		break;
	
	default:
		break;
	}
}

void L5_S2_TigerCloseup::SceneObjectsUpdate()
{
	TaskPendingUpdate();
	if(ProfileSystem::Instance->ActiveProfile.L5_S2_InvBadgeTaken)
		 HOScene->GetObjectByID("badge")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L5_S2_LogFell)
	{
		 HOScene->GetObjectByID("treeLog")->SetEnable(false);
		 HOScene->GetObjectByID("TreeLogOnWater")->SetEnable(true);
	}
	else
		HOScene->GetObjectByID("TreeLogOnWater")->SetEnable(false);

	if( ProfileSystem::Instance->ActiveProfile.L5_S2_InvHammerTaken)
	{
		 HOScene->GetObjectByID("beforePuzzle")->SetEnable(false);
		 HOScene->GetObjectByID("afterPuzzle")->SetEnable(true);
	}
	else
	{
		 HOScene->GetObjectByID("afterPuzzle")->SetEnable(false);
		 HOScene->GetObjectByID("beforePuzzle")->SetEnable(true);
	}
}

void L5_S2_TigerCloseup::Required()
{
	HiddenObjectSystem::Required();	//forced update

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

void L5_S2_TigerCloseup::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	CPoint pos = Control::Input->Mpos();

	if( ActivePopupID == ePopInactive )
	{
		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed()) {
				if(Hud!=NULL && GFInstance->IsInGameMenuPopup != true)Hud->CallPauseMenu();
			}
		#endif
		Cursor::SetMode(CA_Normal);
		if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
			SparkleUpdate();

		//popup click checks
		for (int i = 0; i < ePopL5S2_Max; i++)
		{
			if(PopStates[i] != ePopupState_Inactive) //if popup is active
			{
				if(PopPolyArray[i]!=NULL && PopPolyArray[i]->Intersection(&pos))
				{
					Cursor::SetMode(CA_Look);
					if( Control::Input->LBclicked() )
					{
						Cursor::SetMode(CA_Normal);
						ActivePopupID = i;
						PopupArray[i]->OpenPopup();
					}
					return;
				}
			}
		}

		if( ProfileSystem::Instance->ActiveProfile.L5_S2_LogFell && SceneRectPond.Intersection(&pos) )
		{	
			
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
//Cursor::SetMode(CA_ExitRight,eArea_L5_S3_Pond);
#else
		Cursor::SetMode(CA_ExitRight,eArea_L5_S3_Pond);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L5_S3_Pond, 1, SceneRectPond);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S3_Pond;
				SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
					
			}
		}
		else if( !ProfileSystem::Instance->ActiveProfile.L5_S2_InvHammerTaken && SceneRectPZBlocks.Intersection(&pos) )
		{
#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_Look);
#else
		Cursor::SetMode(CA_Look);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L5_S2_PZBlocks, 0, SceneRectPZBlocks);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S2_PZBlocks;
				SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
					
			}
		}

		if(InfoRectFace.Intersection(&pos))
		{
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L5craft");
			}
		}
	}
}

void L5_S2_TigerCloseup::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	for (int i = 0; i < ePopL5S2_Max; i++)
	{
		if(PopStates[i] != ePopupState_Inactive) //if popup is active
		{
			if(PopupArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S2_TigerCloseup] = isAnyTaskLeft;

	if(ProfileSystem::Instance->ActiveProfile.L5_S2_GreaseTaken && !ProfileSystem::Instance->ActiveProfile.L5_S4_GreaseUsed)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S4_Dungeon] = true;

	if(ProfileSystem::Instance->ActiveProfile.L5_S2_InvSoldierBadgeTaken&& !ProfileSystem::Instance->ActiveProfile.L5_S5_IsSoldierBadgeUsedOnDoor)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S5_MountainBelow] = true;
}

void L5_S2_TigerCloseup::SparkleUpdate()
{
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
		for (int i = 0; i < ePopL5S2_Max; i++)
		{
			if(PopStates[i] != ePopupState_Inactive) //if popup is active
			{
				if(PopupArray[i]->isTaskLeft())
				{
					sparklePoints[numSparkles].SetXY(PopPolyArray[i]->center.x, PopPolyArray[i]->center.y);
					numSparkles++;
				}
			}
		}

		if(!ProfileSystem::Instance->ActiveProfile.L5_S2_PZBlocksPuzzleSolved )
		{
			sparklePoints[numSparkles].SetXY(SceneRectPZBlocks.GetCenterX(), SceneRectPZBlocks.GetCenterY());
			numSparkles++;
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

void L5_S2_TigerCloseup::ReceiveMessage(int val)
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
			if(PopupArray[ActivePopupID]!=NULL)
				PopupArray[ActivePopupID]->UpdateBgGraphic();
		}
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
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			if(ActivePopupID == ePopInactive)
			{
				//check for popup actions
				//UpdatePopups(dropPos,true);
				if(ActivePopupID == ePopInactive)
				{
					//If No popups responded
					GFInstance->WrongInvDropMsg(Hud);
				}
				return;
			}
			else
			{
				if(PopupArray[ActivePopupID]!=NULL)
					PopupArray[ActivePopupID]->HandleInventoryDrop(dropPos, itemVal);
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
		}
	}
	else if( val == INV_CLOSE )
	{		


	}
}

void L5_S2_TigerCloseup::ProcessSkip()
{
}

void L5_S2_TigerCloseup::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S2Collected)
		{
			InitHintTrailEmit(410, 320);
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S2DogStatue])
		{
			if (ActivePopupID != ePopInactive)
				InitHintTrailEmit(870, 400);
			else
				InitHintTrailEmit(640, 410);
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S2FootStone])
		{
			if (ActivePopupID != ePopInactive)
				InitHintTrailEmit(900, 390);
			else
				InitHintTrailEmit(500, 480);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}
	int iNonHOHint = 0; 
	int MaxNonHOHints = 6;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
			break;

		case 0: ++iNonHOHint;
			if( ActivePopupID != ePopInactive )
			{
				PopupArray[ActivePopupID]->ProcessHint();
				return;
			}
			break;
		case 1: ++iNonHOHint;
			for (int i = 0; i < ePopL5S2_Max; i++)
			{
				if(PopStates[i] != ePopupState_Inactive) //if popup is active
				{
					if(PopupArray[i]->isTaskLeft())
					{
						InitHintTrailEmit(&PopPolyArray[i]->center,false);
						return;
					}
				}
			}
			break;
		case 2: ++iNonHOHint;
			 if( !ProfileSystem::Instance->ActiveProfile.L5_S2_InvHammerTaken)
			{
			InitHintTrailEmit(&SceneRectPZBlocks);
			return;
			}
			break;
		case 3: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S3_Pond] || (!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L5_S3_Pond] && ProfileSystem::Instance->ActiveProfile.L5_S2_LogFell) || !ProfileSystem::Instance->ActiveProfile.L5_S3_PZCompleted)
			{
			InitHintTrailEmit(&SceneRectPond,true,CA_ExitRight);
			return;
			}
			break;
		case 4: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
		case 5: ++iNonHOHint;
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

L5_S2_TigerCloseup::~L5_S2_TigerCloseup()
{
	TaskPendingUpdate();
	SAFE_DELETE(S2MoonFXFX);

	for (int i = 0; i < ePopL5S2_Max; i++)
	{
		SAFE_DELETE(PopupArray[i]);
		SAFE_DELETE(PopPolyArray[i]);
	}

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL5s2TigerCloseUpAmb);
	Control::Audio->UnloadSample(aAmbL5s2TigerCloseUpAmb);
	//MUSIC_SFX
}
void L5_S2_TigerCloseup::NavigateToScene()
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