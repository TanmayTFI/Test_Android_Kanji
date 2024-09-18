//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 04 : CAVE
//====================================

#include "L2_S4_Cave.h"
#include "Banners.h"
#include "Hud.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"

const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);

const CRectangle  ScnRectTunnelEnd(940.0f,132.0f,250.0f,280.0f);
const CRectangle  ScnRectMooppansDen(192.0f,77.0f,207.0f,329.0f);

const CRectangle  ScnRectPuzzleDieties(656.0f,433.0f,133.0f,97.0f);

const CRectangle  InfoRectDietyStatue(598.0f,17.0f,233.0f,259.0f);


L2_S4_Cave::L2_S4_Cave()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV2\\S4\\S4.SCN");
	musicFadeOutTimer = 0;
	//HOScene->GetObjectByID("fireanim2")->SetRotate(-15);

	isPuzzleDietiesActive = false;
	if(ProfileSystem::Instance->ActiveProfile.L2_S4_RockSlabRemoved && ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L2_S4_PZDeities] && !ProfileSystem::Instance->ActiveProfile.L2_S4_InvMetalLeverTaken)
		isPuzzleDietiesActive = true;

	HOScene->GetObjectByID("fireglowleft")->Animate(0.15f,0.45f,3.0f, eBlend_AlphaAdditive);

	HOScene->GetObjectByID("fireglow")->Animate(0.15f,0.75f,3.0f, 1.34f,eBlend_AlphaAdditive);

	HOScene->GetObjectByID("statueglow")->Animate(0.25f,0.90f, 5.0f, 1.34f,eBlend_AlphaAdditive);

	HOScene->GetObjectByID("hangrootleft")->Animate(0,4,0,0,8.0f);
	HOScene->GetObjectByID("hangrootright")->Animate(0,4,0,0,8.0f, 3.14f);

	HOScene->GetObjectByID("leftplant")->SetPivot(-10, 43);
	HOScene->GetObjectByID("leftplant")->Animate(0,0,4,0,8.0f);
	
	HOScene->GetObjectByID("plantleft")->SetPivot(-34, 78);
	HOScene->GetObjectByID("plantleft")->Animate(0,0,4,0,7.0f, 3.14f);

	HOScene->GetObjectByID("plantleftsmall")->SetPivot(-22, 47);
	HOScene->GetObjectByID("plantleftsmall")->Animate(0,0,3,0,6.0f);

	meshWater = NULL;
	meshWater = new MeshObject(HOScene->GetObjectByID("watermeshref"), 5, 5);
	meshWater->meshFX->SetSpeedAndDisplacement(4, 2, 4, 2);

	SuperFX *s4firesparksFX, *s4firesparksRightFX, *s4denfliesFX, *s4tunnelfliesFX, *S4smokeFX;

	s4firesparksFX = new SuperFX("PRT\\L2\\s4firesparks.INI", PointSystem::CreateCPoint(470,363,0), 50); 
	s4firesparksFX->Start();
	s4firesparksFX->AdvanceByTime(2.0f);

	s4firesparksRightFX = new SuperFX("PRT\\L2\\s4firesparks.INI", PointSystem::CreateCPoint(918,348,0), 50); 
	s4firesparksRightFX->Start();
	s4firesparksRightFX->AdvanceByTime(2.0f);

	s4denfliesFX = new SuperFX("PRT\\L2\\s4denflies.INI", PointSystem::CreateCPoint(337,273,0), 50); 
	s4denfliesFX->Start();
	s4denfliesFX->AdvanceByTime(2.0f);

	s4tunnelfliesFX = new SuperFX("PRT\\L2\\s4tunnelflies.INI", PointSystem::CreateCPoint(1052,257,0), 50); 
	s4tunnelfliesFX->Start();
	s4tunnelfliesFX->AdvanceByTime(2.0f);

	S4smokeFX = new SuperFX("PRT\\L2\\S4smoke.INI", PointSystem::CreateCPoint(1212,412,0), 50); 
	S4smokeFX->Start();
	S4smokeFX->AdvanceByTime(2.0f);

	VFXArray.push_back(s4firesparksFX);
	VFXArray.push_back(s4firesparksRightFX);
	VFXArray.push_back(s4denfliesFX);
	VFXArray.push_back(s4tunnelfliesFX);
	VFXArray.push_back(S4smokeFX);

	GFInstance->PostUpdate(2);

	InitPopups();

	GFInstance->PostUpdate(2);

	SceneObjectsUpdate();

	GFInstance->PostUpdate(2);

	cv_LasyaAttack = NULL;
	time = 0;
	subIndex = 0;
	subMax = 3;
	vidTimer = 0;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));
	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("lasyaattack1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("lasyaattack2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("lasyaattack3"));
	isLasyaAttackVideoPlaying = false;
	//lasyaAttackStartTimer = 1.0f;

	Control::Audio->LoadSample(aVOL2MaryRefS4_1,	AudioVO);
	Control::Audio->LoadSample(aAmbs4Cave,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbs4Cave) )		
	{
		Control::Audio->PlaySample(aAmbs4Cave,true);
	}
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L2_S4_Cave]  = true;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S4_IsLasyaAttackVideoPlayed)
	{
		HOScene->GetObjectByID("closedroot")->SetEnable(false);

		//CRectangle RectLasyaVid(171, 96, 1024, 576);
		CRectangle RectLasyaVid(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
		cv_LasyaAttack = new CVideo("OGV\\L2\\S4LASYAATTACK.OGV", RectLasyaVid, false, eZOrder_CutsceneFull);
		//cv_LasyaAttack->SetScale(1.334f);
		cv_LasyaAttack->SetScale(GFApp->video_aspectRatio);
		cv_LasyaAttack->SetSkipActive();
	}

	if(ProfileSystem::Instance->ActiveProfile.L2_S6_IsWoodCutDown)
		HOScene->GetObjectByID("closedroot")->SetEnable(false);

	//Morph Object
#ifdef _CEBUILD
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S4Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("morphtarget"), HOScene->GetObjectByID("morphbase"));
		CRectangle  MorphRect(351.0f,433.0f,60.0f,56.0f);
		Hud->setMorphObjRect(MorphRect);
	}
	else
	{
		HOScene->GetObjectByID("morphtarget")->SetEnable(false);
	}
