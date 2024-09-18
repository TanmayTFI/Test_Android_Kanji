

#include "L2_S7_SoldiersPuzzle.h"
#include "Util.h"
#include"Application.h"
#include"Hud.h"
#include"L3_S1_FortEntrance.h"
class ParticleEffect;
class CHOScene;




#pragma region SOLDIER PUZZLE
L2S7SoldiersPuzzle::L2S7SoldiersPuzzle()
{
	AwardManager::GetInstance()->StartHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	//AwardManager::GetInstance()->StartMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	_cVideoCutScene = NULL;
	time = 0;
	subIndex = 0;
	subMax = 13; 
	vidTimer = 0;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end3"));
	subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end4"));
	subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end5"));
	subtitles[5] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end6"));
	subtitles[6] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end7"));
	subtitles[7] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end8"));
	subtitles[8] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end9"));
	subtitles[9] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end10"));
	subtitles[10] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end11"));
	subtitles[11] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end12"));
	subtitles[12] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end13"));

	IsCutscenePlaying = false;
	_soldierBackLayer = NULL;
	_bgScene = NULL;
	_fog = NULL;
	_bg = NULL;
	_path = NULL;
	_whiteFlashBg = NULL;
	_whiteFlashBg = new CObject();
	_whiteFlashBg->LoadInit("LV2\\S7\\SoldierPuzzle\\White.SPR", 0, 0, eZOrder_Popup);
	_whiteFlashBg->SetScale(10);
	_whiteFlashBg->SetEnable(false);
	_p3ShieldClickCount = 0;

	_jsonAnimP4LeverPullingAnim = NULL;
	_jsonAnimP4LeverPlacingAnim = NULL;
	_isP4LeverOnHand = false;
	_p4Lever = NULL;
	_p4LeverHO = NULL;
	_p4LeverBase = NULL;
	_p5PrevSelectedShield = NULL;

	_p6ClothHO = NULL;
	_p6ClothOnHand = NULL;
	_isP6ClothOnHand = false;
	
	_bgFogVFX = NULL;
  
    rtGraphic =  NULL;
    
	LoadHud(INVENTORY_V1);
	Hud->InvBoxCtrl->SetLockBase(PUZBOX_FRAME);//lock in HO anim
	Hud->IsHOScene = true;
	Hud->HOState = HOActive;
	//	Hud->InvBox_StartPuzzleMode();

	const CRectangle  TmpRect(158.0f,39.0f,1059.0f,584.0f);




	InitializeWithProfileData();
	_isMouseInteractionEnabled = true;
	_gamePlayAreaRect->SetIsInteractionEnabled(true);

	Control::Audio->TurnOffLvlMscTracks();

	if( !Control::Audio->IsPlaying(aTrackL2s7Puzzle) )
		Control::Audio->PlaySample(aTrackL2s7Puzzle,true);

	Control::Audio->LoadSample(aAmbs7ArmorPuzzle,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbs7ArmorPuzzle) )		
	{
		Control::Audio->PlaySample(aAmbs7ArmorPuzzle,true);
	}

	//Journal Entry Add
	if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L2P7_MeetTheSoldiers])
	{
		//Flag Set
		ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L2P7_MeetTheSoldiers] = true;

		//Add to Note Array
		ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L2P7_MeetTheSoldiers;

		ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

		//Increment Notes
		ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
		Hud->ShowJrnEntryFX();
	}

}

L2S7SoldiersPuzzle::~L2S7SoldiersPuzzle()
{


	UnLoadPuzzle();

	if( rtGraphic != NULL )
    {
       delete rtGraphic; rtGraphic = NULL;
    }

	SAFE_DELETE(_soldierBackLayer);
	SAFE_DELETE(_bgScene);
	SAFE_DELETE(_whiteFlashBg);
	SAFE_DELETE(_jsonAnimP4LeverPullingAnim);
	SAFE_DELETE(_jsonAnimP4LeverPlacingAnim);
	SAFE_DELETE(_cVideoCutScene);
	SAFE_DELETE(subtitleText);
	_p4Lever = NULL;
	_p4LeverHO = NULL;
	_p4LeverBase = NULL;
	//Hud->InvBox_StopPuzzleMode();
	//MUSIC_SFX
	//Quick SFX unloaded in HUD
	Hud->InvBoxCtrl->ResetToInvBox();
	Control::Audio->StopSample(aAmbs7ArmorPuzzle);
	Control::Audio->UnloadSample(aAmbs7ArmorPuzzle);
	//MUSIC_SFX
}

void L2S7SoldiersPuzzle::InitializeWithProfileData()
{
	_currentPuzzleNumber = ProfileSystem::Instance->ActiveProfile.L2_S7_CurrentPuzzle;
	K_LOG("SOLDIER PUZZLE NUM: %d",_currentPuzzleNumber);
	//_currentPuzzleNumber = 5;
	// ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState = eSoldierPuzzleState_p4_ShldOpened;
	LoadPuzzle(_currentPuzzleNumber);
}

