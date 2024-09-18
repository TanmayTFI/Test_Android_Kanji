//====================================
//DQFYLH
// Hidden Object System
//====================================

#include "Application.h"
#include "L1_S2_PZFuse.h"
#include "Hud.h"
#include "Banners.h"
#include "HOScene.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "Rect.h"
#include "Resources.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "AwardManager.h"

const CRectangle  StatFuse1Rect(412.0f,318.0f,82.0f,35.0f);
const CRectangle  StatFuse2Rect(898.0f,543.0f,85.0f,37.0f);
const CRectangle  StatFuseFixLeftRect(629.0f,325.0f,72.0f,112.0f);
const CRectangle  StatFuseFixRightRect(710.0f,325.0f,65.0f,113.0f);

const CRectangle  LeverRect(1069.0f,446.0f,130.0f,123.0f);

const CRectangle  LeftDragArea(311.0f,22.0f,300.0f,281.0f);
const CRectangle  RightDragArea(691.0f,30.0f,286.0f,280.0f);

const CRectangle  GreenWireDownRect(275.0f,535.0f,79.0f,55.0f);
const CRectangle  GreenWireTopRect(1063.0f,126.0f,78.0f,107.0f);

const CRectangle  GreenWireTopDestRect(981.0f,131.0f,62.0f,64.0f);
const CRectangle  GreenWireDownDestRect(344.0f,513.0f,49.0f,51.0f);


#define _SparkGapConst 50
const CPoint GreenWireDownPos(363,540+50,0);
const CPoint GreenWireTopPos(1010,164+50,0);

const CPoint LeftFusePos(670,406+50,0);
const CPoint RightFusePos(740,360+50,0);

const float  wirePos[4][2] = {{471,232+_SparkGapConst},{577,88+_SparkGapConst},{765,93+_SparkGapConst},{866,240+_SparkGapConst}};
const float  RotGapPos[5][2] = {{558,306+_SparkGapConst},{575,194+_SparkGapConst},{673,141+_SparkGapConst},{772,199+_SparkGapConst},{783,310+_SparkGapConst}};

float wFuseStartX[MAX_FUSES]	= {398,448,878,926};
float wFuseStartY[MAX_FUSES]	= {100,100,100,100};

float wFuseEndX[MAX_FUSES]		= {480,530,940,1000};
float wFuseEndY[MAX_FUSES]		= {190,190,140,180};

float wFusePhase[MAX_FUSES]		= {0,180,-180,90};
float wFuseRefXPos[MAX_FUSES]	= {0,0,0,0};

bool IsWireFuseFixed[MAX_FUSES]			= {false,false,false,false};
bool IsWireFuseFixEmpty[MAX_FUSES]		= {true,true,true,true};
int	 WireFuseConnectedToId[MAX_FUSES]	= {-1,-1,-1,-1};

bool IsRotFusePowerOn[MAX_FUSES]		= {false,false,false,false};

float lightAlphaArray3[20] = {0,0,0,0,0,0.15f,0.75f,0.95f,0.85f,0.75f,0.85f,0.65f,0.95f,0.15f,0.25f,0.55f,0.15f,0,0,0};

L1_S2_PZFuse::L1_S2_PZFuse()
{
	Visible = false;	
	LoadHud(INVENTORY_V1);	
	LoadScene("LV1\\S2\\P1\\P1.SCN");

	//Localized
	CObject* temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L1\\S2\\LIFT.SPR", 1175, 500, 2);
	temp->SetUID("Lift");
	HOScene->PushObject(temp);

	IsAllStaticFusesFixed = false;

	if(ProfileSystem::Instance->ActiveProfile.L1S2_IsP1LeftFuseFixed && ProfileSystem::Instance->ActiveProfile.L1S2_IsP1RightFuseFixed
		&& ProfileSystem::Instance->ActiveProfile.L1S2_IsP1GreenWireDownFixed && ProfileSystem::Instance->ActiveProfile.L1S2_IsP1GreenWireTopFixed )	
	{
		IsAllStaticFusesFixed = true;
		Hud->InvBox_StartPuzzleMode();
	}

	RainBgVFX = new SuperFX("PRT\\BG\\rainfuse.INI", PointSystem::CreateCPoint(580.0f,-50.0f,0), 100);
	RainBgVFX->Start();
	RainBgVFX->AdvanceByTime(2.0f);

	SparkVFX = new SuperFX("PRT\\L1\\fusesparks.INI", PointSystem::CreateCPoint(580.0f,-50.0f,0), 100);

	FliesLeftVFX = new SuperFX("PRT\\L1\\fuseflies.INI", PointSystem::CreateCPoint(173,490,0), 100);
	FliesLeftVFX->Start();

	FliesRightVFX = new SuperFX("PRT\\L1\\fuseflies.INI", PointSystem::CreateCPoint(1210,454,0), 100);
	FliesRightVFX->Start();

	//raindropsVFX	
	RaindropsVFX = new SuperFX("PRT\\L1\\raindrops.INI", PointSystem::CreateCPoint(657,41,0), 100);
	RaindropsVFX->Start();

	centerLightTimer = 0;
	musicFadeOutTimer = 0;

	IsShaking = false;
	ShakeObject = NULL;
	ShakeTimer = 0;

	FuseSparkGlow1 = HOScene->GetObjectByID("FuseGlow1");
	FuseSparkGlow1->SetBlendMode(eBlend_AlphaAdditive);
	FuseSparkGlow2 = HOScene->GetObjectByID("FuseGlow2");
	FuseSparkGlow2->SetBlendMode(eBlend_AlphaAdditive);

	FuseSpark1 = HOScene->GetObjectByID("FuseSpark1");
	FuseSpark2 = HOScene->GetObjectByID("FuseSpark2");

	FuseSpark1->SetEnable(false);
	FuseSpark2->SetEnable(false);

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

	sparkTimer = 0;
	sparkTimeGap = 2.0f+(rand()%10)/10.0f*4.0f; //from 2 to 6 seconds timegap

	greenSparkTimer = 0;
	greenSparkTimeGap = 3.0f+(rand()%10)/10.0f*4.0f; //from 3 to 7 seconds timegap;

	sparkWireTimer = 0;
	sparkWireTimeGap = 3.0f+(rand()%10)/10.0f*4.0f;
	
	//Static Fuses Stuffs
	StaticFuseTaken = NULL;

	IsBgLightAnimationPlaying = false;
	//BgLitAnim = HOScene->GetObjectByID("bgLit");

	HOScene->GetObjectByID("bgLeftRose")->SetPivot(-62,64);
	HOScene->GetObjectByID("bgLeftRose")->Animate(0,0,4,0,7.0f);

	HOScene->GetObjectByID("bgRoseRight")->SetPivot(40,55);
	HOScene->GetObjectByID("bgRoseRight")->Animate(0,0,4,0,6.0f);

	StaticFuse1 = HOScene->GetObjectByID("StatFuseBgL");
	StaticFuse1->InitPos = StaticFuse1->Pos;
	StaticFuse2 = HOScene->GetObjectByID("StatFuseBgR");
	StaticFuse2->InitPos = StaticFuse2->Pos;

	GreenWireDown = HOScene->GetObjectByID("GreenWireDown");
	GreenWireTop = HOScene->GetObjectByID("GreenWireUp");

	IsGreenWireDownFixed = ProfileSystem::Instance->ActiveProfile.L1S2_IsP1GreenWireDownFixed;
	IsGreenWireTopFixed = ProfileSystem::Instance->ActiveProfile.L1S2_IsP1GreenWireTopFixed;

	if(ProfileSystem::Instance->ActiveProfile.L1S2_IsP1GreenWireDownFixed)
		GreenWireDown->PlayAnimation(2);

	if(ProfileSystem::Instance->ActiveProfile.L1S2_IsP1GreenWireTopFixed)
		GreenWireTop->PlayAnimation(2);

	StaticFuseFixedLeft		= HOScene->GetObjectByID("StatFuseFixLeft");
	StaticFuseFixedLeft->InitPos = StaticFuseFixedLeft->Pos;

	StaticFuseFixedRight	= HOScene->GetObjectByID("StatFuseFixRight");
	StaticFuseFixedRight->InitPos = StaticFuseFixedRight->Pos;

	LightPlus	= HOScene->GetObjectByID("PlusLit");
	LightMinus	= HOScene->GetObjectByID("MinusLit");

	GreenLightUpLeft = HOScene->GetObjectByID("GreenTopL");
	GreenLightUpRight = HOScene->GetObjectByID("GreenTopR");

	GreenLightGlowLeft = HOScene->GetObjectByID("GreenSmallGlowL");
	GreenLightGlowLeft->SetBlendMode(eBlend_AlphaAdditive);
	GreenLightGlowLeft->Animate(0.15f,0.75f,3.75f);
	GreenLightGlowLeft->Show(false);

	GreenLightGlowRight = HOScene->GetObjectByID("GreenSmallGlowR");
	GreenLightGlowRight->SetBlendMode(eBlend_AlphaAdditive);
	GreenLightGlowRight->Animate(0.15f,0.65f,4.05f,1.25f);
	GreenLightGlowRight->Show(false);

	CenterLight = HOScene->GetObjectByID("CenterLightAnim");
	CenterLight->Show(false);

	GreenLightUpLeft->Show(false);
	GreenLightUpRight->Show(false);

	LeverGlow = HOScene->GetObjectByID("LeverGlow");
	LeverGlow->Show(false);
	LeverLiftGlow = HOScene->GetObjectByID("liftTextglow");
	LeverLiftGlow->Show(false);

	Lever = HOScene->GetObjectByID("LeverAnim");

	IsLeverTurningUp = false;
	IsLeverTurningDown = false;
	LeverAnimatingTimer = 0.0f;

	if(!ProfileSystem::Instance->ActiveProfile.L1S2_IsP1LeftFuseFixed)
		StaticFuseFixedLeft->Show(false);

	if(!ProfileSystem::Instance->ActiveProfile.L1S2_IsP1RightFuseFixed)
		StaticFuseFixedRight->Show(false);
	
	if(ProfileSystem::Instance->ActiveProfile.L1S3_IsP1LeftFuseTaken)
		StaticFuse1->Show(false);
	if(ProfileSystem::Instance->ActiveProfile.L1S2_IsP1RightFuseTaken)
		StaticFuse2->Show(false);

	if(!(ProfileSystem::Instance->ActiveProfile.L1S2_IsP1FusePuzzleActive))
		LightPlus->Show(false);
	LightMinus->Show(false);

	RopeGraphic = KPTK::createKGraphic();

	Lightning = HOScene->GetObjectByID("Lightning");
	Lightning->SetBlendMode(eBlend_AlphaAdditive);
	Lightning->SetAlpha(0);
	Lightning->Show(false);
	Lightning->SetScale(2.0f);

	lightningTimer = 0;
	lightFrame = 0;
	nextLightningThreshold = 8.0f;

	if( GFInstance->availablePhyMem >= MAX_LOWEND_PEAKMEMORY )
	{
	}

	//RotatingFuses Stuff
	InitRotatingFuses();
	InitWireFuses();

	InitPuzzle();

	rotateTime	= 0.45f;	//ms
	tweenTime	= 0;

	GFInstance->PostUpdate(1);
	
	//MUSIC_SFX
	Control::Audio->ClearUnusedTracks();

	if( Control::Audio->IsPlaying(aTrackL1Broken) ||
		Control::Audio->IsPlaying(aTrackL1Mansion))
	{
		ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;	
	}

	Control::Audio->LoadSample(aAmbS2PuzFuse,AudioAmb);
	Control::Audio->PlaySample(aAmbS2PuzFuse,true);

	Control::Audio->LoadSample(aSFXL1S3P1Rotate,AudioSFX);
	Control::Audio->LoadSample(aSFXL1S3P1Spark1,AudioSFX);
	Control::Audio->LoadSample(aSFXL1S3P1Spark2,AudioSFX);
		
	Control::Audio->LoadSample(aSFXL1S3P1Spark3,AudioSFX);
	Control::Audio->LoadSample(aSFXL1S3P1Spark4,AudioSFX);
	Control::Audio->LoadSample(aSFXL1S3P1GreenLit,AudioSFX);

	Control::Audio->LoadSample(aSFXL1S3P1Click,AudioSFX);		
	Control::Audio->LoadSample(aSFXL1S3P1FuseFix,AudioSFX);	
	Control::Audio->LoadSample(aSFXL1S3P1LeverTurn,AudioSFX);
	//MUSIC_SFX

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L1_S2_PZFuse]  = true;

	if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S2_PZFuse])
		ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S2_PZFuse] = true;

	
	
	
	//Tutorial show
	if(Hud->Tutor != NULL && Hud->Tutor->IsActive && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly)
	{
		//puzzle is activated only when fuses are fixed
		if(!ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT16_SkipPuzzle] && IsAllStaticFusesFixed)
			Hud->Tutor->ShowTutorial(TUT16_SkipPuzzle);
	}
	Visible = true;//CRenderListEntity
}