#else
	HOScene->GetObjectByID("morphtarget")->SetEnable(false);
#endif 

	TaskPendingUpdate();
}

void L2_S4_Cave::InitPopups()
{
	ActivePopupID = ePopInactive;

	for (int i = 0; i < ePop_Max; i++)
	{
		PopupArray[i] = NULL;
		PopStates[i] = ePopupState_Inactive;
		PopPolyArray[i] = NULL;
	}
		
	//Popup 1
	if(!ProfileSystem::Instance->ActiveProfile.L2_S4_InvFirewoodTaken)
	{
		int pop1Pts[] = {433,539,495,538,555,577,556,639,489,663,387,614};
		CPolygon* pop1ClickPoly=  new CPolygon(6);
		pop1ClickPoly->Init(pop1Pts);

		Popup* Pop1Scene = new L2S4_Pop1WoodCut(ePop1_WoodCut, Hud, HOScene, eZOrder_Popup, pop1ClickPoly->center, CC_CALLBACK_1(L2_S4_Cave::OnPopupClosed, this));
		Pop1Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[ePop1_WoodCut] = pop1ClickPoly;
		PopupArray[ePop1_WoodCut]	= Pop1Scene;

		PopStates[ePop1_WoodCut] = ePopupState_Closed;
	}

	//Popup 2
	if(!ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchTaken)
	{
		int pop2Pts[] = {468,415,522,409,525,358,484,304,432,325,422,391};
		CPolygon* pop2ClickPoly=  new CPolygon(6);
		pop2ClickPoly->Init(pop2Pts);

		Popup* Pop2Scene = new L2S4_Pop2FirePop(ePop2_FirePopL, Hud, HOScene, eZOrder_Popup, pop2ClickPoly->center, CC_CALLBACK_1(L2_S4_Cave::OnPopupClosed, this));
		Pop2Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[ePop2_FirePopL] = pop2ClickPoly;
		PopupArray[ePop2_FirePopL]	= Pop2Scene;

		int pop2bPts[] = {889,291,929,287,948,336,941,396,891,392,855,355};
		CPolygon* pop2bClickPoly=  new CPolygon(6);
		pop2bClickPoly->Init(pop2bPts);

		PopPolyArray[ePop2_FirePopR] = pop2bClickPoly;
		PopupArray[ePop2_FirePopR]	= Pop2Scene;

		PopStates[ePop2_FirePopL] = ePopupState_Closed;
		PopStates[ePop2_FirePopR] = ePopupState_Closed;
	}

	//Popup 3
#ifdef _CEBUILD
	if(!ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchStickOiledTaken || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S4TigerMask])