void L2S7SoldiersPuzzle::LoadPuzzle(int puzzleNumber)
{

	ProfileSystem::Instance->ActiveProfile.L2_S7_CurrentPuzzle = puzzleNumber;
	//Hud->InvBox_StartPuzzleMode();
	Hud->InvBoxCtrl->SetLockBase(PUZBOX_FRAME);
	_soldierBackLayer = NULL;
	_bgScene = NULL;

	_fog = NULL;
	_bg = NULL;
	_path = NULL;

	_jsonAnimP4LeverPullingAnim = NULL;
	_jsonAnimP4LeverPlacingAnim = NULL;
	_p4Lever = NULL;
	_p4LeverHO = NULL;
	_p4LeverBase = NULL;

	_p5PrevSelectedShield = NULL;

	_p6ClothHO = NULL;
	_p6ClothOnHand = NULL;
	_web0 = NULL;
	_web1 = NULL;
	_jsonAnimP6Cloth0 = NULL;
	_jsonAnimP6Cloth1 = NULL;
	_gamePlayAreaRect = NULL;
	_isWrongClick = true;


	SoldierPuzzleState puzzleState = (SoldierPuzzleState)ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState;
	if (!HOScene)
	{
		HOScene = new CHOScene();
	}
    bool isScreenshotSuccess = false;
	if (puzzleNumber < 7)
	{
		_bgScene = new CHOScene();
		LoadLayer(puzzleNumber + 1, _bgScene);

		_bg->SetEnable(false);
		if (_path)
		{
			_path->SetEnable(false);
		}
		//_soldierBackLayer = GFInstance->CreateSceneScreenshot(_bgScene, &isScreenshotSuccess);
        //isScreenshotSuccess = GFInstance->CreateSceneScreenshot(_bgScene, _soldierBackLayer);
    #if !defined (K_ANDROID) && !defined (K_MACOS) && !defined (K_iOS)
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		isScreenshotSuccess = false;
		screenshot = new CObject();
		screenshot->LoadSprite("UI\\ZOOMBG.SPR");
		screenshot->SetZ(0);
		screenshot->SetEnable(false);	

		bool isScnBg1024Res = false;
        if( rtGraphic != NULL )
            rtGraphic->freePicture();
        rtGraphic =  NULL;
        rtGraphic =  KPTK::createKGraphic();
		bool isRTGraphic = rtGraphic->makeRenderTarget(1366,768,true,true,true);
		if( !isRTGraphic )
		{
			//try a 1024 render target
			isRTGraphic = rtGraphic->makeRenderTarget(1024,768,true,true,true);
			isScnBg1024Res = true;
			if( !isRTGraphic )
			{
				K_LOG("CurrentSceneGraphic failed, Render Target not supported");
				/*
				#ifdef K_WIN32
					FATAL((const char*)L"Er0x1602- Game uses render targets. Your video memory device is not compatible with this game. Please contact customer support for assistance!"); 
				#else
					FATAL((const char*)"Er0x1602- Game uses render targets. Your video memory device is not compatible with this game. Please contact customer support for assistance!"); 
				#endif
				 */
				isScnBg1024Res = false;
			
			}
		}
		if(isRTGraphic){
			rtGraphic->clearRenderTarget();
			rtGraphic->beginRenderToTarget();//RENDER TO TARGET STARTS

			if(isScnBg1024Res)
			{
				HOScene->RenderToTarget(-171.0f);
			}
			else
			{
				HOScene->RenderToTarget();
			}
			rtGraphic->endRenderToTarget();
			screenshot->pSprite->Image = rtGraphic;
			isScreenshotSuccess = true;
			_soldierBackLayer = screenshot;
		}
    #endif
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        _bgScene->Show(false);
		//_bgScene->DeleateAllObjects();
		for (int i = 0; i < (int)(_arrayOfSoldiers.size()); i++)
		{
			delete _arrayOfSoldiers.at(i);
		}
		_arrayOfSoldiers.clear();
		if(isScreenshotSuccess)_soldierBackLayer->SetEnable(true);
		if(isScreenshotSuccess)_soldierBackLayer->Show(true);
	}

	LoadLayer(puzzleNumber, HOScene);
	if (puzzleNumber < 7)
	{
		CPoint zoomOrigin = CPoint(Application::CurrentResolution.Width*0.5f, Application::CurrentResolution.Height*0.5f,(float)(_fog->ZOrder - 1));
		if(isScreenshotSuccess)_soldierBackLayer->SetPivot(_soldierBackLayer->GetWorldRect().w/2, _soldierBackLayer->GetWorldRect().h/2);
		if(isScreenshotSuccess)_soldierBackLayer->SetZ(_fog->ZOrder - 1);
		if(isScreenshotSuccess)_soldierBackLayer->SetScale(0.5);
	}

	if (puzzleNumber != 1 && puzzleNumber != 7)
	{
		int soldierNums[7] = {2, 3, 4, 3, 4, 3, 2};
		int rightArrowIndies[7] = {0, 1, 0, 0, 2, 0 , 0};
		std::string str = std::string("P")+MKSTR(puzzleNumber)+std::string("_Sol_Opened_0");
		CObject * firstSoldier = HOScene->GetObjectByID(str);
		str = "";
		str = std::string("P")+MKSTR(puzzleNumber)+std::string("_Sol_Opened_")+MKSTR(soldierNums[puzzleNumber - 1] - 1);
		CObject * lastSoldier = HOScene->GetObjectByID(str);
		float xDistanceBetweenSoldiers = (lastSoldier->Pos.x - firstSoldier->Pos.x) / (soldierNums[puzzleNumber - 1] - 1);
		for (int i = 0; i < soldierNums[puzzleNumber -1] - 1; i++)
		{
			float xPos = firstSoldier->Pos.x + (xDistanceBetweenSoldiers / 2) + (xDistanceBetweenSoldiers * i);
			CPoint * pos = PointSystem::CreateCPoint(xPos, firstSoldier->Pos.y + 100 , firstSoldier->Pos.z);
			UIButton * arrowBtn = NULL;
			if (i == rightArrowIndies[puzzleNumber - 1])
			{
				arrowBtn = new UIButton("LV2\\S7\\SoldierPuzzle\\ArrowBtn.SPR", pos->x, pos->y, 100, CC_CALLBACK_1(L2S7SoldiersPuzzle::OnRightArrowClick, this));
			}
			else
			{
				arrowBtn = new UIButton("LV2\\S7\\SoldierPuzzle\\ArrowBtn.SPR", pos->x, pos->y, 100, CC_CALLBACK_1(L2S7SoldiersPuzzle::OnWrongArrowClick, this));
			}

			if (!_isSoldiersOpened)
			{
				arrowBtn->SetEnable(false);
				arrowBtn->SetIsInteractionEnabled(false);
			}

			_arrayOfArrowBtns.push_back(arrowBtn);
			_arrayOfInteractiveObjects.push_back(arrowBtn);


			//delete pos;
		}
	}

	switch (puzzleNumber)
	{
	case 1:
		{
			StartSoldiersOpenAnim();
		}
		break;
	case 2:
		{
			if (puzzleState < eSoldierPuzzleState_P2_ShldRotated)
			{
				CObject * shield = HOScene->GetObjectByID("P2_Shld_Closed_0");
				shield->SetMouseClickCallback(CC_CALLBACK_1(L2S7SoldiersPuzzle::OnP2ShieldClick, this));
				_arrayOfInteractiveObjects.push_back(shield);
				shield->SetRotate(180);
			}
			if (_isSoldiersOpened)
			{
				HOScene->GetObjectByID("P2_ShieldLowerDoor_1")->Show(false);
				HOScene->GetObjectByID("P2_ShieldUpperDoor_1")->Show(false);
			}
		}
		break;
	case 3:
		{
			_p3ShieldClickCount = 0;
			if (puzzleState < eSoldierPuzzleState_p3_ShldOpened)
			{
				for (int i = 0; i < 4; i++)
				{

					L2S7Shield * shield = new L2S7Shield();
					shield->shieldId = i;
					shield->shieldClosed = HOScene->GetObjectByID(std::string("P3_Shld_Closed_") + MKSTR(i));
					shield->shieldClosed->SetMouseClickCallback(CC_CALLBACK_1(L2S7SoldiersPuzzle::OnP3ShieldClick, this), (CObject*)shield);
					_arrayOfInteractiveObjects.push_back(shield->shieldClosed);
					shield->shieldYellowGlow = HOScene->GetObjectByID(std::string("P3_ShieldYellowGlow_") + MKSTR(i));
					shield->shieldYellowGlow->Show(false);
					shield->shieldRedGlow = HOScene->GetObjectByID(std::string("P3_ShieldRedGlow_") + MKSTR(i));
					shield->shieldRedGlow->Show(false);
					_arrayOfShields.push_back(shield);
				}

			}
			else
			{
				for (int i = 0; i < 4; i++)
				{
					HOScene->GetObjectByID(std::string("P3_ShieldYellowGlow_") + MKSTR(i))->Show(false);
					HOScene->GetObjectByID(std::string("P3_ShieldRedGlow_") + MKSTR(i))->Show(false);
				}
				_p3Stone0->Show(true);
				_p3Stone1->Show(true);

			}
		}
		break;

	case 4:
		{
			_p4Lever = HOScene->GetObjectByID("P4_LeverHandle");
			_p4LeverBase = HOScene->GetObjectByID("P4_LeverBase");
			_p4LeverHO = HOScene->GetObjectByID("P4_LeverHO");


			int ArrayPts[] = {519,515,575,469,600,507,546,562};
			CPolygon * leverRect =  new CPolygon(4);
			leverRect->Init(ArrayPts);
			_p4Lever->SetMouseClickProperties(leverRect , CC_CALLBACK_1(L2S7SoldiersPuzzle::OnP4LeverClicked, this));

			if (ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState == eSoldierPuzzleState_p4_LeverFixed)
			{
				_jsonAnimP4LeverPullingAnim->SetEnableObjects(true);
				_jsonAnimP4LeverPullingAnim->SetAnimTime(0);
				_p4LeverHO->SetEnable(false);
			}

			if (ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState < eSoldierPuzzleState_p4_LeverFixed)
			{
				int ArrayPts[] = {493,502,580,465,577,602,500,604};
				CPolygon * leverPlaceArearRect=  new CPolygon(4);
				leverPlaceArearRect->Init(ArrayPts);
				_p4LeverBase->SetMouseClickProperties(leverPlaceArearRect, CC_CALLBACK_1(L2S7SoldiersPuzzle::OnP4LeverBaseAreaClicked, this));
				_p4LeverHO->SetMouseClickCallback(CC_CALLBACK_1(L2S7SoldiersPuzzle::OnP4LeverHOClicked, this));
				_arrayOfInteractiveObjects.push_back(_p4LeverHO);
			}

			else if (ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState == eSoldierPuzzleState_p4_ShldOpened)
			{
				_jsonAnimP4LeverPullingAnim->SetEnableObjects(true);
				_jsonAnimP4LeverPullingAnim->SetAnimTime(1);
				_p4LeverHO->SetEnable(false);
			}
		}
		break;

	case 5:
		{

			int shuffledShieldIndices[4] = {3, 0, 1, 2};
			for (int i = 0; i < 4; i++)
			{

				L2S7Shield * shield = new L2S7Shield();
				shield->shieldId = i;
				shield->shieldCurrentIndex = shuffledShieldIndices[i];
				shield->shieldSolutionIndex = i;
				shield->shieldSymbol = HOScene->GetObjectByID(std::string("P5_ShieldSymbol_") + MKSTR(shield->shieldId));
				shield->shieldSymbol->SetMouseClickCallback(CC_CALLBACK_1(L2S7SoldiersPuzzle::OnP5ShieldClick, this), (CObject*)shield);
				_arrayOfInteractiveObjects.push_back(shield->shieldSymbol);
				shield->shieldDoorDown = HOScene->GetObjectByID(std::string("P5_ShieldLowerDoor_") + MKSTR(shield->shieldId));
				shield->shieldDoorUp = HOScene->GetObjectByID(std::string("P5_ShieldUpperDoor_") + MKSTR(shield->shieldId));
				shield->shieldYellowGlow = HOScene->GetObjectByID(std::string("P5_ShieldYellowGlow_") + MKSTR(shield->shieldId));
				shield->shieldYellowGlow->Show(false);
				shield->shieldMask = HOScene->GetObjectByID(std::string("P5_ShieldMask_") + MKSTR(shield->shieldId));

				_arrayOfShields.push_back(shield);


				L2S7Soldier * soldier = _arrayOfSoldiers.at(i);
				soldier->symbol = HOScene->GetObjectByID(std::string("P5_SoldierSymbol_") + MKSTR(i));

				if (_isSoldiersOpened)
				{
					shield->shieldSymbol->Show(false);
					shield->shieldDoorDown->Show(false);
					shield->shieldDoorUp->Show(false);
					shield->shieldYellowGlow->Show(false);
					shield->shieldSymbol->SetIsInteractionEnabled(false);
					shield->shieldMask->Show(false);
					soldier->symbol->Show(false);

				}
			}
		}
		break;

	case 6:
		{
			if (puzzleState < eSoldierPuzzleState_p6_ShldOpened)
			{
				for (int i = 0; i < 3; i++)
				{

					L2S7Shield * shield = new L2S7Shield();
					shield->shieldId = i;
					shield->shieldClosed = HOScene->GetObjectByID(std::string("P6_Shld_Closed_") + MKSTR(shield->shieldId));
					shield->shieldClosed->SetMouseClickCallback(CC_CALLBACK_1(L2S7SoldiersPuzzle::OnP6ShieldClick, this), (CObject*)shield);
					_arrayOfInteractiveObjects.push_back(shield->shieldClosed);
					shield->shieldArrow = HOScene->GetObjectByID(std::string("P6_ShldArrow_Closed_") + MKSTR(shield->shieldId));
					shield->shieldArrow->SetPivot(0,17.5);
					shield->shieldArrowGlow = HOScene->GetObjectByID(std::string("P6_ShldArrowGlow_") + MKSTR(shield->shieldId));
					shield->shieldArrowGlow->SetPivot(0,17.5);
					shield->shieldArrowGlow->Show(false);
					shield->shieldArrowGlow->SetXY(shield->shieldArrow->Pos.x, shield->shieldArrow->Pos.y);
					if (puzzleState < eSoldierPuzzleState_p6_Web1Removed)
					{
						shield->shieldClosed->SetIsInteractionEnabled(false);
					}
					//shield->shieldArrowCenter = HOScene->GetObjectByID(std::string("P6_ShldCenterPoint_Closed_") + MKSTR(shield->shieldId));
					_arrayOfShields.push_back(shield);
				}
			}


			_web0->Show(false);
			_web1->Show(false);
			_p6ClothHO->Show(false);
			_p6ClothOnHand->Show(false);
			_p6ClothOnHand->SetEnable(false);
			_p6ClothHO->SetEnable(false);

			int ArrayPts[] = {413,574,458,582,477,557,451,547};
			CPolygon * clothPoly =  new CPolygon(4);
			clothPoly->Init(ArrayPts);
			_p6ClothHO->SetMouseClickProperties(clothPoly , CC_CALLBACK_1(L2S7SoldiersPuzzle::OnP6ClothHOClicked, this));
			_p6ClothHO->SetIsInteractionEnabled(false);

			if (ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState < eSoldierPuzzleState_p6_Web0Removed)
			{
				_web0->Show(true);
				_web1->Show(true);
				_web0->SetMouseClickCallback(CC_CALLBACK_1(L2S7SoldiersPuzzle::OnP6WebAreaClicked, this), (CObject*)_arrayOfShields.at(0));
				_arrayOfInteractiveObjects.push_back(_web0);
				_web1->SetMouseClickCallback(CC_CALLBACK_1(L2S7SoldiersPuzzle::OnP6WebAreaClicked, this), (CObject*)_arrayOfShields.at(2));
				_arrayOfInteractiveObjects.push_back(_web1);
				_p6ClothHO->SetEnable(true);
				_p6ClothHO->Show(true);
				_p6ClothHO->SetIsInteractionEnabled(true);
				_arrayOfInteractiveObjects.push_back(_p6ClothHO);
				_numOfWebsRemoved = 0;
			}
			else if (ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState == eSoldierPuzzleState_p6_Web0Removed)
			{
				_web1->Show(true);
				_web1->SetMouseClickCallback(CC_CALLBACK_1(L2S7SoldiersPuzzle::OnP6WebAreaClicked, this), (CObject*)_arrayOfShields.at(2));
				_arrayOfInteractiveObjects.push_back(_web1);
				_p6ClothHO->SetEnable(true);
				_p6ClothHO->Show(true);
				_p6ClothHO->SetIsInteractionEnabled(true);
				_arrayOfInteractiveObjects.push_back(_p6ClothHO);
				_numOfWebsRemoved = 1;
			}
			else if (ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState == eSoldierPuzzleState_p6_Web1Removed)
			{
				_web0->Show(true);
				_web0->SetMouseClickCallback(CC_CALLBACK_1(L2S7SoldiersPuzzle::OnP6WebAreaClicked, this), (CObject*)_arrayOfShields.at(2));
				_arrayOfInteractiveObjects.push_back(_web0);
				_p6ClothHO->SetEnable(true);
				_p6ClothHO->Show(true);
				_p6ClothHO->SetIsInteractionEnabled(true);
				_arrayOfInteractiveObjects.push_back(_p6ClothHO);
				_numOfWebsRemoved = 1;
			}
		}
		break;

	case 7:
		{
			puzzleState = eSoldierPuzzleState_p7_Cutscene;
			if( GFInstance->IsReplayingGame )
			{
				if(!ProfileSystem::Instance->ActiveProfile.IsMainChapterCompleted) 
				{
					GFInstance->IsReplayingGame = false;
				}
				bool isPendingCollectableInLevel = false;
				if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S1Collected || 
				    !ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S2Collected ||
					!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S3Collected ||
					!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S4Collected )
				{
					isPendingCollectableInLevel = true;		
					if( !Hud->MessageText->Visible && Hud->BannerAlpha == 0.0f && Hud->BannerState == eHMS_Idle)//avoid looped banner calls!!
						Hud->ShowBannerText("L1mismo");
				}
				else 
				{
					for( int i = 0; i < 5; i++ )				
						if( !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[(ProfileSystem::Instance->ActiveProfile.CurrentLevel*5)+i] )
						{
							isPendingCollectableInLevel = true;
							if( !Hud->MessageText->Visible && Hud->BannerAlpha == 0.0f && Hud->BannerState == eHMS_Idle)//avoid looped banner calls!!
								 Hud->ShowBannerText("L1mismc");
							break;
						}
				}

				//No pending collectables!!
				if( !isPendingCollectableInLevel && !IsCutscenePlaying)
				{
					_isMouseInteractionEnabled = false;
					//CRectangle RectVid(171, 96, 1024, 576);
					CRectangle RectVid(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
					_cVideoCutScene = new CVideo("OGV\\L2\\S6ENDCUTSCENE.OGV", RectVid, false, eZOrder_CutsceneFull);
					//_cVideoCutScene->SetScale(1.334f);
					_cVideoCutScene->SetScale(GFApp->video_aspectRatio);
					_cVideoCutScene->PlayVideo();
					_cVideoCutScene->SetSkipActive();
					IsCutscenePlaying = true;
					Hud->EnableInput(false);
					Hud->HideHud(false);
					if( Control::Audio->IsPlaying(aTrackL2s7Puzzle) )
						Control::Audio->StopSample(aTrackL2s7Puzzle);
					if( Control::Audio->IsPlaying(aAmbs7ArmorPuzzle) )		
						Control::Audio->StopSample(aAmbs7ArmorPuzzle);	
				}	
			}
			else if( !IsCutscenePlaying )
			{
				_isMouseInteractionEnabled = false;
				//CRectangle RectVid(171, 96, 1024, 576);
				CRectangle RectVid(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
				_cVideoCutScene = new CVideo("OGV\\L2\\S6ENDCUTSCENE.OGV", RectVid, false, eZOrder_CutsceneFull);
				//_cVideoCutScene->SetScale(1.334f);
				_cVideoCutScene->SetScale(GFApp->video_aspectRatio);
				_cVideoCutScene->PlayVideo();
				_cVideoCutScene->SetSkipActive();
				IsCutscenePlaying = true;
				Hud->EnableInput(false);
				Hud->HideHud(false);
				if( Control::Audio->IsPlaying(aTrackL2s7Puzzle) )
					Control::Audio->StopSample(aTrackL2s7Puzzle);
				if( Control::Audio->IsPlaying(aAmbs7ArmorPuzzle) )		
					Control::Audio->StopSample(aAmbs7ArmorPuzzle);
			}
		}
		break;

	default:
		break;
	}



	_bgFogVFX = new SuperFX("PRT\\L2\\S7SoldierPuzzleFog.INI", PointSystem::CreateCPoint(683,384,0), 1100); 
	_bgFogVFX->Start();
	_bgFogVFX->AdvanceByTime(2.0f);


	_gamePlayAreaRect = new ClickableRectangle(0, 158.0f,39.0f,1059.0f,584.0f, CC_CALLBACK_1(L2S7SoldiersPuzzle::OnGamePlayAreaClick, this));
	_gamePlayAreaRect->SetIsInteractionEnabled(false);

}

void L2S7SoldiersPuzzle::LoadLayer(int layerNumber, CHOScene* scene)
{
	_bg = NULL;
	_path = NULL;
	_fog = NULL;
	int soldierNums[7] = {2, 3, 4, 3, 4, 3, 2};
	std::string pathIds[7] = {"P1_Path1", "P2_Path2", "P3_Path3", "P4_Path2", "P5_Path3", "P6_Path2", "P7_Path1"};
	std::string solOpenAnimStr = "";
	std::string solAttackAnimStr = "";
	_isSoldiersOpened = false;


	switch (layerNumber)
	{
	case 1:
		{
			scene->Load("LV2\\S7\\SOLDIERPUZZLE\\P1.SCN");
			solOpenAnimStr = "ANIM\\L2\\S7P1SoldierOpeningAnim_";

		}
		break;
	case 2:
		{
			scene->Load("LV2\\S7\\SOLDIERPUZZLE\\P2.SCN");
			solOpenAnimStr = "ANIM\\L2\\S7P2SoldierOpeningAnim_";
			solAttackAnimStr = "ANIM\\L2\\S7P2SoldierAttackingAnim_";
			if(ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState == eSoldierPuzzleState_p2_ShldOpened)
			{
				_isSoldiersOpened = true;
			}

		}
		break;

	case 3:
		{
			scene->Load("LV2\\S7\\SOLDIERPUZZLE\\P3.SCN");
			solOpenAnimStr = "ANIM\\L2\\S7P3SoldierOpeningAnim_";
			solAttackAnimStr = "ANIM\\L2\\S7P3SoldierAttackingAnim_";
			_p3Stone0 = scene->GetObjectByID("P3_Stone0");
			_p3Stone1 = scene->GetObjectByID("P3_Stone1");
			_p3Stone0->Show(false);
			_p3Stone1->Show(false);
			if(ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState == eSoldierPuzzleState_p3_ShldOpened)
			{
				_isSoldiersOpened = true;
			}
		}
		break;

	case 4:
		{
			scene->Load("LV2\\S7\\SOLDIERPUZZLE\\P4.SCN");

			CObject* temp = new CObject();
			temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L2\\CHEATERS2.SPR", 979, 373, scene->GetObjectByID("P4_LeverHO")->ZOrder + 5);
			temp->SetUID("cheater2Txt");
			scene->PushObject(temp);

			temp = new CObject();
			temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L2\\CHEATERS.SPR", 683, 294, scene->GetObjectByID("P4_LeverHO")->ZOrder + 5);
			temp->SetUID("cheaterTxt");
			scene->PushObject(temp);

			solOpenAnimStr = "ANIM\\L2\\S7P4SOLDIEROPENINGANIM_";
			solAttackAnimStr = "ANIM\\L2\\S7P4SOLDIERATTACKINGANIM_";

			_jsonAnimP4LeverPullingAnim = new JSONAnimator(1, CC_CALLBACK_1(L2S7SoldiersPuzzle::OnP4LeverPullingAnimComplete, this));
			_jsonAnimP4LeverPullingAnim->parseAndLoadJSON("ANIM\\L2\\S7P4LEVERPULLINGANIM.JSON", scene);
			_jsonAnimP4LeverPullingAnim->PlayAnim();
			_jsonAnimP4LeverPullingAnim->StopAnim();
			_jsonAnimP4LeverPullingAnim->SetAnimTime(0);
			_jsonAnimP4LeverPullingAnim->SetEnableObjects(false);

			_jsonAnimP4LeverPlacingAnim = new JSONAnimator(1, CC_CALLBACK_1(L2S7SoldiersPuzzle::OnP4LeverPlacingAnimComplete, this));
			_jsonAnimP4LeverPlacingAnim->parseAndLoadJSON("ANIM\\L2\\S7P4LEVERPLACINGANIM.JSON", scene);
			_jsonAnimP4LeverPlacingAnim->PlayAnim();
			_jsonAnimP4LeverPlacingAnim->StopAnim();
			_jsonAnimP4LeverPlacingAnim->SetAnimTime(0);
			_jsonAnimP4LeverPlacingAnim->SetEnableObjects(false);

			scene->GetObjectByID("P4_LeverHolder")->SetEnable(true);
			if(ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState == eSoldierPuzzleState_p4_ShldOpened)
			{
				_isSoldiersOpened = true;
			}
		}
		break;

	case 5:
		{
			scene->Load("LV2\\S7\\SOLDIERPUZZLE\\P5.SCN");
			solOpenAnimStr = "ANIM\\L2\\S7P5SOLDIEROPENINGANIM_";
			solAttackAnimStr = "ANIM\\L2\\S7P5SOLDIERATTACKINGANIM_";
			if(ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState == eSoldierPuzzleState_p5_ShldOpened)
			{
				_isSoldiersOpened = true;
			}
		}
		break;

	case 6:
		{
			scene->Load("LV2\\S7\\SOLDIERPUZZLE\\P6.SCN");
			solOpenAnimStr = "ANIM\\L2\\S7P6SOLDIEROPENINGANIM_";
			solAttackAnimStr = "ANIM\\L2\\S7P6SOLDIERATTACKINGANIM_";

			_p6ClothHO = scene->GetObjectByID("P6_ClothHO");
			_p6ClothOnHand = scene->GetObjectByID("P6_ClothOnHand");
			_p6ClothOnHand->SetEnable(false);
			_p6ClothOnHand->Show(false);
			_web0 = scene->GetObjectByID("P6_Sol_Web_0");
			_web1 = scene->GetObjectByID("P6_Sol_Web_2");


			_jsonAnimP6Cloth0 = new JSONAnimator(1, CC_CALLBACK_1(L2S7SoldiersPuzzle::OnP6ClothAnimComplete, this));
			_jsonAnimP6Cloth0->parseAndLoadJSON("ANIM\\L2\\S7P6CLOTHANIM_0.JSON", scene);
			_jsonAnimP6Cloth0->PlayAnim();
			_jsonAnimP6Cloth0->StopAnim();
			_jsonAnimP6Cloth0->SetAnimTime(0);
			_jsonAnimP6Cloth0->SetEnableObjects(false);


			_jsonAnimP6Cloth1 = new JSONAnimator(2, CC_CALLBACK_1(L2S7SoldiersPuzzle::OnP6ClothAnimComplete, this));
			_jsonAnimP6Cloth1->parseAndLoadJSON("ANIM\\L2\\S7P6CLOTHANIM_1.JSON", scene);
			_jsonAnimP6Cloth1->PlayAnim();
			_jsonAnimP6Cloth1->StopAnim();
			_jsonAnimP6Cloth1->SetAnimTime(0);
			_jsonAnimP6Cloth1->SetEnableObjects(false);


			_web0->SetEnable(true);
			_web1->SetEnable(true);
			if(ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState == eSoldierPuzzleState_p6_ShldOpened)
			{
				_isSoldiersOpened = true;
			}
		}
		break;
	case 7:
		{
			scene->Load("LV2\\S7\\SOLDIERPUZZLE\\P7.SCN");
			solOpenAnimStr = "ANIM\\L2\\S7P7SoldierOpeningAnim_";
			solAttackAnimStr = "ANIM\\L2\\S7P7SoldierAttackingAnim_";
		}
		break;

	default:
		break;
	}

	std::string str = MKSTR("P") + MKSTR(layerNumber) + MKSTR("_SoldierPuzzleBG");
	_bg = scene->GetObjectByID(str);

	str = MKSTR("P") + MKSTR(layerNumber) + MKSTR("_Fog");
	_fog = scene->GetObjectByID(str);


	_path = scene->GetObjectByID(pathIds[layerNumber - 1]);

	for (int i = 0; i < soldierNums[layerNumber - 1]; i++)
	{
		L2S7Soldier * soldier = new L2S7Soldier(layerNumber);
		if (i == 0)
		{
			soldier->jsonAnimOpening = new JSONAnimator(1, CC_CALLBACK_1(L2S7SoldiersPuzzle::OnSoldiersOpenAnimComplete, this));
			if (layerNumber != 1 && layerNumber != 7)
			{
				soldier->jsonAnimAttacking = new JSONAnimator(1, CC_CALLBACK_1(L2S7SoldiersPuzzle::OnSoldiersAttackAnimComplete, this));
				std::string str = solAttackAnimStr + MKSTR(i) + std::string(".JSON");
				soldier->jsonAnimAttacking->parseAndLoadJSON(str, scene);
				soldier->jsonAnimAttacking->PlayAnim();
				soldier->jsonAnimAttacking->StopAnim();
				soldier->jsonAnimAttacking->SetAnimTime(0);
				soldier->jsonAnimAttacking->SetEnableObjects(false);
			}

		}
		else
		{
			soldier->jsonAnimOpening = new JSONAnimator(1);
			if (i == (soldierNums[layerNumber - 1] - 1) && layerNumber != 1 && layerNumber != 7)
			{
				soldier->jsonAnimAttacking = new JSONAnimator(1);
				std::string str = solAttackAnimStr + MKSTR(i) + std::string(".JSON");
				soldier->jsonAnimAttacking->parseAndLoadJSON(str, scene);
				soldier->jsonAnimAttacking->PlayAnim();
				soldier->jsonAnimAttacking->StopAnim();
				soldier->jsonAnimAttacking->SetAnimTime(0);
				soldier->jsonAnimAttacking->SetEnableObjects(false);
			}
		}
		std::string str = solOpenAnimStr + MKSTR(i) + std::string(".JSON");
		soldier->jsonAnimOpening->parseAndLoadJSON(str, scene);
		soldier->jsonAnimOpening->PlayAnim();
		soldier->jsonAnimOpening->StopAnim();
		soldier->jsonAnimOpening->SetAnimTime(0);
		if (_isSoldiersOpened)
		{
			soldier->SetState(eL2S7SoldiersState_Opened);
		}
		_arrayOfSoldiers.push_back(soldier);
	}
}

void L2S7SoldiersPuzzle::LoadNextPuzzle()
{
	UnLoadPuzzle();
	_currentPuzzleNumber++;

	LoadPuzzle(_currentPuzzleNumber);
}

void L2S7SoldiersPuzzle::UnLoadPuzzle()
{
	delete HOScene;
	HOScene = NULL;
    
	for (int i = 0; i < (int)(_arrayOfSoldiers.size()); i++)
	{
		delete _arrayOfSoldiers.at(i);
	}
	_arrayOfSoldiers.clear();

	for (int i = 0; i < (int)(_arrayOfArrowBtns.size()); i++)
	{
		delete _arrayOfArrowBtns.at(i);
	}
	_arrayOfArrowBtns.clear();

	for (int i = 0; i < (int)(_arrayOfShields.size()); i++)
	{
		delete _arrayOfShields.at(i);
	}
	_arrayOfShields.clear();

	_arrayOfInteractiveObjects.clear();

	SAFE_DELETE(_soldierBackLayer);
	SAFE_DELETE(_bgScene);

	SAFE_DELETE(_jsonAnimP4LeverPullingAnim);
	SAFE_DELETE(_jsonAnimP4LeverPlacingAnim);

	SAFE_DELETE(_jsonAnimP6Cloth0);
	SAFE_DELETE(_jsonAnimP6Cloth1);
	SAFE_DELETE(_gamePlayAreaRect);
	SAFE_DELETE(_bgFogVFX);

	_isP4LeverOnHand = false;
	_isP6ClothOnHand = false;
}

void L2S7SoldiersPuzzle::MoveToNextLayer()
{
	_isMouseInteractionEnabled = false;
	Cursor::SetMode(CA_Normal);
	FlashScreen();

    // Rendertarget functionality is OFF by default for all Android devices.
    
    // Quick hack for MAC OS if there is Rendertarget enabled mac, the current build crashes in those mac as per a client
    // as we dont have a Mac with rendertarget enabled, we cant test it, so we are forcefully disabling rendertarget functionality for this build.

#if !defined (K_ANDROID) && !defined (K_MACOS) && !defined (K_iOS)

	bool isScreenshotsuccess = false;
	//_screenShot = GFInstance->CreateSceneScreenshot(HOScene, &isScreenshotsuccess);
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//CObject * screenshot = new CObject();
	screenshot->LoadSprite("UI\\ZOOMBG.SPR");
	screenshot->SetZ(0);
	screenshot->SetEnable(false);	

	bool isScnBg1024Res = false;
    if( rtGraphic != NULL )
        rtGraphic->freePicture();
    rtGraphic = NULL;
	rtGraphic =  KPTK::createKGraphic();
	bool isRTGraphic = rtGraphic->makeRenderTarget(1366,768,true,true,true);
	if( !isRTGraphic )
	{
		//try a 1024 render target
		isRTGraphic = rtGraphic->makeRenderTarget(1024,768,true,true,true);
		isScnBg1024Res = true;
		if( !isRTGraphic )
		{
			K_LOG("CurrentSceneGraphic failed, Render Target not supported");
            /*
            #ifdef K_WIN32
				FATAL((const char*)L"Er0x1602- Game uses render targets. Your video memory device is not compatible with this game. Please contact customer support for assistance!"); 
			#else
				FATAL((const char*)"Er0x1602- Game uses render targets. Your video memory device is not compatible with this game. Please contact customer support for assistance!"); 
			#endif
             */
			isScnBg1024Res = false;
			
		}
	}
    if(isRTGraphic){
        rtGraphic->clearRenderTarget();
        rtGraphic->beginRenderToTarget();//RENDER TO TARGET STARTS

        if(isScnBg1024Res)
        {
            HOScene->RenderToTarget(-171.0f);
        }
        else
        {
            HOScene->RenderToTarget();
        }
        rtGraphic->endRenderToTarget();
        screenshot->pSprite->Image = rtGraphic;
		isScreenshotsuccess = true;
		_screenShot = screenshot;
	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    if(isScreenshotsuccess){
        _screenShot->SetPivot(_screenShot->GetWorldRect().w/2, _screenShot->GetWorldRect().h/2);
        _screenShot->SetZ(eZOrder_Popup);
        _screenShot->SetEnable(true);
        _screenShot->Show(true);

        HOScene->Show(false);
        for (int i = 0; i < (int)(_arrayOfArrowBtns.size()); i++)
        {
            _arrayOfArrowBtns.at(i)->Show(false);
        }
        //_bg->Show(true);

        float tweenVals[5] = {0, -1, -1, 2, -1};
        _screenShot->TweenTo(tweenVals, 1, TW_EASE, true, CC_CALLBACK_1(L2S7SoldiersPuzzle::OnMoveToNextLayerComplete, this));

        float tweenVals2[5] = {0, -1, -1, 1, -1};
        _soldierBackLayer->TweenTo(tweenVals2, 1, TW_EASE, true);
    }
#endif

	Control::Audio->QuickLoadAndPlaySFX("s7_zoomin");
}

void L2S7SoldiersPuzzle::OnMoveToNextLayerComplete(CObject * object)
{

}


void L2S7SoldiersPuzzle::OnGamePlayAreaClick(int id)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;
	if (_isWrongClick)
	{
		SoldierPuzzleState puzzleState = (SoldierPuzzleState)ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState;
		switch (puzzleState)
		{
		case eSoldierPuzzleState_Started:
			{
				Hud->ShowBannerText("L2shield");
			}
			break;
		case eSoldierPuzzleState_P2_ShldRotated:
			break;
		case eSoldierPuzzleState_p2_ShldOpened:
			{
				Hud->ShowBannerText("L2panic");
			}
			break;
		case eSoldierPuzzleState_p3_ShldOpened:
			{
				Hud->ShowBannerText("L2missing");
			}
			break;
		case eSoldierPuzzleState_p4_LeverFixed:
			{
				Hud->ShowBannerText("L2guess");
			}
			break;
		case eSoldierPuzzleState_p4_ShldOpened:
			{
				Hud->ShowBannerText("L2symbol");
			}
			break;
		case eSoldierPuzzleState_p5_ShldOpened:
			{
				Hud->ShowBannerText("L2hmm");
			}
			break;
		case eSoldierPuzzleState_p6_Web0Removed:
			break;
		case eSoldierPuzzleState_p6_Web1Removed:
			break;
		case eSoldierPuzzleState_p6_AllWebsRemoved:
			break;
		case eSoldierPuzzleState_p6_ShldOpened:
			break;
		default:
			break;
		}
	}
}

void L2S7SoldiersPuzzle::StartSoldiersOpenAnim()
{
	for (int i = 0; i < (int)(_arrayOfSoldiers.size()); i++)
	{
		_arrayOfSoldiers.at(i)->jsonAnimOpening->PlayAnim();
	}
	Control::Audio->QuickLoadAndPlaySFX("s7_wepondown");
}

void L2S7SoldiersPuzzle::OnSoldiersOpenAnimComplete(int animId)
{
	switch (_currentPuzzleNumber)
	{
	case 1:
		{
			MoveToNextLayer();
		}
		break;
	case 2:
		{
			FadeInArrows();
			ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState = eSoldierPuzzleState_p2_ShldOpened;
		}
		break;
	case 3:
		{
			FadeInArrows();
			FadeInP3Stones();
			ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState = eSoldierPuzzleState_p3_ShldOpened;
		}
		break;
	case 4:
		{
			FadeInArrows();
			ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState = eSoldierPuzzleState_p4_ShldOpened;
		}
		break;
	case 5:
		{
			FadeInArrows();
			ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState = eSoldierPuzzleState_p5_ShldOpened;
		}
		break;
	case 6:
		{
			FadeInArrows();
			ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState = eSoldierPuzzleState_p6_ShldOpened;

			//Show Objective completed
			if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L2O4_FindWayOutOfNetherworld] && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L2O4_FindWayOutOfNetherworld] )
			{
				ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L2O4_FindWayOutOfNetherworld] = true;
				Hud->ShowObjectiveFX(eObj_L2O4_FindWayOutOfNetherworld,true);
			}
		}
		break;
	case 7:
		{

		}
		break;
	default:
		break;
	}
}