void L1_S2_PZFuse::InitRotatingFuses()
{
	RotFuseRects[0] = new CRectangle(497.0f,200.0f,76.0f,91.0f);
	RotFuseRects[1] = new CRectangle(571.0f,103.0f,84.0f,79.0f);
	RotFuseRects[2] = new CRectangle(699.0f,103.0f,78.0f,81.0f);
	RotFuseRects[3] = new CRectangle(772.0f,207.0f,76.0f,83.0f);

	RotFuses[0]		=	HOScene->GetObjectByID("RotFuse1");
	RotFuses[1]		=	HOScene->GetObjectByID("RotFuse2");
	RotFuses[2]		=	HOScene->GetObjectByID("RotFuse3");	
	RotFuses[3]		=	HOScene->GetObjectByID("RotFuse4");

	RotFuseGlows[0]		=	HOScene->GetObjectByID("RotFuseGlow1");
	RotFuseGlows[1]		=	HOScene->GetObjectByID("RotFuseGlow2");
	RotFuseGlows[2]		=	HOScene->GetObjectByID("RotFuseGlow3");	
	RotFuseGlows[3]		=	HOScene->GetObjectByID("RotFuseGlow4");

	RotFuseGlows[0]->SetBlendMode(eBlend_AlphaAdditive);
	RotFuseGlows[1]->SetBlendMode(eBlend_AlphaAdditive);
	RotFuseGlows[2]->SetBlendMode(eBlend_AlphaAdditive);
	RotFuseGlows[3]->SetBlendMode(eBlend_AlphaAdditive);

	RotFuseGlows[0]->Show(false);
	RotFuseGlows[1]->Show(false);
	RotFuseGlows[2]->Show(false);
	RotFuseGlows[3]->Show(false);
}

void L1_S2_PZFuse::InitWireFuses()
{
	IsWireFusesFixCorrect = false;
	IsWireFuseInHand = false;

	wFuseTimer = 0;

	WireFuses[0]	=	HOScene->GetObjectByID("HangFuse1");
	WireFuses[1]	=	HOScene->GetObjectByID("HangFuse2");
	WireFuses[2]	=	HOScene->GetObjectByID("HangFuse3");
	WireFuses[3]	=	HOScene->GetObjectByID("HangFuse4");
	
	for (int i = 0; i < MAX_FUSES; i++)
	{
		IsWireFuseFixed[i] = ProfileSystem::Instance->ActiveProfile.L1S2_IsP1WireFuseFixed[i];
		IsWireFuseFixEmpty[i] = ProfileSystem::Instance->ActiveProfile.L1S2_IsP1WireFuseFixEmpty[i];

		WireFuseConnectedToId[i] = ProfileSystem::Instance->ActiveProfile.L1S2_IsP1WireConnectId[i];

		if( IsWireFuseFixed[i] )
			WireFuses[i]->Show(false);
		WireFuses[i]->InitPos = WireFuses[i]->Pos;

		wFuseRefXPos[i] = WireFuses[i]->Pos.x;
	}

	WireFuseFixed[0]	=	HOScene->GetObjectByID("FixFuse1");
	WireFuseFixed[1]	=	HOScene->GetObjectByID("FixFuse2");
	WireFuseFixed[2]	=	HOScene->GetObjectByID("FixFuse3");
	WireFuseFixed[3]	=	HOScene->GetObjectByID("FixFuse4");

	WireFuseFixedAlt[0]	=	HOScene->GetObjectByID("FixFuseAlt1");
	WireFuseFixedAlt[1]	=	HOScene->GetObjectByID("FixFuseAlt2");
	WireFuseFixedAlt[2]	=	HOScene->GetObjectByID("FixFuseAlt3");
	WireFuseFixedAlt[3]	=	HOScene->GetObjectByID("FixFuseAlt4");

	WireFuseFixed[0]->Show(false);
	WireFuseFixed[1]->Show(false);
	WireFuseFixed[2]->Show(false);
	WireFuseFixed[3]->Show(false);

	WireFuseFixedAlt[0]->Show(false);
	WireFuseFixedAlt[1]->Show(false);
	WireFuseFixedAlt[2]->Show(false);
	WireFuseFixedAlt[3]->Show(false);

	//Rope
	SetZ(15);
	for (int i = 0; i < 4; i++)
	{
		Ropes[i] = new Rope(wFuseStartX[i],wFuseStartY[i],wFuseEndX[i],wFuseEndY[i]);
		Ropes[i]->nodes[Ropes[i]->numOfNodes-1]->x = WireFuses[i]->Pos.x;
		Ropes[i]->nodes[Ropes[i]->numOfNodes-1]->y = WireFuses[i]->Pos.y;
	}

	//if( ProfileSystem::Instance->ActiveProfile.L1S2_IsP1FusePuzzleActive )
	{
		//Take care of WireFuse Fixes only when puzzle is active
		for (int i = 0; i < 4; i++)
		{
			if(IsWireFuseFixed[i])
			{
				//Wire fuse i is fixed
				//Show hide connected Fuse
				int conFuseId = WireFuseConnectedToId[i];
				
				if( conFuseId == i )
				{
					WireFuseFixed[i]->Show(true);

					Ropes[i]->nodes[Ropes[i]->numOfNodes-1]->x = WireFuseFixed[i]->Pos.x;
					Ropes[i]->nodes[Ropes[i]->numOfNodes-1]->y = WireFuseFixed[i]->Pos.y-10;
				}
				else
				{
					WireFuseFixedAlt[conFuseId]->Show(true);

					Ropes[i]->nodes[Ropes[i]->numOfNodes-1]->x = WireFuseFixedAlt[conFuseId]->Pos.x;
					Ropes[i]->nodes[Ropes[i]->numOfNodes-1]->y = WireFuseFixedAlt[conFuseId]->Pos.y-10;
				}
			}
		}
	}

	WireFuseRects[0] = new CRectangle(352.0f,155.0f,70.0f,76.0f);
	WireFuseRects[1] = new CRectangle(426.0f,123.0f,69.0f,73.0f);
	WireFuseRects[2] = new CRectangle(827.0f,110.0f,78.0f,81.0f);
	WireFuseRects[3] = new CRectangle(911.0f,147.0f,61.0f,79.0f);

	WireFuseFixRects[0] = new CRectangle(425.0f,195.0f,77.0f,78.0f);
	WireFuseFixRects[1] = new CRectangle(525.0f,37.0f,84.0f,75.0f);
	WireFuseFixRects[2] = new CRectangle(726.0f,37.0f,83.0f,94.0f);
	WireFuseFixRects[3] = new CRectangle(845.0f,194.0f,81.0f,87.0f);

	WireFuseTakeRects[0] = new CRectangle(433.0f,209.0f,50.0f,44.0f);
	WireFuseTakeRects[1] = new CRectangle(547.0f,54.0f,46.0f,46.0f);
	WireFuseTakeRects[2] = new CRectangle(752.0f,50.0f,44.0f,50.0f);
	WireFuseTakeRects[3] = new CRectangle(856.0f,211.0f,57.0f,47.0f);
}

void L1_S2_PZFuse::InitPuzzle()
{
	IsRotating  = false;
	IsStaticFuseLeftFixed	= ProfileSystem::Instance->ActiveProfile.L1S2_IsP1LeftFuseFixed;
	IsStaticFuseRightFixed	= ProfileSystem::Instance->ActiveProfile.L1S2_IsP1RightFuseFixed;
	IsStaticFuse1InHand		= false;
	IsStaticFuse2InHand		= false;
	IsStaticFuse1Fixed		= ProfileSystem::Instance->ActiveProfile.L1S3_IsP1LeftFuseTaken;
	IsStaticFuse2Fixed		= ProfileSystem::Instance->ActiveProfile.L1S2_IsP1RightFuseTaken;

	IsRotatingFusesCorrect = false;
	
	if(!ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleShuffled)
	{
		/*
		//Disabled Shuffle
		ProfileSystem::Instance->ActiveProfile.L1S2_IsP1RotFuseRot[0] = 1;
		ProfileSystem::Instance->ActiveProfile.L1S2_IsP1RotFuseRot[1] = 1;
		ProfileSystem::Instance->ActiveProfile.L1S2_IsP1RotFuseRot[2] = 1;
		ProfileSystem::Instance->ActiveProfile.L1S2_IsP1RotFuseRot[3] = 2;
		*/

		ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleShuffled = true;
	}

	/*RotFuses[0]->SetRotate(-120.0f);
	RotFuses[1]->SetRotate(-120.0f);
	RotFuses[2]->SetRotate(-120.0f);
	RotFuses[3]->SetRotate(-240.0f);*/

	for( int i = 0; i < MAX_FUSES; i++ )
		RotFuses[i]->SetRotate(-ProfileSystem::Instance->ActiveProfile.L1S2_IsP1RotFuseRot[i]*120.0f);

	if( ProfileSystem::Instance->ActiveProfile.L1S2_IsP1FusePuzzleActive )
		setStatusOfLights();

	//srand((int)time(NULL)); //Random Seed
	//rotatingFuseId = 0;
	//for( int i = 0; i < MAX_FUSES; i++ )
	//{
	//	int rotations = rand()%2+1; //generates (1-2)
	//	RotFuses[i]->SetRotate(-rotations*120.0f); //negative since rotation is in clockwise
	//}
}

