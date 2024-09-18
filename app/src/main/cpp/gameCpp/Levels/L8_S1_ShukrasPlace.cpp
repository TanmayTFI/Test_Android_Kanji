//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL8 - SCENE1 : SHUKRAS PLACE
//====================================

#include "L8_S1_ShukrasPlace.h"
#include "Util.h"

const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  SceneRectGarden(1014.0f,292.0f,100.0f,145.0f);
const CRectangle  PZRect(276.0f,282.0f,58.0f,144.0f);

enum eL6S1HOPopEnums
{
	eL8S1Pop_Inactive = -1,
	eL8S1Pop1_MagicWand,
	eL8S1Pop2_HiddenCompartment,
	eL8S1Pop3_Door
};

#define X_OFFSET_MASK	170
#define Y_OFFSET_MASK	120

L8_S1_ShukrasPlace::L8_S1_ShukrasPlace()
{
	//Backup Save for level restart
	if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L8_S2_Garden])
		ProfileSystem::Instance->SaveProfileBackup(ProfileSystem::Instance->ProfileSlot);

	LoadHud(INVENTORY_V1);
	LoadScene("LV8\\S1\\S1.SCN");

	int Pop1ArrayPts[] = {593,389,831,385,830,512,585,509,583,388,591,386};
	ClickablePolygon *popClickPoly = new ClickablePolygon(eL8S1Pop1_MagicWand, 6, Pop1ArrayPts);
	PopRectArray.push_back(popClickPoly);
	L8S1_Pop1MagicWand *pop1 = new  L8S1_Pop1MagicWand(eL8S1Pop1_MagicWand, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L8_S1_ShukrasPlace::OnPopupClosed, this));
	pop1->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop1);
	popClickPoly = NULL;

	int Pop2ArrayPts[] = {272,489,236,525,248,559,353,550,399,502,345,483,272,485};
	popClickPoly = new ClickablePolygon(eL8S1Pop2_HiddenCompartment, 7, Pop2ArrayPts);
	PopRectArray.push_back(popClickPoly);
	L8S1_Pop2HiddenCompartment *pop2 = new  L8S1_Pop2HiddenCompartment(eL8S1Pop2_HiddenCompartment, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L8_S1_ShukrasPlace::OnPopupClosed, this));
	pop2->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop2);
	popClickPoly = NULL;

	int Pop3ArrayPts[] = {319,272,274,289,265,318,271,430,352,429,358,336,360,294,340,273,317,264,320,266,294,279};
	popClickPoly = new ClickablePolygon(eL8S1Pop3_Door, 11, Pop3ArrayPts);
	PopRectArray.push_back(popClickPoly);
	pop3 = new  L8S1_Pop3DoorPopup(eL8S1Pop3_Door, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L8_S1_ShukrasPlace::OnPopupClosed, this));
	pop3->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop3);
	popClickPoly = NULL;

	for (int i = 0; i < MAX_L8S1_FLYING_DOTS; i++)
	{
		flyingDots[i] = NULL;
		flyingDots[i] = new FlyingDot();
		flyingDots[i]->setdotObject(HOScene->GetObjectByID(std::string("flydot" + MKSTR((i+1))).c_str()));
		flyingDots[i]->SetProperties(685,344,110,55,((i<3)?true:false),((i<3)?i*60:60+(i-3)*50.0f));
	}

	mo_flag1 = new MeshObject(HOScene->GetObjectByID("Flag01"),5,5);
	mo_flag1->meshFX->SetSpeedAndDisplacement(5, 2, 2, 1);
	//mo_flag1->SetZ(eZOrder_Popup - 1);
	HOScene->InsertObject(mo_flag1, 2);

	mo_flag2 = new MeshObject(HOScene->GetObjectByID("Flag02"),5,5);
	mo_flag2->meshFX->SetSpeedAndDisplacement(5, 2, 2, 1);
	//mo_flag2->SetZ(eZOrder_Popup - 1);
	HOScene->InsertObject(mo_flag2, 2);

	mo_flag3 = new MeshObject(HOScene->GetObjectByID("Flag03"),5,5);
	mo_flag3->meshFX->SetSpeedAndDisplacement(1, 2, 2, 5);
	//mo_flag3->SetZ(eZOrder_Popup - 1);
	HOScene->InsertObject(mo_flag3, 2);

	mo_flag4 = new MeshObject(HOScene->GetObjectByID("Flag04"),5,5);
	mo_flag4->meshFX->SetSpeedAndDisplacement(1, 2, 2, 5);
	//mo_flag4->SetZ(eZOrder_Popup - 1);
	HOScene->InsertObject(mo_flag4, 2);

	ActivePopUpId = eL8S1Pop_Inactive;

	sceneMagicGlass = new CHOScene();
	sceneMagicGlass->Load("LV8\\S_MG\\S_MG.SCN");
	sceneMagicGlass->SetZOff(eZOrder_FoundInvPopup);
	sceneMagicGlass->Show(false);
	isGlassInHand = false;

	isVideoPlaying = false;
	lpIntroCutscene = NULL;

	time = 0;
	subIndex = 0;
	subMax = 3;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("lv8intro1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("lv8intro2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("lv8intro3"));

	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	lpIntroCutscene = new CVideo("OGV\\L8\\S1INTROCUTSCENE.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	//lpIntroCutscene->SetScale(1.334f);
	lpIntroCutscene->SetScale(GFApp->video_aspectRatio);
	vidTimer = 0;
	int ArrayPts[] = {683,241,606,265,579,335,597,400,634,429,690,445,747,417,780,363,763,286,723,250,681,237};
	globPoly =  new CPolygon(11);
	globPoly->Init(ArrayPts);

	for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
	{
		symbols[i] = HOScene->GetObjectByID(MKSTR(i));
		redSymbols[i] = HOScene->GetObjectByID(MKSTR(i) + "_2");

#ifdef TOUCH_UI
		symbols[i]->SetScale(1.5f);
		redSymbols[i]->SetScale(1.5f);
		//symbols[i]->SetTint(1,1,0);
#endif

		if (i % 2 == 0)
		{
			silhouttes[i /2] = HOScene->GetObjectByID("sil_" + MKSTR(i / 2));
			silhouttes[i /2]->SetZ(eZOrder_HUD + 1);
		}
	}
	silhouttes[(MAX_L8_S1_MATCHING_SYMBOLS_COUNT / 2)] = HOScene->GetObjectByID("sil_" + MKSTR(MAX_L8_S1_MATCHING_SYMBOLS_COUNT / 2));
	silhouttes[(MAX_L8_S1_MATCHING_SYMBOLS_COUNT / 2)]->SetZ(eZOrder_HUD + 1);
	selectedSymbol = -1;

	InitParticles();

	SceneObjectsUpdate();
	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aVOL8GuruRefS1_1,	AudioVO);
	Control::Audio->LoadSample(aAmbL8s1ShukrasPalaceAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL8s1ShukrasPalaceAmb) )
	{
		Control::Audio->PlaySample(aAmbL8s1ShukrasPalaceAmb,true);
	}


	//MUSIC_SFX
	HOScene->GetObjectByID("skipbtn")->SetEnable(false);

	SkipBtn = new CObject();
	SkipBtn->LoadInit("UI\\HUD\\BTNSKIP.SPR", 100, 50, eZOrder_HUD);
	SkipBtn->SetUID("SkipBtn");
	HOScene->PushObject(SkipBtn);
	SkipBtn->PlayAnimation(2);
	SkipBtn->SetEnable(false);

	skipBTN_text = new CObject();
	skipBTN_text->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\SKIP.SPR", 100, 50, eZOrder_HUD+1);
	skipBTN_text->SetUID("skipBTN_text");
	HOScene->PushObject(skipBTN_text);
	skipBTN_text->SetEnable(false);
}

