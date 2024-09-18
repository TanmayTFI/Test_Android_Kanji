//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L5_S2_PZBlocks.h"
#include "Banners.h"
#include "Application.h"
#include "SDrop.h"
#include "Util.h"
#include "Hud.h"

const int Grid_Num_Rows		= 5;
const int Grid_Num_Colums	= 6;
const int Grid_Num_Blocks	= 30; //6x4

const float Grid_Start_X = 426;
const float Grid_Start_Y = 138;
const float Grid_Width	 = 84;
const float Grid_HalfWidth = 42;

const int zorderPlacedBlock = 24;
const int zorderDraggingBlock = 40;

const CRectangle  GridBoundsRect(426.0f,138.0f,504.0f,420.0f);
const CRectangle  InvItemRectHammer(520.0f,205.0f,325.0f,271.0f);

const int SolvedStartRow[NUM_BLOCKS]={4,1,1,3,1,1,0,0};
const int SolvedStartCol[NUM_BLOCKS]={1,3,5,5,1,0,2,0};

L5_S2_PZBlocks::L5_S2_PZBlocks()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV5\\S2\\P1\\P1.SCN");

	if(!ProfileSystem::Instance->ActiveProfile.L5_S2_PZBlocksPuzzleSolved)
		Hud->InvBox_StartPuzzleMode();

	s2topdustFX = new SuperFX("PRT\\L5\\s2topdust.INI", PointSystem::CreateCPoint(691,145,0), 100); 
	s2bottomdustFX = new SuperFX("PRT\\L5\\s2bottomdust.INI", PointSystem::CreateCPoint(678,547,0), 100); 	

	s2fliesFX = new SuperFX("PRT\\L5\\s2flies.INI", PointSystem::CreateCPoint(687,173,0), 1100); 
	s2fliesFX->Start();
	s2fliesFX->AdvanceByTime(2.0f);

	s2glaresFX = new SuperFX("PRT\\L5\\s2glares.INI", PointSystem::CreateCPoint(680,107,0), 1100); 
	s2glaresFX->Start();
	s2glaresFX->AdvanceByTime(2.0f);

	HOScene->GetObjectByID("bgblueglow")->SetScale(2.00f);
	HOScene->GetObjectByID("bgblueglow")->Animate(0.25f,0.95f,4.0f,0, eBlend_AlphaAdditive);

	ResetBtn = new CObject();
	ResetBtn->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\RESETBTN.SPR", HOScene->GetObjectByID("ResetBtn")->Pos.x, HOScene->GetObjectByID("ResetBtn")->Pos.y, HOScene->GetObjectByID("ResetBtn")->ZOrder);
	ResetBtn->SetUID("resetBtn");
	HOScene->PushObject(ResetBtn);

	ResetBtn->SetEnable(false);
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		ResetBtn->SetEnable(false);

	timer = 10.0f;
	isFadeIn = false;
	isFadeOut = false;
	isResetDiaFadeIn = false;
	isResetDiaFadeOut = true;

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

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL5s1BlockPuzzleAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL5s1BlockPuzzleAmb) )		
	{
		Control::Audio->PlaySample(aAmbL5s1BlockPuzzleAmb,true);
	}
	
	//MUSIC_SFX

	InitPuzzle();

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L5_S2_PZBlocks]  = true;
}

