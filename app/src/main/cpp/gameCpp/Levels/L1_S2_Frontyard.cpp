//====================================
//	DQFYLH
//	HO System
//	Level 1 - Scene 02 : Frontyard
//====================================

#include "L1_S2_Frontyard.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "Rect.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"

const CPoint	CenterPoint(683,334,0);
const CPoint	PopCloseRectBtnPoint(998,98,0);

const CRectangle  PopCloseButtonRect(968.0f,71.0f,62.0f,61.0f);
const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  popRectSack(486.0f,362.0f,196.0f,188.0f);
const CRectangle  popRectHose(667.0f,183.0f,119.0f,104.0f);
const CRectangle  SceneRectFuzePuzzle(804.0f,345.0f,184.0f,142.0f);

//Scene Rects
const CRectangle  SceneRectFoyer(666.0f,260.0f,179.0f,196.0f);
const CRectangle  SceneRectBoatdock(332.0f,459.0f,195.0f,117.0f);

const CRectangle  InvItemRectPlank(390.0f,226.0f,563.0f,343.0f);

//Item Rects
const CRectangle  InvItemRectClothP1CarDicky(1115,304,96.0f,50.0f);
const CRectangle  InvItemRectClothP2(502.0f,206.0f,110.0f,200.0f);
const CRectangle  InvItemRect_Cloth1Swing(794.0f,124.0f,140.0f,82.0f);
const CRectangle  InvDestRect_SwingRopeCut(706.0f,86.0f,237.0f,395.0f);
const CRectangle  InvDestRect_SwingRopeCut2(387.0f,218.0f,558.0f,351.0f);

//Popup Rects
const CRectangle  PopRectCarDickie(978.0f,358.0f,242.0f,232.0f);

const CRectangle  PopRectLove(160.0f,219.0f,237.0f,314.0f);
const CRectangle  PopRect3Swing(594.0f,345.0f,54.0f,111.0f);

const CRectangle  PopLoveFrameRect(508.0f,156.0f,395.0f,356.0f);

const CPoint baboonPos(1037,283,0);
const CRectangle  SceneRectBaboon(1047.0f,266.0f,212.0f,270.0f);
const CRectangle  SceneBigRectBaboon(1047.0f-50,266.0f-50,212.0f+100,270.0f+100);

//609,255
const CPoint baboonRunAnim1Pos(619,264,0);
const CPoint baboonRunAnim2Pos(347,453,0);
const CPoint baboonRunAnim3Pos(310,471,0);
const CPoint baboonRunAnim4Pos(310,480,0);

const CPoint BananaThrowAnimPos(692,414,0);
const float BaboonAnimThreshold = 2.8f;		//( 69 frames * 41 )
const CRectangle  InvDestRectCrowbarDicky(380.0f,120.0f,552.0f,420.0f);

const CRectangle  InfoRectMansion(714.0f,7.0f,488.0f,249.0f);
const CRectangle  InfoRectMoon(496.0f,119.0f,217.0f,196.0f);
const CRectangle  InfoRectBeetleTree(1204.0f,5.0f,159.0f,423.0f);
const CRectangle  InfoRectGards(619.0f,372.0f,95.0f,110.0f);
const CRectangle  InfoRectBranch(361.0f,111.0f,196.0f,124.0f);

const CRectangle  PaptusFlyRect(682.0f,171.0f,68.0f,69.0f);

//Video Rects
float lightAlphaArray[20] = {0,0,0,0,0,0.15f,0.75f,0.95f,0.85f,0.75f,0.85f,0.65f,0.95f,0.15f,0.25f,0.55f,0.15f,0,0,0};

L1_S2_Frontyard::L1_S2_Frontyard()
{
	Visible = false;

	RT_BgScn = NULL;

	//Main Scene
	LoadHud(INVENTORY_V1);

	IsMansionLit = ProfileSystem::Instance->ActiveProfile.L1_IsMansionPowerOn;

	RainBgVFX = new SuperFX("PRT\\BG\\rainfuse.INI", PointSystem::CreateCPoint(580.0f,-50.0f,0), 100);
	RainBgVFX->Start();
	RainBgVFX->AdvanceByTime(2.0f);

	RainDropsVFX = new SuperFX("PRT\\L1\\raindropsFY.INI", PointSystem::CreateCPoint(759.0f,581.0f,0), 100);
	RainDropsVFX->Start();
	RainDropsVFX->AdvanceByTime(2.0f);

	WolfHowlTimer = 0;
	WolfHowlTimeThreshold = 12.0f + (float)(rand()%8);

	flyingInsects = new FlyingInsects();
	flyingInsects->InitFlies(281,118,353,190);

	Scn_Sparks = NULL;
	Scn_FuseBgGlow = NULL;

	scn_cloudBig = NULL;
	scn_cloudSmall = NULL;

	if(IsMansionLit)
	{
		LoadScene("LV1\\S2\\LIGHTSCN.SCN");
		
		scn_cloudBig = HOScene->GetObjectByID("cloudbig");
		scn_cloudSmall = HOScene->GetObjectByID("cloudsmall");

		Scn_Lightning = HOScene->GetObjectByID("lightning");

		plant1 = HOScene->GetObjectByID("lightPlant1");
		plant2 = HOScene->GetObjectByID("lightPlant2");
		plant3 = HOScene->GetObjectByID("lightPlant3");
		plant4 = HOScene->GetObjectByID("lightPlant4");
		plant5 = HOScene->GetObjectByID("lightPlant5");
		plant6 = HOScene->GetObjectByID("lightPlant6");
		plant7 = HOScene->GetObjectByID("lightPlant7");

		branch1 = HOScene->GetObjectByID("lightBranch1");
		branch2 = HOScene->GetObjectByID("lightBranch2");
		branch4 = HOScene->GetObjectByID("lightBranch4");

		HOScene->GetObjectByID("litgateglow")->Animate(0.25f, 0.85f, 3.0f);
		HOScene->GetObjectByID("litgroundlamp")->Animate(0.15f, 0.85f, 2.0f, 1.0f);

		HOScene->GetObjectByID("litwindowglowtop")->SetScale(0.65f);
		HOScene->GetObjectByID("litwindowglowtop")->SetBlendMode(eBlend_AlphaAdditive);
		HOScene->GetObjectByID("litwindowglowtop")->Animate(0.15f, 0.65f, 6.0f);

		HOScene->GetObjectByID("litwindowglowdown")->SetScale(0.45f);
		HOScene->GetObjectByID("litwindowglowdown")->SetBlendMode(eBlend_AlphaAdditive);
		HOScene->GetObjectByID("litwindowglowdown")->Animate(0.15f, 0.65f, 5.0f, 1.0f);

		clothCarDicky = HOScene->GetObjectByID("ClothCarDicky");
		clothLoveSign = HOScene->GetObjectByID("ClothLoveSign");
		

		carDickyOpened = HOScene->GetObjectByID("carDickyOpened");
		DieselCan = HOScene->GetObjectByID("DieselCan");
		hose = HOScene->GetObjectByID("hose");
		sack = HOScene->GetObjectByID("sack");
		wood = HOScene->GetObjectByID("wood2");
		openedCarDickyDoor = HOScene->GetObjectByID("openedCarDickyDoor");
		walkieTalkie = HOScene->GetObjectByID("walkieTalkie");
		rope = HOScene->GetObjectByID("rope");
		bottle = HOScene->GetObjectByID("bottle");
		shoe = HOScene->GetObjectByID("shoe");

		tree = HOScene->GetObjectByID("darkTree");

		if( ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth2LovePopTaken )
		{
			clothLoveSign->SetEnable( false );
		}
		if( ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth3CarDickyTaken )
		{
			clothCarDicky->SetEnable( false);
		}

		if(!ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite)
		{
			 HOScene->GetObjectByID("warriors")->SetEnable(false);
		}
		else
		{	
		}

		if(ProfileSystem::Instance->ActiveProfile.L1S2_IsCarDickeyOpen)
		{
			if(ProfileSystem::Instance->ActiveProfile.L1S2_P1_IsSackMoved)
			{
				sack->SetEnable(false);
			}
			if(ProfileSystem::Instance->ActiveProfile.L1S2_P1_IsWoodMoved)
			{
				wood->SetEnable(false);
			}

			if(ProfileSystem::Instance->ActiveProfile.L1S2_InvDieselCanTaken)
			{
				DieselCan->SetEnable(false);
			}
			if(ProfileSystem::Instance->ActiveProfile.L1S2_InvHoseTaken)
			{
				hose->SetEnable(false);
			}
		}
		else
		{
			carDickyOpened->SetEnable(false);
			DieselCan->SetEnable(false);
			hose->SetEnable(false);
			sack->SetEnable(false);
			wood->SetEnable(false);
			openedCarDickyDoor->SetEnable(false);
			walkieTalkie->SetEnable(false);
			rope->SetEnable(false);
			bottle->SetEnable(false);
			shoe->SetEnable(false);
		}
	}
	else
	{
		LoadScene("LV1\\S2\\DARKSCN.SCN");		
		
		Scn_Lightning = HOScene->GetObjectByID("lightning");

		scn_cloudBig = HOScene->GetObjectByID("cloudbig");
		scn_cloudSmall = HOScene->GetObjectByID("cloudsmall");

		plant1 = HOScene->GetObjectByID("darkPlant1");
//		plant1->SetZ(101);
		plant2 = HOScene->GetObjectByID("darkPlant2");
	//	plant2->SetZ(102);
		plant3 = HOScene->GetObjectByID("darkPlant3");
		plant4 = HOScene->GetObjectByID("darkPlant4");
		plant5 = HOScene->GetObjectByID("darkPlant5");
		plant6 = HOScene->GetObjectByID("darkPlant6");
		plant7 = HOScene->GetObjectByID("darkPlant7");

		branch1 = HOScene->GetObjectByID("darkBranch1");
		branch2 = HOScene->GetObjectByID("darkBranch2");
		branch4 = HOScene->GetObjectByID("darkBranch4");

		tree = HOScene->GetObjectByID("darkTree");

		Scn_Sparks = HOScene->GetObjectByID("Sparks");
		Scn_Sparks->Paused = true;
		Scn_FuseBgGlow = HOScene->GetObjectByID("sparkglow");
		Scn_FuseBgGlow->Paused = true;

		clothCarDicky = HOScene->GetObjectByID("ClothCarDickyDark");
		clothLoveSign = HOScene->GetObjectByID("ClothLoveSignDark");
		
		if( ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth2LovePopTaken )
		{
			clothLoveSign->SetEnable( false );
		}
		if( ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth3CarDickyTaken )
		{
			clothCarDicky->SetEnable( false);
		}

	}

	scn_cloudBig->SetBlendMode(eBlend_AlphaAdditive);
	scn_cloudSmall->SetBlendMode(eBlend_AlphaAdditive);

	scn_cloudBig->Pos.x -= rand()%200;
	scn_cloudSmall->Pos.x -= rand()%200;

	scn_cloudBig->SetAlpha(0.55f);
	scn_cloudSmall->SetAlpha(0.55f);

	FuseSparkTimer = 0;
	FuseSparkThreshold = 2.0f + rand()%3;

	Scn_Lightning->SetScale(2.0f);
	Scn_Lightning->SetBlendMode(eBlend_AlphaAdditive);
	Scn_Lightning->Show(false);

	lightFrame = 0;
	lightningTimer = 0;
	nextLightningThreshold = 8.0f;

	BaboonNormalAnim = NULL;
	BaboonHungryAnim = NULL;
	BaboonAttackAnim = NULL;
	BananaThrowAnim = NULL;
	isBaboonHungryAnimPlaying = false;
	isBaboonAttackAnimPlaying = false;
	isBananaThrowAnimPlaying = false;
	isBaboonRunAnimPlaying = false;
	BaboonAnimTimer = 0.0f;
	BaboonAnimThresholdMultiplier = 2.0f + std::rand()%3;
	baboonRunPointer = 0;
	
	for (int i = 0; i < 2; i++)
	{
		BaboonRunAnim[i] = NULL;
	}

	if(!ProfileSystem::Instance->ActiveProfile.L1S2_IsMonkeyTookBanana)
	{
		BaboonNormalAnim = new CObject();
		BaboonNormalAnim->LoadSprite("LV1\\S2\\BaboonNormalAnim.SPR");
		BaboonNormalAnim->Pos = baboonPos;
		BaboonNormalAnim->SetZ(100);

		BaboonAttackAnim = new CObject();
		BaboonAttackAnim->LoadSprite("LV1\\S2\\BaboonAttackAnim.SPR");
		BaboonAttackAnim->Pos = baboonPos;
		
		BaboonAttackAnim->SetZ(100);
		BaboonAttackAnim->SetEnable(false);

		BaboonHungryAnim = new CObject();
		BaboonHungryAnim->LoadInit("LV1\\S2\\BaboonHungryAnim.SPR", 1051, 292, 100, false); 

		HOScene->PushObject(BaboonHungryAnim);
		HOScene->PushObject(BaboonAttackAnim);
		HOScene->PushObject(BaboonNormalAnim);

		if(ProfileSystem::Instance->ActiveProfile.L1S3_IsBananaTaken)
		{
			//Baboon Run animations - baboonRunPointer to switch anims
			BaboonRunAnim[0] = new CObject();
			BaboonRunAnim[0]->LoadSprite("LV1\\S2\\BaboonJumpAnim.SPR");
			BaboonRunAnim[0]->Pos = baboonRunAnim1Pos;
			BaboonRunAnim[0]->SetZ(100);
			BaboonRunAnim[0]->SetEnable(false);

			BaboonRunAnim[1] = new CObject();
			BaboonRunAnim[1]->LoadSprite("LV1\\S2\\BaboonRunAnim.SPR");
			BaboonRunAnim[1]->Pos = baboonRunAnim2Pos;
			BaboonRunAnim[1]->SetZ(100);
			BaboonRunAnim[1]->SetEnable(false);

			BananaThrowAnim = new CObject();
			BananaThrowAnim->LoadSprite("LV1\\S2\\BananaThrowAnim.SPR");
			BananaThrowAnim->Pos = BananaThrowAnimPos;
			BananaThrowAnim->SetZ(100);
			BananaThrowAnim->SetEnable(false);
		}
	}

	tree->SetPivot(25,80);
	tree->Animate(0,0,1,0,8);

	HOScene->GetObjectByID("swingrope")->Animate(2, 0, 0, 0, 7);
	HOScene->GetObjectByID("swingplank")->Animate(1, 0, 0, 0, 7);

	plant5->SetPivot(-36,88);
	plant5->Animate(0,0,3,0,8);

	plant1->SetPivot(-36,88);
	plant1->Animate(0,0,3,0,10);

	plant2->SetPivot(-36,88);
	plant2->Animate(0,0,3,0,4);

	plant3->SetPivot(-36,88);
	plant3->Animate(0,0,3,0,8);

	plant4->SetPivot(-36,88);
	plant4->Animate(0,0,3,0,6);

	plant6->SetPivot(-36,88);
	plant6->Animate(0,0,3,0,10);

	plant7->SetPivot(-36,88);
	plant7->Animate(0,0,3,0,7);

	branch1->SetPivot(-242,-93);
	branch1->Animate(0,0,3,0,7);

	branch2->SetPivot(-36,88);
	branch2->Animate(0,0,3,0,7);

	branch4->SetPivot(-36,88);
	branch4->Animate(0,0,3,0,4);

	if(ProfileSystem::Instance->ActiveProfile.L1S2_P3IsSwingRopeCut)
	{
		HOScene->GetObjectByID("swingplank")->SetEnable(false);
	}

	//Localized
	CObject* temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L1\\S2\\LOVEBUILD.SPR", 275, 380, 100);
	temp->SetUID("love");
	HOScene->PushObject(temp);

	InitPopups();
	
	int ArrayPts1[] = {625,329,698,111,761,123,691,333};
	p1PolyWood =  new CPolygon(4);
	p1PolyWood->Init(ArrayPts1);

	int ArrayPts2[] = {491,448,486,416,562,381,647,427,645,464,590,491,574,487,553,493,534,487};
	p1PolyDieselCan =  new CPolygon(9);
	p1PolyDieselCan->Init(ArrayPts2);


	if( GFInstance->availablePhyMem >= MAX_LOWEND_PEAKMEMORY )
	{
	}

	
	GFInstance->PostUpdate(2);	

	zOrderScnVideo = 20;
	zOrderPopVideo = eZOrder_Popup + 10;

	isVidFireLoaded = false;

	//Add Visible = true; to see anything
	SetZ(zOrderScnVideo);

	lpVid_fire = NULL;

	if(ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite)
	{
		lpVid_fire = new KVideo;
		bool bSuccess = lpVid_fire->openVideo (FileSystem::charPath("OGV\\L1S2\\FIRE.OGV"),true,NULL,NULL,true);
		if (!bSuccess) 
		{
			KMiscTools::messageBox ("Video loading error!", "Couldn't open Fire.OGV");
			exit(0);
		}
		isVidFireLoaded = true;
	}

	//MUSIC_SFX
	Control::Audio->ClearUnusedTracks();

	//After Escape From Tunnel

	if(ProfileSystem::Instance->ActiveProfile.IsStartMusic)
	{
		ProfileSystem::Instance->ActiveProfile.IsStartMusic = false;
		if(ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite)
		{
			if( !Control::Audio->IsPlaying(aTrackL1EscapePlan) )
				Control::Audio->PlaySample(aTrackL1EscapePlan,false);
		}
		else if( ProfileSystem::Instance->ActiveProfile.L1S4_IsCreekOutroPlayed &&
			!ProfileSystem::Instance->ActiveProfile.L1S4_MayaPushMaryVideoPlayed )
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

	if(IsMansionLit)
	{
		Control::Audio->LoadSample(aAmbS2FrontyardLit,AudioAmb);
		Control::Audio->PlaySample(aAmbS2FrontyardLit,true);
	}
	else
	{
		Control::Audio->LoadSample(aAmbS2FrontyardUnlit,AudioAmb);
		Control::Audio->PlaySample(aAmbS2FrontyardUnlit,true);
	}

	Control::Audio->LoadSample(aVOL1MaryRefS2_1,AudioVO);
	Control::Audio->LoadSample(aVOL1MaryRefS2_2,AudioVO);
	//MUSIC_SFX

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L1_S2_Frontyard]  = true;

	if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S2_Frontyard])
		ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S2_Frontyard] = true;

	

	
		

