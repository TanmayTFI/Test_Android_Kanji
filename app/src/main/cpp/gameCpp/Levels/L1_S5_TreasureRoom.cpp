//====================================
//	DQFYLH
//	HO System
//	Level 1 - Scene 05 : Treasure Room
//====================================

#include "L1_S5_TreasureRoom.h"
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

const CPoint		CenterPoint(683,334,0);
const CPoint		PopCloseRectBtnPoint(998,98,0);
const CRectangle	PopCloseButtonRect(968.0f,71.0f,62.0f,61.0f);
const CRectangle	PopAreaRect(345.0f,90.0f,665.0f,501.0f);

//Scene Rects
const CRectangle  SceneRectTunnelDoor(924.0f,80.0f,220.0f,320.0f);
const CRectangle  SceneRectDynamitePuzzle(580.0f,187.0f,306.0f,214.0f);

//Popup Rects
const CRectangle  PopRect1WallSide(406.0f,187.0f,156.0f,114.0f);
const CRectangle  PopRect2FireLamp(259.0f,269.0f,137.0f,154.0f);

const CRectangle  PopRectTorchFlaming(499.0f,160.0f,371.0f,361.0f);


//Inventory Destination Rects

//Inventory Item Rects
const CRectangle  InvItemRectHammerHead(726.0f,247.0f,142.0f,257.0f);
const CRectangle  InvItemRectBoatKey(614.0f,258.0f,168.0f,130.0f);
const CRectangle  InvItemRectSpoon(664.0f,316.0f,140.0f,160.0f);

const CRectangle  InfoRectsmoke(302.0f,0.0f,307.0f,224.0f);



//Info Rects
const CRectangle  InfoRectMatchBoxUse(553.0f,218.0f,276.0f,227.0f);

