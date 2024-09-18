//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L7_S2_LasyaPrison.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"

const CRectangle  HORectRightHand(962.0f,190.0f,286.0f,354.0f);
const CRectangle  HORectLeftHand(125.0f,175.0f,263.0f,382.0f);
const CRectangle  HORectBrokenHandRight(898.0f,399.0f,302.0f,183.0f);
const CRectangle  HORectBrokenHandLeft(170.0f,384.0f,312.0f,198.0f);
const CRectangle  PuzzleRectEye(554.0f,109.0f,254.0f,202.0f);
const CRectangle  PuzzleRectLasyaStatue(598.0f,303.0f,156.0f,197.0f);

const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);

const CRectangle  LasyaStatueMask(591.0f,268.0f,156.0f,239.0f);

// const float GlobeDownDepth = 100.0f;  // Sac: Commenting Unused variable

L7_S2_LasyaPrison::L7_S2_LasyaPrison()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV7\\S2\\BG\\S2.SCN");

	hypnoticCircle = HOScene->GetObjectByID("hypnosis");
	hypnoticCircle->SetScale(2.0f);
	hypnoticCircle->SetAlpha(0.30f);
	hypnoticCircle->SetBlendMode(eBlend_AlphaAdditive);

	HOScene->GetObjectByID("lefthandrays")->Animate(0.65f,0.95f,4.0f,0,eBlend_AlphaAdditive);
	HOScene->GetObjectByID("righthandrays")->Animate(0.65f,0.95f,4.0f,4.5f,eBlend_AlphaAdditive);

	HOScene->GetObjectByID("eyeglow")->Animate(0,1.25f,0,0,3.5f);

	s2bgstarsFX = new SuperFX("PRT\\L7\\s2bgstars.INI", PointSystem::CreateCPoint(678,354,0), 1); 
	s2bgstarsFX->Start();
	s2bgstarsFX->AdvanceByTime(2.0f);

	s2headglowFX = NULL;
	/*s2headglowFX = new SuperFX("PRT\\L7\\s2headglow.INI", PointSystem::CreateCPoint(681,183,0), 1); 
	s2headglowFX->Start();
	s2headglowFX->AdvanceByTime(2.0f);*/

	s2lasyaredraysFX = new SuperFX("PRT\\L7\\s2lasyaredrays.INI", PointSystem::CreateCPoint(672,381,0), 100); 
	s2lasyareddotsFX = new SuperFX("PRT\\L7\\s2lasyareddots.INI", PointSystem::CreateCPoint(681,427,0), 100); 	

	s2statuedustFX = new SuperFX("PRT\\L7\\s2statuedust.INI", PointSystem::CreateCPoint(685,466,0), 100); 

	s2smokefgFX = new SuperFX("PRT\\L7\\s2smokefg.INI", PointSystem::CreateCPoint(760,667,0), 100); 
	s2smokefgFX->Start();
	s2smokefgFX->AdvanceByTime(2.0f);

	s2lefthandFX = new SuperFX("PRT\\L7\\s2sphereglobe.INI", PointSystem::CreateCPoint(271,298,0), 100); 
	s2lefthandFX->Start();
	s2lefthandFX->AdvanceByTime(2.0f);

	s2sphereglobeFX = new SuperFX("PRT\\L7\\s2lefthand.INI", PointSystem::CreateCPoint(683,384,0), 100);


	s2righthandFX = NULL;
	if(!ProfileSystem::Instance->ActiveProfile.L7_S2_IsRightHandExplodeShown)
	{
		s2righthandFX = new SuperFX("PRT\\L7\\s2righthand.INI", PointSystem::CreateCPoint(1077,286,0), 100); 
		s2righthandFX->Start();
		s2righthandFX->AdvanceByTime(2.0f);
	}

	HOScene->GetObjectByID("Statue")->SetMaskRect(LasyaStatueMask);

	if(ProfileSystem::Instance->ActiveProfile.L7_S2_PopSphereFullPlaced)
		ProfileSystem::Instance->ActiveProfile.L7_S2_LasyaStatueRisenUp = true; //Force set. If user exited when globe is going down

	//handle Explosions
	s2handexplodeFX = NULL;
	s2handexplodesmokeFX = NULL;
	
	handExplodeTimer = 0;
	isExplodeLightShining = false;

	explodeHandLight = NULL;
	s2handexplodesmokeFX = NULL;

	isRightHandExploding = false;
	isLeftHandExploding = false;

	if(ProfileSystem::Instance->ActiveProfile.L7_S2_HOLeftHandCompleted && !ProfileSystem::Instance->ActiveProfile.L7_S2_IsLeftHandExplodeShown)
	{
		isLeftHandExploding = true;
		isExplodeLightShining = true;
		explodeHandLight = new CObject();
		explodeHandLight->LoadInit("LV7\\S2\\BG\\explodeglowL.SPR",302,280,20);
		explodeHandLight->SetAlpha(0.5f);

		Control::Audio->QuickLoadAndPlaySFX("s2_handbreak");

		HOScene->Shake(3,1, 2.5f);

		s2handexplodeFX = new SuperFX("PRT\\L7\\s2lefthandexplode.INI", PointSystem::CreateCPoint(255,295,0), 100);
		s2handexplodesmokeFX = new SuperFX("PRT\\L7\\s2handexplodesmoke.INI", PointSystem::CreateCPoint(252,318,0), 100); //LEFT HAND
	}

	if(ProfileSystem::Instance->ActiveProfile.L7_S2_HORightHandCompleted && !ProfileSystem::Instance->ActiveProfile.L7_S2_IsRightHandExplodeShown)
	{
		isRightHandExploding = true;
		isExplodeLightShining = true;
		explodeHandLight = new CObject();
		explodeHandLight->LoadInit("LV7\\S2\\BG\\explodeglowR.SPR",1051,275,20);
		explodeHandLight->SetAlpha(0);

		Control::Audio->QuickLoadAndPlaySFX("s2_handbreak");

		HOScene->Shake(3,1, 2.5f);

		s2handexplodeFX = new SuperFX("PRT\\L7\\s2lefthandexplode.INI", PointSystem::CreateCPoint(1110,300,0), 100);
		s2handexplodesmokeFX = new SuperFX("PRT\\L7\\s2handexplodesmoke.INI", PointSystem::CreateCPoint(1139,329,0), 120); //RIGHT HAND
	}

	//Lasya Rising and Globe going down
	lasyaRiseTimer = 0;
	isStatueRising = false;
	isGlobeGoingDown = false;

	globeStand = NULL;
	globe = NULL;
	lasyaStatue = NULL;

	InitPopups();

	SceneObjectsUpdate();

	TaskPendingUpdate();

	//Journal Entry Add
	if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L7P1_EnterLasyaPrison])
	{
		//Flag Set
		ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L7P1_EnterLasyaPrison] = true;

		//Add to Note Array
		ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L7P1_EnterLasyaPrison;

		ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

		//Increment Notes
		ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
		Hud->ShowJrnEntryFX();
	}

