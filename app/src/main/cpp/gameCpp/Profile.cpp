//====================================
// Framework
// Profile Class
//====================================

#include "Profile.h"
#include "IOStream.h"
#include "SConstants.h"
#include "ProfileSystem.h"


Profile::Profile()
{
	InitGameVars();
}

Profile::~Profile()
{
}

void Profile::InitGameVars()
{	
	_IsSplashDone = false;
	_IsFromSplash = false;
	_IsFromGame = false;

	_iActiveProfileSlot = -1;
	_PrevArea = eArea_Nil;

}

void Profile::Write(CIOStream *stream)
{
	stream->WriteInt(CurrentArea);	
	stream->WriteInt(CurrentLevel);
	stream->WriteInt(PrevComboActiveArea);
	stream->WriteInt(PrevMapActiveArea);
	stream->WriteInt(MusicVol);
	stream->WriteInt(SFXVol);
	stream->WriteInt(EnvirVol);
	stream->WriteInt(VoiceVol);

	stream->WriteBool(IsLowSystem);
	stream->WriteBool(IsCheckedForLowSystem);
	stream->WriteBool(IsWideScreen);
	stream->WriteBool(IsHOScenesSkippable);
	stream->WriteBool(IsMuted);
	stream->WriteBool(IsStartMusic);
	stream->WriteBool(IsHidingTips);
	stream->WriteBool(IsAwardPopupsEnabled);
	stream->WriteBool(IsInvLocked);
	stream->WriteBool(IsExpertMode);
	stream->WriteBool(IsHideSparkles);
	stream->WriteBool(IsHideHOSparkles);
	stream->WriteBool(IsTutorialDisabled);
	stream->WriteBool(IsHintDisabled);
	stream->WriteBool(IsSkipDisabled);
	#ifdef INSTANT_RECHARGE
	CustomHintTimer = 1.0f;
	CustomSkipTimer = 1.0f;	
	#endif
	stream->WriteFloat(CustomHintTimer);
	stream->WriteFloat(CustomSkipTimer);

	stream->WriteBool(IsStartCutscenePlayed);
	stream->WriteBool(IsHaveYouCutscenePlayed);
	stream->WriteBool(IsEndCutscenePlayed);	
	stream->WriteBool(IsPromoCutscenePlayed);
	stream->WriteBool(IsCrystalOrbPicked);

	stream->WriteInt(ExtrasCurrentPageId);
	stream->WriteInt(ExtrasCurrentPageIndex);


	//User Journal Notes
	stream->WriteString("JournalNotes");
	stream->WriteBool(IsJObjBtnActive);

	stream->WriteInt(JrnPageLastUnlocked);
	stream->WriteInt(JrnPageMaxRead);
	stream->WriteInt(JrnPageLastRead);

	for (int i = 0; i < MAX_JOURNAL_NOTES; i++)
	{
		stream->WriteBool(IsJrnNotesFound[i]);
		stream->WriteInt(JrnNotesCollected[i]);
	}
	stream->WriteInt(NumJrnNotesCollected);
	for (int i = 0; i < MAX_CHARACTERS; i++)
	{
		stream->WriteBool(isCharacterUnlocked[i]);
	}
	stream->WriteInt(lastUnlockedCharacter);
	stream->WriteInt(lastViewedCharacter);

	for (int i = 0; i < MAX_HALL_OF_FAME_AWARDS; i++)
	{
		stream->WriteInt(currentNumOfEvents[i]);
	}
	for (int i = 0; i < eArea_Max; i++)
	{
		stream->WriteBool(isHintUsedAfterHOStart[i]);
		stream->WriteBool(isMiniGameCompleted[i]);
		stream->WriteBool(isHOCompleted[i]);
		stream->WriteBool(isSkipUsedAfterMiniGameStart[i]);
		stream->WriteBool(isHOCompleteWithMatch3[i]);
	}



	stream->WriteBool(isMiniGameSkipedAfterGameStart);
	stream->WriteBool(isJigsawPuzzleSkipped);
	stream->WriteBool(isCutSceneSkipedAfterGameStart);
	stream->WriteBool(isHintUsedAfterGameStart);
	stream->WriteBool(isHOHintUsedAfterGameStart);
	stream->WriteBool(isHintUsedAfterCreeksMansionStart);

	for (int i = 0; i < MAX_LVL_OBJECTIVES; i++)
	{
		stream->WriteBool(IsLVLObjectiveCompleted[i]);
		stream->WriteBool(IsLVLObjectiveAdded[i]);
	}
	for (int i = 0; i < MAX_CURRENT_OBJECTIVES; i++)
		stream->WriteInt(CurrentLvLObjectives[i]);

	stream->WriteInt(NumObjectivesActive);

	for( int i=0; i < eInv_Max; ++i )
		stream->WriteBool(Inventory[i]);

	for( int i=0; i < MAX_INVBOX_OBJECTS; ++i )
	{
		stream->WriteInt(InvObjects[i].InvId);
		stream->WriteInt(InvObjects[i].nPiecePicked);
		stream->WriteInt(InvObjects[i].MaxPieces);		
	}
	stream->WriteInt(InvObjectCnt);
	stream->WriteInt(iInvBoxStartObj);	

	stream->WriteInt(HintSavedTime);

	for( int i=0; i < eArea_Max; ++i )
		stream->WriteInt(SkipTimer[i]);

	stream->WriteBool(TutorialSpecialFeaturesOnly);
	stream->WriteInt(TutorialState);
	for( int i=0; i < MAX_TUTORIAL; ++i )
		stream->WriteBool(IsTutorialShown[i]);

	for( int i=0; i < eArea_Max; ++i )
		stream->WriteBool(IsTaskLeft[i]);

	for( int i=0; i < eArea_Max; ++i )
		stream->WriteBool(IsAreaLocked[i]);

	for( int i=0; i < eArea_Max; ++i )
		stream->WriteBool(IsAreaVisited[i]);

	for (int i = 0; i < eMem_MAX; i++)
		stream->WriteBool(IsMemoryCollected[i]);

	stream->WriteInt(NumMemoriesCollected);
	stream->WriteInt(LastMemoryCollected);

	for (int i = 0; i < eAward_MAX; i++)
		stream->WriteBool(IsAchievementUnlocked[i]);
	for (int i = 0; i < eAward_MAX; i++)
		stream->WriteBool(IsAchievementFixed[i]);
	stream->WriteInt(NumAchievementsCollected);
	stream->WriteInt(ElapsedGamePlayTime);
	for (int i = 0; i < eArea_Max; i++)
		stream->WriteInt(AreaStartTime[i]);
	stream->WriteInt(IsPlayingGameOnExpertMode);

	stream->WriteBool(IsPaptusL1S2Collected);
	stream->WriteBool(IsPaptusL1S3Collected);
	stream->WriteBool(IsPaptusL2S6Collected);
	stream->WriteBool(IsPaptusL3S5Collected);
	stream->WriteBool(IsPaptusL4S3Collected);
	stream->WriteBool(IsPaptusL5S5Collected);
	stream->WriteBool(IsPaptusL6S1Collected);
	stream->WriteBool(IsPaptusL6S1popCollected);
	stream->WriteBool(IsPaptusL7S3Collected);

	stream->WriteBool(IsMorphObjL1S1Collected);
	stream->WriteBool(IsMorphObjL1S2Collected);
	stream->WriteBool(IsMorphObjL1S3Collected);
	stream->WriteBool(IsMorphObjL1S7Collected);

	stream->WriteBool(IsMorphObjL2S1Collected);
	stream->WriteBool(IsMorphObjL2S2Collected);
	stream->WriteBool(IsMorphObjL2S3Collected);
	stream->WriteBool(IsMorphObjL2S4Collected);
	stream->WriteBool(IsMorphObjL2S5Collected);
	stream->WriteBool(IsMorphObjL2S6Collected);

	stream->WriteBool(IsMorphObjL3S2Collected);
	stream->WriteBool(IsMorphObjL3S3Collected);
	stream->WriteBool(IsMorphObjL3S1Collected);
	stream->WriteBool(IsMorphObjL3S4Collected);

	stream->WriteBool(IsMorphObjL4S1Collected);
	stream->WriteBool(IsMorphObjL4S3Collected);
	stream->WriteBool(IsMorphObjL4S4Collected);
	stream->WriteBool(IsMorphObjL4S5Collected);

	stream->WriteBool(IsMorphObjL5S2Collected);
	stream->WriteBool(IsMorphObjL5S4Collected);
	stream->WriteBool(IsMorphObjL5S5Collected);
	stream->WriteBool(IsMorphObjL5S6Collected);

	stream->WriteBool(IsMorphObjL6S1Collected);
	stream->WriteBool(IsMorphObjL6S2Collected);
	stream->WriteBool(IsMorphObjL6S3Collected);
	stream->WriteBool(IsMorphObjL6S4Collected);

	stream->WriteBool(IsMorphObjL7S1Collected);
	stream->WriteBool(IsMorphObjL7S2Collected);
	stream->WriteBool(IsMorphObjL7S3Collected);
	stream->WriteBool(IsMorphObjL7S4Collected);

	stream->WriteInt(NumMorphObjectsCollected);

	//Level 1 - Mansion
	stream->WriteBool(L1_IsMansionPowerOn);
	stream->WriteBool(L1_IsDemoOutroVideoPlayed);

	//Scene_1_Beach
	stream->WriteBool(L1S1_IsTalkedWithRaja);
	stream->WriteBool(L1S1_IsRajaTakenJames);

	stream->WriteBool(L1S1_IsInvBagTaken);
	stream->WriteBool(L1S1_IsInvKnifeTaken);
	stream->WriteBool(L1S1_IsInvLetterCreekTaken);
	stream->WriteBool(L1S1_IsInvPlank3Taken);

	stream->WriteBool(L1S1_LocketPartsPlaced);
	stream->WriteBool(L1S1_LocketPart1Fixed);
	stream->WriteBool(L1S1_LocketPart2Fixed);
	stream->WriteBool(L1S1_LocketPart3Fixed);
	stream->WriteBool(L1S1_LocketPart4Fixed);
	stream->WriteBool(L1S1_LocketAllPartsFixed);
	stream->WriteBool(L1S1_BagOpened);
	stream->WriteBool(L1S1_KidsPaintingPuzzleActive);
	stream->WriteBool(L1S1_KidsPaintingAddedToJournal);
	stream->WriteBool(L1S1_KidsPaintingPzBagActive);
	stream->WriteInt(L1S1_ComboBagInitX);
	stream->WriteInt(L1S1_ComboBagInitY);
	stream->WriteBool(L1S1_BagZipOpened);

	stream->WriteBool(L1S1_P1Branch1Cut);
	stream->WriteBool(L1S1_P1Branch2Cut);
	stream->WriteBool(L1S1_P1Branch3Cut);
	stream->WriteBool(L1S1_P1Leaf1Removed);
	stream->WriteBool(L1S1_P1Leaf2Removed);
	stream->WriteBool(L1S1_P1Leaf3Removed);
	stream->WriteBool(L1S1_P1Ruby1Branch2Taken);
	stream->WriteBool(L1S1_P1Ruby2Branch3Taken);
	stream->WriteBool(L1S1_P1Ruby3UnderLeaf1Taken);
	stream->WriteBool(L1S1_P1Ruby4UnderLeavesTaken);

	stream->WriteBool(L1S1_P2BuffaloClicked) ;
	stream->WriteBool(L1S1_P2CastleClicked);
	stream->WriteBool(L1S1_P2MeClicked);
	stream->WriteBool(L1S1_P2MamaClicked) ;
	stream->WriteBool(L1S1_P2KingClicked);
	stream->WriteBool(L1S1_P2IsPuzzleComplete);

	//Scene_2_Frontyard
	stream->WriteBool(L1S2_IsMonkeyTookBanana);
	stream->WriteBool(L1S2_IsCarDickeyOpen);
	stream->WriteBool(L1S2_P1_IsSackMoved);
	stream->WriteBool(L1S2_P1_IsWoodMoved);
	stream->WriteBool(L1S2_P3IsSwingRopeCut);
	stream->WriteBool(L1S2_P3IsTreeBranchFixed);
	stream->WriteBool(L1S2_IsInvCloth1SwingTaken);
	stream->WriteBool(L1S2_IsInvCloth2LovePopTaken);
	stream->WriteBool(L1S2_IsInvCloth3CarDickyTaken);
	stream->WriteBool(L1S2_InvDieselCanTaken);
	stream->WriteBool(L1S2_InvHoseTaken);
	stream->WriteBool(L1S2_IsInvPlank1Taken);
	stream->WriteBool(L1S2_IsInvTreeBranchTaken);

	stream->WriteBool(L1S2_IsFusePuzzleSolved);
	stream->WriteBool(L1S2_IsFusePuzzleShuffled);
	stream->WriteBool(L1S2_IsP1FusePuzzleActive);
	stream->WriteBool(L1S2_IsP1LeftFuseFixed);
	stream->WriteBool(L1S2_IsP1RightFuseFixed);
	stream->WriteBool(L1S3_IsP1LeftFuseTaken);
	stream->WriteBool(L1S2_IsP1RightFuseTaken);
	stream->WriteBool(L1S2_IsP1GreenWireDownFixed);
	stream->WriteBool(L1S2_IsP1GreenWireTopFixed);

	for (int i = 0; i < 4; i++)
	{
		stream->WriteInt(L1S2_IsP1RotFuseRot[i]);
		stream->WriteInt(L1S2_IsP1WireConnectId[i]);
		stream->WriteBool(L1S2_IsP1WireFuseFixed[i]);
		stream->WriteBool(L1S2_IsP1WireFuseFixEmpty[i]);
	}

	//Scene_3_Foyer
	for (int i = 0; i < 5; i++)
	{
		stream->WriteBool(L1S4_P2IsImagesFound[i]);
	}
	for (int i = 0; i < 5; i++)
	{
		stream->WriteBool(L1S4_P2IsHandFixed[i]);
	}
	stream->WriteBool(L1S4_P2IsKeySetPlaced);
	stream->WriteBool(L1S4_P2IsMissingHandsPlaced);
	stream->WriteBool(L1S4_P2IsShadowPuzHandDoorOpened);
	stream->WriteBool(L1S3_IsShadowPuzzleSolved);
	stream->WriteBool(L1S4_IsShadowPuzFinalKeyTurned);
	stream->WriteBool(L1S3_IsBedroomDoorOpened);

	stream->WriteBool(L1S3_IsObj1Moved);
	stream->WriteBool(L1S3_IsObj2Moved);
	stream->WriteBool(L1S3_IsCatAttacked);
	stream->WriteBool(L1S3_IsBananaTaken);
	stream->WriteBool(L1S3_MatchBoxTaken);
	stream->WriteBool(L1S3_NagaKeyTaken);
	stream->WriteBool(L1S3_RajaTalkOver);
	stream->WriteBool(L1S3_RajaAskedForCreekLetter);
	stream->WriteBool(L1S3_RajaTakenJames);

	stream->WriteBool(L1S3_PZMoopanFlyerTaken);
	stream->WriteBool(L1S3_IsInvHOLocketTaken);
	stream->WriteBool(L1S3_IsInvShadowHand1Taken);
	stream->WriteBool(L1S3_IsP3ShadowHandFallen);
	stream->WriteBool(L1S3_IsMemoryDollTaken);
	stream->WriteBool(L1S3_PZShelfPuzzleSolved);
	for (int i = 0; i < MAX_L1S3_SHELFPUZDIFFS; i++)
	{
		stream->WriteBool(L1S3_PZShelfDifferFound[i]);
	}

	// Door HO
	stream->WriteBool(L1S3_IsDoorHOLocketPlaced);
	stream->WriteBool(L1S3_IsDoorHOLocketDoorTaken);
	stream->WriteBool(L1S3_IsDoorHOListPanelTaken);
	stream->WriteBool(L1S3_DoorHOSolved);
	stream->WriteBool(L1S3_KeyFixed);
	stream->WriteBool(L1S3_KeyDoorOpened);
	stream->WriteBool(L1S3_HOStarDoorOpened);
	stream->WriteBool(L1S3_HORoseFlowerFound);
	stream->WriteBool(L1S3_HOCrescentFound);
	stream->WriteBool(L1S3_HOSnakeFound);
	stream->WriteBool(L1S3_HOMaskFound);
	stream->WriteBool(L1S3_HOOwlFound);
	for (int i = 0; i < 3; i++)
	{
		stream->WriteBool(L1S3_HOLeavesFound[i]);
	}

	stream->WriteBool(L1S3_HOLionHeadFound);
	stream->WriteBool(L1S3_HOFishFound);
	stream->WriteBool(L1S4_HOFishTailFixed);
	for (int i = 0; i < 2; i++)
	{
		stream->WriteBool(L1S3_HOStarsFound[i]);
	}
	stream->WriteBool(L1S3_HOTreeFound);
	stream->WriteBool(L1S3_HOGoddessFound);
	stream->WriteBool(L1S3_HOTurtleFound);	
	stream->WriteBool(L1S3_HOSwapPuzzleSolved);
	stream->WriteBool(L1S3_HOMemoryPuzzleSolved);
	for (int i = 0; i < 3; i++)
	{
		stream->WriteBool(L1S3_HOLockOpened[i]);
	}
	for (int i = 0; i < 4; i++)
	{
		stream->WriteFloat(L1S3_HOSwapLeverAngle[i]);
	}

	stream->WriteBool(L1S3_HOSolved);
	stream->WriteBool(L1S3_HOCompletedWithMatch3);

	//Scene_4_Bedroom
	stream->WriteBool(L1S4_PaintingPuzzleSolved);
	stream->WriteBool(L1S4_InvPaintingFrameTaken);
	stream->WriteBool(L1S4_IsInvShadowHand2Taken);

	//Scene_5_TreasureRoom
	stream->WriteBool(L1S5_InvBoatKeyTaken);
	stream->WriteBool(L1S5_InvHammerHeadTaken);
	stream->WriteBool(L1S5_InvMortarPestleTaken);
	stream->WriteBool(L1S5_IsTreasureFireLeftLit);
	stream->WriteBool(L1S5_IsTreasureFireCenterLit);
	stream->WriteBool(L1S5_IsTreasureFireRightLit);
	stream->WriteBool(L1S5_IsHammerHandFixed);
	stream->WriteBool(L1S5_IsFireAttackVideoPlayed);
	stream->WriteBool(L1S5_IsFireAttckBanner);
	//Scene_5_HOTreasure
	for (int i = 0; i < MAX_L1S5_HOITEMS; i++)
		stream->WriteInt(L1S5_HOItemStates[i]);
	for (int i = 0; i < MAX_HOITEMS; i++)
		stream->WriteInt(L1S5_HOHUDItemStates[i]);
	stream->WriteBool(L1S5_HOP2PuzzleShuffled);
	stream->WriteBool(L1S5_HOP2PuzzleSolved);
	for (int i = 0; i < MAX_L1S5_HOP2TOKENS; i++)
		stream->WriteInt(L1S5_HOP2TokenPosContains[i]);

	stream->WriteBool(L1S5_HOClothFolded);
	stream->WriteBool(L1S5_HOBookPart2Fixed);
	stream->WriteBool(L1S5_HOBookPart3Fixed);
	stream->WriteBool(L1S5_HOBookOpened);
	stream->WriteBool(L1S5_HOHammerMoved);
	stream->WriteBool(L1S5_HOChestBoxOpened);
	stream->WriteBool(L1S5_HOSwapPuzzleSolved);
	stream->WriteBool(L1S5_HOSolved);
	stream->WriteBool(L1S5_HOCompletedWithMatch3);

	//Scene_5_TunnelDoor
	for (int i = 0; i < 3; i++)
	{
		stream->WriteBool(L1S5_P1IsNailPlucked[i]);
	}
	stream->WriteBool(L1S5_P1IsMapMovedFromPlate);
	stream->WriteBool(L1S5_P1IsBoxOpened);
	stream->WriteBool(L1S5_P1MortarPestlePlaced);
	stream->WriteBool(L1S5_P1IsCrystalAddedToGrinder);
	stream->WriteBool(L1S5_P1IsCrystalPowderCreated);
	stream->WriteBool(L1S5_P1IsCrystalPowderAddedToPlate);
	stream->WriteBool(L1S5_P1IsBottleCapRemoved);
	stream->WriteBool(L1S5_P1AmoniAddedToPlate);
	stream->WriteBool(L1S5_P1GreenPowderAddedToBottleHalf);
	stream->WriteBool(L1S5_P1GreenPowderAddedToBottleTreeForth);
	stream->WriteBool(L1S5_P1GreenPowderAddedToBottleFull);
	stream->WriteBool(L1S5_P1ThreadAddedToBottle);
	stream->WriteBool(L1S5_P1WaxCylinderAddedToBottle);
	stream->WriteBool(L1S5_IsInvBombTaken);
	stream->WriteBool(L1S5_IsInvHammerHandTaken);
	stream->WriteBool(L1S5_IsInvHammerPlacedOnScene);
	stream->WriteInt(L1S5_SelectedNailIndex);
	stream->WriteInt(L1S5_POPBombFixed);
	stream->WriteInt(L1S5_POPBombLite);
	stream->WriteInt(L1S5_IsTorchLight);
	stream->WriteInt(L1S5_IsTunnelDoorExploded);
	stream->WriteInt(L1S5_IsEscapedFromTunnel);
	stream->WriteInt(L1S5_IsEscapedFromTunnelBanner);

	stream->WriteBool(L1S4_IsCreekIntroPlayed);
	stream->WriteBool(L1S4_IsCreekDialogPlayed);
	stream->WriteBool(L1S4_IsCreekOutroPlayed);

	stream->WriteBool(L1S4_InvKeySetTaken);
	stream->WriteBool(L1S4_IsBooksOnShelfMoved);
	stream->WriteBool(L1S4_IsBookCodePlaced);
	stream->WriteBool(L1S4_IsBeetleBookTakenFromShelf);
	stream->WriteBool(L1S4_IsBeetlePlacedOnBook);
	stream->WriteBool(L1S4_IsBeetleBookOpened);

	stream->WriteBool(L1S4_IsDrawerSnakeKeyFixed);
	stream->WriteBool(L1S4_IsDrawerOpened);
	stream->WriteBool(L1S4_IsKeyFallenDown);
	stream->WriteBool(L1S4_InvDrawerDiaryTaken);
	stream->WriteBool(L1S4_IsDrawerWaxMelted);
	stream->WriteBool(L1S4_IsDrawerScrewsTakenout);
	stream->WriteBool(L1S4_IsDrawerSecretDoorOpened);
	stream->WriteBool(L1S4_InvBeetleHeadTaken);
	stream->WriteBool(L1S4_IsBeetleTailFixed);

	stream->WriteBool(L1S4_IsPhotoTurned);
	stream->WriteBool(L1S4_IsPhotoPaperTeared);
	stream->WriteBool(L1S4_InvBeetleTailTaken);
	stream->WriteBool(L1S4_InvBookCodeTaken);

	stream->WriteBool(L1S4_IsDiaryPageTurned);
	stream->WriteBool(L1S4_IsDiaryItem1Removed);
	stream->WriteBool(L1S4_IsDiaryItem2Removed);
	stream->WriteBool(L1S4_IsDiaryItem3Removed);
	stream->WriteBool(L1S4_IsDiaryItem4Removed);
	stream->WriteBool(L1S4_IsDiaryItem5Removed);
	stream->WriteBool(L1S4_IsDiaryCreekNoticeTaken);
	stream->WriteBool(L1S4_InvCoinTaken);

	stream->WriteBool(L1S4_IsSecretTreasureDoorOpened);
	stream->WriteBool(L1S4_MayaPushMaryVideoPlayed);

	//Scene_6_BoatShed
	stream->WriteBool(L1S6_HOPaperCutScenePlayed);
	stream->WriteBool(L1S6_HOSquirrelMoved);
	stream->WriteBool(L1S6_HOBeettleMoved);
	stream->WriteBool(L1S6_HOBigBoxRustRemoved);
	stream->WriteBool(L1S6_HOSmallBoxRustRemoved);
	stream->WriteBool(L1S6_HOPaperClipTaken);
	stream->WriteBool(L1S6_HOBigBoxOpened);
	stream->WriteBool(L1S6_HOPaperClipUsed);
	stream->WriteBool(L1S6_HOSmallBoxOpened);
	stream->WriteBool(L1S6_HOFunnelPiecesFixed);
	stream->WriteBool(L1S6_HOTapeAppliedOnFunnel);
	stream->WriteBool(L1S6_HOTapeCutOnFunnel);
	stream->WriteBool(L1S6_HOInvOilDripperTaken);
	stream->WriteBool(L1S6_HOInvFunnelTaken);
	stream->WriteBool(L1S6_HOLeftMiniDoorOpened);

	for (int i = 0; i < 3; i++)
	{
		stream->WriteBool(L1S6_HOIsToolMoved[i]);
	}

	for (int i = 0; i < 3; i++)
	{
		stream->WriteBool(L1S6_HOIsToolInLeftMiniBoxMoved[i]);
	}
	stream->WriteBool(L1S6_HOSolved);
	stream->WriteBool(L1S6_HOCompletedWithMatch3);

	for (int i = 0; i < MAX_HOITEMS; i++)
	{
		stream->WriteInt(L1S6_HOInvItemStates[i]);
	}


	//Scene 8 Boat Shed
	stream->WriteBool(L1S6_IsPumperPlaced); 
	stream->WriteBool(L1S6_IsRoomLite);
	stream->WriteBool(L1S6_IsWebCleaned);
	stream->WriteBool(L1S6_IsHosePlaced);
	stream->WriteBool(L1S6_IsBoxDroped);	
	stream->WriteBool(L1S6_IsCanPlaced);
	stream->WriteBool(L1S6_IsCanFilled);
	stream->WriteBool(L1S6_IsInvCrowbarPicked);	
	stream->WriteBool(L1S6_IsInvCanFullPicked);	
	stream->WriteBool(L1S6_IsBoxOpened);
	stream->WriteBool(L1S6_IsInvOilDipperTaken);

	//Scene 9 Boat Closeup
	stream->WriteBool(L1S7_IsInvPlank2Taken);
	stream->WriteBool(L1S7_IsStepsPlaced);
	stream->WriteBool(L1S7_IsRopeCut);
	stream->WriteBool(L1S7_IsDieselFilled);
	stream->WriteBool(L1S7_IsBoatKeyPlaced);
	stream->WriteBool(L1S7_IsCapOpen);
	stream->WriteBool(L1S7_IsFunnelPlaced);
	stream->WriteBool(L1S7_IsFunnelRemoved);
	stream->WriteBool(L1S7_IsCapClosed);
	stream->WriteBool(L1S7_IsBoatStarted);

	// Level 2
	stream->WriteBool(L2_S1_IsIntroSwimVideoPlayed);
	stream->WriteBool(L2_S1_InvHoneyCombPicked);
	stream->WriteBool(L2_S1_InvAloveraTaken);
	stream->WriteBool(L2_S1_InvPotWithWaterTaken);
	stream->WriteBool(L2_S1_InvScrapperTaken);
	stream->WriteBool(L2_S1_InvStoneBallTaken);
	stream->WriteBool(L2_S1_IsAloveraSliced);
	stream->WriteBool(L2_S1_IsGlovesUsed);
	stream->WriteBool(L2_S1_HoneyCombSmoked);
	stream->WriteBool(L2_S1_HoneyCombSliced);
	stream->WriteBool(L2_S1_IsNagaSculpturePlaced);
	stream->WriteBool(L2_S1_IsNagaSculptKeyTurnedOnce);
	stream->WriteBool(L2_S1_IsUndergroundHatchOpened);
	stream->WriteBool(L2_S1_IsUndergroundPopupInactive);

	stream->WriteBool(L2_S1_DuckFixed);
	stream->WriteBool(L2_S1_DuckOpened);
	stream->WriteBool(L2_S1_Box1Fixed);
	stream->WriteBool(L2_S1_Box2Fixed);
	stream->WriteBool(L2_S1_Box3Fixed);
	stream->WriteBool(L2_S1_BoxOpened);
	stream->WriteBool(L2_S1_SpearUsed);
	stream->WriteBool(L2_S1_StarFixed);
	stream->WriteBool(L2_S1_PentagonFixed);
	stream->WriteBool(L2_S1_StarKeyTurned);
	stream->WriteBool(L2_S1_PentagonKeyTurned);
	stream->WriteBool(L2_S1_PayalRemoved);
	stream->WriteBool(L2_S1_RockMoved);
	stream->WriteBool(L2_S1_ScrewFixed);
	stream->WriteBool(L2_S1_ScrewTightened);
	stream->WriteBool(L2_S1_KeyDoorLeverFixed);
	stream->WriteBool(L2_S1_SmallBoxDoorOpened);
	stream->WriteBool(L2_S1_KeyFixed);
	stream->WriteBool(L2_S1_KeyTurned);
	stream->WriteBool(L2_S1_LeftDoorOpened);
	stream->WriteBool(L2_S1_RightDoorOpened);
	stream->WriteBool(L2_S1_MirrorRemoved);
	stream->WriteBool(L2_S1_InvColorPowderTaken);
	for (int i = 0; i < 3; i++)
	{
		stream->WriteInt(L2_S1_BoxSelectedIndex[i]);
	}
	for (int i = 0; i < MAX_L2S1_HOOBJECTS; i++)
	{
		stream->WriteBool(L2_S1_HOItemsUsed[i]);
	}

	for (int i = 0; i < MAX_L2S1_HOOBJECTS; i++)
	{
		stream->WriteInt(L2S1_HOItemStates[i]);
	}

	for (int i = 0; i < MAX_HOITEMS; i++)
	{
		stream->WriteInt(L2S1_HOHUDItemStates[i]);
	}

	stream->WriteBool(L2_S2_EmberPlaced);
	stream->WriteBool(L2_S2_InvFrankinscenceTaken);
	stream->WriteBool(L2_S2_InvMangoesTaken);
	stream->WriteBool(L2_S2_PZMango1Taken);
	stream->WriteBool(L2_S2_PZMango2Taken);
	stream->WriteBool(L2_S2_PZMango3Taken);
	stream->WriteBool(L2_S2_PZStonePlaced);
	stream->WriteBool(L2_S2_PZIsMiss);


	for (int i = 0; i < MAX_L2S2_HOOBJECTS; i++)
	{
		stream->WriteBool(L2S2_HOItemsFound[i]);
	}
	stream->WriteBool(L2S2_HONagaTempleVisitedOnce);
	stream->WriteBool(L2S2_HONagaTempleCompleted);
	stream->WriteBool(L2S2_HONagaCompletedWithMatch3);

	stream->WriteBool(L2_S2_ColorPlatePlaced);
	stream->WriteBool(L2_S2_PoojaInstrPlaced);

	stream->WriteBool(L2_S2_NagaLampLite);
	stream->WriteBool(L2_S2_NagaLampLiteVideoPlayed);
	stream->WriteBool( L2_S2_CutSceneVideoPlayed);
	stream->WriteBool( L2_S2_NagaPearl1Taken);
	stream->WriteBool( L2_S2_NagaPearl2Taken);
	stream->WriteBool( L2_S2_NagaPearl3Taken);
	stream->WriteBool( L2_S2_NagaPearl4Taken);
	stream->WriteBool(L2_S2_NagaLeaf1Taken);
	stream->WriteBool(L2_S2_NagaLeaf2Taken);
	stream->WriteInt( L2_S2_pop3_R1Color);
	stream->WriteInt( L2_S2_pop3_R2Color);
	stream->WriteInt( L2_S2_pop3_R3Color);
	stream->WriteInt( L2_S2_pop3_R4Color);
	stream->WriteInt( L2_S2_pop3_R5Color);
	stream->WriteInt( L2_S2_pop3_R6Color);
	stream->WriteInt( L2_S2_pop3_R7Color);
	stream->WriteInt( L2_S2_pop3_R8Color);
	stream->WriteInt( L2_S2_pop3_R9Color);
	stream->WriteInt( L2_S2_pop3_R10Color);
	stream->WriteInt( L2_S2_pop3_R11Color);
	stream->WriteInt( L2_S2_pop3_R12Color);
	stream->WriteInt( L2_S2_pop3_R13Color);
	stream->WriteInt( L2_S2_pop3_R14Color);
	stream->WriteBool( L2_S2_kolamComplete);

	stream->WriteBool(L2_S3_Pop1_FirstDigPlayed);
	stream->WriteBool(L2_S3_Pop1_SecondDigPlayed);
	stream->WriteBool(L2_S3_Pop1_PluckingTurmericPlayed);
	stream->WriteBool(L2_S3_Pop1_FrogJumpPlayed);
	stream->WriteBool(L2_S3_InvTurmericPicked);
	stream->WriteBool(L2_S3_InvPoojaLampPicked);

	stream->WriteBool(L2_S3_isStoneBeamPuzzleDataSaved);
	for (int i = 0; i < 10; i++)
	{
		stream->WriteInt(L2_S3_PZStoneBeamCircleRotation[i]);
		stream->WriteFloat(L2_S3_PZStoneBeamCircleXPos[i]);
		stream->WriteFloat(L2_S3_PZStoneBeamCircleYPos[i]);
		stream->WriteInt(L2_S3_PZStoneBeamCircleCurrentPos[i]);
		stream->WriteInt(L2_S3_PZStoneBeamCircleOriginalPos[i]);
	}
	stream->WriteBool(L2_S3_StonePathActivated);
	stream->WriteBool(L2_S3_IsStonePathActivatedMessageShown);
	stream->WriteBool(L2_S2_InvNagaPearlsCollected);

	stream->WriteBool(L2_S4_IsWoodsCut1);
	stream->WriteBool(L2_S4_IsWoodsCut2);
	stream->WriteBool(L2_S4_InvFirewoodTaken);
	stream->WriteBool(L2_S4_InvAxeBluntTaken);
	stream->WriteBool(L2_S4_IsAxeSharpened);
	stream->WriteBool(L2_S4_InvAxeSharpenTaken);
	stream->WriteBool(L2_S4_InvFireTorchTaken);
	stream->WriteBool(L2_S4_InvFireTorchStickOiledTaken);
	stream->WriteBool(L2_S4_IsLasyaAttackVideoPlayed);
	stream->WriteBool(L2_S4_FireTorchOiled);
	stream->WriteBool(L2_S4_RockSlabMoved1);
	stream->WriteBool(L2_S4_BrickPuzzleAreaClicked);
	stream->WriteBool(L2_S4_RockSlabRemoved);
	stream->WriteBool(L2_S4_InvFireTorchFlamed);
	stream->WriteBool(L2_S4_InvRakeTaken);
	stream->WriteInt(L2_S4_DeitiesPuzzleState);
	stream->WriteBool(L2_S4_InvMetalLeverTaken);
	for (int i = 0; i < MAX_L2_S4_DEITIESTILES; i++)
	{
		stream->WriteInt(L2_S4_DeitiesTileIndices[i]);
	}


	stream->WriteBool(L2_S5_IsVisited);
	stream->WriteBool(L2_S5_InvKnifeTaken);
	stream->WriteBool(L2_S5_InvAmberTaken);
	stream->WriteBool(L2_S5_InvPotTaken);

	stream->WriteBool(L2_S5_VinegarPlaced);
	stream->WriteBool(L2_S5_VinegarCorkOpened);
	stream->WriteBool(L2_S5_KnifeSharpened);
	stream->WriteBool(L2_S5_KnifeSharpTaken);

	stream->WriteBool(L2_S5_MangoPlaced);
	stream->WriteBool(L2_S5_MangoSliced);
	stream->WriteBool(L2_S5_MangoAddedToPot);
	stream->WriteBool(L2_S5_HoneyWaxPlaced);
	stream->WriteBool(L2_S5_HoneyWaxSliced);
	stream->WriteBool(L2_S5_HoneyWaxAddedToPot);
	stream->WriteBool(L2_S5_MangoHoneyLiquidCreated);
	stream->WriteBool(L2_S5_MangoHoneyLiquidApplied);
	stream->WriteBool(L2_S5_AloveraPlaced);
	stream->WriteBool(L2_S5_AloveraSliced);
	stream->WriteBool(L2_S5_AloveraApplied);
	stream->WriteBool(L2_S5_WoodPlaced);
	stream->WriteBool(L2_S5_WoodFired);
	stream->WriteBool(L2_S5_PuzzleModeActive);
	stream->WriteBool(L2_S5_PotWithWaterPlaced);
	stream->WriteBool(L2_S5_TurmericPlaced);
	stream->WriteBool(L2_S5_TurmericSliced);
	stream->WriteBool(L2_S5_TurmericAddedToPot);
	stream->WriteBool(L2_S5_TurmericPowderCreated);
	stream->WriteBool(L2_S5_TurmericPowderApplied);
	stream->WriteBool(L2_S5_MedicineCreated);
	stream->WriteBool(L2_S5_MedicineMixed);
	stream->WriteBool(L2_S5_MedicineGivenToMooppan);
	stream->WriteBool(L2_S5_ClickedOnMooppan);
	stream->WriteBool(L2_S5_InvPujaChartTaken);
	stream->WriteBool(L2_S5_InvNagaSculptureTaken);
	stream->WriteBool(L2_S5_ShelfPuzzleSolved);
	stream->WriteInt(L2_S5_EagleBlockPuzzleState);
	for (int i = 0; i < MAX_L2_S5_EAGLEBLOCKS; i++)
	{
		stream->WriteInt(L2_S5_EagleBlockIndices[i]);
	}
	stream->WriteBool(L2_S5_StrawBasketRemoved);
	stream->WriteBool(L2_S5_InvVinegarTaken);
	stream->WriteBool(L2_S5_InvStoneSharpnerTaken);
	stream->WriteBool(L2_S5_InvMedicineTaken);


	stream->WriteBool(L2S6_InvGlovesTaken);
	stream->WriteBool(L2S6_InvRustedKnifeTaken);
	stream->WriteBool(l2S6_Inv3XStonesTaken);
	stream->WriteBool(L2S6_Pop1Visited);
	stream->WriteBool(L2S6_HOWaterFallCompleted);
	stream->WriteBool(L2S6_HOWaterFallCompletedWithMatch3);
	for (int i = 0; i < MAX_L2S6_HOOBJECTS; i++)
	{
		stream->WriteBool(L2S6_HOItemsFound[i]);
	}

	stream->WriteBool(L2S6_InvSmallStone1Taken);
	stream->WriteBool(L2S6_InvSmallStone2Taken);
	stream->WriteBool(L2S6_InvSmallStone3Taken);
	stream->WriteBool(L2_S6_IsWoodCutDown);
	stream->WriteBool(L2_S6_IsP2LocketOpen);
	stream->WriteBool(L2_S6_IsP3HolderOpen);
	stream->WriteBool(L2_S6_IsP3PaperPuzClueTaken);
	//3 saved number Ids of holder
	stream->WriteInt(L2_S6_P3CenterNums[0]);
	stream->WriteInt(L2_S6_P3CenterNums[1]);
	stream->WriteInt(L2_S6_P3CenterNums[2]);
	stream->WriteBool(L2_S6_NagaPearlFixed);
	stream->WriteBool(L2_S6_Shutter1Solved);
	stream->WriteBool(L2_S6_Shutter2Solved);
	stream->WriteBool(L2_S6_Shutter3Solved);
	stream->WriteBool(L2_S6_ClockPuzzleSolved);
	stream->WriteBool(L2_S6_PaperPuzCluePlaced);
	stream->WriteBool(L2_S6_IsP6OilLampLightened);
	stream->WriteBool(L2_S6_Rock1Moved);
	stream->WriteBool(L2_S6_Rock2Moved);
	stream->WriteBool(L2_S6_LeverUsed);
	stream->WriteBool(L2S6_InvStoneBallPlaced);

	stream->WriteBool(L2_S7_Puzzle1Solved);
	stream->WriteBool(L2_S7_Puzzle2Solved);
	stream->WriteBool(L2_S7_Puzzle3Solved);
	stream->WriteBool(L2_S7_Puzzle4Solved);
	stream->WriteBool(L2_S7_Puzzle5Solved);
	stream->WriteBool(L2_S7_Puzzle6Solved);
	stream->WriteBool(L2_S7_Puzzle7Solved);
	stream->WriteInt(L2_S7_SoldierPuzzleState);
	stream->WriteInt(L2_S7_CurrentPuzzle);

	//Level 3
	stream->WriteBool(L3_S1_Visited);
	for (int i = 0; i < 12; i++)
	{
		stream->WriteBool(L3_S1_Objectives[i]);
	}

	stream->WriteBool(L3_S1_InvFlagFullTaken);
	stream->WriteBool(L3_S1_InvFlagStickTaken);
	stream->WriteBool(L3_S1_InvFlagClothTaken);
	stream->WriteInt(L3_S1_EmberPopupState);
	stream->WriteBool(L3_S1_InvLadderStep1Taken);
	stream->WriteBool(L3_S1_InvGreenCrystalTaken);
	stream->WriteBool(L3_S1_LadderPiecesFixed);
	stream->WriteBool(L3_S1_LadderBrokenStepFixed);

	for (int i = 0; i < MAX_L3S1_HOOBJECTS; i++)
		stream->WriteBool(L3_S1_HOObjFound[i]);
	for (int i = 0; i < MAX_HUDL3S1_SYMBOLS; i++)
		stream->WriteInt(L3_S1_HOHudObjID[i]);
	stream->WriteInt(L3_S1_HONextHudSymbolID);
	stream->WriteBool(L3_S1_HOHudSymbolsInitialized);
	stream->WriteBool(L3_S1_HOLadderCompleted);
	stream->WriteBool(L3_S1_HOLadderCompletedWithMatch3);
	stream->WriteBool(L3_S1_InvBrokenLadderTaken);
	stream->WriteBool(L3_S1_InvLadderFixedTaken);

	stream->WriteBool(L3_S1_P1LionHeadPuzzleCompleted);
	stream->WriteBool(L3_S1_invGreenEyeCollected);
	for (int i = 0; i < 4; i++)
	{
		stream->WriteFloat(L3_S1_P1LionHeadRingAngles[i]);
	}
	stream->WriteBool(L3_S2_InvCocoShellPowderFilledTaken);
	stream->WriteBool(L3_S2_InvTwineTaken);

	stream->WriteInt(L3_S2_MazeCurrentRoom);
	stream->WriteBool(L3_S2_MazeMapUsed);

	stream->WriteBool(L3_S2_InvSickleTaken);
	stream->WriteBool(L3_S2_RockMoved);
	stream->WriteBool(L3_S2_TwineReleased);
	stream->WriteBool(L3_S2_Thread1Taken);
	stream->WriteBool(L3_S2_Thread2Taken);
	stream->WriteBool(L3_S2_Thread3Taken);
	stream->WriteBool(L3_S2_Thread4Taken);

	stream->WriteBool(L3_S2_InvCoconutOuterRemoved);
	stream->WriteBool(L3_S2_InvCoconutShellTaken);
	stream->WriteBool(L3_S2_CoconutComboTwineFixed);
	stream->WriteBool(L3_S2_InvCoconutWithTwineTaken);
	stream->WriteBool(L3_S2_CrowFlewAway);
	stream->WriteBool(L3_S2_PoolDoorOpened);

	stream->WriteBool(L3_S2_InvHammerTaken);
	stream->WriteBool(L3_S2_InvHammerHandTaken);
	stream->WriteBool(L3_S2_ToothFixed);
	stream->WriteBool(L3_S2_BackDoorOpened);

	stream->WriteBool(L3_S2_InvCoconnutTaken);
	stream->WriteBool(L3_S2_LeafMoved);
	stream->WriteBool(L3_S2_Stick1Moved);
	stream->WriteBool(L3_S2_Stick2Moved);

	stream->WriteBool(L3_S2_BrickTaken);
	stream->WriteBool(L3_S2_CocoShellPowderFilledFixed);
	stream->WriteBool(L3_S2_TwineGunPowderFixed);
	stream->WriteBool(L3_S2_BrickFixed);
	stream->WriteBool(L3_S2_WallBrocken);

	for (int i = 0; i < 3; i++)
		stream->WriteBool(L3_S2_vineRemovedR1Status[i]);

	stream->WriteBool(L3_S2_InvPipe1R1Taken);

	for (int i = 0; i < 3; i++)
		stream->WriteBool(L3_S2_vineRemovedR2Status[i]);

	for (int i = 0; i < 2; i++)
		stream->WriteBool(L3_S2_R2RemovedMudOnPipe[i]);

	stream->WriteBool(L3_S2_InvPipe2R2Taken);
	stream->WriteBool(L3_S2_InvToothR2Taken);
	stream->WriteInt(L3_S2_P1PlayerTile);
	for (int i = 0; i < 4; i++)
		stream->WriteInt(L3_S2_P1CurrentTiles[i]);

	for (int i = 0; i < 3; i++)
		stream->WriteBool(L3_S2_R3DoorOpen[i]);

	stream->WriteBool(L3_S2_InvPipe3R3Taken);
	stream->WriteBool(L3_S2_InvHammerHeadTaken);
	stream->WriteBool(L3_S2_R3StonesRemoved);

	stream->WriteBool(L3_S2_M4DoorsOpened);
	stream->WriteBool(L3_S2_InvPipe4R4Taken);
	for (int i = 0; i < 8; i++)
	{
		stream->WriteBool(L3_S2_M4SliderLeftRemoved[i]);
	}
	for (int i = 0; i < 8; i++)
	{
		stream->WriteBool(L3_S2_M4SliderRightRemoved[i]);
	}

	stream->WriteBool(L3_S2_M5_PipePuzzleActivated);
	for (int i = 0; i < MAX_PIPE_INDEX; i++)
	{
		stream->WriteInt(L3_S2_M5_PipeGridIndex[i]);
	}

	for (int i = 0; i < MAX_PIPE_INDEX; i++)
	{
		stream->WriteInt(L3_S2_M5_PipeAngles[i]);
	}

	stream->WriteBool(L3_S2_M5_PipePuzzleCompleted);
	stream->WriteBool(L3_S2_M5_PipeSceneDoorsOpend);

	stream->WriteInt(L3_S2_M6_SceneState);
	stream->WriteBool(L3_S2_M6_PillerPuzzleComplete);
	for (int i = 0; i < 3; i++)
	{
		stream->WriteInt(L3_S2_M6_PillerPositions[i]);
	}

	for (int i = 0; i < 2; i++)
	{
		stream->WriteInt(L3_S2_M6_CollectedBoxIndex[i]);
	}	

	stream->WriteBool(L3_S3_VisitingPoolForFirsttime);
	stream->WriteBool(L3_S3_IsDoorToS4Opened);
	stream->WriteBool(L3_S3_PZBlocksPlaced);
	stream->WriteBool(L3_S3_PZUnderwaterPuzzleSolved);
	stream->WriteBool(L3_S3_PZGoingUnderwaterFirstTime);
	stream->WriteBool(L3_S3_IsTempleRisenFromWater);
	stream->WriteBool(L3_S3_InvLadderStep2Taken);
	stream->WriteBool(L3_S3_InvCupFilledTaken);
	stream->WriteBool(L3_S3_InvCupTaken);
	stream->WriteBool(L3_S3_IsTempleGlassBroken);
	stream->WriteBool(L3_S3_InvTempleBoxTaken);
	stream->WriteBool(L3_S3_InvSnakeDoorPartTaken);
	stream->WriteBool(L3_S3_InvSawTaken);
	stream->WriteBool(L3_S3_InvShowelTaken);
	stream->WriteBool(L3_S3_IsRockMovedSaw1);
	stream->WriteBool(L3_S3_IsRockMovedSaw2);
	stream->WriteBool(L3_S3_IsRootLeftSawed);
	stream->WriteBool(L3_S3_IsRootRightSawed);
	stream->WriteBool(L3_S3_IsSecretBoxOpened);
	for (int i = 0; i < 8; i++)
	{
		stream->WriteInt(L3_S3_PZBlockTileIndex[i]);
	}
	stream->WriteInt(L3_S3_PZBlockState);

	for (int i = 0; i < 8; i++)
	{
		stream->WriteBool(L3_S3_PZPinkOuterKeyState[i]);
		stream->WriteBool(L3_S3_PZRedOuterKeyState[i]);
		stream->WriteBool(L3_S3_PZBlueOuterKeyState[i]);
	}
	for (int i = 0; i < 8; i++)
	{
		stream->WriteBool(L3_S3_PZPinkDiagonalKeyState[i]);
		stream->WriteBool(L3_S3_PZRedDiagonalKeyState[i]);
		stream->WriteBool(L3_S3_PZBlueDiagonalKeyState[i]);
	}
	for (int i = 0; i < 4; i++)
	{
		stream->WriteBool(L3_S3_PZPinkInnerKeyState[i]);
		stream->WriteBool(L3_S3_PZRedInnerKeyState[i]);
		stream->WriteBool(L3_S3_PZBlueInnerKeyState[i]);
	}

	for (int i = 0; i < 16; i++)
	{
		stream->WriteBool(L3_S4_HOFortFountainFound[i]);
	}

	stream->WriteBool(L3_S4_HOFortFountainCompleted);

	stream->WriteBool(L3_S4_IsVisitedOnce);
	stream->WriteBool(L3_S4_InvSharpRodTaken);
	stream->WriteBool(L3_S4_InvBambooStickTaken);
	stream->WriteBool(L3_S4_InvMetalSheetBlankTaken);
	stream->WriteBool(L3_S4_InvMapDrawnSheetTaken);
	stream->WriteBool(L3_S4_IsLeaf1OverMetalRemoved);
	stream->WriteBool(L3_S4_IsLeaf2OverMetalRemoved);
	stream->WriteBool(L3_S4_IsAmeehaDiaryNoteTaken);
	stream->WriteBool(L3_S4_IsAmeehaCutscenePlayed);
	stream->WriteBool(L3_S4_IsNagaLockPlaced);
	stream->WriteBool(L3_S4_IsCliffDoorOpened);
	stream->WriteBool(L3_S4_IsPop2AMapDoorOpened);
	stream->WriteBool(L3_S4_IsPop2ABlankSheetPlaced);
	stream->WriteBool(L3_S4_IsPop2AStatueMoved);
	stream->WriteBool(L3_S4_IsPop2ASnakeButtonPressed);
	stream->WriteBool(L3_S4_IsPop2BLeaf1Removed);
	stream->WriteBool(L3_S4_IsPop2BLeaf2Removed);
	stream->WriteBool(L3_S4_IsPop2BStoneRemoved);
	stream->WriteBool(L3_S4_IsPop2BSnakeButtonPressed);
	stream->WriteBool(L3_S4_IsPop2CSnakeButtonPressed);
	stream->WriteBool(L3_S4_IsPop2CStoneRemoved);
	stream->WriteBool(L3_S4_IsPop2CLeavesRemoved);
	stream->WriteBool(L3_S4_IsPop2DSnakeButtonPressed);
	stream->WriteBool(L3_S4_IsPop2DStoneRemoved);
	stream->WriteBool(L3_S4_IsPop2DLeavesRemoved);
	stream->WriteBool(L3_S4_IsPop4StoneRemoved);
	stream->WriteBool(L3_S4_IsPop4LeavesRemoved);
	stream->WriteBool(L3_S4_InvBambooWithSickleTaken);

	stream->WriteBool(L3_S5_InvRopeTaken);
	stream->WriteBool(L3_S5_InvClipTaken);
	stream->WriteBool(L3_S5_GreenStonePlaced);
	stream->WriteBool(L3_S5_DeerGone);
	stream->WriteBool(L3_S5_ClipPlaced);
	stream->WriteBool(L3_S5_RopeTied);
	stream->WriteBool(L3_S5_NailPlaced);
	stream->WriteBool(L3_S5_NailHammered);

	stream->WriteBool(L3_S5_DryGrassTaken1);
	stream->WriteBool(L3_S5_DryGrassTaken2);
	stream->WriteBool(L3_S5_DryGrassTaken3);
	stream->WriteBool(L3_S5_DryGrassTaken4);
	stream->WriteBool(L3_S5_DryGrassTaken5);
	stream->WriteBool(L3_S5_DryGrassTaken6);
	stream->WriteBool(L3_S5_DryGrassTaken7);
	stream->WriteBool(L3_S5_DryGrassTaken8);
	stream->WriteBool(L3_S5_DryGrassTaken9);
	stream->WriteBool(L3_S5_DryGrassTaken10);
	stream->WriteBool(L3_S5_DryGrassTaken11);

	stream->WriteBool(L3_S5_InvGrassTaken1);
	stream->WriteBool(L3_S5_InvGrassTaken2);
	stream->WriteBool(L3_S5_InvGrassTaken3);
	stream->WriteBool(L3_S5_InvGrassTaken4);
	stream->WriteBool(L3_S5_InvGrassTaken5);
	stream->WriteBool(L3_S5_InvGrassTaken6);

	stream->WriteBool(L3_S5_InvAllGrassTaken);

	//Level 4
	stream->WriteBool(L4_S1_IsFirstVisit);
	stream->WriteBool(L4_S1_PocketOpend);
	stream->WriteBool(L4_S1_InvHandKerchiefTaken);
	stream->WriteBool(L4_S1_InvCrutchTaken);
	stream->WriteBool(L4_S1_SkimmerFlyed);
	stream->WriteBool(L4_S1_InvStickTaken);
	stream->WriteBool(L4_S1_InvLeafTaken);
	stream->WriteBool(L4_S1_InvLeafSpoonWithWaterTaken);
	stream->WriteBool(L4_S1_InvHerbalLeavesTaken);
	stream->WriteBool(L4_S1_InvBraceletTaken);
	stream->WriteBool(L4_S1_FirstThornRemoved);
	stream->WriteBool(L4_S1_SecondThornRemoved);
	stream->WriteBool(L4_S1_ShoeRemoved);
	stream->WriteBool(L4_S1_BloodRemoved);
	stream->WriteBool(L4_S1_MedPasteApplied);
	stream->WriteBool(L4_S1_WoundTied);
	stream->WriteBool(L4_S1_PutShoeBack);
	stream->WriteBool(L4_S1_TwigsMoved);
	stream->WriteBool(L4_S1_InvLeafSpoonCollected);
	stream->WriteBool(L4_S1_LeafTored);
	stream->WriteBool(L4_S1_HandKerchiefTored);
	stream->WriteBool(L4_S1_PutLeftShoe);
	stream->WriteBool(L4_S1_CrutchUsed);

	stream->WriteBool(L4_S1_PuzzleSolved);
	stream->WriteBool(L4_S1_InvBinocularLensTaken);
	stream->WriteBool(L4_S1_InvLeftShoeTaken);
	stream->WriteBool(L4_S1_PuzzleVisited);
	for (int i = 0; i < 12; i++)
	{
		stream->WriteBool(L4_S1_HOShoeFound[i]);
	}

	stream->WriteBool(L4_S1_HOShoeCompleted);
	stream->WriteBool(L4_S1_BetelBoxOPened);
	stream->WriteBool(L4_S1_InvKnifeCollected);

	stream->WriteBool(L4_S2_FruitsPlucked);
	stream->WriteBool(L4_S2_FruitsTaken);
	stream->WriteBool(L4_S2_AlligatorPuzzleSolved);
	stream->WriteInt(L4_S2_AlligatorPuzzleState);
	stream->WriteBool(L4_S2_BambooStickTaken);
	stream->WriteBool(L4_S2_Pop2BambooTaskCompleted);
	stream->WriteBool(L4_S2_GrassTaken);
	stream->WriteBool(L4_S2_ComboKnifeAddedToBamboo);
	stream->WriteBool(L4_S2_InvBambooSpearTaken);
	stream->WriteBool(L4_S2_InvRopeTaken);
	stream->WriteBool(L4_S2_InvGrinderHandleTaken);
	stream->WriteBool(L4_S2_DoorToCaveOpened);
	stream->WriteInt(L4_S2_FanPuzzleState);
	stream->WriteInt(L4_s2_GrassRopeFrameNumber);
	stream->WriteBool(L4_S2_MortarTaken);

	stream->WriteBool(L4_S3_IsCaveLit );
	stream->WriteBool(L4_S3_IsDoorPuzzleSolved);
	stream->WriteBool(L4_S3_IsBowPlaced);
	stream->WriteBool(L4_S3_IsEmberCreated);
	stream->WriteBool(L4_S3_IsOilApplied);
	stream->WriteBool(L4_S3_IsFireSticksPlaced);
	stream->WriteBool(L4_S3_IsCoinPlaced);
	stream->WriteBool(L4_S3_IsPlankPlaced);
	stream->WriteBool(L4_S3_IsInvBowlTaken);
	stream->WriteBool(L4_S3_IsMetalPaperTaken);
	stream->WriteBool(L4_S3_IsPotRemoved);

	stream->WriteBool(L4_S4_Visited);
	stream->WriteBool(L4_S4_SkeletonHandOpened);
	stream->WriteBool(L4_S4_InvBinocularTaken);
	stream->WriteBool(L4_S4_FlowerCutscenePlayed);
	stream->WriteBool(L4_S4_GlobGlowEnabled);
	stream->WriteBool(L4_S4_GrinderHandlePlaced);
	stream->WriteBool(L4_S4_OlivePlaced);
	stream->WriteBool(L4_S4_StrainerPlaced);
	stream->WriteBool(L4_S4_PotPlaced);
	stream->WriteBool(L4_S4_InstructionsPlaced);
	stream->WriteBool(L4_S4_OilMachinePlaced);
	stream->WriteBool(L4_S4_OilMachineRotated);
	stream->WriteBool(L4_S4_PotWithOilTaken);
	stream->WriteBool(L4_S4_InvBinocularWithLensTaken);
	stream->WriteBool(L4_S4_JumpCutscenePlayed);
	stream->WriteBool(L4_S4_ClimbCutscenePlayed);

	stream->WriteBool(L4_S2_InvTwig1Taken);
	stream->WriteBool(L4_S4_InvTwig2Taken);
	stream->WriteBool(L4_S5_InvTwig3Taken);
	stream->WriteBool(L4_S5_InvAlligatorHeadTaken);
	stream->WriteBool(L4_S5_InvDoorKeyTaken);
	stream->WriteBool(L4_S5_SwapPuzzleCompleted);


	for (int i = 0; i < 6; i++)
	{
		stream->WriteBool(L4_S4_HOBinocularFound[i]);
	}

	stream->WriteBool(L4_S4_HOBinocularCompleted);
	stream->WriteBool(L4_S4_BannerTextAfterBinocularHOShown);
	stream->WriteBool(L4_S2_InvSmallBambooStick0Taken);
	stream->WriteBool(L4_S2_InvSmallBambooStick1Taken);
	stream->WriteBool(L4_S2_InvSmallBambooStick2Taken);
	stream->WriteBool(L4_S2_InvSmallBambooStick3Taken);
	stream->WriteBool(L4_S2_IsBambooStickCut);

	stream->WriteBool(L4_S5_Visited);
	stream->WriteBool(L4_S5_InvDoorTaken);
	stream->WriteBool(L4_S5_ScrewRemoved);
	stream->WriteBool(L4_S5_ClothRemoved);
	stream->WriteBool(L4_S5_InvOlivesCollected);
	stream->WriteBool(L4_S5_OilMachineBaseCollected);
	stream->WriteBool(L4_S5_InvNetTaken);
	stream->WriteBool(L4_S5_ComboBambooFrameAddedToNet);
	stream->WriteBool(L4_S5_InvBambooStrainerTaken);
	stream->WriteBool(L4_S5_DoorPlaced);

	//Level5
	stream->WriteBool(L5_S1_InvBowTaken);
	stream->WriteBool(L5_S1_InvBowWithArrowFirstTaken);
	stream->WriteBool(L5_S1_InvBowWithArrowSecondTaken);
	stream->WriteBool(L5_S1_InvArrowHolderTaken);
	stream->WriteBool(L5_S1_BowStringFixed);
	stream->WriteBool(L5_S1_BowArrowHolderPlaced);
	stream->WriteBool(L5_S1_BowArrowFirstLoaded);
	stream->WriteBool(L5_S1_BowArrowSecondLoaded);
	stream->WriteBool(L5_S1_BowToTigerCutscenePlayed);

	stream->WriteBool(L5_S2_InvHammerTaken);
	stream->WriteBool(L5_S2_InvSoldierBadgeTaken);

	for (int i = 0; i < MAX_L5S2_NUM_BLOCKS; i++)
		stream->WriteInt(L5_S2_PZBlockStartRow[i]);
	for (int i = 0; i < MAX_L5S2_NUM_BLOCKS; i++)
		stream->WriteInt(L5_S2_PZBlockStartCol[i]);

	stream->WriteBool(L5_S2_PZBlocksPuzzleSolved);

	stream->WriteBool(L5_S2_InvBadgeTaken);
	stream->WriteBool(L5_S2_LogPushed);
	stream->WriteBool(L5_S2_LogFell);
	stream->WriteBool(L5_S2_GreaseTaken);

	stream->WriteBool(L5_S3_IntroVdoCompleted);
	stream->WriteBool(L5_S3_memClothTaken);
	stream->WriteBool(L5_S3_EmblemPlacedOnStone);
	stream->WriteBool(L5_S3_PaperCollectedFromStone);
	stream->WriteBool(L5_S3_InvMoonSolutionTaken);
	stream->WriteBool(L5_S3_VineFreed);
	stream->WriteBool(L5_S3_isHoActive);
	stream->WriteBool(L5_S3_isLostHook);
	stream->WriteBool(L5_S3_PZHaveHook);
	stream->WriteBool(L5_S3_HookLostBannerShowed);
	stream->WriteBool(L5_S3_LeftDoorOpened);
	stream->WriteBool(L5_S3_EnteredLeftDoor);
	stream->WriteInt(L5_S3_PZState);
	for (int i = 0; i < 16; i++)
	{
		stream->WriteInt(L5_S3_PZLocketIndexes[i]);
	}
	stream->WriteBool(L5_S3_PZCompleted);
	stream->WriteBool(L5_S3_HOUnderWaterStarRemoved);
	for (int i = 0; i < 12; i++)
	{
		stream->WriteBool(L5_S3_HOUnderWaterFoundObjs[i]);
	}
	stream->WriteBool(L5_S3_HOUnderWaterCompleted);


	stream->WriteBool(L5_S4_InvPickaxeTaken);
	for (int i = 0; i < 6; i++)
	{
		stream->WriteBool(L5_S4_KeyMatchComplete[i]);
	}
	stream->WriteBool(L5_S4_Level1Completed);
	stream->WriteInt(L5_S4_PZHandCuffState);
	stream->WriteBool(L5_S4_PZHandCuffComplete);
	for (int i = 0; i < 3; i++)
	{
		stream->WriteBool(L5_S4_PZHandCuffLockUnlocked[i]);	
	}
	stream->WriteBool(L5_S4_InvPickaxeHeadTaken);
	stream->WriteBool(L5_S4_GreaseUsed);
	stream->WriteBool(L5_S4_Stone1Moved);
	stream->WriteBool(L5_S4_Stone2Moved);
	stream->WriteBool(L5_S4_BridgeOpened);
	stream->WriteBool(L5_S4_InvBarkTaken);
	stream->WriteBool(L5_S4_PickAxeUsed);
	stream->WriteBool(L5_S4_PickAxeCutScenePlayed);
	stream->WriteBool(L5_S4_AnasuyaTalked);
	stream->WriteBool(L5_S4_InvChainTaken);
	stream->WriteBool(L3_S4_PZ1BridgeCompleted);
	for (int i = 0; i < 5; i++)
	{
		stream->WriteInt(L3_S4_PZ1BridgeRingAngles[i]);
	}

	stream->WriteBool(L5_S5_InvPickaxeHandleTaken);
	stream->WriteBool(L5_S5_InvHandcuffKeyTaken);
	stream->WriteBool(L5_S5_InvWaterproofBagTaken);
	stream->WriteBool(L5_S5_InvBambooLadderTaken);
	stream->WriteBool(L5_S5_InvStoneEmblemTaken);
	stream->WriteBool(L5_S5_IsAmeehaDiaryCutscenePlayed);
	stream->WriteBool(L5_S5_IsSoldierBadgeUsedOnDoor);
	stream->WriteBool(L5_S5_IsBambooLadderPlacedOnScene);
	stream->WriteBool(L5_S5_IsMoonGlassDoorOpened);
	stream->WriteBool(L5_S5_IsTopDoorOpened);
	stream->WriteBool(L5_S5_IsP1Stone1Removed);
	stream->WriteBool(L5_S5_IsP1Stone2Removed);
	stream->WriteBool(L5_S5_IsP1PillerRemoved);

	for (int i = 0; i < 14; i++)
	{
		stream->WriteBool(L5_S5_HOGanpathiFoundObjs[i]);
	}
	stream->WriteBool(L5_S5_HOGanpathiCompleted);
	stream->WriteBool(L5_S5_HOGanapathiActivated);

	for (int i = 0; i < MAX_L5S5_PZBLOCKS; i++)
	{
		stream->WriteInt(L5_S5_PZTokenState[i]);
	}
	stream->WriteBool(L5_S5_PZSolutionPaperPlaced);
	stream->WriteBool(L5_S5_PZStatesInitialized);
	stream->WriteBool(L5_S5_IsSunClockPuzzleCompleted);

	stream->WriteBool(L5_S6_HOSoldierBadgePlaced);
	stream->WriteBool(L5_S6_HOGuardianIntroTalkCompleted);
	for (int i = 0; i < MAX_L5S6_HOOBJECTS; i++)
	{
		stream->WriteBool(L5_S6_HOObjectFound[i]);
	}
	stream->WriteBool(L5_S6_HOCompleted);
	stream->WriteBool(L5_S6_PeacockChainUsed);
	stream->WriteBool(L5_S6_InvPeacockLocketTaken);
	stream->WriteBool(L5_S6_PeacockLocketUsed);
	stream->WriteBool(L5_S6_PeacockArrivingCutScenePlayed);
	stream->WriteBool(L5_S6_PeacockLeavingCutScenePlayed);
	stream->WriteBool(L5_S6_IntroCutScenePlayed);
	stream->WriteBool(L5_S6_GotFreeBannerShown);

	//level 6

	stream->WriteBool(L6_S1_Visited);
	stream->WriteBool(L6_S1_ChappalKnobRemoved);
	stream->WriteBool(L6_S1_LetterOpened);
	stream->WriteBool(L6_S1_VaruniCutscenePlayed);
	stream->WriteBool(L6_S1_MaryFirstBannerTextShown);
	stream->WriteBool(L6_S1_MarySecondBannerTextShown);
	stream->WriteBool(L6_S1_ScrewUnsealed);
	stream->WriteBool(L6_S1_InvEmblemTaken);
	stream->WriteBool(L6_S1_InvMortarTaken);
	stream->WriteBool(L6_S1_InvSteelTaken);
	stream->WriteBool(L6_S1_SolutionFound);
	stream->WriteBool(L6_S1_InvClothTaken);
	stream->WriteBool(L6_S1_StarKeyPlaced);
	stream->WriteBool(L6_S1_EmblemPlaced);
	stream->WriteBool(L6_S1_PathToS3Activated);
	stream->WriteBool(L6_S1_InvClubTaken);
	stream->WriteBool(L6_S1_TextForHOShown);

	for (int i = 0; i < MAX_L6S1_HOSCENES; i++)
	{
		stream->WriteBool(L6_S1_SceneHOSolved[i]);
	}
	stream->WriteBool(L6_S1_DescriptionHOSolved);

	for (int i = 0; i < 4; i++)
		stream->WriteBool(L6_S1_DescHOFound_S1[i]);
	for (int i = 0; i < 2; i++)
		stream->WriteBool(L6_S1_DescHOFound_S2[i]);
	for (int i = 0; i < 3; i++)
		stream->WriteBool(L6_S1_DescHOFound_S3[i]);
	for (int i = 0; i < 4; i++)
		stream->WriteBool(L6_S1_DescHOFound_S4[i]);
	for (int i = 0; i < 2; i++)
		stream->WriteBool(L6_S1_DescHOFound_S5[i]);
	stream->WriteBool(L6_S1_InvHorseShoeTaken);
	stream->WriteBool(L6_S1_LetterTaken);
	stream->WriteBool(L6_S1_ChestPuzzleSolved);

	stream->WriteBool(L6_S2_DoorToS3Opened);
	stream->WriteBool(L6_S2_InvIceKeyTaken);
	stream->WriteBool(L6_S2_InvPestleTaken);
	stream->WriteBool(L6_S2_ColoredPowderTrowed);

	stream->WriteBool(L6_S2_HOComplete);
	for (int i = 0; i < 7; i++)
	{
		stream->WriteBool(L6_S2_HOItemsFound[i]);
	}
	stream->WriteBool(L6_S2_InvPuzzlePiecesCollected);
	stream->WriteBool(L6_S2_PuzzleComplete);
	stream->WriteBool(L6_S2_InvPanTaken);
	stream->WriteBool(L6_S2_InvPanWithPinePitchPlaced);
	stream->WriteBool(L6_S2_InvStickPlaced);
	stream->WriteBool(L6_S2_InvFlintPlaced);
	stream->WriteBool(L6_S2_InvSteelPlaced);
	stream->WriteBool(L6_S2_InvMortarWithCharcoalPlaced);
	stream->WriteBool(L6_S2_InvPestlePlaced);
	for (int i = 0; i < MAX_L6S2_PZOBJECTS; i++)
		stream->WriteInt(L6_S2_PZPineGumObjectsUsed[i]);

	stream->WriteBool(L6_S3_RoomLightened);
	stream->WriteBool(L6_S3_JamesDialogsShown);
	stream->WriteBool(L6_S3_LasyaCutscenePlayed);
	stream->WriteBool(L6_S3_ClubsUsed);
	stream->WriteBool(L6_S3_InvCharcoalTaken);
	stream->WriteBool(L6_S3_InvHorseShoeTaken);
	stream->WriteBool(L6_S3_SteelPlaced);
	stream->WriteBool(L6_S3_FlintStoneUsed);
	stream->WriteBool(L6_S3_PathwayActived);
	stream->WriteBool(L6_S3_PathwayDestructed);
	stream->WriteBool(L6_S3_PZPathwayPuzzleCompleted);
	for (int i = 0; i < 5; i++)
		stream->WriteInt(L6_S3_PZPathwayRingAngles[i]);

	stream->WriteBool(L6_S3_HO_Visited);
	stream->WriteBool(L6_S3_HO_CodeTaken);
	stream->WriteBool(L6_S3_HO_CodeApplied);
	stream->WriteBool(L6_S3_HO_CodePuzzleSolved);
	stream->WriteBool(L6_S3_HO_BottleRemoved);
	stream->WriteBool(L6_S3_HO_PotRemoved);
	stream->WriteBool(L6_S3_HO_MainLockOpened1);
	stream->WriteBool(L6_S3_HO_MainLockOpened2);
	stream->WriteBool(L6_S3_HO_MainLockOpened3);
	stream->WriteBool(L6_S3_HO_MainBoxOpened);
	stream->WriteBool(L6_S3_HO_InvHeartTaken);
	stream->WriteBool(L6_S3_HO_VaseToppled);
	stream->WriteBool(L6_S3_HO_Bolt1Removed);
	stream->WriteBool(L6_S3_HO_Bolt2Removed);
	stream->WriteBool(L6_S3_HO_VaseDoorRemoved);
	stream->WriteInt(L6_S3_HO_CartPosition);
	for (int i = 0; i < 6; i++)
		stream->WriteInt(L6_S3_HO_ObjectStates[i]);

	stream->WriteBool(L6_S4_IsVisited);
	stream->WriteBool(L6_S4_InvStickTaken);
	stream->WriteBool(L6_S4_InvRopeTaken);
	stream->WriteBool(L6_S4_InvPineGumCollected);
	stream->WriteBool(L6_S4_InvPineGumStickCollected);
	stream->WriteBool(L6_S4_PanPlaced);
	stream->WriteBool(L6_S4_RockBrokened);
	stream->WriteBool(L6_S4_InvFlintStoneCollected);
	stream->WriteBool(L6_S4_IsHorseShoeMagnetized);
	stream->WriteBool(L6_S4_InvMagnetisedHorseShoeTaken);
	stream->WriteBool(L6_S4_InvRopeHorseShoeComboTaken);
	stream->WriteBool(L6_S4_InvMagnifyingGlassTaken);
	stream->WriteBool(L6_S4_KeyInIcePlaced);
	stream->WriteBool(L6_S4_InvStarShapedKeyTaken);
	stream->WriteBool(L6_S4_AmeehaNoteTaken);
	stream->WriteBool(L6_S4_MaryTextShown);
	stream->WriteBool(L6_S4_HeartCrystalPlaced);
	stream->WriteBool(L6_S4_GlueApplied);
	stream->WriteBool(L6_S4_ChestOpened);
	stream->WriteBool(L6_S4_NoteOpened);
	stream->WriteBool(L6_S4_InvColoredPowderTaken);
	stream->WriteBool(L6_S4_IsRockManDialogOver);
	stream->WriteBool(L6_S4_MaryWalkingCutscenePlayed);

	stream->WriteBool(L6_S4_PuzzlePiecesPlaced);
	for (int i = 0; i < 7; i++)
	{
		stream->WriteBool(L6_S4_PortalPuzzleObjectStates[i]);
	}
	stream->WriteBool(L6_S4_PuzzleComplete);
	stream->WriteBool(L6_S4_LetterFromAnanthaTaken);

	stream->WriteBool(L6_S5_FoundSophia);
	stream->WriteBool(L6_S5_Visited);
	stream->WriteBool(L6_S5_DialogsOver);
	stream->WriteBool(L6_S5_SophiaTabletDialogOver);

	stream->WriteInt(L6_S5_TabMiniGameSceneNumber);
	stream->WriteInt(L6_S5_TabMiniGameState);
	stream->WriteInt(L6_S5_TabMiniGamePluckedFruitIndex);
	stream->WriteBool(L6_S5_TabMiniGameIsClothRemoved);
	for (int i = 0; i < 5; i++)
	{
		stream->WriteBool(L6_S5_TabMiniGameIsCarrotPicked[i]);
	}
	stream->WriteBool(L6_S5_TabMinigameSolved);

	stream->WriteBool(L6_S5_HO_Complete);
	for (int i = 0; i < 10; i++)
	{
		stream->WriteBool(L6_S5_HOObjFound[i]);
	}

	for (int i = 0; i < MAX_L7S1_JIGSAWPIECES; i++)
		stream->WriteBool(L7_S1_PZJigsawYellowPlaced[i]);
	stream->WriteBool(L7_S1_PZJigsawYellowCompleted);
	for (int i = 0; i < 3; i++)
	{
		stream->WriteBool(L7_S1_PZStarDoorOpened[i]);
		stream->WriteBool(L7_S1_InvPZStarEmblemCollected[i]);
	}

	stream->WriteBool(L7_S1_InvPotEmptyTaken);
	stream->WriteBool(L7_S1_ComboPotChainPlaced);
	stream->WriteBool(L7_S1_InvPotWithChainTaken);
	stream->WriteBool(L7_S1_InvAxeUsed);
	stream->WriteBool(L7_S1_InvRedStarUsed);
	stream->WriteBool(L7_S1_WaterUsedOnRedFlower);
	stream->WriteBool(L7_S1_InvRedFlowerTaken);

	stream->WriteBool(L7_S1_WellLidOpened);
	stream->WriteBool(L7_S1_InvWheelUsed);
	stream->WriteBool(L7_S1_InvMagicWaterTaken);
	stream->WriteBool(L7_S1_InvPotTaken);
	for (int i = 0; i < 8; i++)
		stream->WriteInt(L7_S2_PZEyePartIndex[i]);
	stream->WriteInt(L7_S2_PZEyeGameState);
	stream->WriteBool(L7_S2_PZEyePuzzleCompleted);
	stream->WriteBool(L7_S2_InvSymbolDiscTaken);
	stream->WriteBool(L7_S2_InvStatueTaken);	

	for (int i = 0; i < MAX_L7S2_HANDHOSYMBOLS; i++)
		stream->WriteBool(L7_S2_HOLeftHandSymbolFound[i]);
	for (int i = 0; i < MAX_L7S2_HANDHOSYMBOLS; i++)
		stream->WriteBool(L7_S2_HORightHandSymbolFound[i]);
	stream->WriteBool(L7_S2_HORightHandCompleted);
	stream->WriteBool(L7_S2_HORightHandDiskPlaced);
	stream->WriteBool(L7_S2_HOLeftHandDiskPlaced);
	stream->WriteBool(L7_S2_IsRightHandExplodeShown);
	stream->WriteBool(L7_S2_HOLeftHandCompleted);
	stream->WriteBool(L7_S2_IsLeftHandExplodeShown);
	stream->WriteBool(L7_S2_HOBrokenRightHandCompleted);
	stream->WriteBool(L7_S2_HOBrokenLeftHandCompleted);
	stream->WriteBool(L7_S2_InvSphereHalfLeftToFixTaken);
	stream->WriteBool(L7_S2_InvSphereHalfRightBaseTaken);
	stream->WriteBool(L7_S2_InvSphereFullTaken);
	stream->WriteBool(L7_S2_PopSphereFullPlaced);
	stream->WriteBool(L7_S2_LasyaStatueRisenUp);	
	stream->WriteInt(L7_S2_HOBrockenLeftHandState);
	for (int i = 0; i < 6; i++)
		stream->WriteBool(L7_S2_HOBrockenLeftHandSymbolCompleted[i]);
	stream->WriteInt(L7_S2_HOBrockenRightHandState);
	for (int i = 0; i < 6; i++)
		stream->WriteBool(L7_S2_HOBrockenRightHandSymbolCompleted[i]);
	stream->WriteBool(L7_S2_PZLasyaStatuePlaced);
	stream->WriteBool(L7_S2_PZLasyaStatuePuzzleCompleted);
	for (int i = 0; i < MAX_L7S2_LASYAITEMS; i++)
		stream->WriteBool(L7_S2_PZLasyaDiffFound[i]);

	for (int i = 0; i < MAX_L7S3_JIGSAWPIECES; i++)
		stream->WriteBool(L7_S3_PZJigsawRedPlaced[i]);
	stream->WriteBool(L7_S3_PZJigsawRedCompleted);

	stream->WriteBool(L7_S3_InvAxesTaken);
	stream->WriteBool(L7_S3_InvAxeKeyTaken);
	stream->WriteBool(L7_S3_IsAxeKeyUsed);
	stream->WriteBool(L7_S3_InvLeverTaken);
	stream->WriteBool(L7_S3_IsLeverUsedOnChain);
	stream->WriteBool(L7_S3_InvMetalWireTaken);
	stream->WriteBool(L7_S3_InvMetalChainTaken);
	stream->WriteBool(L7_S3_InvBhasmamPowderTaken);
	stream->WriteBool(L7_S3_IsBroomUsedOnBhasmam);
	stream->WriteBool(L7_S3_InvBroomTaken);
	stream->WriteBool(L7_S3_InvYellowFlowerTaken);
	stream->WriteBool(L7_S3_IsBullMoved);
	stream->WriteBool(L7_S3_IsLetterAmeehaTaken);
	stream->WriteBool(L7_S3_IsDiwaliLampPlaced);
	stream->WriteInt(L7_S3_PZDoorState);
	for (int i = 0; i < 3; i++)
		stream->WriteBool(L7_S3_LocksState[i]);
	stream->WriteBool(L7_S3_PZDoorCompleted);
	stream->WriteBool(L7_S3_IsLasyaAttackCutscnPlayed);
	stream->WriteBool(L7_S3_IsNightmareCutscnPlayed);

	for (int i = 0; i < MAX_L7S4_JIGSAWPIECES; i++)
		stream->WriteBool(L7_S4_PZJigsawBluePlaced[i]);
	stream->WriteBool(L7_S4_PZJigsawBlueCompleted);
	stream->WriteBool(L7_S4_HaveHandPower);
	stream->WriteBool(L7_S4_InvBlueFlowerTaken);
	for(int i = 0; i < 3; i++)
		stream->WriteInt(L7_S4_TableTopFlowerPos[i]);
	stream->WriteBool(L7_S4_TableTopBasmamUsed);
	stream->WriteBool(L7_S4_TableTopBasmamComplete);
	stream->WriteBool(L7_S4_TableTopPoojaPopComplete);
	stream->WriteInt(L7_S4_TableTopPoojaPuzzleMode);

	stream->WriteBool(L7_S4_InvWaterUsedOnBlueFlower);
	stream->WriteBool(L7_S4_InvBlueStarUsed);
	stream->WriteBool(L7_S4_InvWellWheelTaken);
	stream->WriteBool(L7_S4_P1NoteClicked);
	stream->WriteBool(L7_S4_P1NoteTaken);
	stream->WriteBool(L7_S4_AmehaTalked);

	stream->WriteBool(L7_S5_End1CutScenePlayed);
	stream->WriteBool(L7_S5_End2CutScenePlayed);
	stream->WriteBool(L8_S1_IntroVidPlayed);	
	for (int i = 0; i < MAX_L8_MAGIC_SYMBOLS; i++)
		stream->WriteBool(L8_MagicSymbolFound[i]);
	stream->WriteBool(L8_MagicGlassUsed);
	stream->WriteBool(L8_S1_FloorCracked);
	stream->WriteBool(L8_S1_InvIdolTaken);
	stream->WriteBool(L8_S1_InvMagicWandTaken);
	stream->WriteBool(L8_S1_InvHalfKeyTaken);
	stream->WriteBool(L8_S1_NoteTaken);
	stream->WriteBool(L8_S1_DoorKeyPlaced);
	stream->WriteBool(L8_S1_HandPlaced);
	stream->WriteBool(L8_S1_PZDoorOpened);
	stream->WriteBool(L8_S1_DoorKeyTaken);
	stream->WriteBool(L8_S1_MagicWandWithoutKeyTaken);
	for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
		stream->WriteBool(L8_S1_MatchingSymbolFound[i]);
	stream->WriteBool(L8_S1_MatchingHOComplete);

	stream->WriteBool(L8_S1_HandPuzzleSolved);
	stream->WriteBool(L8_S1_InvMagicGlassTaken);
	stream->WriteBool(L8_S5_IsPuzzleDataSaved);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			stream->WriteBool(L8_S5_RingPiece_Visibility[i][j]);
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			stream->WriteInt(L8_S5_RingPiece_CurrentRow[i][j]);
		}
	}

	stream->WriteBool(l8_s1_DoorHOCompleted);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 8; j++)
			stream->WriteBool(l8_s1_DoorHOFoundObjects[i][j]);
	

	stream->WriteBool(L8_S2_Visited);
	stream->WriteBool(L8_S2_SmallDoorKeyPlaced	);
	stream->WriteBool(L8_S2_SmallDoorOpened		);
	stream->WriteBool(L8_S2_InvToolTaken		);
	stream->WriteBool(L8_S2_InvScrollTaken		);
	stream->WriteBool(L8_S2_InvHalfKeyTaken		);
	stream->WriteBool(L8_S2_GateKeyPlaced		);
	stream->WriteBool(L8_S2_GatePuzzleSolved	);
	stream->WriteBool(L8_S2_GateOpened			);
	stream->WriteBool(L8_S2_GatePuzzleStarted	);
	stream->WriteInt(L8_S2_GearPuzzleState	);
	for (int i = 0; i < 10; i++)
	{
		stream->WriteInt(L8_S2_GateSymbolArray[i]);
		
	}

	for (int i = 0; i < L8_S2_NUM_GEARS; i++)
	{
		stream->WriteInt(L8_S2_GearSlotGearId[i]);
	}
	stream->WriteBool(L8_S2_SugarCaneGrown);
	stream->WriteBool(L8_S2_InvSugarCaneTaken	);
	 stream->WriteInt(L8_S2_SugercanePZState);
	stream->WriteInt( L8_S2_SugercanePZLevel);
	stream->WriteBool(L8_S2_IsSugarCaneGivenToElephant);
	stream->WriteBool(L8_S2_IsClickedOnLotus	);
	stream->WriteBool(L8_S2_InvSwordHandleTaken	);
	stream->WriteBool(L8_S2_FloorCracked	);
	stream->WriteBool(L8_S2_InvIdolTaken	);
	for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
		stream->WriteBool(L8_S2_MatchingSymbolFound[i]);
	stream->WriteBool(L8_S2_MatchingHOComplete);

	stream->WriteBool(L8_S3_FloorCracked);
	stream->WriteBool(L8_S3_InvIdolTaken);
	stream->WriteBool(L8_S3_SwordHandlePlaced);
	stream->WriteBool(L8_S3_SwordPuzzleSolved);
	for (int i = 0;i < L8S3_SWORDPIECES;i++)
		stream->WriteBool(L8_S3_SwordPiecePlaced[i]);
	stream->WriteBool(L8_S3_InvSwordTaken);
	stream->WriteBool(L8_S3_LeafTextShown);
	for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
		stream->WriteBool(L8_S3_MatchingSymbolFound[i]);
	stream->WriteBool(L8_S3_MatchingHOComplete);

	stream->WriteBool(L8_S3_LockKeyPlaced);
	for (int i = 0; i< MAX_L8S3_STRIPS; i++)
		stream->WriteInt(L8_S3_SymboOnStrip[i]);
	stream->WriteBool(L8_S3_PZDoorOpened);
	stream->WriteBool(L8_S3_StripPuzzleSolved);
	stream->WriteBool(L8_S3_InvCrystalKeyTaken);
	stream->WriteBool(l8_s3_invMaryLeafTaken);
	stream->WriteBool(l8_s3_MaryLeafHOCompleted);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 4; j++)
			stream->WriteBool(l8_s3_LeafHOFoundObjects[i][j]);

	stream->WriteBool(L8_S4_BronzeIdolPlaced);
	stream->WriteBool(L8_S4_SilverIdolPlaced);
	stream->WriteBool(L8_S4_GoldIdolPlaced);
	stream->WriteBool(L8_S4_IsLeafPlacedInContainer);
	stream->WriteBool(L8_S4_IsMinigameAnimPlayed);
	stream->WriteBool(L8_S4_InvContainerWithLeafTaken);
	stream->WriteBool(L8_S4_ContainerWithLeafOpened);
	stream->WriteBool(L8_S4_IsContainerClosed);
	stream->WriteBool(L8_S4_IsPuzzleDataSaved);
	stream->WriteBool(L8_S4_IsPoolPuzzleSolved);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 4; k++)
				stream->WriteInt(L8_S4_FlowersOnBoardIndex[i][j][k]);

	stream->WriteBool(L8_S5_IntroVideoShown);
	stream->WriteBool(L8_S5_Visited);
	stream->WriteBool(L8_S5_EndVideoPlayed);

	stream->WriteBool(L8_S5_VirtualMiniGameCompleted);;
	stream->WriteInt(L8_S5_VirtualMiniGameState);
	for (int i = 0; i < 3; i++)
	{
		stream->WriteBool(L8_S5_VirtualMiniGameShapesComplete[i]);
	}
	for (int i = 0; i < 9; i++)
	{
		stream->WriteBool(L8_S5_VirtualMiniGameHOState[i]);
	}


	stream->WriteBool(IsMainChapterCompleted);
	stream->WriteBool(IsBonusChapterUnlocked);
	stream->WriteBool(IsBonusChainRemoved);

	//Ver 2
	stream->WriteBool(IsKidsRoomFromMainMenu);
	stream->WriteBool(IsKidsRoomFromExtras);
	stream->WriteBool(IsExtrasRoomFromMainMenu);
	stream->WriteBool(IsAwardsRoomFromExtrasScene);
	stream->WriteBool(IsCreditsFromMainMenu);

	stream->WriteInt(CreditsPrevArea);
	stream->WriteInt(ExtrasRoomPrevArea);
	stream->WriteInt(MemoryRoomPrevArea);
}

