//====================================
//	DQFYLH
//	HO System
//	Level 1 - Scene 08 : BoatShed
//====================================

#include "L1_S6_BoatShed.h"
#include "Hud.h"
#include "Banners.h"
#include "HOScene.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "Rect.h"
#include "Circle.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "TextureFactory.h"


// pop up close button, pos
const CPoint	CenterPoint(683,334,0);
const CRectangle PopCloseButtonRect(968.0f,71.0f,62.0f,61.0f);
const CPoint	 PopCloseRectBtnPoint(998,98,0);

const CRectangle	PopAreaRect(345.0f,90.0f,665.0f,501.0f);

const CRectangle  PopCloseButtonRect1(968.0f,71.0f,62.0f,61.0f);
const CRectangle  PopAreaRect1(345.0f,90.0f,665.0f,501.0f);

const CCircle	PopCirc1CandleArea(868.0f,406.0f,170.0f);
// x +101 from center point, y -171 from center point
const CRectangle PopCloseCircBtnRect(969.0f,235.0f,66.0f,66.0f);
const CPoint	PopCloseCircBtnPoint(1001,269.0f,0);
//const CPoint	PopCircCloseDisp(133,-137,0); //Pop Circle Rect Close Distance - Add this to Circle Popup Center

const CCircle	PopCirc2ShovelArea(666.0f,300.0f,170.0f);
// x +101 from center point, y -171 from center point
const CRectangle PopCloseCirc2BtnRect(767.0f,129.0f,66.0f,66.0f);
const CPoint	PopCloseCirc2BtnPoint(799.0f,163.0f,0);
//const CPoint	PopCircCloseDisp(133,-137,0); //Pop Circle Rect Close Distance - Add this to Circle Popup Center

const CRectangle  PopRectPumper(486.0f,89.0f,196.0f,283.0f);
const CRectangle  PopRectSwitch(1044.0f,73.0f,128.0f,126.0f);


const CRectangle  SceneRectToolBoxHO(234.0f,442.0f,320.0f,187.0f);

const CRectangle  ActionRectPumper(736.0f,150.0f,228.0f,132.0f);
const CRectangle  ActionRectHandle(527.0f,228.0f,156.0f,165.0f);

const CRectangle  HintRectPumpTop(481.0f,95.0f,292.0f,139.0f);
const CRectangle  HintRectHoseCan(664.0f,190.0f,212.0f,386.0f);
const CRectangle  HintRectBarrel(500.0f,384.0f,231.0f,140.0f);


const CRectangle  InvItemRectCanFull(670.0f,402.0f,211.0f,169.0f);
const CRectangle  InvItemRectCrowbar(311.0f,129.0f,144.0f,195.0f);

const CRectangle  InvDestRectPopup(379.0f,107.0f,618.0f,470.0f);

const CRectangle  InvDestRectHose(539.0f,86.0f,285.0f,486.0f);
const CRectangle  InvDestRectCan(697.0f,419.0f,208.0f,138.0f);

const CRectangle  ActionRectBox(289.0f,192.0f,166.0f,131.0f);
const CRectangle  ActionRectBoxWeb(313.0f,62.0f,170.0f,350.0f);
const CRectangle  ActionRectBroom(546.0f,427.0f,91.0f,125.0f);
const CRectangle  ActionRectDog(787.0f,401.0f,79.0f,125.0f);

const CRectangle  MemRectChapathiRoll(396.0f,431.0f,125.0f,50.0f);

const CRectangle  InfoRectDieselBarrel(474.0f,186.0f,251.0f,189.0f);

float lightFlickerAlpha[24] = {1.0f,0.75f,0.15f,1.0f,1.0f,1.0f,1.0f,0.25f,1.0f,1.0f,1.0f,1.0f,1.0f,0.85f,0.45f,0.15f,0,0,0,0,0,0,0,0};

CPoint BroomPos;


