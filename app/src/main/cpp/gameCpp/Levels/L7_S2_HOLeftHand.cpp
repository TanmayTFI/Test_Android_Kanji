//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L7_S2_HOLeftHand.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Hud.h"
#include "Util.h"

//const bool IsLeftHandSymbol[MAX_L7S2_HANDHOSYMBOLS] = {
//	true, false, true, false, 
//	true, true, true, false, 
//	true, false, false, false};

const CRectangle  DiskRotateBtnRect(885.0f,431.0f,75.0f,84.0f);
const CRectangle  InvDestRectSymbolDisk(225.0f,56.0f,812.0f,526.0f);

const CRectangle  hudMaskRect(317.0f,663.0f,736.0f,99.0f);

L7_S2_HOLeftHand::L7_S2_HOLeftHand()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV7\\S2\\HOHANDS\\LHAND.SCN");

	HOScene->GetObjectByID("lefthandrays")->Animate(0.45f,1.0f,3.8f, 0, eBlend_AlphaAdditive);

	s2handhostarsFX = new SuperFX("PRT\\L7\\s2handhostars.INI", PointSystem::CreateCPoint(685,385,0), 1); 
	s2handhostarsFX->Start();

	HOScene->GetObjectByID("diskarrow")->SetPivot(0, 34.0f);

	diskRotateBtn = HOScene->GetObjectByID("diskrotatebtn");

	isHudSymbolsSliding = false;

	for (int i = 0; i < MAX_L7S2_HANDHOSYMBOLS; i++)
	{
		symbolRects[i] = NULL;

		hudSymbols[i] = HOScene->GetObjectByID(std::string("hud_" + MKSTR(i)).c_str());
		hudSymbols[i]->SetZ(eZOrder_INVBOXITEMS);

		hudSymbols[i]->SetMaskRect(hudMaskRect);

		hudSymbols[i]->Show(false);

		if(i<4)
		{
			//Store position of first 4 HUD symbols as reference
			hudPos[i] = hudSymbols[i]->Pos;
		}
		else
		{
			int posId = i%4;
			hudSymbols[i]->SetPos(&hudPos[posId]);
		}

		symbolGlows[i] = HOScene->GetObjectByID(std::string("symglow_" + MKSTR(i)).c_str());
		symbolGlows[i]->Show(false);

	}

	isHOCompleted = false;
	hoWaitTimer = 3.5f;

	numSceneSymbolsFound = 0;

	symbolShowFX = new SuperFX("PRT\\L7\\s2hosymbolfx.INI", PointSystem::CreateCPoint(1065,122,0), 1845);
	hoClickFX = new SuperFX("PRT\\L7\\s2hoclickfx.INI", PointSystem::CreateCPoint(564,368,0), 100); 

	s2hocompleteFX = new SuperFX("PRT\\L7\\s2hocomplete.INI", PointSystem::CreateCPoint(637,543,0), 100);

	if(ProfileSystem::Instance->ActiveProfile.L7_S2_HOLeftHandDiskPlaced)
	{
		ActivateHO();
	}
	else
	{
		DeactivateHO();
	}

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL7s2LasyaPrisonAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL7s2LasyaPrisonAmb) )		
	{
		Control::Audio->PlaySample(aAmbL7s2LasyaPrisonAmb,true);
	}
	
	//MUSIC_SFX

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L7_S2_HOLeftHand]  = true;
}

void L7_S2_HOLeftHand::ActivateHO()
{
	AwardManager::GetInstance()->StartHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);//lock in HO anim
	Hud->IsHOScene = true;
	Hud->HOState = HOActive;

	hudSymbolGroupID = 2;
	ShowNextHUDSymbols(false);

	symbolRects[0] = new CRectangle(383.0f,266.0f,66.0f,65.0f);
	symbolRects[1] = new CRectangle(586.0f,171.0f,71.0f,71.0f);
	symbolRects[2] = new CRectangle(366.0f,94.0f,53.0f,52.0f);
	symbolRects[3] = new CRectangle(585.0f,65.0f,58.0f,53.0f);
	symbolRects[4] = new CRectangle(569.0f,281.0f,71.0f,68.0f);
	symbolRects[5] = new CRectangle(561.0f,498.0f,44.0f,74.0f);
	symbolRects[6] = new CRectangle(712.0f,428.0f,44.0f,65.0f);
	symbolRects[7] = new CRectangle(744.0f,318.0f,45.0f,46.0f);
	symbolRects[8] = new CRectangle(892.0f,347.0f,49.0f,48.0f);
	symbolRects[9] = new CRectangle(673.0f,543.0f,39.0f,49.0f);
	symbolRects[10]= new CRectangle(431.0f,450.0f,36.0f,37.0f);
	symbolRects[11]= new CRectangle(248.0f,278.0f,68.0f,58.0f);

	for (int i = 0; i < MAX_L7S2_HANDHOSYMBOLS; i++)
	{
		//if(IsLeftHandSymbol[i])
		{
			if(ProfileSystem::Instance->ActiveProfile.L7_S2_HOLeftHandSymbolFound[i])
			{
				numSceneSymbolsFound += 1;
				symbolGlows[i]->Show(true);

				hudSymbols[i]->SetAlpha(0.65f);
			}				 
		}
	}

	if(numSceneSymbolsFound>=12)
	{
		hoWaitTimer = 1.5f;
		isHOCompleted = true;
	}
}

