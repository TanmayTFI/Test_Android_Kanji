//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L5_S5_MountainBelow.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"

const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  InfoRectTopExitDoor(208.0f,86.0f,228.0f,199.0f);
const CRectangle  SceneRectTopExitDoor(208.0f,86.0f,228.0f,199.0f);
const CRectangle  InvDestRectBamboo(171.0f,63.0f,392.0f,422.0f);
const CRectangle  PuzzleRectSunClock(298.0f,372.0f,286.0f,170.0f);

const CRectangle  HORectGanapathi(767.0f,113.0f,334.0f,170.0f);

L5_S5_MountainBelow::L5_S5_MountainBelow()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV5\\S5\\S5.SCN");

	HOScene->GetObjectByID("smallfire")->SetScale(2.0f);

	HOScene->GetObjectByID("fireglow")->Animate(0.65f,0.95f,3.0f, 0, eBlend_AlphaAdditive);

	HOScene->GetObjectByID("statueglow")->Animate(0.65f,0.95f,4.0f, 0, eBlend_AlphaAdditive);

	HOScene->GetObjectByID("topdoorlight")->Animate(0.45f,0.85f,4.0f, 0, eBlend_AlphaAdditive);

	HOScene->GetObjectByID("mushroomglowleft")->Animate(0.25f,0.85f,4.0f, 0, eBlend_AlphaAdditive);
	HOScene->GetObjectByID("mushroomglowright")->Animate(0.35f,0.95f,5.0f, 0, eBlend_AlphaAdditive);

	HOScene->GetObjectByID("sunpuzzleglow")->Animate(0.25f,0.75f,4.0f, 0, eBlend_AlphaAdditive);
	HOScene->GetObjectByID("sunpuzzleglow2")->Animate(0.55f,0.85f,2.0f, 0, eBlend_AlphaAdditive);
	
	HOScene->GetObjectByID("greenorbray")->Animate(0.55f,0.85f,2.75f, 0, eBlend_AlphaAdditive);
	HOScene->GetObjectByID("topdoorglowrings")->Animate(0.65f,0.95f,2.75f, 0, eBlend_AlphaAdditive);

	HOScene->GetObjectByID("orb")->Animate(0,2,0,0,4.0f, 1.75f);

	HOScene->GetObjectByID("glassdoorlight")->Animate(0.65f,0.95f,6.0f, 0, eBlend_AlphaAdditive);
	HOScene->GetObjectByID("lightray1")->Animate(0.35f,0.85f,4.0f, 0, eBlend_AlphaAdditive);
	HOScene->GetObjectByID("lightray2")->Animate(0.45f,0.95f,5.0f, 0, eBlend_AlphaAdditive);
	
	
	HOScene->GetObjectByID("hangroot1")->SetPivot(0, -268);
	HOScene->GetObjectByID("hangroot1")->Animate(0,2,1,0,5.0f);

	HOScene->GetObjectByID("hangroot2")->SetPivot(0, -268);
	HOScene->GetObjectByID("hangroot2")->Animate(0,2,1,0,4.0f, 1.75f);

	HOScene->GetObjectByID("treeleaf1")->SetPivot(17, 15);
	HOScene->GetObjectByID("treeleaf1")->Animate(0,1,3,0,4.0f, 1.75f);

	HOScene->GetObjectByID("treeleaf2")->SetPivot(38, -58);
	HOScene->GetObjectByID("treeleaf2")->Animate(0,1,2,0,6.0f, 1.75f);

	HOScene->GetObjectByID("treeleaf3")->SetPivot(20, 10);
	HOScene->GetObjectByID("treeleaf3")->Animate(0,1,3,0,5.0f, 1.75f);

	meshWater = NULL;
	meshWater = new MeshObject(HOScene->GetObjectByID("meshwater"), 24, 3);
	meshWater->meshFX->SetSpeedAndDisplacement(6, 2, 3, 3);
	meshWater->meshFX->setWaterwaves(true);

	S5leftcrystalsFX = new SuperFX("PRT\\L5\\S5leftcrystals.INI", PointSystem::CreateCPoint(223,590,0), 100); 
	S5leftcrystalsFX->Start();
	S5leftcrystalsFX->AdvanceByTime(2.0f);

	S5fliesFX = new SuperFX("PRT\\L5\\S5flies.INI", PointSystem::CreateCPoint(543,369,0), 100); 
	S5fliesFX->Start();
	S5fliesFX->AdvanceByTime(2.0f);

	S5rightcrystalFX = new SuperFX("PRT\\L5\\S5rightcrystal.INI", PointSystem::CreateCPoint(1221,253,0), 100); 
	S5rightcrystalFX->Start();
	S5rightcrystalFX->AdvanceByTime(2.0f);

	HOScene->PushObject(meshWater);

	InitPopups();

	SceneObjectsUpdate();

	s5hodotsFX = NULL;
	s5holinesFX = NULL;

	if(ProfileSystem::Instance->ActiveProfile.L5_S5_HOGanapathiActivated && !ProfileSystem::Instance->ActiveProfile.L5_S5_HOGanpathiCompleted)
	{
		if( !ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles )
		{
			s5hodotsFX = new SuperFX("PRT\\L5\\s5hodots.INI", PointSystem::CreateCPoint(904,210,0), 100); 
			s5hodotsFX->Start();
			s5hodotsFX->AdvanceByTime(2.0f);

			s5holinesFX = new SuperFX("PRT\\L5\\s5holines.INI", PointSystem::CreateCPoint(894,206,0), 100); 
			s5holinesFX->Start();
			s5holinesFX->AdvanceByTime(2.0f);
		}
	}

	TaskPendingUpdate();