bool L8_S1_ShukrasPlace::isPopupActive(int popId)
{
	switch (popId)
	{
		case eL8S1Pop1_MagicWand:	if (!isGlassInHand /*&& (!ProfileSystem::Instance->ActiveProfile.L8_S1_InvHalfKeyTaken || !ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicWandTaken || !ProfileSystem::Instance->ActiveProfile.L8_S1_NoteTaken)*/)		return true;	break;
		case eL8S1Pop2_HiddenCompartment: if (!ProfileSystem::Instance->ActiveProfile.L8_S1_InvIdolTaken && ((isGlassInHand && ProfileSystem::Instance->ActiveProfile.L8_S1_MatchingHOComplete) || ProfileSystem::Instance->ActiveProfile.L8_MagicSymbolFound[1]))		return true;	break;
		case eL8S1Pop3_Door:  if (!isGlassInHand && !ProfileSystem::Instance->ActiveProfile.L8_S1_DoorKeyPlaced)	return true;	break;
		default:
			break;
	}

	return false;
}


void L8_S1_ShukrasPlace::InitPopups()
{

}

void L8_S1_ShukrasPlace::OnPopupClosed(Popup * popup)
{
	SceneObjectsUpdate();

	ActivePopUpId = eL8S1Pop_Inactive;

	switch (popup->GetPopupId())
	{
		case eL8S1Pop2_HiddenCompartment:
			isGlassInHand = false;
			break;
		case eL8S1Pop3_Door:
			if (ProfileSystem::Instance->ActiveProfile.L8_S1_DoorKeyPlaced)
			{
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S1_PZDoorHandPuzzle;
				SendMessage(ControlExit, ID, HOSM_NewArea);
			}
			break;
		default:
			break;
	}
}

void L8_S1_ShukrasPlace::SceneObjectsUpdate()
{
	TaskPendingUpdate();

	if (ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicWandTaken)
	{
		HOScene->GetObjectByID("Sukra-weapon_Scene")->SetEnable(false);
	}

	if (!ProfileSystem::Instance->ActiveProfile.L8_S1_FloorCracked)
	{
		HOScene->GetObjectByID("Crack-on-floor")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("Crack-on-floor")->SetEnable(true);
	}

	HOScene->GetObjectByID("MarkOnFloor")->SetEnable(false);

	for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
	{
		symbols[i]->SetEnable(false);
		redSymbols[i]->SetAlpha(0);
		redSymbols[i]->SetPos(symbols[i]->GetPosPointer());
		silhouttes[i / 2]->SetEnable(false);
	}

	silhouttes[MAX_L8_S1_MATCHING_SYMBOLS_COUNT/2]->SetEnable(false);

	//for (int i = 0; i < 3; i++)
	//{
	//	string id ="BtnGlow0" + MKSTR(i + 1);
	//	if(!ProfileSystem::Instance->ActiveProfile.L8_MagicSymbolFound[i])
	//	{
	//		sceneMagicGlass->GetObjectByID(id)->SetEnable(false);
	//	}
	//}

	if (ProfileSystem::Instance->ActiveProfile.L8_S1_DoorKeyPlaced)
	{
		HOScene->GetObjectByID("Door")->SetEnable(false);
	}

	if (!ProfileSystem::Instance->ActiveProfile.L8_S1_HandPuzzleSolved)
	{
		HOScene->GetObjectByID("door_open")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("door_open")->SetEnable(true);
	}
}

