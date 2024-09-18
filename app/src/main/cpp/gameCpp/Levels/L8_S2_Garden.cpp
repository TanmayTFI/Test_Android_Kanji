
//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "L8_S2_Garden.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "Util.h"


const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  PopCircleRect(484.0f,154.0f,376.0f,368.0f);

const CRectangle  PoolAreaRect(1074.0f,160.0f,203.0f,147.0f);
const CRectangle  BanyanTreeAreaRect(147.0f,159.0f,270.0f,152.0f);
const CRectangle  ElephantRect(375.0f,97.0f,447.0f,259.0f);


#define X_OFFSET_MASK	170
#define Y_OFFSET_MASK	120


L8_S2_Garden::L8_S2_Garden()
{

	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV8\\S2\\S2.SCN");

	//HOScene->GetObjectByID("fireglowleft")->SetBlendMode(eBlend_AlphaAdditive);
	//HOScene->GetObjectByID("fireglowleft")->Animate(0.15f,0.45f,3.0f);

	//HOScene->GetObjectByID("leftplant")->SetPivot(-10, 43);
	//HOScene->GetObjectByID("leftplant")->Animate(0,0,4,0,8.0f);

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL8s2GardenAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL8s2GardenAmb) )
	{
		Control::Audio->PlaySample(aAmbL8s2GardenAmb,true);
	}

	//MUSIC_SFX

	Fog				= HOScene->GetObjectByID("Fog");
	Fog->SetBlendMode(eBlend_AlphaAdditive);
	Fog->Animate(0.25f,0.99f,23.0f);

	for (int i = 0; i < 3; i++)
	{
		Butterflies[i]	= HOScene->GetObjectByID("Butterfly" + MKSTR(i + 1));
		Butterflies[i]->ForcePlayAnimation(0);
		ButterflyAnimThreshold[i] =  0.5f + rand()%5;
		ButterflyAnimTimer[i] = 0.0f;
	}

	sceneMagicGlass = new CHOScene();
	sceneMagicGlass->Load("LV8\\S_MG\\S_MG.SCN");
	sceneMagicGlass->SetZOff(eZOrder_FoundInvPopup);
	sceneMagicGlass->Show(false);
	isGlassInHand = false;

	InitPopups();

	GateClosed = HOScene->GetObjectByID("GateClose");
	GateOpened = HOScene->GetObjectByID("GateOpen");

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L8_S2_Garden]  = true;
	vidTimer = 0;
	IsVideoPlaying = false;
	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	cvMammothCutscn = new CVideo("OGV\\L8\\S2MAMMOTHINTRO.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	//cvMammothCutscn->SetScale(1.334f);
	cvMammothCutscn->SetScale(GFApp->video_aspectRatio);
	cvMammothCutscn->SetSkipActive();

	if( !ProfileSystem::Instance->ActiveProfile.L8_S2_Visited )
	{
		ProfileSystem::Instance->ActiveProfile.L8_S2_Visited = true;
		//Objective Add
		if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L8O2_FindWayToReachMohenjedaro] )
		{
			ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L8O2_FindWayToReachMohenjedaro] = true;
			ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
					= eObj_L8O2_FindWayToReachMohenjedaro;
			//Increment Objectives
			ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
			Hud->ShowObjectiveFX(eObj_L8O2_FindWayToReachMohenjedaro);

			Control::Audio->PlaySample(aSFXMagic1);
		}


	}

	if(ProfileSystem::Instance->ActiveProfile.L8_S2_IsSugarCaneGivenToElephant && ProfileSystem::Instance->ActiveProfile.L8_S4_InvContainerWithLeafTaken)
		Hud->ShowBannerText("L8perfect");

	for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
	{
		symbols[i] = HOScene->GetObjectByID(MKSTR(i));
		redSymbols[i] = HOScene->GetObjectByID(MKSTR(i) + "_2");
		if (i % 2 == 0)
		{
			silhouttes[i /2] = HOScene->GetObjectByID("sil_" + MKSTR(i / 2));
			silhouttes[i /2]->SetZ(eZOrder_HUD + 1);
		}
	}

	silhouttes[(MAX_L8_S1_MATCHING_SYMBOLS_COUNT / 2)] = HOScene->GetObjectByID("sil_" + MKSTR(MAX_L8_S1_MATCHING_SYMBOLS_COUNT / 2));
	silhouttes[(MAX_L8_S1_MATCHING_SYMBOLS_COUNT / 2)]->SetZ(eZOrder_HUD + 1);

	selectedSymbol = -1;

	SceneObjectsUpdate();

	for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
	{
		symbols[i]->SetEnable(false);
		redSymbols[i]->SetAlpha(0);
#ifdef TOUCH_UI
		symbols[i]->SetScale(1.5f);
		redSymbols[i]->SetScale(1.5f);
		//symbols[i]->SetTint(1,1,0);
#endif
		redSymbols[i]->SetPos(symbols[i]->GetPosPointer());
		silhouttes[i / 2]->SetEnable(false);
	}
	silhouttes[MAX_L8_S1_MATCHING_SYMBOLS_COUNT/2]->SetEnable(false);

	InitAnimations();
	InitControlPolys();
	TaskPendingUpdate();

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