void L2S7SoldiersPuzzle::StartSoldiersAttackAnim()
{
	for (int i = 0; i < (int)(_arrayOfSoldiers.size()); i++)
	{
		if (_arrayOfSoldiers.at(i)->jsonAnimAttacking)
		{
			_arrayOfSoldiers.at(i)->jsonAnimAttacking->PlayAnim();
		}
	}
}



void L2S7SoldiersPuzzle::OnSoldiersAttackAnimComplete(int animId)
{
	OnPuzzleFailed();
}

void L2S7SoldiersPuzzle::OnPuzzleFailed()
{
	switch (ProfileSystem::Instance->ActiveProfile.L2_S7_CurrentPuzzle)
	{
	case 1:
		{
			ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState = eSoldierPuzzleState_Started;
		}
		break;
	case 2:
		{
			ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState = eSoldierPuzzleState_Started;
		}
		break;
	case 3:
		{
			ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState = eSoldierPuzzleState_p2_ShldOpened;
		}
		break;
	case 4:
		{
			ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState = eSoldierPuzzleState_p2_ShldOpened;
		}
		break;
	case 5:
		{
			ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState = eSoldierPuzzleState_p4_ShldOpened;
		}
		break;
	case 6:
		{
			ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState = eSoldierPuzzleState_p5_ShldOpened;
		}
		break;
	case 7:
		{
			ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState = eSoldierPuzzleState_p6_ShldOpened;
		}
		break;

	default:
		break;
	}
	//	_currentPuzzleNumber = 5;
	//ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState = eSoldierPuzzleState_p5_ShldOpened;
	_currentPuzzleNumber -= 1;
	FlashScreen();
}




