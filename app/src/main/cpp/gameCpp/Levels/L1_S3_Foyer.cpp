//====================================
//	DQFYLH
//	HO System
//	Level 1 - Scene 03 : Frontdoor
//====================================

#include "L1_S3_Foyer.h"
#include "Hud.h"
#include "Banners.h"
#include "HOScene.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "TextureFactory.h"
#include "Rect.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "AwardManager.h"

const CPoint	CenterPoint(683,334,0);
const CPoint	PopCloseRectBtnPoint(998,98,0);

const CRectangle  PopCloseButtonRect(968.0f,71.0f,62.0f,61.0f);
const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);

//Scene Rects
const CRectangle  SceneRectBedroom(978.0f,126.0f,138.0f,167.0f);
const CRectangle  SceneRectShelfPuzzle(577.0f,315.0f,106.0f,69.0f);
const CRectangle  SceneRectDoorHO(1016.0f,168.0f,85.0f,118.0f);

//Popup Rects

const CRectangle  PopRectTableTop(578.0f,490.0f,225.0f,142.0f);
const CRectangle  PopRectShelf(601.0f,429.0f,112.0f,43.0f);
const CRectangle  PopRectPoto(819.0f,92.0f,141.0f,150.0f);

//Inventory Destination Rects
const CRectangle  MemoryRibbonRect(613.0f,411.0f,76.0f,52.0f);

const CRectangle  InfoRectCreekFamily(713.0f,269.0f,203.0f,154.0f);
const CRectangle  InfoRectMotherKid(462.0f,271.0f,182.0f,133.0f);
const CRectangle  InfoRectCarPic(495.0f,93.0f,243.0f,113.0f);

const CRectangle  InfoRectLabelOnWall(384.0f,490.0f,560.0f,62.0f);

const CRectangle  InfoRectHandAbove(825.0f,89.0f,187.0f,148.0f);
const CRectangle  InvDestRectBranchHand(619.0f,87.0f,388.0f,440.0f);
const CRectangle  InfoRectStair(516.0f,142.0f,261.0f,145.0f);
const CRectangle  InfoRectGargoyle1(742.0f,302.0f,65.0f,67.0f);
const CRectangle  InfoRectGargoyle2(1099.0f,223.0f,100.0f,117.0f);
const CRectangle  InfoRectFirePlace(456.0f,376.0f,117.0f,64.0f);


//Inventory Item Rects
const CRectangle  InvRectBanana(582.0f,259.0f,166.0f,82.0f);
const CRectangle  InvRectMatchBox(671.0f,245.0f,70.0f,76.0f);
const CRectangle  InvItemRectShadowHand(769.0f,208.0f,97.0f,61.0f);

const CRectangle  InvItemRectNagaKey(553.0f,222.0f,206.0f,137.0f);

//Info Rects - Just for information showing
const CRectangle  RajaTalkBoundsRect(156.0f,13.0f,1065.0f,450.0f);

const CRectangle  ActionRectRajaTalk(550.0f,380.0f,180.0f,150.0f);

const CRectangle  PaptusFlyRect(439.0f,388.0f,58.0f,67.0f);

//Video Rects

float lightAlphaArray5[20] = {0,0,0,0,0,0.15f,0.65f,0.75f,0.65f,0.25f,0.15f,0.75f,0.65f,0.05f,0.25f,0.55f,0.15f,0,0,0};

