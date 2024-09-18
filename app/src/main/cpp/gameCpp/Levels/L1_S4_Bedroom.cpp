//====================================
//	DQFYLH
//	HO System
//	Level 1 - Scene 04 : Bedroom
//====================================

#include "L1_S4_Bedroom.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "Rect.h"
#include "TextureFactory.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "AwardManager.h"


const CPoint		CenterPoint(683,334,0);
const CPoint		PopCloseRectBtnPoint(998,98,0);
const CRectangle	PopCloseButtonRect(965.0f,49.0f,78.0f,77.0f);
const CRectangle	PopAreaRect(357.0f,85.0f,657.0f,500.0f);

//Scene Rects
const CRectangle	PaintAndShadowPuzzleRect(1001.0f,109.0f,151.0f,106.0f);

const CRectangle  SceneRectTreasureRoom(143.0f,196.0f,272.0f,365.0f);

//Popup Rects
const CRectangle  PopRect1Drawer(186.0f,349.0f,267.0f,206.0f);
const CRectangle  PopRect2Keyfall(828.0f,479.0f,104.0f,73.0f);
const CRectangle  PopRect3BeetleShelf(970.0f,252.0f,196.0f,127.0f);

const CRectangle  P3RectBookShelf(360.0f,91.0f,646.0f,216.0f);

//Inventory Destination Rects
const CRectangle  InvDestRectBeetleFix(588.0f,350.0f,188.0f,183.0f);
const CRectangle  InvDestRectBookCode(382.0f,97.0f,584.0f,446.0f);

const CRectangle  InvDestRectWaxAnim(499.0f,204.0f,254.0f,221.0f);
const CRectangle  InvDestRectScrewTake(497.0f,200.0f,230.0f,200.0f);
const CRectangle  InvDestRectSnakeKey(562.0f,203.0f,233.0f,126.0f);

const CRectangle  MaskRectDraweOpen(362.0f,89.0f,714.0f,544.0f);

const CRectangle  ActionRectBookOpen(518.0f,336.0f,317.0f,240.0f);
const CRectangle  ActionRectTurnSnakeKey(622.0f,204.0f,147.0f,118.0f);
const CRectangle  ActionRectTakeDiary(457.0f,238.0f,250.0f,157.0f);
const CRectangle  ActionRectChair(1011.0f,387.0f,211.0f,204.0f);

//Inventory Item Rects
const CRectangle  InvItemRectKeySet(605.0f,380.0f,155.0f,117.0f);
const CRectangle  InvItemRectShadowHand(648.0f,168.0f,139.0f,100.0f);

const CRectangle  PopupMaskRect(358.0f,87.0f,656.0f,487.0f);

const CRectangle  MemRectBoxGloves(858.0f,402.0f,133.0f,106.0f);

//Info Rects
const CRectangle  InfoRectFallenKey(573.0f,254.0f,186.0f,180.0f);
const CRectangle  InfoRectMap(218.0f,174.0f,150.0f,116.0f);
const CRectangle  InfoRectBed(438.0f,317.0f,523.0f,203.0f);


float lightAlphaArray6[20] = {0,0,0,0,0,0.15f,0.65f,0.75f,0.65f,0.25f,0.15f,0.75f,0.65f,0.05f,0.25f,0.55f,0.15f,0,0,0};

L1_S4_Bedroom::L1_S4_Bedroom()
{
	Visible = false;
	
	RT_BgScn = NULL;

	bgScaleOffsetX = 0.0f;
	bgScaleOffsetY = 0.0f;

	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV1\\S4\\S4.SCN");

	GFInstance->PostUpdate(1);

	InitPopups();

	hangLampVFX = new SuperFX("PRT\\L1\\s4hanglamp.INI", PointSystem::CreateCPoint(654,79,0), 100); 
	hangLampVFX->Start();
	hangLampVFX->AdvanceByTime(2.0f);
	VFXArray.push_back(hangLampVFX);

	windowVFX = new SuperFX("PRT\\L1\\s4window.INI", PointSystem::CreateCPoint(727,326,0), 100); 
	windowVFX->Start();
	windowVFX->AdvanceByTime(2.0f);
	VFXArray.push_back(windowVFX);

	tableLampVFX = new SuperFX("PRT\\L1\\s4tablelamps.INI", PointSystem::CreateCPoint(298,334,0), 100); 
	tableLampVFX->Start();
	tableLampVFX->AdvanceByTime(2.0f);
	VFXArray.push_back(tableLampVFX);

	rightLampVFX = new SuperFX("PRT\\L1\\s4rightlamp.INI", PointSystem::CreateCPoint(936,216,0), 100); 
	rightLampVFX->Start();
	rightLampVFX->AdvanceByTime(2.0f);
	VFXArray.push_back(rightLampVFX);

	//If Shelf is not moved
	HOScene->GetObjectByID("maphanging")->SetPivot(3, -60);
	HOScene->GetObjectByID("maphanging")->Animate(0,0,2,0,6.0f);

	HOScene->GetObjectByID("TableLampglow1")->Animate(0.15f, 0.75f, 6.0f);
	HOScene->GetObjectByID("TableLampglow2")->Animate(0.15f, 0.75f, 6.0f, 0.15f);

	HOScene->GetObjectByID("chair")->SetPivot(38, 116);
	chairSwingTimer = -1.0f;

	HOScene->GetObjectByID("hanglamp")->SetPivot(0, -80);
	HOScene->GetObjectByID("hanglamp")->Animate(0,0,2,0,4.0f);

	HOScene->GetObjectByID("hanglampLight")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("hanglampLight")->Animate(0.15f, 0.55f, 4.0f, 0.15f);

	HOScene->GetObjectByID("RightLightGlow")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("RightLightGlow")->Animate(0.15f, 0.65f, 5.0f, 1.15f);

	HOScene->GetObjectByID("windowglow")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("windowglow")->Animate(0.15f, 0.25f, 8.0f, 0.15f);

	HOScene->GetObjectByID("windowrays1")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("windowrays1")->Animate(0.15f, 0.75f, 6.0f, 0.15f);
	HOScene->GetObjectByID("windowrays2")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("windowrays2")->Animate(0.15f, 0.75f, 7.0f, 0.35f);
	HOScene->GetObjectByID("windowrays3")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("windowrays3")->Animate(0.15f, 0.75f, 8.0f, 0.45f);

	Scn_lightning = HOScene->GetObjectByID("windowlightning");
	Scn_lightning->SetBlendMode(eBlend_AlphaAdditive);
	Scn_lightning->SetScale(2.0f);
	Scn_lightning->Show(false);

	lightFrame = 0;
	lightningTimer = 0;
	nextLightningThreshold = 8.0f;

	//Scene Elements
	Scn_Bg = HOScene->GetObjectByID("bg");

	if(ProfileSystem::Instance->ActiveProfile.L1S4_InvPaintingFrameTaken)
		HOScene->GetObjectByID("wallpainting")->SetEnable(false);

	if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerOpened)
		HOScene->GetObjectByID("drawerdooropen")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L1S4_IsSecretTreasureDoorOpened)
	{
		HOScene->GetObjectByID("maphanging")->SetEnable(false);
		HOScene->GetObjectByID("TableLampglow1")->SetEnable(false);
		HOScene->GetObjectByID("TableLampglow2")->SetEnable(false); 
		HOScene->GetObjectByID("drawerdooropen")->SetEnable(false);
		HOScene->GetObjectByID("drawerdoor")->SetEnable(false);
	}

	if( GFInstance->availablePhyMem >= MAX_LOWEND_PEAKMEMORY )
	{
	}

	GFInstance->PostUpdate(2);

	zOrderScnVideo = 1;
	zOrderPopVideo = eZOrder_Popup + 10;

	IsZOrderChangeToScene = false;

	IsVidSkippable = false;
	IsSkipMouseOver = false;
	IsSkipMouseClicked = false;
	SkipShowTimer = 0;

	//Video Init : Add Visible = true; to see anything on screen !!
	SetZ(1);

	creekVidStartTimer = 1.0f;

	skipObj = NULL;

	skipObj = new CObject();
	skipObj->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\CNM\\SKIPBTN.SPR", (SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)),SKIPIMAGE_Y, eZOrder_CutsceneFull+10);
	skipObj->Show(false);

	cv_CreekAppear = NULL;	
	cv_CreekTalks = NULL;
	cv_MayaPushVideo = NULL;

	isCreekVideoPlaying = false;
	isCreekIntroPlaying = false;
	isCreekOutroPlaying = false;

	isMayaPushVideoPlaying = false;

	time = 0;
	subIndex = 0;
	subIndex2 = 0;
	subMax = 3;
	subMax2 = 2; 
	vidTimer = 0;
	vidTimer2 = 0;
	vidTimer3 = 0;


	subtitleText = new CBitmapText();
	subtitleText->SetZ(30000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("creektalk1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("creektalk2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("creektalk3"));

	subtitles2[0] = AssetsStringTable::getSub(AssetsStringTable::getString("mayapush1"));
	subtitles2[1] = AssetsStringTable::getSub(AssetsStringTable::getString("mayapush2"));

//	CRectangle RectCreekVid(171, 96, 1024, 576);
	CRectangle RectCreekVid(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsCreekIntroPlayed)
	{
		cv_CreekAppear = new CVideo("OGV\\L1S4\\S4CREEKAPPEARS.OGV", RectCreekVid, false, eZOrder_CutsceneFull);
		//cv_CreekAppear->SetScale(1.334f);
		cv_CreekAppear->SetScale(GFApp->video_aspectRatio);
	}
	
	if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsCreekOutroPlayed)
	{
		cv_CreekTalks = new CVideo("OGV\\L1S4\\S4CREEKTALKS.OGV", RectCreekVid, false, eZOrder_CutsceneFull);
		//cv_CreekTalks->SetScale(1.334f);
		cv_CreekTalks->SetScale(GFApp->video_aspectRatio);		
	}

	if(!ProfileSystem::Instance->ActiveProfile.L1S4_MayaPushMaryVideoPlayed)
	{
		cv_MayaPushVideo = new CVideo("OGV\\L1S4\\MAYAPUSH.OGV", RectCreekVid, false, eZOrder_CutsceneFull);
		//cv_MayaPushVideo->SetScale(1.334f);
		cv_MayaPushVideo->SetScale(GFApp->video_aspectRatio);
	}
	
	//MUSIC_SFX
	Control::Audio->ClearUnusedTracks();

	
	if(ProfileSystem::Instance->ActiveProfile.IsStartMusic)
	{		
		ProfileSystem::Instance->ActiveProfile.IsStartMusic = false;
		if( !Control::Audio->IsPlaying(aTrackL1Hunted) )
			Control::Audio->PlaySample(aTrackL1Hunted,false);
	}
	
	Control::Audio->LoadSample(aTrackCutscnS4CreekAppears,AudioTrack);
	Control::Audio->LoadSample(aTrackCutscnS4MayaPush,AudioTrack);

	Control::Audio->LoadSample(aAmbS4Bedroom,AudioAmb);
	Control::Audio->PlaySample(aAmbS4Bedroom,true);

	Control::Audio->LoadSample(aVOL1MaryRefS4_1,AudioVO);
	//MUSIC_SFX

	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L1_S4_Bedroom]  = true;

	if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S4_Bedroom])
		ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S4_Bedroom] = true;

	

	
	
	
	Visible = true;	//To render anything with renderlistentity

	UpdateBgGraphic();

	CAnim_SlideTreasureDoor = NULL;
	IsSecretTreasureDoorSlidePlaying = false;

	Scn_SlidingDoor = NULL;
	Scn_SlidingDoorMask = NULL;
	dustFallVFX = NULL;
	
	if(ProfileSystem::Instance->ActiveProfile.L1S4_IsShadowPuzFinalKeyTurned)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsSecretTreasureDoorOpened)
		{
			HOScene->GetObjectByID("maphanging")->SetEnable(false);
			HOScene->GetObjectByID("TableLampglow1")->SetEnable(false);
			HOScene->GetObjectByID("TableLampglow2")->SetEnable(false);
			HOScene->GetObjectByID("drawerdooropen")->SetEnable(false);
			HOScene->GetObjectByID("drawerdoor")->SetEnable(false);

			Scn_SlidingDoor = new CObject();
			Scn_SlidingDoor->LoadInit("LV1\\S4\\movingshelfdoor.SPR", 286, 381, 19);

			//HOScene->PushObject(Scn_SlidingDoor);

			Scn_SlidingDoorMask = new CObject();
			Scn_SlidingDoorMask->LoadInit("LV1\\S4\\doormovingalpha.SPR", 547, 401, 20);

			HOScene->PushObject(Scn_SlidingDoorMask);

			dustFallVFX = new SuperFX("PRT\\L1\\s4dustfall.INI", PointSystem::CreateCPoint(274,168,0), 18);
			dustFallVFX->Start();

			CAnim_SlideTreasureDoor = new CustomAnimator("ANIM\\L1\\s4shelfdoormoving.anim",Scn_SlidingDoor, true);
			IsSecretTreasureDoorSlidePlaying = true;

			Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4SecretDoorOpen);

			HOScene->Shake(4,5,5.6f);
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsCreekIntroPlayed)
	{
		//To avoid
		#ifdef _CEBUILD
			if(Hud->awardsVFX!=NULL)
				Hud->awardsVFX->StopImmediate();
			Hud->PauseAwardUI(true);
		#endif 
	}

	dialogSystem = NULL;
	creekTalkScn = NULL;
	//if condition to add
	if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsCreekDialogPlayed)
	{
		dialogSystem = new DialogSystem();
		AddControl(dialogSystem);
	
		creekTalkScn = new CHOScene();
		creekTalkScn->Load("DIALOG\\S4CREEK.SCN");
		creekTalkScn->SetZOff(eZOrder_Popup);
		creekTalkScn->GetObjectByID("creektalk")->SetZ(eZOrder_Popup+2);
		creekTalkScn->GetObjectByID("CreekTalkHead")->SetZ(eZOrder_Popup+3);
		creekTalkScn->Show(false);

		//For the rare cases when user accidentally force quit during creek dialog
		if(ProfileSystem::Instance->ActiveProfile.L1S4_IsCreekIntroPlayed)
		{
			#ifdef _CEBUILD
				if(Hud->awardsVFX!=NULL)
					Hud->awardsVFX->StopImmediate();
				Hud->PauseAwardUI(true);
			#endif 

			creekTalkScn->FadeOut(2.0f);
			creekTalkScn->Show(true);

			dialogSystem->ShowDialog(eDialog_S4Creek, true);
			isDialogActive = true;

			Hud->EnableInput(false);
			Hud->HideHud();
		}
		else
			isDialogActive = false;
	}
	else
		isDialogActive = false;

	TaskPendingUpdate();
}

