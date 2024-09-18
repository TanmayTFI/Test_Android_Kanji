//====================================
//DQFULP
//====================================

#include "HiddenObjectSystem.h"
#include "Hud.h"
#include "HOScene.h"
#include "ParticleEffect.h"
#include "exMath.h"
#include "GameFlow.h"
#include "DataEnums.h"
#include "Application.h"

#ifdef _TABINGAMEUI
#include "Fonts.h"
#endif // _TABINGAMEUI

HiddenObjectSystem::HiddenObjectSystem()
{	
	Hud = NULL;
	HOScene = NULL;

	//Sparkle Handling
	//For initialize sparkle pending as soon as constructor kicks in
	isSparkleUpdatePending=true;
	sparkleTimer = 0;
	sparkleThreshold = 8.0f;

	SparkleFX = NULL;	
	currentSparkleID=0;
	numSparkles=0;
	for (int i = 0; i < MAX_NUMSPARKLES; i++)
	{
		sparklePoints[i].SetXY(0,0);
	}
	//Sparkle Handling

	//Set CurrentLevel Visited
	IsVisitedForFirstTime = false;
	int currentAreaID = ProfileSystem::Instance->ActiveProfile.CurrentArea;
	if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[currentAreaID])
	{
		IsVisitedForFirstTime = true;
		ProfileSystem::Instance->ActiveProfile.IsAreaVisited[currentAreaID] = true;
	}

	_extrasCompleteTimer = 0.0f;