#ifdef _CEBUILD
	//Morph Object Add
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S5Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("morphtarget"), HOScene->GetObjectByID("morphbase"));
		CRectangle  MorphRect(1044.0f,182.0f,55.0f,65.0f);
		Hud->setMorphObjRect(MorphRect);
	}
	else
	{
		HOScene->GetObjectByID("morphtarget")->SetEnable(false);
	}
#else
	HOScene->GetObjectByID("morphtarget")->SetEnable(false);
#endif 

		//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL5s5MoutainBelowAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL5s5MoutainBelowAmb) )		
	{
		Control::Audio->PlaySample(aAmbL5s5MoutainBelowAmb,true);
	}
	
	//MUSIC_SFX
	
	
	if(IsVisitedForFirstTime)
	{
		Hud->ShowBannerText("L5hear");
	}

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L5_S5_MountainBelow]  = true;
}

void L5_S5_MountainBelow::InitPopups()
{
	ActivePopupID = ePopInactive;

	for (int i = 0; i < ePopL5S5_Max; i++)
	{
		PopupArray[i] = NULL;
		PopStates[i] = ePopupState_Inactive;
		PopPolyArray[i] = NULL;
	}

	int popID=0;


	//Popup 1
	popID = ePop1_L5S5Pop1BambooLadder;
#ifdef _CEBUILD
	if (!ProfileSystem::Instance->ActiveProfile.L5_S5_InvBambooLadderTaken || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S5Flower])
#else
	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_InvBambooLadderTaken)
#endif 
	{
		int pop1Pts[] = {669,530,661,575,884,618,966,630,993,601,1004,556,937,519,852,533};
		CPolygon* pop1ClickPoly=  new CPolygon(8);
		pop1ClickPoly->Init(pop1Pts);

		Pop1Scene = new L5S5_Pop1BambooLadder(popID, Hud, HOScene, eZOrder_Popup, pop1ClickPoly->center, CC_CALLBACK_1(L5_S5_MountainBelow::OnPopupClosed, this));
		Pop1Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[popID] = pop1ClickPoly;
		PopupArray[popID]	= Pop1Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	//Popup 2
	popID = ePop2_L5S5Pop2MoonGlassDoor;
	//if(!ProfileSystem::Instance->ActiveProfile.L5_S1_InvArrowHolderTaken)
	{
		int pop2Pts[] = {619,46,700,49,748,105,712,158,589,154,563,91};
		CPolygon* pop2ClickPoly=  new CPolygon(6);
		pop2ClickPoly->Init(pop2Pts);

		Popup* Pop2Scene = new L5S5_Pop2MoonGlass(popID, Hud, HOScene, eZOrder_Popup, pop2ClickPoly->center, CC_CALLBACK_1(L5_S5_MountainBelow::OnPopupClosed, this));
		Pop2Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[popID] = pop2ClickPoly;
		PopupArray[popID]	= Pop2Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	//Popup 3
	popID = ePop3_L5S5Pop3TopExitDoor;
	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsTopDoorOpened)
	{
		int pop3Pts[] = {303,98,377,103,427,165,417,234,368,275,260,268,220,191,243,128};
		CPolygon* pop3ClickPoly=  new CPolygon(8);
		pop3ClickPoly->Init(pop3Pts);

		Popup* Pop3Scene = new L5S5_Pop3TopExitDoor(popID, Hud, HOScene, eZOrder_Popup, pop3ClickPoly->center, CC_CALLBACK_1(L5_S5_MountainBelow::OnPopupClosed, this));
		Pop3Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[popID] = pop3ClickPoly;
		PopupArray[popID]	= Pop3Scene;

		if(ProfileSystem::Instance->ActiveProfile.L5_S5_IsBambooLadderPlacedOnScene)
			PopStates[popID] = ePopupState_Closed;
	}

	//Popup 4
	popID = ePop4_L5S5Pop4Sack;
	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_HOGanapathiActivated)
	{
		int pop4Pts[] = {785,161,767,223,780,278,1014,287,1052,181,950,95};
		CPolygon* pop4ClickPoly=  new CPolygon(6);
		pop4ClickPoly->Init(pop4Pts);

		Popup* Pop4Scene = new L5S5_Pop4Sack(popID, Hud, HOScene, eZOrder_Popup, pop4ClickPoly->center, CC_CALLBACK_1(L5_S5_MountainBelow::OnPopupClosed, this));
		Pop4Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[popID] = pop4ClickPoly;
		PopupArray[popID]	= Pop4Scene;

		PopStates[popID] = ePopupState_Closed;
	}
}