void L8_S2_Garden::InitAnimations()
{
	HOScene->GetObjectByID("TreeBranchAnim01")->SetPivot(0, -56);
	HOScene->GetObjectByID("TreeBranchAnim01")->Animate(0,4,1,0,5.0f);

	HOScene->GetObjectByID("TreeBranchAnim02")->SetPivot(0, -65);
	HOScene->GetObjectByID("TreeBranchAnim02")->Animate(0,4,1,0,9.0f);

	HOScene->GetObjectByID("TreeBranchAnim03")->SetPivot(0, -65);
	HOScene->GetObjectByID("TreeBranchAnim03")->Animate(0,3,1,0,8.0f);

	HOScene->GetObjectByID("LeftTreeBranchAnim")->SetPivot(0, -35);
	HOScene->GetObjectByID("LeftTreeBranchAnim")->Animate(0,2,0,0,7.0f);

	HOScene->GetObjectByID("PlantAnim1")->SetPivot(0, 65);
	HOScene->GetObjectByID("PlantAnim1")->Animate(0,0,5,0,5.0f);

	HOScene->GetObjectByID("PlantAnim2")->SetPivot(0, 73);
	HOScene->GetObjectByID("PlantAnim2")->Animate(0,0,4,0,6.0f);

	HOScene->GetObjectByID("PlantAnim3")->SetPivot(0, 77);
	HOScene->GetObjectByID("PlantAnim3")->Animate(0,0,3,0,7.0f);

	HOScene->GetObjectByID("LightRays")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("LightRays")->Animate(0.55f,0.95f,3.0f);

	HOScene->GetObjectByID("MammothWingUp01")->SetEnable(false);
	HOScene->GetObjectByID("MammothWingUp02")->SetEnable(false);

	S2_FountainFX = new SuperFX("PRT\\L8\\S2_Fountain.INI", PointSystem::CreateCPoint(1087,475,0), 100);
	S2_FountainFX->Start();
	S2_FountainFX->AdvanceByTime(2.0f);


	S2LeafFallFX = new SuperFX("PRT\\L8\\S2LeafFall.INI", PointSystem::CreateCPoint(1167,21,0), 500);
	S2LeafFallFX->Start();
	S2LeafFallFX->AdvanceByTime(2.0f);

	S2LeafFall2FX = new SuperFX("PRT\\L8\\S2LeafFall2.INI", PointSystem::CreateCPoint(267,26,0), 500);
	S2LeafFall2FX->Start();
	S2LeafFall2FX->AdvanceByTime(2.0f);


	s2_MammothAngryFX = new SuperFX("PRT\\L8\\s2_MammothAngry.INI", PointSystem::CreateCPoint(649,263,0), 1100);

	MammothIdleAnim = NULL;
	MammothIdleAnim = new JSONAnimator( 0, CC_CALLBACK_1(L8_S2_Garden::OnAnimationComplete, this ));
	MammothIdleAnim->parseAndLoadJSON("ANIM\\L8\\S2MAMMOTHIDLE.JSON", HOScene );
	MammothIdleAnim->PlayAnim();

	MammothAngryAnim = NULL;
	MammothAngryAnim = new JSONAnimator( 1, CC_CALLBACK_1(L8_S2_Garden::OnAnimationComplete, this ));
	MammothAngryAnim->parseAndLoadJSON("ANIM\\L8\\S2MAMMOTHANGRY.JSON", HOScene );

	MammothEatAnim = NULL;
	MammothEatAnim = new JSONAnimator( 2, CC_CALLBACK_1(L8_S2_Garden::OnAnimationComplete, this ));
	MammothEatAnim->parseAndLoadJSON("ANIM\\L8\\S2_MAMMOTHEAT.JSON", HOScene );

}

void L8_S2_Garden::OnAnimationComplete(int index)
{
	switch (index)
	{
		case 0:		MammothIdleAnim->PlayAnim();
			break;
		case 1:		MammothIdleAnim->PlayAnim();
			break;
		case 2:		MammothIdleAnim->PlayAnim();
			if( ProfileSystem::Instance->ActiveProfile.L8_S4_InvContainerWithLeafTaken)
				Hud->ShowBannerText("L8perfect");
			break;
	}
}

void L8_S2_Garden::StopParticleEmission()
{
	S2_FountainFX->StopImmediate();
	S2LeafFallFX->StopImmediate();
	S2LeafFall2FX->StopImmediate();
	MammothIdleAnim->StopAnim();
}

void L8_S2_Garden::UpdateButterflies()
{
	for (int i = 0; i < 3; i++)
	{
		ButterflyAnimTimer[i] += Control::LogicRate;
		if( Butterflies[i]->IsAnimationFinished())
		{
			Butterflies[i]->StopAnim();
			if( ButterflyAnimTimer[i] >= ButterflyAnimThreshold[i] )
			{
				Butterflies[i]->ForcePlayAnimation(0);
				ButterflyAnimThreshold[i] = 0.5f + rand()%5;
				ButterflyAnimTimer[i] = 0.0f;
			}
		}
	}

}

void L8_S2_Garden::InitControlPolys()
{
	int GatePolyArray[] = {1057,294,1215,303,1221,218,1056,212};
	GatePoly =  new CPolygon(4);
	GatePoly->Init(GatePolyArray);

	int StoneStructureArray[] = {235,350,298,345,305,387,214,395};
	StoneStructurePoly =  new CPolygon(4);
	StoneStructurePoly->Init(StoneStructureArray);


	int SugarCaneArray[] = {742,443,839,447,864,312,737,308};
	SugarCanePoly=  new CPolygon(4);
	SugarCanePoly->Init(SugarCaneArray);
}

void L8_S2_Garden::InitPopups()
{
	ActivePopupID = ePopInactive;

	for (int i = 0; i < ePopL8S2_Max; i++)
	{
		PopupArray[i] = NULL;
		PopStates[i] = ePopupState_Inactive;
		PopPolyArray[i] = NULL;
	}

	int popID=0;

	//Popup 4
	popID = ePop4_L8S2Lotus;
	if(!ProfileSystem::Instance->ActiveProfile.L8_S2_InvSwordHandleTaken )
	{
		int pop4Pts[] = {667,447,784,457,806,490,683,523,551,497,589,447};
		CPolygon* pop4ClickPoly=  new CPolygon(6);
		pop4ClickPoly->Init(pop4Pts);

		Popup* Pop4Scene = new L8S2_Pop4Lotus(popID, Hud, HOScene, eZOrder_Popup, pop4ClickPoly->center, CC_CALLBACK_1(L8_S2_Garden::OnPopupClosed, this));
		Pop4Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[popID] = pop4ClickPoly;
		PopupArray[popID]	= Pop4Scene;

		PopStates[popID] = ePopupState_Closed;
	}


	//Popup 6
	popID = ePop6_L8S2Idol;
	if(!ProfileSystem::Instance->ActiveProfile.L8_S2_InvIdolTaken)
	{
		int pop5Pts[] = {903,445,962,425,1056,438,1042,489,929,482};
		CPolygon* pop6ClickPoly=  new CPolygon(5);
		pop6ClickPoly->Init(pop5Pts);

		Popup* Pop6Scene = new L8S2_Pop6HiddenCompartment(popID, Hud, HOScene, eZOrder_Popup, pop6ClickPoly->center, CC_CALLBACK_1(L8_S2_Garden::OnPopupClosed, this));
		Pop6Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[popID] = pop6ClickPoly;
		PopupArray[popID]	= Pop6Scene;

		if(ProfileSystem::Instance->ActiveProfile.L8_S2_FloorCracked)
			PopStates[popID] = ePopupState_Closed;
	}
}