void L8_S1_ShukrasPlace::InitParticles()
{
	HOScene->GetObjectByID("Front_LeftLampGlow")->Animate(0.3f,1,3,3);
	HOScene->GetObjectByID("Front_RightLampGlow")->Animate(0.3f,1,3,3);
	HOScene->GetObjectByID("LeftLampGlow")->Animate(0.3f,1,3,3);
	HOScene->GetObjectByID("RightLampGlow")->Animate(0.3f,1,3,3);

	HOScene->GetObjectByID("sidewindowglow1")->SetScale(2.00f);
	HOScene->GetObjectByID("sidewindowglow2")->SetScale(2.10f);
	HOScene->GetObjectByID("doorglow")->SetScale(2.00f);
	HOScene->GetObjectByID("topglow1")->SetScale(1.70f);
	HOScene->GetObjectByID("topglow2")->SetScale(2.00f);
	HOScene->GetObjectByID("blueleftglow")->SetScale(2.00f);
	HOScene->GetObjectByID("bluerightglow")->SetScale(2.10f);

	HOScene->GetObjectByID("sidewindowglow1")->Animate(0.45f,0.85f,3.5f,0,eBlend_AlphaAdditive);
	HOScene->GetObjectByID("sidewindowglow2")->Animate(0.35f,0.95f,4.5f,0,eBlend_AlphaAdditive);
	HOScene->GetObjectByID("doorglow")->Animate(0.25f,0.65f,4.5f,0,eBlend_AlphaAdditive);
	HOScene->GetObjectByID("topglow1")->Animate(0.25f,0.75f,3.5f,0,eBlend_AlphaAdditive);
	HOScene->GetObjectByID("topglow2")->Animate(0.25f,0.85f,6.5f,0,eBlend_AlphaAdditive);
	HOScene->GetObjectByID("blueleftglow")->Animate(0.25f,0.75f,3.5f,0,eBlend_AlphaAdditive);
	HOScene->GetObjectByID("bluerightglow")->Animate(0.25f,0.75f,3.5f,0,eBlend_AlphaAdditive);
	HOScene->GetObjectByID("leftwindowglow")->Animate(0.25f,0.75f,4.5f,0,eBlend_AlphaAdditive);
	HOScene->GetObjectByID("leftwindowglow")->Animate(0.25f,0.75f,5.5f,0,eBlend_AlphaAdditive);
	HOScene->GetObjectByID("smallwindowglow")->Animate(0.45f,0.85f,4.5f,0,eBlend_AlphaAdditive);
	HOScene->GetObjectByID("smallwindowglow2")->Animate(0.25f,0.95f,6.5f,0,eBlend_AlphaAdditive);


	HOScene->GetObjectByID("crystal-ball")->Animate(0, 4, 0, 0, 6.0f);
	HOScene->GetObjectByID("Crystal-ball-effect")->Animate(0.25f,0.75f,3.5f,0,eBlend_AlphaAdditive);
	HOScene->GetObjectByID("Crystal-ball-GlowOnGround")->Animate(0,1,4,2);

	s1sidewindowraysFX = new SuperFX("PRT\\L8\\s1sidewindowrays.INI", PointSystem::CreateCPoint(1285,252,0), 100);
	s1sidewindowraysFX->Start();
	s1sidewindowraysFX->AdvanceByTime(2.0f);

	s1doorraysFX = new SuperFX("PRT\\L8\\s1doorrays.INI", PointSystem::CreateCPoint(1050,355,0), 100);
	s1doorraysFX->Start();
	s1doorraysFX->AdvanceByTime(2.0f);

	s1lightflyFX = new SuperFX("PRT\\L8\\s1lightfly.INI", PointSystem::CreateCPoint(437,565,0), 100);
	s1lightflyFX->Start();
	s1lightflyFX->AdvanceByTime(2.0f);

	s1lightfly2FX = new SuperFX("PRT\\L8\\s1lightfly.INI", PointSystem::CreateCPoint(937,565,0), 100);
	s1lightfly2FX->Start();
	s1lightfly2FX->AdvanceByTime(2.0f);

	s1goldglitterFX = new SuperFX("PRT\\L8\\s1goldglitter.INI", PointSystem::CreateCPoint(684,532,0), 100);
	s1goldglitterFX->Start();
	s1goldglitterFX->AdvanceByTime(2.0f);


	holinesFX = new SuperFX("PRT\\L2\\holines.INI", PointSystem::CreateCPoint(320,400,0), 100);

	hodotsFX = new SuperFX("PRT\\L2\\hodots.INI", PointSystem::CreateCPoint(314,407,0), 100);

	if( ProfileSystem::Instance->ActiveProfile.L8_S1_HandPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicGlassTaken)
	{
		if( !ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles )
		{
			holinesFX->Start();
			holinesFX->AdvanceByTime(2.0f);

			hodotsFX->Start();
			hodotsFX->AdvanceByTime(2.0f);
		}
	}
	/*GlobeGlowParticle = new SuperFX("PRT\\L8\\s1GlobGlow.INI", PointSystem::CreateCPoint(680,347,0), 100);
	GlobeGlowParticle->Start();
	GlobeGlowParticle->AdvanceByTime(2.0);*/
}

L8_S1_ShukrasPlace::~L8_S1_ShukrasPlace()
{
	Hud->InvBoxCtrl->ResetToInvBox();
	Hud->IsHOScene = false;

	for (int i = 0; i < MAX_L8S1_FLYING_DOTS; i++)
	{
		SAFE_DELETE(flyingDots[i]);
	}

	pop3 = NULL;
	for (int i = 0; i < (int)(PopupArray.size()); i++)
	{
		SAFE_DELETE(PopupArray[i]);
	}
	PopupArray.clear();
	for (int i = 0; i < (int)(PopRectArray.size()); i++)
	{
		SAFE_DELETE(PopRectArray[i]);
	}
	PopRectArray.clear();

	SAFE_DELETE(s1sidewindowraysFX);
	SAFE_DELETE(s1doorraysFX);
	SAFE_DELETE(s1lightflyFX);
	SAFE_DELETE(s1lightfly2FX);
	SAFE_DELETE(s1goldglitterFX);
	SAFE_DELETE(holinesFX);
	SAFE_DELETE(hodotsFX);

	/*SAFE_DELETE(mo_flag1);
	SAFE_DELETE(mo_flag2);
	SAFE_DELETE(mo_flag3);
	SAFE_DELETE(mo_flag4);*/
	SAFE_DELETE(globPoly);
	SAFE_DELETE(sceneMagicGlass);
	SAFE_DELETE(lpIntroCutscene);
	SAFE_DELETE(subtitleText);
	//MUSIC_SFX
	Control::Audio->UnloadSample(aVOL8GuruRefS1_1);
	Control::Audio->StopSample(aAmbL8s1ShukrasPalaceAmb);
	Control::Audio->UnloadSample(aAmbL8s1ShukrasPalaceAmb);
	//MUSIC_SFX

	//SAFE_DELETE(GlobeGlowParticle);
}

void L8_S1_ShukrasPlace::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if(isVideoPlaying)
		return;

	if( Control::Audio->IsPlaying(aTrackL8ho) )
	{
		Control::Audio->Samples[aTrackL8ho]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL8ho]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL8ho);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL8theme) )
				Control::Audio->PlaySample(aTrackL8theme, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else if( Control::Audio->IsPlaying(aTrackL8puzzle) )
	{
		Control::Audio->Samples[aTrackL8puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL8puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL8puzzle);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL8theme) )
				Control::Audio->PlaySample(aTrackL8theme, true);

		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL8theme))
		{
			Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL8theme, true);
		}
	}
}

