//===========================================================
// 
// Framework
//===========================================================

#ifndef DATA_ENUMS_H
#define DATA_ENUMS_H

#include <string>
#include "Point.h"
#include "Rect.h"
#include "exMath.h"
#include "iPadDefs.h"


#define INVBOXITEM_NORMAL_ZORDER   166
#define INVBOXITEM_DRAG_ZORDER	  7000
#define MAX_BANNER_ALPHA		  1.0f
#define MAX_BANNERTEXT_ALPHA	  0.95f

#define POPUP_SCALE_FACTOR 1.75f
#define POPUP_SCALEDOWN_FACTOR 2.5f

#define MAX_INVBOX_COLS 7
const float InvBoxItemXOff = -252.0f;
const float InvBoxItemYOff = -50.0f+2;
const float InvBoxItemSpaceX = 100.0f;
const float InvBoxItemSpaceY = 89.0f;

const CPoint InvBoxTargetHitPos(472+33,754,0);//939.0f, 711.0f, 0.0f);

const CPoint JrnHOPos(237,708,0);
const CPoint PuzzleOff(932.0f, 40.0f, 0.0f);
const CPoint PuzzleOffb(206.0f, 720.0f, 0.0f);

#define HINT_OFFSETX 3
#define HINT_OFFSETY 2

const CPoint MapButtonPos(939.0f+HINT_OFFSETX, 675.0f+HINT_OFFSETY, 0.0f);

#ifdef TOUCH_UI
const CRectangle HoveredBackBtnRectMax(0.0f, 600.0f,1366.0f,768-600.0f);
#else
const CRectangle HoveredBackBtnRectMax(0.0f, 575.0f,1366.0f,768-575.0f);
#endif

enum _JrnPages{ PAGE_1,PAGE_2,PAGE_3,PAGE_4,PAGE_5,PAGE_6,PAGE_7,PAGE_8,PAGE_9,PAGE_10,
				PAGE_11,PAGE_12,PAGE_13,PAGE_14,PAGE_15,PAGE_16, PAGE_17, PAGE_18, PAGE_19,
				PAGE_20, PAGE_21, PAGE_22, PAGE_23, PAGE_24, PAGE_25, PAGE_26, PAGE_27, PAGE_28, PAGE_29,
				PAGE_30, PAGE_31, PAGE_32, PAGE_33, PAGE_34, PAGE_35, PAGE_36, PAGE_37, PAGE_38, PAGE_39,
				PAGE_40, PAGE_41, PAGE_42, PAGE_43, PAGE_44, PAGE_45, PAGE_46, PAGE_47, PAGE_48, PAGE_49,
				PAGE_50, PAGE_51, PAGE_52, PAGE_53, PAGE_54, PAGE_55, PAGE_56, PAGE_57, PAGE_58, PAGE_59,
				PAGE_60, PAGE_61, PAGE_62, PAGE_63, PAGE_64, PAGE_65, PAGE_66, PAGE_67, PAGE_68, PAGE_69,
				PAGE_70, PAGE_71, PAGE_72, MAX_JRN_PAGES};
enum _JrnStates{ JRN_IDLE, JRN_POPUP, JRN_POPPED, JRN_POPDOWN, JRN_FLYING };
enum _MBannerSFX{ SFX_SOFT_NOTIFY, SFX_WRONG_NOTIFY, SFX_EMPTY_NOTIFY, SFX_NIL };

enum eMessageBannerState
{
	eHMS_TransIn,
	eHMS_Idle,
	eHMS_TransOut
};

enum EAwards
{	
	eAward_PuzzleUnderOneMin,
	eAward_PuzzleAllNoSkip,
	eAward_PuzzleThreeNoSkip,
	eAward_HOFindThreeIn10Secs,
	eAward_HOCompleteWOMisclick,
	eAward_HOCompleteWOHint,
	eAward_ExtrasAllMorphs,
	eAward_ExtrasAllMemories,
	eAward_ExtrasAllAwards,
	eAward_GameCompleteIn10Hours,
	eAward_GameFindWhoWreckedIndus,
	eAward_GameVisit3Worlds,
	eAward_GameTamePeacock,
	eAward_GameUnravelNagaStory,
	eAward_GameSaveSofia,

