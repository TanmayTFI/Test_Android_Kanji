//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 6 :  PUZZLE TUNNELCLOCK
//====================================

#include "L2_S6_PZTunnelClock.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "TextureFactory.h"
#include "ParticleEffect.h"
#include "Rect.h"
#include "Polygon.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"

const CPoint	CenterPoint(683,334,0);
const CPoint	PopCloseRectBtnPoint(998,98,0);

const CRectangle  PopCloseButtonRect(968.0f,71.0f,62.0f,61.0f);
const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);


//Scene Rects
const CRectangle  tunnelRect(931.0f,114.0f,262.0f,295.0f);

const CRectangle  PearlRect1(342.0f,263.0f,130.0f,161.0f);
const CRectangle  PearlRect2(598.0f,27.0f,159.0f,147.0f);
const CRectangle  PearlRect3(857.0f,262.0f,178.0f,159.0f);
const CRectangle  PearlRect4(596.0f,498.0f,179.0f,170.0f);
const CRectangle  InfoRectCenter(581.0f,231.0f,199.0f,219.0f);

const CRectangle  areaRectClock(396.0f,66.0f,575.0f,570.0f);

const CRectangle  InvDestRect(284.0f,54.0f,748.0f,559.0f);

#define MAX_CLOCK_SFX_INTERVAL 0.45f

//Video Rects
const CRectangle  VidRectFull(0,0,1366,768);

float L2_S6_PZTunnelClock::getMouseAngle(CPoint *pos)
{
	float  dx  = pos->x - centerPoint->Pos.x;
	float  dy  = pos->y -  centerPoint->Pos.y;
	float  radians  = atan2(dy, dx);
	float rotation = radians * 180 / 3.14150f;

	return rotation;
}

float L2_S6_PZTunnelClock::getArrowAngle(int index)
{
	float  dx  = arrowOverlay[index]->Pos.x - centerPoint->Pos.x;
	float  dy  = arrowOverlay[index]->Pos.y -  centerPoint->Pos.y;
	float  radians  = atan2(dy, dx);
	float rotation = radians * 180 / 3.1415f;

	return rotation;
}


CRectangle L2_S6_PZTunnelClock::getNewRect(float angle,CRectangle rect)
{
	float x1 = cos(angle) * rect.topleft.x - sin(angle) * rect.topleft.y;
	float y1 = cos(angle) * rect.topleft.y + sin(angle) * rect.topleft.x;

	CRectangle rect2(x1 ,y1 ,rect.w ,rect.h);
	return rect2;
}


L2_S6_PZTunnelClock::L2_S6_PZTunnelClock()
{
	fireAnim = NULL;

	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV2\\S6\\P1\\P1.SCN");
	musicFadeOutTimer = 0;

	GFInstance->PostUpdate(2);

	InitPuzzle();

	GFInstance->PostUpdate(2);

	isPuzzleDoorOpenVideoPlaying = false;

	cv_PuzzleDoorOpen = NULL;
	time = 0;
	subIndex = 0;
	subMax = 2; 
	vidTimer = 0;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));
	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("puzzledooropen1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("puzzledooropen2"));

	//CRectangle RectPuzzleVid(171, 96, 1024, 576);
	CRectangle RectPuzzleVid(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	cv_PuzzleDoorOpen = new CVideo("OGV\\L2\\S6PUZZLEDOOROPEN.OGV", RectPuzzleVid, false, eZOrder_CutsceneFull);
	//cv_PuzzleDoorOpen->SetScale(1.334f);
	cv_PuzzleDoorOpen->SetScale(GFApp->video_aspectRatio);
	cv_PuzzleDoorOpen->SetSkipActive();

	clockLoopSFXInterval = MAX_CLOCK_SFX_INTERVAL;

	Control::Audio->LoadSample(aAmbs6TunnelEnd,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbs6TunnelEnd) )		
	{
		Control::Audio->PlaySample(aAmbs6TunnelEnd,true);
	}
	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L2_S6_PZTunnelClock]  = true;


	bgFogVFX_0 = new SuperFX("PRT\\L2\\S6ClockPuzzleFog1.INI", PointSystem::CreateCPoint(327,446,1), 1); 
	bgFogVFX_0->Start();
	bgFogVFX_0->AdvanceByTime(2.0f);
	

	bgFogVFX_1 = new SuperFX("PRT\\L2\\S6ClockPuzzleFog2.INI", PointSystem::CreateCPoint(896,608,1), 1); 
	bgFogVFX_1->Start();
	bgFogVFX_1->AdvanceByTime(2.0f);

	HOScene->GetObjectByID("Vines_0")->Animate(0, 6, 0, 0, 4, 180);
	HOScene->GetObjectByID("Vines_1")->Animate(0, 6, 0, 0, 4);
	HOScene->GetObjectByID("Vines_1")->Animate(0, 6, 0, 0, 4);
	HOScene->GetObjectByID("Leaves")->SetPivot(100, - 174);
	HOScene->GetObjectByID("Leaves")->Animate(0, 0, 1, 0, 4, 0);
	HOScene->GetObjectByID("FireGlow")->Animate(0.95f, 1.0f, 0.1f);	

	ResetBtn = new CObject();
	ResetBtn->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\RESETBTN.SPR", HOScene->GetObjectByID("ResetBtn")->Pos.x, HOScene->GetObjectByID("ResetBtn")->Pos.y, HOScene->GetObjectByID("ResetBtn")->ZOrder);
	ResetBtn->SetUID("resetBtn");
	HOScene->PushObject(ResetBtn);

	ResetBtn->SetEnable(false);
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		ResetBtn->SetEnable(false);

	ResetDia = new CHOScene();
	ResetDia->Load("UI\\RESETDIA.SCN");

	CObject* resetText = new CObject();
	resetText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\RESETCNF.SPR", 670, 308, 2);
	resetText->SetUID("resetText");
	ResetDia->PushObject(resetText);

	CObject* mask = new CObject();
	mask->LoadInit("UI\\MASK.SPR", 683, 384, -1);
	mask->SetUID("mask");
	mask->SetScale(2);
	ResetDia->PushObject(mask);

	CObject* btnText = new CObject();
	btnText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\YES.SPR", 590, 420, 5);
	btnText->SetUID("yesBTNText");
	ResetDia->PushObject(btnText);

	btnText = new CObject();
	btnText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\NO.SPR", 780, 420, 5);
	btnText->SetUID("noBTNText");
	ResetDia->PushObject(btnText);

	ResetDia->SetZOff(eZOrder_Popup);
	ResetDia->Show(false);
	IsResetPop = false;

	timer = 10.0f;
	isFadeIn = false;
	isFadeOut = false;
	isResetDiaFadeIn = false;
	isResetDiaFadeOut = true;

	//Objective Add
	if( !ProfileSystem::Instance->ActiveProfile.L2_S2_CutSceneVideoPlayed && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L2O3_NagaPearls] )
	{
		ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L2O3_NagaPearls] = true;
		ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive] = eObj_L2O3_NagaPearls;				
		//Increment Objectives
		ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
		Hud->ShowObjectiveFX(eObj_L2O3_NagaPearls);

		Control::Audio->PlaySample(aSFXMagic1);
		Hud->ShowBannerText("L2pearls",SFX_SOFT_NOTIFY);
	}
}

