//====================================
// Framework
// Profile Class
//====================================

#ifndef FRMWRK_PROFILE_DEF
#define FRMWRK_PROFILE_DEF

#include "SaveData.h"
#include "Framework.h"
#include "DataEnums.h"
#include "SConstants.h" 
#include "Banners.h"

#define MAX_HOITEMS		30
#define MAX_TUTORIAL	23


typedef struct PROFILE_INVBOX_OBJ
{
	int  InvId;
	int  nPiecePicked;
	int  MaxPieces;
}PROFILE_INVBOX_OBJ;


enum eGameDiffMode
{
	EMODE_CASUAL,
	EMODE_ADVANCED,
	EMODE_EXPERT,
	EMODE_CUSTOM
};

#define MAX_LVL_OBJECTIVES 32 //eObj_Max check this
#define MAX_CURRENT_OBJECTIVES 6 //Maximum objectives active in single level

#define MAX_CHARACTERS 17 //16+1 - Since the enum Starts from 1

//Level Specific Array Save Vars
#define MAX_L1S4_PUZPIECES 10
#define MAX_L1S4_CLOCK_NUMS 12
#define MAX_L1S5_HOITEMS 9
#define MAX_L1S5_HOP2TOKENS 8

#define MAX_L2S1_HOOBJECTS 12
#define MAX_L2S2_HOOBJECTS 10
#define MAX_L2S6_HOOBJECTS 22

#define MAX_L3S1_HOOBJECTS 12
#define MAX_HUDL3S1_SYMBOLS 7

#define MAX_L1S3_SHELFPUZDIFFS 7
#define MAX_HALL_OF_FAME_AWARDS 41

#define MAX_L2_S5_EAGLEBLOCKS 16
#define MAX_L2_S4_DEITIESTILES 9
#define MAX_L4_S3_SYMBOLS 8
#define MAX_L4_S3_DUMMYSYMBOLS 10

#define MAX_L5S2_NUM_BLOCKS 8
#define MAX_L5S6_HOOBJECTS 14
#define MAX_L5S5_PZBLOCKS 8

#define MAX_L6S1_HOSCENES	6
#define MAX_L6S2_PZOBJECTS 8

#define MAX_L7S1_JIGSAWPIECES 18
#define MAX_L7S3_JIGSAWPIECES 16
#define MAX_L7S4_JIGSAWPIECES 15

#define MAX_L7S2_HANDHOSYMBOLS 12
#define MAX_L7S2_LASYAITEMS 14

#define L8S3_SWORDPIECES 5
#define MAX_L8S3_STRIPS	4
#define MAX_L8_MAGIC_SYMBOLS 3
#define MAX_L8_S1_MATCHING_SYMBOLS_COUNT	10
#define L8_S2_NUM_GEARS 13

class Profile : public SaveData
{
public:
	Profile();
	~Profile();
	void InitGameVars();

	//Profile game Vars - no need to save!!		
	bool	_IsSplashDone;
	bool	_IsFromSplash;
	bool	_IsFromGame;

	bool _IsMatch3Playing;

	int		_iActiveProfileSlot;
	int		_PrevArea;

	//Common Save Variables
	int  CurrentArea;		
	int  CurrentLevel;

	//Variables to save previous areas
	int CreditsPrevArea, ExtrasRoomPrevArea, MemoryRoomPrevArea;
	bool	IsKidsRoomFromMainMenu;
	bool	IsKidsRoomFromExtras;
	bool	IsExtrasRoomFromMainMenu;
	bool	IsAwardsRoomFromExtrasScene;
	bool	IsCreditsFromMainMenu;


	//Area where combo inv is Active
	int	 PrevComboActiveArea;
	int  PrevMapActiveArea;

	int  MusicVol, SFXVol;
	int  EnvirVol, VoiceVol;

	bool IsLowSystem;
	bool IsCheckedForLowSystem;
	bool IsWideScreen;
	bool IsHOScenesSkippable;
	bool IsMuted;
	bool IsStartMusic;
	bool IsHidingTips;
	bool IsAwardPopupsEnabled;
	bool IsInvLocked;
	bool IsExpertMode;
	bool IsHideSparkles;
	bool IsHideHOSparkles;
	bool IsTutorialDisabled;
	bool IsHintDisabled;
	bool IsSkipDisabled;
	float	 CustomHintTimer;
	float	 CustomSkipTimer;

	bool IsStartCutscenePlayed;
	bool IsHaveYouCutscenePlayed;
	bool IsEndCutscenePlayed;		
	bool IsPromoCutscenePlayed;	
	bool IsCrystalOrbPicked;

	//Extras Pages
	int ExtrasCurrentPageId;
	int ExtrasCurrentPageIndex;


	//User Journal Notes
	bool IsJObjBtnActive;

	int JrnPageLastUnlocked;
	int JrnPageMaxRead;
	int JrnPageLastRead;

	bool IsJrnNotesFound[MAX_JOURNAL_NOTES];
	int	 JrnNotesCollected[MAX_JOURNAL_NOTES];
	int	 NumJrnNotesCollected;

	bool isCharacterUnlocked[MAX_CHARACTERS];
	int  lastUnlockedCharacter;
	int  lastViewedCharacter;

	int currentNumOfEvents[MAX_HALL_OF_FAME_AWARDS];


	bool isHintUsedAfterHOStart[eArea_Max];
	bool isMiniGameCompleted[eArea_Max];
	bool isHOCompleted[eArea_Max];
	bool isSkipUsedAfterMiniGameStart[eArea_Max];
	bool isHOCompleteWithMatch3[eArea_Max];
	bool isMiniGameSkipedAfterGameStart;
	bool isJigsawPuzzleSkipped;
	bool isCutSceneSkipedAfterGameStart;
	bool isHintUsedAfterGameStart;
	bool isHOHintUsedAfterGameStart;
	bool isHintUsedAfterCreeksMansionStart;

	//User Objectives
	bool IsLVLObjectiveCompleted[MAX_LVL_OBJECTIVES];
	bool IsLVLObjectiveAdded[MAX_LVL_OBJECTIVES];
	int	 CurrentLvLObjectives[MAX_CURRENT_OBJECTIVES];
	int	 NumObjectivesActive;

	//User Inventory Details
	bool  Inventory[eInv_Max];
	PROFILE_INVBOX_OBJ InvObjects[MAX_INVBOX_OBJECTS];


	int   InvObjectCnt;
	int   iInvBoxStartObj;

	int   HintSavedTime;
	int   SkipTimer[eArea_Max];

	bool TutorialSpecialFeaturesOnly;
	int TutorialState;
	bool IsTutorialShown[MAX_TUTORIAL];

	//Any task left in an area
	bool    IsTaskLeft[eArea_Max];
	//Unlocked areas only available in map
	bool    IsAreaLocked[eArea_Max];
	//Area visted will reveal in the map
	bool    IsAreaVisited[eArea_Max];

	bool    L4S1_Visited;
	bool    L4S2_Visited;
	bool    L4S3_Visited;
	bool    L4S4_Visited;
	bool    L4S5_Visited;

	bool	IsMemoryCollected[eMem_MAX];
	int		NumMemoriesCollected;
	int		LastMemoryCollected;

	bool	IsAchievementUnlocked[eAward_MAX];
	bool	IsAchievementFixed[eAward_MAX]; //To Show animation for first time in Awards Page
	int		NumAchievementsCollected;
	int		ElapsedGamePlayTime;
	int		AreaStartTime[eArea_Max];
	int		IsPlayingGameOnExpertMode;

	bool	IsPaptusL1S2Collected;
	bool	IsPaptusL1S3Collected;
	bool	IsPaptusL2S6Collected;
	bool	IsPaptusL3S5Collected;
	bool	IsPaptusL4S3Collected;
	bool	IsPaptusL5S5Collected;
	bool	IsPaptusL6S1Collected;
	bool	IsPaptusL6S1popCollected;
	bool	IsPaptusL7S3Collected;

	bool	IsMorphObjL1S1Collected;
	bool	IsMorphObjL1S2Collected;
	bool	IsMorphObjL1S3Collected;
	bool	IsMorphObjL1S7Collected;

	bool	IsMorphObjL2S1Collected;
	bool	IsMorphObjL2S2Collected;
	bool	IsMorphObjL2S3Collected;
	bool	IsMorphObjL2S4Collected;
	bool	IsMorphObjL2S5Collected; //No Longer used
	bool	IsMorphObjL2S6Collected; //No Longer used