void L7_S2_HOLeftHand::DeactivateHO()
{
	HOScene->GetObjectByID("handwithdisk")->Show(false);
	HOScene->GetObjectByID("diskarrow")->Show(false);
	diskRotateBtn->Show(false);
	HOScene->GetObjectByID("fingermask")->Show(false);
	HOScene->GetObjectByID("diskoverlayglow")->Show(false);
}

void L7_S2_HOLeftHand::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if (ProfileSystem::Instance->ActiveProfile.L7_S2_HOLeftHandDiskPlaced)
	{
		if(isHOCompleted)
		{
			if (ProfileSystem::Instance->_isExtrasGamePlay)
				return;

			HOScene->update(Control::LogicRate);
			hoWaitTimer -= Control::LogicRate;
			if(hoWaitTimer<0)
			{
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S2_LasyaPrison;
				SendMessage(ControlExit, ID, HOSM_NewArea);
			}
		}
	}

	if( Control::Audio->IsPlaying(aTrackL7theme) )
	{
		Control::Audio->Samples[aTrackL7theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL7theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL7theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL7ho) )
				  Control::Audio->PlaySample(aTrackL7ho, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL7ho))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL7ho, true);
		}
	}
}

void L7_S2_HOLeftHand::CheckAndFindHOSymbol(int id)
{
	//If the clicked symbol is shown in HUD right now
	if(id >= hudSymbolGroupID*4 && id < (hudSymbolGroupID*4)+4 )
	{
		//symbol found
		symbolGlows[id]->FadeIn(1.5f);

		Control::Audio->QuickLoadAndPlaySFX("s2ho1_itemclicked");
		
		ProfileSystem::Instance->ActiveProfile.L7_S2_HOLeftHandSymbolFound[id] = true;
		numSceneSymbolsFound += 1;

		float valArr[5] = {0.65f, -1, -1, -1, -1};
		hudSymbols[id]->TweenTo(valArr, 0.85f, 0, true);		

		hoClickFX->SetPos(&symbolGlows[id]->Pos);
		hoClickFX->Start();

		//Show Particle Effects over HUD also
		symbolShowFX->SetPos(&hudSymbols[id]->Pos);
		symbolShowFX->Start();

		if(numSceneSymbolsFound>=12)
		{
			HOScene->GetObjectByID("handwithdisk")->FadeOut(2.0f);
			HOScene->GetObjectByID("diskarrow")->FadeOut(2.0f);
			diskRotateBtn->FadeOut(2.0f);
			HOScene->GetObjectByID("fingermask")->FadeOut(2.0f);
			HOScene->GetObjectByID("diskoverlayglow")->FadeOut(2.0f);

			s2hocompleteFX->Start();
			ProfileSystem::Instance->ActiveProfile.L7_S2_HOLeftHandCompleted = true;
			isHOCompleted = true;

			Control::Audio->QuickLoadAndPlaySFX("s2ho1_hocomplete");

			if (_handleExtrasGameComplete(false))
				return;

			AwardManager::GetInstance()->CompleteHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);

			HOScene->Shake(3,1,2.85f);
			
		}
	}
}


void L7_S2_HOLeftHand::OnHudSymbolTweenComplete(CObject* obj)
{
	isHudSymbolsSliding = false;

	for (int i = 0; i < MAX_L7S2_HANDHOSYMBOLS; i++)
	{
		if(i >= hudSymbolGroupID*4 && i < (hudSymbolGroupID*4)+4 )
			hudSymbols[i]->Show(true);
		else
			hudSymbols[i]->Show(false);
	}
}


