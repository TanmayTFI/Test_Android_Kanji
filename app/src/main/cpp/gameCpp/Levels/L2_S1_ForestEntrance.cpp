//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 01 : Forest Entrance
//====================================

#include "L2_S1_ForestEntrance.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "Rect.h"
#include "TextureFactory.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "AwardManager.h"

const CPoint		CenterPoint(683,334,0);
const CPoint		PopCloseRectBtnPoint(998,98,0);
const CRectangle	PopCloseButtonRect(968.0f,71.0f,62.0f,61.0f);
const CRectangle	PopAreaRect(345.0f,90.0f,665.0f,501.0f);


//Scene Rects

const CRectangle  SceneRectNagaTemple(643.0f,168.0f,98.0f,85.0f);
const CRectangle  SceneRectCaveMouth(872.0f,186.0f,212.0f,139.0f);

const CRectangle  SceneRectUndergroundHO(375.0f,120.0f,200.0f,200.0f);


//Inventory Item Rects
//Info Rects


L2_S1_ForestEntrance::L2_S1_ForestEntrance()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV2\\S1\\S1.SCN");
	musicFadeOutTimer = 0;
	// Set Objects
	GFInstance->PostUpdate(1);

	SCN_cutAloevera = HOScene->GetObjectByID("Alovera_CUtVersion");
	SCN_HoneyCombUncut = HOScene->GetObjectByID("honey-comb01");
	SCN_HoneyBee = HOScene->GetObjectByID("honey-comb02");
	SCN_Scrapper = HOScene->GetObjectByID("scraper");

	SCN_rightPlant = HOScene->GetObjectByID("Plant07");
	SCN_leftPlant = HOScene->GetObjectByID("Right-plant06");
	SCN_mushroom = HOScene->GetObjectByID("Mushroom1");
	SCN_lotus = HOScene->GetObjectByID("Lotus");
	SCN_lotusReflection = HOScene->GetObjectByID("LotusReflection");
	SCN_roundLeafset1 = HOScene->GetObjectByID("Plant05");
	SCN_roundLeafset2 = HOScene->GetObjectByID("Plant008");
	SCN_topTreeLeafSet1 = HOScene->GetObjectByID("TreeBranches03");
	SCN_topTreeLeafSet2 = HOScene->GetObjectByID("Valli04");
	SCN_topTreeLeafSet3 = HOScene->GetObjectByID("Valli");
	SCN_topTreeLeafSet4 = HOScene->GetObjectByID("Valli02");
	SCN_topTreeLeafSet5 = HOScene->GetObjectByID("Valli01");
	SCN_topTreeLeafSet6 = HOScene->GetObjectByID("TreeBranches02");
	SCN_topTreeLeafSet7 = HOScene->GetObjectByID("TreeBranches");
	SCN_bottomLeafset1 = HOScene->GetObjectByID("Plant03");
	SCN_bottomLeafset2 = HOScene->GetObjectByID("Plant04");


	SCN_rightPlant->SetPivot(98.5,35);
	SCN_rightPlant->Animate(0,0,4,0,5);
	SCN_leftPlant->SetPivot(-25,-58);
	SCN_leftPlant->Animate(0,0,4,0,5);
	SCN_mushroom->SetPivot(0,57);
	SCN_mushroom->Animate(0,0,3,0,5);
	SCN_lotus->SetPivot(0,100);
	SCN_lotus->Animate(0,0,2,0,4);
	SCN_lotusReflection->SetPivot(0,-50);
	SCN_lotusReflection->Animate(0,0,-2,0,4);
	SCN_roundLeafset1->SetPivot(0,90);
	SCN_roundLeafset1->Animate(0,0,4,0,3.5);
	SCN_roundLeafset2->SetPivot(0,93);
	SCN_roundLeafset2->Animate(0,0,-4,0,3);
	SCN_topTreeLeafSet1->SetPivot(0,-46);
	SCN_topTreeLeafSet1->Animate(0,0,5,0,5);
	SCN_topTreeLeafSet2->SetPivot(0,-56);
	SCN_topTreeLeafSet2->Animate(0,0,-6,0,5);
	SCN_topTreeLeafSet3->SetPivot(0,-105);
	SCN_topTreeLeafSet3->Animate(0,0,6,0,4);
	SCN_topTreeLeafSet4->SetPivot(0,-46);
	SCN_topTreeLeafSet4->Animate(0,0,-4,0,3);
	SCN_topTreeLeafSet5->SetPivot(0,-91);
	SCN_topTreeLeafSet5->Animate(0,0,-2,0,3);
	SCN_topTreeLeafSet6->SetPivot(0,-42);
	SCN_topTreeLeafSet6->Animate(0,0,-2,0,3);
	SCN_topTreeLeafSet7->SetPivot(0,-50);
	SCN_topTreeLeafSet7->Animate(0,0,2,0,5); 
	SCN_bottomLeafset1->Animate(0,8,0,0,5,2);
	SCN_bottomLeafset2->Animate(3,8,2,0,5,2);
	GFInstance->PostUpdate(1);
	SCN_pondWater = NULL;
	SCN_pondWater = new MeshObject("LV2\\S1\\PONDWATER.PNG", 5, 5);
	SCN_pondWater->SetPosAndParams(0,532, 2, 2, 2, 5);
	SCN_pondWater->SetZ(15);
	HOScene->InsertObject(SCN_pondWater,15);

	BeesFX = NULL;
	if(!ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked)
	{
		BeesFX = new SuperFX("PRT\\L2\\Bees.INI", PointSystem::CreateCPoint(1087,136,0), 100); 
		BeesFX->Start();
		BeesFX->AdvanceByTime(2.0f);
	}
	GFInstance->PostUpdate(1);
	SceneObjectsUpdate();

	GFInstance->PostUpdate(1);

	InitPopups();
	GFInstance->PostUpdate(1);

	vidTimer = 0;


	// popups
	int p1rect[] = {1065,175,1001,108,1002,59,1170,44,1196,136,1125,177,1063,174};
	pop1ClickPoly = new ClickablePolygon(eHOPop1_HoneyComb, 7, p1rect, CC_CALLBACK_1(L2_S1_ForestEntrance::onPopAreaClicked,this));
	pop1ClickPoly->SetMouseOverCallback(CC_CALLBACK_1(L2_S1_ForestEntrance::onPopAreaMouseOver,this));
	pop1ClickPoly->SetMouseOutCallback(CC_CALLBACK_1(L2_S1_ForestEntrance::onPopAreaMouseOut,this));

	pop1HoneyComb = new L2S1_Pop1HoneyComb(eHOPop1_HoneyComb, Hud, HOScene, eZOrder_Popup, pop1ClickPoly->center, CC_CALLBACK_1(L2_S1_ForestEntrance::onPopupClosed, this));
	pop1HoneyComb->SetPopupBoundingBox(PopAreaRect);
	PopUpArray[eHOPop1_HoneyComb] = pop1HoneyComb;

	int p2Rect[] = {414,145,505,153,510,242,404,233,411,145};
	pop2ClickPoly=  new ClickablePolygon(eHOPop2_UndergroundDoor, 5, p2Rect, CC_CALLBACK_1(L2_S1_ForestEntrance::onPopAreaClicked,this));
	pop2ClickPoly->SetMouseOverCallback(CC_CALLBACK_1(L2_S1_ForestEntrance::onPopAreaMouseOver,this));
	pop2ClickPoly->SetMouseOutCallback(CC_CALLBACK_1(L2_S1_ForestEntrance::onPopAreaMouseOut,this));
	GFInstance->PostUpdate(1);

	pop2UndergroundDoor = new L2S1_Pop2UndergroundDoor(eHOPop2_UndergroundDoor, Hud, HOScene, eZOrder_Popup, pop2ClickPoly->center, CC_CALLBACK_1(L2_S1_ForestEntrance::onPopupClosed, this));
	pop2UndergroundDoor->SetPopupBoundingBox(PopAreaRect);
	PopUpArray[eHOPop2_UndergroundDoor] = pop2UndergroundDoor;

	int p3Rect[] = {1188,454,1109,382,1005,383,868,533,853,617,1039,623,1190,575,1187,457};
	pop3ClickPoly=  new ClickablePolygon(eHOPop3_AloeveraPlant, 8, p3Rect, CC_CALLBACK_1(L2_S1_ForestEntrance::onPopAreaClicked,this));
	pop3ClickPoly->SetMouseOverCallback(CC_CALLBACK_1(L2_S1_ForestEntrance::onPopAreaMouseOver,this));
	pop3ClickPoly->SetMouseOutCallback(CC_CALLBACK_1(L2_S1_ForestEntrance::onPopAreaMouseOut,this));

	pop3AloeveraPlant = new L2S1_Pop3AloeveraPlant(eHOPop3_AloeveraPlant, Hud, HOScene, eZOrder_Popup, pop3ClickPoly->center, CC_CALLBACK_1(L2_S1_ForestEntrance::onPopupClosed, this));
	pop3AloeveraPlant->SetPopupBoundingBox(PopAreaRect);
	PopUpArray[eHOPop3_AloeveraPlant] = pop3AloeveraPlant;

	int p4Rect[] = {715,512,537,552,705,598,840,585,892,521,713,509};
	pop4ClickPoly=  new ClickablePolygon(eHOPop4_CrystalClearWater, 6, p4Rect, CC_CALLBACK_1(L2_S1_ForestEntrance::onPopAreaClicked,this));
	pop4ClickPoly->SetMouseOverCallback(CC_CALLBACK_1(L2_S1_ForestEntrance::onPopAreaMouseOver,this));
	pop4ClickPoly->SetMouseOutCallback(CC_CALLBACK_1(L2_S1_ForestEntrance::onPopAreaMouseOut,this));

	pop4CrystalClearWater = new L2S1_Pop4ClearWater(eHOPop4_CrystalClearWater, Hud, HOScene, eZOrder_Popup, pop4ClickPoly->center, CC_CALLBACK_1(L2_S1_ForestEntrance::onPopupClosed, this));
	pop4CrystalClearWater->SetPopupBoundingBox(PopAreaRect);
	PopUpArray[eHOPop4_CrystalClearWater] = pop4CrystalClearWater;

	int p5Rect[] = {246,490,256,444,323,440,346,480,314,530,258,521,243,489};
	pop5ClickPoly=  new ClickablePolygon(eHOPop5_Boat, 7, p5Rect, CC_CALLBACK_1(L2_S1_ForestEntrance::onPopAreaClicked,this));
	pop5ClickPoly->SetMouseOverCallback(CC_CALLBACK_1(L2_S1_ForestEntrance::onPopAreaMouseOver,this));
	pop5ClickPoly->SetMouseOutCallback(CC_CALLBACK_1(L2_S1_ForestEntrance::onPopAreaMouseOut,this));

	pop5Scrapter = new L2S1_Pop5Scrapper(eHOPop5_Boat, Hud, HOScene, eZOrder_Popup, pop5ClickPoly->center, CC_CALLBACK_1(L2_S1_ForestEntrance::onPopupClosed, this));
	pop5Scrapter->SetPopupBoundingBox(PopAreaRect);
	PopUpArray[eHOPop5_Boat] = pop5Scrapter;
	GFInstance->PostUpdate(1);

	isMouseOverPopup = false;

	holinesFX = new SuperFX("PRT\\L2\\holines.INI", PointSystem::CreateCPoint(452,200,0), 900); 

	hodotsFX = new SuperFX("PRT\\L2\\hodots.INI", PointSystem::CreateCPoint(446,217,0), 900); 

	if( ProfileSystem::Instance->ActiveProfile.L2_S1_IsUndergroundHatchOpened && !ProfileSystem::Instance->ActiveProfile.L2_S1_InvColorPowderTaken)
	{
		if( !ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles )
		{
			holinesFX->Start();
			holinesFX->AdvanceByTime(2.0f);

			hodotsFX->Start();
			hodotsFX->AdvanceByTime(2.0f);
		}
	}
	GFInstance->PostUpdate(1);
	cv_swimCutscene = NULL; 
	time = 0;
	subIndex = 0;
	subMax = 3;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("l2intro1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("l2intro2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("l2intro3"));

	isSwimCutscenePlaying = false;
	if(!ProfileSystem::Instance->ActiveProfile.L2_S1_IsIntroSwimVideoPlayed)
	{
		Hud->EnableInput(false);
		Hud->HideHud();
		Hud->IsHintPaused = true;

		HOScene->Show(false);

		//CRectangle RectVid(171, 96, 1024, 576);
		CRectangle RectVid(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
		cv_swimCutscene = new CVideo("OGV\\L2\\S1INTROCUTSCN.OGV", RectVid, false, eZOrder_CutsceneFull);
		//cv_swimCutscene->SetScale(1.334f);
		cv_swimCutscene->SetScale(GFApp->video_aspectRatio);
		cv_swimCutscene->SetSkipActive();

		cv_swimCutscene->PlayVideo();
		isSwimCutscenePlaying = true;

	}
	GFInstance->PostUpdate(1);
#ifdef _CEBUILD
	//Morph Object Add
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S1Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("morphtarget"), HOScene->GetObjectByID("morphbase"));
		const CRectangle  MorphRect(648.0f,297.0f,54.0f,62.0f);
		Hud->setMorphObjRect(MorphRect);
	}
	else
	{
		HOScene->GetObjectByID("morphtarget")->SetEnable(false);
	}