L1_S3_Foyer::L1_S3_Foyer()
{
    Visible = false;
    ActivePopupID = ePopInactive;

    RT_BgScn = NULL;
    IsPopupTransitionActive = false;

    bgScaleOffsetX = 0.0f;
    bgScaleOffsetY = 0.0f;
	vidTimer = 0;

    zOrderScnVideo = 1;
    zOrderPopVideo = eZOrder_Popup + 10;

    isRajaTakeJamesPlaying = false;

    //Main Scene
    LoadHud(INVENTORY_V1);

    MoonLightVFX = NULL;
    LightVFX = NULL;

    CAnim_RajaTakeJames = NULL;

    flyingInsects = new FlyingInsects();
    flyingInsects->InitFlies(571,26,686,125);

    cv_Cutscn = NULL;
    if( ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved )
    {
        LoadScene("LV1\\S3\\S3LITE.SCN");

        HOScene->GetObjectByID("Moon_Ray_1")->SetBlendMode(eBlend_AlphaAdditive);
        HOScene->GetObjectByID("Moon_Ray_1")->Animate(0.15f, 0.75f, 5.0f);

        HOScene->GetObjectByID("Creeckarea")->SetBlendMode(eBlend_AlphaAdditive);
        HOScene->GetObjectByID("Creeckarea")->Animate(0.15f, 0.75f, 7.0f);

        HOScene->GetObjectByID("Light")->SetBlendMode(eBlend_AlphaAdditive);
        HOScene->GetObjectByID("Light")->Animate(0.15f, 0.75f, 10.0f);

        HOScene->GetObjectByID("ShelfArea")->SetBlendMode(eBlend_AlphaAdditive);
        HOScene->GetObjectByID("ShelfArea")->Animate(0.15f, 0.75f, 5.0f);

        Scn_lightning = HOScene->GetObjectByID("Lighting_Foyer");
        Scn_lightning->SetBlendMode(eBlend_AlphaAdditive);
        Scn_lightning->Show(false);

        lightFrame = 0;
        lightningTimer = 0;
        nextLightningThreshold = 8.0f;

        HOScene->GetObjectByID("HangingLight_Light")->SetPivot(15,-70);
        HOScene->GetObjectByID("HangingLight_Light")->Animate(0,0,1,0,5.0f);

        HOScene->GetObjectByID("Leaves_1_Light")->SetPivot(50,30);
        HOScene->GetObjectByID("Leaves_1_Light")->Animate(0,0,4,0,10.0f);

        HOScene->GetObjectByID("Leaves_2_Light")->SetPivot(50,-25);
        HOScene->GetObjectByID("Leaves_2_Light")->Animate(0,0,4,0,10.0f);

        HOScene->GetObjectByID("Leaves_3_Light")->SetPivot(-12,28);
        HOScene->GetObjectByID("Leaves_3_Light")->Animate(0,0,3,0,10.0f);

        FgCurtainMesh = NULL;

        FgCurtainMesh = new MeshObject("LV1\\S3\\Curtain_LightBg_cut.jpa", 5, 5);
        FgCurtainMesh->SetPosAndParams(0, 0, 3, 2, 5, 2);
        FgCurtainMesh->SetZ(15);

        HOScene->InsertObject(FgCurtainMesh,5);

        MoonLightVFX = new SuperFX();
        MoonLightVFX->LoadEffectIni("PRT\\L1\\moonptr.INI");
        MoonLightVFX->SetPos(PointSystem::CreateCPoint(750.0f,140.0f,0));
        MoonLightVFX->SetZ(100);
        MoonLightVFX->Start();
        MoonLightVFX->AdvanceByTime(2.0f);
        VFXArray.push_back(MoonLightVFX);

        LightVFX = new SuperFX();
        LightVFX->LoadEffectIni("PRT\\L1\\lightprt.INI");
        LightVFX->SetPos(PointSystem::CreateCPoint(775.0f,280.0f,0));
        LightVFX->SetZ(100);
        LightVFX->Start();
        LightVFX->AdvanceByTime(2.0f);
        VFXArray.push_back(LightVFX);

        Scn_Bg = HOScene->GetObjectByID("Bg_Light");

        if(!ProfileSystem::Instance->ActiveProfile.L1S3_IsBedroomDoorOpened)
        {
            HOScene->GetObjectByID("Beddooropen")->SetEnable(false);
            HOScene->GetObjectByID("BedroomLight")->SetEnable(false);
        }
        else if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S4_Bedroom])
        {
            SuperFX *doorDustVFX;
            doorDustVFX = new SuperFX("PRT\\L1\\s3DoorDust.INI", PointSystem::CreateCPoint(1049,214,0), 100);
            doorDustVFX->Start();
            VFXArray.push_back(doorDustVFX);

            HOScene->GetObjectByID("BedroomLight")->Animate(0.7f, 0.9f, 5);
        }
        else
        {
            HOScene->GetObjectByID("BedroomLight")->SetEnable(false);
        }

        if(ProfileSystem::Instance->ActiveProfile.L1S3_RajaTakenJames)
        {
            HOScene->GetObjectByID("RajaLite")->SetEnable(false);
            HOScene->GetObjectByID("JamesLite")->SetEnable(false);

            HOScene->GetObjectByID("RajaJames1")->SetEnable(false);
            HOScene->GetObjectByID("RajaJames2")->SetEnable(false);
        }

        if(!ProfileSystem::Instance->ActiveProfile.L1S3_PZShelfPuzzleSolved)
        {
            HOScene->GetObjectByID("DoorPuzzleArea")->SetEnable(false);
            HOScene->GetObjectByID("DoorPuzzleArea_Final")->SetEnable(false);
            HOScene->GetObjectByID("DoorPuzzleArea_Flyre")->SetEnable(false);
            HOScene->GetObjectByID("DoorPuzzleArea_KeySet")->SetEnable(false);
            HOScene->GetObjectByID("DoorPuzzleArea_Statue")->SetEnable(false);
        }
        else
        {

            if(!ProfileSystem::Instance->ActiveProfile.L1S3_PZMoopanFlyerTaken)
            {
                HOScene->GetObjectByID("DoorPuzzleArea")->SetEnable(false);
                HOScene->GetObjectByID("DoorPuzzleArea_Final")->SetEnable(true);
                HOScene->GetObjectByID("DoorPuzzleArea_Flyre")->SetEnable(false);
                HOScene->GetObjectByID("DoorPuzzleArea_KeySet")->SetEnable(false);
                HOScene->GetObjectByID("DoorPuzzleArea_Statue")->SetEnable(false);
            }
            else if(!ProfileSystem::Instance->ActiveProfile.L1S3_IsInvHOLocketTaken)
            {
                HOScene->GetObjectByID("DoorPuzzleArea")->SetEnable(true);
                HOScene->GetObjectByID("DoorPuzzleArea_Final")->SetEnable(false);
                HOScene->GetObjectByID("DoorPuzzleArea_Flyre")->SetEnable(false);
                HOScene->GetObjectByID("DoorPuzzleArea_KeySet")->SetEnable(true);
                HOScene->GetObjectByID("DoorPuzzleArea_Statue")->SetEnable(true);
            }
            else
            {
                HOScene->GetObjectByID("DoorPuzzleArea")->SetEnable(true);
                HOScene->GetObjectByID("DoorPuzzleArea_Final")->SetEnable(false);
                HOScene->GetObjectByID("DoorPuzzleArea_Flyre")->SetEnable(false);
                HOScene->GetObjectByID("DoorPuzzleArea_KeySet")->SetEnable(false);

                if(ProfileSystem::Instance->ActiveProfile.L1S3_IsMemoryDollTaken)
                    HOScene->GetObjectByID("DoorPuzzleArea_Statue")->SetEnable(false);
            }
        }

        HOScene->GetObjectByID("RajaJames1")->SetEnable(false);
        HOScene->GetObjectByID("RajaJames2")->SetEnable(false);

        CObject* objArr[] = {HOScene->GetObjectByID("JamesLite"), HOScene->GetObjectByID("RajaLite"), HOScene->GetObjectByID("RajaJames1"), HOScene->GetObjectByID("RajaJames2")};
        std::string animArr[] = {"ANIM\\L1\\s3jameslite.anim", "ANIM\\L1\\s3rajalite.anim", "ANIM\\L1\\s3rajajames1.anim", "ANIM\\L1\\s3rajajames2.anim"};
        CAnim_RajaTakeJames = new CustomAnimGroup(4, animArr, objArr);

        //CRectangle RectVidFull(171, 96, 1024, 576);
        CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
        cv_Cutscn = new CVideo("OGV\\L1S3\\CATJUMPING.OGV", RectVidFull, false, eZOrder_CutsceneFull);
        //cv_Cutscn->SetScale(1.334f);
        cv_Cutscn->SetScale(GFApp->video_aspectRatio);
    }
    else
    {
        LoadScene("LV1\\S3\\S3DARK.SCN");

        HOScene->GetObjectByID("Moon_Ray_1")->SetBlendMode(eBlend_AlphaAdditive);
        HOScene->GetObjectByID("Moon_Ray_1")->Animate(0.15f, 0.75f, 5.0f);

        HOScene->GetObjectByID("HangingLight_Dark")->SetPivot(15,-70);
        HOScene->GetObjectByID("HangingLight_Dark")->Animate(0,0,1,0,5.0f);

        HOScene->GetObjectByID("Leaves_1_Dark")->SetPivot(50,30);
        HOScene->GetObjectByID("Leaves_1_Dark")->Animate(0,0,4,0,10.0f);

        HOScene->GetObjectByID("Leaves_2_Dark")->SetPivot(50,-25);
        HOScene->GetObjectByID("Leaves_2_Dark")->Animate(0,0,4,0,10.0f);

        HOScene->GetObjectByID("Leaves_3_Dark")->SetPivot(-12,28);
        HOScene->GetObjectByID("Leaves_3_Dark")->Animate(0,0,3,0,10.0f);

        FgCurtainMesh = NULL;

        FgCurtainMesh = new MeshObject("LV1\\S3\\Curtain_DarkBg_cut.jpa", 5, 5);
        FgCurtainMesh->SetPosAndParams(0, 0, 3, 2, 5, 2);
        FgCurtainMesh->SetZ(15);

        HOScene->InsertObject(FgCurtainMesh,5);

        MoonLightVFX = new SuperFX();
        MoonLightVFX->LoadEffectIni("PRT\\L1\\moonptr.INI");
        MoonLightVFX->SetPos(PointSystem::CreateCPoint(750.0f,140.0f,0));
        MoonLightVFX->SetZ(100);
        MoonLightVFX->Start();
        MoonLightVFX->AdvanceByTime(2.0f);
        VFXArray.push_back(MoonLightVFX);

        Scn_lightning = HOScene->GetObjectByID("Lighting_Foyer");
        Scn_lightning->SetBlendMode(eBlend_AlphaAdditive);
        Scn_lightning->Show(false);

        lightFrame = 0;
        lightningTimer = 0;
        nextLightningThreshold = 8.0f;

        Scn_Bg = HOScene->GetObjectByID("Bg_Dark");
    }



    HOLinesVFX = NULL;
    HODotsVFX = NULL;
    if(!ProfileSystem::Instance->ActiveProfile.L1S3_DoorHOSolved && ProfileSystem::Instance->ActiveProfile.L1S3_IsInvHOLocketTaken)
    {
        //HOLinesVFX = new SuperFX("PRT\\L1\\hoprtlines2.INI", PointSystem::CreateCPoint(1050,220,0), 15);
        HODotsVFX = new SuperFX("PRT\\L1\\hoprdots2.INI", PointSystem::CreateCPoint(1050,230,0), 15);
        //HOLinesVFX->Start();
        if(!ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles)
            HODotsVFX->Start();

        //VFXArray.push_back(HOLinesVFX);
        VFXArray.push_back(HODotsVFX);
    }

    InitPopups();


    dialogSystem = NULL;
    //if condition to add
    dialogSystem = new DialogSystem();
    AddControl(dialogSystem);

    isDialogActive = false;

    //MUSIC_SFX
    Control::Audio->ClearUnusedTracks();
    if(ProfileSystem::Instance->ActiveProfile.IsStartMusic)
    {
        ProfileSystem::Instance->ActiveProfile.IsStartMusic = false;
        if( ProfileSystem::Instance->ActiveProfile.L1S3_DoorHOSolved )
        {
            if( !Control::Audio->IsPlaying(aTrackL1Hunted) )
                Control::Audio->PlaySample(aTrackL1Hunted,false);
        }
        else  if( ProfileSystem::Instance->ActiveProfile.L1_IsMansionPowerOn )
        {
            if( !Control::Audio->IsPlaying(aTrackL1Mansion) )
                Control::Audio->PlaySample(aTrackL1Mansion,false);
        }
        else
        {
            if( !Control::Audio->IsPlaying(aTrackL1Mansion) &&
                !Control::Audio->IsPlaying(aTrackL1Broken) )
                Control::Audio->PlaySample(aTrackL1Broken,false);
        }
    }

    if( ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved )
    {
        Control::Audio->LoadSample(aAmbS3FoyerLit,AudioAmb);
        Control::Audio->PlaySample(aAmbS3FoyerLit,true);
    }
    else
    {
        Control::Audio->LoadSample(aAmbS3FoyerUnlit,AudioAmb);
        Control::Audio->PlaySample(aAmbS3FoyerUnlit,true);
    }

    Control::Audio->LoadSample(aVOL1MaryRefS3_1,AudioVO);
    //MUSIC_SFX

    GFInstance->PostUpdate(1);
    GFInstance->IsScnElmntsPreloaded[eArea_L1_S3_Foyer]  = true;

    if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S3_Foyer])
    {
        ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S3_Foyer] = true;
    }

#ifdef _CEBUILD
    //Morph Object Add. Only when light comes only
    if( ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved )
    {
        if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S3Collected)
        {
            Hud->setMorphObjects(HOScene->GetObjectByID("morpheagle"), HOScene->GetObjectByID("morphangel"));
            CRectangle  MorphRect(392.0f,305.0f,46.0f,52.0f);
            Hud->setMorphObjRect(MorphRect);
        }
        else
        {
            HOScene->GetObjectByID("morpheagle")->SetEnable(false);
        }
    }
#else
    if( ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved )
	{
		HOScene->GetObjectByID("morpheagle")->SetEnable(false);
	}
#endif

    //Scene Changes
    UpdateBgGraphic();

    Visible = true;

    TaskPendingUpdate();

}

void L1_S3_Foyer::TaskPendingUpdate()
{
    bool isAnyTaskLeft = false;
    if(ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby1Branch2Taken && ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby2Branch3Taken &&ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby3UnderLeaf1Taken && ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby4UnderLeavesTaken &&  !ProfileSystem::Instance->ActiveProfile.L1S3_IsBedroomDoorOpened && ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved)
        isAnyTaskLeft = true;
    if((!ProfileSystem::Instance->ActiveProfile.L1S3_MatchBoxTaken || !ProfileSystem::Instance->ActiveProfile.L1S3_NagaKeyTaken || (!ProfileSystem::Instance->ActiveProfile.L1S3_IsInvShadowHand1Taken && ProfileSystem::Instance->ActiveProfile.L1S3_IsP3ShadowHandFallen) || (!ProfileSystem::Instance->ActiveProfile.L1S3_IsP3ShadowHandFallen && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvTreeBranchTaken))&& ProfileSystem::Instance->ActiveProfile.L1S3_RajaTakenJames)
        isAnyTaskLeft = true;
    ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S3_Foyer] = isAnyTaskLeft;

    if(ProfileSystem::Instance->ActiveProfile.L1S3_IsBedroomDoorOpened )
    {
        if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerWaxMelted &&  ProfileSystem::Instance->ActiveProfile.L1S3_MatchBoxTaken)
            ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S4_Bedroom] = true;
        else if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsDrawerOpened && ProfileSystem::Instance->ActiveProfile.L1S3_NagaKeyTaken )
            ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S4_Bedroom] = true;
        else if(!ProfileSystem::Instance->ActiveProfile.L1S4_IsBeetleBookTakenFromShelf)
            ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S4_Bedroom] = true;
        else if(!ProfileSystem::Instance->ActiveProfile.L1S4_P2IsMissingHandsPlaced && ProfileSystem::Instance->ActiveProfile.L1S3_IsInvShadowHand1Taken )
            ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S4_Bedroom] = true;
    }

    if(ProfileSystem::Instance->ActiveProfile.L1S3_IsBananaTaken && !ProfileSystem::Instance->ActiveProfile.L1S2_IsMonkeyTookBanana)
        ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S2_Frontyard] = true;
}