#ifdef _CEBUILD
	//Morph Object Add
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S2Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("morphtarget"), HOScene->GetObjectByID("morphbase"));
		CRectangle  MorphRect(654.0f,523.0f,50.0f,52.0f);
		Hud->setMorphObjRect(MorphRect);
	}
	else
	{
		HOScene->GetObjectByID("morphtarget")->SetEnable(false);
	}
#else
	HOScene->GetObjectByID("morphtarget")->SetEnable(false);
#endif // _CEBUILD

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL7s2LasyaPrisonAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL7s2LasyaPrisonAmb) )		
	{
		Control::Audio->PlaySample(aAmbL7s2LasyaPrisonAmb,true);
	}
	
	//MUSIC_SFX

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L7_S2_LasyaPrison]  = true;
	Control::Audio->LoadSample(aVOL7MaryRefS2_1,	AudioVO);
	if(IsVisitedForFirstTime)
	{
		Hud->ShowBannerText("L7cyclops");
		Control::Audio->PlaySample(aVOL7MaryRefS2_1);

		//Lock Volcano
		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L7_S1_Volcano] = true;
	}

	//Objective Add
	if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L7O1_FindWayOutOfLasyaPrison] )
	{
		ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L7O1_FindWayOutOfLasyaPrison] = true;
		ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
		= eObj_L7O1_FindWayOutOfLasyaPrison;
		//Increment Objectives
		ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
		Hud->ShowObjectiveFX(eObj_L7O1_FindWayOutOfLasyaPrison);

		Control::Audio->PlaySample(aSFXMagic1);
	}

	
}