void L1_S4_Bedroom::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;
	if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvKeySetTaken && ProfileSystem::Instance->ActiveProfile.L1S3_NagaKeyTaken &&  ProfileSystem::Instance->ActiveProfile.L1S3_MatchBoxTaken)
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerOpened && ProfileSystem::Instance->ActiveProfile.L1S3_NagaKeyTaken )
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsSecretTreasureDoorOpened && ProfileSystem::Instance->ActiveProfile.L1S4_InvKeySetTaken && ProfileSystem::Instance->ActiveProfile.L1S4_IsInvShadowHand2Taken && ProfileSystem::Instance->ActiveProfile.L1S3_IsInvShadowHand1Taken)
		isAnyTaskLeft = true;
	else if(ProfileSystem::Instance->ActiveProfile.L1S4_IsSecretTreasureDoorOpened && !ProfileSystem::Instance->ActiveProfile.L1S4_MayaPushMaryVideoPlayed)
		isAnyTaskLeft = true;
	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S4_Bedroom] = isAnyTaskLeft;

	if(ProfileSystem::Instance->ActiveProfile.L1S4_IsSecretTreasureDoorOpened)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S5_TreasureRoom] = true;
}

void L1_S4_Bedroom::InitPopups()
{	
	ActivePopupID = ePopInactive;

	popupCloseDelayTimer = 0.0f;

	diaryTakeDelayTimer = -1.0f;
	diaryTakeAnimTimer = 0;

	Pop_CloseButton = NULL;

	IsPopupTransitionActive = false;

	ActivePop_State = POPUP_CLOSED;

	Pop1Drawer = NULL;
	CAnim_DrawerOpening = NULL;
	CAnim_DiaryTaking = NULL;
	CAnim_P1MatchStickLighting = NULL;

	IsP1DrawerOpeningPlaying = false;
	IsP1DiaryTakingPlaying = false;
	IsP1SnakeKeyAnimPlaying = false;

	IsP1MatchStickPlaying = false;
	IsP1WaxMeltingPlaying = false;

	isKeyFallSoundPlayed = false;

	IsCoinAnim1Playing = IsCoinAnim2Playing = false;

	//if ( !IsAllRubyPiecesTaken )
	{
		Pop1Drawer = new CHOScene();
		Pop1Drawer->Load("LV1\\S4\\POP\\P1.SCN");

		//Add popup frame and close button
		CObject *popFrame = new CObject();
		popFrame->LoadInit("UI\\poprectframe.SPR", 13, -15, 3);
		popFrame->SetUID("popFrame");
		Pop1Drawer->InsertObject(popFrame,2); //in Normal Cases

		//Place it above popup frame
		//Pop1Drawer->InsertObject(popFrame,17);
		//Pop1Drawer->GetObjectByID("p1rosetop")->SetZ(eZOrder_PopFrame+2);

		P1_DrawerClosed = Pop1Drawer->GetObjectByID("p1drawerclosed");

		P1_LeftLockStatic = Pop1Drawer->GetObjectByID("p1LLockStatic");
		P1_RightLockStatic = Pop1Drawer->GetObjectByID("p1RLockStatic");
		P1_SnakeLockAnim = Pop1Drawer->GetObjectByID("p1MLockAnim");
		P1_LeftLockAnim = Pop1Drawer->GetObjectByID("p1LLockAnim");

		P1_DrawerOpened = Pop1Drawer->GetObjectByID("p1draweropen");
		P1_Diary = Pop1Drawer->GetObjectByID("p1diary");
		P1_TableTopmask = Pop1Drawer->GetObjectByID("p1topmask");
		P1_FallingKey = Pop1Drawer->GetObjectByID("p1fallingkey");
		P1_BookKeyMask = Pop1Drawer->GetObjectByID("p1keybookmask");

		P1_SecretSpace = Pop1Drawer->GetObjectByID("p1secretspace");
		P1_BeetleHead = Pop1Drawer->GetObjectByID("p1beetlepart");
		P1_SmallDoor = Pop1Drawer->GetObjectByID("p1smalldoor");
		P1_Screw1 = Pop1Drawer->GetObjectByID("p1screw1");
		P1_Screw2 = Pop1Drawer->GetObjectByID("p1screw2");
		P1_WaxStatic = Pop1Drawer->GetObjectByID("p1waxanimstatic");
		P1_WaxMeltAnim = Pop1Drawer->GetObjectByID("p1waxanim");
		P1_MatchboxInside = Pop1Drawer->GetObjectByID("p1matchboxinside");
		P1_Matchbox = Pop1Drawer->GetObjectByID("p1matchbox");
		P1_MatchStick = Pop1Drawer->GetObjectByID("p1matchstick");
		P1_MatchStickMask = Pop1Drawer->GetObjectByID("p1matchstickmask");
		P1_MatchSpark = Pop1Drawer->GetObjectByID("p1spark");
		P1_MatchFlameStick = Pop1Drawer->GetObjectByID("p1flameanim");

		P1_MatchSpark->SetBlendMode(eBlend_AlphaAdditive);

		P1_CoinAnim1 = Pop1Drawer->GetObjectByID("p1coinanim1");
		P1_CoinAnim2 = Pop1Drawer->GetObjectByID("p1coinanim2");

		P1_CoinAnim1->SetEnable(false);
		P1_CoinAnim2->SetEnable(false);

		P1_WaxMeltAnim->SetEnable(false);
		P1_MatchboxInside->SetEnable(false);
		P1_Matchbox->SetEnable(false);
		P1_MatchStick->SetEnable(false);
		P1_MatchStickMask->SetEnable(false);
		P1_MatchSpark->SetEnable(false);
		P1_MatchFlameStick->SetEnable(false);

		CObject* objArr6[] = {P1_MatchboxInside, P1_MatchFlameStick, P1_MatchSpark, P1_MatchStick};
		std::string animArr6[] = {"ANIM\\L1\\s4matchboxinside.anim", "ANIM\\L1\\s4matchflamestick.anim", "ANIM\\L1\\s4matchspark.anim", "ANIM\\L1\\s4matchstick.anim"};
		CAnim_P1MatchStickLighting = new CustomAnimGroup(4, animArr6, objArr6);

		if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerOpened)
		{
			P1_Diary->SetEnable(false);
			P1_DrawerOpened->SetEnable(false);
			P1_FallingKey->SetEnable(false);
			P1_BookKeyMask->SetEnable(false);

			P1_SecretSpace->SetEnable(false);
			P1_BeetleHead->SetEnable(false);
			P1_SmallDoor->SetEnable(false);
			P1_Screw1->SetEnable(false);
			P1_Screw2->SetEnable(false);
			P1_WaxStatic->SetEnable(false);

			if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerSnakeKeyFixed)
			{
				P1_SnakeLockAnim->SetEnable(false);
				P1_LeftLockAnim->SetEnable(false);
			}
			else
			{
				P1_LeftLockStatic->SetEnable(false);
				P1_RightLockStatic->SetEnable(false);
			}
		}
		else
		{
			//Drawer is opened
			if(ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerWaxMelted)
				P1_WaxStatic->SetEnable(false);
			if(ProfileSystem::Instance->ActiveProfile.L1S4_InvDrawerDiaryTaken)
				P1_Diary->SetEnable(false);

			if(ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerScrewsTakenout)
			{
				P1_Screw1->SetEnable(false);
				P1_Screw2->SetEnable(false);
			}

			P1_LeftLockStatic->SetEnable(false);
			P1_RightLockStatic->SetEnable(false);
			P1_SnakeLockAnim->SetEnable(false);
			P1_LeftLockAnim->SetEnable(false);

			P1_DrawerClosed->SetEnable(false);

			//after slide in animation
			P1_Diary->SetXY(591-CenterPoint.x,290-CenterPoint.y);
			P1_Diary->SetScale(0.525f);

			P1_FallingKey->SetEnable(false);
			P1_BookKeyMask->SetEnable(false);
		}

		CObject* objArr3[] = {P1_DrawerOpened, P1_Diary};
		std::string animArr3[] = {"ANIM\\L1\\draweropen.anim", "ANIM\\L1\\drawerdiaryslide.anim"};
		CAnim_DrawerOpening = new CustomAnimGroup(2, animArr3, objArr3);

		CObject* objArr4[] = {P1_BookKeyMask, P1_Diary, P1_TableTopmask, P1_FallingKey};
		std::string animArr4[] = {"ANIM\\L1\\diarytakebookmask.anim", "ANIM\\L1\\diarytakediary.anim", "ANIM\\L1\\diarytaketopmask.anim", "ANIM\\L1\\diarytakefallkey.anim"};
		CAnim_DiaryTaking = new CustomAnimGroup(4, animArr4, objArr4);

		CObject *popCloseBtn = new CObject();
		popCloseBtn->LoadInit("UI\\closebtn.SPR", 321, -246, eZOrder_PopCloseBtn);
		popCloseBtn->SetUID("popClose");
		Pop1Drawer->PushObject(popCloseBtn);

		Pop1Drawer->Show(false);
		Pop1Drawer->SetPosOff(CenterPoint.x,CenterPoint.y);
		Pop1Drawer->SetZOff(eZOrder_Popup);
	}
	Popup1_State = POPUP_CLOSED;

	Pop2KeyFall = NULL;
	PopOverPopMask = NULL;
	//if ( !IsAllRubyPiecesTaken )
	{
		Pop2KeyFall = new CHOScene();
		Pop2KeyFall->Load("LV1\\S4\\POP\\P2.SCN");

		//Add popup frame and close button
		CObject *popFrame = new CObject();
		popFrame->LoadInit("UI\\popcircframe.SPR", 4, -9, 1);
		popFrame->SetUID("popFrame");
		Pop2KeyFall->InsertObject(popFrame,1); //in Normal Cases

		CObject *popCloseBtn = new CObject();
		popCloseBtn->LoadInit("UI\\closebtn.SPR", 145, -158, 7);
		popCloseBtn->SetUID("popClose");
		Pop2KeyFall->PushObject(popCloseBtn);

		Pop2KeyFall->Show(false);
		Pop2KeyFall->SetPosOff(CenterPoint.x,CenterPoint.y);
		Pop2KeyFall->SetZOff(eZOrder_PopupOverPopup);

		PopOverPopMask = new CObject();
		PopOverPopMask->LoadSprite("UI\\Mask.SPR");
		PopOverPopMask->SetXY(683,384);
		PopOverPopMask->SetScale(2.0);
		PopOverPopMask->SetZ(eZOrder_PopupOverPopupMask);
		PopOverPopMask->Show(false);
	}
	Popup2_State = POPUP_CLOSED;

	Pop3BeetleShelf = NULL;
	P3BookTakePoly = NULL;
	CAnim_bookTaking = NULL;
	CAnim_beetleFixing = NULL;
	P3_HandWithCode = NULL;

	IsP3BookTakingAnimPlaying = false;
	IsP3BeetleFixAnimPlaying = false;
	IsP3LockOpenAnimPlaying = false;

	//if ( !IsAllRubyPiecesTaken )
	{
		Pop3BeetleShelf = new CHOScene();
		Pop3BeetleShelf->Load("LV1\\S4\\POP\\P3.SCN");

		P3_HandWithCode = new CObject();
		P3_HandWithCode->LoadInit("LV1\\S4\\POP\\p3handwithcode.SPR", -100.0f, 121.0f, 12);
		P3_HandWithCode->SetEnable(false);

		Pop3BeetleShelf->PushObject(P3_HandWithCode);

		//Add popup frame and close button
		CObject *popFrame = new CObject();
		popFrame->LoadInit("UI\\poprectframe.SPR", 13, -15, eZOrder_PopFrame);
		popFrame->SetUID("popFrame");
		Pop3BeetleShelf->PushObject(popFrame); //in Normal Cases

		//Place it above popup frame
		//Pop3BeetleShelf->InsertObject(popFrame,17);
		//Pop3BeetleShelf->GetObjectByID("p1rosetop")->SetZ(eZOrder_PopFrame+2);

		CObject *popCloseBtn = new CObject();
		popCloseBtn->LoadInit("UI\\closebtn.SPR", 321, -246, eZOrder_PopCloseBtn);
		popCloseBtn->SetUID("popClose");
		Pop3BeetleShelf->PushObject(popCloseBtn);

		int ArrayPts[] = {580,91,671,95,765,309,687,310};
		P3BookTakePoly=  new CPolygon(4);
		P3BookTakePoly->Init(ArrayPts);

		P3_TakingBook = Pop3BeetleShelf->GetObjectByID("p3booktaking");
		P3_BookOnTable = Pop3BeetleShelf->GetObjectByID("p3bookontable");
		P3_BookOpened = Pop3BeetleShelf->GetObjectByID("p3bookopen");
		P3_BooksOnShelf = Pop3BeetleShelf->GetObjectByID("p3booksdefault");
		P3_BooksOnShelfMoved = Pop3BeetleShelf->GetObjectByID("p3booksaftertaking");
		P3_BookLockStatic = Pop3BeetleShelf->GetObjectByID("booklockstatic");
		P3_BookLockOpenAnim = Pop3BeetleShelf->GetObjectByID("booklockopen");
		P3_KeySet = Pop3BeetleShelf->GetObjectByID("p3keyset");
		P3_ShadowHand = Pop3BeetleShelf->GetObjectByID("p3shadowhand");
		P3_BeetleFix = Pop3BeetleShelf->GetObjectByID("p3fixbeetle");
		P3_BeetleGlowUp = Pop3BeetleShelf->GetObjectByID("p3beetlelightabove");
		P3_BeetleGlowDown = Pop3BeetleShelf->GetObjectByID("p3beetlelightbelow");

		P3_TakingBook->SetMaskRect(PopupMaskRect);

		CObject* objArr[] = {P3_TakingBook, P3_BookOnTable, P3_BookLockStatic};
		std::string animArr[] = {"ANIM\\L1\\s4booktaking.anim", "ANIM\\L1\\s4bookontable.anim", "ANIM\\L1\\s4booklockstatic.anim"};
		CAnim_bookTaking = new CustomAnimGroup(3, animArr, objArr);

		CObject* objArr2[] = {P3_BeetleFix, P3_BeetleGlowUp, P3_BeetleGlowDown};
		std::string animArr2[] = {"ANIM\\L1\\s4beetlefix.anim", "ANIM\\L1\\s4beetlelightabove.anim", "ANIM\\L1\\s4beetlelightbelow.anim"};
		CAnim_beetleFixing = new CustomAnimGroup(3, animArr2, objArr2);

		if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S4BoxingGloves])
		{
			Pop3BeetleShelf->GetObjectByID("p3memboxgloves")->SetEnable(false);
		}

		P3_BookSlide = Pop3BeetleShelf->GetObjectByID("p3bookslide");

		if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleBookOpened)
		{
			P3_KeySet->SetEnable(false);
			P3_BookOpened->SetEnable(false);
			P3_BookLockOpenAnim->SetEnable(false);
		}
		else
		{
			//Book Opened
			P3_BookOnTable->SetEnable(false);
			P3_BookLockStatic->SetEnable(false);
			P3_BookLockOpenAnim->SetFrame(18);//final opened frame. TODO replace this with open static frame

			P3_BeetleFix->SetEnable(false);
			P3_BeetleGlowUp->SetEnable(false);
			P3_BeetleGlowDown->SetEnable(false);
		}

		if(ProfileSystem::Instance->ActiveProfile.L1S4_IsInvShadowHand2Taken)
			P3_ShadowHand->SetEnable(false);

		if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetlePlacedOnBook)
		{
			P3_BeetleFix->SetEnable(false);
			P3_BeetleGlowUp->SetEnable(false);
			P3_BeetleGlowDown->SetEnable(false);
		}

		if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleBookTakenFromShelf)
		{
			P3_BookOnTable->SetEnable(false);
			P3_BookLockStatic->SetEnable(false);

			if(ProfileSystem::Instance->ActiveProfile.L1S4_IsBookCodePlaced)
				P3_HandWithCode->SetEnable(true);
		}
		else
		{
			P3_TakingBook->SetEnable(false);
		}

		if(ProfileSystem::Instance->ActiveProfile.L1S4_IsBooksOnShelfMoved)
		{
			P3_BookSlide->SetEnable(false);
			P3_BooksOnShelf->SetEnable(false);			
		}
		else
		{
			P3_BooksOnShelfMoved->SetEnable(false);
		}

		if(ProfileSystem::Instance->ActiveProfile.L1S4_InvKeySetTaken)
		{
			P3_KeySet->SetEnable(false);
		}

		Pop3BeetleShelf->Show(false);
		Pop3BeetleShelf->SetPosOff(CenterPoint.x,CenterPoint.y);
		Pop3BeetleShelf->SetZOff(eZOrder_Popup);
	}
	Popup3_State = POPUP_CLOSED;

	PopupAlpha = 0.0f;
	PopVal = 0.0f;

	GFInstance->PostUpdate(1);
}