void L1_S3_Foyer::UpdateBgGraphic()
{
    if(RT_BgScn == NULL)
    {
        RT_BgScn = new CObject();
        RT_BgScn->LoadSprite("UI\\ZOOMBG.SPR");
        RT_BgScn->SetZ(19);
        RT_BgScn->SetEnable(false);
        //	K_LOG("Zoombg.spr loaded");
    }


    // Make Inactive Not for Bg Scaling
    Scn_lightning->SetEnable(false);

    if(GFInstance->IsScnRTCreated)
    {
        //  GFInstance->CurrentSceneGraphic->clearRenderTarget();

        GFInstance->CurrentSceneGraphic->beginRenderToTarget();//RENDER TO TARGET STARTS
        GFInstance->IsRenderingToRT = true;
    }
    if(GFInstance->IsScnBg1024Res)
        HOScene->RenderToTarget(-171.0f);
    else
        HOScene->RenderToTarget();
//	K_LOG("Rendertotarget");
    if(GFInstance->IsScnRTCreated)
    {
        GFInstance->IsRenderingToRT = false;
        GFInstance->CurrentSceneGraphic->endRenderToTarget();
    }
    //Make Active
    Scn_lightning->SetEnable(true);

    RT_BgScn->pSprite->Image = GFInstance->CurrentSceneGraphic;
//	K_LOG("RT_BgScn->pSprite->Image = GFInstance->CurrentSceneGraphic");
}

void L1_S3_Foyer::InitPopups()
{
    clickDelayTimer = 0;

    popupCloseDelayTimer = 0;
    Pop_CloseButton = NULL;

    Popup1Shelf = NULL;
    if ( !ProfileSystem::Instance->ActiveProfile.L1S3_NagaKeyTaken )
    {
        IsCatAnimPlaying = false;
        IsCatScaling = false;
        Scalefactor = 0;

        catVelY = -50;

        Popup1Shelf = new CHOScene();
        Popup1Shelf->Load("LV1\\S3\\POP1\\POP1.SCN");

        //Add popup frame and close button
        CObject *popFrame = new CObject();
        popFrame->LoadInit("UI\\poprectframe.SPR", 13, -15, eZOrder_PopFrame);
        popFrame->SetUID("popFrame");
        Popup1Shelf->PushObject(popFrame);

        CObject *popCloseBtn = new CObject();
        popCloseBtn->LoadInit("UI\\closebtn.SPR", 321, -246, eZOrder_PopCloseBtn);
        popCloseBtn->SetUID("popClose");
        Popup1Shelf->PushObject(popCloseBtn);

        IsObj1MovePlaying = false;
        IsObj2MovePlaying = false;

        P1_MovableObj1 = Popup1Shelf->GetObjectByID("MovableObj1");
        P1_MovableObj2 = Popup1Shelf->GetObjectByID("MovableObj2");

        if ( ProfileSystem::Instance->ActiveProfile.L1S3_IsObj1Moved )
            P1_MovableObj1->SetEnable(false);

        if ( ProfileSystem::Instance->ActiveProfile.L1S3_IsObj2Moved )
        {
            P1_MovableObj2->SetEnable(false);
            //Cat is already gone
            Popup1Shelf->GetObjectByID("CatFirstFrame")->SetEnable(false);
        }

        Popup1Shelf->Show(false);
        Popup1Shelf->SetPosOff(CenterPoint.x,CenterPoint.y);

        Popup1Shelf->SetZOff(eZOrder_Popup);
    }
    Popup1_State = POPUP_CLOSED;

    Popup2Table = NULL;
    if ( !ProfileSystem::Instance->ActiveProfile.L1S3_MatchBoxTaken )
    {
        Popup2Table = new CHOScene();
        Popup2Table->Load("LV1\\S3\\POP2\\POP2.SCN");

        //Add popup frame and close button
        CObject *popCirFrame = new CObject();
        popCirFrame->LoadInit("UI\\popcircframe.SPR", 4, -9, 1);
        popCirFrame->SetUID("popFrame");
        Popup2Table->InsertObject(popCirFrame,1);

        CObject *popCirCloseBtn = new CObject();
        popCirCloseBtn->LoadInit("UI\\closebtn.SPR", 145, -158, 7);
        popCirCloseBtn->SetUID("popClose");
        Popup2Table->PushObject(popCirCloseBtn);

        if(ProfileSystem::Instance->ActiveProfile.L1S3_IsBananaTaken)
            Popup2Table->GetObjectByID("Banana")->SetEnable(false);

        Popup2Table->Show(false);
        Popup2Table->SetPosOff(CenterPoint.x,CenterPoint.y);
        Popup2Table->SetZOff(eZOrder_Popup);
    }
    Popup2_State = POPUP_CLOSED;


    isBranchPokeAnimPlaying = false;
    CAnim_BranchHandTake = NULL;

    Popup3Photos = NULL;
    {
        Popup3Photos = new CHOScene();
        Popup3Photos->Load("LV1\\S3\\POP3\\POP3.SCN");

        if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S3RibbonNeck] )
        {
            Popup3Photos->GetObjectByID("ribbon")->SetEnable(false);
        }

        if (ProfileSystem::Instance->ActiveProfile.L1S3_IsInvShadowHand1Taken )
        {
            Popup3Photos->GetObjectByID("hndfall")->SetEnable(false);
        }

        CPoint fallPos(136,-92,0);
        if (ProfileSystem::Instance->ActiveProfile.L1S3_IsP3ShadowHandFallen)
        {
            //Hand Fallen using branch
            Popup3Photos->GetObjectByID("hndfall")->SetPos(&fallPos);

            Popup3Photos->GetObjectByID("hndtopmask")->SetEnable(false);
            Popup3Photos->GetObjectByID("hndbackshadow")->SetEnable(false);
            Popup3Photos->GetObjectByID("hndbackshadow")->SetEnable(false);
        }
        else
        {
            Popup3Photos->GetObjectByID("hndfall")->SetScale(0.8f);
        }

        Popup3Photos->GetObjectByID("paptus")->SetBlendMode(eBlend_AlphaAdditive);
        if(ProfileSystem::Instance->ActiveProfile.IsPaptusL1S3Collected)
        {
            Popup3Photos->GetObjectByID("paptus")->SetEnable(false);
        }

        CObject* objArr[] = {Popup3Photos->GetObjectByID("branch"), Popup3Photos->GetObjectByID("hndfall"), Popup3Photos->GetObjectByID("hndbackshadow"), Popup3Photos->GetObjectByID("hndtopmask")};
        std::string animArr[] = {"ANIM\\L1\\s3handBranch.anim", "ANIM\\L1\\s3handHandAnim.anim", "ANIM\\L1\\s3handshadow.anim", "ANIM\\L1\\s3handTopMask.anim"};
        CAnim_BranchHandTake = new CustomAnimGroup(4, animArr, objArr);

        Popup3Photos->GetObjectByID("branch")->SetEnable(false);

        Popup3Photos->GetObjectByID("glow")->SetBlendMode(eBlend_AlphaAdditive);
        Popup3Photos->GetObjectByID("glow")->Animate(0.40f, 0.65f, 4.0f);

        //Add popup frame and close button
        CObject *popFrame = new CObject();
        popFrame->LoadInit("UI\\poprectframe.SPR", 13, -15, eZOrder_PopFrame);
        popFrame->SetUID("popFrame");
        Popup3Photos->PushObject(popFrame);

        CObject *popCloseBtn = new CObject();
        popCloseBtn->LoadInit("UI\\closebtn.SPR", 321, -246, eZOrder_PopCloseBtn);
        popCloseBtn->SetUID("popClose");
        Popup3Photos->PushObject(popCloseBtn);

        //Localized
        CObject* temp = new CObject();
        temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L1\\S3\\FORGIVE.SPR", 0, 180, eZOrder_PopFrame + 10);
        temp->SetUID("forgive");
        Popup3Photos->PushObject(temp);

        Popup3Photos->Show(false);
        Popup3Photos->SetPosOff(CenterPoint.x,CenterPoint.y);
        Popup3Photos->SetZOff(eZOrder_Popup);
    }
    Popup3_State = POPUP_CLOSED;

    PopupAlpha = 0.0f;
    PopVal = 0.0f;

    GFInstance->PostUpdate(1);

}

void L1_S3_Foyer::SparkleUpdate()
{
    if(isSparkleUpdatePending)
    {
        isSparkleUpdatePending = false;
        if(SparkleFX==NULL)
        {
            SparkleFX = new SuperFX("PRT\\glint.INI",PointSystem::CreateCPoint(0,0,0),120);
        }
        //Game Logic Goes here
        numSparkles = 0;
        currentSparkleID = 0;

        if(!ProfileSystem::Instance->ActiveProfile.L1S3_RajaTakenJames)
        {
            sparklePoints[numSparkles].SetXY(ActionRectRajaTalk.GetCenterX(), ActionRectRajaTalk.GetCenterY());
            numSparkles++;
        }
        else
        {
            if(!ProfileSystem::Instance->ActiveProfile.L1S3_NagaKeyTaken)
            {
                sparklePoints[numSparkles].SetXY(PopRectShelf.GetCenterX(), PopRectShelf.GetCenterY());
                numSparkles++;
            }

            if(!ProfileSystem::Instance->ActiveProfile.L1S3_MatchBoxTaken)
            {
                sparklePoints[numSparkles].SetXY(PopRectTableTop.GetCenterX(), PopRectTableTop.GetCenterY());
                numSparkles++;
            }

            if(!ProfileSystem::Instance->ActiveProfile.L1S3_IsInvShadowHand1Taken)
            {
                sparklePoints[numSparkles].SetXY(PopRectPoto.GetCenterX(), PopRectPoto.GetCenterY());
                numSparkles++;
            }

            if(!ProfileSystem::Instance->ActiveProfile.L1S3_IsInvHOLocketTaken)
            {
                sparklePoints[numSparkles].SetXY(SceneRectShelfPuzzle.GetCenterX(), SceneRectShelfPuzzle.GetCenterY());
                numSparkles++;
            }

        }

    }
    sparkleTimer += Control::LogicRate;
    if(sparkleTimer>sparkleThreshold)
    {
        sparkleTimer = 0;
        if(numSparkles>0)
        {
            SparkleFX->SetPos(&sparklePoints[currentSparkleID]);
            SparkleFX->Start();
            currentSparkleID += 1;
            if(currentSparkleID>=numSparkles)
                currentSparkleID = 0;
        }
    }
}

