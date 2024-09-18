//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 01 :  UNDERGROUND HO
//====================================

#include "L2_S1_UndergroundHO.h"
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
#include "Fonts.h"

using namespace L2S1UndergroundHO;

enum L2S1HOObjects
{
	e_Spear = 0,
	e_DuckFigure,
	e_Box1_Duck,
	e_Box2_Rock,
	e_Box3_Lotus,
	e_Screw,
	e_Screwdriver,
	e_Star,
	e_Pentagon,
	e_Handle,
	e_Key,
	e_Stone
};

const CRectangle  Match3btnRect(559.0f,1.0f,265.0f,39.0f);
const CRectangle  LeftLeverRect(631.0f,153.0f,80.0f,100.0f);
const CRectangle  RightLeverRect(856.0f,158.0f,88.0f,101.0f);
const CRectangle  LockRect(797.0f,160.0f,50.0f,67.0f);

const CRectangle  KeyDoorLeverRect(509.0f,219.0f,54.0f,127.0f);

L2_S1_UndergroundHO::L2_S1_UndergroundHO()
{
	LoadHud(INVENTORY_V1);
	LoadScene("LV2\\S1\\HO\\L2S1HO.SCN");

	isAnimPlaying = false;
	musicFadeOutTimer = 0;
	
	InitSceneElements();
	SetSceneAnimations();

	
	AwardManager::GetInstance()->StartHO(ProfileSystem::Instance->ActiveProfile.CurrentLevel);

	//Match 3
	match3Ctrl  = NULL;
	ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

	//BtnMatch3 = HOScene->GetObjectByID("btnMatch3");
	//BtnSwitchToHO = HOScene->GetObjectByID("btnSwitchToHO");

	match3Back = new CObject();
	match3Back->LoadInit("MNU\\EXTRAS\\BACKBUTTON.SPR", 1266, 30, eZOrder_Popup + 300);
	match3Back->SetUID("match3Back");
	match3Back->Show(false);
	HOScene->PushObject(match3Back);

	match3BackText = new CObject();
	match3BackText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\BACK.SPR", 1250, 27, eZOrder_Popup + 301);
	match3BackText->SetUID("match3BackText");
	match3BackText->Show(false);
	HOScene->PushObject(match3BackText);
#ifndef M3_BUTTON
	match3Back->SetEnable(false);
    match3BackText->SetEnable(false);
#endif
	if (ProfileSystem::Instance->_isExtrasGamePlay)
	{
		match3Back->SetEnable(false);
		match3BackText->SetEnable(false);
	}

	BtnMatch3 = new CObject();
	BtnMatch3->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNMATCH3.SPR", 676, 25, 100);
	BtnMatch3->SetUID("btnMatch3");
	HOScene->PushObject(BtnMatch3);

	BtnSwitchToHO = new CObject();
	BtnSwitchToHO->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNSWITCHTOHO.SPR", 676, 25, 101);
	BtnSwitchToHO->SetUID("btnSwitchToHO");
	HOScene->PushObject(BtnSwitchToHO);

	BtnMatch3->SetZ(100);
	BtnSwitchToHO->SetZ(eZOrder_PopupOverPopup + 300);
	BtnSwitchToHO->Show(false);
	
	isBoxOpenAnimPlaying			= false;
	isHOBoxesAligned				= false;
	isHOBoxesPlaced					= false;
	
	isObjectInHand					= false;
	ObjectInHandID					= -1;
	SelectedHOBoxCount				= 0;
	SelectedHOBoxIndex[0]			= -1;
	SelectedHOBoxIndex[1]			= -1;
	TweenCount						= 0;

	HOInHandObjects[e_Spear]		= SpearOnClick;
	HOInHandObjects[e_DuckFigure]	= DuckFullOnClick;
	HOInHandObjects[e_Box1_Duck]			= BoxPlaced01;
	HOInHandObjects[e_Box2_Rock]			= BoxPlaced02;
	HOInHandObjects[e_Box3_Lotus]			= BoxPlaced03;
	HOInHandObjects[e_Screw]		= ScrewOnClick;
	HOInHandObjects[e_Screwdriver]	= Screwdriver0;
	HOInHandObjects[e_Star]			= StarOnClick;
	HOInHandObjects[e_Pentagon]		= PentagonOnClick;
	HOInHandObjects[e_Handle]		= DoorHandleOnClick;
	HOInHandObjects[e_Key]			= LeyInKeyOnClick;

	InitControlPolys();
	InitAnimations();

	InitHOItemsAndHUD();
	InitSceneHOObjects();

	if( ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[eHOItem3_BoxDuck] == HOITEM_USED &&
		ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[eHOItem4_BoxStone] == HOITEM_USED &&
		ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[eHOItem5_BoxUnderLotus] == HOITEM_USED)
	{
		if( !ProfileSystem::Instance->ActiveProfile.L2_S1_BoxOpened )
			isHOBoxesPlaced = true;
	}

	Hud->InvBoxCtrl->SetLockBase(HO_USEOBJECTS_FRAME);
	Hud->IsTakeAndUseHOScene = true;
	Hud->HOState = HOActive;

	TaskPendingUpdate();

	Control::Audio->LoadSample(aAmbs1Undeground,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbs1Undeground) )		
	{
		Control::Audio->PlaySample(aAmbs1Undeground,true);
	}
}

void L2_S1_UndergroundHO::InitSceneHOObjects()
{
	HOScnObjects[eHOItem1_DuckFigure]		= DuckFull;
	HOScnObjects[eHOItem2_Spear]			= Spear;
	HOScnObjects[eHOItem3_BoxDuck]			= Box1Mask;
	HOScnObjects[eHOItem4_BoxStone]			= BoxInsideRock;
	HOScnObjects[eHOItem5_BoxUnderLotus]	= BoxInsideVessel;
	HOScnObjects[eHOItem6_Screw]			= ScrewOnGround;
	HOScnObjects[eHOItem7_Screwdriver]		= screwdriverInScene;
	HOScnObjects[eHOItem8_StarKey]			= Star;
	HOScnObjects[eHOItem9_PentagonKey]		= pentagon;
	HOScnObjects[eHOItem10_PullHandle]		= DoorHandle;
	HOScnObjects[eHOItem11_KeyDoor]			= KeyInsideBox;
	HOScnObjects[eHOItem12_Stone]			= Rock02;

	for (int i = 0; i < MAX_L2S1_HOOBJECTS; i++)
	{
		if( ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[i] >= HOITEM_TAKEN )
			HOScnObjects[i]->SetEnable(false);
	}
}

void L2_S1_UndergroundHO::InitHOItemsAndHUD()
{
	//ToggleHOItemText(false);

	for (int i = 0; i < MAX_L2S1_HOOBJECTS; i++)
	{
		CObject * onHandItem = new CObject();
		std::string str = std::string("LV2\\S1\\HO\\INV\\onhanditem") + MKSTR(i) + std::string(".SPR");
		onHandItem->LoadInit(str, 100, 100, eZOrder_INVBOXITEMS+120, false);		
		onHandItem->SetScale(0.85f);

		SCN_ItemsOnHand[i] = onHandItem;
	}

	ItemRect_HOItemArray[0]= new CRectangle(243.0f,302.0f,106.0f,136.0f);
	ItemRect_HOItemArray[1]= new CRectangle(285.0f,112.0f,72.0f,154.0f);
	ItemRect_HOItemArray[2]= new CRectangle(260.0f,494.0f,84.0f,66.0f);
	ItemRect_HOItemArray[3]= new CRectangle(1054.0f,354.0f,72.0f,77.0f);
	ItemRect_HOItemArray[4]= new CRectangle(885.0f,482.0f,82.0f,71.0f);
	ItemRect_HOItemArray[5]= new CRectangle(615.0f,560.0f,95.0f,62.0f);
	ItemRect_HOItemArray[6]= new CRectangle(749.0f,463.0f,117.0f,46.0f);
	ItemRect_HOItemArray[7]= new CRectangle(397.0f,345.0f,89.0f,70.0f);
	ItemRect_HOItemArray[8]= new CRectangle(626.0f,350.0f,86.0f,66.0f);
	ItemRect_HOItemArray[9]= new CRectangle(711.0f,276.0f,115.0f,51.0f);
	ItemRect_HOItemArray[10]= new CRectangle(530.0f,355.0f,49.0f,63.0f);
	ItemRect_HOItemArray[11]= new CRectangle(1022.0f,430.0f,91.0f,80.0f);

	DestRect_HOItemArray[0]= new CRectangle(219.0f,409.0f,185.0f,195.0f);
	DestRect_HOItemArray[1]= new CRectangle(1026.0f,275.0f,158.0f,205.0f);
	DestRect_HOItemArray[2]= new CRectangle(646.0f,305.0f,326.0f,161.0f);
	DestRect_HOItemArray[3]= new CRectangle(649.0f,306.0f,324.0f,154.0f);
	DestRect_HOItemArray[4]= new CRectangle(647.0f,306.0f,325.0f,158.0f);
	DestRect_HOItemArray[5]= new CRectangle(478.0f,59.0f,131.0f,112.0f);
	DestRect_HOItemArray[6]= new CRectangle(479.0f,59.0f,179.0f,169.0f);
	DestRect_HOItemArray[7]= new CRectangle(347.0f,158.0f,195.0f,162.0f);
	DestRect_HOItemArray[8]= new CRectangle(1007.0f,114.0f,162.0f,156.0f);
	DestRect_HOItemArray[9]= new CRectangle(464.0f,199.0f,156.0f,227.0f);
	DestRect_HOItemArray[10]= new CRectangle(765.0f,128.0f,129.0f,123.0f);
	DestRect_HOItemArray[11]= new CRectangle(679.0f,98.0f,231.0f,232.0f);

	//initialize HO Objects and states for the first time
	if( ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[eHOItem1_DuckFigure] == HOITEM_LOCKED )
	{
		ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[eHOItem1_DuckFigure] = HOITEM_UNLOCKED;
		ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[eHOItem2_Spear] = HOITEM_UNLOCKED;
	}

	//HO Inv Box for UI Items
	const CRectangle  hoInventoryBoxRect(375.0f,645.0f,620.0f,139.0f);

	int numHOItemsInHUD = 5;

	HOInvBox = new HOInventoryBox(hoInventoryBoxRect, numHOItemsInHUD);
	HOInvBox->SetPosition(CPoint(HOInvBox->GetPosition().x, HOInvBox->GetPosition().y, eZOrder_HUD));
	for (int i = 0; i < eHOItem_MAX; i++)
	{
		std::string str;

		CObject * objItem = new CObject();
		str = std::string("LV2\\S1\\HO\\INV\\invitem") + MKSTR(i) + std::string(".SPR");
		objItem->LoadSprite(str);
		objItem->SetZ(100);

		CObject * objItemShadow = new CObject();
		str = std::string("LV2\\S1\\HO\\INV\\invshadow") + MKSTR(i) + std::string(".SPR");
		objItemShadow->LoadSprite(str);
		objItemShadow->SetZ(100);

		std::vector<int> collectSFXIds;
		collectSFXIds.push_back(aSFXInvCollectv1);
		collectSFXIds.push_back(aSFXInvCollectv2);
		collectSFXIds.push_back(aSFXInvCollectv3);
		InventoryItem * invItem = new InventoryItem(objItemShadow, objItem, i, CPoint(100,100, 100), this, "PRT\\L1\\HOTRAILFX.INI", collectSFXIds, aSFXInvWrongDrop, aSFXInvItemPick);
		HOInvBox->AddInventoryItem(invItem);
	}

	if (ProfileSystem::Instance->ActiveProfile.L2S1_HOHUDItemStates[0] == kInventoryItemState_NotLoaded)//checking first object is loaded
	{
		HOInvBox->LoadInventoryItemsToVisibleArea(0, numHOItemsInHUD);
	}
	else
	{
		HOInvBox->LoadInventoryItemStates(ProfileSystem::Instance->ActiveProfile.L2S1_HOHUDItemStates);
	}
}

