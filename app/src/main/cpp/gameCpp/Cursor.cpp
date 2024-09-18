//====================================
// Framework
// Cursor Class
//====================================

#include "Cursor.h"
#include "Object.h"
#include "GameFlow.h"
#include "BitmapText.h"
#include "Fonts.h"
#include "Hud.h"
#include "LangDefs.h"
#include "GameTextManager.h"

extern HudControl * GFHud;

//Room cursor descriptions
std::string SceneNames[eArea_Max+1] =
{ 
	//Level 1 - demo
	"Beach",				//eArea_L1_S1_Beach
	"Kid_Painting",			//eArea_L1_S1_PZKidsPainting
	"Mansion_Frontyard",	//eArea_L1_S2_Frontyard
	"Fuse_Puzzle",			//eArea_L1_S2_PZFuse

	"Foyer",				//eArea_L1_S3_Foyer
	"Shelf_Puzzle",			//eArea_L1_S3_PZShelf
	"ShadowPuzzle_Door",	//eArea_L1_S4_PZDoor
	"Bedroom",				//eArea_L1_S4_Bedroom
	"Painting_Puzzle",		//eArea_L1_S4_PZPainting
	"Metal_Wall_HO",		//eArea_L1_S3_DoorHO

	"Treasure_Room",		//eArea_L1_S5_TreasureRoom
	"Tunnel_Door",			//eArea_L1_S5_TunnelDoor
	"Dynamite_Puzzle",		//eArea_L1_S5_PZDynamiteMaking
	"TreasureRoom_HO",		//eArea_L1_S5_HOTreasureRoom
	"Boat_Shed",			//eArea_L1_S6_BoatShed
	"ToolBox_HO",			//eArea_L1_S6_HOBoatShed
	"Boat_Dock",			//eArea_L1_S7_BoatDock

	"End_Of_Demo",			//eArea_L1_EndOfDemo
	"Kids_Room",			//eArea_KidsRoom
	"Extras_Cutscene_Player",//eArea_ExtrasCutscenePlayer
	"Extras_Room",			//eArea_ExtrasRoom
	"Awards_Room",			//eArea_AwardsRoom

	// Level 2 - updated 28-10-16 -from Ryan
	"Narara_Entrance",		//eArea_L2_S1_ForestEntrance,
	"Underground",			//eArea_L2_S1_UndergroundHO,
	"Mango_Puzzle",			//eArea_L2_S2_PZMango,
	"Naga_Temple",          //eArea_L2_S2_NagaTemple,
	"HO_Naga_Temple",		//eArea_L2_S2_HONagaTemple
	"Cave_Entrance",		//eArea_L2_S3_CaveMouth
	"Stone_Beam_Puzzle",	//eArea_L2_S3_PZStoneBeam,
	"Cave_Intersection",	//eArea_L2_S4_Cave,
	"Deities_Puzzle",		//eArea_L2_S4_PZDeities
	"Moopans_Cave",			//eArea_L2_S5_MoopansDen,
	"Eagle_Unblock_Puzzle",	//eArea_L2_S5_EagleUnblock,
	"Medicine_Making_Puzzle",//eArea_L2_S5_PZ_Medicine_Making
	"The_Vault",			//eArea_L2_S6_TunnelEnd,
	"Waterfall_HO",			//eArea_L2_S6_HOWaterfall
	"Tunnel_Clock_Puzzle",	//eArea_L2_S6_PZTunnelClock,
	"Soldiers",				//eArea_L2_S7_PZSoldiers

	//Level 3 - updated 16-11-16 -from Ryan
	"Ruined_Castle",		//eArea_L3_S1_FortEntrance,
	"Ancient_Ruins",		//eArea_L3_S1_HOLadder,
	"Lion_Head_Puzzle",		//eArea_L3_S1_PZLionHead,
	"Castle_Interior",		//eArea_L3_S2_FortInterior,
	"Maze",					//eArea_L3_S2_P1Room1Start,
	"Maze",					//eArea_L3_S2_P1Room2Wine,
	"Maze",					//eArea_L3_S2_P1Room3Lock,
	"Maze",					//eArea_L3_S2_P1Room4Match3,
	"Maze",					//eArea_L3_S2_P1Room5Pipe,
	"Maze",					//eArea_L3_S2_P1Room6Pillar,
	"DeadEnd",				//eArea_L3_S2_P1DeadEnd
	"Pool",					//eArea_L3_S3_Pool,
	"Block_Puzzle",          //eArea_L3_S3_PZBlocks
	"Underwater_Puzzle",	//eArea_L3_S3_PZUnderwater,
	"Fountain",				//eArea_L3_S4_FortFountain,
	"Fort_Fountain_HO",		//eArea_L3_S4_FortFountainHO,
	"Cliff",				//eArea_L3_S5_Cliff

	//Level 4
	"Swamp",		//eArea_L4_S1_SwampEntrance,
	"Shoe_HO",				//eArea_L4_S1_ShoeHO,
	"River_Side",			//eArea_L4_S2_RiverSide,
	"Alligator_Head_Puzzle",//eArea_L4_S2_PZAlligatorHead,
	"Door_Puzzle",	//eArea_L4_S2_PZDoor,
	"Cave",					//eArea_L4_S3_Cave,	
	"Fire_Making_Puzzle",	//eArea_L4_S3_PZFireMaking
	"Cave_Door_Puzzle",	//eArea_L4_S3_PZDoor
	"Hill",				//eArea_L4_S4_HillTop,
	"Binocular_HO",				//eArea_L4_S4_BinocularHO
	"Hill_Side",				//eArea_L4_S5_HillSide		
	"Swap_Tile_Puzzle",	//			eArea_L4_S5_PZSwapTile

	"Forest_Entrance",		//eArea_L5_S1_TigerDistant,
	"Forest_Entrance",		//eArea_L5_S2_TigerCloseup,
	"Blocks_Puzzle",		//eArea_L5_S2_PZBlocks,
	"Pond",		//eArea_L5_S3_Pond,
	"DungeonDoor_Puzzle",		//eArea_L5_S3_PZDungeonDoor,
	"Underwater_HO",		//eArea_L5_S3_HOUnderwater,
	"Dungeon",		//eArea_L5_S4_Dungeon,
	"Handcuff_Puzzle",		//eArea_L5_S4_PZHandCuff,
	"Moon_Light_Cave",		//eArea_L5_S5_MountainBelow,
	"Ganapathi_HO",		//eArea_L5_S5_HOGanapathi,
	"Sun_Clock_Puzzle",	//eArea_L5_S5_PZSunClock,
	"The_Guardian",		//eArea_L5_S6_MountainTop,	
	"Tree_HO",		//eArea_L5_S6_HOTree,	

	"Statue",// eArea_L6_S1_FloatingIsland1:
	"Description_HO",
	"Nether_World",// 	eArea_L6_S2_FloatingIsland2:
	"SphereHO",//eArea_L6_S2_ShereHO
	"Pine_Gum_Puzzle",	//eArea_L6_S2_PZPineGum
	"Secret_Cave",// 	eArea_L6_S3_FloatingIsland3:
	"Fake_James",	//eArea_L6_S3_FakeJames
	"Glowing_Heart_HO",			//eArea_L6_S3_GlowingHeartHO
	"Pathway_Puzzle",					//eArea_L6_S3_PZPathway
	"Portal",// 	eArea_L6_S4_FloatingIsland4:
	"PortalChest",//eArea_L6_S4_PortalChest
	"Portal_Puzzle",	//"eArea_L6_S4_PortalPuzzle
	"MarysDream",// 	eArea_L6_S5_MarysDream:
	"Tab_MiniGame", //eArea_L6_S5_TabMiniGame

	"Volcano",		//eArea_L7_S1_Volcano
	"PZJigsawYellow",		//eArea_L7_S1_PZJigsawYellow
	"PZStarPuzzle",		//eArea_L7_S1_PZStarPuzzle
	"Lasyas_Prison",		//eArea_L7_S2_LasyaPrison
	"HORightHand",		//eArea_L7_S2_HORightHand
	"HOLeftHand",		//eArea_L7_S2_HOLeftHand
	"HOBrokenRightHand",		//eArea_L7_S2_HOBrokenRightHand
	"HOBrokenLeftHand",		//eArea_L7_S2_HOBrokenLeftHand
	"PZEyePuzzle",		//eArea_L7_S2_PZEyePuzzle
	"PZLasyaStatue",		//eArea_L7_S2_PZLasyaStatue
	"Temple",		//eArea_L7_S3_Haven
	"PZDoorPuzzle",		//eArea_L7_S3_PZDoorPuzzle
	"PZJigsawRed",		//eArea_L7_S3_PZJigsawRed
	"Ameeha",		//eArea_L7_S4_Ameeha
	"PZJigsawBlue",		//eArea_L7_S4_PZJigsawBlue
	"Abyss",		//eArea_L7_S5_SophiaEnd

	"Shukras_Palace",//	eArea_L8_S1_ShukrasPlace,
	"Hand_Door_Puzzle",//	eArea_L8_S1_PZ1HandDoorPuzzle,
	"Door_HO", //eArea_L8_S1_DoorHO
	"Garden",//eArea_L8_S2_Garden
	"Gears_Puzzle",		//eArea_L8_S2_PZGears
	"Gate_Puzzle", //eArea_L8_S2_PZGate
	"Sugar_Cane_Puzzle", //eArea_L8_S2_PZSugarCane
	"Tree_of_Life",//eArea_L8_S3_BanyanTree
	"Banyan_Tree_HO", //eArea_L8_S3_BanyanTreeHO
	"Pool",//eArea_L8_S4_Pool
	"Pool_Puzzle", //eArea_L8_S4_PZPoolGame
	"Mohenjedaro",//eArea_L8_S5_VolcanoLand
	"Virtual_Minigame"//eArea_L8_S5_VirtualMiniGame
};

