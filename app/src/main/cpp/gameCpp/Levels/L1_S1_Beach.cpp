//====================================
//	DQFYLH
//	HO System
//	Level 1 - Scene 01 : Beach
//====================================

#include "L1_S1_Beach.h"
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
const CRectangle	PopCloseButtonRect(968.0f,71.0f,62.0f,61.0f);
const CRectangle	PopAreaRect(345.0f,90.0f,665.0f,501.0f);

//Scene Rects
const CRectangle  SceneRectFrontyard(426.0f,129.0f,280.0f,365.0f);

//Popup Rects
const CRectangle  PopRect1JamesExtra(768.0f,557.0f,429.0f,118.0f); //PopRect1James is Body of James
const CRectangle  PopRect1Roses(800.0f,402.0f,188.0f,195.0f);
const CRectangle  PopRect2CreekBoard(867.0f,121.0f,139.0f,103.0f);

//Inventory Destination Rects
const CRectangle  InvItemRectBag(630.0f,531.0f,104.0f,95.0f);


const CRectangle  RubyRect4UnderLeaves(724.0f,411.0f,82.0f,66.0f);
const CRectangle  RubyRect3UnderLeafOne(715.0f,374.0f,92.0f,71.0f);

const CRectangle  LeavesRectGround(716.0f,353.0f,216.0f,172.0f);

const CRectangle  RubyRect2Branch3(503.0f,324.0f,76.0f,82.0f);
const CRectangle  RubyRect1Branch2(594.0f,361.0f,82.0f,85.0f);

const CRectangle  TalkRectRaja(468.0f,359.0f,164.0f,239.0f);
const CRectangle  ActionRectJames(642.0f,494.0f,330.0f,150.0f);

const CRectangle  InvDestRectThornBranch(396.0f,120.0f,289.0f,333.0f);

const CRectangle  InvDestRectKnifeRectBranch1(377.0f,119.0f,277.0f,321.0f);
const CRectangle  InvDestRectKnifeRectBranch2(377.0f,119.0f,277.0f,321.0f);
const CRectangle  InvDestRectKnifeRectBranch3(386.0f,115.0f,406.0f,299.0f);

const CRectangle  MemRectTeddy(757.0f,501.0f,59.0f,81.0f);

const CRectangle  InforectCreekBoard(549.0f,194.0f,271.0f,171.0f);
const CRectangle  InvItemRectPlank(530.0f,368.0f,231.0f,89.0f);
const CRectangle  InvItemRectPlank2(681.0f,349.0f,80.0f,30.0f);

const CRectangle  InfoRectStatue1(304.0f,95.0f,109.0f,203.0f);
const CRectangle  InfoRectStatue2(752.0f,121.0f,106.0f,224.0f);
const CRectangle  InfoRectStatueSnake(207.0f,301.0f,152.0f,130.0f);


//Inventory Item Rects
//Info Rects

float lightAlphaArray4[20] = {0,0,0,0,0,0.15f,0.65f,0.75f,0.65f,0.25f,0.15f,0.75f,0.65f,0.05f,0.25f,0.55f,0.15f,0,0,0};

