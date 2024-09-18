

#include "L6_S5_TabMiniGame.h"
#include "Util.h"
#include"Application.h"
#include"Hud.h"
#include"MeshObject.h"


L6S5TabMiniGame::L6S5TabMiniGame()
{
	AwardManager::GetInstance()->StartHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);// inorder to track hint
	//AwardManager::GetInstance()->StartMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);// this minigame added as a puzzle in extras
	HOScene = NULL;
	_rabbit = NULL;
	_isMouseInteractionEnabled = false;
	_isDraggingSomething = false;
	_sunRay = NULL;
	_sunEye1 = NULL;
	_sunEye2 = NULL;
	_cVideoCutScene = NULL;
	isCutscnPlaying = false;
	vidTimer = 0;

	//SCENE_0

	_s0_squirrel = NULL;
	_s0_brick = NULL;
	_s0_fixedBrick = NULL;
	_s0_brickOnHand = NULL;
	_s0_fruitOnHand = NULL;
	_s0_isDraggingBrick = false;
	_s0_isDraggingFruit = false;
	_s0_jsonAnimSquirrel = NULL;
	_s0_jsonAnimRabbitEscape = NULL;

	//SCENE_1

	_s1_sawOnHand = NULL;
	_s1_saw = NULL;
	_s1_stone = NULL;
	_s1_tree = NULL;
	_s1_treeTop = NULL;
	_s1_isDraggingSaw = false;
	_s1_jsonAnimTreeCut = NULL;
	_s1_jsonAnimRabbitEscape = NULL;

	//SCENE_2

	_s2_waterTubeOnHand = NULL;
	_s2_stoneOnHand = NULL;
	_s2_waterTube = NULL;
	_s2_waterTubeOnWater = NULL;
	_s2_waterPipe = NULL;
	_s2_stone = NULL;
	_s2_cloth = NULL;
	_s2_water = NULL;
	_s2_meshObjectWater = NULL;
	_s2_isDraggingWaterTube = false;
	_s2_isDraggingStone = false;
	_s2_pipeWaterVFX = NULL;
	_s2_jsonAnimFillWater = NULL;
	_s2_jsonAnimRabbitEscape = NULL;

	//SENE_3

	_s3_carrotOnHand = NULL;
	_s3_isDraggingCarrot = false;

	_whiteFlashBg = new CObject();
	_whiteFlashBg->LoadInit("LV2\\S7\\SOLDIERPUZZLE\\White.SPR", 0, 0, 25);
	_whiteFlashBg->SetScale(10);
	_whiteFlashBg->SetEnable(false);


	_bgMask = new CObject();
	_bgMask->LoadInit("LV6\\S5\\TABMINIGAME\\S_Bg.SPR", 0, 0, 26);



	LoadHud(INVENTORY_V1);
	Hud->InvBoxCtrl->SetLockBase(PUZBOX_FRAME);//lock in HO anim
	Hud->IsHOScene = true;
	Hud->HOState = HOActive;

	//	Hud->InvBox_StartPuzzleMode();

	InitializeWithProfileData();
	_isMouseInteractionEnabled = true;
	_isDraggingSomething = false;

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	/*Control::Audio->LoadSample(aAmbL6s5TabletMiniGameAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL6s5TabletMiniGameAmb) )		
	{
	Control::Audio->PlaySample(aAmbL6s5TabletMiniGameAmb,true);
	}
	*/
	//MUSIC_SFX


}

L6S5TabMiniGame::~L6S5TabMiniGame()
{

	SAFE_DELETE(_whiteFlashBg);
	SAFE_DELETE(_bgMask);
	SAFE_DELETE(_cVideoCutScene);

	UnloadScene();
	Hud->InvBox_StopPuzzleMode();

	//MUSIC_SFX

	//Control::Audio->StopSample(aAmbL6s5TabletMiniGameAmb);
	//Control::Audio->UnloadSample(aAmbL6s5TabletMiniGameAmb);
	//MUSIC_SFX
}

void L6S5TabMiniGame::InitializeWithProfileData()
{
	_currentSceneNumber = ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameSceneNumber;
	//_currentSceneNumber = 3;
	//ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameState = eTabMiniGameState_S3_Started;



	LoadScene(_currentSceneNumber);
	bool isSolved = true;
	for (int i = 0; i < 5; i++)
	{
		if (ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameIsCarrotPicked[i] == false)
		{
			isSolved = false;
			break;
		}
	}
	if(isSolved)
	{
		ProfileSystem::Instance->ActiveProfile.L6_S5_TabMinigameSolved = true;

		//CRectangle RectVid(171, 96, 1024, 576);
		CRectangle RectVid(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
		_cVideoCutScene = new CVideo("OGV\\L6\\S5ENDCUTSCENE.OGV", RectVid, false, eZOrder_CutsceneFull);
		//_cVideoCutScene->SetScale(1.334f);
		_cVideoCutScene->SetScale(GFApp->video_aspectRatio);
		_cVideoCutScene->PlayVideo();
		_cVideoCutScene->SetSkipActive();
		isCutscnPlaying = true;

		Hud->EnableInput(false);
		Hud->HideHud(false);

		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L6_S1_FloatingIsland1] = false;
		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L6_S2_FloatingIsland2] = false;
		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L6_S3_FloatingIsland3] = false;
		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L6_S4_FloatingIsland4] = false;
		//Hud->InvBox_StopPuzzleMode();
	}
}