#ifdef TOUCH_UI
	 // the Cursor Icon should appear little above the place where the user places their finger, only then the cursor is visible.
	#define CURSOR_TOP_DISPLACEMENT 20.0f 
	#define CURSOR_LOOK_TOP_DISPLACEMENT 30.0f
	#define CURSOR_HANDPOINTY_TOP_DISPLACEMENT 30.0f
	#define CURSOR_GEAR_TOP_DISPLACEMENT 30.0f
	#define CURSOR_HELP_TOP_DISPLACEMENT 40.0f
	#define CURSOR_SPEECH_TOP_DISPLACEMENT 40.0f
	#define CURSOR_HANDTAKE_TOP_DISPLACEMENT 30.0f
#else
	#define CURSOR_TOP_DISPLACEMENT 0.0f
	#define CURSOR_LOOK_TOP_DISPLACEMENT 0.0f
	#define CURSOR_HANDPOINTY_TOP_DISPLACEMENT 0.0f
	#define CURSOR_GEAR_TOP_DISPLACEMENT 0.0f
	#define CURSOR_HELP_TOP_DISPLACEMENT 0.0f
	#define CURSOR_SPEECH_TOP_DISPLACEMENT 0.0f
	#define CURSOR_HANDTAKE_TOP_DISPLACEMENT 0.0f
