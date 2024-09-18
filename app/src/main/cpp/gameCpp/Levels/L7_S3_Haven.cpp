//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L7_S3_Haven.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"

const CRectangle  SceneRectAmeeha(291.0f,32.0f,300.0f,356.0f);
const CRectangle  PuzzleRectJigsawRed(830.0f,224.0f,133.0f,108.0f);
const CRectangle  SceneRectSophiaDoor(1067.0f,203.0f,242.0f,260.0f);
const CRectangle  DarkartaRect(416.0f,238.0f,145.0f,139.0f);

const CRectangle  PopAreaExtraRect(263.0f,76.0f,842.0f,514.0f);

const CRectangle	PopAreaRect(345.0f,90.0f,665.0f,501.0f);

const CRectangle  InfoRectSophia(989.0f,109.0f,135.0f,67.0f);

L7_S3_Haven::L7_S3_Haven()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV7\\S3\\BG\\S3.SCN");

	HOScene->GetObjectByID("dooropened")->SetEnable(false);
	HOScene->GetObjectByID("pinkglow")->SetScale(1.20f);
	HOScene->GetObjectByID("pinkleftglow")->SetScale(2.00f);
	HOScene->GetObjectByID("yellowglowR")->SetScale(2.00f);
	HOScene->GetObjectByID("rockglow")->SetScale(2.00f);
	HOScene->GetObjectByID("fglineglow")->SetScale(2.00f);
	HOScene->GetObjectByID("stepglow")->SetScale(2.00f);

	if( GFInstance->IsReplayingGame )
	{
		HOScene->GetObjectByID("ameeha")->SetEnable(false);
		HOScene->GetObjectByID("sophia")->SetEnable(false);
	}
	else
		HOScene->GetObjectByID("ameeha")->Animate(0, 2, 0, 0, 4.5f);

	HOScene->GetObjectByID("stepglow")->Animate(0.25f,0.95f,4.25f, 0, eBlend_AlphaAdditive);
	HOScene->GetObjectByID("moonglow")->Animate(0.45f,0.85f,4.25f, 0, eBlend_AlphaAdditive);

	HOScene->GetObjectByID("pinksmallglow")->Animate(0.15f,0.85f,4.2f, 0, eBlend_AlphaAdditive);
	HOScene->GetObjectByID("pinksmallglow2")->Animate(0.45f,0.95f,3.2f, 0, eBlend_AlphaAdditive);
	HOScene->GetObjectByID("pinksmallglow3")->Animate(0.35f,0.85f,3.2f, 0, eBlend_AlphaAdditive);
	HOScene->GetObjectByID("pinksmallglow4")->Animate(0.25f,0.85f,3.4f, 0, eBlend_AlphaAdditive);
	HOScene->GetObjectByID("pinksmallglow5")->Animate(0.35f,0.75f,3.6f, 0, eBlend_AlphaAdditive);
	HOScene->GetObjectByID("pinksmallglow6")->Animate(0.25f,0.95f,4.0f, 0, eBlend_AlphaAdditive);
	HOScene->GetObjectByID("pinksmallglow7")->Animate(0.25f,0.75f,3.5f, 3.75f, eBlend_AlphaAdditive);

	HOScene->GetObjectByID("yellowglowR")->Animate(0.15f,0.85f,5.0f, 0, eBlend_AlphaAdditive);
	HOScene->GetObjectByID("rockglow")->Animate(0.45f,0.95f,2.5f, 0, eBlend_AlphaAdditive);
	HOScene->GetObjectByID("pinkglow")->Animate(0.35f,0.65f,4.0f, 0, eBlend_AlphaAdditive);
	HOScene->GetObjectByID("pinkglow2")->Animate(0.45f,0.75f,4.5f, 0, eBlend_AlphaAdditive);
	HOScene->GetObjectByID("pinkleftglow")->Animate(0.25f,0.85f,3.25f, 0, eBlend_AlphaAdditive);
	HOScene->GetObjectByID("fglineglow")->Animate(0.35f,0.85f,3.8f, 0, eBlend_AlphaAdditive);

	meshwater = NULL;
	meshwater = new MeshObject(HOScene->GetObjectByID("meshwater"), 6, 6);
	meshwater->meshFX->SetSpeedAndDisplacement(4, 6, 8, 4);

	HOScene->InsertObject(meshwater,1);

	//HOScene->GetObjectByID("fireglowleft")->SetBlendMode(eBlend_AlphaAdditive);
	//HOScene->GetObjectByID("fireglowleft")->Animate(0.15f,0.45f,3.0f);

	//HOScene->GetObjectByID("leftplant")->SetPivot(-10, 43);
	//HOScene->GetObjectByID("leftplant")->Animate(0,0,4,0,8.0f);

	s3fireprtFX = new SuperFX("PRT\\L7\\s3fireprt.INI", PointSystem::CreateCPoint(129,389,0), 100); 
	s3fireprtFX->Start();
	s3fireprtFX->AdvanceByTime(2.0f);

	s3fireprt2FX = new SuperFX("PRT\\L7\\s3fireprt2.INI", PointSystem::CreateCPoint(262,451,0), 100); 
	s3fireprt2FX->Start();
	s3fireprt2FX->AdvanceByTime(2.0f);

	s3lavafall2FX = new SuperFX("PRT\\L7\\s3lavafall2.INI", PointSystem::CreateCPoint(859,149,0), 100); 
	s3lavafall2FX->Start();
	s3lavafall2FX->AdvanceByTime(2.0f);

	s3lavafall3FX = new SuperFX("PRT\\L7\\s3lavafall3.INI", PointSystem::CreateCPoint(1322,345,0), 100); 
	s3lavafall3FX->Start();
	s3lavafall3FX->AdvanceByTime(2.0f);

	s3lavafall4FX = new SuperFX("PRT\\L7\\s3lavafall4.INI", PointSystem::CreateCPoint(699,175,0), 100); 
	s3lavafall4FX->Start();
	s3lavafall4FX->AdvanceByTime(2.0f);

	s3smokefrontFX = new SuperFX("PRT\\L7\\s3smokefront.INI", PointSystem::CreateCPoint(458,722,0), 100); 
	s3smokefrontFX->Start();
	s3smokefrontFX->AdvanceByTime(2.0f);

	s3smokegroundFX = new SuperFX("PRT\\L7\\s3smokeground.INI", PointSystem::CreateCPoint(722,609,0), 100); 
	s3smokegroundFX->Start();
	s3smokegroundFX->AdvanceByTime(2.0f);

	s3rightsmokedownFX = new SuperFX("PRT\\L7\\s3rightsmokedown.INI", PointSystem::CreateCPoint(1118,643,0), 100); 
	s3rightsmokedownFX->Start();
	s3rightsmokedownFX->AdvanceByTime(2.0f);

	s3starsFX = new SuperFX("PRT\\L7\\s3stars.INI", PointSystem::CreateCPoint(159,104,0), 100); 
	s3starsFX->Start();
	s3starsFX->AdvanceByTime(2.0f);

	s3lavadropsFX = new SuperFX("PRT\\L7\\s3lavadrops.INI", PointSystem::CreateCPoint(489,535,0), 100); 
	s3lavadropsFX->Start();
	s3lavadropsFX->AdvanceByTime(2.0f);

	s3rightsmokeFX = new SuperFX("PRT\\L7\\s3rightsmoke.INI", PointSystem::CreateCPoint(922,380,0), 100); 
	s3rightsmokeFX->Start();
	s3rightsmokeFX->AdvanceByTime(2.0f);

	s3leftsmokeFX = new SuperFX("PRT\\L7\\s3leftsmoke.INI", PointSystem::CreateCPoint(230,509,0), 100); 
	s3leftsmokeFX->Start();
	s3leftsmokeFX->AdvanceByTime(2.0f);

	s3lavadrops2FX = new SuperFX("PRT\\L7\\s3lavadrops2.INI", PointSystem::CreateCPoint(545,469,0), 100); 
	s3lavadrops2FX->Start();
	s3lavadrops2FX->AdvanceByTime(2.0f);

	s3lavadrops3FX = new SuperFX("PRT\\L7\\s3lavadrops3.INI", PointSystem::CreateCPoint(649,544,0), 100); 
	s3lavadrops3FX->Start();
	s3lavadrops3FX->AdvanceByTime(2.0f);

	s3lavadrops4FX = new SuperFX("PRT\\L7\\s3lavadrops4.INI", PointSystem::CreateCPoint(1183,449,0), 100); 
	s3lavadrops4FX->Start();
	s3lavadrops4FX->AdvanceByTime(2.0f);

	s3lavafallsFX = new SuperFX("PRT\\L7\\s3lavafalls.INI", PointSystem::CreateCPoint(1232,516,0), 100); 
	s3lavafallsFX->Start();
	s3lavafallsFX->AdvanceByTime(2.0f);

	InitPopups();

	SceneObjectsUpdate();

	isLasyaAttackVideoPlaying = false;
	cv_LasyaAttack = NULL;

	time = 0;
	subIndex = 0;
	subMax = 10;
	vidTimer = 0;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("lv7lasyaattack1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("lv7lasyaattack2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("lv7lasyaattack3"));
	subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("lv7lasyaattack4"));
	subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("lv7lasyaattack5"));
	subtitles[5] = AssetsStringTable::getSub(AssetsStringTable::getString("lv7lasyaattack6"));
	subtitles[6] = AssetsStringTable::getSub(AssetsStringTable::getString("lv7lasyaattack7"));
	subtitles[7] = AssetsStringTable::getSub(AssetsStringTable::getString("lv7lasyaattack8"));
	subtitles[8] = AssetsStringTable::getSub(AssetsStringTable::getString("lv7lasyaattack9"));
	subtitles[9] = AssetsStringTable::getSub(AssetsStringTable::getString("lv7lasyaattack10"));

	if( GFInstance->IsReplayingGame )
	{
		HOScene->GetObjectByID("ananthahead")->SetEnable(false);
		HOScene->GetObjectByID("ananthahand")->SetEnable(false);
		HOScene->GetObjectByID("ananthabody")->SetEnable(false);			
		HOScene->GetObjectByID("clothmask")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("ananthahead")->SetPivot(-4, 8);
		HOScene->GetObjectByID("ananthahead")->Animate(0,0,3,0,5.0f);

		HOScene->GetObjectByID("ananthahand")->SetPivot(-18, -9);
		HOScene->GetObjectByID("ananthahand")->Animate(0,0,2,0,5.0f);
	}

	
	if(!ProfileSystem::Instance->ActiveProfile.L7_S3_IsLasyaAttackCutscnPlayed)
	{
		if( !GFInstance->IsReplayingGame )
		{
			HOScene->GetObjectByID("ananthabody")->SetEnable(false);
			HOScene->GetObjectByID("ananthahead")->SetEnable(false);
			HOScene->GetObjectByID("ananthahand")->SetEnable(false);
			HOScene->GetObjectByID("clothmask")->SetEnable(false);
		}

		if(ProfileSystem::Instance->ActiveProfile.L7_S3_PZDoorCompleted)
		{
			//CRectangle RectLasyaVid(171, 96, 1024, 576);
			CRectangle RectLasyaVid(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
			cv_LasyaAttack = new CVideo("OGV\\L7\\S3LASYAATTACK.OGV", RectLasyaVid, false, eZOrder_CutsceneFull);
			//cv_LasyaAttack->SetScale(1.334f);
			cv_LasyaAttack->SetScale(GFApp->video_aspectRatio);
			cv_LasyaAttack->SetSkipActive();

			
			isLasyaAttackVideoPlaying = true;
			cv_LasyaAttack->PlayFadeInVideo(100.0f);
			Hud->EnableInput(false);
			Hud->HideHud(false);			
		}
	}

	TaskPendingUpdate();

	dialogSystem = NULL;
	//if condition to add
	dialogSystem = new DialogSystem();
	AddControl(dialogSystem);
	
	isDialogActive = false;

#ifdef _CEBUILD
	//Morph Object Add
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S3Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("morphtarget"), HOScene->GetObjectByID("morphbase"));
		CRectangle  MorphRect(847.0f,377.0f,49.0f,63.0f);
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
	Control::Audio->LoadSample(aVOL4MaryRefS4_2,	AudioVO);
	Control::Audio->LoadSample(aAmbL7s3HavenAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL7s3HavenAmb) )		
	{
		Control::Audio->PlaySample(aAmbL7s3HavenAmb,true);
	}
	
	//MUSIC_SFX
	
	

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L7_S3_Haven]  = true;
}