#ifdef _TABINGAMEUI
	_TabInGameMenuShowing = false;
	_TabInGameAwardEventShowing = false;

	TabMenuMaskBg = new CObject();
	TabMenuMaskBg->LoadInit("NONGAME\\TABMENU\\MASK.SPR", 683, 384, eZOrder_CutsceneFull+10);
	TabMenuMaskBg->SetScale(2.0);
	TabMenuMaskBg->Show(false);

	//Award Events
	std::string AwardEventNames[eAwardEventMax] =
	{
		"StartMainStory",
		"CompleteMainStory",
		"CompleteBonusStory",
		"StartHO",
		"CompleteHO",
		"QuitHO",
		"StartMiniGame",
		"CompleteMiniGame",
		"QuitMiniGame", 
		"UseHoHint",
		"SkipMiniGame",
		"SkipDialogueOrCutScene",
		"CompleteCreeksMansion",
		"UseFastTravell",
		"StartJourneyToNarara",
		"GotOutOftreasureRoom",
		"CollectInventoryItem",
		"CollectMemory",
		"CollectMorphing",
		"SkipTutorial",
		"Pick5ItemsWithinTime",
		"CompleteHOWithMatch3",
		"CollectAward"
	};

	//Awards Texts
	float initX = 120, initY = 20, awardtextGap=28;
	int awardStartIndex = 0;
	for (int i = 0; i < eAwardEventMax; i++)
	{
		ColorRGB blue = {(93/255.0f),(226/255.0f),(255/255.0f)};
		_AwardEventNames[i] = new CBitmapText();
		_AwardEventNames[i]->SetTextProps(StringTable::getStr(AwardEventNames[i]), initX, initY+(awardtextGap*(i-awardStartIndex)), eZOrder_CutsceneFull+15, blue, eFont_Italic24);
		_AwardEventNames[i]->Show(false);

		if((i + 1)%12 == 0)
		{
			initX += 240;
			awardStartIndex = i;
		}
	}

	std::string SceneNames[eArea_Max+1] = //Sac: for Text ids, same as above string, just uses Underscore for whitespaces inbetween
	{ 
		//Level 1 - demo
		"Beach",				//eArea_L1_S1_Beach
		"Sofias_Painting",		//eArea_L1_S1_PZKidsPainting
		"Front_yard",	//eArea_L1_S2_Frontyard
		"Fuse_Puzzle",			//eArea_L1_S2_PZFuse

		"Living_Room",				//eArea_L1_S3_Foyer
		"Shelf_Puzzle",			//eArea_L1_S3_PZShelf
		"Shadowgraphy_Puzzle",	//eArea_L1_S4_PZDoor
		"Bedroom",				//eArea_L1_S4_Bedroom
		"Painting_Puzzle",		//eArea_L1_S4_PZPainting
		"Metal_Wall_HO",		//eArea_L1_S3_DoorHO

		"Treasure_Room",		//eArea_L1_S5_TreasureRoom
		"Tunnel_Door",			//eArea_L1_S5_TunnelDoor
		"Dynamite_Puzzle",		//eArea_L1_S5_PZDynamiteMaking
		"TreasureRoom_HO",		//eArea_L1_S5_HOTreasureRoom
		"Shed",			//eArea_L1_S6_BoatShed
		"ToolBox_HO",			//eArea_L1_S6_HOBoatShed
		"Dock",			//eArea_L1_S7_BoatDock

		"End_Of_Demo",			//eArea_L1_EndOfDemo
		"Sofias_Room",			//eArea_KidsRoom
		"Extras_Cutscene_Player",//eArea_ExtrasCutscenePlayer
		"Extras_Room",			//eArea_ExtrasRoom
		"Awards_Room",			//eArea_AwardsRoom

		"Forest_Entrance",		//eArea_L2_S1_ForestEntrance,
		"Underground",			//eArea_L2_S1_UndergroundHO,
		"Mango_Puzzle",			//eArea_L2_S2_PZMango,
		"Naga_Temple",          //eArea_L2_S2_NagaTemple,
		"HO_Naga_Temple",		//eArea_L2_S2_HONagaTemple
		"Cave_Mouth",			//eArea_L2_S3_CaveMouth
		"Stone_Beam_Puzzle",	//eArea_L2_S3_PZStoneBeam,
		"Cave",					//eArea_L2_S4_Cave,
		"Deities_Puzzle",		//eArea_L2_S4_PZDeities
		"Moopans_Den",			//eArea_L2_S5_MoopansDen,
		"Eagle_Unblock_Puzzle",	//eArea_L2_S5_EagleUnblock,
		"Medicine_Puzzle",		//eArea_L2_S5_PZ_Medicine_Making
		"Tunnel_End",			//eArea_L2_S6_TunnelEnd,
		"Waterfall_HO",			//eArea_L2_S6_HOWaterfall
		"Clock_Puzzle",			//eArea_L2_S6_PZTunnelClock,
		"Soldiers_Puzzle",		//eArea_L2_S7_PZSoldiers

		"Fort_Entrance",		//eArea_L3_S1_FortEntrance,
		"Ladder_HO",		//eArea_L3_S1_HOLadder,
		"Lion_Head_Puzzle",		//eArea_L3_S1_PZLionHead,
		"Fort_Interior",		//eArea_L3_S2_FortInterior,
		"Maze_Room",			//eArea_L3_S2_P1Room1Start,
		"MazeRoomWines",		//eArea_L3_S2_P1Room2Wine,
		"MazeRoomLock",			//eArea_L3_S2_P1Room3Lock,
		"MazeRoomMatch3",		//eArea_L3_S2_P1Room4Match3,
		"MazeRoomPipes",		//eArea_L3_S2_P1Room5Pipe,
		"MazeRoomPillars",		//eArea_L3_S2_P1Room6Pillar,
		"DeadEnd",				//eArea_L3_S2_P1DeadEnd
		"Pool",					//eArea_L3_S3_Pool,
		"Block_Puzzle",          //eArea_L3_S3_PZBlocks
		"Underwater_Puzzle",	//eArea_L3_S3_PZUnderwater,
		"Fort_Fountain",		//eArea_L3_S4_FortFountain,
		"Fort_Fountain_HO",		//eArea_L3_S4_FortFountainHO,
		"Cliff_Side",			//eArea_L3_S5_Cliff,

		"Swamp_Entrance",		//eArea_L4_S1_SwampEntrance,
		"Shoe_HO",				//eArea_L4_S1_ShoeHO
		"River_Side",			//eArea_L4_S2_RiverSide,
		"Alligator_Head_Puzzle",//eArea_L4_S2_PZAlligatorHead
		"Door_Puzzle",			//eArea_L4_S2_PZDoor
		"Level_4_Cave",			//eArea_L4_S3_Cave,	
		"Fire_Making_Puzzle",	//eArea_L4_S3_PZFireMaking
		"Cave_Door_Puzzle",		//eArea_L4_S3_PZDoor
		"Hill_Top",				//eArea_L4_S4_HillTop,
		"Binocular_HO",			//eArea_L4_S4_BinocularHO
		"Hill_Side",			//eArea_L4_S5_HillSide		
		"Swap_Tile_Puzzle",		//eArea_L4_S5_PZSwapTile

		"Forest_Entrance",		//eArea_L5_S1_TigerDistant,
		"Forest_Entrance",		//eArea_L5_S2_TigerCloseup,
		"Blocks_Puzzle",		//eArea_L5_S2_PZBlocks,
		"Pond",		//eArea_L5_S3_Pond,
		"DungeonDoor_Puzzle",		//eArea_L5_S3_PZDungeonDoor,
		"Underwater_HO",		//eArea_L5_S3_HOUnderwater,
		"Dungeon",		//eArea_L5_S4_Dungeon,
		"Handcuff_Puzzle",		//eArea_L5_S4_PZHandCuff,
		"Under_the_Mountain",		//eArea_L5_S5_MountainBelow,
		"Ganapathi_HO",		//eArea_L5_S5_HOGanapathi,
		"Sun_Clock_Puzzle",	//eArea_L5_S5_PZSunClock,
		"The_Guardian",		//eArea_L5_S6_MountainTop,	
		"Tree_HO",		//eArea_L5_S6_HOTree,

		"Statue",// eArea_L6_S1_FloatingIsland1:
		"Description_HO",//
		"Nether_World",// 	eArea_L6_S2_FloatingIsland2:
		"ShereHO",//eArea_L6_S2_ShereHO
		"Pine_Gum_Puzzle",	//eArea_L6_S2_PZPineGum
		"Secret_Cave",// 	eArea_L6_S3_FloatingIsland3:
		"Fake_James",	//eArea_L6_S3_FakeJames
		"Glowing_Heart_HO",	//eArea_L6_S3_GlowingHeartHO
		"Pathway_Puzzle",		//eArea_L6_S3_PZPathway
		"Portal",// 	eArea_L6_S4_FloatingIsland4:
		"Portal_chest",//eArea_L6_S4_PortalChest
		"Portal_Puzzle", //eArea_L6_S4_PortalPuzzle
		"MarysDream",// 	eArea_L6_S5_MarysDream:
		"Tab_Minigame",//    eArea_L6_S5_TabMiniGame

		"Volcano",		//eArea_L7_S1_Volcano
		"PZJigsawYellow",		//eArea_L7_S1_PZJigsawYellow
		"PZStarPuzzle",		//eArea_L7_S1_PZStarPuzzle
		"LasyaPrison",		//eArea_L7_S2_LasyaPrison
		"HORightHand",		//eArea_L7_S2_HORightHand
		"HOLeftHand",		//eArea_L7_S2_HOLeftHand
		"HOBrokenRightHand",		//eArea_L7_S2_HOBrokenRightHand
		"HOBrokenLeftHand",		//eArea_L7_S2_HOBrokenLeftHand
		"PZEyePuzzle",		//eArea_L7_S2_PZEyePuzzle
		"PZLasyaStatue",		//eArea_L7_S2_PZLasyaStatue
		"Haven",		//eArea_L7_S3_Haven
		"PZDoorPuzzle",		//eArea_L7_S3_PZDoorPuzzle
		"PZJigsawRed",		//eArea_L7_S3_PZJigsawRed
		"Ameeha",		//eArea_L7_S4_Ameeha
		"PZJigsawBlue",		//eArea_L7_S4_PZJigsawBlue
		"SophiaEnd",		//eArea_L7_S5_SophiaEnd

		"Shukras_Place",	//eArea_L8_S1_ShukrasPlace,
		"Hand_Door_Puzzle",	//eArea_L8_S1_PZ1HandDoorPuzzle,
		"Door_HO",			//eArea_L8_S1_DoorHO
		"Garden",			//eArea_L8_S2_Garden
		"Gears_Puzzle",		//eArea_L8_S2_PZGears
		"Gate_Puzzle",		//eArea_L8_S2_PZGate
		"Sugar_Cane_Puzzle",//eArea_L8_S2_PZSugarCane
		"Banyan_Tree",		//eArea_L8_S3_BanyanTree
		"Banyan_Tree_HO",   //eArea_L8_S3_BanyanTreeHO
		"Pool",				//eArea_L8_S4_Pool
		"Pool_Puzzle",		//eArea_L8_S4_PZPoolGame
		"Volcano_Land",		//eArea_L8_S5_VolcanoLand
		"Virtual_MiniGame"
	};

	_sceneStartIndex = _sceneEndIndex = 0;

	_invStartIndex =0;
	_invEndIndex = 0;
	switch (ProfileSystem::Instance->ActiveProfile.CurrentLevel)
	{
	case Level_1:	_invStartIndex = eInv_L1S1_Bag; _invEndIndex = eInv_L1S4_BookCode;
		_sceneStartIndex = eArea_L1_S1_Beach;  _sceneEndIndex = eArea_L1_EndOfDemo;
		break;
	case Level_2:	_invStartIndex = eInv_L2S1_Aloevera; _invEndIndex = eInv_L2S6_PaperPuzClue;
		_sceneStartIndex = eArea_L2_S1_ForestEntrance;  _sceneEndIndex = eArea_L2_S7_PZSoldiers;
		break;
	case Level_3:
		_invStartIndex = eInv_L3S1_Charcoal; _invEndIndex = eInv_L3S5_Rope;
		_sceneStartIndex = eArea_L3_S1_FortEntrance;  _sceneEndIndex = eArea_L3_S5_Cliff;
		break;
	case Level_4:	_invStartIndex = eInv_L4_S1_Knife; _invEndIndex = eInv_L4_CB_BambooWithKnife;
		_sceneStartIndex = eArea_L4_S1_SwampEntrance;  _sceneEndIndex = eArea_L4_S5_PZSwapTile;
		break;
	case Level_5:	_invStartIndex = eInv_L5_S1_ArrowHolder; _invEndIndex = eInv_L5_S5_StoneEmblem;
		_sceneStartIndex = eArea_L5_S1_TigerDistant;  _sceneEndIndex = eArea_L5_S6_HOTree;
		break;
	case Level_6:	_invStartIndex = eInv_L6_S1_Mortar; _invEndIndex = eInv_L6_CB_RopeWithMagnetisedHorseShoe;
		_sceneStartIndex = eArea_L6_S1_FloatingIsland1;  _sceneEndIndex = eArea_L6_S5_TabMiniGame;
		break;
	case Level_7:	_invStartIndex = eInv_L7_S1_BlueStarEmblem; _invEndIndex = eInv_L7_S4_WellGear;
		_sceneStartIndex = eArea_L7_S1_Volcano;  _sceneEndIndex = eArea_L7_S5_SophiaEnd;
		break;
	case Level_8:_invStartIndex = eInv_L8_S1_MagicGlass; _invEndIndex = eInv_L8_CB_MagicWandWithoutKey;
		_sceneStartIndex = eArea_L8_S1_ShukrasPlace;  _sceneEndIndex = eArea_L8_S5_VirtualMiniGame;
		break;
	case Level_Max:
		break;
	default:
		break;
	}

	numScenes = 0;

	for (int i = 0; i < MAX_SCENE_PER_LEVEL; i++)
	{
		_SceneNames[i] = NULL;
	}

	//Scene Texts
	float initPosX = 120, initPosY = 20, textGap=28;
	int startIndex=_sceneStartIndex;
	for (int i = _sceneStartIndex; i <= _sceneEndIndex; i++)
	{
		ColorRGB blue = {(93/255.0f),(226/255.0f),(255/255.0f)};
		_SceneNames[numScenes] = new CBitmapText();
		_SceneNames[numScenes]->SetTextProps(StringTable::getStr(SceneNames[i]), initPosX, initPosY+(textGap*(i-startIndex)), eZOrder_CutsceneFull+15, blue, eFont_Italic24);
		_SceneNames[numScenes]->Show(false);

		numScenes += 1;

		if(numScenes>MAX_SCENE_PER_LEVEL)
		{
			#ifdef K_WIN32
				FATAL("Er0x2901- TAB IN GAME CHEATS : Scene Counts on this level exceeds MAX SCENE PER LEVEL COUNT"); ////Removed Wide string conversion for Windows	
			#else
				FATAL((const char*)"Er0x2901- TAB IN GAME CHEATS : Scene Counts on this level exceeds MAX SCENE PER LEVEL COUNT"); 
			#endif
			return;
		}

		if((i - _sceneStartIndex + 1)%12 == 0)
		{
			initPosX += 240;
			startIndex = i;
		}
	}

	//Inv Item Texts
	initPosX = 720; initPosY = 20;
	startIndex = _invStartIndex;

	numInvItems = 0;

	for (int i = 0; i < MAX_INVITEMS_PER_LEVEL; i++)
	{
		_InvNamesTexts[i] = NULL;
	}

	for (int i = _invStartIndex; i <= _invEndIndex; i++)
	{
		ColorRGB green = {(120/255.0f),(240/255.0f),(120/255.0f)};
		_InvNamesTexts[numInvItems] = new CBitmapText();
		_InvNamesTexts[numInvItems]->SetTextProps(StringTable::getStr(InventoryDescrips[i]), initPosX, initPosY+(textGap*(i-startIndex)), eZOrder_CutsceneFull+15, green, eFont_Italic24);
		_InvNamesTexts[numInvItems]->Show(false);

		numInvItems += 1;

		if(numInvItems>MAX_INVITEMS_PER_LEVEL)
		{
			#ifdef K_WIN32
				FATAL("Er0x2902- TAB IN GAME CHEATS : Inv Items on this level exceeds MAX INV ITEMS COUNT"); ////Removed Wide string conversion for Windows	
			#else
				FATAL((const char*)"Er0x2902- TAB IN GAME CHEATS : Inv Items on this level exceeds MAX INV ITEMS COUNT"); 
			#endif

			return;
		}

		if((i - _invStartIndex + 1)%14 == 0)
		{
			initPosX += 240;
			startIndex = i;
		}
	}
