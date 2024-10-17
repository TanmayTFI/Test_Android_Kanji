# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_VERBOSE_MAKEFILE on)

add_definitions(-D__KANJI_ANDROID__)
include_directories("../../../../res/headers/android")
include_directories("../../../../res/headers/android/GameHeaders")
include_directories("gameCpp/Levels/includes")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ANDROID_ASM_COMPILER "${ANDROID_LLVM_TOOLCHAIN_PREFIX}clang${ANDROID_TOOLCHAIN_SUFFIX}")
set(CMAKE_ASM_COMPILER_TARGET ${ANDROID_LLVM_TRIPLE})
set(CMAKE_ASM_COMPILER_EXTERNAL_TOOLCHAIN "${ANDROID_TOOLCHAIN_ROOT}")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")


if( ANDROID_ABI STREQUAL "armeabi-v7a")
    set(can_use_assembler TRUE)
    set(ASM_EXT S)
    set (CMAKE_ASM_FLAGS "${ANDROID_COMPILER_FLAGS} ${CMAKE_ASM_FLAGS} -march=armv7-a -mfpu=neon -mcpu=cortex-a8")
    enable_language(ASM)
endif()
if( ANDROID_ABI STREQUAL "arm64-v8a")
    set(can_use_assembler TRUE)
    set(ASM_EXT S)
    set (CMAKE_ASM_FLAGS "${ANDROID_COMPILER_FLAGS} ${CMAKE_ASM_FLAGS} -march=armv8-a")
    enable_language(ASM)