void L1_S3_Foyer::Required()
{
    HiddenObjectSystem::Required();	//forced update

    if( Hud->PopupState != Popup_Inactive )
        return;

    if( IsPopupTransitionActive )
        UpdatePopupTransition();
}

void L1_S3_Foyer::Update()
{
    if(GFInstance->IsSceneGraphicReloaded)
    {
        UpdateBgGraphic();
        GFInstance->IsSceneGraphicReloaded = false;
    }

    flyingInsects->Update();

    if(!ProfileSystem::Instance->ActiveProfile.L1S3_DoorHOSolved && ProfileSystem::Instance->ActiveProfile.L1S3_IsInvHOLocketTaken)
    {
        if(!ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles)
        {
            if(HODotsVFX!=NULL && !HODotsVFX->Active)
            {
                HODotsVFX->Start();
            }
        }
        else
        {
            if(HODotsVFX!=NULL && HODotsVFX->Active)
            {
                HODotsVFX->StopImmediate();
            }
        }
    }

    if(isDialogActive)
    {
        dialogSystem->Update();
        return;
    }

    if( Hud->PopupState != Popup_Inactive )
        return;

    if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
        return;

    CPoint pos = Control::Input->Mpos();
    if(GFHud->InvBoxCtrl->travelArea != eArea_L1_S2_Frontyard)
        NavigateToScene();

#ifdef _CEBUILD
    if(!Hud->isMouseOverExtraButton)
        Cursor::SetMode(CA_Normal);
#else
    Cursor::SetMode(CA_Normal);
#endif

    if( ActivePopupID == ePopInactive )
    {
        if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
            SparkleUpdate();


        lightningTimer += Control::LogicRate;
        if(lightningTimer>nextLightningThreshold)
        {
            Scn_lightning->Show(true);
            if(lightFrame==0){
                if(rand()%2>1)
                    Control::Audio->PlaySample(aSFXL1ThunderV1, false);
                else
                    Control::Audio->PlaySample(aSFXL1ThunderV2, false);
            }
            lightFrame += Control::LogicRate/2;
            int index = (int)(lightFrame*20.0f);
            if(lightFrame>=1.0f)
            {
                lightFrame = 0;
                index = 0;
                lightningTimer = 0;
                Scn_lightning->Show(false);
                nextLightningThreshold = 12.0f + (float)(rand()%8);
            }
            Scn_lightning->SetAlpha(lightAlphaArray5[index]);
        }

        if(isRajaTakeJamesPlaying)
        {
            if(CAnim_RajaTakeJames->IsAnimFinished())
            {
                HOScene->GetObjectByID("RajaLite")->SetEnable(false);
                HOScene->GetObjectByID("JamesLite")->SetEnable(false);
                HOScene->GetObjectByID("RajaJames1")->SetEnable(false);
                HOScene->GetObjectByID("RajaJames2")->SetEnable(false);
                isRajaTakeJamesPlaying = false;
                ProfileSystem::Instance->ActiveProfile.L1S3_RajaTakenJames = true;
                isSparkleUpdatePending = true;
                UpdateBgGraphic();

                //Objective Add
                if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L1O3_FindBoatKey] )
                {
                    ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L1O3_FindBoatKey] = true;
                    ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
                            = eObj_L1O3_FindBoatKey;
                    //Increment Objectives
                    ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
                    Hud->ShowObjectiveFX(eObj_L1O3_FindBoatKey);

                    Control::Audio->PlaySample(aSFXMagic1);
                }

                //Journal Entry 9 Raja Take James
                if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P9_RajaFoyerJamesTaken])
                {
                    //Flag Set
                    ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P9_RajaFoyerJamesTaken] = true;
                    //Add to Note Array
                    ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected]
                            = eJrn_L1P9_RajaFoyerJamesTaken;

                    ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;
                    //Increment Notes
                    ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
                    Hud->ShowJrnEntryFX();
                }
            }
            return;
        }

        if(ProfileSystem::Instance->ActiveProfile.L1S3_RajaTalkOver && !ProfileSystem::Instance->ActiveProfile.L1S3_RajaTakenJames)
        {
            Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3RajaTakeJames);

            CAnim_RajaTakeJames->PlayAnim();
            isRajaTakeJamesPlaying = true;

            //Show Objective completed
            if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L1O1_HelpJames] && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L1O1_HelpJames] )
            {
                ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L1O1_HelpJames] = true;
                Hud->ShowObjectiveFX(eObj_L1O1_HelpJames,true);
            }
            return;
        }

#ifdef ENABLE_ANDROID_BACK_BUTTON
        if (Control::Input->isKeyBackSensed())
        {
            Control::Audio->PlayFootStepsFX();
            ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S2_Frontyard;
            SendMessage(ControlExit, ID, HOSM_NewArea);
            return;
        }
#endif

        if( IsMouseOverGoBackArea(&pos ))
        {
#ifdef TOUCH_UI
         //   if(!GFHud->InvBoxCtrl->navTapped)
           //     Cursor::SetMode(CA_ExitDown,eArea_L1_S2_Frontyard);
#else
            Cursor::SetMode(CA_ExitDown,eArea_L1_S2_Frontyard);
#endif

            if( Control::Input->LBclicked() )
            {
#ifdef TOUCH_UI

                if(!GFHud->InvBoxCtrl->navTapped)
                {
                    GFHud->InvBoxCtrl->GoToScene(eArea_L1_S2_Frontyard, 2);
                }
                else
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

#else
                Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S2_Frontyard;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

            }
            return;
        }
        else
        {

        }

        if(  !ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved )
        {
            if(ActionRectRajaTalk.Intersection(&pos))
            {
                Cursor::SetMode(CA_Speech);
                if(Control::Input->LBclicked())
                {
                    dialogSystem->ShowDialog(eDialog_S3RajaFoyerDark);
                    isDialogActive = true;
                    Hud->AllowInput = false;
                    Hud->HideHud(true);
                }
            }
            return;
        }
        else if( !ProfileSystem::Instance->ActiveProfile.L1S3_RajaTakenJames)
        {
            if(ActionRectRajaTalk.Intersection(&pos))
            {
                if(!ProfileSystem::Instance->ActiveProfile.L1S3_RajaAskedForCreekLetter)
                {
                    Cursor::SetMode(CA_Speech);
                    if(Control::Input->LBclicked())
                    {
                        dialogSystem->ShowDialog(eDialog_S3RajaFoyerAskForLetter);
                        isDialogActive = true;
                        Hud->AllowInput = false;
                        Hud->HideHud(true);
                    }
                }
                else
                {
                    if(ActionRectRajaTalk.Intersection(&pos))
                    {
                        Cursor::SetMode(CA_Gear);
                        if(Control::Input->LBclicked())
                        {
                            dialogSystem->ShowDialog(eDialog_S3RajaFoyerShowLetter);
                            isDialogActive = true;
                            Hud->AllowInput = false;
                            Hud->HideHud(true);
                        }
                    }
                }
            }
            else
            {
                if(ProfileSystem::Instance->ActiveProfile.L1S3_RajaAskedForCreekLetter)
                {
                    if(ActionRectRajaTalk.Intersection(&pos))//Here
                    {
                        Cursor::SetMode(CA_Help);
                        if(Control::Input->LBclicked())
                        {

                            dialogSystem->ShowDialog(eDialog_S3RajaFoyerShowLetter);
                            isDialogActive = true;
                            Hud->AllowInput = false;
                            Hud->HideHud(true);

                        }
                    }
                }
                else if(!ProfileSystem::Instance->ActiveProfile.L1S3_RajaAskedForCreekLetter)
                {
                    if(Control::Input->LBclicked())
                    {
                        dialogSystem->ShowDialog(eDialog_S3RajaFoyerAskForLetter);
                        isDialogActive = true;
                        Hud->AllowInput = false;
                        Hud->HideHud(true);
                    }
                }
            }
            return;
        }
        else
        {
            if( InfoRectGargoyle1.Intersection(&pos) || InfoRectGargoyle2.Intersection(&pos))
            {
                if( Control::Input->LBclicked() )
                {
                    Hud->ShowBannerText("L1meant");
                }
            }

            if( InfoRectStair.Intersection(&pos) )
            {
                if( Control::Input->LBclicked() )
                {
                    Hud->ShowBannerText("L1really");
                }
            }
            if( InfoRectFirePlace.Intersection(&pos) )
            {
                if( Control::Input->LBclicked() )
                {
                    Hud->ShowBannerText("L1unused");
                }
            }
        }

        if( SceneRectShelfPuzzle.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L1S3_IsMemoryDollTaken)
        {
#ifdef TOUCH_UI
            //if(!GFHud->InvBoxCtrl->navTapped)
            //Cursor::SetMode(CA_Look);
#else
            Cursor::SetMode(CA_Look);
#endif

            if( Control::Input->LBclicked() )
            {
#ifdef TOUCH_UI
                if(!GFHud->InvBoxCtrl->navTapped)
                {
                    GFHud->InvBoxCtrl->GoToScene(eArea_L1_S3_PZShelf, 0, SceneRectShelfPuzzle);
                }
#else
                ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_PZShelf;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

            }
            return;
        }

        if( ProfileSystem::Instance->ActiveProfile.L1S3_IsBedroomDoorOpened )
        {
            if (SceneRectBedroom.Intersection(&pos))
            {
#ifdef TOUCH_UI
                //if(!GFHud->InvBoxCtrl->navTapped)
                //Cursor::SetMode(CA_ExitRight, eArea_L1_S4_Bedroom);
#else
                Cursor::SetMode(CA_ExitRight, eArea_L1_S4_Bedroom);
#endif

                if( Control::Input->LBclicked() )
                {
#ifdef TOUCH_UI

                    if(!GFHud->InvBoxCtrl->navTapped)
                    {
                        GFHud->InvBoxCtrl->GoToScene(eArea_L1_S4_Bedroom, 0, SceneRectBedroom);
                    }
#else
                    ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S4_Bedroom;
					SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

                }
                return;
            }
        }
        else
        {
            if (SceneRectBedroom.Intersection(&pos))
            {
#ifdef TOUCH_UI
                //if(!GFHud->InvBoxCtrl->navTapped)
                //Cursor::SetMode(CA_Look, eArea_L1_S3_DoorHO);
#else
                Cursor::SetMode(CA_Look, eArea_L1_S3_DoorHO);
#endif

                if( Control::Input->LBclicked() )
                {
#ifdef TOUCH_UI
                    if(!GFHud->InvBoxCtrl->navTapped)
                    {
                        GFHud->InvBoxCtrl->GoToScene(eArea_L1_S3_DoorHO, 0, SceneRectBedroom);
                    }
#else
                    ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_DoorHO;
					SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
                }
                return;
            }
        }
    }

    if( !IsPopupTransitionActive )
        UpdatePopups(pos);

}