void L7_S2_LasyaPrison::InitPopups()
{
	ActivePopupID = ePopInactive;

	for (int i = 0; i < ePopL7S2_Max; i++)
	{
		PopupArray[i] = NULL;
		PopStates[i] = ePopupState_Inactive;
		PopPolyArray[i] = NULL;
	}


	//Popup 2
	int popID = ePop1_L7S2Pop1Sphere;
	if(!ProfileSystem::Instance->ActiveProfile.L7_S2_PopSphereFullPlaced)
	{
		int pts1[] = {733,350,605,353,570,484,671,528,785,480};
		CPolygon* clickpoly1=  new CPolygon(5);
		clickpoly1->Init(pts1);

		Popup* Pop1Scene = new L7S2_Pop1SphereGlobe(popID, Hud, HOScene, eZOrder_Popup, clickpoly1->center, CC_CALLBACK_1(L7_S2_LasyaPrison::OnPopupClosed, this));
		Pop1Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[popID] = clickpoly1;
		PopupArray[popID]	= Pop1Scene;

		PopStates[popID] = ePopupState_Closed;
	}
}

void L7_S2_LasyaPrison::OnPopupClosed(Popup* popup)
{
	ActivePopupID = ePopInactive;
	int popID = popup->GetPopupId();

	if(popID == ePop1_L7S2Pop1Sphere)
	{
		if(ProfileSystem::Instance->ActiveProfile.L7_S2_PopSphereFullPlaced)
		{
			PopStates[popID] = ePopupState_Inactive;

			//Start Lasya Statue rising Animation
			s2sphereglobeFX->Start();
			s2sphereglobeFX->AdvanceByTime(1.25f);

			Control::Audio->QuickLoadAndPlaySFX("s2_statuecomingup");

			isGlobeGoingDown = true;

			globeStand = HOScene->GetObjectByID("globestand");
			globe = HOScene->GetObjectByID("greenglobe");
			lasyaStatue = HOScene->GetObjectByID("Statue");
			
			globe->TweenPosTo(0, 95.0f, 0, 2.5f, TW_EASE, true, CC_CALLBACK_1(L7_S2_LasyaPrison::OnTweenEndCallback, this));
			globeStand->TweenPosTo(0, 95.0f, 0, 2.5f, TW_EASE, true);

			HOScene->RemoveObject(globe);
			HOScene->RemoveObject(globeStand);
			HOScene->RemoveObject(lasyaStatue);

			HOScene->Shake(2, 1, 2.5f);
		}
	}
}