void L2S7SoldiersPuzzle::OnRightArrowClick(CObject * obj)
{
	MoveToNextLayer();
}

void L2S7SoldiersPuzzle::OnWrongArrowClick(CObject * obj)
{
	StartSoldiersAttackAnim();
}




void L2S7SoldiersPuzzle::FadeInArrows()
{
	for (int i = 0; i < (int)(_arrayOfArrowBtns.size()); i++)
	{
		CObject * arrow = _arrayOfArrowBtns.at(i);
		arrow->SetEnable(true);
		CPoint tweenToPos = arrow->Pos;
		arrow->SetY(arrow->Pos.y + 100);
		arrow->SetScale(0);
		arrow->SetAlpha(0);

		float tweenVals[5] = {1, -1, tweenToPos.y, 1, -1};
		if (i == 0)
		{
			arrow->TweenTo(tweenVals, 1, TW_EASEBACKOUT, true, CC_CALLBACK_1(L2S7SoldiersPuzzle::OnFadeInArrowsComplete, this));
		}
		else
		{
			arrow->TweenTo(tweenVals, 1, TW_EASEBACKOUT, false);
		}
	}
}

void L2S7SoldiersPuzzle::OnFadeInArrowsComplete(CObject * obj)
{
	for (int i = 0; i < (int)(_arrayOfArrowBtns.size()); i++)
	{
		_arrayOfArrowBtns.at(i)->SetEnable(true);
		_arrayOfArrowBtns.at(i)->SetIsInteractionEnabled(true);
	}
}