void L5_S2_PZBlocks::InitPuzzle()
{
	isPuzzleSolved = ProfileSystem::Instance->ActiveProfile.L5_S2_PZBlocksPuzzleSolved;
	isScalingDown=false;
	isSlidingLeft=false;
	isSlideDoorOpened = isPuzzleSolved;
	endTimer = 0;

	isSkipAnimPlaying = false;
	skipTimer = 0;

	isDragging = false;
	dragBlockID = -1;

	wasMouseInsideGridRect = false;

	objGridComplete = HOScene->GetObjectByID("gridcomplete");
	objGridComplete->SetMaskRect(GridBoundsRect);
	objGridComplete->SetEnable(false);
	objGridStartx = objGridComplete->Pos.x;
	if(!isPuzzleSolved)
	{
		HOScene->GetObjectByID("hammer")->SetEnable(false);
		HOScene->GetObjectByID("ground")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("hammer")->SetEnable(true);
		HOScene->GetObjectByID("ground")->SetEnable(true);
	}

	//scnStonePolyArray
	int Pts0[] = {441,36,449,116,784,103,777,26};
	CPolygon* poly0=  new CPolygon(4);
	poly0->Init(Pts0);
	int Pts1[] = {194,98,234,147,186,187,244,265,295,227,341,272,405,221,370,168,404,137,363,67,307,96,266,51};
	CPolygon* poly1=  new CPolygon(12);
	poly1->Init(Pts1);

	int Pts2[] = {328,318,324,394,408,396,409,318};
	CPolygon* poly2=  new CPolygon(4);
	poly2->Init(Pts2);
	int Pts3[] = {366,450,323,514,373,569,424,511,424,499};
	CPolygon* poly3=  new CPolygon(5);
	poly3->Init(Pts3);

	int Pts4[] = {986,323,1061,341,1048,402,1113,418,1103,479,1171,498,1150,574,1003,542,1016,480,954,465};
	CPolygon* poly4=  new CPolygon(10);
	poly4->Init(Pts4);
	int Pts5[] = {922,51,984,296,1047,280,1043,227,1091,208,1081,142,1028,146,993,29};
	CPolygon* poly5=  new CPolygon(8);
	poly5->Init(Pts5);
	int Pts6[] = {247,278,318,299,281,432,331,446,337,461,296,586,218,563,238,502,175,475};
	CPolygon* poly6=  new CPolygon(9);
	poly6->Init(Pts6);
	int Pts7[] = {1121,186,1193,197,1167,404,1097,391};
	CPolygon* poly7=  new CPolygon(4);
	poly7->Init(Pts7);

	scnStonePolyArray.push_back(poly0);
	scnStonePolyArray.push_back(poly1);
	scnStonePolyArray.push_back(poly2);
	scnStonePolyArray.push_back(poly3);
	scnStonePolyArray.push_back(poly4);
	scnStonePolyArray.push_back(poly5);
	scnStonePolyArray.push_back(poly6);
	scnStonePolyArray.push_back(poly7);

	
	int val0[] ={1, 1, 1, 1, 1};
	TetrisBlock* block0 = new TetrisBlock(HOScene->GetObjectByID("stonebig_0"), HOScene->GetObjectByID("shadow0"), 9,5, 1, 5, val0);
	block0->setDefaultOffsets(0,2);
	blocks_big.push_back(block0);

	int val1[] ={0,1,0, 1,1,1, 0,1,0};
	TetrisBlock* block1 = new TetrisBlock(HOScene->GetObjectByID("stonebig_1"), HOScene->GetObjectByID("shadow1"), 3,2, 3, 3, val1);
	block1->setDefaultOffsets(1,1);
	blocks_big.push_back(block1);

	int val2[] ={1};
	TetrisBlock* block2 = new TetrisBlock(HOScene->GetObjectByID("stonebig_2"), HOScene->GetObjectByID("shadow2"), 9,5, 1, 1, val2);
	block2->setDefaultOffsets(0,0);
	blocks_big.push_back(block2);

	int val3[] ={1};
	TetrisBlock* block3 = new TetrisBlock(HOScene->GetObjectByID("stonebig_3"), HOScene->GetObjectByID("shadow3"), 9,4, 1, 1, val3);
	block3->setDefaultOffsets(0,0);
	blocks_big.push_back(block3);

	int val4[] ={1,0,0, 1,1,0, 0,1,1};
	TetrisBlock* block4 = new TetrisBlock(HOScene->GetObjectByID("stonebig_4"), HOScene->GetObjectByID("shadow4"), 9,5, 3, 3, val4);
	block4->setDefaultOffsets(1,0);
	blocks_big.push_back(block4);

	int val5[] ={1,0, 1,0, 1,1, 1,0};
	TetrisBlock* block5 = new TetrisBlock(HOScene->GetObjectByID("stonebig_5"), HOScene->GetObjectByID("shadow5"), 9,5, 4, 2, val5);
	block5->setDefaultOffsets(1,0);
	blocks_big.push_back(block5);

	int val6[] ={0,1,1,1, 1,1,0,0};
	TetrisBlock* block6 = new TetrisBlock(HOScene->GetObjectByID("stonebig_6"), HOScene->GetObjectByID("shadow6"), 9,5, 2, 4, val6);
	block6->setDefaultOffsets(0,1);
	blocks_big.push_back(block6);

	int val7[] ={1,1,1};
	TetrisBlock* block7 = new TetrisBlock(HOScene->GetObjectByID("stonebig_7"), HOScene->GetObjectByID("shadow7"), 0,5, 1, 3, val7);
	block7->setDefaultOffsets(0,0);
	blocks_big.push_back(block7);

	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		blocks_thumbs.push_back(HOScene->GetObjectByID(std::string("stonescn" + MKSTR(i)).c_str()));
		blocks_big[i]->setEnable(false);

		if(isPuzzleSolved)
			blocks_thumbs[i]->Show(false);
	}


	InitGrid();
	if(!isPuzzleSolved)
		RestorePuzzleState();
}