void L8_S1_ShukrasPlace::Update()
{
	//Cheats goes here
#ifdef _TABINGAMEUI
	if(_TabInGameMenuShowing)
		return;
#endif // _TABINGAMEUI
#ifdef _PARTICLEEDITORENABLED
	if (_isParticleEditorActive)
	{
		return;
	}
#endif

	if( Hud->PopupState != Popup_Inactive )
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;
	if (ActivePopUpId != eL8S1Pop_Inactive)
		return;

#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
#else
	Cursor::SetMode(CA_Normal);
#endif

	HiddenObjectSystem::HOUpdate();

	CPoint pos = Control::Input->Mpos();
	NavigateToScene();

	for (int i = 0; i < MAX_L8S1_FLYING_DOTS; i++)
	{
		flyingDots[i]->update(Control::LogicRate);
	}

	if (!ProfileSystem::Instance->ActiveProfile.L8_S1_IntroVidPlayed && !isVideoPlaying)
	{
		Hud->AllowInput = false;
		Hud->HideHud();
		lpIntroCutscene->SetSkipActive();
		isVideoPlaying = true;
		lpIntroCutscene->PlayVideo();
		return;
	}
	else if (isVideoPlaying )
	{
		//Subtitles Code
		time = lpIntroCutscene->GetTime();

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
		if((
#ifndef FORCE_CLOSE_VID_END
			lpIntroCutscene->isEndReached() ||
#endif
			vidTimer > 24) && !ProfileSystem::Instance->ActiveProfile.L8_S1_IntroVidPlayed)
		{
			subtitleText->SetText("");
			//Objective Add
			if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L8O1_FindWayToResurrectMary] )
			{
				ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L8O1_FindWayToResurrectMary] = true;
				ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
						= eObj_L8O1_FindWayToResurrectMary;
				//Increment Objectives
				ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
				Hud->ShowObjectiveFX(eObj_L8O1_FindWayToResurrectMary);

				Control::Audio->PlaySample(aSFXMagic1);
			}

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L8P1_ShukraVideoEnd])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L8P1_ShukraVideoEnd] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L8P1_ShukraVideoEnd;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
			}

			ProfileSystem::Instance->ActiveProfile.L8_S1_IntroVidPlayed = true;
			isVideoPlaying = false;
			lpIntroCutscene->StopAndFadeOutVideo(2.0f);
			Hud->AllowInput = true;
			Hud->ShowHud();
			Hud->SendUpdateSceneMessage();
			/*Control::Audio->PlaySample(aVOL8GuruRefS1_1);
			Hud->ShowBannerText("I must find Mary's life leaf and reunite her with her family.");*/
		}
		return;
	}

	for (int i = 0; i < (int)(PopupArray.size()); i++)
	{
		if( PopRectArray[i]->Intersection(&pos) && isPopupActive(i))
		{
			Cursor::SetMode(CA_Look);
			if (Control::Input->LBclicked())
			{
				ActivePopUpId = i;
				PopupArray[i]->OpenPopup();
				if (isGlassInHand)
				{
					isGlassInHand = false;
					sceneMagicGlass->Show(false);
					HOScene->GetObjectByID("MarkOnFloor")->SetEnable(false);
					for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
					{
						symbols[i]->SetEnable(false);
					}
					silhouttes[MAX_L8_S1_MATCHING_SYMBOLS_COUNT/2]->SetEnable(false);
					Hud->InvBoxCtrl->ResetToInvBox();
					Hud->IsHOScene = false;
				}
			}
			//	return;
		}
	}
#ifdef TOUCH_UI
	if(isGlassInHand)
		GFApp->pinchDisabled = true;
	else
		GFApp->pinchDisabled = false;