void L7_S3_Haven::InitPopups()
{
	ActivePopupID = ePopInactive;

	for (int i = 0; i < ePopL7S3_Max; i++)
	{
		PopupArray[i] = NULL;
		PopStates[i] = ePopupState_Inactive;
		PopPolyArray[i] = NULL;
	}	

	pop1Bhasmam = NULL;
	pop2BullStatue = NULL;
	pop3Chain = NULL;

	int popID=0;

	bool isPopupsActive = !ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalWireTaken || !ProfileSystem::Instance->ActiveProfile.L7_S3_InvYellowFlowerTaken || !ProfileSystem::Instance->ActiveProfile.L7_S3_InvBhasmamPowderTaken;
	
	popID = ePop1_Bhasmam;
	if(isPopupsActive)
	{
		int pts0[] = {709,316,778,352,806,455,704,497,642,405,646,334};
		CPolygon* clickPoly=  new CPolygon(6);
		clickPoly->Init(pts0);

		L7S3_Pop1Bhasmam* Pop2Scene = new L7S3_Pop1Bhasmam(popID, Hud, HOScene, eZOrder_Popup, clickPoly->center, CC_CALLBACK_1(L7_S3_Haven::OnPopupClosed, this));
		Pop2Scene->SetPopupBoundingBox(PopAreaExtraRect);

		pop1Bhasmam = Pop2Scene;
		pop1Bhasmam->setLoadNextCallback(CC_CALLBACK_2(L7_S3_Haven::LoadNextPopup, this));

		PopPolyArray[popID] = clickPoly;
		PopupArray[popID]	= Pop2Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	popID = ePop2_BullStatue;
	if(isPopupsActive)
	{
		int pts0[] = {709,316,778,352,806,455,704,497,642,405,646,334};
		CPolygon* clickPoly=  new CPolygon(6);
		clickPoly->Init(pts0);

		L7S3_Pop2Bull* Pop2Scene = new L7S3_Pop2Bull(popID, Hud, HOScene, eZOrder_Popup, clickPoly->center, CC_CALLBACK_1(L7_S3_Haven::OnPopupClosed, this));
		
		Pop2Scene->SetPopupBoundingBox(PopAreaExtraRect);

		pop2BullStatue = Pop2Scene;
		pop2BullStatue->setLoadNextCallback(CC_CALLBACK_2(L7_S3_Haven::LoadNextPopup, this));

		PopPolyArray[popID] = clickPoly;
		PopupArray[popID]	= Pop2Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	//Popup 2 wall 3
	popID = ePop3_Chain;
	if(isPopupsActive)
	{
		int pts0[] = {709,316,778,352,806,455,704,497,642,405,646,334};
		CPolygon* clickPoly=  new CPolygon(6);
		clickPoly->Init(pts0);

		L7S3_Pop3Chain* Pop2Scene = new L7S3_Pop3Chain(popID, Hud, HOScene, eZOrder_Popup, clickPoly->center, CC_CALLBACK_1(L7_S3_Haven::OnPopupClosed, this));
		Pop2Scene->SetPopupBoundingBox(PopAreaExtraRect);

		pop3Chain = Pop2Scene;
		pop3Chain->setLoadNextCallback(CC_CALLBACK_2(L7_S3_Haven::LoadNextPopup, this));

		PopPolyArray[popID] = clickPoly;
		PopupArray[popID]	= Pop2Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	//Popup 4 Ameeha
	popID = ePop4_Ameeha;	
	{
		int pts4[] = {316,9,571,16,527,122,486,183,377,183};
		CPolygon* clickPoly=  new CPolygon(5);
		clickPoly->Init(pts4);

		L7S3_Pop4Ameeha* Pop4Scene = new L7S3_Pop4Ameeha(popID, Hud, HOScene, eZOrder_Popup, clickPoly->center, CC_CALLBACK_1(L7_S3_Haven::OnPopupClosed, this));
		Pop4Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[popID] = clickPoly;
		PopupArray[popID]	= Pop4Scene;

		if( GFInstance->IsReplayingGame )
			PopStates[popID] = ePopupState_Inactive;
		else if(ProfileSystem::Instance->ActiveProfile.L7_S3_IsLasyaAttackCutscnPlayed)
			PopStates[popID] = ePopupState_Closed;
		else
			PopStates[popID] = ePopupState_Inactive;
	}
}

void L7_S3_Haven::OnPopupClosed(Popup* popup)
{
	ActivePopupID = ePopInactive;
	// int popID = popup->GetPopupId(); //Sac: Unused variable, so commented it

	if( ProfileSystem::Instance->ActiveProfile.L7_S3_InvYellowFlowerTaken && ProfileSystem::Instance->ActiveProfile.L7_S3_InvBhasmamPowderTaken &&
		ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalWireTaken && ProfileSystem::Instance->ActiveProfile.L7_S3_InvMetalChainTaken )
	{
		PopStates[ePop1_Bhasmam] = ePopInactive;
		PopStates[ePop2_BullStatue] = ePopInactive;
		PopStates[ePop3_Chain] = ePopInactive;

		isSparkleUpdatePending = true;
	}

	TaskPendingUpdate();
}

void L7_S3_Haven::SceneObjectsUpdate()
{
	if(ProfileSystem::Instance->ActiveProfile.L7_S3_PZJigsawRedCompleted)
	{
		HOScene->GetObjectByID("jigsawground")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("jigsawsolved")->SetEnable(false);
	}

	if(!ProfileSystem::Instance->ActiveProfile.L7_S3_IsDiwaliLampPlaced)
	{
		HOScene->GetObjectByID("diwalilamp")->SetEnable(false);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L7_S3_InvBhasmamPowderTaken)
	{
		HOScene->GetObjectByID("bhasmam")->SetEnable(false);
		HOScene->GetObjectByID("diwalilamp")->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L7_S3_PZDoorState >= 6)//if threads removed
	{
		HOScene->GetObjectByID("doorthread")->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L7_S3_IsNightmareCutscnPlayed)
	{
		HOScene->GetObjectByID("dooropened")->SetEnable(true);
	}

	TaskPendingUpdate();
}

void L7_S3_Haven::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if(isLasyaAttackVideoPlaying)
		return;

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

void L7_S3_Haven::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	if(isDialogActive)
	{
		dialogSystem->Update();
		return;
	}

	

	#ifdef _PARTICLEEDITORENABLED
		if (_isParticleEditorActive)
		{
			return;
		}
	#endif

	CPoint pos = Control::Input->Mpos();

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
			vidTimer > 59)
		{
			AwardManager::GetInstance()->CollectAward(kAward_Unveiled_Maskedman);
			isLasyaAttackVideoPlaying = false;
			subtitleText->SetText("");

			ProfileSystem::Instance->ActiveProfile.L7_S3_IsLasyaAttackCutscnPlayed = true;
			
			Hud->AllowInput = true;
			Hud->ShowHud(false);
			//Activate the Popup
			PopStates[ePop4_Ameeha] = ePopupState_Closed;

			HOScene->GetObjectByID("ananthabody")->SetEnable(true);
			HOScene->GetObjectByID("ananthahead")->SetEnable(true);
			HOScene->GetObjectByID("ananthahand")->SetEnable(true);
			HOScene->GetObjectByID("clothmask")->SetEnable(true);	

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L7P6_LasyaAttack1])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L7P6_LasyaAttack1] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L7P6_LasyaAttack1;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
			}

			//Journal Entry Add
			else if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L7P7_LasyaAttack2])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L7P7_LasyaAttack2] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L7P7_LasyaAttack2;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
			}

			//Objective Add
			if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L7O3_FindWayToSaveAmeeha] )
			{
				ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L7O3_FindWayToSaveAmeeha] = true;
				ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
				= eObj_L7O3_FindWayToSaveAmeeha;
				//Increment Objectives
				ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
				Hud->ShowObjectiveFX(eObj_L7O3_FindWayToSaveAmeeha);

				Control::Audio->PlaySample(aSFXMagic1);
			}

			//Hud->ShowBannerText("Something else");//Sac: where is the external text for this ?
			cv_LasyaAttack->StopAndFadeOutVideo(2.0f);
		}
		return;
	}

	if( ActivePopupID == ePopInactive )
	{
		Cursor::SetMode(CA_Normal);
		if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
			SparkleUpdate();

		//popup click checks
		for (int i = 0; i < ePopL7S3_Max; i++)
		{
			if(PopStates[i] != ePopupState_Inactive) //if popup is active
			{
				if(PopPolyArray[i]!=NULL && PopPolyArray[i]->Intersection(&pos))
				{
					Cursor::SetMode(CA_Look);
					if( Control::Input->LBclicked() )
					{
						Cursor::SetMode(CA_Normal);
						if(i<ePop4_Ameeha)
						{
							if(ProfileSystem::Instance->ActiveProfile.L7_S3_InvBhasmamPowderTaken)
							{
								if(ProfileSystem::Instance->ActiveProfile.L7_S3_InvYellowFlowerTaken && ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxeKeyTaken)
								{
									ActivePopupID = ePop3_Chain;
									PopupArray[ePop3_Chain]->OpenPopup();
								}
								else
								{
									ActivePopupID = ePop2_BullStatue;
									PopupArray[ePop2_BullStatue]->OpenPopup();
								}
							}
							else
							{
								ActivePopupID = ePop1_Bhasmam;
								PopupArray[ePop1_Bhasmam]->OpenPopup();
							}
						}
						else
						{
							ActivePopupID = i;
							PopupArray[i]->OpenPopup();
						}
					}
					return;
				}
			}
		}

		if( !GFInstance->IsReplayingGame  )//main chapter completed, no sopia present
			if(InfoRectSophia.Intersection(&pos))
			{
				Cursor::SetMode(CA_Help);
				if( Control::Input->LBclicked() )
				{
					//Objective Add
					if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L7O2_FindWayToSophia] )
					{
						ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L7O2_FindWayToSophia] = true;
						ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
						= eObj_L7O2_FindWayToSophia;
						//Increment Objectives
						ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
						Hud->ShowObjectiveFX(eObj_L7O2_FindWayToSophia);

						Control::Audio->PlaySample(aSFXMagic1);
					}

					Hud->ShowBannerText("L4hurry");
					Control::Audio->PlaySample(aVOL4MaryRefS4_2);
				}
				return;
			}

		if( !GFInstance->IsReplayingGame )
			if(ProfileSystem::Instance->ActiveProfile.L7_S3_IsLasyaAttackCutscnPlayed && DarkartaRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_Speech);
				if( Control::Input->LBclicked() )
				{
					dialogSystem->ShowDialog(eDialog_L7S3Anantha);
					isDialogActive = true;
					Hud->EnableInput(false);
					Hud->HideHud(false);
				}	
			}

		if( !ProfileSystem::Instance->ActiveProfile.L7_S3_IsLasyaAttackCutscnPlayed && SceneRectAmeeha.Intersection(&pos) )
		{
			
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitUp, eArea_L7_S4_Ameeha);
#else
		Cursor::SetMode(CA_ExitUp, eArea_L7_S4_Ameeha);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S4_Ameeha, 0, SceneRectAmeeha);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S4_Ameeha;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
				
			}	
		}

		if( !ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[1] && PuzzleRectJigsawRed.Intersection(&pos) )
		{
			
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_Look, eArea_L7_S3_PZJigsawRed);
#else
		Cursor::SetMode(CA_Look, eArea_L7_S3_PZJigsawRed);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S3_PZJigsawRed, 0, PuzzleRectJigsawRed);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S3_PZJigsawRed;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
				
			}	
		}

		if( SceneRectSophiaDoor.Intersection(&pos) )
		{
			
#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_Look, eArea_L7_S3_PZDoorPuzzle);
#else
		Cursor::SetMode(CA_Look, eArea_L7_S3_PZDoorPuzzle);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S3_PZDoorPuzzle, 0, SceneRectSophiaDoor);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S3_PZDoorPuzzle;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
				
			}
		}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
		//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S1_Volcano;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif	
		if( IsMouseOverGoBackArea(&pos) )
		{
			
#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitDown, eArea_L7_S1_Volcano);
#else
		Cursor::SetMode(CA_ExitDown, eArea_L7_S1_Volcano);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S1_Volcano, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S1_Volcano;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
				
			}	
		}
	}
}

