//===========================================================
// 
// Framework
//===========================================================

#include "Banners.h"
#include "LangDefs.h"
#include "DataEnums.h"


#if defined( ENGLISH_BUILD )
	std::string TextHintRechargeWarning = "Hint_recharging";
	std::string TextSkipRechargeWarning = "Skip_recharging";

	
	std::string MemoryBannerTexts[eMem_MAX] =
	{
		"Explush",//Teddy Bear
		"Exreading",//Story Books
		"Exlong",//Baby Picture
		"Eximitate",//Boxing Gloves
		"Exmess",//Rolling Pin

		"Exremind",//
		"Excat",//
		"Exgetting",//
		"Exdoctor",//
		"Exdad",//

		"Exwear",//
		"Exlived",//
		"Exfelt",//
		"Exfun",//
		"Exrequest",//

		"Exprank",//
		"Exdevise",//
		"Exwont",//
		"Excraft",//
		"Exhuge",//

		"Exfed",//
		"Exlittle",//
		"Exkind",//
		"Exsweet",//
		"Extime",//

		"Exbargain",//
		"Exexperience",//
		"Exmom",//
		"Exwait",//
		"Exprized"//
	};

	std::string WrongInvDropTexts[MAX_WRONGDROP_TEXTS] =
	{
		"Exwork",
		"Excant",
		"Exobject",
		"Exhmm",
		"Exelse",
		"Exfigure",
		"Exthere",
		"Extry",
		"Exitems",
		"Exgoing",
		"Exmabe",
		"Exdont",
		"Exnot",
		"Exkey",
		"Exwould"
	};
#endif
#if defined( ENGLISH_BUILD )

std::string LvLObjTexts[eObj_Max] =
{
	//Level 1
	"L1james",
	"L1pow",
	"L1find",
	"L1escape",
	"L1boat",
	
	//Level 2
	"L2meet",
	"L2prep",
	"L2pearl",
	"L2nether",

	//Level 3
	"L3explore",
	"L3unravel",
	"L3well",//
	"L3castle",//

	//Level 4
	"L4your",
	"L4way",
	"L4find",
	"L4make",

	"L5rid",
	"L5out",
	"L5access",
	"L5way",
	"L5to",

	"L6nether",
	"L6connect",
	"L6man",
	"L6find",

	"L7way",
	"L7sophia",
	"L7find",

	"L8resurrect",
	"L8reach",
	"L8purify"
};

std::string PZHelpTextArray[ePZHelp_Max] =
{
	//Level 1
	"L1blank",//ePZHelp_L1_S1_KidsPainting
	"L1rot",//ePZHelp_L1_S2_Fuse
	"L1seven",//ePZHelp_L2_S3_ShelfPuzzle
	"L1match",//ePZHelp_L2_S3_ShadowPuzzle
	"L1animal",//ePZHelp_L2_S3_ShadowPuzzleDoorOpened
	"L1antique",//ePZHelp_L1_S4_PaintingPuzzle
	"L1dynamite",//ePZHelp_L1_S5_DynamitePuzzle

	//Level 2
	"L2lines",							//ePZHelp_L2_S2_MangoPuzzle
	"L2swap", //ePZHelp_L2_S3_StoneBeamPuzzle
	"L2tiles",							//ePZHelp_L2_S4_DietiesPuzzle
	"L2block",							//ePZHelp_L2_S5_UnblockPuzzle
	"L2per",											//ePZHelp_L2_S5_MedicineMaking
	"L2rotate",	//ePZHelp_L2_S6_TunnelPuzzle
	"L2crush",			//ePZHelp_L2_S7_SoldierPuzzle

	//Level 3
	"L3design",	//ePZHelp_L3_S3_UnderwaterPuzzle
	"L3correct", //ePZHelp_L3_S1_LionHeadPuzzle
	"L3tiles", //ePZHelp_L3_S3_BlockPuzzle
	"L3dangerous", //ePZHelp_L3_S2_CreeperPuzzle
	"L3match",//ePZHelp_L3_S2_Match3Puzzle
	"L3such",
	"L3order",

	//Level 4
	"L4two",
	"L4slide",
	"",//ePZHelp_L4_S3_DoorPuzzle
	"L4swap",//ePZHelp_L4_S5_SwapPuzzle

	//Level 5
	"L5blocks", //ePZHelp_L5_S2_BlocksPuzzle
	"L5rotate", //ePZHelp_L5_S5_MoonPuzzle
	"L5ring",//ePZHelp_L5_S4_Bridge
	"L5colored", //ePZHelp_L5_S3_DoorPuzzle
	"L5click", //ePZHelp_L5_S4_HandCuffPuzzle1
	"L5prongs",//ePZHelp_L5_S4_HandCuffPuzzle2

	//Level 6
	"L6palm", // ePZHelp_L6_S1_StatueChestPuzzle 
	"L6here", //ePZHelp_L6_S2_PineGluePuzzle	
	"L6snake", //ePZHelp_L6_S3_PathwayPuzzle
	"L6select",			//ePZHelp_L6_S3_HO_DialPadPuzzle
	"L6rotate",//ePZHelp_L6_S4_PortalPuzzle//
	"L6carrot", //ePZHelp_L6_S5_TabPuzzle
	
	//Level 7
	"L7restore", //ePZHelp_L7_S1_JigsawPuzzle,
	"L2sequence", //ePZHelp_L7_S1_WellMemoryPuzzle,
	"L7swap", //ePZHelp_L7_S2_EyePuzzle,
	"L7object",  // ePZHelp_L7_S2_RightHandHOPZ,
	"L7animal",//ePZHelp_L7_S2_LeftHandHOPZ,
	"L7restore", //ePZHelp_L7_S3_JigsawPuzzle,
	"L7combine", //ePZHelp_L7_S3_DoorPuzzle,
	"L7complete", //ePZHelp_L7_S4_BhasmamPuzzle1,
	"L7colors", //ePZHelp_L7_S4_BhasmamPuzzle2
	"L7restore", //ePZHelp_L7_S4_JigsawPuzzle,

	//Level 8
	"L8swap",			//ePZHelp_L8_S1_HandDoorPuzzle
	"L8moon",				//ePZHelp_L8_S2_GatePuzzle	
	"L8place",				//ePZHelp_L8_S2_GearPuzzle	
	"L8pair" ,//ePZHelp_L8_S2_SugerCanePuzzlePuzzle
	"L8count",		//ePZHelp_L8_S4_PoolPuzzle
	"L8shape"				//ePZHelp_L8_S5_VirtualPuzzle
};

