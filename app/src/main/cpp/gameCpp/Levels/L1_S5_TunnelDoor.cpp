//====================================
//	DQFYLH
//	HO System
//	Level 1 - Scene 05 : TunnelDoor
//====================================

#include "L1_S5_TunnelDoor.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "Rect.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "Circle.h"
#include "AwardManager.h"

const CPoint	CenterPoint(683,334,0);
const CPoint	PopCloseRectBtnPoint(998,98,0);

const CRectangle  PopCloseButtonRect(968.0f,71.0f,62.0f,61.0f);
const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);

const CCircle	PopCircAreaBomb(700.0f,378.0f,170.0f);
// x +101 from center point, y -171 from center point
const CRectangle PopCloseCircBtnRect(801.0f,207.0f,66.0f,66.0f);
const CPoint	PopCloseCircBtnPoint(833,241.0f,0); //Add 133,-137 to center

const CRectangle  PopRectBomb(593.0f,212.0f,236.0f,261.0f);
const CRectangle  PopRectBomb2(700.0f,378.0f,5.0f,6.0f);

const CRectangle  PopRectBombFixArea(529.0f,168.0f,318.0f,328.0f);

const CRectangle  ActionRectTunnelEscape(380.0f,69.0f,659.0f,449.0f);

const CRectangle  InvDestRectDynamite(546.0f,215.0f,278.0f,259.0f);


L1_S5_TunnelDoor::L1_S5_TunnelDoor()
{
	Visible = false;
	isDustVFXPlaying = false;
	dustVFXtimer = 0;

	//Main Scene
	LoadHud(INVENTORY_V1);

	bomb = NULL;

	#ifdef _PARTICLEEDITORENABLED
		IsParticleEditorEnabled = false;
		if (IsParticleEditorEnabled)
		{
			_particleEditor = new ParticleEditorControl();
			AddControl(_particleEditor); 
		}
	#endif

	if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsTunnelDoorExploded)
	{
		LoadScene("LV1\\S5\\DOOR\\S5DOOR.SCN");
		bomb = HOScene->GetObjectByID("bomb");
		if(ProfileSystem::Instance->ActiveProfile.L1S5_IsInvHammerHandTaken)
			HOScene->GetObjectByID("HammerStick")->SetEnable(false);
	}
	else
	{
		LoadScene("LV1\\S5\\DOOR\\BROKEN.SCN");
		HOScene->GetObjectByID("FireAnim3")->SetScale(0.5f);
	}

	DustVFX = NULL;
	DustVFX = new SuperFX("PRT\\L1\\s5dustfall.INI", PointSystem::CreateCPoint(488,106,0), 100); 

	CPoint VFXPoint(488,106,0);
	VFXPosArray.push_back(VFXPoint);
	CPoint VFXPoint1(651,119,0);
	VFXPosArray.push_back(VFXPoint1);
	CPoint VFXPoint2(824,100,0);
	VFXPosArray.push_back(VFXPoint2);
	CPoint VFXPoint3(824,100,0);
	VFXPosArray.push_back(VFXPoint3);

	WallBrokenScene = NULL;

	if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsTunnelDoorExploded)
	{
		WallBrokenScene = new CHOScene();
		WallBrokenScene->Load("LV1\\S5\\DOOR\\BROKEN.SCN");
		WallBrokenScene->SetZOff(4);
		WallBrokenScene->GetObjectByID("FireAnim3")->SetScale(0.5f);
		WallBrokenScene->Show(false);
	}
	else
	{
		DustVFX->Start();
		isDustVFXPlaying = true;
	}

	
	InitPopups();

	if( GFInstance->availablePhyMem >= MAX_LOWEND_PEAKMEMORY )
	{

	}
	
	GFInstance->PostUpdate(2);

	zOrderScnVideo = 3;
	zOrderPopVideo = eZOrder_Popup + 5;

	IsVidSkippable = false;
	IsSkipMouseOver = false;
	IsSkipMouseClicked = false;
	SkipShowTimer = 0;

	skipObj = NULL;
	vidTimer = 0; vidTimer2 = 0;

	skipObj = new CObject();
	skipObj->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\CNM\\SKIPBTN.SPR", (SKIPIMAGE_XOFFSET_1366 + ((GFApp->ProjX<=0)?GFApp->ProjX:0)),SKIPIMAGE_Y, eZOrder_CutsceneFull+10);
	skipObj->Show(false);
	
	//Add Visible = true; to see anything
	SetZ(zOrderScnVideo);

	cv_DoorExplosion = NULL;
	cv_TunnelRunning = NULL;

	isDoorExplodeVideoPlaying = false;
	isTunnelRunVideoPlaying = false;

	//CRectangle RectFullVid(171, 96, 1024, 576);
	CRectangle RectFullVid(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
	if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsTunnelDoorExploded)
	{
		cv_DoorExplosion = new CVideo("OGV\\L1S5\\DOOREXPLOSION.OGV", RectFullVid, false, eZOrder_CutsceneFull);
		//cv_DoorExplosion->SetScale(1.334f);
		cv_DoorExplosion->SetScale(GFApp->video_aspectRatio);
	}

	if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsEscapedFromTunnel)
	{
		cv_TunnelRunning = new CVideo("OGV\\L1S5\\TUNNELESCAPE.OGV", RectFullVid, false, eZOrder_CutsceneFull);
		//cv_TunnelRunning->SetScale(1.334f);
		cv_TunnelRunning->SetScale(GFApp->video_aspectRatio);
	}
	
	//MUSIC_SFX
	Control::Audio->ClearUnusedTracks();

	if( ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite )
	{
		if( !Control::Audio->IsPlaying(aTrackL1EscapePlan) )
			Control::Audio->PlaySample(aTrackL1EscapePlan,true);
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL1Trapped) )
			Control::Audio->PlaySample(aTrackL1Trapped,true);
	}

	Control::Audio->LoadSample(aTrackCutscnS5TunnelEscape,AudioTrack);

	Control::Audio->LoadSample(aAmbS5TunnelDoor,AudioAmb);
	Control::Audio->PlaySample(aAmbS5TunnelDoor,true);
	Control::Audio->LoadSample(aVOL1MaryRefS5_3,AudioVO);
	//MUSIC_SFX

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L1_S5_TunnelDoor]  = true;

	if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S5_TunnelDoor])
		ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S5_TunnelDoor] = true;

	
	Visible = true;
}