void L1_S3_Foyer::UpdatePopups(CPoint pos,bool ItemDropped)
{
    if(!ProfileSystem::Instance->ActiveProfile.L1S3_RajaTakenJames)
        return;

    if ( (ActivePopupID==ePopInactive || ActivePopupID==ePopL1S3_P1Shelf) && ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved)
    {
        switch( Popup1_State )
        {
            case POPUP_CLOSED:
                if( PopRectShelf.Intersection(&pos) &&  !ProfileSystem::Instance->ActiveProfile.L1S3_NagaKeyTaken)
                {
                    Cursor::SetMode(CA_Look);
                    if( Control::Input->LBclicked() || ItemDropped )
                    {
                        Popup1_State = POPUP_SCALEUP;
                        ActivePopupID = ePopL1S3_P1Shelf;

                        Pop_CloseButton = Popup1Shelf->GetObjectByID("popClose");
                        Pop_CloseButton->PlayAnimation(0);


                        //time to stop nostalgia, switch to cat horror!!
                        if( Control::Audio->IsPlaying(aTrackL1Mansion) )
                            Control::Audio->StopSample(aTrackL1Mansion);
                        if( !Control::Audio->IsPlaying(aTrackL1Hunted) )
                            Control::Audio->PlaySample(aTrackL1Hunted,false);


                        //Control::Audio->PlaySample(aSFXpopup);
                        PopVal = 0;
                        PopupAlpha = 0.0f;
                        SetupPopupTransition(Popup1Shelf,PopRectShelf);
                    }
                    return;
                }
                break;

            case POPUP_OPEN:
                Pop_CloseButton->PlayAnimation(0);
                if(IsCatAnimPlaying)
                {
					vidTimer += Control::LogicRate;

                    if(
#ifndef FORCE_CLOSE_VID_END
                        cv_Cutscn->isEndReached() ||
#endif 
                        vidTimer > 1.1)
                    {
                        cv_Cutscn->StopAndFadeOutVideo(2.0f);
                        IsCatAnimPlaying = false;
                        Popup1Shelf->GetObjectByID("CatFirstFrame")->SetEnable(false);
                        Hud->AllowInput = true;
                        Hud->ShowHud();
                        Hud->ShowBannerText("L1whoa");

                        Control::Audio->PlaySample(aVOL1MaryRefS3_1);

                        //Journal Entry 8 Cat Attack
                        if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P8_CatAttack])
                        {
                            //Flag Set
                            ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P8_CatAttack] = true;
                            //Add to Note Array
                            ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected]
                                    = eJrn_L1P8_CatAttack;

                            ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;
                            //Increment Notes
                            ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
                            Hud->ShowJrnEntryFX();
                        }

                    }
                    return;
                }

                if(IsObj1MovePlaying)
                {
                    if(P1_MovableObj1->IsTweenFinished())
                    {
                        P1_MovableObj1->SetEnable(false);
                        IsObj1MovePlaying = false;
                        ProfileSystem::Instance->ActiveProfile.L1S3_IsObj1Moved = true;
                    }
                    return;
                }

                if(IsObj2MovePlaying)
                {
                    if(P1_MovableObj2->IsTweenFinished())
                    {
                        P1_MovableObj2->SetEnable(false);
                        IsObj2MovePlaying = false;
                        Hud->AllowInput = false;
                        Hud->HideHud();

                        Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3CatAttack);

                        cv_Cutscn->PlayVideo();
                        IsCatAnimPlaying = true;
                        ProfileSystem::Instance->ActiveProfile.L1S3_IsObj2Moved = true;
                    }
                    return;
                }

                if( !PopAreaRect.Intersection(&pos) || PopCloseButtonRect.Intersection(&pos) )//clicked outside or close Btn
                {
                    if(PopCloseButtonRect.Intersection(&pos))
                        Cursor::SetMode(CA_HandPointy);
                    Pop_CloseButton->PlayAnimation(1);
                    if( Control::Input->LBclicked() )
                    {
                        Control::Audio->PlaySample(aSFXpopdown);
                        Popup1_State = POPUP_SCALEDOWN;

                        Popup1Shelf->Show(false);

                        SetupPopupTransition(Popup1Shelf,PopRectShelf);
                    }
                    return;
                }
                else
                {
                    Pop_CloseButton->PlayAnimation(0);

                    if( ProfileSystem::Instance->ActiveProfile.L1S3_IsObj2Moved && !ProfileSystem::Instance->ActiveProfile.L1S3_NagaKeyTaken && !IsCatAnimPlaying && !IsCatScaling)
                    {
                        if(InvItemRectNagaKey.Intersection(&pos))
                        {
                            Cursor::SetMode(CA_HandTake);
                            if( Control::Input->LBclicked() )
                            {
                                TakeInventoryItem(eInv_L1S3_NagaKey);
                                Popup1Shelf->GetObjectByID("NagaKey")->SetEnable(false);

                                Control::Audio->PlaySample(aSFXpopdown);
                                Popup1_State = POPUP_SCALEDOWN;

                                popupCloseDelayTimer = 1.0f;

                                Popup1Shelf->Show(false);

                                SetupPopupTransition(Popup1Shelf,PopRectShelf);
                            }
                        }
                        return;
                    }

                    Cursor::SetMode(CA_HandTake);
                    if( Control::Input->LBclicked() )
                    {
                        if(!ProfileSystem::Instance->ActiveProfile.L1S3_IsObj1Moved)
                        {
                            Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3ObjectMoveed);

                            float tweenArray[5] = {0,998,334,1,0};
                            P1_MovableObj1->TweenTo(tweenArray,1,0,false);
                            IsObj1MovePlaying = true;
                        }
                        else if(!ProfileSystem::Instance->ActiveProfile.L1S3_IsObj2Moved)
                        {
                            Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3ObjectMoveed);

                            float tweenArray[5] = {0,345,334,1,0};
                            P1_MovableObj2->TweenTo(tweenArray,1,0,false);
                            IsObj2MovePlaying = true;
                        }
                    }
                    return;
                }

                break;
        }
    }

    if ( (ActivePopupID==ePopInactive || ActivePopupID==ePopL1S3_P2Table) && ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved )
    {
        switch( Popup2_State )
        {
            case POPUP_CLOSED:
                if( PopRectTableTop.Intersection(&pos) &&  !ProfileSystem::Instance->ActiveProfile.L1S3_MatchBoxTaken)
                {
                    Cursor::SetMode(CA_Look);
                    if( Control::Input->LBclicked() || ItemDropped )
                    {
                        Popup2_State = POPUP_SCALEUP;
                        ActivePopupID = ePopL1S3_P2Table;

                        Pop_CloseButton = Popup2Table->GetObjectByID("popClose");
                        Pop_CloseButton->PlayAnimation(0);

                        //Control::Audio->PlaySample(aSFXpopup);
                        PopVal = 0;
                        PopupAlpha = 0.0f;
                        SetupPopupTransition(Popup2Table,PopRectTableTop);
                    }
                    return;
                }
                break;

            case POPUP_OPEN:
                if( Elixir::Distance(&pos,&CenterPoint) > 180 || Pop_CloseButton->Collide(&pos) )//clicked outside or close Btn
                {
                    if(Pop_CloseButton->Collide(&pos))
                        Cursor::SetMode(CA_HandPointy);
                    Pop_CloseButton->PlayAnimation(1);
                    if( Control::Input->LBclicked() )
                    {
                        Control::Audio->PlaySample(aSFXpopdown);
                        Popup2_State = POPUP_SCALEDOWN;

                        Popup2Table->Show(false);

                        SetupPopupTransition(Popup2Table,PopRectTableTop);
                    }
                    return;
                }
                else
                {
                    Pop_CloseButton->PlayAnimation(0);
                    if(InvRectBanana.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S3_IsBananaTaken)
                    {
                        Cursor::SetMode(CA_HandTake);
                        if( Control::Input->LBclicked() )
                        {
                            Popup2Table->GetObjectByID("Banana")->SetEnable(false);
                            TakeInventoryItem(eInv_L1S3_Banana);
                            clickDelayTimer = 0.15f;
                            return;
                        }
                    }
                    if(clickDelayTimer>0)
                    {
                        clickDelayTimer -= Control::LogicRate;
                    }
                    else if(InvRectMatchBox.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S3_MatchBoxTaken && ProfileSystem::Instance->ActiveProfile.L1S3_IsBananaTaken)
                    {
                        Cursor::SetMode(CA_HandTake);
                        if( Control::Input->LBclicked() )
                        {
                            Popup2Table->GetObjectByID("MatchBox")->SetEnable(false);

                            TakeInventoryItem( eInv_L1S3_MatchBox);

                            Control::Audio->PlaySample(aSFXpopdown);
                            Popup2_State = POPUP_SCALEDOWN;

                            popupCloseDelayTimer = 1.0f;

                            Popup2Table->Show(false);

                            SetupPopupTransition(Popup2Table,PopRectTableTop);
                        }
                    }
                }

                break;
        }
    }

    if ( (ActivePopupID==ePopInactive || ActivePopupID==ePopL1S3_P3Photos) && ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved )
    {
        switch( Popup3_State )
        {
            case POPUP_CLOSED:
                if( PopRectPoto.Intersection(&pos) )
                {
                    Cursor::SetMode(CA_Look);
                    if( Control::Input->LBclicked() || ItemDropped )
                    {
                        Popup3_State = POPUP_SCALEUP;
                        ActivePopupID = ePopL1S3_P3Photos;

                        //time to stop horror, and switch to nostalgia!!
                        if( Control::Audio->IsPlaying(aTrackL1Hunted) )
                            Control::Audio->StopSample(aTrackL1Hunted);
                        if( !Control::Audio->IsPlaying(aTrackL1Mansion) )
                            Control::Audio->PlaySample(aTrackL1Mansion,false);

                        Pop_CloseButton = Popup3Photos->GetObjectByID("popClose");
                        Pop_CloseButton->PlayAnimation(0);
                        //Control::Audio->PlaySample(aSFXpopup);
                        PopVal = 0;
                        PopupAlpha = 0.0f;
                        SetupPopupTransition(Popup3Photos,PopRectPoto);
                    }
                    return;
                }
                break;

            case POPUP_OPEN:

                if(isBranchPokeAnimPlaying)
                {
                    if(CAnim_BranchHandTake->IsAnimFinished())
                    {
                        isBranchPokeAnimPlaying = false;
                    }
                    return;
                }

                if( !PopAreaRect.Intersection(&pos) || PopCloseButtonRect.Intersection(&pos) )//clicked outside or close Btn
                {
                    if(PopCloseButtonRect.Intersection(&pos))
                        Cursor::SetMode(CA_HandPointy);
                    Pop_CloseButton->PlayAnimation(1);
                    if( Control::Input->LBclicked() )
                    {
                        Control::Audio->PlaySample(aSFXpopdown);
                        Popup3_State = POPUP_SCALEDOWN;

                        Popup3Photos->Show(false);

                        SetupPopupTransition(Popup3Photos,PopRectPoto);
                    }
                    return;
                }
                else
                {
                    Pop_CloseButton->PlayAnimation(0);

                    if(!ProfileSystem::Instance->ActiveProfile.IsPaptusL1S3Collected && PaptusFlyRect.Intersection(&pos))
                    {
                        if(Control::Input->LBclicked())
                        {
                            Control::Audio->PlaySample(aSFXMagic2);
                            Popup3Photos->GetObjectByID("paptus")->FadeOut(1.25f);
                            Hud->CollectMysteryFlower(eArea_L1_S3_Foyer, Popup3Photos->GetObjectByID("paptus")->Pos);
                            return;
                        }
                    }

                    if (!ProfileSystem::Instance->ActiveProfile.L1S3_IsP3ShadowHandFallen)
                    {
                        if (InfoRectHandAbove.Intersection(&pos))
                        {
                            Cursor::SetMode(CA_Gear);
                            if( Control::Input->LBclicked() )
                            {
                                Hud->ShowBannerText("L1need");
                            }
                        }
                    }
                    else if ( !ProfileSystem::Instance->ActiveProfile.L1S3_IsInvShadowHand1Taken )
                    {
                        if(InvItemRectShadowHand.Intersection(&pos))
                        {
                            Cursor::SetMode(CA_HandTake);
                            if( Control::Input->LBclicked() )
                            {
                                Popup3Photos->GetObjectByID("hndfall")->SetEnable(false);
                                TakeInventoryItem(eInv_L1S3_ShadowHand1);
                            }
                            return;
                        }
                    }

                    if (InfoRectCreekFamily.Intersection(&pos))
                    {
                        Cursor::SetMode(CA_Help);
                        if( Control::Input->LBclicked() )
                            Hud->ShowBannerText("L1happy");
                    }
                    else if (InfoRectMotherKid.Intersection(&pos))
                    {
                        Cursor::SetMode(CA_Help);
                        if( Control::Input->LBclicked() )
                            Hud->ShowBannerText("L1remind");
                    }
                    else if (InfoRectCarPic.Intersection(&pos))
                    {
                        Cursor::SetMode(CA_Help);
                        if( Control::Input->LBclicked() )
                            Hud->ShowBannerText("L1young");
                    }
                    else if (InfoRectLabelOnWall.Intersection(&pos))
                    {
                        if( Control::Input->LBclicked() )
                            Hud->ShowBannerText("L1scratch");
                    }

#ifdef _CEBUILD
                    if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S3RibbonNeck] && MemoryRibbonRect.Intersection(&pos))
                    {
                        if( Control::Input->LBclicked() )
                        {
                            Popup3Photos->GetObjectByID("ribbon")->FadeOut();
                            Hud->ShowMemoryObjectCollected_CE(eMem_L1S3RibbonNeck, &Popup3Photos->GetObjectByID("ribbon")->Pos);
                            //time to go back to nosatlgic music!!
                            if( !Control::Audio->IsPlaying(aTrackL1Mansion) &&
                                !Control::Audio->IsPlaying(aTrackL1Hunted) &&
                                !Control::Audio->IsPlaying(aTrackL1EscapePlan) )
                                Control::Audio->PlaySample(aTrackL1Mansion,false);
                            return;
                        }
                    }
#else
                    if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S3RibbonNeck] && MemoryRibbonRect.Intersection(&pos))
					{
							if( Control::Input->LBclicked() )
							{
								Popup3Photos->GetObjectByID("ribbon")->FadeOut();
								Hud->ShowMemoryObjectCollected_SE(eMem_L1S3RibbonNeck, &Popup3Photos->GetObjectByID("ribbon")->Pos);
							}
					}
					//No need for any banners here I guess. Already crowded with banners.