	bool	IsMorphObjL3S2Collected;
	bool	IsMorphObjL3S3Collected;
	bool	IsMorphObjL3S1Collected;
	bool	IsMorphObjL3S4Collected;

	bool	IsMorphObjL4S1Collected;
	bool	IsMorphObjL4S3Collected;
	bool	IsMorphObjL4S4Collected;
	bool	IsMorphObjL4S5Collected;

	bool	IsMorphObjL5S2Collected;
	bool	IsMorphObjL5S4Collected;
	bool	IsMorphObjL5S5Collected;
	bool	IsMorphObjL5S6Collected;

	bool	IsMorphObjL6S1Collected;
	bool	IsMorphObjL6S2Collected;
	bool	IsMorphObjL6S3Collected;
	bool	IsMorphObjL6S4Collected;

	bool	IsMorphObjL7S1Collected;
	bool	IsMorphObjL7S2Collected;
	bool	IsMorphObjL7S3Collected;
	bool	IsMorphObjL7S4Collected;

	int		NumMorphObjectsCollected;

	//Level 1 - Mansion
	bool	L1_IsMansionPowerOn;
	bool	L1_IsDemoOutroVideoPlayed;

	//Scene_1_Beach
	bool L1S1_IsTalkedWithRaja;
	bool L1S1_IsRajaTakenJames;

	bool L1S1_IsInvBagTaken;
	bool L1S1_IsInvKnifeTaken;
	bool L1S1_IsInvLetterCreekTaken;
	bool L1S1_IsInvPlank3Taken;

	bool L1S1_LocketPartsPlaced;
	bool L1S1_LocketPart1Fixed;
	bool L1S1_LocketPart2Fixed;
	bool L1S1_LocketPart3Fixed;
	bool L1S1_LocketPart4Fixed;
	bool L1S1_LocketAllPartsFixed;
	bool L1S1_BagOpened;
	bool L1S1_KidsPaintingPuzzleActive;
	bool L1S1_KidsPaintingAddedToJournal;
	bool L1S1_KidsPaintingPzBagActive;
	int L1S1_ComboBagInitX;
	int L1S1_ComboBagInitY;
	bool L1S1_BagZipOpened;

	bool L1S1_P1Branch1Cut;
	bool L1S1_P1Branch2Cut;
	bool L1S1_P1Branch3Cut;
	bool L1S1_P1Leaf1Removed;
	bool L1S1_P1Leaf2Removed;
	bool L1S1_P1Leaf3Removed;
	bool L1S1_P1Ruby1Branch2Taken;
	bool L1S1_P1Ruby2Branch3Taken;
	bool L1S1_P1Ruby3UnderLeaf1Taken;
	bool L1S1_P1Ruby4UnderLeavesTaken;

	bool L1S1_P2BuffaloClicked;
	bool L1S1_P2CastleClicked;
	bool L1S1_P2MeClicked;
	bool L1S1_P2MamaClicked;
	bool L1S1_P2KingClicked;
	bool L1S1_P2IsPuzzleComplete;

	//Scene_2_Frontyard
	bool    L1S2_IsMonkeyTookBanana;
	bool    L1S2_IsCarDickeyOpen;
	bool    L1S2_P1_IsSackMoved;
	bool    L1S2_P1_IsWoodMoved;
	bool    L1S2_P3IsSwingRopeCut;
	bool    L1S2_P3IsTreeBranchFixed;
	bool	L1S2_IsInvCloth1SwingTaken;
	bool	L1S2_IsInvCloth2LovePopTaken;
	bool	L1S2_IsInvCloth3CarDickyTaken;
	bool    L1S2_InvDieselCanTaken;
	bool    L1S2_InvHoseTaken;
	bool	L1S2_IsInvPlank1Taken;
	bool	L1S2_IsInvTreeBranchTaken;

	bool	L1S2_IsFusePuzzleSolved;
	bool	L1S2_IsFusePuzzleShuffled;
	bool	L1S2_IsP1FusePuzzleActive;
	bool	L1S2_IsP1LeftFuseFixed;
	bool	L1S2_IsP1RightFuseFixed;
	bool	L1S3_IsP1LeftFuseTaken;
	bool	L1S2_IsP1RightFuseTaken;
	bool	L1S2_IsP1GreenWireDownFixed;
	bool	L1S2_IsP1GreenWireTopFixed;

	int		L1S2_IsP1RotFuseRot[4];
	int		L1S2_IsP1WireConnectId[4];
	bool	L1S2_IsP1WireFuseFixed[4];
	bool	L1S2_IsP1WireFuseFixEmpty[4];

	//Scene_3_Foyer
	bool	L1S4_P2IsImagesFound[5];
	bool	L1S4_P2IsHandFixed[5];
	bool	L1S4_P2IsKeySetPlaced;
	bool	L1S4_P2IsMissingHandsPlaced;
	bool	L1S4_P2IsShadowPuzHandDoorOpened;
	bool	L1S3_IsShadowPuzzleSolved;
	bool	L1S4_IsShadowPuzFinalKeyTurned;
	bool    L1S3_IsBedroomDoorOpened;

	bool	L1S3_IsObj1Moved;
	bool	L1S3_IsObj2Moved;
	bool	L1S3_IsCatAttacked;
	bool	L1S3_IsBananaTaken;
	bool	L1S3_MatchBoxTaken;
	bool	L1S3_NagaKeyTaken;
	bool	L1S3_RajaTalkOver;
	bool	L1S3_RajaAskedForCreekLetter;
	bool	L1S3_RajaTakenJames;

	bool	L1S3_PZMoopanFlyerTaken;
	bool	L1S3_IsInvHOLocketTaken;
	bool	L1S3_IsInvShadowHand1Taken;
	bool	L1S3_IsP3ShadowHandFallen;
	bool	L1S3_IsMemoryDollTaken;
	bool	L1S3_PZShelfPuzzleSolved;
	bool	L1S3_PZShelfDifferFound[MAX_L1S3_SHELFPUZDIFFS];

	//Door HO
	bool	L1S3_IsDoorHOLocketPlaced;
	bool	L1S3_IsDoorHOLocketDoorTaken;
	bool	L1S3_IsDoorHOListPanelTaken;
	bool	L1S3_DoorHOSolved;
	bool	L1S3_KeyDoorOpened;
	bool	L1S3_KeyFixed;
	bool	L1S3_HOStarDoorOpened;
	bool	L1S3_HORoseFlowerFound;
	bool	L1S3_HOCrescentFound;
	bool	L1S3_HOSnakeFound;
	bool	L1S3_HOMaskFound;
	bool	L1S3_HOOwlFound;
	bool	L1S3_HOLeavesFound[3];
	bool	L1S3_HOLionHeadFound;
	bool	L1S3_HOFishFound;
	bool	L1S4_HOFishTailFixed;
	bool	L1S3_HOStarsFound[2];
	bool	L1S3_HOTreeFound;
	bool	L1S3_HOGoddessFound;
	bool	L1S3_HOTurtleFound;
	bool	L1S3_HOSwapPuzzleSolved;
	bool	L1S3_HOMemoryPuzzleSolved;
	bool	L1S3_HOLockOpened[3];
	float	L1S3_HOSwapLeverAngle[4];
	bool	L1S3_HOSolved;
	bool	L1S3_HOCompletedWithMatch3;

	//Scene_4_Bedroom
	bool    L1S4_PaintingPuzzleSolved;
	bool    L1S4_InvPaintingFrameTaken;
	bool	L1S4_IsInvShadowHand2Taken;

	//Scene_5_TreasureRoom
	bool    L1S5_InvBoatKeyTaken;
	bool    L1S5_InvHammerHeadTaken;
	bool    L1S5_InvMortarPestleTaken;
	bool    L1S5_IsTreasureFireLeftLit;
	bool    L1S5_IsTreasureFireCenterLit;
	bool    L1S5_IsTreasureFireRightLit;
	bool    L1S5_IsHammerHandFixed;
	bool    L1S5_IsFireAttackVideoPlayed;
	bool	L1S5_IsFireAttckBanner;
	//Scene_5_HOTreasure
	int		L1S5_HOItemStates[MAX_L1S5_HOITEMS];
	int		L1S5_HOHUDItemStates[MAX_HOITEMS];
	bool	L1S5_HOP2PuzzleShuffled;
	bool	L1S5_HOP2PuzzleSolved;
	int		L1S5_HOP2TokenPosContains[MAX_L1S5_HOP2TOKENS];