#endif
CObject*		Cursor::Sprite = NULL;
CObject*		Cursor::CurObjs[8] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
int				Cursor::currentMode = 0;
bool			Cursor::SystemCursor = false;
bool			Cursor::inMenu = false; // only used in Android as of now
bool			Cursor::Visible = false;
CObject*		Cursor::rDescTip = NULL;
CBitmapText*	Cursor::rDescText = NULL;

Cursor::Cursor()
{
}

Cursor::~Cursor()
{
}

void Cursor::Create()
{
	if( Sprite != NULL )
		return;
	
		Sprite = new CObject();
		Sprite->LoadSprite("UI\\CUR.SPR");	
		Sprite->pSprite->Image->setTextureQuality(true);
		Sprite->Show(false);
		Sprite->SetZ(32000);
		Sprite->SetEnable(false);

		const std::string CursorPaths[8] =
		{
			"UI\\CUR\\CUR_NOR.SPR",
			"UI\\CUR\\CUR_LENS.SPR",
			"UI\\CUR\\CUR_HNDPTY.SPR",
			"UI\\CUR\\CUR_NAV.SPR",
			"UI\\CUR\\CUR_GEAR.SPR",
			"UI\\CUR\\CUR_HLP.SPR",
			"UI\\CUR\\CUR_TALK.SPR",
			"UI\\CUR\\CUR_HNDTAKE.SPR"
		};

		for (int i = 0; i < 8; i++)
		{
			CurObjs[i] = new CObject();
			CurObjs[i]->LoadSprite(CursorPaths[i]);
			CurObjs[i]->pSprite->Image->setTextureQuality(true);
			CurObjs[i]->Show(false);
			CurObjs[i]->SetZ(31000);
			
			if(i==7 || i==2)
				CurObjs[i]->SetScale(0.85f);
		}

	rDescTip = new CObject();
	rDescTip->LoadSprite("UI\\HUD\\RDESC.SPR");	
	rDescTip->Show(false);
	rDescTip->SetZ(31998);


	rDescText = new CBitmapText();	
	rDescText->SetAlignment(Align_Center);
	rDescText->LoadFont(eFont_25);
	#ifdef JAPANESE_BUILD
		rDescText->SetScale(0.94f);//avoid overlapping text
	#else
		rDescText->SetScale(0.85f);
	#endif
	rDescText->SetText("Tool Tip");

	rDescText->SetColor((233/255.0f),(204/255.0f),(131/255.0f));//gold
	rDescText->Show(false);	
	rDescText->SetZ(31999);
	SystemCursor = false;
	Visible = false;
	inMenu = false; // only used in Android, in every other platforms , this is not used so it is ok if it is false to initiate
}