void L6S5TabMiniGame::Render(const CPoint* pCam)
{

}
void L6S5TabMiniGame::Update()
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	if( isCutscnPlaying )
	{
#ifdef FORCE_CLOSE_VIDEO
		vidTimer += Control::LogicRate;
#endif
		if (_cVideoCutScene && (
#ifndef FORCE_CLOSE_VID_END
			_cVideoCutScene->isEndReached() ||
#endif
			vidTimer > 13))
		{
			isCutscnPlaying = false;
			AwardManager::GetInstance()->CompleteHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			//AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			//Clearing all the Level 6 Objectives
			ProfileSystem::Instance->ClearAllCurrentObjectives();ProfileSystem::Instance->ClearInventoryBox();
			Hud->AllowInput = true;
			Hud->ShowHud(false);			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S1_Volcano;
			ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_7;
			ProfileSystem::Instance->SaveProfileBackup(ProfileSystem::Instance->ProfileSlot);
			SendMessage(ControlExit, ID, HOSM_NewArea);
			return;
		}
		return;//No other processing during cutscene!!
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed()) {
			if(Hud!=NULL && GFInstance->IsInGameMenuPopup != true)Hud->CallPauseMenu();
		}
	#endif
	_isDraggingSomething = false;

	CPoint mousePos = Control::Input->Mpos();
	if (_s0_isDraggingFruit)
	{
		_s0_fruitOnHand->SetXY(mousePos.x, mousePos.y);
		_isDraggingSomething = true;
	}

	if (_s0_isDraggingBrick)
	{
		_s0_brickOnHand->SetXY(mousePos.x, mousePos.y);
		_isDraggingSomething = true;
	}

	if (_s1_isDraggingSaw)
	{
		_s1_sawOnHand->SetXY(mousePos.x, mousePos.y);
		_isDraggingSomething = true;
	}

	if (_s2_isDraggingStone)
	{
		_s2_stoneOnHand->SetXY(mousePos.x, mousePos.y);
		_isDraggingSomething = true;
	}

	if (_s2_isDraggingWaterTube)
	{
		_s2_waterTubeOnHand->SetXY(mousePos.x, mousePos.y);
		_isDraggingSomething = true;
	}

	if (_s3_isDraggingCarrot)
	{
		_s3_carrotOnHand->SetXY(mousePos.x, mousePos.y);
		_isDraggingSomething = true;
	}

	if (_s2_meshObjectWater)
	{
		_s2_meshObjectWater->meshFX->setPosOffset(_s2_water->Pos.x, _s2_water->Pos.y);
	}

	UpdateCursor();

	if (_sunRay)
	{
		_sunRay->SetRotate((float)Elixir::NormalizeAngle((_sunRay->GetRotate() + 0.1f), 0, 360));
		//float angle = Elixir::Angle(&Control::Input->Mpos(), _sunEye1->GetPosPointer());
		//angle = Elixir::Angle(&Control::Input->Mpos(), _sunEye2->GetPosPointer());
		// Linux: gives error of taking address of temporary also potential crash factor
		// below is fix
		CPoint mousePos =Control::Input->Mpos();
		float angle = Elixir::Angle(&mousePos, _sunEye1->GetPosPointer());
		_sunEye1->SetRotate(180 - angle);

		angle = Elixir::Angle(&mousePos, _sunEye2->GetPosPointer());
		_sunEye2->SetRotate(180 - angle);
	}

}

void L6S5TabMiniGame::UpdateCursor()
{
	if (_isMouseInteractionEnabled)
	{
		bool isCursorHandTake = false;
		for (int i = 0; i < (int)(_arrayOfPickableObjects.size()); i++)
		{
			CObject * object = _arrayOfPickableObjects.at(i);
			if (object->GetIsInteractionEnabled())
			{
				if (object->GetCurrentMouseState() == eMouseState_Over || object->GetCurrentMouseState() == eMouseState_Down)
				{
					isCursorHandTake = true;
					break;
				}
			}

		}

		bool isCursorHandPointy = false;
		for (int i = 0; i < (int)(_arrayOfHandPointyObjects.size()); i++)
		{
			CObject * object = _arrayOfHandPointyObjects.at(i);
			if (object->GetIsInteractionEnabled())
			{
				if (object->GetCurrentMouseState() == eMouseState_Over || object->GetCurrentMouseState() == eMouseState_Down)
				{
					isCursorHandPointy = true;
					break;
				}
			}

		}

		bool isCursorGear = false;
		for (int i = 0; i < (int)(_arrayOfTaskObjects.size()); i++)
		{
			CObject * object = _arrayOfTaskObjects.at(i);
			if (object->GetIsInteractionEnabled())
			{
				if (object->GetCurrentMouseState() == eMouseState_Over || object->GetCurrentMouseState() == eMouseState_Down)
				{
					isCursorGear = true;
					break;
				}
			}

		}

		if (_isDraggingSomething)
		{
			Cursor::SetMode(CA_Normal);
		}
		else
		{
			if (isCursorHandTake)
			{
				Cursor::SetMode(CA_HandTake);
			}
			else if (isCursorHandPointy)
			{
				Cursor::SetMode(CA_HandPointy);
			}
			else if (isCursorGear)
			{
				Cursor::SetMode(CA_Gear);
			}
			else
			{
				Cursor::SetMode(CA_Normal);
			}
		}
	}
}

void L6S5TabMiniGame::Required()
{
	HiddenObjectSystem::Required();
	if ( isCutscnPlaying )
		return;

	if( Control::Audio->IsPlaying(aTrackL6s3Puzzle) )
	{
		Control::Audio->Samples[aTrackL6s3Puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL6s3Puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL6s3Puzzle);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL6puzzle) )
				Control::Audio->PlaySample(aTrackL6puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL6puzzle))
		{
			Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL6puzzle, true);
		}
	}
}