#endif // _TABINGAMEUI

#ifdef _PARTICLEEDITORENABLED
	_isParticleEditorActive = false;
	_particleEditor = NULL;
#endif
}

HiddenObjectSystem::~HiddenObjectSystem()
{
	//Hud is auto-collected when user moves from inherited room to next room
#ifdef _TABINGAMEUI
	SAFE_DELETE(TabMenuMaskBg);
	for (int i = 0; i < MAX_SCENE_PER_LEVEL; i++){
		SAFE_DELETE(_SceneNames[i]);
	}

	for (int i = 0; i < MAX_INVITEMS_PER_LEVEL; i++){
		SAFE_DELETE(_InvNamesTexts[i]);
	}

	for (int i = 0; i < eAwardEventMax; i++)
	{
		SAFE_DELETE(_AwardEventNames[i]);
	}
#endif // _TABINGAMEUI

	SAFE_DELETE(SparkleFX);

	//Garbage collection in gameflow
	SAFE_DELETE(HOScene);
}

void HiddenObjectSystem::HOUpdate()
{
	sparkleTimer += Control::LogicRate;

	if (sparkleTimer >= sparkleThreshold)
	{
		sparklePointsVec.clear();
		SparkleUpdate();
		sparkleTimer = 0;
	}
}

void HiddenObjectSystem::SparkleUpdate()
{
	if (!SparkleFX)
		SparkleFX = new SuperFX("PRT\\glint.INI",PointSystem::CreateCPoint(0,0,0),120);	

	int taskCount = (int)sparklePointsVec.size();
	int randomSpark = rand() % taskCount;
	SparkleFX->SetXY(sparklePointsVec[randomSpark].x, sparklePointsVec[randomSpark].y);
	SparkleFX->Start();
}