void Profile::Read(CIOStream *stream)
{
	CurrentArea = stream->ReadInt();	// Line 5 - 8
	CurrentLevel = stream->ReadInt();
	PrevComboActiveArea = stream->ReadInt();
	PrevMapActiveArea = stream->ReadInt();

	MusicVol = stream->ReadInt();	// Line 9 - 13
	SFXVol = stream->ReadInt();
	EnvirVol = stream->ReadInt();
	VoiceVol = stream->ReadInt();

	IsLowSystem = stream->ReadBool();	// Line 14 - 34
	IsCheckedForLowSystem = stream->ReadBool();
	IsWideScreen = stream->ReadBool();
	IsHOScenesSkippable	 = stream->ReadBool();
	IsMuted = stream->ReadBool();
	IsStartMusic =  stream->ReadBool();
	IsHidingTips = stream->ReadBool();
	IsAwardPopupsEnabled = stream->ReadBool();
	IsInvLocked = stream->ReadBool();
	IsExpertMode = stream->ReadBool();
	IsHideSparkles = stream->ReadBool();
	IsHideHOSparkles = stream->ReadBool();
	IsTutorialDisabled = stream->ReadBool();
	IsHintDisabled = stream->ReadBool();
	IsSkipDisabled = stream->ReadBool();
	CustomHintTimer = stream->ReadFloat();
	CustomSkipTimer = stream->ReadFloat();
	#ifdef INSTANT_RECHARGE
	CustomHintTimer = 1.0f;
	CustomSkipTimer = 1.0f;	
	#endif
	IsStartCutscenePlayed= stream->ReadBool();
	IsHaveYouCutscenePlayed= stream->ReadBool();
	IsEndCutscenePlayed= stream->ReadBool();
	IsPromoCutscenePlayed= stream->ReadBool();
	IsCrystalOrbPicked= stream->ReadBool();

	ExtrasCurrentPageId= stream->ReadInt();	// Line 35 - 36
	ExtrasCurrentPageIndex= stream->ReadInt();

	stream->ReadString();//UsrJournalNotes Skip-Text 	// Line 37
	IsJObjBtnActive = stream->ReadBool();	// Line 38

	JrnPageLastUnlocked = stream->ReadInt();	// Line 39
	JrnPageMaxRead = stream->ReadInt();	// Line 40
	JrnPageLastRead = stream->ReadInt();	// Line 41

	for (int i = 0; i < MAX_JOURNAL_NOTES; i++)		//Line 42 - 189
	{
		IsJrnNotesFound[i] = stream->ReadBool();
		JrnNotesCollected[i] = stream->ReadInt();
	}
	NumJrnNotesCollected = stream->ReadInt();		//Line 190
	for (int i = 0; i < MAX_CHARACTERS; i++) 		//Line 191 - 207
	{
		isCharacterUnlocked[i] = stream->ReadBool();
	}
	lastUnlockedCharacter = stream->ReadInt();		//Line 208
	lastViewedCharacter = stream->ReadInt();		//Line 209

	for (int i = 0; i < MAX_HALL_OF_FAME_AWARDS; i++) 		//Line 210 - 250
	{
		currentNumOfEvents[i] = stream->ReadInt();
	}


	for (int i = 0; i < eArea_Max; i++)		//Line 251 - 870
	{
		isHintUsedAfterHOStart[i] = stream->ReadBool();
		isMiniGameCompleted[i] = stream->ReadBool();
		isHOCompleted[i] = stream->ReadBool();
		isSkipUsedAfterMiniGameStart[i] = stream->ReadBool();
		isHOCompleteWithMatch3[i] = stream->ReadBool();
	}
	isMiniGameSkipedAfterGameStart = stream->ReadBool();		//Line 871
	isJigsawPuzzleSkipped = stream->ReadBool();		//Line 872
	isCutSceneSkipedAfterGameStart = stream->ReadBool();		//Line 873
	isHintUsedAfterGameStart = stream->ReadBool();		//Line 874
	isHOHintUsedAfterGameStart = stream->ReadBool();		//Line 875
	isHintUsedAfterCreeksMansionStart = stream->ReadBool();		//Line 876

	for (int i = 0; i < MAX_LVL_OBJECTIVES; i++)		//Line 877 - 940
	{
		IsLVLObjectiveCompleted[i] = stream->ReadBool();
		IsLVLObjectiveAdded[i] = stream->ReadBool();
	}
	for (int i = 0; i < MAX_CURRENT_OBJECTIVES; i++) 		//Line 941 - 946
	{
		CurrentLvLObjectives[i] = stream->ReadInt();
	}
	NumObjectivesActive = stream->ReadInt();		//Line 947

	for( int i=0; i < eInv_Max; ++i )		//Line 948 - 1190
		Inventory[i] = stream->ReadBool();

	for( int i=0; i < MAX_INVBOX_OBJECTS; ++i ) 	//Line 1191 - 1265
	{
		InvObjects[i].InvId = stream->ReadInt();
		InvObjects[i].nPiecePicked = stream->ReadInt();
		InvObjects[i].MaxPieces = stream->ReadInt();
	}
	InvObjectCnt = stream->ReadInt(); 		//Line  1266
	iInvBoxStartObj = stream->ReadInt();		//Line 1267

	HintSavedTime = stream->ReadInt();

	for( int i=0; i < eArea_Max; ++i )
		SkipTimer[i] = stream->ReadInt();

	TutorialSpecialFeaturesOnly = stream->ReadBool();
	TutorialState = stream->ReadInt();
	
	if( ProfileSystem::Instance->ActiveProfile.ProfileVersion <= PROFILE_SAVE_VER_BUG )//Version to 885 we had bug: "20 tutorial read/writes" 
	{
		for( int i=0; i < 20; ++i )
			IsTutorialShown[i] = stream->ReadBool();
	}
	else
	{
		for( int i=0; i < MAX_TUTORIAL; ++i )
			IsTutorialShown[i] = stream->ReadBool();
	}

	for( int i=0; i < eArea_Max; ++i )
		IsTaskLeft[i] = stream->ReadBool();

	for( int i=0; i < eArea_Max; ++i )
		IsAreaLocked[i] = stream->ReadBool();

	for( int i=0; i < eArea_Max; ++i )
		IsAreaVisited[i] = stream->ReadBool();

	//L1 Outside Mansion

	for (int i = 0; i < eMem_MAX; i++)
		IsMemoryCollected[i] = stream->ReadBool();
	NumMemoriesCollected = stream->ReadInt();
	LastMemoryCollected = stream->ReadInt();

	for (int i = 0; i < eAward_MAX; i++)
		IsAchievementUnlocked[i] = stream->ReadBool();
	for (int i = 0; i < eAward_MAX; i++)
		IsAchievementFixed[i] = stream->ReadBool();
	NumAchievementsCollected = stream->ReadInt();
	ElapsedGamePlayTime = stream->ReadInt();
	for (int i = 0; i < eArea_Max; i++)
		AreaStartTime[i] = stream->ReadInt();
	IsPlayingGameOnExpertMode = stream->ReadInt();

	IsPaptusL1S2Collected = stream->ReadBool();
	IsPaptusL1S3Collected = stream->ReadBool();
	IsPaptusL2S6Collected = stream->ReadBool();
	IsPaptusL3S5Collected = stream->ReadBool();
	IsPaptusL4S3Collected = stream->ReadBool();
	IsPaptusL5S5Collected = stream->ReadBool();
	IsPaptusL6S1Collected = stream->ReadBool();
	IsPaptusL6S1popCollected = stream->ReadBool();
	IsPaptusL7S3Collected = stream->ReadBool();

	IsMorphObjL1S1Collected = stream->ReadBool();
	IsMorphObjL1S2Collected = stream->ReadBool();
	IsMorphObjL1S3Collected = stream->ReadBool();
	IsMorphObjL1S7Collected = stream->ReadBool();

	IsMorphObjL2S1Collected = stream->ReadBool();
	IsMorphObjL2S2Collected = stream->ReadBool();
	IsMorphObjL2S3Collected = stream->ReadBool();
	IsMorphObjL2S4Collected = stream->ReadBool();
	IsMorphObjL2S5Collected = stream->ReadBool();
	IsMorphObjL2S6Collected = stream->ReadBool();

	IsMorphObjL3S2Collected = stream->ReadBool(); 
	IsMorphObjL3S3Collected = stream->ReadBool();
	IsMorphObjL3S1Collected = stream->ReadBool();
	IsMorphObjL3S4Collected = stream->ReadBool();

	IsMorphObjL4S1Collected = stream->ReadBool();
	IsMorphObjL4S3Collected = stream->ReadBool();
	IsMorphObjL4S4Collected = stream->ReadBool();
	IsMorphObjL4S5Collected = stream->ReadBool();

	IsMorphObjL5S2Collected = stream->ReadBool();
	IsMorphObjL5S4Collected = stream->ReadBool();
	IsMorphObjL5S5Collected = stream->ReadBool();
	IsMorphObjL5S6Collected = stream->ReadBool();

	IsMorphObjL6S1Collected = stream->ReadBool();
	IsMorphObjL6S2Collected = stream->ReadBool();
	IsMorphObjL6S3Collected = stream->ReadBool();
	IsMorphObjL6S4Collected = stream->ReadBool();

	IsMorphObjL7S1Collected = stream->ReadBool();
	IsMorphObjL7S2Collected = stream->ReadBool();
	IsMorphObjL7S3Collected = stream->ReadBool();
	IsMorphObjL7S4Collected = stream->ReadBool();
	NumMorphObjectsCollected = stream->ReadInt();
	//Level_1_Common
	L1_IsMansionPowerOn = stream->ReadBool();
	L1_IsDemoOutroVideoPlayed = stream->ReadBool();

	//Scene_1_Beach
	L1S1_IsTalkedWithRaja = stream->ReadBool();
	L1S1_IsRajaTakenJames = stream->ReadBool();

	L1S1_IsInvBagTaken = stream->ReadBool();
	L1S1_IsInvKnifeTaken = stream->ReadBool();
	L1S1_IsInvLetterCreekTaken = stream->ReadBool();
	L1S1_IsInvPlank3Taken = stream->ReadBool();

	L1S1_LocketPartsPlaced = stream->ReadBool();
	L1S1_LocketPart1Fixed = stream->ReadBool();
	L1S1_LocketPart2Fixed = stream->ReadBool();
	L1S1_LocketPart3Fixed = stream->ReadBool();
	L1S1_LocketPart4Fixed = stream->ReadBool();
	L1S1_LocketAllPartsFixed = stream->ReadBool();
	L1S1_BagOpened = stream->ReadBool();
	L1S1_KidsPaintingPuzzleActive = stream->ReadBool();
	L1S1_KidsPaintingAddedToJournal = stream->ReadBool();
	L1S1_KidsPaintingPzBagActive = stream->ReadBool();
	L1S1_ComboBagInitX = stream->ReadInt();
	L1S1_ComboBagInitY = stream->ReadInt();
	L1S1_BagZipOpened = stream->ReadBool();

	L1S1_P1Branch1Cut = stream->ReadBool();
	L1S1_P1Branch2Cut = stream->ReadBool();
	L1S1_P1Branch3Cut = stream->ReadBool();
	L1S1_P1Leaf1Removed = stream->ReadBool();
	L1S1_P1Leaf2Removed = stream->ReadBool();
	L1S1_P1Leaf3Removed = stream->ReadBool();
	L1S1_P1Ruby1Branch2Taken = stream->ReadBool();
	L1S1_P1Ruby2Branch3Taken = stream->ReadBool();
	L1S1_P1Ruby3UnderLeaf1Taken = stream->ReadBool();
	L1S1_P1Ruby4UnderLeavesTaken = stream->ReadBool();

	L1S1_P2BuffaloClicked = stream->ReadBool() ;
	L1S1_P2CastleClicked = stream->ReadBool();
	L1S1_P2MeClicked = stream->ReadBool() ;
	L1S1_P2MamaClicked  = stream->ReadBool();
	L1S1_P2KingClicked = stream->ReadBool();
	L1S1_P2IsPuzzleComplete = stream->ReadBool();

	//Scene_2_Frontyard	
	L1S2_IsMonkeyTookBanana = stream->ReadBool();
	L1S2_IsCarDickeyOpen = stream->ReadBool();
	L1S2_P1_IsSackMoved = stream->ReadBool();
	L1S2_P1_IsWoodMoved = stream->ReadBool();
	L1S2_P3IsSwingRopeCut = stream->ReadBool();
	L1S2_P3IsTreeBranchFixed = stream->ReadBool();
	L1S2_IsInvCloth1SwingTaken = stream->ReadBool();
	L1S2_IsInvCloth2LovePopTaken = stream->ReadBool();
	L1S2_IsInvCloth3CarDickyTaken = stream->ReadBool();
	L1S2_InvDieselCanTaken = stream->ReadBool();
	L1S2_InvHoseTaken = stream->ReadBool();
	L1S2_IsInvPlank1Taken = stream->ReadBool();
	L1S2_IsInvTreeBranchTaken = stream->ReadBool();

	L1S2_IsFusePuzzleSolved = stream->ReadBool();
	L1S2_IsFusePuzzleShuffled = stream->ReadBool();
	L1S2_IsP1FusePuzzleActive = stream->ReadBool();
	L1S2_IsP1LeftFuseFixed = stream->ReadBool();
	L1S2_IsP1RightFuseFixed = stream->ReadBool();
	L1S3_IsP1LeftFuseTaken = stream->ReadBool();
	L1S2_IsP1RightFuseTaken = stream->ReadBool();
	L1S2_IsP1GreenWireDownFixed = stream->ReadBool();
	L1S2_IsP1GreenWireTopFixed = stream->ReadBool();

	for (int i = 0; i < 4; i++)
	{
		L1S2_IsP1RotFuseRot[i] = stream->ReadInt();
		L1S2_IsP1WireConnectId[i] = stream->ReadInt();
		L1S2_IsP1WireFuseFixed[i] = stream->ReadBool();
		L1S2_IsP1WireFuseFixEmpty[i] = stream->ReadBool();
	}

	//Scene_3_Foyer
	for (int i = 0; i < 5; i++)
	{
		L1S4_P2IsImagesFound[i] = stream->ReadBool();
	}
	for (int i = 0; i < 5; i++)
	{
		L1S4_P2IsHandFixed[i] = stream->ReadBool();
	}
	L1S4_P2IsKeySetPlaced = stream->ReadBool();
	L1S4_P2IsMissingHandsPlaced = stream->ReadBool();
	L1S4_P2IsShadowPuzHandDoorOpened = stream->ReadBool();
	L1S3_IsShadowPuzzleSolved = stream->ReadBool();
	L1S4_IsShadowPuzFinalKeyTurned = stream->ReadBool();
	L1S3_IsBedroomDoorOpened = stream->ReadBool();

	L1S3_IsObj1Moved = stream->ReadBool();
	L1S3_IsObj2Moved = stream->ReadBool();
	L1S3_IsCatAttacked = stream->ReadBool();
	L1S3_IsBananaTaken = stream->ReadBool();
	L1S3_MatchBoxTaken = stream->ReadBool();
	L1S3_NagaKeyTaken = stream->ReadBool();
	L1S3_RajaTalkOver = stream->ReadBool();
	L1S3_RajaAskedForCreekLetter = stream->ReadBool();
	L1S3_RajaTakenJames = stream->ReadBool();

	L1S3_PZMoopanFlyerTaken = stream->ReadBool();
	L1S3_IsInvHOLocketTaken = stream->ReadBool();
	L1S3_IsInvShadowHand1Taken = stream->ReadBool();
	L1S3_IsP3ShadowHandFallen = stream->ReadBool();
	L1S3_IsMemoryDollTaken = stream->ReadBool();
	L1S3_PZShelfPuzzleSolved = stream->ReadBool();
	for (int i = 0; i < MAX_L1S3_SHELFPUZDIFFS; i++)
	{
		L1S3_PZShelfDifferFound[i] = stream->ReadBool();
	}

	// Door HO
	L1S3_IsDoorHOLocketPlaced = stream->ReadBool();
	L1S3_IsDoorHOLocketDoorTaken = stream->ReadBool();
	L1S3_IsDoorHOListPanelTaken = stream->ReadBool();
	L1S3_DoorHOSolved = stream->ReadBool();
	L1S3_KeyFixed = stream->ReadBool();
	L1S3_KeyDoorOpened = stream->ReadBool();
	L1S3_HOStarDoorOpened = stream->ReadBool();
	L1S3_HORoseFlowerFound = stream->ReadBool();
	L1S3_HOCrescentFound = stream->ReadBool();
	L1S3_HOSnakeFound = stream->ReadBool();
	L1S3_HOMaskFound = stream->ReadBool();
	L1S3_HOOwlFound = stream->ReadBool();
	for (int i = 0; i < 3; i++)
	{
		L1S3_HOLeavesFound[i] = stream->ReadBool();
	}

	L1S3_HOLionHeadFound = stream->ReadBool();
	L1S3_HOFishFound = stream->ReadBool();
	L1S4_HOFishTailFixed = stream->ReadBool();
	for (int i = 0; i < 2; i++)
	{
		L1S3_HOStarsFound[i] = stream->ReadBool();
	}
	L1S3_HOTreeFound = stream->ReadBool();
	L1S3_HOGoddessFound = stream->ReadBool();
	L1S3_HOTurtleFound = stream->ReadBool();
	L1S3_HOSwapPuzzleSolved = stream->ReadBool();
	L1S3_HOMemoryPuzzleSolved = stream->ReadBool();
	for (int i = 0; i < 3; i++)
	{
		L1S3_HOLockOpened[i] = stream->ReadBool();
	}
	for (int i = 0; i < 4; i++)
	{
		L1S3_HOSwapLeverAngle[i] = stream->ReadFloat();
	}

	L1S3_HOSolved = stream->ReadBool();
	L1S3_HOCompletedWithMatch3 = stream->ReadBool();

	//Scene_4_Bedroom
	L1S4_PaintingPuzzleSolved = stream->ReadBool();
	L1S4_InvPaintingFrameTaken = stream->ReadBool();
	L1S4_IsInvShadowHand2Taken = stream->ReadBool();

	//Scene_5_TreasureRoom
	L1S5_InvBoatKeyTaken = stream->ReadBool();
	L1S5_InvHammerHeadTaken = stream->ReadBool();
	L1S5_InvMortarPestleTaken = stream->ReadBool();
	L1S5_IsTreasureFireLeftLit = stream->ReadBool();
	L1S5_IsTreasureFireCenterLit = stream->ReadBool();
	L1S5_IsTreasureFireRightLit = stream->ReadBool();
	L1S5_IsHammerHandFixed = stream->ReadBool();
	L1S5_IsFireAttackVideoPlayed = stream->ReadBool();
	L1S5_IsFireAttckBanner = stream->ReadBool();
	//Scene_5_HOTreasure
	for (int i = 0; i < MAX_L1S5_HOITEMS; i++)
		L1S5_HOItemStates[i]=stream->ReadInt();
	for (int i = 0; i < MAX_HOITEMS; i++)
		L1S5_HOHUDItemStates[i]=stream->ReadInt();
	L1S5_HOP2PuzzleShuffled = stream->ReadBool();
	L1S5_HOP2PuzzleSolved = stream->ReadBool();
	for (int i = 0; i < MAX_L1S5_HOP2TOKENS; i++)
		L1S5_HOP2TokenPosContains[i]=stream->ReadInt();

	L1S5_HOClothFolded = stream->ReadBool();
	L1S5_HOBookPart2Fixed = stream->ReadBool();
	L1S5_HOBookPart3Fixed = stream->ReadBool();
	L1S5_HOBookOpened = stream->ReadBool();
	L1S5_HOHammerMoved = stream->ReadBool();
	L1S5_HOChestBoxOpened = stream->ReadBool();
	L1S5_HOSwapPuzzleSolved = stream->ReadBool();
	L1S5_HOSolved = stream->ReadBool();
	L1S5_HOCompletedWithMatch3 = stream->ReadBool();

	//Scene_5_TunnelDoor
	for (int i = 0; i < 3; i++)
	{
		L1S5_P1IsNailPlucked[i] = stream->ReadBool();
	}
	L1S5_P1IsMapMovedFromPlate = stream->ReadBool();
	L1S5_P1IsBoxOpened = stream->ReadBool();
	L1S5_P1MortarPestlePlaced = stream->ReadBool();
	L1S5_P1IsCrystalAddedToGrinder = stream->ReadBool();
	L1S5_P1IsCrystalPowderCreated = stream->ReadBool();
	L1S5_P1IsCrystalPowderAddedToPlate = stream->ReadBool();
	L1S5_P1IsBottleCapRemoved = stream->ReadBool();
	L1S5_P1AmoniAddedToPlate = stream->ReadBool();
	L1S5_P1GreenPowderAddedToBottleHalf = stream->ReadBool();
	L1S5_P1GreenPowderAddedToBottleTreeForth = stream->ReadBool();
	L1S5_P1GreenPowderAddedToBottleFull = stream->ReadBool();
	L1S5_P1ThreadAddedToBottle = stream->ReadBool();
	L1S5_P1WaxCylinderAddedToBottle = stream->ReadBool();
	L1S5_IsInvBombTaken = stream->ReadBool();
	L1S5_IsInvHammerHandTaken = stream->ReadBool();
	L1S5_IsInvHammerPlacedOnScene = stream->ReadBool();
	L1S5_SelectedNailIndex = stream->ReadInt();
	L1S5_POPBombFixed = stream->ReadBool();
	L1S5_POPBombLite = stream->ReadBool();
	L1S5_IsTorchLight = stream->ReadBool();
	L1S5_IsTunnelDoorExploded = stream->ReadBool();
	L1S5_IsEscapedFromTunnel = stream->ReadBool();
	L1S5_IsEscapedFromTunnelBanner = stream->ReadBool();

	L1S4_IsCreekIntroPlayed = stream->ReadBool();
	L1S4_IsCreekDialogPlayed = stream->ReadBool();
	L1S4_IsCreekOutroPlayed = stream->ReadBool();

	L1S4_InvKeySetTaken = stream->ReadBool();
	L1S4_IsBooksOnShelfMoved = stream->ReadBool();
	L1S4_IsBookCodePlaced = stream->ReadBool();
	L1S4_IsBeetleBookTakenFromShelf = stream->ReadBool();
	L1S4_IsBeetlePlacedOnBook = stream->ReadBool();
	L1S4_IsBeetleBookOpened = stream->ReadBool();

	L1S4_IsDrawerSnakeKeyFixed = stream->ReadBool();
	L1S4_IsDrawerOpened = stream->ReadBool();
	L1S4_IsKeyFallenDown = stream->ReadBool();
	L1S4_InvDrawerDiaryTaken = stream->ReadBool();
	L1S4_IsDrawerWaxMelted = stream->ReadBool();
	L1S4_IsDrawerScrewsTakenout = stream->ReadBool();
	L1S4_IsDrawerSecretDoorOpened = stream->ReadBool();
	L1S4_InvBeetleHeadTaken = stream->ReadBool();
	L1S4_IsBeetleTailFixed = stream->ReadBool();

	L1S4_IsPhotoTurned = stream->ReadBool();
	L1S4_IsPhotoPaperTeared = stream->ReadBool();
	L1S4_InvBeetleTailTaken = stream->ReadBool();
	L1S4_InvBookCodeTaken = stream->ReadBool();

	L1S4_IsDiaryPageTurned = stream->ReadBool();
	L1S4_IsDiaryItem1Removed = stream->ReadBool();
	L1S4_IsDiaryItem2Removed = stream->ReadBool();
	L1S4_IsDiaryItem3Removed = stream->ReadBool();
	L1S4_IsDiaryItem4Removed = stream->ReadBool();
	L1S4_IsDiaryItem5Removed = stream->ReadBool();
	L1S4_IsDiaryCreekNoticeTaken = stream->ReadBool();
	L1S4_InvCoinTaken = stream->ReadBool();

	L1S4_IsSecretTreasureDoorOpened = stream->ReadBool();
	L1S4_MayaPushMaryVideoPlayed = stream->ReadBool();

	// New Scene 6 - BoatShed
	L1S6_HOPaperCutScenePlayed = stream->ReadBool();
	L1S6_HOSquirrelMoved = stream->ReadBool();
	L1S6_HOBeettleMoved = stream->ReadBool();
	L1S6_HOBigBoxRustRemoved = stream->ReadBool();
	L1S6_HOSmallBoxRustRemoved = stream->ReadBool();
	L1S6_HOPaperClipTaken = stream->ReadBool();
	L1S6_HOBigBoxOpened = stream->ReadBool();
	L1S6_HOPaperClipUsed = stream->ReadBool();
	L1S6_HOSmallBoxOpened = stream->ReadBool();
	L1S6_HOFunnelPiecesFixed = stream->ReadBool();
	L1S6_HOTapeAppliedOnFunnel = stream->ReadBool();
	L1S6_HOTapeCutOnFunnel = stream->ReadBool();
	L1S6_HOInvOilDripperTaken = stream->ReadBool();
	L1S6_HOInvFunnelTaken = stream->ReadBool();
	L1S6_HOLeftMiniDoorOpened = stream->ReadBool();

	for (int i = 0; i < 3; i++)
	{
		L1S6_HOIsToolMoved[i] = stream->ReadBool();
	}
	for (int i = 0; i < 3; i++)
	{
		L1S6_HOIsToolInLeftMiniBoxMoved[i] = stream->ReadBool();
	}
	L1S6_HOSolved = stream->ReadBool();
	L1S6_HOCompletedWithMatch3 = stream->ReadBool();

	for (int i = 0; i < MAX_HOITEMS; i++)
	{
		L1S6_HOInvItemStates[i] = stream->ReadInt();
	}


	// scene 8 Boat Shed
	L1S6_IsPumperPlaced = stream->ReadBool();
	L1S6_IsRoomLite = stream->ReadBool();
	L1S6_IsWebCleaned = stream->ReadBool();	
	L1S6_IsHosePlaced = stream->ReadBool();	
	L1S6_IsBoxDroped = stream->ReadBool();	
	L1S6_IsCanPlaced = stream->ReadBool();	
	L1S6_IsCanFilled = stream->ReadBool();	
	L1S6_IsInvCrowbarPicked = stream->ReadBool();	
	L1S6_IsInvCanFullPicked = stream->ReadBool();	
	L1S6_IsBoxOpened = stream->ReadBool();
	L1S6_IsInvOilDipperTaken = stream->ReadBool();

	//scene 9 boatcloseup
	L1S7_IsInvPlank2Taken = stream->ReadBool();
	L1S7_IsStepsPlaced = stream->ReadBool();
	L1S7_IsRopeCut = stream->ReadBool();
	L1S7_IsDieselFilled = stream->ReadBool();
	L1S7_IsBoatKeyPlaced = stream->ReadBool();
	L1S7_IsCapOpen = stream->ReadBool();
	L1S7_IsFunnelPlaced = stream->ReadBool();
	L1S7_IsFunnelRemoved = stream->ReadBool();
	L1S7_IsCapClosed = stream->ReadBool();
	L1S7_IsBoatStarted = stream->ReadBool();

	// Level 2
	L2_S1_IsIntroSwimVideoPlayed = stream->ReadBool();
	L2_S1_InvHoneyCombPicked = stream->ReadBool();
	L2_S1_InvAloveraTaken = stream->ReadBool();
	L2_S1_InvPotWithWaterTaken = stream->ReadBool();
	L2_S1_InvScrapperTaken = stream->ReadBool();
	L2_S1_InvStoneBallTaken = stream->ReadBool();
	L2_S1_IsAloveraSliced = stream->ReadBool();
	L2_S1_IsGlovesUsed = stream->ReadBool();
	L2_S1_HoneyCombSmoked = stream->ReadBool();
	L2_S1_HoneyCombSliced = stream->ReadBool();
	L2_S1_IsNagaSculpturePlaced = stream->ReadBool();
	L2_S1_IsNagaSculptKeyTurnedOnce = stream->ReadBool();
	L2_S1_IsUndergroundHatchOpened = stream->ReadBool();
	L2_S1_IsUndergroundPopupInactive = stream->ReadBool();


	L2_S1_DuckFixed = stream->ReadBool();
	L2_S1_DuckOpened = stream->ReadBool();
	L2_S1_Box1Fixed = stream->ReadBool();
	L2_S1_Box2Fixed = stream->ReadBool();
	L2_S1_Box3Fixed = stream->ReadBool();
	L2_S1_BoxOpened = stream->ReadBool();
	L2_S1_SpearUsed = stream->ReadBool();
	L2_S1_StarFixed = stream->ReadBool();
	L2_S1_PentagonFixed = stream->ReadBool();
	L2_S1_StarKeyTurned = stream->ReadBool();
	L2_S1_PentagonKeyTurned = stream->ReadBool();
	L2_S1_PayalRemoved = stream->ReadBool();
	L2_S1_RockMoved = stream->ReadBool();
	L2_S1_ScrewFixed = stream->ReadBool();
	L2_S1_ScrewTightened = stream->ReadBool();
	L2_S1_KeyDoorLeverFixed = stream->ReadBool();
	L2_S1_SmallBoxDoorOpened = stream->ReadBool();
	L2_S1_KeyFixed = stream->ReadBool();
	L2_S1_KeyTurned = stream->ReadBool();
	L2_S1_LeftDoorOpened = stream->ReadBool();
	L2_S1_RightDoorOpened = stream->ReadBool();
	L2_S1_MirrorRemoved = stream->ReadBool();
	L2_S1_InvColorPowderTaken = stream->ReadBool();
	for (int i = 0; i < 3; i++)
	{
		L2_S1_BoxSelectedIndex[i] = stream->ReadInt();
	}
	for (int i = 0; i < MAX_L2S1_HOOBJECTS; i++)
	{
		L2_S1_HOItemsUsed[i] = stream->ReadBool();
	}

	for (int i = 0; i < MAX_L2S1_HOOBJECTS; i++)
	{
		L2S1_HOItemStates[i] = stream->ReadInt();
	}

	for (int i = 0; i < MAX_HOITEMS; i++)
	{
		L2S1_HOHUDItemStates[i] = stream->ReadInt();
	}

	L2_S2_EmberPlaced  = stream->ReadBool();
	L2_S2_InvFrankinscenceTaken = stream->ReadBool();
	L2_S2_InvMangoesTaken = stream->ReadBool();
	L2_S2_PZMango1Taken = stream->ReadBool();
	L2_S2_PZMango2Taken = stream->ReadBool();
	L2_S2_PZMango3Taken = stream->ReadBool();
	L2_S2_PZStonePlaced = stream->ReadBool();
	L2_S2_PZIsMiss = stream->ReadBool();

	for (int i = 0; i < MAX_L2S2_HOOBJECTS; i++)
	{
		L2S2_HOItemsFound[i] = stream->ReadBool();
	}
	L2S2_HONagaTempleVisitedOnce = stream->ReadBool();
	L2S2_HONagaTempleCompleted = stream->ReadBool();
	L2S2_HONagaCompletedWithMatch3 = stream->ReadBool();

	L2_S2_ColorPlatePlaced = stream->ReadBool();
	L2_S2_PoojaInstrPlaced = stream->ReadBool();
	L2_S2_NagaLampLite= stream->ReadBool();
	L2_S2_NagaLampLiteVideoPlayed= stream->ReadBool();
	L2_S2_CutSceneVideoPlayed= stream->ReadBool();
	L2_S2_NagaPearl1Taken= stream->ReadBool();
	L2_S2_NagaPearl2Taken= stream->ReadBool();
	L2_S2_NagaPearl3Taken= stream->ReadBool();
	L2_S2_NagaPearl4Taken= stream->ReadBool();
	L2_S2_NagaLeaf1Taken= stream->ReadBool();
	L2_S2_NagaLeaf2Taken= stream->ReadBool();
	L2_S2_pop3_R1Color= stream->ReadInt();
	L2_S2_pop3_R2Color= stream->ReadInt();
	L2_S2_pop3_R3Color= stream->ReadInt();
	L2_S2_pop3_R4Color= stream->ReadInt();
	L2_S2_pop3_R5Color= stream->ReadInt();
	L2_S2_pop3_R6Color= stream->ReadInt();
	L2_S2_pop3_R7Color= stream->ReadInt();
	L2_S2_pop3_R8Color= stream->ReadInt();
	L2_S2_pop3_R9Color= stream->ReadInt();
	L2_S2_pop3_R10Color= stream->ReadInt();
	L2_S2_pop3_R11Color= stream->ReadInt();
	L2_S2_pop3_R12Color= stream->ReadInt();
	L2_S2_pop3_R13Color= stream->ReadInt();
	L2_S2_pop3_R14Color= stream->ReadInt();
	L2_S2_kolamComplete= stream->ReadBool();

	L2_S3_Pop1_FirstDigPlayed = stream->ReadBool();
	L2_S3_Pop1_SecondDigPlayed = stream->ReadBool();
	L2_S3_Pop1_PluckingTurmericPlayed = stream->ReadBool();
	L2_S3_Pop1_FrogJumpPlayed  = stream->ReadBool();
	L2_S3_InvTurmericPicked = stream->ReadBool();
	L2_S3_InvPoojaLampPicked = stream->ReadBool();
	

	L2_S3_isStoneBeamPuzzleDataSaved = stream->ReadBool();
	for (int i = 0; i < 10; i++)
	{
		L2_S3_PZStoneBeamCircleRotation[i] = stream->ReadInt();
		L2_S3_PZStoneBeamCircleXPos[i] = stream->ReadFloat();
		L2_S3_PZStoneBeamCircleYPos[i] = stream->ReadFloat();
		L2_S3_PZStoneBeamCircleCurrentPos[i] = stream->ReadInt();
		L2_S3_PZStoneBeamCircleOriginalPos[i] = stream->ReadInt();
	}
	L2_S3_StonePathActivated = stream->ReadBool();
	L2_S3_IsStonePathActivatedMessageShown = stream->ReadBool();
	L2_S2_InvNagaPearlsCollected = stream->ReadBool();

	L2_S4_IsWoodsCut1 = stream->ReadBool();
	L2_S4_IsWoodsCut2 = stream->ReadBool();
	L2_S4_InvFirewoodTaken = stream->ReadBool();
	L2_S4_InvAxeBluntTaken = stream->ReadBool();
	L2_S4_IsAxeSharpened = stream->ReadBool();;
	L2_S4_InvAxeSharpenTaken = stream->ReadBool();
	L2_S4_InvFireTorchTaken = stream->ReadBool();
	L2_S4_InvFireTorchStickOiledTaken = stream->ReadBool();
	L2_S4_IsLasyaAttackVideoPlayed = stream->ReadBool();
	L2_S4_FireTorchOiled = stream->ReadBool();
	L2_S4_RockSlabMoved1 = stream->ReadBool();
	L2_S4_BrickPuzzleAreaClicked = stream->ReadBool();
	L2_S4_RockSlabRemoved = stream->ReadBool();
	L2_S4_InvFireTorchFlamed = stream->ReadBool();
	L2_S4_InvRakeTaken = stream->ReadBool();
	L2_S4_DeitiesPuzzleState = stream->ReadInt();
	L2_S4_InvMetalLeverTaken = stream->ReadBool();
	for (int i = 0; i < MAX_L2_S4_DEITIESTILES; i++)
	{
		L2_S4_DeitiesTileIndices[i] = stream->ReadInt();
	}
	L2_S5_IsVisited = stream->ReadBool();
	L2_S5_InvKnifeTaken = stream->ReadBool();
	L2_S5_InvAmberTaken = stream->ReadBool();
	L2_S5_InvPotTaken = stream->ReadBool();

	L2_S5_VinegarPlaced = stream->ReadBool();
	L2_S5_VinegarCorkOpened = stream->ReadBool();
	L2_S5_KnifeSharpened = stream->ReadBool();
	L2_S5_KnifeSharpTaken = stream->ReadBool();

	L2_S5_MangoPlaced = stream->ReadBool();
	L2_S5_MangoSliced = stream->ReadBool();
	L2_S5_MangoAddedToPot = stream->ReadBool();
	L2_S5_HoneyWaxPlaced = stream->ReadBool();
	L2_S5_HoneyWaxSliced = stream->ReadBool();
	L2_S5_HoneyWaxAddedToPot = stream->ReadBool();
	L2_S5_MangoHoneyLiquidCreated = stream->ReadBool();
	L2_S5_MangoHoneyLiquidApplied = stream->ReadBool();
	L2_S5_AloveraPlaced = stream->ReadBool();
	L2_S5_AloveraSliced = stream->ReadBool();
	L2_S5_AloveraApplied = stream->ReadBool();
	L2_S5_WoodPlaced = stream->ReadBool();
	L2_S5_WoodFired = stream->ReadBool();
	L2_S5_PuzzleModeActive = stream->ReadBool();
	L2_S5_PotWithWaterPlaced = stream->ReadBool();
	L2_S5_TurmericPlaced = stream->ReadBool();
	L2_S5_TurmericSliced = stream->ReadBool();
	L2_S5_TurmericAddedToPot = stream->ReadBool();
	L2_S5_TurmericPowderCreated = stream->ReadBool();
	L2_S5_TurmericPowderApplied = stream->ReadBool();
	L2_S5_MedicineCreated = stream->ReadBool();
	L2_S5_MedicineMixed = stream->ReadBool();
	L2_S5_MedicineGivenToMooppan = stream->ReadBool();
	L2_S5_ClickedOnMooppan = stream->ReadBool();
	L2_S5_InvPujaChartTaken = stream->ReadBool();
	L2_S5_InvNagaSculptureTaken = stream->ReadBool();
	L2_S5_ShelfPuzzleSolved = stream->ReadBool();
	L2_S5_EagleBlockPuzzleState = stream->ReadInt();
	for (int i = 0; i < MAX_L2_S5_EAGLEBLOCKS; i++)
	{
		L2_S5_EagleBlockIndices[i] = stream->ReadInt();
	}
	L2_S5_StrawBasketRemoved = stream->ReadBool();
	L2_S5_InvStoneSharpnerTaken = stream->ReadBool();
	L2_S5_InvVinegarTaken = stream->ReadBool();
	L2_S5_InvMedicineTaken = stream->ReadBool();
	L2S6_InvGlovesTaken = stream->ReadBool();
	L2S6_InvRustedKnifeTaken = stream->ReadBool();
	l2S6_Inv3XStonesTaken = stream->ReadBool();
	L2S6_Pop1Visited = stream->ReadBool();
	L2S6_HOWaterFallCompleted = stream->ReadBool();
	L2S6_HOWaterFallCompletedWithMatch3 = stream->ReadBool();
	for (int i = 0; i < MAX_L2S6_HOOBJECTS; i++)
	{
		L2S6_HOItemsFound[i] = stream->ReadBool();
	}

	L2S6_InvSmallStone1Taken = stream->ReadBool();
	L2S6_InvSmallStone2Taken = stream->ReadBool();
	L2S6_InvSmallStone3Taken = stream->ReadBool();
	L2_S6_IsWoodCutDown = stream->ReadBool();
	L2_S6_IsP2LocketOpen = stream->ReadBool();
	L2_S6_IsP3HolderOpen = stream->ReadBool();
	L2_S6_IsP3PaperPuzClueTaken = stream->ReadBool();
	//3 saved number Ids of holder
	L2_S6_P3CenterNums[0] = stream->ReadInt();
	L2_S6_P3CenterNums[1] = stream->ReadInt();
	L2_S6_P3CenterNums[2] = stream->ReadInt();
	L2_S6_NagaPearlFixed = stream->ReadBool();
	L2_S6_Shutter1Solved = stream->ReadBool();
	L2_S6_Shutter2Solved = stream->ReadBool();
	L2_S6_Shutter3Solved = stream->ReadBool();
	L2_S6_ClockPuzzleSolved = stream->ReadBool();
	L2_S6_PaperPuzCluePlaced = stream->ReadBool();
	L2_S6_IsP6OilLampLightened = stream->ReadBool();
	L2_S6_Rock1Moved = stream->ReadBool();
	L2_S6_Rock2Moved = stream->ReadBool();
	L2_S6_LeverUsed = stream->ReadBool();
	L2S6_InvStoneBallPlaced = stream->ReadBool();

	L2_S7_Puzzle1Solved = stream->ReadBool();
	L2_S7_Puzzle2Solved = stream->ReadBool();
	L2_S7_Puzzle3Solved = stream->ReadBool();
	L2_S7_Puzzle4Solved = stream->ReadBool();
	L2_S7_Puzzle5Solved = stream->ReadBool();
	L2_S7_Puzzle6Solved = stream->ReadBool();
	L2_S7_Puzzle7Solved = stream->ReadBool();
	L2_S7_SoldierPuzzleState = stream->ReadInt();
	L2_S7_CurrentPuzzle = stream->ReadInt();

	//Level 3
	L3_S1_Visited = stream->ReadBool();
	for (int i = 0; i < 12; i++)
	{
		L3_S1_Objectives[i] = stream->ReadBool();
	}

	L3_S1_InvFlagFullTaken = stream->ReadBool();
	L3_S1_InvFlagStickTaken = stream->ReadBool();
	L3_S1_InvFlagClothTaken = stream->ReadBool();
	L3_S1_EmberPopupState = stream->ReadInt();
	L3_S1_InvLadderStep1Taken = stream->ReadBool();
	L3_S1_InvGreenCrystalTaken = stream->ReadBool();
	L3_S1_LadderPiecesFixed = stream->ReadBool();
	L3_S1_LadderBrokenStepFixed = stream->ReadBool();

	for (int i = 0; i < MAX_L3S1_HOOBJECTS; i++)
		L3_S1_HOObjFound[i] = stream->ReadBool();
	for (int i = 0; i < MAX_HUDL3S1_SYMBOLS; i++)
		L3_S1_HOHudObjID[i]=stream->ReadInt();
	L3_S1_HONextHudSymbolID = stream->ReadInt();
	L3_S1_HOHudSymbolsInitialized = stream->ReadBool();
	L3_S1_HOLadderCompleted = stream->ReadBool();
	L3_S1_HOLadderCompletedWithMatch3 = stream->ReadBool();
	L3_S1_InvBrokenLadderTaken = stream->ReadBool();
	L3_S1_InvLadderFixedTaken = stream->ReadBool();

	L3_S1_P1LionHeadPuzzleCompleted = stream->ReadBool();
	L3_S1_invGreenEyeCollected = stream->ReadBool();
	for (int i = 0; i < 4; i++)
	{
		L3_S1_P1LionHeadRingAngles[i] = stream->ReadFloat();
	}
	L3_S2_InvCocoShellPowderFilledTaken = stream->ReadBool();
	L3_S2_InvTwineTaken = stream->ReadBool();
	L3_S2_MazeCurrentRoom = stream->ReadInt();
	L3_S2_MazeMapUsed = stream->ReadBool();
	L3_S2_InvSickleTaken = stream->ReadBool();
	L3_S2_RockMoved = stream->ReadBool();
	L3_S2_TwineReleased = stream->ReadBool();
	L3_S2_Thread1Taken = stream->ReadBool();
	L3_S2_Thread2Taken = stream->ReadBool();
	L3_S2_Thread3Taken = stream->ReadBool();
	L3_S2_Thread4Taken = stream->ReadBool();
	L3_S2_InvCoconutOuterRemoved = stream->ReadBool();
	L3_S2_InvCoconutShellTaken = stream->ReadBool();
	L3_S2_CoconutComboTwineFixed = stream->ReadBool();
	L3_S2_InvCoconutWithTwineTaken = stream->ReadBool();
	L3_S2_CrowFlewAway = stream->ReadBool();
	L3_S2_PoolDoorOpened = stream->ReadBool();

	L3_S2_InvHammerTaken = stream->ReadBool();
	L3_S2_InvHammerHandTaken = stream->ReadBool();
	L3_S2_ToothFixed = stream->ReadBool();
	L3_S2_BackDoorOpened = stream->ReadBool();

	L3_S2_InvCoconnutTaken = stream->ReadBool();
	L3_S2_LeafMoved = stream->ReadBool();
	L3_S2_Stick1Moved = stream->ReadBool();
	L3_S2_Stick2Moved = stream->ReadBool();

	L3_S2_BrickTaken = stream->ReadBool();
	L3_S2_CocoShellPowderFilledFixed = stream->ReadBool();
	L3_S2_TwineGunPowderFixed = stream->ReadBool();
	L3_S2_BrickFixed = stream->ReadBool();
	L3_S2_WallBrocken = stream->ReadBool();

	for (int i = 0; i < 3; i++)
		L3_S2_vineRemovedR1Status[i] = stream->ReadBool();

	L3_S2_InvPipe1R1Taken = stream->ReadBool();

	for (int i = 0; i < 3; i++)
		L3_S2_vineRemovedR2Status[i] = stream->ReadBool();

	for (int i = 0; i < 2; i++)
		L3_S2_R2RemovedMudOnPipe[i] = stream->ReadBool();

	L3_S2_InvPipe2R2Taken = stream->ReadBool();
	L3_S2_InvToothR2Taken = stream->ReadBool();

	L3_S2_P1PlayerTile = stream->ReadInt();
	for (int i = 0; i < 4; i++)
		L3_S2_P1CurrentTiles[i] = stream->ReadInt();

	for (int i = 0; i < 3; i++)
		L3_S2_R3DoorOpen[i] = stream->ReadBool();;

	L3_S2_InvPipe3R3Taken = stream->ReadBool();;
	L3_S2_InvHammerHeadTaken = stream->ReadBool();;
	L3_S2_R3StonesRemoved = stream->ReadBool();;

	L3_S2_M4DoorsOpened = stream->ReadBool();
	L3_S2_InvPipe4R4Taken = stream->ReadBool();
	for (int i = 0; i < 8; i++)
	{
		L3_S2_M4SliderLeftRemoved[i] = stream->ReadBool();
	}
	for (int i = 0; i < 8; i++)
	{
		L3_S2_M4SliderRightRemoved[i] = stream->ReadBool();
	}

	L3_S2_M5_PipePuzzleActivated = stream->ReadBool();
	for (int i = 0; i < MAX_PIPE_INDEX; i++)
	{
		L3_S2_M5_PipeGridIndex[i] = stream->ReadInt();
	}

	for (int i = 0; i < MAX_PIPE_INDEX; i++)
	{
		L3_S2_M5_PipeAngles[i] = stream->ReadInt();
	}

	L3_S2_M5_PipePuzzleCompleted = stream->ReadBool();
	L3_S2_M5_PipeSceneDoorsOpend = stream->ReadBool();

	L3_S2_M6_SceneState = stream->ReadInt();
	L3_S2_M6_PillerPuzzleComplete = stream->ReadBool();
	for (int i = 0; i < 3; i++)
	{
		L3_S2_M6_PillerPositions[i] = stream->ReadInt();
	}
	for (int i = 0; i < 2; i++)
	{
		L3_S2_M6_CollectedBoxIndex[i] = stream->ReadInt();
	}

	L3_S3_VisitingPoolForFirsttime = stream->ReadBool();
	L3_S3_IsDoorToS4Opened= stream->ReadBool();
	L3_S3_PZBlocksPlaced= stream->ReadBool();
	L3_S3_PZUnderwaterPuzzleSolved = stream->ReadBool();
	L3_S3_PZGoingUnderwaterFirstTime = stream->ReadBool();
	L3_S3_IsTempleRisenFromWater = stream->ReadBool();
	L3_S3_InvLadderStep2Taken = stream->ReadBool();
	L3_S3_InvCupFilledTaken = stream->ReadBool();
	L3_S3_InvCupTaken = stream->ReadBool();
	L3_S3_IsTempleGlassBroken = stream->ReadBool();
	L3_S3_InvTempleBoxTaken = stream->ReadBool();
	L3_S3_InvSnakeDoorPartTaken = stream->ReadBool();
	L3_S3_InvSawTaken = stream->ReadBool();
	L3_S3_InvShowelTaken = stream->ReadBool();
	L3_S3_IsRockMovedSaw1 = stream->ReadBool();
	L3_S3_IsRockMovedSaw2 = stream->ReadBool();
	L3_S3_IsRootLeftSawed = stream->ReadBool();
	L3_S3_IsRootRightSawed = stream->ReadBool();
	L3_S3_IsSecretBoxOpened = stream->ReadBool();
	for (int i = 0; i < 8; i++)
	{
		L3_S3_PZBlockTileIndex[i] = stream->ReadInt();
	}
	L3_S3_PZBlockState = stream->ReadInt();

	for (int i = 0; i < 8; i++)
	{
		L3_S3_PZPinkOuterKeyState[i] = stream->ReadBool();
		L3_S3_PZRedOuterKeyState[i] = stream->ReadBool();
		L3_S3_PZBlueOuterKeyState[i] = stream->ReadBool();
	}
	for (int i = 0; i < 8; i++)
	{
		L3_S3_PZPinkDiagonalKeyState[i] = stream->ReadBool();
		L3_S3_PZRedDiagonalKeyState[i] = stream->ReadBool();
		L3_S3_PZBlueDiagonalKeyState[i] = stream->ReadBool();
	}
	for (int i = 0; i < 4; i++)
	{
		L3_S3_PZPinkInnerKeyState[i] = stream->ReadBool();
		L3_S3_PZRedInnerKeyState[i] = stream->ReadBool();
		L3_S3_PZBlueInnerKeyState[i] = stream->ReadBool();
	}

	for (int i = 0; i < 16; i++)
	{
		L3_S4_HOFortFountainFound[i] = stream->ReadBool();
	}

	L3_S4_HOFortFountainCompleted = stream->ReadBool();

	L3_S4_IsVisitedOnce = stream->ReadBool();
	L3_S4_InvSharpRodTaken = stream->ReadBool();
	L3_S4_InvBambooStickTaken = stream->ReadBool();
	L3_S4_InvMetalSheetBlankTaken = stream->ReadBool();
	L3_S4_InvMapDrawnSheetTaken = stream->ReadBool();
	L3_S4_IsLeaf1OverMetalRemoved = stream->ReadBool();
	L3_S4_IsLeaf2OverMetalRemoved = stream->ReadBool();
	L3_S4_IsAmeehaDiaryNoteTaken = stream->ReadBool();
	L3_S4_IsAmeehaCutscenePlayed = stream->ReadBool();
	L3_S4_IsNagaLockPlaced = stream->ReadBool();
	L3_S4_IsCliffDoorOpened = stream->ReadBool();
	L3_S4_IsPop2AMapDoorOpened = stream->ReadBool();
	L3_S4_IsPop2ABlankSheetPlaced = stream->ReadBool();
	L3_S4_IsPop2AStatueMoved = stream->ReadBool();
	L3_S4_IsPop2ASnakeButtonPressed = stream->ReadBool();
	L3_S4_IsPop2BLeaf1Removed = stream->ReadBool();
	L3_S4_IsPop2BLeaf2Removed = stream->ReadBool();
	L3_S4_IsPop2BStoneRemoved = stream->ReadBool();
	L3_S4_IsPop2BSnakeButtonPressed = stream->ReadBool();
	L3_S4_IsPop2CSnakeButtonPressed = stream->ReadBool();
	L3_S4_IsPop2CStoneRemoved = stream->ReadBool();
	L3_S4_IsPop2CLeavesRemoved = stream->ReadBool();
	L3_S4_IsPop2DSnakeButtonPressed = stream->ReadBool();
	L3_S4_IsPop2DStoneRemoved = stream->ReadBool();
	L3_S4_IsPop2DLeavesRemoved = stream->ReadBool();
	L3_S4_IsPop4StoneRemoved = stream->ReadBool();
	L3_S4_IsPop4LeavesRemoved = stream->ReadBool();
	L3_S4_InvBambooWithSickleTaken = stream->ReadBool();

	L3_S5_InvRopeTaken = stream->ReadBool();
	L3_S5_InvClipTaken = stream->ReadBool();
	L3_S5_GreenStonePlaced = stream->ReadBool();
	L3_S5_DeerGone = stream->ReadBool();
	L3_S5_ClipPlaced = stream->ReadBool();
	L3_S5_RopeTied = stream->ReadBool();
	L3_S5_NailPlaced = stream->ReadBool();
	L3_S5_NailHammered = stream->ReadBool();

	L3_S5_DryGrassTaken1 = stream->ReadBool();
	L3_S5_DryGrassTaken2 = stream->ReadBool();
	L3_S5_DryGrassTaken3 = stream->ReadBool();
	L3_S5_DryGrassTaken4 = stream->ReadBool();
	L3_S5_DryGrassTaken5 = stream->ReadBool();
	L3_S5_DryGrassTaken6 = stream->ReadBool();
	L3_S5_DryGrassTaken7 = stream->ReadBool();
	L3_S5_DryGrassTaken8 = stream->ReadBool();
	L3_S5_DryGrassTaken9 = stream->ReadBool();
	L3_S5_DryGrassTaken10 = stream->ReadBool();
	L3_S5_DryGrassTaken11 = stream->ReadBool();

	L3_S5_InvGrassTaken1 = stream->ReadBool();
	L3_S5_InvGrassTaken2 = stream->ReadBool();
	L3_S5_InvGrassTaken3 = stream->ReadBool();
	L3_S5_InvGrassTaken4 = stream->ReadBool();
	L3_S5_InvGrassTaken5 = stream->ReadBool();
	L3_S5_InvGrassTaken6 = stream->ReadBool();

	L3_S5_InvAllGrassTaken = stream->ReadBool();

	//Level 4
	L4_S1_IsFirstVisit = stream->ReadBool();
	L4_S1_PocketOpend = stream->ReadBool();
	L4_S1_InvHandKerchiefTaken = stream->ReadBool();
	L4_S1_InvCrutchTaken = stream->ReadBool();
	L4_S1_SkimmerFlyed = stream->ReadBool();
	L4_S1_InvStickTaken = stream->ReadBool();
	L4_S1_InvLeafTaken = stream->ReadBool();
	L4_S1_InvLeafSpoonWithWaterTaken = stream->ReadBool();
	L4_S1_InvHerbalLeavesTaken = stream->ReadBool();
	L4_S1_InvBraceletTaken = stream->ReadBool();
	L4_S1_FirstThornRemoved = stream->ReadBool();
	L4_S1_SecondThornRemoved = stream->ReadBool();
	L4_S1_ShoeRemoved = stream->ReadBool();
	L4_S1_BloodRemoved = stream->ReadBool();
	L4_S1_MedPasteApplied = stream->ReadBool();
	L4_S1_WoundTied = stream->ReadBool();
	L4_S1_PutShoeBack = stream->ReadBool();
	L4_S1_TwigsMoved = stream->ReadBool();
	L4_S1_InvLeafSpoonCollected = stream->ReadBool();
	L4_S1_LeafTored = stream->ReadBool();
	L4_S1_HandKerchiefTored = stream->ReadBool();
	L4_S1_PutLeftShoe = stream->ReadBool();
	L4_S1_CrutchUsed = stream->ReadBool();

	L4_S1_PuzzleSolved = stream->ReadBool();
	L4_S1_InvBinocularLensTaken = stream->ReadBool();
	L4_S1_InvLeftShoeTaken = stream->ReadBool();
	L4_S1_PuzzleVisited = stream->ReadBool();
	for (int i = 0; i < 12; i++)
	{
		L4_S1_HOShoeFound[i] = stream->ReadBool();
	}

	L4_S1_HOShoeCompleted = stream->ReadBool();
	L4_S1_BetelBoxOPened = stream->ReadBool();
	L4_S1_InvKnifeCollected = stream->ReadBool();


	L4_S2_FruitsPlucked					= stream->ReadBool();
	L4_S2_FruitsTaken					= stream->ReadBool();
	L4_S2_AlligatorPuzzleSolved			= stream->ReadBool();
	L4_S2_AlligatorPuzzleState			= stream->ReadInt();
	L4_S2_BambooStickTaken				= stream->ReadBool();
	L4_S2_Pop2BambooTaskCompleted		= stream->ReadBool();
	L4_S2_GrassTaken					= stream->ReadBool();
	L4_S2_ComboKnifeAddedToBamboo		= stream->ReadBool();
	L4_S2_InvBambooSpearTaken			= stream->ReadBool();
	L4_S2_InvRopeTaken					= stream->ReadBool();
	L4_S2_InvGrinderHandleTaken = stream->ReadBool();
	L4_S2_DoorToCaveOpened = stream->ReadBool();
	L4_S2_FanPuzzleState = stream->ReadInt();
	L4_s2_GrassRopeFrameNumber = stream->ReadInt();
	L4_S2_MortarTaken = stream->ReadBool();

	L4_S3_IsCaveLit 					= stream->ReadBool();
	L4_S3_IsDoorPuzzleSolved			= stream->ReadBool();
	L4_S3_IsBowPlaced					= stream->ReadBool();
	L4_S3_IsEmberCreated				= stream->ReadBool();
	L4_S3_IsOilApplied					= stream->ReadBool();
	L4_S3_IsFireSticksPlaced			= stream->ReadBool();
	L4_S3_IsCoinPlaced					= stream->ReadBool();
	L4_S3_IsPlankPlaced					= stream->ReadBool();
	L4_S3_IsInvBowlTaken				= stream->ReadBool();
	L4_S3_IsMetalPaperTaken				= stream->ReadBool();
	L4_S3_IsPotRemoved					= stream->ReadBool();

	L4_S4_Visited = stream->ReadBool();
	L4_S4_SkeletonHandOpened = stream->ReadBool();
	L4_S4_InvBinocularTaken = stream->ReadBool();
	L4_S4_FlowerCutscenePlayed = stream->ReadBool();
	L4_S4_GlobGlowEnabled = stream->ReadBool();
	L4_S4_GrinderHandlePlaced = stream->ReadBool();
	L4_S4_OlivePlaced = stream->ReadBool();
	L4_S4_StrainerPlaced = stream->ReadBool();
	L4_S4_PotPlaced = stream->ReadBool();
	L4_S4_InstructionsPlaced = stream->ReadBool();
	L4_S4_OilMachinePlaced = stream->ReadBool();
	L4_S4_OilMachineRotated = stream->ReadBool();
	L4_S4_PotWithOilTaken = stream->ReadBool();
	L4_S4_InvBinocularWithLensTaken = stream->ReadBool();
	L4_S4_JumpCutscenePlayed = stream->ReadBool();
	L4_S4_ClimbCutscenePlayed = stream->ReadBool();

	L4_S2_InvTwig1Taken = stream->ReadBool();
	L4_S4_InvTwig2Taken = stream->ReadBool();
	L4_S5_InvTwig3Taken = stream->ReadBool();

	L4_S5_InvAlligatorHeadTaken	= stream->ReadBool();
	L4_S5_InvDoorKeyTaken		= stream->ReadBool();
	L4_S5_SwapPuzzleCompleted	= stream->ReadBool();



	for (int i = 0; i < 6; i++)
	{
		L4_S4_HOBinocularFound[i] = stream->ReadBool();
	}

	L4_S4_HOBinocularCompleted = stream->ReadBool();
	L4_S4_BannerTextAfterBinocularHOShown = stream->ReadBool();

	L4_S2_InvSmallBambooStick0Taken = stream->ReadBool();
	L4_S2_InvSmallBambooStick1Taken = stream->ReadBool();
	L4_S2_InvSmallBambooStick2Taken = stream->ReadBool();
	L4_S2_InvSmallBambooStick3Taken = stream->ReadBool();
	L4_S2_IsBambooStickCut = stream->ReadBool();

	L4_S5_Visited = stream->ReadBool();
	L4_S5_InvDoorTaken = stream->ReadBool();
	L4_S5_ScrewRemoved = stream->ReadBool();
	L4_S5_ClothRemoved = stream->ReadBool();
	L4_S5_InvOlivesCollected = stream->ReadBool();
	L4_S5_OilMachineBaseCollected = stream->ReadBool();
	L4_S5_InvNetTaken = stream->ReadBool();
	L4_S5_ComboBambooFrameAddedToNet = stream->ReadBool();
	L4_S5_InvBambooStrainerTaken = stream->ReadBool();
	L4_S5_DoorPlaced  = stream->ReadBool();

	//Level5
	L5_S1_InvBowTaken = stream->ReadBool();
	L5_S1_InvBowWithArrowFirstTaken = stream->ReadBool();
	L5_S1_InvBowWithArrowSecondTaken = stream->ReadBool();
	L5_S1_InvArrowHolderTaken = stream->ReadBool();
	L5_S1_BowStringFixed = stream->ReadBool();
	L5_S1_BowArrowHolderPlaced = stream->ReadBool();
	L5_S1_BowArrowFirstLoaded = stream->ReadBool();
	L5_S1_BowArrowSecondLoaded = stream->ReadBool();
	L5_S1_BowToTigerCutscenePlayed = stream->ReadBool();

	L5_S2_InvHammerTaken = stream->ReadBool();
	L5_S2_InvSoldierBadgeTaken = stream->ReadBool();

	for (int i = 0; i < MAX_L5S2_NUM_BLOCKS; i++)
		L5_S2_PZBlockStartRow[i] = stream->ReadInt();
	for (int i = 0; i < MAX_L5S2_NUM_BLOCKS; i++)
		L5_S2_PZBlockStartCol[i] = stream->ReadInt();

	L5_S2_PZBlocksPuzzleSolved = stream->ReadBool();
	L5_S2_InvBadgeTaken = stream->ReadBool();
	L5_S2_LogPushed = stream->ReadBool();
	L5_S2_LogFell = stream->ReadBool();
	L5_S2_GreaseTaken = stream->ReadBool();

	L5_S3_IntroVdoCompleted = stream->ReadBool();
	L5_S3_memClothTaken = stream->ReadBool();
	L5_S3_EmblemPlacedOnStone = stream->ReadBool();
	L5_S3_PaperCollectedFromStone = stream->ReadBool();
	L5_S3_InvMoonSolutionTaken = stream->ReadBool();
	L5_S3_VineFreed = stream->ReadBool();
	L5_S3_isHoActive = stream->ReadBool();
	L5_S3_isLostHook = stream->ReadBool();
	L5_S3_PZHaveHook = stream->ReadBool();
	L5_S3_HookLostBannerShowed = stream->ReadBool();
	L5_S3_LeftDoorOpened = stream->ReadBool();
	L5_S3_EnteredLeftDoor = stream->ReadBool();
	L5_S3_PZState = stream->ReadInt();
	for (int i = 0; i < 16; i++)
	{
		L5_S3_PZLocketIndexes[i] = stream->ReadInt();
	}
	L5_S3_PZCompleted = stream->ReadBool();
	L5_S3_HOUnderWaterStarRemoved = stream->ReadBool();
	for (int i = 0; i < 12; i++)
	{
		L5_S3_HOUnderWaterFoundObjs[i] = stream->ReadBool();
	}
	L5_S3_HOUnderWaterCompleted = stream->ReadBool();

	L5_S4_InvPickaxeTaken = stream->ReadBool();
	for (int i = 0; i < 6; i++)
	{
		L5_S4_KeyMatchComplete[i] = stream->ReadBool();
	}
	L5_S4_Level1Completed = stream->ReadBool();
	L5_S4_PZHandCuffState = stream->ReadInt();
	L5_S4_PZHandCuffComplete = stream->ReadBool();
	for (int i = 0; i < 3; i++)
	{
		L5_S4_PZHandCuffLockUnlocked[i] = stream->ReadBool();	
	}
	L5_S4_InvPickaxeHeadTaken = stream->ReadBool();
	L5_S4_GreaseUsed = stream->ReadBool();
	L5_S4_Stone1Moved = stream->ReadBool();
	L5_S4_Stone2Moved = stream->ReadBool();
	L5_S4_BridgeOpened = stream->ReadBool();
	L5_S4_InvBarkTaken = stream->ReadBool();
	L5_S4_PickAxeUsed = stream->ReadBool();
	L5_S4_PickAxeCutScenePlayed = stream->ReadBool();
	L5_S4_AnasuyaTalked = stream->ReadBool();
	L5_S4_InvChainTaken = stream->ReadBool();
	L3_S4_PZ1BridgeCompleted = stream->ReadBool();
	for (int i = 0; i < 5; i++)
	{
		L3_S4_PZ1BridgeRingAngles[i] = stream->ReadInt();
	}


	L5_S5_InvPickaxeHandleTaken = stream->ReadBool();
	L5_S5_InvHandcuffKeyTaken = stream->ReadBool();
	L5_S5_InvWaterproofBagTaken = stream->ReadBool();
	L5_S5_InvBambooLadderTaken = stream->ReadBool();
	L5_S5_InvStoneEmblemTaken = stream->ReadBool();
	L5_S5_IsAmeehaDiaryCutscenePlayed = stream->ReadBool();
	L5_S5_IsSoldierBadgeUsedOnDoor = stream->ReadBool();
	L5_S5_IsBambooLadderPlacedOnScene = stream->ReadBool();
	L5_S5_IsMoonGlassDoorOpened = stream->ReadBool();
	L5_S5_IsTopDoorOpened = stream->ReadBool();
	L5_S5_IsP1Stone1Removed = stream->ReadBool();
	L5_S5_IsP1Stone2Removed = stream->ReadBool();
	L5_S5_IsP1PillerRemoved = stream->ReadBool();
	for (int i = 0; i < 14; i++)
	{
		L5_S5_HOGanpathiFoundObjs[i] = stream->ReadBool();
	}
	L5_S5_HOGanpathiCompleted = stream->ReadBool();
	L5_S5_HOGanapathiActivated = stream->ReadBool();

	for (int i = 0; i < MAX_L5S5_PZBLOCKS; i++)
	{
		L5_S5_PZTokenState[i]=stream->ReadInt();
	}
	L5_S5_PZSolutionPaperPlaced = stream->ReadBool();
	L5_S5_PZStatesInitialized = stream->ReadBool();
	L5_S5_IsSunClockPuzzleCompleted = stream->ReadBool();

	L5_S6_HOSoldierBadgePlaced = stream->ReadBool();
	L5_S6_HOGuardianIntroTalkCompleted = stream->ReadBool();
	for (int i = 0; i < MAX_L5S6_HOOBJECTS; i++)
	{
		L5_S6_HOObjectFound[i] = stream->ReadBool();
	}
	L5_S6_HOCompleted = stream->ReadBool();
	L5_S6_PeacockChainUsed = stream->ReadBool();
	L5_S6_InvPeacockLocketTaken = stream->ReadBool();
	L5_S6_PeacockLocketUsed = stream->ReadBool();
	L5_S6_PeacockArrivingCutScenePlayed = stream->ReadBool();
	L5_S6_PeacockLeavingCutScenePlayed = stream->ReadBool();
	L5_S6_IntroCutScenePlayed = stream->ReadBool();
	L5_S6_GotFreeBannerShown  = stream->ReadBool();

	//Level 6

	L6_S1_Visited  = stream->ReadBool();
	L6_S1_ChappalKnobRemoved = stream->ReadBool();
	L6_S1_LetterOpened = stream->ReadBool();
	L6_S1_VaruniCutscenePlayed = stream->ReadBool();
	L6_S1_MaryFirstBannerTextShown = stream->ReadBool();
	L6_S1_MarySecondBannerTextShown = stream->ReadBool();
	L6_S1_ScrewUnsealed = stream->ReadBool();
	L6_S1_InvEmblemTaken = stream->ReadBool();
	L6_S1_InvMortarTaken = stream->ReadBool();
	L6_S1_InvSteelTaken = stream->ReadBool();
	L6_S1_SolutionFound = stream->ReadBool();
	L6_S1_InvClothTaken = stream->ReadBool();
	L6_S1_StarKeyPlaced = stream->ReadBool();
	L6_S1_EmblemPlaced = stream->ReadBool();
	L6_S1_PathToS3Activated = stream->ReadBool();
	L6_S1_InvClubTaken = stream->ReadBool();
	L6_S1_TextForHOShown = stream->ReadBool();

	for (int i = 0; i < MAX_L6S1_HOSCENES; i++)
		L6_S1_SceneHOSolved[i] = stream->ReadBool();
	L6_S1_DescriptionHOSolved = stream->ReadBool();

	for (int i = 0; i < 4; i++)
		L6_S1_DescHOFound_S1[i] = stream->ReadBool();
	for (int i = 0; i < 2; i++)
		L6_S1_DescHOFound_S2[i] = stream->ReadBool();
	for (int i = 0; i < 3; i++)
		L6_S1_DescHOFound_S3[i] = stream->ReadBool();
	for (int i = 0; i < 4; i++)
		L6_S1_DescHOFound_S4[i] = stream->ReadBool();
	for (int i = 0; i < 2; i++)
		L6_S1_DescHOFound_S5[i] = stream->ReadBool();
	L6_S1_InvHorseShoeTaken = stream->ReadBool();
	L6_S1_LetterTaken = stream->ReadBool();
	L6_S1_ChestPuzzleSolved = stream->ReadBool();

	L6_S2_DoorToS3Opened = stream->ReadBool();
	L6_S2_InvIceKeyTaken = stream->ReadBool();
	L6_S2_InvPestleTaken = stream->ReadBool();
	L6_S2_ColoredPowderTrowed = stream->ReadBool();

	L6_S2_HOComplete = stream->ReadBool();
	for (int i = 0; i < 7; i++)
	{
		L6_S2_HOItemsFound[i] = stream->ReadBool();
	}
	L6_S2_InvPuzzlePiecesCollected = stream->ReadBool();
	L6_S2_PuzzleComplete = stream->ReadBool();
	L6_S2_InvPanTaken					= stream->ReadBool();
	L6_S2_InvPanWithPinePitchPlaced		= stream->ReadBool();
	L6_S2_InvStickPlaced				= stream->ReadBool();
	L6_S2_InvFlintPlaced				= stream->ReadBool();
	L6_S2_InvSteelPlaced				= stream->ReadBool();
	L6_S2_InvMortarWithCharcoalPlaced	= stream->ReadBool();
	L6_S2_InvPestlePlaced				= stream->ReadBool();
	for (int i = 0; i < MAX_L6S2_PZOBJECTS; i++)
		L6_S2_PZPineGumObjectsUsed[i] = stream->ReadInt();

	L6_S3_RoomLightened = stream->ReadBool();
	L6_S3_JamesDialogsShown = stream->ReadBool();
	L6_S3_LasyaCutscenePlayed = stream->ReadBool();
	L6_S3_ClubsUsed = stream->ReadBool();
	L6_S3_InvCharcoalTaken = stream->ReadBool();
	L6_S3_InvHorseShoeTaken = stream->ReadBool();
	L6_S3_SteelPlaced = stream->ReadBool();
	L6_S3_FlintStoneUsed = stream->ReadBool();
	L6_S3_PathwayActived = stream->ReadBool();
	L6_S3_PathwayDestructed = stream->ReadBool();
	L6_S3_PZPathwayPuzzleCompleted = stream->ReadBool();
	for (int i = 0; i < 5; i++)
		L6_S3_PZPathwayRingAngles[i] = stream->ReadInt();

	L6_S3_HO_Visited = stream->ReadBool();
	L6_S3_HO_CodeTaken= stream->ReadBool();
	L6_S3_HO_CodeApplied= stream->ReadBool();
	L6_S3_HO_CodePuzzleSolved= stream->ReadBool();
	L6_S3_HO_BottleRemoved= stream->ReadBool();
	L6_S3_HO_PotRemoved= stream->ReadBool();
	L6_S3_HO_MainLockOpened1= stream->ReadBool();
	L6_S3_HO_MainLockOpened2= stream->ReadBool();
	L6_S3_HO_MainLockOpened3= stream->ReadBool();
	L6_S3_HO_MainBoxOpened= stream->ReadBool();
	L6_S3_HO_InvHeartTaken = stream->ReadBool();
	L6_S3_HO_VaseToppled = stream->ReadBool();
	L6_S3_HO_Bolt1Removed= stream->ReadBool();
	L6_S3_HO_Bolt2Removed= stream->ReadBool();
	L6_S3_HO_VaseDoorRemoved= stream->ReadBool();
	L6_S3_HO_CartPosition= stream->ReadInt();
	for (int i = 0; i < 6; i++)
		L6_S3_HO_ObjectStates[i]= stream->ReadInt();

	L6_S4_IsVisited = stream->ReadBool();
	L6_S4_InvStickTaken = stream->ReadBool();
	L6_S4_InvRopeTaken = stream->ReadBool();
	L6_S4_InvPineGumCollected = stream->ReadBool();
	L6_S4_InvPineGumStickCollected = stream->ReadBool();
	L6_S4_PanPlaced = stream->ReadBool();
	L6_S4_RockBrokened = stream->ReadBool();
	L6_S4_InvFlintStoneCollected = stream->ReadBool();
	L6_S4_IsHorseShoeMagnetized = stream->ReadBool();
	L6_S4_InvMagnetisedHorseShoeTaken = stream->ReadBool();
	L6_S4_InvRopeHorseShoeComboTaken = stream->ReadBool();
	L6_S4_InvMagnifyingGlassTaken = stream->ReadBool();
	L6_S4_KeyInIcePlaced = stream->ReadBool();
	L6_S4_InvStarShapedKeyTaken = stream->ReadBool();
	L6_S4_AmeehaNoteTaken = stream->ReadBool();
	L6_S4_MaryTextShown = stream->ReadBool();
	L6_S4_HeartCrystalPlaced = stream->ReadBool();
	L6_S4_GlueApplied = stream->ReadBool();
	L6_S4_ChestOpened = stream->ReadBool();
	L6_S4_NoteOpened = stream->ReadBool();
	L6_S4_InvColoredPowderTaken = stream->ReadBool();
	L6_S4_IsRockManDialogOver = stream->ReadBool();
	L6_S4_MaryWalkingCutscenePlayed = stream->ReadBool();

	L6_S4_PuzzlePiecesPlaced = stream->ReadBool();
	for (int i = 0; i < 7; i++)
	{
		L6_S4_PortalPuzzleObjectStates[i] = stream->ReadBool();
	}
	L6_S4_PuzzleComplete = stream->ReadBool();
	L6_S4_LetterFromAnanthaTaken = stream->ReadBool();

	L6_S5_FoundSophia = stream->ReadBool();
	L6_S5_Visited = stream->ReadBool();
	L6_S5_DialogsOver = stream->ReadBool();
	L6_S5_SophiaTabletDialogOver  = stream->ReadBool();

	L6_S5_TabMiniGameSceneNumber = stream->ReadInt();
	L6_S5_TabMiniGameState = stream->ReadInt();
	L6_S5_TabMiniGamePluckedFruitIndex = stream->ReadInt();
	L6_S5_TabMiniGameIsClothRemoved = stream->ReadBool();
	for (int i = 0; i < 5; i++)
	{
		L6_S5_TabMiniGameIsCarrotPicked[i] = stream->ReadBool();
	} 
	L6_S5_TabMinigameSolved = stream->ReadBool();

	L6_S5_HO_Complete = stream->ReadBool();
	for (int i = 0; i < 10; i++)
	{
		L6_S5_HOObjFound[i] = stream->ReadBool();
	} 

	for (int i = 0; i < MAX_L7S1_JIGSAWPIECES; i++)
		L7_S1_PZJigsawYellowPlaced[i] = stream->ReadBool();
	L7_S1_PZJigsawYellowCompleted = stream->ReadBool();
	for (int i = 0; i < 3; i++)
	{
		L7_S1_PZStarDoorOpened[i]  = stream->ReadBool();
		L7_S1_InvPZStarEmblemCollected[i] = stream->ReadBool();
	}

	L7_S1_InvPotEmptyTaken = stream->ReadBool();
	L7_S1_ComboPotChainPlaced = stream->ReadBool();
	L7_S1_InvPotWithChainTaken = stream->ReadBool();	
	L7_S1_InvAxeUsed = stream->ReadBool();
	L7_S1_InvRedStarUsed = stream->ReadBool();
	L7_S1_WaterUsedOnRedFlower = stream->ReadBool();
	L7_S1_InvRedFlowerTaken = stream->ReadBool();

	L7_S1_WellLidOpened = stream->ReadBool();
	L7_S1_InvWheelUsed = stream->ReadBool();
	L7_S1_InvMagicWaterTaken = stream->ReadBool();
	L7_S1_InvPotTaken = stream->ReadBool();
	for (int i = 0; i < 8; i++)
		L7_S2_PZEyePartIndex[i] = stream->ReadInt();
	L7_S2_PZEyeGameState = stream->ReadInt();
	L7_S2_PZEyePuzzleCompleted = stream->ReadBool();
	L7_S2_InvSymbolDiscTaken = stream->ReadBool();
	L7_S2_InvStatueTaken = stream->ReadBool();

	for (int i = 0; i < MAX_L7S2_HANDHOSYMBOLS; i++)
		L7_S2_HOLeftHandSymbolFound[i] = stream->ReadBool();
	for (int i = 0; i < MAX_L7S2_HANDHOSYMBOLS; i++)
		L7_S2_HORightHandSymbolFound[i] = stream->ReadBool();
	L7_S2_HORightHandCompleted = stream->ReadBool();
	L7_S2_HORightHandDiskPlaced = stream->ReadBool();
	L7_S2_HOLeftHandDiskPlaced = stream->ReadBool();
	L7_S2_IsRightHandExplodeShown = stream->ReadBool();
	L7_S2_HOLeftHandCompleted = stream->ReadBool();
	L7_S2_IsLeftHandExplodeShown = stream->ReadBool();
	L7_S2_HOBrokenRightHandCompleted = stream->ReadBool();
	L7_S2_HOBrokenLeftHandCompleted = stream->ReadBool();
	L7_S2_InvSphereHalfLeftToFixTaken = stream->ReadBool();
	L7_S2_InvSphereHalfRightBaseTaken = stream->ReadBool();
	L7_S2_InvSphereFullTaken = stream->ReadBool();
	L7_S2_PopSphereFullPlaced = stream->ReadBool();
	L7_S2_LasyaStatueRisenUp = stream->ReadBool();	
	L7_S2_HOBrockenLeftHandState = stream->ReadInt();
	for (int i = 0; i < 6; i++)
		L7_S2_HOBrockenLeftHandSymbolCompleted[i] = stream->ReadBool();
	L7_S2_HOBrockenRightHandState = stream->ReadInt();
	for (int i = 0; i < 6; i++)
		L7_S2_HOBrockenRightHandSymbolCompleted[i] = stream->ReadBool();
	L7_S2_PZLasyaStatuePlaced = stream->ReadBool();
	L7_S2_PZLasyaStatuePuzzleCompleted  = stream->ReadBool();
	for (int i = 0; i < MAX_L7S2_LASYAITEMS; i++)
		L7_S2_PZLasyaDiffFound[i]  = stream->ReadBool();

	for (int i = 0; i < MAX_L7S3_JIGSAWPIECES; i++)
		L7_S3_PZJigsawRedPlaced[i] = stream->ReadBool();
	L7_S3_PZJigsawRedCompleted = stream->ReadBool();

	L7_S3_InvAxesTaken = stream->ReadBool();
	L7_S3_InvAxeKeyTaken = stream->ReadBool();
	L7_S3_IsAxeKeyUsed = stream->ReadBool();
	L7_S3_InvLeverTaken = stream->ReadBool();
	L7_S3_IsLeverUsedOnChain = stream->ReadBool();
	L7_S3_InvMetalWireTaken = stream->ReadBool();
	L7_S3_InvMetalChainTaken = stream->ReadBool();
	L7_S3_InvBhasmamPowderTaken = stream->ReadBool();
	L7_S3_IsBroomUsedOnBhasmam = stream->ReadBool();
	L7_S3_InvBroomTaken = stream->ReadBool();
	L7_S3_InvYellowFlowerTaken = stream->ReadBool();
	L7_S3_IsBullMoved = stream->ReadBool();
	L7_S3_IsLetterAmeehaTaken = stream->ReadBool();
	L7_S3_IsDiwaliLampPlaced = stream->ReadBool();
	L7_S3_PZDoorState = stream->ReadInt();
	for (int i = 0; i < 3; i++)
		L7_S3_LocksState[i] = stream->ReadBool();
	L7_S3_PZDoorCompleted = stream->ReadBool();
	L7_S3_IsLasyaAttackCutscnPlayed = stream->ReadBool();
	L7_S3_IsNightmareCutscnPlayed = stream->ReadBool();

	for (int i = 0; i < MAX_L7S4_JIGSAWPIECES; i++)
		L7_S4_PZJigsawBluePlaced[i] = stream->ReadBool();
	L7_S4_PZJigsawBlueCompleted = stream->ReadBool();
	L7_S4_HaveHandPower = stream->ReadBool();
	L7_S4_InvBlueFlowerTaken = stream->ReadBool();
	for(int i = 0; i < 3; i++)
		L7_S4_TableTopFlowerPos[i] = stream->ReadInt();
	L7_S4_TableTopBasmamUsed = stream->ReadBool();
	L7_S4_TableTopBasmamComplete = stream->ReadBool();
	L7_S4_TableTopPoojaPopComplete = stream->ReadBool();
	L7_S4_TableTopPoojaPuzzleMode = stream->ReadInt();

	L7_S4_InvWaterUsedOnBlueFlower = stream->ReadBool();
	L7_S4_InvBlueStarUsed = stream->ReadBool();
	L7_S4_InvWellWheelTaken = stream->ReadBool();
	L7_S4_P1NoteClicked = stream->ReadBool();
	L7_S4_P1NoteTaken = stream->ReadBool();
	L7_S4_AmehaTalked = stream->ReadBool();

	L7_S5_End1CutScenePlayed = stream->ReadBool();
	L7_S5_End2CutScenePlayed = stream->ReadBool();L8_S1_IntroVidPlayed = stream->ReadBool();	for (int i = 0; i < MAX_L8_MAGIC_SYMBOLS; i++)
		L8_MagicSymbolFound[i] = stream->ReadBool();
	L8_MagicGlassUsed = stream->ReadBool();
	L8_S1_FloorCracked = stream->ReadBool();
	L8_S1_InvIdolTaken = stream->ReadBool();
	L8_S1_InvMagicWandTaken = stream->ReadBool();
	L8_S1_InvHalfKeyTaken = stream->ReadBool();
	L8_S1_NoteTaken = stream->ReadBool();
	L8_S1_DoorKeyPlaced = stream->ReadBool();
	L8_S1_HandPlaced = stream->ReadBool();
	L8_S1_PZDoorOpened = stream->ReadBool();
	L8_S1_DoorKeyTaken = stream->ReadBool();
	L8_S1_MagicWandWithoutKeyTaken = stream->ReadBool();
	for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
		L8_S1_MatchingSymbolFound[i] = stream->ReadBool();
	L8_S1_MatchingHOComplete = stream->ReadBool();

	L8_S1_HandPuzzleSolved = stream->ReadBool();
	L8_S1_InvMagicGlassTaken = stream->ReadBool();
	L8_S5_IsPuzzleDataSaved = stream->ReadBool();
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			L8_S5_RingPiece_Visibility[i][j] = stream->ReadBool();
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			L8_S5_RingPiece_CurrentRow[i][j] = stream->ReadInt();
		}
	}

	l8_s1_DoorHOCompleted = stream->ReadBool();
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 8; j++)
			l8_s1_DoorHOFoundObjects[i][j] = stream->ReadBool();
	

	L8_S2_Visited						= stream->ReadBool();
	L8_S2_SmallDoorKeyPlaced			= stream->ReadBool();
	L8_S2_SmallDoorOpened				= stream->ReadBool();
	L8_S2_InvToolTaken					= stream->ReadBool();
	L8_S2_InvScrollTaken				= stream->ReadBool();
	L8_S2_InvHalfKeyTaken				= stream->ReadBool();
	L8_S2_GateKeyPlaced					= stream->ReadBool();
	L8_S2_GatePuzzleSolved				= stream->ReadBool();
	L8_S2_GateOpened					= stream->ReadBool();
	L8_S2_GatePuzzleStarted				= stream->ReadBool();
	L8_S2_GearPuzzleState				= stream->ReadInt();
	for (int i = 0; i < 10; i++)
	{
		L8_S2_GateSymbolArray[i]		= stream->ReadInt();
		
	}
	for (int i = 0; i < L8_S2_NUM_GEARS; i++)
	{
		L8_S2_GearSlotGearId[i]			= stream->ReadInt();
	}
	L8_S2_SugarCaneGrown				= stream->ReadBool();
	L8_S2_InvSugarCaneTaken				= stream->ReadBool();
	 L8_S2_SugercanePZState				= stream->ReadInt();
	 L8_S2_SugercanePZLevel				= stream->ReadInt();
	L8_S2_IsSugarCaneGivenToElephant	= stream->ReadBool();
	L8_S2_IsClickedOnLotus				= stream->ReadBool();
	L8_S2_InvSwordHandleTaken			= stream->ReadBool();
	L8_S2_FloorCracked					= stream->ReadBool();
	L8_S2_InvIdolTaken					= stream->ReadBool();
	for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
		L8_S2_MatchingSymbolFound[i] = stream->ReadBool();
	L8_S2_MatchingHOComplete = stream->ReadBool();

	L8_S3_FloorCracked = stream->ReadBool();
	L8_S3_InvIdolTaken = stream->ReadBool();
	L8_S3_SwordHandlePlaced = stream->ReadBool();
	L8_S3_SwordPuzzleSolved = stream->ReadBool();
	for (int i = 0;i <L8S3_SWORDPIECES;i++)
		L8_S3_SwordPiecePlaced[i] = stream->ReadBool();
	L8_S3_InvSwordTaken = stream->ReadBool();
	L8_S3_LeafTextShown = stream->ReadBool();
	for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
		L8_S3_MatchingSymbolFound[i] = stream->ReadBool();
	L8_S3_MatchingHOComplete = stream->ReadBool();

	L8_S3_LockKeyPlaced = stream->ReadBool();
	for (int i = 0; i< MAX_L8S3_STRIPS; i++)
		L8_S3_SymboOnStrip[i] = stream->ReadInt();
	L8_S3_PZDoorOpened = stream->ReadBool();
	L8_S3_StripPuzzleSolved = stream->ReadBool();
	L8_S3_InvCrystalKeyTaken = stream->ReadBool();
	l8_s3_invMaryLeafTaken = stream->ReadBool();
	l8_s3_MaryLeafHOCompleted = stream->ReadBool();
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 4; j++)
			l8_s3_LeafHOFoundObjects[i][j] = stream->ReadBool();

	L8_S4_BronzeIdolPlaced				= stream->ReadBool();
	L8_S4_SilverIdolPlaced				= stream->ReadBool();
	L8_S4_GoldIdolPlaced				= stream->ReadBool();
	L8_S4_IsLeafPlacedInContainer		= stream->ReadBool();
	L8_S4_IsMinigameAnimPlayed			= stream->ReadBool();
	L8_S4_InvContainerWithLeafTaken		= stream->ReadBool();
	L8_S4_ContainerWithLeafOpened		= stream->ReadBool();
	L8_S4_IsContainerClosed				= stream->ReadBool();
	L8_S4_IsPuzzleDataSaved = stream->ReadBool();
	L8_S4_IsPoolPuzzleSolved = stream->ReadBool();
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				L8_S4_FlowersOnBoardIndex[i][j][k] = stream->ReadInt();
			}
		}
	}

	L8_S5_IntroVideoShown = stream->ReadBool();
	L8_S5_Visited = stream->ReadBool();
	L8_S5_EndVideoPlayed = stream->ReadBool();

	L8_S5_VirtualMiniGameCompleted = stream->ReadBool();
	L8_S5_VirtualMiniGameState = stream->ReadInt();
	for (int i = 0; i < 3; i++)
	{
		L8_S5_VirtualMiniGameShapesComplete[i] = stream->ReadBool();
	}
	for (int i = 0; i < 9; i++)
	{
		L8_S5_VirtualMiniGameHOState[i] = stream->ReadBool();
	}


	IsMainChapterCompleted = stream->ReadBool();
	IsBonusChapterUnlocked = stream->ReadBool();
	IsBonusChainRemoved = stream->ReadBool();


	//Ver 2
	if( ProfileSystem::Instance->ActiveProfile.ProfileVersion >= PROFILESAVE_VER_2 )
	{
		IsKidsRoomFromMainMenu = stream->ReadBool();
		IsKidsRoomFromExtras = stream->ReadBool();
		IsExtrasRoomFromMainMenu = stream->ReadBool();
		IsAwardsRoomFromExtrasScene = stream->ReadBool();;
		IsCreditsFromMainMenu = stream->ReadBool();

		CreditsPrevArea = stream->ReadInt();
		ExtrasRoomPrevArea = stream->ReadInt();
		MemoryRoomPrevArea = stream->ReadInt();
	}
}