	bool	L1S5_HOClothFolded;
	bool	L1S5_HOBookPart2Fixed;
	bool	L1S5_HOBookPart3Fixed;
	bool	L1S5_HOBookOpened;
	bool	L1S5_HOHammerMoved;
	bool	L1S5_HOChestBoxOpened;
	bool	L1S5_HOSwapPuzzleSolved;
	bool	L1S5_HOSolved;
	bool	L1S5_HOCompletedWithMatch3;

	//Scene_5_TunnelDoor
	bool	L1S5_P1IsNailPlucked[3];
	bool	L1S5_P1IsMapMovedFromPlate;
	bool	L1S5_P1IsBoxOpened;
	bool	L1S5_P1MortarPestlePlaced;
	bool	L1S5_P1IsCrystalAddedToGrinder;
	bool	L1S5_P1IsCrystalPowderCreated;
	bool	L1S5_P1IsCrystalPowderAddedToPlate;
	bool	L1S5_P1IsBottleCapRemoved;
	bool	L1S5_P1AmoniAddedToPlate;
	bool	L1S5_P1GreenPowderAddedToBottleHalf;
	bool	L1S5_P1GreenPowderAddedToBottleTreeForth;
	bool	L1S5_P1GreenPowderAddedToBottleFull;
	bool	L1S5_P1ThreadAddedToBottle;
	bool	L1S5_P1WaxCylinderAddedToBottle;
	bool	L1S5_IsInvBombTaken;
	bool	L1S5_IsInvHammerHandTaken;
	bool	L1S5_IsInvHammerPlacedOnScene;
	int		L1S5_SelectedNailIndex;
	bool    L1S5_POPBombFixed;
	bool    L1S5_POPBombLite;

	bool	L1S5_IsTorchLight;
	bool	L1S5_IsTunnelDoorExploded;
	bool	L1S5_IsEscapedFromTunnel;
	bool	L1S5_IsEscapedFromTunnelBanner;

	bool	L1S4_IsCreekIntroPlayed;
	bool	L1S4_IsCreekDialogPlayed;
	bool	L1S4_IsCreekOutroPlayed;

	bool    L1S4_InvKeySetTaken;
	bool    L1S4_IsBooksOnShelfMoved;
	bool    L1S4_IsBookCodePlaced;
	bool    L1S4_IsBeetleBookTakenFromShelf;
	bool    L1S4_IsBeetlePlacedOnBook;
	bool    L1S4_IsBeetleBookOpened;

	bool    L1S4_IsDrawerSnakeKeyFixed;
	bool    L1S4_IsDrawerOpened;
	bool	L1S4_IsKeyFallenDown;
	bool    L1S4_InvDrawerDiaryTaken;
	bool    L1S4_IsDrawerWaxMelted;
	bool    L1S4_IsDrawerScrewsTakenout;
	bool    L1S4_IsDrawerSecretDoorOpened;
	bool    L1S4_InvBeetleHeadTaken;
	bool    L1S4_IsBeetleTailFixed;

	bool    L1S4_IsPhotoTurned;
	bool    L1S4_IsPhotoPaperTeared;
	bool    L1S4_InvBeetleTailTaken;
	bool    L1S4_InvBookCodeTaken;

	bool    L1S4_IsDiaryPageTurned;
	bool    L1S4_IsDiaryItem1Removed;
	bool    L1S4_IsDiaryItem2Removed;
	bool    L1S4_IsDiaryItem3Removed;
	bool    L1S4_IsDiaryItem4Removed;
	bool    L1S4_IsDiaryItem5Removed;
	bool    L1S4_IsDiaryCreekNoticeTaken;
	bool    L1S4_InvCoinTaken;

	bool    L1S4_IsSecretTreasureDoorOpened;
	bool	L1S4_MayaPushMaryVideoPlayed;

	//Scene_6_BoatShed_HO

	bool    L1S6_HOPaperCutScenePlayed;
	bool    L1S6_HOSquirrelMoved;
	bool    L1S6_HOBeettleMoved;
	bool    L1S6_HOBigBoxRustRemoved;
	bool    L1S6_HOSmallBoxRustRemoved;
	bool    L1S6_HOPaperClipTaken;
	bool    L1S6_HOBigBoxOpened;
	bool	L1S6_HOPaperClipUsed;
	bool    L1S6_HOSmallBoxOpened;
	bool    L1S6_HOFunnelPiecesFixed;
	bool    L1S6_HOTapeAppliedOnFunnel;
	bool    L1S6_HOTapeCutOnFunnel;
	bool    L1S6_HOInvOilDripperTaken;
	bool    L1S6_HOInvFunnelTaken;
	bool	L1S6_HOLeftMiniDoorOpened;
	bool	L1S6_HOIsToolMoved[3];
	bool	L1S6_HOIsToolInLeftMiniBoxMoved[3];

	bool	L1S6_HOSolved;
	bool	L1S6_HOCompletedWithMatch3;

	int L1S6_HOInvItemStates[MAX_HOITEMS];


	//Scene_6_BoatShed
	bool    L1S6_IsPumperPlaced;
	bool	L1S6_IsRoomLite;
	bool	L1S6_IsWebCleaned;
	bool	L1S6_IsHosePlaced;
	bool	L1S6_IsBoxDroped;
	bool	L1S6_IsCanPlaced;
	bool	L1S6_IsCanFilled;
	bool	L1S6_IsInvCrowbarPicked;
	bool	L1S6_IsInvCanFullPicked;
	bool	L1S6_IsBoxOpened;
	bool	L1S6_IsInvOilDipperTaken;

	//Scene_7_BoatDock
	bool	L1S7_IsInvPlank2Taken;
	bool	L1S7_IsStepsPlaced;
	bool	L1S7_IsRopeCut;
	bool	L1S7_IsDieselFilled;
	bool	L1S7_IsBoatKeyPlaced;
	bool	L1S7_IsCapOpen;
	bool	L1S7_IsFunnelPlaced;
	bool	L1S7_IsFunnelRemoved;
	bool	L1S7_IsCapClosed;
	bool	L1S7_IsBoatStarted;

	// Level 2
	bool L2_S1_IsIntroSwimVideoPlayed;
	bool L2_S1_InvHoneyCombPicked;
	bool L2_S1_InvAloveraTaken;
	bool L2_S1_InvPotWithWaterTaken;
	bool L2_S1_InvScrapperTaken;
	bool L2_S1_InvStoneBallTaken;
	bool L2_S1_IsAloveraSliced;
	bool L2_S1_IsGlovesUsed;
	bool L2_S1_HoneyCombSmoked;
	bool L2_S1_HoneyCombSliced;
	bool L2_S1_IsNagaSculpturePlaced;
	bool L2_S1_IsNagaSculptKeyTurnedOnce;
	bool L2_S1_IsUndergroundHatchOpened;
	bool L2_S1_IsUndergroundPopupInactive;

	//underground HO
	bool L2_S1_DuckFixed;
	bool L2_S1_DuckOpened;
	bool L2_S1_Box1Fixed;
	bool L2_S1_Box2Fixed;
	bool L2_S1_Box3Fixed;
	bool L2_S1_BoxOpened;
	bool L2_S1_SpearUsed;
	bool L2_S1_StarFixed;
	bool L2_S1_PentagonFixed;
	bool L2_S1_StarKeyTurned;
	bool L2_S1_PentagonKeyTurned;
	bool L2_S1_PayalRemoved;
	bool L2_S1_RockMoved;
	bool L2_S1_ScrewFixed;
	bool L2_S1_ScrewTightened;
	bool L2_S1_KeyDoorLeverFixed;
	bool L2_S1_SmallBoxDoorOpened;
	bool L2_S1_KeyFixed;
	bool L2_S1_KeyTurned;
	bool L2_S1_LeftDoorOpened;
	bool L2_S1_RightDoorOpened;
	bool L2_S1_MirrorRemoved;
	bool L2_S1_InvColorPowderTaken;
	bool L2_S1_HOItemsUsed[MAX_L2S1_HOOBJECTS];

	int	 L2S1_HOItemStates[MAX_L2S1_HOOBJECTS];
	int	 L2S1_HOHUDItemStates[MAX_HOITEMS];

	int L2_S1_BoxSelectedIndex[3];