void L2_S1_UndergroundHO::OnInventoryItemClick(int invItemType)
{
	//K_LOG("Item Clicked on %d", invItemType);
	if(isObjectInHand)
	{
		SCN_ItemsOnHand[ObjectInHandID]->SetEnable(false);
		isObjectInHand = false;
		HOInvBox->ReturnItem(ObjectInHandID, Control::Input->Mpos());
	}
	else
	{
		//SFX
		int randId = rand()%3;
		switch (randId)
		{
		case 0:		Control::Audio->QuickLoadAndPlaySFX(aSFXL2S2HOObjectClickv1);		break;
		case 1:		Control::Audio->QuickLoadAndPlaySFX(aSFXL2S2HOObjectClickv2);		break;
		case 2:		Control::Audio->QuickLoadAndPlaySFX(aSFXL2S2HOObjectClickv3);		break;
		default:
			break;
		}

		Cursor::Show(false);
		ObjectInHandID = invItemType;
		isObjectInHand = true;
		//SCN_ItemsOnHand[ObjectInHandID]->SetPos(&Control::Input->Mpos());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CPoint mousePos = Control::Input->Mpos();
		SCN_ItemsOnHand[ObjectInHandID]->SetPos(&mousePos);
		SCN_ItemsOnHand[ObjectInHandID]->SetEnable(true);
		Control::Input->ClearMouse();
	}
}

void L2_S1_UndergroundHO::UpdateInventoryChangesToProfile(int itemId)
{
	HOInvBox->UpdateInventoryChangesToProfile(ProfileSystem::Instance->ActiveProfile.L2S1_HOHUDItemStates, itemId);
}

void L2_S1_UndergroundHO::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Control::Audio->IsPlaying(aTrackL2theme) )
	{
		Control::Audio->Samples[aTrackL2theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL2theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL2theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL2ho) )
				  Control::Audio->PlaySample(aTrackL2ho, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else if( Control::Audio->IsPlaying(aTrackL2theme1) )
	{
		Control::Audio->Samples[aTrackL2theme1]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL2theme1]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL2theme1);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL2ho) )
				  Control::Audio->PlaySample(aTrackL2ho, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL2ho))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL2ho, true);
		}
	}
}

void L2_S1_UndergroundHO::Update()
{
	NavigateToScene();
	#ifdef _PARTICLEEDITORENABLED
		if (_isParticleEditorActive)
		{
			return;
		}
	#endif

	if( Hud->PopupState != Popup_Inactive || Hud->IsMapShowing || Hud->IsDiaryShowing)
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	Cursor::SetMode(CA_Normal);

	if(isAnimPlaying)
		return;

	CPoint pos = Control::Input->Mpos();

	if(!BtnMatch3->Visible)
	{
		match3Back->Show(true);
		match3BackText->Show(true);
	}
	else
	{
		match3Back->Show(false);
		match3BackText->Show(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L2_S1_InvColorPowderTaken)
	{
		match3Back->Show(false);
		match3BackText->Show(false);
		BtnMatch3->Show(false);
		BtnSwitchToHO->Show(false);
	}

	if(!ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{
		HOInvBox->HandleMouseInput(Control::Input);

		if(isObjectInHand)
		{
			SCN_ItemsOnHand[ObjectInHandID]->SetPos(&pos);

			if(Control::Input->RBclicked()) //problem with Touch devices
			{
				Cursor::Show(true);
				isObjectInHand = false;
				SCN_ItemsOnHand[ObjectInHandID]->SetEnable(false);
				HOInvBox->ReturnItem(ObjectInHandID, pos);
			}
			else if(Control::Input->LBclicked())
			{
				OnItemOnHandClickHandler(pos);				
			}
			return;
		}

		for (int i = 0; i < MAX_L2S1_HOOBJECTS; i++)
		{
			if(ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[i] == HOITEM_UNLOCKED && ItemRect_HOItemArray[i]->Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if( Control::Input->LBclicked() )
				{
					//vfx_collectFX->SetPos(&pos);
					//vfx_collectFX->Start();
					HOInvBox->CollectItem(i, pos);
					ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[i] = HOITEM_TAKEN;
					HOScnObjects[i]->SetEnable(false);
					SceneObjectsUpdate();
				}
				return;
			}
		}

		if( isHOBoxesPlaced  && !isHOBoxesAligned && !ProfileSystem::Instance->ActiveProfile.L2_S1_BoxOpened)
		{
			if (DestPolyArray[e_Box1_Duck]->Intersection(&pos) || DestPolyArray[e_Box2_Rock]->Intersection(&pos) || DestPolyArray[e_Box3_Lotus]->Intersection(&pos) )
			{
				Cursor::SetMode(CA_HandPointy);
				if( Control::Input->LBclicked() && SelectedHOBoxCount < 2 )
				{
					ShuffleHOBox( &pos);
				} 
				return;
			}
		}

		if(!isObjectInHand)
		{
			if(updateSceneInteractions(pos))
				return;
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
		UpdateBannerMessages(pos);

	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{
		if(match3Back->IsIntersect(pos) && !ProfileSystem::Instance->_isExtrasGamePlay)
		{
			if( Control::Input->LBclicked() )
			{
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S1_ForestEntrance;
				SendMessage(ControlExit, ID, HOSM_NewArea);
			}
		}

		if( match3Ctrl->isPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L2_S1_InvColorPowderTaken )
		{
			ProfileSystem::Instance->ActiveProfile.L2_S1_InvColorPowderTaken = true;
			Inventory->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L2S1_ColorPowder);
			ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
			match3Ctrl->clearBoard();
			BtnMatch3->Show(false);
			BtnSwitchToHO->Show(false);
			Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
			HandleHOComplete();
			TaskPendingUpdate();
			return;
		}


		if(Match3btnRect.Intersection(&pos) && !match3Ctrl->isGemAnimPlaying())
		{
			Cursor::SetMode(CA_HandPointy);
			BtnSwitchToHO->PlayAnimation(1);
			if( Control::Input->LBclicked() )
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXClick);

				HOInvBox->SetisEnabled(true);

				match3Ctrl->clearBoard();
				BtnMatch3->Show(true);
				BtnSwitchToHO->Show(false);
				BtnSwitchToHO->PlayAnimation(0);

				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

				Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
				return;
			}
		}
		else
		{
			BtnSwitchToHO->PlayAnimation(0);
		}
		return;
	}
	else
	{
		if(Match3btnRect.Intersection(&pos) &&!isAnimPlaying )
		{
			Cursor::SetMode(CA_HandPointy);
			BtnMatch3->PlayAnimation(1);
			if( Control::Input->LBclicked() )
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXClick);

				if(match3Ctrl == NULL)
				{
					match3Ctrl = new Match3(9);
					AddControl(match3Ctrl);
				}
				else
				{
					match3Ctrl->ShowBoard();
				}

				HOInvBox->SetisEnabled(false);

				BtnMatch3->Show(false);
				BtnMatch3->PlayAnimation(0);
				BtnSwitchToHO->Show(true);
				
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = true;

				Hud->InvBoxCtrl->SetLockBase(MATCH3_FRAME);
				return;
			}
		}
		else
		{
			BtnMatch3->PlayAnimation(0);
		}
	}

	//No animation playing & no objects in hand!! then we can backtrack!!
	if( !isAnimPlaying && !isObjectInHand ) 
	{
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
		{
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S1_ForestEntrance;
			SendMessage(ControlExit, ID, HOSM_NewArea);
			return;
		}
    #endif

		if( IsMouseOverGoBackArea(&pos)
			#ifdef M3_BUTTON
			&& BtnMatch3->Visible
			#endif
		)
		{

#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
			//	Cursor::SetMode(CA_ExitDown,eArea_L2_S1_ForestEntrance);
#else
			Cursor::SetMode(CA_ExitDown,eArea_L2_S1_ForestEntrance);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L2_S1_ForestEntrance, 2);
				}
#else
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S1_ForestEntrance;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

				return;
			}
		}
	}
}

bool L2_S1_UndergroundHO::updateSceneInteractions(CPoint pos)
{
	bool isAnyInteractionHandled = false;
	if(	MossPoly->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S1_PayalRemoved )
	{
		Cursor::SetMode(CA_HandTake);
		if(	Control::Input->LBclicked() )
		{
			Control::Audio->QuickLoadAndPlaySFX("s1ho_moseremove");

			Moss01->Status = SPR_VISIBLE;
			Moss02->Status = SPR_VISIBLE;
			Moss03->Status = SPR_VISIBLE;
			HOScene->GetObjectByID("lotus2")->Status = SPR_VISIBLE;
			HOScene->GetObjectByID("lotus1")->Status = SPR_VISIBLE;

			Moss01->TweenPosTo(-40, -30, -1, 1.5f, TW_EASEQUADIN, true);
			HOScene->GetObjectByID("lotus2")->TweenPosTo(-40, -30, -1, 1.5f, TW_EASEQUADIN, true);
			Moss02->TweenPosTo(45, -10, -1, 1.40f, TW_EASEQUADIN, true);
			Moss03->TweenPosTo(45, 40, -1, 1.60f, TW_EASEQUADIN, true);
			HOScene->GetObjectByID("lotus1")->TweenPosTo(45, 40, -1, 1.6f, TW_EASEQUADIN, true);

			ProfileSystem::Instance->ActiveProfile.L2_S1_PayalRemoved = true;

			ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[eHOItem5_BoxUnderLotus] = HOITEM_UNLOCKED;

			isAnyInteractionHandled = true;
		}
	}
	else if( DuckFullPoly->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S1_DuckOpened && ProfileSystem::Instance->ActiveProfile.L2_S1_DuckFixed )  
	{
		Cursor::SetMode(CA_HandTake);
		if(	Control::Input->LBclicked() )
		{
			Control::Audio->QuickLoadAndPlaySFX("s1ho_duckturn");

			DuckFull->SetEnable(false);
			DuckFullOnClick->SetEnable(false);
			Box1Mask->SetEnable(true);
			Box1Mask->SetZ(DuckHeadAnimation3->ZOrder + 2);
			DuckOpenAnim->PlayAnim();
			isAnimPlaying = true;
			ProfileSystem::Instance->ActiveProfile.L2_S1_DuckOpened = true;

			isAnyInteractionHandled = true;
		}
	}
	else if( KeyDoorLeverRect.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L2_S1_KeyDoorLeverFixed  && !ProfileSystem::Instance->ActiveProfile.L2_S1_SmallBoxDoorOpened)
	{
		Cursor::SetMode(CA_HandTake);
		if(	Control::Input->LBclicked() )
		{
			Control::Audio->QuickLoadAndPlaySFX("s5pz1_hatchmoved");
			ShelfOpenAnim->PlayAnim();
			isAnimPlaying = true;
			ProfileSystem::Instance->ActiveProfile.L2_S1_SmallBoxDoorOpened = true;
			
			isAnyInteractionHandled = true;
		}		
	}
	else if( DestPolyArray[e_Key]->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L2_S1_KeyFixed && !ProfileSystem::Instance->ActiveProfile.L2_S1_KeyTurned  )
	{
		Cursor::SetMode(CA_HandTake);
		if(	Control::Input->LBclicked() )
		{
			/*if(!(ProfileSystem::Instance->ActiveProfile.L2_S1_PentagonFixed && ProfileSystem::Instance->ActiveProfile.L2_S1_ScrewTightened && ProfileSystem::Instance->ActiveProfile.L2_S1_StarFixed))
			{
				Hud->ShowBannerText("L2stuck");
				return;
			}*/

			Control::Audio->QuickLoadAndPlaySFX("s1ho_keyturn");

			ProfileSystem::Instance->ActiveProfile.L2_S1_KeyTurned = true;
			KeyTurnAnim->PlayAnim();
			isAnimPlaying = true;

			isAnyInteractionHandled = true;
		}
	}
	else if( ProfileSystem::Instance->ActiveProfile.L2_S1_KeyTurned )
	{
		if( LeftLeverRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S1_LeftDoorOpened )
		{
			Cursor::SetMode(CA_HandTake);
			if(	Control::Input->LBclicked() )
			{
				if(ProfileSystem::Instance->ActiveProfile.L2_S1_StarFixed && ProfileSystem::Instance->ActiveProfile.L2_S1_StarKeyTurned)
				{
					Control::Audio->QuickLoadAndPlaySFX("s1ho_leverturndooropen");
					LeftLeverAnim->PlayAnim();
					LeftDoorOpenAnim->PlayAnim();
					isAnimPlaying = true;
					ProfileSystem::Instance->ActiveProfile.L2_S1_LeftDoorOpened = true;
					isAnyInteractionHandled = true;
				}
				else
				{
					LeftLock00->Shake(90, 1.25f,0,0.25f);
					Hud->ShowBannerText("L2stuck");
					isAnyInteractionHandled = true;
				}
			}
		}
		else if( RightLeverRect.Intersection(&pos) &&  !ProfileSystem::Instance->ActiveProfile.L2_S1_RightDoorOpened )
		{
			Cursor::SetMode(CA_HandTake);
			if(	Control::Input->LBclicked() )
			{
				if(ProfileSystem::Instance->ActiveProfile.L2_S1_PentagonFixed && ProfileSystem::Instance->ActiveProfile.L2_S1_PentagonKeyTurned)
				{
					Control::Audio->QuickLoadAndPlaySFX("s1ho_leverturndooropen");
					RightLeverAnim->PlayAnim();
					RightDoorOpenAnim->PlayAnim();
					KeyInKeyHole2->FadeOut(3.0f);
					isAnimPlaying = true;
					ProfileSystem::Instance->ActiveProfile.L2_S1_RightDoorOpened = true;
					isAnyInteractionHandled = true;
				}
				else
				{
					RightLock01->Shake(90, 1.25f,0,0.25f);
					Hud->ShowBannerText("L2stuck");
					isAnyInteractionHandled = true;
				}
			}
		}
		else if( ProfileSystem::Instance->ActiveProfile.L2_S1_MirrorRemoved && !ProfileSystem::Instance->ActiveProfile.L2_S1_InvColorPowderTaken && GlassPoly->Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if( Control::Input->LBclicked())
			{
				ProfileSystem::Instance->ActiveProfile.L2_S1_InvColorPowderTaken = true;
				Inventory->SetEnable(false);
				Hud->TakeInventoryItem(eInv_L2S1_ColorPowder, Control::Input->Mpos());
				TaskPendingUpdate();

				isAnyInteractionHandled = true;
			}
		}
	}

	return isAnyInteractionHandled;
}