#endif
	SkipMagicGlass();
	if (isGlassInHand)
	{
#ifdef TOUCH_UI
		if(Control::Input->LBdown())
		{
			if(!LBPressed)
			{
				LBPressed = true;
				prevPos.x = KInput::getMouseX();
				prevPos.y = KInput::getMouseY();
			}
			else
			{
				mouseDx = KInput::getMouseX() - prevPos.x;
				mouseDy = KInput::getMouseY() - prevPos.y;

				if(mouseDx < -50)
					mouseDx = -50;
				if(mouseDy < -50)
					mouseDy = -50;
				if(mouseDx > 50)
					mouseDx = 50;
				if(mouseDy > 50)
					mouseDy = 50;

				currentPos.x += (float)(mouseDx);
				currentPos.y += (float)(mouseDy);


				if(currentPos.x < -200)
					currentPos.x = -200;
				if(currentPos.y < -200)
					currentPos.y = -200;
				if(currentPos.x > 1500)
					currentPos.x = 1500;
				if(currentPos.y > 800)
					currentPos.y = 800;


				prevPos.x = KInput::getMouseX();
				prevPos.y = KInput::getMouseY();
			}
		}
		else
		{
			LBPressed = false;
		}

		sceneMagicGlass->GetObjectByID("MagnifyingGlass")->SetPos(&currentPos);
#else
		sceneMagicGlass->GetObjectByID("MagnifyingGlass")->SetPos(&pos);
#endif
		for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
		{
			if (!ProfileSystem::Instance->ActiveProfile.L8_S1_MatchingSymbolFound[i] && symbols[i]->GetWorldRect().Intersection(&pos))
			{
				Cursor::SetMode(CA_HandPointy);
				if(Control::Input->LBclicked())
				{
					if (selectedSymbol >= 0)
					{
						if (i % 2)
						{
							if (selectedSymbol == i - 1)
							{
								symbols[i]->PlayAnimation(1);
								symbols[selectedSymbol]->PlayAnimation(1);
								symbols[i]->FadeOut();
								symbols[selectedSymbol]->FadeOut();
								ProfileSystem::Instance->ActiveProfile.L8_S1_MatchingSymbolFound[i] = true;
								ProfileSystem::Instance->ActiveProfile.L8_S1_MatchingSymbolFound[selectedSymbol] = true;
								silhouttes[selectedSymbol / 2]->PlayAnimation(1);

								bool isComplete = true;
								for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
								{
									if (!ProfileSystem::Instance->ActiveProfile.L8_S1_MatchingSymbolFound[i])
									{
										isComplete = false;
									}
								}
								if (isComplete)
								{
									HOScene->GetObjectByID("MarkOnFloor")->SetEnable(true);
									silhouttes[MAX_L8_S1_MATCHING_SYMBOLS_COUNT/2]->SetEnable(true);
									for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
									{
										silhouttes[i / 2]->SetEnable(false);
									}
									ProfileSystem::Instance->ActiveProfile.L8_S1_MatchingHOComplete = true;
								}
							}
							else
							{
								symbols[selectedSymbol]->PlayAnimation(0);
								symbols[i]->PlayAnimation(0);
								redSymbols[selectedSymbol]->FadeInOut();
								redSymbols[i]->FadeInOut();
							}
						}
						else
						{
							if (selectedSymbol == i + 1)
							{
								symbols[i]->PlayAnimation(1);
								symbols[selectedSymbol]->PlayAnimation(1);
								symbols[i]->FadeOut();
								symbols[selectedSymbol]->FadeOut();
								ProfileSystem::Instance->ActiveProfile.L8_S1_MatchingSymbolFound[i] = true;
								ProfileSystem::Instance->ActiveProfile.L8_S1_MatchingSymbolFound[selectedSymbol] = true;
								silhouttes[i / 2]->PlayAnimation(1);

								bool isComplete = true;
								for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
								{
									if (!ProfileSystem::Instance->ActiveProfile.L8_S1_MatchingSymbolFound[i])
									{
										isComplete = false;
									}
								}
								if (isComplete)
								{
									HOScene->GetObjectByID("MarkOnFloor")->SetEnable(true);
									silhouttes[MAX_L8_S1_MATCHING_SYMBOLS_COUNT/2]->SetEnable(true);
									for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
									{
										silhouttes[i / 2]->SetEnable(false);
									}
									ProfileSystem::Instance->ActiveProfile.L8_S1_MatchingHOComplete = true;
								}
							}
							else
							{
								symbols[selectedSymbol]->PlayAnimation(0);
								symbols[i]->PlayAnimation(0);
								redSymbols[selectedSymbol]->FadeInOut();
								redSymbols[i]->FadeInOut();
							}
						}
						selectedSymbol = -1;
					}
					else
					{
						selectedSymbol  = i;
						symbols[i]->PlayAnimation(1);
					}
				}
				//return;

			}
		}


		float startX = HOScene->GetObjectByID("MarkOnFloor")->GetPosPointer()->x - HOScene->GetObjectByID("MarkOnFloor")->GetLocalRect().w/2;
		float startY = HOScene->GetObjectByID("MarkOnFloor")->GetPosPointer()->y - HOScene->GetObjectByID("MarkOnFloor")->GetLocalRect().h/2;
#ifndef TOUCH_UI
		CRectangle  *TmpRect = new CRectangle((pos.x - X_OFFSET_MASK + 40 > startX)?pos.x - X_OFFSET_MASK + 40:startX,(pos.y - Y_OFFSET_MASK > startY)?pos.y - Y_OFFSET_MASK :startY,pos.x + X_OFFSET_MASK - HOScene->GetObjectByID("MarkOnFloor")->GetPosPointer()->x,pos.y + Y_OFFSET_MASK - HOScene->GetObjectByID("MarkOnFloor")->GetPosPointer()->y);
#else
		CRectangle  *TmpRect = new CRectangle((currentPos.x - X_OFFSET_MASK + 40 > startX)?currentPos.x - X_OFFSET_MASK + 40:startX,(currentPos.y - Y_OFFSET_MASK > startY)?currentPos.y - Y_OFFSET_MASK :startY,currentPos.x + X_OFFSET_MASK - HOScene->GetObjectByID("MarkOnFloor")->GetPosPointer()->x,currentPos.y + Y_OFFSET_MASK - HOScene->GetObjectByID("MarkOnFloor")->GetPosPointer()->y);
#endif
		HOScene->GetObjectByID("MarkOnFloor")->SetMaskRect(TmpRect);

		delete(TmpRect);

		for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
		{
			float startX = HOScene->GetObjectByID(MKSTR(i))->GetPosPointer()->x - HOScene->GetObjectByID(MKSTR(i))->GetLocalRect().w/2;
			float startY = HOScene->GetObjectByID(MKSTR(i))->GetPosPointer()->y - HOScene->GetObjectByID(MKSTR(i))->GetLocalRect().h/2;
#ifndef TOUCH_UI
			CRectangle  *TmpRect = new CRectangle((pos.x - X_OFFSET_MASK + 40 > startX)?pos.x - X_OFFSET_MASK + 40:startX,(pos.y - Y_OFFSET_MASK > startY)?pos.y - Y_OFFSET_MASK :startY,pos.x + X_OFFSET_MASK - 30 - HOScene->GetObjectByID(MKSTR(i))->GetPosPointer()->x,pos.y + Y_OFFSET_MASK - HOScene->GetObjectByID(MKSTR(i))->GetPosPointer()->y);
#else
			CRectangle  *TmpRect = new CRectangle((currentPos.x - X_OFFSET_MASK + 40 > startX)?currentPos.x - X_OFFSET_MASK + 40:startX,(currentPos.y - Y_OFFSET_MASK > startY)?currentPos.y - Y_OFFSET_MASK :startY,currentPos.x + X_OFFSET_MASK - 30 - HOScene->GetObjectByID(MKSTR(i))->GetPosPointer()->x,currentPos.y + Y_OFFSET_MASK - HOScene->GetObjectByID(MKSTR(i))->GetPosPointer()->y);
#endif
			symbols[i]->SetMaskRect(TmpRect);
			redSymbols[i]->SetMaskRect(TmpRect);

			delete(TmpRect);
		}
#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			isGlassInHand = false;
			sceneMagicGlass->Show(false);
			HOScene->GetObjectByID("MarkOnFloor")->SetEnable(false);
			for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
			{
				symbols[i]->SetEnable(false);
				silhouttes[i / 2]->SetEnable(false);
			}
			silhouttes[MAX_L8_S1_MATCHING_SYMBOLS_COUNT/2]->SetEnable(false);

			Hud->InvBoxCtrl->ResetToInvBox();
			Hud->IsHOScene = false;
		}