bool L8_S2_Garden::isPopupActive(int popId)
{
	switch (popId)
	{
		case ePop4_L8S2Lotus:
			if (!isGlassInHand && (!ProfileSystem::Instance->ActiveProfile.L8_S2_InvSwordHandleTaken ))	return true;
			break;
		case ePop6_L8S2Idol:
			if (!ProfileSystem::Instance->ActiveProfile.L8_S2_InvIdolTaken && ((isGlassInHand && ProfileSystem::Instance->ActiveProfile.L8_S2_MatchingHOComplete) || ProfileSystem::Instance->ActiveProfile.L8_MagicSymbolFound[0]))		return true;
			break;
		default:
			break;
	}

	return false;
}

void L8_S2_Garden::OnPopupClosed(Popup* popup)
{
	ActivePopupID = ePopInactive;
	int popID = popup->GetPopupId();
	S2_FountainFX->Start();
	S2LeafFallFX->Start();
	S2LeafFall2FX->Start();
	MammothIdleAnim->PlayAnim();

	if(popID == ePop4_L8S2Lotus)
	{
		if(ProfileSystem::Instance->ActiveProfile.L8_S2_InvSwordHandleTaken)
		{
			isSparkleUpdatePending = true;
			PopStates[popID] = ePopupState_Inactive; //disable popup
		}
	}
	else if(popID == ePop6_L8S2Idol)
	{
		if(ProfileSystem::Instance->ActiveProfile.L8_S2_InvIdolTaken )
		{
			isSparkleUpdatePending = true;
			PopStates[popID] = ePopupState_Inactive; //disable popup
		}
		isGlassInHand = false;
	}
	TaskPendingUpdate();
	SceneObjectsUpdate();
}

void L8_S2_Garden::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if(IsVideoPlaying)
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