bool L2_S1_UndergroundHO::CheckIfItemCanBeUsed()
{
	bool canBeUsed = true;

	switch (ObjectInHandID)
	{
	case L2S1UndergroundHO::eHOItem1_DuckFigure:
		break;
	case L2S1UndergroundHO::eHOItem2_Spear:
		break;
	case L2S1UndergroundHO::eHOItem3_BoxDuck:
		break;
	case L2S1UndergroundHO::eHOItem4_BoxStone:
		break;
	case L2S1UndergroundHO::eHOItem5_BoxUnderLotus:
		break;
	case L2S1UndergroundHO::eHOItem6_Screw:		
		break;
	case L2S1UndergroundHO::eHOItem7_Screwdriver:
		{
			if(!ProfileSystem::Instance->ActiveProfile.L2_S1_ScrewFixed)
			{
				canBeUsed = false;
				Hud->ShowBannerText("L2screw");
			}
		}
		break;
	case L2S1UndergroundHO::eHOItem8_StarKey:
		break;
	case L2S1UndergroundHO::eHOItem9_PentagonKey:
		break;
	case L2S1UndergroundHO::eHOItem10_PullHandle:
		{
			if( !ProfileSystem::Instance->ActiveProfile.L2_S1_BoxOpened)
			{
				canBeUsed = false;
				GFInstance->WrongInvDropMsg(Hud);
			}
		}
		break;
	case L2S1UndergroundHO::eHOItem11_KeyDoor:
		break;
	case L2S1UndergroundHO::eHOItem12_Stone:
		{
			if( !(ProfileSystem::Instance->ActiveProfile.L2_S1_LeftDoorOpened && ProfileSystem::Instance->ActiveProfile.L2_S1_RightDoorOpened) )
			{
				canBeUsed = false;
				Hud->ShowBannerText("L2risk");
			}
		}
		break;
	
	default:
		break;
	}

	return canBeUsed;
}

void L2_S1_UndergroundHO::OnItemOnHandClickHandler(CPoint pos)
{
	int BoxPosition = -1;
	if( ObjectInHandID == eHOItem3_BoxDuck ||  ObjectInHandID == eHOItem4_BoxStone || ObjectInHandID == eHOItem5_BoxUnderLotus )
	{
		
		if (DestPolyArray[e_Box1_Duck]->Intersection(&pos) )
			BoxPosition = e_Box1_Duck;
		else if(DestPolyArray[e_Box2_Rock]->Intersection(&pos))
			BoxPosition = e_Box2_Rock;
		else if(DestPolyArray[e_Box3_Lotus]->Intersection(&pos))
			BoxPosition = e_Box3_Lotus;

		if(BoxPosition == -1 )
		{
			GFInstance->WrongInvDropMsg(Hud);
			Cursor::Show(true);
			isObjectInHand = false;		
			SCN_ItemsOnHand[ObjectInHandID]->SetEnable(false);
			HOInvBox->ReturnItem(ObjectInHandID, pos);
			return;
		}
		int positionIndex = -1;
		switch (BoxPosition)
		{
		case e_Box1_Duck:			positionIndex = 0;
			break;
		case e_Box2_Rock:			positionIndex = 1;
			break;
		case e_Box3_Lotus:			positionIndex = 2;
			break;
		default:
			break;
		}
		if( ProfileSystem::Instance->ActiveProfile.L2_S1_BoxSelectedIndex[positionIndex] != -1 )
		{
			for (int i = 0; i < 3; i++)
			{
				if(ProfileSystem::Instance->ActiveProfile.L2_S1_BoxSelectedIndex[i] == -1 )
				{
					switch (i)
					{
					case 0:			BoxPosition = e_Box1_Duck;
						break;
					case 1:			BoxPosition = e_Box2_Rock;
						break;
					case 2:			BoxPosition = e_Box3_Lotus;
						break;
					default:
						break;
					}
				} 
			}
		}
	}

	if(!DestRect_HOItemArray[ObjectInHandID]->Intersection(&pos))
	{
		GFInstance->WrongInvDropMsg(Hud);
		Cursor::Show(true);
		isObjectInHand = false;		
		SCN_ItemsOnHand[ObjectInHandID]->SetEnable(false);
		HOInvBox->ReturnItem(ObjectInHandID, pos);
		return;
	}

	if(!CheckIfItemCanBeUsed())
	{
		Cursor::Show(true);
		isObjectInHand = false;		
		SCN_ItemsOnHand[ObjectInHandID]->SetEnable(false);
		HOInvBox->ReturnItem(ObjectInHandID, pos);
		return;
	}

	switch (ObjectInHandID)
	{
	case L2S1UndergroundHO::eHOItem1_DuckFigure:
		{
			ProfileSystem::Instance->ActiveProfile.L2_S1_DuckFixed = true;
			DuckFullOnClick->SetEnable(false);
			DuckBottom->SetEnable(true);
			DuckHeadAnimation2->SetEnable(true);
			Control::Audio->QuickLoadAndPlaySFX("s1ho_duckfix");
		}
		break;
	case L2S1UndergroundHO::eHOItem2_Spear:
		{
			ProfileSystem::Instance->ActiveProfile.L2_S1_SpearUsed = true;
			SpearRockAnim->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s1ho_spearused");
			isAnimPlaying = true;
		}
		break;
	case L2S1UndergroundHO::eHOItem3_BoxDuck:
		{
			Control::Audio->QuickLoadAndPlaySFX("s1ho_tileplaced");
			ProfileSystem::Instance->ActiveProfile.L2_S1_Box1Fixed = true;
			SetHOBoxAtPosition( eHOItem3_BoxDuck, BoxPosition);
		}
		break;
	case L2S1UndergroundHO::eHOItem4_BoxStone:
		{
			Control::Audio->QuickLoadAndPlaySFX("s1ho_tileplaced");
			ProfileSystem::Instance->ActiveProfile.L2_S1_Box2Fixed = true;
			SetHOBoxAtPosition( eHOItem4_BoxStone, BoxPosition);
		}
		break;
	case L2S1UndergroundHO::eHOItem5_BoxUnderLotus:
		{
			Control::Audio->QuickLoadAndPlaySFX("s1ho_tileplaced");
			ProfileSystem::Instance->ActiveProfile.L2_S1_Box3Fixed = true;
			SetHOBoxAtPosition( eHOItem5_BoxUnderLotus, BoxPosition);
		}
		break;
	case L2S1UndergroundHO::eHOItem6_Screw:
		{
			ProfileSystem::Instance->ActiveProfile.L2_S1_ScrewFixed = true;
			//LeftScrew->SetEnable(true);
			Control::Audio->QuickLoadAndPlaySFX("s1ho_screwfixed");
			ScrewFit->SetEnable(true);
			ScrewMask->SetEnable(true);
		}
		break;
	case L2S1UndergroundHO::eHOItem7_Screwdriver:
		{
			ScrewTightenAnim->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s1ho_screwdriveruse");
			LeftScrew->SetEnable(false);
			isAnimPlaying = true;
			ProfileSystem::Instance->ActiveProfile.L2_S1_ScrewTightened = true;
		}
		break;
	case L2S1UndergroundHO::eHOItem8_StarKey:
		{
			ProfileSystem::Instance->ActiveProfile.L2_S1_StarFixed = true;
			StarFixed->SetEnable(true);
			if( ProfileSystem::Instance->ActiveProfile.L2_S1_ScrewTightened ) 
			{
				Control::Audio->QuickLoadAndPlaySFX("s1ho_pentagonfix");

				StarTurned->SetEnable(true);
				StarTurned->FadeIn(1.5f);

				ProfileSystem::Instance->ActiveProfile.L2_S1_StarKeyTurned = true;
				LeftLockAnim->PlayAnim();
				isAnimPlaying = true;
			}
		}
		break;
	case L2S1UndergroundHO::eHOItem9_PentagonKey:
		{
			Control::Audio->QuickLoadAndPlaySFX("s1ho_pentagonfix");
			ProfileSystem::Instance->ActiveProfile.L2_S1_PentagonFixed= true;
			PentagonFixed->SetEnable(true);

			pentagonTurned->SetEnable(true);
			pentagonTurned->FadeIn(1.5f);

			ProfileSystem::Instance->ActiveProfile.L2_S1_PentagonKeyTurned = true;
			RightLockAnim->PlayAnim();
			isAnimPlaying = true;
		}
		break;
	case L2S1UndergroundHO::eHOItem10_PullHandle:
		{
			if( ProfileSystem::Instance->ActiveProfile.L2_S1_BoxOpened && !ProfileSystem::Instance->ActiveProfile.L2_S1_SmallBoxDoorOpened && !isBoxOpenAnimPlaying)
			{
				Control::Audio->QuickLoadAndPlaySFX("s5pz1_hatchhandlefixed");
				ProfileSystem::Instance->ActiveProfile.L2_S1_KeyDoorLeverFixed = true;
				KeyBoxLeverbase->SetEnable(false);
				KeyBoxLeverFixed->SetEnable(true);
			}
		}
		break;
	case L2S1UndergroundHO::eHOItem11_KeyDoor:
		{
			Control::Audio->QuickLoadAndPlaySFX("s1ho_keyinsert");
			ProfileSystem::Instance->ActiveProfile.L2_S1_KeyFixed = true;

			KeyMask->SetEnable(true);
			KeyFixAnim->PlayAnim();
			isAnimPlaying = true;
		}
		break;
	case L2S1UndergroundHO::eHOItem12_Stone:
		{
			if( ProfileSystem::Instance->ActiveProfile.L2_S1_LeftDoorOpened && ProfileSystem::Instance->ActiveProfile.L2_S1_RightDoorOpened )
			{
				Control::Audio->QuickLoadAndPlaySFX("s1ho_mirroebreak");

				ProfileSystem::Instance->ActiveProfile.L2_S1_MirrorRemoved = true;
				MirrorRemoveAnim->PlayAnim();
				isAnimPlaying = true;
				HandleHOComplete();
				AwardManager::GetInstance()->CompleteHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			}
		}
		break;
	
	default:
			return;
		break;
	}

	Cursor::Show(true);
	isObjectInHand = false;
	SCN_ItemsOnHand[ObjectInHandID]->SetEnable(false);
	ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[ObjectInHandID] = HOITEM_USED;

	int nextInvID = HOInvBox->GetNextInventoryItemID();
	if(nextInvID!=-1)
	{
		if(nextInvID == eHOItem6_Screw || nextInvID == eHOItem7_Screwdriver || nextInvID == eHOItem10_PullHandle || nextInvID == eHOItem12_Stone)
			ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[nextInvID] = HOITEM_UNLOCKED;
		else if(nextInvID == eHOItem8_StarKey || nextInvID == eHOItem9_PentagonKey)
		{
			if(ProfileSystem::Instance->ActiveProfile.L2_S1_BoxOpened)
				ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[nextInvID] = HOITEM_UNLOCKED;
		}
	}

	HOInvBox->UnloadInventoryItemFromVisibleArea(ObjectInHandID);
	HOInvBox->LoadNextInventoryItemToVisibleArea();
}