L1_S2_PZFuse::~L1_S2_PZFuse()
{

	GFHud->InvBoxCtrl->ResetToInvBox();	

	Control::Audio->Samples[aTrackL1Broken]->SetVolume((float)Control::Audio->GlobalMusicVol);
	Control::Audio->Samples[aTrackL1Mansion]->SetVolume((float)Control::Audio->GlobalMusicVol);
	ShakeObject = NULL;

	SAFE_DELETE(RainBgVFX);
	SAFE_DELETE(SparkVFX);
	SAFE_DELETE(FliesLeftVFX);
	SAFE_DELETE(FliesRightVFX);
	SAFE_DELETE(RaindropsVFX);

	for (int i = 0; i < MAX_FUSES; i++)
	{
		SAFE_DELETE(RotFuseRects[i])
		SAFE_DELETE(Ropes[i])
		SAFE_DELETE(WireFuseRects[i])
		SAFE_DELETE(WireFuseFixRects[i])
		SAFE_DELETE(WireFuseTakeRects[i])
	}

	//MUSIC_SFX
	if( Control::Audio->IsPlaying(aTrackL1Puzzle) )
		Control::Audio->StopSample(aTrackL1Puzzle);
	if( Control::Audio->IsPlaying(aTrackL1Broken) ||
		Control::Audio->IsPlaying(aTrackL1Mansion) )
		ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;

	Control::Audio->StopSample(aAmbS2PuzFuse);
	Control::Audio->UnloadSample(aAmbS2PuzFuse);

	Control::Audio->UnloadSample(aSFXL1S3P1Rotate);
	Control::Audio->UnloadSample(aSFXL1S3P1Spark1);
	Control::Audio->UnloadSample(aSFXL1S3P1Spark2);
		
	Control::Audio->UnloadSample(aSFXL1S3P1Spark3);
	Control::Audio->UnloadSample(aSFXL1S3P1Spark4);
	Control::Audio->UnloadSample(aSFXL1S3P1GreenLit);

	Control::Audio->UnloadSample(aSFXL1S3P1Click);		
	Control::Audio->UnloadSample(aSFXL1S3P1FuseFix);	
	Control::Audio->UnloadSample(aSFXL1S3P1LeverTurn);
	//MUSIC_SFX

	if( GFInstance->availablePhyMem >= MAX_LOWEND_PEAKMEMORY )
	{
	}

	SAFE_DELETE(ResetDia);
}

void L1_S2_PZFuse::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Control::Audio->IsPlaying(aTrackL1Broken) )
	{
		Control::Audio->Samples[aTrackL1Broken]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if(Control::Audio->GlobalMusicVol - musicFadeOutTimer<=0)
		{
			Control::Audio->Samples[aTrackL1Broken]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL1Broken);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL1Puzzle) )
				Control::Audio->PlaySample(aTrackL1Puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 12;
	}
	else if( Control::Audio->IsPlaying(aTrackL1Mansion) )
	{
		Control::Audio->Samples[aTrackL1Mansion]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if(Control::Audio->GlobalMusicVol - musicFadeOutTimer<=0)
		{
			Control::Audio->Samples[aTrackL1Mansion]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL1Mansion);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL1Puzzle) )
				Control::Audio->PlaySample(aTrackL1Puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 12;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL1Puzzle) )
			Control::Audio->PlaySample(aTrackL1Puzzle, true);
	}


	if(IsBgLightAnimationPlaying)
	{
		centerLightTimer += Control::LogicRate;
		if(centerLightTimer>3.5f)
		{
			if (!ProfileSystem::Instance->_isExtrasGamePlay)
			{
				Hud->InvBox_StopPuzzleMode();

				if( !Control::Audio->IsPlaying(aTrackL1Mansion) )
					Control::Audio->PlaySample(aTrackL1Mansion,false);

				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S2_Frontyard;
				SendMessage(ControlExit, ID, HOSM_NewArea); 
			}
		}
		return;
	}

	if( Hud->PopupState != Popup_Inactive )
		return;
}