L1_S5_TreasureRoom::L1_S5_TreasureRoom()
{
	Visible = false;
	isTorchLightAnimPlaying = false;
	isMatchBoxAnimPlaying = false;
	isWoodDropAnimPlaying = false;	

	RT_BgScn = NULL;

	bgScaleOffsetX = 0.0f;
	bgScaleOffsetY = 0.0f;

	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV1\\S5\\S5.SCN");


	//Scene Elements
	amoniBottle = HOScene->GetObjectByID("amoniBottle");
	amoniBottleCork = HOScene->GetObjectByID("amoniBottleCork");
	bottle = HOScene->GetObjectByID("bottle");
	closedBoxDoor = HOScene->GetObjectByID("closedBoxDoor");
	crystalInGrinder1 = HOScene->GetObjectByID("crystalInGrinder1");
	crystalInGrinder3 = HOScene->GetObjectByID("crystalInGrinder3");
	greenPowderOnPlate = HOScene->GetObjectByID("greenPowderOnPlate");
	bluePowderOnPlate = HOScene->GetObjectByID("bluePowderOnPlate");
	map1 = HOScene->GetObjectByID("map1");
	map2 = HOScene->GetObjectByID("map2");
	openedBoxDoor = HOScene->GetObjectByID("openedBoxDoor");
	thread = HOScene->GetObjectByID("thread");
	threadOnBottle = HOScene->GetObjectByID("threadOnBottle");
	threadWaxOnBottle = HOScene->GetObjectByID("threadWaxOnBottle");
	waxCylinder = HOScene->GetObjectByID("waxCylinder");
	dark = HOScene->GetObjectByID("dark");
	brockenDoor = HOScene->GetObjectByID("brockenDoor");
	woodDown = HOScene->GetObjectByID("woodDown");
	woodNhammer = HOScene->GetObjectByID("woodNhammer");
	hammer = HOScene->GetObjectByID("hammer");
	key = HOScene->GetObjectByID("key");

	Control::Audio->LoadSample(aVOL1MaryRefS5_1,AudioVO);
	Control::Audio->LoadSample(aVOL1MaryRefS5_2,AudioVO);

	int ArrayPts1[] = {750,443,941,476,1008,199,1008,91,873,88};
	polyWood =  new CPolygon(5);
	polyWood->Init(ArrayPts1);

	Fire[0] = HOScene->GetObjectByID("Fire1Anim");
	Fire[1] = HOScene->GetObjectByID("Fire3Anim");

	Smoke = HOScene->GetObjectByID("L1S2SmokeAnim");

	HOScene->GetObjectByID("darklightray1")->Animate(0.35f,0.65f,6.0f);
	HOScene->GetObjectByID("darklightray2")->Animate(0.30f,0.65f,6.0f,0.56f);

	if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsTorchLight)
	{
		if( !ProfileSystem::Instance->ActiveProfile.L1S5_InvBoatKeyTaken )//no activities started in this scene - then remind audience last action!!
		{
			Hud->ShowBannerText("L1ouch");
		}
		Smoke->SetEnable(false);
		for (int i = 0; i < 2; i++)
		{
			Fire[i]->SetEnable(false);
		}
	}
	else
	{
		if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsFireAttackVideoPlayed)
			Smoke->SetEnable(false);
		else
		{
			if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsFireAttckBanner)
			{
				Hud->ShowBannerText("L1mean");
				Control::Audio->PlaySample(aVOL1MaryRefS5_1);
				ProfileSystem::Instance->ActiveProfile.L1S5_IsFireAttckBanner = true;
			}
		}
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_InvBoatKeyTaken)
	{
		key->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_IsFireAttackVideoPlayed)
	{
		woodNhammer->SetEnable(false);
	}

	if(!ProfileSystem::Instance->ActiveProfile.L1S5_InvHammerHeadTaken)
	{
		woodDown->SetEnable(false);
	}
	else
	{
		woodNhammer->SetEnable(false);
		hammer->SetEnable(false);
	}

	if(!ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite)
	{
		brockenDoor->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_IsTorchLight)
	{
		dark->SetEnable(false);
	}


	if(ProfileSystem::Instance->ActiveProfile.L1S5_P1IsBoxOpened)
	{
		closedBoxDoor->SetEnable(false);
		openedBoxDoor->SetEnable(true);
	}
	else
	{
		openedBoxDoor->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_P1IsMapMovedFromPlate)
	{
		map1->SetEnable(false);
		map2->SetEnable(true);
	}
	else
	{
		map1->SetEnable(true);
		map2->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_P1IsBottleCapRemoved)
	{
		amoniBottleCork->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderAddedToPlate)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S5_P1AmoniAddedToPlate)
		{
			greenPowderOnPlate->SetEnable(true);
			bluePowderOnPlate->SetEnable(false);
			amoniBottle->SetEnable(false);
			amoniBottleCork->SetEnable(false);
		}
		else
		{
			greenPowderOnPlate->SetEnable(false);
			bluePowderOnPlate->SetEnable(true);
		}
	}
	else
	{
		greenPowderOnPlate->SetEnable(false);
		bluePowderOnPlate->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalAddedToGrinder)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S5_P1IsCrystalPowderCreated)
		{
			crystalInGrinder3->SetEnable(true);
			crystalInGrinder1->SetEnable(false);
		}
		else
		{
			crystalInGrinder3->SetEnable(false);
			crystalInGrinder1->SetEnable(true);
		}
	}
	else
	{
		crystalInGrinder1->SetEnable(false);
		crystalInGrinder3->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_P1ThreadAddedToBottle)
	{
		thread->SetEnable(false);
		if(ProfileSystem::Instance->ActiveProfile.L1S5_P1WaxCylinderAddedToBottle)
		{
			waxCylinder->SetEnable(false);
			threadWaxOnBottle->SetEnable(true);
			threadOnBottle->SetEnable(false);
		}
		else
		{
			threadWaxOnBottle->SetEnable(false);
			threadOnBottle->SetEnable(true);
		}
	}
	else
	{
		threadWaxOnBottle->SetEnable(false);
		threadOnBottle->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_IsInvBombTaken)
	{
		bottle->SetEnable(false);
		threadWaxOnBottle->SetEnable(false);
		threadOnBottle->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_IsInvHammerHandTaken)
		HOScene->GetObjectByID("hammerHand")->SetEnable(false);
	if(!ProfileSystem::Instance->ActiveProfile.L1S5_P1MortarPestlePlaced)
	{
		HOScene->GetObjectByID("grinder")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("PestleMask")->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_InvHammerHeadTaken)
		HOScene->GetObjectByID("hammerhead")->SetEnable(false);

	GFInstance->PostUpdate(1);

	#ifdef _PARTICLEEDITORENABLED
		IsParticleEditorEnabled = false;
		if (IsParticleEditorEnabled)
		{
			_particleEditor = new ParticleEditorControl();
			AddControl(_particleEditor); 
		}
	#endif

	InitPopups();	

	if( GFInstance->availablePhyMem >= MAX_LOWEND_PEAKMEMORY )
	{
	}

	
	GoldLeftVFX = new SuperFX("PRT\\L1\\s5goldglitleft.INI", PointSystem::CreateCPoint(543,414,0), 100);
	GoldRightVFX = new SuperFX("PRT\\L1\\s5goldglitright.INI", PointSystem::CreateCPoint(850,447,0), 100);

	VFXArray.push_back(GoldLeftVFX);
	VFXArray.push_back(GoldRightVFX);

	DarkLightVFX = new SuperFX("PRT\\L1\\s5darklight.INI", PointSystem::CreateCPoint(263,59,0), 100);

	VFXArray.push_back(DarkLightVFX);

	if(ProfileSystem::Instance->ActiveProfile.L1S5_IsTorchLight)
	{
		HOScene->GetObjectByID("darklightray1")->SetEnable(false);
		HOScene->GetObjectByID("darklightray2")->SetEnable(false);

		GoldLeftVFX->Start();
		GoldRightVFX->Start();
	}
	else
	{
		DarkLightVFX->Start();
	}

	

	MoonFliesVFX = new SuperFX("PRT\\L1\\moonfxbeach.INI", PointSystem::CreateCPoint(596.0f,210.0f,0), 100);
	MoonFliesVFX->Start();
	MoonFliesVFX->AdvanceByTime(2.0f);

	VFXArray.push_back(MoonFliesVFX);

	GFInstance->PostUpdate(2);

	SetZ(eZOrder_Popup + 200);

	//MUSIC_SFX
	Control::Audio->ClearUnusedTracks();

	if( !Control::Audio->IsPlaying(aTrackL1Trapped) )
		Control::Audio->PlaySample(aTrackL1Trapped,true);


	Control::Audio->LoadSample(aAmbS5TreasureRoom,AudioAmb);
	Control::Audio->PlaySample(aAmbS5TreasureRoom,true);
	//MUSIC_SFX

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L1_S5_TreasureRoom]  = true;

	if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S5_TreasureRoom])
	{
		Hud->ShowBannerText("L1ouch");
		Control::Audio->PlaySample(aVOL1MaryRefS5_2);
		ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S5_TreasureRoom] = true;
	}

	
	
	Visible = true;	//To render anything with renderlistentity

	//Objective Add
	if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L1O4_EscapeMansion] )
	{
		ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L1O4_EscapeMansion] = true;
		ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
		= eObj_L1O4_EscapeMansion;
		//Increment Objectives
		ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
		Hud->ShowObjectiveFX(eObj_L1O4_EscapeMansion);

		Control::Audio->PlaySample(aSFXMagic1);
	}

	//Journal Entry 13 Add
	if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P13_LasyaPush])
	{
		//Flag Set
		ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P13_LasyaPush] = true;
		//Add to Note Array
		ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected]
			= eJrn_L1P13_LasyaPush;
						
		ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;
		//Increment Notes
		ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
		Hud->ShowJrnEntryFX();
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_IsFireAttackVideoPlayed)
	{
		//Journal Entry 14 After Lasya Attack
		if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P14_AfterLasyaAttack])
		{
			//Flag Set
			ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P14_AfterLasyaAttack] = true;
			//Add to Note Array
			ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected]
			= eJrn_L1P14_AfterLasyaAttack;

			ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;
			//Increment Notes
			ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
			Hud->ShowJrnEntryFX();
		} 
	}

	Hud->UnlockCharacter(eChar10_Lasya, 5.0f);

	UpdateBgGraphic();
	TaskPendingUpdate();
}