L2_S1_UndergroundHO::~L2_S1_UndergroundHO()
{
	Hud->InvBoxCtrl->SetLockBase(INVBOX_LR_FRAME);
	Hud->IsTakeAndUseHOScene = false;


	ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
	SAFE_DELETE(DuckFullPoly);
	SAFE_DELETE(GlassPoly);
	SAFE_DELETE(BoxPolyArray);
	SAFE_DELETE(MossPoly);

	SAFE_DELETE(s1holeavesFX);
	SAFE_DELETE(s1hoBrokenGlassFX);
	SAFE_DELETE(s2horippleFX);

	SAFE_DELETE(s1hoflies1FX);
	SAFE_DELETE(s1hoflies2FX);
	
	SAFE_DELETE(HOInvBox);

	for (int i = 0; i < MAX_L2S1_HOOBJECTS; i++)
	{
		SAFE_DELETE(SCN_ItemsOnHand[i]);
		SAFE_DELETE(ItemRect_HOItemArray[i]);
		SAFE_DELETE(DestRect_HOItemArray[i]);
	}

	TaskPendingUpdate();
	for (int i = 0; i < MAX_L2S1_HOOBJECTS; i++)
	{		
		SAFE_DELETE(DestPolyArray[i]);
	}
	SAFE_DELETE(DuckOpenAnim);
	SAFE_DELETE(SpearRockAnim);
	SAFE_DELETE(KeyTurnAnim);
	SAFE_DELETE(LeftLockAnim);
	SAFE_DELETE(LeftLeverAnim);
	SAFE_DELETE(RightLeverAnim);
	SAFE_DELETE(RightLockAnim);
	SAFE_DELETE(LeftDoorOpenAnim);
	SAFE_DELETE(RightDoorOpenAnim);
	SAFE_DELETE(ScrewTightenAnim);
	SAFE_DELETE(BoxOpenAnim);
	SAFE_DELETE(ShelfOpenAnim);
	SAFE_DELETE(MirrorRemoveAnim);

	//MUSIC_SFX


	Control::Audio->StopSample(aAmbs1Undeground);
	Control::Audio->UnloadSample(aAmbs1Undeground);
	//MUSIC_SFX
}

void L2_S1_UndergroundHO::SceneObjectsUpdate()
{

}

void L2_S1_UndergroundHO::SparkleUpdate()
{
	
}

void L2_S1_UndergroundHO::ReceiveMessage(int val)
{	
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == INVBOX_DROP ) 
	{

	}
	else if( val == INV_CLOSE )
	{		
		if( ProfileSystem::Instance->ActiveProfile.L2_S1_InvColorPowderTaken )
		{
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S1_ForestEntrance;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
		}
	}	
}

bool L2_S1_UndergroundHO::CheckIfInvItemCanBeUsed(int hoItemIndex)
{
	bool canBeUsed = true;

	switch (hoItemIndex)
	{
	case L2S1UndergroundHO::eHOItem1_DuckFigure:
		break;
	case L2S1UndergroundHO::eHOItem2_Spear:
		break;
	case L2S1UndergroundHO::eHOItem3_BoxDuck:
		break;
	case L2S1UndergroundHO::eHOItem4_BoxStone:
		break;
	case L2S1UndergroundHO::eHOItem5_BoxUnderLotus:
		break;
	case L2S1UndergroundHO::eHOItem6_Screw:		
		break;
	case L2S1UndergroundHO::eHOItem7_Screwdriver:
		{
			if(!ProfileSystem::Instance->ActiveProfile.L2_S1_ScrewFixed)
			{
				canBeUsed = false;
			}
		}
		break;
	case L2S1UndergroundHO::eHOItem8_StarKey:
		break;
	case L2S1UndergroundHO::eHOItem9_PentagonKey:
		break;
	case L2S1UndergroundHO::eHOItem10_PullHandle:
		{
			if( !ProfileSystem::Instance->ActiveProfile.L2_S1_BoxOpened)
			{
				canBeUsed = false;
			}
		}
		break;
	case L2S1UndergroundHO::eHOItem11_KeyDoor:
		break;
	case L2S1UndergroundHO::eHOItem12_Stone:
		{
			if( !(ProfileSystem::Instance->ActiveProfile.L2_S1_LeftDoorOpened && ProfileSystem::Instance->ActiveProfile.L2_S1_RightDoorOpened) )
			{
				canBeUsed = false;
			}
		}
		break;
	
	default:
		break;
	}

	return canBeUsed;
}

void L2_S1_UndergroundHO::ProcessNonHOHint()
{
	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing )
		return;

	int iNonHOHint = 0; 
	int MaxNonHOHints = 22;
	do
	{
        AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea); // why use this anyway here, no assignment
		switch( iNonHOHint )
		{
                // Sac: why this line below here in switch case ? it is not going to do anything good.
			 //AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			default:			
			//Exit coordinates
			case 0: ++iNonHOHint;
					//0 show hint for inventory Item
					if(ProfileSystem::Instance->ActiveProfile.L2_S1_MirrorRemoved && !ProfileSystem::Instance->ActiveProfile.L2_S1_InvColorPowderTaken )
					{
						InitHintTrailEmit(DestPolyArray[e_Stone]->center.x, DestPolyArray[e_Stone]->center.y,true);
						return;
					}
					break;
			case 1: ++iNonHOHint;
					// Show Hint for Box Swapping
					if( isHOBoxesPlaced && !ProfileSystem::Instance->ActiveProfile.L2_S1_BoxOpened )
					{
						ShowHintForHOBoxSwap();
						return;
					}
					break;
			case 2: ++iNonHOHint;
					//2 Show Hint for Scene Interactions
					if(CheckAndShowHintForSceneInteractions())
					{
						return;
					}
					break;
			case 3: ++iNonHOHint;
					//1 Show Hint for HUD Items first
					for (int i = 0; i < MAX_L2S1_HOOBJECTS; i++)
					{
						if(HOInvBox->GetInventoryItemStateOf(i) == kInventoryItemState_Collected)
						{
							if(CheckIfInvItemCanBeUsed(i))
							{
								CPoint invItemPos(HOInvBox->GetInventoryItem(i)->GetX(),HOInvBox->GetInventoryItem(i)->GetY(),0);
								CPoint destPoint(DestRect_HOItemArray[i]->GetCenterX(), DestRect_HOItemArray[i]->GetCenterY(), 0);

								InitHintTrailEmit(&invItemPos);
								Hud->ShowHintForSceneItem(&destPoint);
								return;
							}							
						}
					}
					break;
			case 4: ++iNonHOHint;
					//2 Show Hint for Scene Take Objects
					for (int i = 0; i < MAX_L2S1_HOOBJECTS; i++)
					{
						if(ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[i] == HOITEM_UNLOCKED && HOInvBox->GetInventoryItemStateOf(i) == kInventoryItemState_NotCollected)
						{							
							CPoint destPoint(ItemRect_HOItemArray[i]->GetCenterX(), ItemRect_HOItemArray[i]->GetCenterY(), 0);

							InitHintTrailEmit(&destPoint);
							return;						
						}
					}
					break;
			case 5: ++iNonHOHint;
					break;
			case 6: ++iNonHOHint;
					break;
			case 7: ++iNonHOHint;
					break;
			case 8: ++iNonHOHint;
					break;
			case 9: ++iNonHOHint;
					break;
			case 10: ++iNonHOHint;
					break;
			case 11: ++iNonHOHint;
					break;
			case 12: ++iNonHOHint;
					break;
			case 13: ++iNonHOHint;
					break;
			case 14: ++iNonHOHint;
					break;
			case 15: ++iNonHOHint;
					break;
			case 16: ++iNonHOHint;
					break;
			case 17: ++iNonHOHint;
					break;
			case 18: ++iNonHOHint;
					break;
			case 19: ++iNonHOHint;
					
					break;
			case 20: ++iNonHOHint;
					//3 show hint to Forest Just In Case
					if( ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S1_ForestEntrance] )
					{
						InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
						return;
					}
					break;		
			case 21: ++iNonHOHint;
					{
						if(Hud->ShowHintForComboInvItems())
							return;
					}
			case 22: ++iNonHOHint;
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

void L2_S1_UndergroundHO::ShowHintForHOBoxSwap()
{
	for (int i = 0; i < 3; i++)
	{
		if( ProfileSystem::Instance->ActiveProfile.L2_S1_BoxSelectedIndex[i] != i )
		{
			Hud->ShowHintForSceneItem(&DestPolyArray[2 + i]->center);
			int _index = ProfileSystem::Instance->ActiveProfile.L2_S1_BoxSelectedIndex[i];
			InitHintTrailEmit(BoxAtPos[_index]->GetCenterX(), BoxAtPos[_index]->GetCenterY(), 0 );
			return;
		} 
	}
}

bool L2_S1_UndergroundHO::CheckAndShowHintForSceneInteractions()
{
	bool isHintShown = false;

	if(	!ProfileSystem::Instance->ActiveProfile.L2_S1_PayalRemoved )
	{
		InitHintTrailEmit(&MossPoly->center);
		isHintShown = true;		
	}
	else if( !ProfileSystem::Instance->ActiveProfile.L2_S1_DuckOpened && ProfileSystem::Instance->ActiveProfile.L2_S1_DuckFixed )  
	{
		InitHintTrailEmit(&DuckFullPoly->center);
		isHintShown = true;
	}
	else if( ProfileSystem::Instance->ActiveProfile.L2_S1_KeyDoorLeverFixed  && !ProfileSystem::Instance->ActiveProfile.L2_S1_SmallBoxDoorOpened)
	{
		InitHintTrailEmit(&DestPolyArray[e_Handle]->center);
		isHintShown = true;
	}
	else if( ProfileSystem::Instance->ActiveProfile.L2_S1_KeyFixed && !ProfileSystem::Instance->ActiveProfile.L2_S1_KeyTurned )
	{
		InitHintTrailEmit(&DestPolyArray[e_Key]->center);
		isHintShown = true;
	}
	else if( ProfileSystem::Instance->ActiveProfile.L2_S1_KeyTurned )
	{
		if( !ProfileSystem::Instance->ActiveProfile.L2_S1_LeftDoorOpened && ProfileSystem::Instance->ActiveProfile.L2_S1_StarKeyTurned)
		{
			InitHintTrailEmit(&LeftLeverRect);
			isHintShown = true;
		}
		else if( !ProfileSystem::Instance->ActiveProfile.L2_S1_RightDoorOpened && ProfileSystem::Instance->ActiveProfile.L2_S1_PentagonKeyTurned)
		{
			InitHintTrailEmit(&RightLeverRect);
			isHintShown = true;
		}
	}

	return isHintShown;
}