std::string InfoTexts[eInfo_Max] = 
{
	"Exfuel"//After funnel is placed fuel meter not checked
};
#endif
using namespace std;

#if defined( ENGLISH_BUILD )

static string Level1[] = {""};
static string Level2[] = {""};
static string Level3[] = {"L3disaster",	// Banner_L3_S1_FortEntrence = 300,
						  "L3its",						//Banner_L3_S1_ClickingOnFortInteriorDoor
						  "L3tricks",// Banner_L3_S1_ClickingOnBurningCoil,
						  "L3coal",														// Banner_L3_S1_SpadeOnBurningCoil,
						  "L3theres",									// Banner_L3_S1_AddingWaterOnBurningCoil,
						  "L3one",								// Banner_L3_S1_ClickOnCooledCoil,
						  "L3spear",						// Banner_L3_S1_ClickOnSpearHook,
						  "L3eyes",												// Banner_L3_S1_ClickOnSkullFace,
						  "L3entrance",															// Banner_L3_S1_ClickNavFortWitoutLadder,
						  "L3oops",									// Banner_L3_S1_MazeWrongPathText1,
						  "L3dear",														// Banner_L3_S1_MazeWrongPathText2,
						  "L3wrong",							// Banner_L3_S1_MazeWrongPathText3,
						  "L3paths",							// Banner_L3_S1_M1_WithoutMap,
						  "L3block",					// Banner_L3_S2_M1_ClickCreeper,
						  "L3another",			// Banner_L3_S2_M1_ClickOnWell,
						  "L3bull",			// Banner_L3_S2_M2_EnterFirstTime,
						  "L3check",							//Banner_L3_S2_M2_CreeperPzComplete,
						  "L3creep",														// Banner_L3_S2_M2_ClickCreeper,
						  "L3appear",									// Banner_L3_S2_M2_ClickOnRubble,
						  "L3walked",							// Banner_L3_S2_M3_EnterFirstTime,
						  "L3hammer",						// Banner_L3_S2_M3_AfterHammerHeadCollect,
						  "L3break",			// Banner_L3_S2_M3_ClickOnLockedDoor,
						  "L3cant",					// Banner_L3_S2_M3_ClickOnSand,
						  "L3closely",              //  Banner_L3_S2_M3_ClosingPopupWithoutTakeHammerHead,
						  "L3linked",				// Banner_L3_S2_M4_ClickOnLockedDoor,
						  "L3pipes",			// Banner_L3_S2_M5_ClickOnLockeDoor,
						  "L3hmm",															// Banner_L3_S2_M5_ClickOnPumb,
						  "L3missing",				// Banner_L3_S2_M5_ClickOnPopupWithoutPipe,
						  "L3connected",					//Banner_L3_S2_M5_ClickOnPopupWithPipe
						  "L3still",												//Banner_L3_S2_M5_DropPartualPipes
						  "L3solution",									// Banner_L3_S2_M6_ClickOnSqueBox,
						  "L3now",											//Banner_L3_S2_M6_PillerSolved
						  "L3determined",								// Banner_L3_S2_LasiayAttackComplete,
						  "L3crystals",							// Banner_L3_S2_MazeOnClickCrystal,
						  "L3shells",							 //  Banner_L3_S3_BeforeCounetOuterRemove,
						  "L3half",			 //  Banner_L3_S3_AfterCocunetOuterRemoved,
						  "L3sophia",												//Banner_L3_S3_WhenSawSophiaShoe,
						  "L3sketched",           // Banner_L3_S3_SophiaCutsceneComplete,
						  "L3handle"};													//Banner_L3_S3_HammerHeadMultiInv