void L1_S4_Bedroom::SparkleUpdate()
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

		if(ProfileSystem::Instance->ActiveProfile.L1S4_IsCreekOutroPlayed)
		{
			
			if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvDrawerDiaryTaken && ProfileSystem::Instance->ActiveProfile.L1S3_NagaKeyTaken)
			{
				sparklePoints[numSparkles].SetXY(PopRect1Drawer.GetCenterX(), PopRect1Drawer.GetCenterY());
				numSparkles++;
			}
			else if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerWaxMelted && ProfileSystem::Instance->ActiveProfile.L1S3_MatchBoxTaken)
			{
				sparklePoints[numSparkles].SetXY(PopRect1Drawer.GetCenterX(), PopRect1Drawer.GetCenterY());
				numSparkles++;
			}
			else if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleHeadTaken && ProfileSystem::Instance->ActiveProfile.L1S4_InvCoinTaken)
			{
				sparklePoints[numSparkles].SetXY(PopRect1Drawer.GetCenterX(), PopRect1Drawer.GetCenterY());
				numSparkles++;
			}

			if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsInvShadowHand2Taken && ProfileSystem::Instance->ActiveProfile.L1S4_InvBookCodeTaken)
			{
				sparklePoints[numSparkles].SetXY(PopRect3BeetleShelf.GetCenterX(), PopRect3BeetleShelf.GetCenterY());
				numSparkles++;
			}
			else if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvKeySetTaken && ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleHeadTaken && ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleTailTaken)
			{
				sparklePoints[numSparkles].SetXY(PopRect3BeetleShelf.GetCenterX(), PopRect3BeetleShelf.GetCenterY());
				numSparkles++;
			}

			if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvPaintingFrameTaken )
			{
				sparklePoints[numSparkles].SetXY(PaintAndShadowPuzzleRect.GetCenterX(), PaintAndShadowPuzzleRect.GetCenterY());
				numSparkles++;
			}
			else if(!ProfileSystem::Instance->ActiveProfile.L1S3_IsShadowPuzzleSolved && ProfileSystem::Instance->ActiveProfile.L1S3_IsInvShadowHand1Taken && ProfileSystem::Instance->ActiveProfile.L1S4_IsInvShadowHand2Taken)
			{
				sparklePoints[numSparkles].SetXY(PaintAndShadowPuzzleRect.GetCenterX(), PaintAndShadowPuzzleRect.GetCenterY());
				numSparkles++;
			}
			else if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsSecretTreasureDoorOpened && ProfileSystem::Instance->ActiveProfile.L1S4_InvKeySetTaken)
			{
				sparklePoints[numSparkles].SetXY(PaintAndShadowPuzzleRect.GetCenterX(), PaintAndShadowPuzzleRect.GetCenterY());
				numSparkles++;
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

void L1_S4_Bedroom::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( IsPopupTransitionActive )
		UpdatePopupTransition();
}