void HiddenObjectSystem::SceneObjectsUpdate()
{
	//Implements in derived function
}

void HiddenObjectSystem::ProcessMessage(sMessage message)
{
	//Process Hud Message
	if( Hud != NULL )
	{
		if( message.CallerID == Hud->ID )
		{
			if( message.Value == HOSM_ReturnToMenu )
			{
				SendMessage(ControlExit, ID, HOSM_ReturnToMenu);
				return;
			}
			else if( message.Value == HOSM_Skip )
			{		
				if (ProfileSystem::Instance->_isExtrasGamePlay)
				{
					return;
				}

				ProcessSkip();
				return;
			}
			else if( message.Value == HOSM_Hint )
			{				
#if defined (K_ANDROID) && !defined(UNLOCKED)
				if (GFApp->noOfHints > 0)
				{
					GFApp->noOfHints--;
					GFApp->UpdateUserHint(to_string(GFApp->noOfHints));
					ProfileSystem::Instance->WriteProfileSystemData();
					ProcessNonHOHint();
				}
				else
				{
					GFHud->showADorBuyDialog(true, 0);
				}
#else
				ProcessNonHOHint();
#endif
			}
			else if( message.Value == HOSM_Map )
			{

				SendMessage(ControlExit, ID, HOSM_Map);
				return;
			}
		}
	}

	ReceiveMessage(message.Value);
}