static string Level4[] = {"L4cant"};                // Banner_L4_S3_ICantReachIt
static string Level5[] = {"L5huge",									// Banner_L5_S3_AfterCutScene = 500,
						  "L5lock",					    // Banner_L5_S3_ClickOnDungonDoor,
						  "L5shoot",																			    // Banner_L5_S3_HookFallsIntoTheWater,
						  "L5work",								// Banner_L5_S3_ClickAfterHookLost,
						  "L5deep",					    // Banner_L5_S3_NoDivingBell,
						  "L5hurry",									    // Banner_L5_S3_UsingDivingBell,
						  "L5pond",	    // Banner_L5_S3_ClickStoneOnMiddle,
						  "L5alive",							    // Banner_L5_S3_SophiaCutSceneComplete,
						  "L5open",    // Banner_L5_S3_NoOutCreeper,
						  "L5bag",	    // Banner_L5_S3_waterproofBagCollect,
						  "L5dungeon",	//Banner_L5_S4_HandCuffClickWithoutKey,
						  "L5giving"};														    //Banner_L5_S4_HandCuffWrongKey
static string Level6[] = {""};
static string Level7[] = {"L7guide",
						  "L7symbol",
						  "L7here",
						  "L7pure",
						  "L7door",
						  "L7break",
						  "L7no"};
static string Level8[] = {"L8magic",
						  "L8stump",
						  "L8ok",
						  "L8sword"};
#endif

static vector<string> level1Banners(Level1, Level1 + sizeof Level1 / sizeof Level1[0]);
static vector<string> level2Banners(Level2, Level2 + sizeof Level2 / sizeof Level2[0]);
static vector<string> level3Banners(Level3, Level3 + sizeof Level3 / sizeof Level3[0]);
static vector<string> level4Banners(Level4, Level4 + sizeof Level4 / sizeof Level4[0]);
static vector<string> level5Banners(Level5, Level5 + sizeof Level5 / sizeof Level5[0]);
static vector<string> level6Banners(Level6, Level6 + sizeof Level6 / sizeof Level6[0]);
static vector<string> level7Banners(Level7, Level7 + sizeof Level7 / sizeof Level7[0]);
static vector<string> level8Banners(Level8, Level8 + sizeof Level8 / sizeof Level8[0]);

static InfoBanner* _infoBanner = NULL;

void InfoBanner::CreateNewLevel()
{
	if (_infoBanner)
	{
		delete _infoBanner;
		_infoBanner = NULL;
	}

	_infoBanner = new InfoBanner();
}

void InfoBanner::SetBanners(int level)
{
	switch (level)
	{
	case 1:
		break;
	
	case 2:
		break;
		
	case 3:
		SetBanners(level3Banners, Banner_L3_S1_FortEntrence);
		break;
		
	case 4:
		SetBanners(level4Banners, Banner_L4_S3_ICantReachIt);
		break;
		
	case 5:
		SetBanners(level5Banners, Banner_L5_S3_AfterCutScene);
		break;
		
	case 6:
		break;
		
	case 7:
		SetBanners(level7Banners, Banner_L7_S1_ClickingOnPuzzleWithoutSolutions);
		break;
		
	case 8:
		SetBanners(level8Banners, Banner_L8_S1_TakeMagicGlass);
		break;

	default:
		break;
	}
}

void InfoBanner::SetBanners(vector<string> bannersArray, int startID)
{
	for (int i = 0; i < (int)(bannersArray.size()); i++)
	{
		_infoTexts[EInfoTexts(startID + i)] = bannersArray[i];
	}
}

InfoBanner* InfoBanner::CreateLevel(int level)
{
	_infoBanner->CreateNewLevel();
	_infoBanner->SetBanners(level);
	return _infoBanner;
}

std::string InfoBanner::getBanner(EInfoTexts txtID)
{
	return _infoBanner->_infoTexts[txtID];
}