L1_S1_Beach::L1_S1_Beach()
{
	//Backup Save for level restart
	//if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S2_Frontyard])
		//ProfileSystem::Instance->SaveProfileBackup(ProfileSystem::Instance->ProfileSlot);

	Visible = false;
	
	RT_BgScn = NULL;

	bgScaleOffsetX = 0.0f;
	bgScaleOffsetY = 0.0f;

	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV1\\S1\\S1.SCN");

	GFInstance->PostUpdate(1);

#ifdef  K_ANDROID
	//GFApp->ShowInterstitialAD();
#endif

	InitPopups();

	//Localized
	CObject* temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L1\\S1\\CREEKMANSION.SPR", 935, 165, 10);
	temp->SetUID("creekBoard");
	HOScene->PushObject(temp);

	RajaJamesSCN = NULL;
	CAnim_RajaTakeJames = NULL;
	isRajaTakeJamesPlaying = false;
	if(!ProfileSystem::Instance->ActiveProfile.L1S1_IsRajaTakenJames)
	{
		RajaJamesSCN = new CHOScene();
		RajaJamesSCN->Load("LV1\\S1\\RAJATAKEJAMES.SCN");
		RajaJamesSCN->SetZOff(10);
		RajaJamesSCN->Show(false);

		RajaJamesSCN->GetObjectByID("james")->Show(true);
		RajaJamesSCN->GetObjectByID("rajasit")->Show(true);		

		CObject* objArr[] = {RajaJamesSCN->GetObjectByID("james"), RajaJamesSCN->GetObjectByID("rajasit"), RajaJamesSCN->GetObjectByID("rajatake1"), RajaJamesSCN->GetObjectByID("rajatake2"), RajaJamesSCN->GetObjectByID("rajatake3"), RajaJamesSCN->GetObjectByID("rajatake4")};
		std::string animArr[] = {"ANIM\\L1\\S1JAMES.ANIM", "ANIM\\L1\\S1RAJSIT.ANIM", "ANIM\\L1\\S1RAJ1.ANIM", "ANIM\\L1\\S1RAJ2.ANIM", "ANIM\\L1\\S1RAJ3.ANIM", "ANIM\\L1\\S1RAJ4.ANIM"};
		CAnim_RajaTakeJames = new CustomAnimGroup(6, animArr, objArr);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S1_IsInvBagTaken)
		HOScene->GetObjectByID("Bag")->SetEnable(false);

	if( ProfileSystem::Instance->ActiveProfile.L1S1_IsInvPlank3Taken )
		HOScene->GetObjectByID("Plank")->SetEnable(false);

	//Scene Animations
	HOScene->GetObjectByID("moonglow")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("moonglow")->Animate(0.15f, 0.65f, 6.0f);
	
	HOScene->GetObjectByID("moonrayleft")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("moonrayleft")->Animate(0.15f, 0.75f, 5.0f);

	HOScene->GetObjectByID("moonrayright")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("moonrayright")->Animate(0.15f, 0.75f, 6.0f);

	HOScene->GetObjectByID("RtreeLbranch")->SetPivot(-80, -50);
	HOScene->GetObjectByID("RtreeLbranch")->Animate(0,0,4,0,9.0f);	

	HOScene->GetObjectByID("RtreeMbranch")->SetPivot(-110, -64);
	HOScene->GetObjectByID("RtreeMbranch")->Animate(0,0,3,0,7.0f);

	HOScene->GetObjectByID("RtreeRbranch")->SetPivot(-30, -8);
	HOScene->GetObjectByID("RtreeRbranch")->Animate(0,0,4,0,12.0f);

	HOScene->GetObjectByID("LtreeLbranch")->SetPivot(48, 10);
	HOScene->GetObjectByID("LtreeLbranch")->Animate(0,0,4,0,10.0f);

	HOScene->GetObjectByID("LtreeRbranch")->Animate(4,10,0,0,12.0f);

	Scn_lightning = HOScene->GetObjectByID("lightninghalf");
	Scn_lightning->SetBlendMode(eBlend_AlphaAdditive);
	Scn_lightning->SetScale(2.0f);
	Scn_lightning->Show(false);

	lightFrame = 0;
	lightningTimer = 0;
	nextLightningThreshold = 8.0f;

	p2lightTimer = 0;
	p2lightFrame = 0;

	Wave2 = NULL;
	Wave1 = NULL;

	Wave1 = HOScene->GetObjectByID("wave1");
	Wave2 = HOScene->GetObjectByID("wave2");

	HOScene->GetObjectByID("shadow")->Animate(0.15f,0.75f,3.5f,1.85f);

	waveTimer = 0;

	//Init Pos for Wave
	Wave1->Pos.x = Wave1->InitPos.x + Elixir::Cos(waveTimer)*40.0f - Elixir::Sin(waveTimer)*20.0f; 
	Wave1->Pos.y = Wave1->InitPos.y + Elixir::Cos(waveTimer)*30.0f;

	Wave1->SetAlpha( (-Elixir::Cos(waveTimer)+1.0f)*0.5f +0.25f);

	Wave2->Pos.x = Wave2->InitPos.x + Elixir::Cos(waveTimer+160)*50.0f + Elixir::Sin(waveTimer+160)*20.0f; 
	Wave2->Pos.y = Wave2->InitPos.y + Elixir::Cos(waveTimer+160)*35.0f;

	Wave2->SetAlpha( (-Elixir::Cos(waveTimer+180)+1.0f)*0.5f +0.25f);


	FgPlantMesh = NULL;

	FgPlantMesh = new MeshObject("LV1\\S1\\ROSEPLANT.JPA", 5, 5);
	//FgPlantMesh->LoadImg("LV1\\S1\\ROSEPLANT.JPA");
	FgPlantMesh->SetPosAndParams(802, 396, 3, 2, 5, 2);
	FgPlantMesh->SetZ(15);
	
	HOScene->InsertObject(FgPlantMesh,15);

	//Scene Elements
	//Scn_lightning	= HOScene->GetObjectByID("LightN");	

	if( GFInstance->availablePhyMem >= MAX_LOWEND_PEAKMEMORY )
	{
	}

	RainBgVFX = new SuperFX("PRT\\BG\\rainfuse.INI", PointSystem::CreateCPoint(580.0f,-50.0f,0), 100);
	RainBgVFX->Start();
	RainBgVFX->AdvanceByTime(2.0f);

	VFXArray.push_back(RainBgVFX);

	RainDropsVFX = new SuperFX("PRT\\L1\\dropsstairsbeach.INI", PointSystem::CreateCPoint(560.0f,408.0f,0), 16);
	RainDropsVFX->Start();
	RainDropsVFX->AdvanceByTime(2.0f);

	VFXArray.push_back(RainDropsVFX);

	RainSandDropsVFX = new SuperFX("PRT\\L1\\rainsanddrops.INI", PointSystem::CreateCPoint(790.0f,625.0f,0), 16);
	RainSandDropsVFX->Start();
	RainSandDropsVFX->AdvanceByTime(2.0f);

	VFXArray.push_back(RainSandDropsVFX);

	MoonFliesVFX = new SuperFX("PRT\\L1\\moonfxbeach.INI", PointSystem::CreateCPoint(596.0f,210.0f,0), 100);
	MoonFliesVFX->Start();
	MoonFliesVFX->AdvanceByTime(2.0f);

	VFXArray.push_back(MoonFliesVFX);

	GFInstance->PostUpdate(2);

	zOrderScnVideo = 1;
	zOrderPopVideo = eZOrder_Popup + 10;

	IsZOrderChangeToScene = false;

	//Video Init : Add Visible = true; to see anything on screen !!
	SetZ(1);
	
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
		else  if( ProfileSystem::Instance->ActiveProfile.L1_IsMansionPowerOn )
		{
			if( !Control::Audio->IsPlaying(aTrackL1Mansion) )
				Control::Audio->PlaySample(aTrackL1Mansion,false);
		}
		else
		{
			if( !Control::Audio->IsPlaying(aTrackL1Mansion) &&
				!Control::Audio->IsPlaying(aTrackL1Broken) )
				Control::Audio->PlaySample(aTrackL1Broken,false);
		}
	}

	Control::Audio->LoadSample(aAmbS1Beach,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbS1Beach) )		
	{
		Control::Audio->PlaySample(aAmbS1Beach,true);
	}	
	//MUSIC_SFX

	

	memObj_Teddy = NULL;
	//memory object activated only after going to frontyard
	memObj_Teddy = HOScene->GetObjectByID("Teddy");
	if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S2_Frontyard])
		memObj_Teddy->SetEnable(false);

	else if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S1Teddy])
	{
		memObj_Teddy->SetEnable(false);
	}

	

	//if Tutorial is disabled from custom Mode. Disable all Tutorials here since it is connected
	if(ProfileSystem::Instance->ActiveProfile.IsTutorialDisabled)
	{
		ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT00_Prompt] = true;
		ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT01_TalkWithRaja] = true;
		ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT02_TakeJames] = true;
		ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT03_Journal] = true;
	}

	//Tutorial show
	if(Hud->Tutor != NULL && Hud->Tutor->IsActive)
	{
		if(!ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT00_Prompt])
			Hud->Tutor->ShowTutorial(TUT00_Prompt);
		else
		{
			if(ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly)
			{
				if( ProfileSystem::Instance->ActiveProfile.L1S1_IsInvBagTaken && !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT06_BagZoomLook])
					Hud->Tutor->ShowTutorial(TUT06_BagZoomLook);				
				else
				{					
					#ifdef _CEBUILD						
						if( ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT10_UseKnifeOnRose] && ProfileSystem::Instance->ActiveProfile.L1S1_IsRajaTakenJames && 
							!ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT11_MorphObject])
						{
							if( ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S1Collected )//already picked morph objects, then no need to teach it!!
							{
								ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT11_MorphObject] = true;
								if( !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT12_NavigationUp] && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly )
									Hud->Tutor->ShowTutorial(TUT12_NavigationUp);	
								else
									Hud->Tutor->HideTutorial();	
							}
							else
							{
								Hud->ShowMorphObjects();		
								Hud->Tutor->ShowTutorial(TUT11_MorphObject);
								Hud->isMorphTutorialActive = true;
								Hud->isMorphObjectsActive = true;
								Hud->morphTimerThreshold = 1.5f; //quick loading if not fading in or out 
							}
						}
					#else
						if( !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT12_NavigationUp] )
							Hud->Tutor->ShowTutorial(TUT12_NavigationUp);					
						//Nothing
					#endif 
				}
			}
			else
			{
				//Full Tutorial
				if(!ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT01_TalkWithRaja])
					Hud->Tutor->ShowTutorial(TUT01_TalkWithRaja);
				else if(!ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT02_TakeJames])
					Hud->Tutor->ShowTutorial(TUT02_TakeJames);
				else if(!ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT03_Journal])
					Hud->Tutor->ShowTutorial(TUT03_Journal);
				else if(!ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT05_BagInvTake])
					Hud->Tutor->ShowTutorial(TUT05_BagInvTake);
				else if( ProfileSystem::Instance->ActiveProfile.L1S1_IsInvBagTaken && !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT06_BagZoomLook])
					Hud->Tutor->ShowTutorial(TUT06_BagZoomLook); //special tutorial for both!!
				else
				{					
					#ifdef _CEBUILD						
						if( ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT10_UseKnifeOnRose] && ProfileSystem::Instance->ActiveProfile.L1S1_IsRajaTakenJames && 
							!ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT11_MorphObject]) //special tutorial for both!!
						{
							if( ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S1Collected )//already picked morph objects, then no need to teach it!!
							{
								ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT11_MorphObject] = true;
								if( !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT12_NavigationUp] && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly )
									Hud->Tutor->ShowTutorial(TUT12_NavigationUp);	
								else
									Hud->Tutor->HideTutorial();	
							}
							else
							{
								Hud->ShowMorphObjects();		
								Hud->Tutor->ShowTutorial(TUT11_MorphObject);
								Hud->isMorphTutorialActive = true;
								Hud->isMorphObjectsActive = true;
								Hud->morphTimerThreshold = 1.5f; //quick loading if not fading in or out 
							}
						}
					#else
						if( !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT12_NavigationUp] )
							Hud->Tutor->ShowTutorial(TUT12_NavigationUp);					
						//Nothing
					#endif 
				}
			}
		}
	}

	dialogSystem = NULL;
	//if condition to add
	dialogSystem = new DialogSystem();
	AddControl(dialogSystem);		
	
	isDialogActive = false;

#ifdef _CEBUILD
	//Morph Object Add
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S1Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("morphtarget"), HOScene->GetObjectByID("morphbase"));
		CRectangle  MorphRect(904.0f,66.0f,72.0f,53.0f);
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

	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L1_S1_Beach]  = true;

	if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S1_Beach])
		ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S1_Beach] = true;

	Visible = true;	//To render anything with renderlistentity
}

void L1_S1_Beach::SparkleUpdate()
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
		if(!ProfileSystem::Instance->ActiveProfile.L1S1_IsRajaTakenJames)
		{
			if(!ProfileSystem::Instance->ActiveProfile.L1S1_IsTalkedWithRaja)
			{
				sparklePoints[numSparkles].SetXY(TalkRectRaja.GetCenterX(), TalkRectRaja.GetCenterY());
				numSparkles++;
			}
			else
			{
				sparklePoints[numSparkles].SetXY(ActionRectJames.GetCenterX(), ActionRectJames.GetCenterY());
				numSparkles++;
			}
		}
		else
		{
			if(!ProfileSystem::Instance->ActiveProfile.L1S1_IsInvBagTaken)
			{
				sparklePoints[numSparkles].SetXY(InvItemRectBag.GetCenterX(), InvItemRectBag.GetCenterY());
				numSparkles++;
			}

			if( !IsAllRubyPiecesTaken )
			{
				sparklePoints[numSparkles].SetXY(PopRect1Roses.GetCenterX(), PopRect1Roses.GetCenterY());
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

void L1_S1_Beach::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L1S1_IsRajaTakenJames)
		isAnyTaskLeft = true;
	else
	{
		//Raja taken james
		if(!ProfileSystem::Instance->ActiveProfile.L1S1_IsInvBagTaken)
			isAnyTaskLeft = true;
		if(!IsAllRubyPiecesTaken)
		{
			if(!ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby3UnderLeaf1Taken || !ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby4UnderLeavesTaken)
				isAnyTaskLeft = true;
			if(ProfileSystem::Instance->ActiveProfile.L1S1_IsInvKnifeTaken)
			{
				//if player has knife in his hands. either he can cut them or he can take them
				if(!ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby1Branch2Taken || !ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby2Branch3Taken)
					isAnyTaskLeft = true;
			}
			else
				isAnyTaskLeft = true;
		}

		if(!ProfileSystem::Instance->ActiveProfile.L1S1_IsInvPlank3Taken)
			isAnyTaskLeft = true;
	}
	
	if(ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby1Branch2Taken && ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby2Branch3Taken &&ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby3UnderLeaf1Taken && ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby4UnderLeavesTaken &&  ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L1S3_IsBedroomDoorOpened)
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S3_Foyer] = true;

	if(ProfileSystem::Instance->ActiveProfile.L1S1_IsInvPlank3Taken && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvPlank1Taken && ProfileSystem::Instance->ActiveProfile.L1S7_IsInvPlank2Taken && !ProfileSystem::Instance->ActiveProfile.L1S7_IsStepsPlaced)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S7_BoatDock] = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S1_Beach] = isAnyTaskLeft;
}