#else
	if(!ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchStickOiledTaken)	
#endif 
	{
		int pop3Pts[] = {663,268,828,266,819,420,785,426,674,419,655,303};
		CPolygon* pop3ClickPoly=  new CPolygon(6);
		pop3ClickPoly->Init(pop3Pts);

		Popup* Pop3Scene = new L2S4_Pop3OilStoneLamp(ePop3_OilStoneLamp, Hud, HOScene, eZOrder_Popup, pop3ClickPoly->center, CC_CALLBACK_1(L2_S4_Cave::OnPopupClosed, this));
		Pop3Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[ePop3_OilStoneLamp] = pop3ClickPoly;
		PopupArray[ePop3_OilStoneLamp]	= Pop3Scene;

		PopStates[ePop3_OilStoneLamp] = ePopupState_Closed;
	}

	//Popup 4
	if(!ProfileSystem::Instance->ActiveProfile.L2_S4_RockSlabRemoved || !ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L2_S4_PZDeities])
	{
		int pop4Pts[] = {776,441,802,458,792,502,735,525,634,514,620,470,662,436};
		CPolygon* pop4ClickPoly=  new CPolygon(7);
		pop4ClickPoly->Init(pop4Pts);

		Popup* Pop4Scene = new L2S4_Pop4StoneBrick(ePop4_StoneBrick, Hud, HOScene, eZOrder_Popup, pop4ClickPoly->center, CC_CALLBACK_1(L2_S4_Cave::OnPopupClosed, this));
		Pop4Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[ePop4_StoneBrick] = pop4ClickPoly;
		PopupArray[ePop4_StoneBrick]	= Pop4Scene;

		PopStates[ePop4_StoneBrick] = ePopupState_Closed;
	}

	//Popup 5
	if(!ProfileSystem::Instance->ActiveProfile.L2_S4_InvRakeTaken)
	{
		int pop5Pts[] = {872,465,901,487,884,546,817,563,791,532,811,485};
		CPolygon* pop5ClickPoly=  new CPolygon(6);
		pop5ClickPoly->Init(pop5Pts);

		Popup* Pop5Scene = new L2S4_Pop5Rake(ePop5_Rake, Hud, HOScene, eZOrder_Popup, pop5ClickPoly->center, CC_CALLBACK_1(L2_S4_Cave::OnPopupClosed, this));
		Pop5Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[ePop5_Rake] = pop5ClickPoly;
		PopupArray[ePop5_Rake]	= Pop5Scene;

		PopStates[ePop5_Rake] = ePopupState_Closed;
	}
}

void L2_S4_Cave::OnPopupClosed(Popup* popup)
{
	ActivePopupID = ePopInactive;
	int popID = popup->GetPopupId();
	if(popID == ePop1_WoodCut)
	{
		if(ProfileSystem::Instance->ActiveProfile.L2_S4_InvFirewoodTaken)
		{
			//disable popup
			PopStates[popID] = ePopupState_Inactive;

			isSparkleUpdatePending = true;
		}
	}
	else if(popID == ePop2_FirePopL)
	{
		if(ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchTaken)
		{
			//disable popups
			PopStates[popID] = ePopupState_Inactive;
			PopStates[ePop2_FirePopR] = ePopupState_Inactive; //also

			isSparkleUpdatePending = true;
		}
	}
	else if(popID == ePop3_OilStoneLamp)
	{
#ifdef _CEBUILD
		if(ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchStickOiledTaken && ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S4TigerMask])
		{
			//disable popups
			PopStates[popID] = ePopupState_Inactive;

			isSparkleUpdatePending = true;
		}
#else
		if(ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchStickOiledTaken)
		{
			//disable popups
			PopStates[popID] = ePopupState_Inactive;
			isSparkleUpdatePending = true;
		}
#endif 	
	}
	else if(popID == ePop4_StoneBrick)
	{
		if(ProfileSystem::Instance->ActiveProfile.L2_S4_RockSlabRemoved && ProfileSystem::Instance->ActiveProfile.L2_S4_BrickPuzzleAreaClicked)
		{
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S4_PZDeities;
			SendMessage(ControlExit, ID, HOSM_NewArea);
			isSparkleUpdatePending = true;
		}
		//Do this in constructor only
		if(ProfileSystem::Instance->ActiveProfile.L2_S4_RockSlabRemoved && ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L2_S4_PZDeities])
		{
			//disable popups
			PopStates[popID] = ePopupState_Inactive;
			isSparkleUpdatePending = true;
		}
	}
	else if(popID == ePop5_Rake)
	{
		if(ProfileSystem::Instance->ActiveProfile.L2_S4_InvRakeTaken)
		{
			//disable popup
			PopStates[popID] = ePopupState_Inactive;
			isSparkleUpdatePending = true;
		}
	}
}