void L6S5TabMiniGame::LoadScene(int sceneNumber)
{
	ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameSceneNumber = sceneNumber;
	//Hud->InvBox_StartPuzzleMode();

	TabMiniGameState gameState = (TabMiniGameState)ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameState;
	if (!HOScene)
	{
		HOScene = new CHOScene();
	}

	switch (sceneNumber)
	{
	case 0:
		{
			HOScene->Load("LV6\\S5\\TABMINIGAME\\SCENE_0.SCN");
			_rabbit = HOScene->GetObjectByID("S_Rabit");
			_rabbit->SetMouseClickCallback(CC_CALLBACK_1(L6S5TabMiniGame::OnS0RabbitClick, this));
			_s0_squirrel = HOScene->GetObjectByID("S0_Squirrel");
			_s0_brick = HOScene->GetObjectByID("S0_Brick");
			_s0_brickOnHand = HOScene->GetObjectByID("S0_BrickOnHand");
			_s0_fruitOnHand = HOScene->GetObjectByID("S0_FruitOnHand");
			_s0_fixedBrick = HOScene->GetObjectByID("S0_BrickFixed");
			_s0_jsonAnimSquirrel = new JSONAnimator(1);
			_s0_jsonAnimSquirrel->parseAndLoadJSON("ANIM\\L6\\S5_P0_SQUIRRELRUN.JSON", HOScene);
			_s0_jsonAnimSquirrel->SetEnableObjects(false);

			_s0_jsonAnimRabbitEscape = new JSONAnimator(2,  CC_CALLBACK_1(L6S5TabMiniGame::OnRabbitEscapeAnimComplete, this));
			_s0_jsonAnimRabbitEscape->parseAndLoadJSON("ANIM\\L6\\S5_P0_RABBITESCAPE.JSON", HOScene);
			_s0_jsonAnimRabbitEscape->SetAnimTime(0);

			_s0_brickOnHand->Show(false);
			_s0_fruitOnHand->Show(false);
			_s0_fixedBrick->Show(false);

			int ArrayPts1[] = {654,385,799,350,810,383,664,419};
			CPolygon * brickClickArea =  new CPolygon(4);
			brickClickArea->Init(ArrayPts1);
			_s0_brick->SetMouseClickProperties(brickClickArea, CC_CALLBACK_1(L6S5TabMiniGame::OnS0BrickClick, this));


			_s0_fixedBrick->SetMouseClickCallback(CC_CALLBACK_1(L6S5TabMiniGame::OnS0FixedBrickClick, this));

			int ArrayPts2[] = {674,318,749,287,785,343,706,368};
			CPolygon * squirrelClickArea =  new CPolygon(4);
			squirrelClickArea->Init(ArrayPts2);
			_s0_squirrel->SetMouseClickProperties(squirrelClickArea, CC_CALLBACK_1(L6S5TabMiniGame::OnS0SquirrelClick, this));

			for (int i = 0; i < 3; i++)
			{
				std::string fruitId = MKSTR("S0_Fruit_") + MKSTR(i);
				CObject * fruit = HOScene->GetObjectByID(fruitId);
				_s0_arrayOfFruits.push_back(fruit);
				fruit->SetMouseClickCallback(CC_CALLBACK_1(L6S5TabMiniGame::OnS0FruitClick, this));
			}

			if (gameState >= eTabMiniGameState_S0_Squirrel_Moved)
			{
				_s0_squirrel->Show(false);
				if (gameState == eTabMiniGameState_S0_Squirrel_Moved)
				{
					_s0_arrayOfFruits.at(ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGamePluckedFruitIndex)->Show(false);
				}
			}

			UpdateCursorObjects(eTabMiniGameState_S0_Started);
			UpdateCursorObjects(gameState);
		}
		break;

	case 1:
		{
			HOScene->Load("LV6\\S5\\TABMINIGAME\\SCENE_1.SCN");

			_rabbit = HOScene->GetObjectByID("S_Rabit");
			_rabbit->SetMouseClickCallback(CC_CALLBACK_1(L6S5TabMiniGame::OnS1RabbitClick, this));

			_s1_sawOnHand = HOScene->GetObjectByID("S1_SawOnHand");
			_s1_saw = HOScene->GetObjectByID("S1_Saw");
			_s1_stone = HOScene->GetObjectByID("S1_Stone");
			_s1_tree = HOScene->GetObjectByID("S1_Tree");
			_s1_treeTop = HOScene->GetObjectByID("S1_TreeTop");

			_s1_jsonAnimRabbitEscape = new JSONAnimator(2,  CC_CALLBACK_1(L6S5TabMiniGame::OnRabbitEscapeAnimComplete, this));
			_s1_jsonAnimRabbitEscape->parseAndLoadJSON("ANIM\\L6\\S5_P1_RABBITESCAPE.JSON", HOScene);
			_s1_jsonAnimRabbitEscape->SetAnimTime(0);

			_s1_jsonAnimTreeCut = new JSONAnimator(2,  CC_CALLBACK_1(L6S5TabMiniGame::OnS1TreeCutAnimComplete, this));
			_s1_jsonAnimTreeCut->parseAndLoadJSON("ANIM\\L6\\S5_P1_TREECUT.JSON", HOScene);
			_s1_jsonAnimTreeCut->SetEnableObjects(false);


			_s1_sawOnHand->Show(false);


			_s1_saw->SetMouseClickCallback(CC_CALLBACK_1(L6S5TabMiniGame::OnS1SawClick, this));
			_s1_stone->SetMouseClickCallback(CC_CALLBACK_1(L6S5TabMiniGame::OnS1StonelClick, this));

			int ArrayPts1[] = {551,204,600,206,599,386,545,383};
			CPolygon * treeClickArea =  new CPolygon(4);
			treeClickArea->Init(ArrayPts1);
			_s1_tree->SetMouseClickProperties(treeClickArea, CC_CALLBACK_1(L6S5TabMiniGame::OnS1TreeClick, this));


			int ArrayPts2[] = {577,423,850,378,854,299,576,302};
			CPolygon * treeTopClickArea =  new CPolygon(4);
			treeTopClickArea->Init(ArrayPts2);

			_s1_treeTop->SetMouseClickProperties(treeTopClickArea,CC_CALLBACK_1(L6S5TabMiniGame::OnS1TreeTopClick, this));
			_s1_treeTop->SetIsInteractionEnabled(false);

			UpdateCursorObjects(eTabMiniGameState_S1_Started);
			UpdateCursorObjects(gameState);

			if (gameState == eTabMiniGameState_S1_Started)
			{
				_s1_jsonAnimTreeCut->SetAnimTime(0);
			}

			if (gameState == eTabMiniGameState_S1_Tree_Cut)
			{
				_s1_jsonAnimTreeCut->SetAnimTime(3.2f);
				_s1_tree->Show(false);
				_s1_saw->Show(false);
			}

		}
		break;

	case 2:
		{
			HOScene->Load("LV6\\S5\\TABMINIGAME\\SCENE_2.SCN");
			_rabbit = HOScene->GetObjectByID("S_Rabit");
			_rabbit->SetMouseClickCallback(CC_CALLBACK_1(L6S5TabMiniGame::OnS2RabbitClick, this));
			_s2_waterTubeOnHand = HOScene->GetObjectByID("S2_WaterTubeOnHand");
			_s2_waterTube = HOScene->GetObjectByID("S2_WaterTube0");
			_s2_waterTubeOnWater = HOScene->GetObjectByID("S2_WaterTube1");
			_s2_waterPipe = HOScene->GetObjectByID("S2_WaterPipeClosed");
			_s2_stone = HOScene->GetObjectByID("S2_Stone");
			_s2_stoneOnHand = HOScene->GetObjectByID("S2_StoneOnHand");
			_s2_stone->SetScale(0.5);
			_s2_cloth = HOScene->GetObjectByID("S2_Cloth");
			_s2_water = HOScene->GetObjectByID("S2_Water");
			_s2_water->Show(false);


			_s2_meshObjectWater = new MeshObject(_s2_water, 3, 3);
			_s2_meshObjectWater->meshFX->SetSpeedAndDisplacement(3, 3, 5, 5);
			HOScene->PushObject(_s2_meshObjectWater);

			_s2_jsonAnimRabbitEscape = new JSONAnimator(2,  CC_CALLBACK_1(L6S5TabMiniGame::OnRabbitEscapeAnimComplete, this));
			_s2_jsonAnimRabbitEscape->parseAndLoadJSON("ANIM\\L6\\S5_P2_RABBITESCAPE.JSON", HOScene);
			_s2_jsonAnimRabbitEscape->SetAnimTime(0);

			std::vector<SuperFX*> superfxArray;
			_s2_pipeWaterVFX = new SuperFX("PRT\\L6\\S5_PipeWater.INI", PointSystem::CreateCPoint(488,106,0), _s2_waterPipe->ZOrder+1); 
			superfxArray.push_back(_s2_pipeWaterVFX);


			_s2_jsonAnimFillWater = new JSONAnimator(2,  CC_CALLBACK_1(L6S5TabMiniGame::OnS2WaterFillingAnimComplete, this));
			_s2_jsonAnimFillWater->parseAndLoadJSON("ANIM\\L6\\S5_P2_WATERFILLING.JSON", HOScene, &superfxArray);
			if (gameState == eTabMiniGameState_S2_Started)
			{
				_s2_jsonAnimFillWater->SetAnimTime(0);
			}
			else if(gameState == eTabMiniGameState_S2_Pipe_Unlocked)
			{
				_s2_jsonAnimFillWater->SetAnimTime(4.0f);
				_s2_pipeWaterVFX->Show(false);
				_s2_stone->Show(false);
			}


			_s2_waterTube->SetMouseClickCallback(CC_CALLBACK_1(L6S5TabMiniGame::OnS2WaterTubeClick, this));
			_s2_cloth->SetMouseClickCallback(CC_CALLBACK_1(L6S5TabMiniGame::OnS2ClothClick, this));
			_s2_stone->SetMouseClickCallback(CC_CALLBACK_1(L6S5TabMiniGame::OnS2StoneClick, this));




			int ArrayPts1[] = {694,430,769,428,769,493,695,490};
			CPolygon * pipeClickArea =  new CPolygon(4);
			pipeClickArea->Init(ArrayPts1);
			_s2_waterPipe->SetMouseClickProperties(pipeClickArea,CC_CALLBACK_1(L6S5TabMiniGame::OnS2PipeClick, this));


			int ArrayPts2[] = {718,368,726,520,922,525,920,374};
			CPolygon * waterClickArea =  new CPolygon(4);
			waterClickArea->Init(ArrayPts2);
			_s2_waterPos = &waterClickArea->center;
			_s2_water->SetMouseClickProperties(waterClickArea, CC_CALLBACK_1(L6S5TabMiniGame::OnS2WaterClick, this));

			_s2_waterTubeOnHand->Show(false);
			_s2_waterTubeOnWater->Show(false);
			_s2_stoneOnHand->Show(false);

			_s2_waterTube->SetIsInteractionEnabled(false);

			if (ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameIsClothRemoved)
			{
				_s2_cloth->Show(false);
				_s2_cloth->SetIsInteractionEnabled(false);
			}

			UpdateCursorObjects(eTabMiniGameState_S2_Started);
			UpdateCursorObjects(gameState);


		}
		break;

	case 3:
		{
			HOScene->Load("LV6\\S5\\TABMINIGAME\\SCENE_3.SCN");
			_rabbit = HOScene->GetObjectByID("S_Rabit");
			_rabbit->SetMouseClickCallback(CC_CALLBACK_1(L6S5TabMiniGame::OnS3RabbitClick, this));
			_s3_carrotOnHand = HOScene->GetObjectByID("S3_CarrotOnHand");
			_s3_carrotOnHand->Show(false);
			for (int i = 0; i < 5; i++)
			{
				std::string carrotId = MKSTR("S3_Carrot_") + MKSTR(i);
				CObject * carrot = HOScene->GetObjectByID(carrotId);
				_s3_arrayOfCarrots.push_back(carrot);
				carrot->SetMouseClickCallback(CC_CALLBACK_1(L6S5TabMiniGame::OnS3CarrotClick, this));
				if (ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameIsCarrotPicked[i])
				{
					carrot->Show(false);
				}
			}
			UpdateCursorObjects(eTabMiniGameState_S3_Started);
			UpdateCursorObjects(gameState);
		}
		break;
	default:
		break;
	}

	_sunRay = HOScene->GetObjectByID("S_SunRay");
	_sunEye1 = HOScene->GetObjectByID("S_Eye1");
	_sunEye2 = HOScene->GetObjectByID("S_Eye2");
}