void L8_S2_Garden::Update()
{
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

	if( ActivePopupID != ePopInactive )
		return;

	HOScene->update(Control::LogicRate);

	HiddenObjectSystem::HOUpdate();

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);
	NavigateToScene();

	UpdateButterflies();

	if (IsVideoPlaying)
	{
#ifdef FORCE_CLOSE_VIDEO
		vidTimer += Control::LogicRate;
#endif
		Cursor::SetMode(CA_Normal);
		if(
#ifndef FORCE_CLOSE_VID_END
			cvMammothCutscn->isEndReached() ||
#endif
			vidTimer > 40)
		{
			IsVideoPlaying = false;
			cvMammothCutscn->StopAndFadeOutVideo(2.0f);
			Hud->AllowInput = true;

			Hud->ShowHud();

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S5_VolcanoLand;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
		return;
	}

	if( ActivePopupID == ePopInactive )
	{
		//if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		//	SparkleUpdate();

		//popup click checks
		for (int i = 0; i < ePopL8S2_Max; i++)
		{
			if(PopStates[i] != ePopupState_Inactive) //if popup is active
			{
				if(PopPolyArray[i]!=NULL && PopPolyArray[i]->Intersection(&pos) && isPopupActive(i))
				{
					Cursor::SetMode(CA_Look);
					if( Control::Input->LBclicked() )
					{
						Cursor::SetMode(CA_Normal);
						ActivePopupID = i;
						PopupArray[i]->OpenPopup();
						if (isGlassInHand)
						{
							sceneMagicGlass->Show(false);
							HOScene->GetObjectByID("SymbolOnGround")->SetEnable(false);
							for (int j = 0; j < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; j++) //Sac: this was dicleared as i while there is already i in external for loop
							{
								symbols[j]->SetEnable(false);
							}
							silhouttes[MAX_L8_S1_MATCHING_SYMBOLS_COUNT/2]->SetEnable(false);
							Hud->InvBoxCtrl->ResetToInvBox();
							Hud->IsHOScene = false;
						}
					}
					return;
				}
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
				if (!ProfileSystem::Instance->ActiveProfile.L8_S2_MatchingSymbolFound[i] && symbols[i]->GetWorldRect().Intersection(&pos))
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
									ProfileSystem::Instance->ActiveProfile.L8_S2_MatchingSymbolFound[i] = true;
									ProfileSystem::Instance->ActiveProfile.L8_S2_MatchingSymbolFound[selectedSymbol] = true;
									silhouttes[selectedSymbol / 2]->PlayAnimation(1);

									bool isComplete = true;
									for (int j = 0; j < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; j++)  //Sac: this was dicleared as i while there is already i in external for loop
									{
										if (!ProfileSystem::Instance->ActiveProfile.L8_S2_MatchingSymbolFound[j])
										{
											isComplete = false;
										}
									}
									if (isComplete)
									{
										HOScene->GetObjectByID("SymbolOnGround")->SetEnable(true);
										silhouttes[MAX_L8_S1_MATCHING_SYMBOLS_COUNT/2]->SetEnable(true);
										for (int j = 0; j < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; j++)  //Sac: this was dicleared as i while there is already i in external for loop
										{
											silhouttes[j / 2]->SetEnable(false);
										}
										ProfileSystem::Instance->ActiveProfile.L8_S2_MatchingHOComplete = true;
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
									ProfileSystem::Instance->ActiveProfile.L8_S2_MatchingSymbolFound[i] = true;
									ProfileSystem::Instance->ActiveProfile.L8_S2_MatchingSymbolFound[selectedSymbol] = true;
									silhouttes[i / 2]->PlayAnimation(1);

									bool isComplete = true;
									for (int j = 0; j < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; j++)  //Sac: this was dicleared as i while there is already i in external for loop
									{
										if (!ProfileSystem::Instance->ActiveProfile.L8_S2_MatchingSymbolFound[j])
										{
											isComplete = false;
										}
									}
									if (isComplete)
									{
										HOScene->GetObjectByID("SymbolOnGround")->SetEnable(true);
										silhouttes[MAX_L8_S1_MATCHING_SYMBOLS_COUNT/2]->SetEnable(true);
										for (int j = 0; j < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; j++) //Sac: this was dicleared as i while there is already i in external for loop
										{
											silhouttes[j / 2]->SetEnable(false);
										}
										ProfileSystem::Instance->ActiveProfile.L8_S2_MatchingHOComplete = true;
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



			float startX = HOScene->GetObjectByID("SymbolOnGround")->GetPosPointer()->x - HOScene->GetObjectByID("SymbolOnGround")->GetLocalRect().w/2;
			float startY = HOScene->GetObjectByID("SymbolOnGround")->GetPosPointer()->y - HOScene->GetObjectByID("SymbolOnGround")->GetLocalRect().h/2;
#ifndef TOUCH_UI
			CRectangle  TmpRect = new CRectangle((pos.x - X_OFFSET_MASK + 40 > startX)?pos.x - X_OFFSET_MASK + 40:startX,(pos.y - Y_OFFSET_MASK > startY)?pos.y - Y_OFFSET_MASK :startY,pos.x + X_OFFSET_MASK - HOScene->GetObjectByID("SymbolOnGround")->GetPosPointer()->x,pos.y + Y_OFFSET_MASK - HOScene->GetObjectByID("SymbolOnGround")->GetPosPointer()->y);
#else
			CRectangle  TmpRect = new CRectangle((currentPos.x - X_OFFSET_MASK + 40 > startX)?currentPos.x - X_OFFSET_MASK + 40:startX,(currentPos.y - Y_OFFSET_MASK > startY)?currentPos.y - Y_OFFSET_MASK :startY,currentPos.x + X_OFFSET_MASK - HOScene->GetObjectByID("SymbolOnGround")->GetPosPointer()->x,currentPos.y + Y_OFFSET_MASK - HOScene->GetObjectByID("SymbolOnGround")->GetPosPointer()->y);
#endif
			HOScene->GetObjectByID("SymbolOnGround")->SetMaskRect(TmpRect);

			for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
			{
				float startX = HOScene->GetObjectByID(MKSTR(i))->GetPosPointer()->x - HOScene->GetObjectByID(MKSTR(i))->GetLocalRect().w/2;
				float startY = HOScene->GetObjectByID(MKSTR(i))->GetPosPointer()->y - HOScene->GetObjectByID(MKSTR(i))->GetLocalRect().h/2;
#ifndef TOUCH_UI
				CRectangle  TmpRect = new CRectangle((pos.x - X_OFFSET_MASK + 40 > startX)?pos.x - X_OFFSET_MASK + 40:startX,(pos.y - Y_OFFSET_MASK > startY)?pos.y - Y_OFFSET_MASK :startY,pos.x + X_OFFSET_MASK - 30 - HOScene->GetObjectByID(MKSTR(i))->GetPosPointer()->x,pos.y + Y_OFFSET_MASK - HOScene->GetObjectByID(MKSTR(i))->GetPosPointer()->y);
#else
				CRectangle  TmpRect = new CRectangle((currentPos.x - X_OFFSET_MASK + 40 > startX)?currentPos.x - X_OFFSET_MASK + 40:startX,(currentPos.y - Y_OFFSET_MASK > startY)?currentPos.y - Y_OFFSET_MASK :startY,currentPos.x + X_OFFSET_MASK - 30 - HOScene->GetObjectByID(MKSTR(i))->GetPosPointer()->x,currentPos.y + Y_OFFSET_MASK - HOScene->GetObjectByID(MKSTR(i))->GetPosPointer()->y);
#endif
				symbols[i]->SetMaskRect(TmpRect);
				redSymbols[i]->SetMaskRect(TmpRect);
			}
#ifdef ENABLE_ANDROID_BACK_BUTTON
			if (Control::Input->isKeyBackSensed())
			{
				isGlassInHand = false;
				sceneMagicGlass->Show(false);
				HOScene->GetObjectByID("SymbolOnGround")->SetEnable(false);
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
					HOScene->GetObjectByID("SymbolOnGround")->SetEnable(false);
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

		if( PoolAreaRect.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L8_S2_GateOpened && !isGlassInHand )
		{

#ifdef TOUCH_UI
			//	if(!GFHud->InvBoxCtrl->navTapped)
			//		Cursor::SetMode(CA_ExitRight, eArea_L8_S4_Pool);
#else
			Cursor::SetMode(CA_ExitRight, eArea_L8_S4_Pool);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L8_S4_Pool, 1, PoolAreaRect);
				}
#else
				Cursor::SetMode(CA_Normal);
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S4_Pool;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
			return;
		}

		if( BanyanTreeAreaRect.Intersection(&pos) && !isGlassInHand )
		{

#ifdef TOUCH_UI
			//	if(!GFHud->InvBoxCtrl->navTapped)
			//		Cursor::SetMode(CA_ExitLeft,eArea_L8_S3_BanyanTree);
#else
			Cursor::SetMode(CA_ExitLeft,eArea_L8_S3_BanyanTree);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L8_S3_BanyanTree, 3, BanyanTreeAreaRect);
				}
#else
				Cursor::SetMode(CA_Normal);
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S3_BanyanTree;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
			return;
		}


		if( GatePoly->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L8_S2_GateOpened && !isGlassInHand )
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
					GFHud->InvBoxCtrl->GoToScene(eArea_L8_S2_PZGate, 0, CRectangle{1090, 170, 150, 100});
				}
#else
				Cursor::SetMode(CA_Normal);
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S2_PZGate;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
			return;
		}

		if( StoneStructurePoly->Intersection(&pos) && ( !ProfileSystem::Instance->ActiveProfile.L8_S2_InvScrollTaken ||
														!ProfileSystem::Instance->ActiveProfile.L8_S2_InvToolTaken ) && !isGlassInHand )
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
					GFHud->InvBoxCtrl->GoToScene(eArea_L8_S2_PZGears, 0, CRectangle {220,345, 100, 100});//, StoneStructurePoly);
				}