#ifdef _CEBUILD
	//Morph Object Add
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S2Collected)
	{
		if(IsMansionLit)
			Hud->setMorphObjects(HOScene->GetObjectByID("Morph2"), HOScene->GetObjectByID("Morph1"));
		else
			Hud->setMorphObjects(HOScene->GetObjectByID("Morph2"), HOScene->GetObjectByID("Morph1"));
		CRectangle  MorphRect(475.0f,395.0f,80.0f,51.0f);
		Hud->setMorphObjRect(MorphRect);
	}
	else
	{
		if(IsMansionLit)
			HOScene->GetObjectByID("Morph2")->SetEnable(false);
		else
			HOScene->GetObjectByID("Morph2")->SetEnable(false);
	}
#else
	if(IsMansionLit)
		HOScene->GetObjectByID("Morph2")->SetEnable(false);
	else
		HOScene->GetObjectByID("Morph2")->SetEnable(false);
#endif 
	
	Visible = true;

	//Tutorial show
	if(Hud->Tutor != NULL && Hud->Tutor->IsActive && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly)
	{
		if(!ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT13_HintTutorial])
			Hud->Tutor->ShowTutorial(TUT13_HintTutorial);
		else if(!ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT14_MapTutorial])
			Hud->Tutor->ShowTutorial(TUT14_MapTutorial);
	}

	TaskPendingUpdate();
}

void L1_S2_Frontyard::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;
	if(!ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved )
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L1S2_IsMonkeyTookBanana && ProfileSystem::Instance->ActiveProfile.L1S3_IsBananaTaken )
		isAnyTaskLeft = true;
	else 
		{
			if( !ProfileSystem::Instance->ActiveProfile.L1S2_InvDieselCanTaken && ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCrowbarPicked && ProfileSystem::Instance->ActiveProfile.L1S3_IsBananaTaken )
				isAnyTaskLeft = true;
			else if( !ProfileSystem::Instance->ActiveProfile.L1S2_InvHoseTaken && ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCrowbarPicked && ProfileSystem::Instance->ActiveProfile.L1S3_IsBananaTaken )
				isAnyTaskLeft = true;
			if( !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth3CarDickyTaken && ProfileSystem::Instance->ActiveProfile.L1S3_IsBananaTaken )
				isAnyTaskLeft = true;
		}
	if(!ProfileSystem::Instance->ActiveProfile.L1S2_IsInvPlank1Taken && ProfileSystem::Instance->ActiveProfile.L1S1_IsInvKnifeTaken)
				isAnyTaskLeft = true;

	if(!ProfileSystem::Instance->ActiveProfile.L1S2_IsInvTreeBranchTaken && ProfileSystem::Instance->ActiveProfile.L1S3_IsBananaTaken)
				isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S2_Frontyard] = isAnyTaskLeft;

	if(ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved )
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby1Branch2Taken && ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby2Branch3Taken &&ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby3UnderLeaf1Taken && ProfileSystem::Instance->ActiveProfile.L1S1_P1Ruby4UnderLeavesTaken && !ProfileSystem::Instance->ActiveProfile.L1S3_IsBedroomDoorOpened)
			ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S3_Foyer] = true;

		if(!ProfileSystem::Instance->ActiveProfile.L1S3_IsP3ShadowHandFallen && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvTreeBranchTaken)
			ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S3_Foyer] = true;
		
		if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S7_BoatDock]) 
			ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S7_BoatDock] = true;
	}
	if(ProfileSystem::Instance->ActiveProfile.L1S7_IsStepsPlaced)
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S2_InvDieselCanTaken && !ProfileSystem::Instance->ActiveProfile.L1S6_IsCanPlaced )
			ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S6_BoatShed] = true;
		if(ProfileSystem::Instance->ActiveProfile.L1S2_InvHoseTaken && !ProfileSystem::Instance->ActiveProfile.L1S6_IsHosePlaced )
			ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S6_BoatShed] = true;	
	}
	else if(ProfileSystem::Instance->ActiveProfile.L1S1_IsInvPlank3Taken && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvPlank1Taken && ProfileSystem::Instance->ActiveProfile.L1S7_IsInvPlank2Taken )
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S7_BoatDock] = true;

}