void L1_S2_PZFuse::ReceiveMessage(int val)
{	
	if( val == MAP_NEW_AREA ) 
	{
		Hud->InvBox_StopPuzzleMode();
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
			GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{
		
		
	}	
}

void L1_S2_PZFuse::PlayRandomSparkSFX()
{
	int randVal = rand()%4;
	switch (randVal)
	{
	case 0: Control::Audio->PlaySample(aSFXL1S3P1Spark1,false); break;
	case 1: Control::Audio->PlaySample(aSFXL1S3P1Spark2,false); break;
	case 2: Control::Audio->PlaySample(aSFXL1S3P1Spark3,false); break;
	default: Control::Audio->PlaySample(aSFXL1S3P1Spark4,false); break;
	}
}

void L1_S2_PZFuse::Shake(CObject *obj)
{
	if(!IsShaking)
	{
		Control::Audio->PlaySample(aSFXL1S3P1Click,false);
		IsShaking = true;
		ShakeObject = obj;
		ShakeRotValue = ShakeObject->GetRotate();
		ShakeTimer = 0;
	}
}

void L1_S2_PZFuse::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	if(Hud->Tutor != NULL && Hud->Tutor->IsActive)
	{
		if(Hud->Tutor->State == TUT16_SkipPuzzle && Hud->Tutor->IsVisible)
		{
			return;
		}
	}

	if(Hud->IsMapShowing)
		return;

	Cursor::SetMode(CA_Normal);
	
	ResetPuzzle();

	if (IsResetPop)
	{
		GFHud->AllowInput = false;
		return;
	}
	else
		GFHud->AllowInput = true;


	if(IsBgLightAnimationPlaying)
	{
		return;
	}

	wFuseTimer += 100*Control::LogicRate;
	for (int i = 0; i < MAX_FUSES; i++)
	{
		WireFuses[i]->Pos.x = wFuseRefXPos[i] + Elixir::Sin(wFuseTimer+wFusePhase[i])*4.0f;
	}

	lightningTimer += Control::LogicRate;
	if(lightningTimer>nextLightningThreshold)
	{
		Lightning->Show(true);
		if(lightFrame==0){
			if(rand()%2>1)
				Control::Audio->PlaySample(aSFXL1ThunderV1, false);
			else
				Control::Audio->PlaySample(aSFXL1ThunderV2, false);
		}
		lightFrame += Control::LogicRate/2;
		int index = (int)(lightFrame*20.0f);
		if(lightFrame>=1.0f)
		{
			lightFrame = 0;
			index = 0;
			lightningTimer = 0;

			nextLightningThreshold = 12.0f + (float)(rand()%8);
			Lightning->Show(false);
		}
		Lightning->SetAlpha(lightAlphaArray3[index]);
	}

	if(IsLeverTurningUp || IsLeverTurningDown)
	{
		if( Lever->IsAnimationFinished() )
		{
			if(IsLeverTurningUp)
			{
				if(IsWireFusesFixCorrect && IsRotatingFusesCorrect)
				{					
					Control::Audio->PlaySample(aSFXL1S3P1PowerOn,false);

					ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved = true;
					ProfileSystem::Instance->ActiveProfile.L1_IsMansionPowerOn = true;
					CenterLight->ForceShow();					
					CenterLight->PlayAnimation(1);

					IsBgLightAnimationPlaying = true;

					if (_handleExtrasGameComplete())
					{
						return;
					}

					Hud->ShowBannerText("L1power",SFX_EMPTY_NOTIFY);
					AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);

					//Show Objective completed
					if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L1O2_LitTheMansion] && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L1O2_LitTheMansion] )
					{
						ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L1O2_LitTheMansion] = true;
						Hud->ShowObjectiveFX(eObj_L1O2_LitTheMansion,true);
					}
				}
				else
				{
					Control::Audio->PlaySample(aSFXL1S3P1PowerOff,false);
					IsLeverTurningDown = true;
					IsLeverTurningUp = false;

					Lever->PlayAnimation(2);
				}
			}
			else if(IsLeverTurningDown)
			{
				IsLeverTurningDown = false;
				//Show messages or game complete

				if(IsAllStaticFusesFixed)
				{
					//Should rotate the fuses
					if( !IsRotatingFusesCorrect )
						Hud->ShowBannerText("L1connection",SFX_EMPTY_NOTIFY);
					else if( !IsWireFusesFixCorrect )
						Hud->ShowBannerText("L1negative",SFX_EMPTY_NOTIFY);
				}
				else
				{
					if(IsStaticFuse1Fixed && IsStaticFuse2Fixed)
						Hud->ShowBannerText("L1wires",SFX_EMPTY_NOTIFY);//Fuses fixed. Now connect green wires
					else
						Hud->ShowBannerText("L1fuses",SFX_EMPTY_NOTIFY);//should fix the fuses first
				}
			}
		}
		return;
	}

	CPoint pos		= Control::Input->Mpos();
	bool clicked	= Control::Input->LBclicked();
	bool Rclicked	= Control::Input->RBclicked();

	if(IsShaking)
	{
		ShakeTimer += 2.0f*Control::LogicRate;

		ShakeObject->SetRotate(ShakeRotValue+Elixir::Sin(ShakeTimer*450)*8.0f);
		if(ShakeTimer>0.8f)
		{
			IsShaking = false;
			ShakeObject->SetRotate(ShakeRotValue);
			ShakeObject = NULL;
		}
	}

	if(LeverRect.Intersection(&pos))
	{
		LeverGlow->Show(true);
		LeverLiftGlow->Show(false);
		Cursor::SetMode(CA_HandTake);
		if(clicked)
		{
			Control::Audio->PlaySample(aSFXL1S3P1LeverTurn,false);

			LeverGlow->Show(false);
			LeverLiftGlow->Show(false);

			Lever->PlayAnimation(1);
			IsLeverTurningUp = true;
		}
	}
	else
	{
		LeverGlow->Show(false);
		LeverLiftGlow->Show(false);
	}

	//Check if Static Fuses are Fixed
	if( IsAllStaticFusesFixed )
	{
		sparkWireTimer += Control::LogicRate;
		if(sparkWireTimer>sparkWireTimeGap)
		{
			sparkWireTimer = 0;
			sparkWireTimeGap = 3.0f+(rand()%10)/10.0f*4.0f;;

			if(FuseSpark2->IsActive)
				PlayRandomSparkSFX();
			FuseSpark2->ForcePlayAnimation(1);
		}

		sparkTimer += Control::LogicRate;
		if(sparkTimer>sparkTimeGap)
		{
			sparkTimer = 0;
			sparkTimeGap = 3.0f+(rand()%10)/10.0f*4.0f; //from 3 to 7 seconds timegap

			if(FuseSpark1->IsActive)
				PlayRandomSparkSFX();
			FuseSpark1->ForcePlayAnimation(1);
			//FuseSparkGlow1->ForcePlayAnimation(1);
		}

		//Check if Centre Rotating Fuses are aligned Correctly
		//Check if Wired Fuses are placed correctly
		if(!IsWireFuseInHand)
		{
			for (int i = 0; i < MAX_FUSES; i++)
			{
				if(WireFuseRects[i]->Intersection(&pos) && !IsWireFuseFixed[i])
				{
					WireFuses[i]->SetTint(1.0f,0.94f,0.73f);
					Cursor::SetMode(CA_HandPointy);
					if(clicked && !IsRotating)
					{
						IsWireFuseInHand = true;
						wireFuseTakenId = i;
					}
					break;
				}
				else if(IsWireFuseFixed[i] && WireFuseTakeRects[WireFuseConnectedToId[i]]->Intersection(&pos))
				{
					Cursor::SetMode(CA_HandPointy);
					if(clicked && !IsRotating)
					{
						Control::Audio->PlaySample(aSFXL1S3P1Click,false);

						//remove the Fuse
						WireFuses[i]->Show(true);
						WireFuses[i]->SetTint(1.0f,1.0f,1.0f);

						IsWireFuseFixed[i] = false;
						ProfileSystem::Instance->ActiveProfile.L1S2_IsP1WireFuseFixed[i] = false;

						int fixFuseId = WireFuseConnectedToId[i];
						IsWireFuseFixEmpty[fixFuseId]=true;
						ProfileSystem::Instance->ActiveProfile.L1S2_IsP1WireFuseFixEmpty[fixFuseId] = true;
						if(fixFuseId==i)
							WireFuseFixed[fixFuseId]->Show(false);
						else
							WireFuseFixedAlt[fixFuseId]->Show(false);

						WireFuseConnectedToId[i] = -1;

						IsWireFusesFixCorrect = false;

						WireFuses[i]->SetPos(&WireFuses[i]->InitPos);
						Ropes[i]->nodes[Ropes[i]->numOfNodes-1]->x = WireFuses[i]->Pos.x;
						Ropes[i]->nodes[Ropes[i]->numOfNodes-1]->y = WireFuses[i]->Pos.y-10;

						setStatusOfLights();
					}

					break;
				}
				else
				{
					WireFuses[i]->SetTint(1.0f,1.0f,1.0f);
				}
			}

			if(!IsWireFuseInHand)
			{
				//Check and Update Rotating Fuses here
				int i;
				for( i = 0; i < MAX_FUSES; i++ )
				{
					if(RotFuseRects[i]->Intersection(&pos))
					{
						Cursor::SetMode(CA_HandPointy);
						RotFuseGlows[i]->Show(true);
						if(clicked && !IsRotating)
						{
							if(IsWireFuseFixEmpty[i])
							{
								Control::Audio->QuickLoadAndPlaySFX(aSFXL1S2FuseRotate);

								IsRotating = true;
								rotatingFuseId = i;
								startAngle = RotFuses[i]->GetRotate();
								endAngle = startAngle - 120;
								RotFuseGlows[i]->Show(false);
							}
							else
							{
								Hud->ShowBannerText("L1rotate");
							}
						}
						break;
					}
					else
						RotFuseGlows[i]->Show(false);
				}
			}
		}
		else
		{
			//Fuse is in hand - Check if placed in right position
			//Check boundary
			bool IsInsideBoundary=false;
			if(wireFuseTakenId<2)//for 0 & 1
			{
				if(LeftDragArea.Intersection(&pos))
				{
					//Boundary check
					IsInsideBoundary = true;
				}
			}
			else
			{
				if(RightDragArea.Intersection(&pos))
				{
					//Boundary check
					IsInsideBoundary = true;
				}
			}

			if(IsInsideBoundary)
			{
				WireFuses[wireFuseTakenId]->SetXY(pos.x,pos.y);
				wFuseEndX[wireFuseTakenId] = pos.x;
				wFuseEndY[wireFuseTakenId] = pos.y-20;

				Ropes[wireFuseTakenId]->nodes[Ropes[wireFuseTakenId]->numOfNodes-1]->x = pos.x;
				Ropes[wireFuseTakenId]->nodes[Ropes[wireFuseTakenId]->numOfNodes-1]->y = pos.y-10;

				for (int i = 0; i < MAX_FUSES; i++)
				{
					if( WireFuseFixRects[i]->Intersection(&pos) && IsWireFuseFixEmpty[i])
					{
						RotFuseGlows[i]->Show(true);
					}
					else
						RotFuseGlows[i]->Show(false);
				}

				if(clicked)
				{
					for (int i = 0; i < MAX_FUSES; i++)
					{
						RotFuseGlows[i]->Show(false);
					}
					Control::Audio->PlaySample(aSFXL1S3P1FuseFix,false);
					CheckWireFusePlacements(pos);
				}	
			}
			else
			{
				Hud->ShowBannerText("L1short");

				WireFuses[wireFuseTakenId]->SetPos(&WireFuses[wireFuseTakenId]->InitPos);
				Ropes[wireFuseTakenId]->nodes[Ropes[wireFuseTakenId]->numOfNodes-1]->x = WireFuses[wireFuseTakenId]->Pos.x;
				Ropes[wireFuseTakenId]->nodes[Ropes[wireFuseTakenId]->numOfNodes-1]->y = WireFuses[wireFuseTakenId]->Pos.y-10;
				IsWireFuseInHand = false;
			}

			if(Rclicked)
			{
				WireFuses[wireFuseTakenId]->SetPos(&WireFuses[wireFuseTakenId]->InitPos);
				Ropes[wireFuseTakenId]->nodes[Ropes[wireFuseTakenId]->numOfNodes-1]->x = WireFuses[wireFuseTakenId]->Pos.x;
				Ropes[wireFuseTakenId]->nodes[Ropes[wireFuseTakenId]->numOfNodes-1]->y = WireFuses[wireFuseTakenId]->Pos.y-10;
				IsWireFuseInHand = false;
			}
		}
	}
	else
	{
		sparkTimer += Control::LogicRate;
		if( (!IsStaticFuseLeftFixed || !IsStaticFuseRightFixed) && sparkTimer>sparkTimeGap)
		{
			sparkTimer = 0;
			sparkTimeGap = 3.0f+(rand()%10)/10.0f*4.0f; //from 3 to 7 seconds timegap

			PlayRandomSparkSFX();
			if(!IsStaticFuseLeftFixed && !IsStaticFuseRightFixed)
			{
				if(sparkTimeGap>4.0f)
				{
					FuseSpark1->SetEnable(true);					
					FuseSpark1->SetPos(&LeftFusePos);	
					FuseSpark1->ForcePlayAnimation(1);
		
					FuseSparkGlow1->SetPos(&LeftFusePos);
					FuseSparkGlow1->ForcePlayAnimation(1);
				}
				else
				{
					FuseSpark1->SetEnable(true);

					FuseSpark1->SetPos(&RightFusePos);
					FuseSpark1->ForcePlayAnimation(1);	

					FuseSparkGlow1->SetPos(&RightFusePos);
					FuseSparkGlow1->ForcePlayAnimation(1);
				}
			}
			else if(IsStaticFuseLeftFixed)
			{
				FuseSpark1->SetEnable(true);

				FuseSpark1->SetPos(&RightFusePos);	
				FuseSpark1->ForcePlayAnimation(1);

				FuseSparkGlow1->SetPos(&RightFusePos);
				FuseSparkGlow1->ForcePlayAnimation(1);

			}
			else if(IsStaticFuseRightFixed)
			{
				FuseSpark1->SetEnable(true);
				FuseSpark1->SetPos(&LeftFusePos);	
				FuseSpark1->ForcePlayAnimation(1);

				FuseSparkGlow1->SetPos(&LeftFusePos);
				FuseSparkGlow1->ForcePlayAnimation(1);
			}
		}

		//if green wires are not fixed
		if( !IsGreenWireDownFixed || !IsGreenWireTopFixed )
		{
			greenSparkTimer += Control::LogicRate;
			if(greenSparkTimer>greenSparkTimeGap)
			{
				greenSparkTimer = 0;
				greenSparkTimeGap = 3.0f+(rand()%10)/10.0f*4.0f; //from 3 to 7 seconds timegap

				PlayRandomSparkSFX();
				if(!IsGreenWireDownFixed && !IsGreenWireTopFixed)
				{
					if(greenSparkTimeGap>5.0f)
					{
						FuseSpark2->SetEnable(true);					
						FuseSpark2->SetPos(&GreenWireDownPos);	
						FuseSpark2->ForcePlayAnimation(1);

						FuseSparkGlow2->SetPos(&GreenWireDownPos);
						FuseSparkGlow2->ForcePlayAnimation(1);
					}
					else
					{
						FuseSpark2->SetEnable(true);
						FuseSpark2->SetPos(&GreenWireTopPos);
						FuseSpark2->ForcePlayAnimation(1);

						FuseSparkGlow2->SetPos(&GreenWireTopPos);
						FuseSparkGlow2->ForcePlayAnimation(1);
					}
				}
				else if(IsGreenWireDownFixed)
				{
					FuseSpark2->SetEnable(true);
					FuseSpark2->SetPos(&GreenWireTopPos);	
					FuseSpark2->ForcePlayAnimation(1);

					FuseSparkGlow2->SetPos(&GreenWireTopPos);
					FuseSparkGlow2->ForcePlayAnimation(1);
				}
				else if(IsGreenWireTopFixed)
				{
					FuseSpark2->SetEnable(true);
					FuseSpark2->SetPos(&GreenWireDownPos);	
					FuseSpark2->ForcePlayAnimation(1);

					FuseSparkGlow2->SetPos(&GreenWireDownPos);
					FuseSparkGlow2->ForcePlayAnimation(1);
				}
			}

			if(!IsGreenWireDownFixed)
			{
				if (GreenWireDownRect.Intersection(&pos))
				{
					Cursor::SetMode(CA_HandPointy);
					GreenWireDown->PlayAnimation(1);
					if( clicked )
					{
						GreenWireDown->PlayAnimation(2);
						IsGreenWireDownFixed = true;

						Control::Audio->PlaySample(aSFXL1S3P1FuseFix,false);
						ProfileSystem::Instance->ActiveProfile.L1S2_IsP1GreenWireDownFixed = true;

						if( IsGreenWireTopFixed && IsStaticFuseLeftFixed && IsStaticFuseRightFixed )
						{
							//Tutorial show
							if(Hud->Tutor != NULL && Hud->Tutor->IsActive && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly)
							{
								//puzzle is activated only when fuses are fixed
								if(!ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT16_SkipPuzzle])
									Hud->Tutor->ShowTutorial(TUT16_SkipPuzzle);
							}

							IsAllStaticFusesFixed = true;

							Hud->InvBox_StartPuzzleMode();
							ProfileSystem::Instance->ActiveProfile.L1S2_IsP1FusePuzzleActive = true;
							//Both fuses are now fixed
							LightPlus->Show(true);

							CenterLight->SetAlpha(1.0f);
							CenterLight->Show(true);
							CenterLight->FadeOut(1.0f);
							Control::Audio->PlaySample(aSFXL1S3P1PowerOff,false);

							FuseSpark2->SetEnable(false);

						}

						SparkVFX->SetPos(&GreenWireDownPos);
						//spark correction
						SparkVFX->Pos.y -= 50;
						SparkVFX->Start();
					} 
				}
				else
				{
					GreenWireDown->PlayAnimation(0);
				}
			}
			if(!IsGreenWireTopFixed)
			{
				if (GreenWireTopRect.Intersection(&pos))
				{
					Cursor::SetMode(CA_HandPointy);
					GreenWireTop->PlayAnimation(1);
					if( clicked )
					{
						GreenWireTop->PlayAnimation(2);
						IsGreenWireTopFixed = true;

						Control::Audio->PlaySample(aSFXL1S3P1FuseFix,false);
						ProfileSystem::Instance->ActiveProfile.L1S2_IsP1GreenWireTopFixed = true;

						if( IsGreenWireDownFixed && IsStaticFuseLeftFixed && IsStaticFuseRightFixed )
						{
							//Tutorial show
							if(Hud->Tutor != NULL && Hud->Tutor->IsActive && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly)
							{
								//puzzle is activated only when fuses are fixed
								if(!ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT16_SkipPuzzle])
									Hud->Tutor->ShowTutorial(TUT16_SkipPuzzle);
							}

							IsAllStaticFusesFixed = true;

							Hud->InvBox_StartPuzzleMode();
							ProfileSystem::Instance->ActiveProfile.L1S2_IsP1FusePuzzleActive = true;
							//Both fuses are now fixed
							LightPlus->Show(true);

							CenterLight->SetAlpha(1.0f);
							CenterLight->Show(true);
							CenterLight->FadeOut(1.0f);
							Control::Audio->PlaySample(aSFXL1S3P1PowerOff,false);

							FuseSpark2->SetEnable(false);

							setStatusOfLights();
						}

						SparkVFX->SetPos(&GreenWireTopPos);
						//spark correction
						SparkVFX->Pos.y -= 50;
						SparkVFX->Start();
					} 
				}
				else
				{
					GreenWireTop->PlayAnimation(0);
				}
			}
		}

		//Check if Static fuses are handled
		//Static Fuse 1
		for (int i = 0; i < MAX_FUSES; i++)
		{
			if(WireFuseRects[i]->Intersection(&pos) || RotFuseRects[i]->Intersection(&pos))
			{
				Cursor::SetMode(CA_HandPointy);
				if(clicked)
				{
					if(RotFuseRects[i]->Intersection(&pos))
						Shake(RotFuses[i]);
					if(IsStaticFuse1Fixed && IsStaticFuse2Fixed)
						Hud->ShowBannerText("L1wires",SFX_EMPTY_NOTIFY);//Fuses fixed. Now connect green wires
					else
						Hud->ShowBannerText("L1fuses",SFX_EMPTY_NOTIFY);
				}
				break;
			}
		}

		

		if(!IsGreenWireDownFixed && GreenWireDownDestRect.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if(clicked)
				Hud->ShowBannerText("L1wires");
		}

		if(!IsGreenWireTopFixed && GreenWireTopDestRect.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if(clicked)
				Hud->ShowBannerText("L1wires");
		}
		if(!IsStaticFuseRightFixed && StatFuseFixRightRect.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if(clicked && !IsStaticFuse1InHand && !IsStaticFuse2InHand)
				Hud->ShowBannerText("L1remove");
		}

		if(!IsStaticFuseLeftFixed && StatFuseFixLeftRect.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if(clicked && !IsStaticFuse1InHand && !IsStaticFuse2InHand )
				Hud->ShowBannerText("L1remove");
		}
		if(!IsStaticFuseRightFixed && StatFuseFixRightRect.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if(clicked && !IsStaticFuse1InHand && !IsStaticFuse2InHand)
				Hud->ShowBannerText("L1remove");
		}

		if(!IsStaticFuse1Fixed && !IsStaticFuse2InHand)
		{
			if(!IsStaticFuse1InHand)
			{
				if(StatFuse1Rect.Intersection(&pos))
				{
					Cursor::SetMode(CA_HandPointy);
					if(clicked)
					{
						IsStaticFuse1InHand = true;
						StaticFuse1->Show(false);
						if(!IsStaticFuseLeftFixed)
							StaticFuseTaken = StaticFuseFixedLeft;
						else if(!IsStaticFuseRightFixed)
							StaticFuseTaken = StaticFuseFixedRight;
						StaticFuseTaken->SetXY(pos.x,pos.y);
						StaticFuseTaken->Show(true);
					}
				}
			}
			else
			{
				//Fuse 1 is in hand
				Cursor::SetMode(CA_HandPointy);
				StaticFuseTaken->SetXY(pos.x,pos.y);
				if(IsMouseOverGoBackArea(&pos))
				{
					IsStaticFuse1InHand = false;
					StaticFuseTaken->Show(false);
					StaticFuse1->Show(true);
					return;
				}

				if(clicked)
				{
					if(!IsStaticFuseLeftFixed && StatFuseFixLeftRect.Intersection(&pos))
					{
						Control::Audio->PlaySample(aSFXL1S3P1FuseFix,false);

						IsStaticFuse1Fixed = true;
						IsStaticFuse1InHand = false;
						IsStaticFuseLeftFixed = true;

						StaticFuseTaken->Show(false);

						StaticFuseFixedLeft->Show(true);
						StaticFuseFixedLeft->SetPos(&StaticFuseFixedLeft->InitPos);

						ProfileSystem::Instance->ActiveProfile.L1S3_IsP1LeftFuseTaken = true;
						ProfileSystem::Instance->ActiveProfile.L1S2_IsP1LeftFuseFixed = true;

						StaticFuseTaken = NULL;
						FuseSpark1->SetEnable(false);

						if(IsStaticFuseRightFixed && IsGreenWireDownFixed && IsGreenWireTopFixed)
						{
							//Tutorial show
							if(Hud->Tutor != NULL && Hud->Tutor->IsActive && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly)
							{
								//puzzle is activated only when fuses are fixed
								if(!ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT16_SkipPuzzle])
									Hud->Tutor->ShowTutorial(TUT16_SkipPuzzle);
							}

							IsAllStaticFusesFixed = true;

							Hud->InvBox_StartPuzzleMode();
							ProfileSystem::Instance->ActiveProfile.L1S2_IsP1FusePuzzleActive = true;
							//Both fuses are now fixed
							LightPlus->Show(true);

							CenterLight->SetAlpha(1.0f);
							CenterLight->Show(true);
							CenterLight->FadeOut(1.0f);
							Control::Audio->PlaySample(aSFXL1S3P1PowerOff,false);
						}
					}
					else if(!IsStaticFuseRightFixed && StatFuseFixRightRect.Intersection(&pos))
					{
						Control::Audio->PlaySample(aSFXL1S3P1FuseFix,false);

						IsStaticFuse1Fixed = true;
						IsStaticFuse1InHand = false;
						IsStaticFuseRightFixed = true;

						StaticFuseTaken->Show(false);

						StaticFuseFixedRight->Show(true);
						StaticFuseFixedRight->SetPos(&StaticFuseFixedRight->InitPos);
						
						ProfileSystem::Instance->ActiveProfile.L1S3_IsP1LeftFuseTaken = true;

						ProfileSystem::Instance->ActiveProfile.L1S2_IsP1RightFuseFixed = true;

						StaticFuseTaken = NULL;
						FuseSpark1->SetEnable(false);

						if(IsStaticFuseLeftFixed && IsGreenWireDownFixed && IsGreenWireTopFixed)
						{
							//Tutorial show
							if(Hud->Tutor != NULL && Hud->Tutor->IsActive && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly)
							{
								//puzzle is activated only when fuses are fixed
								if(!ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT16_SkipPuzzle])
									Hud->Tutor->ShowTutorial(TUT16_SkipPuzzle);
							}

							IsAllStaticFusesFixed = true;

							Hud->InvBox_StartPuzzleMode();
							ProfileSystem::Instance->ActiveProfile.L1S2_IsP1FusePuzzleActive = true;
							//Both fuses are now fixed
							LightPlus->Show(true);

							CenterLight->SetAlpha(1.0f);
							CenterLight->Show(true);
							CenterLight->FadeOut(1.0f);
							Control::Audio->PlaySample(aSFXL1S3P1PowerOff,false);
						}
					}
				}
				else if(Rclicked)
				{
					IsStaticFuse1InHand = false;
					StaticFuseTaken->Show(false);
					StaticFuse1->Show(true);
				}
			}
		}
		//Static Fuse 2
		if(!IsStaticFuse2Fixed && !IsStaticFuse1InHand)
		{
			if(!IsStaticFuse2InHand)
			{
				if( StatFuse2Rect.Intersection(&pos) )
				{
					Cursor::SetMode(CA_HandPointy);
					if(clicked)
					{
						IsStaticFuse2InHand = true;
						StaticFuse2->Show(false);
						if(!IsStaticFuseLeftFixed)
							StaticFuseTaken = StaticFuseFixedLeft;
						else if(!IsStaticFuseRightFixed)
							StaticFuseTaken = StaticFuseFixedRight;
						StaticFuseTaken->SetXY(pos.x,pos.y);
						StaticFuseTaken->Show(true);
					}
				}
			}
			else
			{
				//Fuse 2 is in hand
				Cursor::SetMode(CA_HandPointy);
				StaticFuseTaken->SetXY(pos.x,pos.y);
				if(IsMouseOverGoBackArea(&pos))
				{
					IsStaticFuse2InHand = false;
					StaticFuseTaken->Show(false);
					StaticFuse2->Show(true);
					return;
				}
				if(clicked)
				{
					if(!IsStaticFuseLeftFixed && StatFuseFixLeftRect.Intersection(&pos))
					{
						Control::Audio->PlaySample(aSFXL1S3P1FuseFix,false);

						IsStaticFuse2Fixed = true;
						IsStaticFuse2InHand = false;
						IsStaticFuseLeftFixed = true;

						StaticFuseTaken->Show(false);

						StaticFuseFixedLeft->Show(true);

						StaticFuseFixedLeft->SetPos(&StaticFuseFixedLeft->InitPos);

						ProfileSystem::Instance->ActiveProfile.L1S2_IsP1RightFuseTaken = true;

						ProfileSystem::Instance->ActiveProfile.L1S2_IsP1LeftFuseFixed = true;
						FuseSpark1->SetEnable(false);
						StaticFuseTaken = NULL;

						if(IsStaticFuseRightFixed && IsGreenWireDownFixed && IsGreenWireTopFixed)
						{
							//Tutorial show
							if(Hud->Tutor != NULL && Hud->Tutor->IsActive && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly)
							{
								//puzzle is activated only when fuses are fixed
								if(!ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT16_SkipPuzzle])
									Hud->Tutor->ShowTutorial(TUT16_SkipPuzzle);
							}

							IsAllStaticFusesFixed = true;

							Hud->InvBox_StartPuzzleMode();
							ProfileSystem::Instance->ActiveProfile.L1S2_IsP1FusePuzzleActive = true;
							//Both fuses are now fixed
							LightPlus->Show(true);

							CenterLight->SetAlpha(1.0f);
							CenterLight->Show(true);
							CenterLight->FadeOut(1.0f);
							Control::Audio->PlaySample(aSFXL1S3P1PowerOff,false);
						}
					}
					else if(!IsStaticFuseRightFixed && StatFuseFixRightRect.Intersection(&pos))
					{
						Control::Audio->PlaySample(aSFXL1S3P1FuseFix,false);

						IsStaticFuse2Fixed = true;
						IsStaticFuse2InHand = false;
						IsStaticFuseRightFixed = true;

						StaticFuseTaken->Show(false);

						StaticFuseFixedRight->Show(true);
						
						StaticFuseFixedRight->SetPos(&StaticFuseFixedRight->InitPos);

						ProfileSystem::Instance->ActiveProfile.L1S2_IsP1RightFuseTaken = true;

						ProfileSystem::Instance->ActiveProfile.L1S2_IsP1RightFuseFixed = true;
						FuseSpark1->SetEnable(false);
						StaticFuseTaken = NULL;

						if(IsStaticFuseLeftFixed && IsGreenWireDownFixed && IsGreenWireTopFixed)
						{
							//Tutorial show
							if(Hud->Tutor != NULL && Hud->Tutor->IsActive && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly)
							{
								//puzzle is activated only when fuses are fixed
								if(!ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT16_SkipPuzzle])
									Hud->Tutor->ShowTutorial(TUT16_SkipPuzzle);
							}

							IsAllStaticFusesFixed = true;

							Hud->InvBox_StartPuzzleMode();
							ProfileSystem::Instance->ActiveProfile.L1S2_IsP1FusePuzzleActive = true;
							//Both fuses are now fixed
							LightPlus->Show(true);

							CenterLight->SetAlpha(1.0f);
							CenterLight->Show(true);
							CenterLight->FadeOut(1.0f);
							Control::Audio->PlaySample(aSFXL1S3P1PowerOff,false);
						}
					}
				}
				else if(Rclicked)
				{
					IsStaticFuse2InHand = false;
					StaticFuseTaken->Show(false);
					StaticFuse2->Show(true);
				}
			}
		}
	}

	if(IsRotating)
	{
		tweenTime += Control::LogicRate;

		float angle = startAngle+(endAngle-startAngle)*easing(tweenTime,0,1,rotateTime);
		RotFuses[rotatingFuseId]->SetRotate(angle);

		if(tweenTime>=rotateTime)
		{
			if(endAngle<-350) //ie -360, change to zero, for win condition checking
			{
					endAngle = 0;
			}
			RotFuses[rotatingFuseId]->SetRotate(endAngle);

			ProfileSystem::Instance->ActiveProfile.L1S2_IsP1RotFuseRot[rotatingFuseId] = int(-endAngle/120.0f);

			tweenTime = 0;
			IsRotating = false;
			//IsKnobRotation = false;

			setStatusOfLights();
		}
	}

	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			Hud->InvBox_StopPuzzleMode();
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S2_Frontyard;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}
	#endif
	if( IsMouseOverGoBackArea(&pos) )
	{
#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
			//Cursor::SetMode(CA_ExitDown,eArea_L1_S2_Frontyard);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L1_S2_Frontyard);
#endif
		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L1_S2_Frontyard, 2);
			}