int L2_S6_PZTunnelClock::checkShutterImageStatus()
{
	// int index = 0;  //Sac: Unused variable, so commented it
	bool flag1 = true;
	bool flag2 = false;

	float SuccessAngles[3];

	for (int x = 0; x < 4; x++)
	{
		flag1 = true;
		switch (x)
		{
			case 0:
				SuccessAngles[0] = leftSuccessAngles[0];
				SuccessAngles[1] = leftSuccessAngles[1];
				SuccessAngles[2] = leftSuccessAngles[2];
			break;
			case 1:
				SuccessAngles[0] = rightSuccessAngles[0];
				SuccessAngles[1] = rightSuccessAngles[1];
				SuccessAngles[2] = rightSuccessAngles[2];
			break;
			case 2:
				SuccessAngles[0] = topSuccessAngles[0];
				SuccessAngles[1] = topSuccessAngles[1];
				SuccessAngles[2] = topSuccessAngles[2];
			break;
			case 3:
				SuccessAngles[0] = bottomSuccessAngles[0];
				SuccessAngles[1] = bottomSuccessAngles[1];
				SuccessAngles[2] = bottomSuccessAngles[2];
			break;
			
		}

		if(isPuzzleSolved[x] == false)
		{
			float addedValues[3];
			for (int i = 0; i < 3; i++)
			{
				addedValues[i] = -1;
			}
			for (int i = 0; i < 3; i++)
			{
				flag2 = false;
				float angle = arrow[i]->GetRotate();
				
				for (int j = 0; j  < 3; j ++)
				{
					if(SuccessAngles[j] == angle )
					{
						bool alreadyAdded = false;
						for (int k = 0; k  < 3; k ++)
						{
							if(addedValues[k] == angle)
							{
								alreadyAdded = true;
							}
						}
						if(alreadyAdded == false)
						{
							addedValues[i] = angle;
							flag2 = true;
							break;
						}
					}
				}

				if(flag2 == false)
				{
					flag1 = false;
					break;
				}
			}

			if(flag1 == true)
			{
				isPuzzleSolved[x] = true;
				return x;
			}
		}
	}

	return -1;
}