L1_S6_Boatshed::L1_S6_Boatshed()
{
	Visible = false;

	RT_BgScn = NULL;
	IsPopupTransitionActive = false;

	bgScaleOffsetX = 0.0f;
	bgScaleOffsetY = 0.0f;	

	popupCloseDelayTimer = 0.0f;

	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV1\\S6\\S6.SCN");
	
	InitPopups();

	HOScene->GetObjectByID("web")->SetEnable(false);
	HOScene->GetObjectByID("WebBox")->SetEnable(false);

	HOLinesVFX = NULL;
	HODotsVFX = NULL; 

	LightVFX = NULL;

	IsDogBarking = false;
	dogBarkSFXID = 0;	
	dogWhineTimer = 0;
	dogWhineThreshold = 10.0f + (float)(rand()%4);

	scn_dogbarking = HOScene->GetObjectByID("DogBarking");

	Scn_BGDark = NULL;

	lightFrameTimer = 0;
	isLightFlickering = false;

	if(ProfileSystem::Instance->ActiveProfile.L1S6_IsRoomLite)
	{
		LightVFX = new SuperFX();
		LightVFX->LoadEffectIni("PRT\\L1\\LIGHTPRT.INI");
		LightVFX->SetPos(PointSystem::CreateCPoint(296.0f,285.0f,0));
		LightVFX->SetZ(100);
		LightVFX->Start();
		LightVFX->AdvanceByTime(2.0f);

		if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsWebCleaned)
		{

			FgWebBoxMesh = NULL;

			FgWebBoxMesh = new MeshObject("LV1\\S6\\WEBBOX.JPA", 5, 5);
			FgWebBoxMesh->SetPosAndParams(243, 0, -1, 0, 0, 0);
			FgWebBoxMesh->SetZ(15);	

			HOScene->InsertObject(FgWebBoxMesh,20);
		}

		HOScene->GetObjectByID("Light")->SetBlendMode(eBlend_AlphaAdditive);
		HOScene->GetObjectByID("Light")->Animate(0.10f, 0.90f, 3.0f);

		HOScene->GetObjectByID("Chain")->SetPivot(28,-125);
		HOScene->GetObjectByID("Chain")->Animate(0,0,1,0,2.0f);

		HOScene->GetObjectByID("SquirrelTail")->SetPivot(0,35);
		HOScene->GetObjectByID("SquirrelTail")->Animate(0,0,1,0,3.0f);
		HOScene->GetObjectByID("SquirrelHand")->SetPivot(-18,-10);
		HOScene->GetObjectByID("SquirrelHand")->Animate(0,0,5,0,5.0f);

		HOScene->GetObjectByID("Squirrel")->SetPivot(15,35);
		HOScene->GetObjectByID("Squirrel")->Animate(0,0,0.5,0,10.0f);

		if(ProfileSystem::Instance->ActiveProfile.L1S6_HOSquirrelMoved)
		{
			HOScene->GetObjectByID("SquirrelTail")->SetEnable(false);
			HOScene->GetObjectByID("SquirrelHand")->SetEnable(false);
			HOScene->GetObjectByID("Squirrel")->SetEnable(false);
		}

		if(ProfileSystem::Instance->ActiveProfile.L1S6_HOPaperCutScenePlayed)
			HOScene->GetObjectByID("newspaper")->SetEnable(false);

		if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOSolved)
		{
			HOLinesVFX = new SuperFX("PRT\\L1\\HOPRTLINES2.INI", PointSystem::CreateCPoint(353,493,0), 15);
			HODotsVFX = new SuperFX("PRT\\L1\\HOPRDOTS2.INI", PointSystem::CreateCPoint(354,525,0), 15);

			if(!ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles)
			{
				HOLinesVFX->Start();
				HODotsVFX->Start();
			}
		}
	}
	else
	{
		Scn_BGDark = new CObject();
		Scn_BGDark->LoadInit("LV1\\S6\\DARKBG.SPR", 0, 0, 21);

		HOScene->Show(false);
	}

	FgWebMesh = NULL;

	FgWebMesh = new MeshObject("LV1\\S6\\web.jpa", 5, 5);
	FgWebMesh->SetPosAndParams(795, 0, 3, 2, 5, 2);
	FgWebMesh->SetZ(25);
	
	HOScene->PushObject(FgWebMesh);

	IsBoxMovingAnimPlaying = false;
	IsBroomAnimPlaying = false;
	IsbroomTaken = false;

	MoonLightVFX = NULL;

	MoonLightVFX = new SuperFX();
	MoonLightVFX->LoadEffectIni("PRT\\L1\\MOONPTR.INI");
	MoonLightVFX->SetPos(PointSystem::CreateCPoint(1254.0f,151.0f,0));
	MoonLightVFX->SetZ(100);
	MoonLightVFX->Start();
	MoonLightVFX->AdvanceByTime(2.0f);

	if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsPumperPlaced )
	{
		HOScene->GetObjectByID("PumpMask")->SetEnable(false);
		HOScene->GetObjectByID("Pump")->SetEnable(false);
	}
		
	if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsHosePlaced )
	{
		HOScene->GetObjectByID("Pipe")->SetEnable(false);
	}
	if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsCanPlaced)
	{
		HOScene->GetObjectByID("Can")->SetEnable(false);
	}
	if(ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCanFullPicked)
	{
		HOScene->GetObjectByID("Can")->SetEnable(false);
	}
	
	HOScene->GetObjectByID("toolboxopen")->SetEnable(false);
	HOScene->GetObjectByID("BoxMoved")->SetEnable(false);
	HOScene->GetObjectByID("WebBox")->SetEnable(false);
	HOScene->GetObjectByID("broom")->SetEnable(true);

	if(ProfileSystem::Instance->ActiveProfile.L1S6_HOBigBoxOpened)
	{
		HOScene->GetObjectByID("ToolBoxClosed")->SetEnable(false);
		HOScene->GetObjectByID("toolboxopen")->SetEnable(true);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S6_IsBoxDroped)
	{
		HOScene->GetObjectByID("Box")->SetEnable(false);	
		HOScene->GetObjectByID("BoxMoved")->SetEnable(true);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S6_IsWebCleaned)
	{
		HOScene->GetObjectByID("broom")->SetEnable(false);
		
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCrowbarPicked)
	{
		HOScene->GetObjectByID("Crowbar")->SetEnable(false);
	}

	HOScene->GetObjectByID("broomtaken")->SetEnable(false);

	HOScene->GetObjectByID("moonlight")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("moonlight")->Animate(0.15f, 0.50f, 5.0f);

	BroomPos = HOScene->GetObjectByID("broom")->Pos;

	CAnim_WebBox = NULL; CAnim_Broom = NULL;

	CAnim_WebBox = new CustomAnimator("ANIM\\L1\\S8WebBox.anim",HOScene->GetObjectByID("WebBox"));
	CAnim_Broom = new CustomAnimator("ANIM\\L1\\S8Broom.anim",HOScene->GetObjectByID("broomtaken"));

	CAnim_Box = NULL; CAnim_BoxMoved = NULL;

	CAnim_Box = new CustomAnimator("ANIM\\L1\\S8Box.anim",HOScene->GetObjectByID("Box"));
	CAnim_BoxMoved = new CustomAnimator("ANIM\\L1\\S8BoxMoved.anim",HOScene->GetObjectByID("BoxMoved"));	

	InitPopups();

	GFInstance->PostUpdate(2);

	zOrderPopVideo = eZOrder_Popup + 5;

	SetZ(zOrderScnVideo);

	IsZOrderChangeToScene = false;

	//MUSIC_SFX
	Control::Audio->ClearUnusedTracks();

	//After Escape From Tunnel
	if(ProfileSystem::Instance->ActiveProfile.IsStartMusic)
	{
		ProfileSystem::Instance->ActiveProfile.IsStartMusic = false;
		if(ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite)
		{
			if( !Control::Audio->IsPlaying(aTrackL1EscapePlan) )
				Control::Audio->PlaySample(aTrackL1EscapePlan,false);
		}
		else if( ProfileSystem::Instance->ActiveProfile.L1S4_IsCreekOutroPlayed &&
			!ProfileSystem::Instance->ActiveProfile.L1S4_MayaPushMaryVideoPlayed )
		{
			if( !Control::Audio->IsPlaying(aTrackL1Hunted) )
				Control::Audio->PlaySample(aTrackL1Hunted,false);
		}
		else
		{
			if( !Control::Audio->IsPlaying(aTrackL1Mansion) )
				Control::Audio->PlaySample(aTrackL1Mansion,false);
		}
	}

	Control::Audio->LoadSample(aAmbS6ShedRoom,AudioAmb);
	Control::Audio->PlaySample(aAmbS6ShedRoom,true);	
	//MUSIC_SFX
	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L1_S6_BoatShed]  = true;

	if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S6_BoatShed])
		ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S6_BoatShed] = true;
	
	

	
	

	UpdateBgGraphic();

	Visible = true;	//To render anything with renderlistentity
	TaskPendingUpdate();
}

void L1_S6_Boatshed::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;
	if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCrowbarPicked || !ProfileSystem::Instance->ActiveProfile.L1S6_HOInvFunnelTaken)
		isAnyTaskLeft = true;
	if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCanFullPicked && ProfileSystem::Instance->ActiveProfile.L1S2_InvHoseTaken && ProfileSystem::Instance->ActiveProfile.L1S2_InvDieselCanTaken)
		isAnyTaskLeft = true;
	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S6_BoatShed] = isAnyTaskLeft;

	if( ProfileSystem::Instance->ActiveProfile.L1S6_HOInvFunnelTaken &&  ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCanFullPicked)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S7_BoatDock] = true;
	
	if(!ProfileSystem::Instance->ActiveProfile.L1S2_InvDieselCanTaken && ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCrowbarPicked  && ProfileSystem::Instance->ActiveProfile.L1S3_IsBananaTaken)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S2_Frontyard] = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L1S2_InvHoseTaken && ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCrowbarPicked  && ProfileSystem::Instance->ActiveProfile.L1S3_IsBananaTaken)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S2_Frontyard] = true;

}