void L1_S1_Beach::InitPopups()
{	
	ActivePopupID = ePopInactive;

	popupCloseDelayTimer = 0.0f;	
	Pop_CloseButton = NULL;

	IsPopupTransitionActive = false;	


	IsAllRubyPiecesTaken = false;
	if(ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby1Branch2Taken && ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby2Branch3Taken &&
		ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby3UnderLeaf1Taken && ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby4UnderLeavesTaken)
		IsAllRubyPiecesTaken = true;

	Pop1Roses = NULL;
	P1RainVFX = NULL;
	P1RaindropsVFX = NULL;

	if ( !IsAllRubyPiecesTaken )
	{
		Pop1Roses = new CHOScene();
		Pop1Roses->Load("LV1\\S1\\POP\\P1.SCN");

		//Add popup frame and close button
		CObject *popFrame = new CObject();
		popFrame->LoadInit("UI\\poprectframe.SPR", 13, -15, eZOrder_PopFrame);
		popFrame->SetUID("popFrame");
		//Pop1Roses->PushObject(popFrame); //in Normal Cases

		//Place it above popup frame
		Pop1Roses->InsertObject(popFrame,18);
		Pop1Roses->GetObjectByID("p1rosetop")->SetZ(eZOrder_PopFrame+2);

		CObject *popCloseBtn = new CObject();
		popCloseBtn->LoadInit("UI\\closebtn.SPR", 321, -246, eZOrder_PopCloseBtn);
		popCloseBtn->SetUID("popClose");
		Pop1Roses->PushObject(popCloseBtn);

		
		P1RainVFX = new SuperFX("PRT\\L1\\popRain.INI", PointSystem::CreateCPoint(722,225,0), eZOrder_Popup+60);
		P1RaindropsVFX = new SuperFX("PRT\\L1\\pops1drops.INI", PointSystem::CreateCPoint(683,430,0), eZOrder_Popup+50); 

		//Objects and stuffs
		IsKnifeCut1Playing = IsKnifeCut2Playing = IsKnifeCut3Playing = false;

		P1_SwissKnife = Pop1Roses->GetObjectByID("p1swissknife");

		P1_Branch1 = Pop1Roses->GetObjectByID("p1cutbranch1");
		P1_Branch2 = Pop1Roses->GetObjectByID("p1cutbranch2");
		P1_Branch3 = Pop1Roses->GetObjectByID("p1cutbranch3");

		P1_Ruby1branch2 = Pop1Roses->GetObjectByID("p1ruby1branch2");
		P1_Ruby2Branch3 = Pop1Roses->GetObjectByID("p1ruby2branch3");
		P1_Ruby3UnderLeaf1 = Pop1Roses->GetObjectByID("p1ruby3underleaf1");
		P1_Ruby4UnderLeaves = Pop1Roses->GetObjectByID("p1ruby4underleaves");

		P1_Leaf1 = Pop1Roses->GetObjectByID("p1leaf1");
		P1_Leaf2 = Pop1Roses->GetObjectByID("p1leaf2");
		P1_Leaf3 = Pop1Roses->GetObjectByID("p1leaf3");

		Pop1Roses->GetObjectByID("p1rosedown")->SetPivot(-15,27);
		Pop1Roses->GetObjectByID("p1rosedown")->Animate(0,0,4,0,6.0f);
		Pop1Roses->GetObjectByID("p1rosetop")->SetPivot(16,35);
		Pop1Roses->GetObjectByID("p1rosetop")->Animate(0,0,3,0,8.0f,1.45f);
		Pop1Roses->GetObjectByID("p1rosetopleft")->SetPivot(30,10);
		Pop1Roses->GetObjectByID("p1rosetopleft")->Animate(0,0,4,0,7.0f,0.45f);

		CAnim_KnifeCut1 = NULL;	CAnim_KnifeCut2 = NULL;	CAnim_KnifeCut3 = NULL;

		if(!ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch1Cut)
			CAnim_KnifeCut1 = new CustomAnimator("ANIM\\L1\\s1knife1cut.anim",P1_SwissKnife);
		if(!ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch2Cut)
			CAnim_KnifeCut2 = new CustomAnimator("ANIM\\L1\\s1knife2cut.anim",P1_SwissKnife);
		if(!ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch3Cut)
			CAnim_KnifeCut3 = new CustomAnimator("ANIM\\L1\\s1knife3cut.anim",P1_SwissKnife);

		CAnim_BranchCut1 = NULL; CAnim_BranchCut2 = NULL; CAnim_BranchCut3 = NULL;

		if(!ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch1Cut)
			CAnim_BranchCut1 = new CustomAnimator("ANIM\\L1\\s1branch1cut.anim",P1_Branch1);
		if(!ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch2Cut)
			CAnim_BranchCut2 = new CustomAnimator("ANIM\\L1\\s1branch2cut.anim",P1_Branch2);
		if(!ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch3Cut)
			CAnim_BranchCut3 = new CustomAnimator("ANIM\\L1\\s1branch3cut.anim",P1_Branch3);

		custAnimArray.push_back(CAnim_KnifeCut1);
		custAnimArray.push_back(CAnim_KnifeCut2);
		custAnimArray.push_back(CAnim_KnifeCut3);

		custAnimArray.push_back(CAnim_BranchCut1);
		custAnimArray.push_back(CAnim_BranchCut2);
		custAnimArray.push_back(CAnim_BranchCut3);

		P1_SwissKnife->SetEnable(false);

		if(ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch1Cut)
			P1_Branch1->SetEnable(false);
		if(ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch2Cut)
			P1_Branch2->SetEnable(false);
		if(ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch3Cut)
			P1_Branch3->SetEnable(false);

		if(ProfileSystem::Instance->ActiveProfile.L1S1_P1Leaf1Removed)
			P1_Leaf1->SetEnable(false);
		if(ProfileSystem::Instance->ActiveProfile.L1S1_P1Leaf2Removed)
			P1_Leaf2->SetEnable(false);
		if(ProfileSystem::Instance->ActiveProfile.L1S1_P1Leaf3Removed)
			P1_Leaf3->SetEnable(false);

		if(ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby1Branch2Taken)
			P1_Ruby1branch2->SetEnable(false);
		if(ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby2Branch3Taken)
			P1_Ruby2Branch3->SetEnable(false);
		if(ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby3UnderLeaf1Taken)
			P1_Ruby3UnderLeaf1->SetEnable(false);
		if(ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby4UnderLeavesTaken)
			P1_Ruby4UnderLeaves->SetEnable(false);

		Pop1Roses->Show(false);
		Pop1Roses->SetPosOff(CenterPoint.x,CenterPoint.y);
		Pop1Roses->SetZOff(eZOrder_Popup);
	}
	Popup1_State = POPUP_CLOSED;

	Pop2CreekBoard = NULL;
	//if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsTunnelDoorExploded)
	{
		Pop2CreekBoard = new CHOScene();
		Pop2CreekBoard->Load("LV1\\S1\\POP\\P2.SCN");

		//Add popup frame and close button
		CObject *popCirFrame = new CObject();
		popCirFrame->LoadInit("UI\\popcircframe.SPR", 4, -9, 1);
		popCirFrame->SetUID("popFrame");
		Pop2CreekBoard->InsertObject(popCirFrame, 1);

		CObject *popCirCloseBtn = new CObject();
		popCirCloseBtn->LoadInit("UI\\closebtn.SPR", 145, -158, 7);
		popCirCloseBtn->SetUID("popClose");
		Pop2CreekBoard->PushObject(popCirCloseBtn);


		CObject* creekPop = new CObject();
		creekPop->LoadInit("LOCALIZED\\"+ StringTable::GetMyLangauge() +"\\L1\\S1\\CREEKMANSION_POP.SPR", 0, 0, 10);
		creekPop->SetUID("creekPop");
		Pop2CreekBoard->PushObject(creekPop);

		P2_Lightning = Pop2CreekBoard->GetObjectByID("p2lightn");
		P2_Lightning->SetBlendMode(eBlend_AlphaAdditive);
		P2_Lightning->SetEnable(false);

		if(  ProfileSystem::Instance->ActiveProfile.L1S1_IsInvPlank3Taken )
			Pop2CreekBoard->GetObjectByID("p2plank")->SetEnable(false);
	
		Pop2CreekBoard->Show(false);
		Pop2CreekBoard->SetPosOff(CenterPoint.x, CenterPoint.y);
		Pop2CreekBoard->SetZOff(eZOrder_PopupOverPopup);


		/*
		SceneData creekBoardPopData;
		creekBoardPopData.objType = "Object";
		creekBoardPopData.uid = "creekPop";
		creekBoardPopData.spriteString = "LOCALIZED\\EN\\L1\\S1\\CREEKMANSION_POP.SPR";
		creekBoardPopData.spriteAnim = 2;
		creekBoardPopData.x = 0;
		creekBoardPopData.y = 0;
		creekBoardPopData.ZOrder = 100;

		creekBoardPop = NULL;
		creekBoardPop = new CObject();
		creekBoardPop = Pop2CreekBoard->CreateObject(creekBoardPopData);
		creekBoardPop->Show(true);
		*/
		
	}
	Popup2_State = POPUP_CLOSED;

	PopupAlpha = 0.0f;
	PopVal = 0.0f;

	GFInstance->PostUpdate(1);
}