#else
				Cursor::SetMode(CA_Normal);
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S2_PZGears;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
			return;
		}

		if( SugarCanePoly->Intersection(& pos) && !ProfileSystem::Instance->ActiveProfile.L8_S2_InvSugarCaneTaken && !isGlassInHand )
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
					GFHud->InvBoxCtrl->GoToScene(eArea_L8_S2_PZSugarCane, 0, CRectangle{740, 360, 100, 100});//, SugarCanePoly);
				}
#else
				Cursor::SetMode(CA_Normal);
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S2_PZSugarCane;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
			return;
		}

		if( ElephantRect.Intersection(&pos)  )
		{

			if( !ProfileSystem::Instance->ActiveProfile.L8_S2_IsSugarCaneGivenToElephant && !ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown)
			{
				Cursor::SetMode(CA_Gear);
				if( Control::Input->LBclicked() )
				{
					//Journal Entry Add
					if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L8P3_MammothMeet])
					{
						//Flag Set
						ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L8P3_MammothMeet] = true;

						//Add to Note Array
						ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L8P3_MammothMeet;

						ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

						//Increment Notes
						ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
						Hud->ShowJrnEntryFX();
					}

					MammothAngryAnim->PlayAnim();
					HOScene->Shake(4,4,2.0f);
					Control::Audio->QuickLoadAndPlaySFX("s2_mammothAngry");
					s2_MammothAngryFX->Start();
					s2_MammothAngryFX->AdvanceByTime(2.0f);
					Hud->ShowBannerText("L8hello");
				}

			}
			else if( !ProfileSystem::Instance->ActiveProfile.L8_S2_IsSugarCaneGivenToElephant && ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown )
			{
				Cursor::SetMode(CA_Gear);
				if( Control::Input->LBclicked() )
				{
					Hud->ShowBannerText("L8feed");
				}
			}
			else if(ProfileSystem::Instance->ActiveProfile.L8_S2_IsSugarCaneGivenToElephant && !ProfileSystem::Instance->ActiveProfile.L8_S4_InvContainerWithLeafTaken)
			{
				Cursor::SetMode(CA_Gear);
				if( Control::Input->LBclicked() )
				{
					Hud->ShowBannerText("L8hang");
				}
			}
			else if ( ProfileSystem::Instance->ActiveProfile.L8_S2_IsSugarCaneGivenToElephant && ProfileSystem::Instance->ActiveProfile.L8_S4_InvContainerWithLeafTaken)
			{
				Cursor::SetMode(CA_ExitUp, eArea_L8_S5_VolcanoLand);
				if( Control::Input->LBclicked() )
				{
					//Hud->ShowBannerText("L8perfect");
					//ProfileSystem::Instance->ClearInventoryBox();
					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_S4_ContainerWithPurifiedLeaf);
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S4_ContainerWithPurifiedLeaf] = false;
					StopParticleEmission();
					Hud->AllowInput = false;
					Hud->HideHud();
					cvMammothCutscn->PlayFadeInVideo(2.0f);
					IsVideoPlaying = true;

					//Show Objective completed
					if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L8O2_FindWayToReachMohenjedaro] &&
						!ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L8O2_FindWayToReachMohenjedaro] )
					{
						ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L8O2_FindWayToReachMohenjedaro] = true;
						Hud->ShowObjectiveFX(eObj_L8O2_FindWayToReachMohenjedaro,true);
					}

				}
			}
			return;
		}
#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed() && !isGlassInHand)
		{
			Cursor::SetMode(CA_Normal);
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S1_ShukrasPlace;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
#endif
		if( IsMouseOverGoBackArea(&pos))
		{
			if (!isGlassInHand)
			{

#ifdef TOUCH_UI
				//	if(!GFHud->InvBoxCtrl->navTapped)
				//		Cursor::SetMode(CA_ExitDown,eArea_L8_S1_ShukrasPlace);
#else
				Cursor::SetMode(CA_ExitDown,eArea_L8_S1_ShukrasPlace);
#endif

				if( Control::Input->LBclicked() )
				{
#ifdef TOUCH_UI
					if(!GFHud->InvBoxCtrl->navTapped)
					{
						GFHud->InvBoxCtrl->GoToScene(eArea_L8_S1_ShukrasPlace, 2);
					}
#else
					Cursor::SetMode(CA_Normal);
					Control::Audio->PlayFootStepsFX();
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S1_ShukrasPlace;
					SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

				}
			}
			return;
		}

		if( Control::Input->LBclicked() )
		{
			Hud->ShowBannerText("L8flowers");
		}
	}
}