void Cursor::Destroy()
{
	delete Sprite;
	delete rDescTip;
	delete rDescText;

	for (int i = 0; i < 8; i++)
	{
		delete CurObjs[i];
	}
}

void Cursor::Show(bool f)
{
	Visible = f;

	if( f )
	{
		if( GFHud )
			GFHud->EnableInput(true);
	}
	else
	{
		Control::Is_iPadCursorOverVideo = false;//forced turn off
		if( GFHud )
			GFHud->EnableInput(false);
	}

	if( SystemCursor == false )
	{
		if( Sprite == NULL )
			return;
		#ifndef TOUCH_UI
			Sprite->Show(f);
			if(CurObjs[currentMode]!=NULL)
				CurObjs[currentMode]->Show(f);
			else
				{
					#ifdef K_WIN32
						FATAL((const char*)L"Er0x3201- Cursor Load Error");
					#else
						FATAL((const char*)"Er0x3201- Cursor Load Error");
					#endif
				}
		#else
			switch( Sprite->GetAnimation() )
				{
					case CA_Normal    :
						Sprite->Show(false);
						if(CurObjs[currentMode]!=NULL)
							CurObjs[currentMode]->Show(false);
						else
							FATAL((const char*)"Er0x3201- Cursor Load Error");
						break;
					case CA_HandPointy:
					case CA_Look	  :			
					case CA_HandTake  : 
					case CA_Help	  :
					case CA_Gear	  :
					case CA_Speech	  :				
					case CA_ExitLeft  :					
					case CA_ExitRight :
					case CA_ExitUp    :
					case CA_ExitDown  :
						if (inMenu)
						{
							Sprite->Show(false);				
							if(CurObjs[currentMode]!=NULL)
								CurObjs[currentMode]->Show(false);
							else
								FATAL((const char*)"Er0x3201- Cursor Load Error");
						}
						else
						{
							if(!GFInstance->IsInGameMenuPopup)
							{
								Sprite->Show(f);				
								if(CurObjs[currentMode]!=NULL)
									CurObjs[currentMode]->Show(f);
								else
									FATAL((const char*)"Er0x3201- Cursor Load Error");
							}
						}
						break;
				}
		#endif
		for (int i = 0; i < 8; i++)
		{
			CurObjs[i]->SetEnable(f);
		}

		switch( Sprite->GetAnimation() )
		{
		default           :if( f ) break;
		case CA_Normal    :
		case CA_HandPointy:
		case CA_Look	  :rDescTip->Show(false);
			rDescText->Show(false);
			break;
		}
	}
	else
	{		
			if( f )		
				KInput::showPointer();						
			else
				KInput::hidePointer();
	}
}

void Cursor::SetXY( float x, float y )
{
	Sprite->SetXY(x,y-CURSOR_TOP_DISPLACEMENT);
	CurObjs[currentMode]->SetXY(x,y-CURSOR_TOP_DISPLACEMENT);
}

float Cursor::GetX()
{
	return Sprite->Pos.x;
}

float Cursor::GetY()
{
	return Sprite->Pos.y;
}