void L1_S6_Boatshed::InitPopups()
{
	ActivePopupID = ePopInactive;

	IsPopupTransitionActive = false;
	IsPumpingAnimPlaying = false;
	PumpAnimTimer = 0;
	isFlowStarted = false;
	isFillStarted = false;

	Popup1Pumper = NULL;
	Popup2Switch = NULL;

	CAnim_Pumper = NULL; CAnim_Handle = NULL; CAnim_PumperPlaced = NULL;

	if ( !ProfileSystem::Instance->ActiveProfile.L1S6_IsRoomLite )
	{
		Popup2Switch = new CHOScene();
		Popup2Switch->Load("LV1\\S6\\POP2\\POP2.SCN");

		//Add popup frame and close button
		CObject *popCirFrame = new CObject();
		popCirFrame->LoadInit("UI\\popcircframe.SPR", 4, -9, 1);
		popCirFrame->SetUID("popFrame");
		Popup2Switch->InsertObject(popCirFrame,1);

		CObject *popCirCloseBtn = new CObject();
		popCirCloseBtn->LoadInit("UI\\closebtn.SPR", 145, -158, 7);
		popCirCloseBtn->SetUID("popClose");
		Popup2Switch->PushObject(popCirCloseBtn);

		Popup2Switch->Show(false);
		Popup2Switch->SetPosOff(CenterPoint.x,CenterPoint.y);
		Popup2Switch->SetZOff(eZOrder_Popup);

		SparkVFX = new SuperFX("PRT\\L1\\FUSESPARKS.INI", &CenterPoint, eZOrder_Popup+1);

		Popup2Switch->GetObjectByID("SwitchOn")->SetEnable(false);
	}
	else
	{
		Popup1Pumper = new CHOScene();
		Popup1Pumper->Load("LV1\\S6\\POP\\P1.SCN");

		//Add popup frame and close button
		CObject *popFrame = new CObject();
		popFrame->LoadInit("UI\\poprectframe.SPR", 13, -15, eZOrder_PopFrame);
		popFrame->SetUID("popFrame");
		Popup1Pumper->InsertObject(popFrame, 2);

		CObject *popCloseBtn = new CObject();
		popCloseBtn->LoadInit("UI\\closebtn.SPR", 321, -246, eZOrder_PopCloseBtn);
		popCloseBtn->SetUID("popClose");
		Popup1Pumper->PushObject(popCloseBtn);

		/*Popup1Pumper->GetObjectByID("P1PumpHandle")->SetZ(eZOrder_Popup+2);
		Popup1Pumper->GetObjectByID("P1Pump")->SetZ(eZOrder_Popup+1);*/
		Popup1Pumper->Show(false);

		Popup1Pumper->SetPosOff(CenterPoint.x,CenterPoint.y);
		Popup1Pumper->SetZOff(eZOrder_Popup);

		Popup1Pumper->GetObjectByID("HoseAnimComplete")->SetEnable(false);

		if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S6ChappathiRoll])
		{			
			Popup1Pumper->GetObjectByID("P1ChappathiRoll")->SetEnable(false);
		}

		if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsPumperPlaced )
		{
			Popup1Pumper->GetObjectByID("P1Pump")->SetEnable(false);
			Popup1Pumper->GetObjectByID("P1PumpHandle")->SetEnable(false);
			Popup1Pumper->GetObjectByID("P1Pumpshadow")->SetEnable(false);
		}
		else
		{
			Popup1Pumper->GetObjectByID("P1PumpPlaced")->SetEnable(false);
		}
		
		if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsHosePlaced )
		{
			Popup1Pumper->GetObjectByID("HoseAnim")->SetEnable(false);
		}
		if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsCanPlaced )
		{
			Popup1Pumper->GetObjectByID("P1OilCan")->SetEnable(false);
		}
		else if( ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCanFullPicked )
		{
			Popup1Pumper->GetObjectByID("P1OilCan")->SetEnable(false);
			Popup1Pumper->GetObjectByID("FuellFillAnim")->SetEnable(false);
		}

		if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsCanFilled)
		{
			Popup1Pumper->GetObjectByID("FuellFillAnim")->SetEnable(false);
		}

		CAnim_Pumper = new CustomAnimator("ANIM\\L1\\S8pumper.anim",Popup1Pumper->GetObjectByID("P1Pump"));
		CAnim_Handle = new CustomAnimator("ANIM\\L1\\S8handle.anim",Popup1Pumper->GetObjectByID("P1PumpHandle"));
		CAnim_PumperPlaced = new CustomAnimator("ANIM\\L1\\S8pumperplaced.anim",Popup1Pumper->GetObjectByID("P1PumpPlaced"));
		CAnim_Pumpershadow = new CustomAnimator("ANIM\\L1\\S8pumpershadow.anim",Popup1Pumper->GetObjectByID("P1Pumpshadow"));
	}

	Popup2_State = POPUP_CLOSED;
	Popup1_State = POPUP_CLOSED;

	GFInstance->PostUpdate(1);
}

void L1_S6_Boatshed::Required()
{
	
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if(IsPopupTransitionActive)
		UpdatePopupTransition();
}

void L1_S6_Boatshed::Update()
{
	NavigateToScene();
	if(GFInstance->IsSceneGraphicReloaded)
	{
		UpdateBgGraphic();
		GFInstance->IsSceneGraphicReloaded = false;
	}

	if(isLightFlickering)
	{
		lightFrameTimer += Control::LogicRate;
		int index = (int)(lightFrameTimer*24.0f);
		if(lightFrameTimer>=1.0f)
		{
			lightFrameTimer = 0;
			index = 0;
			Scn_BGDark->Show(false);
		}
		if(Scn_BGDark!=NULL)
			Scn_BGDark->SetAlpha(lightFlickerAlpha[index]);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S6_IsRoomLite && !ProfileSystem::Instance->ActiveProfile.L1S6_HOSolved)
	{
		if(!ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles)
		{
			if(HOLinesVFX!=NULL && !HOLinesVFX->Active)
			{
				HOLinesVFX->Start();
				HODotsVFX->Start();
			}
		}
		else
		{
			if(HOLinesVFX!=NULL && HOLinesVFX->Active)
			{
				HOLinesVFX->StopImmediate();
				HODotsVFX->StopImmediate();
			}
		}
	}


	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	if(IsZOrderChangeToScene)
	{
		//Problem with SetZ() inside render function
		SetZ(zOrderScnVideo);
		IsZOrderChangeToScene = false;

		Cursor::Show(true);
	}


#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
#else
	Cursor::SetMode(CA_Normal);
#endif 

	if(IsBoxMovingAnimPlaying)
	{
		if(CAnim_Box->IsAnimFinished())
		{
			HOScene->GetObjectByID("Box")->SetEnable(false);	
			HOScene->GetObjectByID("BoxMoved")->SetEnable(true);
			ProfileSystem::Instance->ActiveProfile.L1S6_IsBoxDroped = true;
			IsBoxMovingAnimPlaying = false;
			isSparkleUpdatePending = true;
		}
		return;
	}

	if(IsBroomAnimPlaying)
	{
		if(CAnim_Broom->IsAnimFinished())
		{
			HOScene->GetObjectByID("broom")->SetEnable(false);	
			HOScene->GetObjectByID("broomtaken")->SetEnable(false);
			HOScene->GetObjectByID("WebBox")->SetEnable(false);
			ProfileSystem::Instance->ActiveProfile.L1S6_IsWebCleaned = true;
			IsBroomAnimPlaying = false;
			isSparkleUpdatePending = true;
		}
		return;
	}

	CPoint pos = Control::Input->Mpos();

	if(IsbroomTaken)
	{
		HOScene->GetObjectByID("broomtaken")->SetPos(&pos);
		if( Control::Input->LBclicked() )
		{
			if(ActionRectBoxWeb.Intersection(&pos))
			{
				FgWebBoxMesh->Show(false);
				FgWebBoxMesh->SetEnable(false);
				HOScene->GetObjectByID("broomtaken")->SetEnable(true);	
				HOScene->GetObjectByID("WebBox")->SetEnable(true);
				CAnim_WebBox->PlayAnim();
				CAnim_Broom->PlayAnim();
				IsbroomTaken = false;
				IsBroomAnimPlaying = true;

				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6WebCleaned);
			}
			else
			{
				HOScene->GetObjectByID("broom")->SetEnable(true);	
				HOScene->GetObjectByID("broomtaken")->SetEnable(false);
				HOScene->GetObjectByID("broomtaken")->SetPos(&BroomPos);
				IsbroomTaken = false;
			}
	
		}	
		return;
	}
	if( ActivePopupID == ePopInactive)
	{		
		if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
			SparkleUpdate();

		if( IsDogBarking )
		{
			if(scn_dogbarking->IsAnimationFinished())
			{
				IsDogBarking = false;
				scn_dogbarking->PlayAnimation(0);
			}
		}
		else
		{
			if(ProfileSystem::Instance->ActiveProfile.L1S6_IsRoomLite)
			{
				if(ActionRectDog.Intersection(&pos) )
				{
					Cursor::SetMode(CA_HandPointy);
					if( Control::Input->LBclicked() )
					{
						scn_dogbarking->PlayAnimation(1);

						if(dogBarkSFXID==0)
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6DogBarkV1);
						else
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6DogBarkV2);

						dogBarkSFXID += 1;
						if( dogBarkSFXID>1 )
							dogBarkSFXID=0;

						IsDogBarking = true;
						dogWhineTimer = 0;

						return;
					}
				}

				dogWhineTimer += Control::LogicRate;
				if(dogWhineTimer>dogWhineThreshold)
				{
					dogWhineTimer = 0;

					Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6DogWhine);
					//Play whine sound
					dogWhineThreshold = 12.0f + (float)(rand()%8);
				}
			}
		}	
	}
	
	if( ActivePopupID == ePopInactive && InvItemRectCrowbar.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S6_IsBoxDroped && !ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCrowbarPicked  )
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked() )
		{
			TakeInventoryItem(eInv_L1S6_Crowbar);
		}			
		return;
	}

	if( ActivePopupID == ePopInactive && ActionRectBox.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S6_IsWebCleaned && !ProfileSystem::Instance->ActiveProfile.L1S6_IsBoxDroped )
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked() )
		{
			HOScene->GetObjectByID("BoxMoved")->SetEnable(true);
			Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6BoxDroped);
			CAnim_Box->PlayAnim();
			CAnim_BoxMoved->PlayAnim();
			IsBoxMovingAnimPlaying = true;
		}			
		return;
	}


	if( ActivePopupID == ePopInactive && ActionRectBroom.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S6_IsWebCleaned && ProfileSystem::Instance->ActiveProfile.L1S6_IsRoomLite)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked() )
		{
			Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6BroomPicked);

			HOScene->GetObjectByID("broom")->SetEnable(false);	
			HOScene->GetObjectByID("broomtaken")->SetEnable(true);
			IsbroomTaken = true;
	
		}			
		return;
	}

	if( ActivePopupID == ePopInactive && ActionRectBox.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S6_IsWebCleaned && ProfileSystem::Instance->ActiveProfile.L1S6_IsRoomLite)
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked() )
		{
			Hud->ShowBannerText("L1spiders");
		}			
		return;
	}

	if( ActivePopupID==ePopInactive && SceneRectToolBoxHO.Intersection(&pos)&& ProfileSystem::Instance->ActiveProfile.L1S6_IsRoomLite && !ProfileSystem::Instance->ActiveProfile.L1S6_HOSolved)
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
				GFHud->InvBoxCtrl->GoToScene(eArea_L1_S6_HOBoatShed, 0, SceneRectToolBoxHO);
			}