void L6S5TabMiniGame::UnloadScene()
{
	_rabbit = NULL;
	_sunRay = NULL;
	_sunEye1 = NULL;
	_sunEye2 = NULL;
	_arrayOfPickableObjects.clear();
	_arrayOfTaskObjects.clear();
	_arrayOfHandPointyObjects.clear();
	_isMouseInteractionEnabled = false;
	_isDraggingSomething = false;


	//SCENE_0
	_s0_squirrel = NULL;
	_s0_brick = NULL;
	_s0_fixedBrick = NULL;
	_s0_brickOnHand = NULL;
	_s0_fruitOnHand = NULL;
	_s0_isDraggingBrick = false;
	_s0_isDraggingFruit = false;
	_s0_arrayOfFruits.clear();
	SAFE_DELETE(_s0_jsonAnimSquirrel);
	SAFE_DELETE(_s0_jsonAnimRabbitEscape);

	//SCENE_1

	_s1_sawOnHand = NULL;
	_s1_saw = NULL;
	_s1_stone = NULL;
	_s1_tree = NULL;
	_s1_treeTop = NULL;
	_s1_isDraggingSaw = false;
	SAFE_DELETE(_s1_jsonAnimTreeCut);
	SAFE_DELETE(_s1_jsonAnimRabbitEscape);

	//SCENE_2

	_s2_waterTubeOnHand = NULL;
	_s2_stoneOnHand = NULL;
	_s2_waterTube = NULL;
	_s2_waterTubeOnWater = NULL;
	_s2_waterPipe = NULL;
	_s2_stone = NULL;
	_s2_cloth = NULL;
	_s2_water = NULL;
	_s2_meshObjectWater = NULL;
	_s2_isDraggingWaterTube = false;
	_s2_isDraggingStone = false;
	SAFE_DELETE(_s2_pipeWaterVFX);
	SAFE_DELETE(_s2_jsonAnimFillWater);
	SAFE_DELETE(_s2_jsonAnimRabbitEscape);

	//SENE_3

	_s3_carrotOnHand = NULL;
	_s3_isDraggingCarrot = false;

	SAFE_DELETE(HOScene);
}