void L2_S1_UndergroundHO::SetSceneAnimations()
{
	LightAnimation->SetBlendMode(eBlend_AlphaAdditive);
	LightAnimation->Animate(0.45f, 1.0f, 3.5f);

	HOScene->GetObjectByID("LeftLight")->SetScale(2.0f);
	HOScene->GetObjectByID("LeftLight")->Animate(0.35f, 1.0f, 4.5f,0, eBlend_AlphaAdditive);

	Plant0->SetPivot(-66, 109);
	Plant0->Animate(0,0,3,0,4.0f);	

	Plant1->SetPivot(-66, 109);
	Plant1->Animate(0,0,2,0,5.0f);

	RootAnimation1->SetPivot(0, 0);
	RootAnimation1->Animate(1,4,0,0,9.0f);

	RootAnimation3->SetPivot(0, 0);
	RootAnimation3->Animate(1,4,0,0,7.0f);	

	RootAnimation2->SetPivot(0, 0);
	RootAnimation2->Animate(1,4,0,0,8.0f);	
	
	ClothAnimation->SetPivot(0, 0);
	ClothAnimation->Animate(1,4,0,0,8.0f);	
}

void L2_S1_UndergroundHO::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;
	if( !ProfileSystem::Instance->ActiveProfile.L2_S1_InvColorPowderTaken  && ProfileSystem::Instance->ActiveProfile.L2_S1_IsNagaSculpturePlaced )
			isAnyTaskLeft = true;		

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S1_UndergroundHO] = isAnyTaskLeft;

	if( ProfileSystem::Instance->ActiveProfile.L2_S1_InvColorPowderTaken  )
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S2_NagaTemple] = true;
}

void L2_S1_UndergroundHO::HandleHOComplete()
{
	if(_handleExtrasGameComplete(false))
		return;

	Hud->IsTakeAndUseHOScene = false;
	Hud->HOState = HOInactive;
	Hud->InvBoxCtrl->SetLockBase(INVBOX_LR_FRAME);
}

void L2_S1_UndergroundHO::UpdateBannerMessages( const CPoint pos )
{
	if( isObjectInHand || !Control::Input->LBclicked())
		return;

	if (DestPolyArray[e_DuckFigure]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S1_DuckFixed )
	{
		Hud->ShowBannerText("L2stand");
	} 
	else if (DestPolyArray[e_Spear]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S1_SpearUsed )
	{
		Rock01->Shake(-125, 2, 0.5f, 0.55f);
		Hud->ShowBannerText("L2leverage");
	}
	else if (BoxPolyArray->Intersection(&pos) && ( !ProfileSystem::Instance->ActiveProfile.L2_S1_Box1Fixed ||
		!ProfileSystem::Instance->ActiveProfile.L2_S1_Box2Fixed || !ProfileSystem::Instance->ActiveProfile.L2_S1_Box3Fixed ))
	{ 
		Hud->ShowBannerText("L2cube");
	}
	else if (LeftLeverRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S1_ScrewFixed )
	{
		LeftLock00->Shake(90, 1.25f,0,0.25f);
		Hud->ShowBannerText("L2handle");
	}
	else if (( LeftLeverRect.Intersection(&pos) || DestPolyArray[e_Screwdriver]->Intersection(&pos) ) && !ProfileSystem::Instance->ActiveProfile.L2_S1_ScrewTightened &&
			 ProfileSystem::Instance->ActiveProfile.L2_S1_ScrewFixed )
	{
		Hud->ShowBannerText("L2tight");
	}
	else if ( LeftLeverRect.Intersection(&pos)  && !ProfileSystem::Instance->ActiveProfile.L2_S1_StarFixed && ProfileSystem::Instance->ActiveProfile.L2_S1_ScrewTightened )
	{
		Hud->ShowBannerText("L2star");
	}
	else if ( DestPolyArray[e_Stone]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S1_KeyFixed && !ProfileSystem::Instance->ActiveProfile.L2_S1_SmallBoxDoorOpened )
	{
		Hud->ShowBannerText("L2key");
	}
	else if ( DestPolyArray[e_Star]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S1_StarFixed )
	{
		Hud->ShowBannerText("L2miss");
	}
	else if (RightLeverRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S1_PentagonFixed )
	{
		RightLock01->Shake(90, 1.25f,0,0.25f);
		Hud->ShowBannerText("L2lever");
	}
	else if ( DestPolyArray[e_Pentagon]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S1_PentagonFixed )
	{
		Hud->ShowBannerText("L2hexagon");
	}
	else if (RightLeverRect.Intersection(&pos)  && ProfileSystem::Instance->ActiveProfile.L2_S1_PentagonFixed && !ProfileSystem::Instance->ActiveProfile.L2_S1_KeyTurned )
	{
		Hud->ShowBannerText("L2seem");
	}
	else if (LeftLeverRect.Intersection(&pos)  && ProfileSystem::Instance->ActiveProfile.L2_S1_StarFixed && !ProfileSystem::Instance->ActiveProfile.L2_S1_KeyTurned )
	{
		Hud->ShowBannerText("L2seem");
	}
	else if( DestPolyArray[e_Stone]->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L2_S1_KeyTurned && 
		(!ProfileSystem::Instance->ActiveProfile.L2_S1_LeftDoorOpened || !ProfileSystem::Instance->ActiveProfile.L2_S1_RightDoorOpened) )
	{
		LeftDoor->Shake(180,2,0,0.25f);
		RightDoor->Shake(0,2,0,0.25f);
		Hud->ShowBannerText("L2unless");
	}
	else if (DestPolyArray[e_Stone]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S1_StarFixed && !ProfileSystem::Instance->ActiveProfile.L2_S1_KeyFixed && 
		!ProfileSystem::Instance->ActiveProfile.L2_S1_PentagonFixed)
	{
		LeftDoor->Shake(180,2,0,0.25f);
		RightDoor->Shake(0,2,0,0.25f);
		Hud->ShowBannerText("L2force");
	}
	else if (DestPolyArray[e_Handle]->Intersection(&pos)  && !ProfileSystem::Instance->ActiveProfile.L2_S1_KeyDoorLeverFixed )
	{
		Shelf->Shake(-115, 2, 0, 0.35f);
		Hud->ShowBannerText("L2slide");
	}	
	else if (DestPolyArray[e_Stone]->Intersection(&pos)  && ProfileSystem::Instance->ActiveProfile.L2_S1_LeftDoorOpened && ProfileSystem::Instance->ActiveProfile.L2_S1_RightDoorOpened &&
			!ProfileSystem::Instance->ActiveProfile.L2_S1_MirrorRemoved )
	{
		Hud->ShowBannerText("L2glass");
	}	
}

void L2_S1_UndergroundHO::InitControlPolys()
{
	int DestPointsArray0[] = {1126,332,1063,363,1063,455,1125,512,1241,484,1222,409};
	DestPolyArray[e_Spear]=  new CPolygon(6);
	DestPolyArray[e_Spear]->Init(DestPointsArray0);

	int DestPointsArray1[] = {253,447,351,445,420,497,426,584,342,638,218,614,179,518};
	DestPolyArray[e_DuckFigure] =  new CPolygon(7);
	DestPolyArray[e_DuckFigure] ->Init(DestPointsArray1);

	int DestPointsArray2[] = {653,380,737,369,793,458,696,478};
	DestPolyArray[e_Box1_Duck]=  new CPolygon(4);
	DestPolyArray[e_Box1_Duck]->Init(DestPointsArray2);
	int DestPointsArray3[] = {733,368,788,458,883,445,819,354};
	DestPolyArray[e_Box2_Rock]=  new CPolygon(4);
	DestPolyArray[e_Box2_Rock]->Init(DestPointsArray3);
	int DestPointsArray4[] = {815,352,896,345,975,441,877,448};
	DestPolyArray[e_Box3_Lotus]=  new CPolygon(4);
	DestPolyArray[e_Box3_Lotus]->Init(DestPointsArray4);

	int DestPointsArray5[] = {490,153,459,121,492,62,550,55,656,87,656,130,622,170,619,207,544,221};
	DestPolyArray[e_Screw] =  new CPolygon(9);
	DestPolyArray[e_Screw] ->Init(DestPointsArray5);
	int DestPointsArray6[] = {513,82,548,81,606,136,585,204,486,205,473,144};
	DestPolyArray[e_Screwdriver] =  new CPolygon(6);
	DestPolyArray[e_Screwdriver] ->Init(DestPointsArray6);
	int DestPointsArray7[] = {447,189,485,210,489,252,455,275,409,260,399,213};
	DestPolyArray[e_Star] =  new CPolygon(6);
	DestPolyArray[e_Star] ->Init(DestPointsArray7);
	int DestPointsArray8[] = {1068,152,1099,148,1114,187,1095,218,1051,218,1038,184};
	DestPolyArray[e_Pentagon] =  new CPolygon(6);
	DestPolyArray[e_Pentagon] ->Init(DestPointsArray8);
	
	int DestPointsArray9[] = {507,323,558,320,584,344,607,418,518,422};
	DestPolyArray[e_Handle] =  new CPolygon(5);
	DestPolyArray[e_Handle] ->Init(DestPointsArray9);

	int DestPointsArray10[] = {836,242,783,250,754,206,745,151,790,124,835,130,864,164,864,213};
	DestPolyArray[e_Key] =  new CPolygon(8);
	DestPolyArray[e_Key] ->Init(DestPointsArray10);
	int DestPointsArray11[] = {779,103,729,119,692,159,694,234,745,272,801,274,838,244,863,211,860,145,818,109};
	DestPolyArray[e_Stone] =  new CPolygon(10);
	DestPolyArray[e_Stone] ->Init(DestPointsArray11);

	int ArrayPts[] = {913,463,983,474,1018,532,979,561,885,542,856,493};
	MossPoly=  new CPolygon(6);
	MossPoly->Init(ArrayPts);

	int ArrayPts1[] = {338,425,335,463,352,495,362,521,370,546,318,581,268,579,250,533,262,481,301,444,291,400,334,383,374,395,386,411,360,421};
	DuckFullPoly	=  new CPolygon(15);
	DuckFullPoly->Init(ArrayPts1);

	int GlassArray[] = {785,104,732,114,699,153,695,213,723,256,765,278,822,261,853,230,865,174,844,129,808,109};
	GlassPoly =  new CPolygon(11);
	GlassPoly->Init(GlassArray);

	int ArrayPts3[] = {633,366,902,332,988,435,680,493};
	BoxPolyArray =  new CPolygon(4);
	BoxPolyArray->Init(ArrayPts3);

	



}