void L7_S2_LasyaPrison::SceneObjectsUpdate()
{
	if(ProfileSystem::Instance->ActiveProfile.L7_S2_PZEyePuzzleCompleted)
	{
		hypnoticCircle->SetEnable(false);

		HOScene->GetObjectByID("eye")->SetEnable(false);
		HOScene->GetObjectByID("eyeglow")->SetEnable(false);
	}

	if(!ProfileSystem::Instance->ActiveProfile.L7_S2_HOLeftHandCompleted)
	{
		HOScene->GetObjectByID("leftboxclosed")->SetEnable(false);
		HOScene->GetObjectByID("leftboxopen")->SetEnable(false);
		HOScene->GetObjectByID("spherepartleft")->SetEnable(false);
		HOScene->GetObjectByID("leftbrokenparts")->SetEnable(false);
	}
	else
	{
		if(ProfileSystem::Instance->ActiveProfile.L7_S2_IsLeftHandExplodeShown)
		{
			HOScene->GetObjectByID("lefthand")->SetEnable(false);
			HOScene->GetObjectByID("lefthandrays")->SetEnable(false);

			if(ProfileSystem::Instance->ActiveProfile.L7_S2_HOBrokenLeftHandCompleted)
			{
				HOScene->GetObjectByID("leftboxclosed")->SetEnable(false);
				HOScene->GetObjectByID("leftboxopen")->SetEnable(true);	
				if(ProfileSystem::Instance->ActiveProfile.L7_S2_InvSphereHalfLeftToFixTaken)
					HOScene->GetObjectByID("spherepartleft")->SetEnable(false);
			}
			else
			{
				HOScene->GetObjectByID("leftboxopen")->SetEnable(false);
			}
		}
		else
		{
			HOScene->GetObjectByID("leftboxclosed")->SetEnable(false);
			HOScene->GetObjectByID("spherepartleft")->SetEnable(false);
			HOScene->GetObjectByID("leftbrokenparts")->SetEnable(false);
			HOScene->GetObjectByID("leftboxopen")->SetEnable(false);
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L7_S2_HORightHandCompleted)
	{
		HOScene->GetObjectByID("rightboxclosed")->SetEnable(false);
		HOScene->GetObjectByID("spherepartright")->SetEnable(false);
		HOScene->GetObjectByID("rightbrokenparts")->SetEnable(false);
		HOScene->GetObjectByID("rightboxopen")->SetEnable(false);

	}
	else
	{
		if(ProfileSystem::Instance->ActiveProfile.L7_S2_IsRightHandExplodeShown)
		{
			HOScene->GetObjectByID("righthandrays")->SetEnable(false);
			HOScene->GetObjectByID("righthand")->SetEnable(false);

			
			if(ProfileSystem::Instance->ActiveProfile.L7_S2_HOBrokenRightHandCompleted)
			{
				HOScene->GetObjectByID("rightboxclosed")->SetEnable(false);
				HOScene->GetObjectByID("rightboxopen")->SetEnable(true);	
				if(ProfileSystem::Instance->ActiveProfile.L7_S2_InvSphereHalfRightBaseTaken)
					HOScene->GetObjectByID("spherepartright")->SetEnable(false);
			}
			else
			{
				HOScene->GetObjectByID("rightboxopen")->SetEnable(false);
			}
		}
		else
		{
			HOScene->GetObjectByID("rightboxclosed")->SetEnable(false);
			HOScene->GetObjectByID("spherepartright")->SetEnable(false);
			HOScene->GetObjectByID("rightbrokenparts")->SetEnable(false);
			HOScene->GetObjectByID("rightboxopen")->SetEnable(false);
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L7_S2_PopSphereFullPlaced)
	{
		HOScene->GetObjectByID("greenglobe")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("greenglobe")->SetEnable(true);
	}

	if(!ProfileSystem::Instance->ActiveProfile.L7_S2_LasyaStatueRisenUp)
	{
		HOScene->GetObjectByID("Statue")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("greenglobe")->SetEnable(false);
		HOScene->GetObjectByID("globestand")->SetEnable(false);
	}

	TaskPendingUpdate();
}

void L7_S2_LasyaPrison::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if(isRightHandExploding)
	{
		handExplodeTimer += 0.45f*Control::LogicRate;
		if(isExplodeLightShining)
		{
			HOScene->update(Control::LogicRate);
			explodeHandLight->SetAlpha(0.85f+Elixir::Sin(handExplodeTimer*360.0f)*0.5f);

			if(handExplodeTimer>1.25f)
			{
				ProfileSystem::Instance->ActiveProfile.L7_S2_IsRightHandExplodeShown = true;

				isExplodeLightShining = false;

				s2handexplodeFX->Start();
				s2handexplodeFX->AdvanceByTime(0.15f);

				s2handexplodesmokeFX->Start();
				s2handexplodesmokeFX->AdvanceByTime(0.15f);

				handExplodeTimer = 0;

				s2righthandFX->StopEmit();

				explodeHandLight->FadeOut(3.0f);
				HOScene->GetObjectByID("righthandrays")->FadeOut(2.0f);
				HOScene->GetObjectByID("righthand")->FadeOut(2.0f);

				HOScene->GetObjectByID("rightboxclosed")->SetEnable(true);
				HOScene->GetObjectByID("spherepartright")->SetEnable(true);
				HOScene->GetObjectByID("rightbrokenparts")->SetEnable(true);

				HOScene->GetObjectByID("rightboxclosed")->FadeIn(3.0f);
				HOScene->GetObjectByID("spherepartright")->FadeIn(2.0f);
				HOScene->GetObjectByID("rightbrokenparts")->FadeIn(3.0f);

				HOScene->Shake(3,5, 1.5f);
			}
		}
		else
		{
			if(handExplodeTimer>1.35f)
				isRightHandExploding = false;
		}
	}
	else if(isLeftHandExploding)
	{
		handExplodeTimer += 0.45f*Control::LogicRate;
		if(isExplodeLightShining)
		{
			HOScene->update(Control::LogicRate);
			explodeHandLight->SetAlpha(0.85f+Elixir::Sin(handExplodeTimer*360.0f)*0.5f);

			if(handExplodeTimer>1.25f)
			{
				ProfileSystem::Instance->ActiveProfile.L7_S2_IsLeftHandExplodeShown = true;

				isExplodeLightShining = false;

				s2handexplodeFX->Start();
				s2handexplodeFX->AdvanceByTime(0.15f);

				s2handexplodesmokeFX->Start();
				s2handexplodesmokeFX->AdvanceByTime(0.15f);

				handExplodeTimer = 0;

				s2lefthandFX->StopEmit();

				explodeHandLight->FadeOut(3.0f);

				HOScene->GetObjectByID("lefthandrays")->FadeOut(2.0f);
				HOScene->GetObjectByID("lefthand")->FadeOut(2.0f);

				HOScene->GetObjectByID("leftboxclosed")->SetEnable(true);
				HOScene->GetObjectByID("spherepartleft")->SetEnable(true);
				HOScene->GetObjectByID("leftbrokenparts")->SetEnable(true);

				HOScene->GetObjectByID("leftboxclosed")->FadeIn(3.0f);
				HOScene->GetObjectByID("spherepartleft")->FadeIn(2.0f);
				HOScene->GetObjectByID("leftbrokenparts")->FadeIn(3.0f);

				HOScene->Shake(3,5, 1.5f);
			}
		}
		else
		{
			if(handExplodeTimer>1.35f)
				isLeftHandExploding = false;
		}
	}

	if( Control::Audio->IsPlaying(aTrackL7ho) )
		{
			Control::Audio->Samples[aTrackL7ho]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL7ho]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL7ho);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL7theme) )
					  Control::Audio->PlaySample(aTrackL7theme, true);
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
	else if( Control::Audio->IsPlaying(aTrackL7puzzle) )
		{
			Control::Audio->Samples[aTrackL7puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL7puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL7puzzle);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL7theme) )
					  Control::Audio->PlaySample(aTrackL7theme, true);
					  
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
		else
		{
			if( !Control::Audio->IsPlaying(aTrackL7theme))
			{
				Control::Audio->TurnOffLvlMscTracks();
				Control::Audio->PlaySample(aTrackL7theme, true);
			}
		}
}