void L5_S2_PZBlocks::RestorePuzzleState()
{
	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		if(ProfileSystem::Instance->ActiveProfile.L5_S2_PZBlockStartRow[i]>-1) //if not -1, block is placed on grid
		{
			blocks_thumbs[i]->Show(false);

			blocks_big[i]->isPlacedOnGrid = true;
			blocks_big[i]->getObj()->SetEnable(true);
			blocks_big[i]->startRow = ProfileSystem::Instance->ActiveProfile.L5_S2_PZBlockStartRow[i];
			blocks_big[i]->startCol = ProfileSystem::Instance->ActiveProfile.L5_S2_PZBlockStartCol[i];

			int blockID = blocks_big[i]->startRow*Grid_Num_Colums + blocks_big[i]->startCol;

			blocks_big[i]->getObj()->SetPos(&gridRectArray[blockID]->topleft); //Place it on column

			//To set index array
			getValueArrayFromGrid(blocks_big[i]->startRow, blocks_big[i]->startCol, blocks_big[i]->getRows(), blocks_big[i]->getColumns());

			int size = (int)indexArray.size();  //Sac: converted Implicit to explicit
			for (int k = 0; k < size; k++)
			{
				if(blocks_big[i]->values[k]==1)
				{
					gridValArray[indexArray[k]] = 1; //Set Grid Value as Block occupied
				}
			}
		}
	}
}

void L5_S2_PZBlocks::InitGrid()
{
	CObject* tempGlow = HOScene->GetObjectByID("blueglow");

	for (int i = 0; i < Grid_Num_Blocks; i++)
	{
		gridValArray.push_back(0); //0 for empty - 1 for filled

		int row = i/Grid_Num_Colums;
		int col = i%Grid_Num_Colums;

		CObject* gridGlow = new CObject();
		gridGlow->LoadInit("LV5\\S2\\P1\\blueglow.SPR", 0, 0, tempGlow->ZOrder);
		gridGlow->SetBlendMode(eBlend_AlphaAdditive);
		gridGlow->Pos.x = Grid_Start_X+col*Grid_Width;
		gridGlow->Pos.y = Grid_Start_Y+row*Grid_Width;

		grid_glowArray.push_back(gridGlow);

		glow_TargetAlpha.push_back(0);

		gridGlow->SetAlpha(0);

		CRectangle* rect = new CRectangle(Grid_Start_X+col*Grid_Width, Grid_Start_Y+row*Grid_Width, Grid_Width, Grid_Width);
		gridRectArray.push_back(rect);
	}

	tempGlow->SetEnable(false);
}


void L5_S2_PZBlocks::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Control::Audio->IsPlaying(aTrackL5theme) )
	{
		Control::Audio->Samples[aTrackL5theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL5theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL5theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL5puzzle) )
				  Control::Audio->PlaySample(aTrackL5puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL5puzzle))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL5puzzle, true);
		}
	}
}

void L5_S2_PZBlocks::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

#ifdef DISABLE_PREV_CUR
	Hud->PrevCursorMode = -1;