void L6S5TabMiniGame::LoadNextScene()
{
	UnloadScene();
	_currentSceneNumber++;
	ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameState = TabMiniGameState(ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameState + 1);
	LoadScene(_currentSceneNumber);
}

void L6S5TabMiniGame::OnRabbitEscapeAnimComplete(int animId)
{
	StartFadeInWhiteBg();
}

void L6S5TabMiniGame::UpdateCursorObjects(TabMiniGameState gameState)
{

	for (int i = 0; i < (int)(_arrayOfHandPointyObjects.size()); i++)
	{
		_arrayOfHandPointyObjects.at(i)->SetIsInteractionEnabled(false);
	}
	_arrayOfHandPointyObjects.clear();

	for (int i = 0; i < (int)(_arrayOfPickableObjects.size()); i++)
	{
		_arrayOfPickableObjects.at(i)->SetIsInteractionEnabled(false);
	}
	_arrayOfPickableObjects.clear();
	for (int i = 0; i < (int)(_arrayOfTaskObjects.size()); i++)
	{
		_arrayOfTaskObjects.at(i)->SetIsInteractionEnabled(false);
	}
	_arrayOfTaskObjects.clear();

	switch (gameState)
	{
	case eTabMiniGameState_S0_Started:
		{
			for (int i = 0; i < (int)(_s0_arrayOfFruits.size()); i++)
			{
				CObject * fruit = _s0_arrayOfFruits.at(i);
				_arrayOfPickableObjects.push_back(fruit);
			}

			_arrayOfTaskObjects.push_back(_s0_brick);
			_arrayOfTaskObjects.push_back(_s0_squirrel);
			_arrayOfTaskObjects.push_back(_s0_fixedBrick);
			_arrayOfTaskObjects.push_back(_rabbit);
		}
		break;
	case eTabMiniGameState_S0_Squirrel_Moved:
		{
			_arrayOfPickableObjects.push_back(_s0_brick);
			_arrayOfTaskObjects.push_back(_s0_fixedBrick);
			_arrayOfTaskObjects.push_back(_rabbit);
		}
		break;
	case eTabMiniGameState_S1_Started:
		{
			_arrayOfPickableObjects.push_back(_s1_saw);
			_arrayOfTaskObjects.push_back(_s1_tree);
			_arrayOfTaskObjects.push_back(_s1_stone);
			_arrayOfTaskObjects.push_back(_rabbit);
		}
		break;
	case eTabMiniGameState_S1_Tree_Cut:
		{
			_arrayOfTaskObjects.push_back(_s1_treeTop);
			_arrayOfHandPointyObjects.push_back(_s1_stone);
			_arrayOfTaskObjects.push_back(_rabbit);
		}
		break;
	case eTabMiniGameState_S2_Started:
		{
			_arrayOfPickableObjects.push_back(_s2_stone);
			if (ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameIsClothRemoved)
			{
				_arrayOfTaskObjects.push_back(_s2_waterTube);
			}
			else
			{
				_arrayOfHandPointyObjects.push_back(_s2_cloth);
			}
			_arrayOfTaskObjects.push_back(_s2_waterPipe);
			_arrayOfTaskObjects.push_back(_s2_water);
			_arrayOfTaskObjects.push_back(_rabbit);
		}
		break;
	case eTabMiniGameState_S2_Pipe_Unlocked:
		{
			if (ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameIsClothRemoved)
			{
				_arrayOfPickableObjects.push_back(_s2_waterTube);
			}
			else
			{
				_arrayOfHandPointyObjects.push_back(_s2_cloth);
			}
			_arrayOfTaskObjects.push_back(_s2_water);
			_arrayOfTaskObjects.push_back(_rabbit);
		}
		break;
	case eTabMiniGameState_S3_Started:
		{
			for (int i = 0; i < 5; i++)
			{
				CObject * carrot = _s3_arrayOfCarrots.at(i);
				if (ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameIsCarrotPicked[i] == false)
				{
					_arrayOfPickableObjects.push_back(carrot);
				}
			}
			_arrayOfTaskObjects.push_back(_rabbit);
		}
		break;
	default:
		break;
	}


	for (int i = 0; i < (int)(_arrayOfHandPointyObjects.size()); i++)
	{
		_arrayOfHandPointyObjects.at(i)->SetIsInteractionEnabled(true);
	}
	for (int i = 0; i < (int)(_arrayOfPickableObjects.size()); i++)
	{
		_arrayOfPickableObjects.at(i)->SetIsInteractionEnabled(true);
	}
	for (int i = 0; i < (int)(_arrayOfTaskObjects.size()); i++)
	{
		_arrayOfTaskObjects.at(i)->SetIsInteractionEnabled(true);
	}


}