void L2_S6_PZTunnelClock::InitPuzzle()
{
	tweenTime = 0.0f;
	alphaTime = 1.0f;
	shutterOpenTime = 1.0f;
	rotTime = 1.00f;

	waitTime = 1.50f;

	// Sac: somehow the currentIndex value is not initiated, because of which first time the player 
	// opens the time puzzle, it will crash, as the currentIndex will take some arbitary value and
	// will try to find the arrow[currentIndex] while arrow array's total value is 3. 
	// so initiate it with 2
	currentIndex = 2;
	
	s6ClockWrongFX[0] = new SuperFX("PRT\\L2\\s6ClockWrong.INI", PointSystem::CreateCPoint(475,350,0), 100); 
	s6ClockWrongFX[1] = new SuperFX("PRT\\L2\\s6ClockWrong.INI", PointSystem::CreateCPoint(876,345,0), 100); 
	s6ClockWrongFX[2] = new SuperFX("PRT\\L2\\s6ClockWrong.INI", PointSystem::CreateCPoint(675,147,0), 100); 
	s6ClockWrongFX[3] = new SuperFX("PRT\\L2\\s6ClockWrong.INI", PointSystem::CreateCPoint(677,545,0), 100);

	s6ClockRightFX[0] = new SuperFX("PRT\\L2\\s6ClockRight.INI", PointSystem::CreateCPoint(475,350,0), 100); 
	s6ClockRightFX[1] = new SuperFX("PRT\\L2\\s6ClockRight.INI", PointSystem::CreateCPoint(876,345,0), 100); 
	s6ClockRightFX[2] = new SuperFX("PRT\\L2\\s6ClockRight.INI", PointSystem::CreateCPoint(675,147,0), 100); 
	s6ClockRightFX[3] = new SuperFX("PRT\\L2\\s6ClockRight.INI", PointSystem::CreateCPoint(677,545,0), 100);

	isPearlPlaced = false;
	isPearlAlphaChanging = false;
	isShutterOpening = false;
	isPuzzleOver = false;

	isPreparingReverseClockAnimation = false;
	isReverseClockAnimation = false;

	leftSuccessAngles[0] = 480.0f;
	leftSuccessAngles[1] = 360.0f;
	leftSuccessAngles[2] = 330.0f;

	rightSuccessAngles[0] = 300.0f;
	rightSuccessAngles[1] = 450.0f;
	rightSuccessAngles[2] = 210.0f;

	topSuccessAngles[0] = 390.0f;
	topSuccessAngles[1] = 270.0f;
	topSuccessAngles[2] = 540.0f;

	bottomSuccessAngles[0] = 240.0f;
	bottomSuccessAngles[1] = 420.0f;
	bottomSuccessAngles[2] = 510.0f;

	snapValues[0] = 0.0f;//360
	snapValues[1] = 30.0f;//390
	snapValues[2] = 60.0f;//420
	snapValues[3] = 90.0f;//450
	snapValues[4] = 120.0f;//480
	snapValues[5] = 150.0f;//510
	snapValues[6] = 180.0f;//540
	snapValues[7] = -150.0f;//210
	snapValues[8] = -120.0f;//240
	snapValues[9] = -90.0f;//270
	snapValues[10] = -60.0f;//300
	snapValues[11] = -30.0f;//330

	snapValues2[0] = 0.0f;
	snapValues2[1] = 30.0f;
	snapValues2[2] = 60.0f;
	snapValues2[3] = 90.0f;
	snapValues2[4] = -250.0f;
	snapValues2[5] = -220.0f;
	snapValues2[6] = -180.0f;

	snapValues2[7] = -150.0f;
	snapValues2[8] = -120.0f;
	snapValues2[9] = -90.0f;
	snapValues2[10] = -60.0f;
	snapValues2[11] = -30.0f;

	isPuzzleSolved[0] = ProfileSystem::Instance->ActiveProfile.L2_S6_Shutter1Solved;
	isPuzzleSolved[1] = ProfileSystem::Instance->ActiveProfile.L2_S6_Shutter2Solved;
	isPuzzleSolved[2] = ProfileSystem::Instance->ActiveProfile.L2_S6_Shutter3Solved;
	isPuzzleSolved[3] = false;

	pearl[0] = HOScene->GetObjectByID("Pearl_Left");
	pearl[1] = HOScene->GetObjectByID("Pearl_Right");
	pearl[2] = HOScene->GetObjectByID("Pearl_Top");
	pearl[3] = HOScene->GetObjectByID("Pearl_Bottom");

	greenPearlGlows[0] = HOScene->GetObjectByID("greenglow_left");
	greenPearlGlows[1] = HOScene->GetObjectByID("greenglow_right");
	greenPearlGlows[2] = HOScene->GetObjectByID("greenglow_top");
	greenPearlGlows[3] = HOScene->GetObjectByID("greenglow_bottom");

	centerPoint =  HOScene->GetObjectByID("Arrow_Overlay");

//	float arrAngles[12] = {30.0f,60.0f,90.0f,120.0f,150.0f,180.0f};  //Sac: Unused variable, so commented it
	arrow[0] = HOScene->GetObjectByID("Arrow1");
	arrow[1] = HOScene->GetObjectByID("Arrow2");
	arrow[2] = HOScene->GetObjectByID("Arrow3");

	arrowOverlay[0] = HOScene->GetObjectByID("overlay1");
	arrowOverlay[1] = HOScene->GetObjectByID("overlay2");
	arrowOverlay[2] = HOScene->GetObjectByID("overlay3");

	closedShutter[0] = HOScene->GetObjectByID("Shutter_Left");
	closedShutter[1] = HOScene->GetObjectByID("Shutter_Right");
	closedShutter[2] = HOScene->GetObjectByID("Shutter_Top");
	closedShutter[3] = HOScene->GetObjectByID("Shutter_Bottom");
	
	shutterOpenAni[0] = HOScene->GetObjectByID("shutter_open_left");
	shutterOpenAni[1] = HOScene->GetObjectByID("shutter_open_right");
	shutterOpenAni[2] = HOScene->GetObjectByID("shutter_open_top");
	shutterOpenAni[3] = HOScene->GetObjectByID("shutter_open_bottom");
	fireAnim = HOScene->GetObjectByID("Fire");
	fireAnim->SetScale(2);

	shutterOpenAni[0]->Show(false);
	shutterOpenAni[1]->Show(false);
	shutterOpenAni[2]->Show(false);
	shutterOpenAni[3]->Show(false);

	chart = HOScene->GetObjectByID("Chart");
	chart->Animate(2, 5, 0, 0, 4); 
	if(!ProfileSystem::Instance->ActiveProfile.L2_S6_PaperPuzCluePlaced)
	{
		chart->Show(false);
	}

	for (int i = 0; i < 4; i++)
	{
		greenPearlGlows[i]->Show(false);
		greenPearlGlows[i]->Animate(0.75f, 1.0f, 2.5f, 0, eBlend_AlphaAdditive);
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L2_S6_NagaPearlFixed)
	{
		closedShutter[0]->Show(false);
		closedShutter[1]->Show(false);
		closedShutter[2]->Show(false);
		closedShutter[3]->Show(false);
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			pearl[i]->Show(false);
		}
		//one pearl is visible
		pearl[3]->Show(true);
	}


	if(ProfileSystem::Instance->ActiveProfile.L2_S6_PaperPuzCluePlaced && ProfileSystem::Instance->ActiveProfile.L2_S6_NagaPearlFixed)
	{
		Hud->InvBox_StartPuzzleMode();
	}

	for (int i = 0; i < 4; i++)
	{
		redAnimation[i] = HOScene->GetObjectByID(std::string("redcircle" + MKSTR(i)).c_str());
		redAnimation[i]->Show(false);

		greenAnimation[i] = HOScene->GetObjectByID(std::string("greencircle" + MKSTR(i)).c_str());
		greenAnimation[i]->Show(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L2_S6_Shutter1Solved)
	{
		greenAnimation[0]->Show(true);
	}
	if(ProfileSystem::Instance->ActiveProfile.L2_S6_Shutter2Solved)
	{
		greenAnimation[1]->Show(true);
	}
	if(ProfileSystem::Instance->ActiveProfile.L2_S6_Shutter3Solved)
	{
		greenAnimation[2]->Show(true);
	}
	
	//Show green glows
	if(ProfileSystem::Instance->ActiveProfile.L2_S6_Shutter3Solved)
		greenPearlGlows[3]->Show(true);
	else if(ProfileSystem::Instance->ActiveProfile.L2_S6_Shutter2Solved)
		greenPearlGlows[2]->Show(true);
	else if(ProfileSystem::Instance->ActiveProfile.L2_S6_Shutter1Solved)
		greenPearlGlows[1]->Show(true);
	else if(ProfileSystem::Instance->ActiveProfile.L2_S6_PaperPuzCluePlaced && ProfileSystem::Instance->ActiveProfile.L2_S6_NagaPearlFixed)
		greenPearlGlows[0]->Show(true);

	for (int i = 0; i < 12; i++)
	{
		snapGlow[i] = HOScene->GetObjectByID(std::string("glow" + MKSTR(i)).c_str());
		snapGlow[i]->Show(false);
	}

	float angle;
	// int count=0;  //Sac: Unused variable, so commented it

	for (int i = 0;i < 3;)
	{
		//angle = arrAngles[rand() % 6];
		angle = snapValues[rand() % 11] + 360;
		bool isContinue = false;
		for (int j = 0; j < 3;j++)
		{
			if(arrowAngle[j] == angle)
			{
				isContinue = true;
			}
		}

		if(isContinue) continue;

		arrow[i]->SetRotate((float)angle);
		endAngle[i] = angle; // for reverse arrow

		arrowAngle[i] = angle;

		angle += 90.0f;
		float rad = angle * 3.14f / 180.0f;
		float radius = 100.0f;
		float x = centerPoint->Pos.x + radius *cos(-rad);
		float y =  centerPoint->Pos.y + radius * sin(-rad);

		arrowOverlay[i]->Pos.x = x;
		arrowOverlay[i]->Pos.y = y;

		arrowOverlay[i]->SetAlpha(0.0f);
		i++;
	}

	isMouseMove = false;
}