#else
			Hud->InvBox_StopPuzzleMode();
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S2_Frontyard;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}
	}
}

void L1_S2_PZFuse::CheckWireFusePlacements(CPoint pos)
{
	if(wireFuseTakenId==0)
	{
		if(WireFuseFixRects[0]->Intersection(&pos) && IsWireFuseFixEmpty[0])
		{
			WireFuses[wireFuseTakenId]->Show(false);
			WireFuseFixed[wireFuseTakenId]->Show(true);

			WireFuseConnectedToId[0] = 0;

			IsWireFuseFixed[wireFuseTakenId] = true;
			IsWireFuseFixEmpty[0] = false;

			IsWireFuseInHand = false;

			Ropes[0]->nodes[Ropes[0]->numOfNodes-1]->x = WireFuseFixed[0]->Pos.x;
			Ropes[0]->nodes[Ropes[0]->numOfNodes-1]->y = WireFuseFixed[0]->Pos.y-10;
		}
		else if(WireFuseFixRects[1]->Intersection(&pos) && IsWireFuseFixEmpty[1])
		{
			WireFuses[wireFuseTakenId]->Show(false);
			WireFuseFixedAlt[1]->Show(true);

			WireFuseConnectedToId[0] = 1;
					
			IsWireFuseFixed[wireFuseTakenId] = true;
			IsWireFuseFixEmpty[1] = false;

			Ropes[0]->nodes[Ropes[0]->numOfNodes-1]->x = WireFuseFixedAlt[1]->Pos.x;
			Ropes[0]->nodes[Ropes[0]->numOfNodes-1]->y = WireFuseFixedAlt[1]->Pos.y-10;

			IsWireFuseInHand = false;
		}
	}
	else if(wireFuseTakenId==1)
	{
		if(WireFuseFixRects[0]->Intersection(&pos) && IsWireFuseFixEmpty[0])
		{
			WireFuses[wireFuseTakenId]->Show(false);
			WireFuseFixedAlt[0]->Show(true);

			WireFuseConnectedToId[1] = 0;
					
			IsWireFuseFixed[wireFuseTakenId] = true;
			IsWireFuseFixEmpty[0] = false;

			Ropes[1]->nodes[Ropes[1]->numOfNodes-1]->x = WireFuseFixedAlt[0]->Pos.x;
			Ropes[1]->nodes[Ropes[1]->numOfNodes-1]->y = WireFuseFixedAlt[0]->Pos.y-10;

			IsWireFuseInHand = false;
		}
		else if(WireFuseFixRects[1]->Intersection(&pos) && IsWireFuseFixEmpty[1])
		{
			WireFuses[wireFuseTakenId]->Show(false);
			WireFuseFixed[1]->Show(true);

			WireFuseConnectedToId[1] = 1;
					
			IsWireFuseFixed[wireFuseTakenId] = true;
			IsWireFuseFixEmpty[1] = false;

			Ropes[1]->nodes[Ropes[1]->numOfNodes-1]->x = WireFuseFixed[1]->Pos.x;
			Ropes[1]->nodes[Ropes[1]->numOfNodes-1]->y = WireFuseFixed[1]->Pos.y-10;

			IsWireFuseInHand = false;
		}					
	}
	else if(wireFuseTakenId==2)
	{
		if(WireFuseFixRects[2]->Intersection(&pos) && IsWireFuseFixEmpty[2])
		{
			WireFuses[wireFuseTakenId]->Show(false);
			WireFuseFixed[2]->Show(true);

			WireFuseConnectedToId[2] = 2;
					
			IsWireFuseFixed[wireFuseTakenId] = true;
			IsWireFuseFixEmpty[2] = false;

			Ropes[2]->nodes[Ropes[2]->numOfNodes-1]->x = WireFuseFixed[2]->Pos.x;
			Ropes[2]->nodes[Ropes[2]->numOfNodes-1]->y = WireFuseFixed[2]->Pos.y-10;

			IsWireFuseInHand = false;
		}
		else if(WireFuseFixRects[3]->Intersection(&pos) && IsWireFuseFixEmpty[3])
		{
			WireFuses[wireFuseTakenId]->Show(false);
			WireFuseFixedAlt[3]->Show(true);

			WireFuseConnectedToId[2] = 3;
					
			IsWireFuseFixed[wireFuseTakenId] = true;
			IsWireFuseFixEmpty[3] = false;

			Ropes[2]->nodes[Ropes[2]->numOfNodes-1]->x = WireFuseFixedAlt[3]->Pos.x;
			Ropes[2]->nodes[Ropes[2]->numOfNodes-1]->y = WireFuseFixedAlt[3]->Pos.y-10;

			IsWireFuseInHand = false;
		}					
	}
	else if(wireFuseTakenId==3)
	{
		if(WireFuseFixRects[2]->Intersection(&pos) && IsWireFuseFixEmpty[2])
		{
			WireFuses[wireFuseTakenId]->Show(false);
			WireFuseFixedAlt[2]->Show(true);

			WireFuseConnectedToId[3] = 2;
					
			IsWireFuseFixed[wireFuseTakenId] = true;
			IsWireFuseFixEmpty[2] = false;

			Ropes[3]->nodes[Ropes[3]->numOfNodes-1]->x = WireFuseFixedAlt[2]->Pos.x;
			Ropes[3]->nodes[Ropes[3]->numOfNodes-1]->y = WireFuseFixedAlt[2]->Pos.y-10;

			IsWireFuseInHand = false;
		}
		else if(WireFuseFixRects[3]->Intersection(&pos) && IsWireFuseFixEmpty[3])
		{
			WireFuses[wireFuseTakenId]->Show(false);
			WireFuseFixed[3]->Show(true);

			WireFuseConnectedToId[3] = 3;
					
			IsWireFuseFixed[wireFuseTakenId] = true;
			IsWireFuseFixEmpty[3] = false;

			Ropes[3]->nodes[Ropes[3]->numOfNodes-1]->x = WireFuseFixed[3]->Pos.x;
			Ropes[3]->nodes[Ropes[3]->numOfNodes-1]->y = WireFuseFixed[3]->Pos.y-10;

			IsWireFuseInHand = false;
		}				
	}

	//Save Stuffs
	for (int i = 0; i < 4; i++)
	{
		ProfileSystem::Instance->ActiveProfile.L1S2_IsP1WireConnectId[i] = WireFuseConnectedToId[i];
		ProfileSystem::Instance->ActiveProfile.L1S2_IsP1WireFuseFixed[i] = IsWireFuseFixed[i];
		ProfileSystem::Instance->ActiveProfile.L1S2_IsP1WireFuseFixEmpty[i] = IsWireFuseFixEmpty[i];
	}

	//IsWireFusesFixCorrect = true;
	//for (int i = 0; i < 4; i++)
	//{
	//	if(!IsWireFuseFixed[i])//Not fixed
	//	{
	//		IsWireFusesFixCorrect = false;
	//		break;
	//	}
	//	if(WireFuseConnectedToId[i]!=i)
	//	{
	//		IsWireFusesFixCorrect = false;
	//		break;
	//	}
	//}

	setStatusOfLights();
}