void L7_S2_HOLeftHand::ShowNextHUDSymbols(bool rotateNeedle)
{
	int prevSymbolGroupID = hudSymbolGroupID;
	hudSymbolGroupID += 1;
	if(hudSymbolGroupID>2)
		hudSymbolGroupID = 0;

	if(rotateNeedle)
	{
		isHudSymbolsSliding = true;

		float rotate = HOScene->GetObjectByID("diskarrow")->GetRotate();
		float valArr[5] = {-1, -1, -1, -1, rotate - 120.0f};
		HOScene->GetObjectByID("diskarrow")->TweenTo(valArr, 0.75f, TW_EASEBACKOUT, true, CC_CALLBACK_1(L7_S2_HOLeftHand::OnHudSymbolTweenComplete,this));

		float offset = 600.0f;

		//Slide Off prev symbols
		int startID = prevSymbolGroupID*4;
		int endID = (prevSymbolGroupID*4)+4; //0-3, 4-7, 8-11
		for (int i = startID; i < endID; i++)
		{
			float valArr[5] = {-1, hudSymbols[i]->Pos.x - offset, -1, -1, -1};
			hudSymbols[i]->TweenTo(valArr, 0.75f, TW_EASEBACKOUT, true);
		}

		//Slide In Next symbols
		startID = hudSymbolGroupID*4;
		endID = (hudSymbolGroupID*4)+4; //0-3, 4-7, 8-11
		for (int i = startID; i < endID; i++)
		{
			int posID = i%4;
			hudSymbols[i]->Pos.x = hudPos[posID].x+offset;
			hudSymbols[i]->Pos.y = hudPos[posID].y;
			float valArr[5] = {-1, hudPos[posID].x, -1, -1, -1};
			hudSymbols[i]->TweenTo(valArr, 0.75f, TW_EASEBACKOUT, true);
		}
	}
	else
	{
		OnHudSymbolTweenComplete(NULL);
	}

	/*for (int i = 0; i < MAX_L7S2_HANDHOSYMBOLS; i++)
	{
		if(i >= hudSymbolGroupID*4 && i < (hudSymbolGroupID*4)+4 )
			hudSymbols[i]->Show(true);
		else
			hudSymbols[i]->Show(false);
	}*/
}

void L7_S2_HOLeftHand::Update()
{
	NavigateToScene();

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

#ifdef _CHEATS
	if(Control::Input->KpressedWithShift(K_VK_SPACE))
	{
		if(!ProfileSystem::Instance->ActiveProfile.L7_S2_HOLeftHandCompleted)
		{
			ProfileSystem::Instance->ActiveProfile.L7_S2_HOLeftHandCompleted = true;
			return;
		}
	}
#endif // _CHEATS

	CPoint pos = Control::Input->Mpos();
	bool isClicked = Control::Input->LBclicked();
	Cursor::SetMode(CA_Normal);
	if(isHudSymbolsSliding)
		return;

	if (ProfileSystem::Instance->ActiveProfile.L7_S2_HOLeftHandDiskPlaced)
	{
		if(isHOCompleted)
		{
			return;
		}

		if(isClicked)
		{
			for (int i = 0; i < MAX_L7S2_HANDHOSYMBOLS; i++)
			{
				//if(IsLeftHandSymbol[i])
				{
					//only check for Left hand symbols only
					if(!ProfileSystem::Instance->ActiveProfile.L7_S2_HOLeftHandSymbolFound[i] && symbolRects[i]->Intersection(&pos))
					{
						CheckAndFindHOSymbol(i);
						return;
					}
				}
			}
		}

		if(DiskRotateBtnRect.Intersection(&pos))
		{
			diskRotateBtn->PlayAnimation(1);
			Cursor::SetMode(CA_HandPointy);
			if( isClicked )
			{
				diskRotateBtn->PlayAnimation(2);
				ShowNextHUDSymbols();
			}
		}
		else
		{
			diskRotateBtn->PlayAnimation(0);
		}
	}
	else
	{
		if(InvDestRectSymbolDisk.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if( isClicked )
			{
				if(!ProfileSystem::Instance->ActiveProfile.L7_S2_InvSymbolDiscTaken)
					Hud->ShowBannerText("L7strange");
				else
					Hud->ShowBannerText("L7same");
			}
		}
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
		//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S2_LasyaPrison;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif	
	if( IsMouseOverGoBackArea(&pos) )
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L7_S2_LasyaPrison);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L7_S2_LasyaPrison);
#endif
		if( isClicked )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L7_S2_LasyaPrison, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S2_LasyaPrison;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
			
		}	
	}
}