void L2_S6_PZTunnelClock::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;	

	if(isPuzzleDoorOpenVideoPlaying)
		return;

	if( Control::Audio->IsPlaying(aTrackL2theme) )
	{
		Control::Audio->Samples[aTrackL2theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL2theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL2theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL2puzzle) )
				  Control::Audio->PlaySample(aTrackL2puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL2puzzle))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL2puzzle, true);
		}
	}
}

int L2_S6_PZTunnelClock::getNearestArrowIndex(float angle)
{
	int index = 0;
	float diff1 = abs(angle - arrowAngle[0]);
	float diff2;
	for (int i = 1; i < 3; i++)
	{
		diff2 = abs(angle - arrowAngle[i]);
		if(diff2 < diff1)
		{
			diff1 = diff2;
			index = i;
		}
	}
	return index;
}


void L2_S6_PZTunnelClock::Update()
{
	NavigateToScene();
	#ifdef _PARTICLEEDITORENABLED
		if(_isParticleEditorActive)
			return;
	#endif // _PARTICLEEDITORENABLED

	if( Hud->PopupState != Popup_Inactive )
		return;

	ResetPuzzle();
	if(IsResetPop)
	{
		GFHud->AllowInput = false;
		return;
	}
	else
		GFHud->AllowInput = true;

	if(isPuzzleDoorOpenVideoPlaying)
	{
		Cursor::SetMode(CA_Normal);//Subtitles Code
		time = cv_PuzzleDoorOpen->GetTime();

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
			cv_PuzzleDoorOpen->isEndReached() ||
#endif
			vidTimer > 23)
		{
			subtitleText->SetText("");
			isPuzzleDoorOpenVideoPlaying = false;
			Hud->AllowInput = true;
			Hud->ShowHud(false);
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S7_PZSoldiers;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
		return;
	}	

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing)
		return;

	if(isPuzzleOver)
	{
		return;
	}

	Cursor::SetMode(CA_Normal);
	CPoint pos = Control::Input->Mpos();

	if( IsMouseOverGoBackArea(&pos) )
	{

#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L2_S6_TunnelEnd);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L2_S6_TunnelEnd);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L2_S6_TunnelEnd, 2);
			}
