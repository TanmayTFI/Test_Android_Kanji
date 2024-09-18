//====================================
//	DQFYLH
//	HO System
//	Level 1 - Scene 01 :  KIDS PAINTING
//====================================

#include "L1_S1_PZKidsPainting.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "TextureFactory.h"
#include "Rect.h"
#include "Polygon.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "Fonts.h"
#include "AwardManager.h"

//Scene Rects

// Action Rects
const CRectangle  ActionRectMom(812.0f, 254.0f, 137.0f, 330.0f);
const CRectangle  ActionRectMe(691.0f, 361.0f, 113.0f, 207.0f);
const CRectangle  ActionRectMoon(505.0f, 89.0f, 76.0f, 66.0f);
const CRectangle  ActionRectStar1(325.0f, 150.0f, 63.0f, 61.0f);
const CRectangle  ActionRectStar2(398.0f, 106.0f, 69.0f, 78.0f);
const CRectangle  ActionRectStar3(595.0f, 100.0f, 59.0f, 64.0f);
const CRectangle  ActionRectStar4(838.0f, 94.0f, 58.0f, 51.0f);
const CRectangle  ActionRectStar5(875.0f, 154.0f, 55.0f, 54.0f);
const CRectangle  ActionRectStar6(969.0f, 114.0f, 64.0f, 75.0f);
const CRectangle  ActionRectCastle(606.0f, 170.0f, 195.0f, 183.0f);
const CRectangle  ActionRectKing(425.0f, 202.0f, 77.0f, 138.0f);
const CRectangle  ActionRectBuffalo(366.0f, 293.0f, 227.0f, 212.0f);
const CRectangle  ActionRectFlower1(593.0f, 435.0f, 76.0f, 81.0f);
const CRectangle  ActionRectFlower2(962.0f, 425.0f, 78.0f, 78.0f);

CPoint mamaPos;
CPoint KingPos;
CPoint mePos;
CPoint buffaloPos;
CPoint CastlePos;

//Video Rects

