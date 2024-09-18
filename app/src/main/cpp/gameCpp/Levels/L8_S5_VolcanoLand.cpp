//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL6 - SCENE4 : FLOATING ISLAND4
//====================================

#include "L8_S5_VolcanoLand.h"
#include "Util.h"
#include "GameFlow.h"
#include "Hud.h"
#include "MeshObject.h"

const CRectangle  SceneRect(75.0f,32.0f,1244.0f,695.0f);


L8_S5_VolcanoLand::L8_S5_VolcanoLand()
{
	LoadHud(INVENTORY_V1);
	LoadScene("LV8\\S5\\S5.SCN");

	for (int i = 0; i < 6; i++)
	{
		std::string papusId = std::string("PapusAnim_") + MKSTR(i);
		CObject* papus = HOScene->GetObjectByID(papusId);
		papus->SetScale(2);
		papus->SetFrame(rand() % 48);
	}

	InitParticles();
	isVideoPlaying = false;
	/*lpVidIntroCutscene = NULL;*/
	lpVidEndCutscene = NULL;
	time = 0;
	subIndex = 0;
	subMax = 4;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("climax1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("climax2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("climax3"));
	subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("climax4"));
	vidTimer = 0;
	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	lpVidEndCutscene = new CVideo("OGV\\L8\\S5CLIMAX.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	//lpVidEndCutscene->SetScale(1.334f);
	lpVidEndCutscene->SetScale(GFApp->video_aspectRatio);
	//MUSIC_SFX
	musicFadeOutTimer = 0;
		Control::Audio->LoadSample(aAmbL8s5VolcanoLandAmb,AudioAmb);
		if( !Control::Audio->IsPlaying(aAmbL8s5VolcanoLandAmb) )
		{
			Control::Audio->PlaySample(aAmbL8s5VolcanoLandAmb,true);
		}



	lavaParticleFX = new SuperFX("PRT\\L8\\s5LavaParticle.INI", PointSystem::CreateCPoint(716,363,0), 2);
	lavaParticleFX->Start();
	lavaParticleFX->AdvanceByTime(2.0f);

	mary = HOScene->GetObjectByID("Mary");
	mary->Animate(0, 0, 5, 0, 15);

	volcanoSmokeFX = new SuperFX("PRT\\L8\\s5VolcanoSmoke.INI", PointSystem::CreateCPoint(705,366,0), 3);
	volcanoSmokeFX->Start();
	volcanoSmokeFX->AdvanceByTime(2.0f);


	lava = HOScene->GetObjectByID("Lava");
	lava->Animate(0, 1, 4);
	HOScene->GetObjectByID("BgLight")->Animate(0.3f, 1, 3);
	HOScene->GetObjectByID("Hand")->Animate(2, 3, 0, 0, 4);

	boxGlow = HOScene->GetObjectByID("OpeningGlow");
	boxGlow->Animate(2, 3, 0, 0, 4);

	leafArea = NULL;
	int ArrayPts[] = {341,394,651,349,615,536,507,691,330,652,304,462};
	leafArea=  new CPolygon(6);
	leafArea->Init(ArrayPts);

	MaryRect = NULL;
	int ArrayPts1[] = {634,335,570,338,567,352,575,359,614,354,665,350,682,367,685,403,713,428,740,468,751,482,763,471,727,394,756,435,795,463,802,456,761,412,749,367,761,359,746,351,723,338,718,315,775,249,765,238,753,242,711,291,695,270,660,270,638,296,635,328,633,333};
	MaryRect =  new CPolygon(31);
	MaryRect->Init(ArrayPts1);


	boxGlow->SetMouseClickProperties(leafArea, CC_CALLBACK_1(L8_S5_VolcanoLand::OnClickLeafBox, this));
	HOScene->GetObjectByID("WaterMesh")->Show(false);


	//MUSIC_SFX
	Control::Audio->LoadSample(aTrackBonusCredit,AudioTrack,true);

	SceneObjectsUpdate();

	//Journal Entry Add
	if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L8P4_FlownToMohenjedaro])
	{
		//Flag Set
		ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L8P4_FlownToMohenjedaro] = true;

		//Add to Note Array
		ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L8P4_FlownToMohenjedaro;

		ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

		//Increment Notes
		ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
		Hud->ShowJrnEntryFX();
	}
}