void L7_S2_LasyaPrison::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	CPoint pos = Control::Input->Mpos();

	#ifdef _PARTICLEEDITORENABLED
		if (_isParticleEditorActive)
		{
			return;
		}
	#endif

	if(hypnoticCircle->Visible)
		hypnoticCircle->SetRelativeRotate(Control::LogicRate*90.0f);

	if(isRightHandExploding)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	if(isLeftHandExploding)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	if( ActivePopupID == ePopInactive )
	{
		if(isStatueRising || isGlobeGoingDown)
		{
			Cursor::SetMode(CA_Normal);
			HOScene->update(Control::LogicRate);
			return;
		}

		Cursor::SetMode(CA_Normal);
		if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
			SparkleUpdate();

		//popup click checks
		for (int i = 0; i < ePopL7S2_Max; i++)
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

		if( !ProfileSystem::Instance->ActiveProfile.L7_S2_InvStatueTaken && PuzzleRectEye.Intersection(&pos))
		{
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_Look);
#else
		Cursor::SetMode(CA_Look);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S2_PZEyePuzzle, 0, PuzzleRectEye);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S2_PZEyePuzzle;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
				
			}	
		}

		//RIGHT SIDE HOS
		if( !ProfileSystem::Instance->ActiveProfile.L7_S2_HORightHandCompleted && HORectRightHand.Intersection(&pos))
		{
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_Look);
#else
		Cursor::SetMode(CA_Look);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S2_HORightHand, 0, HORectRightHand);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S2_HORightHand;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
				
			}	
		}
		else if( ProfileSystem::Instance->ActiveProfile.L7_S2_HORightHandCompleted && !ProfileSystem::Instance->ActiveProfile.L7_S2_InvSphereHalfRightBaseTaken && HORectBrokenHandRight.Intersection(&pos))
		{
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_Look);
#else
		Cursor::SetMode(CA_Look);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S2_HOBrokenRightHand, 0, HORectBrokenHandRight);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S2_HOBrokenRightHand;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
				
			}	
		}

		//LEFT SIDE HOS
		if( !ProfileSystem::Instance->ActiveProfile.L7_S2_HOLeftHandCompleted && HORectLeftHand.Intersection(&pos))
		{
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_Look);
#else
		Cursor::SetMode(CA_Look);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S2_HOLeftHand, 0, HORectLeftHand);
			}