void L1_S4_Bedroom::Update()
{
	NavigateToScene();
	if(isDialogActive)
	{
		if(dialogSystem!=NULL)
			dialogSystem->Update();
		return;
	}

	if(GFInstance->IsSceneGraphicReloaded)
	{
		UpdateBgGraphic();
		GFInstance->IsSceneGraphicReloaded = false;
	}

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	if(isMayaPushVideoPlaying)
	{
		Cursor::SetMode(CA_Normal);
		//Subtitles Code

		time = cv_MayaPushVideo->GetTime();

		if (time >= subtitles2[subIndex2].begin && time < subtitles2[subIndex2].end)
		{
			subtitleText->SetText(subtitles2[subIndex2].text);
		}
		else
			subtitleText->SetText("");

		if (time > subtitles2[subIndex2].end)
		{
			if(subMax2-1 > subIndex2)
				subIndex2++;
		}

		vidTimer += Control::LogicRate;

		if( IsVidSkippable )
		{
			if(SkipShowTimer>2.0f)
			{
				if(!skipObj->Visible)
					skipObj->Show(true);

				CPoint mousePos = Control::Input->Mpos();
				IsSkipMouseOver = ( mousePos.x >= (SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)) && 
					mousePos.y <= (SKIPIMAGE_Y + SKIPIMAGE_HEIGHT) &&
					mousePos.y >= SKIPIMAGE_Y &&
					mousePos.x <= ((SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)) + SKIPIMAGE_WIDTH) );

				if( IsSkipMouseOver && !IsSkipMouseClicked)  
				{
					Cursor::SetMode(CA_HandPointy);
					skipObj->PlayAnimation(1);
					IsSkipMouseClicked = Control::Input->LBclicked();					
					if( IsSkipMouseClicked )
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

				if(
#ifndef FORCE_CLOSE_VID_END
					cv_MayaPushVideo->isEndReached() ||
#endif
					IsSkipMouseClicked || vidTimer > 14)
				{
					subtitleText->SetText("");
					IsSkipMouseClicked = false;

					cv_MayaPushVideo->StopVideo();
					isMayaPushVideoPlaying = false;

					Hud->AllowInput = true;
					Hud->ShowHud(false);

					//Control::Audio->StopSample(aTrackCutscnS4MayaPush);

					ProfileSystem::Instance->ActiveProfile.L1S4_MayaPushMaryVideoPlayed = true;

					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S5_TreasureRoom;
					SendMessage(ControlExit, ID, HOSM_NewArea);
					Hud->ShowBannerText("L1ouch");
					return;
				}
			}
			else
				SkipShowTimer += 10*Control::LogicRate;
		}
		return;
	}

	if(isCreekVideoPlaying)
	{
		Cursor::SetMode(CA_Normal);
		if(isCreekIntroPlaying)
		{
			vidTimer2 += Control::LogicRate;

			if( IsVidSkippable )
			{
				if(SkipShowTimer>2.0f)
				{
					if(!skipObj->Visible)
						skipObj->Show(true);

					CPoint mousePos = Control::Input->Mpos();
					IsSkipMouseOver = ( mousePos.x >= (SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)) && 
						mousePos.y <= (SKIPIMAGE_Y + SKIPIMAGE_HEIGHT) &&
						mousePos.y >= SKIPIMAGE_Y &&
						mousePos.x <= ((SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)) + SKIPIMAGE_WIDTH) );

					if( IsSkipMouseOver && !IsSkipMouseClicked)  
					{
						Cursor::SetMode(CA_HandPointy);
						skipObj->PlayAnimation(1);
						IsSkipMouseClicked = Control::Input->LBclicked();					
						if( IsSkipMouseClicked )
						{
							skipObj->Show(false);
							Control::Audio->PlaySample(aSFXClick);
							Control::Input->ClearMouse();
							AwardManager::GetInstance()->SkipDialogueOrCutScene();
						}
					}
					else //Cursor not over skip region
					{
						skipObj->PlayAnimation(0);
					}

					if(
#ifndef FORCE_CLOSE_VID_END
						cv_CreekAppear->isEndReached() ||
#endif
						IsSkipMouseClicked || vidTimer2 > 9)
					{
						Cursor::SetMode(CA_Normal);
						subtitleText->SetText("");
						IsSkipMouseClicked = false;
						skipObj->Show(false);

						cv_CreekAppear->StopAndFadeOutVideo(2.0f);
						isCreekIntroPlaying = false;

						ProfileSystem::Instance->ActiveProfile.L1S4_IsCreekIntroPlayed = true;

						creekTalkScn->FadeOut(2.0f);
						creekTalkScn->Show(true);

						dialogSystem->ShowDialog(eDialog_S4Creek, true);
						isDialogActive = true;
						/*Hud->EnableInput(false);
						Hud->HideHud();*/

						if( !Control::Audio->IsPlaying(aTrackL1Hunted) )
							Control::Audio->PlaySample(aTrackL1Hunted,true);
						if( !Control::Audio->IsPlaying(aAmbS4Bedroom) )
							Control::Audio->PlaySample(aAmbS4Bedroom,true);
	
						//Control::Audio->StopSample(aTrackCutscnS4CreekAppears);
						return;
					}
				}
				else
					SkipShowTimer += 10*Control::LogicRate;
			}
		}
		else if(isCreekOutroPlaying)
		{
			vidTimer3 += Control::LogicRate;

			if( IsVidSkippable )
			{
				if(SkipShowTimer>2.0f)
				{
					if(!skipObj->Visible)
						skipObj->Show(true);

					time = cv_CreekTalks->GetTime();

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

					CPoint mousePos = Control::Input->Mpos();
					IsSkipMouseOver = ( mousePos.x >= (SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)) && 
						mousePos.y <= (SKIPIMAGE_Y + SKIPIMAGE_HEIGHT) &&
						mousePos.y >= SKIPIMAGE_Y &&
						mousePos.x <= ((SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)) + SKIPIMAGE_WIDTH) );

					if( IsSkipMouseOver && !IsSkipMouseClicked)  
					{
						Cursor::SetMode(CA_HandPointy);
						skipObj->PlayAnimation(1);
						IsSkipMouseClicked = Control::Input->LBclicked();					
						if( IsSkipMouseClicked )
						{
							skipObj->Show(false);
							Control::Audio->PlaySample(aSFXClick);
							Control::Input->ClearMouse();
							AwardManager::GetInstance()->SkipDialogueOrCutScene();
						}
					}
					else //Cursor not over skip region
					{
						skipObj->PlayAnimation(0);
					}

					if(
#ifndef FORCE_CLOSE_VID_END
						cv_CreekTalks->isEndReached() ||
#endif
					IsSkipMouseClicked || vidTimer3 > 29)
					{
						IsSkipMouseClicked = false;

						Hud->EnableInput(true);
						Hud->ShowHud();

						subtitleText->SetText("");

						cv_CreekTalks->StopAndFadeOutVideo(2.0f);
						isCreekOutroPlaying = false;

						if( !Control::Audio->IsPlaying(aTrackL1Hunted) )
							Control::Audio->PlaySample(aTrackL1Hunted,true);
						if( !Control::Audio->IsPlaying(aAmbS4Bedroom) )
							Control::Audio->PlaySample(aAmbS4Bedroom,true);
	
						
						#ifdef _CEBUILD
							Hud->PauseAwardUI(false);
						#endif 

						Hud->ShowBannerText("L1sophia");
						Control::Audio->PlaySample(aVOL1MaryRefS4_1);
						//Journal Entry 10 Creeks warning
						if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P10_CreekEscaped])
						{
							//Flag Set
							ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P10_CreekEscaped] = true;
							//Add to Note Array
							ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected]
								= eJrn_L1P10_CreekEscaped;
						
							ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;
							//Increment Notes
							ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
							Hud->ShowJrnEntryFX();
						}

						Hud->UnlockCharacter(eChar4_Creek);

						isCreekVideoPlaying = false;

						skipObj->Show(false);
						return;
					}
				}
				else
					SkipShowTimer += 10*Control::LogicRate;
			}
		}
		return;
	}
	if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsCreekIntroPlayed)
	{
		if(creekVidStartTimer<0)
		{
			isCreekVideoPlaying = true;
			cv_CreekAppear->PlayFadeInVideo(1.0f);
			isCreekIntroPlaying = true;

			IsVidSkippable = true;
			SkipShowTimer = 0;
			
			Hud->EnableInput(false);
			Hud->HideHud();

			if( Control::Audio->IsPlaying(aTrackL1Mansion) )
				Control::Audio->StopSample(aTrackL1Mansion);
			if( Control::Audio->IsPlaying(aAmbS4Bedroom) )
				Control::Audio->StopSample(aAmbS4Bedroom);
	
			//Control::Audio->PlaySample(aTrackCutscnS4CreekAppears,false);
		}
		else
			creekVidStartTimer -= Control::LogicRate;
		return;
	}

	if(IsSecretTreasureDoorSlidePlaying)
	{
		HOScene->update(Control::LogicRate);
		if(CAnim_SlideTreasureDoor->IsAnimFinished())
		{
			IsSecretTreasureDoorSlidePlaying = false;
			Scn_SlidingDoor->SetEnable(false);
			Scn_SlidingDoorMask->SetEnable(false);

			dustFallVFX->StopEmit();

			ProfileSystem::Instance->ActiveProfile.L1S4_IsSecretTreasureDoorOpened = true;
			TaskPendingUpdate();
			isSparkleUpdatePending = true;
			Hud->ShowBannerText("L1secret");
			Control::Audio->QuickLoadAndPlaySFX(aSFXJrnlFlyer);
		}
		return;
	}

	CPoint pos = Control::Input->Mpos();
#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
#else
	Cursor::SetMode(CA_Normal);
#endif 

	if(IsZOrderChangeToScene)
	{
		IsZOrderChangeToScene = false;
		SetZ(zOrderScnVideo);
	}
	
	if( ActivePopupID == ePopInactive )
	{
		if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
			SparkleUpdate();


		if(chairSwingTimer<0)
		{
			if(ActionRectChair.Intersection(&pos) && !HOScene->GetObjectByID("chair")->IsTweening)
			{
				Cursor::SetMode(CA_HandTake);
				if( Control::Input->LBclicked() )
				{
					Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4ChairSway);
					HOScene->GetObjectByID("chair")->Animate(0,0,6,0,2.0f);
					chairSwingTimer = 4.5f;
				}
			}
		}
		else
		{
			chairSwingTimer -= Control::LogicRate;
			if(chairSwingTimer<0)
			{
				//tween ease out to zero - appsr

				HOScene->GetObjectByID("chair")->Status = SPR_VISIBLE;
				float vals[5]={1.0f,HOScene->GetObjectByID("chair")->Pos.x, HOScene->GetObjectByID("chair")->Pos.y, 1.0f, 0};
				HOScene->GetObjectByID("chair")->TweenTo(vals,0.70f,TW_EASE,true);
				chairSwingTimer = -1;
			}
		}

		lightningTimer += Control::LogicRate;
		if(lightningTimer>nextLightningThreshold)
		{
			Scn_lightning->Show(true);
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
				Scn_lightning->Show(false);
				nextLightningThreshold = 12.0f + (float)(rand()%8);
			}
			Scn_lightning->SetAlpha(lightAlphaArray6[index]);
		}

		if( !ProfileSystem::Instance->ActiveProfile.L1S4_InvPaintingFrameTaken && PaintAndShadowPuzzleRect.Intersection(&pos) )
		{
#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
				//Cursor::SetMode(CA_Look);
#else
			Cursor::SetMode(CA_Look);
#endif
			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L1_S4_PZPainting, 0, PaintAndShadowPuzzleRect);
				}
#else
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S4_PZPainting;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
			return;
		}
		else if( !ProfileSystem::Instance->ActiveProfile.L1S4_IsShadowPuzFinalKeyTurned && PaintAndShadowPuzzleRect.Intersection(&pos) )
		{
#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
				//Cursor::SetMode(CA_Look);
#else
			Cursor::SetMode(CA_Look);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L1_S4_PZDoor, 0, PaintAndShadowPuzzleRect);
				}
#else
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S4_PZDoor;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
			return;
		}
		
		if( ProfileSystem::Instance->ActiveProfile.L1S4_IsSecretTreasureDoorOpened && SceneRectTreasureRoom.Intersection(&pos) )
		{
			Cursor::SetMode(CA_ExitUp, eArea_L1_S5_TreasureRoom);
			if( Control::Input->LBclicked() )
			{
				if(!ProfileSystem::Instance->ActiveProfile.L1S4_MayaPushMaryVideoPlayed)
				{
					cv_MayaPushVideo->PlayFadeInVideo(2.0f);
					isMayaPushVideoPlaying = true;

					Hud->EnableInput(false);
					Hud->HideHud(false);

					IsVidSkippable = true;
					SkipShowTimer = 0;

					if( Control::Audio->IsPlaying(aTrackL1Mansion) )
						Control::Audio->StopSample(aTrackL1Mansion);
					if( Control::Audio->IsPlaying(aTrackL1Hunted) )
						Control::Audio->StopSample(aTrackL1Hunted);
					if( Control::Audio->IsPlaying(aAmbS4Bedroom) )
						Control::Audio->StopSample(aAmbS4Bedroom);

					//Control::Audio->PlaySample(aTrackCutscnS4MayaPush,false);

					HOScene->FadeOut(2.0f);
				}
				else
				{
					//usually never happens
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S5_TreasureRoom;
					SendMessage(ControlExit, ID, HOSM_NewArea);
				}
			}
			return;
		}

		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_Foyer;
				SendMessage(ControlExit, ID, HOSM_NewArea);								
			return;
		}
		#endif

		if( IsMouseOverGoBackArea(&pos ))
		{
#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
				//Cursor::SetMode(CA_ExitDown,eArea_L1_S3_Foyer);
#else
			Cursor::SetMode(CA_ExitDown,eArea_L1_S3_Foyer);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L1_S3_Foyer, 2);
				}
#else
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_Foyer;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}			
			return;
		}
		if(ProfileSystem::Instance->ActiveProfile.L1S4_IsCreekOutroPlayed)
		{
			if( InfoRectMap.Intersection(&pos) )
			{
				if( Control::Input->LBclicked() )
				{
					Hud->ShowBannerText("L1map");
				}
			}

			if( InfoRectBed.Intersection(&pos) )
			{
				if( Control::Input->LBclicked() )
				{
					Hud->ShowBannerText("L1bed");
				}
			}
		}
	}

	if( !IsPopupTransitionActive )
		UpdatePopups(pos);
}