#endif

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup )
		return;

	ResetPuzzle();
	if(IsResetPop)
	{
		GFHud->AllowInput = false;
		return;
	}
	else
		GFHud->AllowInput = true;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	if(isPuzzleSolved)
	{
		if (ProfileSystem::Instance->_isExtrasGamePlay)
			return;

		if(isSkipAnimPlaying)
		{
			skipTimer += Control::LogicRate;
			if(skipTimer>3.5f)
			{
				for (int i = 0; i < NUM_BLOCKS; i++)
				{
					blocks_big[i]->getObj()->FadeOut(2.0f);
				}
				objGridComplete->SetEnable(true);
				HOScene->GetObjectByID("hammer")->SetEnable(true);
				HOScene->GetObjectByID("ground")->SetEnable(true);

				s2topdustFX->Start();
				s2bottomdustFX->Start();

				Control::Audio->QuickLoadAndPlaySFX("s2pz1_puzzlecomplete");

				isScalingDown = true;
				endTimer = 0;
				isSkipAnimPlaying = false;
			}
		}
		else if(isScalingDown)
		{
			endTimer += Control::LogicRate;
			if(endTimer>=0)
			{
				objGridComplete->SetScale(1.0f - 0.12f*easing(endTimer,0,1,1.8f));
				if(endTimer>1.8f)
				{				
					isScalingDown = false;

					endTimer = 0;
					isSlidingLeft = true;
				}
			}
		}
		else if(isSlidingLeft)
		{
			endTimer += Control::LogicRate;
			objGridComplete->Pos.x = objGridStartx - 800.0f*easing(endTimer,0,1,2.5f);
			if(endTimer>2.5f)
			{
				Hud->InvBox_StopPuzzleMode();

				objGridComplete->FadeOut(4.0f);
				isSlidingLeft = false;
				endTimer = 0;
			}
		}
		else
		{
			if(!ProfileSystem::Instance->ActiveProfile.L5_S2_InvHammerTaken && InvItemRectHammer.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if( Control::Input->LBclicked() )
				{
					HOScene->GetObjectByID("hammer")->SetEnable(false);
					ProfileSystem::Instance->ActiveProfile.L5_S2_InvHammerTaken = true;
					Hud->TakeInventoryItem(eInv_L5_S2_Hammer);
					return;
				}
			}
			else if( IsMouseOverGoBackArea(&pos) )
			{
				
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown, eArea_L5_S2_TigerCloseup);
#else
		Cursor::SetMode(CA_ExitDown, eArea_L5_S2_TigerCloseup);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L5_S2_TigerCloseup, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S2_TigerCloseup;
					SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
					
				}	
			}
			#ifdef ENABLE_ANDROID_BACK_BUTTON
			if (Control::Input->isKeyBackSensed())
			{
				//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S2_TigerCloseup;
				SendMessage(ControlExit, ID, HOSM_NewArea);						
			}	
			#endif	

		}
		return;
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed()&& !isDragging)
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S2_TigerCloseup;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}	
	#endif	
	if( IsMouseOverGoBackArea(&pos) && !isDragging)
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown, eArea_L5_S2_TigerCloseup);
#else
		Cursor::SetMode(CA_ExitDown, eArea_L5_S2_TigerCloseup);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L5_S2_TigerCloseup, 2);
			}
#else			
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S2_TigerCloseup;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
								
		}	
	}

	if(!isDragging)
	{
		for (int i = 0; i < NUM_BLOCKS; i++)
		{
			//clear big block tint
			if(blocks_big[i]->IsActive())
				blocks_big[i]->SetTint(false);
		}
		//check for taking blocks
		for (int i = 0; i < NUM_BLOCKS; i++)
		{
			if(blocks_thumbs[i]->Visible && scnStonePolyArray[i]->Intersection(&pos))
			{
				Cursor::SetMode(CA_HandPointy);
				if( Control::Input->LBclicked() )
				{
					Control::Audio->QuickLoadAndPlaySFX("s2pz1_blocktake");

					isDragging = true;
					dragBlockID = i;

					blocks_thumbs[i]->Show(false);
					blocks_big[i]->SetZ(zorderDraggingBlock);
					blocks_big[i]->setPos(pos, (int)(Grid_HalfWidth), (int)(Grid_HalfWidth));
					blocks_big[i]->setEnable(true);
				}
				break;
			}
			else if(blocks_big[i]->isCollided(pos))
			{
				Cursor::SetMode(CA_HandPointy);
				blocks_big[i]->SetTint(true);
				if( Control::Input->LBclicked() )
				{
					Control::Audio->QuickLoadAndPlaySFX("s2pz1_blocktake");

					isDragging = true;
					dragBlockID = i;

					eraseBlockGridData(blocks_big[i]);

					blocks_thumbs[i]->Show(false);
					blocks_big[i]->SetTint(false);
					blocks_big[i]->SetZ(zorderDraggingBlock);
					blocks_big[i]->setPos(pos, (int)(Grid_HalfWidth), (int)(Grid_HalfWidth));
					blocks_big[i]->setEnable(true);
				}
				break;
			}
		}
	}
	else
	{
		if (dragBlockID!=-1)
		{
			blocks_big[dragBlockID]->setPos(pos, (int)(Grid_HalfWidth), (int)(Grid_HalfWidth));
			updateGlowAroundMouse(pos);
			if( Control::Input->RBclicked() )
			{
				Control::Audio->QuickLoadAndPlaySFX("s2pz1_blocknotfixed");

				blocks_big[dragBlockID]->setEnable(false);
				blocks_thumbs[dragBlockID]->Show(true);
				
				isDragging = false;
				dragBlockID = -1;

				wasMouseInsideGridRect = false;
				clearGridGlows();
			}
			else if( Control::Input->LBclicked() )
			{
				CheckAndPlaceBlockOnGrid(pos);
			}
		}
		else
		{
				WARN("Er0x2301- Bad Drag ID");
		}
	}
}