void Cursor::SetPos(const CPoint* pos)
{
	if( Sprite == NULL )
		return;

 long yDisplacement = 0; // value to displace the cursor in top
	if( Visible )
	{
		if( SystemCursor == false )
		{
			#ifndef TOUCH_UI
				Sprite->Show(true);
				if(CurObjs[currentMode]!=NULL)
					CurObjs[currentMode]->Show(true);
			#else
				switch( Sprite->GetAnimation() )
				{
					case CA_Normal    :
						break;
					case CA_HandPointy:
						yDisplacement = CURSOR_HANDPOINTY_TOP_DISPLACEMENT;
						break;
					case CA_Look	  :			
						yDisplacement = CURSOR_LOOK_TOP_DISPLACEMENT;
						break;
					case CA_HandTake  : 
						yDisplacement = CURSOR_HANDTAKE_TOP_DISPLACEMENT;
						break;
					case CA_Help	  :
						yDisplacement = CURSOR_HELP_TOP_DISPLACEMENT;
						break;
					case CA_Gear	  :
						yDisplacement = CURSOR_GEAR_TOP_DISPLACEMENT;
						break;
					case CA_Speech	  :				
						yDisplacement = CURSOR_GEAR_TOP_DISPLACEMENT;
						break;
					case CA_ExitLeft  :					
					case CA_ExitRight :
					case CA_ExitUp    :
					case CA_ExitDown  :
						yDisplacement = CURSOR_TOP_DISPLACEMENT;
						break;
				}
				switch( Sprite->GetAnimation() )
				{
					case CA_Normal    :
						Sprite->Show(false);
						if(CurObjs[currentMode]!=NULL)
							CurObjs[currentMode]->Show(false);
						break;
					case CA_HandPointy:
					case CA_Look	  :			
					case CA_HandTake  : 
					case CA_Help	  :
					case CA_Gear	  :
					case CA_Speech	  :				
					case CA_ExitLeft  :					
					case CA_ExitRight :
					case CA_ExitUp    :
					case CA_ExitDown  :
						if (inMenu)
						{
							Sprite->Show(false);				
							if(CurObjs[currentMode]!=NULL)
								CurObjs[currentMode]->Show(false);
						}
						else
						{
							if(!GFInstance->IsInGameMenuPopup)
							{
								Sprite->Show(true);				
								if(CurObjs[currentMode]!=NULL)
									CurObjs[currentMode]->Show(true);
							}
						}
						break;
				}
			#endif			
			KInput::hidePointer();
		}
		else
		{
			#ifdef K_WIN32
				switch( Sprite->GetAnimation() )
				{
				case CA_Normal    :
					::SetCursor(::LoadCursor(NULL, IDC_ARROW));break;

				case CA_HandPointy:
				case CA_HandTake  : 
					::SetCursor(::LoadCursor(NULL, IDC_HAND));break;
				case CA_Help	  :
					::SetCursor(::LoadCursor(NULL, IDC_HELP));break;

				case CA_Look	  :					
				case CA_Gear	  :
				case CA_Speech	  :				
					::SetCursor(::LoadCursor(NULL, IDC_HAND));break;

				case CA_ExitLeft  :					
				case CA_ExitRight :
					::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));break;
				case CA_ExitUp    :
				case CA_ExitDown  :
					::SetCursor(::LoadCursor(NULL, IDC_SIZENS));break;
				}
			#endif
		}
	}
	else
		KInput::hidePointer();

	const CPoint tempPos((*pos).x,(*pos).y-yDisplacement,(*pos).z);
	Sprite->SetPos(&tempPos);
	if(CurObjs[currentMode]!=NULL)
		CurObjs[currentMode]->SetPos(&tempPos);
}

bool Cursor::IsSystemCursor()
{
	return SystemCursor;
}