void L2_S1_UndergroundHO::ShuffleHOBox(CPoint *pos )
{
	for (int i = e_Box1_Duck; i <= e_Box3_Lotus; i++)
	{
		if( DestPolyArray[i]->Intersection(pos) )
		{
			int positionIndex = -1;
			if( SelectedHOBoxCount > 0 && SelectedHOBoxIndex[SelectedHOBoxCount] == i )
			{
				return;
			}
			switch (i)
			{
			case e_Box1_Duck:	positionIndex = 0;
				break;
			case e_Box2_Rock:	positionIndex = 1;
				break;
			case e_Box3_Lotus:	positionIndex = 2;
				break;
			default:
				break;
			}
			Control::Audio->QuickLoadAndPlaySFX("s3pz1_stoneselect");
			SelectedHOBoxPos[SelectedHOBoxCount] = positionIndex;
			BoxHilight[positionIndex]->SetEnable(true);
			SelectedHOBoxIndex[SelectedHOBoxCount] = ProfileSystem::Instance->ActiveProfile.L2_S1_BoxSelectedIndex[positionIndex];
			SelectedHOBoxCount++;
			break;
		}
	}

	if( SelectedHOBoxCount > 1  )
	{
		//hack to overcome the tween position set after OnComplete Callback..
		for (int i = 0; i < 3; i++)
		{
			if( i == SelectedHOBoxPos[0] || i == SelectedHOBoxPos[1] )
			{
				BoxHilight[i]->SetEnable(false);
				BoxTweenAtPos[i]->SetEnable(true);
				BoxTweenAtPos[i]->PlayAnimation( BoxAtPos[i]->GetAnimation());
				BoxTweenAtPos[i]->SetPos(&HOBoxPos[i]);
				BoxAtPos[i]->SetEnable(false);
			}
		}

		Control::Audio->QuickLoadAndPlaySFX("s1ho_boxshuffle");

		float tweenArray1[5] = {1,BoxAtPos[SelectedHOBoxPos[1]]->Pos.x, BoxAtPos[SelectedHOBoxPos[1]]->Pos.y, 1, 0 };
		BoxTweenAtPos[SelectedHOBoxPos[0]]->TweenTo(tweenArray1, 0.65f, TW_EASEQUADOUT,  false);
			
		float tweenArray2[5] = {1,BoxAtPos[SelectedHOBoxPos[0]]->Pos.x, BoxAtPos[SelectedHOBoxPos[0]]->Pos.y, 1, 0 };
		BoxTweenAtPos[SelectedHOBoxPos[1]]->TweenTo(tweenArray2, 0.65f, TW_EASEQUADOUT, true, CC_CALLBACK_1(L2_S1_UndergroundHO::OnTweenComplete,this));
	}

}

void L2_S1_UndergroundHO::OnTweenComplete(CObject *obj)
{
	
	for (int i = 0; i < 3; i++)
	{
		BoxTweenAtPos[i]->SetEnable(false);
		BoxAtPos[i]->SetEnable(true);
		//HOBoxPos[i]	= BoxTweenAtPos[i]->Pos;
	}
	BoxAtPos[SelectedHOBoxPos[0]]->PlayAnimation(SelectedHOBoxIndex[1]);
	BoxAtPos[SelectedHOBoxPos[1]]->PlayAnimation(SelectedHOBoxIndex[0]);
	ProfileSystem::Instance->ActiveProfile.L2_S1_BoxSelectedIndex[0] = BoxAtPos[0]->GetAnimation();
	ProfileSystem::Instance->ActiveProfile.L2_S1_BoxSelectedIndex[1] = BoxAtPos[1]->GetAnimation();
	ProfileSystem::Instance->ActiveProfile.L2_S1_BoxSelectedIndex[2] = BoxAtPos[2]->GetAnimation();

	TweenCount = 0;
	SelectedHOBoxCount = 0;
	SelectedHOBoxIndex[0] = -1;
	SelectedHOBoxIndex[1] = -1;
	SelectedHOBoxPos[0] = -1;
	SelectedHOBoxPos[1] = -1;
	if( CheckIfHOBoxesAligned() )
	{
		OpenHOBoxShelf();
	}
}

void L2_S1_UndergroundHO::SetHOBoxAtPosition(int boxIndex, int boxPos)
{
	ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[boxIndex] = HOITEM_USED;

	int positionIndex = -1;

	switch (boxPos)
	{
	case e_Box1_Duck:			positionIndex = 0;
		break;
	case e_Box2_Rock:			positionIndex = 1;
		break;
	case e_Box3_Lotus:			positionIndex = 2;
		break;
	default:
		break;
	}

	switch (boxIndex)
	{
	case eHOItem3_BoxDuck:			boxIndex = 0;
		break;
	case eHOItem4_BoxStone:			boxIndex = 1;
		break;
	case eHOItem5_BoxUnderLotus:	boxIndex = 2;
		break;
	default:
		break;
	}

	

	Control::Audio->QuickLoadAndPlaySFX("s1ho_tileplaced");
	BoxAtPos[positionIndex]->SetEnable(true);
	BoxAtPos[positionIndex]->PlayAnimation(boxIndex);
	ProfileSystem::Instance->ActiveProfile.L2_S1_BoxSelectedIndex[positionIndex] = boxIndex;
	
	


	if( ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[eHOItem3_BoxDuck] == HOITEM_USED &&
		ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[eHOItem4_BoxStone] == HOITEM_USED &&
		ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[eHOItem5_BoxUnderLotus] == HOITEM_USED)
	{
		isHOBoxesPlaced = true;
	}
	if( CheckIfHOBoxesAligned() )
	{
		OpenHOBoxShelf();
	}
}

bool L2_S1_UndergroundHO::CheckIfHOBoxesAligned()
{
	bool flag = true;
	
	for (int i = 0; i < 3; i++)
	{
		if( ProfileSystem::Instance->ActiveProfile.L2_S1_BoxSelectedIndex[i] != i )
		{
			flag = false;
		} 
	}
	return(flag);
}

void L2_S1_UndergroundHO::OpenHOBoxShelf()
{
	Control::Audio->QuickLoadAndPlaySFX("s1ho_blockdooropen");

	BoxOpenAnim->PlayAnim();
	isAnimPlaying = true;
	isBoxOpenAnimPlaying = true;
	BoxPlaced01->SetEnable(false);
	BoxPlaced02->SetEnable(false);
	BoxPlaced03->SetEnable(false);
	BoxClosed->SetEnable(false);
	isHOBoxesAligned = true;
	for (int i = 0; i < 3; i++)
		BoxAtPos[i]->SetEnable(false);
	ProfileSystem::Instance->ActiveProfile.L2_S1_BoxOpened = true;
}

void L2_S1_UndergroundHO::OnAnimationComplete(int index)
{
	switch (index)
	{
	case Anim_DuckOpen:
		DuckOpenAnim->SetEnableObjects(false);
		DuckHeadAnimation3->SetEnable(true);
		Box1Mask->SetEnable(true);
		ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[eHOItem3_BoxDuck] = HOITEM_UNLOCKED;
		break;
	case Anim_SpearRock:
		SpearRockAnim->SetEnableObjects(false);
		Rock02->SetEnable(true);
		ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[eHOItem4_BoxStone] = HOITEM_UNLOCKED;
		break;
	case Anim_KeyFix:
		KeyFixAnim->SetEnableObjects(false);
		KeyInKeyHole1->SetEnable(true);
		KeyMask->SetEnable(false);
		break;
	case Anim_KeyTurn:	
		KeyTurnAnim->SetEnableObjects(false);
		KeyInKeyHole2->SetEnable(true);
		HOScene->GetObjectByID("keylockleftmask")->SetEnable(false);
		HOScene->GetObjectByID("keylockrightmask")->SetEnable(false);
		break;
	case Anim_LeftLock: 
		LeftLockAnim->SetEnableObjects(false);
		LeftLock02->SetEnable(true);
		break;
	case Anim_LeftLever:
		LeftLeverAnim->SetEnableObjects(false);
		LeftLever005->SetEnable(true);
		break;
	case Anim_RightLever:
		RightLeverAnim->SetEnableObjects(false);
		RightLever05->SetEnable(true);
		break;
	case Anim_RightLock: 
		RightLockAnim->SetEnableObjects(false);
		RightLock03->SetEnable(true);
		break;
	case Anim_LeftDoorOpen:
		LeftDoorOpenAnim->SetEnableObjects(false);
		break;
	case Anim_RightDoorOpen:
		RightDoorOpenAnim->SetEnableObjects(false);
		break;
	case Anim_ScrewTighten:	
		ScrewTightenAnim->SetEnableObjects(false);
		LeftScrew->SetEnable(true);
		LeftLock00->SetEnable(true);

		if( ProfileSystem::Instance->ActiveProfile.L2_S1_StarFixed ) 
		{
			StarTurned->SetEnable(true);
			StarTurned->FadeIn(1.5f);

			Control::Audio->QuickLoadAndPlaySFX("s1ho_pentagonfix");

			ProfileSystem::Instance->ActiveProfile.L2_S1_StarKeyTurned = true;
			LeftLockAnim->PlayAnim();
			isAnimPlaying = true;
		}
		break;
	case Anim_BoxDoorOpen:
		BoxOpenAnim->SetEnableObjects(false);
		isBoxOpenAnimPlaying = false;
		Shelf->SetEnable(true);
		KeyInsideBox->SetEnable(true); 
		BoxOpen->SetEnable(true);
		Star->SetEnable(true);
		pentagon->SetEnable(true);
		KeyBoxLeverbase->SetEnable(true);

		if(HOInvBox->GetInventoryItemStateOf(eHOItem8_StarKey) == kInventoryItemState_NotCollected)
		{
			ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[eHOItem8_StarKey] = HOITEM_UNLOCKED;
		}
		if(HOInvBox->GetInventoryItemStateOf(eHOItem9_PentagonKey) == kInventoryItemState_NotCollected)
		{
			ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[eHOItem9_PentagonKey] = HOITEM_UNLOCKED;
		}

		break;
	case Anim_ShelfOpen:
		 ShelfOpenAnim->SetEnableObjects(false);
		 KeyBoxLeverTurned->SetEnable(true);
		 KeyInsideBox->SetEnable(true);

		 ProfileSystem::Instance->ActiveProfile.L2S1_HOItemStates[eHOItem11_KeyDoor] = HOITEM_UNLOCKED;
		break;
	case Anim_RemoveMirror:
		MirrorRemoveAnim->SetEnableObjects(false);
		Inventory->SetEnable(true);
		MirrorBroken1->SetEnable(true);
		break;
	default:
		break;
	}
	isAnimPlaying = false;
}