#else
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S6_HOBoatShed;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}			
		return;
	}
#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (ActivePopupID == ePopInactive && Control::Input->isKeyBackSensed())
	{
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S7_BoatDock;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		return;
	}
#endif

	if( ActivePopupID==ePopInactive && IsMouseOverGoBackArea(&pos ))
	{
#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
			//Cursor::SetMode(CA_ExitDown,eArea_L1_S7_BoatDock);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L1_S7_BoatDock);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L1_S7_BoatDock, 2);
			}
#else
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S7_BoatDock;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}			
		return;
	}

	if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsRoomLite && !PopRectSwitch.Intersection(&pos) &&  ActivePopupID==ePopInactive)
	{
		Cursor::SetMode(CA_Help);
		if( Control::Input->LBclicked() )
		{
			Hud->ShowBannerText("L1lying");
		}
	}

	if( !IsPopupTransitionActive )
		UpdatePopups(pos);
}

void  L1_S6_Boatshed::SetupPopupTransition(CHOScene *Popup, CRectangle popRect)
{

		if( ActivePopupID==ePopL1S6_P1Pumper )
		{
			//Sac: update...
			//HOScene->Show(false);
			HOScene->PauseObjects(true);
			bgScaleOffsetX = popRect.GetCenterX();
			bgScaleOffsetY = popRect.GetCenterY();
		
			RT_BgScn->SetEnable(true);
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

		if(Popup1_State == POPUP_SCALEUP)
			GFInstance->CreateGreyscaleScene();

		SetZ(zOrderPopVideo);
	
}

void L1_S6_Boatshed::UpdateBgGraphic()
{
	if(RT_BgScn == NULL)
	{
		RT_BgScn = new CObject();
		RT_BgScn->LoadSprite("UI\\ZOOMBG.SPR");
		RT_BgScn->SetZ(30);
		RT_BgScn->SetEnable(false);
	}
    if(GFInstance->IsScnRTCreated)
    {
        //GFInstance->CurrentSceneGraphic->clearRenderTarget();
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
	RT_BgScn->pSprite->Image = GFInstance->CurrentSceneGraphic;
			

}

void L1_S6_Boatshed::UpdatePopupTransition()
{

	if ( ActivePopupID==ePopL1S6_P1Pumper )
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

	if ( ActivePopupID==ePopL1S6_P1Pumper ) 
	{
		switch( Popup1_State )
		{
		case POPUP_SCALEUP:
			PopVal += POPUP_SCALE_FACTOR *  Control::LogicRate;
			PopupAlpha = easingQuadOut(PopVal,0,1,1.0f);

			if( PopVal >= 1.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 1.0f;				
				Popup1Pumper->Show(true);
				Popup1_State = POPUP_OPEN;
			}
			GFInstance->UpdateBlurScene(PopupAlpha>1.0f?1.0f:PopupAlpha);
			break;
		case POPUP_SCALEDOWN:
			popupCloseDelayTimer -= 1.0f * Control::LogicRate;
			if( popupCloseDelayTimer > 0.0f )
			{
				return;
			}
			else
			{
				popupCloseDelayTimer = 0.0;
			}
			PopupAlpha -= POPUP_SCALEDOWN_FACTOR * Control::LogicRate;
			if( PopupAlpha <= 0.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 0.0f;								
				GFInstance->TurnOffBlurScene();
				Popup1_State = POPUP_CLOSED;
				ActivePopupID = ePopInactive;

				EndBgGraphicScaling();

				SetZ(zOrderScnVideo);
			}		
			else
				GFInstance->UpdateBlurScene(PopupAlpha);
			break;
		} 
	}

	if ( ActivePopupID==ePopL1S6_P2Switch ) 
	{
		switch( Popup2_State )
		{
		case POPUP_SCALEUP:
			PopVal += POPUP_SCALE_FACTOR *  Control::LogicRate;
			PopupAlpha = easingQuadOut(PopVal,0,1,1.0f);
			
			if( PopVal >= 1.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 1.0f;				
				Popup2Switch->Show(true);
				Popup2_State = POPUP_OPEN;
			}
			GFInstance->UpdateBlurScene(PopupAlpha>1.0f?1.0f:PopupAlpha);			
			break;
		case POPUP_SCALEDOWN:
			popupCloseDelayTimer -= 1.0f * Control::LogicRate;
			if( popupCloseDelayTimer > 0.0f )
			{
				return;
			}
			else
			{
				popupCloseDelayTimer = 0.0;
			}
			PopupAlpha -= POPUP_SCALEDOWN_FACTOR * Control::LogicRate;
			if( PopupAlpha <= 0.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 0.0f;								
				GFInstance->TurnOffBlurScene();
				Popup2_State = POPUP_CLOSED;
				ActivePopupID = ePopInactive;

				InitPopups();

				//EndBgGraphicScaling();

				SetZ(zOrderScnVideo);
			}		
			else
				GFInstance->UpdateBlurScene(PopupAlpha);
			break;
		} 
	}
}

void L1_S6_Boatshed::TakeInventoryItem(EHOInventory ItemID)
{
	std::vector<int> vecInvIds;
	switch (ItemID)
	{
	case eInv_L1S6_CanFull:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S6_CanFull] = true;
			ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCanFullPicked = true;		
			vecInvIds.push_back(eInv_L1S6_CanFull);
			Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
			isSparkleUpdatePending = true;
			TaskPendingUpdate();
			
		break;
	case eInv_L1S6_Crowbar:
			Hud->InvBoxCtrl->Activate(1);			
			HOScene->GetObjectByID("Crowbar")->SetEnable(false);	
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S6_Crowbar] = true;
			ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCrowbarPicked = true;		
			vecInvIds.push_back(eInv_L1S6_Crowbar);
			Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
			isSparkleUpdatePending = true;
			TaskPendingUpdate();
		break;
	
	default:
		break;
	}
}