void L8_S5_VolcanoLand::SceneObjectsUpdate()
{
	TaskPendingUpdate();

	if (ProfileSystem::Instance->ActiveProfile.L8_S5_EndVideoPlayed)
	{
		HOScene->GetObjectByID("Mary")->SetEnable(false);
		HOScene->GetObjectByID("OpeningGlow")->SetEnable(false);
		HOScene->GetObjectByID("Hand")->SetEnable(false);
	}
}

void L8_S5_VolcanoLand::InitParticles()
{
	/*SnowFX_L = new SuperFX("PRT\\L6\\S2_SnowLeft.INI", PointSystem::CreateCPoint(120,150,0), 19);
	SnowFX_L->Start();
	SnowFX_L->AdvanceByTime(2.0f);

	SnowFX_R = new SuperFX("PRT\\L6\\S2_SnowRight.INI", PointSystem::CreateCPoint(1200,400,0), 19);
	SnowFX_R->Start();
	SnowFX_R->AdvanceByTime(2.0f);

	PlantGlowFX_L = new SuperFX("PRT\\L6\\S2_PlantGlowLeft.INI", PointSystem::CreateCPoint(70,355,0), 19);
	PlantGlowFX_L->Start();
	PlantGlowFX_L->AdvanceByTime(2.0f);

	PlantGlowFX_R = new SuperFX("PRT\\L6\\S2_PlantGlowRight.INI", PointSystem::CreateCPoint(1300,400,0), 19);
	PlantGlowFX_R->Start();
	PlantGlowFX_R->AdvanceByTime(2.0f);*/
}

void L8_S5_VolcanoLand::OnPopupClosed(Popup * popup)
{

}


L8_S5_VolcanoLand::~L8_S5_VolcanoLand()
{
	/*SAFE_DELETE(lpVidIntroCutscene);*/
	SAFE_DELETE(lpVidEndCutscene);
	SAFE_DELETE(subtitleText);
	SAFE_DELETE(volcanoSmokeFX);
	SAFE_DELETE(lavaParticleFX);
	//SAFE_DELETE(MaryRect);
	//SAFE_DELETE(leafArea);
	//MUSIC_SFX

	Control::Audio->StopSample(aAmbL8s5VolcanoLandAmb);
	Control::Audio->UnloadSample(aAmbL8s5VolcanoLandAmb);
	//MUSIC_SFX
}

void L8_S5_VolcanoLand::Required()
{
	HiddenObjectSystem::Required();	//forced update

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

void L8_S5_VolcanoLand::Update()
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
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;
	lava->SetRotate(lava->GetRotate() - 0.01f);

#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
#else
	Cursor::SetMode(CA_Normal);
#endif

	CPoint pos = Control::Input->Mpos();
	if (!ProfileSystem::Instance->ActiveProfile.L8_S5_VirtualMiniGameCompleted && ((boxGlow->GetCurrentMouseState() == eMouseState_Over || boxGlow->GetCurrentMouseState() == eMouseState_Down) && !ProfileSystem::Instance->ActiveProfile.L8_S5_EndVideoPlayed))
	{
		Cursor::SetMode(CA_Look);
	}

	//waterMesh->SetRotate(mary->GetRotate());

	if (!ProfileSystem::Instance->ActiveProfile.L8_S5_Visited)
	{
		ProfileSystem::Instance->ActiveProfile.L8_S5_Visited = true;
		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L8_S1_ShukrasPlace] = true;
		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L8_S2_Garden] = true;
		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L8_S3_BanyanTree] = true;
		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L8_S4_Pool] = true;
	}

	if (isVideoPlaying )
	{
		//Subtitles Code
		time = lpVidEndCutscene->GetTime();

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
		if ((
#ifndef FORCE_CLOSE_VID_END
			lpVidEndCutscene->isEndReached() ||
#endif
			vidTimer > 52) && !ProfileSystem::Instance->ActiveProfile.L8_S5_EndVideoPlayed)
		{
			subtitleText->SetText("");
			ProfileSystem::Instance->ActiveProfile.L8_S5_EndVideoPlayed = true;
			isVideoPlaying = false;
			lpVidEndCutscene->StopAndFadeOutVideo(2.0f);
			Hud->AllowInput = true;
			Hud->ShowHud();
			AwardManager::GetInstance()->CompleteBonusStory();
			Hud->SendUpdateSceneMessage();

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L8P5_MaryResurrect])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L8P5_MaryResurrect] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L8P5_MaryResurrect;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
			}

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L8P6_GrandReunion])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L8P6_GrandReunion] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L8P6_GrandReunion;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
			}

			//Clearing all the Level 8 Objectives
			ProfileSystem::Instance->ClearAllCurrentObjectives();ProfileSystem::Instance->ClearInventoryBox();

			//SendMessage(ControlExit, ID, HOSM_ReturnToMenu);
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_Credits;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
	}
	else if (ProfileSystem::Instance->ActiveProfile.L8_S5_VirtualMiniGameCompleted && !isVideoPlaying && !ProfileSystem::Instance->ActiveProfile.L8_S5_EndVideoPlayed)
	{

		/*ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L8_S1_ShukrasPlace] = false;
		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L8_S2_Garden] = false;
		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L8_S3_BanyanTree] = false;
		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L8_S4_Pool] = false;*/
		Hud->AllowInput = false;
		Hud->HideHud();
		isVideoPlaying = true;
		lpVidEndCutscene->PlayVideo();

		Control::Audio->PlaySample(aTrackBonusCredit,true);
	}
	else if (MaryRect->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L8_S5_EndVideoPlayed)
	{
		Cursor::SetMode(CA_Help);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L8sacrifice");
		}
	}
}