void L1_S5_TunnelDoor::TaskPendingUpdate()
{
//	bool isAnyTaskLeft = false;  //Sac: Unused variable, so commented it
	//ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S5_TreasureRoom] = isAnyTaskLeft;
}

void L1_S5_TunnelDoor::InitPopups()
{
	ActivePopupID = ePopInactive;

	IsPopupTransitionActive = false;
	isBombLitAnimPlaying = false;

	PopVal = 0;
	PopupAlpha = 0;

	Pop_CloseButton = NULL;

	Popup2Bomb = NULL;

	if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsTunnelDoorExploded)
	{
		Popup2Bomb = new CHOScene();
		Popup2Bomb->Load("LV1\\S5\\DOOR\\POP2.SCN");

		//Add popup frame and close button
		CObject *popCirFrame = new CObject();
		popCirFrame->LoadInit("UI\\popcircframe.SPR", 4, -9, 1);
		popCirFrame->SetUID("popFrame");
		Popup2Bomb->PushObject(popCirFrame);

		CObject *popCirCloseBtn = new CObject();
		popCirCloseBtn->LoadInit("UI\\closebtn.SPR", 145, -158, 7);
		popCirCloseBtn->SetUID("popClose");
		Popup2Bomb->PushObject(popCirCloseBtn);

		p2Bomb = Popup2Bomb->GetObjectByID("p2Bomb");
		p2Thread = Popup2Bomb->GetObjectByID("p2Thread");
		p2BombLitAnim = Popup2Bomb->GetObjectByID("p2BombLitAnim");
		p2HammerHand =  Popup2Bomb->GetObjectByID("p2HammerHandle");
		p2BombLitAnim->SetEnable(false);

		if(ProfileSystem::Instance->ActiveProfile.L1S5_IsInvHammerHandTaken)
			p2HammerHand->SetEnable(false);
	
		if(!ProfileSystem::Instance->ActiveProfile.L1S5_POPBombFixed)
		{
			p2Bomb->SetEnable(false);
			p2Thread->SetEnable(false);
			bomb->Show(false);
		}
	
		Popup2Bomb->Show(false);
		Popup2Bomb->SetPosOff(PopCircAreaBomb.center.x, PopCircAreaBomb.center.y);
		Popup2Bomb->SetZOff(eZOrder_PopupOverPopup);
	}

	Popup1_State = POPUP_CLOSED;
}