void L7_S3_Haven::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	for (int i = 0; i < ePopL7S3_Max; i++)
	{
		if(PopStates[i] != ePopupState_Inactive) //if popup is active
		{
			if(PopupArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L7_S3_PZJigsawRedCompleted)
		isAnyTaskLeft = true;

	if(!ProfileSystem::Instance->ActiveProfile.L7_S3_InvLeverTaken)
		isAnyTaskLeft = true;

	if(ProfileSystem::Instance->ActiveProfile.L7_S4_HaveHandPower && !ProfileSystem::Instance->ActiveProfile.L7_S3_PZDoorCompleted)
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S3_Haven] = isAnyTaskLeft;


	if(ProfileSystem::Instance->ActiveProfile.L7_S3_PZJigsawRedCompleted && !ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[1])
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S1_Volcano] = true;

	if(ProfileSystem::Instance->ActiveProfile.L7_S3_InvAxesTaken && !ProfileSystem::Instance->ActiveProfile.L7_S1_InvAxeUsed)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S1_Volcano] = true;

	if(ProfileSystem::Instance->ActiveProfile.L7_S3_InvBhasmamPowderTaken && !ProfileSystem::Instance->ActiveProfile.L7_S4_TableTopBasmamUsed)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S4_Ameeha] = true;

	if(ProfileSystem::Instance->ActiveProfile.L7_S3_InvYellowFlowerTaken && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L7_S3_YellowFlowe]) //if yellow flower not placed
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S4_Ameeha] = true;
}