void L7_S2_HOLeftHand::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		Hud->ShowMap();
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			if (itemVal == eInv_L7_S2_SymbolDisk && InvDestRectSymbolDisk.Intersection(&dropPos))
			{
				if(!ProfileSystem::Instance->ActiveProfile.L7_S2_HOLeftHandDiskPlaced)
				{
					
					ProfileSystem::Instance->ActiveProfile.L7_S2_HOLeftHandDiskPlaced = true;

					Control::Audio->QuickLoadAndPlaySFX("s2ho1_symboldiscused");

					Hud->ReleaseDrag();
					Hud->CloseInventory(); 

					HOScene->GetObjectByID("handwithdisk")->FadeIn(2.0f);
					HOScene->GetObjectByID("diskarrow")->FadeIn(2.0f);
					diskRotateBtn->FadeIn(2.0f);
					HOScene->GetObjectByID("fingermask")->FadeIn(2.0f);
					HOScene->GetObjectByID("diskoverlayglow")->FadeIn(2.0f);

					if(ProfileSystem::Instance->ActiveProfile.L7_S2_HORightHandDiskPlaced)
					{
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L7_S2_SymbolDisk] = false;
						Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L7_S2_SymbolDisk);
					}

					ActivateHO();
				}
			}
			else
				GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		


	}
}

void L7_S2_HOLeftHand::ProcessSkip()
{
}

void L7_S2_HOLeftHand::ShowHintForSymbols()
{
	int startID = hudSymbolGroupID*4;
	int endID = (hudSymbolGroupID*4)+4; //0-3, 4-7, 8-11
	int idToFind = -1;
	//Check if symbol is in HUD
	for (int i = startID; i < endID; i++)
	{
		//IsLeftHandSymbol[i] && 
		if(!ProfileSystem::Instance->ActiveProfile.L7_S2_HOLeftHandSymbolFound[i])
		{
			idToFind = i;
			break;
		}
	}

	//If the clicked symbol is shown in HUD right now
	if(idToFind!=-1)
	{
		//symbol found
		Hud->InitHintTrailEmit(symbolGlows[idToFind]->Pos.x, symbolGlows[idToFind]->Pos.y, false);
		AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	}
	else
	{
		Hud->InitHintTrailEmit(&DiskRotateBtnRect, true);
		AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	}
}

void L7_S2_HOLeftHand::ProcessNonHOHint()
{
	if(isHOCompleted)
		return;

	int iNonHOHint = 0; 
	int MaxNonHOHints = 8;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
			break;

		case 0: ++iNonHOHint;

			break;
		case 1: ++iNonHOHint;
			if (!ProfileSystem::Instance->ActiveProfile.L7_S2_HOLeftHandDiskPlaced)
			{
				if(ProfileSystem::Instance->ActiveProfile.L7_S2_InvSymbolDiscTaken)
				{
					Hud->ShowHintForInvItem(eInv_L7_S2_SymbolDisk, &InvDestRectSymbolDisk);
				}
				else
				{
					Hud->InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
				}
				return;
			}
			break;
		case 2: ++iNonHOHint;
				ShowHintForSymbols();
				return;
			break;
		case 3: ++iNonHOHint;

			break;
		case 4: ++iNonHOHint;

			break;
		case 5: ++iNonHOHint;

			break;
		case 6: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
				break;
		case 7: ++iNonHOHint;
				{
					Hud->teleportSystem->ShowTeleport();
					return;
				}
				break;

		}
	}
	while( iNonHOHint < MaxNonHOHints );

	Hud->ShowNothingToDoMsgBanner();
}

L7_S2_HOLeftHand::~L7_S2_HOLeftHand()
{
	if(Hud->IsHOScene)
	{
		Hud->InvBoxCtrl->SetLockBase(INVBOX_LR_FRAME);
		Hud->IsHOScene = false;
		Hud->HOState = HOInactive;
	}

	SAFE_DELETE(s2handhostarsFX);
	SAFE_DELETE(s2hocompleteFX);

	SAFE_DELETE(symbolShowFX);
	SAFE_DELETE(hoClickFX);

	for (int i = 0; i < MAX_L7S2_HANDHOSYMBOLS; i++)
	{
		SAFE_DELETE(symbolRects[i]);
	}

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL7s2LasyaPrisonAmb);
	Control::Audio->UnloadSample(aAmbL7s2LasyaPrisonAmb);
	//MUSIC_SFX
}
void L7_S2_HOLeftHand::NavigateToScene()
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