float L5_S2_PZBlocks::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L5_S2_PZBlocks::clearGridGlows()
{
	for (int i = 0; i < Grid_Num_Blocks; i++)
	{
		glow_TargetAlpha[i] = 0;
		grid_glowArray[i]->SetAlpha(0);
	}
}


void L5_S2_PZBlocks::updateGlowAroundMouse(CPoint pos)
{
	if(GridBoundsRect.Intersection(&pos))
	{
		wasMouseInsideGridRect = true;
		int row=-1, col=-1;
		int glowID=-1;

		for (int i = 0; i < Grid_Num_Blocks; i++)
		{
			float alpha = 0;
			if(glow_TargetAlpha[i]>0)
			{
				alpha = grid_glowArray[i]->pSprite->Alpha + 2.0f*Control::LogicRate;
				if(alpha>glow_TargetAlpha[i])
					alpha = glow_TargetAlpha[i];
			}
			else
			{
				alpha = grid_glowArray[i]->pSprite->Alpha - 2.0f*Control::LogicRate;
				if(alpha<0)
					alpha=0;
			}
			grid_glowArray[i]->SetAlpha(alpha);

			if(glowID == -1 && gridRectArray[i]->Intersection(&pos))
			{
				glowID = i;

				row = i/Grid_Num_Colums;
				col = i%Grid_Num_Colums;
			}
		}

		if(glowID>0)
		{
			for (int i = 0; i < Grid_Num_Blocks; i++)
				glow_TargetAlpha[i] = 0;

			glow_TargetAlpha[glowID] = 1.0f; //grid_glowArray[glowID]->SetAlpha(1.0f);

			int top=glowID-Grid_Num_Colums;
			int topLeft=glowID-Grid_Num_Colums-1;
			int topRight=glowID-Grid_Num_Colums+1;
			int bot=glowID+Grid_Num_Colums;
			int botLeft=glowID+Grid_Num_Colums-1;
			int botRight=glowID+Grid_Num_Colums+1;

			if( row>0 )
			{
				glow_TargetAlpha[top] = 0.75f; //grid_glowArray[top]->SetAlpha(0.75f);
				if( col>0 )
					glow_TargetAlpha[topLeft] = 0.65f; //grid_glowArray[topLeft]->SetAlpha(0.65f); //TopLeft
				if( col<(Grid_Num_Colums-1) )
					glow_TargetAlpha[topRight] = 0.65f;//grid_glowArray[topRight]->SetAlpha(0.65f); //TopRight
			}
			if( row<(Grid_Num_Rows-1) )
			{
				glow_TargetAlpha[bot] = 0.75f; //grid_glowArray[bot]->SetAlpha(0.75f);
				if( col>0 )
					glow_TargetAlpha[botLeft] = 0.65f; //grid_glowArray[botLeft]->SetAlpha(0.65f); //BottomLeft
				if( col<(Grid_Num_Colums-1) )
					glow_TargetAlpha[botRight] = 0.65f; //grid_glowArray[botRight]->SetAlpha(0.65f); //BottomRight
			}
			if( col>0 )
				glow_TargetAlpha[glowID-1] = 0.75f; //grid_glowArray[glowID-1]->SetAlpha(0.75f);
			if( col<(Grid_Num_Colums-1) )
				glow_TargetAlpha[glowID+1] = 0.75f; //grid_glowArray[glowID+1]->SetAlpha(0.75f);
		}
	}
	else
	{
		//check if on mouseout any cells were in glow state
		if(wasMouseInsideGridRect)
		{
			wasMouseInsideGridRect = false;
			clearGridGlows();
		}
	}
}

bool L5_S2_PZBlocks::checkForGameWin()
{
	bool gameWon = true;
	for (int i = 0; i < Grid_Num_Blocks; i++)
	{
		if(gridValArray[i]==0)
		{
			gameWon = false;
			break;
		}
	}
	return gameWon;
}


