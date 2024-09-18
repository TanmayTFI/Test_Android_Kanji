//===========================================================
// 
// Banner Texts
// Faisal
//===========================================================

#ifndef BANNER_ENUMS_H
#define BANNER_ENUMS_H

#include <string>
#include <map>
#include <vector>
#include "GameTextManager.h"

using namespace std;
/**
* Info Text Array
* Format -- eInfo_LxSx_prefixName
*
* divided into 5 (or less depending on scene Sx) group for better readability & ordering
*
* Normal Info banner - Prefix None + detail
* Inventory Dest banner - Prefix 'Dest'
* Wrong inv placed banner - Prefix 'WrgInv'
* Video played after banner - Prefix 'VidNameEnd'
* Special Puzzle texts banner - Prefix 'Pz1Stuff'
* HO texts banner - Prefix 'HOStuff'
**/

//also change MAX_WRONGDROP_MSG in GameFlow if changing this
#define MAX_WRONGDROP_TEXTS 15

enum eObjectives
{
	eObj_L1O1_HelpJames,
	eObj_L1O2_LitTheMansion,
	eObj_L1O3_FindBoatKey,
	eObj_L1O4_EscapeMansion,
	eObj_L1O5_FindBoatFuel,

	eObj_L2O1_MeetMooppan,
	eObj_L2O2_PrepareMedicine,
	eObj_L2O3_NagaPearls,
	eObj_L2O4_FindWayOutOfNetherworld,

	eObj_L3O1_ExploreArea,
	eObj_L3O2_UnravelMazeMystery,
	eObj_L3O3_UnravelWellRiddle,
	eObj_L3O4_FindWayFromBrokenCastle,

	eObj_L4O1_BandageTheWound,
	eObj_L4O2_FindWayOutOfForest,
	eObj_L4O3_FindWayToMakeFire,
	eObj_L4O4_FindWayToMakeOil,

	eObj_L5O1_GetRidOfTiger,
	eObj_L5O2_FindWayOutOfAtharwaSwamps,
	eObj_L5O3_OpenTheDungeon,
	eObj_L5O4_FindWayToAnasuya,
	eObj_L5O5_FindWayToFreeAnasuya,

	eObj_L6O1_FindWayOutOfNetherworld,
	eObj_L6O2_FindWayOutToConnectBridge,
	eObj_L6O3_FindMissingRockManHeart,
	eObj_L6O4_FindSophia,

	eObj_L7O1_FindWayOutOfLasyaPrison,
	eObj_L7O2_FindWayToSophia,
	eObj_L7O3_FindWayToSaveAmeeha,

	eObj_L8O1_FindWayToResurrectMary,
	eObj_L8O2_FindWayToReachMohenjedaro,
	eObj_L8O3_PurifyLeaf,

	eObj_Max
};


enum EMemCollectables
{
	eMem_L1S1Teddy,
	eMem_L1S3MaryLamb,
	eMem_L1S3RibbonNeck,
	eMem_L1S4BoxingGloves,
	eMem_L1S6ChappathiRoll,

	eMem_L2S5Statue,
	eMem_L2S4TigerMask,
	eMem_L2S5HoneyBottle,
	eMem_L2S5Stethoscope,
	eMem_L2S6Spectacles,

	eMem_L3S1MemMirror,
	eMem_L3S2SunDrawing,
	eMem_L3S3Shoes,
	eMem_L3S4CatStatue,
	eMem_L3S5EasterEgg,

	eMem_L4S1Pumpkin,
	eMem_L4S2Banana,
	eMem_L4S3Honey,
	eMem_L4S4Colorleaf,
	eMem_L4S5StarLocket,

	eMem_L5S2DogStatue,
	eMem_L5S2FootStone,
	eMem_L5S3AngelStatue,
	eMem_L5S4Watermelon,
	eMem_L5S5Flower,

	eMem_L6S1LocketChain,
	eMem_L6S2MotherkidStatue,
	eMem_L6S3PaintBrush,
	eMem_L6S4MotherStatue,
	eMem_L6S5KidSandal,
	eMem_MAX
};

enum EInfoTexts
{
	//L1
	eInfo_L1S9_DestDieselIfFuelNeeded,