L1_S1_PZKidsPainting::L1_S1_PZKidsPainting()
{

	IsEndAnimPlaying = false;
	IsCutScenePlaying = false;
	endAnimTimer = 0;
	IsMiniGameSkiped = false;

	tempObj = NULL;
	tempTxtObj = NULL;

	IsObjFading = false;
	musicFadeOutTimer = 0;

	IsVidSkippable = true;
	IsSkipMouseOver = false;
	IsSkipMouseClicked = false;
	SkipShowTimer = 0;

	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L1\\S1\\KIDSPAINTING\\P2.SCN");

	HOScene->SetZOff(10);

	Hud->InvBox_StartPuzzleMode();

	mom = HOScene->GetObjectByID("mom");
	me = HOScene->GetObjectByID("me");
	king = HOScene->GetObjectByID("king");
	buffalo = HOScene->GetObjectByID("buffalo");
	castle = HOScene->GetObjectByID("castle");
	stars = HOScene->GetObjectByID("stars");
	moon = HOScene->GetObjectByID("moon");
	flower1 = HOScene->GetObjectByID("flower1");
	flower2 = HOScene->GetObjectByID("flower2");

	momred = HOScene->GetObjectByID("momred");
	mered = HOScene->GetObjectByID("mered");
	kingred = HOScene->GetObjectByID("kingred");
	buffalored = HOScene->GetObjectByID("buffalored");
	castlered = HOScene->GetObjectByID("castlered");
	starsred = HOScene->GetObjectByID("starsred");
	moonred = HOScene->GetObjectByID("moonred");
	flower1red = HOScene->GetObjectByID("flower1red");
	flower2red = HOScene->GetObjectByID("flower2red");

	momyellow = HOScene->GetObjectByID("momyellow");
	meyellow = HOScene->GetObjectByID("meyellow");
	kingyellow = HOScene->GetObjectByID("kingyellow");
	buffaloyellow = HOScene->GetObjectByID("buffaloyellow");
	castleyellow = HOScene->GetObjectByID("castleyellow");
	starsyellow = HOScene->GetObjectByID("starsyellow");
	moonyellow = HOScene->GetObjectByID("moonyellow");
	flower1yellow = HOScene->GetObjectByID("flower1yellow");
	flower2yellow = HOScene->GetObjectByID("flower2yellow");

	txtMama = HOScene->GetObjectByID("txtmama");
	mamaPos = txtMama->Pos;
	txtMoon = HOScene->GetObjectByID("txtmoon");
	txtKing = HOScene->GetObjectByID("txtking");
	KingPos = txtKing->Pos;
	txtMe = HOScene->GetObjectByID("txtme");
	mePos = txtMe->Pos;
	txtBuffalo = HOScene->GetObjectByID("txtbuffalo");
	buffaloPos = txtBuffalo->Pos;
	txtStars = HOScene->GetObjectByID("txtstars");
	txtFlower = HOScene->GetObjectByID("txtflower");
	txtCastle = HOScene->GetObjectByID("txtcastle");
	CastlePos = txtCastle->Pos;

	momred->SetEnable(false);
	mered->SetEnable(false);
	kingred->SetEnable(false);
	buffalored->SetEnable(false);
	castlered->SetEnable(false);
	starsred->SetEnable(false);
	moonred->SetEnable(false);
	flower1red->SetEnable(false);
	flower2red->SetEnable(false);

	momyellow->SetEnable(false);
	meyellow->SetEnable(false);
	kingyellow->SetEnable(false);
	buffaloyellow->SetEnable(false);
	castleyellow->SetEnable(false);
	starsyellow->SetEnable(false);
	moonyellow->SetEnable(false);
	flower1yellow->SetEnable(false);
	flower2yellow->SetEnable(false);

	txtMama->SetEnable(false);
	txtMoon->SetEnable(false);
	txtKing->SetEnable(false);
	txtMe->SetEnable(false);
	txtBuffalo->SetEnable(false);
	txtStars->SetEnable(false);
	txtFlower->SetEnable(false);
	txtCastle->SetEnable(false);

	FgBgMesh = NULL;

	FgBgMesh = new MeshObject("LV1\\S1\\P2\\roombg.jpa", 20, 20);
	FgBgMesh->SetPosAndParams(0, 0, 3, 2, 5, 2);
	FgBgMesh->SetZ(0);

	HOScene->InsertObject(FgBgMesh, 0);

	if (ProfileSystem::Instance->ActiveProfile.L1S1_P2BuffaloClicked)
	{
		buffaloyellow->SetEnable(true);
		txtBuffalo->SetEnable(true);
	}

	if (ProfileSystem::Instance->ActiveProfile.L1S1_P2CastleClicked)
	{
		castleyellow->SetEnable(true);
		txtCastle->SetEnable(true);
	}

	if (ProfileSystem::Instance->ActiveProfile.L1S1_P2MeClicked)
	{
		meyellow->SetEnable(true);
		txtMe->SetEnable(true);
	}

	if (ProfileSystem::Instance->ActiveProfile.L1S1_P2MamaClicked)
	{
		momyellow->SetEnable(true);
		txtMama->SetEnable(true);
	}

	if (ProfileSystem::Instance->ActiveProfile.L1S1_P2KingClicked)
	{
		kingyellow->SetEnable(true);
		txtKing->SetEnable(true);
	}

	//HOScene->GetObjectByID("whitemask")->Show(false);

	endGameVFX = new SuperFX("PRT\\L1\\paintendfx.INI", PointSystem::CreateCPoint(676, 349, 0), 100);

	//Add Visible = true; to see anything
	SetZ(eZOrder_Popup + 50);
	vidTimer = 0;

	time = 0;
	subIndex = 0;
	subMax = 2;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(30000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("kidspainting1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("kidspainting2"));

	cv_Cutscn = NULL;
	skipObj = NULL;

	//CRectangle RectVidFull(171, 96, 1024, 576);
	CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY, GFApp->video_width, GFApp->video_height);
	cv_Cutscn = new CVideo("OGV\\L1S1\\KIDSPAINTING.OGV", RectVidFull, false, eZOrder_CutsceneFull);
	//cv_Cutscn->SetScale(1.334f);
	cv_Cutscn->SetScale(GFApp->video_aspectRatio);

	skipObj = new CObject();
	skipObj->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\CNM\\SKIPBTN.SPR", (SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX <= 0) ? GFApp->ProjX : 0)), SKIPIMAGE_Y, eZOrder_CutsceneFull + 10);
	skipObj->Show(false);

	//Tutorial show
	if (Hud->Tutor != NULL && Hud->Tutor->IsActive && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly)
	{
		//puzzle is activated only when fuses are fixed
		if (!ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT16_SkipPuzzle])
			Hud->Tutor->ShowTutorial(TUT16_SkipPuzzle);
	}

	//MUSIC_SFX
	Control::Audio->ClearUnusedTracks();

	if (Control::Audio->IsPlaying(aTrackL1Mansion) ||
		Control::Audio->IsPlaying(aTrackL1Broken))
		ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;

	Control::Audio->LoadSample(aTrackL1KidsPuzzleBGM, AudioTrack, true);
	Control::Audio->LoadSample(aTrackCutscnS1KidsPainting, AudioTrack);

	//MUSIC_SFX
}