void L1_S2_PZFuse::setStatusOfLights()
{
//	bool IsAllFusesCorrect=false;  //Sac: Unused variable, so commented it

	int fuseId = 0;

	//Set Status of Lights : Check Fuse 0
	IsRotFusePowerOn[0] = false;
	IsRotFusePowerOn[1] = false;
	IsRotFusePowerOn[2] = false;
	IsRotFusePowerOn[3] = false;

	RotFuses[0]->PlayAnimation(0);
	RotFuses[1]->PlayAnimation(0);
	RotFuses[2]->PlayAnimation(0);
	RotFuses[3]->PlayAnimation(0);

	bool PreviousRotFuseCorrectState = IsRotatingFusesCorrect;
	IsRotatingFusesCorrect = false;

	LightMinus->Show(false);

	if(RotFuses[0]->GetRotate() == 0)
	{
		RotFuses[0]->PlayAnimation(1);
		IsRotFusePowerOn[0] = true;

		if(RotFuses[1]->GetRotate() == 0 || RotFuses[1]->GetRotate() == -240)
		{
			RotFuses[1]->PlayAnimation(1);
			IsRotFusePowerOn[1] = true;
			
			if(RotFuses[1]->GetRotate() == 0 && (RotFuses[2]->GetRotate() == 0 || RotFuses[2]->GetRotate() == -240))
			{
				RotFuses[2]->PlayAnimation(1);
				IsRotFusePowerOn[2] = true;

				if(RotFuses[2]->GetRotate() == 0 && RotFuses[3]->GetRotate() == 0)
				{
					RotFuses[3]->PlayAnimation(1);
					IsRotFusePowerOn[3] = true;

					IsRotatingFusesCorrect = true;
					LightMinus->Show(true);
					//disable sparks
					FuseSpark1->SetEnable(false);
				}
				else if(RotFuses[2]->GetRotate() == -240 && RotFuses[3]->GetRotate() != 0)
				{
					RotFuses[3]->PlayAnimation(1);
					IsRotFusePowerOn[3] = true;

					if(RotFuses[3]->GetRotate()==-120)
					{
						IsRotatingFusesCorrect = true;
						LightMinus->Show(true);

						//disable sparks
						FuseSpark1->SetEnable(false);
					}
					else
					{
						PlayRandomSparkSFX();
						sparkTimer = 0;
						fuseId = 4;
						FuseSpark1->SetEnable(true);
						FuseSpark1->SetXY(RotGapPos[fuseId][0],RotGapPos[fuseId][1]);
						FuseSpark1->ForcePlayAnimation(1);

						FuseSparkGlow1->SetXY(RotGapPos[fuseId][0],RotGapPos[fuseId][1]);
						FuseSparkGlow1->ForcePlayAnimation(1);
					}
				}
				else
				{
					PlayRandomSparkSFX();
					sparkTimer = 0;
					fuseId = 3;
					FuseSpark1->SetEnable(true);
					FuseSpark1->SetXY(RotGapPos[fuseId][0],RotGapPos[fuseId][1]);
					FuseSpark1->ForcePlayAnimation(1);

					FuseSparkGlow1->SetXY(RotGapPos[fuseId][0],RotGapPos[fuseId][1]);
					FuseSparkGlow1->ForcePlayAnimation(1);
				}
			}
			else if(RotFuses[1]->GetRotate() == -240 && RotFuses[2]->GetRotate() == -120)
			{
				RotFuses[2]->PlayAnimation(1);
				IsRotFusePowerOn[2] = true;

				if(RotFuses[3]->GetRotate() != 0)
				{
					RotFuses[3]->PlayAnimation(1);
					IsRotFusePowerOn[3] = true;

					if(RotFuses[3]->GetRotate()==-120)
					{
						LightMinus->Show(true);
						//disable sparks
						FuseSpark1->SetEnable(false);
					}
					else
					{
						PlayRandomSparkSFX();
						sparkTimer = 0;
						fuseId = 4;
						FuseSpark1->SetEnable(true);
						FuseSpark1->SetXY(RotGapPos[fuseId][0],RotGapPos[fuseId][1]);
						FuseSpark1->ForcePlayAnimation(1);
					}
				}
				else
				{
					PlayRandomSparkSFX();
					sparkTimer = 0;
					fuseId = 3;
					FuseSpark1->SetEnable(true);
					FuseSpark1->SetXY(RotGapPos[fuseId][0],RotGapPos[fuseId][1]);
					FuseSpark1->ForcePlayAnimation(1);
				}
			}
			else
			{
				PlayRandomSparkSFX();
				sparkTimer = 0;
				fuseId = 2;
				FuseSpark1->SetEnable(true);
				FuseSpark1->SetXY(RotGapPos[fuseId][0],RotGapPos[fuseId][1]);
				FuseSpark1->ForcePlayAnimation(1);
			}
		}
		else
		{
			PlayRandomSparkSFX();
			sparkTimer = 0;
			fuseId = 1;
			FuseSpark1->SetEnable(true);
			FuseSpark1->SetXY(RotGapPos[fuseId][0],RotGapPos[fuseId][1]);
			FuseSpark1->ForcePlayAnimation(1);
		}
	}
	else
	{
		PlayRandomSparkSFX();
		sparkTimer = 0;
		fuseId = 0;
		FuseSpark1->SetEnable(true);
		FuseSpark1->SetXY(RotGapPos[fuseId][0],RotGapPos[fuseId][1]);
		FuseSpark1->ForcePlayAnimation(1);
	}

	if(IsRotatingFusesCorrect)
	{
		if(!PreviousRotFuseCorrectState)
			Control::Audio->PlaySample(aSFXMagic2);
		RotFuses[0]->PlayAnimation(2);
		RotFuses[1]->PlayAnimation(2);
		RotFuses[2]->PlayAnimation(2);
		RotFuses[3]->PlayAnimation(2);
	}

	IsWireFusesFixCorrect = true;

	bool	IsGreenLitLeft = true;
	bool	IsGreenLitRight = true;

	FuseSpark2->SetEnable(false);

	for (int i = 0; i < 4; i++)
	{
		if(IsWireFuseFixed[i])
		{
			//Wire fuse i is fixed
			//Check if connected Fuse is powered up
			int conFuseId = WireFuseConnectedToId[i];

			//Turn Off Lights
			WireFuseFixedAlt[conFuseId]->PlayAnimation(0);
			WireFuseFixed[conFuseId]->PlayAnimation(0);

			if(IsRotFusePowerOn[conFuseId])
			{
				//Connected Rot Fuse is ON
				if (i==0)
				{
					if(conFuseId==0)
						WireFuseFixed[i]->PlayAnimation(1);
					else if(conFuseId==1 && RotFuses[1]->GetRotate() == -240)
						WireFuseFixedAlt[1]->PlayAnimation(1);
					else
					{
						sparkWireTimer = 0;
						if(conFuseId==0)
							fuseId = 0;
						else
							fuseId = 1;
						FuseSpark2->SetEnable(true);
						FuseSpark2->SetXY(wirePos[fuseId][0],wirePos[fuseId][1]);
						FuseSpark2->ForcePlayAnimation(1);

						IsGreenLitLeft = false;
						IsWireFusesFixCorrect = false;
					}
				}
				else if (i==1)
				{
					if(conFuseId==1 && RotFuses[1]->GetRotate() == 0)
						WireFuseFixed[1]->PlayAnimation(1);
					else
					{
						sparkWireTimer = 0;
						if(conFuseId==1)
							fuseId = 1;
						else
							fuseId = 0;
						FuseSpark2->SetEnable(true);
						FuseSpark2->SetXY(wirePos[fuseId][0],wirePos[fuseId][1]);
						FuseSpark2->ForcePlayAnimation(1);

						IsGreenLitLeft = false;
						IsWireFusesFixCorrect = false;
					}
				}
				else if (i==2)
				{
					if(conFuseId==2 && RotFuses[2]->GetRotate() != -240)
						WireFuseFixed[i]->PlayAnimation(1);
					else if(conFuseId==3 && RotFuses[3]->GetRotate() == -120)
						WireFuseFixedAlt[3]->PlayAnimation(1);
					else
					{
						sparkWireTimer = 0;
						if(conFuseId==2)
							fuseId = 2;
						else
							fuseId = 3;
						FuseSpark2->SetEnable(true);
						FuseSpark2->SetXY(wirePos[fuseId][0],wirePos[fuseId][1]);
						FuseSpark2->ForcePlayAnimation(1);

						IsGreenLitRight = false;
						IsWireFusesFixCorrect = false;
					}
				}
				else if (i==3)
				{
					if(conFuseId==2 && RotFuses[2]->GetRotate() == -240)
						WireFuseFixedAlt[2]->PlayAnimation(1);
					else if(conFuseId==3 && RotFuses[3]->GetRotate() != -120)
						WireFuseFixed[3]->PlayAnimation(1);
					else
					{
						sparkWireTimer = 0;
						if(conFuseId==2)
							fuseId = 2;
						else
							fuseId = 3;
						FuseSpark2->SetEnable(true);
						FuseSpark2->SetXY(wirePos[fuseId][0],wirePos[fuseId][1]);
						FuseSpark2->ForcePlayAnimation(1);

						IsGreenLitRight = false;
						IsWireFusesFixCorrect = false;
					}
				}
			}
			else
			{
				if(conFuseId<2)
					IsGreenLitLeft = false;
				else
					IsGreenLitRight = false;
				IsWireFusesFixCorrect = false;

				sparkWireTimer = 0;
				fuseId = i;
				FuseSpark2->SetEnable(true);
				FuseSpark2->SetXY(wirePos[fuseId][0],wirePos[fuseId][1]);
				FuseSpark2->ForcePlayAnimation(1);
			}
		}
		else
		{
			if(i<2)
				IsGreenLitLeft = false;
			else
				IsGreenLitRight = false;

			IsWireFusesFixCorrect = false;
		}
	}

	//Turn on Green light as per wire Fuse Fixes
	if(GreenLightUpLeft->Visible)
	{
		if(!IsGreenLitLeft)
		{
			GreenLightUpLeft->FadeOut(2.0f);//Turn Off
			GreenLightGlowLeft->Show(false);
		}
	}
	else
	{
		if(IsGreenLitLeft)
		{
			GreenLightUpLeft->FadeIn(2.0f);//Turn On
			GreenLightGlowLeft->Show(true);
			Control::Audio->PlaySample(aSFXL1S3P1GreenLit,false);
		}
	}

	if(GreenLightUpRight->Visible)
	{
		if(!IsGreenLitRight)
		{
			GreenLightUpRight->FadeOut(2.0f);//Turn Off
			GreenLightGlowRight->Show(false);
		}
	}
	else
	{
		if(IsGreenLitRight)
		{
			GreenLightUpRight->FadeIn(2.0f);//Turn On
			GreenLightGlowRight->Show(true);
			Control::Audio->PlaySample(aSFXL1S3P1GreenLit,false);
		}
	}
}