	eAward_MAX
};


enum EJrnNotes
{
	//L1 Pages
	eJrn_L1P1_MomsThoughts,
	eJrn_L1P2_VacationBegins,
	eJrn_L1P3_SophiaIsTakenAway,
	eJrn_L1P4_KidPainting,
	eJrn_L1P5_CursedMansion,
	eJrn_L1P6_ForgiveMeAnnMary,
	eJrn_L1P7_MooppanFlyer,
	eJrn_L1P8_CatAttack,
	eJrn_L1P9_RajaFoyerJamesTaken,
	eJrn_L1P10_CreekEscaped,
	eJrn_L1P11_CompanyWarningNotice,
	eJrn_L1P12_CreekDiaryNotes,
	eJrn_L1P13_LasyaPush,
	eJrn_L1P14_AfterLasyaAttack,
	eJrn_L1P15_AfterTunnelblast,
	eJrn_L1P16_DakartaNewspaper,

	//L2 Pages
	eJrn_L2P1_BoatAttackReflection,
	eJrn_L2P2_ReachedNarara,
	eJrn_L2P3_SnakeTempleStorm,
	eJrn_L2P4_MooppanMeet,
	eJrn_L2P5_CrystalBallSophia,
	eJrn_L2P6_LasyaAttacksCave,
	eJrn_L2P7_MeetTheSoldiers,
	eJrn_L2P8_GrandpaMurdered,
	eJrn_L2P9_PastMemories,

	//L3 Pages
	eJrn_L3P1_WallExplode,
	eJrn_L3P2_LasyaAttack,
	eJrn_L3P3_AmeehaDiaryNote,
	eJrn_L3P4_GrandmaTalkEnds,
	eJrn_L3P5_CrystalVideoEnds,
	eJrn_L3P6_MaryFallDown,

	//L4 Pages
	eJrn_L4P1_AmeehaDiaryNoteEnds,
	eJrn_L4P2_MaryJumpGap,
	eJrn_L4P3_CrystalVideo,
	eJrn_L4P4_MaryL4Ending,

	//L5 Pages
	eJrn_L5P1_TigerCutscene,
	eJrn_L5P2_SnakeVideoEnds,
	eJrn_L5P3_CrystalVideo,
	eJrn_L5P4_MeetingLathika,
	eJrn_L5P5_AmeehaDiaryNote,
	eJrn_L5P6_LathikaFreed,
	eJrn_L5P7_TreeReleased,
	eJrn_L5P8_MeetingPeacock,
	eJrn_L5P9_PeacockRideEnds,

	//L6 Pages
	eJrn_L6P1_VaruniVideoEnd1,
	eJrn_L6P2_VaruniVideoEnd2,
	eJrn_L6P3_DescriptionHOSolved,
	eJrn_L6P4_NaginNotice,
	eJrn_L6P5_LasyaBrokeBridge,
	eJrn_L6P6_AmeehaNote,
	eJrn_L6P7_AnanthaNote,
	eJrn_L6P8_RockmanPowderGot,
	eJrn_L6P9_MagicPowderFlier,
	eJrn_L6P10_CrystalBallVideo,
	eJrn_L6P11_EnterSophiaBedroom,
	eJrn_L6P12_SophiaMemoriesEnd,

	//L7 pages
	eJrn_L7P1_EnterLasyaPrison,
	eJrn_L7P2_AmeehaNotice,
	eJrn_L7P3_AmeehaDialogEnd,
	eJrn_L7P4_ThreeFlowerNotice,
	eJrn_L7P5_HavenOfResurrectionNotice,
	eJrn_L7P6_LasyaAttack1,
	eJrn_L7P7_LasyaAttack2,
	eJrn_L7P8_AnanthaWeepDialogEnd,
	eJrn_L7P9_Nightmare,
	eJrn_L7P10_DarkartaThrowsKid,

	//L8 pages
	eJrn_L8P1_ShukraVideoEnd,
	eJrn_L8P2_PhoenixNoteEnd,
	eJrn_L8P3_MammothMeet,
	eJrn_L8P4_FlownToMohenjedaro,
	eJrn_L8P5_MaryResurrect,
	eJrn_L8P6_GrandReunion

};