void L1_S4_Bedroom::UpdatePopups(CPoint pos,bool ItemDropped)
{
	//Pop1_Drawer
	if ( !ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleHeadTaken && (ActivePopupID==ePopInactive || ActivePopupID==ePopL1S4_P1Drawer) )
	{
		switch( Popup1_State )
		{
		case POPUP_CLOSED:
			if( PopRect1Drawer.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Look);
				if( Control::Input->LBclicked() || ItemDropped )
				{
					UpdateBgGraphic();

					ActivePop_State = POPUP_SCALEUP;
					ActivePopupID = ePopL1S4_P1Drawer;

					Pop_CloseButton = Pop1Drawer->GetObjectByID("popClose");

					//Control::Audio->PlaySample(aSFXpopup);
					PopVal = 0;
					PopupAlpha = 0.0f;
					SetupPopupTransition(Pop1Drawer,PopRect1Drawer);
				}
				return;
			}
			break;
		
		case POPUP_OPEN:
			Pop_CloseButton->PlayAnimation(0);

			if(IsP1DrawerOpeningPlaying)
			{
				if(IsP1SnakeKeyAnimPlaying)
				{
					if(P1_SnakeLockAnim->IsAnimationFinished())
					{
						IsP1SnakeKeyAnimPlaying = false;

						P1_DrawerClosed->FadeOut(4.0f);
						P1_SnakeLockAnim->FadeOut(4.0f);
						P1_LeftLockAnim->FadeOut(4.0f);

						P1_DrawerOpened->SetMaskRect(MaskRectDraweOpen);

						CAnim_DrawerOpening->PlayAnim();
						P1_Diary->SetEnable(true);
						P1_DrawerOpened->SetEnable(true);
					}
				}
				else
				{
					if(CAnim_DrawerOpening->IsAnimFinished())
					{
						P1_DrawerOpened->pSprite->IsMasked = false;
						IsP1DrawerOpeningPlaying = false;

						HOScene->GetObjectByID("drawerdooropen")->SetEnable(true);

						P1_SecretSpace->SetEnable(true);
						P1_BeetleHead->SetEnable(true);
						P1_SmallDoor->SetEnable(true);
						P1_Screw1->SetEnable(true);
						P1_Screw2->SetEnable(true);
						P1_WaxStatic->SetEnable(true);

						UpdateBgGraphic();
					}
				}
				return;
			}

			if(IsP1DiaryTakingPlaying)
			{
				diaryTakeAnimTimer += Control::LogicRate;
				if(!isKeyFallSoundPlayed && diaryTakeAnimTimer>1.0f)
				{
					isKeyFallSoundPlayed = true;
					Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4P1KeyLost);
				}
				if(CAnim_DiaryTaking->IsAnimFinished())
				{
					IsP1DiaryTakingPlaying = false;
					P1_FallingKey->SetEnable(false);

					diaryTakeDelayTimer = 1.5f;

					Hud->ShowBannerText("L1fell");
					ProfileSystem::Instance->ActiveProfile.L1S4_IsKeyFallenDown = true;
				}
				return;
			}
			else
			{
				if(diaryTakeDelayTimer>0)
				{
					diaryTakeDelayTimer -= Control::LogicRate;
					if(diaryTakeDelayTimer<=0)
					{
						diaryTakeDelayTimer = -1;
						if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvDrawerDiaryTaken)
						{
							//Take the Diary
							P1_Diary->SetEnable(false);
							P1_BookKeyMask->SetEnable(false);

							TakeInventoryItem(eInv_L1S4_Diary);
						}
					}
					return;
				}
			}

			if(IsP1MatchStickPlaying)
			{
				if(CAnim_P1MatchStickLighting->IsAnimFinished())
				{
					IsP1MatchStickPlaying = false;

					P1_MatchboxInside->SetEnable(false);
					P1_Matchbox->SetEnable(false);
					P1_MatchStick->SetEnable(false);
					P1_MatchStickMask->SetEnable(false);
					P1_MatchSpark->SetEnable(false);

					P1_WaxStatic->SetEnable(false);
					P1_WaxMeltAnim->SetEnable(true);
					P1_WaxMeltAnim->PlayAnimation(1);

					IsP1WaxMeltingPlaying = true;
				}
				return;
			}

			if(IsP1WaxMeltingPlaying)
			{
				if(P1_WaxMeltAnim->IsAnimationFinished())
				{
					P1_WaxMeltAnim->SetEnable(false);
					P1_MatchFlameStick->SetEnable(false);

					IsP1WaxMeltingPlaying = false;
				}
				return;
			}

			if(IsCoinAnim1Playing || IsCoinAnim2Playing)
			{
				if(IsCoinAnim1Playing && P1_CoinAnim1->IsAnimationFinished())
				{
					IsCoinAnim1Playing = false;

					P1_CoinAnim1->SetEnable(false);

					P1_Screw2->SetEnable(false);
					P1_CoinAnim2->SetEnable(true);
					P1_CoinAnim2->PlayAnimation(1);

					IsCoinAnim2Playing = true;
				}
				else if(IsCoinAnim2Playing && P1_CoinAnim2->IsAnimationFinished())
				{
					IsCoinAnim2Playing = false;

					P1_CoinAnim2->SetEnable(false);

					ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerScrewsTakenout = true;
				}
				return;
			}

			if( !PopAreaRect.Intersection(&pos) || PopCloseButtonRect.Intersection(&pos) )//clicked outside or close Btn
			{
				if(PopCloseButtonRect.Intersection(&pos))
					Cursor::SetMode(CA_HandPointy);
				Pop_CloseButton->PlayAnimation(1);
				if( Control::Input->LBclicked() )
				{
					CloseActivePopup();
				}
				return;
			}
			else
			{
				if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerSnakeKeyFixed )
				{
					if (InvDestRectSnakeKey.Intersection(&pos))
					{
						Cursor::SetMode(CA_Gear);
						if( Control::Input->LBclicked() )
						{
							P1_DrawerClosed->Shake(90.0f,3.0f,0,0.25f);
							Control::Audio->PlaySample(aSFXWoodShake);
							Hud->ShowBannerText("L1miss",SFX_NIL);
						}
					}
				}
				else
				{
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerOpened )
					{
						if (ActionRectTurnSnakeKey.Intersection(&pos))
						{
							Cursor::SetMode(CA_HandTake);
							if( Control::Input->LBclicked() )
							{
								Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4P1DrawerOpened);

								IsP1DrawerOpeningPlaying = true;

								ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerOpened = true;

								IsP1SnakeKeyAnimPlaying = true;
								P1_LeftLockAnim->PlayAnimation(1);
								P1_SnakeLockAnim->PlayAnimation(1);
							} 
						}
					}
					else
					{
						//drawer is opened. Take the diary from inside
						if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsKeyFallenDown)
						{
							if (ActionRectTakeDiary.Intersection(&pos))
							{
								Cursor::SetMode(CA_HandTake);
								if( Control::Input->LBclicked() )
								{
									IsP1DiaryTakingPlaying = true;

									//ProfileSystem::Instance->ActiveProfile.L1S4_InvDrawerDiaryTaken = true;

									Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4P1DiaryTake);

									CAnim_DiaryTaking->PlayAnim();
									P1_BookKeyMask->SetEnable(true);
									P1_FallingKey->SetEnable(true);
								} 
							}
						}
						else
						{
							if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerWaxMelted)
							{
								if (InvDestRectWaxAnim.Intersection(&pos))
								{
									Cursor::SetMode(CA_Gear);
									if( Control::Input->LBclicked() )
									{
										Hud->ShowBannerText("L1wax");
									} 
								}
							}
							else if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerScrewsTakenout)
							{
								if (InvDestRectScrewTake.Intersection(&pos))
								{
									Cursor::SetMode(CA_Gear);
									if( Control::Input->LBclicked() )
									{
										P1_SmallDoor->Shake(90.0f,2.0f,0,0.25f);
										Hud->ShowBannerText("L1scr");
									} 
								}
							}
							else if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerSecretDoorOpened)
							{
								if (InvDestRectScrewTake.Intersection(&pos))
								{
									Cursor::SetMode(CA_HandTake);
									if( Control::Input->LBclicked() )
									{
										Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4ShadowPieceSlide);
										ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerSecretDoorOpened = true;
										float valArr[5] = {0, P1_SmallDoor->Pos.x, P1_SmallDoor->Pos.y-40.0f, 1.0f, 0};
										P1_SmallDoor->TweenTo(valArr, 1.0f, 0, true);
									} 
								}
							}
							else if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleHeadTaken)
							{
								if (InvDestRectScrewTake.Intersection(&pos))
								{
									Cursor::SetMode(CA_HandTake);
									if( Control::Input->LBclicked() )
									{
										P1_BeetleHead->SetEnable(false);

										TakeInventoryItem(eInv_L1S4_BeetleHead);

										CloseActivePopup(true);
									} 
								}
							}
						}
					}
				}
			}
			break;
		} 
	}

	//Pop2_Keyfall
	if ( (ActivePopupID==ePopL1S4_P1Drawer || ActivePopupID==ePopL1S4_P2KeyFall) && ProfileSystem::Instance->ActiveProfile.L1S4_InvDrawerDiaryTaken)
	{
		switch( Popup2_State )
		{
		case POPUP_CLOSED:
			if( PopRect2Keyfall.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Look);
				if( Control::Input->LBclicked() || ItemDropped )
				{
					ActivePop_State = POPUP_SCALEUP;
					ActivePopupID = ePopL1S4_P2KeyFall;

					Pop_CloseButton = Pop2KeyFall->GetObjectByID("popClose");

					PopVal = 0;
					PopupAlpha = 0.2f;
					SetupPopupTransition(Pop2KeyFall,PopRect2Keyfall);
				}
				return;
			}
			break;
		
		case POPUP_OPEN:
			Pop_CloseButton->PlayAnimation(0);
			if( Elixir::Distance(&pos,&CenterPoint) > 240 || Pop_CloseButton->Collide(&pos) )//clicked outside or close Btn
			{
				if(Pop_CloseButton->Collide(&pos))
					Cursor::SetMode(CA_HandPointy);
				Pop_CloseButton->PlayAnimation(1);
				if( Control::Input->LBclicked() )
				{
					CloseActivePopup();
				}
				return;
			}
			else
			{
				if(InfoRectFallenKey.Intersection(&pos))
				{
					Cursor::SetMode(CA_Help);
					if( Control::Input->LBclicked() )
					{
						Hud->ShowBannerText("L1key");
					}
				}
			}
			break;
		} 
	}

	#ifdef _CEBUILD
		bool isMemoryCollected = ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S4BoxingGloves];
	#else
		bool isMemoryCollected = false;
	#endif 

	//Pop3_BeetleShelf
	if ( (ActivePopupID==ePopInactive || ActivePopupID==ePopL1S4_P3BeetleShelf) && !(ProfileSystem::Instance->ActiveProfile.L1S4_InvKeySetTaken && ProfileSystem::Instance->ActiveProfile.L1S4_IsInvShadowHand2Taken && isMemoryCollected))
	{
		switch( Popup3_State )
		{
		case POPUP_CLOSED:
			if( PopRect3BeetleShelf.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Look);
				if( Control::Input->LBclicked() || ItemDropped )
				{
					UpdateBgGraphic();

					ActivePop_State = POPUP_SCALEUP;
					ActivePopupID = ePopL1S4_P3BeetleShelf;

					Pop_CloseButton = Pop3BeetleShelf->GetObjectByID("popClose");

					PopVal = 0;
					PopupAlpha = 0.0f;
					SetupPopupTransition(Pop3BeetleShelf,PopRect3BeetleShelf);
				}
				return;
			}
			break;
		
		case POPUP_OPEN:
			if(IsP3BookTakingAnimPlaying)
			{
				if(CAnim_bookTaking->IsAnimFinished())
				{
					IsP3BookTakingAnimPlaying = false;
					P3_TakingBook->SetEnable(false);
				}
				return;
			}

			if(IsP3BeetleFixAnimPlaying)
			{
				if(IsP3LockOpenAnimPlaying)
				{
					if(P3_BookLockOpenAnim->IsAnimationFinished())
					{
						IsP3LockOpenAnimPlaying = false;
						IsP3BeetleFixAnimPlaying = false;
					}
				}
				else
				{
					if(CAnim_beetleFixing->IsAnimFinished())
					{
						IsP3LockOpenAnimPlaying = true;
						P3_BookLockStatic->SetEnable(false);
						P3_BookLockOpenAnim->SetEnable(true);
						P3_BookLockOpenAnim->ForcePlayAnimation(0);
					}
				}
				return;
			}

			Pop_CloseButton->PlayAnimation(0);
			if( !PopAreaRect.Intersection(&pos) || PopCloseButtonRect.Intersection(&pos) )//clicked outside or close Btn
			{
				if(PopCloseButtonRect.Intersection(&pos))
					Cursor::SetMode(CA_HandPointy);
				Pop_CloseButton->PlayAnimation(1);
				if( Control::Input->LBclicked() )
				{
					CloseActivePopup();
				}
				return;
			}
			else
			{
				#ifdef _CEBUILD
					if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S4BoxingGloves] && MemRectBoxGloves.Intersection(&pos))
					{			
						if( Control::Input->LBclicked() )
						{
							Pop3BeetleShelf->GetObjectByID("p3memboxgloves")->FadeOut();
							Hud->ShowMemoryObjectCollected_CE(eMem_L1S4BoxingGloves, &Pop3BeetleShelf->GetObjectByID("p3memboxgloves")->Pos);

							//time to go back to nosatlgic music!!
							if( !Control::Audio->IsPlaying(aTrackL1Mansion) &&
								!Control::Audio->IsPlaying(aTrackL1Hunted) &&
								!Control::Audio->IsPlaying(aTrackL1EscapePlan) )
								 Control::Audio->PlaySample(aTrackL1Mansion,false);

							if(ProfileSystem::Instance->ActiveProfile.L1S4_InvKeySetTaken && ProfileSystem::Instance->ActiveProfile.L1S4_IsInvShadowHand2Taken)
								CloseActivePopup(true);
							return;
						}
					}
				#else
					if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S4BoxingGloves] && MemRectBoxGloves.Intersection(&pos))
					{	
						Cursor::SetMode(CA_Help);
						if( Control::Input->LBclicked() )
						{
							Pop3BeetleShelf->GetObjectByID("p3memboxgloves")->FadeOut();
							Hud->ShowMemoryObjectCollected_SE(eMem_L1S4BoxingGloves, &Pop3BeetleShelf->GetObjectByID("p3memboxgloves")->Pos);
						}
					}
				#endif 

				if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleBookTakenFromShelf)
				{
					if(ProfileSystem::Instance->ActiveProfile.L1S4_IsBookCodePlaced) //if the symbol is found User can take the correct book
					{
						if(P3RectBookShelf.Intersection(&pos))
						{
							Cursor::SetMode(CA_HandTake);
							if(P3BookTakePoly->Intersection(&pos))
							{
								if( Control::Input->LBclicked() )
								{
									if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsBooksOnShelfMoved)
									{
										ProfileSystem::Instance->ActiveProfile.L1S4_IsBooksOnShelfMoved = true;
										P3_BookSlide->SetEnable(false);

										Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4BooksMoved);

										P3_BooksOnShelf->FadeOut(4.0f);
										P3_BooksOnShelfMoved->FadeIn(4.0f);
										P3_BooksOnShelfMoved->SetEnable(true);
									}
									else
									{
										ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleBookTakenFromShelf = true;

										P3_HandWithCode->FadeOut(1.5f);

										IsP3BookTakingAnimPlaying = true;
										P3_BookOnTable->SetEnable(true);
										P3_BookLockStatic->SetEnable(true);
										CAnim_bookTaking->PlayAnim();

										Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4BookPicked);
									}
								}
							}
							else
							{
								if( Control::Input->LBclicked() )
								{
									Hud->ShowBannerText("L1book");
								}
							}
						}
					}
					else
					{
						if(P3RectBookShelf.Intersection(&pos))
						{
							Cursor::SetMode(CA_Gear);
							if( Control::Input->LBclicked() )
							{
								P3_BookSlide->ForcePlayAnimation(1);
								Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4BooksMoved);
								Hud->ShowBannerText("L1symbol");
							}
						}
					}
				}
				else
				{
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsInvShadowHand2Taken)
					{
						if(InvItemRectShadowHand.Intersection(&pos))
						{
							Cursor::SetMode(CA_HandTake);
							if( Control::Input->LBclicked() )
							{
								P3_ShadowHand->SetEnable(false);

								TakeInventoryItem(eInv_L1S4_ShadowHand2);

								if(ProfileSystem::Instance->ActiveProfile.L1S4_InvKeySetTaken)
									CloseActivePopup(true);
							}
						}
					}
					//Book is placed on table
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetlePlacedOnBook)
					{
						if(InvDestRectBeetleFix.Intersection(&pos))
						{
							Cursor::SetMode(CA_Gear);
							if( Control::Input->LBclicked() )
							{
								P3_BookOnTable->Shake(90,2,0,0.25f);
								Hud->ShowBannerText("L1scarab");
							}
						}
					}
					else
					{
						//Beetle is placed
						if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleBookOpened)
						{
							if(ActionRectBookOpen.Intersection(&pos))
							{
								Cursor::SetMode(CA_HandTake);
								if( Control::Input->LBclicked() )
								{
									//Fadein opened book
									P3_BookOnTable->FadeOut(3.0f);
									P3_BeetleFix->FadeOut(3.0f);
									P3_BeetleGlowUp->FadeOut(3.0f);
									P3_BeetleGlowDown->FadeOut(3.0f);

									P3_BookOpened->FadeIn(4.0f);
									P3_BookOpened->SetEnable(true);

									P3_KeySet->FadeIn(4.0f);
									P3_KeySet->SetEnable(true);

									Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4BookOpened);

									ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleBookOpened = true;
									Control::Input->ClearMouse();
								}
							}
						}//Book is opened
						else if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvKeySetTaken)
						{
							if(InvItemRectKeySet.Intersection(&pos))
							{
								Cursor::SetMode(CA_HandTake);
								if( Control::Input->LBclicked() )
								{
									P3_KeySet->SetEnable(false);

									Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4KeySetPick);

									TakeInventoryItem(eInv_L1S4_KeySet);

									if(ProfileSystem::Instance->ActiveProfile.L1S4_IsInvShadowHand2Taken)
										CloseActivePopup(true);
								}
							}
						}
					}
				}
			}
			break;
		} 
	}
}