float L1_S2_PZFuse::easing(float t,float b,float c,float d)
{
	//t is the current time (or position) of the tween. This can be seconds or frames, steps, seconds, ms, whatever as long as the unit is the same as is used for the total time [3].
	//b is the beginning value of the property.
	//c is the change between the beginning and destination value of the property.
	//d is the total time of the tween.
	return (c * t / d + b);
}


void L1_S2_PZFuse::ProcessSkip()
{
	Control::Audio->PlaySample(aSFXL1S3P1PowerOn,false);

	LeverGlow->Show(false);

	for( int i = 0; i < MAX_FUSES; i++ )
	{
		RotFuses[i]->SetRotate(0);
		RotFuses[i]->PlayAnimation(2);

		WireFuses[i]->Show(false);
		WireFuseFixed[i]->Show(true);
		WireFuseFixedAlt[i]->Show(false);

		Ropes[i]->nodes[Ropes[i]->numOfNodes-1]->x = WireFuseFixed[i]->Pos.x;
		Ropes[i]->nodes[Ropes[i]->numOfNodes-1]->y = WireFuseFixed[i]->Pos.y-10;
	}

	LightPlus->Show(true);
	LightMinus->Show(true);

	StaticFuse1->Show(false);
	StaticFuse2->Show(false);

	StaticFuseFixedLeft->Show(true);
	StaticFuseFixedRight->Show(true);

	GreenLightUpLeft->Show(true);
	GreenLightUpRight->Show(true);
	

	ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved = true;
	ProfileSystem::Instance->ActiveProfile.L1_IsMansionPowerOn = true;
	AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	
	CenterLight->ForceShow();
	CenterLight->PlayAnimation(1);
	IsBgLightAnimationPlaying = true;

	//Show Objective completed
    if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L1O2_LitTheMansion] && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L1O2_LitTheMansion] )
    {
        ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L1O2_LitTheMansion] = true;
        Hud->ShowObjectiveFX(eObj_L1O2_LitTheMansion,true);
    }

	Lever->Show(true);
	Lever->SetAlpha(1.0f);
	Lever->PlayAnimation(1);

	Control::Audio->PlaySample(aSFXMagic2,false);