void L2_S4_Cave::SceneObjectsUpdate()
{
	if(ProfileSystem::Instance->ActiveProfile.L2_S4_InvAxeBluntTaken)
		HOScene->GetObjectByID("axe")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L2_S4_IsWoodsCut1)
	{
		HOScene->GetObjectByID("wooduncut")->SetEnable(false);
		HOScene->GetObjectByID("woodcut1")->SetEnable(true);
		if(ProfileSystem::Instance->ActiveProfile.L2_S4_IsWoodsCut2)
		{
			HOScene->GetObjectByID("woodcut1")->SetEnable(false);
			HOScene->GetObjectByID("woodcut")->SetEnable(true);
			if(ProfileSystem::Instance->ActiveProfile.L2_S4_InvFirewoodTaken)
				HOScene->GetObjectByID("woodcut")->SetEnable(false);
		}
		else
		{
			HOScene->GetObjectByID("woodcut")->SetEnable(false);
		}
	}
	else
	{
		HOScene->GetObjectByID("woodcut1")->SetEnable(false);
		HOScene->GetObjectByID("woodcut")->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchStickOiledTaken)
		HOScene->GetObjectByID("firetorch")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L2_S4_InvRakeTaken)
		HOScene->GetObjectByID("rake")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L2_S4_RockSlabRemoved)
	{
		HOScene->GetObjectByID("stone")->SetEnable(false);
		HOScene->GetObjectByID("stonetaken")->SetEnable(true);
	}
	else
	{
		HOScene->GetObjectByID("stonetaken")->SetEnable(false);
	}
	TaskPendingUpdate();
}

void L2_S4_Cave::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if (isLasyaAttackVideoPlaying )
		return;

	if( Control::Audio->IsPlaying(aTrackL2theme1))
		return;

	if( Control::Audio->IsPlaying(aTrackL2ho) )
		{
			Control::Audio->Samples[aTrackL2ho]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL2ho]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL2ho);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL2theme) )
					  Control::Audio->PlaySample(aTrackL2theme, true);
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
	else if( Control::Audio->IsPlaying(aTrackL2puzzle) )
		{
			Control::Audio->Samples[aTrackL2puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL2puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL2puzzle);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL2theme) )
					  Control::Audio->PlaySample(aTrackL2theme, true);
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
		else
		{
			if( !Control::Audio->IsPlaying(aTrackL2theme))
			{
				Control::Audio->TurnOffLvlMscTracks();
				Control::Audio->PlaySample(aTrackL2theme, true);
			}
		}
}