void L2S7SoldiersPuzzle::FlashScreen()
{
	_whiteFlashBg->SetEnable(true);
	_whiteFlashBg->SetAlpha(0);
	float tweenVals[5] = {1, -1, -1, -1, -1};
	_whiteFlashBg->TweenTo(tweenVals, 1, TW_EASE, true, CC_CALLBACK_1(L2S7SoldiersPuzzle::OnFlashScreenHalfComplete, this));
}

void L2S7SoldiersPuzzle::OnFlashScreenHalfComplete(CObject* obj)
{
	if (_currentPuzzleNumber == 6)
	{
		if(_handleExtrasGameComplete())
		{
			return;
		}
	}
	if (_currentPuzzleNumber < 7)
	{
		LoadNextPuzzle();
		_whiteFlashBg->SetEnable(true);
		_whiteFlashBg->SetAlpha(1);
		float tweenVals[5] = {0, -1, -1, -1, -1};
		_whiteFlashBg->TweenTo(tweenVals, 2, TW_EASE, true, CC_CALLBACK_1(L2S7SoldiersPuzzle::OnFlashScreenFullComplete, this));
	}
}

void L2S7SoldiersPuzzle::OnFlashScreenFullComplete(CObject* obj)
{
	_whiteFlashBg->SetEnable(true);
	_whiteFlashBg->SetAlpha(0);
	_gamePlayAreaRect->SetIsInteractionEnabled(true);
	_isMouseInteractionEnabled = true;
}