#else
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S6_TunnelEnd;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}
		return;
	}

	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S6_TunnelEnd;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
			return;
		}
	#endif

	if(!ProfileSystem::Instance->ActiveProfile.L2_S6_NagaPearlFixed)
	{
		if( InvDestRect.Intersection(&pos) )
		{
			Cursor::SetMode(CA_Gear);
			if( Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L2vault");
			}
		}
		return;
	}

	if(isPearlAlphaChanging == true)
	{
		tweenTime += Control::LogicRate;
		float tempAlpha = startAlpha + (endAlpha - startAlpha) * easing(tweenTime,0,1,alphaTime);

		for (int i = 0; i < 4; i++)
		{
			if(i!=3)
				pearl[i]->SetAlpha(tempAlpha);
		}

		if( tweenTime >= alphaTime )
		{
			tweenTime = 0;
			for (int i = 0; i < 4; i++)
			{
				pearl[i]->SetAlpha(endAlpha);
				closedShutter[i]->Show(false);
				shutterOpenAni[i]->Show(true);
				shutterOpenAni[i]->PlayAnimation(0);
			}
			isPearlAlphaChanging = false;
			isShutterOpening = true;
		}
		return;
	}

	if(isShutterOpening)
	{
		tweenTime += Control::LogicRate;
		easing(tweenTime,0,1,shutterOpenTime);

		if( tweenTime >= shutterOpenTime )
		{
			tweenTime = 0;
			for (int i = 0; i < 4; i++)
			{
				shutterOpenAni[i]->Show(false);
			}
			isShutterOpening = false;
		}
		return;
	}
	
	if(!ProfileSystem::Instance->ActiveProfile.L2_S6_PaperPuzCluePlaced)
	{
		if( InvDestRect.Intersection(&pos) )
		{
			Cursor::SetMode(CA_Gear);
			if( Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L2fit");
			}
		}
		return;
	}
		
	if(isPreparingReverseClockAnimation)
	{
		tweenTime += Control::LogicRate;
		easing(tweenTime,0,1,waitTime);

		if(tweenTime >= waitTime)
		{
			tweenTime = 0.0f;
			isPreparingReverseClockAnimation = false;
			isReverseClockAnimation = true;
		}
		return;
	}

	if(isReverseClockAnimation)
	{
		tweenTime += Control::LogicRate;
		float easeTime = easing(tweenTime,0,1,rotTime);

		float angle;
		for (int i = 0; i < 3; i++)
		{
			angle = startAngle[i] + (endAngle[i] - startAngle[i]) * easeTime;
			arrow[i]->SetRotate(angle);
		}

		if(tweenTime >= rotTime)
		{
			tweenTime = 0.0f;
			for (int i = 0; i < 3; i++)
			{
				angle = endAngle[i];
				arrow[i]->SetRotate(angle);

				angle += 90.0f;
				float rad = angle * 3.14f / 180.0f;
				float radius = 100.0f;
				float x = centerPoint->Pos.x + radius *cos(-rad);
				float y =  centerPoint->Pos.y + radius * sin(-rad);

				arrowOverlay[i]->Pos.x = x;
				arrowOverlay[i]->Pos.y = y;

				for (int i = 0; i < 4; i++)
				{
					redAnimation[i]->Show(false);
				}
			}
			isReverseClockAnimation = false;
		}
		return;
	}

	float mouseAngle = getMouseAngle(&pos);
	float tempArrowAngle;

	if(Control::Input->LBdown() == false)
	{	
		if(isMouseDown == true)
		{
			float angle2 = getMouseAngle(&pos);
			angle2 *= -1;
			
			float angle = arrow[currentIndex]->GetRotate() - 360;
			if(angle<-240)
			{
				angle += 360;
			}

			float minValue = angle - 17;
			float maxValue = angle + 17;

			for (int i = 0; i < 12; i++)
			{
				if((snapValues2[i] > minValue && snapValues2[i] < maxValue) || (snapValues[i] > minValue && snapValues[i] < maxValue))
				{
					angle = snapValues[i] + 360;
					arrow[currentIndex]->SetRotate(angle);

					angle += 90.0f;
					float rad = angle * 3.14f / 180.0f;
					float radius = 100.0f;
					float x = centerPoint->Pos.x + radius *cos(-rad);
					float y =  centerPoint->Pos.y + radius * sin(-rad);

					arrowOverlay[currentIndex]->Pos.x = x;
					arrowOverlay[currentIndex]->Pos.y = y;
					break;
				}
			}

			/*for (int j = 0; j < 12; j++)
			{
				snapGlow[j]->Show(false);
			}*/

			int index = checkShutterImageStatus();
			if(index != -1)
			{
				if(index == 0)
				{
					greenAnimation[index]->Show(true);
					//greenAnimation[index]->SetAnimation(0);
					greenAnimation[index]->ForcePlayAnimation(0);
					s6ClockRightFX[index]->Start();
					isPuzzleSolved[index] = true;
					ProfileSystem::Instance->ActiveProfile.L2_S6_Shutter1Solved = true;

					Control::Audio->QuickLoadAndPlaySFX("s6pz1_greenbulblite");

					greenPearlGlows[0]->Show(false);
					greenPearlGlows[1]->Show(true);
				}
				else
				{
					if(isPuzzleSolved[index - 1] == true)
					{
						greenAnimation[index]->Show(true);
						//greenAnimation[index]->SetAnimation(0);
						greenAnimation[index]->ForcePlayAnimation(0);
						isPuzzleSolved[index] = true;
						s6ClockRightFX[index]->Start();

						Control::Audio->QuickLoadAndPlaySFX("s6pz1_greenbulblite");

						if(index == 1)
						{
							greenPearlGlows[1]->Show(false);
							greenPearlGlows[2]->Show(true);

							ProfileSystem::Instance->ActiveProfile.L2_S6_Shutter2Solved = true;
						}
						else if (index == 2)
						{
							greenPearlGlows[2]->Show(false);
							greenPearlGlows[3]->Show(true);

							ProfileSystem::Instance->ActiveProfile.L2_S6_Shutter3Solved = true;
						}
					}
					else
					{
						redAnimation[index]->Show(true);
						//redAnimation[index]->SetAnimation(0);
						redAnimation[index]->ForcePlayAnimation(0);

						Control::Audio->QuickLoadAndPlaySFX("s6pz1_redbulblite");

						s6ClockWrongFX[index]->Start();

						for (int i = 0; i < 4; i++)
							greenPearlGlows[i]->Show(false);

						greenPearlGlows[0]->Show(true);

						ProfileSystem::Instance->ActiveProfile.L2_S6_Shutter1Solved = false;
						ProfileSystem::Instance->ActiveProfile.L2_S6_Shutter2Solved = false;
						ProfileSystem::Instance->ActiveProfile.L2_S6_Shutter3Solved = false;

						for (int i = 0; i < 4; i++)
						{
							greenAnimation[i]->Show(false);
							isPuzzleSolved[i] = false;
						}

						//SART: prepare for reverse animation
						for (int i = 0; i < 3; i++)
						{
							startAngle[i] = arrow[i]->GetRotate() - 360;
						}
						isPreparingReverseClockAnimation = true;

						Hud->ShowBannerText("L2oops");
						//return;
					}
				}

				//checking puzzle finished or not
				bool flag = true;
				for (int i = 0; i < 4; i++)
				{
					if(isPuzzleSolved[i] == false)
					{
						flag = false;
						break;
					}
				}

				if(flag == true)
				{
					HandlePuzzleComplete();
					
					return;
				}
			}
			else
			{
				//Clock Arm Not activated any patterns. Just released
				Control::Audio->QuickLoadAndPlaySFX("s6pz1_clockarmFix");
			}
		}
		isMouseMove = false;
		isMouseDown = false;
		currentIndex = -1;
	}

	for (int j = 0; j < 12; j++)
	{
		snapGlow[j]->Show(false);
	}

	for (int k = 0; k < 3; k++)
	{
		float angle = arrow[k]->GetRotate() - 360;

		if(angle<-240)
		{
			angle += 360;
		}

		float minValue = angle - 17;
		float maxValue = angle + 17;

		for (int i = 0; i < 12; i++)
		{
			if((snapValues2[i] > minValue && snapValues2[i] < maxValue) || (snapValues[i] > minValue && snapValues[i] < maxValue) )
			{
				for (int j = 0; j < 12; j++)
				{
					if(i == j)
					{
						snapGlow[j]->Show(true);
					}
				}
				break;
			}
		}
	}
	
	if(isMouseMove == false)
	{
		for (int i = 2; i >= 0; i--)
		{
			tempArrowAngle = getArrowAngle(i);

			if(tempArrowAngle<-170)
			{
				tempArrowAngle=180;
			}

			if(mouseAngle<-160)
			{
				mouseAngle += 360;
			}

			float minValue = mouseAngle - 15;
			float maxValue = mouseAngle + 15;

			float dx = centerPoint->Pos.x - pos.x;
			float dy = centerPoint->Pos.y - pos.y;

			float distance = sqrt(dx * dx + dy * dy);
		
			if(tempArrowAngle > minValue && tempArrowAngle < maxValue && distance < 160)
			{
				Cursor::SetMode(CA_HandPointy);

				if (Control::Input->LBdown())
				{
					Control::Audio->QuickLoadAndPlaySFX("s6pz1_clockarmselect");

					isMouseMove = true;
					isMouseDown = true;
					currentIndex = i;

					prevAngle = getMouseAngle(&pos);
					return;
				}
			}
		}
	}
	else
	{
		Cursor::SetMode(CA_HandPointy);
		float angle2 = getMouseAngle(&pos);		

		angle2 *= -1;
		arrow[currentIndex]->SetRotate(angle2 += 270);
		
		angle2 += 90.0f;
		float rad = angle2 * 3.14f / 180.0f;
		float radius = 100.0f;
		float x = centerPoint->Pos.x + radius *cos(-rad);
		float y =  centerPoint->Pos.y + radius * sin(-rad);

		arrowOverlay[currentIndex]->Pos.x = x;
		arrowOverlay[currentIndex]->Pos.y = y;

		////snap indication
		//float angle = arrow[currentIndex]->GetRotate() - 360;
		//	
		//for (int i = 0; i < 12; i++)
		//{
		//	float minValue = angle - 17;
		//	float maxValue = angle + 17;
		//	//K_LOG ("SREERAJ ---> arrow angle = %f  minValue = %f  maxValue = %f --- snapValues2[%d] = %f",angle , minValue,maxValue,i,snapValues2[i]);
		//	if(snapValues2[i] > minValue && snapValues2[i] < maxValue)
		//	{
		//		for (int j = 0; j < 12; j++)
		//		{
		//			if(i == j)
		//			{
		//				snapGlow[j]->Show(true);
		//			}
		//			else
		//			{
		//				snapGlow[j]->Show(false);
		//			}
		//		}
		//		break;
		//	}
		//}
	}
}
void L2_S6_PZTunnelClock::ProcessSkip()
{
	if(!isPuzzleOver)
	{
		AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
		HandlePuzzleComplete();	
	}
}