void L8_S2_Garden::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	for (int i = 0; i < ePopL8S2_Max; i++)
	{
		if(PopStates[i] != ePopupState_Inactive) //if popup is active
		{
			if(PopupArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}

	if( !ProfileSystem::Instance->ActiveProfile.L8_S2_InvHalfKeyTaken )
		isAnyTaskLeft = true;

	if( !ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown  && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_CB_MagicWandWithoutKey] )
		isAnyTaskLeft = true;

	if (ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicGlassTaken && !ProfileSystem::Instance->ActiveProfile.L8_S2_FloorCracked)
		isAnyTaskLeft = true;

	if( ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown  && ProfileSystem::Instance->ActiveProfile.L8_S3_InvSwordTaken &&
		!ProfileSystem::Instance->ActiveProfile.L8_S2_InvSugarCaneTaken)
		isAnyTaskLeft = true;

	if( ProfileSystem::Instance->ActiveProfile.L8_S2_InvSugarCaneTaken  && !ProfileSystem::Instance->ActiveProfile.L8_S2_IsSugarCaneGivenToElephant )
		isAnyTaskLeft = true;

	if( !ProfileSystem::Instance->ActiveProfile.L8_S2_GateOpened  && ProfileSystem::Instance->ActiveProfile.L8_S3_InvCrystalKeyTaken )
		isAnyTaskLeft = true;

	if ((!ProfileSystem::Instance->ActiveProfile.L8_S2_InvScrollTaken || !ProfileSystem::Instance->ActiveProfile.L8_S2_InvToolTaken))
		isAnyTaskLeft = true;

	if(( !ProfileSystem::Instance->ActiveProfile.L8_S2_InvScrollTaken || !ProfileSystem::Instance->ActiveProfile.L8_S2_InvToolTaken )
	   && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_CB_FullKey] )
		isAnyTaskLeft = true;

	if( ProfileSystem::Instance->ActiveProfile.L8_S4_InvContainerWithLeafTaken )
		isAnyTaskLeft = true;

	//if(ProfileSystem::Instance->ActiveProfile.L8_S2


	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S2_Garden] = isAnyTaskLeft;

	if( ((ProfileSystem::Instance->ActiveProfile.L8_S3_InvIdolTaken && !ProfileSystem::Instance->ActiveProfile.L8_S4_BronzeIdolPlaced ) ||
		 (ProfileSystem::Instance->ActiveProfile.L8_S2_InvIdolTaken && !ProfileSystem::Instance->ActiveProfile.L8_S4_SilverIdolPlaced ) ||
		 (ProfileSystem::Instance->ActiveProfile.L8_S1_InvIdolTaken && !ProfileSystem::Instance->ActiveProfile.L8_S4_GoldIdolPlaced )) &&
		(ProfileSystem::Instance->ActiveProfile.L8_S2_GateOpened ))
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S4_Pool] = true;

	if( ProfileSystem::Instance->ActiveProfile.L8_S2_InvSwordHandleTaken && !ProfileSystem::Instance->ActiveProfile.L8_S3_SwordHandlePlaced )
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S3_BanyanTree] = true;
}

void L8_S2_Garden::ReceiveMessage(int val)
{

	SceneObjectsUpdate();

	if( val == MAP_NEW_AREA )
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);
	}
	else if( val == MAP_HUD_CLICK )
	{
		Hud->ShowMap();
	}
	else if( val == INVBOX_DROP )
	{
		CPoint dropPos;
		int itemVal;

		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			if (itemVal == eInv_L8_S1_MagicGlass && !ProfileSystem::Instance->ActiveProfile.L8_MagicSymbolFound[0])
			{
				Control::Audio->QuickLoadAndPlaySFX("s2_magicGlassSymbolFound");
				sceneMagicGlass->Show(true);
				isGlassInHand = true;
				currentPos = Control::Input->Mpos();
				//HOScene->GetObjectByID("SymbolOnGround")->SetEnable(true);


				Hud->ReleaseDrag();
				Hud->CloseInventory();

				bool isComplete = true;
				for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
				{
					if (!ProfileSystem::Instance->ActiveProfile.L8_S2_MatchingSymbolFound[i])
					{
						isComplete = false;
						symbols[i]->SetEnable(true);
					}
					else
					{
						silhouttes[i / 2]->PlayAnimation(1);
					}
					if (!ProfileSystem::Instance->ActiveProfile.L8_S2_MatchingHOComplete)
						silhouttes[i / 2]->SetEnable(true);
				}

				if (ProfileSystem::Instance->ActiveProfile.L8_S2_MatchingHOComplete && !ProfileSystem::Instance->ActiveProfile.L8_S2_InvIdolTaken)
				{
					HOScene->GetObjectByID("SymbolOnGround")->SetEnable(true);
					silhouttes[MAX_L8_S1_MATCHING_SYMBOLS_COUNT/2]->SetEnable(true);
				}

				Hud->InvBoxCtrl->SetLockBase(HO_FINDPAIRS_FRAME);
				Hud->IsHOScene = true;
				Hud->HOState = HOActive;
				PopStates[ePop6_L8S2Idol] = ePopupState_Closed;
			}
			else if( itemVal == eInv_L8_S2_Sugarcane && ElephantRect.Intersection(&dropPos)  && !ProfileSystem::Instance->ActiveProfile.L8_S2_IsSugarCaneGivenToElephant )
			{
				MammothEatAnim->PlayAnim();
				Control::Audio->QuickLoadAndPlaySFX("s2p3_giveSugarCaneToElephant");
				ProfileSystem::Instance->ActiveProfile.L8_S2_IsSugarCaneGivenToElephant = true;
				Hud->ReleaseDrag();
				Hud->CloseInventory();
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_S2_Sugarcane);
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S2_Sugarcane] = false;
				Hud->ShowBannerText("L8fill");
				Control::Audio->PlaySample(aVOL8GuruRefS2_1);
			}

			else if(ActivePopupID != ePopInactive)
			{
				if(PopupArray[ActivePopupID]!=NULL)
					PopupArray[ActivePopupID]->HandleInventoryDrop(dropPos, itemVal );
			}
			else
				GFInstance->WrongInvDropMsg(Hud);
		}

	}
	else if( val == INV_CLOSE )
	{


	}
	else if(val == UPDATE_SCENE_OBJECTS)
	{
		SceneObjectsUpdate();
		if(ActivePopupID != ePopInactive)
		{
			if(PopupArray[ActivePopupID]!=NULL)
				PopupArray[ActivePopupID]->UpdateBgGraphic();
		}
	}
}