void L8_S5_VolcanoLand::Render(const CPoint* pCam)
{

}

void L8_S5_VolcanoLand::ReceiveMessage(int val)
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
			if (itemVal == eInv_L8_S4_ContainerWithPurifiedLeaf)
			{
				if (!ProfileSystem::Instance->ActiveProfile.L8_S5_VirtualMiniGameCompleted /*&& ProfileSystem::Instance->ActiveProfile.L8_S5_IntroVideoShown*/)
				{
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S5_VirtualMiniGame;
					SendMessage(ControlExit, ID, HOSM_NewArea);
				}
			}
			//If no drop items in this scene !!

			GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		


	}	
}

float L8_S5_VolcanoLand::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}


void L8_S5_VolcanoLand::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;
	if (!ProfileSystem::Instance->ActiveProfile.L8_S5_VirtualMiniGameCompleted/* && ProfileSystem::Instance->ActiveProfile.L8_S5_IntroVideoShown*/)
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S5_VolcanoLand] = isAnyTaskLeft;
}

void L8_S5_VolcanoLand::ProcessNonHOHint()
{
	int iNonHOHint = 0; 
	int MaxNonHOHints = 10;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
			break;
		case 0: ++iNonHOHint;
			if(!ProfileSystem::Instance->ActiveProfile.L8_S5_VirtualMiniGameCompleted /*&& ProfileSystem::Instance->ActiveProfile.L8_S5_IntroVideoShown*/ )
			{
				//Hud->InitHintTrailEmit(&boxGlow->GetWorldRect());
				// Linux: gives error of taking address of temporary also potential crash factor
				// below is fix
				CRectangle boxGlowObjRect =boxGlow->GetWorldRect();
				Hud->InitHintTrailEmit(&boxGlowObjRect);
				return;
			}
			break;
		case 1: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
		case 2: ++iNonHOHint;
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
void L8_S5_VolcanoLand::SparkleUpdate()
{
	if (ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		return;


	CheckForPendingTask();

	if (!isSparkleUpdatePending)
		return;

	HiddenObjectSystem::SparkleUpdate();
}

void L8_S5_VolcanoLand::CheckForPendingTask()
{
	isSparkleUpdatePending = false;
	if(!ProfileSystem::Instance->ActiveProfile.L8_S5_VirtualMiniGameCompleted /*&& ProfileSystem::Instance->ActiveProfile.L8_S5_IntroVideoShown*/ )
	{
		sparklePointsVec.push_back(CPoint(SceneRect.GetCenterX(), SceneRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;

	}
}

void L8_S5_VolcanoLand::OnClickLeafBox(CObject * obj)
{
	if (!ProfileSystem::Instance->ActiveProfile.L8_S5_VirtualMiniGameCompleted /*&& ProfileSystem::Instance->ActiveProfile.L8_S5_IntroVideoShown*/)
	{
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S5_VirtualMiniGame;
		SendMessage(ControlExit, ID, HOSM_NewArea);
	}
}