void L1_S1_PZKidsPainting::Required()
{
	HiddenObjectSystem::Required();	//forced update
	if (Control::Audio->IsPlaying(aTrackL1Broken))
	{
		Control::Audio->Samples[aTrackL1Broken]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if ((Control::Audio->GlobalMusicVol - musicFadeOutTimer) <= 0)
		{
			Control::Audio->Samples[aTrackL1Broken]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL1Broken);
			musicFadeOutTimer = 0;
			if (!Control::Audio->IsPlaying(aTrackL1KidsPuzzleBGM))
				Control::Audio->PlaySample(aTrackL1KidsPuzzleBGM, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else if (Control::Audio->IsPlaying(aTrackL1Mansion))
	{
		Control::Audio->Samples[aTrackL1Mansion]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if ((Control::Audio->GlobalMusicVol - musicFadeOutTimer) <= 0)
		{
			Control::Audio->Samples[aTrackL1Mansion]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL1Mansion);
			musicFadeOutTimer = 0;
			if (!Control::Audio->IsPlaying(aTrackL1KidsPuzzleBGM))
				Control::Audio->PlaySample(aTrackL1KidsPuzzleBGM, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if (!Control::Audio->IsPlaying(aTrackL1KidsPuzzleBGM) && !IsCutScenePlaying)
			Control::Audio->PlaySample(aTrackL1KidsPuzzleBGM, true);
	}

	if (IsEndAnimPlaying)
	{
		if (IsCutScenePlaying)
		{
			CPoint pos = Control::Input->Mpos();
			Cursor::SetMode(CA_Normal);

			//Subtitles Code

			time = cv_Cutscn->GetTime();

			if (time >= subtitles[subIndex].begin && time < subtitles[subIndex].end)
			{
				subtitleText->SetText(subtitles[subIndex].text);
			}
			else
				subtitleText->SetText("");

			if (time > subtitles[subIndex].end)
			{
				if(subMax-1 > subIndex)
					subIndex++;
			}

			if (IsVidSkippable)
			{
				if (SkipShowTimer > 2.0f)
				{
					if (!skipObj->Visible)
					{
						skipObj->Show(true);
						//HOScene->GetObjectByID("whitemask")->Show(false);
					}

					CPoint mousePos = Control::Input->Mpos();
					IsSkipMouseOver = (mousePos.x >= (SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX <= 0) ? GFApp->ProjX : 0)) &&
						mousePos.y <= (SKIPIMAGE_Y + SKIPIMAGE_HEIGHT) &&
						mousePos.y >= SKIPIMAGE_Y &&
						mousePos.x <= ((SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX <= 0) ? GFApp->ProjX : 0)) + SKIPIMAGE_WIDTH));

					if (IsSkipMouseOver && !IsSkipMouseClicked)
					{
						Cursor::SetMode(CA_HandPointy);
						skipObj->PlayAnimation(1);
						IsSkipMouseClicked = Control::Input->LBclicked();
						if (IsSkipMouseClicked)
						{
							Control::Audio->PlaySample(aSFXClick);
							Control::Input->ClearMouse();
							AwardManager::GetInstance()->SkipDialogueOrCutScene();
						}
					}
					else //Cursor not over skip region
					{
						skipObj->PlayAnimation(0);
					}
					vidTimer += Control::LogicRate;

					if (
						#ifndef FORCE_CLOSE_VID_END
						cv_Cutscn->isEndReached() || 
						#endif
						IsSkipMouseClicked || vidTimer > 7)
					{
						subtitleText->SetText("");
						cv_Cutscn->StopAndFadeOutVideo(2.0f);
						IsCutScenePlaying = false;
						IsEndAnimPlaying = false;

						Hud->AllowInput = true;
						Hud->ShowHud();

						skipObj->Show(false);

						if (Hud->IsPuzzleScene)
							Hud->InvBox_StopPuzzleMode();

						Control::Audio->StopSample(aTrackCutscnS1KidsPainting);

						//return to nostalgic world!!!
						if (!Control::Audio->IsPlaying(aTrackL1Broken) &&
							!Control::Audio->IsPlaying(aTrackL1Mansion))
							Control::Audio->PlaySample(aTrackL1Mansion, false);

						ProfileSystem::Instance->ActiveProfile.L1S1_P2IsPuzzleComplete = true;
						AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);

						ProfileSystem::Instance->ActiveProfile.L1S1_KidsPaintingPzBagActive = true;

						ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;

						ProfileSystem::Instance->ActiveProfile.CurrentArea = ProfileSystem::Instance->ActiveProfile.PrevComboActiveArea;
						SendMessage(ControlExit, ID, HOSM_NewArea);
						return;
					}
				}
				else
					SkipShowTimer += 10 * Control::LogicRate;
			}
			return;
		}
		else
		{

			endAnimTimer += Control::LogicRate;
			//HOScene->GetObjectByID("whitemask")->SetAlpha(endAnimTimer*0.33);
			if (endAnimTimer > 1.0f)
			{
				mom->SetEnable(false);
				me->SetEnable(false);
				king->SetEnable(false);
				buffalo->SetEnable(false);
				castle->SetEnable(false);
				stars->SetEnable(false);
				moon->SetEnable(false);
				flower1->SetEnable(false);
				flower2->SetEnable(false);

				momred->SetEnable(false);
				mered->SetEnable(false);
				kingred->SetEnable(false);
				buffalored->SetEnable(false);
				castlered->SetEnable(false);
				starsred->SetEnable(false);
				moonred->SetEnable(false);
				flower1red->SetEnable(false);
				flower2red->SetEnable(false);

				momyellow->SetEnable(false);
				meyellow->SetEnable(false);
				kingyellow->SetEnable(false);
				buffaloyellow->SetEnable(false);
				castleyellow->SetEnable(false);
				starsyellow->SetEnable(false);
				moonyellow->SetEnable(false);
				flower1yellow->SetEnable(false);
				flower2yellow->SetEnable(false);

				txtMama->SetEnable(false);
				txtMoon->SetEnable(false);
				txtKing->SetEnable(false);
				txtMe->SetEnable(false);
				txtBuffalo->SetEnable(false);
				txtStars->SetEnable(false);
				txtFlower->SetEnable(false);
				txtCastle->SetEnable(false);
				HOScene->GetObjectByID("txtoneday")->SetEnable(false);

				HOScene->GetObjectByID("bg")->SetScale(endAnimTimer);

				if (endAnimTimer > 2.0f)
				{

					Hud->AllowInput = false;
					Hud->HideHud();
					cv_Cutscn->PlayVideo();

					if (Control::Audio->IsPlaying(aTrackL1KidsPuzzleBGM))
						Control::Audio->StopSample(aTrackL1KidsPuzzleBGM);

					//Control::Audio->PlaySample(aTrackCutscnS1KidsPainting,false);

					IsCutScenePlaying = true;
				}
			}

		}


	}
	if (Hud->PopupState != Popup_Inactive)
		return;
}