void L5_S5_MountainBelow::OnPopupClosed(Popup* popup)
{
	ActivePopupID = ePopInactive;
	int popID = popup->GetPopupId();

	TaskPendingUpdate();

	if(popID == ePop1_L5S5Pop1BambooLadder)
	{
		#ifdef _CEBUILD
			if (ProfileSystem::Instance->ActiveProfile.L5_S5_InvBambooLadderTaken && ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S5Flower])
		#else
			if(ProfileSystem::Instance->ActiveProfile.L5_S5_InvBambooLadderTaken)
		#endif 
			{
				PopStates[popID] = ePopupState_Inactive; //disable popup
			}
	}
	else if(popID == ePop2_L5S5Pop2MoonGlassDoor)
	{
		if(ProfileSystem::Instance->ActiveProfile.L5_S5_IsMoonGlassDoorOpened)
		{
			//PopStates[popID] = ePopupState_Inactive; //disable popup
		}
	}
	else if(popID == ePop3_L5S5Pop3TopExitDoor)
	{
		if(ProfileSystem::Instance->ActiveProfile.L5_S5_IsTopDoorOpened)
		{
			PopStates[ePop3_L5S5Pop3TopExitDoor] = ePopupState_Inactive; //disable popup
		}
	}
	else if(popID == ePop4_L5S5Pop4Sack)
	{
		if(ProfileSystem::Instance->ActiveProfile.L5_S5_HOGanapathiActivated)
		{
			PopStates[popID] = ePopupState_Inactive; //disable popup

			Hud->ShowBannerText("L5scavenge");

			//activate HO particles
			if( !ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles )
			{
				s5hodotsFX = new SuperFX("PRT\\L5\\s5hodots.INI", PointSystem::CreateCPoint(904,210,0), 100); 
				s5hodotsFX->Start();
				s5hodotsFX->AdvanceByTime(2.0f);

				s5holinesFX = new SuperFX("PRT\\L5\\s5holines.INI", PointSystem::CreateCPoint(894,206,0), 100); 
				s5holinesFX->Start();
				s5holinesFX->AdvanceByTime(2.0f);
			}
		}
	}
}