void L2_S4_Cave::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;


	if(isLasyaAttackVideoPlaying)
	{
		if(!cv_LasyaAttack->isSkipMouseOver())
			Cursor::SetMode(CA_Normal);
		//Subtitles Code
		time = cv_LasyaAttack->GetTime();

		if (time >= subtitles[subIndex].begin && time < subtitles[subIndex].end)
		{
			subtitleText->SetText(subtitles[subIndex].text);
		}
		else
			subtitleText->SetText("");

		if (time > subtitles[subIndex].end)
		{
			if (subMax - 1 > subIndex)
				subIndex++;
		}
#ifdef FORCE_CLOSE_VIDEO
		vidTimer += Control::LogicRate;
#endif
		if(
#ifndef FORCE_CLOSE_VID_END
			cv_LasyaAttack->isEndReached() ||
#endif
			vidTimer > 25)
		{
			isLasyaAttackVideoPlaying = false;
			ProfileSystem::Instance->ActiveProfile.L2_S4_IsLasyaAttackVideoPlayed = true;
			subtitleText->SetText("");
			HOScene->GetObjectByID("closedroot")->SetEnable(true);

			Hud->ShowBannerText("L2pest");
			Control::Audio->PlaySample(aVOL2MaryRefS4_1);
			cv_LasyaAttack->StopAndFadeOutVideo(2.0f);
			Hud->AllowInput = true;
			Hud->ShowHud(false);

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L2P6_LasyaAttacksCave])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L2P6_LasyaAttacksCave] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L2P6_LasyaAttacksCave;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
				Hud->ShowJrnEntryFX();
			}
			
			if( !Control::Audio->IsPlaying(aAmbs4Cave) )		
				Control::Audio->PlaySample(aAmbs4Cave,true);
		}
		return;
	}
	{
		/*lasyaAttackStartTimer -= Control::LogicRate;
		if(lasyaAttackStartTimer<0)
		{
			isLasyaAttackVideoPlaying = true;
			cv_LasyaAttack->PlayFadeInVideo(1.0f);
		}*/
	}

	CPoint pos = Control::Input->Mpos();

	if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		SparkleUpdate();

	if( ActivePopupID == ePopInactive )
	{
		Cursor::SetMode(CA_Normal);
		for (int i = 0; i < ePop_Max; i++)
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
						if(ActivePopupID == ePop2_FirePopL || ActivePopupID == ePop2_FirePopR)
						{
							PopupArray[i]->SetZoomOrigin(PopPolyArray[i]->center);
						}
						PopupArray[i]->OpenPopup();
					}
					return;
				}
			}
		}

		if( InfoRectDietyStatue.Intersection(&pos) )
		{
			Cursor::SetMode(CA_Help);
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L2worship");
			}			
			return;
		}

		
		if( isPuzzleDietiesActive && ScnRectPuzzleDieties.Intersection(&pos) )
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
					GFHud->InvBoxCtrl->GoToScene(eArea_L2_S4_PZDeities, 0, ScnRectPuzzleDieties);
				}
#else
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S4_PZDeities;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
		}
		else if( ScnRectMooppansDen.Intersection(&pos) )
		{	

#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
			//	Cursor::SetMode(CA_ExitLeft, eArea_L2_S5_MoopansDen);
#else
			Cursor::SetMode(CA_ExitLeft, eArea_L2_S5_MoopansDen);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L2_S5_MoopansDen, 3, ScnRectMooppansDen);
				}
#else
				Control::Audio->PlayFootStepsFX();

				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S5_MoopansDen;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
		}
		else if( ScnRectTunnelEnd.Intersection(&pos) )
		{
			if(!ProfileSystem::Instance->ActiveProfile.L2_S4_IsLasyaAttackVideoPlayed)
			{
				Cursor::SetMode(CA_ExitUp, -1);
				if( Control::Input->LBclicked() )
				{
					isLasyaAttackVideoPlaying = true;
					cv_LasyaAttack->PlayFadeInVideo(1.0f);
					Hud->EnableInput(false);
					Hud->HideHud(false);
					if( Control::Audio->IsPlaying(aTrackL2theme) )
						 Control::Audio->StopSample(aTrackL2theme);
					if( Control::Audio->IsPlaying(aAmbs4Cave) )		
						Control::Audio->StopSample(aAmbs4Cave);
				}
			}
			else
			{

#ifdef TOUCH_UI
			//	if(!GFHud->InvBoxCtrl->navTapped)
			//		Cursor::SetMode(CA_ExitUp, eArea_L2_S6_TunnelEnd);
#else
				Cursor::SetMode(CA_ExitUp, eArea_L2_S6_TunnelEnd);
#endif

				if( Control::Input->LBclicked() )
				{
#ifdef TOUCH_UI
					if(!GFHud->InvBoxCtrl->navTapped)
					{
						GFHud->InvBoxCtrl->GoToScene(eArea_L2_S6_TunnelEnd, 0, ScnRectTunnelEnd);
					}
#else
					Control::Audio->PlayFootStepsFX();

					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S6_TunnelEnd;
					SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

				}
			}
		}
		else if( IsMouseOverGoBackArea(&pos) )
		{

#ifdef TOUCH_UI
		//	if(!GFHud->InvBoxCtrl->navTapped)
			//	Cursor::SetMode(CA_ExitDown,eArea_L2_S3_CaveMouth);
#else
			Cursor::SetMode(CA_ExitDown,eArea_L2_S3_CaveMouth);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L2_S3_CaveMouth, 2);
				}