void Profile::Clear()
{	
	InitGameVars();

	CurrentArea = eArea_L1_S1_Beach;
	CurrentLevel = Level_1;	
	PrevComboActiveArea = eArea_L1_S1_Beach;
	PrevMapActiveArea = eArea_L1_S1_Beach;

	MusicVol = 70;//100;
	SFXVol = 70;//100;
	EnvirVol = 70;//100;
	VoiceVol = 70;//100;

	IsLowSystem = false;
	IsCheckedForLowSystem = false;
	IsWideScreen	 = true; //false;
	IsHOScenesSkippable	 = false;
	IsMuted = false;
	IsStartMusic = false;
	IsHidingTips = false;
#ifdef K_ANDROID
	IsAwardPopupsEnabled = true;
#else
	IsAwardPopupsEnabled = true;
#endif
	IsInvLocked = true;
	IsExpertMode = false;
	IsHideSparkles = false;
	IsHideHOSparkles = false;
	IsTutorialDisabled = false;
	IsHintDisabled = false;
	IsSkipDisabled = false;
	#ifdef INSTANT_RECHARGE
	CustomHintTimer = 1.0f;
	CustomSkipTimer = 1.0f;	
	#else
	CustomHintTimer = 1.0f/5.0f;	//Initial 5 seconds
	CustomSkipTimer = 1.0f/5.0f;	//Initial 5 seconds
	#endif

	IsStartCutscenePlayed = false;
	IsHaveYouCutscenePlayed = false;
	IsEndCutscenePlayed = false;	
	IsPromoCutscenePlayed = false;
	IsCrystalOrbPicked = false;

	ExtrasCurrentPageId = -1;
	ExtrasCurrentPageIndex = -1;

	//User Journal Notes
	IsJObjBtnActive = false;

	JrnPageLastUnlocked = PAGE_2;//Page1 note is free
	JrnPageMaxRead = -1;
	JrnPageLastRead = -1;

	for (int i = 0; i < MAX_JOURNAL_NOTES; i++)
	{
		IsJrnNotesFound[i] = false;
		JrnNotesCollected[i] = -1;
	}
	NumJrnNotesCollected = 2;//2 is collected default
	IsJrnNotesFound[0] = true;
	IsJrnNotesFound[1] = true;
	JrnNotesCollected[0] = 0;
	JrnNotesCollected[1] = 1;

	for (int i = 0; i < MAX_CHARACTERS; i++)
	{
		isCharacterUnlocked[i] = false;
	}
	lastUnlockedCharacter = 1; //sophia is unlocked default
	lastViewedCharacter = 1; //sophia last viewed default

	//Unlocking characters sophia, mary, james, raja & darkarta by default
	isCharacterUnlocked[1] = true;
	isCharacterUnlocked[2] = true;
	isCharacterUnlocked[3] = true;
	isCharacterUnlocked[13] = true;
	isCharacterUnlocked[6] = true;

	for (int i = 0; i < MAX_HALL_OF_FAME_AWARDS; i++)
	{
		currentNumOfEvents[i] = 0;
	}

	for (int i = 0; i < eArea_Max; i++)
	{
		isHintUsedAfterHOStart[i] = false;
		isMiniGameCompleted[i] = false;
		isHOCompleted[i] = false;
		isSkipUsedAfterMiniGameStart[i] = false;
		isHOCompleteWithMatch3[i] = false;
	}
	isMiniGameSkipedAfterGameStart = false;
	isJigsawPuzzleSkipped = false;
	isCutSceneSkipedAfterGameStart = false;
	isHintUsedAfterGameStart = false;
	isHOHintUsedAfterGameStart = false;
	isHintUsedAfterCreeksMansionStart = false;

	for (int i = 0; i < MAX_LVL_OBJECTIVES; i++)
	{
		IsLVLObjectiveCompleted[i] = false;
		IsLVLObjectiveAdded[i] = false;
	}
	for (int i = 0; i < MAX_CURRENT_OBJECTIVES; i++)
	{
		CurrentLvLObjectives[i] = -1;
	}
	NumObjectivesActive = 0;

	for( int i=0; i < eInv_Max; ++i )
		Inventory[i] = false;
	for( int i=0; i < MAX_INVBOX_OBJECTS; ++i )
	{
		InvObjects[i].InvId = -1;
		InvObjects[i].nPiecePicked = 0; 
		InvObjects[i].MaxPieces = 0;
	}
	InvObjectCnt = 0;
	iInvBoxStartObj = 0;

	HintSavedTime = 0;
	for( int i=0; i < eArea_Max; ++i )
		SkipTimer[i] = 0;	

	TutorialSpecialFeaturesOnly = false;
	TutorialState = 0;
	for( int i=0; i < MAX_TUTORIAL; ++i )
		IsTutorialShown[i] = false;

	for( int i=0; i < eArea_Max; ++i )
		IsTaskLeft[i] = false;

	for( int i=0; i < eArea_Max; ++i )
		IsAreaLocked[i] = false;

	for( int i=0; i < eArea_Max; ++i )
		IsAreaVisited[i] = false;

	InitProfileLevel1Vars();
	//other levels too initialize
}