#ifdef K_ANDROID
	GFApp->AnalyticsSkipPuzzle(1);
#endif
}


void L1_S2_PZFuse::Render(const CPoint* pCam)
{
	for (int i = 0; i < 4; i++)
	{
		Ropes[i]->Render();
		//RopeGraphic->drawLine(wFuseStartX[i],wFuseStartY[i],wFuseEndX[i],wFuseEndY[i],1.0f,0,0,1.0f);
	}
}


void L1_S2_PZFuse::ProcessNonHOHint()
{
	int iNonHOHint = 0;
	int MaxNonHOHints = 4;	//if last case x: then x+1
	do
	{
		switch( iNonHOHint )
		{
			default:	//Exit coordinates
			
			case 0:	++iNonHOHint;
				if( !ProfileSystem::Instance->ActiveProfile.L1S3_IsP1LeftFuseTaken ){
					InitHintTrailEmit(StatFuse1Rect.GetCenterX(), StatFuse1Rect.GetCenterY());	return;
				}					
				break;
			case 1:	++iNonHOHint;
				if( !ProfileSystem::Instance->ActiveProfile.L1S2_IsP1RightFuseTaken ){
					InitHintTrailEmit(StatFuse2Rect.GetCenterX(), StatFuse2Rect.GetCenterY());	return;
				}
				break;
			case 2:	++iNonHOHint;
				if( !ProfileSystem::Instance->ActiveProfile.L1S2_IsP1GreenWireDownFixed ){
					InitHintTrailEmit(GreenWireDownRect.GetCenterX(), GreenWireDownRect.GetCenterY());	return;
				}
				break;
			case 3:	++iNonHOHint;
				if( !ProfileSystem::Instance->ActiveProfile.L1S2_IsP1GreenWireTopFixed ){
					InitHintTrailEmit(GreenWireTopRect.GetCenterX(), GreenWireTopRect.GetCenterY());	return;
				}
				break;
		}
	}
	while(  iNonHOHint < MaxNonHOHints );	//Loop all cases

	Hud->ShowNothingToDoMsgBanner();
}

Rope::Rope(float x1, float y1, float x2, float y2)
{
	RopeGraphic = KPTK::createKGraphic();

	this->numOfNodes = 20;
	this->damp = 0.35f;
	this->gravity = 1.05f;

	//Init values
	int n = numOfNodes;

	float dx = ( x2 - x1 ) / (float)n;
//	float dy = ( y2 - y1 ) / (float)n;  // Sac: Commenting Unused variable

	for (int i = 0; i < n; i++)
	{
		RNode *node = new RNode(x1 + (float)dx*i,y1 + (float)dx*i);
		nodes[i] = node;
	}
}

void Rope::Render()
{
	int n = numOfNodes;
	n--;	//Not checking for last node
	while(--n>0) //n-2 to 1
	{
		//Code 1
		//add gravity
		nodes[n]->y += gravity;

		float vecx = nodes[n]->x - nodes[n+1]->x;
		float vecy = nodes[n]->y - nodes[n+1]->y;

		float dx = abs(nodes[n]->x - nodes[n+1]->x);
		/*if(dx<100)
			nodes[n]->x += (nodes[n+1]->x - nodes[n]->x)*0.15f;*/

		float dy = abs(nodes[n]->y - nodes[n+1]->y);
		/*if(dy<100)
			nodes[n]->y += (nodes[n+1]->y - nodes[n]->y)*0.15f;*/

		float dist = sqrt(dx*dx + dy*dy);

		//unit vector
		vecx = vecx/dist;
		vecy = vecy/dist;

		if(dist<5)
		{
			nodes[n]->x += dist*vecx*damp;
			nodes[n]->y += dist*vecy*damp;
		}
		else if(dist>7)
		{
			nodes[n]->x -= dist*vecx*damp;
			nodes[n]->y -= dist*vecy*damp;
		}
	}

	n = 0;
	while(n<numOfNodes-2) //n-2 to 1
	{
		float vecx = nodes[n+1]->x - nodes[n]->x;
		float vecy = nodes[n+1]->y - nodes[n]->y;

		float dx = abs(nodes[n]->x - nodes[n+1]->x);
		float dy = abs(nodes[n]->y - nodes[n+1]->y);

		float dist = sqrt(dx*dx + dy*dy);

		//unit vector
		vecx = vecx/dist;
		vecy = vecy/dist;

		if(dist<5)
		{
			nodes[n+1]->x += dist*vecx*damp;
			nodes[n+1]->y += dist*vecy*damp;
		}
		else if(dist>7)
		{
			nodes[n+1]->x -= dist*vecx*damp;
			nodes[n+1]->y -= dist*vecy*damp;
		}
		n++;
	}

	for (int i = 0; i < numOfNodes-1; i++)
	{			
		RopeGraphic->setTextureQuality(true);
		RopeGraphic->drawLine(nodes[i]->x-2,nodes[i]->y,nodes[i+1]->x-2,nodes[i+1]->y,(122/255.0f),(25/255.0f),(1/255.0f),0.8f);
		RopeGraphic->drawLine(nodes[i]->x-1,nodes[i]->y,nodes[i+1]->x-1,nodes[i+1]->y,(132/255.0f),(25/255.0f),(1/255.0f),0.8f);
		RopeGraphic->drawLine(nodes[i]->x,nodes[i]->y,nodes[i+1]->x,nodes[i+1]->y,(142/255.0f),(25/255.0f),(1/255.0f),0.8f);
		RopeGraphic->drawLine(nodes[i]->x+1,nodes[i]->y,nodes[i+1]->x+1,nodes[i+1]->y,(132/255.0f),(25/255.0f),(1/255.0f),0.8f);
		RopeGraphic->drawLine(nodes[i]->x+2,nodes[i]->y,nodes[i+1]->x+2,nodes[i+1]->y,(122/255.0f),(25/255.0f),(1/255.0f),0.8f);
		//RopeGraphic->drawRect(nodes[i]->x,nodes[i]->y,nodes[i]->x+5,nodes[i]->y+5,1.0f,0,0,1.0f);
	}
}


float Rope::DistanceBetween(float x1,float y1,float x2, float y2)
{
	return ((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

Rope::~Rope()
{
	delete RopeGraphic;

	for (int i = 0; i < 20; i++)
	{
		SAFE_DELETE(nodes[i])
	}
}

RNode::RNode(float x, float y)
{
	this->x = x;
	this->y = y;
}

void L1_S2_PZFuse::ResetPuzzle()
{
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		return;
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
			ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved = false;
			ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleShuffled = false;
			ProfileSystem::Instance->ActiveProfile.L1S2_IsP1FusePuzzleActive = false;
			ProfileSystem::Instance->ActiveProfile.L1S2_IsP1LeftFuseFixed = true;
			ProfileSystem::Instance->ActiveProfile.L1S2_IsP1RightFuseFixed = false;
			ProfileSystem::Instance->ActiveProfile.L1S3_IsP1LeftFuseTaken = false;
			ProfileSystem::Instance->ActiveProfile.L1S2_IsP1RightFuseTaken = true;
			ProfileSystem::Instance->ActiveProfile.L1S2_IsP1GreenWireDownFixed = true;
			ProfileSystem::Instance->ActiveProfile.L1S2_IsP1GreenWireTopFixed = false;

			for (int i = 0; i < 4; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L1S2_IsP1RotFuseRot[i] = 0;
				ProfileSystem::Instance->ActiveProfile.L1S2_IsP1WireConnectId[i] = -1;
				ProfileSystem::Instance->ActiveProfile.L1S2_IsP1WireFuseFixed[i] = false;
				ProfileSystem::Instance->ActiveProfile.L1S2_IsP1WireFuseFixEmpty[i] = true;
			}

			//Nerfed puzzle difficulty
			ProfileSystem::Instance->ActiveProfile.L1S2_IsP1RotFuseRot[2] = 1;
			ProfileSystem::Instance->ActiveProfile.L1S2_IsP1RotFuseRot[3] = 2;
			ProfileSystem::Instance->ActiveProfile.L1S2_IsP1WireConnectId[1] = 1;
			ProfileSystem::Instance->ActiveProfile.L1S2_IsP1WireFuseFixed[1] = true;
			ProfileSystem::Instance->ActiveProfile.L1S2_IsP1WireFuseFixEmpty[1] = false;

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S2_PZFuse;
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
void L1_S2_PZFuse::NavigateToScene()
{
	if(GFHud->InvBoxCtrl->navTapped)
	{
		if( Control::Input->LBclicked() )
		{
			if(Control::Input->Mpos().x > GFHud->InvBoxCtrl->navRectX2 && Control::Input->Mpos().x < GFHud->InvBoxCtrl->navRectX1 && Control::Input->Mpos().y > GFHud->InvBoxCtrl->navRectY2 && Control::Input->Mpos().y < GFHud->InvBoxCtrl->navRectY1)
			{
				//Cursor::SetMode(CA_ExitUp);

				//GFHud->InvBoxCtrl->NavigateText->Show(false);
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