void L5_S5_MountainBelow::SceneObjectsUpdate()
{
	if(ProfileSystem::Instance->ActiveProfile.L5_S5_InvBambooLadderTaken)
		HOScene->GetObjectByID("bambooladder")->SetEnable(false);
	
	if(ProfileSystem::Instance->ActiveProfile.L5_S5_IsP1PillerRemoved)
		HOScene->GetObjectByID("rockoverbamboo")->SetEnable(false);

	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsBambooLadderPlacedOnScene)
		HOScene->GetObjectByID("bambooladderfix")->SetEnable(false);

	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsSunClockPuzzleCompleted)
	{
		HOScene->GetObjectByID("sunpuzzleglow")->SetEnable(false);
		HOScene->GetObjectByID("sunpuzzleglow2")->SetEnable(false);
		HOScene->GetObjectByID("greenorbray")->SetEnable(false);
		HOScene->GetObjectByID("topdoorglowrings")->SetEnable(false);
		HOScene->GetObjectByID("orb")->SetEnable(false);
	}
	else
	{
		if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsTopDoorOpened)
			HOScene->GetObjectByID("topdoorglowrings")->SetEnable(true);
		else
			HOScene->GetObjectByID("topdoorglowrings")->SetEnable(false);
	}

	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsMoonGlassDoorOpened)
	{
		HOScene->GetObjectByID("glassdooropened")->SetEnable(false);
		HOScene->GetObjectByID("glassdoorlight")->SetEnable(false);
		HOScene->GetObjectByID("lightray1")->SetEnable(false);
		HOScene->GetObjectByID("lightray2")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("glassdooropened")->SetEnable(true);
		HOScene->GetObjectByID("glassdoorlight")->SetEnable(true);
		HOScene->GetObjectByID("lightray1")->SetEnable(true);
		HOScene->GetObjectByID("lightray2")->SetEnable(true);
	}

	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsTopDoorOpened)
	{
		HOScene->GetObjectByID("topdoorlight")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("topdoorlight")->SetEnable(true);
		HOScene->GetObjectByID("topdoormask")->SetEnable(false);
		HOScene->GetObjectByID("topdoorright")->SetEnable(false);
		HOScene->GetObjectByID("topdoorleft")->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L5_S5_HOGanapathiActivated)
		HOScene->GetObjectByID("hosack")->SetEnable(false);
		
	TaskPendingUpdate();
}

void L5_S5_MountainBelow::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_InvBambooLadderTaken)
		if(Pop1Scene->isAmeehaCutscnPlaying)
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

void L5_S5_MountainBelow::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	CPoint pos = Control::Input->Mpos();

	if( ActivePopupID == ePopInactive )
	{
		Cursor::SetMode(CA_Normal);
		if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
			SparkleUpdate();

		if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsBambooLadderPlacedOnScene)
		{
			if(InfoRectTopExitDoor.Intersection(&pos))
			{
				Cursor::SetMode(CA_Gear);
				if(Control::Input->LBclicked())
				{					
					Hud->ShowBannerText("L5reach");
				}
			}
		}
		else
		{
			if(ProfileSystem::Instance->ActiveProfile.L5_S5_IsTopDoorOpened)
			{
				if(SceneRectTopExitDoor.Intersection(&pos))
				{
					
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitUp, eArea_L5_S6_MountainTop);
#else
		Cursor::SetMode(CA_ExitUp, eArea_L5_S6_MountainTop);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L5_S6_MountainTop, 0, SceneRectTopExitDoor);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
						ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S6_MountainTop;
						SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
						
					}
				}
			}
		}

		//popup click checks
		for (int i = 0; i < ePopL5S5_Max; i++)
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

		if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsSunClockPuzzleCompleted)
		{
			if (PuzzleRectSunClock.Intersection(&pos))
			{
				if(ProfileSystem::Instance->ActiveProfile.L5_S5_IsMoonGlassDoorOpened)
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
				GFHud->InvBoxCtrl->GoToScene(eArea_L5_S5_PZSunClock, 0, PuzzleRectSunClock);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S5_PZSunClock;
						SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
						
					}
				}
				else
				{
					Cursor::SetMode(CA_Help);
					if( Control::Input->LBclicked() )
					{
						Hud->ShowBannerText("L5activate");
					}
				}
				return;
			}
		}

		if(ProfileSystem::Instance->ActiveProfile.L5_S5_HOGanapathiActivated && !ProfileSystem::Instance->ActiveProfile.L5_S5_HOGanpathiCompleted && HORectGanapathi.Intersection(&pos))
		{
			Cursor::SetMode(CA_Look);
			if( Control::Input->LBclicked() )
			{
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S5_HOGanapathi;
				SendMessage(ControlExit, ID, HOSM_NewArea);
			}
			return;
		}
		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S3_Pond;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}	
		#endif	

		if( IsMouseOverGoBackArea(&pos) )
		{
			
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown, eArea_L5_S3_Pond);
#else
		Cursor::SetMode(CA_ExitDown, eArea_L5_S3_Pond);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L5_S3_Pond, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S3_Pond;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
				
			}	
		}
	}
}

void L5_S5_MountainBelow::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	for (int i = 0; i < ePopL5S5_Max; i++)
	{
		if(PopStates[i] != ePopupState_Inactive) //if popup is active
		{
			if(PopupArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}

	if(ProfileSystem::Instance->ActiveProfile.L5_S5_HOGanapathiActivated && !ProfileSystem::Instance->ActiveProfile.L5_S5_HOGanpathiCompleted )
		isAnyTaskLeft = true;

	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsSunClockPuzzleCompleted && ProfileSystem::Instance->ActiveProfile.L5_S5_IsMoonGlassDoorOpened)
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S5_MountainBelow] = isAnyTaskLeft;
}