void L1_S1_PZKidsPainting::DisableItems()
{
	tempObj->SetAlpha(1.0f);
	tempTxtObj->SetAlpha(1.0f);

	momred->SetEnable(false);
	mered->SetEnable(false);
	kingred->SetEnable(false);
	buffalored->SetEnable(false);
	castlered->SetEnable(false);
	starsred->SetEnable(false);
	moonred->SetEnable(false);
	flower1red->SetEnable(false);
	flower2red->SetEnable(false);

	momyellow->SetEnable(false);
	meyellow->SetEnable(false);
	kingyellow->SetEnable(false);
	buffaloyellow->SetEnable(false);
	castleyellow->SetEnable(false);
	starsyellow->SetEnable(false);
	moonyellow->SetEnable(false);
	flower1yellow->SetEnable(false);
	flower2yellow->SetEnable(false);

	txtMama->SetEnable(false);
	txtMoon->SetEnable(false);
	txtKing->SetEnable(false);
	txtMe->SetEnable(false);
	txtBuffalo->SetEnable(false);
	txtStars->SetEnable(false);
	txtFlower->SetEnable(false);
	txtCastle->SetEnable(false);

	ProfileSystem::Instance->ActiveProfile.L1S1_P2BuffaloClicked = false;
	ProfileSystem::Instance->ActiveProfile.L1S1_P2CastleClicked = false;
	ProfileSystem::Instance->ActiveProfile.L1S1_P2MeClicked = false;
	ProfileSystem::Instance->ActiveProfile.L1S1_P2MamaClicked = false;
	ProfileSystem::Instance->ActiveProfile.L1S1_P2KingClicked = false;

	tempObj = NULL;
	tempTxtObj = NULL;
}