void L1_S2_Frontyard::InitPopups()
{
	ActivePopupID = ePopInactive;

	popupCloseDelayTimer = 0.0f;
	IsPopupTransitionActive = false;

	Pop_CloseButton = NULL;

	Popup1CarDickie = NULL;
	isCarDickeyAnimPlaying = false;

	//for popup3 swing rain particle
	P3RainVFX = NULL;
	P3RaindropsVFX = NULL;

	CAnim_CarDickieOpen = NULL;

	if ( (!ProfileSystem::Instance->ActiveProfile.L1S2_InvDieselCanTaken || !ProfileSystem::Instance->ActiveProfile.L1S2_InvHoseTaken ) )
	{
		Popup1CarDickie = new CHOScene();
		Popup1CarDickie->Load("LV1\\S2\\POP\\POP1.SCN");

		//Add popup frame and close button
		CObject *popFrame = new CObject();
		popFrame->LoadInit("UI\\poprectframe.SPR", 13, -15, eZOrder_PopFrame);
		popFrame->SetUID("popFrame");
		Popup1CarDickie->PushObject(popFrame); //in Normal Cases

		CObject *popCloseBtn = new CObject();
		popCloseBtn->LoadInit("UI\\closebtn.SPR", 321, -246, eZOrder_PopCloseBtn);
		popCloseBtn->SetUID("popClose");
		Popup1CarDickie->PushObject(popCloseBtn);

		
		p1DieselCan	= Popup1CarDickie->GetObjectByID("p1DieselCan");
		p1Hose 	= Popup1CarDickie->GetObjectByID("p1Hose");
		p1ClosedDickie = Popup1CarDickie->GetObjectByID("p1ClosedDickie");
		p1OpenedDickie	= Popup1CarDickie->GetObjectByID("p1OpenedDickie");
		p1Sack = Popup1CarDickie->GetObjectByID("p1Sack");
		p1SpiderNet1 = Popup1CarDickie->GetObjectByID("p1SpiderNet1");
		p1SpiderNet2 = Popup1CarDickie->GetObjectByID("p1SpiderNet2");
		//p1SpiderNet3 = Popup1CarDickie->GetObjectByID("p1SpiderNet3");
		p1Wood2	= Popup1CarDickie->GetObjectByID("p1Wood2");
		p1Crowbar = Popup1CarDickie->GetObjectByID("p1Crowbar");
		p1Crowbar->SetZ(102);//above popup frame
		p1ClosedDickie->SetZ(104);
		p1Crowbar->SetEnable(false);

		CObject* objArr[] = {p1Crowbar, p1ClosedDickie, p1OpenedDickie};
		std::string animArr[] = {"ANIM\\L1\\s2crowbar.anim", "ANIM\\L1\\s2closedickie.anim", "ANIM\\L1\\s2opendickie.anim"};
		CAnim_CarDickieOpen = new CustomAnimGroup(3, animArr, objArr);



		if(!ProfileSystem::Instance->ActiveProfile.L1S2_IsCarDickeyOpen)
		{
			p1OpenedDickie->SetEnable(false);
		}
		else
		{
			p1ClosedDickie->SetEnable(false);
			
			if(ProfileSystem::Instance->ActiveProfile.L1S2_P1_IsSackMoved)
			{
				p1Sack->SetEnable(false);
			}
			if(ProfileSystem::Instance->ActiveProfile.L1S2_P1_IsWoodMoved)
			{
				p1Wood2->SetEnable(false);
			}

			if(ProfileSystem::Instance->ActiveProfile.L1S2_InvDieselCanTaken)
			{
				p1DieselCan->SetEnable(false);
			}
			if(ProfileSystem::Instance->ActiveProfile.L1S2_InvHoseTaken)
			{
				p1Hose->SetEnable(false);
			}
		}


		Popup1CarDickie->Show(false);
		Popup1CarDickie->SetPosOff(CenterPoint.x,CenterPoint.y);
		Popup1CarDickie->SetZOff(eZOrder_Popup);
	}
	Popup1_State = POPUP_CLOSED;

	Popup2Love = NULL;

	Popup2Love = new CHOScene();
	Popup2Love->Load("LV1\\S2\\POP\\POP2.SCN");

	//Localized
	CObject* temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\L1\\S2\\LOVEBUILD_POP.SPR", 0, 0, 5);
	temp->SetUID("LOVEBUILD_POP");
	Popup2Love->PushObject(temp);

	Popup2Love->Show(false);
	Popup2Love->SetPosOff(CenterPoint.x,CenterPoint.y);
	Popup2Love->SetZOff(eZOrder_Popup);
	if( ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth2LovePopTaken  )
	{
		Popup2Love->GetObjectByID("p2cloth")->SetEnable(false);
	}

	Popup2Love->GetObjectByID("paptus")->SetBlendMode(eBlend_AlphaAdditive);
	if(ProfileSystem::Instance->ActiveProfile.IsPaptusL1S2Collected)
	{
		Popup2Love->GetObjectByID("paptus")->SetEnable(false);
	}

	Popup2_State = POPUP_CLOSED;

	CAnim_RopeCut = NULL;
	CAnim_TreeBranchFix = NULL;
	isTreeBranchFixAnimPlaying = false;
	isRopeCutPlaying = false;
	Popup3Swing = NULL;

	if ( !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvTreeBranchTaken )
	{
		Popup3Swing = new CHOScene();
		Popup3Swing->Load("LV1\\S2\\POP\\POP3.SCN");


		//Add popup frame and close button
		CObject *popFrame = new CObject();
		popFrame->LoadInit("UI\\poprectframe.SPR", 13, -15, eZOrder_PopFrame);
		popFrame->SetUID("popFrame");
		Popup3Swing->PushObject(popFrame); //in Normal Cases


		CObject *popCloseBtn = new CObject();
		popCloseBtn->LoadInit("UI\\closebtn.SPR", 321, -246, eZOrder_PopCloseBtn);
		popCloseBtn->SetUID("popClose");
		Popup3Swing->PushObject(popCloseBtn);
		Popup3Swing->GetObjectByID("P3stickPatched1")->SetEnable(false);
		Popup3Swing->GetObjectByID("P3stickPatched2")->SetEnable(false);

		P3RainVFX = new SuperFX("PRT\\L1\\poprain.INI", PointSystem::CreateCPoint(722,225,0), eZOrder_Popup+60);
		P3RaindropsVFX = new SuperFX("PRT\\L1\\pops1drops.INI", PointSystem::CreateCPoint(683,430,0), eZOrder_Popup+50); 
		
		Popup3Swing->GetObjectByID("P3clothStraight1")->SetEnable(false);
		Popup3Swing->GetObjectByID("P3clothStraight2")->SetEnable(false);
		Popup3Swing->GetObjectByID("P3clothStraight3")->SetEnable(false);

		Popup3Swing->GetObjectByID("P3swissknife")->SetEnable(false);

		CObject* objArr[] = {Popup3Swing->GetObjectByID("P3ropeCut"),Popup3Swing->GetObjectByID("P3ropeUncut"), Popup3Swing->GetObjectByID("P3plankOnOneRope"), Popup3Swing->GetObjectByID("P3swissknife"), Popup3Swing->GetObjectByID("P3plankOnGround")};
		std::string animArr[] = {"ANIM\\L1\\S2RopeCut_RopeCut.anim", "ANIM\\L1\\S2RopeCut_RopeUncut.anim", "ANIM\\L1\\S2RopeCut_PlankOnRope.anim", "ANIM\\L1\\S2RopeCut_Knife.anim", "ANIM\\L1\\S2RopeCut_PlankGround.anim"};
		CAnim_RopeCut = new CustomAnimGroup(5, animArr, objArr);

		isRopeCutPlaying = false;
		isTreeBranchFixAnimPlaying = false;

		CObject* TreeBranchObjArr[] = {Popup3Swing->GetObjectByID("P3stickBroken"),Popup3Swing->GetObjectByID("P3clothStraight1"),Popup3Swing->GetObjectByID("P3clothStraight2"),Popup3Swing->GetObjectByID("P3clothStraight3"), Popup3Swing->GetObjectByID("P3stickPatched1"), Popup3Swing->GetObjectByID("P3stickPatched3")};
		std::string TreeBranchAnimArr[] = {"ANIM\\L1\\S2BranchFix_Broken.anim", "ANIM\\L1\\S2BranchFix_Cloth1.anim", "ANIM\\L1\\S2BranchFix_Cloth2.anim", "ANIM\\L1\\S2BranchFix_Cloth3.anim", "ANIM\\L1\\S2BranchFix_Patched1.anim", "ANIM\\L1\\S2BranchFix_Patched3.anim"};
		CAnim_TreeBranchFix = new CustomAnimGroup(6, TreeBranchAnimArr, TreeBranchObjArr);

		if(ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth1SwingTaken)
		{
			Popup3Swing->GetObjectByID("P3clothPiece")->SetEnable(false);
		}
		
		if(ProfileSystem::Instance->ActiveProfile.L1S2_P3IsSwingRopeCut )
		{
			//Rope cut
			Popup3Swing->GetObjectByID("P3ropeCut")->SetEnable(true);
			Popup3Swing->GetObjectByID("P3plankOnOneRope")->SetEnable(false);
			if( ProfileSystem::Instance->ActiveProfile.L1S2_IsInvPlank1Taken )
			{
				Popup3Swing->GetObjectByID("P3plankOnGround")->SetEnable(false);
				if( !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvTreeBranchTaken )
				{
					if( !ProfileSystem::Instance->ActiveProfile.L1S2_P3IsTreeBranchFixed )
					{
						Popup3Swing->GetObjectByID("P3stickPatched3")->SetEnable(false);
					}
					else
					{
						Popup3Swing->GetObjectByID("P3stickBroken")->SetEnable(false);
					}
				}
			}
			else
			{
				Popup3Swing->GetObjectByID("P3stickPatched3")->SetEnable(false);
			}
			Popup3Swing->GetObjectByID("P3ropeUncut")->SetEnable(false);
		}
		else
		{
			//Rope Not cut
			Popup3Swing->GetObjectByID("P3plankOnGround")->SetEnable(false);
			Popup3Swing->GetObjectByID("P3ropeCut")->SetEnable(false);
			Popup3Swing->GetObjectByID("P3stickPatched3")->SetEnable(false);
		}


		Popup3Swing->Show(false);
		Popup3Swing->SetPosOff(CenterPoint.x,CenterPoint.y);
		Popup3Swing->SetZOff(eZOrder_Popup);
	}
	Popup3_State = POPUP_CLOSED;

	PopupAlpha = 0.0f;
	PopVal = 0.0f;
}

void L1_S2_Frontyard::Required()
{
	
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( IsPopupTransitionActive )
		UpdatePopupTransition();
}