void L1_S6_Boatshed::EndBgGraphicScaling()
{
	RT_BgScn->SetScale(1.0f);
	RT_BgScn->SetEnable(false);	

	HOScene->Show(true);
}

void L1_S6_Boatshed::UpdatePopups(CPoint pos, bool ItemDropped)
{
	if ( (ActivePopupID==ePopInactive || ActivePopupID==ePopL1S6_P1Pumper) )
	{
		switch( Popup1_State )
		{
		case POPUP_CLOSED:
			if( PopRectPumper.Intersection(&pos) &&  ProfileSystem::Instance->ActiveProfile.L1S6_IsRoomLite && (!ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCanFullPicked || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S6ChappathiRoll]) )
			{
				Cursor::SetMode(CA_Look);
				if( Control::Input->LBclicked() || ItemDropped )
				{
					Popup1_State = POPUP_SCALEUP;
					ActivePopupID = ePopL1S6_P1Pumper;

					Pop_CloseButton = Popup1Pumper->GetObjectByID("popClose");
					Pop_CloseButton->PlayAnimation(0);

					UpdateBgGraphic();

					//Control::Audio->PlaySample(aSFXpopup);
					PopVal = 0;
					PopupAlpha = 0.0f;				
					SetupPopupTransition(Popup1Pumper,PopRectPumper);
				}
				return;
			}
			break;
		
		case POPUP_OPEN:

			if(IsPumpingAnimPlaying)
			{
				PumpAnimTimer += Control::LogicRate;
				
				if(!isFlowStarted)
				{
					if(PumpAnimTimer>1.5f)
					{
						Popup1Pumper->GetObjectByID("HoseAnim")->PlayAnimation(1);
						isFlowStarted = true;
					}
				}
				if(!isFillStarted)
				{
					if(PumpAnimTimer>2.15f)
					{
						Popup1Pumper->GetObjectByID("FuellFillAnim")->PlayAnimation(1);
						Popup1Pumper->GetObjectByID("FuellFillAnim")->SetEnable(true);
						isFillStarted = true;
					}
				}

				if(PumpAnimTimer>4.0f)
				{
					PumpAnimTimer = 0;
					Popup1Pumper->GetObjectByID("P1PumpHandle")->Status = SPR_VISIBLE;
					IsPumpingAnimPlaying = false;
					
					ProfileSystem::Instance->ActiveProfile.L1S6_IsCanFilled = true;

					//Show Objective completed
					if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L1O5_FindBoatFuel] && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L1O5_FindBoatFuel] )
					{
						ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L1O5_FindBoatFuel] = true;
						Hud->ShowObjectiveFX(eObj_L1O5_FindBoatFuel,true);
					}

					Popup1Pumper->GetObjectByID("HoseAnimComplete")->SetEnable(true);
					Popup1Pumper->GetObjectByID("HoseAnimComplete")->SetAlpha(0.0f);
					Popup1Pumper->GetObjectByID("HoseAnimComplete")->FadeIn(2.5f);
					Popup1Pumper->GetObjectByID("HoseAnim")->FadeOut(1.5f);

				}
				UpdateBgGraphic();
				return;
			}

			if( !PopAreaRect.Intersection(&pos) || PopCloseButtonRect.Intersection(&pos) )//clicked outside or close Btn
			{
				if(PopCloseButtonRect.Intersection(&pos))
					Cursor::SetMode(CA_HandPointy);
				Pop_CloseButton->PlayAnimation(1);
				if( Control::Input->LBclicked() )
				{
					Control::Audio->PlaySample(aSFXpopdown);
					Popup1_State = POPUP_SCALEDOWN;

					Popup1Pumper->Show(false);
					UpdateBgGraphic();
					SetupPopupTransition(Popup1Pumper,PopRectPumper);
				}
				UpdateBgGraphic();
				return;
			}
			else 
			{
				Pop_CloseButton->PlayAnimation(0);

				#ifdef _CEBUILD
					if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S6ChappathiRoll] && MemRectChapathiRoll.Intersection(&pos))
					{			
						if( Control::Input->LBclicked() )
						{
							Popup1Pumper->GetObjectByID("P1ChappathiRoll")->FadeOut();
							Hud->ShowMemoryObjectCollected_CE(eMem_L1S6ChappathiRoll, &Popup1Pumper->GetObjectByID("P1ChappathiRoll")->Pos);

							//time to go back to nosatlgic music!!
							if( !Control::Audio->IsPlaying(aTrackL1Mansion) &&
								!Control::Audio->IsPlaying(aTrackL1Hunted) &&
								!Control::Audio->IsPlaying(aTrackL1EscapePlan) )
								 Control::Audio->PlaySample(aTrackL1Mansion,false);
								 
							return;
						}
					}
				#else
					if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S6ChappathiRoll] && MemRectChapathiRoll.Intersection(&pos))
					{	
						Cursor::SetMode(CA_Help);
						if( Control::Input->LBclicked() )
						{
							Popup1Pumper->GetObjectByID("P1ChappathiRoll")->FadeOut();
							Hud->ShowMemoryObjectCollected_SE(eMem_L1S6ChappathiRoll, &Popup1Pumper->GetObjectByID("P1ChappathiRoll")->Pos);
						}
					}
				#endif 

				if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsPumperPlaced )
				{
					if(ActionRectPumper.Intersection(&pos))
					{
						Cursor::SetMode(CA_HandTake);
						if( Control::Input->LBclicked() )
						{
							Popup1Pumper->GetObjectByID("P1PumpHandle")->SetEnable(true);
							Popup1Pumper->GetObjectByID("P1Pump")->SetEnable(true);
							Popup1Pumper->GetObjectByID("P1Pumpshadow")->SetEnable(true);

							CAnim_Pumper->PlayAnim();
							CAnim_Handle->PlayAnim();
							CAnim_PumperPlaced->PlayAnim();
							CAnim_Pumpershadow->PlayAnim();

							HOScene->GetObjectByID("PumpMask")->SetEnable(true);
							HOScene->GetObjectByID("Pump")->SetEnable(true);

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6PumpFixed);


							ProfileSystem::Instance->ActiveProfile.L1S6_IsPumperPlaced = true;
							isSparkleUpdatePending = true;
						}
					}
					else if(InfoRectDieselBarrel.Intersection(&pos))
					{
						Cursor::SetMode(CA_Help);
						if( Control::Input->LBclicked())
						{
							Hud->ShowBannerText("L1barrel");
						}
					}
				}
				else if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsHosePlaced)
				{
					if(ProfileSystem::Instance->ActiveProfile.L1S6_IsPumperPlaced && ActionRectHandle.Intersection(&pos))
					{
						Cursor::SetMode(CA_HandTake);
						if( Control::Input->LBclicked() )
						{
							Popup1Pumper->GetObjectByID("P1PumpHandle")->SetPivot(-40,-55);
							Popup1Pumper->GetObjectByID("P1PumpHandle")->Shake(0,0,6,0.35f);

							Hud->ShowBannerText("L1hose");
						}
					}
					if(InvDestRectHose.Intersection(&pos))
					{
						Cursor::SetMode(CA_Gear);
						if( Control::Input->LBclicked() )
						{
							Hud->ShowBannerText("L1hose");
						}
					}
				}
				else if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsCanPlaced)
				{
					if(ProfileSystem::Instance->ActiveProfile.L1S6_IsPumperPlaced && ActionRectHandle.Intersection(&pos))
					{
						Cursor::SetMode(CA_HandTake);
						if( Control::Input->LBclicked() )
						{
							Popup1Pumper->GetObjectByID("P1PumpHandle")->SetPivot(-40,-55);
							Popup1Pumper->GetObjectByID("P1PumpHandle")->Shake(0,0,6,0.35f);

							Hud->ShowBannerText("L1waste");
						}
					}
					if(InvDestRectCan.Intersection(&pos))
					{
						Cursor::SetMode(CA_Gear);
						if( Control::Input->LBclicked() )
						{
							Hud->ShowBannerText("L1kind");
						}
					}
				}
				else if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsCanFilled)
				{
					if(ActionRectHandle.Intersection(&pos))
					{
						Cursor::SetMode(CA_HandTake);
						if( Control::Input->LBclicked() )
						{
							Popup1Pumper->GetObjectByID("P1PumpHandle")->SetPivot(-40,-55);
							Popup1Pumper->GetObjectByID("P1PumpHandle")->Animate(0,0,6,0,1.0f);

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6PumpingDiesel);

							IsPumpingAnimPlaying = true;
						}
					}
					else if( HintRectBarrel.Intersection(&pos) || HintRectHoseCan.Intersection(&pos) || HintRectPumpTop.Intersection(&pos) )
					{
						if( Control::Input->LBclicked() )
						{
							Hud->ShowBannerText("L1pump");
						}

					}

				}
				else if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCanFullPicked)//can filled & not picked!!
				{
					if(InvItemRectCanFull.Intersection(&pos))
					{
						Cursor::SetMode(CA_HandTake);
						if( Control::Input->LBclicked() )
						{
							TakeInventoryItem(eInv_L1S6_CanFull);
							
							Popup1Pumper->GetObjectByID("P1OilCan")->SetEnable(false);
							HOScene->GetObjectByID("Can")->SetEnable(false);
							Popup1Pumper->GetObjectByID("FuellFillAnim")->SetEnable(false);

							UpdateBgGraphic();

							Control::Audio->PlaySample(aSFXpopdown);
							Popup1_State = POPUP_SCALEDOWN;

							popupCloseDelayTimer = 1.0f;

							Popup1Pumper->Show(false);
							SetupPopupTransition(Popup1Pumper,PopRectPumper);
						}
					}
				}

				
				return;
				
			}
			
			break;
		} 
	}

	if ( (ActivePopupID==ePopInactive || ActivePopupID==ePopL1S6_P2Switch) )
	{
		switch( Popup2_State )
		{
		case POPUP_CLOSED:
			if( PopRectSwitch.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S6_IsRoomLite )
			{
				Cursor::SetMode(CA_Look);
				if( Control::Input->LBclicked() || ItemDropped )
				{
					Popup2_State = POPUP_SCALEUP;
					ActivePopupID = ePopL1S6_P2Switch;

					Pop_CloseButton = Popup2Switch->GetObjectByID("popClose");
					Pop_CloseButton->PlayAnimation(0);

					//Control::Audio->PlaySample(aSFXpopup);
					PopVal = 0;
					PopupAlpha = 0.0f;				
					SetupPopupTransition(Popup2Switch,PopRectSwitch);
				}
				UpdateBgGraphic();
				return;
			}
			break;
		
		case POPUP_OPEN:
		Pop_CloseButton->PlayAnimation(0);
		if( Elixir::Distance(&pos,&CenterPoint) > 180 || Pop_CloseButton->Collide(&pos) )//clicked outside or close Btn
			{
				if(Pop_CloseButton->Collide(&pos))
					Cursor::SetMode(CA_HandPointy);
				Pop_CloseButton->PlayAnimation(1);
				if( Control::Input->LBclicked() )
				{
					Pop_CloseButton->PlayAnimation(0);

					Control::Audio->PlaySample(aSFXpopdown);
					Popup2_State = POPUP_SCALEDOWN;

					Popup2Switch->Show(false);
					
					SetupPopupTransition(Popup2Switch,PopRectSwitch);
				}
				UpdateBgGraphic();
				return;
			}
		else if( Elixir::Distance(&pos,&CenterPoint) < 120 && !ProfileSystem::Instance->ActiveProfile.L1S6_IsRoomLite )
		{
			Cursor::SetMode(CA_HandPointy);
			if( Control::Input->LBclicked() )
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6SwitchOn);

				Popup2Switch->GetObjectByID("SwitchOn")->SetEnable(true);
				Popup2Switch->GetObjectByID("SwitchOff")->SetEnable(false);

				isLightFlickering = true;

				HOScene->Show(true);

				SparkVFX->Start();
				popupCloseDelayTimer = 1.0f;
				LightVFX = new SuperFX();
				LightVFX->LoadEffectIni("PRT\\L1\\LIGHTPRT.INI");
				LightVFX->SetPos(PointSystem::CreateCPoint(296.0f,285.0f,0));
				LightVFX->SetZ(100);
				LightVFX->Start();
				LightVFX->AdvanceByTime(2.0f);

				FgWebBoxMesh = NULL;

				FgWebBoxMesh = new MeshObject("LV1\\S6\\WEBBOX.JPA", 5, 5);
				FgWebBoxMesh->SetPosAndParams(243, 0, -1, 0, 0, 0);
				FgWebBoxMesh->SetZ(15);	

				HOScene->InsertObject(FgWebBoxMesh,20);
				HOScene->GetObjectByID("Light")->SetEnable(true);
				HOScene->GetObjectByID("Light")->SetBlendMode(eBlend_AlphaAdditive);
				HOScene->GetObjectByID("Light")->Animate(0.10f, 0.90f, 3.0f);

				HOScene->GetObjectByID("Chain")->SetPivot(28,-125);
				HOScene->GetObjectByID("Chain")->Animate(0,0,1,0,2.0f);

				HOScene->GetObjectByID("SquirrelTail")->SetPivot(0,35);
				HOScene->GetObjectByID("SquirrelTail")->Animate(0,0,1,0,3.0f);
				HOScene->GetObjectByID("SquirrelHand")->SetPivot(-18,-10);
				HOScene->GetObjectByID("SquirrelHand")->Animate(0,0,5,0,5.0f);

				HOScene->GetObjectByID("Squirrel")->SetPivot(15,35);
				HOScene->GetObjectByID("Squirrel")->Animate(0,0,0.5,0,10.0f);

				if(!ProfileSystem::Instance->ActiveProfile.L1S6_HOSolved)
				{
					HOLinesVFX = new SuperFX("PRT\\L1\\HOPRTLINES2.INI", PointSystem::CreateCPoint(353,493,0), 15);
					HODotsVFX = new SuperFX("PRT\\L1\\HOPRDOTS2.INI", PointSystem::CreateCPoint(354,525,0), 15);

					if(!ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles)
					{
						HOLinesVFX->Start();
						HODotsVFX->Start();
					}
				}

				ProfileSystem::Instance->ActiveProfile.L1S6_IsRoomLite = true;
					
				isSparkleUpdatePending = true;
				Control::Audio->PlaySample(aSFXpopdown);
				Popup2_State = POPUP_SCALEDOWN;

				Popup2Switch->Show(false);
					
				SetupPopupTransition(Popup2Switch,PopRectSwitch);
			}
			
		}
			
			
			break;
		} 
	}
}