void L1_S5_TreasureRoom::TaskPendingUpdate()
{
	bool isAnyTaskLeft = true;
	ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L1_S1_Beach] = true;
	ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L1_S2_Frontyard] = true;
	ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L1_S3_Foyer] = true;
	ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L1_S4_Bedroom] = true;
	ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L1_S6_BoatShed] = true;
	ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L1_S7_BoatDock] = true;
	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S5_TreasureRoom] = isAnyTaskLeft;
}

void L1_S5_TreasureRoom::InitPopups()
{	
	ActivePopupID = ePopInactive;

	popupCloseDelayTimer = 0.0f;
	
	Pop_CloseButton = NULL;

	IsPopupTransitionActive = false;


	Pop2FireLamp = NULL;
	if ( !ProfileSystem::Instance->ActiveProfile.L1S5_IsTorchLight )
	{
		Pop2FireLamp = new CHOScene();
		Pop2FireLamp->Load("LV1\\S5\\POP\\P2.SCN");

		//Add popup frame and close button
		CObject *popCirFrame = new CObject();
		popCirFrame->LoadInit("UI\\popcircframe.SPR", 4, -9, 1);
		popCirFrame->SetUID("popFrame");
		Pop2FireLamp->InsertObject(popCirFrame,1);

		CObject *popCirCloseBtn = new CObject();
		popCirCloseBtn->LoadInit("UI\\closebtn.SPR", 145, -158, 7);
		popCirCloseBtn->SetUID("popClose");
		Pop2FireLamp->PushObject(popCirCloseBtn);

		Pop2FireLamp->Show(false);
		Pop2FireLamp->SetPosOff(CenterPoint.x,CenterPoint.y);
		Pop2FireLamp->SetZOff(eZOrder_Popup);

		p2key = Pop2FireLamp->GetObjectByID("p2key");

		if(ProfileSystem::Instance->ActiveProfile.L1S5_InvBoatKeyTaken)
		{
			p2key->SetEnable(false);
		}
		p2TorchLightAnim = Pop2FireLamp->GetObjectByID("p2TorchLightAnim");
		p2TorchLightAnim->SetEnable(false);

		p2MatchBoxAnim = Pop2FireLamp->GetObjectByID("p2MatchBoxAnim");
		p2MatchBoxAnim->SetEnable(false);
	}
	Popup2_State = POPUP_CLOSED;

	PopupAlpha = 0.0f;
	PopVal = 0.0f;

	GFInstance->PostUpdate(1);
}