#endif
		if (IsMouseOverGoBackArea(&pos))
		{
			Cursor::SetMode(CA_ExitDown);
			if (Control::Input->LBclicked())
			{
				isGlassInHand = false;
				sceneMagicGlass->Show(false);
				HOScene->GetObjectByID("MarkOnFloor")->SetEnable(false);
				for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
				{
					symbols[i]->SetEnable(false);
					silhouttes[i / 2]->SetEnable(false);
				}
				silhouttes[MAX_L8_S1_MATCHING_SYMBOLS_COUNT/2]->SetEnable(false);

				Hud->InvBoxCtrl->ResetToInvBox();
				Hud->IsHOScene = false;
			}
		}

		return;
	}

	if (SceneRectGarden.Intersection(&pos) && !isGlassInHand)
	{

#ifdef TOUCH_UI
		//	if(!GFHud->InvBoxCtrl->navTapped)
		//		Cursor::SetMode(CA_ExitRight,eArea_L8_S2_Garden);
#else
		Cursor::SetMode(CA_ExitRight,eArea_L8_S2_Garden);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L8_S2_Garden, 1, SceneRectGarden);
			}
#else
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S2_Garden;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}
	}
	else if (ProfileSystem::Instance->ActiveProfile.L8_S1_DoorKeyPlaced && !ProfileSystem::Instance->ActiveProfile.L8_S1_HandPuzzleSolved && PopRectArray[eL8S1Pop3_Door]->Intersection(&pos))
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
				GFHud->InvBoxCtrl->GoToScene(eArea_L8_S1_PZDoorHandPuzzle, 0, CRectangle(260, 290, 100, 150));//, PopRectArray[eL8S1Pop3_Door]);
			}
#else
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S1_PZDoorHandPuzzle;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}
	}
	else if (ProfileSystem::Instance->ActiveProfile.L8_S1_HandPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicGlassTaken && PopRectArray[eL8S1Pop3_Door]->Intersection(&pos))
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
				GFHud->InvBoxCtrl->GoToScene(eArea_L8_S1_DoorHO, 0, CRectangle(260, 290, 100, 150));//, PopRectArray[eL8S1Pop3_Door]);
			}
#else
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S1_DoorHO;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}
	}
	else if (globPoly->Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L8all");
		}
	}
	else if (PopRectArray[eL8S1Pop2_HiddenCompartment]->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L8_S1_InvIdolTaken)
	{
		Cursor::SetMode(CA_Help);
		if(Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L8idols");
		}
	}
	else if (Control::Input->LBclicked())
	{
		Hud->ShowBannerText("L8took");
	}

	if (isGlassInHand)
	{
		//sceneMagicGlass->GetObjectByID("MagnifyingGlass")->SetPos(&pos);
		///*sceneMagicGlass->GetObjectByID("BtnGlow01")->SetPos(new CPoint(pos.x - 78,pos.y + 114,pos.z));
		//sceneMagicGlass->GetObjectByID("BtnGlow02")->SetPos(new CPoint(pos.x,pos.y + 129,pos.z));
		//sceneMagicGlass->GetObjectByID("BtnGlow03")->SetPos(new CPoint(pos.x + 78,pos.y + 113,pos.z));*/

		//float startX = HOScene->GetObjectByID("MarkOnFloor")->GetPosPointer()->x - HOScene->GetObjectByID("MarkOnFloor")->GetLocalRect().w/2;
		//float startY = HOScene->GetObjectByID("MarkOnFloor")->GetPosPointer()->y - HOScene->GetObjectByID("MarkOnFloor")->GetLocalRect().h/2;
		//CRectangle  TmpRect = new CRectangle((pos.x - X_OFFSET_MASK + 40 > startX)?pos.x - X_OFFSET_MASK + 40:startX,(pos.y - Y_OFFSET_MASK > startY)?pos.y - Y_OFFSET_MASK :startY,pos.x + X_OFFSET_MASK - HOScene->GetObjectByID("MarkOnFloor")->GetPosPointer()->x,pos.y + Y_OFFSET_MASK - HOScene->GetObjectByID("MarkOnFloor")->GetPosPointer()->y);

		//HOScene->GetObjectByID("MarkOnFloor")->SetMaskRect(TmpRect);

		//for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
		//{
		//	float startX = HOScene->GetObjectByID(MKSTR(i))->GetPosPointer()->x - HOScene->GetObjectByID(MKSTR(i))->GetLocalRect().w/2;
		//	float startY = HOScene->GetObjectByID(MKSTR(i))->GetPosPointer()->y - HOScene->GetObjectByID(MKSTR(i))->GetLocalRect().h/2;
		//	CRectangle  TmpRect = new CRectangle((pos.x - X_OFFSET_MASK + 40 > startX)?pos.x - X_OFFSET_MASK + 40:startX,(pos.y - Y_OFFSET_MASK > startY)?pos.y - Y_OFFSET_MASK :startY,pos.x + X_OFFSET_MASK - 30 - HOScene->GetObjectByID(MKSTR(i))->GetPosPointer()->x,pos.y + Y_OFFSET_MASK - HOScene->GetObjectByID(MKSTR(i))->GetPosPointer()->y);

		//	symbols[i]->SetMaskRect(TmpRect);
		//	redSymbols[i]->SetMaskRect(TmpRect);
		//}
	}


}

void L8_S1_ShukrasPlace::ProcessSkip()
{
	if (ActivePopUpId == eL8S1Pop3_Door )
	{
		pop3->ProcessSkip();
	}

}

void L8_S1_ShukrasPlace::Render(const CPoint* pCam)
{

}


void L8_S1_ShukrasPlace::TakeInventoryItem(EHOInventory ItemID)
{

}