	bool L2_S2_EmberPlaced;
	bool L2_S2_InvFrankinscenceTaken;
	bool L2_S2_InvMangoesTaken;
	bool L2_S2_PZMango1Taken;
	bool L2_S2_PZMango2Taken;
	bool L2_S2_PZMango3Taken;
	bool L2_S2_PZStonePlaced;
	bool L2_S2_PZIsMiss;

	bool L2S2_HOItemsFound[MAX_L2S2_HOOBJECTS];
	bool L2S2_HONagaTempleVisitedOnce;
	bool L2S2_HONagaTempleCompleted;
	bool L2S2_HONagaCompletedWithMatch3;

	bool L2_S2_ColorPlatePlaced;
	bool L2_S2_PoojaInstrPlaced;
	bool L2_S2_NagaLampLite;
	bool L2_S2_NagaLampLiteVideoPlayed;
	bool L2_S2_CutSceneVideoPlayed;
	bool L2_S2_NagaPearl1Taken;
	bool L2_S2_NagaPearl2Taken;
	bool L2_S2_NagaPearl3Taken;
	bool L2_S2_NagaPearl4Taken;
	bool L2_S2_NagaLeaf1Taken;
	bool L2_S2_NagaLeaf2Taken;
	int L2_S2_pop3_R1Color;
	int L2_S2_pop3_R2Color;
	int L2_S2_pop3_R3Color;
	int L2_S2_pop3_R4Color;
	int L2_S2_pop3_R5Color;
	int L2_S2_pop3_R6Color;
	int L2_S2_pop3_R7Color;
	int L2_S2_pop3_R8Color;
	int L2_S2_pop3_R9Color;
	int L2_S2_pop3_R10Color;
	int L2_S2_pop3_R11Color;
	int L2_S2_pop3_R12Color;
	int L2_S2_pop3_R13Color;
	int L2_S2_pop3_R14Color;
	bool L2_S2_kolamComplete;

	bool L2_S3_Pop1_FirstDigPlayed;
	bool L2_S3_Pop1_SecondDigPlayed;
	bool L2_S3_Pop1_PluckingTurmericPlayed;
	bool L2_S3_Pop1_FrogJumpPlayed;
	bool L2_S3_InvTurmericPicked;
	bool L2_S3_InvPoojaLampPicked;

	bool L2_S3_isStoneBeamPuzzleDataSaved;
	int  L2_S3_PZStoneBeamCircleRotation[10];
	float  L2_S3_PZStoneBeamCircleXPos[10];
	float  L2_S3_PZStoneBeamCircleYPos[10];
	int	 L2_S3_PZStoneBeamCircleCurrentPos[10];
	int	 L2_S3_PZStoneBeamCircleOriginalPos[10];
	bool L2_S3_StonePathActivated;
	bool L2_S3_IsStonePathActivatedMessageShown;
	bool L2_S2_InvNagaPearlsCollected;

	bool L2_S4_IsWoodsCut1;
	bool L2_S4_IsWoodsCut2;
	bool L2_S4_InvFirewoodTaken;
	bool L2_S4_InvAxeBluntTaken;
	bool L2_S4_IsAxeSharpened;
	bool L2_S4_InvAxeSharpenTaken;
	bool L2_S4_InvFireTorchTaken;
	bool L2_S4_InvFireTorchStickOiledTaken;
	bool L2_S4_IsLasyaAttackVideoPlayed;
	bool L2_S4_FireTorchOiled;		
	bool L2_S4_RockSlabMoved1;
	bool L2_S4_BrickPuzzleAreaClicked;
	bool L2_S4_RockSlabRemoved;
	bool L2_S4_InvFireTorchFlamed;
	bool L2_S4_InvRakeTaken;

	int	 L2_S4_DeitiesPuzzleState;
	bool L2_S4_InvMetalLeverTaken;
	int  L2_S4_DeitiesTileIndices[MAX_L2_S4_DEITIESTILES];

	//Moopans Den
	bool L2_S5_IsVisited;
	bool L2_S5_InvKnifeTaken;
	bool L2_S5_InvAmberTaken;
	bool L2_S5_InvPotTaken;
	bool L2_S5_InvNagaSculptureTaken;
	bool L2_S5_InvStoneSharpnerTaken;
	bool L2_S5_InvVinegarTaken;
	bool L2_S5_InvMedicineTaken;

	//Medicine Making Puzzle	
	bool L2_S5_VinegarPlaced;
	bool L2_S5_VinegarCorkOpened;
	bool L2_S5_KnifeSharpened;
	bool L2_S5_KnifeSharpTaken;	
	bool L2_S5_MangoSliced;
	bool L2_S5_MangoPlaced;
	bool L2_S5_MangoAddedToPot;
	bool L2_S5_HoneyWaxPlaced;
	bool L2_S5_HoneyWaxSliced;
	bool L2_S5_HoneyWaxAddedToPot;
	bool L2_S5_MangoHoneyLiquidCreated;
	bool L2_S5_MangoHoneyLiquidApplied;
	bool L2_S5_AloveraPlaced;
	bool L2_S5_AloveraSliced;
	bool L2_S5_AloveraApplied;
	bool L2_S5_WoodPlaced;
	bool L2_S5_WoodFired;
	bool L2_S5_PuzzleModeActive;
	bool L2_S5_PotWithWaterPlaced;
	bool L2_S5_TurmericPlaced;
	bool L2_S5_TurmericSliced;
	bool L2_S5_TurmericAddedToPot;
	bool L2_S5_TurmericPowderCreated;
	bool L2_S5_TurmericPowderApplied;
	bool L2_S5_MedicineCreated;
	bool L2_S5_MedicineMixed;
	bool L2_S5_MedicineGivenToMooppan;
	bool L2_S5_ClickedOnMooppan;
	bool L2_S5_ShelfPuzzleSolved;
	int L2_S5_EagleBlockPuzzleState;
	int  L2_S5_EagleBlockIndices[MAX_L2_S5_EAGLEBLOCKS];
	bool L2_S5_InvPujaChartTaken;
	bool L2_S5_StrawBasketRemoved;


	bool L2S6_InvGlovesTaken;
	bool L2S6_InvRustedKnifeTaken;
	bool l2S6_Inv3XStonesTaken;
	bool L2S6_Pop1Visited;
	bool L2S6_HOWaterFallCompleted;
	bool L2S6_HOWaterFallCompletedWithMatch3;
	bool L2S6_HOItemsFound[MAX_L2S6_HOOBJECTS];

	bool L2S6_InvSmallStone1Taken;
	bool L2S6_InvSmallStone2Taken;
	bool L2S6_InvSmallStone3Taken;
	bool L2_S6_IsWoodCutDown;
	bool L2_S6_IsP2LocketOpen;
	bool L2_S6_IsP3HolderOpen;
	bool L2_S6_IsP3PaperPuzClueTaken;
	bool L2_S6_IsP6OilLampLightened;
	//3 saved number Ids of holder
	int  L2_S6_P3CenterNums[3];
	bool L2_S6_NagaPearlFixed;
	bool L2_S6_Shutter1Solved;
	bool L2_S6_Shutter2Solved;
	bool L2_S6_Shutter3Solved;
	bool L2_S6_ClockPuzzleSolved;
	bool L2_S6_PaperPuzCluePlaced;
	bool L2_S6_Rock1Moved;
	bool L2_S6_Rock2Moved;
	bool L2_S6_LeverUsed;
	bool L2S6_InvStoneBallPlaced;

	bool L2_S7_Puzzle1Solved;
	bool L2_S7_Puzzle2Solved;
	bool L2_S7_Puzzle3Solved;
	bool L2_S7_Puzzle4Solved;
	bool L2_S7_Puzzle5Solved;
	bool L2_S7_Puzzle6Solved;
	bool L2_S7_Puzzle7Solved;

	int L2_S7_SoldierPuzzleState;
	int L2_S7_CurrentPuzzle;

	//Level 3
	bool L3_S1_Visited;
	bool L3_S1_Objectives[12];
	bool L3_S1_InvFlagFullTaken;
	bool L3_S1_InvFlagStickTaken;
	bool L3_S1_InvFlagClothTaken;
	int L3_S1_EmberPopupState;
	bool L3_S1_InvLadderStep1Taken;
	bool L3_S1_InvGreenCrystalTaken;
	bool L3_S1_LadderPiecesFixed;
	bool L3_S1_LadderBrokenStepFixed;