#else			
			
				//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S2_HOLeftHand;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
			}	
		}
		else if( ProfileSystem::Instance->ActiveProfile.L7_S2_HOLeftHandCompleted && !ProfileSystem::Instance->ActiveProfile.L7_S2_InvSphereHalfLeftToFixTaken && HORectBrokenHandLeft.Intersection(&pos))
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
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S2_HOBrokenLeftHand, 0, HORectBrokenHandLeft);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S2_HOBrokenLeftHand;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
				
			}	
		}

		if(ProfileSystem::Instance->ActiveProfile.L7_S2_LasyaStatueRisenUp && PuzzleRectLasyaStatue.Intersection(&pos))
		{
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_Look);
#else
		Cursor::SetMode(CA_Look);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S2_PZLasyaStatue, 0, PuzzleRectLasyaStatue);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S2_PZLasyaStatue;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
				
			}
		}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
		// Sac: Back button Not handled gracefully, 
		//      need to check the game play flow and then decide what to show when Back button is pressed 
		Hud->ShowBannerText("L7trap");					
	}	
	#endif	
		if( IsMouseOverGoBackArea(&pos) )
		{
			Cursor::SetMode(CA_ExitDown,-1);
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L7trap");
				//Control::Audio->PlayFootStepsFX();
				//ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S3_CaveMouth;
				//SendMessage(ControlExit, ID, HOSM_NewArea);						
			}	
		}
	}
}

void L7_S2_LasyaPrison::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	for (int i = 0; i < ePopL7S2_Max; i++)
	{
		if(PopStates[i] != ePopupState_Inactive) //if popup is active
		{
			if(PopupArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S2_LasyaPrison] = isAnyTaskLeft;
}

void L7_S2_LasyaPrison::SparkleUpdate()
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
		for (int i = 0; i < ePopL7S2_Max; i++)
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

		//Scene Tasks
		if( !ProfileSystem::Instance->ActiveProfile.L7_S2_InvStatueTaken )
		{
			sparklePoints[numSparkles].SetXY(PuzzleRectEye.GetCenterX(),PuzzleRectEye.GetCenterY());
			numSparkles++;
		}

		//RIGHT SIDE HOS
		if( !ProfileSystem::Instance->ActiveProfile.L7_S2_HORightHandCompleted && ProfileSystem::Instance->ActiveProfile.L7_S2_InvSymbolDiscTaken)
		{
			sparklePoints[numSparkles].SetXY(HORectRightHand.GetCenterX(),HORectRightHand.GetCenterY());
			numSparkles++;
		}
		else if( ProfileSystem::Instance->ActiveProfile.L7_S2_HORightHandCompleted && !ProfileSystem::Instance->ActiveProfile.L7_S2_InvSphereHalfRightBaseTaken )
		{
			sparklePoints[numSparkles].SetXY(HORectBrokenHandRight.GetCenterX(),HORectBrokenHandRight.GetCenterY());
			numSparkles++;
		}

		//LEFT SIDE HOS
		if( !ProfileSystem::Instance->ActiveProfile.L7_S2_HOLeftHandCompleted && ProfileSystem::Instance->ActiveProfile.L7_S2_InvSymbolDiscTaken )
		{
			sparklePoints[numSparkles].SetXY(HORectLeftHand.GetCenterX(),HORectLeftHand.GetCenterY());
			numSparkles++;
		}
		else if( ProfileSystem::Instance->ActiveProfile.L7_S2_HOLeftHandCompleted && !ProfileSystem::Instance->ActiveProfile.L7_S2_InvSphereHalfLeftToFixTaken )
		{
			sparklePoints[numSparkles].SetXY(HORectBrokenHandLeft.GetCenterX(),HORectBrokenHandLeft.GetCenterY());
			numSparkles++;	
		}

		if(ProfileSystem::Instance->ActiveProfile.L7_S2_LasyaStatueRisenUp)
		{
			sparklePoints[numSparkles].SetXY(PuzzleRectLasyaStatue.GetCenterX(),PuzzleRectLasyaStatue.GetCenterY());
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

void L7_S2_LasyaPrison::OnTweenEndCallback(CObject* obj)
{
	if(isStatueRising)
	{
		s2statuedustFX->Start();
		s2statuedustFX->AdvanceByTime(0.1f);
		isStatueRising = false;

		s2lasyaredraysFX->StopEmit();
		s2lasyareddotsFX->StopEmit();
		return;
	}

	if(isGlobeGoingDown)
	{
		s2sphereglobeFX->StopEmit();

		isGlobeGoingDown = false;

		isStatueRising = true;
		lasyaStatue->Pos.y += 150.0f;
		lasyaStatue->SetEnable(true);
		lasyaStatue->TweenPosTo(0, -150.0f, 0, 2.5f, TW_EASE, true, CC_CALLBACK_1(L7_S2_LasyaPrison::OnTweenEndCallback, this));

		s2lasyaredraysFX->Start();
		s2lasyareddotsFX->Start();

		HOScene->Shake(4, 2, 2.45f);

		ProfileSystem::Instance->ActiveProfile.L7_S2_LasyaStatueRisenUp = true;
	}
}

void L7_S2_LasyaPrison::ReceiveMessage(int val)
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

		if(ProfileSystem::Instance->ActiveProfile.L7_S2_InvSphereFullTaken)
			isSparkleUpdatePending = true;
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

void L7_S2_LasyaPrison::ProcessSkip()
{
}

void L7_S2_LasyaPrison::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S2Collected)
		{
			InitHintTrailEmit(680, 550);
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

		case 0: ++iNonHOHint;
			if( ActivePopupID != ePopInactive )
			{
				PopupArray[ActivePopupID]->ProcessHint();
				return;
			}
			break;
		case 1: ++iNonHOHint;
			for (int i = 0; i < ePopL7S2_Max; i++)
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
			if( !ProfileSystem::Instance->ActiveProfile.L7_S2_InvStatueTaken && !ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L7_S2_LasyaStatue])
			{
				InitHintTrailEmit(&PuzzleRectEye,false);
				return;
			}
			break;
		case 3: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L7_S2_HOLeftHandCompleted )
			{
				InitHintTrailEmit(&HORectLeftHand,false);
				return;
			}
			break;
		case 4: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L7_S2_HORightHandCompleted)
			{
				InitHintTrailEmit(&HORectRightHand,false);
				return;
			}
			break;
		case 5: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L7_S2_HOLeftHandCompleted && !ProfileSystem::Instance->ActiveProfile.L7_S2_InvSphereHalfLeftToFixTaken )
			{
				InitHintTrailEmit(&HORectBrokenHandLeft,false);
				return;
			}
			break;

		case 6: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L7_S2_HORightHandCompleted && !ProfileSystem::Instance->ActiveProfile.L7_S2_InvSphereHalfRightBaseTaken )
			{
				InitHintTrailEmit(&HORectBrokenHandRight,false);
				return;
			}
			break;

		case 7: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L7_S2_LasyaStatueRisenUp )
			{
				InitHintTrailEmit(&PuzzleRectLasyaStatue,false);
				return;
			}
			break;
		case 8: ++iNonHOHint;
			{
				if(Hud->ShowHintForComboInvItems())
					return;
			}
			break;
		case 9: ++iNonHOHint;
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