void L1_S5_TreasureRoom::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( IsPopupTransitionActive )
		UpdatePopupTransition();
}

void L1_S5_TreasureRoom::Update()
{
	NavigateToScene();
	#ifdef _PARTICLEEDITORENABLED
		if (IsParticleEditorEnabled)
		{
			if(!_particleEditor->IsDisabled)
				return; 
		}
	#endif

	if(GFInstance->IsSceneGraphicReloaded)
	{
		UpdateBgGraphic();
		GFInstance->IsSceneGraphicReloaded = false;
	}
	
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	if( ActivePopupID == ePopInactive )
	{
		if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
			SparkleUpdate();

		if( SceneRectTunnelDoor.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S5_IsTorchLight)
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
					GFHud->InvBoxCtrl->GoToScene(eArea_L1_S5_TunnelDoor, 0, SceneRectTunnelDoor);
				}
#else
				Control::Audio->PlayFootStepsFX();

				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S5_TunnelDoor;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}			
			return;
		}
		else if( SceneRectDynamitePuzzle.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S5_IsTorchLight && !ProfileSystem::Instance->ActiveProfile.L1S5_IsInvBombTaken)
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
					GFHud->InvBoxCtrl->GoToScene(eArea_L1_S5_PZDynamiteMaking, 0, SceneRectDynamitePuzzle);
				}
#else
				Control::Audio->PlayFootStepsFX();

				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S5_PZDynamiteMaking;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}			
			return;
		}
		else if( PopRect1WallSide.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S5_IsTorchLight && !ProfileSystem::Instance->ActiveProfile.L1S5_IsFireAttackVideoPlayed)
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
					GFHud->InvBoxCtrl->GoToScene(eArea_L1_S5_HOTreasureRoom, 0, PopRect1WallSide);
				}
#else
				Control::Audio->PlayFootStepsFX();

				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S5_HOTreasureRoom;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}			
			return;
		}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
		{
				if(ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L1_S4_Bedroom])
				{
					Hud->ShowBannerText("L1pretty");
				}
				else
				{
					Control::Audio->PlayFootStepsFX();
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S4_Bedroom;
					SendMessage(ControlExit, ID, HOSM_NewArea);						
				}	
			return;
		}
	#endif

		if( IsMouseOverGoBackArea(&pos) )
		{

#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
				//Cursor::SetMode(CA_ExitDown,eArea_L1_S4_Bedroom);
#else
			Cursor::SetMode(CA_ExitDown,eArea_L1_S4_Bedroom);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					if(ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L1_S4_Bedroom]) {
						Hud->ShowBannerText("L1pretty");
					} else{
						GFHud->InvBoxCtrl->GoToScene(eArea_L1_S4_Bedroom, 2);
					}
				}
#else
				if(ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L1_S4_Bedroom])
				{
					Hud->ShowBannerText("L1pretty");
				}
				else
				{
					Control::Audio->PlayFootStepsFX();
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S4_Bedroom;
					SendMessage(ControlExit, ID, HOSM_NewArea);
				}