void L1_S4_Bedroom::CloseActivePopup(bool isDelayed)
{
	if(isDelayed)
		popupCloseDelayTimer = 1.0f;

	Control::Audio->PlaySample(aSFXpopdown);
	ActivePop_State = POPUP_SCALEDOWN;

	if( ActivePopupID==ePopL1S4_P1Drawer )
	{
		Pop1Drawer->Show(false);
		SetupPopupTransition(Pop1Drawer,PopRect1Drawer);
	}
	else if( ActivePopupID==ePopL1S4_P2KeyFall)
	{
		Pop2KeyFall->Show(false);
		SetupPopupTransition(Pop2KeyFall,PopRect2Keyfall);
	}
	else if( ActivePopupID==ePopL1S4_P3BeetleShelf )
	{
		Pop3BeetleShelf->Show(false);
		SetupPopupTransition(Pop3BeetleShelf,PopRect3BeetleShelf);
	}
}

void L1_S4_Bedroom::UpdatePopupTransition()
{
	
	if(ActivePopupID != ePopL1S4_P2KeyFall)
	{
		if(!GFInstance->IsScnBg1024Res)
		{
			//1366 resolution
			RT_BgScn->SetScale(1.0f + PopupAlpha * 0.25f);
			RT_BgScn->SetXY(bgScaleOffsetX - (bgScaleOffsetX * (1.0f + PopupAlpha  * 0.25f)) ,bgScaleOffsetY - (bgScaleOffsetY * (1.0f + PopupAlpha * 0.25f)));
		}
		else
		{
			//1024 resolution
			RT_BgScn->SetScale(1.0f + PopupAlpha * 0.25f);
			RT_BgScn->SetXY(bgScaleOffsetX - (bgScaleOffsetX * (1.0f + PopupAlpha  * 0.25f)) +171.0f ,bgScaleOffsetY - (bgScaleOffsetY * (1.0f + PopupAlpha * 0.25f)));
		}
	}

	switch( ActivePop_State )
	{
	case POPUP_SCALEUP:
		PopVal += POPUP_SCALE_FACTOR *  Control::LogicRate;
		PopupAlpha = easingQuadOut(PopVal,0,1,1.0f);
		if( PopVal >= 1.0f )
		{
			IsPopupTransitionActive = false;
			PopupAlpha = 1.0f;

			if( ActivePopupID == ePopL1S4_P1Drawer)
			{
				Pop1Drawer->Show(true);
				Popup1_State = POPUP_OPEN;
			}
			else if( ActivePopupID == ePopL1S4_P2KeyFall)
			{
				Pop2KeyFall->Show(true);
				Popup2_State = POPUP_OPEN;
			}
			else if( ActivePopupID == ePopL1S4_P3BeetleShelf)
			{
				Pop3BeetleShelf->Show(true);
				Popup3_State = POPUP_OPEN;
			}
		}
		if( ActivePopupID != ePopL1S4_P2KeyFall)
			GFInstance->UpdateBlurScene(PopupAlpha>1.0f?1.0f:PopupAlpha);
		else
			PopOverPopMask->SetAlpha(PopupAlpha>1.0f?1.0f:PopupAlpha);
		break;
	case POPUP_SCALEDOWN:
		popupCloseDelayTimer -= 1.0f * Control::LogicRate;
		if( popupCloseDelayTimer > 0.0f )
			return;
		else
			popupCloseDelayTimer = 0.0;
			
		PopupAlpha -= POPUP_SCALEDOWN_FACTOR * Control::LogicRate;
		if( PopupAlpha <= 0.0f )
		{
			IsPopupTransitionActive = false;
			PopupAlpha = 0.0f;
			if( ActivePopupID != ePopL1S4_P2KeyFall)
			{
				GFInstance->TurnOffBlurScene();
				EndBgGraphicScaling();
			}

			if( ActivePopupID == ePopL1S4_P1Drawer)
				Popup1_State = POPUP_CLOSED;
			else if( ActivePopupID == ePopL1S4_P2KeyFall)
				Popup2_State = POPUP_CLOSED;
			else if( ActivePopupID == ePopL1S4_P3BeetleShelf)
				Popup3_State = POPUP_CLOSED;

			if( ActivePopupID == ePopL1S4_P2KeyFall)
			{
				PopOverPopMask->Show(false);

				PopVal = 1.0f;
				PopupAlpha = 1.0f;
				ActivePopupID = ePopL1S4_P1Drawer;
				Pop_CloseButton = Pop1Drawer->GetObjectByID("popClose");
			}
			else
			{
				ActivePopupID = ePopInactive;
				HOScene->PauseObjects(false);
			}
		}		
		else
		{
			if( ActivePopupID != ePopL1S4_P2KeyFall)
				GFInstance->UpdateBlurScene(PopupAlpha);
			else
				PopOverPopMask->SetAlpha(PopupAlpha>1.0f?1.0f:PopupAlpha);
		}
		break;
	}
}

float L1_S4_Bedroom::easingQuadOut(float t,float b,float c,float d)
{
	return (c * -t*(t-2) / d + b);
}

void L1_S4_Bedroom::Render(const CPoint* pCam)
{  
	if( !Visible ) 
		return;

	static int FrameCnt = 0;
	FrameCnt = (FrameCnt+1)%2;

	if(ActivePopupID != ePopInactive)
	{
		if(IsPopupTransitionActive)
		{

            //Init again if render target lost due to fullscreen-window switches or similar
			if( GFInstance->RTGraphic != NULL )
			{
				GFInstance->RTGraphic->blitAlphaRectFx(0,0,1366,768,
				short(GFInstance->PStartX+float(GFInstance->PEndX-GFInstance->PStartX)*PopupAlpha),
				short(GFInstance->PStartY+float(GFInstance->PEndY-GFInstance->PStartY)*PopupAlpha),
				-5*(1.0f-PopupAlpha),
				PopupAlpha,PopupAlpha>1.0f?1.0f:PopupAlpha);

				//Gradient alpha causes alpha on transition, leaking the bg scene through popups during transition
				GFInstance->RTGraphic->blitAlphaRectFx(0,0,1366,768,
				short(GFInstance->PStartX+float(GFInstance->PEndX-GFInstance->PStartX)*PopupAlpha),
				short(GFInstance->PStartY+float(GFInstance->PEndY-GFInstance->PStartY)*PopupAlpha),
				-5*(1.0f-PopupAlpha),
				PopupAlpha,PopupAlpha>1.0f?1.0f:PopupAlpha);
            }
		}	
	}
}

void L1_S4_Bedroom::EndBgGraphicScaling()
{
	RT_BgScn->SetScale(1.0f);
	RT_BgScn->SetEnable(false);

    int size = (int) VFXArray.size();  //Sac: converted Implicit to explicit

	for (int i = 0; i < size; i++)		
		VFXArray[i]->Show(true);

	Scn_Bg->Show(true);
}

void L1_S4_Bedroom::UpdateBgGraphic()
{
	if(RT_BgScn == NULL)
	{
		RT_BgScn = new CObject();
		RT_BgScn->LoadSprite("UI\\ZOOMBG.SPR");
		RT_BgScn->SetZ(19);
		RT_BgScn->SetEnable(false);
	}

    
        // Make Inactive Not for Bg Scaling
        Scn_lightning->SetEnable(false);
    if(GFInstance->IsScnRTCreated)
    {
        GFInstance->CurrentSceneGraphic->clearRenderTarget();
        GFInstance->CurrentSceneGraphic->beginRenderToTarget();//RENDER TO TARGET STARTS
        GFInstance->IsRenderingToRT = true;
    }
        if(GFInstance->IsScnBg1024Res)
            HOScene->RenderToTarget(-171.0f);
        else
            HOScene->RenderToTarget();
    if(GFInstance->IsScnRTCreated)
    {
        GFInstance->IsRenderingToRT = false;
        GFInstance->CurrentSceneGraphic->endRenderToTarget();
    }
        //Make Active
        Scn_lightning->SetEnable(true);
	RT_BgScn->pSprite->Image = GFInstance->CurrentSceneGraphic;
}