float L1_S6_Boatshed::easingQuadOut(float t,float b,float c,float d)
{
	return (c * -t*(t-2) / d + b);
}

void L1_S6_Boatshed::Render(const CPoint* pCam)
{  
	if( !Visible )
		return;

	if(ActivePopupID != ePopInactive)
	{
		if(IsPopupTransitionActive)
		{
            if( GFInstance->IsRTCreated )


			//Init again if render target lost due to fullscreen-window switches or similar
			if( GFInstance->RTGraphic != NULL )
			{
				GFInstance->RTGraphic->blitAlphaRectFx(0,0,1366,768,
				short(GFInstance->PStartX+float(GFInstance->PEndX-GFInstance->PStartX)*PopupAlpha),
				short(GFInstance->PStartY+float(GFInstance->PEndY-GFInstance->PStartY)*PopupAlpha),
				-5*(1.0f-PopupAlpha),
				PopupAlpha,PopupAlpha>1.0f?1.0f:PopupAlpha);

				GFInstance->RTGraphic->blitAlphaRectFx(0,0,1366,768,
				short(GFInstance->PStartX+float(GFInstance->PEndX-GFInstance->PStartX)*PopupAlpha),
				short(GFInstance->PStartY+float(GFInstance->PEndY-GFInstance->PStartY)*PopupAlpha),
				-5*(1.0f-PopupAlpha),
				PopupAlpha,PopupAlpha>1.0f?1.0f:PopupAlpha);
                
            }
		}	
	}
}