void L2S7SoldiersPuzzle::Render(const CPoint* pCam)
{

}

void L2S7SoldiersPuzzle::Update()
{
#ifdef _PARTICLEEDITORENABLED
	if(_isParticleEditorActive)
		return;
#endif // _PARTICLEEDITORENABLED

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	if (_isP4LeverOnHand)
	{
	//_p4LeverHO->SetPos(&Control::Input->Mpos());
	// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CPoint mousePos = Control::Input->Mpos();
		_p4LeverHO->SetPos(&mousePos);
		_p4LeverHO->SetZ(60);
	}

	if (_isP6ClothOnHand)
	{
		//_p6ClothOnHand->SetPos(&Control::Input->Mpos());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CPoint mousePos = Control::Input->Mpos();
		_p6ClothOnHand->SetPos(&mousePos);
	}

	if( IsCutscenePlaying )
		//Subtitles Code
		time = _cVideoCutScene->GetTime();

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
		if ((_cVideoCutScene && _cVideoCutScene->isEndReached()) )//|| vidTimer > 98)
		{
			subtitleText->SetText("");
			IsCutscenePlaying = false;
			//_cVideoCutScene->StopVideo();
			Hud->AllowInput = true;
			Hud->ShowHud(false);

			AwardManager::GetInstance()->CompleteHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			//AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			//Clearing all the Level 2 Objectives
			ProfileSystem::Instance->ClearAllCurrentObjectives();ProfileSystem::Instance->ClearInventoryBox();

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L2P9_PastMemories])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L2P9_PastMemories] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L2P9_PastMemories;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
				Hud->ShowJrnEntryFX();
			}

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S1_FortEntrance;
			ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_3;

			//Backup Save for level restart
			ProfileSystem::Instance->SaveProfileBackup(ProfileSystem::Instance->ProfileSlot);

			SendMessage(ControlExit, ID, HOSM_NewArea);		
			return;

		}

	if (_isMouseInteractionEnabled)
	{
		bool isCursorHandPointy = false;
		for (int i = 0; i < (int)(_arrayOfInteractiveObjects.size()); i++)
		{
			CObject * object = _arrayOfInteractiveObjects.at(i);
			if (object->GetIsInteractionEnabled())
			{
				if (object->GetCurrentMouseState() == eMouseState_Over || object->GetCurrentMouseState() == eMouseState_Down)
				{
					isCursorHandPointy = true;
					break;
				}
			}

		}
		if (isCursorHandPointy)
		{
			Cursor::SetMode(CA_HandPointy);
		}
		else
		{
			Cursor::SetMode(CA_Normal);
		}
	}




}

void L2S7SoldiersPuzzle::UpdateChangesToProfile()
{

}

void L2S7SoldiersPuzzle::Required()
{
	HiddenObjectSystem::Required();
}

float L2S7SoldiersPuzzle::easing(float t,float b,float c,float d)
{
	return 0;
}

void L2S7SoldiersPuzzle::ReceiveMessage(int val)
{
	if( val == MAP_HUD_CLICK ) 
	{
		Hud->ShowMap();
	}
	else if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
}

void L2S7SoldiersPuzzle::ProcessNonHOHint()
{
	AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	//AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	SoldierPuzzleState puzzleState = (SoldierPuzzleState)ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState;
	// _currentPuzzleNumber; Sac: Why this is called without any opeartion on or of it ??
	switch (puzzleState)
	{
	case eSoldierPuzzleState_Started:
		{
			//Hud->ShowHintForSceneItem(&HOScene->GetObjectByID("P2_Shld_Closed_0")->Pos);
			InitHintTrailEmit(&HOScene->GetObjectByID("P2_Shld_Closed_0")->Pos);
		}
		break;
	case eSoldierPuzzleState_p2_ShldOpened:
		{
			if (_currentPuzzleNumber == 2)
			{
				InitHintTrailEmit(&_arrayOfArrowBtns.at(1)->Pos);
			}

			if (_currentPuzzleNumber == 3)
			{
				int orderedShieldIds[4] = {0, 3, 2, 1}; 
				int shieldId = orderedShieldIds[_p3ShieldClickCount];
				for(int i = 0; i < (int)(_arrayOfShields.size()); i++)
				{
					L2S7Shield * tempShield = _arrayOfShields.at(i);
					if (tempShield->shieldId == shieldId)
					{
						InitHintTrailEmit(&tempShield->shieldYellowGlow->Pos);
						break;
					}
				}
			}

		}
		break;
	case eSoldierPuzzleState_p3_ShldOpened:
		{
			if (_currentPuzzleNumber == 3)
			{
				InitHintTrailEmit(&_arrayOfArrowBtns.at(0)->Pos);
			}

			if (_currentPuzzleNumber == 4)
			{
				Hud->ShowHintForSceneItem(&_p4LeverBase->Pos);
				InitHintTrailEmit(&_p4LeverHO->Pos);
			}
		}
		break;
	case eSoldierPuzzleState_p4_LeverFixed:
		{
			InitHintTrailEmit(&_p4Lever->Pos);
		}
		break;
	case eSoldierPuzzleState_p4_ShldOpened:
		{
			if (_currentPuzzleNumber == 4)
			{
				InitHintTrailEmit(&_arrayOfArrowBtns.at(0)->Pos);
			}

			if (_currentPuzzleNumber == 5)
			{
				for (int i = 0; i < (int)(_arrayOfShields.size()); i++)
				{
					L2S7Shield * shield = _arrayOfShields.at(i);
					if (shield->shieldCurrentIndex != shield->shieldSolutionIndex)
					{
						L2S7Shield * shield1 = _arrayOfShields.at(shield->shieldCurrentIndex);
						L2S7Shield * shield2 = _arrayOfShields.at(shield->shieldSolutionIndex);
						Hud->ShowHintForSceneItem(&shield2->shieldSymbol->Pos);
						InitHintTrailEmit(&shield1->shieldSymbol->Pos);
						break;
					}

				}
			}
		}
		break;
	case eSoldierPuzzleState_p5_ShldOpened:
		{

			if (_currentPuzzleNumber == 5)
			{
				InitHintTrailEmit(&_arrayOfArrowBtns.at(2)->Pos);
			}

			if (_currentPuzzleNumber == 6)
			{
				Hud->ShowHintForSceneItem(&_web0->Pos);
				InitHintTrailEmit(&_p6ClothHO->Pos);
			}
		}
		break;
	case eSoldierPuzzleState_p6_Web0Removed:
		{
			Hud->ShowHintForSceneItem(&_web1->Pos);
			InitHintTrailEmit(&_p6ClothHO->Pos);
		}
		break;
	case eSoldierPuzzleState_p6_Web1Removed:
		{
			Hud->ShowHintForSceneItem(&_web0->Pos);
			InitHintTrailEmit(&_p6ClothHO->Pos);
		}
		break;
	case eSoldierPuzzleState_p6_AllWebsRemoved:
		{
			for (int i = 0; i < (int)(_arrayOfShields.size()); i++)
			{
				L2S7Shield * shield = _arrayOfShields.at(i);
				if (!shield->shieldArrowGlow->Visible)
				{
					InitHintTrailEmit(&shield->shieldArrowGlow->Pos);
					break;
				}
			}

		}
		break;
	case eSoldierPuzzleState_p6_ShldOpened:
		{
			if( _arrayOfArrowBtns.size() > 0  )
				InitHintTrailEmit(&_arrayOfArrowBtns.at(0)->Pos);
		}
		break;
	default:
		break;
	}
}  