#endif

			}			
			return;
		}
		else if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsTorchLight && !PopRect2FireLamp.Intersection(&pos))
		{
			Cursor::SetMode(CA_Help);
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L1down");
			}
			return;
		}
		else if(InfoRectsmoke.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S5_IsFireAttackVideoPlayed)
		{
			Cursor::SetMode(CA_Help);
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L1house");
			}
			return;
		}
		else if(ProfileSystem::Instance->ActiveProfile.L1S5_IsTorchLight && !SceneRectDynamitePuzzle.Intersection(&pos)&& !SceneRectTunnelDoor.Intersection(&pos) && !PopRect1WallSide.Intersection(&pos))
		{
			Cursor::SetMode(CA_Help);
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L1precious");
			}
			return;
		}
	}
	

	if( !IsPopupTransitionActive )
		UpdatePopups(pos);

}

void L1_S5_TreasureRoom::UpdatePopups(CPoint pos,bool ItemDropped)
{
	

	//Pop2_Firelamp
	if ( (ActivePopupID==ePopInactive || ActivePopupID==ePopL1S5_P2FireLamp) )
	{
		switch( Popup2_State )
		{
		case POPUP_CLOSED:
			if( PopRect2FireLamp.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S5_IsTorchLight)
			{
				Cursor::SetMode(CA_Look);
				if( Control::Input->LBclicked() || ItemDropped )
				{
					Popup2_State = POPUP_SCALEUP;
					ActivePopupID = ePopL1S5_P2FireLamp;

					UpdateBgGraphic();

					Pop_CloseButton = Pop2FireLamp->GetObjectByID("popClose");

					//Control::Audio->PlaySample(aSFXpopup);
					PopVal = 0;
					PopupAlpha = 0.2f;
					SetupPopupTransition(Pop2FireLamp, PopRect2FireLamp);
				}
				return;
			}
			break;
		
		case POPUP_OPEN:
			if(isTorchLightAnimPlaying)
			{
				if(p2TorchLightAnim->IsAnimationFinished())
				{
					isTorchLightAnimPlaying = false;
					dark->SetEnable(false);

					HOScene->GetObjectByID("darklightray1")->SetEnable(false);
					HOScene->GetObjectByID("darklightray2")->SetEnable(false);
					DarkLightVFX->StopEmit();

					GoldLeftVFX->Start();
					GoldRightVFX->Start();

					for (int i = 0; i < 2; i++)
					{
						Fire[i]->SetEnable(true);
					}

					//setting the smoke animation here 
					//Smoke->SetEnable(true);

					UpdateBgGraphic();
					CloseActivePopup();
				}
				return;
			}
			if(isMatchBoxAnimPlaying)
			{
				if(p2MatchBoxAnim->IsAnimationFinished())
				{
					isMatchBoxAnimPlaying = false;
					p2MatchBoxAnim->FadeOut(4.0f);

					p2TorchLightAnim->SetEnable(true);
					p2TorchLightAnim->ForcePlayAnimation(0);
					isTorchLightAnimPlaying = true;
				}
				return;
			}
			Pop_CloseButton->PlayAnimation(0);
			if( Elixir::Distance(&pos,&CenterPoint) > 240 || Pop_CloseButton->Collide(&pos) ) //clicked outside or close Btn
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
				if(!ProfileSystem::Instance->ActiveProfile.L1S5_InvBoatKeyTaken)
				{
					if(InvItemRectBoatKey.Intersection(&pos))
					{
						Cursor::SetMode(CA_HandTake);
						if( Control::Input->LBclicked() )
						{
							//Show Objective completed
							if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L1O3_FindBoatKey] && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L1O3_FindBoatKey] )
							{
								ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L1O3_FindBoatKey] = true;
								Hud->ShowObjectiveFX(eObj_L1O3_FindBoatKey,true);
							}

							key->SetEnable(false);
							UpdateBgGraphic();

							p2key->SetEnable(false);
							TakeInventoryItem(eInv_L1S5_BoatKey);
						}
					}
				}
				else if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsTorchLight)
				{
					if(InfoRectMatchBoxUse.Intersection(&pos))
					{
						Cursor::SetMode(CA_Gear);
						if( Control::Input->LBclicked())
						{
							Hud->ShowBannerText("L1oil");

						}
					}
				}
			}
			break;
		} 
	}
}

void L1_S5_TreasureRoom::CloseActivePopup(bool isDelayed)
{
	if(isDelayed)
		popupCloseDelayTimer = 1.0f;

	Control::Audio->PlaySample(aSFXpopdown);

	if( ActivePopupID==ePopL1S5_P2FireLamp )
	{
		Popup2_State = POPUP_SCALEDOWN;

		Pop2FireLamp->Show(false);
		SetupPopupTransition(Pop2FireLamp,PopRect2FireLamp);
	}
}