#endif
                }

                break;
        }

    }

}

float L1_S3_Foyer::easingQuadOut(float t,float b,float c,float d)
{
    return (c * -t*(t-2) / d + b);
}

void L1_S3_Foyer::UpdatePopupTransition()
{
    if(!GFInstance->IsScnBg1024Res)
    {
        //1366 resolution
        RT_BgScn->SetScale(1.0f + PopupAlpha * 0.25f);
        RT_BgScn->SetXY(bgScaleOffsetX - (bgScaleOffsetX * (1.0f + PopupAlpha  * 0.25f)) ,bgScaleOffsetY - (bgScaleOffsetY * (1.0f + PopupAlpha * 0.25f)));
    }
    else
    {

        //1024 resolution
        RT_BgScn->SetScale(1.0f + PopupAlpha * 0.25f);
        RT_BgScn->SetXY(bgScaleOffsetX - (bgScaleOffsetX * (1.0f + PopupAlpha  * 0.25f)) +171.0f ,bgScaleOffsetY - (bgScaleOffsetY * (1.0f + PopupAlpha * 0.25f)));
    }

    if ( ActivePopupID==ePopL1S3_P1Shelf ) //Popup 5 - Mom statue
    {
        switch( Popup1_State )
        {
            case POPUP_SCALEUP:
                PopVal += POPUP_SCALE_FACTOR *  Control::LogicRate;
                PopupAlpha = easingQuadOut(PopVal,0,1,1.0f);
                //PopupAlpha += POPUP_SCALE_FACTOR *  Control::LogicRate;
                if( PopVal >= 1.0f )
                {
                    IsPopupTransitionActive = false;

                    PopupAlpha = 1.0f;
                    Popup1Shelf->Show(true);
                    Popup1_State = POPUP_OPEN;
                }
                GFInstance->UpdateBlurScene(PopupAlpha>1.0f?1.0f:PopupAlpha);
                break;
            case POPUP_SCALEDOWN:
                popupCloseDelayTimer -= 1.0f * Control::LogicRate;
                if( popupCloseDelayTimer > 0.0f )
                {
                    return;
                }
                else
                {
                    popupCloseDelayTimer = 0.0;
                }

                PopupAlpha -= POPUP_SCALEDOWN_FACTOR * Control::LogicRate;
                if( PopupAlpha <= 0.0f )
                {
                    IsPopupTransitionActive = false;

                    PopupAlpha = 0.0f;
                    GFInstance->TurnOffBlurScene();
                    Popup1_State = POPUP_CLOSED;
                    ActivePopupID = ePopInactive;

                    HOScene->PauseObjects(false);

#ifdef _CEBUILD
                    Hud->ShowMorphObjects();
#endif

                    EndBgGraphicScaling();

                    SetZ(zOrderScnVideo);
                }
                else
                    GFInstance->UpdateBlurScene(PopupAlpha);
                break;
        }
    }

    if ( ActivePopupID==ePopL1S3_P2Table ) //Popup 5 - Mom statue
    {
        switch( Popup2_State )
        {
            case POPUP_SCALEUP:
                PopVal += POPUP_SCALE_FACTOR *  Control::LogicRate;
                PopupAlpha = easingQuadOut(PopVal,0,1,1.0f);
                if( PopVal >= 1.0f )
                {
                    IsPopupTransitionActive = false;

                    PopupAlpha = 1.0f;
                    Popup2Table->Show(true);
                    Popup2_State = POPUP_OPEN;
                }
                GFInstance->UpdateBlurScene(PopupAlpha>1.0f?1.0f:PopupAlpha);

                break;
            case POPUP_SCALEDOWN:
                popupCloseDelayTimer -= 1.0f * Control::LogicRate;
                if( popupCloseDelayTimer > 0.0f )
                {
                    return;
                }
                else
                {
                    popupCloseDelayTimer = 0.0;
                }
                PopupAlpha -= POPUP_SCALEDOWN_FACTOR * Control::LogicRate;
                if( PopupAlpha <= 0.0f )
                {
                    IsPopupTransitionActive = false;

                    PopupAlpha = 0.0f;
                    GFInstance->TurnOffBlurScene();
                    Popup2_State = POPUP_CLOSED;
                    ActivePopupID = ePopInactive;

                    HOScene->PauseObjects(false);

#ifdef _CEBUILD
                    Hud->ShowMorphObjects();
#endif

                    EndBgGraphicScaling();

                    SetZ(zOrderScnVideo);
                }
                else
                    GFInstance->UpdateBlurScene(PopupAlpha);
                break;
        }
    }

    if ( ActivePopupID==ePopL1S3_P3Photos ) //Popup 5 - Mom statue
    {
        switch( Popup3_State )
        {
            case POPUP_SCALEUP:
                PopVal += POPUP_SCALE_FACTOR *  Control::LogicRate;
                PopupAlpha = easingQuadOut(PopVal,0,1,1.0f);

                if( PopVal >= 1.0f )
                {
                    IsPopupTransitionActive = false;

                    PopupAlpha = 1.0f;
                    Popup3Photos->Show(true);
                    Popup3_State = POPUP_OPEN;
                }
                GFInstance->UpdateBlurScene(PopupAlpha>1.0f?1.0f:PopupAlpha);

                break;
            case POPUP_SCALEDOWN:
                popupCloseDelayTimer -= 1.0f * Control::LogicRate;
                if( popupCloseDelayTimer > 0.0f )
                {
                    return;
                }
                else
                {
                    popupCloseDelayTimer = 0.0;
                }
                PopupAlpha -= POPUP_SCALEDOWN_FACTOR * Control::LogicRate;
                if( PopupAlpha <= 0.0f )
                {
                    IsPopupTransitionActive = false;

                    PopupAlpha = 0.0f;
                    GFInstance->TurnOffBlurScene();
                    Popup3_State = POPUP_CLOSED;
                    ActivePopupID = ePopInactive;

#ifdef _CEBUILD
                    Hud->ShowMorphObjects();
#endif

                    HOScene->PauseObjects(false);

                    EndBgGraphicScaling();

                    //Journal Entry 6 Add
                    if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P6_ForgiveMeAnnMary])
                    {
                        //Flag Set
                        ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P6_ForgiveMeAnnMary] = true;
                        //Add to Note Array
                        ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected]
                                = eJrn_L1P6_ForgiveMeAnnMary;

                        ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;
                        //Increment Notes
                        ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
                        Hud->ShowJrnEntryFX();
                    }

                    SetZ(zOrderScnVideo);
                }
                else
                    GFInstance->UpdateBlurScene(PopupAlpha);
                break;
        }
    }

}