void L2S7SoldiersPuzzle::ProcessSkip()
{
	_currentPuzzleNumber = 6;
	MoveToNextLayer();
}

#pragma endregion


#pragma region Puzzle 2

void L2S7SoldiersPuzzle::OnP2ShieldClick(CObject * object)
{
	_isWrongClick = false;
	object->SetIsInteractionEnabled(false);
	float tweenArray[5] = {-1,-1,-1,-1,0};
	object->TweenTo(tweenArray,1,0,true, CC_CALLBACK_1(L2S7SoldiersPuzzle::OnP2ShieldRotationComplete, this));
	Control::Audio->QuickLoadAndPlaySFX("s7_rotatedial");
}

void L2S7SoldiersPuzzle::OnP2ShieldRotationComplete(CObject * obj)
{
	ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState = eSoldierPuzzleState_P2_ShldRotated;
	CObject * door1 = HOScene->GetObjectByID("P2_ShieldLowerDoor_1");
	float tweenVals1[5] = {-1, -1, door1->Pos.y+45, -1, -1};
	door1->TweenTo(tweenVals1, 1, TW_EASE, true, CC_CALLBACK_1(L2S7SoldiersPuzzle::OnP2ShieldOpenComplete, this));

	CObject * door2 = HOScene->GetObjectByID("P2_ShieldUpperDoor_1");
	float tweenVals2[5] = {-1, -1, door2->Pos.y-45, -1, -1};
	door2->TweenTo(tweenVals2, 1, TW_EASE, false);

	Control::Audio->QuickLoadAndPlaySFX("s7_weponeyeopen");

}

void L2S7SoldiersPuzzle::OnP2ShieldOpenComplete(CObject * obj)
{
	StartSoldiersOpenAnim();
}

#pragma endregion

#pragma region Puzzle 3

void L2S7SoldiersPuzzle::OnP3ShieldClick(CObject * object)
{
	L2S7Shield * shield = (L2S7Shield*)object;
	int orderedShieldIds[4] = {0, 3, 2, 1}; 
	if (shield->shieldId == orderedShieldIds[_p3ShieldClickCount])
	{
		_isWrongClick = false;
		_p3ShieldClickCount++;
		shield->shieldYellowGlow->Show(true);
		for(int i = 0; i < (int)(_arrayOfShields.size()); i++)
		{
			L2S7Shield * tempShield = _arrayOfShields.at(i);
			tempShield->shieldRedGlow->Show(false);
		}
		Control::Audio->QuickLoadAndPlaySFX("s7_tileselect");
	}
	else
	{
		_isWrongClick = true;
		_p3ShieldClickCount = 0;
		shield->shieldRedGlow->Show(true);
		shield->shieldRedGlow->SetAlpha(1);
		shield->shieldRedGlow->FadeOut(0.25);



		for(int i = 0; i < (int)(_arrayOfShields.size()); i++)
		{
			L2S7Shield * tempShield = _arrayOfShields.at(i);
			tempShield->shieldYellowGlow->Show(false);
		}
	}

	if (_p3ShieldClickCount == 4)
	{
		for(int i = 0; i < (int)(_arrayOfShields.size()); i++)
		{
			L2S7Shield * tempShield = _arrayOfShields.at(i);
			tempShield->shieldYellowGlow->Show(false);
			tempShield->shieldClosed->SetIsInteractionEnabled(false);
		}
		StartSoldiersOpenAnim();
	}

}


void L2S7SoldiersPuzzle::FadeInP3Stones()
{

	_p3Stone0->Show(true);
	CPoint tweenToPos = _p3Stone0->Pos;
	_p3Stone0->SetY(_p3Stone0->Pos.y + 100);
	_p3Stone0->SetAlpha(0);

	_p3Stone1->Show(true);
	_p3Stone1->SetY(_p3Stone1->Pos.y + 100);
	_p3Stone1->SetAlpha(0);


	float tweenVals[5] = {1, -1, tweenToPos.y, -1, -1};

	_p3Stone0->TweenTo(tweenVals, 1, TW_EASEBACKOUT, false);
	_p3Stone1->TweenTo(tweenVals, 1, TW_EASEBACKOUT, false);


}

#pragma endregion

#pragma region Puzzle 4

void L2S7SoldiersPuzzle::OnP4LeverClicked(CObject * obj)
{
	if(ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState ==  eSoldierPuzzleState_p4_LeverFixed)
	{
		_jsonAnimP4LeverPullingAnim->PlayAnim(false);
		_p4Lever->SetIsInteractionEnabled(false);
		_isWrongClick = false;
		Control::Audio->QuickLoadAndPlaySFX("s7_leverturned");
	}
}

void L2S7SoldiersPuzzle::OnP4LeverHOClicked(CObject * obj)
{
	_isWrongClick = false;
	_isP4LeverOnHand = true;
	_p4LeverHO->SetIsInteractionEnabled(false);
}

void L2S7SoldiersPuzzle::OnP4LeverBaseAreaClicked(CObject * obj)
{
	if (_isP4LeverOnHand)
	{
		ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState = eSoldierPuzzleState_p4_LeverFixed;
		_p4LeverBase->SetIsInteractionEnabled(false);
		_p4LeverHO->SetEnable(false);
		_p4Lever->SetEnable(true);
		_jsonAnimP4LeverPlacingAnim->SetEnableObjects(true);
		_jsonAnimP4LeverPlacingAnim->PlayAnim(false);
		Control::Audio->QuickLoadAndPlaySFX("s7_leverplaced");
	}
}

void L2S7SoldiersPuzzle::OnP4LeverPullingAnimComplete(int id)
{
	StartSoldiersOpenAnim();
}

void L2S7SoldiersPuzzle::OnP4LeverPlacingAnimComplete(int id)
{
	_p4Lever->SetIsInteractionEnabled(true);
	_isWrongClick = true;
}
#pragma endregion