void L2_S1_UndergroundHO::InitAnimations()
{
	s2horippleFX = new SuperFX("PRT\\L2\\s2horipple.INI", PointSystem::CreateCPoint(936,521,0), 43); 
	s2horippleFX->Start();
	s2horippleFX->AdvanceByTime(2.0f);

	s1holeavesFX = new SuperFX("PRT\\L2\\s1holeaves.INI", PointSystem::CreateCPoint(776,3,0), 120); 
	s1holeavesFX->Start();

	s1hoflies1FX = new SuperFX("PRT\\L2\\s1hoflies1.INI", PointSystem::CreateCPoint(1173,162,0), 218); 
	s1hoflies1FX->Start();
	s1hoflies1FX->AdvanceByTime(2.0f);

	s1hoflies2FX = new SuperFX("PRT\\L2\\s1hoflies2.INI", PointSystem::CreateCPoint(266,241,0), 120); 
	s1hoflies2FX->Start();
	s1hoflies2FX->AdvanceByTime(2.0f);

	DuckOpenAnim = NULL;
	DuckOpenAnim = new JSONAnimator( Anim_DuckOpen, CC_CALLBACK_1(L2_S1_UndergroundHO::OnAnimationComplete, this ));
	DuckOpenAnim->parseAndLoadJSON("ANIM\\L2\\S1HODUCKOPENANIM.JSON", HOScene );

	SpearRockAnim = NULL;
	SpearRockAnim = new JSONAnimator( Anim_SpearRock, CC_CALLBACK_1(L2_S1_UndergroundHO::OnAnimationComplete, this ));
	SpearRockAnim->parseAndLoadJSON("ANIM\\L2\\S1HOSPEARROCKANIM.JSON", HOScene );

	KeyFixAnim = NULL;
	KeyFixAnim = new JSONAnimator( Anim_KeyFix, CC_CALLBACK_1(L2_S1_UndergroundHO::OnAnimationComplete, this ));
	KeyFixAnim->parseAndLoadJSON("ANIM\\L2\\S1HOKEYFIXANIM.JSON", HOScene );

	KeyTurnAnim = NULL;
	KeyTurnAnim = new JSONAnimator( Anim_KeyTurn, CC_CALLBACK_1(L2_S1_UndergroundHO::OnAnimationComplete, this ));
	KeyTurnAnim->parseAndLoadJSON("ANIM\\L2\\S1HOKEYTURNANIM.JSON", HOScene );

	LeftLockAnim = NULL;
	LeftLockAnim = new JSONAnimator( Anim_LeftLock, CC_CALLBACK_1(L2_S1_UndergroundHO::OnAnimationComplete, this ));
	LeftLockAnim->parseAndLoadJSON("ANIM\\L2\\S1HOLEFTLOCKANIM.JSON", HOScene );

	LeftLeverAnim = NULL;
	LeftLeverAnim = new JSONAnimator( Anim_LeftLever, CC_CALLBACK_1(L2_S1_UndergroundHO::OnAnimationComplete, this ));
	LeftLeverAnim->parseAndLoadJSON("ANIM\\L2\\S1HOLEFTLEVERANIM.JSON", HOScene );

	RightLeverAnim = NULL;
	RightLeverAnim = new JSONAnimator( Anim_RightLever, CC_CALLBACK_1(L2_S1_UndergroundHO::OnAnimationComplete, this ));
	RightLeverAnim->parseAndLoadJSON("ANIM\\L2\\S1HORIGHTLEVERANIM.JSON", HOScene );

	RightLockAnim = NULL;
	RightLockAnim = new JSONAnimator( Anim_RightLock, CC_CALLBACK_1(L2_S1_UndergroundHO::OnAnimationComplete, this ));
	RightLockAnim->parseAndLoadJSON("ANIM\\L2\\S1HORIGHTLOCKANIM.JSON", HOScene );

	LeftDoorOpenAnim = NULL;
	LeftDoorOpenAnim = new JSONAnimator( Anim_LeftDoorOpen, CC_CALLBACK_1(L2_S1_UndergroundHO::OnAnimationComplete, this ));
	LeftDoorOpenAnim->parseAndLoadJSON("ANIM\\L2\\S1HOLEFTDOOROPENANIM.JSON", HOScene );

	RightDoorOpenAnim = NULL;
	RightDoorOpenAnim = new JSONAnimator( Anim_RightDoorOpen, CC_CALLBACK_1(L2_S1_UndergroundHO::OnAnimationComplete, this ));
	RightDoorOpenAnim->parseAndLoadJSON("ANIM\\L2\\S1HORIGHTDOOROPENANIM.JSON", HOScene );
	
	ScrewTightenAnim = NULL;
	ScrewTightenAnim = new JSONAnimator( Anim_ScrewTighten, CC_CALLBACK_1(L2_S1_UndergroundHO::OnAnimationComplete, this ));
	ScrewTightenAnim->parseAndLoadJSON("ANIM\\L2\\S1HOSCREWTIGHTENANIM.JSON", HOScene );

	BoxOpenAnim = NULL;
	BoxOpenAnim = new JSONAnimator( Anim_BoxDoorOpen, CC_CALLBACK_1(L2_S1_UndergroundHO::OnAnimationComplete, this ));
	BoxOpenAnim->parseAndLoadJSON("ANIM\\L2\\S1HOBOXOPENANIM.JSON", HOScene );
	BoxOpenAnim->SetEnableObjects(false);

	ShelfOpenAnim = NULL;
	ShelfOpenAnim = new JSONAnimator( Anim_ShelfOpen, CC_CALLBACK_1(L2_S1_UndergroundHO::OnAnimationComplete, this ));
	ShelfOpenAnim->parseAndLoadJSON("ANIM\\L2\\S1HOSHELFOPENANIM.JSON", HOScene );

	s1hoBrokenGlassFX = new SuperFX("PRT\\L2\\s1hobrokenglass.INI", PointSystem::CreateCPoint(776,3,0), 120);
	std::vector<SuperFX*> superfxArray;
	superfxArray.push_back(s1hoBrokenGlassFX);
	
	MirrorRemoveAnim = NULL;
	MirrorRemoveAnim = new JSONAnimator( Anim_RemoveMirror, CC_CALLBACK_1(L2_S1_UndergroundHO::OnAnimationComplete, this ));
	MirrorRemoveAnim->parseAndLoadJSON("ANIM\\L2\\S1HOMIRRORREMOVEANIM.JSON", HOScene, &superfxArray);
}