enum ELevel
{
	Level_Nil = -1,
	Level_1,
	Level_2,
	Level_3,
	Level_4,
	Level_5,
	Level_6,
	Level_7,
	Level_8,
	Level_Max
};

enum EArea
{
	eArea_Nil = -1,
	//Level 1 - demo
	eArea_L1_S1_Beach,
	eArea_L1_S1_PZKidsPainting,
	eArea_L1_S2_Frontyard,
	eArea_L1_S2_PZFuse,

	eArea_L1_S3_Foyer,
	eArea_L1_S3_PZShelf,
	eArea_L1_S4_PZDoor,
	eArea_L1_S4_Bedroom,
	eArea_L1_S4_PZPainting,
	eArea_L1_S3_DoorHO,

	eArea_L1_S5_TreasureRoom,
	eArea_L1_S5_TunnelDoor,
	eArea_L1_S5_PZDynamiteMaking,
	eArea_L1_S5_HOTreasureRoom,
	eArea_L1_S6_BoatShed,
	eArea_L1_S6_HOBoatShed,
	eArea_L1_S7_BoatDock,

	eArea_L1_EndOfDemo,
	eArea_KidsRoom,
	eArea_ExtrasCutscenePlayer,
	eArea_ExtrasRoom,
	eArea_AwardsRoom,

	//Level 2
	eArea_L2_S1_ForestEntrance,
	eArea_L2_S1_UndergroundHO,
	eArea_L2_S2_PZMango,
	eArea_L2_S2_NagaTemple,
	eArea_L2_S2_HONagaTemple,
	eArea_L2_S3_CaveMouth,
	eArea_L2_S3_PZStoneBeam,
	eArea_L2_S4_Cave,
	eArea_L2_S4_PZDeities,
	eArea_L2_S5_MoopansDen,
	eArea_L2_S5_PZEagleUnblock,
	eArea_L2_S5_PZMedicine_Making,
	eArea_L2_S6_TunnelEnd,
	eArea_L2_S6_HOWaterfall,
	eArea_L2_S6_PZTunnelClock,
	eArea_L2_S7_PZSoldiers,

	//Level 3
	eArea_L3_S1_FortEntrance,
	eArea_L3_S1_HOLadder,
	eArea_L3_S1_PZLionHead, 
	eArea_L3_S2_FortInterior,
	eArea_L3_S2_P1Room1Start,
	eArea_L3_S2_P1Room2Wine,
	eArea_L3_S2_P1Room3Lock,
	eArea_L3_S2_P1Room4Match3,
	eArea_L3_S2_P1Room5Pipe,
	eArea_L3_S2_P1Room6Pillar,
	eArea_L3_S2_P1DeadEnd,
	eArea_L3_S3_Pool,
	eArea_L3_S3_PZBlocks,
	eArea_L3_S3_PZUnderwater,
	eArea_L3_S4_FortFountain,
	eArea_L3_S4_FortFountainHO,	
	eArea_L3_S5_Cliff,

	//Level 4
	eArea_L4_S1_SwampEntrance,
	eArea_L4_S1_ShoeHO,
	eArea_L4_S2_RiverSide,
	eArea_L4_S2_PZAlligatorHead,
	eArea_L4_S2_PZDoor,
	eArea_L4_S3_Cave,
	eArea_L4_S3_PZFireMaking,
	eArea_L4_S3_PZDoor,
	eArea_L4_S4_HillTop,
	eArea_L4_S4_BinocularHO,
	eArea_L4_S5_HillSide,
	eArea_L4_S5_PZSwapTile,

	//Level 5
	eArea_L5_S1_TigerDistant,
	eArea_L5_S2_TigerCloseup,
	eArea_L5_S2_PZBlocks,
	eArea_L5_S3_Pond,
	eArea_L5_S3_PZDungeonDoor,
	eArea_L5_S3_HOUnderwater,
	eArea_L5_S4_Dungeon,
	eArea_L5_S4_PZHandCuff,
	eArea_L5_S5_MountainBelow,
	eArea_L5_S5_HOGanapathi,
	eArea_L5_S5_PZSunClock,
	eArea_L5_S6_MountainTop,	
	eArea_L5_S6_HOTree,