	bool L3_S1_HOObjFound[MAX_L3S1_HOOBJECTS];
	int	 L3_S1_HOHudObjID[MAX_HUDL3S1_SYMBOLS];
	int  L3_S1_HONextHudSymbolID;
	bool L3_S1_HOHudSymbolsInitialized;
	bool L3_S1_HOLadderCompleted;
	bool L3_S1_HOLadderCompletedWithMatch3;
	bool L3_S1_InvBrokenLadderTaken;
	bool L3_S1_InvLadderFixedTaken;

	bool L3_S1_P1LionHeadPuzzleCompleted;
	float L3_S1_P1LionHeadRingAngles[4];
	bool L3_S1_invGreenEyeCollected;
	bool L3_S2_InvCocoShellPowderFilledTaken;
	bool L3_S2_InvTwineTaken;

	int L3_S2_MazeCurrentRoom;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
	bool L3_S2_MazeMapUsed;

	bool L3_S2_InvSickleTaken;
	bool L3_S2_RockMoved;
	bool L3_S2_TwineReleased;
	bool L3_S2_Thread1Taken;
	bool L3_S2_Thread2Taken;
	bool L3_S2_Thread3Taken;
	bool L3_S2_Thread4Taken;


	bool L3_S2_InvCoconutOuterRemoved;
	bool L3_S2_InvCoconutShellTaken;
	bool L3_S2_CoconutComboTwineFixed;
	bool L3_S2_InvCoconutWithTwineTaken;
	bool L3_S2_CrowFlewAway;
	bool L3_S2_PoolDoorOpened;

	bool L3_S2_InvHammerTaken;
	bool L3_S2_InvHammerHandTaken;
	bool L3_S2_ToothFixed;
	bool L3_S2_BackDoorOpened;

	bool L3_S2_InvCoconnutTaken;
	bool L3_S2_LeafMoved;
	bool L3_S2_Stick1Moved;
	bool L3_S2_Stick2Moved;

	bool L3_S2_BrickTaken;
	bool L3_S2_CocoShellPowderFilledFixed;
	bool L3_S2_TwineGunPowderFixed;
	bool L3_S2_BrickFixed;
	bool L3_S2_WallBrocken;

	bool L3_S2_vineRemovedR1Status[3];
	bool L3_S2_InvPipe1R1Taken;

	bool L3_S2_vineRemovedR2Status[3];
	bool L3_S2_R2RemovedMudOnPipe[2];
	bool L3_S2_InvPipe2R2Taken;
	bool L3_S2_InvToothR2Taken;
	int L3_S2_P1PlayerTile;
	int L3_S2_P1CurrentTiles[4];

	bool L3_S2_R3DoorOpen[3];
	bool L3_S2_InvPipe3R3Taken;
	bool L3_S2_InvHammerHeadTaken;
	bool L3_S2_R3StonesRemoved;

	bool L3_S2_M4DoorsOpened;
	bool L3_S2_InvPipe4R4Taken;
	bool L3_S2_M4SliderLeftRemoved[8];
	bool L3_S2_M4SliderRightRemoved[8];

	#define MAX_PIPE_INDEX 8
	bool L3_S2_M5_PipePuzzleActivated;
	int L3_S2_M5_PipeGridIndex[MAX_PIPE_INDEX];
	int L3_S2_M5_PipeAngles[MAX_PIPE_INDEX];
	bool L3_S2_M5_PipePuzzleCompleted;
	bool L3_S2_M5_PipeSceneDoorsOpend;

	int  L3_S2_M6_SceneState;
	bool L3_S2_M6_PillerPuzzleComplete;
	int  L3_S2_M6_PillerPositions[3];
	int  L3_S2_M6_CollectedBoxIndex[2];

	bool L3_S3_VisitingPoolForFirsttime;
	bool L3_S3_IsDoorToS4Opened;
	bool L3_S3_PZBlocksPlaced;
	bool L3_S3_PZUnderwaterPuzzleSolved;
	bool L3_S3_PZGoingUnderwaterFirstTime;
	bool L3_S3_IsTempleRisenFromWater;
	bool L3_S3_InvLadderStep2Taken;
	bool L3_S3_InvCupFilledTaken;
	bool L3_S3_InvCupTaken;
	bool L3_S3_IsTempleGlassBroken;
	bool L3_S3_InvTempleBoxTaken;
	bool L3_S3_InvSnakeDoorPartTaken;
	bool L3_S3_InvSawTaken;
	bool L3_S3_InvShowelTaken;
	bool L3_S3_IsRockMovedSaw1;
	bool L3_S3_IsRockMovedSaw2;
	bool L3_S3_IsRootLeftSawed;
	bool L3_S3_IsRootRightSawed;
	bool L3_S3_IsSecretBoxOpened;

	bool L3_S3_PZPinkOuterKeyState[8];
	bool L3_S3_PZPinkDiagonalKeyState[8];
	bool L3_S3_PZPinkInnerKeyState[4];
	bool L3_S3_PZRedOuterKeyState[8];
	bool L3_S3_PZRedDiagonalKeyState[8];
	bool L3_S3_PZRedInnerKeyState[4];
	bool L3_S3_PZBlueOuterKeyState[8];
	bool L3_S3_PZBlueDiagonalKeyState[8];
	bool L3_S3_PZBlueInnerKeyState[4];
	int L3_S3_PZBlockTileIndex[8];
	int L3_S3_PZBlockState;

	bool L3_S4_HOFortFountainFound[16];
	bool L3_S4_HOFortFountainCompleted;

	bool L3_S4_IsVisitedOnce;
	bool L3_S4_InvSharpRodTaken;
	bool L3_S4_InvBambooStickTaken;
	bool L3_S4_InvMetalSheetBlankTaken;
	bool L3_S4_InvMapDrawnSheetTaken;
	bool L3_S4_IsLeaf1OverMetalRemoved;
	bool L3_S4_IsLeaf2OverMetalRemoved;
	bool L3_S4_IsAmeehaDiaryNoteTaken;
	bool L3_S4_IsAmeehaCutscenePlayed;
	bool L3_S4_IsNagaLockPlaced;
	bool L3_S4_IsCliffDoorOpened;
	bool L3_S4_IsPop2AMapDoorOpened;
	bool L3_S4_IsPop2ABlankSheetPlaced;
	bool L3_S4_IsPop2AStatueMoved;
	bool L3_S4_IsPop2ASnakeButtonPressed;
	bool L3_S4_IsPop2BLeaf1Removed;
	bool L3_S4_IsPop2BLeaf2Removed;
	bool L3_S4_IsPop2BStoneRemoved;
	bool L3_S4_IsPop2BSnakeButtonPressed;
	bool L3_S4_IsPop2CSnakeButtonPressed;
	bool L3_S4_IsPop2CStoneRemoved;
	bool L3_S4_IsPop2CLeavesRemoved;
	bool L3_S4_IsPop2DSnakeButtonPressed;
	bool L3_S4_IsPop2DStoneRemoved;
	bool L3_S4_IsPop2DLeavesRemoved;
	bool L3_S4_IsPop4StoneRemoved;
	bool L3_S4_IsPop4LeavesRemoved;
	bool L3_S4_InvBambooWithSickleTaken;

	bool L3_S5_InvRopeTaken;
	bool L3_S5_InvClipTaken;
	bool L3_S5_GreenStonePlaced;
	bool L3_S5_DeerGone;
	bool L3_S5_ClipPlaced;
	bool L3_S5_RopeTied;
	bool L3_S5_NailPlaced;
	bool L3_S5_NailHammered;

	bool L3_S5_DryGrassTaken1;
	bool L3_S5_DryGrassTaken2;
	bool L3_S5_DryGrassTaken3;
	bool L3_S5_DryGrassTaken4;
	bool L3_S5_DryGrassTaken5;
	bool L3_S5_DryGrassTaken6;
	bool L3_S5_DryGrassTaken7;
	bool L3_S5_DryGrassTaken8;
	bool L3_S5_DryGrassTaken9;
	bool L3_S5_DryGrassTaken10;
	bool L3_S5_DryGrassTaken11;

	bool L3_S5_InvGrassTaken1;
	bool L3_S5_InvGrassTaken2;
	bool L3_S5_InvGrassTaken3;
	bool L3_S5_InvGrassTaken4;
	bool L3_S5_InvGrassTaken5;
	bool L3_S5_InvGrassTaken6;

	bool L3_S5_InvAllGrassTaken;