void L1_S1_PZKidsPainting::ResetItems(CObject* Obj, CObject* txtObj)
{
	if (!ProfileSystem::Instance->ActiveProfile.L1S1_P2BuffaloClicked)
	{
		txtObj->SetPos(&buffaloPos);
		txtObj->SetEnable(true);
		//txtObj->FadeOut();
		Obj->SetEnable(true);
		//Obj->FadeOut();
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L1S1_P2CastleClicked)
	{
		txtObj->SetPos(&CastlePos);
		txtObj->SetEnable(true);
		//txtObj->FadeOut();
		Obj->SetEnable(true);
		//Obj->FadeOut();
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L1S1_P2KingClicked)
	{
		txtObj->SetPos(&KingPos);
		txtObj->SetEnable(true);
		//txtObj->FadeOut();
		Obj->SetEnable(true);
		//Obj->FadeOut();
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L1S1_P2MeClicked)
	{
		txtObj->SetPos(&mePos);
		txtObj->SetEnable(true);
		//txtObj->FadeOut();
		Obj->SetEnable(true);
		//Obj->FadeOut();
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L1S1_P2MamaClicked)
	{
		txtObj->SetPos(&mamaPos);
		txtObj->SetEnable(true);
		//txtObj->FadeOut();
		Obj->SetEnable(true);
		//Obj->FadeOut();
	}

	tempObj = Obj;
	tempTxtObj = txtObj;
	IsObjFading = true;

	Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1KidsPaintingWrongClick);
}