void L5_S5_MountainBelow::SparkleUpdate()
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
		for (int i = 0; i < ePopL5S5_Max; i++)
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

void L5_S5_MountainBelow::ReceiveMessage(int val)
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
				if (itemVal == eInv_L5_S5_BambooLadder && InvDestRectBamboo.Intersection(&dropPos))
				{
					ProfileSystem::Instance->ActiveProfile.L5_S5_IsBambooLadderPlacedOnScene = true;

					Control::Audio->QuickLoadAndPlaySFX("s5_bamboouse");

					HOScene->GetObjectByID("bambooladderfix")->SetEnable(true);

					//Activate Popup
					PopStates[ePop3_L5S5Pop3TopExitDoor] = ePopupState_Closed;

					Hud->ReleaseDrag();
					Hud->CloseInventory(); 

					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S5_BambooLadder] = false;
					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L5_S5_BambooLadder);
					return;
				}
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

void L5_S5_MountainBelow::ProcessSkip()
{
}

void L5_S5_MountainBelow::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S5Collected)
		{
			InitHintTrailEmit(1060, 210);
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S5Flower])
		{
			if (ActivePopupID != ePopInactive)
				InitHintTrailEmit(530, 200);
			else
				InitHintTrailEmit(810, 560);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}
	int iNonHOHint = 0; 
	int MaxNonHOHints = 8;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			++iNonHOHint;
			break;

		case 0: ++iNonHOHint;
			if( ActivePopupID != ePopInactive )
			{
				PopupArray[ActivePopupID]->ProcessHint();
				return;
			}
			break;
		case 1: ++iNonHOHint;
			for (int i = 0; i < ePopL5S5_Max; i++)
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
			if( ProfileSystem::Instance->ActiveProfile.L5_S5_InvBambooLadderTaken && !ProfileSystem::Instance->ActiveProfile.L5_S5_IsBambooLadderPlacedOnScene)
			{
				Hud->ShowHintForInvItem(eInv_L5_S5_BambooLadder, &InvDestRectBamboo);
				return;
			}
			break;
		case 3: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L5_S5_HOGanapathiActivated && !ProfileSystem::Instance->ActiveProfile.L5_S5_HOGanpathiCompleted)
			{
				InitHintTrailEmit(&HORectGanapathi);
				return;
			}
			break;
		case 4: ++iNonHOHint;

			if (!ProfileSystem::Instance->ActiveProfile.L5_S5_IsTopDoorOpened && ProfileSystem::Instance->ActiveProfile.L5_S2_InvBadgeTaken && ProfileSystem::Instance->ActiveProfile.L5_S5_IsSunClockPuzzleCompleted)
			{
				InitHintTrailEmit(&SceneRectTopExitDoor);
				return;
			}
			else if( (ProfileSystem::Instance->ActiveProfile.L5_S5_IsTopDoorOpened && !ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L5_S6_MountainTop]) || ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S6_MountainTop])
			{
				InitHintTrailEmit(&SceneRectTopExitDoor,true,CA_ExitUp);
				return;
			}
			else if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsSunClockPuzzleCompleted && ProfileSystem::Instance->ActiveProfile.L5_S5_IsMoonGlassDoorOpened && ProfileSystem::Instance->ActiveProfile.L5_S3_InvMoonSolutionTaken)
			{
				InitHintTrailEmit(&PuzzleRectSunClock);
				return;
			}
			break;

		case 5: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S3_Pond])
			{
				InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
				return;
			}
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

L5_S5_MountainBelow::~L5_S5_MountainBelow()
{
	SAFE_DELETE(s5hodotsFX);
	SAFE_DELETE(s5holinesFX);

	SAFE_DELETE(S5leftcrystalsFX);
	SAFE_DELETE(S5fliesFX);
	SAFE_DELETE(S5rightcrystalFX);

	for (int i = 0; i < ePopL5S5_Max; i++)
	{
		SAFE_DELETE(PopupArray[i]);
		SAFE_DELETE(PopPolyArray[i]);
	}

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL5s5MoutainBelowAmb);
	Control::Audio->UnloadSample(aAmbL5s5MoutainBelowAmb);
	//MUSIC_SFX
}
void L5_S5_MountainBelow::NavigateToScene()
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