#else
	HOScene->GetObjectByID("morphtarget")->SetEnable(false);
#endif 

	//MUSIC_SFX

	if(ProfileSystem::Instance->ActiveProfile.L2_S1_IsIntroSwimVideoPlayed)
	{

		Control::Audio->LoadSample(aAmbs1ForestEntrance,AudioAmb);
		if( !Control::Audio->IsPlaying(aAmbs1ForestEntrance) )		
		{
			Control::Audio->PlaySample(aAmbs1ForestEntrance,true);
		}
	}
	//MUSIC_SFX
	GFInstance->PostUpdate(1);
	TaskPendingUpdate();

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L2_S1_ForestEntrance]  = true;
}

void L2_S1_ForestEntrance::onPopAreaMouseOver(int popId)
{
	if (ActivePopupID != eHOS1Pop_Inactive || isSwimCutscenePlaying)
		return;

	switch (popId)
	{
	case eHOPop1_HoneyComb: if (!ProfileSystem::Instance->ActiveProfile.L2_S1_InvHoneyCombPicked)
								isMouseOverPopup = true;
		break;
	case eHOPop2_UndergroundDoor:	if (!ProfileSystem::Instance->ActiveProfile.L2_S1_IsNagaSculpturePlaced)
										isMouseOverPopup = true;
		break;
	case eHOPop3_AloeveraPlant: if (!ProfileSystem::Instance->ActiveProfile.L2_S1_InvStoneBallTaken || !ProfileSystem::Instance->ActiveProfile.L2_S1_InvAloveraTaken)
									isMouseOverPopup = true;
		break;
	case eHOPop4_CrystalClearWater: if (!ProfileSystem::Instance->ActiveProfile.L2_S1_InvPotWithWaterTaken)
										isMouseOverPopup = true;
		break;
	case eHOPop5_Boat: if (!ProfileSystem::Instance->ActiveProfile.L2_S1_InvScrapperTaken)
						   isMouseOverPopup = true;
		break;
	default:
		break;
	}

}