void L1_S5_TreasureRoom::UpdatePopupTransition()
{
	
	RT_BgScn->SetScale(1.0f + PopupAlpha * 0.25f);
	RT_BgScn->SetXY(bgScaleOffsetX - (bgScaleOffsetX * (1.0f + PopupAlpha  * 0.25f)) ,bgScaleOffsetY - (bgScaleOffsetY * (1.0f + PopupAlpha * 0.25f)));
	//for 1024
	//RT_BgScn->SetXY(bgScaleOffsetX - (bgScaleOffsetX * (1.0f + PopupAlpha  * 0.25f)) +171.0f ,bgScaleOffsetY - (bgScaleOffsetY * (1.0f + PopupAlpha * 0.25f)));

	if ( ActivePopupID==ePopL1S5_P2FireLamp )
	{
		switch( Popup2_State )
		{
		case POPUP_SCALEUP:
			PopVal += POPUP_SCALE_FACTOR *  Control::LogicRate;
			PopupAlpha = easingQuadOut(PopVal,0,1,1.0f);
			//PopupAlpha += POPUP_SCALE_FACTOR *  Control::LogicRate;
			if( PopVal >= 1.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 1.0f;				
				Pop2FireLamp->Show(true);
				Popup2_State = POPUP_OPEN;
			}			
			//if( PopupAlpha < 0.5f )
				GFInstance->UpdateBlurScene(PopupAlpha>1.0f?1.0f:PopupAlpha);
			//else
				//GFInstance->UpdateBlurScene(1.0f);
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
			PopupAlpha -= 2.0f*POPUP_SCALE_FACTOR * Control::LogicRate;
			if( PopupAlpha <= 0.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 0.0f;								
				GFInstance->TurnOffBlurScene();
				Popup2_State = POPUP_CLOSED;
				ActivePopupID = ePopInactive;

				EndBgGraphicScaling();
			}		
			else
				GFInstance->UpdateBlurScene(PopupAlpha);
			break;
		} 
	}
	
}

float L1_S5_TreasureRoom::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

float L1_S5_TreasureRoom::easingQuadOut(float t,float b,float c,float d)
{
	return (c * -t*(t-2) / d + b);
}

void L1_S5_TreasureRoom::Render(const CPoint* pCam)
{  
	if( !Visible ) 
		return;

	static int FrameCnt = 0;
	FrameCnt = (FrameCnt+1)%2;

	if(ActivePopupID != ePopInactive)
	{
		if(IsPopupTransitionActive)
		{
			// render whole screen

            //Init again if render target lost due to fullscreen-window switches or similar
			if( GFInstance->RTGraphic != NULL )
				GFInstance->RTGraphic->blitAlphaRectFx(0,0,1366,768,
				short(GFInstance->PStartX+float(GFInstance->PEndX-GFInstance->PStartX)*PopupAlpha),
				short(GFInstance->PStartY+float(GFInstance->PEndY-GFInstance->PStartY)*PopupAlpha),
				-5*(1.0f-PopupAlpha),
				//0.2f,0.6f); //test gap between frames
				PopupAlpha,PopupAlpha>1.0f?1.0f:PopupAlpha);
        }
	}
}

void L1_S5_TreasureRoom::EndBgGraphicScaling()
{
	RT_BgScn->SetScale(1.0f);
	RT_BgScn->SetEnable(false);

    int size = (int) VFXArray.size();  //Sac: converted Implicit to explicit

	for (int i = 0; i < size; i++)		
		VFXArray[i]->Show(true);

	HOScene->PauseObjects(false);
}

void L1_S5_TreasureRoom::UpdateBgGraphic()
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
	RT_BgScn->pSprite->Image = GFInstance->CurrentSceneGraphic;
}

void  L1_S5_TreasureRoom::SetupPopupTransition(CHOScene *Popup, CRectangle popRect)
{

		HOScene->PauseObjects(true);

        int size = (int)VFXArray.size();  //Sac: converted Implicit to explicit

		for (int i = 0; i < size; i++)		
			VFXArray[i]->Show(false);
		
		bgScaleOffsetX = popRect.GetCenterX();
		bgScaleOffsetY = popRect.GetCenterY();
		
		RT_BgScn->SetEnable(true);

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

		if( Popup2_State == POPUP_SCALEUP)
			GFInstance->CreateGreyscaleScene();
}