CRectangle LeftHudRect(168,620,141,148);
CRectangle RightHudRect(1011,602,183,166);	
CRectangle InvBoxMaxRect(342,636,666,132);  
CRectangle InvBoxMinRect(342,710,666,768-710);  

bool HiddenObjectSystem::IsMouseOverGoBackArea(const CPoint* pPos)
{
	//ARE WE INSIDE HOVER AREA 
	if( HoveredBackBtnRectMax.Intersection(pPos) )
	{
		//Never get into LEFTHUD(Menu/Diary/Map) or RIGHTHUD(Hint/Guide) Area 
		if( LeftHudRect.Intersection(pPos) || RightHudRect.Intersection(pPos) )
			return false;

		//HANDLE COLLISION WITH MIDDLE HUD!!
		//If InvBox present - avoid collision with that too...
		//bool isHOPuzzleScene = GFInstance->IsHOPuzzleScene(ProfileSystem::Instance->ActiveProfile.CurrentArea);//Later: don't need IsHOPuzzleScene()
		//if( !(isHOPuzzleScene ||  Hud->InvBoxCtrl->InvBoxFrame == PUZBOX_FRAME || Hud->HOState == HOActive) ) 
		if( !(Hud->InvBoxCtrl->InvBoxFrame == PUZBOX_FRAME || Hud->HOState == HOActive) )
		{
			//UP
			CPoint mousePos = Control::Input->Mpos();
			if( ProfileSystem::Instance->ActiveProfile.IsInvLocked )
			{
				if( InvBoxMaxRect.Intersection(pPos) )
					return false;
			}
			//DOWN
			else if( InvBoxMinRect.Intersection(pPos) )
				return false;
			//MIDDLE - Handle inbetween case too!!
				//else if( Hud->InvBoxCtrl->InvBox->Collide(&Control::Input->Mpos()) )
					// Linux: gives error of taking address of temporary also potential crash factor
					// below is fix
					
			else if( Hud->InvBoxCtrl->InvBox->Collide(&mousePos) )
				return false;
		}

		//TODO: Hack code. Need to recheck
		if(Hud->IsTakeAndUseHOScene)
		{
			if( InvBoxMaxRect.Intersection(pPos) )
				return false;
		}

		if (ProfileSystem::Instance->_isExtrasGamePlay)
		{
			#ifdef ENABLE_ANDROID_BACK_BUTTON
				if (Control::Input->isKeyBackSensed()) {
					Hud->BackToMainMenu();				
				}			
            #endif

#ifndef TOUCH_UI
			Cursor::SetMode(CA_ExitDown);
			if(Control::Input->LBclicked())
			{
				Hud->BackToMainMenu();
			}
#endif
			return false;	
		}

		if(ProfileSystem::Instance->ActiveProfile.IsCrystalOrbPicked)
		{
			if(Hud->crystalOrbPoly->Intersection(pPos))
			{
				return false;
			}
		}

		//NO HUD COLLISION - WE ARE IN BACKBTNAREA!!
		return true;

	}
	else //NOT IN HOVERBACKBTN AREA!!
		return false;
}