void Profile::InitProfileLevel1Vars()
{

	//L1 Outside Mansion	


	for (int i = 0; i < eMem_MAX; i++)
		IsMemoryCollected[i] = false;
	NumMemoriesCollected = 0;
	LastMemoryCollected = -1;

	for (int i = 0; i < eAward_MAX; i++)
		IsAchievementUnlocked[i] = false;
	for (int i = 0; i < eAward_MAX; i++)
		IsAchievementFixed[i] = false;
	NumAchievementsCollected = 0;
	ElapsedGamePlayTime = 0;
	for (int i = 0; i < eArea_Max; i++)
		AreaStartTime[i] = 0;
	IsPlayingGameOnExpertMode = -1;

	IsPaptusL1S2Collected = false;
	IsPaptusL1S3Collected = false;
	IsPaptusL2S6Collected = false;
	IsPaptusL3S5Collected = false;
	IsPaptusL4S3Collected = false;
	IsPaptusL5S5Collected = false;
	IsPaptusL6S1Collected = false;
	IsPaptusL6S1popCollected = false;
	IsPaptusL7S3Collected = false;

	IsMorphObjL1S1Collected = false;
	IsMorphObjL1S2Collected = false;
	IsMorphObjL1S3Collected = false;
	IsMorphObjL1S7Collected = false;
	IsMorphObjL2S1Collected = false;
	IsMorphObjL2S2Collected = false;
	IsMorphObjL2S3Collected = false;
	IsMorphObjL2S4Collected = false;
	IsMorphObjL2S5Collected = false;
	IsMorphObjL2S6Collected = false;
	IsMorphObjL3S2Collected = false;
	IsMorphObjL3S3Collected = false;
	IsMorphObjL3S1Collected = false;
	IsMorphObjL3S4Collected = false;
	IsMorphObjL4S1Collected = false;
	IsMorphObjL4S3Collected = false;
	IsMorphObjL4S4Collected = false;
	IsMorphObjL4S5Collected = false;

	IsMorphObjL5S2Collected = false;
	IsMorphObjL5S4Collected = false;
	IsMorphObjL5S5Collected = false;
	IsMorphObjL5S6Collected = false;

	IsMorphObjL6S1Collected = false;
	IsMorphObjL6S2Collected = false;
	IsMorphObjL6S3Collected = false;
	IsMorphObjL6S4Collected = false;

	IsMorphObjL7S1Collected = false;
	IsMorphObjL7S2Collected = false;
	IsMorphObjL7S3Collected = false;
	IsMorphObjL7S4Collected = false;

	NumMorphObjectsCollected = 0;

	//Level 1 - Mansion + Narara Islands
	L1_IsMansionPowerOn	= false;
	L1_IsDemoOutroVideoPlayed = false;

	//Scene_1 - Beach
	L1S1_IsTalkedWithRaja = false;
	L1S1_IsRajaTakenJames = false;

	L1S1_IsInvBagTaken = false;
	L1S1_IsInvKnifeTaken = false;
	L1S1_IsInvLetterCreekTaken = false;
	L1S1_IsInvPlank3Taken = false;

	L1S1_LocketPartsPlaced = false;
	L1S1_LocketPart1Fixed = false;
	L1S1_LocketPart2Fixed = false;
	L1S1_LocketPart3Fixed = false;
	L1S1_LocketPart4Fixed = false;
	L1S1_LocketAllPartsFixed = false;
	L1S1_BagOpened = false;
	L1S1_KidsPaintingPuzzleActive = false;
	L1S1_KidsPaintingAddedToJournal = false;
	L1S1_KidsPaintingPzBagActive = false;
	L1S1_ComboBagInitX = 0;
	L1S1_ComboBagInitY = 0;
	L1S1_BagZipOpened = false;

	L1S1_P1Branch1Cut = false;
	L1S1_P1Branch2Cut = false;
	L1S1_P1Branch3Cut = false;
	L1S1_P1Leaf1Removed = false;
	L1S1_P1Leaf2Removed = false;
	L1S1_P1Leaf3Removed = false;
	L1S1_P1Ruby1Branch2Taken = false;
	L1S1_P1Ruby2Branch3Taken = false;
	L1S1_P1Ruby3UnderLeaf1Taken = false;
	L1S1_P1Ruby4UnderLeavesTaken = false;

	L1S1_P2BuffaloClicked = false;
	L1S1_P2CastleClicked = false;
	L1S1_P2MeClicked = false;
	L1S1_P2MamaClicked = false;
	L1S1_P2KingClicked = false;
	L1S1_P2IsPuzzleComplete = false;

	//Scene 2 Frontyard
	L1S2_IsMonkeyTookBanana = false;
	L1S2_IsCarDickeyOpen = false;
	L1S2_P1_IsSackMoved = false;
	L1S2_P1_IsWoodMoved = false;
	L1S2_P3IsSwingRopeCut = false;
	L1S2_P3IsTreeBranchFixed = false;
	L1S2_IsInvCloth1SwingTaken= false;
	L1S2_IsInvCloth2LovePopTaken= false;
	L1S2_IsInvCloth3CarDickyTaken= false;
	L1S2_InvDieselCanTaken = false;
	L1S2_InvHoseTaken = false;
	L1S2_IsInvPlank1Taken = false;
	L1S2_IsInvTreeBranchTaken = false;

	L1S2_IsFusePuzzleSolved = false;
	L1S2_IsFusePuzzleShuffled = false;
	L1S2_IsP1FusePuzzleActive = false;
	L1S2_IsP1LeftFuseFixed = true;
	L1S2_IsP1RightFuseFixed = false;
	L1S3_IsP1LeftFuseTaken = false;
	L1S2_IsP1RightFuseTaken = true;
	L1S2_IsP1GreenWireDownFixed = true;
	L1S2_IsP1GreenWireTopFixed = false;

	for (int i = 0; i < 4; i++)
	{
		L1S2_IsP1RotFuseRot[i] = 0;
		L1S2_IsP1WireConnectId[i] = -1;
		L1S2_IsP1WireFuseFixed[i] = false;
		L1S2_IsP1WireFuseFixEmpty[i] = true;
	}

	//Nerfed puzzle difficulty 
	L1S2_IsP1RotFuseRot[2] = 1;
	L1S2_IsP1RotFuseRot[3] = 2;
	L1S2_IsP1WireConnectId[1] = 1;
	L1S2_IsP1WireFuseFixed[1] = true;
	L1S2_IsP1WireFuseFixEmpty[1] = false;

	//Scene_3_Foyer	
	for (int i = 0; i < 5; i++)
	{
		L1S4_P2IsImagesFound[i] = false;
	}
	for (int i = 0; i < 5; i++)
	{
		L1S4_P2IsHandFixed[i] = false;
	}
	L1S4_P2IsKeySetPlaced = false;
	L1S4_P2IsMissingHandsPlaced = false;
	L1S4_P2IsShadowPuzHandDoorOpened = false;
	L1S3_IsShadowPuzzleSolved = false;
	L1S4_IsShadowPuzFinalKeyTurned = false;
	L1S3_IsBedroomDoorOpened = false;

	L1S3_IsObj1Moved = false;
	L1S3_IsObj2Moved = false;
	L1S3_IsCatAttacked = false;
	L1S3_IsBananaTaken = false;
	L1S3_MatchBoxTaken = false;
	L1S3_NagaKeyTaken = false;
	L1S3_RajaTalkOver = false;
	L1S3_RajaAskedForCreekLetter = false;
	L1S3_RajaTakenJames = false;

	L1S3_PZMoopanFlyerTaken = false;
	L1S3_IsInvHOLocketTaken = false;
	L1S3_IsInvShadowHand1Taken = false;
	L1S3_IsP3ShadowHandFallen = false;
	L1S3_IsMemoryDollTaken = false;
	L1S3_PZShelfPuzzleSolved = false;
	for (int i = 0; i < MAX_L1S3_SHELFPUZDIFFS; i++)
	{
		L1S3_PZShelfDifferFound[i] = false;
	}

	// Door HO
	L1S3_IsDoorHOLocketPlaced = false;
	L1S3_IsDoorHOLocketDoorTaken = false;
	L1S3_IsDoorHOListPanelTaken = false;
	L1S3_DoorHOSolved = false;
	L1S3_KeyFixed = false;
	L1S3_KeyDoorOpened = false;
	L1S3_HOStarDoorOpened = false;
	L1S3_HORoseFlowerFound = false;
	L1S3_HOCrescentFound = false;
	L1S3_HOSnakeFound = false;
	L1S3_HOMaskFound = false;
	L1S3_HOOwlFound = false;
	for (int i = 0; i < 3; i++)
	{
		L1S3_HOLeavesFound[i] = false;
	}

	L1S3_HOLionHeadFound = false;
	L1S3_HOFishFound = false;
	L1S4_HOFishTailFixed = false;
	for (int i = 0; i < 2; i++)
	{
		L1S3_HOStarsFound[i] = false;
	}
	L1S3_HOTreeFound = false;
	L1S3_HOGoddessFound = false;
	L1S3_HOTurtleFound = false;
	L1S3_HOSwapPuzzleSolved = false;
	L1S3_HOMemoryPuzzleSolved = false;
	for (int i = 0; i < 3; i++)
	{
		L1S3_HOLockOpened[i] = false;
	}

	for (int i = 0; i < 4; i++)
	{
		L1S3_HOSwapLeverAngle[i] = 0.0f;
	}

	L1S3_HOSolved = false;
	L1S3_HOCompletedWithMatch3 = false;

	//Scene_4_Bedroom
	L1S4_PaintingPuzzleSolved = false;
	L1S4_InvPaintingFrameTaken = false;
	L1S4_IsInvShadowHand2Taken = false;

	//Scene_5_TreasureRoom
	L1S5_InvBoatKeyTaken = false;
	L1S5_InvHammerHeadTaken = false;
	L1S5_InvMortarPestleTaken = false;
	L1S5_IsTreasureFireLeftLit = false;
	L1S5_IsTreasureFireCenterLit = false;
	L1S5_IsTreasureFireRightLit = false;
	L1S5_IsHammerHandFixed = false;
	L1S5_IsFireAttackVideoPlayed = false;
	L1S5_IsFireAttckBanner = false;
	//Scene_5_HOTreasure
	for (int i = 0; i < MAX_L1S5_HOITEMS; i++)
		L1S5_HOItemStates[i]=0;
	for (int i = 0; i < MAX_HOITEMS; i++)
		L1S5_HOHUDItemStates[i]=0;
	L1S5_HOP2PuzzleShuffled = false;
	L1S5_HOP2PuzzleSolved = false;
	for (int i = 0; i < MAX_L1S5_HOP2TOKENS; i++)
		L1S5_HOP2TokenPosContains[i]=0;

	L1S5_HOClothFolded = false;
	L1S5_HOBookPart2Fixed = false;
	L1S5_HOBookPart3Fixed = false;
	L1S5_HOBookOpened = false;
	L1S5_HOHammerMoved = false;
	L1S5_HOChestBoxOpened = false;
	L1S5_HOSwapPuzzleSolved = false;
	L1S5_HOSolved = false;
	L1S5_HOCompletedWithMatch3 = false;

	//Scene_5_TunnelDoor
	for (int i = 0; i < 3; i++)
	{
		L1S5_P1IsNailPlucked[i] = false;
	}
	L1S5_P1IsMapMovedFromPlate = false;
	L1S5_P1IsBoxOpened = false;
	L1S5_P1MortarPestlePlaced = false;
	L1S5_P1IsCrystalAddedToGrinder = false;
	L1S5_P1IsCrystalPowderCreated = false;
	L1S5_P1IsCrystalPowderAddedToPlate = false;
	L1S5_P1IsBottleCapRemoved = false;
	L1S5_P1AmoniAddedToPlate = false;
	L1S5_P1GreenPowderAddedToBottleHalf = false;
	L1S5_P1GreenPowderAddedToBottleTreeForth = false;
	L1S5_P1GreenPowderAddedToBottleFull = false;
	L1S5_P1ThreadAddedToBottle = false;
	L1S5_P1WaxCylinderAddedToBottle = false;
	L1S5_IsInvBombTaken = false;
	L1S5_IsInvHammerHandTaken = false;
	L1S5_IsInvHammerPlacedOnScene = false;
	L1S5_SelectedNailIndex = 0;
	L1S5_POPBombFixed = false;
	L1S5_POPBombLite = false;
	L1S5_IsTorchLight = false;
	L1S5_IsTunnelDoorExploded = false;
	L1S5_IsEscapedFromTunnel = false;
	L1S5_IsEscapedFromTunnelBanner = false;

	L1S4_IsCreekIntroPlayed = false;
	L1S4_IsCreekDialogPlayed = false;
	L1S4_IsCreekOutroPlayed = false;

	L1S4_InvKeySetTaken = false;
	L1S4_IsBooksOnShelfMoved = false;
	L1S4_IsBookCodePlaced = false;
	L1S4_IsBeetleBookTakenFromShelf = false;
	L1S4_IsBeetlePlacedOnBook = false;
	L1S4_IsBeetleBookOpened = false;

	L1S4_IsDrawerSnakeKeyFixed = false;
	L1S4_IsDrawerOpened = false;
	L1S4_IsKeyFallenDown = false;
	L1S4_InvDrawerDiaryTaken = false;
	L1S4_IsDrawerWaxMelted = false;
	L1S4_IsDrawerScrewsTakenout = false;
	L1S4_IsDrawerSecretDoorOpened = false;
	L1S4_InvBeetleHeadTaken = false;
	L1S4_IsBeetleTailFixed = false;

	L1S4_IsPhotoTurned = false;
	L1S4_IsPhotoPaperTeared = false;
	L1S4_InvBeetleTailTaken = false;
	L1S4_InvBookCodeTaken = false;

	L1S4_IsDiaryPageTurned = false;
	L1S4_IsDiaryItem1Removed = false;
	L1S4_IsDiaryItem2Removed = false;
	L1S4_IsDiaryItem3Removed = false;
	L1S4_IsDiaryItem4Removed = false;
	L1S4_IsDiaryItem5Removed = false;
	L1S4_IsDiaryCreekNoticeTaken = false;
	L1S4_InvCoinTaken = false;

	L1S4_IsSecretTreasureDoorOpened = false;
	L1S4_MayaPushMaryVideoPlayed = false;

	// New Scene 6 - BoatShed

	L1S6_HOPaperCutScenePlayed = false;
	L1S6_HOSquirrelMoved = false;
	L1S6_HOBeettleMoved = false;
	L1S6_HOBigBoxRustRemoved = false;
	L1S6_HOSmallBoxRustRemoved = false;
	L1S6_HOPaperClipTaken = false;
	L1S6_HOBigBoxOpened = false;
	L1S6_HOPaperClipUsed = false;
	L1S6_HOSmallBoxOpened = false;
	L1S6_HOFunnelPiecesFixed = false;
	L1S6_HOTapeAppliedOnFunnel = false;
	L1S6_HOTapeCutOnFunnel = false;
	L1S6_HOInvOilDripperTaken = false;
	L1S6_HOInvFunnelTaken = false;
	L1S6_HOLeftMiniDoorOpened = false;

	for (int i = 0; i < 3; i++)
	{
		L1S6_HOIsToolMoved[i] = false;
	}

	for (int i = 0; i < 3; i++)
	{
		L1S6_HOIsToolInLeftMiniBoxMoved[i] = false;
	}
	L1S6_HOSolved = false;
	L1S6_HOCompletedWithMatch3 = false;

	for (int i = 0; i < MAX_HOITEMS; i++)
	{
		L1S6_HOInvItemStates[i] = 0;
	}

	//Scene 8 Boat Shed
	L1S6_IsPumperPlaced = false;
	L1S6_IsRoomLite = false;
	L1S6_IsWebCleaned = false;
	L1S6_IsHosePlaced = false;
	L1S6_IsBoxDroped = false;
	L1S6_IsCanPlaced = false;
	L1S6_IsCanFilled = false;
	L1S6_IsInvCrowbarPicked = false;
	L1S6_IsInvCanFullPicked = false;

	L1S6_IsBoxOpened = false;
	L1S6_IsInvOilDipperTaken = false;

	//Scene 9 Boat Close
	L1S7_IsInvPlank2Taken = false;
	L1S7_IsStepsPlaced = false;
	L1S7_IsRopeCut = false;
	L1S7_IsDieselFilled = false;
	L1S7_IsBoatKeyPlaced = false;
	L1S7_IsCapOpen = false;
	L1S7_IsFunnelPlaced = false;
	L1S7_IsFunnelRemoved = false;
	L1S7_IsCapClosed = false;
	L1S7_IsBoatStarted = false;

	//Level 2
	L2_S1_IsIntroSwimVideoPlayed = false;
	L2_S1_InvHoneyCombPicked = false;
	L2_S1_InvAloveraTaken = false;
	L2_S1_InvPotWithWaterTaken = false;
	L2_S1_InvScrapperTaken = false;
	L2_S1_InvStoneBallTaken = false;
	L2_S1_IsAloveraSliced = false;
	L2_S1_IsGlovesUsed = false;
	L2_S1_HoneyCombSmoked = false;
	L2_S1_HoneyCombSliced = false;
	L2_S1_IsNagaSculpturePlaced = false;
	L2_S1_IsNagaSculptKeyTurnedOnce = false;
	L2_S1_IsUndergroundHatchOpened = false;
	L2_S1_IsUndergroundPopupInactive = false;

	L2_S1_DuckFixed = false;
	L2_S1_DuckOpened = false;
	L2_S1_Box1Fixed = false;
	L2_S1_Box2Fixed = false;
	L2_S1_Box3Fixed = false;
	L2_S1_BoxOpened = false;
	L2_S1_SpearUsed = false;
	L2_S1_StarFixed = false;
	L2_S1_PentagonFixed = false;
	L2_S1_StarKeyTurned = false;
	L2_S1_PentagonKeyTurned = false;
	L2_S1_PayalRemoved = false;
	L2_S1_RockMoved = false;
	L2_S1_ScrewFixed = false;
	L2_S1_ScrewTightened = false;
	L2_S1_KeyDoorLeverFixed = false;
	L2_S1_SmallBoxDoorOpened = false;
	L2_S1_KeyFixed = false;
	L2_S1_KeyTurned = false;
	L2_S1_LeftDoorOpened = false;
	L2_S1_RightDoorOpened = false;
	L2_S1_MirrorRemoved = false;
	L2_S1_InvColorPowderTaken = false;
	for (int i = 0; i < 3; i++)
	{
		L2_S1_BoxSelectedIndex[i] = -1;
	}
	for (int i = 0; i < MAX_L2S1_HOOBJECTS; i++)
	{
		L2_S1_HOItemsUsed[i] = false;
	}

	for (int i = 0; i < MAX_L2S1_HOOBJECTS; i++)
	{
		L2S1_HOItemStates[i] = 0;
	}

	for (int i = 0; i < MAX_HOITEMS; i++)
	{
		L2S1_HOHUDItemStates[i] = 0;
	}

	L2_S2_EmberPlaced = false;
	L2_S2_InvFrankinscenceTaken = false;
	L2_S2_InvMangoesTaken = false;
	L2_S2_PZMango1Taken = false;
	L2_S2_PZMango2Taken = false;
	L2_S2_PZMango3Taken = false;
	L2_S2_PZStonePlaced = false;
	L2_S2_PZIsMiss = false;

	for (int i = 0; i < MAX_L2S2_HOOBJECTS; i++)
	{
		L2S2_HOItemsFound[i] = false;
	}
	L2S2_HONagaTempleVisitedOnce = false;
	L2S2_HONagaTempleCompleted = false;
	L2S2_HONagaCompletedWithMatch3 = false;

	L2_S2_ColorPlatePlaced = false;
	L2_S2_PoojaInstrPlaced = false;
	L2_S2_NagaLampLite= false;
	L2_S2_NagaLampLiteVideoPlayed= false;
	L2_S2_CutSceneVideoPlayed= false;
	L2_S2_NagaPearl1Taken= false;
	L2_S2_NagaPearl2Taken= false;
	L2_S2_NagaPearl3Taken= false;
	L2_S2_NagaPearl4Taken= false;
	L2_S2_NagaLeaf1Taken = false;
	L2_S2_NagaLeaf2Taken = false;
	L2_S2_pop3_R1Color = 0;
	L2_S2_pop3_R2Color = 0;
	L2_S2_pop3_R3Color = 0;
	L2_S2_pop3_R4Color = 0;
	L2_S2_pop3_R5Color = 0;
	L2_S2_pop3_R6Color = 0;
	L2_S2_pop3_R7Color = 0;
	L2_S2_pop3_R8Color = 0;
	L2_S2_pop3_R9Color = 0;
	L2_S2_pop3_R10Color = 0;
	L2_S2_pop3_R11Color = 0;
	L2_S2_pop3_R12Color = 0;
	L2_S2_pop3_R13Color = 0;
	L2_S2_pop3_R14Color = 0;
	L2_S2_kolamComplete= false;

	L2_S3_Pop1_FirstDigPlayed = false;
	L2_S3_Pop1_SecondDigPlayed = false;
	L2_S3_Pop1_PluckingTurmericPlayed = false;
	L2_S3_Pop1_FrogJumpPlayed = false;
	L2_S3_InvTurmericPicked = false;
	L2_S3_InvPoojaLampPicked = false;

	L2_S3_isStoneBeamPuzzleDataSaved = false;
	for (int i = 0; i < 10; i++)
	{
		L2_S3_PZStoneBeamCircleRotation[i] = 0;
		L2_S3_PZStoneBeamCircleXPos[i] = 0.0f;
		L2_S3_PZStoneBeamCircleYPos[i] = 0.0f;
		L2_S3_PZStoneBeamCircleCurrentPos[i] = 0;
		L2_S3_PZStoneBeamCircleOriginalPos[i] = 0;
	}
	L2_S3_StonePathActivated = false;
	L2_S3_IsStonePathActivatedMessageShown = false;
	L2_S2_InvNagaPearlsCollected = false;

	L2_S4_IsWoodsCut1 = false;
	L2_S4_IsWoodsCut2 = false;
	L2_S4_InvFirewoodTaken = false;
	L2_S4_InvAxeBluntTaken = false;
	L2_S4_IsAxeSharpened = false;
	L2_S4_InvAxeSharpenTaken = false;
	L2_S4_InvFireTorchTaken = false;
	L2_S4_InvFireTorchStickOiledTaken = false;
	L2_S4_IsLasyaAttackVideoPlayed = false;
	L2_S4_FireTorchOiled = false;
	L2_S4_RockSlabMoved1 = false;
	L2_S4_BrickPuzzleAreaClicked = false;
	L2_S4_RockSlabRemoved = false;
	L2_S4_InvFireTorchFlamed = false;
	L2_S4_InvRakeTaken = false;
	L2_S4_DeitiesPuzzleState = 0;
	L2_S4_InvMetalLeverTaken = false;
	for (int i = 0; i < MAX_L2_S4_DEITIESTILES; i++)
	{
		L2_S4_DeitiesTileIndices[i] = -1;
	}
	L2_S5_IsVisited = false;
	L2_S5_InvKnifeTaken = false;
	L2_S5_InvAmberTaken = false;
	L2_S5_InvPotTaken = false;

	L2_S5_VinegarPlaced = false;
	L2_S5_VinegarCorkOpened = false;
	L2_S5_KnifeSharpened = false;
	L2_S5_KnifeSharpTaken = false;

	L2_S5_MangoPlaced = false;
	L2_S5_MangoSliced = false;
	L2_S5_MangoAddedToPot = false;

	L2_S5_HoneyWaxPlaced = false;
	L2_S5_HoneyWaxSliced = false;
	L2_S5_HoneyWaxAddedToPot = false;

	L2_S5_MangoHoneyLiquidCreated = false;
	L2_S5_MangoHoneyLiquidApplied = false;

	L2_S5_AloveraPlaced = false;
	L2_S5_AloveraSliced = false;
	L2_S5_AloveraApplied = false;

	L2_S5_WoodPlaced = false;
	L2_S5_WoodFired = false;
	L2_S5_PuzzleModeActive = false;
	L2_S5_PotWithWaterPlaced = false;

	L2_S5_TurmericPlaced = false;
	L2_S5_TurmericSliced = false;
	L2_S5_TurmericAddedToPot = false;

	L2_S5_TurmericPowderCreated = false;
	L2_S5_TurmericPowderApplied = false;

	L2_S5_MedicineCreated = false;
	L2_S5_MedicineMixed = false;
	L2_S5_MedicineGivenToMooppan = false;
	L2_S5_ClickedOnMooppan = false;
	L2_S5_InvPujaChartTaken = false;
	L2_S5_InvNagaSculptureTaken = false;
	L2_S5_ShelfPuzzleSolved = false;
	L2_S5_EagleBlockPuzzleState = 0;
	for (int i = 0; i < MAX_L2_S5_EAGLEBLOCKS; i++)
	{
		L2_S5_EagleBlockIndices[i] = -1;
	}
	L2_S5_StrawBasketRemoved = false;
	L2_S5_InvStoneSharpnerTaken = false;
	L2_S5_InvVinegarTaken = false;
	L2_S5_InvMedicineTaken = false;
	L2S6_InvGlovesTaken = false;
	L2S6_InvRustedKnifeTaken = false;
	l2S6_Inv3XStonesTaken = false;
	L2S6_Pop1Visited = false;
	L2S6_HOWaterFallCompleted = false;
	L2S6_HOWaterFallCompletedWithMatch3 = false;
	for (int i = 0; i < MAX_L2S6_HOOBJECTS; i++)
	{
		L2S6_HOItemsFound[i] = false;
	}

	L2S6_InvSmallStone1Taken = false;
	L2S6_InvSmallStone2Taken = false;
	L2S6_InvSmallStone3Taken = false;
	L2_S6_IsWoodCutDown = false;
	L2_S6_IsP2LocketOpen = false;
	L2_S6_IsP3HolderOpen = false;
	L2_S6_IsP3PaperPuzClueTaken = false;
	//3 saved number Ids of holder
	L2_S6_P3CenterNums[0] = 5;
	L2_S6_P3CenterNums[1] = 4;
	L2_S6_P3CenterNums[2] = 2;
	L2_S6_NagaPearlFixed = false;
	L2_S6_Shutter1Solved = false;
	L2_S6_Shutter2Solved = false;
	L2_S6_Shutter3Solved = false;
	L2_S6_ClockPuzzleSolved = false;
	L2_S6_PaperPuzCluePlaced = false;
	L2_S6_IsP6OilLampLightened = false;
	L2_S6_Rock1Moved = false;
	L2_S6_Rock2Moved = false;
	L2_S6_LeverUsed = false;
	L2S6_InvStoneBallPlaced = false;

	L2_S7_Puzzle1Solved = false;
	L2_S7_Puzzle2Solved = false;
	L2_S7_Puzzle3Solved = false;
	L2_S7_Puzzle4Solved = false;
	L2_S7_Puzzle5Solved = false;
	L2_S7_Puzzle6Solved = false;
	L2_S7_Puzzle7Solved = false;
	L2_S7_SoldierPuzzleState = 0;
	L2_S7_CurrentPuzzle = 1;

	//Level 3

	L3_S1_Visited = false;
	for (int i = 0; i < 12; i++)
	{
		L3_S1_Objectives[i] = false;
	}

	L3_S1_InvFlagFullTaken = false;
	L3_S1_InvFlagStickTaken = false;
	L3_S1_InvFlagClothTaken = false;
	L3_S1_EmberPopupState = 0;
	L3_S1_InvLadderStep1Taken = false;
	L3_S1_InvGreenCrystalTaken = false;
	L3_S1_LadderPiecesFixed = false;
	L3_S1_LadderBrokenStepFixed = false;

	for (int i = 0; i < MAX_L3S1_HOOBJECTS; i++)
		L3_S1_HOObjFound[i] = false; 
	for (int i = 0; i < MAX_HUDL3S1_SYMBOLS; i++)
		L3_S1_HOHudObjID[i]=0;
	L3_S1_HONextHudSymbolID = MAX_HUDL3S1_SYMBOLS;
	L3_S1_HOHudSymbolsInitialized = false;
	L3_S1_HOLadderCompleted = false;
	L3_S1_HOLadderCompletedWithMatch3 = false;
	L3_S1_InvBrokenLadderTaken = false;
	L3_S1_InvLadderFixedTaken = false;

	L3_S1_P1LionHeadPuzzleCompleted = false;
	L3_S1_invGreenEyeCollected = false;
	for (int i = 0; i < 4; i++)
	{
		L3_S1_P1LionHeadRingAngles[i] = -1;
	}
	L3_S2_InvCocoShellPowderFilledTaken = false;
	L3_S2_InvTwineTaken = false;

	L3_S2_MazeCurrentRoom = eArea_L3_S2_P1Room1Start;
	L3_S2_MazeMapUsed = false;

	L3_S2_InvSickleTaken = false;
	L3_S2_RockMoved = false;
	L3_S2_TwineReleased = false;
	L3_S2_Thread1Taken = false;
	L3_S2_Thread2Taken = false;
	L3_S2_Thread3Taken = false;
	L3_S2_Thread4Taken = false;

	L3_S2_InvCoconutOuterRemoved = false;
	L3_S2_InvCoconutShellTaken = false;
	L3_S2_CoconutComboTwineFixed = false;
	L3_S2_InvCoconutWithTwineTaken = false;
	L3_S2_CrowFlewAway = false;
	L3_S2_PoolDoorOpened = false;

	L3_S2_InvHammerTaken = false;
	L3_S2_InvHammerHandTaken = false;
	L3_S2_ToothFixed = false;
	L3_S2_BackDoorOpened = false;

	L3_S2_InvCoconnutTaken = false;
	L3_S2_LeafMoved = false;
	L3_S2_Stick1Moved = false;
	L3_S2_Stick2Moved = false;

	L3_S2_BrickTaken = false;
	L3_S2_CocoShellPowderFilledFixed = false;
	L3_S2_TwineGunPowderFixed = false;
	L3_S2_BrickFixed = false;
	L3_S2_WallBrocken = false;

	for (int i = 0; i < 3; i++)
		L3_S2_vineRemovedR1Status[i] = false;

	L3_S2_InvPipe1R1Taken = false;

	for (int i = 0; i < 3; i++)
		L3_S2_vineRemovedR2Status[i] = false;

	for (int i = 0; i < 2; i++)
		L3_S2_R2RemovedMudOnPipe[i] = false;

	L3_S2_InvPipe2R2Taken = false;
	L3_S2_InvToothR2Taken = false;
	L3_S2_P1PlayerTile = 0;
	for (int i = 0; i < 4; i++)
		L3_S2_P1CurrentTiles[i] = -1;

	for (int i = 0; i < 3; i++)
		L3_S2_R3DoorOpen[i] = false;

	L3_S2_InvPipe3R3Taken = false;
	L3_S2_InvHammerHeadTaken = false;
	L3_S2_R3StonesRemoved = false;

	L3_S2_M4DoorsOpened = false;
	L3_S2_InvPipe4R4Taken = false;
	for (int i = 0; i < 8; i++)
	{
		L3_S2_M4SliderLeftRemoved[i] = false;
	}
	for (int i = 0; i < 8; i++)
	{
		L3_S2_M4SliderRightRemoved[i] = false;
	}

	L3_S2_M5_PipePuzzleActivated = false;
	for (int i = 0; i < MAX_PIPE_INDEX; i++)
	{
		L3_S2_M5_PipeGridIndex[i] = -1;
	}

	static int PipeAngles[] = {0, 90, 180, 270};
	for (int i = 0; i < MAX_PIPE_INDEX; i++)
	{
		L3_S2_M5_PipeAngles[i] = PipeAngles[rand()%4];
	}

	L3_S2_M5_PipePuzzleCompleted = false;
	L3_S2_M5_PipeSceneDoorsOpend = false;

	L3_S2_M6_SceneState = 0;
	L3_S2_M6_PillerPuzzleComplete = false;
	for (int i = 0; i < 3; i++)
	{
		L3_S2_M6_PillerPositions[i] = 0;
	}
	for (int i = 0; i < 2; i++)
	{
		L3_S2_M6_CollectedBoxIndex[i] = -1;
	}

	L3_S3_VisitingPoolForFirsttime = false;
	L3_S3_IsDoorToS4Opened= false;
	L3_S3_PZBlocksPlaced= false;
	L3_S3_PZUnderwaterPuzzleSolved= false;
	L3_S3_PZGoingUnderwaterFirstTime = false;
	L3_S3_IsTempleRisenFromWater= false;
	L3_S3_InvLadderStep2Taken= false;
	L3_S3_InvCupFilledTaken= false;
	L3_S3_InvCupTaken= false;
	L3_S3_IsTempleGlassBroken= false;
	L3_S3_InvTempleBoxTaken= false;
	L3_S3_InvSnakeDoorPartTaken = false;
	L3_S3_InvSawTaken = false;
	L3_S3_InvShowelTaken = false;
	L3_S3_IsRockMovedSaw1 = false;
	L3_S3_IsRockMovedSaw2 = false;
	L3_S3_IsRootLeftSawed = false;
	L3_S3_IsRootRightSawed = false;
	L3_S3_IsSecretBoxOpened = false;

	int puzzle[] =  {0, 7, 6, 8, 5, 2, 3, 1};
	for (int i = 0; i < 8; i++)
	{
		L3_S3_PZBlockTileIndex[i] = puzzle[i];
	}
	L3_S3_PZBlockState = 0;

	for (int i = 0; i < 8; i++)
	{
		L3_S3_PZPinkOuterKeyState[i] = false;
		L3_S3_PZRedOuterKeyState[i] = false;
		L3_S3_PZBlueOuterKeyState[i] = false;
	}
	for (int i = 0; i < 8; i++)
	{
		L3_S3_PZPinkDiagonalKeyState[i] = false;
		L3_S3_PZRedDiagonalKeyState[i] = false;
		L3_S3_PZBlueDiagonalKeyState[i] = false;
	}
	for (int i = 0; i < 4; i++)
	{
		L3_S3_PZPinkInnerKeyState[i] = false;
		L3_S3_PZRedInnerKeyState[i] = false;
		L3_S3_PZBlueInnerKeyState[i] = false;
	}

	for (int i = 0; i < 16; i++)
	{
		L3_S4_HOFortFountainFound[i] = false;
	}

	L3_S4_HOFortFountainCompleted = false;

	L3_S4_IsVisitedOnce = false;
	L3_S4_InvSharpRodTaken = false;
	L3_S4_InvBambooStickTaken = false;
	L3_S4_InvMetalSheetBlankTaken = false;
	L3_S4_InvMapDrawnSheetTaken = false;
	L3_S4_IsLeaf1OverMetalRemoved = false;
	L3_S4_IsLeaf2OverMetalRemoved = false;
	L3_S4_IsAmeehaDiaryNoteTaken = false;
	L3_S4_IsAmeehaCutscenePlayed = false;
	L3_S4_IsNagaLockPlaced = false;
	L3_S4_IsCliffDoorOpened = false;
	L3_S4_IsPop2AMapDoorOpened = false;
	L3_S4_IsPop2ABlankSheetPlaced = false;
	L3_S4_IsPop2AStatueMoved = false;
	L3_S4_IsPop2ASnakeButtonPressed = false;
	L3_S4_IsPop2BLeaf1Removed = false;
	L3_S4_IsPop2BLeaf2Removed = false;
	L3_S4_IsPop2BStoneRemoved = false;
	L3_S4_IsPop2BSnakeButtonPressed = false;
	L3_S4_IsPop2CSnakeButtonPressed = false;
	L3_S4_IsPop2CStoneRemoved = false;
	L3_S4_IsPop2CLeavesRemoved = false;
	L3_S4_IsPop2DSnakeButtonPressed = false;
	L3_S4_IsPop2DStoneRemoved = false;
	L3_S4_IsPop2DLeavesRemoved = false;
	L3_S4_IsPop4StoneRemoved = false;
	L3_S4_IsPop4LeavesRemoved = false;
	L3_S4_InvBambooWithSickleTaken = false;

	L3_S5_InvRopeTaken = false;
	L3_S5_InvClipTaken = false;
	L3_S5_GreenStonePlaced = false;
	L3_S5_DeerGone = false;
	L3_S5_ClipPlaced = false;
	L3_S5_RopeTied = false;
	L3_S5_NailPlaced = false;
	L3_S5_NailHammered = false;

	L3_S5_DryGrassTaken1 = false;
	L3_S5_DryGrassTaken2 = false;
	L3_S5_DryGrassTaken3 = false;
	L3_S5_DryGrassTaken4 = false;
	L3_S5_DryGrassTaken5 = false;
	L3_S5_DryGrassTaken6 = false;
	L3_S5_DryGrassTaken7 = false;
	L3_S5_DryGrassTaken8 = false;
	L3_S5_DryGrassTaken9 = false;
	L3_S5_DryGrassTaken10 = false;
	L3_S5_DryGrassTaken11 = false;

	L3_S5_InvGrassTaken1 = false;
	L3_S5_InvGrassTaken2 = false;
	L3_S5_InvGrassTaken3 = false;
	L3_S5_InvGrassTaken4 = false;
	L3_S5_InvGrassTaken5 = false;
	L3_S5_InvGrassTaken6 = false;

	L3_S5_InvAllGrassTaken = false;


	//Level 4
	L4_S1_IsFirstVisit = true;
	L4_S1_PocketOpend = false;
	L4_S1_InvHandKerchiefTaken = false;
	L4_S1_InvCrutchTaken = false;
	L4_S1_SkimmerFlyed = false;
	L4_S1_InvStickTaken = false;
	L4_S1_InvLeafTaken = false;
	L4_S1_InvLeafSpoonWithWaterTaken = false;
	L4_S1_InvHerbalLeavesTaken = false;
	L4_S1_InvBraceletTaken = false;
	L4_S1_FirstThornRemoved = false;
	L4_S1_SecondThornRemoved = false;
	L4_S1_ShoeRemoved = false;
	L4_S1_BloodRemoved = false;
	L4_S1_MedPasteApplied = false;
	L4_S1_WoundTied = false;
	L4_S1_PutShoeBack = false;
	L4_S1_TwigsMoved = false;
	L4_S1_InvLeafSpoonCollected = false;
	L4_S1_LeafTored = false;
	L4_S1_HandKerchiefTored = false;
	L4_S1_PutLeftShoe = false;
	L4_S1_CrutchUsed = false;

	L4_S1_PuzzleSolved = false;
	L4_S1_InvBinocularLensTaken = false;
	L4_S1_InvLeftShoeTaken = false;
	L4_S1_PuzzleVisited = false;
	for (int i = 0; i < 12; i++)
	{
		L4_S1_HOShoeFound[i] = false;
	}

	L4_S1_HOShoeCompleted = false;
	L4_S1_BetelBoxOPened = false;
	L4_S1_InvKnifeCollected = false;

	L4_S2_FruitsPlucked					=  false;
	L4_S2_FruitsTaken					=  false;
	L4_S2_AlligatorPuzzleSolved			=  false;
	L4_S2_AlligatorPuzzleState			=  0;
	L4_S2_BambooStickTaken				=  false;
	L4_S2_Pop2BambooTaskCompleted		=  false;
	L4_S2_GrassTaken					=  false;
	L4_S2_ComboKnifeAddedToBamboo		= false;
	L4_S2_InvBambooSpearTaken			= false;
	L4_S2_InvRopeTaken					=  false;
	L4_S2_InvGrinderHandleTaken         = false;
	L4_S2_DoorToCaveOpened              = false;
	L4_S2_FanPuzzleState				= 0;
	L4_s2_GrassRopeFrameNumber			= 0;
	L4_S2_MortarTaken					= false;

	L4_S3_IsCaveLit 					= false;
	L4_S3_IsDoorPuzzleSolved			= false;
	L4_S3_IsBowPlaced					= false;
	L4_S3_IsEmberCreated				= false;
	L4_S3_IsOilApplied					= false;
	L4_S3_IsFireSticksPlaced			= false;
	L4_S3_IsCoinPlaced					= false;
	L4_S3_IsPlankPlaced					= false;
	L4_S3_IsInvBowlTaken				= false;
	L4_S3_IsMetalPaperTaken				= false;
	L4_S3_IsPotRemoved					= false;

	L4_S4_Visited = false;
	L4_S4_SkeletonHandOpened = false;
	L4_S4_InvBinocularTaken = false;
	L4_S4_FlowerCutscenePlayed = false;
	L4_S4_GlobGlowEnabled = false;
	L4_S4_GrinderHandlePlaced = false;
	L4_S4_OlivePlaced = false;
	L4_S4_StrainerPlaced = false;
	L4_S4_PotPlaced = false;
	L4_S4_InstructionsPlaced = false;
	L4_S4_OilMachinePlaced = false;
	L4_S4_OilMachineRotated = false;
	L4_S4_PotWithOilTaken = false;
	L4_S4_InvBinocularWithLensTaken = false;
	L4_S4_JumpCutscenePlayed = false;
	L4_S4_ClimbCutscenePlayed = false;

	L4_S2_InvTwig1Taken =  false;
	L4_S4_InvTwig2Taken =  false;
	L4_S5_InvTwig3Taken =  false;

	L4_S5_InvAlligatorHeadTaken	= false;
	L4_S5_InvDoorKeyTaken		= false;
	L4_S5_SwapPuzzleCompleted	= false;

	for (int i = 0; i < 6; i++)
	{
		L4_S4_HOBinocularFound[i] = false;
	}

	L4_S4_HOBinocularCompleted = false;
	L4_S4_BannerTextAfterBinocularHOShown = false;

	L4_S2_InvSmallBambooStick0Taken = false;
	L4_S2_InvSmallBambooStick1Taken = false;
	L4_S2_InvSmallBambooStick2Taken = false;
	L4_S2_InvSmallBambooStick3Taken = false;

	L4_S2_IsBambooStickCut = false;

	L4_S5_Visited = false;
	L4_S5_InvDoorTaken = false;
	L4_S5_ScrewRemoved = false;
	L4_S5_ClothRemoved = false;
	L4_S5_InvOlivesCollected = false;
	L4_S5_OilMachineBaseCollected = false;
	L4_S5_InvNetTaken = false;
	L4_S5_ComboBambooFrameAddedToNet = false;
	L4_S5_InvBambooStrainerTaken = false;
	L4_S5_DoorPlaced  = false;

	//Level5
	L5_S1_InvBowTaken = false;
	L5_S1_InvBowWithArrowFirstTaken = false;
	L5_S1_InvBowWithArrowSecondTaken = false;
	L5_S1_InvArrowHolderTaken = false;
	L5_S1_BowStringFixed = false;
	L5_S1_BowArrowHolderPlaced = false;
	L5_S1_BowArrowFirstLoaded = false;
	L5_S1_BowArrowSecondLoaded = false;
	L5_S1_BowToTigerCutscenePlayed = false;

	L5_S2_InvHammerTaken = false;
	L5_S2_InvSoldierBadgeTaken = false;

	for (int i = 0; i < MAX_L5S2_NUM_BLOCKS; i++)
		L5_S2_PZBlockStartRow[i] = -1;
	for (int i = 0; i < MAX_L5S2_NUM_BLOCKS; i++)
		L5_S2_PZBlockStartCol[i] = -1;

	L5_S2_PZBlocksPuzzleSolved = false;
	L5_S2_InvBadgeTaken = false;
	L5_S2_LogPushed = false;
	L5_S2_LogFell = false;
	L5_S2_GreaseTaken = false;

	L5_S3_IntroVdoCompleted = false;
	L5_S3_memClothTaken = false;
	L5_S3_EmblemPlacedOnStone = false;
	L5_S3_PaperCollectedFromStone = false;
	L5_S3_InvMoonSolutionTaken = false;
	L5_S3_VineFreed = false;
	L5_S3_isHoActive = false;
	L5_S3_isLostHook = false;
	L5_S3_PZHaveHook = true;
	L5_S3_HookLostBannerShowed = false;
	L5_S3_LeftDoorOpened = false;
	L5_S3_EnteredLeftDoor = false;
	L5_S3_PZState = 0;
	int tmpInitialState[16] = {0,12,4,8,13,5,1,2,10,11,7,15,14,3,9,6};
	for (int i = 0; i < 16; i++)
	{
		L5_S3_PZLocketIndexes[i] = tmpInitialState[i];
	}
	L5_S3_PZCompleted = false;
	L5_S3_HOUnderWaterStarRemoved = false;
	for (int i = 0; i < 12; i++)
	{
		L5_S3_HOUnderWaterFoundObjs[i] = false;
	}
	L5_S3_HOUnderWaterCompleted = false;


	L5_S4_InvPickaxeTaken = false;
	for (int i = 0; i < 6; i++)
	{
		L5_S4_KeyMatchComplete[i] = false;
	}
	L5_S4_Level1Completed = false;
	L5_S4_PZHandCuffState = 0;
	L5_S4_PZHandCuffComplete = false;
	for (int i = 0; i < 3; i++)
	{
		L5_S4_PZHandCuffLockUnlocked[i] = false;
	}
	L5_S4_InvPickaxeHeadTaken = false;
	L5_S4_GreaseUsed = false;
	L5_S4_BridgeOpened = false;
	L5_S4_Stone1Moved = false;
	L5_S4_Stone2Moved = false;
	L5_S4_InvBarkTaken = false;
	L5_S4_PickAxeUsed = false;
	L5_S4_PickAxeCutScenePlayed = false;
	L5_S4_AnasuyaTalked = false;
	L5_S4_InvChainTaken = false;
	L3_S4_PZ1BridgeCompleted = false;
	for (int i = 0; i < 5; i++)
	{
		L3_S4_PZ1BridgeRingAngles[i] = -1;
	}

	L5_S5_InvPickaxeHandleTaken = false;
	L5_S5_InvHandcuffKeyTaken = false;
	L5_S5_InvWaterproofBagTaken = false;
	L5_S5_InvBambooLadderTaken = false;
	L5_S5_InvStoneEmblemTaken = false;
	L5_S5_IsAmeehaDiaryCutscenePlayed = false;
	L5_S5_IsSoldierBadgeUsedOnDoor = false;
	L5_S5_IsBambooLadderPlacedOnScene = false;
	L5_S5_IsMoonGlassDoorOpened = false;
	L5_S5_IsTopDoorOpened = false;
	L5_S5_IsP1Stone1Removed = false;
	L5_S5_IsP1Stone2Removed = false;
	L5_S5_IsP1PillerRemoved = false;
	for (int i = 0; i < 14; i++)
	{
		L5_S5_HOGanpathiFoundObjs[i] = false;
	}
	L5_S5_HOGanpathiCompleted = false;
	L5_S5_HOGanapathiActivated = false;

	for (int i = 0; i < MAX_L5S5_PZBLOCKS; i++)
	{
		L5_S5_PZTokenState[i]=0;
	}
	L5_S5_PZSolutionPaperPlaced = false;
	L5_S5_PZStatesInitialized = false;
	L5_S5_IsSunClockPuzzleCompleted = false;

	L5_S6_HOSoldierBadgePlaced = false;
	L5_S6_HOGuardianIntroTalkCompleted = false;
	for (int i = 0; i < MAX_L5S6_HOOBJECTS; i++)
	{
		L5_S6_HOObjectFound[i] = false;
	}
	L5_S6_HOCompleted = false;
	L5_S6_PeacockChainUsed = false;
	L5_S6_InvPeacockLocketTaken = false;
	L5_S6_PeacockLocketUsed = false;
	L5_S6_PeacockArrivingCutScenePlayed = false;
	L5_S6_PeacockLeavingCutScenePlayed = false;
	L5_S6_IntroCutScenePlayed = false;
	L5_S6_GotFreeBannerShown = false;

	//Level 6

	L6_S1_Visited  = false;
	L6_S1_ChappalKnobRemoved = false;
	L6_S1_LetterOpened = false;
	L6_S1_VaruniCutscenePlayed = false;
	L6_S1_MaryFirstBannerTextShown = false;
	L6_S1_MarySecondBannerTextShown = false;
	L6_S1_ScrewUnsealed = false;
	L6_S1_InvEmblemTaken = false;
	L6_S1_InvMortarTaken = false;
	L6_S1_InvSteelTaken = false;
	L6_S1_SolutionFound = false;
	L6_S1_InvClothTaken = false;
	L6_S1_StarKeyPlaced = false;
	L6_S1_EmblemPlaced = false;
	L6_S1_PathToS3Activated = false;
	L6_S1_InvClubTaken = false;
	L6_S1_TextForHOShown = false;

	for (int i = 0; i < MAX_L6S1_HOSCENES; i++)
	{
		L6_S1_SceneHOSolved[i] = false;
	}
	L6_S1_DescriptionHOSolved = false;

	for (int i = 0; i < 4; i++)
		L6_S1_DescHOFound_S1[i] = false;
	for (int i = 0; i < 2; i++)
		L6_S1_DescHOFound_S2[i] = false;
	for (int i = 0; i < 3; i++)
		L6_S1_DescHOFound_S3[i] = false;
	for (int i = 0; i < 4; i++)
		L6_S1_DescHOFound_S4[i] = false;
	for (int i = 0; i < 2; i++)
		L6_S1_DescHOFound_S5[i] = false;
	L6_S1_InvHorseShoeTaken = false;
	L6_S1_LetterTaken = false;
	L6_S1_ChestPuzzleSolved = false;

	L6_S2_DoorToS3Opened = false;
	L6_S2_InvIceKeyTaken = false;
	L6_S2_InvPestleTaken = false;
	L6_S2_ColoredPowderTrowed = false;

	L6_S2_HOComplete = false;
	for (int i = 0; i < 7; i++)
	{
		L6_S2_HOItemsFound[i] = false;
	}
	L6_S2_InvPuzzlePiecesCollected = false;
	L6_S2_PuzzleComplete = false;
	L6_S2_InvPanTaken					= false;
	L6_S2_InvPanWithPinePitchPlaced		= false;
	L6_S2_InvStickPlaced				= false;
	L6_S2_InvFlintPlaced				= false;
	L6_S2_InvSteelPlaced				= false;
	L6_S2_InvMortarWithCharcoalPlaced	= false;
	L6_S2_InvPestlePlaced				= false;
	for (int i = 0; i < MAX_L6S2_PZOBJECTS; i++)
		L6_S2_PZPineGumObjectsUsed[i] = 0;

	L6_S3_RoomLightened = false;
	L6_S3_JamesDialogsShown = false;
	L6_S3_LasyaCutscenePlayed = false;
	L6_S3_ClubsUsed = false;
	L6_S3_InvCharcoalTaken = false;
	L6_S3_InvHorseShoeTaken = false;
	L6_S3_SteelPlaced = false;
	L6_S3_FlintStoneUsed = false;
	L6_S3_PathwayActived = false;
	L6_S3_PathwayDestructed = false;
	L6_S3_PZPathwayPuzzleCompleted = false;
	for (int i = 0; i < 5; i++)
		L6_S3_PZPathwayRingAngles[i] = -1;

	L6_S3_HO_Visited = false;
	L6_S3_HO_CodeTaken = false;
	L6_S3_HO_CodeApplied = false;
	L6_S3_HO_CodePuzzleSolved = false;
	L6_S3_HO_BottleRemoved = false;
	L6_S3_HO_PotRemoved = false;
	L6_S3_HO_MainLockOpened1 = false;
	L6_S3_HO_MainLockOpened2 = false;
	L6_S3_HO_MainLockOpened3 = false;
	L6_S3_HO_MainBoxOpened = false;
	L6_S3_HO_InvHeartTaken = false;
	L6_S3_HO_VaseToppled = false;
	L6_S3_HO_Bolt1Removed=false;
	L6_S3_HO_Bolt2Removed=false;
	L6_S3_HO_VaseDoorRemoved= false;
	L6_S3_HO_CartPosition = 0;
	for (int i = 0; i < 6; i++)
		L6_S3_HO_ObjectStates[i] = -1;

	L6_S4_IsVisited = false;
	L6_S4_InvStickTaken = false;
	L6_S4_InvRopeTaken = false;
	L6_S4_InvPineGumCollected = false;
	L6_S4_InvPineGumStickCollected = false;
	L6_S4_PanPlaced = false;
	L6_S4_RockBrokened = false;
	L6_S4_InvFlintStoneCollected = false;
	L6_S4_IsHorseShoeMagnetized = false;
	L6_S4_InvMagnetisedHorseShoeTaken = false;
	L6_S4_InvRopeHorseShoeComboTaken = false;
	L6_S4_InvMagnifyingGlassTaken = false;
	L6_S4_KeyInIcePlaced = false;
	L6_S4_InvStarShapedKeyTaken = false;
	L6_S4_AmeehaNoteTaken = false;
	L6_S4_MaryTextShown = false;
	L6_S4_HeartCrystalPlaced = false;
	L6_S4_GlueApplied = false;
	L6_S4_ChestOpened = false;
	L6_S4_NoteOpened = false;
	L6_S4_InvColoredPowderTaken = false;
	L6_S4_IsRockManDialogOver  = false;
	L6_S4_MaryWalkingCutscenePlayed = false;

	L6_S4_PuzzlePiecesPlaced = false;
	for (int i = 0; i < 7; i++)
	{
		L6_S4_PortalPuzzleObjectStates[i] = false;
	}
	L6_S4_PuzzleComplete = false;
	L6_S4_LetterFromAnanthaTaken = false;

	L6_S5_FoundSophia = false;
	L6_S5_Visited = false;
	L6_S5_DialogsOver = false;
	L6_S5_SophiaTabletDialogOver = false;

	L6_S5_TabMiniGameSceneNumber = 0;
	L6_S5_TabMiniGameState = 0;
	L6_S5_TabMiniGamePluckedFruitIndex = -1;
	L6_S5_TabMiniGameIsClothRemoved = false;
	for (int i = 0; i < 5; i++)
	{
		L6_S5_TabMiniGameIsCarrotPicked[i] = false;
	}
	L6_S5_TabMinigameSolved = false;

	L6_S5_HO_Complete = false;
	for (int i = 0; i < 10; i++)
	{
		L6_S5_HOObjFound[i] = false;
	}

	for (int i = 0; i < MAX_L7S1_JIGSAWPIECES; i++)
		L7_S1_PZJigsawYellowPlaced[i] = false;
	L7_S1_PZJigsawYellowCompleted = false;
	for (int i = 0; i < 3; i++)
	{
		L7_S1_PZStarDoorOpened[i] = false;
		L7_S1_InvPZStarEmblemCollected[i] = false;
	}

	L7_S1_InvPotEmptyTaken = false;
	L7_S1_ComboPotChainPlaced = false;
	L7_S1_InvPotWithChainTaken = false;	
	L7_S1_InvAxeUsed = false;
	L7_S1_InvRedStarUsed = false;
	L7_S1_WaterUsedOnRedFlower = false;
	L7_S1_InvRedFlowerTaken = false;

	L7_S1_WellLidOpened = false;
	L7_S1_InvWheelUsed = false;
	L7_S1_InvMagicWaterTaken = false;
	L7_S1_InvPotTaken = false;
	for (int i = 0; i < 8; i++)
		L7_S2_PZEyePartIndex[i] = -1;
	L7_S2_PZEyeGameState = 0;
	L7_S2_PZEyePuzzleCompleted = false;
	L7_S2_InvSymbolDiscTaken = false;
	L7_S2_InvStatueTaken = false;	

	for (int i = 0; i < MAX_L7S2_HANDHOSYMBOLS; i++)
		L7_S2_HOLeftHandSymbolFound[i] = false;
	for (int i = 0; i < MAX_L7S2_HANDHOSYMBOLS; i++)
		L7_S2_HORightHandSymbolFound[i] = false;
	L7_S2_HORightHandCompleted = false;
	L7_S2_HORightHandDiskPlaced = false;
	L7_S2_HOLeftHandDiskPlaced = false;
	L7_S2_IsRightHandExplodeShown = false;
	L7_S2_HOLeftHandCompleted = false;
	L7_S2_IsLeftHandExplodeShown = false;
	L7_S2_HOBrokenRightHandCompleted = false;
	L7_S2_HOBrokenLeftHandCompleted = false;
	L7_S2_InvSphereHalfLeftToFixTaken = false;
	L7_S2_InvSphereHalfRightBaseTaken = false;
	L7_S2_InvSphereFullTaken = false;
	L7_S2_PopSphereFullPlaced = false;
	L7_S2_LasyaStatueRisenUp = false;	
	L7_S2_HOBrockenLeftHandState = 0;
	for (int i = 0; i < 6; i++)
		L7_S2_HOBrockenLeftHandSymbolCompleted[i] = false;
	L7_S2_HOBrockenRightHandState = 0;
	for (int i = 0; i < 6; i++)
		L7_S2_HOBrockenRightHandSymbolCompleted[i] = false;

	L7_S2_PZLasyaStatuePlaced = false;
	L7_S2_PZLasyaStatuePuzzleCompleted = false;
	for (int i = 0; i < MAX_L7S2_LASYAITEMS; i++)
		L7_S2_PZLasyaDiffFound[i] = false;
	for (int i = 0; i < MAX_L7S3_JIGSAWPIECES; i++)
		L7_S3_PZJigsawRedPlaced[i] = false;
	L7_S3_PZJigsawRedCompleted = false;

	L7_S3_InvAxesTaken = false;
	L7_S3_InvAxeKeyTaken = false;
	L7_S3_IsAxeKeyUsed = false;
	L7_S3_InvLeverTaken = false;
	L7_S3_IsLeverUsedOnChain = false;
	L7_S3_InvMetalWireTaken = false;
	L7_S3_InvMetalChainTaken = false;
	L7_S3_InvBhasmamPowderTaken = false;
	L7_S3_IsBroomUsedOnBhasmam = false;
	L7_S3_InvBroomTaken = false;
	L7_S3_InvYellowFlowerTaken = false;
	L7_S3_IsBullMoved = false;
	L7_S3_IsLetterAmeehaTaken = false;
	L7_S3_IsDiwaliLampPlaced = false;
	L7_S3_PZDoorState = 0;
	for (int i = 0; i < 3; i++)
		L7_S3_LocksState[i] = false;
	L7_S3_PZDoorCompleted = false;
	L7_S3_IsLasyaAttackCutscnPlayed = false;
	L7_S3_IsNightmareCutscnPlayed = false;

	for (int i = 0; i < MAX_L7S4_JIGSAWPIECES; i++)
		L7_S4_PZJigsawBluePlaced[i] = false;
	L7_S4_PZJigsawBlueCompleted = false;
	L7_S4_HaveHandPower = false;
	L7_S4_InvBlueFlowerTaken = false;
	for(int i = 0; i < 3; i++)
		L7_S4_TableTopFlowerPos[i] = -1;
	L7_S4_TableTopBasmamUsed = false;
	L7_S4_TableTopBasmamComplete = false;
	L7_S4_TableTopPoojaPopComplete = false;
	L7_S4_TableTopPoojaPuzzleMode = -1;

	L7_S4_InvWaterUsedOnBlueFlower = false;
	L7_S4_InvBlueStarUsed = false;
	L7_S4_InvWellWheelTaken = false;
	L7_S4_P1NoteClicked = false;
	L7_S4_P1NoteTaken = false;
	L7_S4_AmehaTalked = false;

	L7_S5_End1CutScenePlayed = false;
	L7_S5_End2CutScenePlayed = false;L8_S1_IntroVidPlayed = false;
	for (int i = 0; i < MAX_L8_MAGIC_SYMBOLS; i++)
		L8_MagicSymbolFound[i] = false;
	L8_MagicGlassUsed = false;
	L8_S1_FloorCracked = false;
	L8_S1_InvIdolTaken = false;
	L8_S1_InvMagicWandTaken = false;
	L8_S1_InvHalfKeyTaken = false;
	L8_S1_NoteTaken = false;
	L8_S1_DoorKeyPlaced = false;
	L8_S1_HandPlaced = false;
	L8_S1_PZDoorOpened = false;
	L8_S1_DoorKeyTaken = false;
	L8_S1_MagicWandWithoutKeyTaken = false;
	for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
		L8_S1_MatchingSymbolFound[i] = false;
	L8_S1_MatchingHOComplete = false;

	L8_S1_HandPuzzleSolved = false;
	L8_S1_InvMagicGlassTaken = false;
	L8_S5_IsPuzzleDataSaved = false;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			L8_S5_RingPiece_Visibility[i][j] = false;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			L8_S5_RingPiece_CurrentRow[i][j] = -1;
		}
	}

	l8_s1_DoorHOCompleted = false;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 8; j++)
			l8_s1_DoorHOFoundObjects[i][j] = false;
	

	L8_S2_Visited						= false;		
	L8_S2_SmallDoorKeyPlaced			= false;
	L8_S2_SmallDoorOpened				= false;
	L8_S2_InvToolTaken					= false;
	L8_S2_InvScrollTaken				= false;
	L8_S2_InvHalfKeyTaken				= false;
	L8_S2_GateKeyPlaced					= false;
	L8_S2_GatePuzzleSolved				= false;
	L8_S2_GateOpened					= false;
	L8_S2_GatePuzzleStarted				= false;
	L8_S2_GearPuzzleState				= 0;
	for (int i = 0; i < 10; i++)
	{
		L8_S2_GateSymbolArray[i]		= -1;
	}
	for (int i = 0; i < L8_S2_NUM_GEARS; i++)
	{
		L8_S2_GearSlotGearId[i]		= -1;
	}

	L8_S2_SugarCaneGrown				= false;
	L8_S2_InvSugarCaneTaken				= false;
	 L8_S2_SugercanePZState				= 0;
	 L8_S2_SugercanePZLevel				= 0;
	L8_S2_IsSugarCaneGivenToElephant	= false;
	L8_S2_IsClickedOnLotus				= false;
	L8_S2_InvSwordHandleTaken			= false;
	L8_S2_FloorCracked					= false;
	L8_S2_InvIdolTaken					= false;
	for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
		L8_S2_MatchingSymbolFound[i] = false;
	L8_S2_MatchingHOComplete = false;

	L8_S3_FloorCracked = false;
	L8_S3_InvIdolTaken = false;
	L8_S3_SwordHandlePlaced = false;
	L8_S3_SwordPuzzleSolved = false;
	for(int i = 0; i< L8S3_SWORDPIECES; i++)
		L8_S3_SwordPiecePlaced[i] = false;
	L8_S3_InvSwordTaken = false;
	L8_S3_LeafTextShown = false;
	for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
		L8_S3_MatchingSymbolFound[i] = false;
	L8_S3_MatchingHOComplete = false;

	L8_S3_LockKeyPlaced = false;
	for (int i = 0; i < MAX_L8S3_STRIPS; i++)
		L8_S3_SymboOnStrip[i] = 1 + rand() % 5;
	L8_S3_PZDoorOpened = false;
	L8_S3_StripPuzzleSolved = false;
	L8_S3_InvCrystalKeyTaken = false;
	l8_s3_invMaryLeafTaken = false;
	l8_s3_MaryLeafHOCompleted = false;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 4; j++)
			l8_s3_LeafHOFoundObjects[i][j] = false;

	L8_S4_BronzeIdolPlaced				= false;
	L8_S4_SilverIdolPlaced				= false;
	L8_S4_GoldIdolPlaced				= false;
	L8_S4_IsLeafPlacedInContainer		= false;
	L8_S4_IsMinigameAnimPlayed			= false;
	L8_S4_InvContainerWithLeafTaken		= false;
	L8_S4_ContainerWithLeafOpened		= false;
	L8_S4_IsContainerClosed				= false;
	L8_S4_IsPuzzleDataSaved				= false;
	L8_S4_IsPoolPuzzleSolved			= false;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 4; k++)
				L8_S4_FlowersOnBoardIndex[i][j][k] = -1;

	L8_S5_IntroVideoShown = false;
	L8_S5_Visited = false;
	L8_S5_EndVideoPlayed = false;

	L8_S5_VirtualMiniGameCompleted = false;
	L8_S5_VirtualMiniGameState = 0;
	for (int i = 0; i < 3; i++)
	{
		L8_S5_VirtualMiniGameShapesComplete[i] = false;
	}
	for (int i = 0; i < 9; i++)
	{
		L8_S5_VirtualMiniGameHOState[i] = false;
	}

	IsMainChapterCompleted = false;
	IsBonusChapterUnlocked = false;
	IsBonusChainRemoved = false;

	IsKidsRoomFromMainMenu = false;
	IsKidsRoomFromExtras = false;
	IsExtrasRoomFromMainMenu = false;
	IsAwardsRoomFromExtrasScene = false;
	IsCreditsFromMainMenu = false;

	CreditsPrevArea = -1;
	ExtrasRoomPrevArea = -1;
	MemoryRoomPrevArea = -1;
}