void L1_S1_PZKidsPainting::Update()
{	
	NavigateToScene();
	if (Hud->PopupState != Popup_Inactive)
		return;

	if (Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem || GFInstance->IsActivePopup)
		return;

	if (Hud->Tutor != NULL && Hud->Tutor->IsActive)
	{
		if (Hud->Tutor->State == TUT16_SkipPuzzle && Hud->Tutor->IsVisible)
		{
			return;
		}
	}

	if (IsObjFading)
	{
		if (tempObj != NULL)
		{
			if (tempObj->pSprite->Alpha > 0)
			{
				tempObj->SetAlpha(tempObj->pSprite->Alpha - 0.01f);
				tempTxtObj->SetAlpha(tempTxtObj->pSprite->Alpha - 0.01f);
			}
			else
			{
				IsObjFading = false;
				DisableItems();
			}

		}
		return;
	}

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	if (IsCutScenePlaying && IsEndAnimPlaying)
		return;

	if (ActionRectMom.Intersection(&pos))
	{
		if (Control::Input->LBclicked())
		{
			if (ProfileSystem::Instance->ActiveProfile.L1S1_P2MeClicked)
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1KidsPaintingRightClick);
				momyellow->SetEnable(true);

				txtMama->SetPos(&mamaPos);
				txtMama->SetEnable(true);

				ProfileSystem::Instance->ActiveProfile.L1S1_P2MamaClicked = true;
				if (!IsEndAnimPlaying && !ProfileSystem::Instance->ActiveProfile.L1S1_P2IsPuzzleComplete)
				{
					Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1KidsPaintingComplete);
					if (_handleExtrasGameComplete())
						return;
					IsEndAnimPlaying = true;
					endGameVFX->Start();

					/*HOScene->GetObjectByID("whitemask")->SetAlpha(0);
					HOScene->GetObjectByID("whitemask")->Show(true);*/
				}

			}
			else
			{
				ResetItems(momred, txtMama);
			}
		}
	}
	else if (ActionRectMoon.Intersection(&pos))
	{
		if (Control::Input->LBclicked())
		{
			ResetItems(moonred, txtMoon);
		}
	}
	else if (ActionRectKing.Intersection(&pos))
	{
		if (Control::Input->LBclicked())
		{
			if (ProfileSystem::Instance->ActiveProfile.L1S1_P2CastleClicked)
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1KidsPaintingRightClick);

				kingyellow->SetEnable(true);
				txtKing->SetPos(&KingPos);
				txtKing->SetEnable(true);
				ProfileSystem::Instance->ActiveProfile.L1S1_P2KingClicked = true;
			}
			else
			{
				ResetItems(kingred, txtKing);
			}
		}
	}
	else if (ActionRectMe.Intersection(&pos))
	{
		if (Control::Input->LBclicked())
		{
			if (ProfileSystem::Instance->ActiveProfile.L1S1_P2KingClicked)
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1KidsPaintingRightClick);

				meyellow->SetEnable(true);
				txtMe->SetPos(&mePos);
				txtMe->SetEnable(true);
				ProfileSystem::Instance->ActiveProfile.L1S1_P2MeClicked = true;
			}
			else
			{
				ResetItems(mered, txtMe);
			}
		}
	}
	else if (ActionRectBuffalo.Intersection(&pos))
	{
		if (Control::Input->LBclicked())
		{
			if (!ProfileSystem::Instance->ActiveProfile.L1S1_P2BuffaloClicked)
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1KidsPaintingRightClick);

				buffaloyellow->SetEnable(true);
				txtBuffalo->SetPos(&buffaloPos);
				txtBuffalo->SetEnable(true);
				ProfileSystem::Instance->ActiveProfile.L1S1_P2BuffaloClicked = true;
			}
			else
			{
				ResetItems(buffalored, txtBuffalo);
			}
		}
	}
	else if (ActionRectCastle.Intersection(&pos))
	{
		if (Control::Input->LBclicked())
		{
			if (ProfileSystem::Instance->ActiveProfile.L1S1_P2BuffaloClicked)
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1KidsPaintingRightClick);

				castleyellow->SetEnable(true);
				txtCastle->SetPos(&CastlePos);
				txtCastle->SetEnable(true);
				ProfileSystem::Instance->ActiveProfile.L1S1_P2CastleClicked = true;
			}
			else
			{
				ResetItems(castlered, txtCastle);
			}
		}
	}
	else if (ActionRectFlower1.Intersection(&pos))
	{
		if (Control::Input->LBclicked())
		{
			ResetItems(flower1red, txtFlower);
		}
	}
	else if (ActionRectFlower2.Intersection(&pos))
	{
		if (Control::Input->LBclicked())
		{
			ResetItems(flower2red, txtFlower);
		}
	}
	else if (ActionRectStar1.Intersection(&pos) || ActionRectStar2.Intersection(&pos) || ActionRectStar3.Intersection(&pos) || ActionRectStar4.Intersection(&pos) || ActionRectStar5.Intersection(&pos) || ActionRectStar6.Intersection(&pos))
	{
		if (Control::Input->LBclicked())
		{
			ResetItems(starsred, txtStars);
		}
	}


#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed()) {
		ProfileSystem::Instance->ActiveProfile.L1S1_KidsPaintingPzBagActive = true;
		Hud->InvBox_StopPuzzleMode();
		//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = ProfileSystem::Instance->ActiveProfile.PrevComboActiveArea;
		SendMessage(ControlExit, ID, HOSM_NewArea);
	}