	//Level4
	bool L4_S1_IsFirstVisit;
	bool L4_S1_PocketOpend;
	bool L4_S1_InvHandKerchiefTaken;
	bool L4_S1_InvCrutchTaken;
	bool L4_S1_SkimmerFlyed;
	bool L4_S1_InvStickTaken;
	bool L4_S1_InvLeafTaken;
	bool L4_S1_InvLeafSpoonWithWaterTaken;
	bool L4_S1_InvHerbalLeavesTaken;
	bool L4_S1_InvBraceletTaken;
	bool L4_S1_FirstThornRemoved;
	bool L4_S1_SecondThornRemoved;
	bool L4_S1_ShoeRemoved;
	bool L4_S1_BloodRemoved;
	bool L4_S1_MedPasteApplied;
	bool L4_S1_WoundTied;
	bool L4_S1_PutShoeBack;
	bool L4_S1_TwigsMoved;
	bool L4_S1_InvLeafSpoonCollected;
	bool L4_S1_LeafTored;
	bool L4_S1_HandKerchiefTored;
	bool L4_S1_PutLeftShoe;
	bool L4_S1_CrutchUsed;

	bool L4_S1_PuzzleSolved;
	bool L4_S1_InvBinocularLensTaken;
	bool L4_S1_InvLeftShoeTaken;
	bool L4_S1_PuzzleVisited;
	bool L4_S1_HOShoeFound[12];
	bool L4_S1_HOShoeCompleted;
	bool L4_S1_BetelBoxOPened;
	bool L4_S1_InvKnifeCollected;

	bool L4_S2_FruitsPlucked;
	bool L4_S2_FruitsTaken;
	bool L4_S2_AlligatorPuzzleSolved;
	int L4_S2_AlligatorPuzzleState;
	bool L4_S2_BambooStickTaken;
	bool L4_S2_Pop2BambooTaskCompleted;
	bool L4_S2_GrassTaken;
	bool L4_S2_ComboKnifeAddedToBamboo;
	bool L4_S2_InvBambooSpearTaken;
	bool L4_S2_InvRopeTaken;
	bool L4_S2_InvGrinderHandleTaken;
	bool L4_S2_DoorToCaveOpened;
	int L4_S2_FanPuzzleState;
	int L4_s2_GrassRopeFrameNumber;
	bool L4_S2_MortarTaken;

	bool L4_S3_IsCaveLit;
	bool L4_S3_IsDoorPuzzleSolved;
	bool L4_S3_IsBowPlaced;
	bool L4_S3_IsEmberCreated;
	bool L4_S3_IsOilApplied;
	bool L4_S3_IsFireSticksPlaced;
	bool L4_S3_IsCoinPlaced;
	bool L4_S3_IsPlankPlaced;
	bool L4_S3_IsInvBowlTaken;
	bool L4_S3_IsMetalPaperTaken;
	bool L4_S3_IsPotRemoved;

	bool L4_S4_Visited;
	bool L4_S4_SkeletonHandOpened;
	bool L4_S4_InvBinocularTaken;
	bool L4_S4_FlowerCutscenePlayed;
	bool L4_S4_GlobGlowEnabled;
	bool L4_S4_GrinderHandlePlaced;
	bool L4_S4_OlivePlaced;
	bool L4_S4_StrainerPlaced;
	bool L4_S4_PotPlaced;
	bool L4_S4_InstructionsPlaced;
	bool L4_S4_OilMachinePlaced;
	bool L4_S4_OilMachineRotated;
	bool L4_S4_PotWithOilTaken;
	bool L4_S4_InvBinocularWithLensTaken;
	bool L4_S4_JumpCutscenePlayed;
	bool L4_S4_ClimbCutscenePlayed;

	bool L4_S4_HOBinocularFound[6];
	bool L4_S4_HOBinocularCompleted;
	bool L4_S4_BannerTextAfterBinocularHOShown;

	bool L4_S2_InvSmallBambooStick0Taken;
	bool L4_S2_InvSmallBambooStick1Taken;
	bool L4_S2_InvSmallBambooStick2Taken;
	bool L4_S2_InvSmallBambooStick3Taken;
	bool L4_S2_IsBambooStickCut;

	bool L4_S5_Visited;
	bool L4_S5_InvDoorTaken;
	bool L4_S5_ScrewRemoved;
	bool L4_S5_ClothRemoved;
	bool L4_S5_InvOlivesCollected;
	bool L4_S5_OilMachineBaseCollected;
	bool L4_S5_InvNetTaken;
	bool L4_S5_ComboBambooFrameAddedToNet;
	bool L4_S5_InvBambooStrainerTaken;
	bool L4_S5_DoorPlaced;

	bool L4_S2_InvTwig1Taken;
	bool L4_S4_InvTwig2Taken;
	bool L4_S5_InvTwig3Taken;
	bool L4_S5_InvAlligatorHeadTaken;
	bool L4_S5_InvDoorKeyTaken;
	bool L4_S5_SwapPuzzleCompleted;

	//Level5
	bool L5_S1_InvBowTaken;
	bool L5_S1_InvBowWithArrowFirstTaken;
	bool L5_S1_InvBowWithArrowSecondTaken;
	bool L5_S1_InvArrowHolderTaken;
	bool L5_S1_BowStringFixed;
	bool L5_S1_BowArrowHolderPlaced;
	bool L5_S1_BowArrowFirstLoaded;
	bool L5_S1_BowArrowSecondLoaded;
	bool L5_S1_BowToTigerCutscenePlayed;

	bool L5_S2_InvHammerTaken;
	bool L5_S2_InvSoldierBadgeTaken;

	int L5_S2_PZBlockStartRow[MAX_L5S2_NUM_BLOCKS];
	int L5_S2_PZBlockStartCol[MAX_L5S2_NUM_BLOCKS];
	bool L5_S2_PZBlocksPuzzleSolved;
	bool L5_S2_InvBadgeTaken;
	bool L5_S2_LogPushed;
	bool L5_S2_LogFell;
	bool L5_S2_GreaseTaken;

	bool L5_S3_IntroVdoCompleted;
	bool L5_S3_memClothTaken;
	bool L5_S3_EmblemPlacedOnStone;
	bool L5_S3_PaperCollectedFromStone;
	bool L5_S3_InvMoonSolutionTaken;
	bool L5_S3_VineFreed;
	bool L5_S3_isHoActive;
	bool L5_S3_isLostHook;
	bool L5_S3_PZHaveHook;
	bool L5_S3_HookLostBannerShowed;
	bool L5_S3_LeftDoorOpened;
	bool L5_S3_EnteredLeftDoor;
	int L5_S3_PZLocketIndexes[16];
	int L5_S3_PZState;
	bool L5_S3_PZCompleted;
	bool L5_S3_HOUnderWaterStarRemoved;
	bool L5_S3_HOUnderWaterFoundObjs[12];
	bool L5_S3_HOUnderWaterCompleted;

	bool L5_S4_InvPickaxeTaken;
	bool L5_S4_KeyMatchComplete[6];
	bool L5_S4_Level1Completed;
	int L5_S4_PZHandCuffState;
	bool L5_S4_PZHandCuffLockUnlocked[3];
	bool L5_S4_PZHandCuffComplete;

	bool L5_S4_InvPickaxeHeadTaken;
	bool L5_S4_GreaseUsed;
	bool L5_S4_Stone1Moved;
	bool L5_S4_Stone2Moved;
	bool L5_S4_BridgeOpened;
	bool L5_S4_InvBarkTaken;
	bool L5_S4_PickAxeUsed;
	bool L5_S4_PickAxeCutScenePlayed;
	bool L5_S4_AnasuyaTalked;
	bool L5_S4_InvChainTaken;
	bool L3_S4_PZ1BridgeCompleted;
	int L3_S4_PZ1BridgeRingAngles[5];

	bool L5_S5_InvPickaxeHandleTaken;
	bool L5_S5_InvHandcuffKeyTaken;
	bool L5_S5_InvWaterproofBagTaken;
	bool L5_S5_InvBambooLadderTaken;
	bool L5_S5_InvStoneEmblemTaken;
	bool L5_S5_IsAmeehaDiaryCutscenePlayed;
	bool L5_S5_IsSoldierBadgeUsedOnDoor;
	bool L5_S5_IsBambooLadderPlacedOnScene;
	bool L5_S5_IsMoonGlassDoorOpened;
	bool L5_S5_IsTopDoorOpened;
	bool L5_S5_IsP1Stone1Removed;
	bool L5_S5_IsP1Stone2Removed;
	bool L5_S5_IsP1PillerRemoved;
	bool L5_S5_HOGanpathiFoundObjs[14];
	bool L5_S5_HOGanpathiCompleted;
	bool L5_S5_HOGanapathiActivated;