void L1_S2_Frontyard::Update()
{
	NavigateToScene();
	if(GFInstance->IsSceneGraphicReloaded)
	{
		UpdateBgGraphic();
		GFInstance->IsSceneGraphicReloaded = false;
	}

	if( Hud->PopupState != Popup_Inactive || Hud->IsMapShowing || Hud->IsDiaryShowing)
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	flyingInsects->Update();

	CPoint pos = Control::Input->Mpos();

	/*WolfHowlTimer += Control::LogicRate;
	if(WolfHowlTimer>WolfHowlTimeThreshold)
	{
		Control::Audio->QuickLoadAndPlaySFX(aSFXL1S2WolfHowl);
		WolfHowlTimer = 0;
		WolfHowlTimeThreshold = 18.0f + (float)(rand()%12);
	}*/

	if(!IsMansionLit)
	{
		//Fuse Puzzle Sparks
		FuseSparkTimer += Control::LogicRate;
		if( FuseSparkTimer>FuseSparkThreshold )
		{
			FuseSparkTimer = 0;
			FuseSparkThreshold = 2.0f + rand()%3;

			Scn_Sparks->ForcePlayAnimation(0);
			Scn_Sparks->Paused = false;
			Scn_FuseBgGlow->ForcePlayAnimation(0);
			Scn_FuseBgGlow->Paused = false;
			
			int dispX = rand()%10;
			int dispY = 10-rand()%10;

			Scn_Sparks->Pos.x = Scn_Sparks->InitPos.x + dispX;
			Scn_Sparks->Pos.y = Scn_Sparks->InitPos.y + dispY;			
		}
	}

	if(Hud->Tutor != NULL && Hud->Tutor->IsActive)
	{
		if(Hud->Tutor->IsVisible && (Hud->Tutor->State == TUT13_HintTutorial) )
		{
			return;
		}
		else if(Hud->Tutor->IsVisible && (Hud->Tutor->State == TUT14_MapTutorial) )
		{
			return;
		}
	}

#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
#else
	Cursor::SetMode(CA_Normal);
#endif 

	

	if(isBananaThrowAnimPlaying)
	{
		if(BananaThrowAnim->IsAnimationFinished())
		{
			isBananaThrowAnimPlaying = false;
			BananaThrowAnim->SetEnable(false);
			baboonRunPointer = 0;
			BaboonNormalAnim->SetEnable(false);
			BaboonAttackAnim->SetEnable(false);
			BaboonHungryAnim->SetEnable(false);
			BaboonRunAnim[baboonRunPointer]->SetEnable(true);
			BaboonRunAnim[baboonRunPointer]->ForcePlayAnimation(0);
			isBaboonRunAnimPlaying = true;
		}
		return;
	}

	if(isBaboonRunAnimPlaying)
	{
		if(BaboonRunAnim[baboonRunPointer]->IsAnimationFinished())
		{
			if(baboonRunPointer == 1)
			{
				baboonRunPointer = 0;
				BaboonNormalAnim->SetEnable(false);
				BaboonAttackAnim->SetEnable(false);
				BaboonHungryAnim->SetEnable(false);
				isBaboonRunAnimPlaying = false;
				BaboonRunAnim[baboonRunPointer]->SetEnable(false);				
			}
			else
			{
				baboonRunPointer++;
				BaboonRunAnim[baboonRunPointer - 1]->SetEnable(false);
				BaboonRunAnim[baboonRunPointer]->SetEnable(true);
				BaboonRunAnim[baboonRunPointer]->ForcePlayAnimation(0);
			}
		}
		return;
	}
	

	if (!ProfileSystem::Instance->ActiveProfile.L1S2_IsMonkeyTookBanana)
	{

		if( isBaboonHungryAnimPlaying )
		{
			if( BaboonHungryAnim->IsAnimationFinished() )
			{
			//	K_LOG("Hungry Anim End");
				BaboonAnimTimer = 0;
				BaboonAnimThresholdMultiplier = 2.0f + std::rand()%3;
				isBaboonHungryAnimPlaying = false;
				BaboonHungryAnim->SetEnable(false);
				BaboonNormalAnim->SetEnable(true);
				BaboonNormalAnim->ForcePlayAnimation(0);
				return;
			}
		}

		if( !isBaboonHungryAnimPlaying && !isBaboonAttackAnimPlaying && !isBaboonRunAnimPlaying && !isBananaThrowAnimPlaying && ActivePopupID == ePopInactive )
		{
			if( BaboonAnimTimer >= (BaboonAnimThreshold * BaboonAnimThresholdMultiplier) )
			{
			//	K_LOG("Hungry Anim starts");
				isBaboonHungryAnimPlaying = true;
				BaboonHungryAnim->SetEnable(true);
				BaboonNormalAnim->SetEnable(false);
				BaboonHungryAnim->ForcePlayAnimation(0);
			}
			else
			{
				BaboonAnimTimer += Control::LogicRate * 1.0f;
			}
		} 
	}


	if( ActivePopupID == ePopInactive )
	{
		if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
			SparkleUpdate();

		if(!ProfileSystem::Instance->ActiveProfile.L1S2_IsMonkeyTookBanana)
		{
			if(isBaboonAttackAnimPlaying)
			{
				if(BaboonAttackAnim->IsAnimationFinished())
				{
					BaboonAnimTimer = 0;
					BaboonAnimThresholdMultiplier = 2.0f + std::rand()%3;
					isBaboonAttackAnimPlaying = false;
					BaboonAttackAnim->SetEnable(false);
					BaboonNormalAnim->SetEnable(true);
					BaboonHungryAnim->ForcePlayAnimation(0);
				}
			}			
		}
		else if( !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth3CarDickyTaken)
		{
			if(InvItemRectClothP1CarDicky.Intersection( &pos))
			{
				Cursor::SetMode(CA_HandTake);
				if(Control::Input->LBclicked())
				{
					ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth3CarDickyTaken = true;
					clothCarDicky->SetEnable(false);
							
					TakeInventoryItem(eInv_L1S2_Cloth3CarDicky);	
					return;
				}

			}
		}


		lightningTimer += Control::LogicRate;
		if(lightningTimer>nextLightningThreshold)
		{
			Scn_Lightning->Show(true);
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
				Scn_Lightning->Show(false);
				nextLightningThreshold = 12.0f + (float)(rand()%8);
			}
			Scn_Lightning->SetAlpha(lightAlphaArray[index]);
		}
		else
		{
			if(Scn_Lightning->Visible)
				Scn_Lightning->Show(false);
		}

		scn_cloudBig->Pos.x -= Control::LogicRate*5.0f;
		scn_cloudSmall->Pos.x -= Control::LogicRate*7.5f;		

		if(scn_cloudBig->Pos.x<200.0f)
		{
			scn_cloudBig->Pos.x = 956.0f + rand()%100;
		}

		if(scn_cloudSmall->Pos.x<158.0f)
		{
			scn_cloudSmall->Pos.x = 865.0f + rand()%100;
		}

		if(SceneRectBaboon.Intersection(&pos) && isBaboonAttackAnimPlaying == false && !ProfileSystem::Instance->ActiveProfile.L1S2_IsMonkeyTookBanana)
		{
			Cursor::SetMode(CA_Gear);
			if(Control::Input->LBclicked())
			{
				BaboonAttackAnim->SetEnable(true);
				BaboonNormalAnim->SetEnable(false);
				BaboonHungryAnim->SetEnable(false);
				BaboonAttackAnim->ForcePlayAnimation(0);
				isBaboonAttackAnimPlaying = true;
				Hud->ShowBannerText("L1baboon");
				Control::Audio->QuickLoadAndPlaySFX(aSFXL1S2MonkeyClicked);
				return;
			}
		}

		if( !ProfileSystem::Instance->ActiveProfile.L1_IsMansionPowerOn && SceneRectFuzePuzzle.Intersection(&pos))
		{
			#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)			
				//Cursor::SetMode(CA_Look);
			#else				
				Cursor::SetMode(CA_Look);
			#endif
			if(Control::Input->LBclicked())
			{
			#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L1_S2_PZFuse, 0, SceneRectFuzePuzzle);
				}
			#else
				//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S2_PZFuse;
				SendMessage(ControlExit, ID, HOSM_NewArea);		
			#endif
					
			}
			return;
		}

		if( SceneRectFoyer.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite)
		{
			#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)	
				
			//Cursor::SetMode(CA_ExitRight,eArea_L1_S3_Foyer);
			#else
				
			Cursor::SetMode(CA_ExitRight,eArea_L1_S3_Foyer);
			#endif
			if( Control::Input->LBclicked() )
			{
			#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L1_S3_Foyer, 1, SceneRectFoyer);
				}
			#else
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S3_Foyer;
				SendMessage(ControlExit, ID, HOSM_NewArea);
			#endif
										
			}
			return;
		}
		else if( SceneRectBoatdock.Intersection(&pos) )
		{
			if(!ProfileSystem::Instance->ActiveProfile.L1_IsMansionPowerOn)
			{
				Cursor::SetMode(CA_Help);
				if( Control::Input->LBclicked() )
				{
					Hud->ShowBannerText("L1dark");
				}
			}
			else
			{
				#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)				
				//Cursor::SetMode(CA_ExitLeft,eArea_L1_S7_BoatDock);
			#else
				Cursor::SetMode(CA_ExitLeft,eArea_L1_S7_BoatDock);
			#endif
				
				if( Control::Input->LBclicked() )
				{
					#ifdef TOUCH_UI
					if(!GFHud->InvBoxCtrl->navTapped)
					{
						GFHud->InvBoxCtrl->GoToScene(eArea_L1_S7_BoatDock, 3, SceneRectBoatdock);
					}
					#else
						//Control::Audio->PlayFootStepsFX();
						ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S7_BoatDock;
						SendMessage(ControlExit, ID, HOSM_NewArea);
					#endif
				} 
			}
			return;		
		}
		else if( InfoRectMansion.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite)
		{
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L1vines");
			}
		}
		else if( InfoRectMansion.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite)
		{
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L1burn");
				Control::Audio->PlaySample(aVOL1MaryRefS2_2);
			}
		}
		else if( InfoRectBeetleTree.Intersection(&pos) )
		{
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L1beetle");
			}
		}
		else if( InfoRectMoon.Intersection(&pos) )
		{
			if( Control::Input->LBclicked() )
			{
				if( !ProfileSystem::Instance->ActiveProfile.L1_IsMansionPowerOn )
					Hud->ShowBannerText("L1full");//clicking birds!!
				else
					Hud->ShowBannerText("L1moon");
			}
		}
		else if(InfoRectGards.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite)
		{
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L1eagle");
				if( !Control::Audio->IsPlaying(aSFXJrnlFlyer) )				
					Control::Audio->QuickLoadAndPlaySFX(aSFXJrnlFlyer);
				Control::Audio->PlaySample(aVOL1MaryRefS2_1);
			}
		}
	}


	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed() && ActivePopupID == ePopInactive)
		{
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S1_Beach;
			SendMessage(ControlExit, ID, HOSM_NewArea);								
			return;
		}
	#endif

	if( ActivePopupID == ePopInactive && IsMouseOverGoBackArea(&pos ))
	{
#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
			//Cursor::SetMode(CA_ExitDown,eArea_L1_S1_Beach);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L1_S1_Beach);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L1_S1_Beach, 2);
			}
#else
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S1_Beach;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}			
		return;
	}

	if( !IsPopupTransitionActive )
		UpdatePopups(pos);
}

float L1_S2_Frontyard::easingQuadOut(float t,float b,float c,float d)
{
	return (c * -t*(t-2) / d + b);
}