void Cursor::HideSystemCursor()
{
	SystemCursor = false;
	KInput::hidePointer();
	#ifndef TOUCH_UI
		Sprite->Show(true);
		if(CurObjs[currentMode]!=NULL)
			CurObjs[currentMode]->Show(true);
	#else
			switch( Sprite->GetAnimation() )
			{
				case CA_Normal    :
					Sprite->Show(false);
					if(CurObjs[currentMode]!=NULL)
						CurObjs[currentMode]->Show(false);
					break;
				case CA_HandPointy:
				case CA_Look	  :			
				case CA_HandTake  : 
				case CA_Help	  :
				case CA_Gear	  :
				case CA_Speech	  :				
				case CA_ExitLeft  :					
				case CA_ExitRight :
				case CA_ExitUp    :
				case CA_ExitDown  :
					if (inMenu)
						{
							Sprite->Show(false);				
							if(CurObjs[currentMode]!=NULL)
								CurObjs[currentMode]->Show(false);
						}
						else
						{
							if(!GFInstance->IsInGameMenuPopup)
							{
								Sprite->Show(true);				
								if(CurObjs[currentMode]!=NULL)
									CurObjs[currentMode]->Show(true);
							}
						}
					break;
			}
	#endif			
	for (int i = 0; i < 8; i++)
	{
		CurObjs[i]->SetEnable(true);
	}
}
void Cursor::UseSystemCursor()
{
	SystemCursor = true;
	KInput::showPointer();
	Sprite->Show(false);
	for (int i = 0; i < 8; i++)
	{
		CurObjs[i]->SetEnable(false);
	}
}

void Cursor::SetInMenu(bool f) // as of now used only in Android, to see if the game is in Menu state or in-game state
{
	// if the game is in Menu state, then we dont show the cursors, else we show the required cursors
	if(f)inMenu = true;
	else inMenu = false;
}
int Cursor::GetMode()
{
	if( Sprite == NULL )
		return -1;

	return Sprite->GetAnimation();
}