void L1_S1_Beach::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( ActivePopupID == ePopInactive )
	{
		waveTimer += 60*Control::LogicRate;

		Wave1->Pos.x = Wave1->InitPos.x + Elixir::Cos(waveTimer)*40.0f - Elixir::Sin(waveTimer)*20.0f; 
		Wave1->Pos.y = Wave1->InitPos.y + Elixir::Cos(waveTimer)*30.0f;

		Wave1->SetAlpha( (-Elixir::Cos(waveTimer)+1.0f)*0.5f +0.25f);

		Wave2->Pos.x = Wave2->InitPos.x + Elixir::Cos(waveTimer+160)*50.0f + Elixir::Sin(waveTimer+160)*20.0f; 
		Wave2->Pos.y = Wave2->InitPos.y + Elixir::Cos(waveTimer+160)*35.0f;

		Wave2->SetAlpha( (-Elixir::Cos(waveTimer+180)+1.0f)*0.5f +0.25f);

		if(waveTimer>360)
		{
			waveTimer -= 360;
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
			Scn_lightning->SetAlpha(lightAlphaArray4[index]);
		}
	}
	else
	{
		if(Scn_lightning->Visible)
			Scn_lightning->Show(false);
	}

	if( IsPopupTransitionActive )
		UpdatePopupTransition();
}

void L1_S1_Beach::Update()
{
	NavigateToScene();
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

	if(GFInstance->IsSceneGraphicReloaded)
	{
		UpdateBgGraphic();
		GFInstance->IsSceneGraphicReloaded = false;

		//since hide morph objects in updatebgGraphic
		#ifdef _CEBUILD
			Hud->ShowMorphObjects();
		#endif 
	}
	
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	CPoint pos = Control::Input->Mpos();

	if(Hud->Tutor != NULL && Hud->Tutor->IsActive)
	{
		if(Hud->Tutor->isPromptWindowShowing && Hud->Tutor->IsVisible)
		{
			if(!Hud->Tutor->isMouseOverTutorUI)
				Cursor::SetMode(CA_Normal);
			return;
		}
		else if(Hud->Tutor->IsVisible && (Hud->Tutor->State == TUT01_TalkWithRaja) )
		{
			if(!Hud->Tutor->isMouseOverTutorUI)
				Cursor::SetMode(CA_Normal);
			if(TalkRectRaja.Intersection(&pos))
			{			
				Cursor::SetMode(CA_Speech);
				if(Control::Input->LBclicked())
				{
					Hud->Tutor->HideTutorial(true);
					ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT01_TalkWithRaja] = true;

					#ifdef _CEBUILD
						Hud->HideAwardUI();
					#endif 

					dialogSystem->ShowDialog(eDialog_S1RajaBeach);
					isDialogActive = true;
					Hud->EnableInput(false);
					Hud->HideHud(true);
				}				
			}
			return;
		}
		else if(Hud->Tutor->IsVisible && (Hud->Tutor->State == TUT02_TakeJames) )
		{
			if(!Hud->Tutor->isMouseOverTutorUI)
				Cursor::SetMode(CA_Normal);
			if(ActionRectJames.Intersection(&pos))
			{
				if(ProfileSystem::Instance->ActiveProfile.L1S1_IsTalkedWithRaja)
				{
					Cursor::SetMode(CA_HandTake);
					if(Control::Input->LBclicked())
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1RajaTakingJames);

						isRajaTakeJamesPlaying = true;

						CAnim_RajaTakeJames->PlayAnim();
						RajaJamesSCN->Show(true);

						Hud->AllowInput = false;

						ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT02_TakeJames] = true;
						Hud->Tutor->HideTutorial();
					}
				}
			}
			return;
		}
		else if(Hud->Tutor->IsVisible && (Hud->Tutor->State == TUT03_Journal) )
		{
			return;
		}
		else if(!Hud->Tutor->IsVisible && ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT04_ObjectivesTab] && !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT05_BagInvTake] && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly)
		{
			Hud->Tutor->ShowTutorial(TUT05_BagInvTake);
			return;
		}
		else if(Hud->Tutor->IsVisible && (Hud->Tutor->State == TUT05_BagInvTake) )
		{
			if(!Hud->Tutor->isMouseOverTutorUI)
				Cursor::SetMode(CA_Normal);
			if( !ProfileSystem::Instance->ActiveProfile.L1S1_IsInvBagTaken && InvItemRectBag.Intersection(&pos) )
			{
				Cursor::SetMode(CA_HandTake);
				if( Control::Input->LBclicked() )
				{
					ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT05_BagInvTake] = true;
					Hud->Tutor->HideTutorial();

					isSparkleUpdatePending = true;
					HOScene->GetObjectByID("Bag")->SetEnable(false);
					TakeInventoryItem(eInv_L1S1_Bag);
				}
			}
			else
				Cursor::SetMode(CA_Normal);
			return;
		}
		else if(Hud->Tutor->IsVisible && (Hud->Tutor->State == TUT06_BagZoomLook) )
		{
			if(!Hud->Tutor->isMouseOverTutorUI)
				Cursor::SetMode(CA_Normal);
			return;
		}
		else if(Hud->Tutor->IsVisible && (Hud->Tutor->State == TUT09_RosePopupZoom || Hud->Tutor->State == TUT10_UseKnifeOnRose) )
		{
			if(!Hud->Tutor->isMouseOverTutorUI)
				Cursor::SetMode(CA_Normal);
			//Pop1_Roses
			if ( ActivePopupID==ePopInactive || ActivePopupID==ePopL1S1_P1Roses )
			{
				switch( Popup1_State )
				{
				case POPUP_CLOSED:
					if( PopRect1Roses.Intersection(&pos) )
					{
						Cursor::SetMode(CA_Look);
						if( Control::Input->LBclicked() )
						{
							UpdateBgGraphic();

							Popup1_State = POPUP_SCALEUP;
							ActivePopupID = ePopL1S1_P1Roses;

							Pop_CloseButton = Pop1Roses->GetObjectByID("popClose");
							Pop_CloseButton->PlayAnimation(0);

							ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT09_RosePopupZoom] = true;
							Hud->Tutor->ShowTutorial(TUT10_UseKnifeOnRose);

							Control::Audio->PlaySample(aSFXpopup);
							PopVal = 0;
							PopupAlpha = 0.0f;
							SetupPopupTransition(Pop1Roses,PopRect1Roses);
						}
						return;
					}
					else
					{
						if(!Hud->Tutor->isMouseOverTutorUI)
							Cursor::SetMode(CA_Normal);
					}
					break;
		
				case POPUP_OPEN:
					if(Hud->Tutor->State == TUT10_UseKnifeOnRose)
					{
						if( !ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch2Cut && !ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby1Branch2Taken && InvDestRectThornBranch.Intersection(&pos) )
						{
							Cursor::SetMode(CA_Gear);
							if( Control::Input->LBclicked())
							{
								Pop1Roses->GetObjectByID("p1fixedbranch1")->Shake(270,3,0,0.25f);
								P1_Branch1->Shake(270,3,0,0.25f);
								Control::Audio->PlaySample(aSFXNotifySoft2);
								Hud->ShowBannerText("L1bush",SFX_NIL);
							}
						}
					}
					break;
				} 
			}
			return;
		}
		else if(Hud->Tutor->IsVisible && (Hud->Tutor->State == TUT11_MorphObject) )
		{
			if(!Hud->Tutor->isMouseOverTutorUI)
				Cursor::SetMode(CA_Normal);
			return;
		}
		else if(Hud->Tutor->IsVisible && (Hud->Tutor->State == TUT12_NavigationUp) )
		{
			if(!Hud->Tutor->isMouseOverTutorUI)
				Cursor::SetMode(CA_Normal);
			if( ActivePopupID == ePopInactive )
			{
				if( SceneRectFrontyard.Intersection(&pos) )
				{	
					Cursor::SetMode(CA_ExitUp,eArea_L1_S2_Frontyard);
					if( Control::Input->LBclicked() )
					{
						Control::Audio->PlayFootStepsFX();
						ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT12_NavigationUp] = true;
						GFHud->Tutor->HideTutorial(true);

						ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S2_Frontyard;
						SendMessage(ControlExit, ID, HOSM_NewArea);		
					}			
					return;
				}
			}
			return;
		}
	}
	else
	{
		// if not in Tutorial mode, then check for the BACK Key press
		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed() && ActivePopupID == ePopInactive ) {
				if(Hud!=NULL && GFInstance->IsInGameMenuPopup != true)Hud->CallPauseMenu();
			}
		#endif
	}

#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
#else
	Cursor::SetMode(CA_Normal);