void L1_S2_Frontyard::UpdatePopupTransition()
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

	//Popup1 Eagle
	if( ActivePopupID == ePopL1S2_P1CarDickie )
	{
		switch( Popup1_State )
		{
		case POPUP_SCALEUP:
			PopVal += POPUP_SCALE_FACTOR * Control::LogicRate;
			PopupAlpha = easingQuadOut(PopVal,0,1,1.0f);

			if( PopVal >= 1.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 1.0f;				
				Popup1CarDickie->Show(true);
				Popup1_State = POPUP_OPEN;
			}			
			GFInstance->UpdateBlurScene(PopupAlpha);
			break;
		case POPUP_SCALEDOWN:
			popupCloseDelayTimer -= 1.0f * Control::LogicRate;
			if( popupCloseDelayTimer > 0.0f ){
				return;	}
			else{
				popupCloseDelayTimer = 0.0;	}

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

	if( ActivePopupID == ePopL1S2_P2Love )
	{
		switch( Popup2_State )
		{
		case POPUP_SCALEUP:
			PopVal += POPUP_SCALE_FACTOR * Control::LogicRate;
			PopupAlpha = easingQuadOut(PopVal,0,1,1.0f);

			if( PopVal >= 1.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 1.0f;				
				Popup2Love->Show(true);
				Popup2_State = POPUP_OPEN;
			}			
			GFInstance->UpdateBlurScene(PopupAlpha);

			break;

		case POPUP_SCALEDOWN:
			PopupAlpha -= POPUP_SCALEDOWN_FACTOR * Control::LogicRate;
			if( PopupAlpha <= 0.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 0.0f;								
				GFInstance->TurnOffBlurScene();
				Popup2_State = POPUP_CLOSED;
				ActivePopupID = ePopInactive;
				Popup2Love->Show(false);

				HOScene->PauseObjects(false);

				#ifdef _CEBUILD
				Hud->ShowMorphObjects();
				#endif 
				
				EndBgGraphicScaling();
				
				SetZ(zOrderScnVideo);
				if(!ProfileSystem::Instance->ActiveProfile.L1S2_IsMonkeyTookBanana)
				{
					if( isBaboonHungryAnimPlaying )
					{
						BaboonHungryAnim->SetEnable(true);
					}
					else
					{
						BaboonNormalAnim->SetEnable(true);
					}
				}

				//Journal Entry 5 Add
				if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P5_CursedMansion])
				{
					//Flag Set
					ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L1P5_CursedMansion] = true;
					//Add to Note Array
					ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected]
						= eJrn_L1P5_CursedMansion;
						
					ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;
					//Increment Notes
					ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
					Hud->ShowJrnEntryFX();
				}
			}
			else
				GFInstance->UpdateBlurScene(PopupAlpha);
			break;
		}
	}

	//Popup 3 Swing

	if( ActivePopupID == ePopL1S2_P3Swing )
	{
		switch( Popup3_State )
		{
		case POPUP_SCALEUP:
			PopVal += POPUP_SCALE_FACTOR * Control::LogicRate;
			PopupAlpha = easingQuadOut(PopVal,0,1,1.0f);

			if( PopVal >= 1.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 1.0f;				
				Popup3Swing->Show(true);
				Popup3_State = POPUP_OPEN;

				P3RainVFX->Start();
				P3RainVFX->AdvanceByTime(2.0f);
				P3RaindropsVFX->Start();
				P3RaindropsVFX->AdvanceByTime(2.0f);
			}			
			GFInstance->UpdateBlurScene(PopupAlpha);

			break;

		case POPUP_SCALEDOWN:
			popupCloseDelayTimer -= 1.0f * Control::LogicRate;
			if( popupCloseDelayTimer > 0.0f ){
				return;	}
			else{
				popupCloseDelayTimer = 0.0;	}
			PopupAlpha -= POPUP_SCALEDOWN_FACTOR * Control::LogicRate;
			if( PopupAlpha <= 0.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 0.0f;								
				GFInstance->TurnOffBlurScene();
				Popup3_State = POPUP_CLOSED;
				ActivePopupID = ePopInactive;
				Popup3Swing->Show(false);

				#ifdef _CEBUILD
				Hud->ShowMorphObjects();
				#endif 

				HOScene->PauseObjects(false);
				
				EndBgGraphicScaling();
			}
			else
				GFInstance->UpdateBlurScene(PopupAlpha);
			break;
		}
	}

}

void L1_S2_Frontyard::UpdatePopups(CPoint pos,bool ItemDropped)
{
	bool clicked = Control::Input->LBclicked();
	//Popup1 CarDicky
	if( (ActivePopupID==ePopInactive || ActivePopupID == ePopL1S2_P1CarDickie)  && IsMansionLit && ProfileSystem::Instance->ActiveProfile.L1S2_IsMonkeyTookBanana)
	{
		switch( Popup1_State )
		{
		case POPUP_CLOSED:
			if( PopRectCarDickie.Intersection(&pos) && (!ProfileSystem::Instance->ActiveProfile.L1S2_InvDieselCanTaken || !ProfileSystem::Instance->ActiveProfile.L1S2_InvHoseTaken ))
			{
				Cursor::SetMode(CA_Look);
				if( clicked || ItemDropped)
				{					
					Popup1_State = POPUP_SCALEUP;
					ActivePopupID = ePopL1S2_P1CarDickie;
					Control::Audio->PlaySample(aSFXpopup);

					Pop_CloseButton = Popup1CarDickie->GetObjectByID("popClose");
					Pop_CloseButton->PlayAnimation(0);

					UpdateBgGraphic();

					PopVal = 0;
					PopupAlpha = 0.0f;
					SetupPopupTransition(Popup1CarDickie, PopRectCarDickie);					
				}
				return;
			}
			break;

		case POPUP_OPEN:

			if(isCarDickeyAnimPlaying)
			{
				if(CAnim_CarDickieOpen->IsAnimFinished())
				{
					isCarDickeyAnimPlaying = false;
					p1Crowbar->SetEnable(false);
					p1ClosedDickie->SetEnable(false);
					p1OpenedDickie->SetEnable(true);
					openedCarDickyDoor->SetEnable(true);
					carDickyOpened->SetEnable(true);

					DieselCan->SetEnable(true);
					hose->SetEnable(true);
					sack->SetEnable(true);
					wood->SetEnable(true);
					walkieTalkie->SetEnable(true);
					rope->SetEnable(true);
					bottle->SetEnable(true);
					shoe->SetEnable(true);
					UpdateBgGraphic();
				}
				return;
			}
			#ifdef ENABLE_ANDROID_BACK_BUTTON
			if (Control::Input->isKeyBackSensed()) {
				ClosePopup();
			}
			#endif
			if( !PopAreaRect.Intersection(&pos) || PopCloseButtonRect.Intersection(&pos) )//clicked outside or close Btn
			{
				Pop_CloseButton->PlayAnimation(1);
				if(PopCloseButtonRect.Intersection(&pos))
					Cursor::SetMode(CA_HandPointy);
				if( clicked )
				{
					Pop_CloseButton->PlayAnimation(0);
					//Control::Audio->PlaySample(aSFXpopdown);
					ClosePopup();
				}
				return;
			}
			else
			{
				Pop_CloseButton->PlayAnimation(0);
				
				if(!ProfileSystem::Instance->ActiveProfile.L1S2_IsCarDickeyOpen)
				{
					if (InvDestRectCrowbarDicky.Intersection(&pos))
					{
						Cursor::SetMode(CA_Gear);
						if(clicked)
						{
							p1ClosedDickie->Shake(50,4,0,0.25f);
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5PlankMoved);
							Hud->ShowBannerText("L1stuck",SFX_NIL);
						}
					}
					return;
				}

				if(ProfileSystem::Instance->ActiveProfile.L1S2_InvDieselCanTaken && ProfileSystem::Instance->ActiveProfile.L1S2_InvHoseTaken )
				{
					popupCloseDelayTimer = 1.0f;
					ClosePopup();
					return;
				}

				if(!ProfileSystem::Instance->ActiveProfile.L1S2_P1_IsSackMoved && popRectSack.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S2_IsCarDickeyOpen)
				{
					Cursor::SetMode(CA_HandTake);
					if( clicked )
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S2P1SackRemoved);

						float tweenArray[5] = {0,998,334,1,0};
						p1Sack->TweenTo(tweenArray,1,0,false);
						ProfileSystem::Instance->ActiveProfile.L1S2_P1_IsSackMoved = true;
						Control::Input->ClearMouse();//avoid this click going to bkgnd!!
						sack->SetEnable(false);
						//p1Sack->FadeOut(4.0f);
						UpdateBgGraphic();
						return;
					}
				}

				if(!ProfileSystem::Instance->ActiveProfile.L1S2_P1_IsWoodMoved && p1PolyWood->Intersection(&pos))
				{
					Cursor::SetMode(CA_HandTake);
					if( clicked )
					{
						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S5PlankMoved);
						Control::Input->ClearMouse();//avoid this click going to bkgnd!!
						//p1Wood2->FadeOut(4.0f);
						float tweenArray[5] = {0,998,334,1,0};
						p1Wood2->TweenTo(tweenArray,1,0,false);
						wood->SetEnable(false);
						ProfileSystem::Instance->ActiveProfile.L1S2_P1_IsWoodMoved = true;
						UpdateBgGraphic();
						return;
					}
				}

				if(ProfileSystem::Instance->ActiveProfile.L1S2_P1_IsWoodMoved && popRectHose.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L1S2_InvHoseTaken)
				{
					Cursor::SetMode(CA_HandTake);
					if( clicked )
					{
						p1Hose->SetEnable(false);
						hose->SetEnable(false);
						UpdateBgGraphic();
						ProfileSystem::Instance->ActiveProfile.L1S2_InvHoseTaken = true;
						TakeInventoryItem(eInv_L1S2_Hose);
						return;
					}
				}

				if(ProfileSystem::Instance->ActiveProfile.L1S2_P1_IsSackMoved && p1PolyDieselCan->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L1S2_IsCarDickeyOpen && !ProfileSystem::Instance->ActiveProfile.L1S2_InvDieselCanTaken)
				{
					Cursor::SetMode(CA_HandTake);
					if( clicked )
					{
						TakeInventoryItem(eInv_L1S2_EmptyCan);
						ProfileSystem::Instance->ActiveProfile.L1S2_InvDieselCanTaken = true;
						p1DieselCan->SetEnable(false);
						DieselCan->SetEnable(false);
						UpdateBgGraphic();
						return;
					}
				}
			}
		}
	}

	if( (ActivePopupID==ePopInactive || ActivePopupID == ePopL1S2_P2Love))
	{
		switch( Popup2_State )
		{
		case POPUP_CLOSED:
			if( PopRectLove.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Look);
				if( clicked || ItemDropped)
				{
					Popup2_State = POPUP_SCALEUP;
					ActivePopupID = ePopL1S2_P2Love;

					UpdateBgGraphic();

					Control::Audio->QuickLoadAndPlaySFX(aSFXL1S2LovePopup);
					PopVal = 0;
					PopupAlpha = 0.0f;
					
					SetupPopupTransition(Popup2Love, PopRectLove);
				
					if(ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite)
					{
						if( !Control::Audio->IsPlaying(aTrackL1EscapePlan) )
							Control::Audio->PlaySample(aTrackL1EscapePlan,false);
					}
					else if( ProfileSystem::Instance->ActiveProfile.L1S4_IsCreekOutroPlayed &&
						!ProfileSystem::Instance->ActiveProfile.L1S4_MayaPushMaryVideoPlayed )
					{
						if( !Control::Audio->IsPlaying(aTrackL1Hunted) )
							Control::Audio->PlaySample(aTrackL1Hunted,false);
					}
					else
					{
						if( !Control::Audio->IsPlaying(aTrackL1Broken) &&
							!Control::Audio->IsPlaying(aTrackL1Mansion)  )
							Control::Audio->PlaySample(aTrackL1Mansion,false);
					}
				}
				return;
			}
			break;

		case POPUP_OPEN:
			#ifdef ENABLE_ANDROID_BACK_BUTTON
			if (Control::Input->isKeyBackSensed()) {
				Popup2_State = POPUP_SCALEDOWN;
				Popup2Love->Show(false);
				
				SetupPopupTransition(Popup2Love, PopRectLove);
			}
			#endif
			if(PopLoveFrameRect.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Help);
				if( clicked )
				{
					Hud->ShowBannerText("L1old");
				}
			}
			else
			{
				Cursor::SetMode(CA_HandPointy);
				if( clicked )
				{
					Popup2_State = POPUP_SCALEDOWN;
					Popup2Love->Show(false);
				
					SetupPopupTransition(Popup2Love, PopRectLove);
				}
			}

			if(!ProfileSystem::Instance->ActiveProfile.IsPaptusL1S2Collected && PaptusFlyRect.Intersection(&pos))
			{
				if(clicked)
				{
					Control::Audio->PlaySample(aSFXMagic2);
					Popup2Love->GetObjectByID("paptus")->FadeOut(1.25f);
					Hud->CollectMysteryFlower(eArea_L1_S2_Frontyard, Popup2Love->GetObjectByID("paptus")->Pos);
				}
			}

			if( !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth2LovePopTaken  )
			{
				if(InvItemRectClothP2.Intersection( &pos))
				{
					Cursor::SetMode(CA_HandTake);
					if( clicked )
					{
						ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth2LovePopTaken = true;
						clothLoveSign->SetEnable(false);
						Popup2Love->GetObjectByID("p2cloth")->SetEnable(false);
						TakeInventoryItem(eInv_L1S2_Cloth2LovePop);
						UpdateBgGraphic();
						return;
					}

				}
			}

			break;
		}
	}

	//Popup3 Swing
	if( (ActivePopupID==ePopInactive || ActivePopupID == ePopL1S2_P3Swing) && (!ProfileSystem::Instance->ActiveProfile.L1S2_IsInvTreeBranchTaken || !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth1SwingTaken) )
	{
		switch( Popup3_State )
		{
		case POPUP_CLOSED:
			if( PopRect3Swing.Intersection(&pos) )
			{
				Cursor::SetMode(CA_Look);
				if( clicked || ItemDropped)
				{
					Popup3_State = POPUP_SCALEUP;
					ActivePopupID = ePopL1S2_P3Swing;

					Pop_CloseButton = Popup3Swing->GetObjectByID("popClose");
					Pop_CloseButton->PlayAnimation(0);

					

					UpdateBgGraphic();

					PopVal = 0;
					PopupAlpha = 0.0f;
					
					SetupPopupTransition(Popup3Swing, PopRect3Swing);					
				}
				return;
			}
			break;

		case POPUP_OPEN:
			if( isRopeCutPlaying )
			{
				if( CAnim_RopeCut->IsAnimFinished() )
				{
					isRopeCutPlaying = false;
					CAnim_RopeCut->SetEnableObjects(false);
					Popup3Swing->GetObjectByID("P3plankOnGround")->SetEnable(true);
					Popup3Swing->GetObjectByID("P3ropeCut")->SetEnable(true);
				}
				return;
			}
			if( isTreeBranchFixAnimPlaying )
			{
				if( CAnim_TreeBranchFix->IsAnimFinished() )
				{
					isTreeBranchFixAnimPlaying = false;
					CAnim_TreeBranchFix->SetEnableObjects(false);
					Popup3Swing->GetObjectByID("P3stickPatched3")->SetEnable(true);
				}
				return;
			}
			#ifdef ENABLE_ANDROID_BACK_BUTTON
			if (Control::Input->isKeyBackSensed()) {
				ClosePopup();
			}
			#endif
			if( !PopAreaRect.Intersection(&pos) || PopCloseButtonRect.Intersection(&pos) )//clicked outside or close Btn
			{
				Pop_CloseButton->PlayAnimation(1);
				if(PopCloseButtonRect.Intersection(&pos))
				Cursor::SetMode(CA_HandPointy);
				if( clicked )
				{
					Pop_CloseButton->PlayAnimation(0);
					//Control::Audio->PlaySample(aSFXpopdown);
					ClosePopup();
				}
				return;
			}
			else
			{
				Pop_CloseButton->PlayAnimation(0);
				if( !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth1SwingTaken )
				{
					if( InvItemRect_Cloth1Swing.Intersection( &pos) )
					{
						Cursor::SetMode(CA_HandTake);
						if( clicked )
						{
							ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth1SwingTaken = true;
							Popup3Swing->GetObjectByID("P3clothPiece")->SetEnable(false);
							TakeInventoryItem(eInv_L1S2_Cloth1Swing);

							if(ProfileSystem::Instance->ActiveProfile.L1S2_IsInvTreeBranchTaken)
							{
								popupCloseDelayTimer = 1.0f;
								ClosePopup();
							}
							return;
						}

					}
				}
				if( !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvPlank1Taken )
				{
					if( InvItemRectPlank.Intersection( &pos) )
					{
						if( ProfileSystem::Instance->ActiveProfile.L1S2_P3IsSwingRopeCut )
						{
							Cursor::SetMode(CA_HandTake);
							if( clicked )
							{
								ProfileSystem::Instance->ActiveProfile.L1S2_IsInvPlank1Taken = true;
								Popup3Swing->GetObjectByID("P3plankOnGround")->SetEnable(false);
								TakeInventoryItem(eInv_L1S2_Plank1);
								return;
							}
						}
						else
						{
							Cursor::SetMode(CA_Gear);
							if( clicked )
								Hud->ShowBannerText("L1plank");
						}
					}
					else if( InfoRectBranch.Intersection( &pos) )
					{
							Cursor::SetMode(CA_Gear);
							if( clicked )
								Hud->ShowBannerText("L1behind");
					}
				}
				if( ProfileSystem::Instance->ActiveProfile.L1S2_IsInvPlank1Taken  )
				{
					if( InvItemRectPlank.Intersection( &pos) )
					{
						if(  ProfileSystem::Instance->ActiveProfile.L1S2_P3IsTreeBranchFixed )
						{
							Cursor::SetMode(CA_HandTake);
							if( clicked )
							{
								ProfileSystem::Instance->ActiveProfile.L1S2_IsInvTreeBranchTaken = true;
								Popup3Swing->GetObjectByID("P3stickPatched3")->SetEnable(false);
								TakeInventoryItem(eInv_L1S2_TreeBranch);
								Pop_CloseButton->PlayAnimation(0);

								if(ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth1SwingTaken)
								{
									popupCloseDelayTimer = 1.0f;
									ClosePopup();
								}
							}
							return;
						}
						else
						{
							Cursor::SetMode(CA_Gear);
							if( clicked )
								Hud->ShowBannerText("L1branch");
						}
					}
					
				}
			}
			break;
		}
	}
}