void L2_S1_ForestEntrance::onPopAreaMouseOut(int popId)
{
	isMouseOverPopup = false;
}

void L2_S1_ForestEntrance::onPopAreaClicked(int popId)
{
	if (ActivePopupID != eHOS1Pop_Inactive || isSwimCutscenePlaying)
		return;

	GFHud->InvBoxCtrl->NavigateText->Text = "";
	GFHud->InvBoxCtrl->travelArea = -1;
	GFHud->InvBoxCtrl->navTapped = false;
	GFHud->InvBoxCtrl->Navigate->Show(false);
	GFHud->InvBoxCtrl->NavArrow->Show(false);

	switch (popId)
	{

	case eHOPop1_HoneyComb:
		if (!ProfileSystem::Instance->ActiveProfile.L2_S1_InvHoneyCombPicked)
		{
			ActivePopupID = popId;
			pop1HoneyComb->OpenPopup();
		}
		if (!ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked)
			Hud->ShowBannerText("L2stupid");
		break;

	case eHOPop2_UndergroundDoor:
		if (!ProfileSystem::Instance->ActiveProfile.L2_S1_IsNagaSculpturePlaced)
		{
			ActivePopupID = popId;
			pop2UndergroundDoor->OpenPopup();
		}
		break;

	case eHOPop3_AloeveraPlant:
		if (!ProfileSystem::Instance->ActiveProfile.L2_S1_InvStoneBallTaken || !ProfileSystem::Instance->ActiveProfile.L2_S1_InvAloveraTaken)
		{
			ActivePopupID = popId;
			pop3AloeveraPlant->OpenPopup();
		}
		break;

	case eHOPop4_CrystalClearWater:
		if (!ProfileSystem::Instance->ActiveProfile.L2_S1_InvPotWithWaterTaken)
		{
			ActivePopupID = popId;
			pop4CrystalClearWater->OpenPopup();
			Hud->ShowBannerText("L2water");
		}
		break;

	case eHOPop5_Boat:
		if (!ProfileSystem::Instance->ActiveProfile.L2_S1_InvScrapperTaken)
		{
			ActivePopupID = popId;
			pop5Scrapter->OpenPopup();
		}
		break;

	default:
		break;
	}
}