	//Level 6
	eArea_L6_S1_FloatingIsland1,
	eArea_L6_S1_DescriptionHO,
	eArea_L6_S2_FloatingIsland2,
	eArea_L6_S2_ShereHO,
	eArea_L6_S2_PZPineGum,
	eArea_L6_S3_FloatingIsland3,
	eArea_L6_S3_FakeJames,
	eArea_L6_S3_GlowingHeartHO,
	eArea_L6_S3_PZPathway,
	eArea_L6_S4_FloatingIsland4,
	eArea_L6_S4_PortalChest,
	eArea_L6_S4_PortalPuzzle,
	eArea_L6_S5_MarysDream,
	eArea_L6_S5_TabMiniGame,

	//Level 7
	eArea_L7_S1_Volcano,
	eArea_L7_S1_PZJigsawYellow,
	eArea_L7_S1_PZStarPuzzle,
	eArea_L7_S2_LasyaPrison,
	eArea_L7_S2_HORightHand,
	eArea_L7_S2_HOLeftHand,
	eArea_L7_S2_HOBrokenRightHand,
	eArea_L7_S2_HOBrokenLeftHand,
	eArea_L7_S2_PZEyePuzzle,
	eArea_L7_S2_PZLasyaStatue,
	eArea_L7_S3_Haven,
	eArea_L7_S3_PZDoorPuzzle,
	eArea_L7_S3_PZJigsawRed,
	eArea_L7_S4_Ameeha,
	eArea_L7_S4_PZJigsawBlue,
	eArea_L7_S5_SophiaEnd,

	//Level 8
	eArea_L8_S1_ShukrasPlace,
	eArea_L8_S1_PZDoorHandPuzzle,
	eArea_L8_S1_DoorHO,
	eArea_L8_S2_Garden,
	eArea_L8_S2_PZGears,
	eArea_L8_S2_PZGate,
	eArea_L8_S2_PZSugarCane,
	eArea_L8_S3_BanyanTree,
	eArea_L8_S3_BanyanTreeHO,
	eArea_L8_S4_Pool,
	eArea_L8_S4_PZPoolGame,
	eArea_L8_S5_VolcanoLand,
	eArea_L8_S5_VirtualMiniGame,

	eArea_Credits,

	eArea_Max
};

enum EPopup
{
	ePopInactive = -1,
	
	//		Level 1		//

	//L1S1_Beach
	ePopL1S1_P1Roses,
	ePopL1S1_P2CreekBoard,

	//L1S2_Frontyard
	ePopL1S2_P1CarDickie,
	ePopL1S2_P2Love,
	ePopL1S2_P3Swing,
	
	//L1S3_Foyer
	ePopL1S3_P1Shelf,
	ePopL1S3_P2Table,
	ePopL1S3_P3Photos,

	//L1S4_Bedroom
	ePopL1S4_P1Drawer,
	ePopL1S4_P2KeyFall,
	ePopL1S4_P3BeetleShelf,

	//L1S5_TreasureRoom
	ePopL1S5_P1WallSide,
	ePopL1S5_P2FireLamp,

	//L1S5_TunnelDoor
	ePopL1S5_P3HelpManual,
	ePopL1S5_P2Bomb,

	//L1S6_Boatshed
	ePopL1S6_P1Pumper,
	ePopL1S6_P2Switch,

	//L1S7_Boatdock
	ePopL1S7_P1DieselTank,
	ePopL1S7_P2BoatKeyStart,	

	ePopL3S2_PZP1PipePuzzle
};

//Zorder Enums
enum EZOrder
{
	eZOrder_PopFrame=100,
	eZOrder_PopCloseBtn=101,
	
	eZOrder_PopupMask=150,
	eZOrder_Popup=1000,
	eZOrder_PopupCloseButton=1100, //Not used in level 1
	eZOrder_PopupOverPopupMask=1200,
	eZOrder_PopupOverPopup=1250,
	eZOrder_PopupOverPopupCloseButton=1300,
	
	eZOrder_ComboInvMask=1400,
	eZOrder_ComboInvScene=1450,
	eZOrder_ComboPopMask=1550,
	eZOrder_ComboPopup=1600,