void L5_S2_PZBlocks::CheckAndPlaceBlockOnGrid(CPoint pos)
{
	bool isCanBePlaced = false;

	wasMouseInsideGridRect = false;
	clearGridGlows();

	//check if clicked inside grid
	int clickedGridRow=-1, clickedGridColumn = -1;
	int blockID = -1;
	for (int i = 0; i < Grid_Num_Blocks; i++)
	{
		if(gridRectArray[i]->Intersection(&pos))
		{
			clickedGridRow = i/Grid_Num_Colums;
			clickedGridColumn = i%Grid_Num_Colums;
			blockID = i;

			//convert clickedGridRow relative to TopLeft of each block
			clickedGridRow -= blocks_big[dragBlockID]->_rowOffset;
			clickedGridColumn -= blocks_big[dragBlockID]->_columnOffset;
			if(clickedGridRow<0)
			{
				isCanBePlaced = false;
				//SLOG::Log("Out of Bounds TOP");
			}
			else if(clickedGridColumn<0)
			{
				isCanBePlaced = false;
				//SLOG::Log("Out of Bounds Left");
			}
			else
			{
				blockID = clickedGridRow*Grid_Num_Colums + clickedGridColumn;
				//SLOG::Log("place in BlockID-",blockID);
				isCanBePlaced = true;
			}
			break;
		}
	}
	

	if(isCanBePlaced)
	{
		//check for empty space and overflow
		if(checkForEmptySpace(blocks_big[dragBlockID], clickedGridRow, clickedGridColumn))
		{
			//SLOG::Log("Empty Space Found");

			Control::Audio->QuickLoadAndPlaySFX("s2pz1_blockfix");

			//SetBlockOnGrid(startBlockID)
			blocks_big[dragBlockID]->setEnableShadow(false);
			blocks_big[dragBlockID]->SetZ(zorderPlacedBlock);

			blocks_big[dragBlockID]->setPosWithoutOffset(gridRectArray[blockID]->topleft);
			blocks_big[dragBlockID]->startRow = clickedGridRow;
			blocks_big[dragBlockID]->startCol = clickedGridColumn;

			blocks_big[dragBlockID]->isPlacedOnGrid = true;

			int size = (int)indexArray.size();  //Sac: converted Implicit to explicit
			for (int i = 0; i < size; i++)
			{
				if(blocks_big[dragBlockID]->values[i]==1)
				{
					gridValArray[indexArray[i]] = 1; //Set Grid Value as Block occupied
				}
			}
			//SLOG::Log("gridValArray post place:",gridValArray, Grid_Num_Colums);
			if(checkForGameWin())
			{				
				SolvePuzzle();
			}
		}
		else
		{
			Control::Audio->QuickLoadAndPlaySFX("s2pz1_blocknotfixed");

			blocks_big[dragBlockID]->isPlacedOnGrid = false;
			blocks_big[dragBlockID]->setEnable(false);
			blocks_thumbs[dragBlockID]->Show(true);
		}
	}
	else
	{
		Control::Audio->QuickLoadAndPlaySFX("s2pz1_blocknotfixed");

		blocks_big[dragBlockID]->isPlacedOnGrid = false;
		blocks_big[dragBlockID]->setEnable(false);
		blocks_thumbs[dragBlockID]->Show(true);
	}

	isDragging = false;
	dragBlockID = -1;

	SavePuzzleState();
}

void L5_S2_PZBlocks::SavePuzzleState()
{
	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		if(blocks_big[i]->isPlacedOnGrid)
		{
			ProfileSystem::Instance->ActiveProfile.L5_S2_PZBlockStartRow[i] = blocks_big[i]->startRow;
			ProfileSystem::Instance->ActiveProfile.L5_S2_PZBlockStartCol[i] = blocks_big[i]->startCol;
		}
		else
		{
			ProfileSystem::Instance->ActiveProfile.L5_S2_PZBlockStartRow[i] = -1;
			ProfileSystem::Instance->ActiveProfile.L5_S2_PZBlockStartCol[i] = -1;
		}
	}
}