void L2_S1_ForestEntrance::SparkleUpdate()
{
	if (ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		return;

	if (ActivePopupID != ePopInactive)
		return;

	CheckForPendingTask();

	if (!isSparkleUpdatePending)
		return;

	HiddenObjectSystem::SparkleUpdate();
}

void L2_S1_ForestEntrance::CheckForPendingTask()
{
	isSparkleUpdatePending = false;

	if( !ProfileSystem::Instance->ActiveProfile.L2_S1_InvPotWithWaterTaken  && ProfileSystem::Instance->ActiveProfile.L2_S5_InvPotTaken)
	{
		sparklePointsVec.push_back(CPoint(PopUpArray[eHOPop4_CrystalClearWater]->GetSparkleUpdatePoint()));
		isSparkleUpdatePending = true;
	}

	if( !ProfileSystem::Instance->ActiveProfile.L2_S1_IsUndergroundHatchOpened  && ProfileSystem::Instance->ActiveProfile.L2_S5_InvNagaSculptureTaken)
	{
		sparklePointsVec.push_back(CPoint(PopUpArray[eHOPop2_UndergroundDoor]->GetSparkleUpdatePoint()));
		isSparkleUpdatePending = true;
	}

	if( ProfileSystem::Instance->ActiveProfile.L2_S2_InvFrankinscenceTaken && !ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked)
	{
		sparklePointsVec.push_back(CPoint(PopUpArray[eHOPop1_HoneyComb]->GetSparkleUpdatePoint()));
		isSparkleUpdatePending = true;
	}

	if( ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken && !ProfileSystem::Instance->ActiveProfile.L2_S1_InvHoneyCombPicked && ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked)
	{
		sparklePointsVec.push_back(CPoint(PopUpArray[eHOPop1_HoneyComb]->GetSparkleUpdatePoint()));
		isSparkleUpdatePending = true;
	}

	if(  !ProfileSystem::Instance->ActiveProfile.L2_S1_InvStoneBallTaken)
	{
		sparklePointsVec.push_back(CPoint(PopUpArray[eHOPop3_AloeveraPlant]->GetSparkleUpdatePoint()));
		isSparkleUpdatePending = true;
	}

	if( !ProfileSystem::Instance->ActiveProfile.L2_S1_IsGlovesUsed && ProfileSystem::Instance->ActiveProfile.L2S6_InvGlovesTaken)
	{
		sparklePointsVec.push_back(CPoint(PopUpArray[eHOPop3_AloeveraPlant]->GetSparkleUpdatePoint()));
		isSparkleUpdatePending = true;
	}

	if( ProfileSystem::Instance->ActiveProfile.L2_S1_IsGlovesUsed && ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken && !ProfileSystem::Instance->ActiveProfile.L2_S1_InvAloveraTaken)
	{
		sparklePointsVec.push_back(CPoint(PopUpArray[eHOPop3_AloeveraPlant]->GetSparkleUpdatePoint()));
		isSparkleUpdatePending = true;
	}

	if(  !ProfileSystem::Instance->ActiveProfile.L2_S1_InvScrapperTaken)
	{
		sparklePointsVec.push_back(CPoint(PopUpArray[eHOPop5_Boat]->GetSparkleUpdatePoint()));
		isSparkleUpdatePending = true;
	}
}

void L2_S1_ForestEntrance::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;


	if( !ProfileSystem::Instance->ActiveProfile.L2_S1_InvPotWithWaterTaken  && ProfileSystem::Instance->ActiveProfile.L2_S5_InvPotTaken)
	{
		isAnyTaskLeft = true;
	}

	if( !ProfileSystem::Instance->ActiveProfile.L2_S1_IsUndergroundHatchOpened  && ProfileSystem::Instance->ActiveProfile.L2_S5_InvNagaSculptureTaken)
	{
		isAnyTaskLeft = true;
	}

	if( ProfileSystem::Instance->ActiveProfile.L2_S1_IsUndergroundHatchOpened  && !ProfileSystem::Instance->ActiveProfile.L2_S1_InvColorPowderTaken)
	{
		isAnyTaskLeft = true;
	}

	if( ProfileSystem::Instance->ActiveProfile.L2_S2_InvFrankinscenceTaken && !ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked)
	{
		isAnyTaskLeft = true;
	}

	if( ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken && !ProfileSystem::Instance->ActiveProfile.L2_S1_InvHoneyCombPicked && ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked)
	{
		isAnyTaskLeft = true;
	}

	if(  !ProfileSystem::Instance->ActiveProfile.L2_S1_InvStoneBallTaken)
	{
		isAnyTaskLeft = true;
	}

	if( !ProfileSystem::Instance->ActiveProfile.L2_S1_IsGlovesUsed && ProfileSystem::Instance->ActiveProfile.L2S6_InvGlovesTaken)
	{
		isAnyTaskLeft = true;
	}

	if( ProfileSystem::Instance->ActiveProfile.L2_S1_IsGlovesUsed && ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken && !ProfileSystem::Instance->ActiveProfile.L2_S1_InvAloveraTaken)
	{
		isAnyTaskLeft = true;
	}

	if(  !ProfileSystem::Instance->ActiveProfile.L2_S1_InvScrapperTaken)
	{
		isAnyTaskLeft = true;
	}

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S1_ForestEntrance] = isAnyTaskLeft;

	if( !ProfileSystem::Instance->ActiveProfile.L2_S2_ColorPlatePlaced && ProfileSystem::Instance->ActiveProfile.L2_S1_InvColorPowderTaken)
	{
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S2_NagaTemple] = true;
	}
	if( (ProfileSystem::Instance->ActiveProfile.L2_S2_EmberPlaced && !ProfileSystem::Instance->ActiveProfile.L2_S2_InvFrankinscenceTaken && ProfileSystem::Instance->ActiveProfile.L2_S1_InvScrapperTaken) ||
		//If 3 stones picked & not stone dropped  			
			(!ProfileSystem::Instance->ActiveProfile.L2_S2_PZStonePlaced && 
			ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone1Taken && 
			ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone2Taken && 
			ProfileSystem::Instance->ActiveProfile.L2S6_InvSmallStone3Taken )  )
	{
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S2_NagaTemple] = true;
	}

	if( (ProfileSystem::Instance->ActiveProfile.L2_S1_InvStoneBallTaken && ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened && !ProfileSystem::Instance->ActiveProfile.L2S6_InvStoneBallPlaced) ||
		//if 3 pearls picked & not dropped, direct them to tunnel end scene!! 
			(ProfileSystem::Instance->ActiveProfile.L2_S2_InvNagaPearlsCollected && !ProfileSystem::Instance->ActiveProfile.L2_S6_NagaPearlFixed) )
	{
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S6_TunnelEnd] = true;
	}

	if( (!ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraPlaced && ProfileSystem::Instance->ActiveProfile.L2_S1_InvAloveraTaken) || 
		(!ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxPlaced && ProfileSystem::Instance->ActiveProfile.L2_S1_InvHoneyCombPicked ) || 
		(!ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced &&	ProfileSystem::Instance->ActiveProfile.L2_S1_InvPotWithWaterTaken) ||
		//if 3 mango picked & not dropped, direct them to mooppan scene!! 
		(ProfileSystem::Instance->ActiveProfile.L2_S2_InvMangoesTaken && !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoPlaced) ||
		//if medicine picked & not given to mooppan, direct them to mooppan scene!! 
		(ProfileSystem::Instance->ActiveProfile.L2_S5_InvMedicineTaken && !ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineGivenToMooppan))
	{
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S5_MoopansDen] = true;
	}

	if (ProfileSystem::Instance->ActiveProfile.L2_S2_kolamComplete && !ProfileSystem::Instance->ActiveProfile.L2_S2_NagaLampLiteVideoPlayed)
	{
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S2_NagaTemple] = true;
	}
}