void HiddenObjectSystem::LoadHud(int InventorySize)
{	
	Hud = new HudControl(InventorySize);//baseclass Control
	AddControl(Hud);
}


void HiddenObjectSystem::LoadScene(const std::string scene)
{
	if( HOScene )
	{
		return; //Log
	}

	HOScene = new CHOScene();
	HOScene->Load(scene);

    extrasGoBackBtn = new CObject();
    extrasGoBackBtn->LoadInit("MNU\\EXTRAS\\BACKBUTTON.SPR", 1266, 30, eZOrder_Popup + 300);
    extrasGoBackBtn->SetUID("extrasGoBackBtn");
    HOScene->PushObject(extrasGoBackBtn);

    extrasGoBackBtnText = new CObject();
    extrasGoBackBtnText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\BACK.SPR", 1250, 27, eZOrder_Popup + 301);
    extrasGoBackBtnText->SetUID("extrasGoBackBtnText");
    HOScene->PushObject(extrasGoBackBtnText);
#ifndef TOUCH_UI
    extrasGoBackBtn->SetEnable(false);
    extrasGoBackBtnText->SetEnable(false);
#endif

    if (!ProfileSystem::Instance->_isExtrasGamePlay)
    {
        extrasGoBackBtn->SetEnable(false);
        extrasGoBackBtnText->SetEnable(false);
    }

}

bool HiddenObjectSystem::_handleExtrasGameComplete(bool isPuzzle)
{
	if (ProfileSystem::Instance->_isExtrasGamePlay)
	{
		_extrasCompleteTimer = 2.0f;

		if (isPuzzle)
			Hud->ShowBannerText("Expuzzle");
		else
			Hud->ShowBannerText("Exho");

		return true;
	}

	return false;
}