L1_S6_Boatshed::~L1_S6_Boatshed()
{
	TaskPendingUpdate();
	SAFE_DELETE(LightVFX);
	SAFE_DELETE(MoonLightVFX);

	SAFE_DELETE(Scn_BGDark);

	SAFE_DELETE(HOLinesVFX);
	SAFE_DELETE(HODotsVFX);

	RT_BgScn->pSprite->Image = NULL;
	SAFE_DELETE(RT_BgScn);

	SAFE_DELETE(Popup1Pumper);
	SAFE_DELETE(Popup2Switch);

	SAFE_DELETE(CAnim_Pumper);
	SAFE_DELETE(CAnim_Handle);
	SAFE_DELETE(CAnim_PumperPlaced);

	SAFE_DELETE(CAnim_WebBox);
	SAFE_DELETE(CAnim_Broom);

	SAFE_DELETE(CAnim_Box);
	SAFE_DELETE(CAnim_BoxMoved);

	//MUSIC_SFX
	Control::Audio->StopSample(aAmbS6ShedRoom);
	Control::Audio->UnloadSample(aAmbS6ShedRoom);
	//MUSIC_SFX
}

void L1_S6_Boatshed::ReceiveMessage(int val)
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
			if( itemVal == eInv_L1S2_Hose )
			{			
				if(Popup1_State== POPUP_OPEN)
				{
					if(InvDestRectPopup.Intersection(&dropPos))
					{
						if(ProfileSystem::Instance->ActiveProfile.L1S6_IsPumperPlaced)
						{
							Popup1Pumper->GetObjectByID("HoseAnim")->SetEnable(true);
							ProfileSystem::Instance->ActiveProfile.L1S6_IsHosePlaced = true;

							HOScene->GetObjectByID("Pipe")->SetEnable(true);

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6HoseFixed);

							Hud->ReleaseDrag();
							Hud->CloseInventory(); 

							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S2_Hose] = false;
							Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S2_Hose);
							isSparkleUpdatePending = true;
						}
						else
						{
							Hud->ReleaseDrag();
							Hud->CloseInventory(); 

							Hud->ShowBannerText("L1connect");
						}
					}
					else
						GFInstance->WrongInvDropMsg(Hud);
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if( itemVal == eInv_L1S2_EmptyCan )
			{
				if(Popup1_State== POPUP_OPEN && InvDestRectPopup.Intersection(&dropPos))
				{
					Popup1Pumper->GetObjectByID("P1OilCan")->SetEnable(true);

					ProfileSystem::Instance->ActiveProfile.L1S6_IsCanPlaced = true;

					Control::Audio->QuickLoadAndPlaySFX(aSFXL1S6CanPlaced);

					HOScene->GetObjectByID("Can")->SetEnable(true);

					Hud->ReleaseDrag();
					Hud->CloseInventory(); 

					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S2_EmptyCan] = false;
					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S2_EmptyCan);
					isSparkleUpdatePending = true;
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else
				GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		
		
		
	}	
}