void L7_S3_Haven::LoadNextPopup(Popup* popup, bool loadPrevious)
{
	if(popup->GetPopupId() == ePop1_Bhasmam && !loadPrevious)
	{
		pop2BullStatue->StartSceneSlideIn(false);
		ActivePopupID = ePop2_BullStatue;
	}
	else if(popup->GetPopupId() == ePop2_BullStatue)
	{
		if(loadPrevious)
		{
			pop1Bhasmam->StartSceneSlideIn(true);
			ActivePopupID = ePop1_Bhasmam;
		}
		else
		{
			pop3Chain->StartSceneSlideIn(false);
			ActivePopupID = ePop3_Chain;
		}
	}
	else if(popup->GetPopupId() == ePop3_Chain)
	{
		if(loadPrevious)
		{
			ActivePopupID = ePop2_BullStatue;
			pop2BullStatue->StartSceneSlideIn(true);
		}
	}
}

void L7_S3_Haven::SparkleUpdate()
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
		for (int i = 0; i < ePopL7S3_Max; i++)
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

		if( !ProfileSystem::Instance->ActiveProfile.L7_S1_InvPZStarEmblemCollected[1] )
		{
			sparklePoints[numSparkles].SetXY(PuzzleRectJigsawRed.GetCenterX(), PuzzleRectJigsawRed.GetCenterY());
			numSparkles++;
		}

		if( !ProfileSystem::Instance->ActiveProfile.L7_S3_InvLeverTaken || ProfileSystem::Instance->ActiveProfile.L7_S4_HaveHandPower )
		{
			sparklePoints[numSparkles].SetXY(SceneRectSophiaDoor.GetCenterX(), SceneRectSophiaDoor.GetCenterY());
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

void L7_S3_Haven::ReceiveMessage(int val)
{
	if( val == HOSM_DialogClosed ) 
	{
		isDialogActive = false;
		Hud->AllowInput = true;
		Hud->ShowHud(false);

		//Journal Entry Add
		if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L7P8_AnanthaWeepDialogEnd])
		{
			//Flag Set
			ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L7P8_AnanthaWeepDialogEnd] = true;

			//Add to Note Array
			ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L7P8_AnanthaWeepDialogEnd;

			ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

			//Increment Notes
			ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
			Hud->ShowJrnEntryFX();
		}
	}
	else if( val == MAP_NEW_AREA ) 
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