	eZOrder_FoundInvMask=1650,
	eZOrder_FoundInvPopup=1700,

	eZOrder_INVBOXBG=1750,
	eZOrder_HUD=1900,
	eZOrder_INVBOXITEMS=1800,

	eZOrder_MapdiaryMask=2000,
	eZOrder_MapdiaryPopup=2050,

	eZOrder_CutsceneFull=2100,

	eZOrder_InGameMenuMask=2200,
	eZOrder_InGameMenu=2250,

	eZOrder_OptionsScn=2300,
	eZOrder_ModesMenuMask=2400,
	eZOrder_ModeSelectScn=2450,

	eZOrder_Banner = 3150,
};

//Inventory Details
enum EHOInventory
{					
	//Level 1 - 30
	eInv_L1S1_Bag, //FIRST LEVEL1 ITEM
	eInv_L1S1_RubyPart1,
	eInv_L1S1_RubyPart2,
	eInv_L1S1_RubyPart3,
	eInv_L1S1_RubyPart4,
	eInv_L1S1_SwissKnife,
	eInv_L1S1_Plank3,

	eInv_L1S2_EmptyCan,
	eInv_L1S2_Hose,
	eInv_L1S2_Plank1,
	eInv_L1S2_TreeBranch,
	eInv_L1S2_Cloth1Swing,
	eInv_L1S2_Cloth2LovePop,
	eInv_L1S2_Cloth3CarDicky,

	eInv_L1S3_Banana,
	eInv_L1S3_MatchBox,
	eInv_L1S3_NagaKey,
	eInv_L1S3_HOLocket,
	eInv_L1S3_ShadowHand1,

	eInv_L1S4_ShadowHand2,
	eInv_L1S4_KeySet,
	eInv_L1S4_BeetleHead,
	eInv_L1S4_BeetleTail,
	eInv_L1S4_Coin,
	eInv_L1S4_Diary,
	eInv_L1S4_PhotoFrame,

	eInv_L1S5_BoatKey,
	eInv_L1S5_Dynamite,
	eInv_L1S5_HammerHandle,
	eInv_L1S5_HammerHead,
	eInv_L1S5_MortarPestle,

	eInv_L1S7_Plank2,
	eInv_L1S6_CanFull,
	eInv_L1S6_Crowbar,
	eInv_L1S6_Funnel,
	eInv_L1S6_OilDipper,

	eInv_L1CB_Beetle,
	eInv_L1CB_Hammer,
	eInv_L1S1_CreekLetter,
	eInv_L1S4_BookCode, //LAST LEVEL1 ITEM

	// Level 2
	eInv_L2S1_Aloevera,
	eInv_L2S1_HoneyComb,
	eInv_L2S1_PotWithWater,
	eInv_L2S1_ColorPowder,
	eInv_L2S1_RoundStone,
	eInv_L2S1_Scrapper,

	eInv_L2S2_Frankinscence,
	eInv_L2S2_Mangoes,
	eInv_L2S2_Pearl1,
	eInv_L2S2_Pearl2,
	eInv_L2S2_Pearl3,
	eInv_L2S2_Pearl4,

	eInv_L2S3_PoojaLamp,
	eInv_L2S3_Turmeric,

	eInv_L2S4_AxeBlunt,
	eInv_L2S4_AxeSharpen,
	eInv_L2S4_LitTorch,
	eInv_L2S4_UnlitTorch,
	eInv_L2S4_UnlitOilTorch,
	eInv_L2S4_Firewoods,
	eInv_L2S4_Rake,
	eInv_L2S4_Lever,
	
	eInv_L2S5_SnakeChart,
	eInv_L2S5_MedicineJar,
	eInv_L2S5_NagaSculpture,
	eInv_L2S5_PotEmpty,
	eInv_L2S5_Ember,
	eInv_L2S5_StoneSharpener,
	eInv_L2S5_VinegarBottle,

	eInv_L2S6_Gloves,
	eInv_L2S6_SmallStone1,
	eInv_L2S6_SmallStone2,
	eInv_L2S6_SmallStone3,
	eInv_L2S6_KnifeBlunt,
	eInv_L2S6_KnifeSharpen,
	eInv_L2S6_PaperPuzClue,