void L1_S3_Foyer::EndBgGraphicScaling()
{
    RT_BgScn->SetScale(1.0f);
    RT_BgScn->SetEnable(false);

    int size = (int) VFXArray.size();  //Sac: converted Implicit to explicit

    for (int i = 0; i < size; i++)
        VFXArray[i]->Show(true);

    Scn_Bg->Show(true);
    HOScene->GetObjectByID("BedroomLight")->Show(true);
    //HOScene->Show(true);
}

void L1_S3_Foyer::Render(const CPoint* pCam)
{
    if( !Visible )
        return;

    if(ActivePopupID != ePopInactive)
    {
        if(IsPopupTransitionActive)
        {
            //Init again if render target lost due to fullscreen-window switches or similar
            if( GFInstance->RTGraphic != NULL )
            {
                GFInstance->RTGraphic->blitAlphaRectFx(0,0,1366,768,
                                                       short(GFInstance->PStartX+float(GFInstance->PEndX-GFInstance->PStartX)*PopupAlpha),
                                                       short(GFInstance->PStartY+float(GFInstance->PEndY-GFInstance->PStartY)*PopupAlpha),
                                                       -5*(1.0f-PopupAlpha),
                                                       PopupAlpha,PopupAlpha>1.0f?1.0f:PopupAlpha);

                //Gradient alpha causes alpha on transition, leaking the bg scene through popups during transition
                GFInstance->RTGraphic->blitAlphaRectFx(0,0,1366,768,
                                                       short(GFInstance->PStartX+float(GFInstance->PEndX-GFInstance->PStartX)*PopupAlpha),
                                                       short(GFInstance->PStartY+float(GFInstance->PEndY-GFInstance->PStartY)*PopupAlpha),
                                                       -5*(1.0f-PopupAlpha),
                                                       PopupAlpha,PopupAlpha>1.0f?1.0f:PopupAlpha);

            }
        }
    }
}

L1_S3_Foyer::~L1_S3_Foyer()
{
    TaskPendingUpdate();

    SAFE_DELETE(flyingInsects);

    int size = (int) VFXArray.size();  //Sac: converted Implicit to explicit

    for (int i = 0; i < size; i++){
        SAFE_DELETE(VFXArray[i]);
    }

    SAFE_DELETE(CAnim_RajaTakeJames);

    SAFE_DELETE(Popup1Shelf);
    SAFE_DELETE(Popup2Table);
    SAFE_DELETE(Popup3Photos);

    SAFE_DELETE(CAnim_BranchHandTake);

    if(RT_BgScn!=NULL)
        RT_BgScn->pSprite->Image = NULL;
    SAFE_DELETE(RT_BgScn);

    //MUSIC_SFX
    if( ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved )
    {
        Control::Audio->StopSample(aAmbS3FoyerLit);
        Control::Audio->UnloadSample(aAmbS3FoyerLit);
    }
    else
    {
        Control::Audio->StopSample(aAmbS3FoyerUnlit);
        Control::Audio->UnloadSample(aAmbS3FoyerUnlit);
    }
    Control::Audio->UnloadSample(aVOL1MaryRefS3_1);
    //MUSIC_SFX
}

void L1_S3_Foyer::ReceiveMessage(int val)
{
    if( val == HOSM_DialogClosed )
    {
        isDialogActive = false;
        Hud->AllowInput = true;
        Hud->ShowHud(true);

        if(  !ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved )
        {
            //Objective Add
            if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L1O2_LitTheMansion] )
            {
                ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L1O2_LitTheMansion] = true;
                ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
                        = eObj_L1O2_LitTheMansion;
                //Increment Objectives
                ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
                Hud->ShowObjectiveFX(eObj_L1O2_LitTheMansion);

                Control::Audio->PlaySample(aSFXMagic1);
            }
        }
    }
    else if( val == MAP_HUD_CLICK )
    {
        TaskPendingUpdate();
        Hud->ShowMap();
    }
    else if( val == MAP_NEW_AREA )
    {
        SendMessage(ControlExit, ID, HOSM_NewArea);
    }
    else if( val == INVBOX_DROP )
    {
        CPoint dropPos;
        int itemVal;
        if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
        {
            if( itemVal == eInv_L1S1_CreekLetter && ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved)
            {
                if(ActionRectRajaTalk.Intersection(&dropPos))
                {
                    dialogSystem->ShowDialog(eDialog_S3RajaFoyer);
                    isDialogActive = true;

                    Hud->AllowInput = false;
                    Hud->HideHud(true);

                    Hud->ReleaseDrag();
                    Hud->CloseInventory();

                    Hud->HideMessageBanner();

                    ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S1_CreekLetter] = false;
                    Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S1_CreekLetter);
                    return;
                }
                else
                {
                    Hud->ShowBannerText("L1care");
                }
            }

            if(ActivePopupID == ePopInactive)
            {
                //check for popup actions
                UpdatePopups(dropPos,true);
                if(ActivePopupID == ePopInactive)
                {
                    //If No popups responded
                    GFInstance->WrongInvDropMsg(Hud);
                }
                return;
            }
            else if( itemVal == eInv_L1S2_TreeBranch)
            {
                if(ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved && Popup3_State == POPUP_OPEN)
                {
                    if(InvDestRectBranchHand.Intersection(&dropPos) && !ProfileSystem::Instance->ActiveProfile.L1S3_IsP3ShadowHandFallen)
                    {
                        ProfileSystem::Instance->ActiveProfile.L1S3_IsP3ShadowHandFallen = true;

                        Hud->ReleaseDrag();
                        Hud->CloseInventory();

                        ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S2_TreeBranch] = false;
                        Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S2_TreeBranch);
                        Control::Audio->QuickLoadAndPlaySFX(aSFXL1S3StickUsed);
                        isBranchPokeAnimPlaying = true;
                        CAnim_BranchHandTake->PlayAnim();
                    }
                    else
                        GFInstance->WrongInvDropMsg(Hud);
                }
                else
                    GFInstance->WrongInvDropMsg(Hud);
            }
            else if( itemVal == eInv_L1S6_Crowbar)
            {
                if(ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved && Popup3_State == POPUP_OPEN)
                {
                    if(InvDestRectBranchHand.Intersection(&dropPos) && !ProfileSystem::Instance->ActiveProfile.L1S3_IsP3ShadowHandFallen)
                    {
                        Hud->ShowBannerText("L1too");
                        Hud->ReleaseDrag();
                        Hud->CloseInventory();
                    }
                    else
                        GFInstance->WrongInvDropMsg(Hud);
                }
                else
                    GFInstance->WrongInvDropMsg(Hud);
            }
            else
                GFInstance->WrongInvDropMsg(Hud);
        }
    }
    else if( val == INV_CLOSE )
    {



    }
}

void L1_S3_Foyer::TakeInventoryItem(EHOInventory ItemID)
{
    std::vector<int> vecInvIds;
    switch (ItemID)
    {
        case eInv_L1S3_ShadowHand1:
            Hud->InvBoxCtrl->Activate(1);
            ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S3_ShadowHand1] = true;
            ProfileSystem::Instance->ActiveProfile.L1S3_IsInvShadowHand1Taken = true;
            vecInvIds.push_back(eInv_L1S3_ShadowHand1);
            Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
            TaskPendingUpdate();
            isSparkleUpdatePending = true;
            break;

        case eInv_L1S3_NagaKey:
            Hud->InvBoxCtrl->Activate(1);
            ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S3_NagaKey] = true;
            ProfileSystem::Instance->ActiveProfile.L1S3_NagaKeyTaken = true;
            vecInvIds.push_back(eInv_L1S3_NagaKey);
            Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
            TaskPendingUpdate();
            isSparkleUpdatePending = true;
            break;

        case eInv_L1S3_Banana:
            Hud->InvBoxCtrl->Activate(1);
            ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S3_Banana] = true;
            ProfileSystem::Instance->ActiveProfile.L1S3_IsBananaTaken = true;
            vecInvIds.push_back(eInv_L1S3_Banana);
            Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
            TaskPendingUpdate();
            isSparkleUpdatePending = true;
            break;

        case  eInv_L1S3_MatchBox:
            Hud->InvBoxCtrl->Activate(1);
            ProfileSystem::Instance->ActiveProfile.Inventory[ eInv_L1S3_MatchBox] = true;
            ProfileSystem::Instance->ActiveProfile.L1S3_MatchBoxTaken = true;
            vecInvIds.push_back(eInv_L1S3_MatchBox);
            Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
            TaskPendingUpdate();
            isSparkleUpdatePending = true;
            break;

        default:
            break;
    }

}