void L1_S2_Frontyard::TakeInventoryItem(EHOInventory ItemID)
{
	std::vector<int> vecInvIds;
	switch (ItemID)
	{
		case eInv_L1S2_TreeBranch:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S2_TreeBranch] = true;		
			ProfileSystem::Instance->ActiveProfile.L1S2_IsInvTreeBranchTaken = true;
			vecInvIds.push_back(ItemID);
			Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
			TaskPendingUpdate();
			isSparkleUpdatePending = true;
		break;
		case eInv_L1S2_Plank1:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S2_Plank1] = true;		
			ProfileSystem::Instance->ActiveProfile.L1S2_IsInvPlank1Taken = true;
			vecInvIds.push_back(ItemID);
			Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
			TaskPendingUpdate();
			isSparkleUpdatePending = true;
		break;
		case eInv_L1S2_Hose:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S2_Hose] = true;		
			vecInvIds.push_back(ItemID);
			Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
			TaskPendingUpdate();
			isSparkleUpdatePending = true;
		break;
		case eInv_L1S2_EmptyCan:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S2_EmptyCan] = true;		
			vecInvIds.push_back(ItemID);
			Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
			TaskPendingUpdate();
			isSparkleUpdatePending = true;
		break;

		case eInv_L1S2_Cloth1Swing:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S2_Cloth1Swing] = true;		
			vecInvIds.push_back(ItemID);
			Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
			TaskPendingUpdate();
			isSparkleUpdatePending = true;
		break;

		case eInv_L1S2_Cloth2LovePop:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S2_Cloth2LovePop] = true;		
			vecInvIds.push_back(ItemID);
			Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
			TaskPendingUpdate();
			isSparkleUpdatePending = true;
		break;

		case eInv_L1S2_Cloth3CarDicky:
			Hud->InvBoxCtrl->Activate(1);			
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S2_Cloth3CarDicky] = true;		
			vecInvIds.push_back(ItemID);
			Hud->ShowInventoryDirect(&vecInvIds,Control::Input->Mpos());
			TaskPendingUpdate();
			isSparkleUpdatePending = true;
		break;
	default:
		break;
	}
}

void L1_S2_Frontyard::Render(const CPoint* pCam)
{  
	
	static int FrameCnt = 0;
	
	if (ActivePopupID != ePopInactive)
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
		return; //No BG Video when popups are active
	}
	else
	{
		if(ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite)
		{
			if( isVidFireLoaded )
			{
				FrameCnt = (FrameCnt+1)%2;
				if( FrameCnt  || !lpVid_fire->isVideoAvailable() )
					lpVid_fire->readFrame();

				lpVid_fire->getKGraphic()->setAlphaMode(K_ALPHA_ADDITIVE);
				lpVid_fire->getKGraphic()->blitAlphaRectFx (0, 0, 112, 153, 751, 237, 0, 1.0f, 1.0f);
				lpVid_fire->getKGraphic()->blitAlphaRectFx (0, 0, 112, 153, 775, 231, 0, 1.0f, 1.0f);

				lpVid_fire->getKGraphic()->blitAlphaRectFx (0, 0, 112, 153, 944, 231, 0, 1.0f, 1.0f);
				lpVid_fire->getKGraphic()->blitAlphaRectFx (0, 0, 112, 153, 731, 86, 0, 1.0f, 1.0f);
				lpVid_fire->getKGraphic()->blitAlphaRectFx (0, 0, 112, 153, 922, 84, 0, 1.0f, 1.0f);

				lpVid_fire->getKGraphic()->blitAlphaRectFx (0, 0, 112, 153, 1030, 100, -20, 0.5f, 1.0f);
				lpVid_fire->getKGraphic()->blitAlphaRectFx (0, 0, 112, 153, 1030, 115, -20, 0.5f, 1.0f);
				//lpVid_fire->getKGraphic()->blitAlphaRectFx (0, 0, 112, 153, 922, 84, 0, 1.0f, 1.0f);
			}
		}
	}
}

L1_S2_Frontyard::~L1_S2_Frontyard()
{
	if( GFInstance->availablePhyMem >= MAX_LOWEND_PEAKMEMORY )
	{
	}
	TaskPendingUpdate();
	
	SAFE_DELETE(flyingInsects);

	SAFE_DELETE(CAnim_CarDickieOpen);

	SAFE_DELETE(Popup1CarDickie);
	SAFE_DELETE(Popup2Love);
	SAFE_DELETE(Popup3Swing);

	SAFE_DELETE(CAnim_RopeCut);
	SAFE_DELETE(CAnim_TreeBranchFix);

	SAFE_DELETE(p1PolyWood);
	SAFE_DELETE(p1PolyDieselCan);

	/* Added to HOScene. So no need to clear.
	SAFE_DELETE(BaboonNormalAnim);
	SAFE_DELETE(BaboonAttackAnim);*/
	for (int i = 0; i < 2; i++)
	{
		SAFE_DELETE(BaboonRunAnim[i]);
	}
	SAFE_DELETE(BananaThrowAnim);

	SAFE_DELETE(RainBgVFX);
	SAFE_DELETE(RainDropsVFX);

	SAFE_DELETE(P3RainVFX);
	SAFE_DELETE(P3RaindropsVFX);

	if(RT_BgScn!=NULL)
		RT_BgScn->pSprite->Image = NULL;
	SAFE_DELETE(RT_BgScn);

	SAFE_DELETE(lpVid_fire);
	
	//MUSIC_SFX
	if(IsMansionLit)
	{
		Control::Audio->StopSample(aAmbS2FrontyardLit);
		Control::Audio->UnloadSample(aAmbS2FrontyardLit);
	}
	else
	{
		Control::Audio->StopSample(aAmbS2FrontyardUnlit);
		Control::Audio->UnloadSample(aAmbS2FrontyardUnlit);
	}

	Control::Audio->UnloadSample(aVOL1MaryRefS2_1);
	Control::Audio->UnloadSample(aVOL1MaryRefS2_2);
	//MUSIC_SFX
}