endif()

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("test_android_kanji")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#
# In this top level CMakeLists.txt, ${CMAKE_PROJECT_NAME} is used to define
# the target library name; in the sub-module's CMakeLists.txt, ${PROJECT_NAME}
# is preferred for the same purpose.
#
# In order to load a library into your app from Java/Kotlin, you must call
# System.loadLibrary() and pass the name of the library defined here;
# for GameActivity/NativeActivity derived applications, the same library name must be
# used in the AndroidManifest.xml file.
add_library(kanjigamelib  SHARED

        #GameSources
        gameCpp/Main.cpp gameCpp/AchieveRoom.cpp gameCpp/AlphaControl.cpp gameCpp/Animation.cpp  gameCpp/Application.cpp gameCpp/AudioData.cpp gameCpp/AudioSample.cpp gameCpp/AudioSlave.cpp gameCpp/Award.cpp gameCpp/AwardManager.cpp gameCpp/AwardsRoom.cpp gameCpp/Background.cpp gameCpp/Banners.cpp gameCpp/Bezier2D.cpp gameCpp/BitmapText.cpp gameCpp/Camera.cpp gameCpp/Circle.cpp gameCpp/ClickablePolygon.cpp gameCpp/ClickableRectangle.cpp gameCpp/ComboInventory.cpp
        gameCpp/Control.cpp gameCpp/Credits.cpp gameCpp/Cursor.cpp gameCpp/CustomAnimator.cpp gameCpp/CustomAnimGroup.cpp gameCpp/CVideo.cpp gameCpp/Data.cpp gameCpp/Debugger.cpp gameCpp/DialogSystem.cpp gameCpp/DistMesh.cpp gameCpp/EndCutscene.cpp gameCpp/exMath.cpp
        gameCpp/exRandom.cpp gameCpp/ExtrasComicControl.cpp gameCpp/ExtrasCutscenePlayer.cpp gameCpp/ExtrasMenuControl.cpp gameCpp/FlyingInsects.cpp gameCpp/Fonts.cpp gameCpp/FoundInvPopup.cpp gameCpp/GameFlow.cpp gameCpp/GameTextManager.cpp gameCpp/Global.cpp
        gameCpp/HaveYouCutScene.cpp gameCpp/HiddenObjectSystem.cpp gameCpp/HOInventoryBox.cpp gameCpp/HOObject.cpp gameCpp/HOScene.cpp gameCpp/Hud.cpp gameCpp/InfoPanel.cpp gameCpp/Input.cpp gameCpp/InteractiveObjectList.cpp gameCpp/InventoryItem.cpp gameCpp/IOStream.cpp gameCpp/Journal.cpp gameCpp/JSONAnimator.cpp gameCpp/KidsRoom.cpp
        gameCpp/MainMenuControl.cpp gameCpp/Map.cpp gameCpp/Match3.cpp gameCpp/MenuSystem.cpp gameCpp/MeshEffect.cpp gameCpp/MeshObject.cpp gameCpp/mp4_iPad.cpp gameCpp/MultilineText.cpp gameCpp/Object.cpp gameCpp/Particle.cpp gameCpp/ParticleEditor.cpp gameCpp/ParticleEffect.cpp gameCpp/Point.cpp gameCpp/Polygon.cpp gameCpp/Popup.cpp
        gameCpp/Profile.cpp gameCpp/ProfileSystem.cpp gameCpp/Rect.cpp gameCpp/RenderList.cpp gameCpp/Resources.cpp gameCpp/SaveData.cpp gameCpp/Scene2D.cpp gameCpp/SexyColor.cpp gameCpp/SParticle.cpp gameCpp/Splash.cpp gameCpp/Sprite.cpp gameCpp/StartCutscene.cpp gameCpp/SteamManager.cpp gameCpp/StrategyGuide.cpp gameCpp/StringTable.cpp gameCpp/MultilineTextBox.cpp gameCpp/AssetsStringTable.cpp
        gameCpp/InventoryBox.cpp gameCpp/SuperFX.cpp gameCpp/SuperLog.cpp gameCpp/Teleport.cpp gameCpp/TextBox.cpp gameCpp/Texture.cpp gameCpp/TextureFactory.cpp gameCpp/Timer.cpp gameCpp/PromoScene.cpp gameCpp/TTMath.cpp gameCpp/Tutorial.cpp gameCpp/UIButton.cpp gameCpp/UnitAnimator.cpp gameCpp/UpdateList.cpp

        #Levels
        gameCpp/Levels/EndOfDemoScene.cpp gameCpp/Levels/Flowers.cpp gameCpp/Levels/GridPuzzle.cpp  gameCpp/Levels/UnblockPuzzle.cpp
        gameCpp/Levels/L1_S1_Beach.cpp gameCpp/Levels/L1_S1_PZKidsPainting.cpp gameCpp/Levels/L1_S2_Frontyard.cpp gameCpp/Levels/L1_S2_PZFuse.cpp gameCpp/Levels/L1_S3_DoorHO.cpp gameCpp/Levels/L1_S3_Foyer.cpp gameCpp/Levels/L1_S3_PZShelf.cpp gameCpp/Levels/L1_S4_PZDoor.cpp gameCpp/Levels/L1_S4_Bedroom.cpp gameCpp/Levels/L1_S4_PZPainting.cpp gameCpp/Levels/L1_S5_HOTreasureRoom.cpp gameCpp/Levels/L1_S5_PZDynamiteMaking.cpp
        gameCpp/Levels/L1_S5_TreasureRoom.cpp gameCpp/Levels/L1_S5_TunnelDoor.cpp gameCpp/Levels/L1_S6_BoatShed.cpp gameCpp/Levels/L1_S6_HOBoatShed.cpp gameCpp/Levels/L1_S7_Boatdock.cpp gameCpp/Levels/L2S1_Pop1HoneyComb.cpp gameCpp/Levels/L2S1_Pop2UndergroundDoor.cpp gameCpp/Levels/L2S1_Pop3AloeveraPlant.cpp gameCpp/Levels/L2S1_Pop4ClearWater.cpp gameCpp/Levels/L2S1_Pop5Scrapper.cpp
        gameCpp/Levels/L2S2_Pop1NagaHead.cpp gameCpp/Levels/L2S2_Pop3Koolam.cpp gameCpp/Levels/L2S2_Pop4Pearls.cpp gameCpp/Levels/L2S3_Pop1Turmeric.cpp gameCpp/Levels/L2S4_Pop1WoodCut.cpp gameCpp/Levels/L2S4_Pop2FirePop.cpp gameCpp/Levels/L2S4_Pop3OilStoneLamp.cpp gameCpp/Levels/L2S4_Pop4StoneBrick.cpp gameCpp/Levels/L2S4_Pop5Rake.cpp gameCpp/Levels/L2S5_Pop1Vinegar.cpp
        gameCpp/Levels/L2S2_Pop2Frankinscence.cpp gameCpp/Levels/L2S5_Pop3Ember.cpp gameCpp/Levels/L2S5_Pop5Basket.cpp gameCpp/Levels/L2S6_Pop1StonesNKnife.cpp gameCpp/Levels/L2S6_Pop2CreeksSkeleton.cpp gameCpp/Levels/L2S6_Pop3Locket.cpp gameCpp/Levels/L2S6_Pop4Holder.cpp gameCpp/Levels/L2S6_Pop5StoneFace.cpp gameCpp/Levels/L2S6_Pop6OilLamp.cpp gameCpp/Levels/L2S5_Pop7Moopan.cpp gameCpp/Levels/L2S5_Pop6Chart.cpp
        gameCpp/Levels/L2_S1_ForestEntrance.cpp gameCpp/Levels/L2_S1_UndergroundHO.cpp gameCpp/Levels/L2_S2_NagaTemple.cpp gameCpp/Levels/L2_S2_NagaTempleHO.cpp gameCpp/Levels/L2_S2_PZMango.cpp gameCpp/Levels/L2_S3_CaveMouth.cpp gameCpp/Levels/L2_S3_PZStoneBeam.cpp gameCpp/Levels/L2_S4_Cave.cpp gameCpp/Levels/L2_S4_DeitiesPuzzle.cpp gameCpp/Levels/L2_S5_EagleBlockPuzzle.cpp gameCpp/Levels/L2_S5_MoopansDen.cpp
        gameCpp/Levels/L2_S5_PZMedicineMaking.cpp gameCpp/Levels/L2_S6_HOWaterfall.cpp gameCpp/Levels/L2_S6_PZTunnelClock.cpp gameCpp/Levels/L2_S6_TunnelEnd.cpp gameCpp/Levels/L2_S7_SoldiersPuzzle.cpp gameCpp/Levels/L3S2_Pop3Coconut.cpp gameCpp/Levels/L3S2_Pop4Crow.cpp gameCpp/Levels/L3S2_Pop5BackDoor.cpp gameCpp/Levels/L3S2_Pop6GunPowder.cpp gameCpp/Levels/L3S2_Pop7MazeDoor.cpp gameCpp/Levels/L3S3_Pop1Saw.cpp
        gameCpp/Levels/L3S3_Pop2Showel.cpp gameCpp/Levels/L3S3_Pop3Pool.cpp gameCpp/Levels/L3S3_Pop4Lamp.cpp gameCpp/Levels/L3S3_Pop5Door.cpp gameCpp/Levels/L3S4_Pop1BambooDoor.cpp gameCpp/Levels/L3S4_Pop2MapWallFour.cpp gameCpp/Levels/L3S4_Pop2MapWallOne.cpp gameCpp/Levels/L3S4_Pop2MapWallThree.cpp gameCpp/Levels/L3S4_Pop2MapWallTwo.cpp gameCpp/Levels/L3S4_Pop3CliffDoor.cpp
        gameCpp/Levels/L3S4_Pop4Metalsheet.cpp gameCpp/Levels/L3S5_Pop1Grass.cpp gameCpp/Levels/L3S5_Pop2Deer.cpp gameCpp/Levels/L3S5_Pop3Rope.cpp gameCpp/Levels/L3S5_Pop4Clip.cpp gameCpp/Levels/L3S5_Pop5Bucket.cpp gameCpp/Levels/L3_S1_EmberPopup.cpp gameCpp/Levels/L3S2_Pop1Thread.cpp gameCpp/Levels/L3S2_Pop2Skull.cpp gameCpp/Levels/L3_S1_FlagPickPopup.cpp gameCpp/Levels/L3_S1_FortEntrance.cpp
        gameCpp/Levels/L3_S1_HOLadder.cpp gameCpp/Levels/L3_S1_PZLionHead.cpp gameCpp/Levels/L3_S1_SkullFacePopup.cpp gameCpp/Levels/L3_S2_FortInterior.cpp gameCpp/Levels/L3_S2_M1PipePopUp.cpp gameCpp/Levels/L3_S2_M2PipePopUp.cpp gameCpp/Levels/L3_S2_M4Pop1Match3.cpp gameCpp/Levels/L3_S2_M4Pop2Pipe.cpp gameCpp/Levels/L3_S2_MazeDeadEnd.cpp gameCpp/Levels/L3_S2_P1R2CreeperPuzzle.cpp
        gameCpp/Levels/L3_S2_P1R3HammerPopup.cpp gameCpp/Levels/L3_S2_P1R3LockOpen.cpp gameCpp/Levels/L3_S2_P1R3PipePopup.cpp gameCpp/Levels/L3_S2_P1Room1Start.cpp gameCpp/Levels/L3_S2_P1Room2Wine.cpp gameCpp/Levels/L3_S2_P1Room3Lock.cpp gameCpp/Levels/L3_S2_P1Room4Match3.cpp gameCpp/Levels/L3_S2_P1Room5Pipe.cpp gameCpp/Levels/L3_S2_P1Room6Pillar.cpp gameCpp/Levels/L3_S2_P1_MazePipePopup.cpp
        gameCpp/Levels/L3_S3_Pool.cpp gameCpp/Levels/L3_S3_PZBlock.cpp gameCpp/Levels/L3_S3_PZUnderwater.cpp gameCpp/Levels/L3_S4_FortFountain.cpp gameCpp/Levels/L3_S4_FortFountainHO.cpp gameCpp/Levels/L3_S5_Cliff.cpp gameCpp/Levels/L4S1_Pop1RightFoot.cpp gameCpp/Levels/L4S1_Pop2HealingObjects.cpp gameCpp/Levels/L4S1_Pop3Pocket.cpp gameCpp/Levels/L4S1_Pop4Crutch.cpp
        gameCpp/Levels/L4S1_Pop5TwigSquarePopup.cpp gameCpp/Levels/L4S1_PopLeafChewing.cpp gameCpp/Levels/L4S2_Pop1MagicFroot.cpp gameCpp/Levels/L4S2_Pop2Bamboo.cpp gameCpp/Levels/L4S2_Pop3TreeBottom.cpp gameCpp/Levels/L4S2_Pop4Grass.cpp gameCpp/Levels/L4S3_Pop1AntiquePot.cpp gameCpp/Levels/L4S3_PZPopChart.cpp gameCpp/Levels/L4S4_Pop1StoneMill.cpp gameCpp/Levels/L4S4_Pop3Skeleton.cpp
        gameCpp/Levels/L4S4_PopInstruction.cpp gameCpp/Levels/L4S5_Pop1Door.cpp gameCpp/Levels/L4S5_Pop2OliveTree.cpp gameCpp/Levels/L4S5_Pop3Skeleton.cpp gameCpp/Levels/L4S5_Pop4NetSquare.cpp gameCpp/Levels/L4S5_Pop5Bridge.cpp gameCpp/Levels/L4_S1_PZMushroom.cpp gameCpp/Levels/L4_S1_ShoeHO.cpp gameCpp/Levels/L4_S1_SwampEntrance.cpp gameCpp/Levels/L4_S2_AlligatorPuzzle.cpp
        gameCpp/Levels/L4_S2_FanPuzzle.cpp gameCpp/Levels/L4_S2_RiverSide.cpp gameCpp/Levels/L4_S3_Cave.cpp gameCpp/Levels/L4_S3_PZDoor.cpp gameCpp/Levels/L4_S3_PZFireMaking.cpp gameCpp/Levels/L4_S4_BinocularHO.cpp gameCpp/Levels/L4_S4_HillTop.cpp gameCpp/Levels/L4_S5_HillSide.cpp gameCpp/Levels/L4_S5_PZSwapTiles.cpp gameCpp/Levels/L5S1_Pop1Bow.cpp gameCpp/Levels/L5S1_Pop2Arrow.cpp
        gameCpp/Levels/L5S1_Pop3Tiger.cpp gameCpp/Levels/L5S2_Pop1Skelton.cpp gameCpp/Levels/L5S2_Pop2Log.cpp gameCpp/Levels/L5S2_Pop3Grease.cpp gameCpp/Levels/L5S3_Pop1BowVine.cpp gameCpp/Levels/L5S3_Pop3ToUnderWater.cpp gameCpp/Levels/L5S3_Popup2Stone.cpp gameCpp/Levels/L5S4_Pop1Bridge.cpp gameCpp/Levels/L5S4_Pop2Pond.cpp gameCpp/Levels/L5S4_Pop3HandCuff.cpp
        gameCpp/Levels/L5S5_Pop1BambooLadder.cpp gameCpp/Levels/L5S5_Pop2MoonGlass.cpp gameCpp/Levels/L5S5_Pop3TopExitDoor.cpp gameCpp/Levels/L5S5_Pop4Sack.cpp gameCpp/Levels/L5S6_Pop1Peacock.cpp gameCpp/Levels/L5S6_Pop2Moon.cpp gameCpp/Levels/L5_S1_TigerDistant.cpp gameCpp/Levels/L5_S2_PZBlocks.cpp gameCpp/Levels/L5_S2_TigerCloseup.cpp gameCpp/Levels/L5_S3_HOUnderwater.cpp
        gameCpp/Levels/L5_S3_Pond.cpp gameCpp/Levels/L5_S3_PZDungeonDoor.cpp gameCpp/Levels/L5_S4_Dungeon.cpp gameCpp/Levels/L5_S4_PZHandCuff.cpp gameCpp/Levels/L5_S5_HOGanapathi.cpp gameCpp/Levels/L5_S5_MountainBelow.cpp gameCpp/Levels/L5_S5_PZSunClock.cpp gameCpp/Levels/L5_S6_HOTree.cpp gameCpp/Levels/L5_S6_MountainTop.cpp gameCpp/Levels/L6S1_Pop1Water.cpp gameCpp/Levels/L6S1_Pop2StatueChest.cpp
        gameCpp/Levels/L6S1_Pop3StatueHand.cpp gameCpp/Levels/L6S1_Pop4StatueSandals.cpp gameCpp/Levels/L6S1_PopAnanthaNote.cpp gameCpp/Levels/L6S2_Pop1Ice.cpp gameCpp/Levels/L6S2_Pop2Door.cpp gameCpp/Levels/L6S2_PZPop1Chart.cpp gameCpp/Levels/L6S3_HOPop1Code.cpp gameCpp/Levels/L6S3_HOPop2Vase.cpp gameCpp/Levels/L6S3_Pop1Charcoal.cpp gameCpp/Levels/L6S3_Pop2Fire.cpp gameCpp/Levels/L6S4_Pop1PineTree.cpp
        gameCpp/Levels/L6S4_Pop2HotSpring.cpp gameCpp/Levels/L6S4_Pop3MagnetizingRocks.cpp gameCpp/Levels/L6S4_PopAmeehaNote.cpp gameCpp/Levels/L6S1_PopNaginNote.cpp gameCpp/Levels/L6S4_PopAnathaNote.cpp gameCpp/Levels/L6S4_PopSecretLetter.cpp gameCpp/Levels/L6S5_Pop1Sophia.cpp gameCpp/Levels/L6_S1_DescriptionHO.cpp gameCpp/Levels/L6_S1_FloatingIsland_1.cpp
        gameCpp/Levels/L6_S2_FloatingIsland_2.cpp gameCpp/Levels/L6_S2_PZPineGum.cpp gameCpp/Levels/L6_S2_SphereHO.cpp gameCpp/Levels/L6_S3_FakeJames.cpp gameCpp/Levels/L6_S3_GlowingHeartHO.cpp gameCpp/Levels/L6_S3_FloatingIsland_3.cpp gameCpp/Levels/L6_S3_PZPathway.cpp gameCpp/Levels/L6_S4_FloatingIsland_4.cpp gameCpp/Levels/L6_S4_PortalChest.cpp gameCpp/Levels/L6_S4_PortalPuzzle.cpp
        gameCpp/Levels/L6_S5_MarysDream.cpp gameCpp/Levels/L6_S5_TabMiniGame.cpp gameCpp/Levels/L7S1_Pop1Flower.cpp gameCpp/Levels/L7S1_Pop2Well.cpp gameCpp/Levels/L7S2_Pop1SphereGlobe.cpp gameCpp/Levels/L7S3_Pop1Bhasmam.cpp gameCpp/Levels/L7S3_Pop2Bull.cpp gameCpp/Levels/L7S3_Pop3Chain.cpp gameCpp/Levels/L7S3_Pop4Ameeha.cpp gameCpp/Levels/L7S4_Pop1Pooja.cpp gameCpp/Levels/L7S4_Pop2Flower.cpp
        gameCpp/Levels/L7_S1_PZJigsawYellow.cpp gameCpp/Levels/L7_S1_PZStarPuzzle.cpp gameCpp/Levels/L7_S1_Volcano.cpp gameCpp/Levels/L7_S2_HandHOBasic.cpp gameCpp/Levels/L7_S2_HOBrokenLeftHand.cpp gameCpp/Levels/L7_S2_HOBrokenRightHand.cpp gameCpp/Levels/L7_S2_HOLeftHand.cpp gameCpp/Levels/L7_S2_HORightHand.cpp gameCpp/Levels/L7_S2_LasyaPrison.cpp gameCpp/Levels/L7_S2_PZEyePuzzle.cpp
        gameCpp/Levels/L7_S2_PZLasyaStatue.cpp gameCpp/Levels/L7_S3_Haven.cpp gameCpp/Levels/L7_S3_PZDoorPuzzle.cpp gameCpp/Levels/L7_S3_PZJigsawRed.cpp gameCpp/Levels/L7_S4_Ameeha.cpp gameCpp/Levels/L7_S4_PZJigsawBlue.cpp gameCpp/Levels/L7_S5_SophiaEnd.cpp
        gameCpp/Levels/L8S1_Pop1MagicWand.cpp gameCpp/Levels/L8S1_Pop2HiddenCompartment.cpp gameCpp/Levels/L8S1_Pop3DoorPopup.cpp gameCpp/Levels/L8S2_Pop4Lotus.cpp gameCpp/Levels/L8S2_Pop6HiddenCompartment.cpp gameCpp/Levels/L8S3_Pop1CrystalKey.cpp gameCpp/Levels/L8S3_Pop1SwordPopup.cpp gameCpp/Levels/L8S3_Pop2HiddenCompartment.cpp gameCpp/Levels/L8S1_PopRisingPheonix.cpp
        gameCpp/Levels/L8S4_Pop1BronzeIdol.cpp gameCpp/Levels/L8S4_Pop2SilverIdol.cpp gameCpp/Levels/L8S4_Pop3GoldIdol.cpp gameCpp/Levels/L8S4_Pop4LeafContainer.cpp gameCpp/Levels/L8_S1_HODoor.cpp gameCpp/Levels/L8_S1_PZDoor.cpp gameCpp/Levels/L8_S1_ShukrasPlace.cpp gameCpp/Levels/L8_S2_Garden.cpp gameCpp/Levels/L8_S2_GearPuzzle.cpp gameCpp/Levels/L8_S2_PZBubble.cpp
        gameCpp/Levels/L8_S2_PZGate.cpp gameCpp/Levels/L8_S2_PZSugarCane.cpp gameCpp/Levels/L8_S3_BanyanTree.cpp gameCpp/Levels/L8_S3_BanyanTreeHO.cpp gameCpp/Levels/L8_S4_Pool.cpp gameCpp/Levels/L8_S4_PZPoolGame.cpp gameCpp/Levels/L8_S5_VirtualMiniGame.cpp gameCpp/Levels/L8_S5_VolcanoLand.cpp

        # List C/C++ source files with relative paths to this CMakeLists.txt.
        native-lib.cpp
        # Wrapper file
        gameCpp/LibWrapper.cpp
)