#endif 


	if(isRajaTakeJamesPlaying)
	{
		if(CAnim_RajaTakeJames->IsAnimFinished())
		{
			isRajaTakeJamesPlaying = false;
			RajaJamesSCN->Show(false);

			if(Hud->Tutor != NULL && Hud->Tutor->IsActive)
			{
				Hud->AllowInput = true;
			}

			//Journal Entry 2 Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P3_SophiaIsTakenAway])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P3_SophiaIsTakenAway] = true;
				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected]
					= eJrn_L1P3_SophiaIsTakenAway;
						
				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;
				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
				Hud->ShowJrnEntryFX();

				if(Hud->Tutor != NULL && Hud->Tutor->IsActive && !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT03_Journal] && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly)
					Hud->Tutor->ShowTutorial(TUT03_Journal);
			}

			if( ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT10_UseKnifeOnRose] && !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT11_MorphObject])
			{
				if(Hud->Tutor != NULL && Hud->Tutor->IsActive )
				{				
#ifdef _CEBUILD
				if( ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S1Collected )//already picked morph objects, then no need to teach it!!
				{
					ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT11_MorphObject] = true;
					if( !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT12_NavigationUp] && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly )
						Hud->Tutor->ShowTutorial(TUT12_NavigationUp);	
					else
						Hud->Tutor->HideTutorial();	
				}
				else
				{
					Hud->ShowMorphObjects();
					Hud->Tutor->ShowTutorial(TUT11_MorphObject);
					Hud->isMorphTutorialActive = true;
					Hud->isMorphObjectsActive = true;				
					Hud->morphTimerThreshold = 1.5f; //quick loading if not fading in or out
				}
#else
				//nothing
				if( !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT12_NavigationUp] )
					Hud->Tutor->ShowTutorial(TUT12_NavigationUp);
#endif 
				}
			}

			//Objective Add
			if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L1O1_HelpJames] )
			{
				ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L1O1_HelpJames] = true;
				ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
						= eObj_L1O1_HelpJames;
				//Increment Objectives
				ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
				Hud->ShowObjectiveFX(eObj_L1O1_HelpJames);
			}

			ProfileSystem::Instance->ActiveProfile.L1S1_IsRajaTakenJames = true;

			ProfileSystem::Instance->SaveProfileBackup(ProfileSystem::Instance->ProfileSlot);

			TaskPendingUpdate();
			isSparkleUpdatePending = true;
		}
		return;
	}

	if(!ProfileSystem::Instance->ActiveProfile.L1S1_IsRajaTakenJames)
	{
		if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
			SparkleUpdate();
		
		if(TalkRectRaja.Intersection(&pos))
		{			
			Cursor::SetMode(CA_Speech);
			if(Control::Input->LBclicked())
			{
				if(!ProfileSystem::Instance->ActiveProfile.L1S1_IsTalkedWithRaja)
				{
					#ifdef _CEBUILD
						Hud->HideAwardUI();
					#endif 

					dialogSystem->ShowDialog(eDialog_S1RajaBeach);
					isDialogActive = true;
					Hud->EnableInput(false);
					Hud->HideHud(true);
				}
				else
				{
					Hud->ShowBannerText("L1mansion");
				}
				return;
			}
		}
		if(ActionRectJames.Intersection(&pos))
		{
			if(ProfileSystem::Instance->ActiveProfile.L1S1_IsTalkedWithRaja)
			{
				Cursor::SetMode(CA_HandTake);
				if(Control::Input->LBclicked())
				{
					Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1RajaTakingJames);

					isRajaTakeJamesPlaying = true;

					CAnim_RajaTakeJames->PlayAnim();
					RajaJamesSCN->Show(true);
				}
			}
			else
			{
				Cursor::SetMode(CA_Help);
				if(Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L1alive");
				}
			}
		}
		return;
	}

	if(IsZOrderChangeToScene)
	{
		IsZOrderChangeToScene = false;
		SetZ(zOrderScnVideo);
	}
	
	if( ActivePopupID == ePopInactive )
	{

		if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
			SparkleUpdate();


		#ifdef _CEBUILD
			if ( memObj_Teddy->IsActive && !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S1Teddy] && MemRectTeddy.Intersection(&pos))
			{						
				if( Control::Input->LBclicked() )
				{
					memObj_Teddy->FadeOut();
					Hud->ShowMemoryObjectCollected_CE(eMem_L1S1Teddy, &memObj_Teddy->Pos);
					//time to go back to nosatlgic music!!
					if( !Control::Audio->IsPlaying(aTrackL1Broken) &&
						!Control::Audio->IsPlaying(aTrackL1Mansion) &&
						!Control::Audio->IsPlaying(aTrackL1Hunted) &&
						!Control::Audio->IsPlaying(aTrackL1EscapePlan) )
						 Control::Audio->PlaySample(aTrackL1Mansion,false);
					return;
				}
			}
		#else
			if ( memObj_Teddy->IsActive && !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S1Teddy] && MemRectTeddy.Intersection(&pos))
			{
				Cursor::SetMode(CA_Help);
				if( Control::Input->LBclicked() )
				{		
					memObj_Teddy->FadeOut();
					Hud->ShowMemoryObjectCollected_SE(eMem_L1S1Teddy, &memObj_Teddy->Pos);					
				}
			}
		#endif 

		if( InfoRectStatue1.Intersection(&pos) || InfoRectStatue2.Intersection(&pos))
		{
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L1statues");
			}
		}

		if( InfoRectStatueSnake.Intersection(&pos) )
		{
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L1snake");
			}
		}

		if( SceneRectFrontyard.Intersection(&pos) )
		{	
			#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)				
				//Cursor::SetMode(CA_ExitUp,eArea_L1_S2_Frontyard);
			#else
				Cursor::SetMode(CA_ExitUp,eArea_L1_S2_Frontyard);
			#endif
			if( Control::Input->LBclicked() )
			{		
			
			#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L1_S2_Frontyard, 0, SceneRectFrontyard);
				}
			#else
				Control::Audio->PlayFootStepsFX();

				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S2_Frontyard;
				SendMessage(ControlExit, ID, HOSM_NewArea);		
			#endif
			}			

			return;
		}		

		if( !ProfileSystem::Instance->ActiveProfile.L1S1_IsInvBagTaken && InvItemRectBag.Intersection(&pos) )
		{
			Cursor::SetMode(CA_HandTake);
			if( Control::Input->LBclicked() )
			{
				isSparkleUpdatePending = true;
				HOScene->GetObjectByID("Bag")->SetEnable(false);
				TakeInventoryItem(eInv_L1S1_Bag);
			}
		}
	}

	if( !IsPopupTransitionActive )
		UpdatePopups(pos);

}