	int  L5_S5_PZTokenState[MAX_L5S5_PZBLOCKS];	
	bool L5_S5_PZSolutionPaperPlaced;
	bool L5_S5_PZStatesInitialized;
	bool L5_S5_IsSunClockPuzzleCompleted;

	bool L5_S6_HOSoldierBadgePlaced;
	bool L5_S6_HOGuardianIntroTalkCompleted;
	bool L5_S6_HOObjectFound[MAX_L5S6_HOOBJECTS];
	bool L5_S6_HOCompleted;
	bool L5_S6_PeacockChainUsed;
	bool L5_S6_InvPeacockLocketTaken;
	bool L5_S6_PeacockLocketUsed;
	bool L5_S6_PeacockArrivingCutScenePlayed;
	bool L5_S6_PeacockLeavingCutScenePlayed;
	bool L5_S6_IntroCutScenePlayed;
	bool L5_S6_GotFreeBannerShown;

	//Level 6
	bool L6_S1_Visited;
	bool L6_S1_ChappalKnobRemoved;
	bool L6_S1_LetterOpened;
	bool L6_S1_VaruniCutscenePlayed;
	bool L6_S1_MaryFirstBannerTextShown;
	bool L6_S1_MarySecondBannerTextShown;
	bool L6_S1_ScrewUnsealed;
	bool L6_S1_InvEmblemTaken;
	bool L6_S1_InvMortarTaken;
	bool L6_S1_InvSteelTaken;
	bool L6_S1_SolutionFound;
	bool L6_S1_InvClothTaken;
	bool L6_S1_StarKeyPlaced;
	bool L6_S1_EmblemPlaced;
	bool L6_S1_PathToS3Activated;
	bool L6_S1_InvClubTaken;
	bool L6_S1_TextForHOShown;

	bool L6_S1_SceneHOSolved[MAX_L6S1_HOSCENES];
	bool L6_S1_DescriptionHOSolved;
	bool L6_S1_DescHOFound_S1[4];
	bool L6_S1_DescHOFound_S2[2];
	bool L6_S1_DescHOFound_S3[3];
	bool L6_S1_DescHOFound_S4[4];
	bool L6_S1_DescHOFound_S5[2];
	bool L6_S1_InvHorseShoeTaken;
	bool L6_S1_LetterTaken;
	bool L6_S1_ChestPuzzleSolved;

	bool L6_S2_DoorToS3Opened;
	bool L6_S2_InvIceKeyTaken;
	bool L6_S2_InvPestleTaken;
	bool L6_S2_ColoredPowderTrowed;

	bool L6_S2_HOComplete;
	bool L6_S2_HOItemsFound[7];
	bool L6_S2_InvPuzzlePiecesCollected;
	bool L6_S2_PuzzleComplete;
	bool L6_S2_InvPanTaken;
	bool L6_S2_InvPanWithPinePitchPlaced;
	bool L6_S2_InvStickPlaced;
	bool L6_S2_InvFlintPlaced;
	bool L6_S2_InvSteelPlaced;
	bool L6_S2_InvMortarWithCharcoalPlaced;
	bool L6_S2_InvPestlePlaced;
	int	 L6_S2_PZPineGumObjectsUsed[MAX_L6S2_PZOBJECTS];

	bool L6_S3_RoomLightened;
	bool L6_S3_JamesDialogsShown;
	bool L6_S3_LasyaCutscenePlayed;
	bool L6_S3_ClubsUsed;
	bool L6_S3_InvCharcoalTaken;
	bool L6_S3_InvHorseShoeTaken;
	bool L6_S3_SteelPlaced;
	bool L6_S3_FlintStoneUsed;
	bool L6_S3_PathwayActived;
	bool L6_S3_PathwayDestructed;
	bool L6_S3_PZPathwayPuzzleCompleted;
	int  L6_S3_PZPathwayRingAngles[5];

	bool L6_S3_HO_Visited;
	bool L6_S3_HO_CodeTaken;
	bool L6_S3_HO_CodeApplied;
	bool L6_S3_HO_CodePuzzleSolved;
	bool L6_S3_HO_BottleRemoved;
	bool L6_S3_HO_PotRemoved;
	bool L6_S3_HO_MainLockOpened1;
	bool L6_S3_HO_MainLockOpened2;
	bool L6_S3_HO_MainLockOpened3;
	bool L6_S3_HO_MainBoxOpened;
	bool L6_S3_HO_InvHeartTaken;
	bool L6_S3_HO_VaseToppled;
	bool L6_S3_HO_Bolt1Removed;
	bool L6_S3_HO_Bolt2Removed;
	bool L6_S3_HO_VaseDoorRemoved;
	int	 L6_S3_HO_CartPosition;
	int  L6_S3_HO_ObjectStates[6];

	bool L6_S4_IsVisited;
	bool L6_S4_InvStickTaken;
	bool L6_S4_InvRopeTaken;
	bool L6_S4_InvPineGumCollected;
	bool L6_S4_InvPineGumStickCollected;
	bool L6_S4_PanPlaced;
	bool L6_S4_RockBrokened;
	bool L6_S4_InvFlintStoneCollected;
	bool L6_S4_IsHorseShoeMagnetized;
	bool L6_S4_InvMagnetisedHorseShoeTaken;
	bool L6_S4_InvRopeHorseShoeComboTaken;
	bool L6_S4_InvMagnifyingGlassTaken;
	bool L6_S4_KeyInIcePlaced;
	bool L6_S4_InvStarShapedKeyTaken;
	bool L6_S4_AmeehaNoteTaken;
	bool L6_S4_MaryTextShown;
	bool L6_S4_HeartCrystalPlaced;
	bool L6_S4_GlueApplied;
	bool L6_S4_ChestOpened;
	bool L6_S4_NoteOpened;
	bool L6_S4_InvColoredPowderTaken;
	bool L6_S4_IsRockManDialogOver;
	bool L6_S4_MaryWalkingCutscenePlayed;

	bool L6_S4_PuzzlePiecesPlaced;
	bool L6_S4_PortalPuzzleObjectStates[7];
	bool L6_S4_PuzzleComplete;
	bool L6_S4_LetterFromAnanthaTaken;

	bool L6_S5_FoundSophia;
	bool L6_S5_Visited;
	bool L6_S5_DialogsOver;
	bool L6_S5_SophiaTabletDialogOver;

	int L6_S5_TabMiniGameSceneNumber;
	int L6_S5_TabMiniGameState;
	int L6_S5_TabMiniGamePluckedFruitIndex;
	bool L6_S5_TabMiniGameIsClothRemoved;
	bool L6_S5_TabMiniGameIsCarrotPicked[5];
	bool L6_S5_TabMinigameSolved;

	bool L6_S5_HO_Complete;
	bool L6_S5_HOObjFound[10];

	bool L7_S1_PZJigsawYellowPlaced[MAX_L7S1_JIGSAWPIECES];
	bool L7_S1_PZJigsawYellowCompleted;
	bool L7_S1_PZStarDoorOpened[3];
	bool L7_S1_InvPZStarEmblemCollected[3];

	bool L7_S1_InvPotEmptyTaken;
	bool L7_S1_ComboPotChainPlaced;
	bool L7_S1_InvPotWithChainTaken;
	bool L7_S1_InvAxeUsed;
	bool L7_S1_InvRedStarUsed;
	bool L7_S1_WaterUsedOnRedFlower;
	bool L7_S1_InvRedFlowerTaken;

	bool L7_S1_WellLidOpened;
	bool L7_S1_InvWheelUsed;
	bool L7_S1_InvMagicWaterTaken;
	bool L7_S1_InvPotTaken;
	int L7_S2_PZEyePartIndex[8];
	int L7_S2_PZEyeGameState;
	bool L7_S2_PZEyePuzzleCompleted;
	bool L7_S2_InvSymbolDiscTaken;
	bool L7_S2_InvStatueTaken;