	//Level 3
	eInv_L3S1_Charcoal,
	eInv_L3S1_FlagFull,
	eInv_L3S1_Spear,
	eInv_L3S1_FlagCloth,
	eInv_L3S1_GreenEye,
	eInv_L3S1_GreenCrystal,
	eInv_L3S1_LadderBroken,
	eInv_L3S1_LadderPiece1,
	eInv_L3S1_LadderFixed,

	eInv_L3S2_CoconutFull,
	eInv_L3S2_CoconutShell,
	eInv_L3S2_CocoShellPowderFilled,
	eInv_L3S2_CoconutDynamite,
	eInv_L3S2_Twine,
	eInv_L3S2_Sickle,
	eInv_L3S2_SkullTeeth,
	eInv_L3S2_Hammer,
	eInv_L3S2_HammerHandle,
	eInv_L3S2_HammerHead,
	eInv_L3S2_PipePiece1Room1,
	eInv_L3S2_PipePiece2Room2,
	eInv_L3S2_PipePiece3Room3,
	eInv_L3S2_PipePiece4Room4,	
	eInv_L3S2_PuzzleBlock1Left,
	eInv_L3S2_PuzzleBlock2Right,

	eInv_L3S3_Box,
	eInv_L3S3_SnakeDoorPart,
	eInv_L3S3_CupEmpty,
	eInv_L3S3_CupWaterFilled,
	eInv_L3S3_Saw,
	eInv_L3S3_Showel,
	eInv_L3S3_LadderPiece2,

	eInv_L3S4_Nail,	
	eInv_L3S4_MetalSheetBlank,
	eInv_L3S4_MetalSheetMapDrawn,
	eInv_L3S4_BambooStick,
	eInv_L3S4_SharpTool,

	eInv_L3S5_Grass1,
	eInv_L3S5_Grass2,
	eInv_L3S5_Grass3,
	eInv_L3S5_Grass4,
	eInv_L3S5_Grass5,
	eInv_L3S5_Grass6,
	eInv_L3S5_MetalClip,
	eInv_L3S5_Rope,

	//level 4
	eInv_L4_S1_Knife,
	eInv_L4_S1_Bracelet,
	eInv_L4_S1_KerchiefPiece1,
	eInv_L4_S1_KerchiefPiece2,
	eInv_L4_S1_Shoe1,
	eInv_L4_S1_Shoe2,
	eInv_L4_S1_BinocularLens,
	eInv_L4_S1_Leaf,
	eInv_L4_S1_Stick,
	eInv_L4_S1_LeafSpoon,
	eInv_L4_S1_LeafSpoonWithWater,
	eInv_L4_S1_Medicine,
	eInv_L4_S1_Crutches,

	eInv_L4_S2_Bamboo,
	eInv_L4_S2_BambooStick1,
	eInv_L4_S2_BambooStick2,
	eInv_L4_S2_BambooStick3,
	eInv_L4_S2_BambooStick4,
	eInv_L4_S2_Twig1,
	eInv_L4_S2_GrinderHandle,
	eInv_L4_S2_GrassRope,
	eInv_L4_S2_GrassBunch,
	eInv_L4_S2_Strainer,
	
	eInv_L4_S3_Pot,
	eInv_L4_S3_PotWithOlive,
	
	eInv_L4_S4_BinocularWithoutLens,
	eInv_L4_S4_Twig2,
	
	eInv_L4_S5_Door,
	eInv_L4_S5_Olives,
	eInv_L4_S5_CaveDoorKey,
	eInv_L4_S5_Twig3,
	eInv_L4_S5_AlligatorFace,
	eInv_L4_S5_Net,
	eInv_L4_S5_GrinderTop,

	eInv_L4_CB_HandKerchief,
	eInv_L4_CB_Binocular,
	eInv_L4_CB_BambooWithKnife,