void L2_S6_PZTunnelClock::HandlePuzzleComplete()
{
	AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	isPuzzleOver = true; // puzzle cleared

	if(_handleExtrasGameComplete())
		return;

	Hud->ShowBannerText("L2yes");

	isPuzzleDoorOpenVideoPlaying = true;
	cv_PuzzleDoorOpen->PlayFadeInVideo(2.0f);
	Hud->EnableInput(false);
	Hud->HideHud(false);
	
	if( Control::Audio->IsPlaying(aTrackL2puzzle) )
			Control::Audio->StopSample(aTrackL2puzzle);
	if( Control::Audio->IsPlaying(aAmbs6TunnelEnd) )		
		Control::Audio->StopSample(aAmbs6TunnelEnd);

	for (int i = 0; i < 12; i++)
	{
		snapGlow[i]->Show(false);
	}

	chart->FadeOut(4.0f);
	for (int i = 0; i < 4; i++)
	{
		greenPearlGlows[i]->Show(false);
		pearl[i]->FadeOut(3.0f);
		closedShutter[i]->FadeOut(3.0f);
		shutterOpenAni[i]->FadeOut(3.0f);
		redAnimation[i]->FadeOut(3.0f);
		greenAnimation[i]->FadeOut(3.0f);
	}
	for (int i = 0; i < 3; i++)
	{
		arrow[i]->FadeOut(3.0f);
		arrowOverlay[i]->FadeOut(3.0f);
	}

	centerPoint->Show(false);
	ProfileSystem::Instance->ActiveProfile.L2_S6_ClockPuzzleSolved = true;
	
}