void L1_S3_Foyer::ProcessNonHOHint()
{
    if (GFInstance->IsReplayingGame)
    {
        if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S3Collected)
        {
            InitHintTrailEmit(420, 340);
        }
        else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S3MaryLamb])
        {
            Hud->teleportSystem->ShowTeleport();
        }
        else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S3RibbonNeck])
        {
            if(Popup3_State == POPUP_OPEN)
                InitHintTrailEmit(650, 430);
            else
                InitHintTrailEmit(900, 160);
        }
        else
        {
            Hud->teleportSystem->ShowTeleport();
        }
        return;
    }

    if(isRajaTakeJamesPlaying)
        return;

    int iNonHOHint = 0;
    int MaxNonHOHints = 16;
    do
    {
        switch( iNonHOHint )
        {
            default:
                //Exit coordinates
            case 0: ++iNonHOHint;
                if( !ProfileSystem::Instance->ActiveProfile.L1S3_MatchBoxTaken &&  Popup2_State == POPUP_OPEN)
                {
                    InitHintTrailEmit(	InvRectMatchBox.GetCenterX(), InvRectMatchBox.GetCenterY(),false);
                    return;
                }
                break;

            case 1: ++iNonHOHint;
                if(  !ProfileSystem::Instance->ActiveProfile.L1S3_NagaKeyTaken  && Popup1_State == POPUP_OPEN)
                {
                    InitHintTrailEmit(	PopAreaRect.GetCenterX(), PopAreaRect.GetCenterY(),false);
                    return;
                }
                break;
            case 2: ++iNonHOHint;
                if(  !ProfileSystem::Instance->ActiveProfile.L1S3_IsP3ShadowHandFallen && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvTreeBranchTaken  && Popup3_State == POPUP_OPEN)
                {
                    Hud->InvBoxCtrl->IsLocked = true;
                    Hud->InvBoxCtrl->Activate();

                    for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
                    {
                        if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
                        {
                            if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S2_TreeBranch)
                            {
                                if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!!
                                    Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
                                else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
                                    Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);

                                InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
                            }
                        }
                    }

                    CPoint tempPoint (Popup3Photos->GetObjectByID("hndfall")->Pos.x, Popup3Photos->GetObjectByID("hndfall")->Pos.y,0);
                    Hud->Hint2FX->ResetPos(&tempPoint);
                    Hud->Hint2FX->Emit();

                    return;
                }
                break;
            case 3: ++iNonHOHint;
                if(  !ProfileSystem::Instance->ActiveProfile.L1S3_IsInvShadowHand1Taken && ProfileSystem::Instance->ActiveProfile.L1S3_IsP3ShadowHandFallen && Popup3_State == POPUP_OPEN)
                {
                    InitHintTrailEmit(	InvItemRectShadowHand.GetCenterX(), InvItemRectShadowHand.GetCenterY(),false);
                    return;
                }
                break;
            case 4: ++iNonHOHint;
                if( Popup3_State == POPUP_OPEN)
                {
                    InitHintTrailEmit(	PopCloseButtonRect.GetCenterX(), PopCloseButtonRect.GetCenterY(),true);
                    return;
                }
                break;
            case 5: ++iNonHOHint;
                if(!ProfileSystem::Instance->ActiveProfile.L1S3_RajaTalkOver && ProfileSystem::Instance->ActiveProfile.L1S1_IsInvLetterCreekTaken && ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved)
                {
                    Hud->InvBoxCtrl->IsLocked = true;
                    Hud->InvBoxCtrl->Activate();

                    for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
                    {
                        if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
                        {
                            if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S1_CreekLetter)
                            {
                                if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!!
                                    Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
                                else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
                                    Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);

                                InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
                            }
                        }
                    }

                    CPoint tempPoint (ActionRectRajaTalk.GetCenterX(), ActionRectRajaTalk.GetCenterY(),0);
                    Hud->Hint2FX->ResetPos(&tempPoint);
                    Hud->Hint2FX->Emit();

                    return;
                }
                break;
            case 6: ++iNonHOHint;
                if( !ProfileSystem::Instance->ActiveProfile.L1S3_NagaKeyTaken && ProfileSystem::Instance->ActiveProfile.L1S3_RajaTakenJames)
                {
                    InitHintTrailEmit(	PopRectShelf.GetCenterX(), PopRectShelf.GetCenterY(),false);
                    return;
                }
                break;

            case 7: ++iNonHOHint;
                if(  !ProfileSystem::Instance->ActiveProfile.L1S3_MatchBoxTaken   && ProfileSystem::Instance->ActiveProfile.L1S3_RajaTakenJames)
                {
                    InitHintTrailEmit(	PopRectTableTop.GetCenterX(), PopRectTableTop.GetCenterY(),false);
                    return;
                }
                break;
            case 8: ++iNonHOHint;
                if( !ProfileSystem::Instance->ActiveProfile.L1S3_IsP3ShadowHandFallen && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvTreeBranchTaken && ProfileSystem::Instance->ActiveProfile.L1S3_RajaTakenJames)
                {
                    InitHintTrailEmit(	PopRectPoto.GetCenterX(), PopRectPoto.GetCenterY(),false);
                    return;
                }
                break;
            case 9: ++iNonHOHint;
                if(  !ProfileSystem::Instance->ActiveProfile.L1S3_IsInvShadowHand1Taken  && ProfileSystem::Instance->ActiveProfile.L1S3_IsP3ShadowHandFallen && ProfileSystem::Instance->ActiveProfile.L1S3_RajaTakenJames)
                {
                    InitHintTrailEmit(	PopRectPoto.GetCenterX(), PopRectPoto.GetCenterY(),false);
                    return;
                }
                break;
            case 10: ++iNonHOHint;
                if( !ProfileSystem::Instance->ActiveProfile.L1S3_IsInvHOLocketTaken && ProfileSystem::Instance->ActiveProfile.L1S3_RajaTakenJames)
                {
                    InitHintTrailEmit(	SceneRectShelfPuzzle.GetCenterX(), SceneRectShelfPuzzle.GetCenterY(),false);
                    return;
                }
                break;

            case 11: ++iNonHOHint;
                if(  !ProfileSystem::Instance->ActiveProfile.L1S3_IsBedroomDoorOpened   && ProfileSystem::Instance->ActiveProfile.L1S3_IsInvHOLocketTaken)
                {
                    InitHintTrailEmit(	SceneRectDoorHO.GetCenterX(), SceneRectDoorHO.GetCenterY(),false);
                    return;
                }
                break;
            case 12: ++iNonHOHint;
                if(ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby1Branch2Taken && ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby2Branch3Taken &&ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby3UnderLeaf1Taken && ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby4UnderLeavesTaken &&  !ProfileSystem::Instance->ActiveProfile.L1S1_IsInvLetterCreekTaken)
                {
                    Hud->InvBoxCtrl->IsLocked = true;
                    Hud->InvBoxCtrl->Activate();

                    for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
                    {
                        if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
                        {
                            if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S1_Bag)
                            {
                                if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!!
                                    Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
                                else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
                                    Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);

                                InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
                            }
                        }
                    }

                    return;
                }
                break;
            case 13: ++iNonHOHint;
                if((!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S4_Bedroom] && ProfileSystem::Instance->ActiveProfile.L1S3_IsBedroomDoorOpened) || ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S4_Bedroom])
                {
                    InitHintTrailEmit(SceneRectBedroom.GetCenterX(), SceneRectBedroom.GetCenterY(),true,CA_ExitRight);
                    return;
                }
                break;
            case 14: ++iNonHOHint;
                if( ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S2_Frontyard])
                {
                    InitHintTrailEmit(HoveredBackBtnRectMax.topleft.x + HoveredBackBtnRectMax.w/2 ,HoveredBackBtnRectMax.topleft.y + HoveredBackBtnRectMax.h/2,true,CA_ExitDown);
                    return;
                }
                break;
            case 15: ++iNonHOHint;
                {
                    Hud->teleportSystem->ShowTeleport();
                    return;
                }
                break;

        }
    }
    while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
    Hud->ShowNothingToDoMsgBanner();
}

void  L1_S3_Foyer::SetupPopupTransition(CHOScene *Popup, CRectangle popRect)
{

    // Scn_Bg->Show(false); Sac: why hide the background ??
    HOScene->GetObjectByID("BedroomLight")->Show(false);
    //	HOScene->Show(false);

#ifdef _CEBUILD
    Hud->HideMorphObjects();
#endif

    HOScene->PauseObjects(true);

    int size = (int) VFXArray.size();  //Sac: converted Implicit to explicit

    for (int i = 0; i < size; i++)
        VFXArray[i]->Show(false);

    bgScaleOffsetX = popRect.GetCenterX();
    bgScaleOffsetY = popRect.GetCenterY();

    RT_BgScn->SetEnable(true);

    IsPopupTransitionActive = true;
    if( GFInstance->IsRTCreated )
    {
        GFInstance->RTGraphic->clearRenderTarget();
        GFInstance->RTGraphic->beginRenderToTarget();//RENDER TO TARGET STARTS
        GFInstance->IsRenderingToRT = true;
    }
    Popup->RenderToTarget(-171,0);
    if( GFInstance->IsRTCreated )
    {
        GFInstance->IsRenderingToRT = false;
        GFInstance->RTGraphic->endRenderToTarget();
    }
    GFInstance->SetPopupTween( (int)popRect.GetCenterX(-683) ,(int)popRect.GetCenterY(-384 ),171,0);

    if(Popup1_State == POPUP_SCALEUP || Popup2_State == POPUP_SCALEUP || Popup3_State == POPUP_SCALEUP)
        GFInstance->CreateGreyscaleScene();

    SetZ(zOrderPopVideo);

}


void L1_S3_Foyer::NavigateToScene()
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