void L8_S1_ShukrasPlace::ReceiveMessage(int val)
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
			if (itemVal == eInv_L8_S1_MagicGlass && !ProfileSystem::Instance->ActiveProfile.L8_MagicSymbolFound[1])
			{

				sceneMagicGlass->Show(true);
				isGlassInHand = true;
				currentPos = Control::Input->Mpos();

				Hud->ReleaseDrag();
				Hud->CloseInventory();

				bool isComplete = true;
				for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
				{
					if (!ProfileSystem::Instance->ActiveProfile.L8_S1_MatchingSymbolFound[i])
					{
						isComplete = false;
						symbols[i]->SetEnable(true);
					}
					else
					{
						silhouttes[i / 2]->PlayAnimation(1);
					}
					if (!ProfileSystem::Instance->ActiveProfile.L8_S1_MatchingHOComplete)
						silhouttes[i / 2]->SetEnable(true);
				}

				if (ProfileSystem::Instance->ActiveProfile.L8_S1_MatchingHOComplete && !ProfileSystem::Instance->ActiveProfile.L8_S1_InvIdolTaken)
				{
					HOScene->GetObjectByID("MarkOnFloor")->SetEnable(true);
					silhouttes[MAX_L8_S1_MATCHING_SYMBOLS_COUNT/2]->SetEnable(true);
				}

				Hud->InvBoxCtrl->SetLockBase(HO_FINDPAIRS_FRAME);
				Hud->IsHOScene = true;
				Hud->HOState = HOActive;
			}
			else
			{
				if (ActivePopUpId != ePopInactive)
				{
					PopupArray[ActivePopUpId]->HandleInventoryDrop(dropPos, itemVal);
					return;
				}
				//If no drop items in this scene !!
				GFInstance->WrongInvDropMsg(Hud);
			}

		}
	}
	else if( val == INV_CLOSE )
	{
		if (ActivePopUpId == eL8S1Pop1_MagicWand)
		{
			((L8S1_Pop1MagicWand*)(PopupArray[eL8S1Pop1_MagicWand]))->ReceiveMessage(val);
		}
	}
}

float L8_S1_ShukrasPlace::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}


void L8_S1_ShukrasPlace::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	for (int i = 0; i < (int)(PopupArray.size()); i++)
	{
		if(isPopupActive(i)) //if popup is active
		{
			if(PopupArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}

	if (ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicGlassTaken && !ProfileSystem::Instance->ActiveProfile.L8_S1_FloorCracked)
		isAnyTaskLeft = true;
	if (ProfileSystem::Instance->ActiveProfile.L8_S1_DoorKeyPlaced && !ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicGlassTaken)
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S1_ShukrasPlace] = isAnyTaskLeft;

	if (ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicGlassTaken && !ProfileSystem::Instance->ActiveProfile.L8_S2_FloorCracked)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S2_Garden] = true;
	if (ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicGlassTaken && !ProfileSystem::Instance->ActiveProfile.L8_S3_FloorCracked)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S3_BanyanTree] = true;

	if( ((ProfileSystem::Instance->ActiveProfile.L8_S3_InvIdolTaken && !ProfileSystem::Instance->ActiveProfile.L8_S4_BronzeIdolPlaced ) ||
		 (ProfileSystem::Instance->ActiveProfile.L8_S2_InvIdolTaken && !ProfileSystem::Instance->ActiveProfile.L8_S4_SilverIdolPlaced ) ||
		 (ProfileSystem::Instance->ActiveProfile.L8_S1_InvIdolTaken && !ProfileSystem::Instance->ActiveProfile.L8_S4_GoldIdolPlaced )) &&
		(ProfileSystem::Instance->ActiveProfile.L8_S2_GateOpened ))
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S4_Pool] = true;
}