void L7_S3_Haven::ProcessSkip()
{
}

void L7_S3_Haven::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S3Collected)
		{
			InitHintTrailEmit(860, 410);
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
			for (int i = 0; i < ePopL7S3_Max; i++)
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
			if( !ProfileSystem::Instance->ActiveProfile.L7_S3_PZJigsawRedCompleted )
			{
				InitHintTrailEmit(&PuzzleRectJigsawRed,false);
				return;
			}
			break;
		case 3: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L7_S3_InvLeverTaken )
			{
				InitHintTrailEmit(&SceneRectSophiaDoor,true);
				return;
			}
			else if( ProfileSystem::Instance->ActiveProfile.L7_S4_HaveHandPower)
			{
				InitHintTrailEmit(&SceneRectSophiaDoor,true);
				return;
			}
			break;
		case 4: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L7_S4_Ameeha] && ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S4_Ameeha] )
			{
				InitHintTrailEmit(&SceneRectAmeeha,true,CA_ExitUp);
				return;
			}
			break;
		case 5: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S1_Volcano])
			{
				InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
				return;
			}
			break;
		case 6: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L7_S5_SophiaEnd])
			{
				InitHintTrailEmit(&SceneRectSophiaDoor,true,CA_ExitRight);
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
	while( iNonHOHint < MaxNonHOHints );

	Hud->ShowNothingToDoMsgBanner();
}