void L6S5TabMiniGame::StartFadeInWhiteBg()
{
	_whiteFlashBg->SetEnable(true);
	_whiteFlashBg->SetAlpha(0);
	float tweenVals[5] = {1, -1, -1, -1, -1};
	_whiteFlashBg->TweenTo(tweenVals, 1, TW_EASE, true, CC_CALLBACK_1(L6S5TabMiniGame::OnFadeInWhiteBgComplete, this));
}

void L6S5TabMiniGame::OnFadeInWhiteBgComplete(CObject * object)
{
	if (_currentSceneNumber < 4)
	{
		LoadNextScene();
		StartFadeOutWhiteBg();
	}
	if (_currentSceneNumber == 4)
	{
		ProfileSystem::Instance->ActiveProfile.L6_S5_TabMinigameSolved = true;
	}
}

void L6S5TabMiniGame::StartFadeOutWhiteBg()
{
	_whiteFlashBg->SetEnable(true);
	_whiteFlashBg->SetAlpha(1);
	float tweenVals[5] = {0, -1, -1, -1, -1};
	_whiteFlashBg->TweenTo(tweenVals, 2, TW_EASE, true, CC_CALLBACK_1(L6S5TabMiniGame::OnCompleteFadeOutWhiteBg, this));
}

void L6S5TabMiniGame::OnCompleteFadeOutWhiteBg(CObject * object)
{
	_whiteFlashBg->SetEnable(false);
	_whiteFlashBg->SetAlpha(0);
	_isMouseInteractionEnabled = true;
}

void L6S5TabMiniGame::ReceiveMessage(int val)
{

}

void L6S5TabMiniGame::ProcessNonHOHint()
{
	AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	//AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	TabMiniGameState gameState = (TabMiniGameState)ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameState;
	switch (gameState)
	{
	case eTabMiniGameState_S0_Started:
		{
			Hud->ShowHintForSceneItem(&_s0_squirrel->Pos);
			int random = rand() % 3;
			InitHintTrailEmit(&_s0_arrayOfFruits.at(random)->Pos);
		}
		break;
	case eTabMiniGameState_S0_Squirrel_Moved:
		{
			Hud->ShowHintForSceneItem(&_s0_fixedBrick->Pos);
			InitHintTrailEmit(&_s0_brick->Pos);
		}
		break;
	case eTabMiniGameState_S1_Started:
		{
			Hud->ShowHintForSceneItem(&_s1_tree->Pos);
			InitHintTrailEmit(&_s1_saw->Pos);
		}
		break;
	case eTabMiniGameState_S1_Tree_Cut:
		{
			InitHintTrailEmit(&_s1_stone->Pos);
		}
		break;
	case eTabMiniGameState_S2_Started:
		{
			Hud->ShowHintForSceneItem(&_s2_waterPipe->Pos);
			InitHintTrailEmit(&_s2_stone->Pos);
		}
		break;
	case eTabMiniGameState_S2_Pipe_Unlocked:
		{
			if (ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameIsClothRemoved)
			{
				Hud->ShowHintForSceneItem(_s2_waterPos);
				InitHintTrailEmit(&_s2_waterTube->Pos);
			}
			else
			{
				InitHintTrailEmit(&_s2_cloth->Pos);
			}
		}
		break;
	case eTabMiniGameState_S3_Started:
		{
			const CRectangle  rabbitRect(334.0f,290.0f,109.0f,136.0f);

			for (int i = 0; i < (int)_s3_arrayOfCarrots.size(); i++)
			{
				CObject * carrot = _s3_arrayOfCarrots.at(i);
				if (carrot->Visible)
				{
					Hud->ShowHintForSceneItem(PointSystem::CreateCPoint(rabbitRect.GetCenterX(), rabbitRect.GetCenterY(), 0));
					InitHintTrailEmit(&carrot->Pos);
					break;
				}
			}
		}
		break;
	default:
		break;
	}

}