void L2_S1_UndergroundHO::InitSceneElements()
{
	Inventory			   = HOScene->GetObjectByID("Inventory");
	MirrorFull			   = HOScene->GetObjectByID("MirrorFull");
	MirrorBroken0		   = HOScene->GetObjectByID("MirrorBroken0");
	MirrorBroken1		   = HOScene->GetObjectByID("MirrorBroken1");
	RightDoor			   = HOScene->GetObjectByID("LeftDoor");
	LeftDoor			   = HOScene->GetObjectByID("RightDoor");
	LeftLock00			   = HOScene->GetObjectByID("LeftLock00");
	LeftLock01			   = HOScene->GetObjectByID("LeftLock01");
	LeftLock02			   = HOScene->GetObjectByID("LeftLock02");
	LeftLever001		   = HOScene->GetObjectByID("LeftLever001");
	RightLock01			   = HOScene->GetObjectByID("RightLock01");
	LeftLever002		   = HOScene->GetObjectByID("LeftLever002");
	LeftLever003		   = HOScene->GetObjectByID("LeftLever003");
	LeftLever004		   = HOScene->GetObjectByID("LeftLever004");
	LeftLever005		   = HOScene->GetObjectByID("LeftLever005");
	RightLock02			   = HOScene->GetObjectByID("RightLock02");
	RightLock03			   = HOScene->GetObjectByID("RightLock03");
	RightLever01		   = HOScene->GetObjectByID("RightLever01");
	RightLever02		   = HOScene->GetObjectByID("RightLever02");
	RightLever03		   = HOScene->GetObjectByID("RightLever03");
	RightLever04		   = HOScene->GetObjectByID("RightLever04");
	RightLever05		   = HOScene->GetObjectByID("RightLever05");
	Spear				   = HOScene->GetObjectByID("Spear");
	SpearOnClick		   = HOScene->GetObjectByID("SpearOnClick");
	screwdriverInScene	   = HOScene->GetObjectByID("screwdriverInScene");
	DuckBottom			   = HOScene->GetObjectByID("DuckBottom");
	DuckFull			   = HOScene->GetObjectByID("DuckFull");
	DuckFullOnClick		   = HOScene->GetObjectByID("DuckFullOnClick");
	BoxInsideRock		   = HOScene->GetObjectByID("BoxInsideRock");
	DoorHandle			   = HOScene->GetObjectByID("keydoorleverscn");

	StarTurned				= HOScene->GetObjectByID("starkeyturned");
	pentagonTurned			= HOScene->GetObjectByID("pentagonkeyturned");

	KeyBoxLeverbase		= HOScene->GetObjectByID("keyboxleverbase");
	KeyBoxLeverFixed	= HOScene->GetObjectByID("keyboxleverfixed");
	KeyBoxLeverTurned	= HOScene->GetObjectByID("keyboxleverturned");
	
	LeftScrew			   = HOScene->GetObjectByID("LeftScrew");
	LightAnimation		   = HOScene->GetObjectByID("LightAnimation");
	Rock01				   = HOScene->GetObjectByID("movablerock");
	Rock02				   = HOScene->GetObjectByID("Rock02");
	Box1				   = HOScene->GetObjectByID("Box1");
	DuckHeadAnimation1	   = HOScene->GetObjectByID("DuckHeadAnimation1");
	DuckHeadAnimation2	   = HOScene->GetObjectByID("DuckHeadAnimation2");
	DuckHeadAnimation3	   = HOScene->GetObjectByID("DuckHeadAnimation3");
	StarFixed			   = HOScene->GetObjectByID("StarFixed");
	PentagonFixed		   = HOScene->GetObjectByID("PentagonFixed");
	BoxInsideVessel		   = HOScene->GetObjectByID("BoxInsideVessel");
	Moss01				   = HOScene->GetObjectByID("Moss01");
	Moss02				   = HOScene->GetObjectByID("Moss02");
	Moss03				   = HOScene->GetObjectByID("Moss03");
	KeyMask				   = HOScene->GetObjectByID("KeyMask");
	LeyInKeyOnClick		   = HOScene->GetObjectByID("LeyInKeyOnClick");
	KeyInKeyHole1		   = HOScene->GetObjectByID("LeyInKeyHole1");
	KeyInKeyHole2		   = HOScene->GetObjectByID("LeyInKeyHole2");
	RootAnimation2		   = HOScene->GetObjectByID("RootAnimation2");
	RootAnimation3		   = HOScene->GetObjectByID("RootAnimation3");
	RootAnimation1		   = HOScene->GetObjectByID("RootAnimation1");
	ClothAnimation		   = HOScene->GetObjectByID("ClothAnimation");
	Plant0				   = HOScene->GetObjectByID("Plant0");
	Plant1				   = HOScene->GetObjectByID("Plant1");
	Screwdriver0		   = HOScene->GetObjectByID("Screwdriver0");
	Screwdriver1		   = HOScene->GetObjectByID("Screwdriver1");
	Screwdriver2		   = HOScene->GetObjectByID("Screwdriver2");
	Screwdriver3		   = HOScene->GetObjectByID("Screwdriver3");
	Screwdriver4		   = HOScene->GetObjectByID("Screwdriver4");
	Screwdriver5		   = HOScene->GetObjectByID("Screwdriver5");
	ScrewOnGround		   = HOScene->GetObjectByID("ScrewOnGround");
	ScrewOnClick		   = HOScene->GetObjectByID("ScrewOnClick");
	ScrewFit			   = HOScene->GetObjectByID("ScrewFit");
	ScrewMask			   = HOScene->GetObjectByID("ScrewMask");
	BoxClosed			   = HOScene->GetObjectByID("BoxClosed");
	BoxPlaced03			   = HOScene->GetObjectByID("Box2");
	BoxPlaced01			   = HOScene->GetObjectByID("Boxe0");
	BoxPlaced02			   = HOScene->GetObjectByID("Boxe1");
	BoxOpenMidle		   = HOScene->GetObjectByID("BoxOpenMidle");
	BoxOpen				   = HOScene->GetObjectByID("BoxOpen");
	Star				   = HOScene->GetObjectByID("Star");
	StarOnClick			   = HOScene->GetObjectByID("StarOnClick");
	pentagon			   = HOScene->GetObjectByID("pentagon");
	PentagonOnClick		   = HOScene->GetObjectByID("PentagonOnClick");
	KeyInsideBox		   = HOScene->GetObjectByID("KeyInsideBox");
	Shelf				   = HOScene->GetObjectByID("Shelf");
	ShelfMask			   = HOScene->GetObjectByID("ShelfMask");
	DoorHandleInPosition   = HOScene->GetObjectByID("DoorHandleInPosition");
	DoorHandleOnClick	   = HOScene->GetObjectByID("DoorHandleOnClick");
	Box1Mask			   = HOScene->GetObjectByID("Box1Mask");
	BoxAtPos[0]			   = HOScene->GetObjectByID("BoxP1");
	BoxAtPos[1]			   = HOScene->GetObjectByID("BoxP2");
	BoxAtPos[2]			   = HOScene->GetObjectByID("BoxP3");
	BoxTweenAtPos[0]	   = HOScene->GetObjectByID("BoxP1Tween");
	BoxTweenAtPos[1]	   = HOScene->GetObjectByID("BoxP2Tween");
	BoxTweenAtPos[2]	   = HOScene->GetObjectByID("BoxP3Tween");
	BoxHilight[0]		   = HOScene->GetObjectByID("Box1Hilight");
	BoxHilight[1]		   = HOScene->GetObjectByID("Box2Hilight");
	BoxHilight[2]		   = HOScene->GetObjectByID("Box3Hilight");

	Moss01->Animate(1,2,0,0,4.0f);
	Moss02->Animate(2,3,0,0,5.25f);
	Moss03->Animate(1,2,0,0,5.5f);

	HOScene->GetObjectByID("lotus2")->Animate(1,2,0,0,4.0f);
	HOScene->GetObjectByID("lotus1")->Animate(1,2,0,0,5.5f);

	HOScene->GetObjectByID("RockOnClick")->SetEnable(false);

	for (int i = 0; i < 3; i++)
	{
		HOBoxPos[i]			= BoxAtPos[i]->Pos;
		BoxTweenAtPos[i]	->SetEnable(false); 
	}

	DuckPos				   = DuckFullOnClick->Pos;

	if( ProfileSystem::Instance->ActiveProfile.L2_S1_BoxOpened )
	{
		for (int i = 0; i < 3; i++)
		{
			BoxAtPos[i]->SetEnable(false); 
		}
	}
	else if(ProfileSystem::Instance->ActiveProfile.L2_S1_BoxSelectedIndex[0] != -1 )
		BoxAtPos[0]->PlayAnimation(ProfileSystem::Instance->ActiveProfile.L2_S1_BoxSelectedIndex[0]);
	else
		BoxAtPos[0]			->SetEnable(false);

	if(	ProfileSystem::Instance->ActiveProfile.L2_S1_BoxSelectedIndex[1] != -1 )
		BoxAtPos[1]->PlayAnimation(ProfileSystem::Instance->ActiveProfile.L2_S1_BoxSelectedIndex[1]);
	else
		BoxAtPos[1]			->SetEnable(false);

	if(	ProfileSystem::Instance->ActiveProfile.L2_S1_BoxSelectedIndex[2] != -1 )
		BoxAtPos[2]->PlayAnimation(ProfileSystem::Instance->ActiveProfile.L2_S1_BoxSelectedIndex[2]);
	else
		BoxAtPos[2]			->SetEnable(false);

	ShelfMask				->SetEnable(false);
	BoxHilight[0]		    ->SetEnable(false);
	BoxHilight[1]		    ->SetEnable(false);
	BoxHilight[2]		    ->SetEnable(false);

	DuckFullOnClick		 	->SetEnable(false);
	DuckHeadAnimation1		->SetEnable(false);
	//Box1Mask		 		->SetEnable(false);
	if( !ProfileSystem::Instance->ActiveProfile.L2_S1_DuckFixed )
	{
		DuckHeadAnimation2	 ->SetEnable(false);
		DuckHeadAnimation3	->SetEnable(false);
		DuckBottom			->SetEnable(false);
		Box1Mask			->SetEnable(false);
	}
	else if( !ProfileSystem::Instance->ActiveProfile.L2_S1_DuckOpened )
	{
		DuckFull			->SetEnable(false);
		DuckHeadAnimation3	->SetEnable(false);
		Box1Mask			->SetEnable(false);
	}
	else
	{
		DuckHeadAnimation2	->SetEnable(false);
		DuckFull			->SetEnable(false);
	}
	if( ProfileSystem::Instance->ActiveProfile.L2_S1_Box1Fixed)
	{
		Box1Mask			->SetEnable(false);
	}


	Screwdriver0		 	->SetEnable(false);
	Screwdriver1		 	->SetEnable(false);
	Screwdriver2		 	->SetEnable(false);
	Screwdriver3		 	->SetEnable(false);
	Screwdriver4		 	->SetEnable(false);
	Screwdriver5		 	->SetEnable(false);
	ScrewOnClick		 	->SetEnable(false);
	ScrewMask				->SetEnable(false);
	LeftLock01			 	->SetEnable(false);
	LeftLever002		 	->SetEnable(false);
	LeftLever003		 	->SetEnable(false);
	LeftLever004		 	->SetEnable(false);
	RightLock02			 	->SetEnable(false);
	RightLever02		 	->SetEnable(false);
	RightLever03		 	->SetEnable(false);
	RightLever04		 	->SetEnable(false);
	StarOnClick			 	->SetEnable(false);
	PentagonOnClick		 	->SetEnable(false);
	
	if( !ProfileSystem::Instance->ActiveProfile.L2_S1_ScrewFixed )
	{
		LeftScrew			 	->SetEnable(false);
		ScrewFit				->SetEnable(false);
		if( ProfileSystem::Instance->ActiveProfile.L2_S1_StarFixed )
			LeftLock02			 	->SetEnable(false);
	}
	else if( !ProfileSystem::Instance->ActiveProfile.L2_S1_ScrewTightened )
	{
		ScrewOnGround		 	->SetEnable(false);
		LeftScrew			 	->SetEnable(false);
		if( ProfileSystem::Instance->ActiveProfile.L2_S1_StarFixed )
			LeftLock02			 	->SetEnable(false);
	}
	else
	{
		//Screw Tightened
		ScrewOnGround		 	->SetEnable(false);
		screwdriverInScene	 	->SetEnable(false);
		ScrewFit				->SetEnable(false);
		if( ProfileSystem::Instance->ActiveProfile.L2_S1_StarFixed )
		{
			LeftLock00			 	->SetEnable(false);
			LeftScrew			 	->SetEnable(false);
		}
	}

	if( !ProfileSystem::Instance->ActiveProfile.L2_S1_StarFixed )
	{
		LeftLock02			 	->SetEnable(false);
		StarFixed			 	->SetEnable(false); 
	}
	else
	{
		Star				 	->SetEnable(false);
	}

	if (!ProfileSystem::Instance->ActiveProfile.L2_S1_PentagonFixed )
	{
		PentagonFixed		 	->SetEnable(false); 
		RightLock03			 	->SetEnable(false);
	}
	else
	{
		if (!ProfileSystem::Instance->ActiveProfile.L2_S1_PentagonKeyTurned )
		{
			RightLock01			 	->SetEnable(false);
			pentagon			 	->SetEnable(false);
		}
	}

	if( !ProfileSystem::Instance->ActiveProfile.L2_S1_StarKeyTurned )
		StarTurned->SetEnable(false);

	if( !ProfileSystem::Instance->ActiveProfile.L2_S1_PentagonKeyTurned )
		pentagonTurned->SetEnable(false);

	if(!ProfileSystem::Instance->ActiveProfile.L2_S1_LeftDoorOpened ) 
		LeftLever005		 	->SetEnable(false);
	else
	{
		LeftDoor			 	->SetEnable(false);
		LeftLever001		 	->SetEnable(false);
	}

	if(!ProfileSystem::Instance->ActiveProfile.L2_S1_RightDoorOpened) 
		RightLever05		 	->SetEnable(false);
	else
	{
		KeyInKeyHole2			->SetEnable(false);
		RightDoor			 	->SetEnable(false);
		RightLever01		 	->SetEnable(false);
	}


	MirrorBroken0		 	->SetEnable(false);
	if( !ProfileSystem::Instance->ActiveProfile.L2_S1_MirrorRemoved ) 
		MirrorBroken1		 	->SetEnable(false);
	else 
	{		
		MirrorFull				->SetEnable(false);
	}
	if( ProfileSystem::Instance->ActiveProfile.L2_S1_InvColorPowderTaken ) 
		Inventory				->SetEnable(false);
	
	SpearOnClick		 	->SetEnable(false);
	if( ProfileSystem::Instance->ActiveProfile.L2_S1_SpearUsed ) 
	{
		Rock01				 	->SetEnable(false);
		Spear				 	->SetEnable(false);
	}

	if( ProfileSystem::Instance->ActiveProfile.L2_S1_PayalRemoved )
	{
		Moss01				 	->SetEnable(false);
		Moss02				 	->SetEnable(false);
		Moss03				 	->SetEnable(false);
		HOScene->GetObjectByID("lotus1")->SetEnable(false);
		HOScene->GetObjectByID("lotus2")->SetEnable(false);
	}

	DoorHandleOnClick	 	->SetEnable(false);
	
	LeyInKeyOnClick		 	->SetEnable(false);
	if (!ProfileSystem::Instance->ActiveProfile.L2_S1_KeyFixed )
	{
		KeyInKeyHole1		 	->SetEnable(false);
		KeyInKeyHole2		 	->SetEnable(false);
	}
	else
	{
		KeyInKeyHole1		 	->SetEnable(false);
	}

	
	if( ProfileSystem::Instance->ActiveProfile.L2_S1_KeyDoorLeverFixed && !ProfileSystem::Instance->ActiveProfile.L2_S1_KeyFixed )
		KeyInsideBox		 	->SetEnable(true); 
	else 
		KeyInsideBox		 	->SetEnable(false); 
	
	KeyMask					->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L2_S1_KeyTurned)
	{
		KeyInKeyHole1		 	->SetEnable(false);
		KeyInKeyHole2		 	->SetEnable(false);
		HOScene->GetObjectByID("keylockleftmask")->SetEnable(false);
		HOScene->GetObjectByID("keylockrightmask")->SetEnable(false);
		HOScene->GetObjectByID("keylockteeth")->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L2_S1_LeftDoorOpened)
	{
		LeftDoor->SetEnable(false);
		LeftLever001->SetEnable(false);
		LeftLock00->SetEnable(false);
		LeftScrew->SetEnable(false);
		LeftLever005->SetEnable(true);
		LeftLock02->SetEnable(true);
	}
	if(ProfileSystem::Instance->ActiveProfile.L2_S1_RightDoorOpened)
	{
		RightDoor->SetEnable(false);
		RightLever01->SetEnable(false);
		RightLock01->SetEnable(false);
		RightLever05->SetEnable(true);
		RightLock03->SetEnable(true);
		KeyInKeyHole2->SetEnable(false);
	}
	if( ProfileSystem::Instance->ActiveProfile.L2_S1_MirrorRemoved )
	{
		MirrorFull->SetEnable(false);
		MirrorBroken1->SetEnable(true);
	}
	
	
	BoxOpenMidle		 	->SetEnable(false);
	BoxPlaced01				->SetEnable(false); 
	BoxPlaced02				->SetEnable(false); 
	BoxPlaced03				->SetEnable(false);

	KeyBoxLeverbase->SetEnable(false);
	KeyBoxLeverFixed->SetEnable(false);
	KeyBoxLeverTurned->SetEnable(false);

	if ( !ProfileSystem::Instance->ActiveProfile.L2_S1_BoxOpened )
	{
			BoxOpen				 	->SetEnable(false); 
			pentagon			 	->SetEnable(false);
			Shelf				 	->SetEnable(false);
			Star				 	->SetEnable(false);
			KeyInsideBox		 	->SetEnable(false);
	}
	else
	{
		BoxClosed			 	->SetEnable(false);

		KeyBoxLeverbase->SetEnable(true);
		if( ProfileSystem::Instance->ActiveProfile.L2_S1_KeyDoorLeverFixed )
		{
			KeyBoxLeverbase->SetEnable(false);
			KeyBoxLeverFixed->SetEnable(true);
			if( ProfileSystem::Instance->ActiveProfile.L2_S1_SmallBoxDoorOpened )
			{
				KeyBoxLeverFixed->SetEnable(false);
				KeyBoxLeverTurned->SetEnable(false);
			}
		}
	}

	DoorHandleInPosition	->SetEnable(false);
	
	if( ProfileSystem::Instance->ActiveProfile.L2_S1_SmallBoxDoorOpened )
	{
		Shelf				 	->SetEnable(false);
	}

	if(!ProfileSystem::Instance->ActiveProfile.L2_S1_Box1Fixed )
		BoxPlaced01				->SetEnable(false);
		
	Box1				 		->SetEnable(false);
	
	if(!ProfileSystem::Instance->ActiveProfile.L2_S1_Box2Fixed )
		BoxPlaced02			 	->SetEnable(false);
	else
		BoxInsideRock->SetEnable(false);
	
	if(!ProfileSystem::Instance->ActiveProfile.L2_S1_Box3Fixed )
		BoxPlaced03				 ->SetEnable(false);
	else
		BoxInsideVessel		 	->SetEnable(false);

	if( ProfileSystem::Instance->ActiveProfile.L2_S1_MirrorRemoved && !ProfileSystem::Instance->ActiveProfile.L2_S1_InvColorPowderTaken )
	{
		HandleHOComplete();
	}

	
}
void L2_S1_UndergroundHO::NavigateToScene()
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