# Searches for a specified prebuilt library and stores the path as a
# variable. Because system libraries are included in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )

find_library( android-lib android )

find_library( GLESv2-lib GLESv2 )

if(ANDROID_ABI STREQUAL "armeabi")
    target_link_libraries(kanjigamelib ${CMAKE_SOURCE_DIR}/../../../../res/lib/android/libkanjigles2-armeabi.a)
    target_link_libraries(kanjigamelib ${CMAKE_SOURCE_DIR}/../../../../res/lib/android/libkanjigame-armeabi.a)
endif()

if(ANDROID_ABI STREQUAL "armeabi-v7a")
    target_link_libraries(kanjigamelib ${CMAKE_SOURCE_DIR}/../../../../res/lib/android/libkanjigles2-armeabi-v7a.a)
    target_link_libraries(kanjigamelib ${CMAKE_SOURCE_DIR}/../../../../res/lib/android/libkanjigame-armeabi-v7a.a)
    target_link_libraries(kanjigamelib ${CMAKE_SOURCE_DIR}/../../../../res/lib/android/libcurl-armeabi-v7a.a)
endif()

if(ANDROID_ABI STREQUAL "arm64-v8a")
    target_link_libraries(kanjigamelib ${CMAKE_SOURCE_DIR}/../../../../res/lib/android/libkanjigles2-arm64-v8a.a)
    target_link_libraries(kanjigamelib ${CMAKE_SOURCE_DIR}/../../../../res/lib/android/libkanjigame-arm64-v8a.a)
    target_link_libraries(kanjigamelib ${CMAKE_SOURCE_DIR}/../../../../res/lib/android/libcurl-arm64-v8a.a)