void L2_S1_ForestEntrance::SceneObjectsUpdate()
{
	if (!ProfileSystem::Instance->ActiveProfile.L2_S1_InvAloveraTaken)
	{
		SCN_cutAloevera->SetEnable(false);
	}
	else
	{
		SCN_cutAloevera->SetEnable(true);
	}

	if (ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked)
	{
		SCN_HoneyBee->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L2_S1_InvHoneyCombPicked)
	{
		SCN_HoneyCombUncut->SetEnable(false);
	}

	if( ProfileSystem::Instance->ActiveProfile.L2_S1_InvScrapperTaken)
	{
		SCN_Scrapper->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L2_S1_InvStoneBallTaken)
	{
		HOScene->GetObjectByID("round-stone")->SetEnable(false);
	}

	if (!ProfileSystem::Instance->ActiveProfile.L2_S1_IsUndergroundHatchOpened)
	{
		HOScene->GetObjectByID("open-manhole-off")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("open-manhole-off")->SetEnable(true);
	}

	TaskPendingUpdate();
}

void L2_S1_ForestEntrance::InitPopups()
{	
	ActivePopupID = ePopInactive;
}

void L2_S1_ForestEntrance::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if(isSwimCutscenePlaying)
		return;

	if( Control::Audio->IsPlaying(aTrackL2theme1))
		return;

	if( Control::Audio->IsPlaying(aTrackL2ho) )
	{
		Control::Audio->Samples[aTrackL2ho]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL2ho]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL2ho);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL2theme) )
				Control::Audio->PlaySample(aTrackL2theme, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else if( Control::Audio->IsPlaying(aTrackL2puzzle) )
	{
		Control::Audio->Samples[aTrackL2puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL2puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL2puzzle);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL2theme) )
				Control::Audio->PlaySample(aTrackL2theme, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL2theme))
		{
			Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL2theme, true);
		}
	}
}