	bool L7_S2_HOLeftHandSymbolFound[MAX_L7S2_HANDHOSYMBOLS];
	bool L7_S2_HORightHandSymbolFound[MAX_L7S2_HANDHOSYMBOLS];
	bool L7_S2_HORightHandCompleted;
	bool L7_S2_HORightHandDiskPlaced;
	bool L7_S2_HOLeftHandDiskPlaced;
	bool L7_S2_IsRightHandExplodeShown;
	bool L7_S2_HOLeftHandCompleted;
	bool L7_S2_IsLeftHandExplodeShown;
	bool L7_S2_HOBrokenRightHandCompleted;
	bool L7_S2_HOBrokenLeftHandCompleted;
	bool L7_S2_InvSphereHalfLeftToFixTaken;
	bool L7_S2_InvSphereHalfRightBaseTaken;
	bool L7_S2_InvSphereFullTaken;
	bool L7_S2_PopSphereFullPlaced;
	bool L7_S2_LasyaStatueRisenUp;

	int L7_S2_HOBrockenLeftHandState;
	bool L7_S2_HOBrockenLeftHandSymbolCompleted[6];
	int L7_S2_HOBrockenRightHandState;
	bool L7_S2_HOBrockenRightHandSymbolCompleted[6];

	bool L7_S2_PZLasyaStatuePlaced;
	bool L7_S2_PZLasyaStatuePuzzleCompleted;
	bool L7_S2_PZLasyaDiffFound[MAX_L7S2_LASYAITEMS];

	bool L7_S3_PZJigsawRedPlaced[MAX_L7S3_JIGSAWPIECES];
	bool L7_S3_PZJigsawRedCompleted;

	bool L7_S3_InvAxesTaken;
	bool L7_S3_InvAxeKeyTaken;
	bool L7_S3_IsAxeKeyUsed;
	bool L7_S3_InvLeverTaken;
	bool L7_S3_IsLeverUsedOnChain;
	bool L7_S3_InvMetalWireTaken;
	bool L7_S3_InvMetalChainTaken;
	bool L7_S3_InvBhasmamPowderTaken;
	bool L7_S3_IsBroomUsedOnBhasmam;
	bool L7_S3_InvBroomTaken;
	bool L7_S3_InvYellowFlowerTaken;
	bool L7_S3_IsBullMoved;
	bool L7_S3_IsLetterAmeehaTaken;
	bool L7_S3_IsDiwaliLampPlaced;
	int L7_S3_PZDoorState;
	bool L7_S3_LocksState[3];
	bool L7_S3_PZDoorCompleted;
	bool L7_S3_IsLasyaAttackCutscnPlayed;
	bool L7_S3_IsNightmareCutscnPlayed;

	bool L7_S4_PZJigsawBluePlaced[MAX_L7S4_JIGSAWPIECES];
	bool L7_S4_PZJigsawBlueCompleted;
	bool L7_S4_HaveHandPower;
	bool L7_S4_InvBlueFlowerTaken;
	int L7_S4_TableTopFlowerPos[3];
	bool L7_S4_TableTopBasmamUsed;
	bool L7_S4_TableTopBasmamComplete;
	bool L7_S4_TableTopPoojaPopComplete;
	int L7_S4_TableTopPoojaPuzzleMode;

	bool L7_S4_InvWaterUsedOnBlueFlower;
	bool L7_S4_InvBlueStarUsed;
	bool L7_S4_InvWellWheelTaken;
	bool L7_S4_P1NoteClicked;
	bool L7_S4_P1NoteTaken;
	bool L7_S4_AmehaTalked;

	bool L7_S5_End1CutScenePlayed;
	bool L7_S5_End2CutScenePlayed;


	//Level 8
	bool L8_S1_IntroVidPlayed;
	bool L8_MagicSymbolFound[MAX_L8_MAGIC_SYMBOLS];
	bool L8_MagicGlassUsed;
	bool L8_S1_FloorCracked;
	bool L8_S1_InvIdolTaken;
	bool L8_S1_InvMagicWandTaken;
	bool L8_S1_InvHalfKeyTaken;
	bool L8_S1_NoteTaken;
	bool L8_S1_DoorKeyPlaced;
	bool L8_S1_HandPlaced;
	bool L8_S1_PZDoorOpened;
	bool L8_S1_DoorKeyTaken;
	bool L8_S1_MagicWandWithoutKeyTaken;
	bool L8_S1_MatchingSymbolFound[MAX_L8_S1_MATCHING_SYMBOLS_COUNT];
	bool L8_S1_MatchingHOComplete;

	bool L8_S1_HandPuzzleSolved;
	bool L8_S1_InvMagicGlassTaken;
	bool L8_S5_IsPuzzleDataSaved;
	bool L8_S5_RingPiece_Visibility[3][12];
	int L8_S5_RingPiece_CurrentRow[3][12];

	bool l8_s1_DoorHOCompleted;
	bool l8_s1_DoorHOFoundObjects[3][8];

	bool L8_S2_Visited;
	bool L8_S2_SmallDoorKeyPlaced;
	bool L8_S2_SmallDoorOpened;
	bool L8_S2_InvToolTaken;
	bool L8_S2_InvScrollTaken;
	bool L8_S2_InvHalfKeyTaken;
	bool L8_S2_GateKeyPlaced;
	bool L8_S2_GatePuzzleSolved;
	bool L8_S2_GateOpened;
	bool L8_S2_GatePuzzleStarted;
	int L8_S2_GearPuzzleState;
	int L8_S2_GateSymbolArray[10];
	int	 L8_S2_GearSlotGearId[L8_S2_NUM_GEARS];
	bool L8_S2_SugarCaneGrown;
	bool L8_S2_InvSugarCaneTaken;
	int L8_S2_SugercanePZState;
	int L8_S2_SugercanePZLevel;
	bool L8_S2_IsSugarCaneGivenToElephant;
	bool L8_S2_IsClickedOnLotus;
	bool L8_S2_InvSwordHandleTaken;
	bool L8_S2_FloorCracked;
	bool L8_S2_InvIdolTaken;
	bool L8_S2_MatchingSymbolFound[MAX_L8_S1_MATCHING_SYMBOLS_COUNT];
	bool L8_S2_MatchingHOComplete;

	bool L8_S3_FloorCracked;
	bool L8_S3_InvIdolTaken;
	bool L8_S3_SwordHandlePlaced;
	bool L8_S3_SwordPuzzleSolved;
	bool L8_S3_SwordPiecePlaced[L8S3_SWORDPIECES];
	bool L8_S3_InvSwordTaken;
	bool L8_S3_LeafTextShown;
	bool L8_S3_MatchingSymbolFound[MAX_L8_S1_MATCHING_SYMBOLS_COUNT];
	bool L8_S3_MatchingHOComplete;

	bool L8_S3_LockKeyPlaced;
	int L8_S3_SymboOnStrip[MAX_L8S3_STRIPS];
	bool L8_S3_PZDoorOpened;
	bool L8_S3_StripPuzzleSolved;
	bool L8_S3_InvCrystalKeyTaken;
	bool l8_s3_invMaryLeafTaken;
	bool l8_s3_MaryLeafHOCompleted;
	bool  l8_s3_LeafHOFoundObjects[3][4];

	bool L8_S4_BronzeIdolPlaced;
	bool L8_S4_SilverIdolPlaced;
	bool L8_S4_GoldIdolPlaced;
	bool L8_S4_IsLeafPlacedInContainer;
	bool L8_S4_IsMinigameAnimPlayed;
	bool L8_S4_InvContainerWithLeafTaken;
	bool L8_S4_ContainerWithLeafOpened;
	bool L8_S4_IsContainerClosed;
	bool L8_S4_IsPuzzleDataSaved;
	bool L8_S4_IsPoolPuzzleSolved;
	int L8_S4_FlowersOnBoardIndex[3][3][4];

	bool L8_S5_IntroVideoShown;
	bool L8_S5_Visited;
	bool L8_S5_EndVideoPlayed;

	bool L8_S5_VirtualMiniGameCompleted;
	int L8_S5_VirtualMiniGameState;
	bool L8_S5_VirtualMiniGameShapesComplete[3];
	bool L8_S5_VirtualMiniGameHOState[9];


	bool IsMainChapterCompleted;
	bool IsBonusChapterUnlocked;
	bool IsBonusChainRemoved;

	void Clear();
	void InitProfileLevel1Vars();

	void setForExtras();

protected:
	//Inherited methods
	void Write(CIOStream *stream);
	void Read(CIOStream* stream);
};

#endif