endif()

if(ANDROID_ABI STREQUAL "x86")
    target_link_libraries(kanjigamelib ${CMAKE_SOURCE_DIR}/../../../../res/lib/android/libkanjigles2-x86.a)
    target_link_libraries(kanjigamelib ${CMAKE_SOURCE_DIR}/../../../../res/lib/android/libkanjigame-x86.a)
    target_link_libraries(kanjigamelib ${CMAKE_SOURCE_DIR}/../../../../res/lib/android/libcurl-x86.a)
endif()

if(ANDROID_ABI STREQUAL "x86_64")
    target_link_libraries(kanjigamelib ${CMAKE_SOURCE_DIR}/../../../../res/lib/android/libkanjigles2-x86_64.a)
    target_link_libraries(kanjigamelib ${CMAKE_SOURCE_DIR}/../../../../res/lib/android/libkanjigame-x86_64.a)
    target_link_libraries(kanjigamelib ${CMAKE_SOURCE_DIR}/../../../../res/lib/android/libcurl-x86_64.a)
endif()



# Specifies libraries CMake should link to your target library. You
# can link libraries from various origins, such as libraries defined in this
# build script, prebuilt third-party libraries, or Android system libraries.
target_link_libraries(kanjigamelib
        # List libraries link to the target library
        z

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        ${android-lib}
        ${GLESv2-lib}
)