void Profile::setForExtras()
{
	Clear();

	// set default values here to activate the Puzzle when entering the Puzzle

	//Some Common Elements
	TutorialState = -1; //disable tutorial
	IsHaveYouCutscenePlayed = true;
	IsStartCutscenePlayed = true;

	/*---------LEVEL 1-----------*/
	//L1_FusePuzzle
	L1S2_IsP1GreenWireDownFixed = true;
	L1S2_IsP1GreenWireTopFixed = true;
	L1S2_IsP1LeftFuseFixed = true;
	L1S2_IsP1RightFuseFixed = true;
	L1S2_IsP1FusePuzzleActive = true;
	L1S3_IsP1LeftFuseTaken = true;
	L1S2_IsP1RightFuseTaken = true;

	//L1_S4_PZDoor Shadow Puzzle
	L1S4_P2IsShadowPuzHandDoorOpened = true;
	L1S4_P2IsMissingHandsPlaced = true;
	for (int i = 0; i < 5; i++)
	{
		L1S4_P2IsImagesFound[i] = true;
	}

	//L1_S3_DoorHO
	L1S3_IsDoorHOListPanelTaken = true;
	L1S3_IsDoorHOLocketPlaced = true;

	//L1_S6_HOBoatShed
	L1S6_HOPaperCutScenePlayed = true;

	/*---------LEVEL 2-----------*/
	//L2_S2_MangoPuzzle
	L2_S2_PZStonePlaced = true;
	//L2_S6_PZTunnelClock
	L2_S2_CutSceneVideoPlayed = true; //To avoid objective adding
	L2_S6_PaperPuzCluePlaced = true;
	L2_S6_NagaPearlFixed = true;


	/*---------LEVEL 3-----------*/
	//L3_BlockPuzzle
	L3_S3_PZBlockState = 4;

	//L3_S3_PZUnderwater
	L3_S3_PZBlocksPlaced = true;

	/*---------LEVEL 4-----------*/

	L4_S2_AlligatorPuzzleState = 1;
	L4_S2_FanPuzzleState = 1;



	/*---------LEVEL 5-----------*/
	//L5_S5_PZSunClock
	L5_S5_PZSolutionPaperPlaced = true;
	L5_S3_PZState = 64;
	L5_S4_PZHandCuffState = 120;
	//L5_S6_HOTree
	L5_S6_HOSoldierBadgePlaced = true;
	L5_S6_HOGuardianIntroTalkCompleted = true;

	/*---------LEVEL 6-----------*/

	L6_S1_SceneHOSolved[0] = true;
	L6_S4_PuzzlePiecesPlaced = true;

	/*---------LEVEL 7-----------*/
	//L7_S2_HOLeftHand & L7_S2_HORightHand
	L7_S2_HOLeftHandDiskPlaced = true;
	L7_S2_HORightHandDiskPlaced = true;
	L7_S2_PZLasyaStatuePlaced = true;

	L7_S3_PZDoorState = 6;
}