L7_S3_Haven::~L7_S3_Haven()
{	
	SAFE_DELETE(s3lavadropsFX);
	SAFE_DELETE(s3rightsmokeFX);
	SAFE_DELETE(s3leftsmokeFX);
	SAFE_DELETE(s3lavadrops2FX);
	SAFE_DELETE(s3lavadrops3FX);
	SAFE_DELETE(s3lavadrops4FX);
	SAFE_DELETE(s3lavafallsFX);
	SAFE_DELETE(s3fireprtFX);
	SAFE_DELETE(s3fireprt2FX);
	SAFE_DELETE(s3lavafall2FX);
	SAFE_DELETE(s3lavafall3FX);
	SAFE_DELETE(s3lavafall4FX);
	SAFE_DELETE(s3smokefrontFX);
	SAFE_DELETE(s3smokegroundFX);
	SAFE_DELETE(s3rightsmokedownFX);
	SAFE_DELETE(s3starsFX);

	SAFE_DELETE(cv_LasyaAttack);
	SAFE_DELETE(subtitleText);

	for (int i = 0; i < ePopL7S3_Max; i++)
	{
		SAFE_DELETE(PopupArray[i]);
		SAFE_DELETE(PopPolyArray[i]);
	}

		//MUSIC_SFX
	Control::Audio->UnloadSample(aVOL4MaryRefS4_2);
	Control::Audio->StopSample(aAmbL7s3HavenAmb);
	Control::Audio->UnloadSample(aAmbL7s3HavenAmb);
	//MUSIC_SFX
}
void L7_S3_Haven::NavigateToScene()
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