void L6S5TabMiniGame::OnPuzzleComplete(UpdateListEntity * obj)
{
	if (!ProfileSystem::Instance->ActiveProfile.L6_S5_TabMinigameSolved)
	{
		ProfileSystem::Instance->ActiveProfile.L6_S5_TabMinigameSolved = true;
		Control::Audio->QuickLoadAndPlaySFX("s5pz1_gameWin");

	}
	else
	{
		
		//CRectangle RectVid(171, 96, 1024, 576);
		CRectangle RectVid(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
		_cVideoCutScene = new CVideo("OGV\\L6\\S5ENDCUTSCENE.OGV", RectVid, false, eZOrder_CutsceneFull);
		//_cVideoCutScene->SetScale(1.334f);
		_cVideoCutScene->SetScale(GFApp->video_aspectRatio);
		_cVideoCutScene->PlayVideo();
		_cVideoCutScene->SetSkipActive();
		isCutscnPlaying = true;

		Hud->EnableInput(false);
		Hud->HideHud(false);

		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L6_S1_FloatingIsland1] = false;
		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L6_S2_FloatingIsland2] = false;
		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L6_S3_FloatingIsland3] = false;
		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L6_S4_FloatingIsland4] = false;
		//Hud->InvBox_StopPuzzleMode();
	}

}


#pragma region Scene 0

void L6S5TabMiniGame::OnS0FruitClick(CObject * obj)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	_s0_isDraggingFruit = true;
	obj->Show(false);
	_s0_fruitOnHand->Show(true);
	Control::Audio->QuickLoadAndPlaySFX("s5pz1_pluckFruit");
	for (int i = 0; i < (int)(_s0_arrayOfFruits.size()); i++)
	{
		CObject * fruit = _s0_arrayOfFruits.at(i);
		if (fruit == obj)
		{
			ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGamePluckedFruitIndex = i;
		}
		fruit->SetIsInteractionEnabled(false);
	}
}
void L6S5TabMiniGame::OnS0BrickClick(CObject * obj)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	TabMiniGameState gameState = (TabMiniGameState)ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameState;
	if (gameState == eTabMiniGameState_S0_Squirrel_Moved)
	{
		_s0_isDraggingBrick= true;
		_s0_brick->Show(false);
		_s0_brickOnHand->Show(true);
		_s0_brick->SetIsInteractionEnabled(false);
		Control::Audio->QuickLoadAndPlaySFX("s5pz1_pickBrick");
	}
	else
	{
		Hud->ShowBannerText("L6loose");
	}

}
void L6S5TabMiniGame::OnS0SquirrelClick(CObject * obj)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	if (_s0_isDraggingFruit)
	{
		ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameState = eTabMiniGameState_S0_Squirrel_Moved;
		UpdateCursorObjects();
		_s0_fruitOnHand->Show(false);
		_s0_squirrel->Show(false);
		_s0_isDraggingFruit = false;
		_s0_squirrel->SetIsInteractionEnabled(false);
		_s0_jsonAnimSquirrel->PlayAnim();
		Control::Audio->QuickLoadAndPlaySFX("s5pz1_squirrelRun");
	}
	else
	{
		Hud->ShowBannerText("L6who");
	}
}
void L6S5TabMiniGame::OnS0FixedBrickClick(CObject * obj)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	if (_s0_isDraggingBrick)
	{
		_s0_brickOnHand->Show(false);
		_s0_fixedBrick->Show(true);
		_s0_isDraggingBrick = false;
		_s0_fixedBrick->SetIsInteractionEnabled(false);
		_s0_jsonAnimRabbitEscape->PlayAnim();
		Control::Audio->QuickLoadAndPlaySFX("s5pz1_placeBrick");
		Control::Audio->QuickLoadAndPlaySFX("s5pz1_level1Complete");
	}
	else
	{
		if (ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameState == eTabMiniGameState_S0_Started)
		{
			Hud->ShowBannerText("L6should");
		}
		else if (ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameState == eTabMiniGameState_S0_Squirrel_Moved)
		{
			Hud->ShowBannerText("L6piece");
		}

	}

}
void L6S5TabMiniGame::OnS0SquirrelRunAnimComplete(int id)
{
	if (_currentSceneNumber < 3)
	{
		StartFadeInWhiteBg();
	}
}

void L6S5TabMiniGame::OnS0RabbitClick(CObject * obj)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	if (ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameState == eTabMiniGameState_S0_Started)
	{
		Hud->ShowBannerText("L6rabbit");
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameState == eTabMiniGameState_S0_Squirrel_Moved)
	{
		Hud->ShowBannerText("L6way");
	}

}

#pragma endregion



#pragma region Scene 1

void L6S5TabMiniGame::OnS1SawClick(CObject * obj)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	_s1_isDraggingSaw = true;
	obj->Show(false);
	_s1_sawOnHand->Show(true);
	_s1_saw->SetIsInteractionEnabled(false);
	Control::Audio->QuickLoadAndPlaySFX("s5pz1_sawPick");
}
void L6S5TabMiniGame::OnS1TreeClick(CObject * obj)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	if (_s1_isDraggingSaw)
	{
		_s1_sawOnHand->Show(false);
		_s1_tree->Show(false);
		_s1_isDraggingSaw = false;
		_s1_tree->SetIsInteractionEnabled(false);
		_s1_jsonAnimTreeCut->PlayAnim();
		Control::Audio->QuickLoadAndPlaySFX("s5pz1_treeCut");
	}
	else
	{
		Hud->ShowBannerText("L6wonder");
	}
}

void L6S5TabMiniGame::OnS1StonelClick(CObject * obj)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameState == eTabMiniGameState_S1_Tree_Cut)
	{
		_s1_jsonAnimRabbitEscape->PlayAnim();
		_s1_treeTop->SetIsInteractionEnabled(false);
		Control::Audio->QuickLoadAndPlaySFX("s5pz1_stoneFall");
	}
	else
	{
		Hud->ShowBannerText("L6that");
	}

}