void L1_S1_Beach::UpdatePopups(CPoint pos,bool ItemDropped)
{
	//Pop1_Roses
	if ( (ActivePopupID==ePopInactive || ActivePopupID==ePopL1S1_P1Roses) && !IsAllRubyPiecesTaken )
	{
		switch( Popup1_State )
		{
		case POPUP_CLOSED:
			if( PopRect1Roses.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Look);
				if( Control::Input->LBclicked() || ItemDropped )
				{
					UpdateBgGraphic();

					Popup1_State = POPUP_SCALEUP;
					ActivePopupID = ePopL1S1_P1Roses;

					Pop_CloseButton = Pop1Roses->GetObjectByID("popClose");
					Pop_CloseButton->PlayAnimation(0);

					Control::Audio->PlaySample(aSFXpopup);
					PopVal = 0;
					PopupAlpha = 0.0f;
					SetupPopupTransition(Pop1Roses,PopRect1Roses);
				}
				return;
			}
			break;
		
		case POPUP_OPEN:
			#ifdef ENABLE_ANDROID_BACK_BUTTON
			if (Control::Input->isKeyBackSensed()) {
				CloseActivePopup();
			}
			#endif
			if(IsKnifeCut1Playing || IsKnifeCut2Playing || IsKnifeCut3Playing)
			{
				if( IsKnifeCut1Playing && CAnim_KnifeCut1->IsAnimFinished())
				{
					P1_SwissKnife->SetEnable(false);
					P1_Branch1->SetEnable(false);
					IsKnifeCut1Playing = false;
					Hud->InvBoxCtrl->ShowDragInv(true);
				}
				else if( IsKnifeCut2Playing && CAnim_KnifeCut2->IsAnimFinished())
				{
					P1_SwissKnife->SetEnable(false);
					P1_Branch2->SetEnable(false);
					IsKnifeCut2Playing = false;
					Hud->InvBoxCtrl->ShowDragInv(true);
				}
				else if( IsKnifeCut3Playing && CAnim_KnifeCut3->IsAnimFinished())
				{
					P1_SwissKnife->SetEnable(false);
					P1_Branch3->SetEnable(false);
					IsKnifeCut3Playing = false;
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
				if( !ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch2Cut && !ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby1Branch2Taken && InvDestRectThornBranch.Intersection(&pos) )
				{
					Cursor::SetMode(CA_Gear);
					if( Control::Input->LBclicked())
					{
						Pop1Roses->GetObjectByID("p1fixedbranch1")->Shake(270,3,0,0.25f);
						P1_Branch1->Shake(270,3,0,0.25f);
						Hud->ShowBannerText("L1bush",SFX_NIL);
						Control::Audio->PlaySample(aSFXNotifySoft2);
					}
				}

				if( ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch2Cut && !ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby1Branch2Taken)
				{
					if( RubyRect1Branch2.Intersection(&pos) )
					{
						Cursor::SetMode(CA_HandTake);
						if( Control::Input->LBclicked())
						{
							P1_Ruby1branch2->SetEnable(false);

							isSparkleUpdatePending = true;
							ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby1Branch2Taken = true;
							TakeInventoryItem(eInv_L1S1_RubyPart1);

							if( ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby4UnderLeavesTaken && ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby2Branch3Taken)
							{
								IsAllRubyPiecesTaken = true;
								CloseActivePopup(true);
								return;
							}
						}
						return;
					}
				}
				else if( !ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch3Cut && !ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby2Branch3Taken && InvDestRectThornBranch.Intersection(&pos))
				{
					Cursor::SetMode(CA_Gear);
					if( Control::Input->LBclicked())
					{
						Hud->ShowBannerText("L1bush",SFX_NIL);
						Control::Audio->PlaySample(aSFXNotifySoft2);
					}
				}

				if( ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch3Cut && !ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby2Branch3Taken)
				{
					if( RubyRect2Branch3.Intersection(&pos) )
					{
						Cursor::SetMode(CA_HandTake);
						if( Control::Input->LBclicked())
						{
							P1_Ruby2Branch3->SetEnable(false);

							isSparkleUpdatePending = true;
							ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby2Branch3Taken = true;
							TakeInventoryItem(eInv_L1S1_RubyPart2);

							if( ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby4UnderLeavesTaken && ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby1Branch2Taken)
							{
								IsAllRubyPiecesTaken = true;
								CloseActivePopup(true);
								return;
							}
						}
					}
				}

				if( ProfileSystem::Instance->ActiveProfile.L1S1_P1Leaf3Removed && !ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby4UnderLeavesTaken)
				{
					if( RubyRect4UnderLeaves.Intersection(&pos) )
					{
						Cursor::SetMode(CA_HandTake);
						if( Control::Input->LBclicked())
						{
							P1_Ruby4UnderLeaves->SetEnable(false);

							isSparkleUpdatePending = true;
							ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby4UnderLeavesTaken = true;
							TakeInventoryItem(eInv_L1S1_RubyPart4);

							if( ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby2Branch3Taken && ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby1Branch2Taken)
							{
								IsAllRubyPiecesTaken = true;
								CloseActivePopup(true);
								return;
							}
						}
					}
				}

				if( ProfileSystem::Instance->ActiveProfile.L1S1_P1Leaf1Removed && !ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby3UnderLeaf1Taken)
				{
					if( RubyRect3UnderLeafOne.Intersection(&pos) )
					{
						Cursor::SetMode(CA_HandTake);
						if( Control::Input->LBclicked())
						{
							P1_Ruby3UnderLeaf1->SetEnable(false);

							isSparkleUpdatePending = true;
							ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby3UnderLeaf1Taken = true;
							TakeInventoryItem(eInv_L1S1_RubyPart3);
						}
						return;
					}
				}

				if( !ProfileSystem::Instance->ActiveProfile.L1S1_P1Leaf1Removed )
				{
					if( LeavesRectGround.Intersection(&pos) )
					{
						Cursor::SetMode(CA_HandTake);
						if( Control::Input->LBclicked())
						{
							ProfileSystem::Instance->ActiveProfile.L1S1_P1Leaf1Removed = true;

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1LeafRemove);

							float valArr[5] = {0, P1_Leaf1->Pos.x+40, P1_Leaf1->Pos.y+40, 1.0, 15.0f};
							P1_Leaf1->TweenTo(valArr, 1.0f, 0, true);
						}
					}
				}
				else if( !ProfileSystem::Instance->ActiveProfile.L1S1_P1Leaf2Removed && ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby3UnderLeaf1Taken)
				{
					if( LeavesRectGround.Intersection(&pos) )
					{
						Cursor::SetMode(CA_HandTake);
						if( Control::Input->LBclicked())
						{
							ProfileSystem::Instance->ActiveProfile.L1S1_P1Leaf2Removed = true;

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1LeafRemove);

							float valArr[5] = {0, P1_Leaf2->Pos.x-40, P1_Leaf2->Pos.y-40, 1.0, -15.0f};
							P1_Leaf2->TweenTo(valArr, 1.0f, 0, true);							
						}
					}
				}
				else if( !ProfileSystem::Instance->ActiveProfile.L1S1_P1Leaf3Removed && ProfileSystem::Instance->ActiveProfile.L1S1_P1Leaf2Removed)
				{
					if( LeavesRectGround.Intersection(&pos) )
					{
						Cursor::SetMode(CA_HandTake);
						if( Control::Input->LBclicked())
						{
							ProfileSystem::Instance->ActiveProfile.L1S1_P1Leaf3Removed = true;

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1LeafRemove);

							float valArr[5] = {0, P1_Leaf3->Pos.x-40, P1_Leaf3->Pos.y-40, 1.0, -15.0f};
							P1_Leaf3->TweenTo(valArr, 1.0f, 0, true);							
						}
					}
				}
				
			}
			break;
		} 
	}

	//Pop2_CreekBoard
	if ( (ActivePopupID==ePopInactive || ActivePopupID==ePopL1S1_P2CreekBoard) )
	{
		switch( Popup2_State )
		{
		case POPUP_CLOSED:
			if( PopRect2CreekBoard.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Look);
				if( Control::Input->LBclicked() || ItemDropped )
				{
					UpdateBgGraphic();

					Popup2_State = POPUP_SCALEUP;
					ActivePopupID = ePopL1S1_P2CreekBoard;

					Pop_CloseButton = Pop2CreekBoard->GetObjectByID("popClose");
					Pop_CloseButton->PlayAnimation(0);

					Control::Audio->PlaySample(aSFXpopup);
					PopVal = 0;
					PopupAlpha = 0.0f;
					SetupPopupTransition(Pop2CreekBoard,PopRect2CreekBoard);

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
						if( !Control::Audio->IsPlaying(aTrackL1Broken) &&
							!Control::Audio->IsPlaying(aTrackL1Mansion) )
							Control::Audio->PlaySample(aTrackL1Mansion,false);
					}
				}
				return;
			}
			break;
		
		case POPUP_OPEN:
			#ifdef ENABLE_ANDROID_BACK_BUTTON
			if (Control::Input->isKeyBackSensed()) {
				CloseActivePopup();
			}
			#endif
			//Lightning
			p2lightTimer += Control::LogicRate;
			if(p2lightTimer>10.0f)
			{
				P2_Lightning->SetEnable(true);
				if(p2lightFrame==0){
					if(rand()%2>1)
						Control::Audio->PlaySample(aSFXL1ThunderV1, false);
					else
						Control::Audio->PlaySample(aSFXL1ThunderV2, false);
				}
				p2lightFrame += Control::LogicRate/2;
				int index = (int)(p2lightFrame*20.0f);
				if(p2lightFrame>=1.0f)
				{
					p2lightFrame = 0;
					index = 0;
					p2lightTimer = 0;
					P2_Lightning->SetEnable(false);
				}
				P2_Lightning->SetAlpha(lightAlphaArray4[index]);
			}
			
			Pop_CloseButton->PlayAnimation(0);
			if( Elixir::Distance(&pos,&CenterPoint) > 180 || Pop_CloseButton->Collide(&pos))//clicked outside
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
				if(  !ProfileSystem::Instance->ActiveProfile.L1S1_IsInvPlank3Taken )
				{
					if( InvItemRectPlank.Intersection(&pos) || InvItemRectPlank2.Intersection(&pos) )
					{
						Cursor::SetMode(CA_HandTake);
						if( Control::Input->LBclicked())
						{
							Pop2CreekBoard->GetObjectByID("p2plank")->SetEnable(false);

							HOScene->GetObjectByID("Plank")->SetEnable(false);
							UpdateBgGraphic();

							isSparkleUpdatePending = true;
							ProfileSystem::Instance->ActiveProfile.L1S1_IsInvPlank3Taken = true;
							TakeInventoryItem(eInv_L1S1_Plank3);
						}
						return;
					}
				}
				if( InforectCreekBoard.Intersection(&pos) )
				{
					Cursor::SetMode(CA_Help);
					if( Control::Input->LBclicked())
					{
						Hud->ShowBannerText("L1parents");
					}
				}
			}
			break;
		} 
	}
}

void L1_S1_Beach::CloseActivePopup(bool isDelayed)
{
	if(isDelayed)
		popupCloseDelayTimer = 1.0f;

	if( ActivePopupID==ePopL1S1_P1Roses )
	{
		

		P1RainVFX->StopImmediate();
		P1RaindropsVFX->StopImmediate();

		Control::Audio->PlaySample(aSFXpopdown);
		Popup1_State = POPUP_SCALEDOWN;

		Pop1Roses->Show(false);
		SetupPopupTransition(Pop1Roses,PopRect1Roses);
	}
	else if( ActivePopupID==ePopL1S1_P2CreekBoard )
	{
		/*P1RainVFX->StopImmediate();
		P1RaindropsVFX->StopImmediate();*/

		Control::Audio->PlaySample(aSFXpopdown);
		Popup2_State = POPUP_SCALEDOWN;

		P2_Lightning->SetEnable(false);

		Pop2CreekBoard->Show(false);
		SetupPopupTransition(Pop2CreekBoard,PopRect2CreekBoard);
	} 

}