void L1_S5_TunnelDoor::Required()
{
	HiddenObjectSystem::Required();	//forced update
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( IsPopupTransitionActive )
		UpdatePopupTransition();
}

void L1_S5_TunnelDoor::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive || Hud->IsMapShowing)
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	#ifdef _PARTICLEEDITORENABLED
		if (IsParticleEditorEnabled)
		{
			if(!_particleEditor->IsDisabled)
				return; 
		}
	#endif

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	if(isDoorExplodeVideoPlaying)
	{
		vidTimer += Control::LogicRate;

		if(
#ifndef FORCE_CLOSE_VID_END
			cv_DoorExplosion->isEndReached() ||
#endif
			vidTimer > 7)
		{
			HOScene->Show(false);
			WallBrokenScene->Show(true);
			Hud->AllowInput = true;
			Hud->ShowHud(false);

			cv_DoorExplosion->StopAndFadeOutVideo(2.0f);
						
			Hud->ShowBannerText("L1fast");
			Control::Audio->PlaySample(aVOL1MaryRefS5_3);

			//Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5ExplodeCough);
			isDoorExplodeVideoPlaying = false; 
			
			
			DustVFX->Start();
			isDustVFXPlaying = true;
		}
		return;
	}

	if(isTunnelRunVideoPlaying)
	{
		if( IsVidSkippable )
		{
			vidTimer2 += Control::LogicRate;

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
					cv_TunnelRunning->isEndReached() ||
#endif
				IsSkipMouseClicked || vidTimer2 > 8)
				{
					cv_TunnelRunning->StopVideo();
					Hud->AllowInput = true;
					Hud->ShowHud(false);

					ProfileSystem::Instance->ActiveProfile.L1S5_IsEscapedFromTunnel = true;

					Control::Audio->StopSample(aTrackCutscnS5TunnelEscape);

					//Update Map and TaskLeft
					ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L1_S1_Beach] = false;
					ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L1_S2_Frontyard] = false;
					ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L1_S5_TreasureRoom] = true;
					ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L1_S6_BoatShed] = false;
					ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L1_S7_BoatDock] = false;
					ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S5_TreasureRoom] = false;

					Control::Audio->QuickLoadAndPlaySFX(aSFXL1S2LovePopup);
					if( !Control::Audio->IsPlaying(aTrackL1EscapePlan) )
						 Control::Audio->PlaySample(aTrackL1EscapePlan,true);
					

					isTunnelRunVideoPlaying = false;
					AwardManager::GetInstance()->CompleteCreeksMansion();
					AwardManager::GetInstance()->CollectAward(kAward_TreasureRoom_Escape);
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S7_BoatDock;
					SendMessage(ControlExit, ID, HOSM_NewArea);
					return;
				}
			}
			else
				SkipShowTimer += 10*Control::LogicRate;
		}
		return;
	}

	if(isDustVFXPlaying)
	{
			dustVFXtimer += 10*Control::LogicRate;
			if(dustVFXtimer>= 10)
			{
				int randId = rand()%4;	
				DustVFX->SetPos(&VFXPosArray[randId]);
				dustVFXtimer = 0;
				DustVFX->Start();
			}
		
	}

	if(ProfileSystem::Instance->ActiveProfile.L1S5_IsTunnelDoorExploded)
	{
		if(ActionRectTunnelEscape.Intersection(&pos))
		{

#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
				//Cursor::SetMode(CA_ExitUp);
#else
			Cursor::SetMode(CA_ExitUp);
#endif
			if( Control::Input->LBclicked() )
			{
				if( Control::Audio->IsPlaying(aTrackL1Trapped) )
					Control::Audio->StopSample(aTrackL1Trapped);
				if( Control::Audio->IsPlaying(aTrackL1EscapePlan) )
					Control::Audio->StopSample(aTrackL1EscapePlan);
				if( Control::Audio->IsPlaying(aAmbS5TunnelDoor) )
					Control::Audio->StopSample(aAmbS5TunnelDoor);
				if( ProfileSystem::Instance->ActiveProfile.L1S5_IsEscapedFromTunnel )
				{
					if( !Control::Audio->IsPlaying(aTrackL1EscapePlan) )
						 Control::Audio->PlaySample(aTrackL1EscapePlan,true);

#ifdef TOUCH_UI
					if(!GFHud->InvBoxCtrl->navTapped)
					{
						GFHud->InvBoxCtrl->GoToScene(eArea_L1_S7_BoatDock, 0, ActionRectTunnelEscape);
					}
#else
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S7_BoatDock;
					SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

				}
				else
				{
					IsVidSkippable = true;
					SkipShowTimer = 0;

					Hud->EnableInput(false);
					Hud->HideHud(false);

					isTunnelRunVideoPlaying = true;
				
						cv_TunnelRunning->PlayFadeInVideo(2.0f);

					Control::Audio->PlaySample(aTrackCutscnS5TunnelEscape,false);
					
				}
				return;
			}
		}
	}
	

	if(isBombLitAnimPlaying)
	{
		Hud->EnableInput(false);
		Hud->HideHud(false);
		if(p2BombLitAnim->IsAnimationFinished())
		{
			Popup1_State = POPUP_SCALEDOWN;

			//Hud->EnableInput(true);
			//Hud->HideHud(true);
			Popup2Bomb->Show(false);
			SetupPopupTransition(Popup2Bomb, PopRectBomb2);
		}
		return;
	}

	if( ActivePopupID == ePopInactive  )
	{
		if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
			SparkleUpdate();
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
		{
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S5_TreasureRoom;
			SendMessage(ControlExit, ID, HOSM_NewArea);
			return;
		}
	#endif
		if(IsMouseOverGoBackArea(&pos ))
		{

#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
				//Cursor::SetMode(CA_ExitDown,eArea_L1_S5_TreasureRoom);
#else
			Cursor::SetMode(CA_ExitDown,eArea_L1_S5_TreasureRoom);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L1_S5_TreasureRoom, 2);
				}