	//Level 5
	eInv_L5_S1_ArrowHolder,
	eInv_L5_S1_BowArrowFixed,
	eInv_L5_S1_Bow,
	eInv_L5_S2_GreasySapBark,
	eInv_L5_S2_Hammer,
	eInv_L5_S2_SoldierBadge,
	eInv_L5_S3_Hook,
	eInv_L5_S3_MoonSolution,
	eInv_L5_S4_PeacockChain,
	eInv_L5_S4_Pickaxe,
	eInv_L5_S4_PickaxeBlade,
	eInv_L5_S4_Woodbark,
	eInv_L5_S5_BambooLadder,
	eInv_L5_S5_DivingBag,
	eInv_L5_S5_Keyhandcuff,
	eInv_L5_S5_PickaxeHandle,
	eInv_L5_S6_Locket,
	eInv_L5_S5_StoneEmblem,

	//Level 6
	eInv_L6_S1_Mortar,
	eInv_L6_S1_Steel,
	eInv_L6_S1_MetalContainer,
	eInv_L6_S1_Emblem,
	eInv_L6_S1_RagCloth,
	eInv_L6_S1_Tongs,
	eInv_L6_S1_Club,
	eInv_L6_S2_PuzzlePices,
	eInv_L6_S2_PinegumStick,
	eInv_L6_S2_StarShapedKeyInIce,
	eInv_L6_S2_Pestle,
	eInv_L6_S3_PuzzleCode,
	eInv_L6_S3_GlowingHeart,
	eInv_L6_S3_Charcoal,
	eInv_L6_S3_HorseShoe,
	eInv_L6_S4_Stick,
	eInv_L6_S4_PinePitch,
	eInv_L6_S4_Rope,
	eInv_L6_S4_FlintStones,
	eInv_L6_S4_MagnetisedHorseShoe,
	eInv_L6_S4_StarShapedKey,
	eInv_L6_S4_Lens,
	eInv_L6_S4_HeartContainer,
	eInv_L6_S4_ColouredPowder,

	eInv_L6_CB_HeartContainerWithCrystal,
	eInv_L6_CB_RopeWithMagnetisedHorseShoe,

	//Level 7
	eInv_L7_S1_BlueStarEmblem,
	eInv_L7_S1_PotEmpty,
	eInv_L7_S1_PotWithMagicWater,
	eInv_L7_S1_PotWithChain,
	eInv_L7_S1_RedFlower,
	eInv_L7_S1_RedStarEmblem,
	eInv_L7_S1_YellowStarEmblem,

	eInv_L7_S2_LasyaStatue,
	eInv_L7_S2_SphereFull,
	eInv_L7_S2_SphereHalfBase,
	eInv_L7_S2_SphereHalfToFix,
	eInv_L7_S2_SymbolDisk,

	eInv_L7_S3_AxeKey,
	eInv_L7_S3_Axes,
	eInv_L7_S3_Bhasmam,
	eInv_L7_S3_Broom,
	eInv_L7_S3_Lever,
	eInv_L7_S3_MetalChain,
	eInv_L7_S3_MetalWire,
	eInv_L7_S3_YellowFlowe,

	eInv_L7_S4_BlueFlower,
	eInv_L7_S4_DiwaliLamp,
	eInv_L7_S4_WellGear,

	//Level 8

	eInv_L8_S1_MagicGlass,
	eInv_L8_S1_MagicWand,
	eInv_L8_S1_DoorKey,
	eInv_L8_S1_HalfKey,
	eInv_L8_S1_IdolGold,

	eInv_L8_S2_Sugarcane,
	eInv_L8_S2_HalfKey,
	eInv_L8_S2_Tool,
	eInv_L8_S2_SymbolCombination,
	eInv_L8_S2_SwordHandle,
	eInv_L8_S2_IdolSilver,

	eInv_L8_S3_Sword,
	eInv_L8_S3_CrystalKey,
	eInv_L8_S3_MaryLeaf,
	eInv_L8_S3_IdolBronze,

	eInv_L8_S4_ContainerWithPurifiedLeaf,
	eInv_L8_CB_FullKey,
	eInv_L8_CB_MagicWandWithoutKey,

	eInv_Max
};

extern std::string	InventoryDescrips[eInv_Max];
extern std::string	InventorySprNames[eInv_Max];
extern std::string	InvBigSprNames[eInv_Max];
extern std::string	LevelAreaNames[eArea_Max+1];
//End Inventory details

#endif