void L1_S1_Beach::UpdatePopupTransition()
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

	if ( ActivePopupID==ePopL1S1_P1Roses )
	{
		switch( Popup1_State )
		{
		case POPUP_SCALEUP:
			PopVal += POPUP_SCALE_FACTOR * Control::LogicRate;
			PopupAlpha = easingQuadOut(PopVal,0,1,1.0f);

			if( PopVal >= 1.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 1.0f;				
				Pop1Roses->Show(true);
				Popup1_State = POPUP_OPEN;

				P1RainVFX->Start();
				P1RainVFX->AdvanceByTime(2.0f);
				P1RaindropsVFX->Start();
				P1RaindropsVFX->AdvanceByTime(2.0f);
			}
			GFInstance->UpdateBlurScene(PopupAlpha>1.0f?1.0f:PopupAlpha);
			break;

		case POPUP_SCALEDOWN:
			popupCloseDelayTimer -= 1.0f * Control::LogicRate;
			if( popupCloseDelayTimer > 0.0f ){
				return;	}
			else{
				popupCloseDelayTimer = 0.0;	}
			
			PopupAlpha -= POPUP_SCALEDOWN_FACTOR*Control::LogicRate;
			if( PopupAlpha <= 0.0f )
			{
				IsPopupTransitionActive = false;

				HOScene->PauseObjects(false);

				PopupAlpha = 0.0f;
				GFInstance->TurnOffBlurScene();

				Popup1_State = POPUP_CLOSED;
				ActivePopupID = ePopInactive;

				EndBgGraphicScaling();

				SetZ(zOrderScnVideo);

				if( Hud->Tutor != NULL && Hud->Tutor->IsActive && 
					ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT10_UseKnifeOnRose] && !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT11_MorphObject] && 
					ProfileSystem::Instance->ActiveProfile.L1S1_IsRajaTakenJames )
				{
					#ifdef _CEBUILD
						if( ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S1Collected )//already picked morph objects, then no need to teach it!!
						{
							ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT11_MorphObject] = true;
							if( !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT12_NavigationUp] && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly )
								Hud->Tutor->ShowTutorial(TUT12_NavigationUp);	
							else
								Hud->Tutor->HideTutorial();		
						}
						else
						{
							Hud->ShowMorphObjects();
							Hud->Tutor->ShowTutorial(TUT11_MorphObject);						
							Hud->isMorphTutorialActive = true;
							Hud->isMorphObjectsActive = true;
							Hud->morphTimerThreshold = 1.5f; //quick loading if not fading in or out
						}
					#else
						if( !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT12_NavigationUp] )
							Hud->Tutor->ShowTutorial(TUT12_NavigationUp);
					#endif 
				}

				#ifdef _CEBUILD
					Hud->ShowMorphObjects();
				#endif 
			}		
			else
				GFInstance->UpdateBlurScene(PopupAlpha);
			break;
		} 
	}
	else if ( ActivePopupID==ePopL1S1_P2CreekBoard )
	{
		switch( Popup2_State )
		{
		case POPUP_SCALEUP:
			PopVal += POPUP_SCALE_FACTOR * Control::LogicRate;
			PopupAlpha = easingQuadOut(PopVal,0,1,1.0f);

			if( PopVal >= 1.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 1.0f;				
				Pop2CreekBoard->Show(true);
				Popup2_State = POPUP_OPEN;

				/*P1RainVFX->Start();
				P1RainVFX->AdvanceByTime(2.0f);
				P1RaindropsVFX->Start();
				P1RaindropsVFX->AdvanceByTime(2.0f);*/
			}
			GFInstance->UpdateBlurScene(PopupAlpha>1.0f?1.0f:PopupAlpha);
			break;

		case POPUP_SCALEDOWN:
			popupCloseDelayTimer -= 1.0f * Control::LogicRate;
			if( popupCloseDelayTimer > 0.0f ){
				return;	}
			else{
				popupCloseDelayTimer = 0.0;	}
			
			PopupAlpha -= POPUP_SCALEDOWN_FACTOR*Control::LogicRate;
			if( PopupAlpha <= 0.0f )
			{
				IsPopupTransitionActive = false;

				HOScene->PauseObjects(false);

				PopupAlpha = 0.0f;
				GFInstance->TurnOffBlurScene();

				Popup2_State = POPUP_CLOSED;
				ActivePopupID = ePopInactive;

				EndBgGraphicScaling();

				SetZ(zOrderScnVideo);

				#ifdef _CEBUILD
					Hud->ShowMorphObjects();
				#endif 
			}		
			else
				GFInstance->UpdateBlurScene(PopupAlpha);
			break;
		} 
	}

	
}

float L1_S1_Beach::easingQuadOut(float t,float b,float c,float d)
{
	return (c * -t*(t-2) / d + b);
}

void L1_S1_Beach::Render(const CPoint* pCam)
{  
	if( !Visible ) 
		return;

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

void L1_S1_Beach::EndBgGraphicScaling()
{
	RT_BgScn->SetScale(1.0f);
	RT_BgScn->SetEnable(false);

	int size = (int)VFXArray.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)		
		VFXArray[i]->Show(true);
}

void L1_S1_Beach::UpdateBgGraphic()
{
	if(RT_BgScn == NULL)
	{
		RT_BgScn = new CObject();
		RT_BgScn->LoadSprite("UI\\ZOOMBG.SPR");
	//	K_LOG("zoombg loaded!");
		RT_BgScn->SetZ(17);
		RT_BgScn->SetEnable(false);
	//	K_LOG("zoombg set enable!");
	}

	#ifdef _CEBUILD
		Hud->HideMorphObjects();
	#endif 

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
        //K_LOG("RT_BgScn->pSprite->Image = GFInstance->CurrentSceneGraphic");
}

void  L1_S1_Beach::SetupPopupTransition(CHOScene *Popup, CRectangle popRect)
{
	
		HOScene->PauseObjects(true);

		int size = (int)VFXArray.size();  //Sac: converted Implicit to explicit
		for (int i = 0; i < size; i++)		
			VFXArray[i]->Show(false);
		
		bgScaleOffsetX = popRect.GetCenterX();
		bgScaleOffsetY = popRect.GetCenterY();
		
		RT_BgScn->SetEnable(true);
		RT_BgScn->SetScale(1.0f);
		RT_BgScn->SetXY(0,0);

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
		
		if(Popup1_State == POPUP_SCALEUP || Popup2_State == POPUP_SCALEUP)
			GFInstance->CreateGreyscaleScene();
		
		SetZ(zOrderPopVideo);

}

L1_S1_Beach::~L1_S1_Beach()
{
	TaskPendingUpdate();

	int size =(int) VFXArray.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++){
		SAFE_DELETE(VFXArray[i]);
	}

	size = (int)custAnimArray.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++){
		SAFE_DELETE(custAnimArray[i]);
	}

	if( GFInstance->availablePhyMem >= MAX_LOWEND_PEAKMEMORY )
	{
	}

	SAFE_DELETE(RajaJamesSCN);
	SAFE_DELETE(CAnim_RajaTakeJames);

	SAFE_DELETE(Pop1Roses);
	SAFE_DELETE(Pop2CreekBoard);

	SAFE_DELETE(P1RainVFX);
	SAFE_DELETE(P1RaindropsVFX);
	
	if(RT_BgScn!=NULL)
		RT_BgScn->pSprite->Image = NULL;
	SAFE_DELETE(RT_BgScn);
	
	//MUSIC_SFX
	//Quick SFX unloaded in HUD
	Control::Audio->StopSample(aAmbS1Beach);
	Control::Audio->UnloadSample(aAmbS1Beach);
	//MUSIC_SFX
}

void L1_S1_Beach::TakeInventoryItem(EHOInventory ItemID)
{
	std::vector<int> vecInvIds;
	switch (ItemID)
	{
	case eInv_L1S1_Bag:
			Hud->InvBoxCtrl->Activate(1);
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S1_Bag] = true;
			ProfileSystem::Instance->ActiveProfile.L1S1_IsInvBagTaken = true;
			vecInvIds.push_back(ItemID);
			Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
		break;
	case eInv_L1S1_Plank3:
			Hud->InvBoxCtrl->Activate(1);
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S1_Plank3] = true;
			ProfileSystem::Instance->ActiveProfile.L1S1_IsInvPlank3Taken = true;
			vecInvIds.push_back(ItemID);
			Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
		break;
	case eInv_L1S1_RubyPart1:
			Hud->InvBoxCtrl->Activate(1);
			ProfileSystem::Instance->ActiveProfile.Inventory[ItemID] = true;
			vecInvIds.push_back(ItemID);
			Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
			
		break;
	case eInv_L1S1_RubyPart2:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[ItemID] = true;			
			vecInvIds.push_back(ItemID);
			Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
			
		break;

	case eInv_L1S1_RubyPart3:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[ItemID] = true;
			vecInvIds.push_back(ItemID);
			Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
			
		break;
	case eInv_L1S1_RubyPart4:
			Hud->InvBoxCtrl->Activate(1);
			ProfileSystem::Instance->ActiveProfile.Inventory[ItemID] = true;			
			vecInvIds.push_back(ItemID);
			Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
		break;
	default:
		break;
	}

	TaskPendingUpdate();
}