L1_S5_TreasureRoom::~L1_S5_TreasureRoom()
{
	TaskPendingUpdate();
    int size = (int) VFXArray.size();  //Sac: converted Implicit to explicit

	for (int i = 0; i < size; i++){
		SAFE_DELETE(VFXArray[i]);
	}

	
	SAFE_DELETE(Pop2FireLamp);
	
	if( RT_BgScn!=NULL)
		RT_BgScn->pSprite->Image = NULL;
	SAFE_DELETE(RT_BgScn);

	//MUSIC_SFX
	Control::Audio->StopSample(aAmbS5TreasureRoom);
	Control::Audio->UnloadSample(aAmbS5TreasureRoom);
	Control::Audio->UnloadSample(aVOL1MaryRefS5_1);
	Control::Audio->UnloadSample(aVOL1MaryRefS5_2);
	//MUSIC_SFX
}

void L1_S5_TreasureRoom::TakeInventoryItem(EHOInventory ItemID)
{
	std::vector<int> vecInvIds;
	switch (ItemID)
	{
	case eInv_L1S5_BoatKey:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S5_BoatKey] = true;
			ProfileSystem::Instance->ActiveProfile.L1S5_InvBoatKeyTaken = true;			
			vecInvIds.push_back(ItemID);
			Hud->ShowInventory(&vecInvIds);
			isSparkleUpdatePending = true;
		break;
	case eInv_L1S5_HammerHead:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S5_HammerHead] = true;
			ProfileSystem::Instance->ActiveProfile.L1S5_InvHammerHeadTaken = true;			
			vecInvIds.push_back(ItemID);
			Hud->ShowInventoryDirect(&vecInvIds, Control::Input->Mpos());
			isSparkleUpdatePending = true;
		break;
	case eInv_L1S5_MortarPestle:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S5_MortarPestle] = true;
			ProfileSystem::Instance->ActiveProfile.L1S5_InvMortarPestleTaken = true;			
			vecInvIds.push_back(ItemID);
			Hud->ShowInventoryDirect(&vecInvIds, Control::Input->Mpos());
			isSparkleUpdatePending = true;
		break;
	default:
		break;
	}	
}

void L1_S5_TreasureRoom::ReceiveMessage(int val)
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
			if( itemVal == eInv_L1S3_MatchBox )
			{
				if( Popup2_State == POPUP_OPEN )
				{
					if( ProfileSystem::Instance->ActiveProfile.L1S5_InvBoatKeyTaken && !ProfileSystem::Instance->ActiveProfile.L1S5_IsTorchLight)
					{
						if(PopRectTorchFlaming.Intersection(&dropPos))
						{
							ProfileSystem::Instance->ActiveProfile.L1S5_IsTorchLight = true;

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5LampLite);

							isMatchBoxAnimPlaying = true;
							p2MatchBoxAnim->SetEnable(true);
							p2MatchBoxAnim->ForcePlayAnimation(0);
							Hud->ReleaseDrag();
							//Hud->CloseInventory();  
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
			else
				GFInstance->WrongInvDropMsg(Hud);
		}

		UpdateBgGraphic();
	}
	else if( val == INV_CLOSE )
	{		
		
	}	
}