void L2_S1_ForestEntrance::Update()
{
	//Cheats goes here
#ifdef _TABINGAMEUI
	if(_TabInGameMenuShowing)
		return;
#endif // _TABINGAMEUI
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	if (ActivePopupID != eHOS1Pop_Inactive)
		return;
	HiddenObjectSystem::HOUpdate();

	CPoint pos = Control::Input->Mpos();


	if(isSwimCutscenePlaying)
	{

		if(!cv_swimCutscene->isSkipMouseOver())
			Cursor::SetMode(CA_Normal);

		//Subtitles Code
		time = cv_swimCutscene->GetTime();

		if (time >= subtitles[subIndex].begin && time < subtitles[subIndex].end)
		{
			subtitleText->SetText(subtitles[subIndex].text);
		}
		else
			subtitleText->SetText("");

		if (time > subtitles[subIndex].end)
		{
			if (subMax - 1 > subIndex)
				subIndex++;
		}
#ifdef FORCE_CLOSE_VIDEO
		vidTimer += Control::LogicRate;
#endif
		if(
#ifndef FORCE_CLOSE_VID_END
			cv_swimCutscene->isEndReached() ||
#endif
			vidTimer > 20)
		{
			HOScene->Show(true);
			subtitleText->SetText("");
			isSwimCutscenePlaying = false;
			ProfileSystem::Instance->ActiveProfile.L2_S1_IsIntroSwimVideoPlayed = true;

			cv_swimCutscene->StopAndFadeOutVideo(2.0f);

			//Objective Add
			if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L2O1_MeetMooppan] )
			{
				ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L2O1_MeetMooppan] = true;
				ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
				= eObj_L2O1_MeetMooppan;
				//Increment Objectives
				ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
				Hud->ShowObjectiveFX(eObj_L2O1_MeetMooppan);

				Control::Audio->PlaySample(aSFXMagic1);
			}

			Hud->UnlockCharacter(eChar7_NaginMaryPast, 2.95f);

			//Journal Entry 1 Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L2P1_BoatAttackReflection])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L2P1_BoatAttackReflection] = true;
				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected]
					= eJrn_L2P1_BoatAttackReflection;
						
				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;
				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
				Hud->ShowJrnEntryFX();
			}

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L2P2_ReachedNarara])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L2P2_ReachedNarara] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L2P2_ReachedNarara;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
				Hud->ShowJrnEntryFX();
			}

			Hud->AllowInput = true;
			Hud->ShowHud(true);

			Hud->IsHintPaused = false;

			Hud->ShowBannerText("L2mopan");
			if( !Control::Audio->IsPlaying(aTrackL2theme1) )
				Control::Audio->PlaySample(aTrackL2theme1,false);


			Control::Audio->LoadSample(aAmbs1ForestEntrance,AudioAmb);
			if( !Control::Audio->IsPlaying(aAmbs1ForestEntrance) )		
			{
				Control::Audio->PlaySample(aAmbs1ForestEntrance,true);
			}


		}
		return;
	}


#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
#else
	Cursor::SetMode(CA_Normal);