void L6S5TabMiniGame::OnS1TreeTopClick(CObject * obj)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	Hud->ShowBannerText("L6could");
}

void L6S5TabMiniGame::OnS1TreeCutAnimComplete(int id)
{
	ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameState = eTabMiniGameState_S1_Tree_Cut;
	UpdateCursorObjects();
	_s1_treeTop->Show(true);
}

void L6S5TabMiniGame::OnS1RabbitClick(CObject * obj)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	if (ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameState == eTabMiniGameState_S1_Started)
	{
		Hud->ShowBannerText("L6huge");
	}
}

#pragma endregion


#pragma region Scene 2

void L6S5TabMiniGame::OnS2ClothClick(CObject * obj)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	_s2_cloth->FadeOut();
	ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameIsClothRemoved = true;
	Control::Audio->QuickLoadAndPlaySFX("s5pz1_clothRemove");
	UpdateCursorObjects();
}

void L6S5TabMiniGame::OnS2StoneClick(CObject * obj)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	_s2_isDraggingStone = true;
	_s2_stoneOnHand->Show(true);
	_s2_stone->Show(false);
	_s2_stone->SetIsInteractionEnabled(false);
}

void L6S5TabMiniGame::OnS2PipeClick(CObject * obj)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	if (_s2_isDraggingStone)
	{
		_s2_jsonAnimFillWater->PlayAnim();
		_s2_waterPipe->SetIsInteractionEnabled(false);
		_s2_isDraggingStone = false;
		Control::Audio->QuickLoadAndPlaySFX("s5pz1_pipeCutAndWaterFill");
	}
	else
	{
		Hud->ShowBannerText("L6break");
	}

}

void L6S5TabMiniGame::OnS2WaterTubeClick(CObject * obj)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameState == eTabMiniGameState_S2_Started)
	{
		Hud->ShowBannerText("L6life");
	}
	else if(ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameState == eTabMiniGameState_S2_Pipe_Unlocked)
	{
		_s2_isDraggingWaterTube = true;
		_s2_waterTube->Show(false);
		_s2_waterTubeOnHand->Show(true);
		Control::Audio->QuickLoadAndPlaySFX("s5pz1_takeWaterTube");
	}
}

void L6S5TabMiniGame::OnS2WaterClick(CObject * obj)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	if (_s2_isDraggingWaterTube)
	{
		_s2_waterTubeOnWater->Show(true);
		_s2_waterTubeOnHand->Show(false);
		_s2_isDraggingWaterTube = false;
		_s2_jsonAnimRabbitEscape->PlayAnim();
		_s2_water->SetIsInteractionEnabled(false);
		Control::Audio->QuickLoadAndPlaySFX("s5pz1_placeWaterTube");
		Control::Audio->QuickLoadAndPlaySFX("s5pz1_level3Complete");

	}
	else
	{
		if (ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameState == eTabMiniGameState_S2_Started)
		{
			Hud->ShowBannerText("L6water");
		}
		else if (ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameState == eTabMiniGameState_S2_Pipe_Unlocked)
		{
			Hud->ShowBannerText("L6gap");
		}

	}

}

void L6S5TabMiniGame::OnS2WaterFillingAnimComplete(int id)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameState = eTabMiniGameState_S2_Pipe_Unlocked;
	_s2_pipeWaterVFX->Show(false);
	UpdateCursorObjects();
}

void L6S5TabMiniGame::OnS2RabbitClick(CObject * obj)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	if (ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameState == eTabMiniGameState_S2_Started)
	{
		Hud->ShowBannerText("L6between");
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameState == eTabMiniGameState_S2_Pipe_Unlocked)
	{
		Hud->ShowBannerText("L6seem");
	}

}
#pragma endregion


#pragma region Scene 3

void L6S5TabMiniGame::OnS3CarrotClick(CObject * obj)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	if (!_s3_isDraggingCarrot)
	{
		_s3_isDraggingCarrot = true;
		_s3_carrotOnHand->Show(true);
		obj->Show(false);
		Control::Audio->QuickLoadAndPlaySFX("s5pz1_pickCarrot");
		for (int i = 0; i < 5; i++)
		{
			if (obj == _s3_arrayOfCarrots.at(i))
			{
				ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameIsCarrotPicked[i] = true;
				break;
			}
		}

		UpdateCursorObjects();
	}

}

void L6S5TabMiniGame::OnS3RabbitClick(CObject * obj)
{
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
	{
		return;
	}
	if (_s3_isDraggingCarrot)
	{
		_rabbit->PlayAnimation(0);
		_rabbit->PlayAnimation(1);
		Control::Audio->QuickLoadAndPlaySFX("s5pz1_eatCarrot");
		_s3_carrotOnHand->Show(false);
		_s3_isDraggingCarrot = false;
		bool isSolved = true;
		for (int i = 0; i < 5; i++)
		{
			if (ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameIsCarrotPicked[i] == false)
			{
				isSolved = false;
				break;
			}
		}
		if (isSolved)
		{
			_isMouseInteractionEnabled = false;
			_rabbit->SetIsInteractionEnabled(false);

			if (_handleExtrasGameComplete())
			{
				return;
			}

			CallFunction * callFn = new CallFunction(2, CC_CALLBACK_1(L6S5TabMiniGame::OnPuzzleComplete,this), (UpdateListEntity*)this);
			callFn->Run();

			CallFunction * callFn2 = new CallFunction(2, CC_CALLBACK_1(L6S5TabMiniGame::OnPuzzleComplete,this), (UpdateListEntity*)this);
			callFn2->Run();
		}
	}
	else
	{
		Hud->ShowBannerText("L6easy");
	}

}

#pragma endregion