void L1_S2_Frontyard::ReceiveMessage(int val)
{	
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		TaskPendingUpdate();
		Hud->ShowMap();
	}

	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			if(ProfileSystem::Instance->ActiveProfile.L1S2_IsMonkeyTookBanana && ActivePopupID == ePopInactive)
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

			if(ActivePopupID == ePopInactive)
			{
				
				if( itemVal == eInv_L1S3_Banana && !ProfileSystem::Instance->ActiveProfile.L1S2_IsMonkeyTookBanana)
				{
					if(SceneBigRectBaboon.Intersection(&dropPos))
					{
						isBananaThrowAnimPlaying = true;
						BananaThrowAnim->SetEnable(true);
						BananaThrowAnim->ForcePlayAnimation(0);

						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S2MonkeyTakeBanana);

						Hud->ReleaseDrag();
						Hud->CloseInventory(); 
					
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S3_Banana] = false;

						Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S3_Banana);
						ProfileSystem::Instance->ActiveProfile.L1S2_IsMonkeyTookBanana = true;
						isSparkleUpdatePending = true;
						return;
					}
					else
					{
						GFInstance->WrongInvDropMsg(Hud);
					}
				}
				else
				{
					GFInstance->WrongInvDropMsg(Hud);
				}
				return;
			}
			
			if( itemVal == eInv_L1S6_Crowbar )
			{
				if( Popup1_State == POPUP_OPEN && !ProfileSystem::Instance->ActiveProfile.L1S2_IsCarDickeyOpen)
				{
					if (InvDestRectCrowbarDicky.Intersection(&dropPos))
					{
							isCarDickeyAnimPlaying = true;

							CAnim_CarDickieOpen->PlayAnim();

							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S2P1CrowBarUse);

							Hud->ReleaseDrag();
							Hud->CloseInventory(); 

							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S6_Crowbar] = false;
							Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S6_Crowbar);

							ProfileSystem::Instance->ActiveProfile.L1S2_IsCarDickeyOpen = true; 
						
					}
					else
						GFInstance->WrongInvDropMsg(Hud);
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if( itemVal == eInv_L1S1_SwissKnife )
			{
				if( Popup3_State == POPUP_OPEN && !ProfileSystem::Instance->ActiveProfile.L1S2_P3IsSwingRopeCut )
				{
					if (InvDestRect_SwingRopeCut.Intersection(&dropPos) || InvDestRect_SwingRopeCut2.Intersection(&dropPos) )
					{
						isRopeCutPlaying = true;
						CAnim_RopeCut->PlayAnim();
						//Control::Audio->QuickLoadAndPlaySFX(aSFXL1S2P1CrowBarUse);

						Hud->ReleaseDrag();
						Hud->CloseInventory(); 

						Popup3Swing->GetObjectByID("P3ropeCut")->SetEnable(true);
						ProfileSystem::Instance->ActiveProfile.L1S2_P3IsSwingRopeCut = true;

						if (ProfileSystem::Instance->ActiveProfile.L1S1_P1Branch3Cut && ProfileSystem::Instance->ActiveProfile.L1S7_IsRopeCut)
						{
							//if Beach Rose Cut and Boat Rope cut, no need for knife
							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S1_SwissKnife] = false;
							Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S1_SwissKnife); 
						}
						Control::Audio->QuickLoadAndPlaySFX(aSFXL1S2RopeCut);
						HOScene->GetObjectByID("swingplank")->SetEnable(false);
					}
					else
						GFInstance->WrongInvDropMsg(Hud);
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if( itemVal == eInv_L1S2_Cloth1Swing )
			{
				if( InvItemRectPlank.Intersection(&dropPos) )
				{
					if( Popup3_State == POPUP_OPEN && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvPlank1Taken )
					{
						if( (ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth1SwingTaken && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth2LovePopTaken && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth3CarDickyTaken) )
						{
							isTreeBranchFixAnimPlaying = true;
							CAnim_TreeBranchFix->PlayAnim();

							Hud->ReleaseDrag();
							Hud->CloseInventory(); 

							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S2_Cloth1Swing] = false;
							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S2_Cloth2LovePop] = false;
							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L1S2_Cloth3CarDicky] = false;

							Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L1S2_Cloth1Swing);
							Control::Audio->QuickLoadAndPlaySFX(aSFXL1S2StickFixed);
							ProfileSystem::Instance->ActiveProfile.L1S2_P3IsTreeBranchFixed = true;
						}
						else
						{
							Hud->ReleaseDrag();
							Hud->CloseInventory(); 
							if( (!ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth1SwingTaken && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth2LovePopTaken && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth3CarDickyTaken) )
							{
								Hud->ShowBannerText("L1one");
							}
							else if( (ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth1SwingTaken && !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth2LovePopTaken && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth3CarDickyTaken) )
							{
								Hud->ShowBannerText("L1one");
							}
							else if( (ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth1SwingTaken && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth2LovePopTaken && !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth3CarDickyTaken) )
							{
								Hud->ShowBannerText("L1one");
							}
							else if( (ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth1SwingTaken && !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth2LovePopTaken && !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth3CarDickyTaken) )
							{
								Hud->ShowBannerText("L1two");
							}
							else if( (!ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth1SwingTaken && !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth2LovePopTaken && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth3CarDickyTaken) )
							{
								Hud->ShowBannerText("L1two");
							}
							else if( (!ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth1SwingTaken && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth2LovePopTaken && !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth3CarDickyTaken) )
							{
								Hud->ShowBannerText("L1two");
							}
						}
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
		UpdateBgGraphic();
		TaskPendingUpdate();
	}
	else if( val == INV_CLOSE )
	{		
		
		
	}	
}

void L1_S2_Frontyard::ClosePopup()
{
	if( ActivePopupID == ePopL1S2_P1CarDickie )
	{		
		Popup1_State = POPUP_SCALEDOWN;
		Popup1CarDickie->Show(false);
		
		SetupPopupTransition(Popup1CarDickie, PopRectCarDickie);
	}
	else if( ActivePopupID == ePopL1S2_P3Swing )
	{		
		P3RainVFX->StopImmediate();
		P3RaindropsVFX->StopImmediate();

		Popup3_State = POPUP_SCALEDOWN;
		Popup3Swing->Show(false);
		
		SetupPopupTransition(Popup3Swing, PopRect3Swing );
	}

}