#endif 

	if (ActivePopupID != ePopInactive)
		return;

	if (isMouseOverPopup)
	{
		Cursor::SetMode(CA_Look);
		return;
	}

	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed()) {
				if(Hud!=NULL && GFInstance->IsInGameMenuPopup != true)Hud->CallPauseMenu();
			}
	#endif
	if (SceneRectNagaTemple.Intersection(&pos))
	{


#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
			//Cursor::SetMode(CA_ExitUp, eArea_L2_S2_NagaTemple);
#else
		Cursor::SetMode(CA_ExitUp, eArea_L2_S2_NagaTemple);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L2_S2_NagaTemple, 0, SceneRectNagaTemple);
			}
#else
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S2_NagaTemple;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}

		return;
	}

	if (SceneRectCaveMouth.Intersection(&pos))
	{

#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
			//Cursor::SetMode(CA_ExitRight, eArea_L2_S3_CaveMouth);
#else
		Cursor::SetMode(CA_ExitRight, eArea_L2_S3_CaveMouth);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L2_S3_CaveMouth, 1, SceneRectCaveMouth);
			}
#else
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S3_CaveMouth;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}

		return;
	}

	if( ActivePopupID == ePopInactive && SceneRectUndergroundHO.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L2_S1_IsNagaSculpturePlaced && !ProfileSystem::Instance->ActiveProfile.L2_S1_InvColorPowderTaken)
	{

#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_Look);
#else
		Cursor::SetMode(CA_Look);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L2_S1_UndergroundHO, 0, SceneRectUndergroundHO);
			}
#else
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S1_UndergroundHO;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}			
		return;
	}

	if (pos.y > 550 || pos.y < 100 || pos.x > 1200 || pos.x < 100)
	{
		Cursor::SetMode(CA_Help);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L2nature");
		}
	}
}

void L2_S1_ForestEntrance::onPopupClosed(Popup * popup)
{
	SceneObjectsUpdate();

	ActivePopupID = eHOS1Pop_Inactive;

	switch (popup->GetPopupId())
	{
	case eHOPop1_HoneyComb:
		if(ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked)
		{
			if(BeesFX!=NULL)
				BeesFX->StopImmediate();
		}
		break;
	case eHOPop2_UndergroundDoor:
		if(ProfileSystem::Instance->ActiveProfile.L2_S1_IsNagaSculpturePlaced)
		{
			/*if(pop2UndergroundDoor->isToUndergroundAreaClicked)
			{
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S1_UndergroundHO;
			SendMessage(ControlExit, ID, HOSM_NewArea);
			}*/
			//Start HO Sparkles
			if( !ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles )
			{
				holinesFX->Start();
				holinesFX->AdvanceByTime(2.0f);

				hodotsFX->Start();
				hodotsFX->AdvanceByTime(2.0f);
			}
		}
		break;
	default:
		break;
	}
}

L2_S1_ForestEntrance::~L2_S1_ForestEntrance()
{
	TaskPendingUpdate();
	if(BeesFX)
		SAFE_DELETE(BeesFX);

	SAFE_DELETE(pop1HoneyComb);
	SAFE_DELETE(pop2UndergroundDoor);
	SAFE_DELETE(pop3AloeveraPlant);
	SAFE_DELETE(pop4CrystalClearWater);
	SAFE_DELETE(pop5Scrapter);	

	SAFE_DELETE(pop1ClickPoly);
	SAFE_DELETE(pop2ClickPoly);
	SAFE_DELETE(pop3ClickPoly);
	SAFE_DELETE(pop4ClickPoly);
	SAFE_DELETE(pop5ClickPoly);

	SAFE_DELETE(cv_swimCutscene);
	SAFE_DELETE(holinesFX);
	SAFE_DELETE(hodotsFX);
	SAFE_DELETE(subtitleText);
	//	SAFE_DELETE(SCN_pondWater);	

	//MUSIC_SFX
	//Quick SFX unloaded in HUD
	Control::Audio->StopSample(aAmbs1ForestEntrance);
	Control::Audio->UnloadSample(aAmbs1ForestEntrance);
	//MUSIC_SFX
}