#else
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S5_TreasureRoom;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}			
			return;
		}
		
	}

	if( !IsPopupTransitionActive )
		UpdatePopups(pos);
}

void L1_S5_TunnelDoor::UpdatePopupTransition()
{
	//Popup1 Eagle
	if( ActivePopupID == ePopL1S5_P2Bomb )
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
				Popup2Bomb->Show(true);
				Popup1_State = POPUP_OPEN;
			}			
			GFInstance->UpdateBlurScene(PopupAlpha);
			break;
		case POPUP_SCALEDOWN:
			
			PopupAlpha -= POPUP_SCALEDOWN_FACTOR * Control::LogicRate;
			if( PopupAlpha <= 0.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 0.0f;								
				GFInstance->TurnOffBlurScene();
				Popup1_State = POPUP_CLOSED;
				ActivePopupID = ePopInactive;

				if(isBombLitAnimPlaying)
				{
					isBombLitAnimPlaying = false;
					Hud->EnableInput(false);
					Hud->HideHud(false);

					cv_DoorExplosion->PlayFadeInVideo(4.0f);
					isDoorExplodeVideoPlaying = true;
					ProfileSystem::Instance->ActiveProfile.L1S5_IsTunnelDoorExploded = true;
				}
			}		
			else
				GFInstance->UpdateBlurScene(PopupAlpha);
			break;

		}
	}
}