void L5_S2_PZBlocks::SolvePuzzle(bool isSkipped)
{
	AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	ProfileSystem::Instance->ActiveProfile.L5_S2_PZBlocksPuzzleSolved = true;
	isPuzzleSolved = true;

	if(_handleExtrasGameComplete())
		return;
	clearGridGlows();
	if(isSkipped)
	{
		isSkipAnimPlaying = true;
		for (int i = 0; i < NUM_BLOCKS; i++)
		{
			blocks_thumbs[i]->FadeOut(2.0f);

			int blockID = SolvedStartRow[i]*Grid_Num_Colums + SolvedStartCol[i];
			float diffX = gridRectArray[blockID]->topleft.x - blocks_thumbs[i]->Pos.x;
			float diffY = gridRectArray[blockID]->topleft.y - blocks_thumbs[i]->Pos.y;

			blocks_big[i]->getObj()->SetPos(&blocks_thumbs[i]->Pos);
			blocks_big[i]->getObj()->TweenPosTo(diffX, diffY, 0, 1.8f+0.2f*i, TW_EASEQUADIN, true);

			blocks_big[i]->setEnable(true);
			blocks_big[i]->setEnableShadow(false);

			//blocks_big[i]->setPosWithoutOffset(gridRectArray[blockID]->topleft);
			//blocks_big[i]->SetZ(zorderPlacedBlock);
		}
	}
	else
	{
		Control::Audio->QuickLoadAndPlaySFX("s2pz1_puzzlecomplete");

		for (int i = 0; i < NUM_BLOCKS; i++)
		{
			blocks_big[i]->getObj()->FadeOut(2.0f);
		}
		objGridComplete->SetEnable(true);
		HOScene->GetObjectByID("hammer")->SetEnable(true);
		HOScene->GetObjectByID("ground")->SetEnable(true);

		s2topdustFX->Start();
		s2bottomdustFX->Start();

		isScalingDown = true;
		endTimer = -2.0f;
		//play dust particle
	}
}

void L5_S2_PZBlocks::eraseBlockGridData(TetrisBlock* block)
{
	std::vector<int> valArray = getValueArrayFromGrid(block->startRow, block->startCol, block->getRows(), block->getColumns());

	int size = (int)indexArray.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		if(block->values[i]==1)
		{
			gridValArray[indexArray[i]] = 0; //Set Grid Value as empty
		}
	}
}

bool L5_S2_PZBlocks::checkForEmptySpace(TetrisBlock *block, int clickRow, int clickColumn)
{
	bool isGridBlocksEmpty = true;

	int blockRows = block->getRows();
	int blockColumns = block->getColumns();
	
	// int currentRowStart = (clickRow)*Grid_Num_Colums;  // Sac: Commenting Unused variable
//	int currentRowEnd = (clickRow+1)*Grid_Num_Colums;  // Sac: Commenting Unused variable

	if(clickColumn+blockColumns>Grid_Num_Colums) //if block width exceeds grid width
	{
		//SLOG::Log("Block goes out right");
		return false;
	}
	
	if(clickRow+blockRows>Grid_Num_Rows) //if block width exceeds grid height
	{
		//SLOG::Log("Block goes out Down");
		return false;
	}

	std::vector<int> valArray = getValueArrayFromGrid(clickRow, clickColumn, blockRows, blockColumns);

	//SLOG::Log("index Array:", indexArray);
	//SLOG::Log("valArray:", valArray);

    int size =(int) valArray.size(); //Sac: Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		if( (blocks_big[dragBlockID]->values[i] == 1) && (valArray[i]==1))
		{
			//Block Block Collision
			isGridBlocksEmpty = false;
			//SLOG::Log("Block block Collision");
			break;
		}
	}

	return isGridBlocksEmpty;
}

std::vector<int> L5_S2_PZBlocks::getValueArrayFromGrid(int clickRow, int clickColumn, int numRows, int numCols)
{
	std::vector<int> temp;

	int gridStartIndex = clickRow*Grid_Num_Colums + clickColumn;

	indexArray.clear();

	int index = gridStartIndex;
	for (int i = 0; i < numRows; i++)
	{
		for (int k = 0; k < numCols; k++)
		{
			temp.push_back(gridValArray[index]);
			indexArray.push_back(index);

			index++;
		}
		index = gridStartIndex + (i+1)*Grid_Num_Colums;
	}

	return temp;
}

void L5_S2_PZBlocks::ReceiveMessage(int val)
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
			GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{
		if(isPuzzleSolved && ProfileSystem::Instance->ActiveProfile.L5_S2_InvHammerTaken)
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S2_TigerCloseup;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
	}
}