void Cursor::SetMode(int mode, int rDescId)
{
	if (Sprite == NULL)
	{
		if (mode == CA_Gear)
		{
			//Glow Code Here
			K_LOG("Object Glow");
		}
		return;
	}

	for (int i = 0; i < 8; i++)
	{
		CurObjs[i]->Show(false);
	}

	CPoint prevPos = CurObjs[currentMode]->Pos;
//	prevPos.y-=CURSOR_TOP_DISPLACEMENT;
	switch( Sprite->GetAnimation() )
	{
		case CA_HandPointy:
			prevPos.y-= CURSOR_HANDPOINTY_TOP_DISPLACEMENT;
			break;
		case CA_Look	  :			
			prevPos.y-= CURSOR_LOOK_TOP_DISPLACEMENT;
			break;
		case CA_HandTake  : 
			prevPos.y-= CURSOR_HANDTAKE_TOP_DISPLACEMENT;
			break;
		case CA_Help	  :
			prevPos.y-= CURSOR_HELP_TOP_DISPLACEMENT;
			break;
		case CA_Gear	  :
			prevPos.y-= CURSOR_GEAR_TOP_DISPLACEMENT;
			break;
		case CA_Speech	  :				
			prevPos.y-= CURSOR_SPEECH_TOP_DISPLACEMENT;
			break;
		case CA_Normal    :
		case CA_ExitLeft  :					
		case CA_ExitRight :
		case CA_ExitUp    :
		case CA_ExitDown  :
			prevPos.y-= CURSOR_TOP_DISPLACEMENT;
			break;
	}

	if( Sprite->GetAnimation() != mode )
		Sprite->SetAnimation(mode);
	switch( mode )
	{
	case CA_Normal    :
		currentMode = mode;
		CurObjs[currentMode]->Pos = prevPos;
		#ifndef TOUCH_UI
			CurObjs[currentMode]->Show(true);
		#else
			CurObjs[currentMode]->Show(false);
		#endif			
		rDescTip->Show(false);
		rDescText->Show(false);						   
		break;
	case CA_HandPointy:
	case CA_Look	  :
		currentMode = mode;
		CurObjs[currentMode]->Pos = prevPos;
		#ifdef TOUCH_UI
			if (inMenu)
				{	
					CurObjs[currentMode]->Show(false);
				}
			else
				{
					CurObjs[currentMode]->Show(false);
					//Disabled cursors because it looks weird on touch
					//if(!GFInstance->IsInGameMenuPopup) CurObjs[currentMode]->Show(true);
				}
		#else
			CurObjs[currentMode]->Show(true);
		#endif
		rDescTip->Show(false);
		rDescText->Show(false);						   
		break;
	case CA_Gear	  : 
	case CA_Help	  :
	case CA_Speech	  :
	case CA_HandTake  : currentMode = mode-5;
		CurObjs[currentMode]->Pos = prevPos;
		#ifdef TOUCH_UI
			if (inMenu)
				{	
					CurObjs[currentMode]->Show(false);
				}
			else
				{
					CurObjs[currentMode]->Show(true);
				}
		#else
			CurObjs[currentMode]->Show(true);
		#endif
		break;
	case CA_ExitLeft  :
	case CA_ExitRight :		
	case CA_ExitUp    :
	case CA_ExitDown  :if( rDescId != -1 && Sprite->Visible )
					   {

						   rDescText->SetText(StringTable::getStr(SceneNames[rDescId]));
						   int rDescWidth = rDescText->StringWidth();

						   int HalfTipWidth = 0;		
						   if( rDescWidth <= 85 )
						   {
							   rDescTip->PlayAnimation(0);//-51
							   HalfTipWidth = 51;
						   }
						   else if( rDescWidth <= 100 )//9=>+3
						   {
							   rDescTip->PlayAnimation(1);//-60
							   HalfTipWidth = 60;
						   }
						   else if( rDescWidth <= 125 )//14=>+4
						   {
							   rDescTip->PlayAnimation(2);//-74
							   HalfTipWidth = 74;
						   }
						   else if( rDescWidth <= 146 )//9=>+3
						   {
							   rDescTip->PlayAnimation(3);//-83
							   HalfTipWidth = 83;
						   }
						   else if( rDescWidth <= 178 )//14=>+4
						   {
							   rDescTip->PlayAnimation(4);//-97
							   HalfTipWidth = 97;
						   }
						   else if( rDescWidth <= 190 )//8=>+3
						   {
							   rDescTip->PlayAnimation(5);//-105
							   HalfTipWidth = 105;
						   }
						   else if( rDescWidth <= 211 )//10=>+3
						   {
							   rDescTip->PlayAnimation(6);//-115
							   HalfTipWidth = 115;
						   }
						   else if( rDescWidth <= 220 )//10=>+3
						   {
							   rDescTip->PlayAnimation(7);//-115
							   HalfTipWidth = 119;
						   }
						   else //~220 and all++ - //4=>+1
						   {
							   rDescTip->PlayAnimation(8);//-119
							   HalfTipWidth = 133;
						   }



						   CPoint Pos = Sprite->Pos;								
						   switch( mode )
						   {
								#ifndef TOUCH_UI
									case CA_ExitLeft  :Pos.x += (-36+HalfTipWidth);Pos.y += -38; break;
									case CA_ExitRight :Pos.x += (-70+HalfTipWidth);Pos.y += -38; break;		
									case CA_ExitUp    :Pos.x += 2;Pos.y += 58; break;
									case CA_ExitDown  :Pos.x -= 4; Pos.y -= 22; break;
								#else
									case CA_ExitLeft  :Pos.x += (-36+HalfTipWidth);Pos.y += -3.25*CURSOR_TOP_DISPLACEMENT; break;
									case CA_ExitRight :Pos.x += (-70+HalfTipWidth);Pos.y += -3.25*CURSOR_TOP_DISPLACEMENT; break;		
									case CA_ExitUp    :Pos.x += 2;Pos.y += -3.25*CURSOR_TOP_DISPLACEMENT; break;
									case CA_ExitDown  :Pos.x -= 4; Pos.y += -3.25*CURSOR_TOP_DISPLACEMENT; break;
								#endif
						   }
						   rDescTip->SetPos(&Pos);								
						   rDescTip->Show(true);
						   Pos.y -= 15;
						   rDescText->SetPos(&Pos);
						   rDescText->Show(true);
					   }
					   else
					   {
						   rDescTip->Show(false);
						   rDescText->Show(false);
					   }
					   currentMode = 3;
					   CurObjs[currentMode]->Pos = prevPos;
						CurObjs[currentMode]->Show(true);
					   switch( mode )
					   {
					   case CA_ExitLeft  :CurObjs[currentMode]->SetRotate(90); break;
					   case CA_ExitRight :CurObjs[currentMode]->SetRotate(-90); break;		
					   case CA_ExitUp    :CurObjs[currentMode]->SetRotate(0);break;
					   case CA_ExitDown  :CurObjs[currentMode]->SetRotate(180);break;
					   }
					   break;
	}
}