void L1_S5_TunnelDoor::TakeInventoryItem(EHOInventory ItemID)
{
	std::vector<int> vecInvIds;

	switch (ItemID)
	{
	case eInv_L1S5_HammerHandle:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S5_HammerHandle] = true;
			ProfileSystem::Instance->ActiveProfile.L1S5_IsInvHammerHandTaken = true;			
			vecInvIds.push_back(eInv_L1S5_HammerHandle);
			Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
			isSparkleUpdatePending = true;
		break;
	
	default:
		break;
	}	
}

float L1_S5_TunnelDoor::easingQuadOut(float t,float b,float c,float d)
{
	return (c * -t*(t-2) / d + b);
}

void L1_S5_TunnelDoor::UpdatePopups(CPoint pos,bool ItemDropped)
{
	// bool clicked = Control::Input->LBclicked();  // Sac: Commenting Unused variable
	if ( (ActivePopupID == ePopInactive || ActivePopupID == ePopL1S5_P2Bomb) && !ProfileSystem::Instance->ActiveProfile.L1S5_IsTunnelDoorExploded )
	{
		switch( Popup1_State )
		{
		case POPUP_CLOSED:
			if( PopRectBomb.Intersection(&pos))
			{
				Cursor::SetMode(CA_Look);
				if( Control::Input->LBclicked() || ItemDropped)
				{
					ActivePopupID = ePopL1S5_P2Bomb;
					Popup1_State = POPUP_SCALEUP;

					Pop_CloseButton = Popup2Bomb->GetObjectByID("popClose");
					Pop_CloseButton->PlayAnimation(0);

					PopVal = 0;
					PopupAlpha = 0.0f;
					SetupPopupTransition(Popup2Bomb, PopRectBomb2);
				}
			}
			break;

		case POPUP_OPEN:
			if( Elixir::Distance(&pos,&PopCircAreaBomb.center) > 180 || Pop_CloseButton->Collide(&pos))//clicked outside
			{
				Pop_CloseButton->PlayAnimation(1);
				if(Pop_CloseButton->Collide(&pos))
					Cursor::SetMode(CA_HandPointy);
				if( Control::Input->LBclicked() )
				{					
					Pop_CloseButton->PlayAnimation(0);
					Popup1_State = POPUP_SCALEDOWN;

					Popup2Bomb->Show(false);
					SetupPopupTransition(Popup2Bomb, PopRectBomb2);
				}
				return;
			}
			else 
			{
				//pop close button handling
				Pop_CloseButton->PlayAnimation(0);

				if(PopRectBombFixArea.Intersection(&pos))
				{
					if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsInvHammerHandTaken)
					{
						Cursor::SetMode(CA_HandTake);
						if( Control::Input->LBclicked())
						{
							p2HammerHand->SetEnable(false);
							HOScene->GetObjectByID("HammerStick")->SetEnable(false);
							TakeInventoryItem(eInv_L1S5_HammerHandle);
							return;
						}
						
					}
					else
					{
						Cursor::SetMode(CA_Gear);
						if( Control::Input->LBclicked() )
						{
							if(!ProfileSystem::Instance->ActiveProfile.L1S5_POPBombFixed)
							{
								//Bomb Fixed
								Hud->ShowBannerText("L1solid");
							}
							else
							{
								//Matchbox light up
								Hud->ShowBannerText("L1light");
							}
						}
					}
				}
			}
		}
	}
	
}