void L5_S2_PZBlocks::ProcessSkip()
{
	if(!isPuzzleSolved)
	{
		AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
		SolvePuzzle(true);
	}
}

void L5_S2_PZBlocks::ProcessNonHOHint()
{
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
			if(isPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L5_S2_InvHammerTaken)
			{
				InitHintTrailEmit(&InvItemRectHammer);
				return;
			}
			break;
		case 2: ++iNonHOHint;

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

L5_S2_PZBlocks::~L5_S2_PZBlocks()
{
	int size = (int)blocks_big.size(); //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		SAFE_DELETE(blocks_big[i]);
	}
	blocks_big.clear();

	size = (int)gridRectArray.size(); //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		SAFE_DELETE(gridRectArray[i]);
	}
	gridRectArray.clear();

	size = (int)scnStonePolyArray.size(); //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		SAFE_DELETE(scnStonePolyArray[i]);
	}
	scnStonePolyArray.clear();	

	size = (int)grid_glowArray.size(); //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		SAFE_DELETE(grid_glowArray[i]);
	}
	grid_glowArray.clear();	

	SAFE_DELETE(s2topdustFX);
	SAFE_DELETE(s2bottomdustFX);
	SAFE_DELETE(s2fliesFX);
	SAFE_DELETE(s2glaresFX);

	SAFE_DELETE(ResetDia);

	if(!isPuzzleSolved)
		Hud->InvBox_StopPuzzleMode();

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL5s1BlockPuzzleAmb);
	Control::Audio->UnloadSample(aAmbL5s1BlockPuzzleAmb);
	//MUSIC_SFX
}

TetrisBlock::TetrisBlock(CObject *obj, CObject *shadow, int shadowOffsetX, int shadowOffsetY, int numRow, int numCol, int val[])
{
	_obj = obj;
	_shadow = shadow;

	_shadowOff.x = (float)shadowOffsetX;
	_shadowOff.y = (float)shadowOffsetY;

	_rows = numRow;
	_colums = numCol;
	_numGridBlocks = _rows*_colums;

	for (int i = 0; i < MAX_BLOCK_GRIDSIZE; i++)
	{
		if(i < _numGridBlocks)
			values[i] = val[i];
		else
			values[i] = 0;
	}

	setDefaultOffsets(0, 0);

	isPlacedOnGrid = false;
}

void TetrisBlock::setPos(CPoint pos, int xOff, int yOff) {
	CPoint temp(pos.x-xOff-(_columnOffset*Grid_Width), pos.y-yOff-(_rowOffset*Grid_Width), 0);
	_obj->SetPos(&temp);
	temp = temp-_shadowOff;
	_shadow->SetPos(&temp);
};

void TetrisBlock::setPosWithoutOffset(CPoint pos) {
	_obj->SetPos(&pos);
	CPoint temp = pos-_shadowOff;
	_shadow->SetPos(&temp);
};

bool TetrisBlock::isCollided(CPoint pos)
{
	bool isCollided = false;

	if(_obj->IsActive && _obj->Collide(&pos))
	{
		//check for blocks
		for (int i = 0; i < _numGridBlocks; i++)
		{
			int row=i/_colums;
			int col=i%_colums;
			if(values[i]==1) //if block is there
			{
				CRectangle rect(_obj->Pos.x+col*Grid_Width, _obj->Pos.y+row*Grid_Width, Grid_Width, Grid_Width);
				if(rect.Intersection(&pos))
				{
					isCollided = true;
					break;
				}
			}
		}
	}

	return isCollided;
}

void L5_S2_PZBlocks::ResetPuzzle()
{
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		return;
	if(ProfileSystem::Instance->ActiveProfile.L5_S2_PZBlocksPuzzleSolved)
	{
		ResetBtn->SetEnable(false);
		return;
	}
	ResetBtn->SetEnable(true);
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
			for (int i = 0; i < MAX_L5S2_NUM_BLOCKS; i++)
				ProfileSystem::Instance->ActiveProfile.L5_S2_PZBlockStartRow[i] = -1;
			for (int i = 0; i < MAX_L5S2_NUM_BLOCKS; i++)
				ProfileSystem::Instance->ActiveProfile.L5_S2_PZBlockStartCol[i] = -1;

			ProfileSystem::Instance->ActiveProfile.L5_S2_PZBlocksPuzzleSolved = false;

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L5_S2_PZBlocks;
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
void L5_S2_PZBlocks::NavigateToScene()
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