void HiddenObjectSystem::Required()
{
#ifdef TOUCH_UI
    if (ProfileSystem::Instance->_isExtrasGamePlay)
    {
       if(extrasGoBackBtn->IsIntersect(Control::Input->Mpos()))
       {
           if(Control::Input->LBclicked())
           {
               Hud->BackToMainMenu();
           }
       }
    }
#endif
#ifdef _TABINGAMEUI

#ifdef _DEBUG
	if( Control::Input->KpressedWithShift(K_VK_S) )
	{
		SLOG::Show(!SLOG::Visible);
	}

	if(Control::Input->KpressedWithShift(K_VK_D))
	{
		SLOG::DumpTextsToKLOG();
		return;
	}
#endif // _DEBUG


	if(_TabInGameMenuShowing)
	{
		Cursor::SetMode(CA_Normal);

		if(_TabInGameAwardEventShowing)
		{
			for (int i = 0; i < eAwardEventMax; i++)
			{
				if(_AwardEventNames[i]->Collide(&Control::Input->Mpos()))
				{
					Cursor::SetMode(CA_HandPointy);

					if(Control::Input->LBclicked())
					{
						//TODO fill the remaining functions
						switch (i)
						{
						case eStartMainStory:
							AwardManager::GetInstance()->StartMainStory();
							break;
						case eCompleteMainStory:
							AwardManager::GetInstance()->CompleteMainStory();
							break;
						case eCompleteBonusStory:
							AwardManager::GetInstance()->CompleteBonusStory();
							break;
						case eStartHO:
							AwardManager::GetInstance()->StartHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
							break;
						case eCompleteHO:
							AwardManager::GetInstance()->CompleteHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
							break;
						case eQuitHO:
							break;
						case eStartMiniGame:
							AwardManager::GetInstance()->StartMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
							break;
						case eCompleteMiniGame:
							AwardManager::GetInstance()->CompleteMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
							break;
						case eQuitMiniGame:
							break;
						case eUseHint:
							AwardManager::GetInstance()->UseHint();
							break;
						case eSkipMiniGame:
							AwardManager::GetInstance()->SkipMiniGame(ProfileSystem::Instance->ActiveProfile.CurrentArea);
							break;
						case eSkipDialogueOrCutScene:
							AwardManager::GetInstance()->SkipDialogueOrCutScene();
							break;
						case eCompleteCreeksMansion:
							AwardManager::GetInstance()->CompleteCreeksMansion();
							break;
						case eUseFastTravell:
							AwardManager::GetInstance()->CollectAward(kAward_FastTravel);
							break;
						case eStartJourneyToNarara:
							AwardManager::GetInstance()->CollectAward(kAward_Start_Journey_Narara);
							break;
						case eGotOutOftreasureRoom:
							AwardManager::GetInstance()->CollectAward(kAward_TreasureRoom_Escape);
							break;
						case eCollectInventoryItem:
							AwardManager::GetInstance()->CollectInventoryItem();
							break;
						case eCollectMemory:
							AwardManager::GetInstance()->CollectMemory();
							break;
						case eCollectMorphing:
							AwardManager::GetInstance()->CollectMorphing();
							break;
						case eSkipTutorial:
							AwardManager::GetInstance()->SkipTutorial();
							break;
						case ePick5ItemsWithinTime:
							AwardManager::GetInstance()->Pick5ItemsWithinTime();
							break;
						case eCompleteHOWithMatch3:
							AwardManager::GetInstance()->CompleteHOWithMatch3(ProfileSystem::Instance->ActiveProfile.CurrentArea);
							break;
						case eCollectAward:
							//AwardManager::GetInstance()->CollectAward();
							break;
						}
					}
					break;
				}
			}
		}
		else
		{
			for (int i = 0; i < numScenes; i++)
			{
				if(_SceneNames[i]->Collide(&Control::Input->Mpos()))
				{
					Cursor::SetMode(CA_HandPointy);

					if(Control::Input->LBclicked())
					{
						ProfileSystem::Instance->ActiveProfile.CurrentArea = _sceneStartIndex+i;
						SendMessage(ControlExit, ID, HOSM_NewArea);
						return;
					}
					break;
				}
			}

			for (int i = 0; i < numInvItems; i++)
			{
				if(_InvNamesTexts[i]->Collide(&Control::Input->Mpos()))
				{
					Cursor::SetMode(CA_HandPointy);

					if(Control::Input->LBclicked())
					{
						std::vector<int> vecInvIds;

						updateProfileVarForMultiInventory(_invStartIndex+i); //For multiinventory Items

						Hud->InvBoxCtrl->Activate(1);
						ProfileSystem::Instance->ActiveProfile.Inventory[_invStartIndex+i] = true;
						vecInvIds.push_back(_invStartIndex+i);
						Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
						return;
					}
					break;
				}
			} 
		}

		if( Control::Input->Kpressed(K_VK_TAB) )
		{
			_TabInGameMenuShowing = false;
			TabMenuMaskBg->Show(false);
			if(_TabInGameAwardEventShowing)
			{
				for (int i = 0; i < eAwardEventMax; i++)
				{
					_AwardEventNames[i]->Show(false);
				}
			}
			else
			{
				for (int i = 0; i < numScenes; i++)
					_SceneNames[i]->Show(false);
				for (int i = 0; i < numInvItems; i++)
					_InvNamesTexts[i]->Show(false); 
			}
		}
	}
	else
	{
		if(Control::Input->Kpressed(K_VK_R))
		{
			//show next sparkle cheat
			sparkleTimer = sparkleThreshold - 0.1f;
		}

		if( Control::Input->Kpressed(K_VK_TAB) )
		{
			TabMenuMaskBg->Show(true);
			_TabInGameMenuShowing = true;
			_TabInGameAwardEventShowing = false;
			if( Control::Input->Kdown(K_VK_L_SHIFT) )
				_TabInGameAwardEventShowing = true;
			if(_TabInGameAwardEventShowing)
			{
				for (int i = 0; i < eAwardEventMax; i++)
				{
					_AwardEventNames[i]->Show(true);
				}
			}
			else
			{
				for (int i = 0; i < numScenes; i++)
					_SceneNames[i]->Show(true);
				for (int i = 0; i < numInvItems; i++)
					_InvNamesTexts[i]->Show(true);
			}
		}
	}
#endif // _TABINGAMEUI

#ifdef _PARTICLEEDITORENABLED
	if(_isParticleEditorActive)
	{
		if( Control::Input->KpressedWithCtrl(K_VK_P) )
		{
			_isParticleEditorActive = false;
			//disable and show UI
			_particleEditor->Show(false);
		}
		else if(_particleEditor!=NULL && _particleEditor->IsDisabled)
		{
			_isParticleEditorActive = false;
		}
	}
	else
	{
		if( Control::Input->KpressedWithCtrl(K_VK_P) )
		{
			_isParticleEditorActive = true;
			if(_particleEditor==NULL)
			{
				_particleEditor = new ParticleEditorControl();
				AddControl(_particleEditor);
			}
			//enable and show UI
			_particleEditor->Show(true);
		}
	}
#endif

	if (_extrasCompleteTimer > 0)
	{
		_extrasCompleteTimer -= Control::LogicRate;
		if (_extrasCompleteTimer <= 0)
		{
			Hud->BackToMainMenu();
		}
	}
}