L7_S2_LasyaPrison::~L7_S2_LasyaPrison()
{
	//To do the shaking
	SAFE_DELETE(globeStand);
	SAFE_DELETE(globe);
	SAFE_DELETE(lasyaStatue);

	SAFE_DELETE(s2lasyaredraysFX);
	SAFE_DELETE(s2lasyareddotsFX);

	SAFE_DELETE(s2statuedustFX);
	SAFE_DELETE(explodeHandLight);
	SAFE_DELETE(s2handexplodeFX);
	SAFE_DELETE(s2handexplodesmokeFX);

	SAFE_DELETE(s2bgstarsFX);
	SAFE_DELETE(s2headglowFX);
	SAFE_DELETE(s2smokefgFX);
	SAFE_DELETE(s2sphereglobeFX);
	SAFE_DELETE(s2lefthandFX);
	SAFE_DELETE(s2righthandFX);

	for (int i = 0; i < ePopL7S2_Max; i++)
	{
		SAFE_DELETE(PopupArray[i]);
		SAFE_DELETE(PopPolyArray[i]);
	}

	//MUSIC_SFX
	Control::Audio->UnloadSample(aVOL7MaryRefS2_1);
	Control::Audio->StopSample(aAmbL7s2LasyaPrisonAmb);
	Control::Audio->UnloadSample(aAmbL7s2LasyaPrisonAmb);
	//MUSIC_SFX
}
void L7_S2_LasyaPrison::NavigateToScene()
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