void L2_S1_ForestEntrance::ReceiveMessage(int val)
{
	if( val == HOSM_DialogClosed ) 
	{
		//isDialogActive = false;
		Hud->AllowInput = true;
		Hud->ShowHud(true);

		isSparkleUpdatePending = true;
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
			/*switch (ActivePopupID)
			{
			case eHOPop1_HoneyComb:
			pop1HoneyComb->HandleInventoryDrop(dropPos, itemVal);
			break;
			case eHOPop2_UndergroundDoor:
			pop2UndergroundDoor->HandleInventoryDrop(dropPos, itemVal);
			break;
			case eHOPop3_AloeveraPlant:
			pop3AloeveraPlant->HandleInventoryDrop(dropPos, itemVal);
			break;
			case eHOPop5_Boat:
			pop5Scrapter->HandleInventoryDrop(dropPos, itemVal);
			break;
			case eHOPop4_CrystalClearWater:
			pop4CrystalClearWater->HandleInventoryDrop(dropPos, itemVal);
			break;
			default:				
			break;
			}*/

			if (ActivePopupID != ePopInactive)
			{
				/*if (itemVal == eInv_L2S5_PotEmpty && !ProfileSystem::Instance->ActiveProfile.L2_S1_InvPotWithWaterTaken)
				{

				pop4CrystalClearWater->OpenPopup();
				ActivePopupID = eHOPop4_CrystalClearWater;
				}
				else
				{*/
				switch (ActivePopupID)
				{
				case eHOPop1_HoneyComb:
					pop1HoneyComb->HandleInventoryDrop(dropPos, itemVal);
					break;
				case eHOPop2_UndergroundDoor:
					pop2UndergroundDoor->HandleInventoryDrop(dropPos, itemVal);
					break;
				case eHOPop3_AloeveraPlant:
					pop3AloeveraPlant->HandleInventoryDrop(dropPos, itemVal);
					break;
				case eHOPop5_Boat:
					pop5Scrapter->HandleInventoryDrop(dropPos, itemVal);
					break;
				case eHOPop4_CrystalClearWater:
					pop4CrystalClearWater->HandleInventoryDrop(dropPos, itemVal);
					break;
				default:
					break;
				}
				return;
			}
			//If no drop items in this scene !!
			GFInstance->WrongInvDropMsg(Hud);
			/*}*/
		}
		/*else
		GFInstance->WrongInvDropMsg(Hud);*/
	}
}

void L2_S1_ForestEntrance::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S1Collected)
		{
			InitHintTrailEmit(670, 345);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}

	if (ActivePopupID != ePopInactive)
	{
		if(PopUpArray[ActivePopupID]!=NULL)
			PopUpArray[ActivePopupID]->ProcessHint();
		return;
	}

	int iNonHOHint = 0; 
	int MaxNonHOHints = 12;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
		case 0: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L2_S1_InvPotWithWaterTaken  && ProfileSystem::Instance->ActiveProfile.L2_S5_InvPotTaken)
			{
				InitHintTrailEmit(	pop4ClickPoly->center.x, pop4ClickPoly->center.y,false);
				return;
			}
			break;
		case 1: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L2_S1_IsUndergroundHatchOpened  && ProfileSystem::Instance->ActiveProfile.L2_S5_InvNagaSculptureTaken)
			{
				InitHintTrailEmit(	pop2ClickPoly->center.x, pop2ClickPoly->center.y,false);
				return;
			}
			break;
		case 2: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L2_S1_IsUndergroundHatchOpened  && !ProfileSystem::Instance->ActiveProfile.L2_S1_InvColorPowderTaken)
			{
				InitHintTrailEmit(	pop2ClickPoly->center.x, pop2ClickPoly->center.y,false);
				return;
			}
			break;
		case 3: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L2_S2_InvFrankinscenceTaken && !ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked)
			{
				InitHintTrailEmit(	pop1ClickPoly->center.x, pop1ClickPoly->center.y,false);
				return;
			}
			break;
		case 4: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken && !ProfileSystem::Instance->ActiveProfile.L2_S1_InvHoneyCombPicked && ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked)
			{
				InitHintTrailEmit(	pop1ClickPoly->center.x, pop1ClickPoly->center.y,false);
				return;
			}
			break;
		case 5: ++iNonHOHint;
			if(  !ProfileSystem::Instance->ActiveProfile.L2_S1_InvStoneBallTaken)
			{
				InitHintTrailEmit(	pop3ClickPoly->center.x, pop3ClickPoly->center.y,false);
				return;
			}
			break;
		case 6: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L2_S1_IsGlovesUsed && ProfileSystem::Instance->ActiveProfile.L2S6_InvGlovesTaken)
			{
				InitHintTrailEmit(	pop3ClickPoly->center.x, pop3ClickPoly->center.y,false);
				return;
			}
			break;
		case 7: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L2_S1_IsGlovesUsed && ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken && !ProfileSystem::Instance->ActiveProfile.L2_S1_InvAloveraTaken)
			{
				InitHintTrailEmit(	pop3ClickPoly->center.x, pop3ClickPoly->center.y,false);
				return;
			}
			break;
		case 8: ++iNonHOHint;
			if(  !ProfileSystem::Instance->ActiveProfile.L2_S1_InvScrapperTaken)
			{
				InitHintTrailEmit(	pop5ClickPoly->center.x, pop5ClickPoly->center.y,false);
				return;
			}
			break;
		case 9: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S2_NagaTemple] || !ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L2_S2_NagaTemple])
			{
				InitHintTrailEmit(SceneRectNagaTemple.GetCenterX(), SceneRectNagaTemple.GetCenterY(),true,CA_ExitUp);
				return;
			}
			break;
		case 10: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S3_CaveMouth] || !ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L2_S3_CaveMouth])
			{
				InitHintTrailEmit(SceneRectCaveMouth.GetCenterX(), SceneRectCaveMouth.GetCenterY(),true,CA_ExitRight);
				return;
			}
			break;
		case 11: ++iNonHOHint;
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


void L2_S1_ForestEntrance::NavigateToScene()
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