	Banner_L3_S1_FortEntrence = 300,
	Banner_L3_S1_ClickingOnFortInteriorDoor,
	Banner_L3_S1_ClickingOnBurningCoil,
	Banner_L3_S1_SpadeOnBurningCoil,
	Banner_L3_S1_AddingWaterOnBurningCoil,
	Banner_L3_S1_ClickOnCooledCoil,
	Banner_L3_S1_ClickOnSpearHook,
	Banner_L3_S1_ClickOnSkullFace,
	Banner_L3_S1_ClickNavFortWitoutLadder,
	Banner_L3_S1_MazeWrongPathText1,
	Banner_L3_S1_MazeWrongPathText2,
	Banner_L3_S1_MazeWrongPathText3,
	Banner_L3_S1_M1_WithoutMap,
	Banner_L3_S2_M1_ClickCreeper,
	Banner_L3_S2_M1_ClickOnWell,
	Banner_L3_S2_M2_EnterFirstTime,
	Banner_L3_S2_M2_CreeperPzComplete,
	Banner_L3_S2_M2_ClickCreeper,
	Banner_L3_S2_M2_ClickOnRubble,
	Banner_L3_S2_M3_EnterFirstTime,
	Banner_L3_S2_M3_AfterHammerHeadCollect,
	Banner_L3_S2_M3_ClickOnLockedDoor,
	Banner_L3_S2_M3_ClickOnSand,
	Banner_L3_S2_M3_ClosingPopupWithoutTakeHammerHead,
	Banner_L3_S2_M4_ClickOnLockedDoor,
	Banner_L3_S2_M5_ClickOnLockeDoor,
	Banner_L3_S2_M5_ClickOnPumb,
	Banner_L3_S2_M5_ClickOnPopupWithoutPipe,
	Banner_L3_S2_M5_ClickOnPopupWithPipe,
	Banner_L3_S2_M5_DropPartualPipes,
	Banner_L3_S2_M6_ClickOnSqueBox,
	Banner_L3_S2_M6_PillerSolved,
	Banner_L3_S2_LasiayAttackComplete,
	Banner_L3_S2_MazeOnClickCrystal,
	Banner_L3_S3_BeforeCounetOuterRemove,
	Banner_L3_S3_AfterCocunetOuterRemoved,
	Banner_L3_S3_WhenSawSophiaShoe,
	Banner_L3_S3_SophiaCutsceneComplete,
	Banner_L3_S3_HammerHeadMultiInv,
	
	Banner_L4_S3_ICantReachIt,

	Banner_L5_S3_AfterCutScene = 500,
	Banner_L5_S3_ClickOnDungonDoor,
	Banner_L5_S3_HookFallsIntoTheWater,
	Banner_L5_S3_ClickAfterHookLost,
	Banner_L5_S3_NoDivingBell,
	Banner_L5_S3_UsingDivingBell,
	Banner_L5_S3_ClickStoneOnMiddle,
	Banner_L5_S3_SophiaCutSceneComplete,
	Banner_L5_S3_NoOutCreeper,	
	Banner_L5_S3_waterproofBagCollect,
	Banner_L5_S4_HandCuffClickWithoutKey,
	Banner_L5_S4_HandCuffWrongKey,

	Banner_L7_S1_ClickingOnPuzzleWithoutSolutions,
	Banner_L7_S1_EnetrPuzzleWithSolutions,
	Banner_L7_S1_ClickingOnPuzzleAfterCompletingOnePuzzle,
	Banner_L7_S3_AfterReadNote,
	Banner_L7_S3_ClickingOnDoorWithoutPower,
	Banner_L7_S3_ClickingOnWiredDoorWithoutPower,
	Banner_L7_S3_AfterNightmareVdo,

	Banner_L8_S1_TakeMagicGlass,
	Banner_L8_S2_UsingSwardBeforePuzzle,
	Banner_L8_S2_ClickWitoutSugetcane,
	Banner_L8_S2_ClickAfterSugetCaneGrown,

	eInfo_Max
};