void L1_S1_Beach::ReceiveMessage(int val)
{
	if( val == HOSM_DialogClosed ) 
	{
		isDialogActive = false;
		Hud->AllowInput = true;
		Hud->ShowHud(true);

		isSparkleUpdatePending = true;

		if(Hud->Tutor != NULL && Hud->Tutor->IsActive && !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT02_TakeJames] && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly)
			Hud->Tutor->ShowTutorial(TUT02_TakeJames);
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		TaskPendingUpdate();
		Hud->ShowMap();
	}
	else if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
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
			if( itemVal == eInv_L1S1_SwissKnife )
			{
				if( Popup1_State == POPUP_OPEN )
				{
					if(IsKnifeCut1Playing || IsKnifeCut2Playing || IsKnifeCut3Playing)
						return;
					if( !ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch1Cut )
					{
						if(InvDestRectThornBranch.Intersection(&dropPos))
						{
							ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch1Cut = true;

							P1_SwissKnife->SetEnable(true);
							CAnim_BranchCut1->PlayAnim();
							CAnim_KnifeCut1->PlayAnim();

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1BranchCutSFX);

							IsKnifeCut1Playing = true;

							GFHud->InvBoxCtrl->ShowDragInv(false);

							if(Hud->Tutor != NULL && Hud->Tutor->IsActive)
							{
								ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT10_UseKnifeOnRose] = true;
								GFHud->Tutor->HideTutorial(true);
							}
						}
						else
							GFInstance->WrongInvDropMsg(Hud);
					}
					else if( !ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch2Cut )
					{
						if(InvDestRectThornBranch.Intersection(&dropPos))
						{
							ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch2Cut = true;

							P1_SwissKnife->SetZ(P1_Branch2->ZOrder+1);
							P1_SwissKnife->SetEnable(true);
							CAnim_BranchCut2->PlayAnim();
							CAnim_KnifeCut2->PlayAnim();

							GFHud->InvBoxCtrl->ShowDragInv(false);

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1BranchCutSFX);

							IsKnifeCut2Playing = true;

							//Hud->ReleaseDrag();
							//Hud->CloseInventory();
						}
						else
							GFInstance->WrongInvDropMsg(Hud);
					}
					else if( !ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch3Cut )
					{
						if(InvDestRectThornBranch.Intersection(&dropPos))
						{
							ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch3Cut = true;

							P1_SwissKnife->SetZ(P1_Branch3->ZOrder-1);
							P1_SwissKnife->SetEnable(true);
							CAnim_BranchCut3->PlayAnim();
							CAnim_KnifeCut3->PlayAnim();

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S1BranchCutSFX);

							IsKnifeCut3Playing = true;

							Hud->ReleaseDrag();
							if (ProfileSystem::Instance->ActiveProfile.L1S2_P3IsSwingRopeCut && ProfileSystem::Instance->ActiveProfile.L1S7_IsRopeCut)
							{
								//if frontyard swing and Boat Rope cut, no need for knife
								ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S1_SwissKnife] = false;
								Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S1_SwissKnife); 
							}

							
							
							//Hud->CloseInventory();  
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
		if(Hud->Tutor != NULL && Hud->Tutor->IsActive && !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT06_BagZoomLook] && ProfileSystem::Instance->ActiveProfile.L1S1_IsInvBagTaken)
		{
			Hud->InvBoxCtrl->IsLocked = true;																							 
			Hud->InvBoxCtrl->Activate();

			Hud->Tutor->ShowTutorial(TUT06_BagZoomLook);
		}

	}
}

void L1_S1_Beach::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S1Collected)
		{
			InitHintTrailEmit(930, 100);
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S1Teddy])
		{
			InitHintTrailEmit(785, 540);
		}
		else 
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}

	if(isRajaTakeJamesPlaying)
		return;

	 int iNonHOHint = 0; 
	 int MaxNonHOHints = 16;
	K_LOG("Sac: Non HO HO hint about to start at Beach....");
	do
	{
		K_LOG("Sac: Non HO HO hint do Loopp.... %d time",iNonHOHint);
		switch( iNonHOHint )
		{
			default:			
					K_LOG("Sac: Non HO HO hint do Loopp switch case default....");
			//Exit coordinates
			case 0: 
					++iNonHOHint;
					K_LOG("Sac: Non HO HO hint do Loopp switch case 0.... %d time",iNonHOHint);
					if( !ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby1Branch2Taken  && ProfileSystem::Instance->ActiveProfile.L1S1_IsInvKnifeTaken && Popup1_State == POPUP_OPEN)
					{
						K_LOG("Sac: Non HO HO hint do Loopp switch case 0 first IF....");
						if( !ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch3Cut)
						{
							K_LOG("Sac: Non HO HO hint do Loopp switch case 0 Second IF....");

							Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S1_SwissKnife)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}


							CPoint tempPoint (RubyRect1Branch2.GetCenterX(), RubyRect1Branch2.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						}
						else
							InitHintTrailEmit(	RubyRect1Branch2.GetCenterX(), RubyRect1Branch2.GetCenterY(),false);
						return;
					}
					break;
					
			case 1: 
					++iNonHOHint;
					 if(  !ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby2Branch3Taken && ProfileSystem::Instance->ActiveProfile.L1S1_IsInvKnifeTaken && Popup1_State == POPUP_OPEN)
					{
						if( !ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch3Cut)
						{
							Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S1_SwissKnife)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}

							CPoint tempPoint (RubyRect2Branch3.GetCenterX(), RubyRect2Branch3.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						}
						else
						InitHintTrailEmit(	RubyRect2Branch3.GetCenterX(), RubyRect2Branch3.GetCenterY(),false);
						return;
					}
					break;	
			case 2: 
					++iNonHOHint;
					 if(!ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby4UnderLeavesTaken  && Popup1_State == POPUP_OPEN)
					{
						InitHintTrailEmit(	RubyRect4UnderLeaves.GetCenterX(), RubyRect4UnderLeaves.GetCenterY(),false);
						return;
					}
					break;	
			case 3: 
					++iNonHOHint;
					 if(!ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby3UnderLeaf1Taken  && Popup1_State == POPUP_OPEN)
					{
						InitHintTrailEmit(	RubyRect3UnderLeafOne.GetCenterX(), RubyRect3UnderLeafOne.GetCenterY(),false);
						return;
					}
					break;	
			case 4: 
					++iNonHOHint;
					 if( Popup1_State == POPUP_OPEN)
					{
						InitHintTrailEmit(	PopCloseButtonRect.GetCenterX(), PopCloseButtonRect.GetCenterY(),true);
						return;
					}
					break;	
			case 5: 
					++iNonHOHint;
					 if(!ProfileSystem::Instance->ActiveProfile.L1S1_IsInvPlank3Taken && Popup2_State == POPUP_OPEN)
					{
						InitHintTrailEmit(	InvItemRectPlank.GetCenterX(), InvItemRectPlank.GetCenterY(),false);
						return;
					}
					break;	
			case 6: 
					++iNonHOHint;
					 if( Popup2_State == POPUP_OPEN)
					{
						InitHintTrailEmit(Pop_CloseButton->Pos.x, Pop_CloseButton->Pos.y,true);
						return;
					}
					break;	
			case 7: 
					++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S1_IsTalkedWithRaja)
					{
						InitHintTrailEmit(TalkRectRaja.GetCenterX(), TalkRectRaja.GetCenterY(),false);
						return;
					}
					break;
			case 8: 
					++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S1_IsRajaTakenJames)
					{
						InitHintTrailEmit(ActionRectJames.GetCenterX(), ActionRectJames.GetCenterY(),false);
						return;
					}
					break;
			case 9: 
					++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L1S1_IsRajaTakenJames && !ProfileSystem::Instance->ActiveProfile.L1S1_IsInvBagTaken)
					{
						InitHintTrailEmit(InvItemRectBag.GetCenterX(), InvItemRectBag.GetCenterY(),false);
						return;
					}
					break;
			case 10: 
					++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L1S1_IsInvBagTaken && !ProfileSystem::Instance->ActiveProfile.L1S1_IsInvKnifeTaken)
					{																																  
						Hud->InvBoxCtrl->IsLocked = true;																							 
						Hud->InvBoxCtrl->Activate();
						
						for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
						{
							if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
							{	
								if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S1_Bag)
								{
									if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
											Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
									else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
											Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
									InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
								}
							}
						}
											
						return;
					}
					break;

			case 11: 
					++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L1S1_IsInvKnifeTaken && !ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby2Branch3Taken)
					{
						InitHintTrailEmit(PopRect1Roses.GetCenterX(), PopRect1Roses.GetCenterY(),false);
						return;
					}
					break;
			case 12: 
					++iNonHOHint;
					 if(!ProfileSystem::Instance->ActiveProfile.L1S1_IsInvPlank3Taken )
					{
						InitHintTrailEmit(	PopRect2CreekBoard.GetCenterX(), PopRect2CreekBoard.GetCenterY(),false);
						return;
					}
					break;	
			case 13: 
					++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S2_Frontyard] || ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S2_Frontyard])
					{
						InitHintTrailEmit(SceneRectFrontyard.GetCenterX(), SceneRectFrontyard.GetCenterY(),true,CA_ExitUp);
						return;
					}
					break;
			case 14:
				++iNonHOHint;
				if (!ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby3UnderLeaf1Taken || !ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby4UnderLeavesTaken)
				{
					InitHintTrailEmit(PopRect1Roses.GetCenterX(), PopRect1Roses.GetCenterY(), false);
					return;
				}
				break;
			case 15: 
					++iNonHOHint;
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

void L1_S1_Beach::NavigateToScene()
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