void  L1_S4_Bedroom::SetupPopupTransition(CHOScene *Popup, CRectangle popRect)
{

		if(ActivePopupID != ePopL1S4_P2KeyFall)
		{
		//	Scn_Bg->Show(false); Sac: not needed here, it makes the backgroud disapper

			HOScene->PauseObjects(true);

            int size = (int) VFXArray.size();  //Sac: converted Implicit to explicit

			for (int i = 0; i < size; i++)		
				VFXArray[i]->Show(false);
		
			bgScaleOffsetX = popRect.GetCenterX();
			bgScaleOffsetY = popRect.GetCenterY();
		
			RT_BgScn->SetEnable(true);
		}
		else
		{
			PopOverPopMask->SetAlpha(0);
			PopOverPopMask->Show(true);
		}

		IsPopupTransitionActive = true;
    if( GFInstance->IsRTCreated )
    {
		GFInstance->RTGraphic->clearRenderTarget();   
		GFInstance->RTGraphic->beginRenderToTarget();//RENDER TO TARGET STARTS	
		GFInstance->IsRenderingToRT = true;
    }
		Popup->RenderToTarget(-171,0);
    if( GFInstance->IsRTCreated )
    {
		GFInstance->IsRenderingToRT = false;
		GFInstance->RTGraphic->endRenderToTarget();
    }
		GFInstance->SetPopupTween( (int)popRect.GetCenterX(-683) ,(int)popRect.GetCenterY(-384 ),171,0);

		if(ActivePop_State == POPUP_SCALEUP && ActivePopupID != ePopL1S4_P2KeyFall) //|| Popup2_State == POPUP_SCALEUP not used currently
			GFInstance->CreateGreyscaleScene();

		if(ActivePopupID == ePopL1S4_P2KeyFall)
			SetZ(zOrderPopVideo+300);
		else
			SetZ(zOrderPopVideo);

}

L1_S4_Bedroom::~L1_S4_Bedroom()
{
	TaskPendingUpdate();
    int size = (int) VFXArray.size();  //Sac: converted Implicit to explicit
    for (int i = 0; i < size; i++){
        SAFE_DELETE(VFXArray[i]);
    }
    size =(int) custAnimArray.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++){
		SAFE_DELETE(custAnimArray[i]);
	}

	if( GFInstance->availablePhyMem >= MAX_LOWEND_PEAKMEMORY )
	{
	}

	SAFE_DELETE(Scn_SlidingDoor);
	SAFE_DELETE(dustFallVFX);
	SAFE_DELETE(CAnim_SlideTreasureDoor);

	SAFE_DELETE(Pop1Drawer);
	SAFE_DELETE(Pop2KeyFall);
	SAFE_DELETE(Pop3BeetleShelf);

	SAFE_DELETE(PopOverPopMask);

	SAFE_DELETE(CAnim_DrawerOpening);
	SAFE_DELETE(CAnim_DiaryTaking);
	SAFE_DELETE(CAnim_P1MatchStickLighting);

	SAFE_DELETE(CAnim_bookTaking);
	SAFE_DELETE(CAnim_beetleFixing);
	SAFE_DELETE(P3BookTakePoly);

	SAFE_DELETE(creekTalkScn);

	if(RT_BgScn!=NULL)
		RT_BgScn->pSprite->Image = NULL;
	SAFE_DELETE(RT_BgScn);

	SAFE_DELETE(cv_CreekAppear);
	SAFE_DELETE(cv_CreekTalks);
	SAFE_DELETE(cv_MayaPushVideo);
	SAFE_DELETE(skipObj);

	//MUSIC_SFX
	Control::Audio->UnloadSample(aTrackCutscnS4CreekAppears);
	Control::Audio->UnloadSample(aTrackCutscnS4MayaPush);

	Control::Audio->StopSample(aAmbS4Bedroom);
	Control::Audio->UnloadSample(aAmbS4Bedroom);
	Control::Audio->UnloadSample(aVOL1MaryRefS4_1);
	//MUSIC_SFX

	SAFE_DELETE(subtitleText);
}

void L1_S4_Bedroom::TakeInventoryItem(EHOInventory ItemID)
{
	std::vector<int> vecInvIds;
	switch (ItemID)
	{
	case eInv_L1S4_ShadowHand2:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_ShadowHand2] = true;
			ProfileSystem::Instance->ActiveProfile.L1S4_IsInvShadowHand2Taken = true;			
			vecInvIds.push_back(eInv_L1S4_ShadowHand2);
			Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
			isSparkleUpdatePending = true;
		break;
	case eInv_L1S4_KeySet:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_KeySet] = true;
			ProfileSystem::Instance->ActiveProfile.L1S4_InvKeySetTaken = true;			
			vecInvIds.push_back(eInv_L1S4_KeySet);
			Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
			isSparkleUpdatePending = true;
		break;
	case eInv_L1S4_Diary:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[ItemID] = true;
			ProfileSystem::Instance->ActiveProfile.L1S4_InvDrawerDiaryTaken = true;
			vecInvIds.push_back(ItemID);
			Hud->ShowInventory(&vecInvIds);
			isSparkleUpdatePending = true;
		break;
	case eInv_L1S4_BeetleHead:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[ItemID] = true;
			ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleHeadTaken = true;
			vecInvIds.push_back(ItemID);
			Hud->ShowInventory(&vecInvIds);
			isSparkleUpdatePending = true;
		break;
	default:
		break;
	}
}

void L1_S4_Bedroom::ReceiveMessage(int val)
{	
	if( val == HOSM_DialogClosed ) 
	{
		isDialogActive = false;

		if(ProfileSystem::Instance->ActiveProfile.L1S4_IsCreekDialogPlayed)
		{
			//play outro video
			isCreekVideoPlaying = true; //if dialog activate from constructor

			cv_CreekTalks->PlayVideo();
			isCreekOutroPlaying = true;

			IsVidSkippable = true;
			SkipShowTimer = 0;

			if( Control::Audio->IsPlaying(aTrackL1Mansion) )
				Control::Audio->StopSample(aTrackL1Mansion);
			if( Control::Audio->IsPlaying(aTrackL1Hunted) )
				Control::Audio->StopSample(aTrackL1Hunted);
			if( Control::Audio->IsPlaying(aAmbS4Bedroom) )
				Control::Audio->StopSample(aAmbS4Bedroom);
	

			ProfileSystem::Instance->ActiveProfile.L1S4_IsCreekOutroPlayed = true;
			isSparkleUpdatePending = true;
		}
		else
		{
			Hud->EnableInput(true);
			Hud->ShowHud();
		}
	}
	else if( val == MAP_NEW_AREA ) 
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
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			if(ActivePopupID == ePopInactive)
			{
				//check for popup actions
				UpdatePopups(dropPos,true);
				if(ActivePopupID == ePopInactive)
				{
					//If No popups responded
					GFInstance->WrongInvDropMsg(Hud);
				}
				return;
			}
			if( itemVal == eInv_L1S3_MatchBox )
			{
				if( Popup1_State == POPUP_OPEN )
				{
					if(ProfileSystem::Instance->ActiveProfile.L1S4_InvDrawerDiaryTaken && !ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerWaxMelted)
					{
						if(InvDestRectWaxAnim.Intersection(&dropPos))
						{
							CAnim_P1MatchStickLighting->PlayAnim();

							P1_MatchboxInside->SetEnable(true);
							P1_Matchbox->SetEnable(true);
							P1_MatchStick->SetEnable(true);
							P1_MatchStickMask->SetEnable(true);
							P1_MatchSpark->SetEnable(true);
							P1_MatchFlameStick->SetEnable(true);

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4P1WaxMelted);

							IsP1MatchStickPlaying = true;

							Hud->ReleaseDrag();
							Hud->CloseInventory(); 

							ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerWaxMelted = true;
							isSparkleUpdatePending = true;
						}
						else
							GFInstance->WrongInvDropMsg(Hud);
					}
					else
						GFInstance->WrongInvDropMsg(Hud);
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if( itemVal == eInv_L1S4_Coin )
			{
				if( Popup1_State == POPUP_OPEN && !IsP1WaxMeltingPlaying && !IsP1MatchStickPlaying)
				{
					if(ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerWaxMelted && !ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerScrewsTakenout)
					{
						if(InvDestRectScrewTake.Intersection(&dropPos))
						{
							P1_Screw1->SetEnable(false);

							P1_CoinAnim1->SetEnable(true);
							P1_CoinAnim1->PlayAnimation(1);

							IsCoinAnim1Playing = true;

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4CoinUsed);

							Hud->ReleaseDrag();
							Hud->CloseInventory(); 

							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_Coin] = false;

							Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S4_Coin);
						}
						else
							GFInstance->WrongInvDropMsg(Hud);
					}
					else
						GFInstance->WrongInvDropMsg(Hud);
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if( itemVal == eInv_L1S1_SwissKnife)
			{
				if( !ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerWaxMelted )
				{
					if(InvDestRectWaxAnim.Intersection(&dropPos))
					{
						Hud->ShowBannerText("L1blade");
						Hud->ReleaseDrag();
						Hud->CloseInventory(); 
					}
					else
						GFInstance->WrongInvDropMsg(Hud);
				}
				//wax is melted
				else if( !ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerScrewsTakenout)
				{
					if(InvDestRectScrewTake.Intersection(&dropPos))
					{
						Hud->ShowBannerText("L1thin");
						Hud->ReleaseDrag();
						Hud->CloseInventory(); 
					}
					else
						GFInstance->WrongInvDropMsg(Hud);
				}
				else
						GFInstance->WrongInvDropMsg(Hud);
			}
			else if( itemVal == eInv_L1S3_NagaKey )
			{
				if( Popup1_State == POPUP_OPEN )
				{
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerSnakeKeyFixed)
					{
						if(InvDestRectSnakeKey.Intersection(&dropPos))
						{
							P1_SnakeLockAnim->SetEnable(true);
							P1_LeftLockAnim->SetEnable(true);

							P1_LeftLockStatic->SetEnable(false);
							P1_RightLockStatic->SetEnable(false);

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4P1NagaKeyFixed);

							Hud->ReleaseDrag();
							Hud->CloseInventory(); 

							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S3_NagaKey] = false;

							Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S3_NagaKey);

							ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerSnakeKeyFixed = true;
						}
						else
							GFInstance->WrongInvDropMsg(Hud);
					}
					else
						GFInstance->WrongInvDropMsg(Hud);
				}				
			}
			else if( itemVal == eInv_L1CB_Beetle )
			{
				if( Popup3_State == POPUP_OPEN )
				{
					if( ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleBookTakenFromShelf && !ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetlePlacedOnBook )
					{
						if(InvDestRectBeetleFix.Intersection(&dropPos))
						{
							ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetlePlacedOnBook = true;						
							
							IsP3BeetleFixAnimPlaying = true;
							CAnim_beetleFixing->PlayAnim();
							P3_BeetleFix->SetEnable(true);
							P3_BeetleGlowUp->SetEnable(true);
							P3_BeetleGlowDown->SetEnable(true);

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4BeetleFixedOnBook);

							Hud->ReleaseDrag();
							Hud->CloseInventory(); 

							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1CB_Beetle] = false;

							Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1CB_Beetle);
						}
						else
							GFInstance->WrongInvDropMsg(Hud);
					}
					else
						GFInstance->WrongInvDropMsg(Hud);
					return;
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if( itemVal == eInv_L1S4_BeetleTail )
			{
				if( Popup3_State == POPUP_OPEN )
				{
					if( ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleBookTakenFromShelf && !ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetlePlacedOnBook )
					{
						if(InvDestRectBeetleFix.Intersection(&dropPos))
						{
							Hud->ReleaseDrag();
							Hud->CloseInventory(); 

							Hud->ShowBannerText("L1incomplete");
						}
						else
							GFInstance->WrongInvDropMsg(Hud);
					}
					else
						GFInstance->WrongInvDropMsg(Hud);
					return;
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if( itemVal == eInv_L1S4_BookCode )
			{
				if( Popup3_State == POPUP_OPEN )
				{
					if( !ProfileSystem::Instance->ActiveProfile.L1S4_IsBookCodePlaced )
					{
						if(InvDestRectBookCode.Intersection(&dropPos))
						{
							ProfileSystem::Instance->ActiveProfile.L1S4_IsBookCodePlaced = true;						
							
							P3_HandWithCode->SetAlpha(0);
							P3_HandWithCode->FadeIn(1.5f);
							P3_HandWithCode->SetEnable(true);

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S4CodeSymbolFixed);

							Hud->ReleaseDrag();
							//Hud->CloseInventory();  

							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S4_BookCode] = false;

							GFHud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S4_BookCode);
						}
						else
							GFInstance->WrongInvDropMsg(Hud);
					}
					else
						GFInstance->WrongInvDropMsg(Hud);
					return;
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else
				GFInstance->WrongInvDropMsg(Hud);
		}

		UpdateBgGraphic();
	}
	else if( val == INV_CLOSE )
	{
		if(Hud->Tutor != NULL && Hud->Tutor->IsActive && !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT18_ComboInvZoomLook])
		{
			//if beetle tail taken from painting
			//if combo inv popup showing. Tutorial will get activate when closing the combo inv popup
			if(!Hud->IsComboInvPopupShowing && ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleTailTaken && ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleHeadTaken)
			{
				Hud->Tutor->ShowTutorial(TUT18_ComboInvZoomLook);
			}
		}

		
		
	}	
}