void L1_S5_TreasureRoom::SparkleUpdate()
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

		if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsTorchLight)
		{
			sparklePoints[numSparkles].SetXY(PopRect2FireLamp.GetCenterX(), PopRect2FireLamp.GetCenterY());
			numSparkles++;
		}
		else 
		{
			if(!ProfileSystem::Instance->ActiveProfile.L1S5_InvMortarPestleTaken)
			{
				sparklePoints[numSparkles].SetXY(PopRect1WallSide.GetCenterX(), PopRect1WallSide.GetCenterY());
				numSparkles++;
			}

			if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsInvBombTaken && ProfileSystem::Instance->ActiveProfile.L1S5_InvHammerHeadTaken && ProfileSystem::Instance->ActiveProfile.L1S5_IsInvHammerHandTaken && ProfileSystem::Instance->ActiveProfile.L1S5_InvMortarPestleTaken)
			{
				sparklePoints[numSparkles].SetXY(SceneRectDynamitePuzzle.GetCenterX(), SceneRectDynamitePuzzle.GetCenterY());
				numSparkles++;
			}

			if(!ProfileSystem::Instance->ActiveProfile.L1S5_InvHammerHeadTaken)
			{
				sparklePoints[numSparkles].SetXY(SceneRectTunnelDoor.GetCenterX(), SceneRectTunnelDoor.GetCenterY());
				numSparkles++;
			}
			else if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsEscapedFromTunnel && ProfileSystem::Instance->ActiveProfile.L1S5_IsInvBombTaken )
			{
				sparklePoints[numSparkles].SetXY(SceneRectTunnelDoor.GetCenterX(), SceneRectTunnelDoor.GetCenterY());
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

void L1_S5_TreasureRoom::ProcessNonHOHint()
{
	int iNonHOHint = 0; 
	int MaxNonHOHints = 12;
	do
	{
		switch( iNonHOHint )
		{
			default:			
			//Exit coordinates
			case 0: ++iNonHOHint;
					/*if(!ProfileSystem::Instance->ActiveProfile.L1S5_InvHammerHeadTaken  && Popup1_State == POPUP_OPEN)
					{
						InitHintTrailEmit(InvItemRectHammerHead.GetCenterX(), InvItemRectHammerHead.GetCenterY(),false);
						return;
					}*/
					break;
			case 1: ++iNonHOHint;
					/*if(ProfileSystem::Instance->ActiveProfile.L1S5_InvHammerHeadTaken && !ProfileSystem::Instance->ActiveProfile.L1S5_InvMortarPestleTaken  && Popup1_State == POPUP_OPEN)
					{
						InitHintTrailEmit(InvItemRectSpoon.GetCenterX(), InvItemRectSpoon.GetCenterY(),false);
						return;
					}*/
					break;
			case 2: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S5_InvBoatKeyTaken && Popup2_State == POPUP_OPEN)
					{
						InitHintTrailEmit(InvItemRectBoatKey.GetCenterX(), InvItemRectBoatKey.GetCenterY(),false);
						return;
					}
					break;
			case 3: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S5_IsTorchLight && ProfileSystem::Instance->ActiveProfile.L1S5_InvBoatKeyTaken && Popup2_State == POPUP_OPEN)
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

							CPoint tempPoint (InvItemRectBoatKey.GetCenterX(), InvItemRectBoatKey.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						return;
					}
					break;
			case 4: ++iNonHOHint;
					if(Popup2_State == POPUP_OPEN)
					{
						InitHintTrailEmit(Pop_CloseButton->Pos.x, Pop_CloseButton->Pos.y,true);
						return;
					}
					break;
			case 5: ++iNonHOHint;
					/*if(!ProfileSystem::Instance->ActiveProfile.L1S5_InvHammerHeadTaken  && ProfileSystem::Instance->ActiveProfile.L1S5_IsTorchLight)
					{
						InitHintTrailEmit(PopRect1WallSide.GetCenterX(), PopRect1WallSide.GetCenterY(),false);
						return;
					}*/
					break;
			case 6: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S5_HOSolved && ProfileSystem::Instance->ActiveProfile.L1S5_IsTorchLight)
					{
						InitHintTrailEmit(PopRect1WallSide.GetCenterX(), PopRect1WallSide.GetCenterY(),false);
						return;
					}
					break;
			case 7: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S5_InvBoatKeyTaken )
					{
						InitHintTrailEmit(PopRect2FireLamp.GetCenterX(), PopRect2FireLamp.GetCenterY(),false);
						return;
					}
					break;
			case 8: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S5_IsTorchLight && ProfileSystem::Instance->ActiveProfile.L1S5_InvBoatKeyTaken )
					{
						InitHintTrailEmit(PopRect2FireLamp.GetCenterX(), PopRect2FireLamp.GetCenterY(),false);
						return;
					}
					break;
			case 9: ++iNonHOHint;
				if( !ProfileSystem::Instance->ActiveProfile.L1S5_IsInvHammerHandTaken )
					{
						InitHintTrailEmit(SceneRectTunnelDoor.GetCenterX(), SceneRectTunnelDoor.GetCenterY(),false);
						return;
					}
					break;
			case 10: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S5_IsInvBombTaken && ProfileSystem::Instance->ActiveProfile.L1S5_IsTorchLight )
					{
						InitHintTrailEmit(SceneRectDynamitePuzzle.GetCenterX(), SceneRectDynamitePuzzle.GetCenterY(),false);
						return;
					}
					break;
			case 11: ++iNonHOHint;
				if(ProfileSystem::Instance->ActiveProfile.L1S5_IsInvBombTaken )
					{
						InitHintTrailEmit(SceneRectTunnelDoor.GetCenterX(), SceneRectTunnelDoor.GetCenterY(),false);
						return;
					}
					break;
			
			
		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();
}

void L1_S5_TreasureRoom::NavigateToScene()
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