void L8_S2_Garden::SceneObjectsUpdate()
{
	HOScene->GetObjectByID("MammothNose02")->SetEnable(false);
	HOScene->GetObjectByID("SugarCaneInHand")->SetEnable(false);
	HOScene->GetObjectByID("SugarcaneAnim")->SetEnable(false);

	if( !ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown || ProfileSystem::Instance->ActiveProfile.L8_S2_InvSugarCaneTaken)
		HOScene->GetObjectByID("Sugarcane")->SetEnable(false);
	else
		HOScene->GetObjectByID("Sugarcane")->SetEnable(true);

	if( !ProfileSystem::Instance->ActiveProfile.L8_S2_SmallDoorOpened )
		HOScene->GetObjectByID("PopupDoorOpen")->SetEnable(false);
	else
		HOScene->GetObjectByID("PopupDoorClose")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L8_S2_GateOpened)
	{
		GateClosed->SetEnable(false);
		GateOpened->SetEnable(true);
	}
	else
	{
		GateOpened->SetEnable(false);
		GateClosed->SetEnable(true);
	}

	if (!ProfileSystem::Instance->ActiveProfile.L8_S2_FloorCracked)
		HOScene->GetObjectByID("CrackOnGround")->SetEnable(false);
	else
		HOScene->GetObjectByID("CrackOnGround")->SetEnable(true);

	if ( !isGlassInHand || ProfileSystem::Instance->ActiveProfile.L8_S2_FloorCracked)
		HOScene->GetObjectByID("SymbolOnGround")->SetEnable(false);

}

void L8_S2_Garden::ProcessNonHOHint()
{
	int iNonHOHint = 0;
	int MaxNonHOHints = 20;
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
			case 1:++iNonHOHint;
				if (ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicGlassTaken && !ProfileSystem::Instance->ActiveProfile.L8_S2_FloorCracked && !isGlassInHand)
				{
					//Hud->ShowHintForInvItem(eInv_L8_S1_MagicGlass, &HOScene->GetObjectByID("Bg")->GetWorldRect());
					// Linux: gives error of taking address of temporary also potential crash factor
					// below is fix
					CRectangle ObjRect =HOScene->GetObjectByID("Bg")->GetWorldRect();
					Hud->ShowHintForInvItem(eInv_L8_S1_MagicGlass, &ObjRect);
					return;
				}
				break;
			case 2:++iNonHOHint;
				if (isGlassInHand && !ProfileSystem::Instance->ActiveProfile.L8_S2_MatchingHOComplete)
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
							if (!ProfileSystem::Instance->ActiveProfile.L8_S2_MatchingSymbolFound[i])
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
			case 3: ++iNonHOHint;
				for (int i = 0; i < ePopL8S2_Max; i++)
				{
					if(PopStates[i] != ePopupState_Inactive) //if popup is active
					{
						if(PopupArray[i]->isTaskLeft() && isPopupActive(i))
						{
							InitHintTrailEmit(&PopPolyArray[i]->center,false);
							return;
						}
					}
				}
				break;
			case 4: ++iNonHOHint;
				if( !ProfileSystem::Instance->ActiveProfile.L8_S2_InvHalfKeyTaken )
				{
					InitHintTrailEmit(&SugarCanePoly->center, false);
					return;
				}
				break;
			case 5: ++iNonHOHint;
				if( !ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown  && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_CB_MagicWandWithoutKey] )
				{
					InitHintTrailEmit(&SugarCanePoly->center, false);
					return;
				}
				break;
			case 6: ++iNonHOHint;
				if( ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown  && ProfileSystem::Instance->ActiveProfile.L8_S3_InvSwordTaken &&
					!ProfileSystem::Instance->ActiveProfile.L8_S2_InvSugarCaneTaken)
				{
					InitHintTrailEmit(&SugarCanePoly->center, false);
					return;
				}
				break;
			case 7: ++iNonHOHint;
				if( !ProfileSystem::Instance->ActiveProfile.L8_S2_GateOpened  && ProfileSystem::Instance->ActiveProfile.L8_S3_InvCrystalKeyTaken )
				{
					InitHintTrailEmit(&GatePoly->center, false);
					return;
				}
				break;
			case 8: ++iNonHOHint;
				if(( !ProfileSystem::Instance->ActiveProfile.L8_S2_InvScrollTaken || !ProfileSystem::Instance->ActiveProfile.L8_S2_InvToolTaken )
				   && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_CB_FullKey] )
				{
					InitHintTrailEmit(&StoneStructurePoly->center, false);
					return;
				}
				break;
			case 9:  ++iNonHOHint;
				if( ProfileSystem::Instance->ActiveProfile.L8_S2_InvSugarCaneTaken && !ProfileSystem::Instance->ActiveProfile.L8_S2_IsSugarCaneGivenToElephant )
				{
					Hud->ShowHintForInvItem(eInv_L8_S2_Sugarcane, &ElephantRect);
					return;
				}
				break;
			case 10: ++iNonHOHint;
				if( ProfileSystem::Instance->ActiveProfile.L8_S4_InvContainerWithLeafTaken )
				{
					InitHintTrailEmit(&ElephantRect);
					return;
				}
				break;
			case 11: ++iNonHOHint;
				if (ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S1_ShukrasPlace])
				{
					InitHintTrailEmit(&HoveredBackBtnRectMax, true, CA_ExitDown);
					return;
				}
				break;
			case 12: ++iNonHOHint;
				if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S3_BanyanTree])
				{
					InitHintTrailEmit(&BanyanTreeAreaRect,true,CA_ExitLeft);
					return;
				}
				break;
			case 13: ++iNonHOHint;
				if (ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S4_Pool])
				{
					InitHintTrailEmit(&PoolAreaRect, true, CA_ExitRight);
					return;
				}
				break;
			case 14: ++iNonHOHint;
				if ((!ProfileSystem::Instance->ActiveProfile.L8_S2_InvScrollTaken || !ProfileSystem::Instance->ActiveProfile.L8_S2_InvToolTaken))
				{
					InitHintTrailEmit(&StoneStructurePoly->center, false);
					return;
				}
				break;
			case 15: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
				break;
			case 16: ++iNonHOHint;
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