#else
				//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S3_CaveMouth;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}	
		}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
				//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S3_CaveMouth;
				SendMessage(ControlExit, ID, HOSM_NewArea);						
		}
	#endif
	}	
}

L2_S4_Cave::~L2_S4_Cave()
{
	SAFE_DELETE(cv_LasyaAttack);

	int size = (int) VFXArray.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		SAFE_DELETE(VFXArray[i]);
	}
	VFXArray.clear();

	for (int i = 0; i < ePop_Max; i++)
	{
		if(i==ePop2_FirePopR)
			PopupArray[i] = NULL;
		else
			SAFE_DELETE(PopupArray[i]);
		SAFE_DELETE(PopPolyArray[i]);
	}

	SAFE_DELETE(subtitleText);
	SAFE_DELETE(meshWater);
	Control::Audio->UnloadSample(aVOL2MaryRefS4_1);
	//MUSIC_SFX
	//Quick SFX unloaded in HUD
	Control::Audio->StopSample(aAmbs4Cave);
	Control::Audio->UnloadSample(aAmbs4Cave);
	//MUSIC_SFX
	
}

void L2_S4_Cave::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	for (int i = 0; i < ePop_Max; i++)
	{
		if(PopStates[i] != ePopupState_Inactive) //if popup is active
		{
			if(PopupArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S4_Cave] = isAnyTaskLeft;

	if( !ProfileSystem::Instance->ActiveProfile.L2_S3_InvTurmericPicked && ProfileSystem::Instance->ActiveProfile.L2_S4_InvRakeTaken)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S3_CaveMouth] = true;
	if( ProfileSystem::Instance->ActiveProfile.L2_S4_InvMetalLeverTaken && !ProfileSystem::Instance->ActiveProfile.L2_S6_LeverUsed && ProfileSystem::Instance->ActiveProfile.L2_S6_IsWoodCutDown)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S6_TunnelEnd] = true;
	if( ProfileSystem::Instance->ActiveProfile.L2_S6_IsWoodCutDown && !ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened && ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchTaken)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S6_TunnelEnd] = true;
	if( ProfileSystem::Instance->ActiveProfile.L2_S5_WoodPlaced &&	ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchFlamed &&	!ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S5_MoopansDen] = true;

}

void L2_S4_Cave::SparkleUpdate()
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
		for (int i = 0; i < ePop_Max; i++)
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

void L2_S4_Cave::ReceiveMessage(int val)
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

void L2_S4_Cave::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S4Collected)
		{
			InitHintTrailEmit(370, 450);
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S4TigerMask])
		{
			if(ActivePopupID != ePopInactive)
				InitHintTrailEmit(900, 390);
			else
				InitHintTrailEmit(730, 350);
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
					for (int i = 0; i < ePop_Max; i++)
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
					if( ProfileSystem::Instance->ActiveProfile.L2_S4_RockSlabRemoved && !ProfileSystem::Instance->ActiveProfile.L2_S4_InvMetalLeverTaken )
					{
						InitHintTrailEmit(&ScnRectPuzzleDieties,false);
						return;
					}
					break;
			case 3: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L2_S4_IsLasyaAttackVideoPlayed)
					{
						InitHintTrailEmit(&ScnRectTunnelEnd,false);
						return;
					}
					break;
			case 4: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S5_MoopansDen] || !ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L2_S5_MoopansDen])
					{
						InitHintTrailEmit(&ScnRectMooppansDen,true,CA_ExitLeft);
						return;
					}
					break;
			case 5: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S6_TunnelEnd])
					{
						InitHintTrailEmit(&ScnRectTunnelEnd,true,CA_ExitRight);
						return;
					}
					break;
			case 6: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S3_CaveMouth])
					{
						InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
						return;
					}
					break;
			case 7: ++iNonHOHint;
					{
						if(Hud->ShowHintForComboInvItems())
							return;
					}
					break;
			case 8: ++iNonHOHint;
					{
						Hud->teleportSystem->ShowTeleport();
						return;
					}
					break;
		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!

	Hud->ShowNothingToDoMsgBanner();
}
void L2_S4_Cave::NavigateToScene()
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