void L1_S4_Bedroom::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S4BoxingGloves])
		{
			if(Popup3_State == POPUP_OPEN)
				InitHintTrailEmit(900, 450);
			else
				InitHintTrailEmit(1035, 280);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}

	if(isMayaPushVideoPlaying || isCreekVideoPlaying || IsSecretTreasureDoorSlidePlaying )
		return;

	int iNonHOHint = 0; 
	int MaxNonHOHints = 35;
	do
	{
		switch( iNonHOHint )
		{
			default:			
			//Exit coordinates
			case 0: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerSnakeKeyFixed && ProfileSystem::Instance->ActiveProfile.L1S3_NagaKeyTaken && Popup1_State == POPUP_OPEN)
					{
						Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S3_NagaKey)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}

							CPoint tempPoint (InvDestRectSnakeKey.GetCenterX(), InvDestRectSnakeKey.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						return;
					}
					break;
			case 1: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerOpened && ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerSnakeKeyFixed && Popup1_State == POPUP_OPEN)
					{
						InitHintTrailEmit(ActionRectTurnSnakeKey.GetCenterX(), ActionRectTurnSnakeKey.GetCenterY(),false);
						return;
					}
					break;
			case 2: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvDrawerDiaryTaken && ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerOpened && Popup1_State == POPUP_OPEN)
					{
						InitHintTrailEmit(ActionRectTakeDiary.GetCenterX(), ActionRectTakeDiary.GetCenterY(),false);
						return;
					}
					break;
			case 3: ++iNonHOHint;
					if( ProfileSystem::Instance->ActiveProfile.L1S4_IsKeyFallenDown && Popup2_State == POPUP_OPEN)
					{
						InitHintTrailEmit(Pop_CloseButton->Pos.x, Pop_CloseButton->Pos.y,true);
						return;
					}
					break;

			case 4: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerWaxMelted && ProfileSystem::Instance->ActiveProfile.L1S4_IsKeyFallenDown && ProfileSystem::Instance->ActiveProfile.L1S3_MatchBoxTaken && Popup1_State == POPUP_OPEN)
					{
						Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S3_MatchBox)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}

							CPoint tempPoint (InvDestRectWaxAnim.GetCenterX(), InvDestRectWaxAnim.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						return;
					}
					break;
			case 5: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerScrewsTakenout && ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerWaxMelted && ProfileSystem::Instance->ActiveProfile.L1S4_InvCoinTaken && Popup1_State == POPUP_OPEN)
					{
						Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S4_Coin)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}

							CPoint tempPoint (InvDestRectScrewTake.GetCenterX(), InvDestRectScrewTake.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						return;
					}
					break;
			case 6: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerSecretDoorOpened && ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerScrewsTakenout && Popup1_State == POPUP_OPEN)
					{
						InitHintTrailEmit(InvDestRectScrewTake.GetCenterX(), InvDestRectScrewTake.GetCenterY(),false);
						return;
					}
					break;
			case 7: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleHeadTaken && ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerSecretDoorOpened && Popup1_State == POPUP_OPEN)
					{
						InitHintTrailEmit(InvDestRectScrewTake.GetCenterX(), InvDestRectScrewTake.GetCenterY(),false);
						return;
					}
					break;
			case 8: ++iNonHOHint;
					 if( Popup1_State == POPUP_OPEN)
					{
						InitHintTrailEmit(	PopCloseButtonRect.GetCenterX(), PopCloseButtonRect.GetCenterY(),true);
						return;
					}
					break;	
			case 9: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsBookCodePlaced && ProfileSystem::Instance->ActiveProfile.L1S4_InvBookCodeTaken && Popup3_State == POPUP_OPEN)
					{
						Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S4_BookCode)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}

							CPoint tempPoint (InvDestRectBookCode.GetCenterX(), InvDestRectBookCode.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						return;
					}
					break;
			case 10: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleBookTakenFromShelf && ProfileSystem::Instance->ActiveProfile.L1S4_IsBookCodePlaced && Popup3_State == POPUP_OPEN)
					{
						InitHintTrailEmit(InvDestRectBookCode.GetCenterX(), InvDestRectBookCode.GetCenterY(),false);
						return;
					}
					break;
			case 11: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleBookTakenFromShelf && !ProfileSystem::Instance->ActiveProfile.L1S4_IsInvShadowHand2Taken && Popup3_State == POPUP_OPEN)
					{
						InitHintTrailEmit(InvItemRectShadowHand.GetCenterX(), InvItemRectShadowHand.GetCenterY(),false);
						return;
					}
					break;
			case 12: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetlePlacedOnBook && ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleBookTakenFromShelf &&  ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleTailFixed && Popup3_State == POPUP_OPEN)
					{
						Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1CB_Beetle)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}

							CPoint tempPoint (InvDestRectBeetleFix.GetCenterX(), InvDestRectBeetleFix.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						return;
					}
					break;
			case 13: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvKeySetTaken && ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetlePlacedOnBook  && Popup3_State == POPUP_OPEN)
					{
						InitHintTrailEmit(InvDestRectBeetleFix.GetCenterX(), InvDestRectBeetleFix.GetCenterY(),false);
						return;
					}
					break;
			case 14: ++iNonHOHint;
					 if( Popup3_State == POPUP_OPEN)
					{
						InitHintTrailEmit(	PopCloseButtonRect.GetCenterX(), PopCloseButtonRect.GetCenterY(),true);
						return;
					}
					break;	
			case 15: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsSecretTreasureDoorOpened && ProfileSystem::Instance->ActiveProfile.L1S4_InvKeySetTaken && ProfileSystem::Instance->ActiveProfile.L1S4_IsInvShadowHand2Taken && ProfileSystem::Instance->ActiveProfile.L1S3_IsInvShadowHand1Taken )
					{
						InitHintTrailEmit(PaintAndShadowPuzzleRect.GetCenterX(), PaintAndShadowPuzzleRect.GetCenterY(),false);
						return;
					}
					break;
			case 16: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvPaintingFrameTaken )
					{
						InitHintTrailEmit(PaintAndShadowPuzzleRect.GetCenterX(), PaintAndShadowPuzzleRect.GetCenterY(),false);
						return;
					}
					break;
			case 17: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerSnakeKeyFixed && ProfileSystem::Instance->ActiveProfile.L1S3_NagaKeyTaken )
					{
						InitHintTrailEmit(PopRect1Drawer.GetCenterX(), PopRect1Drawer.GetCenterY(),false);
						return;
					}
					break;
			case 18: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerOpened && ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerSnakeKeyFixed )
					{
						InitHintTrailEmit(PopRect1Drawer.GetCenterX(), PopRect1Drawer.GetCenterY(),false);
						return;
					}
					break;
			case 19: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvDrawerDiaryTaken && ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerOpened )
					{
						InitHintTrailEmit(PopRect1Drawer.GetCenterX(), PopRect1Drawer.GetCenterY(),false);
						return;
					}
					break;

			case 20: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerWaxMelted && ProfileSystem::Instance->ActiveProfile.L1S4_IsKeyFallenDown && ProfileSystem::Instance->ActiveProfile.L1S3_MatchBoxTaken )
					{
						InitHintTrailEmit(PopRect1Drawer.GetCenterX(), PopRect1Drawer.GetCenterY(),false);
						return;
					}
					break;
			case 21: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerScrewsTakenout && ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerWaxMelted && ProfileSystem::Instance->ActiveProfile.L1S4_InvCoinTaken )
					{
						InitHintTrailEmit(PopRect1Drawer.GetCenterX(), PopRect1Drawer.GetCenterY(),false);
						return;
					}
					break;
			case 22: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerSecretDoorOpened && ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerScrewsTakenout )
					{
						InitHintTrailEmit(PopRect1Drawer.GetCenterX(), PopRect1Drawer.GetCenterY(),false);
						return;
					}
					break;
			case 23: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleHeadTaken && ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerSecretDoorOpened )
					{
						InitHintTrailEmit(PopRect1Drawer.GetCenterX(), PopRect1Drawer.GetCenterY(),false);
						return;
					}
					break;
			
			case 24: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsBookCodePlaced && ProfileSystem::Instance->ActiveProfile.L1S4_InvBookCodeTaken )
					{
						InitHintTrailEmit(PopRect3BeetleShelf.GetCenterX(), PopRect3BeetleShelf.GetCenterY(),false);
						return;
					}
					break;
			case 25: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleBookTakenFromShelf && ProfileSystem::Instance->ActiveProfile.L1S4_IsBookCodePlaced )
					{
						InitHintTrailEmit(PopRect3BeetleShelf.GetCenterX(), PopRect3BeetleShelf.GetCenterY(),false);
						return;
					}
					break;
			case 26: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleBookTakenFromShelf && !ProfileSystem::Instance->ActiveProfile.L1S4_IsInvShadowHand2Taken )
					{
						InitHintTrailEmit(PopRect3BeetleShelf.GetCenterX(), PopRect3BeetleShelf.GetCenterY(),false);
						return;
					}
					break;
			case 27: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetlePlacedOnBook && ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleBookTakenFromShelf &&  ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleTailFixed)
					{
						InitHintTrailEmit(PopRect3BeetleShelf.GetCenterX(), PopRect3BeetleShelf.GetCenterY(),false);
						return;
					}
					break;
			case 28: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvKeySetTaken && ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetlePlacedOnBook  )
					{
						InitHintTrailEmit(PopRect3BeetleShelf.GetCenterX(), PopRect3BeetleShelf.GetCenterY(),false);
						return;
					}
					break;
			case 29: ++iNonHOHint;
					if((!ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleTailTaken ||!ProfileSystem::Instance->ActiveProfile.L1S4_InvBookCodeTaken)  && ProfileSystem::Instance->ActiveProfile.L1S4_InvPaintingFrameTaken  )
					{																																  
						Hud->InvBoxCtrl->IsLocked = true;																							 
						Hud->InvBoxCtrl->Activate();																								  
						
						for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
						{
							if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
							{	
								if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S4_PhotoFrame)
								{
									if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
											Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
									else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
											Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
									InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,true);
								}
							}
						}
											
						return;
					}
					break;
			case 30: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S4_InvCoinTaken && ProfileSystem::Instance->ActiveProfile.L1S4_InvDrawerDiaryTaken  )
					{																																  
						Hud->InvBoxCtrl->IsLocked = true;																							 
						Hud->InvBoxCtrl->Activate();																								  
						
						for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
						{
							if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
							{	
								if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S4_Diary)
								{
									if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
											Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
									else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
											Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
									InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,true);
								}
							}
						}
											
						return;
					}
					break;
			case 31: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleHeadTaken && ProfileSystem::Instance->ActiveProfile.L1S4_InvBeetleTailTaken && !ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleTailFixed )
					{																																  
						Hud->InvBoxCtrl->IsLocked = true;																							 
						Hud->InvBoxCtrl->Activate();																								  
						
						for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
						{
							if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
							{	
								if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S4_BeetleHead)
								{
									if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
											Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
									else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
											Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
									InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,true);
								}
							}
						}
											
						return;
					}
					break;
			case 32: ++iNonHOHint;
					if( ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S5_TreasureRoom])
					{
						InitHintTrailEmit(SceneRectTreasureRoom.GetCenterX(), SceneRectTreasureRoom.GetCenterY(),true,CA_ExitUp);
						return;
					}
					break;
			case 33: ++iNonHOHint;
					if( ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S3_Foyer])
					{
						InitHintTrailEmit(HoveredBackBtnRectMax.topleft.x + HoveredBackBtnRectMax.w/2 ,HoveredBackBtnRectMax.topleft.y + HoveredBackBtnRectMax.h/2,true,CA_ExitDown);
						return;
					}
					break;
			case 34: ++iNonHOHint;
					 
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

void L1_S4_Bedroom::NavigateToScene()
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