void L8_S2_Garden::SparkleUpdate()
{
	if (ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		return;

	if (ActivePopupID != ePopInactive)
		return;

	CheckForPendingTask();

	if (!isSparkleUpdatePending)
		return;

	HiddenObjectSystem::SparkleUpdate();
}

void L8_S2_Garden::CheckForPendingTask()
{
	isSparkleUpdatePending = false;
	for (int i = 0; i < ePopL8S2_Max; i++)
	{
		Popup * popup = PopupArray[i];
		if (popup && isPopupActive(i))
		{
			if(popup->GetPopupState() == ePopupState_Inactive || popup->GetPopupState() == ePopupState_Closed)
			{
				if(popup->isTaskLeft())
				{
					sparklePointsVec.push_back(popup->GetSparkleUpdatePoint());
					isSparkleUpdatePending = true;
				}
			}
		}

	}

	if( !ProfileSystem::Instance->ActiveProfile.L8_S2_FloorCracked && isGlassInHand )
	{
		sparklePointsVec.push_back(PopPolyArray[ePop6_L8S2Idol]->center);
		isSparkleUpdatePending = true;
	}

	if( !ProfileSystem::Instance->ActiveProfile.L8_S2_InvHalfKeyTaken )
	{
		sparklePointsVec.push_back(SugarCanePoly->center);
		isSparkleUpdatePending = true;
	}

	if( !ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown  && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_CB_MagicWandWithoutKey] )
	{
		sparklePointsVec.push_back(SugarCanePoly->center);
		isSparkleUpdatePending = true;
	}

	if( ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown  && ProfileSystem::Instance->ActiveProfile.L8_S3_InvSwordTaken &&
		!ProfileSystem::Instance->ActiveProfile.L8_S2_InvSugarCaneTaken)
	{
		sparklePointsVec.push_back(SugarCanePoly->center);
		isSparkleUpdatePending = true;
	}

	if( ProfileSystem::Instance->ActiveProfile.L8_S2_InvSugarCaneTaken  && !ProfileSystem::Instance->ActiveProfile.L8_S2_IsSugarCaneGivenToElephant)
	{
		sparklePointsVec.push_back(CPoint(ElephantRect.GetCenterX(), ElephantRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}

	if( !ProfileSystem::Instance->ActiveProfile.L8_S2_GateOpened  && ProfileSystem::Instance->ActiveProfile.L8_S3_InvCrystalKeyTaken )
	{
		sparklePointsVec.push_back(GatePoly->center);
		isSparkleUpdatePending = true;
	}

	if(( !ProfileSystem::Instance->ActiveProfile.L8_S2_InvScrollTaken || !ProfileSystem::Instance->ActiveProfile.L8_S2_InvToolTaken )
	   && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_CB_FullKey] )
	{
		sparklePointsVec.push_back(StoneStructurePoly->center);
		isSparkleUpdatePending = true;
	}

	if( ProfileSystem::Instance->ActiveProfile.L8_S4_InvContainerWithLeafTaken )
	{
		sparklePointsVec.push_back(CPoint(ElephantRect.GetCenterX(), ElephantRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}

	if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S4_Pool])
	{
		sparklePointsVec.push_back(CPoint(PoolAreaRect.GetCenterX(), PoolAreaRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}

	if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S3_BanyanTree])
	{
		sparklePointsVec.push_back(CPoint(BanyanTreeAreaRect.GetCenterX(), BanyanTreeAreaRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}
}

L8_S2_Garden::~L8_S2_Garden()
{
	Hud->InvBoxCtrl->ResetToInvBox();
	Hud->IsHOScene = false;

	//MUSIC_SFX

	Control::Audio->StopSample(aAmbL8s2GardenAmb);
	Control::Audio->UnloadSample(aAmbL8s2GardenAmb);
	Control::Audio->UnloadSample(aVOL8GuruRefS2_1);
	//MUSIC_SFX
	SAFE_DELETE(MammothAngryAnim);
	SAFE_DELETE(MammothEatAnim);
	SAFE_DELETE(s2_MammothAngryFX);
	SAFE_DELETE(SugarCanePoly);
	SAFE_DELETE(GatePoly);
	SAFE_DELETE(StoneStructurePoly);
	SAFE_DELETE(S2LeafFallFX);
	SAFE_DELETE(S2LeafFall2FX);
	SAFE_DELETE(cvMammothCutscn);
	SAFE_DELETE(S2_FountainFX);
	SAFE_DELETE(sceneMagicGlass);
	SAFE_DELETE(MammothIdleAnim);
}

void L8_S2_Garden::SkipMagicGlass()
{
	if(ProfileSystem::Instance->ActiveProfile.L8_S2_MatchingHOComplete || !isGlassInHand)
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
				ProfileSystem::Instance->ActiveProfile.L8_S2_MatchingSymbolFound[i] = true;
			ProfileSystem::Instance->ActiveProfile.L8_S2_MatchingHOComplete = true;

			ProfileSystem::Instance->ActiveProfile.L8_S2_InvIdolTaken = true;
			ProfileSystem::Instance->ActiveProfile.L8_S2_FloorCracked = true;
			ProfileSystem::Instance->ActiveProfile.L8_MagicSymbolFound[0] = true;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S2_IdolSilver] = true;

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

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S2_Garden;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
	}
	else
	{
		SkipBtn->SetAnimation(0);
		Cursor::SetMode(CA_Normal);
	}
}
void L8_S2_Garden::NavigateToScene()
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