void L1_S5_TunnelDoor::Render(const CPoint* pCam)
{  
	static int FrameCnt = 0;
	FrameCnt = (FrameCnt+1)%2;

	
	if (ActivePopupID != ePopInactive)
	{
		//No Videos Active when Popup is active

		if(IsPopupTransitionActive)
		{

			//Init again if render target lost due to fullscreen-window switches or similar
			if( GFInstance->RTGraphic != NULL )
				GFInstance->RTGraphic->blitAlphaRectFx(0,0,1366,768,
				short(GFInstance->PStartX+float(GFInstance->PEndX-GFInstance->PStartX)*PopupAlpha),
				short(GFInstance->PStartY+float(GFInstance->PEndY-GFInstance->PStartY)*PopupAlpha),
				-5*(1.0f-PopupAlpha),
				//0.2f,0.6f); //test gap between frames
				PopupAlpha,PopupAlpha);
			return;
            
        }
	}
}

L1_S5_TunnelDoor::~L1_S5_TunnelDoor()
{	
	
	

	SAFE_DELETE(DustVFX);

	SAFE_DELETE(WallBrokenScene);

	Pop_CloseButton = NULL;
	SAFE_DELETE(Popup2Bomb);

	SAFE_DELETE(skipObj);
	
	SAFE_DELETE(cv_DoorExplosion);
	SAFE_DELETE(cv_TunnelRunning);
	
	//MUSIC_SFX
	Control::Audio->UnloadSample(aTrackCutscnS5TunnelEscape);

	Control::Audio->StopSample(aAmbS5TunnelDoor);
	Control::Audio->UnloadSample(aAmbS5TunnelDoor);
	Control::Audio->UnloadSample(aVOL1MaryRefS5_3);
	//MUSIC_SFX
}

void L1_S5_TunnelDoor::ReceiveMessage(int val)
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
			
			if( itemVal == eInv_L1S5_Dynamite )
			{
				if( Popup1_State == POPUP_OPEN && PopRectBombFixArea.Intersection(&dropPos) && !ProfileSystem::Instance->ActiveProfile.L1S5_POPBombFixed && ProfileSystem::Instance->ActiveProfile.L1S5_IsInvHammerHandTaken)
				{
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 

					Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5BombPlaced);

					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S5_Dynamite] = false;

					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S5_Dynamite);

					p2Bomb->SetEnable(true);
					p2Thread->SetEnable(true);
					bomb->Show(true);

					ProfileSystem::Instance->ActiveProfile.L1S5_POPBombFixed = true;
					isSparkleUpdatePending = true;
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if( itemVal == eInv_L1S3_MatchBox )
			{
				if( Popup1_State == POPUP_OPEN && PopRectBombFixArea.Intersection(&dropPos)&& ProfileSystem::Instance->ActiveProfile.L1S5_POPBombFixed)
				{

					Hud->ReleaseDrag();
					Hud->CloseInventory(); 

					Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5BombExplode);

					//if matchbox needs to be removed
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S3_MatchBox] = false;
					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S3_MatchBox);

					p2Thread->SetEnable(false);
					p2BombLitAnim->SetEnable(true);
					p2BombLitAnim->ForcePlayAnimation(0);

					isBombLitAnimPlaying = true;
					ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite = true;
					if( Control::Audio->IsPlaying(aTrackL1Trapped))
						Control::Audio->StopSample(aTrackL1Trapped);
					Control::Audio->PlaySample(aTrackL1EscapePlan,false);
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