#ifdef _TABINGAMEUI
void  HiddenObjectSystem::updateProfileVarForMultiInventory(int eInvId)
{	
	switch( eInvId )
	{
	case eInv_L1S1_RubyPart1:
		ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby1Branch2Taken = true;
		break;
	case eInv_L1S1_RubyPart2:
		ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby2Branch3Taken = true;
		break;
	case eInv_L1S1_RubyPart3:
		ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby3UnderLeaf1Taken = true;
		break;
	case eInv_L1S1_RubyPart4:
		ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby4UnderLeavesTaken = true;
		break;

	case eInv_L1S2_Plank1:
		ProfileSystem::Instance->ActiveProfile.L1S2_IsInvPlank1Taken = true;
		break;

	case eInv_L1S7_Plank2:
		ProfileSystem::Instance->ActiveProfile.L1S7_IsInvPlank2Taken = true;
		break;

	case eInv_L1S1_Plank3:
		ProfileSystem::Instance->ActiveProfile.L1S1_IsInvPlank3Taken = true;
		break;

	case eInv_L1S2_Cloth1Swing:
		ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth1SwingTaken = true;
		break;
	case eInv_L1S2_Cloth2LovePop:
		ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth2LovePopTaken = true;
		break;
	case eInv_L1S2_Cloth3CarDicky:
		ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth3CarDickyTaken = true;
		break;

	case eInv_L1S3_ShadowHand1:
		ProfileSystem::Instance->ActiveProfile.L1S3_IsInvShadowHand1Taken = true;
		break;
	case eInv_L1S4_ShadowHand2:
		ProfileSystem::Instance->ActiveProfile.L1S4_IsInvShadowHand2Taken = true;
		break;

	case eInv_L2S2_Pearl1:
		ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl1Taken = true;
		break;
	case eInv_L2S2_Pearl3:
		ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl3Taken = true;
		break;
	case eInv_L2S2_Pearl4:
		ProfileSystem::Instance->ActiveProfile.L2_S2_NagaPearl4Taken = true;
		break;

	case eInv_L2S6_SmallStone1:
		ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone1Taken = true;
		break;
	case eInv_L2S6_SmallStone2:
		ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone2Taken = true;
		break;
	case eInv_L2S6_SmallStone3:
		ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone3Taken = true;
		break;

	case eInv_L3S2_PuzzleBlock1Left:
		ProfileSystem::Instance->ActiveProfile.L3_S2_M6_CollectedBoxIndex[0] = 0;
		break;

	case eInv_L3S2_PuzzleBlock2Right:
		ProfileSystem::Instance->ActiveProfile.L3_S2_M6_CollectedBoxIndex[1] = 1;
		break;

	case eInv_L3S1_LadderPiece1:
		ProfileSystem::Instance->ActiveProfile.L3_S1_InvLadderStep1Taken = true;
		break;

	case eInv_L3S3_LadderPiece2:
		ProfileSystem::Instance->ActiveProfile.L3_S3_InvLadderStep2Taken = true;
		break;

	case eInv_L3S2_PipePiece1Room1:
		ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe1R1Taken = true;
		break;

	case eInv_L3S2_PipePiece2Room2:
		ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe2R2Taken = true;
		break;

	case eInv_L3S2_PipePiece3Room3:
		ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe3R3Taken = true;
		break;

	case eInv_L3S2_PipePiece4Room4:
		ProfileSystem::Instance->ActiveProfile.L3_S2_InvPipe4R4Taken = true;
		break;

	case eInv_L4_S2_BambooStick1:
		ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick0Taken = true;
		break;

	case eInv_L4_S2_BambooStick2:
		ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick1Taken = true;
		break;

	case eInv_L4_S2_BambooStick3:
		ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick2Taken = true;
		break;
	case eInv_L4_S2_BambooStick4:
		ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick3Taken = true;
		break;

	case eInv_L4_S2_Twig1:
		ProfileSystem::Instance->ActiveProfile.L4_S2_InvTwig1Taken = true;
		break;

	case eInv_L4_S4_Twig2:
		ProfileSystem::Instance->ActiveProfile.L4_S4_InvTwig2Taken = true;
		break;

	case eInv_L4_S5_Twig3:
		ProfileSystem::Instance->ActiveProfile.L4_S5_InvTwig3Taken = true;
		break;

	default: 
		break;
	}
}
#endif //_TABINGAMEUI

void HiddenObjectSystem::InitHintTrailEmit(float x, float y, bool isInstantRecharge, eCursorAnim cursorId)
{
	
	Hud->InitHintTrailEmit(x, y, isInstantRecharge, cursorId);
}

void  HiddenObjectSystem::InitHintTrailEmit(CPoint *destPoint, bool isInstantRecharge, eCursorAnim cursorId)
{
	InitHintTrailEmit(destPoint->x, destPoint->y, isInstantRecharge, cursorId);
}

void HiddenObjectSystem::InitHintTrailEmit(const CRectangle *destRect, bool isInstantRecharge, eCursorAnim cursorId)
{
	InitHintTrailEmit(destRect->GetCenterX(), destRect->GetCenterY(), isInstantRecharge, cursorId);
}