enum EPZHelpText
{					
	//Level 1
	ePZHelp_L1_S1_KidsPainting,
	ePZHelp_L1_S2_Fuse,
	ePZHelp_L2_S3_ShelfPuzzle,
	ePZHelp_L2_S3_ShadowPuzzle,
	ePZHelp_L2_S3_ShadowPuzzleDoorOpened,
	ePZHelp_L1_S4_PaintingPuzzle,
	ePZHelp_L1_S5_DynamitePuzzle,

	//Level 2
	ePZHelp_L2_S2_MangoPuzzle,
	ePZHelp_L2_S3_StoneBeamPuzzle,
	ePZHelp_L2_S4_DietiesPuzzle,
	ePZHelp_L2_S5_UnblockPuzzle,
	ePZHelp_L2_S5_MedicineMaking,
	ePZHelp_L2_S6_TunnelPuzzle,
	ePZHelp_L2_S7_SoldierPuzzle,

	ePZHelp_L3_S3_UnderwaterPuzzle,
	ePZHelp_L3_S1_LionHeadPuzzle,
	ePZHelp_L3_S3_BlockPuzzle,
	ePZHelp_L3_S2_CreeperPuzzle,
	ePZHelp_L3_S2_Match3Puzzle,
	ePZHelp_L3_S2_PipePuzzle,
	ePZHelp_L3_S2_ThreadPuzzle,

	ePZHelp_L4_S2_AlligatorPuzzle,
	ePZHelp_L4_S2_FanPuzzle,
	ePZHelp_L4_S3_DoorPuzzle,
	ePZHelp_L4_S5_SwapPuzzle,

	ePZHelp_L5_S2_BlocksPuzzle,
	ePZHelp_L5_S5_MoonPuzzle,
	ePZHelp_L5_S4_Bridge,
	ePZHelp_L5_S3_DoorPuzzle,
	ePZHelp_L5_S4_HandCuffPuzzle1,
	ePZHelp_L5_S4_HandCuffPuzzle2,

	ePZHelp_L6_S1_StatueChestPuzzle,
	ePZHelp_L6_S2_PineGluePuzzle,
	ePZHelp_L6_S3_PathwayPuzzle,
	ePZHelp_L6_S3_HO_DialPadPuzzle,
	ePZHelp_L6_S4_PortalPuzzle,
	ePZHelp_L6_S5_TabPuzzle,

	ePZHelp_L7_S1_JigsawPuzzle,
	ePZHelp_L7_S1_WellMemoryPuzzle,
	ePZHelp_L7_S2_EyePuzzle,
	ePZHelp_L7_S2_RightHandHOPZ,
	ePZHelp_L7_S2_LeftHandHOPZ,
	ePZHelp_L7_S3_JigsawPuzzle,
	ePZHelp_L7_S3_DoorPuzzle,
	ePZHelp_L7_S4_BhasmamPuzzle1,
	ePZHelp_L7_S4_BhasmamPuzzle2,
	ePZHelp_L7_S4_JigsawPuzzle,	
	
	ePZHelp_L8_S1_HandDoorPuzzle,	
	ePZHelp_L8_S2_GatePuzzle,	
	ePZHelp_L8_S2_GearPuzzle,	
	ePZHelp_L8_S2_SugerCanePuzzlePuzzle,
	ePZHelp_L8_S4_PoolPuzzle,	
	ePZHelp_L8_S5_VirtualPuzzle,	

	ePZHelp_Max
};

extern std::string TextHintRechargeWarning;
extern std::string TextSkipRechargeWarning;
extern std::string	WrongInvDropTexts[MAX_WRONGDROP_TEXTS];
extern std::string	InfoTexts[eInfo_Max];
extern std::string	LvLObjTexts[eObj_Max];
extern std::string	PZHelpTextArray[ePZHelp_Max];
extern std::string	MemoryBannerTexts[eMem_MAX];

class InfoBanner
{
	std::map<EInfoTexts, string> _infoTexts;

public:
	
	static InfoBanner* CreateLevel(int level);
	static string getBanner(EInfoTexts txtID);

private:

	void CreateNewLevel();
	void SetBanners(int level);
	void SetBanners(vector<string> bannersArray, int startID);

	InfoBanner(){_infoTexts.clear();}
	~InfoBanner(){_infoTexts.clear();};
};

#endif