float L2_S6_PZTunnelClock::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L2_S6_PZTunnelClock::~L2_S6_PZTunnelClock()
{
	Hud->InvBox_StopPuzzleMode();

	SAFE_DELETE(cv_PuzzleDoorOpen);

	SAFE_DELETE(subtitleText);
	SAFE_DELETE(bgFogVFX_0);
	SAFE_DELETE(bgFogVFX_1);
	SAFE_DELETE(ResetDia);

	for (int i = 0; i < 4; i++)
	{
		SAFE_DELETE(s6ClockWrongFX[i]);
	}
	
	//MUSIC_SFX

	

	Control::Audio->StopSample(aAmbs6TunnelEnd);
	Control::Audio->UnloadSample(aAmbs6TunnelEnd);
	//MUSIC_SFX
}

void L2_S6_PZTunnelClock::ReceiveMessage(int val)
{	

	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		//TaskPendingUpdate();
		Hud->ShowMap();
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			if( itemVal == eInv_L2S2_Pearl1)
			{
				if( !ProfileSystem::Instance->ActiveProfile.L2_S6_NagaPearlFixed )
				{
						
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 

					Control::Audio->QuickLoadAndPlaySFX("s6pz1_nagapearlsfix");

					ProfileSystem::Instance->ActiveProfile.L2_S6_NagaPearlFixed = true;
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S2_Pearl1] = false;
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S2_Pearl2] = false;
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S2_Pearl3] = false;
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S2_Pearl4] = false;

					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S2_Pearl1);

					isPearlAlphaChanging = true;
					isPearlPlaced = true;
					startAlpha = 0.0f;
					endAlpha = 1.0f;
					for (int i = 0; i < 4; i++)
					{
						pearl[i]->Show(true);
					}
					if( ProfileSystem::Instance->ActiveProfile.L2_S6_NagaPearlFixed && ProfileSystem::Instance->ActiveProfile.L2_S6_PaperPuzCluePlaced)
						Hud->InvBox_StartPuzzleMode();
				}
				return;
			}
			else if( itemVal == eInv_L2S6_PaperPuzClue)
			{
				if( !ProfileSystem::Instance->ActiveProfile.L2_S6_NagaPearlFixed )
				{
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 

					Hud->ShowBannerText("L2clue");
				}
				else if( !ProfileSystem::Instance->ActiveProfile.L2_S6_PaperPuzCluePlaced )
				{
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 

					Control::Audio->QuickLoadAndPlaySFX("s6pz1_clueplaced");

					ProfileSystem::Instance->ActiveProfile.L2_S6_PaperPuzCluePlaced = true;
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S6_PaperPuzClue] = false;

					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S6_PaperPuzClue);

					greenPearlGlows[0]->Show(true); //show first one glow

					chart->FadeIn(3.0f);
					if( ProfileSystem::Instance->ActiveProfile.L2_S6_NagaPearlFixed && ProfileSystem::Instance->ActiveProfile.L2_S6_PaperPuzCluePlaced)
						Hud->InvBox_StartPuzzleMode();
				}
				return;
			}
			else
				GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		
		
		
	}	
}