void L1_S6_Boatshed::SparkleUpdate()
{
	if(isSparkleUpdatePending)
	{
		isSparkleUpdatePending = false;
		if(SparkleFX==NULL)
		{
			SparkleFX = new SuperFX("PRT\\GLINT.INI",PointSystem::CreateCPoint(0,0,0),120);		
		}
		//Game Logic Goes here
		numSparkles = 0;
		currentSparkleID = 0;

		if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsRoomLite)
		{
			sparklePoints[numSparkles].SetXY(PopRectSwitch.GetCenterX(), PopRectSwitch.GetCenterY());
			numSparkles++;
		}
		else 
		{
			if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsWebCleaned)
			{
				sparklePoints[numSparkles].SetXY(ActionRectBroom.GetCenterX(), ActionRectBroom.GetCenterY());
				numSparkles++;
			}
			else if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCrowbarPicked)
			{
				sparklePoints[numSparkles].SetXY(ActionRectBox.GetCenterX(), ActionRectBox.GetCenterY());
				numSparkles++;
			}

			if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsPumperPlaced)
			{
				sparklePoints[numSparkles].SetXY(PopRectPumper.GetCenterX(), PopRectPumper.GetCenterY());
				numSparkles++;
			}
			else if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsHosePlaced && ProfileSystem::Instance->ActiveProfile.L1S2_InvHoseTaken)
			{
				sparklePoints[numSparkles].SetXY(PopRectPumper.GetCenterX(), PopRectPumper.GetCenterY());
				numSparkles++;
			}
			else if(!ProfileSystem::Instance->ActiveProfile.L1S6_IsCanPlaced && ProfileSystem::Instance->ActiveProfile.L1S2_InvDieselCanTaken)
			{
				sparklePoints[numSparkles].SetXY(PopRectPumper.GetCenterX(), PopRectPumper.GetCenterY());
				numSparkles++;
			}
			else if(ProfileSystem::Instance->ActiveProfile.L1S6_IsHosePlaced && ProfileSystem::Instance->ActiveProfile.L1S6_IsCanPlaced && ProfileSystem::Instance->ActiveProfile.L1S6_IsPumperPlaced && !ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCanFullPicked)
			{
				sparklePoints[numSparkles].SetXY(PopRectPumper.GetCenterX(), PopRectPumper.GetCenterY());
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


void L1_S6_Boatshed::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S6ChappathiRoll])
		{
			if(Popup1_State == POPUP_OPEN)
				InitHintTrailEmit(450, 450);
			else
				InitHintTrailEmit(500, 150);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}
	if(IsBoxMovingAnimPlaying || IsBroomAnimPlaying || IsbroomTaken)
		return;

	int iNonHOHint = 0; 
	int MaxNonHOHints = 19;
	do
	{
		switch( iNonHOHint )
		{
			default:			
			//Exit coordinates
			case 0: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S6_IsRoomLite &&  Popup2_State == POPUP_OPEN)
					{
						InitHintTrailEmit(CenterPoint.x, CenterPoint.y,false);
						return;
					}
					break;
					
			case 1: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S6_IsPumperPlaced && ProfileSystem::Instance->ActiveProfile.L1S6_IsRoomLite && Popup1_State == POPUP_OPEN)
					{
						InitHintTrailEmit(	ActionRectPumper.GetCenterX(), ActionRectPumper.GetCenterY(),false);
						return;
					}
					break;	
			
			case 2: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S6_IsHosePlaced && ProfileSystem::Instance->ActiveProfile.L1S6_IsPumperPlaced && ProfileSystem::Instance->ActiveProfile.L1S2_InvHoseTaken && Popup1_State == POPUP_OPEN)
					{
						Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S2_Hose)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}

							CPoint tempPoint (InvDestRectHose.GetCenterX(), InvDestRectHose.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						return;
					}
					break;
			case 3: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S6_IsCanPlaced && ProfileSystem::Instance->ActiveProfile.L1S6_IsPumperPlaced && ProfileSystem::Instance->ActiveProfile.L1S2_InvDieselCanTaken && Popup1_State == POPUP_OPEN)
					{
						Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S2_EmptyCan)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}

							CPoint tempPoint (InvDestRectCan.GetCenterX(), InvDestRectCan.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						return;
					}
					break;
					
			case 4: ++iNonHOHint;
					if( ! ProfileSystem::Instance->ActiveProfile.L1S6_IsCanFilled && ProfileSystem::Instance->ActiveProfile.L1S6_IsCanPlaced && ProfileSystem::Instance->ActiveProfile.L1S6_IsPumperPlaced && ProfileSystem::Instance->ActiveProfile.L1S6_IsCanPlaced && Popup1_State == POPUP_OPEN)
					{
						InitHintTrailEmit(	ActionRectHandle.GetCenterX(), ActionRectHandle.GetCenterY(),false);
						return;
					}
					break;	
		   case 5: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCanFullPicked && ProfileSystem::Instance->ActiveProfile.L1S6_IsCanFilled && Popup1_State == POPUP_OPEN)
					{
						InitHintTrailEmit(	InvDestRectCan.GetCenterX(), InvDestRectCan.GetCenterY(),false);
						return;
					}
					break;
			case 6: ++iNonHOHint;
					 if( Popup1_State == POPUP_OPEN)
					{
						InitHintTrailEmit(	PopCloseButtonRect.GetCenterX(), PopCloseButtonRect.GetCenterY(),true);
						return;
					}
					break;	
					
			case 7: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S6_IsPumperPlaced && ProfileSystem::Instance->ActiveProfile.L1S6_IsRoomLite )
					{
						InitHintTrailEmit(	PopRectPumper.GetCenterX(), PopRectPumper.GetCenterY(),false);
						return;
					}
					break;	
			
			case 8: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S6_IsHosePlaced && ProfileSystem::Instance->ActiveProfile.L1S6_IsPumperPlaced && ProfileSystem::Instance->ActiveProfile.L1S2_InvHoseTaken )
					{
						InitHintTrailEmit(	PopRectPumper.GetCenterX(), PopRectPumper.GetCenterY(),false);
						return;
					}
					break;
			case 9: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S6_IsCanPlaced && ProfileSystem::Instance->ActiveProfile.L1S6_IsPumperPlaced && ProfileSystem::Instance->ActiveProfile.L1S2_InvDieselCanTaken )
					{
						InitHintTrailEmit(	PopRectPumper.GetCenterX(), PopRectPumper.GetCenterY(),false);
						return;
					}
					break;
					
			case 10: ++iNonHOHint;
					if( ! ProfileSystem::Instance->ActiveProfile.L1S6_IsCanFilled && ProfileSystem::Instance->ActiveProfile.L1S6_IsCanPlaced && ProfileSystem::Instance->ActiveProfile.L1S6_IsPumperPlaced && ProfileSystem::Instance->ActiveProfile.L1S6_IsCanPlaced )
					{
						InitHintTrailEmit(	PopRectPumper.GetCenterX(), PopRectPumper.GetCenterY(),false);
						return;
					}
					break;	
		   case 11: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCanFullPicked && ProfileSystem::Instance->ActiveProfile.L1S6_IsCanFilled )
					{
						InitHintTrailEmit(	PopRectPumper.GetCenterX(), PopRectPumper.GetCenterY(),false);
						return;
					}
					break;
			case 12: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S6_IsRoomLite)
					{
						InitHintTrailEmit(PopRectSwitch.GetCenterX(), PopRectSwitch.GetCenterY(),false);
						return;
					}
					break;
			case 13: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S6_IsWebCleaned && ProfileSystem::Instance->ActiveProfile.L1S6_IsRoomLite )
					{
						InitHintTrailEmit(	ActionRectBroom.GetCenterX(), ActionRectBroom.GetCenterY(),false);
						return;
					}
					break;	
			case 14: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S6_IsBoxDroped && ProfileSystem::Instance->ActiveProfile.L1S6_IsWebCleaned && ProfileSystem::Instance->ActiveProfile.L1S6_IsRoomLite )
					{
						InitHintTrailEmit(	ActionRectBoxWeb.GetCenterX(), ActionRectBoxWeb.GetCenterY(),false);
						return;
					}
					break;	
			case 15: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCrowbarPicked && ProfileSystem::Instance->ActiveProfile.L1S6_IsBoxDroped && ProfileSystem::Instance->ActiveProfile.L1S6_IsRoomLite )
					{
						InitHintTrailEmit(	ActionRectBoxWeb.GetCenterX(), ActionRectBoxWeb.GetCenterY(),false);
						return;
					}
					break;	
			case 16: ++iNonHOHint;
					if(  !ProfileSystem::Instance->ActiveProfile.L1S6_HOSolved && ProfileSystem::Instance->ActiveProfile.L1S6_IsRoomLite )
					{
						InitHintTrailEmit(	SceneRectToolBoxHO.GetCenterX(), SceneRectToolBoxHO.GetCenterY(),false);
						return;
					}
					break;	
			case 17: ++iNonHOHint;
					if( ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S7_BoatDock])
					{
						InitHintTrailEmit(HoveredBackBtnRectMax.topleft.x + HoveredBackBtnRectMax.w/2 ,HoveredBackBtnRectMax.topleft.y + HoveredBackBtnRectMax.h/2,true,CA_ExitDown);
						return;
					}
					break;
			case 18: ++iNonHOHint;
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

void L1_S6_Boatshed::NavigateToScene()
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