void L1_S2_Frontyard::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S2Collected)
		{
			InitHintTrailEmit(520, 400);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		} 
		return;
	}

	if(isBananaThrowAnimPlaying || isBaboonRunAnimPlaying)
		return;

	int iNonHOHint = 0; 
	int MaxNonHOHints = 28;
	do
	{
		switch( iNonHOHint )
		{
			default:			
			//Exit coordinates
			case 0: ++iNonHOHint;
				if( !ProfileSystem::Instance->ActiveProfile.L1S2_IsCarDickeyOpen  && ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCrowbarPicked && Popup1_State == POPUP_OPEN)
					{
						Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S6_Crowbar)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}

							CPoint tempPoint (InvDestRectCrowbarDicky.GetCenterX(), InvDestRectCrowbarDicky.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						return;
					}
					break;	
			case 1: ++iNonHOHint;
					 if(  !ProfileSystem::Instance->ActiveProfile.L1S2_InvHoseTaken && ProfileSystem::Instance->ActiveProfile.L1S2_IsCarDickeyOpen && Popup1_State == POPUP_OPEN)
					{
						InitHintTrailEmit(	popRectHose.GetCenterX(), popRectHose.GetCenterY(),false);
						return;
					}
					break;	
			case 2: ++iNonHOHint;
					 if(  !ProfileSystem::Instance->ActiveProfile.L1S2_InvDieselCanTaken && ProfileSystem::Instance->ActiveProfile.L1S2_IsCarDickeyOpen && Popup1_State == POPUP_OPEN)
					{
						InitHintTrailEmit(	popRectSack.GetCenterX(), popRectSack.GetCenterY(),false);
						return;
					}
					break;	
			case 3: ++iNonHOHint;
					 if( Popup1_State == POPUP_OPEN)
					{
						InitHintTrailEmit(	PopCloseButtonRect.GetCenterX(), PopCloseButtonRect.GetCenterY(),true);
						return;
					}
					break;	
			case 4: ++iNonHOHint;
					 if(  !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth2LovePopTaken && Popup2_State == POPUP_OPEN)
					{
						InitHintTrailEmit(	InvItemRectClothP2.GetCenterX(), InvItemRectClothP2.GetCenterY(),false);
						return;
					}
					break;	
			case 5: ++iNonHOHint;
					 if( Popup2_State == POPUP_OPEN)
					{
						Hud->ShowNothingToDoMsgBanner();
						return;
					}
					break;	
			case 6: ++iNonHOHint;
					 if(  !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth1SwingTaken && Popup3_State == POPUP_OPEN)
					{
						InitHintTrailEmit(	InvItemRect_Cloth1Swing.GetCenterX(), InvItemRect_Cloth1Swing.GetCenterY(),false);
						return;
					}
					break;	
			case 7: ++iNonHOHint;
					 if(  !ProfileSystem::Instance->ActiveProfile.L1S2_P3IsSwingRopeCut && ProfileSystem::Instance->ActiveProfile.L1S1_IsInvKnifeTaken && Popup3_State == POPUP_OPEN)
					{
						Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S1_SwissKnife)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}

							CPoint tempPoint (InvDestRect_SwingRopeCut.GetCenterX(), InvDestRect_SwingRopeCut.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						return;
					}
					break;	
			case 8: ++iNonHOHint;
					 if(  !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvPlank1Taken && ProfileSystem::Instance->ActiveProfile.L1S2_P3IsSwingRopeCut && Popup3_State == POPUP_OPEN)
					{
						InitHintTrailEmit(	InvItemRectPlank.GetCenterX(), InvItemRectPlank.GetCenterY(),false);
						return;
					}
					break;
			case 9: ++iNonHOHint;
					 if(  !ProfileSystem::Instance->ActiveProfile.L1S2_P3IsTreeBranchFixed && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth1SwingTaken && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth2LovePopTaken && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth3CarDickyTaken && Popup3_State == POPUP_OPEN)
					{
						Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S2_Cloth1Swing)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}

							CPoint tempPoint (InvItemRectPlank.GetCenterX(), InvItemRectPlank.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						return;
					}
					break;	
			case 10: ++iNonHOHint;
					 if(  !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvTreeBranchTaken && ProfileSystem::Instance->ActiveProfile.L1S2_P3IsTreeBranchFixed && Popup3_State == POPUP_OPEN)
					{
						InitHintTrailEmit(	InvItemRectPlank.GetCenterX(), InvItemRectPlank.GetCenterY(),false);
						return;
					}
					break;
			case 11: ++iNonHOHint;
					 if( Popup3_State == POPUP_OPEN)
					{
						InitHintTrailEmit(	PopCloseButtonRect.GetCenterX(), PopCloseButtonRect.GetCenterY(),true);
						return;
					}
					break;	
			case 12: ++iNonHOHint;
					 if(!ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved)
					{
						InitHintTrailEmit(SceneRectFuzePuzzle.GetCenterX(), SceneRectFuzePuzzle.GetCenterY(),false);
						return;
					}
					break;
			case 13: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S2_IsMonkeyTookBanana  && ProfileSystem::Instance->ActiveProfile.L1S3_IsBananaTaken )
					{
						Hud->InvBoxCtrl->IsLocked = true;																							 
							Hud->InvBoxCtrl->Activate();
						
							for(int i=0;  i < MAX_INVBOX_OBJECTS; ++i)
							{
								if( Hud->InvBoxCtrl->InvBoxObj[i].HOObj != NULL  )
								{	
									if(  Hud->InvBoxCtrl->InvBoxObj[i].HOObj->PublicValue == eInv_L1S3_Banana)
									{
										if((i-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
												Hud->InvBoxCtrl->ShiftInvBoxLeft(i-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
										else if( i < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
												Hud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-i);
									
										InitHintTrailEmit(Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.x, Hud->InvBoxCtrl->InvBoxObj[i].HOObj->Pos.y,false);
									}
								}
							}

							CPoint tempPoint (SceneRectBaboon.GetCenterX(), SceneRectBaboon.GetCenterY(),0);
							Hud->Hint2FX->ResetPos(&tempPoint);
							Hud->Hint2FX->Emit();
						return;
					}
					break;
			case 14: ++iNonHOHint;
					if(  ProfileSystem::Instance->ActiveProfile.L1S2_IsMonkeyTookBanana && !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth3CarDickyTaken )
					{
						InitHintTrailEmit(	InvItemRectClothP1CarDicky.GetCenterX(), InvItemRectClothP1CarDicky.GetCenterY(),false);
						return;
					}
					break;		
			case 15: ++iNonHOHint;
					 if(  !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth2LovePopTaken )
					{
						InitHintTrailEmit(	PopRectLove.GetCenterX(), PopRectLove.GetCenterY(),false);
						return;
					}
					break;	
			case 16: ++iNonHOHint;
					 if(  !ProfileSystem::Instance->ActiveProfile.L1S2_P3IsSwingRopeCut && ProfileSystem::Instance->ActiveProfile.L1S1_IsInvKnifeTaken )
					{
						InitHintTrailEmit(	PopRect3Swing.GetCenterX(), PopRect3Swing.GetCenterY(),false);
						return;
					}
					break;	
			case 17: ++iNonHOHint;
					 if(  !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvPlank1Taken && ProfileSystem::Instance->ActiveProfile.L1S2_P3IsSwingRopeCut )
					{
						InitHintTrailEmit(	PopRect3Swing.GetCenterX(), PopRect3Swing.GetCenterY(),false);
						return;
					}
					break;
			case 18: ++iNonHOHint;
					 if(  !ProfileSystem::Instance->ActiveProfile.L1S2_P3IsTreeBranchFixed && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth1SwingTaken && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth2LovePopTaken && ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth3CarDickyTaken)
					{
						InitHintTrailEmit(	PopRect3Swing.GetCenterX(), PopRect3Swing.GetCenterY(),false);
						return;
					}
					break;	
			case 19: ++iNonHOHint;
					 if(  !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvTreeBranchTaken && ProfileSystem::Instance->ActiveProfile.L1S2_P3IsTreeBranchFixed )
					{
						InitHintTrailEmit(	PopRect3Swing.GetCenterX(), PopRect3Swing.GetCenterY(),false);
						return;
					}
					break;
			case 20: ++iNonHOHint;
					 if(  !ProfileSystem::Instance->ActiveProfile.L1S2_IsInvCloth1SwingTaken )
					{
						InitHintTrailEmit(	PopRect3Swing.GetCenterX(), PopRect3Swing.GetCenterY(),false);
						return;
					}
					break;
			case 21: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L1S2_IsCarDickeyOpen  && ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCrowbarPicked && ProfileSystem::Instance->ActiveProfile.L1S2_IsMonkeyTookBanana)
					{
						InitHintTrailEmit(PopRectCarDickie.GetCenterX(), PopRectCarDickie.GetCenterY(),false);
						return;
					}
					break;
					
			case 22: ++iNonHOHint;
					 if(  !ProfileSystem::Instance->ActiveProfile.L1S2_InvHoseTaken && ProfileSystem::Instance->ActiveProfile.L1S2_IsCarDickeyOpen )
					{
						InitHintTrailEmit(PopRectCarDickie.GetCenterX(), PopRectCarDickie.GetCenterY(),false);
						return;
					}
					break;	
			case 23: ++iNonHOHint;
					 if(  !ProfileSystem::Instance->ActiveProfile.L1S2_InvDieselCanTaken && ProfileSystem::Instance->ActiveProfile.L1S2_IsCarDickeyOpen )
					{
						InitHintTrailEmit(PopRectCarDickie.GetCenterX(), PopRectCarDickie.GetCenterY(),false);
						return;
					}
					break;	
			case 24: ++iNonHOHint;
					if((!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S3_Foyer] && ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved) || ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S3_Foyer])
					{
						InitHintTrailEmit(SceneRectFoyer.GetCenterX(), SceneRectFoyer.GetCenterY(),true,CA_ExitRight);
						return;
					}
					break;
			case 25: ++iNonHOHint;
					if((!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S7_BoatDock] && ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved) || ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S7_BoatDock])
					{
						InitHintTrailEmit(SceneRectBoatdock.GetCenterX(), SceneRectBoatdock.GetCenterY(),true,CA_ExitLeft);
						return;
					}
					break;
			case 26: ++iNonHOHint;
					if( ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L1_S1_Beach])
					{
						InitHintTrailEmit(HoveredBackBtnRectMax.topleft.x + HoveredBackBtnRectMax.w/2 ,HoveredBackBtnRectMax.topleft.y + HoveredBackBtnRectMax.h/2,true,CA_ExitDown);
						return;
					}
					break;
			case 27: ++iNonHOHint;
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

void L1_S2_Frontyard::EndBgGraphicScaling()
{
	RT_BgScn->SetScale(1.0f);
	RT_BgScn->SetEnable(false);
}

void L1_S2_Frontyard::SparkleUpdate()
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

		if(!ProfileSystem::Instance->ActiveProfile.L1S2_IsInvPlank1Taken)
		{
			sparklePoints[numSparkles].SetXY(InvItemRectPlank.GetCenterX(), InvItemRectPlank.GetCenterY());
			numSparkles++;
		}
		if(!ProfileSystem::Instance->ActiveProfile.L1S2_IsFusePuzzleSolved)
		{
			sparklePoints[numSparkles].SetXY(SceneRectFuzePuzzle.GetCenterX(), SceneRectFuzePuzzle.GetCenterY());
			numSparkles++;
		}
		else
		{
			if(!ProfileSystem::Instance->ActiveProfile.L1S2_IsMonkeyTookBanana && ProfileSystem::Instance->ActiveProfile.L1S3_IsBananaTaken)
			{
				sparklePoints[numSparkles].SetXY(SceneRectBaboon.GetCenterX(), SceneRectBaboon.GetCenterY());
				numSparkles++;
			}
			else if( ProfileSystem::Instance->ActiveProfile.L1S2_IsMonkeyTookBanana )
			{
				if((!ProfileSystem::Instance->ActiveProfile.L1S2_InvHoseTaken || !ProfileSystem::Instance->ActiveProfile.L1S2_InvDieselCanTaken) && ProfileSystem::Instance->ActiveProfile.L1S6_IsInvCrowbarPicked)
				{
					sparklePoints[numSparkles].SetXY(PopRectCarDickie.GetCenterX(), PopRectCarDickie.GetCenterY());
					numSparkles++;
				}
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

void L1_S2_Frontyard::UpdateBgGraphic()
{
	if(RT_BgScn == NULL)
	{
		RT_BgScn = new CObject();
		RT_BgScn->LoadSprite("UI\\ZOOMBG.SPR");
		RT_BgScn->SetZ(104);

		RT_BgScn->SetEnable(false);
	}

	#ifdef _CEBUILD
		Hud->HideMorphObjects();
	#endif 
        Scn_Lightning->SetEnable(false);
    if(GFInstance->IsScnRTCreated)
    {
        GFInstance->CurrentSceneGraphic->clearRenderTarget();
        GFInstance->CurrentSceneGraphic->beginRenderToTarget();//RENDER TO TARGET STARTS
        GFInstance->IsRenderingToRT = true;
    }
        if(GFInstance->IsScnBg1024Res)
        {
            HOScene->RenderToTarget(-171.0f);
        }
        else
        {
            HOScene->RenderToTarget();
        }
	if(ProfileSystem::Instance->ActiveProfile.L1S5_POPBombLite)
	{
		if( isVidFireLoaded )
		{
			lpVid_fire->getKGraphic()->setAlphaMode(K_ALPHA_ADDITIVE);
			lpVid_fire->getKGraphic()->blitAlphaRectFx (0, 0, 112, 153, 751, 237, 0, 1.0f, 1.0f);
			lpVid_fire->getKGraphic()->blitAlphaRectFx (0, 0, 112, 153, 775, 231, 0, 1.0f, 1.0f);

			lpVid_fire->getKGraphic()->blitAlphaRectFx (0, 0, 112, 153, 944, 231, 0, 1.0f, 1.0f);
			lpVid_fire->getKGraphic()->blitAlphaRectFx (0, 0, 112, 153, 731, 86, 0, 1.0f, 1.0f);
			lpVid_fire->getKGraphic()->blitAlphaRectFx (0, 0, 112, 153, 922, 84, 0, 1.0f, 1.0f);

			lpVid_fire->getKGraphic()->blitAlphaRectFx (0, 0, 112, 153, 1030, 100, -20, 0.5f, 1.0f);
			lpVid_fire->getKGraphic()->blitAlphaRectFx (0, 0, 112, 153, 1030, 115, -20, 0.5f, 1.0f);
			//lpVid_fire->getKGraphic()->blitAlphaRectFx (0, 0, 112, 153, 922, 84, 0, 1.0f, 1.0f);
		}
	}
    if(GFInstance->IsScnRTCreated)
    {
        GFInstance->IsRenderingToRT = false;
        GFInstance->CurrentSceneGraphic->endRenderToTarget();
    }
        Scn_Lightning->SetEnable(true);
			
	RT_BgScn->pSprite->Image = GFInstance->CurrentSceneGraphic;
}

void  L1_S2_Frontyard::SetupPopupTransition(CHOScene *Popup, CRectangle popRect)
{

		
		bgScaleOffsetX = popRect.GetCenterX();
		bgScaleOffsetY = popRect.GetCenterY();
		
		RT_BgScn->SetEnable(true);
		RT_BgScn->SetScale(1.0f);
		RT_BgScn->SetXY(0,0);

		HOScene->PauseObjects(true);

		IsPopupTransitionActive = true;
    if( GFInstance->IsRTCreated )
    {
		GFInstance->RTGraphic->clearRenderTarget();   
		GFInstance->RTGraphic->beginRenderToTarget();//RENDER TO TARGET STARTS	
		GFInstance->IsRenderingToRT = true;
    }
		Popup->RenderToTarget(-171.0f,0);
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
void L1_S2_Frontyard::NavigateToScene()
{
	if(GFHud->InvBoxCtrl->navTapped)
	{
		if( Control::Input->LBclicked() )
		{
			if(Control::Input->Mpos().x > GFHud->InvBoxCtrl->navRectX2 && Control::Input->Mpos().x < GFHud->InvBoxCtrl->navRectX1 && Control::Input->Mpos().y > GFHud->InvBoxCtrl->navRectY2 && Control::Input->Mpos().y < GFHud->InvBoxCtrl->navRectY1)
			{	
				Cursor::SetMode(CA_ExitUp);
				
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