void L2_S6_PZTunnelClock::ProcessNonHOHint()
{
	int iNonHOHint = 0; 
	int MaxNonHOHints = 5;
	do
	{
		switch( iNonHOHint )
		{
			default:			
				//Exit coordinates
				break;
			
			
			case 0: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L2_S6_NagaPearlFixed && !ProfileSystem::Instance->ActiveProfile.L2_S6_PaperPuzCluePlaced && ProfileSystem::Instance->ActiveProfile.L2_S6_IsP3PaperPuzClueTaken )
					{
						Hud->ShowHintForInvItem(eInv_L2S6_PaperPuzClue,&InvDestRect);
						return;
					}
					break;
			case 1: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L2_S6_NagaPearlFixed && ProfileSystem::Instance->ActiveProfile.L2_S2_InvNagaPearlsCollected)
					{
						Hud->ShowHintForInvItem(eInv_L2S2_Pearl1,&InvDestRect);
						return;
					}
					break;
			case 2: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S6_TunnelEnd])
					{
						InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
						return;
					}
					break;
			case 3: ++iNonHOHint;
					{
						if(Hud->ShowHintForComboInvItems())
							return;
					}
			case 4: ++iNonHOHint;
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

void L2_S6_PZTunnelClock::ResetPuzzle()
{
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		return;
	if(!ProfileSystem::Instance->ActiveProfile.L2_S6_NagaPearlFixed)
	{
		ResetBtn->SetEnable(false);
		return;
	}
	if(!ProfileSystem::Instance->ActiveProfile.L2_S6_PaperPuzCluePlaced)
	{
		ResetBtn->SetEnable(false);
		return;
	}
	ResetBtn->SetEnable(true);
	if(timer > 0)
	{
		timer -= (float(Control::MasterTmr->RealDMS) / 1000.0f);
		ResetBtn->SetEnable(false);
		if(!IsResetPop)
		{
			if(!isResetDiaFadeOut)
				ResetDia->FadeOut(2);
			isResetDiaFadeOut = true;
		}
		return;
	}
	else
	{
		ResetBtn->SetEnable(true);	
		if(!isFadeIn)
			ResetBtn->FadeIn(1);
		isFadeIn = true;
	}
	if(ResetBtn->IsIntersect(Control::Input->Mpos()))
	{
		ResetBtn->SetAnimation(1);
		if(!IsResetPop)
			Cursor::SetMode(CA_HandPointy);
		if(KInput::getLeftButtonState() == ISDOWN)
		{
			IsResetPop = true;
		}
	}
	else
	{
		ResetBtn->SetAnimation(2);
		Cursor::SetMode(CA_Normal);
	}

	if(IsResetPop)
	{
		if(!isResetDiaFadeIn)
		{
			ResetDia->FadeIn(2);
		}
		if(!isFadeOut)
			ResetBtn->FadeOut(1);
		isFadeOut = true;
		isResetDiaFadeIn = true;
		CObject *resetYes = ResetDia->GetObjectByID("BtnYes");
		CObject *resetNo = ResetDia->GetObjectByID("BtnNo");

		if(Hud->CheckPopupButton(resetYes) == Popup_Click)
		{
			ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[0] = 5;
			ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[1] = 4;
			ProfileSystem::Instance->ActiveProfile.L2_S6_P3CenterNums[2] = 2;
			ProfileSystem::Instance->ActiveProfile.L2_S6_Shutter1Solved = false;
			ProfileSystem::Instance->ActiveProfile.L2_S6_Shutter2Solved = false;
			ProfileSystem::Instance->ActiveProfile.L2_S6_Shutter3Solved = false;
			ProfileSystem::Instance->ActiveProfile.L2_S6_ClockPuzzleSolved = false;
			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S6_PZTunnelClock;
			SendMessage(ControlExit, ID, HOSM_NewArea);
			IsResetPop = false;
		}
		if(Hud->CheckPopupButton(resetNo) == Popup_Click)
		{
			IsResetPop = false;
			timer = 10.0f;
			isFadeIn = false;
			isFadeOut = false;
			isResetDiaFadeOut = false;
			isResetDiaFadeIn = false;
		}
	}
	else
	{
		ResetBtn->SetEnable(true);
	}
}
void L2_S6_PZTunnelClock::NavigateToScene()
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