#pragma region Puzzle 5
void L2S7SoldiersPuzzle::OnP5ShieldClick(CObject * obj)
{
	L2S7Shield * shield = (L2S7Shield*)obj;

	if (_p5PrevSelectedShield == NULL)
	{
		_p5PrevSelectedShield = shield;
		L2S7Shield * parentShield = _arrayOfShields.at(shield->shieldCurrentIndex);
		parentShield->shieldYellowGlow->Show(true);
		Control::Audio->QuickLoadAndPlaySFX("s7_tileselect");
	}
	else if (_p5PrevSelectedShield && _p5PrevSelectedShield != shield)
	{
		float tweenVals[5] = {1, shield->shieldSymbol->Pos.x, shield->shieldSymbol->Pos.y, -1, -1};
		_p5PrevSelectedShield->shieldSymbol->TweenTo(tweenVals, 0.5, TW_EASE, true, CC_CALLBACK_1(L2S7SoldiersPuzzle::OnP5ShieldSwapingComplete, this));
		int prevSelectedIndex = _p5PrevSelectedShield->shieldCurrentIndex;
		_p5PrevSelectedShield->shieldCurrentIndex = shield->shieldCurrentIndex;

		float tweenVals2[5] = {1, _p5PrevSelectedShield->shieldSymbol->Pos.x, _p5PrevSelectedShield->shieldSymbol->Pos.y, -1, -1};
		shield->shieldSymbol->TweenTo(tweenVals2, 0.5, TW_EASE, false);
		shield->shieldCurrentIndex = prevSelectedIndex;

		L2S7Shield * parentShield = _arrayOfShields.at(shield->shieldCurrentIndex);
		parentShield->shieldYellowGlow->Show(false);

		L2S7Shield * parentShield2 = _arrayOfShields.at(_p5PrevSelectedShield->shieldCurrentIndex);
		parentShield2->shieldYellowGlow->Show(false);

		_p5PrevSelectedShield = NULL;
		_isWrongClick = false;
		Control::Audio->QuickLoadAndPlaySFX("s7_swaptiles");
	}
	else if(_p5PrevSelectedShield && _p5PrevSelectedShield == shield)
	{
		_p5PrevSelectedShield = NULL;
		L2S7Shield * parentShield = _arrayOfShields.at(shield->shieldCurrentIndex);
		parentShield->shieldYellowGlow->Show(false);
	}
}

void L2S7SoldiersPuzzle::OnP5ShieldSwapingComplete(CObject * obj)
{
	bool isSolved = true;
	for (int i = 0; i < (int)(_arrayOfShields.size()); i++)
	{
		L2S7Shield * shield = _arrayOfShields.at(i);
		if (shield->shieldCurrentIndex != shield->shieldSolutionIndex)
		{
			isSolved = false;
			break;
		}

	}

	if (isSolved)
	{

		for (int i = 0; i < (int)(_arrayOfShields.size()); i++)
		{
			L2S7Shield * shield = _arrayOfShields.at(i);
			float tweenVals[5] = {0, -1, -1, -1, -1};
			shield->shieldSymbol->TweenTo(tweenVals, 0.5, TW_EASE, false);
			shield->shieldSymbol->SetIsInteractionEnabled(false);




			float tweenVals1[5] = {-1, -1, shield->shieldDoorDown->Pos.y+30, -1, -1};
			shield->shieldDoorDown->TweenTo(tweenVals1, 1, TW_EASE, true, CC_CALLBACK_1(L2S7SoldiersPuzzle::OnP5ShieldOpenComplete, this));

			float tweenVals2[5] = {-1, -1, shield->shieldDoorUp->Pos.y-30, -1, -1};
			shield->shieldDoorUp->TweenTo(tweenVals2, 1, TW_EASE, false);

			L2S7Soldier * soldier = _arrayOfSoldiers.at(i);
			float tweenVals3[5] = {0, -1, -1, -1, -1};
			soldier->symbol->TweenTo(tweenVals3, 1, TW_EASE, false);


		}
	}

}

void L2S7SoldiersPuzzle::OnP5ShieldOpenComplete(CObject * obj)
{
	for (int i = 0; i < (int)(_arrayOfShields.size()); i++)
	{
		L2S7Shield * shield = _arrayOfShields.at(i);
		shield->shieldDoorDown->Show(false);
		shield->shieldDoorUp->Show(false);
		shield->shieldMask->Show(false);
	}
	StartSoldiersOpenAnim();
}

#pragma endregion

#pragma region Puzzle 6

void L2S7SoldiersPuzzle::OnP6ShieldClick(CObject * obj)
{

	L2S7Shield * shield = (L2S7Shield*)obj;
	shield->shieldArrow->SetRotate(shield->shieldArrow->GetRotate() - 15);
	Control::Audio->QuickLoadAndPlaySFX("s7_rotatedial");




	bool isSolved = true;
	float solutionAngles[3] = { (270),  (225),  (30)};

	for (int i = 0; i < (int)(_arrayOfShields.size()); i++)
	{
		L2S7Shield * shield = _arrayOfShields.at(i);
		float rotation = (float)(Elixir::NormalizeAngle((double)(shield->shieldArrow->GetRotate())));
		if (rotation == solutionAngles[i])
		{
			shield->shieldArrowGlow->Show(true);
			shield->shieldArrowGlow->SetXY(shield->shieldArrow->Pos.x, shield->shieldArrow->Pos.y); 
			shield->shieldArrowGlow->SetRotate(shield->shieldArrow->GetRotate());
		}
		else
		{
			shield->shieldArrowGlow->Show(false);
			isSolved = false;
		}
	}

	if (isSolved)
	{

		for (int i = 0; i < (int)(_arrayOfShields.size()); i++)
		{
			L2S7Shield * shield = _arrayOfShields.at(i);
			shield->shieldArrowGlow->Show(false);
		}

		StartSoldiersOpenAnim();
	}
}

void L2S7SoldiersPuzzle::OnP6ClothAnimComplete(int id)
{
	if (_numOfWebsRemoved < 2)
	{
		_p6ClothHO->SetEnable(true);
		_p6ClothHO->SetIsInteractionEnabled(true);
	}
	L2S7Shield * shield = _arrayOfShields.at(id);
	shield->shieldClosed->SetIsInteractionEnabled(true);
	if (_numOfWebsRemoved == 2)
	{
		_p6ClothHO->SetEnable(false);
		_p6ClothHO->SetIsInteractionEnabled(false);
		_p6ClothOnHand->SetEnable(false);
		_isP6ClothOnHand = false;
		for (int i = 0; i < (int)(_arrayOfShields.size()); i++)
		{
			L2S7Shield * shield = _arrayOfShields.at(i);
			shield->shieldClosed->SetIsInteractionEnabled(true);
		}
	}
}

void L2S7SoldiersPuzzle::OnP6ClothHOClicked(CObject * obj)
{
	if (!_isP6ClothOnHand)
	{
		_isP6ClothOnHand = true;
		_p6ClothOnHand->SetEnable(true);
		_p6ClothOnHand->Show(true);
		_p6ClothOnHand->SetAlpha(1);
		_p6ClothHO->SetEnable(false);
	}
}

void L2S7SoldiersPuzzle::OnP6WebAreaClicked(CObject * obj)
{
	if (_isP6ClothOnHand)
	{
		Control::Audio->QuickLoadAndPlaySFX("s7_clothclean");
		_isWrongClick = false;
		_isP6ClothOnHand = false;
		_p6ClothHO->SetIsInteractionEnabled(false);
		L2S7Shield	 * shield = (L2S7Shield*)obj;
		if (shield->shieldId == 0)
		{
			_jsonAnimP6Cloth0->PlayAnim();
			ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState = eSoldierPuzzleState_p6_Web0Removed;
			_web0->SetIsInteractionEnabled(false);
			Cursor::SetMode(CA_Normal);
			_numOfWebsRemoved++;
		}
		else if(shield->shieldId == 2)
		{
			_jsonAnimP6Cloth1->PlayAnim();
			ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState = eSoldierPuzzleState_p6_Web1Removed;
			_web1->SetIsInteractionEnabled(false);
			Cursor::SetMode(CA_Normal);
			_numOfWebsRemoved++;
		}

		if (_numOfWebsRemoved >= 2)
		{
			ProfileSystem::Instance->ActiveProfile.L2_S7_SoldierPuzzleState = eSoldierPuzzleState_p6_AllWebsRemoved;
		}
	}
}

#pragma endregion





#pragma region SOLDIER

L2S7Soldier::L2S7Soldier(int layerId)
{
	symbolId = -1;
	shieldSymbolId = -1;
	symbol = NULL;
	jsonAnimOpening = NULL;
	jsonAnimAttacking = NULL;
}

L2S7Soldier::~L2S7Soldier()
{
	SAFE_DELETE(jsonAnimOpening);
	SAFE_DELETE(jsonAnimAttacking);
}

void L2S7Soldier::SetState(L2S7SoldiersState state)
{
	if (_state == state)
	{
		return;
	}
	_state = state;

	switch (state)
	{

	case eL2S7SoldiersState_Closed:
		{
			jsonAnimOpening->SetAnimTime(0);
		}
		break;
	case eL2S7SoldiersState_Opened:
		{

			jsonAnimOpening->SetAnimTime(1);
		}
		break;
	case eL2S7SoldiersState_Attacked:
		{

		}
		break;

	case eL2S7SoldiersState_Opening:
	case eL2S7SoldiersState_Default:
	case eL2S7SoldiersState_Attacking:
	default:
		break;
	}
}
#pragma endregion

#pragma region SHIELD

L2S7Shield::L2S7Shield()
{
    layerId = layerId; //Sac: why ??
	shieldClosed = NULL;
	shieldAttacked = NULL;
	shieldOpened = NULL;
	shieldSymbol = NULL;
	shieldArrow = NULL;
	shieldArrowGlow = NULL;
	shieldArrowCenter = NULL;
	shieldMask = NULL;
	shieldDoorUp = NULL;
	shieldDoorDown = NULL;
	shieldYellowGlow = NULL;
	shieldRedGlow = NULL;
}

L2S7Shield::~L2S7Shield()
{

}
#pragma endregion