void L1_S5_TunnelDoor::SparkleUpdate()
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

		
			
			if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsInvHammerHandTaken )
			{
				sparklePoints[numSparkles].SetXY(PopRectBomb.GetCenterX(), PopRectBomb.GetCenterY());
				numSparkles++;
			}
			else if(!ProfileSystem::Instance->ActiveProfile.L1S5_POPBombFixed && ProfileSystem::Instance->ActiveProfile.L1S5_IsInvBombTaken )
			{
				sparklePoints[numSparkles].SetXY(PopRectBomb.GetCenterX(), PopRectBomb.GetCenterY());
				numSparkles++;
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

void L1_S5_TunnelDoor::ProcessNonHOHint()
{
	if(isDoorExplodeVideoPlaying || isTunnelRunVideoPlaying || isBombLitAnimPlaying)
		return;

	int iNonHOHint = 0; 
	int MaxNonHOHints = 9;
	do
	{
		switch( iNonHOHint )
		{
			default:			
			//Exit coordinates
			
			case 0: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S5_POPBombFixed  && ProfileSystem::Instance->ActiveProfile.L1S5_IsInvBombTaken && Popup1_State == POPUP_OPEN )
					{
						Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S5_Dynamite)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}

							CPoint tempPoint (PopRectBombFixArea.GetCenterX(), PopRectBombFixArea.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						return;
					}
					break;
			case 1: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite  && ProfileSystem::Instance->ActiveProfile.L1S5_POPBombFixed && ProfileSystem::Instance->ActiveProfile.L1S3_MatchBoxTaken && Popup1_State == POPUP_OPEN )
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

							CPoint tempPoint (PopRectBombFixArea.GetCenterX(), PopRectBombFixArea.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						return;
					}
					break;
			case 2: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsInvHammerHandTaken && Popup1_State == POPUP_OPEN )
					{
						InitHintTrailEmit(PopRectBombFixArea.GetCenterX(), PopRectBombFixArea.GetCenterY(),false);
						return;
					}
					break;
			case 3: ++iNonHOHint;
					if(Popup1_State == POPUP_OPEN)
					{
						InitHintTrailEmit(Pop_CloseButton->Pos.x, Pop_CloseButton->Pos.y,true);
						return;
					}
					break;
			case 4: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S5_POPBombFixed  && ProfileSystem::Instance->ActiveProfile.L1S5_IsInvBombTaken  )
					{
						InitHintTrailEmit(PopRectBomb.GetCenterX(), PopRectBomb.GetCenterY(),false);
						return;
					}
					break;
			case 5: ++iNonHOHint;
				   if(!ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite  && ProfileSystem::Instance->ActiveProfile.L1S5_POPBombFixed && ProfileSystem::Instance->ActiveProfile.L1S3_MatchBoxTaken )
					{
						InitHintTrailEmit(PopRectBomb.GetCenterX(), PopRectBomb.GetCenterY(),false);
						return;
					}
					break;
			case 6: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L1S5_IsInvHammerHandTaken  )
					{
						InitHintTrailEmit(PopRectBomb.GetCenterX(), PopRectBomb.GetCenterY(),false);
						return;
					}
					break;
			case 7: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite )
					{
						InitHintTrailEmit(PopRectBomb.GetCenterX(), PopRectBomb.GetCenterY(),false);
						return;
					}
					break;
			case 8: ++iNonHOHint;
					{
						InitHintTrailEmit(HoveredBackBtnRectMax.topleft.x + HoveredBackBtnRectMax.w/2 ,HoveredBackBtnRectMax.topleft.y + HoveredBackBtnRectMax.h/2,true,CA_ExitDown);
						return;
					}
					break;	
		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();
}

void  L1_S5_TunnelDoor::SetupPopupTransition(CHOScene *Popup, CRectangle popRect)
{
	IsPopupTransitionActive = true;
    if( GFInstance->IsRTCreated )
    {
		GFInstance->RTGraphic->clearRenderTarget();   
		GFInstance->RTGraphic->beginRenderToTarget();//RENDER TO TARGET STARTS	
		GFInstance->IsRenderingToRT = true;
    }
		Popup->RenderToTarget();
    if( GFInstance->IsRTCreated )
    {
		GFInstance->IsRenderingToRT = false;
		GFInstance->RTGraphic->endRenderToTarget();
    }
		GFInstance->SetPopupTween( (int)popRect.GetCenterX(-683) ,(int)popRect.GetCenterY(-384 ),0,0);

		GFInstance->CreateGreyscaleScene();

		SetZ(zOrderPopVideo);
}

void L1_S5_TunnelDoor::NavigateToScene()
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