void L8_S1_ShukrasPlace::ProcessNonHOHint()
{
	int iNonHOHint = 0;
	int MaxNonHOHints = 16;
	do
	{
		switch( iNonHOHint )
		{
			default:
				//Exit coordinates
			case 0: ++iNonHOHint;
				if( ActivePopUpId != ePopInactive )
				{
					PopupArray[ActivePopUpId]->ProcessHint();
					return;
				}
				break;
			case 1: ++iNonHOHint;
				for (int i = 0; i < (int)(PopupArray.size()); i++)
				{
					if(isPopupActive(i)) //if popup is active
					{
						if(PopupArray[i]->isTaskLeft())
						{
							ClickablePolygon *rect = PopRectArray[i];
							InitHintTrailEmit(rect->center.x,rect->center.y,false);
							return;
						}
					}
				}
				break;
			case 2:++iNonHOHint;
				if (ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicGlassTaken && !ProfileSystem::Instance->ActiveProfile.L8_S1_FloorCracked && !isGlassInHand)
				{
					//Hud->ShowHintForInvItem(eInv_L8_S1_MagicGlass, &HOScene->GetObjectByID("Bg")->GetWorldRect());
					// Linux: gives error of taking address of temporary also potential crash factor
					// below is fix
					CRectangle ObjRect =HOScene->GetObjectByID("Bg")->GetWorldRect();
					Hud->ShowHintForInvItem(eInv_L8_S1_MagicGlass, &ObjRect);
					return;
				}
				break;
			case 3:++iNonHOHint;
				if (isGlassInHand && !ProfileSystem::Instance->ActiveProfile.L8_S1_MatchingHOComplete)
				{
					if (selectedSymbol >= 0)
					{
						if (selectedSymbol % 2)
						{
							//Hud->InitHintTrailEmit(&symbols[selectedSymbol - 1]->GetWorldRect());
							// Linux: gives error of taking address of temporary also potential crash factor
							// below is fix
							CRectangle selectObjRect =symbols[selectedSymbol - 1]->GetWorldRect();
							Hud->InitHintTrailEmit(&selectObjRect);
							return;
						}
						else
						{
							//Hud->InitHintTrailEmit(&symbols[selectedSymbol + 1]->GetWorldRect());
							// Linux: gives error of taking address of temporary also potential crash factor
							// below is fix
							CRectangle selectObjRect =symbols[selectedSymbol + 1]->GetWorldRect();
							Hud->InitHintTrailEmit(&selectObjRect);
							return;
						}
					}
					else
					{
						for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
						{
							if (!ProfileSystem::Instance->ActiveProfile.L8_S1_MatchingSymbolFound[i])
							{
								//Hud->InitHintTrailEmit(&symbols[i]->GetWorldRect());
								// Linux: gives error of taking address of temporary also potential crash factor
								// below is fix
								CRectangle selectObjRect =symbols[i]->GetWorldRect();
								Hud->InitHintTrailEmit(&selectObjRect);
								return;
							}
						}
					}
				}
				break;
			case 4:++iNonHOHint;
				if (ProfileSystem::Instance->ActiveProfile.L8_S1_DoorKeyPlaced && !ProfileSystem::Instance->ActiveProfile.L8_S1_HandPuzzleSolved)
				{
					InitHintTrailEmit(&PZRect,false);
					return;
				}
				break;
			case 5:++iNonHOHint;
				if (ProfileSystem::Instance->ActiveProfile.L8_S1_HandPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicGlassTaken)
				{
					InitHintTrailEmit(&PZRect,false);
					return;
				}
				break;
			case 6: ++iNonHOHint;
				if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S2_Garden] || !ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L8_S2_Garden])
				{
					InitHintTrailEmit(&SceneRectGarden,true,CA_ExitRight);
					return;
				}
				break;;
			case 7: ++iNonHOHint;
				if(Hud->ShowHintForComboInvItems())
					return;
				break;

			case 8: ++iNonHOHint;
				Hud->teleportSystem->ShowTeleport();
				return;
				break;
		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();
}
void L8_S1_ShukrasPlace::SparkleUpdate()
{
	if (ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		return;

	if (ActivePopUpId != ePopInactive)
		return;

	CheckForPendingTask();

	if (!isSparkleUpdatePending)
		return;

	HiddenObjectSystem::SparkleUpdate();
}

void L8_S1_ShukrasPlace::CheckForPendingTask()
{
	isSparkleUpdatePending = false;
	for (int i = 0; i < (int)PopupArray.size(); i++)
	{
		Popup * popup = PopupArray[i];
		if (popup && isPopupActive(i))
		{
			//if(popup->GetPopupState() == ePopupState_Inactive || popup->GetPopupState() == ePopupState_Closed)
			{
				if(popup->isTaskLeft())
				{
					sparklePointsVec.push_back(popup->GetSparkleUpdatePoint());
					isSparkleUpdatePending = true;
				}
			}
		}

	}


	if (ProfileSystem::Instance->ActiveProfile.L8_S1_DoorKeyPlaced && !ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicGlassTaken)
	{
		sparklePointsVec.push_back(CPoint(PZRect.GetCenterX(), PZRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}
}

//////////////////////FLying DOT//////////////////////

FlyingDot::FlyingDot()
{
	_isClockwise = true;
	_dotObj = NULL;
	_centerPt = NULL;

	_trailsFX = new SuperFX("PRT\\L8\\s1dottrails.INI", PointSystem::CreateCPoint(656,650,0), 4);

	_minHideAngle = 0;
	_maxHideAngle = 360;
}

FlyingDot::~FlyingDot()
{
	_dotObj = NULL;
	SAFE_DELETE(_centerPt);
	SAFE_DELETE(_trailsFX);
}

void FlyingDot::SetProperties(float centerX, float centerY, float radius, float speed, bool isClockwise, float initAngle)
{
	_centerPt = new CPoint(centerX, centerY,0);
	_radius = radius;
	_speed = speed;

	_isClockwise = isClockwise;
	if(isClockwise)
	{
		_minHideAngle = 60;
		_maxHideAngle = 140;
	}
	else
	{
		_minHideAngle = 160;
		_maxHideAngle = 230;
	}

	_trailsFX->Start();
	_angleDeg = initAngle;
}

void FlyingDot::update(float ds)
{
	_angleDeg += ds*_speed;

	if(_angleDeg>360)
		_angleDeg = 360-_angleDeg;

	if(_dotObj!=NULL)
	{
		if(_isClockwise)
		{
			//SLOG::Log("AngDeg=",_angleDeg);
			if(_dotObj->Visible && _dotObj->Status != SPR_FADEOUT)
			{
				if(_angleDeg>_minHideAngle && _angleDeg<_maxHideAngle)
				{
					_dotObj->FadeOut(4.0f);
					_trailsFX->StopEmit();
				}
			}
			else if(!_dotObj->Visible)
			{
				if(_angleDeg>_maxHideAngle)
				{
					_dotObj->FadeIn(4.0f);
					_trailsFX->Start();
				}
			}

			_dotObj->Pos.x = _centerPt->x + _radius*Elixir::Cos(_angleDeg);
			_dotObj->Pos.y = _centerPt->y + _radius*0.85f*Elixir::Sin(_angleDeg+240.0f);
		}
		else
		{
			if(_dotObj->Visible && _dotObj->Status != SPR_FADEOUT)
			{
				if(_angleDeg>_minHideAngle && _angleDeg<_maxHideAngle)
				{
					_dotObj->FadeOut(4.0f);
					_trailsFX->StopEmit();
				}
			}
			else if(!_dotObj->Visible)
			{
				if(_angleDeg>_maxHideAngle)
				{
					_dotObj->FadeIn(4.0f);
					_trailsFX->Start();
				}
			}

			_dotObj->Pos.x = _centerPt->x + _radius*0.85f*Elixir::Cos(_angleDeg+240);
			_dotObj->Pos.y = _centerPt->y + _radius*Elixir::Sin(_angleDeg);
		}

		_trailsFX->SetPos(&_dotObj->Pos);
	}
}

void L8_S1_ShukrasPlace::SkipMagicGlass()
{
	if(ProfileSystem::Instance->ActiveProfile.L8_S1_MatchingHOComplete || !isGlassInHand)
	{
		SkipBtn->SetEnable(false);
		skipBTN_text->SetEnable(false);
		return;
	}
	SkipBtn->SetEnable(true);
	skipBTN_text->SetEnable(true);
	if(SkipBtn->IsIntersect(Control::Input->Mpos()))
	{
		SkipBtn->SetAnimation(1);
		Cursor::SetMode(CA_HandPointy);
		if(Control::Input->LBclicked())
		{
			for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
				ProfileSystem::Instance->ActiveProfile.L8_S1_MatchingSymbolFound[i] = true;
			ProfileSystem::Instance->ActiveProfile.L8_S1_MatchingHOComplete = true;

			ProfileSystem::Instance->ActiveProfile.L8_S1_InvIdolTaken = true;
			ProfileSystem::Instance->ActiveProfile.L8_S1_FloorCracked = true;
			ProfileSystem::Instance->ActiveProfile.L8_MagicSymbolFound[1] = true;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S1_IdolGold] = true;

			bool allSymbolsFound = true;
			for (int i = 0; i < MAX_L8_MAGIC_SYMBOLS; i++)
			{
				if(!ProfileSystem::Instance->ActiveProfile.L8_MagicSymbolFound[i])
				{
					allSymbolsFound = false;
					break;
				}
			}

			if (allSymbolsFound)
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S1_MagicGlass] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_S1_MagicGlass);
			}

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S1_ShukrasPlace;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
	}
	else
	{
		SkipBtn->SetAnimation(0);
		Cursor::SetMode(CA_Normal);
	}
}
void L8_S1_ShukrasPlace::NavigateToScene()
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