#endif

	if (IsMouseOverGoBackArea(&pos))
	{
#ifdef TOUCH_UI
		//if (!GFHud->InvBoxCtrl->navTapped)
			//Cursor::SetMode(CA_ExitDown, ProfileSystem::Instance->ActiveProfile.PrevComboActiveArea);
#else
		Cursor::SetMode(CA_ExitDown, ProfileSystem::Instance->ActiveProfile.PrevComboActiveArea);
#endif

		if (Control::Input->LBclicked())
		{
#ifdef TOUCH_UI
			if (!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(ProfileSystem::Instance->ActiveProfile.PrevComboActiveArea, 2);
			}
#else
			ProfileSystem::Instance->ActiveProfile.L1S1_KidsPaintingPzBagActive = true;

			Hud->InvBox_StopPuzzleMode();
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = ProfileSystem::Instance->ActiveProfile.PrevComboActiveArea;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}
	}
}

float L1_S1_PZKidsPainting::easing(float t, float b, float c, float d)
{
	return (c * t / d + b);
}
void L1_S1_PZKidsPainting::ProcessSkip()
{
	if (!IsEndAnimPlaying && !ProfileSystem::Instance->ActiveProfile.L1S1_P2IsPuzzleComplete)
	{
		Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1KidsPaintingComplete);

		IsEndAnimPlaying = true;
		endGameVFX->Start();
		IsMiniGameSkiped = true;
		AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
		/*HOScene->GetObjectByID("whitemask")->SetAlpha(0);
		HOScene->GetObjectByID("whitemask")->Show(true);*/
	}
	//Control::Audio->PlaySample(aSFXClick);
}
void L1_S1_PZKidsPainting::Render(const CPoint* pCam)
{
	if (!Visible)
		return;

	static int FrameCnt = 0;
	FrameCnt = (FrameCnt + 1) % 2;
}

L1_S1_PZKidsPainting::~L1_S1_PZKidsPainting()
{

	GFHud->InvBoxCtrl->ResetToInvBox();

	tempObj = NULL;
	tempTxtObj = NULL;

	SAFE_DELETE(cv_Cutscn);
	SAFE_DELETE(skipObj);

	SAFE_DELETE(endGameVFX);

	SAFE_DELETE(subtitleText);

	//MUSIC_SFX
	Control::Audio->Samples[aTrackL1Broken]->SetVolume((float)Control::Audio->GlobalMusicVol);
	Control::Audio->Samples[aTrackL1Mansion]->SetVolume((float)Control::Audio->GlobalMusicVol);

	Control::Audio->StopSample(aTrackCutscnS1KidsPainting);

	Control::Audio->UnloadSample(aTrackCutscnS1KidsPainting);

	Control::Audio->StopSample(aTrackL1KidsPuzzleBGM);
	Control::Audio->UnloadSample(aTrackL1KidsPuzzleBGM);
	//MUSIC_SFX

	ProfileSystem::Instance->ActiveProfile.CurrentArea = ProfileSystem::Instance->ActiveProfile.PrevComboActiveArea;
}

void L1_S1_PZKidsPainting::ReceiveMessage(int val)
{
	if (val == MAP_NEW_AREA)
	{
		Hud->InvBox_StopPuzzleMode();
		SendMessage(ControlExit, ID, HOSM_NewArea);
	}
	else if (val == MAP_HUD_CLICK)
	{
		//TaskPendingUpdate();
		Hud->ShowMap();
	}
}

void L1_S1_PZKidsPainting::ProcessNonHOHint()
{

}

void L1_S1_PZKidsPainting::NavigateToScene()
{
	if (GFHud->InvBoxCtrl->navTapped)
	{
		if (Control::Input->LBclicked())
		{
			if (Control::Input->Mpos().x > GFHud->InvBoxCtrl->navRectX2&& Control::Input->Mpos().x < GFHud->InvBoxCtrl->navRectX1 && Control::Input->Mpos().y > GFHud->InvBoxCtrl->navRectY2&& Control::Input->Mpos().y < GFHud->InvBoxCtrl->navRectY1)
			{
				//Cursor::SetMode(CA_ExitUp);

				//GFHud->InvBoxCtrl->NavigateText->Show(false);
				ProfileSystem::Instance->ActiveProfile.L1S1_KidsPaintingPzBagActive = true;

				Hud->InvBox_StopPuzzleMode();
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