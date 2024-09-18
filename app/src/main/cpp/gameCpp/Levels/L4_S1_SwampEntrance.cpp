//====================================
// DQFULP
// Hidden Object System
// Author : Sajith
// LEVEL4 - SCENE1 : Swamp Entrance
//====================================

#include "L4_S1_SwampEntrance.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "TextureFactory.h"
#include "ParticleEffect.h"
#include "Rect.h"
#include "Circle.h"
#include "Polygon.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "L4S1_Pop4Crutch.h"
#include "L4S1_Pop5TwigSquarePopup.h"

const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  PopCircleRect(484.0f,154.0f,376.0f,368.0f);

CRectangle  RightLegRect(391.0f,196.0f,258.0f,223.0f);
CRectangle  WaterPopupRect(941.0f,450.0f,249.0f,249.0f);

CRectangle  PocketRect(768.0f,472.0f,153.0f,161.0f);
CRectangle  medallionPopRect(1075.0f,275.0f,126.0f,150.0f);

CRectangle  MushroomMiniGameRect(479.0f,22.0f,74.0f,123.0f);
const CRectangle  SceneRectShoeHO(11.0f,310.0f,458.0f,388.0f);

const CRectangle  SceneRectRiverSide(736.0f,100.0f,279.0f,169.0f);

const CRectangle  InvDestRectLeftShoe(636.0f,373.0f,133.0f,137.0f);
const CRectangle  InvDestRectCrutches(455.0f,200.0f,537.0f,574.0f);

#define PZ_ID	4
#define FADE_OUT_SPEED 0.25

enum eL4S1HOPopEnums
{
	ePop1_RightLeg,
	ePop2_HealingObjects,
	ePop3_Pocket,
	ePop5_TwigSquare,
	ePop_Max
};

int WaterArrayPts[] = {1145,317,1090,315,1063,346,1091,398,1130,475,1146,514,1057,580,1140,612,1196,628,1215,610,1208,577,1259,553,1284,565,1321,551,1332,522,1355,524,1360,399,1144,314};
	
L4_S1_SwampEntrance::L4_S1_SwampEntrance()
{

	WaterFallFX			= NULL;
	WaterFallBottomFX	= NULL;
	GlowPlant_FX		= NULL;
	GlowPlant_FX1		= NULL;
	holinesFX			= NULL;
	hodotsFX			= NULL;
	waterAreaPoly		= NULL;
	


	LoadHud(INVENTORY_V1);
	LoadScene("LV4\\S1\\S1.SCN");

	CPoint Pop1center;
	L4S1_Pop1RightFoot *pop1RightLeg;
	Pop1center.x = RightLegRect.GetCenterX();
	Pop1center.y = RightLegRect.GetCenterY();
	PopRectArray.push_back(&RightLegRect);
	pop1RightLeg = new L4S1_Pop1RightFoot(ePop1_RightLeg, Hud, HOScene, eZOrder_Popup, Pop1center, CC_CALLBACK_1(L4_S1_SwampEntrance::OnPopupClosed, this));
	pop1RightLeg->SetPopupBoundingBox(PopCircleRect);
	PopupArray.push_back(pop1RightLeg);

	CPoint Pop2center;
	L4S1_Pop2HealingObjects *pop2HealingObjects;
	Pop2center.x = WaterPopupRect.GetCenterX();
	Pop2center.y = WaterPopupRect.GetCenterY();
	pop2HealingObjects= new L4S1_Pop2HealingObjects(ePop2_HealingObjects, Hud, HOScene, eZOrder_Popup, Pop2center, CC_CALLBACK_1(L4_S1_SwampEntrance::OnPopupClosed, this));
	pop2HealingObjects->SetPopupBoundingBox(PopAreaRect);
	PopRectArray.push_back(&WaterPopupRect);
	PopupArray.push_back(pop2HealingObjects);

	CPoint Pop3center;
	L4S1_Pop3Pocket *pop3Pocket;
	Pop3center.x = PocketRect.GetCenterX();
	Pop3center.y = PocketRect.GetCenterY();
	pop3Pocket = new L4S1_Pop3Pocket(ePop3_Pocket, Hud, HOScene, eZOrder_Popup, Pop3center, CC_CALLBACK_1(L4_S1_SwampEntrance::OnPopupClosed, this));
	pop3Pocket->SetPopupBoundingBox(PopCircleRect);
	PopRectArray.push_back(&PocketRect);
	PopupArray.push_back(pop3Pocket);

	CPoint Pop5center;
	L4S1_Pop5TwigSquarePopup *pop5;
	Pop5center.x = medallionPopRect.GetCenterX(); 
	Pop5center.y = medallionPopRect.GetCenterY();
	pop5 = new L4S1_Pop5TwigSquarePopup(ePop5_TwigSquare, Hud, HOScene, eZOrder_Popup, Pop5center, CC_CALLBACK_1(L4_S1_SwampEntrance::OnPopupClosed, this));
	pop5->SetPopupBoundingBox(PopAreaRect);
	PopRectArray.push_back(&medallionPopRect);
	PopupArray.push_back(pop5);

	CPoint PZ_Center;
	PZ_Center.x = MushroomMiniGameRect.GetCenterX();
	PZ_Center.y = MushroomMiniGameRect.GetCenterY();
	PZ_Mushroom = new L4_S1_PZMushroom(PZ_ID, Hud, HOScene, eZOrder_Popup, PZ_Center, CC_CALLBACK_1(L4_S1_SwampEntrance::OnPopupClosed, this));

	resetBtn = new CObject();
	resetBtn->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\RESETBTN.SPR", 683, 125, eZOrder_Popup + 100);
	resetBtn->SetUID("resetBtn");
	PZ_Mushroom->PushObject(resetBtn);
	resetBtn->PlayAnimation(0);
	resetBtn->Show(false);

	mo_water = NULL;
	mo_water = new MeshObject(HOScene->GetObjectByID("PondAnimation"), 5, 5);
	mo_water->meshFX->SetSpeedAndDisplacement(1, 2, 2, 3);
	//HOScene->PushObject(mo_water);
	HOScene->InsertObject(mo_water,2);

	

	//HOScene->GetObjectByID("AlgeyWOBracelet")->Animate(1,2,0,0,3,2);
	HOScene->GetObjectByID("AlgeyWithBracelet")->Animate(1,2,0,0,3,2);

	HOScene->GetObjectByID("CactusPlantForeground")->Animate(1,0,0,0,5,3);
	HOScene->GetObjectByID("Magical_Plants")->SetPivot(168, 155);
	HOScene->GetObjectByID("Magical_Plants")->Animate(0,0,1.2f,0,7,3);
	HOScene->GetObjectByID("MushroomGlowBg")->Animate(0,1,5);
	HOScene->GetObjectByID("TreeGhostGlowBg01")->Animate(0,1,6);
	HOScene->GetObjectByID("scarletskimmer")->SetScale(0.25f);
	HOScene->GetObjectByID("scarletskimmer")->SetAnimSpeed(5.0f);
	HOScene->GetObjectByID("scarletskimmer")->SetRelativePos(PointSystem::CreateCPoint(80, 48, 0));

	for (int i = 0; i < 3; i++)
	{
		HOScene->GetObjectByID("LightRays0" + MKSTR(i + 2))->Animate(0.8f,1.4f,8);
	}

	for (int i = 0; i < 3; i++)
	{
		HOScene->GetObjectByID("LongGrassLeaves0" + MKSTR(i + 1))->Animate(1.5f,0,0,0,5,(float)i);
	}

	HOScene->GetObjectByID("Character_RT_leg2")->SetPivot(-160,0);
	HOScene->GetObjectByID("Character_RT_leg2")->Animate(0,0,0.25,0,2.5);
	HOScene->GetObjectByID("Character_RT_leg1")->SetPivot(0,144);
	HOScene->GetObjectByID("Character_RT_leg1")->Animate(0,0,0.25,0,2.5);
	//Scene->GetObjectByID("Character_LT_legfeet")->Animate(0,2,0,0,2);
	activePopUPId = ePopInactive;


#ifdef _CEBUILD
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S1Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("morphtarget"), HOScene->GetObjectByID("morphbase"));
		CRectangle  MorphRect(1042.0f,156.0f,62.0f,75.0f);

		Hud->setMorphObjRect(MorphRect);
	}
	else
	{
		HOScene->GetObjectByID("morphtarget")->SetEnable(false);
	}
#else
	HOScene->GetObjectByID("morphtarget")->SetEnable(false);
#endif 

	waterAreaPoly =  new CPolygon(18);
	waterAreaPoly->Init(WaterArrayPts);


	WaterFallFX = new SuperFX("PRT\\L4\\S1_WaterFallSmoke.INI", PointSystem::CreateCPoint(888,137,0), 80); 
	WaterFallFX->Start();
	WaterFallFX->AdvanceByTime(2.0f);

	WaterFallBottomFX = new SuperFX("PRT\\L4\\S1_WaterFallSmoke_1.INI", PointSystem::CreateCPoint(888,210,0), 80); 
	WaterFallBottomFX->Start();
	WaterFallBottomFX->AdvanceByTime(2.0f);

	GlowPlant_FX = new SuperFX("PRT\\L4\\CaveGlowPlant4.INI", PointSystem::CreateCPoint(245,300,0), 100); 
	GlowPlant_FX->Start();
	GlowPlant_FX->AdvanceByTime(2.0f);

	GlowPlant_FX1 = new SuperFX("PRT\\L4\\CaveGlowPlant1.INI", PointSystem::CreateCPoint(1280,230,0), 100); 
	GlowPlant_FX1->Start();
	GlowPlant_FX1->AdvanceByTime(2.0f);

	holinesFX = new SuperFX("PRT\\L2\\holines.INI", PointSystem::CreateCPoint(240,410,0), 100); 

	hodotsFX = new SuperFX("PRT\\L2\\hodots.INI", PointSystem::CreateCPoint(234,417,0), 100); 

	if( !ProfileSystem::Instance->ActiveProfile.L4_S1_HOShoeCompleted)
	{
		if( !ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles )
		{
			holinesFX->Start();
			holinesFX->AdvanceByTime(2.0f);

			hodotsFX->Start();
			hodotsFX->AdvanceByTime(2.0f);
		}
	}

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL4s1SwapEntranceAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL4s1SwapEntranceAmb) )		
	{
		Control::Audio->PlaySample(aAmbL4s1SwapEntranceAmb,true);
	}

	BannerTimer = -1;

	//MUSIC_SFX
	SceneObjectsUpdate();



}


void L4_S1_SwampEntrance::SceneObjectsUpdate()
{
	TaskPendingUpdate();

	if (ProfileSystem::Instance->ActiveProfile.L4_S1_InvCrutchTaken)
	{
		HOScene->GetObjectByID("Crutch")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S1_FirstThornRemoved)
	{
		HOScene->GetObjectByID("Character_Thorn01")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S1_SecondThornRemoved)
	{
		HOScene->GetObjectByID("Character_Thorn02")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S1_ShoeRemoved && !ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack)
	{
		HOScene->GetObjectByID("Character_RT_legShoe")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("Character_RT_legShoe")->SetEnable(true);
	}

	HOScene->GetObjectByID("Character_LT_legShoe")->SetEnable(false);

	if (ProfileSystem::Instance->ActiveProfile.L4_S1_PutLeftShoe /*&& ! ProfileSystem::Instance->ActiveProfile.L4_S1_CrutchUsed*/)
	{
		HOScene->GetObjectByID("Character_LT_legShoe")->SetEnable(true);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S1_InvBraceletTaken)
	{
		HOScene->GetObjectByID("AlgeyWithBracelet")->SetEnable(false);
		//HOScene->GetObjectByID("AlgeyWOBracelet")->SetEnable(true);
	}
	else 
	{
		//	HOScene->GetObjectByID("AlgeyWOBracelet")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafTaken)
	{
		HOScene->GetObjectByID("Single_leaf")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S1_InvStickTaken)
	{
		HOScene->GetObjectByID("stick_plant")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S1_SkimmerFlyed)
	{
		HOScene->GetObjectByID("scarletskimmer")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S1_MedPasteApplied)
	{
		HOScene->GetObjectByID("PasteOnLeg")->SetEnable(true);
	}
	else
	{
		HOScene->GetObjectByID("PasteOnLeg")->SetEnable(false);
	}

	HOScene->GetObjectByID("Kerchief_Scene")->SetEnable(false);

	if (ProfileSystem::Instance->ActiveProfile.L4_S1_PocketOpend)
	{
		HOScene->GetObjectByID("PocketOpenScene")->SetEnable(true);
		if (!ProfileSystem::Instance->ActiveProfile.L4_S1_InvHandKerchiefTaken)
		{
			HOScene->GetObjectByID("Kerchief_Scene")->SetEnable(true);
		}
	}
	else
	{
		HOScene->GetObjectByID("PocketOpenScene")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S1_WoundTied)
	{
		HOScene->GetObjectByID("Cloth_HealingObj")->SetEnable(true);
	}
	else
	{
		HOScene->GetObjectByID("Cloth_HealingObj")->SetEnable(false);
	}

	//if (ProfileSystem::Instance->ActiveProfile.L4_S1_BetelBoxOPened)
	{
		HOScene->GetObjectByID("DoorVettila_chellam01")->SetEnable(false);
	}

	if (!ProfileSystem::Instance->ActiveProfile.L4_S1_BloodRemoved)
	{
		HOScene->GetObjectByID("LegCleaned")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("LegCleaned")->SetEnable(true);
	}

	/*if (ProfileSystem::Instance->ActiveProfile.L4_S1_CrutchUsed)*/
	if(ProfileSystem::Instance->ActiveProfile.L4_S1_PutLeftShoe && ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack)
	{
		HOScene->GetObjectByID("Character_Body-")->SetEnable(false);
		HOScene->GetObjectByID("Character_LT_leg1")->SetEnable(false);
		HOScene->GetObjectByID("Character_LT_leg2")->SetEnable(false);
		HOScene->GetObjectByID("Character_LT_legShoe")->SetEnable(false);
		HOScene->GetObjectByID("Character_LT_legfeet")->SetEnable(false);
		HOScene->GetObjectByID("Character_RT_leg1")->SetEnable(false);
		HOScene->GetObjectByID("Character_RT_leg2")->SetEnable(false);
		HOScene->GetObjectByID("Character_RT_legShoe")->SetEnable(false);
		HOScene->GetObjectByID("Character_RT_legFeet")->SetEnable(false);
		HOScene->GetObjectByID("PocketOpenScene")->SetEnable(false);
		HOScene->GetObjectByID("Cloth_HealingObj")->SetEnable(false);
		HOScene->GetObjectByID("LegCleaned")->SetEnable(false);
	}
}

void L4_S1_SwampEntrance::InitPopups()
{
	activePopUPId = ePopInactive;
}

void L4_S1_SwampEntrance::OnPopupClosed(Popup * popup)
{
	SceneObjectsUpdate();

	activePopUPId = ePopInactive;

	switch (popup->GetPopupId())
	{
	case ePop1_RightLeg:
		if (ProfileSystem::Instance->ActiveProfile.L4_S1_PutLeftShoe && ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack)
		{
			HOScene->GetObjectByID("Character_Body-")->FadeOut(FADE_OUT_SPEED);
			HOScene->GetObjectByID("Character_LT_leg1")->FadeOut(FADE_OUT_SPEED);
			HOScene->GetObjectByID("Character_LT_leg2")->FadeOut(FADE_OUT_SPEED);
			HOScene->GetObjectByID("Character_LT_legShoe")->FadeOut(FADE_OUT_SPEED);
			HOScene->GetObjectByID("Character_LT_legfeet")->FadeOut(FADE_OUT_SPEED);
			HOScene->GetObjectByID("Character_RT_leg1")->FadeOut(FADE_OUT_SPEED);
			HOScene->GetObjectByID("Character_RT_leg2")->FadeOut(FADE_OUT_SPEED);
			HOScene->GetObjectByID("Character_RT_legShoe")->FadeOut(FADE_OUT_SPEED);
			HOScene->GetObjectByID("Character_RT_legFeet")->FadeOut(FADE_OUT_SPEED);
			HOScene->GetObjectByID("PocketOpenScene")->FadeOut(FADE_OUT_SPEED);
			HOScene->GetObjectByID("Cloth_HealingObj")->FadeOut(FADE_OUT_SPEED);
			HOScene->GetObjectByID("LegCleaned")->FadeOut(FADE_OUT_SPEED);

			//Show Objective completed
			if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L4O1_BandageTheWound] && 
				!ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L4O1_BandageTheWound] )
			{
				ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L4O1_BandageTheWound] = true;
				Hud->ShowObjectiveFX(eObj_L4O1_BandageTheWound,true);
			}

			//Objective Add
			if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L4O2_FindWayOutOfForest] )
			{
				ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L4O2_FindWayOutOfForest] = true;
				ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
				= eObj_L4O2_FindWayOutOfForest;
				//Increment Objectives
				ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
				Hud->ShowObjectiveFX(eObj_L4O2_FindWayOutOfForest);

				Control::Audio->PlaySample(aSFXMagic1);
			}
		}
		break;
	case 4:
		Hud->InvBox_StopPuzzleMode();
		break;
	default:
		break;
	}
}

L4_S1_SwampEntrance::~L4_S1_SwampEntrance()
{
	for (int i = 0; i < (int)PopupArray.size(); i++)
	{
		SAFE_DELETE(PopupArray.at(i));
	}
	PopupArray.clear();
	SAFE_DELETE(WaterFallFX);
	SAFE_DELETE(WaterFallBottomFX);
	SAFE_DELETE(GlowPlant_FX);
	SAFE_DELETE(GlowPlant_FX1);
	SAFE_DELETE(holinesFX);
	SAFE_DELETE(hodotsFX);
	SAFE_DELETE(waterAreaPoly);
	

	//MUSIC_SFX

	Control::Audio->StopSample(aAmbL4s1SwapEntranceAmb);
	Control::Audio->UnloadSample(aAmbL4s1SwapEntranceAmb);
	//MUSIC_SFX
}

void L4_S1_SwampEntrance::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Control::Audio->IsPlaying(aTrackL4ho) )
	{
		Control::Audio->Samples[aTrackL4ho]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL4ho]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL4ho);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL4theme) )
				Control::Audio->PlaySample(aTrackL4theme, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else if( Control::Audio->IsPlaying(aTrackL4puzzle) )
	{
		Control::Audio->Samples[aTrackL4puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL4puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL4puzzle);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL4theme) )
				Control::Audio->PlaySample(aTrackL4theme, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL4theme))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL4theme, true);
		}
	}
}

void L4_S1_SwampEntrance::Update()
{
	//Cheats goes here
#ifdef _TABINGAMEUI
	if(_TabInGameMenuShowing)
		return;
#endif // _TABINGAMEUI
#ifdef _PARTICLEEDITORENABLED
	if (_isParticleEditorActive)
	{
		return;
	}
#endif

	NavigateToScene();
	HiddenObjectSystem::HOUpdate();

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	if (activePopUPId != ePopInactive)
		return;

	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed()) {
				if(Hud!=NULL && GFInstance->IsInGameMenuPopup != true)Hud->CallPauseMenu();
			}
	#endif
	
#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
#else
	Cursor::SetMode(CA_Normal);
#endif 


	if( BannerTimer >= 0.0f  &&  BannerTimer <= 6.0f )
	{
		BannerTimer += Control::LogicRate;
		return;
	}

	if (ProfileSystem::Instance->ActiveProfile.L4_S1_IsFirstVisit)
	{
		ProfileSystem::Instance->ActiveProfile.L4_S1_IsFirstVisit = false;
		Hud->ShowBannerText("L4ouch");

		BannerTimer = 0;
		//Objective Add
		if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L4O1_BandageTheWound] )
		{
			ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L4O1_BandageTheWound] = true;
			ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
			= eObj_L4O1_BandageTheWound;
			//Increment Objectives
			ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
			Hud->ShowObjectiveFX(eObj_L4O1_BandageTheWound);

			Control::Audio->PlaySample(aSFXMagic1);
		}


	}

	CPoint pos = Control::Input->Mpos();
	for (int i = 0; i < (int)(PopupArray.size()); i++)
	{
		if( PopRectArray[i]->Intersection(&pos) && isPopupActive(i))
		{
			Cursor::SetMode(CA_Look);
			if (Control::Input->LBclicked())
			{
				activePopUPId = i;
				PopupArray[i]->OpenPopup();
			}
			return;
		} 
	}

	if( SceneRectRiverSide.Intersection(&pos))
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitUp,eArea_L4_S2_RiverSide);
#else
		Cursor::SetMode(CA_ExitUp,eArea_L4_S2_RiverSide);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				if (/*ProfileSystem::Instance->ActiveProfile.L4_S1_CrutchUsed && */ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack && ProfileSystem::Instance->ActiveProfile.L4_S1_PutLeftShoe)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L4_S2_RiverSide, 0, SceneRectRiverSide);	
				}
				else
				{
					Hud->ShowBannerText("L4ahead");
				}	
					
				}
#else			
			if (/*ProfileSystem::Instance->ActiveProfile.L4_S1_CrutchUsed && */ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack && ProfileSystem::Instance->ActiveProfile.L4_S1_PutLeftShoe)
			{
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S2_RiverSide;
				SendMessage(ControlExit, ID, HOSM_NewArea);		
			}
			else
			{
				Hud->ShowBannerText("L4ahead");
			}	
#endif
						
		}
	}
	else if (MushroomMiniGameRect.Intersection(& pos))
	{
		if (!ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack)
		{
			if (Control::Input->LBclicked())
				Hud->ShowBannerText("L4edible");
		}
		else if (!ProfileSystem::Instance->ActiveProfile.L4_S1_InvBinocularLensTaken && ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeftShoeTaken)
		{
			Cursor::SetMode(CA_Look);
			if (Control::Input->LBclicked())
			{
				activePopUPId = PZ_ID;
				PZ_Mushroom->OpenPopup();
				PZ_Mushroom->ResetTouch();
				Hud->InvBox_StartPuzzleMode();
			}
		}
	}
	else if (SceneRectShoeHO.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S1_HOShoeCompleted)
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
				GFHud->InvBoxCtrl->GoToScene(eArea_L4_S1_ShoeHO, 0, SceneRectShoeHO);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S1_ShoeHO;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
						
		}
	}
	else if (InvDestRectLeftShoe.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S1_PutLeftShoe)
	{
		Cursor::SetMode(CA_Gear);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L4left");
		}
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L4_S1_BloodRemoved && ProfileSystem::Instance->ActiveProfile.L4_S1_ShoeRemoved)
	{
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L4better");
		}
	}
	else if (ProfileSystem::Instance->ActiveProfile.L4_S1_BloodRemoved && !ProfileSystem::Instance->ActiveProfile.L4_S1_MedPasteApplied)
	{
		if (Control::Input->LBclicked())
			Hud->ShowBannerText("L4apply");
	}
	else if (ProfileSystem::Instance->ActiveProfile.L4_S1_MedPasteApplied && !ProfileSystem::Instance->ActiveProfile.L4_S1_WoundTied)
	{
		if (Control::Input->LBclicked())
			Hud->ShowBannerText("L4bandage");
	}
	else if (ProfileSystem::Instance->ActiveProfile.L4_S1_WoundTied && (!ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack || !ProfileSystem::Instance->ActiveProfile.L4_S1_PutLeftShoe))
	{
		if (Control::Input->LBclicked())
			Hud->ShowBannerText("L4hurt");
	}
	else if (!ProfileSystem::Instance->ActiveProfile.L4_S1_FirstThornRemoved || !ProfileSystem::Instance->ActiveProfile.L4_S1_SecondThornRemoved)
	{
		if (Control::Input->LBclicked())
			Hud->ShowBannerText("L4thorns");
	}
	/*else if (ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack && !ProfileSystem::Instance->ActiveProfile.L4_S1_CrutchUsed)
	{
	if (Control::Input->LBclicked())
	Hud->ShowBannerText("It still hurts. I need a support of some kind. That branch looks promising.");//Sac: where is the external text for this ?
	}*/
}

void L4_S1_SwampEntrance::ProcessSkip()
{
	PZ_Mushroom->ProcessSkip();
}

bool L4_S1_SwampEntrance::isPopupActive(int popId)
{

	switch (popId)
	{
	case ePop1_RightLeg:
		{
			if (!ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack)	
				return true;	
		}
		break;
	case ePop2_HealingObjects:
		{
#ifdef _CEBUILD
			if (!ProfileSystem::Instance->ActiveProfile.L4_S1_InvHerbalLeavesTaken || !ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafSpoonWithWaterTaken || 
				!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S1Pumpkin] )	
#else
			if (!ProfileSystem::Instance->ActiveProfile.L4_S1_InvHerbalLeavesTaken || !ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafSpoonWithWaterTaken )	
#endif 
				return true;
		}break;
	case ePop3_Pocket:	
		{if (!ProfileSystem::Instance->ActiveProfile.L4_S1_InvHandKerchiefTaken)
			return true;

		}break;
	case ePop5_TwigSquare:
		{
			if (!ProfileSystem::Instance->ActiveProfile.L4_S1_InvBraceletTaken && ProfileSystem::Instance->ActiveProfile.L4_S1_PutLeftShoe && ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack)
				return true;
		}
		break;
	default:
		break;
	}

	return false;
}

void L4_S1_SwampEntrance::Render(const CPoint* pCam)
{

}

void L4_S1_SwampEntrance::ReceiveMessage(int val)
{	

	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		//TaskPendingUpdate();
		Hud->ShowMap();
	}
	else if( val == UPDATE_SCENE_OBJECTS )
	{
		SceneObjectsUpdate();
		if (activePopUPId == PZ_ID)
		{
			PZ_Mushroom->UpdateBgGraphic();
		}
		else if(activePopUPId != ePopInactive)
		{
			if(PopupArray[activePopUPId]!=NULL)
				PopupArray[activePopUPId]->UpdateBgGraphic();
		}
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )
		{
			//If no drop items in this scene !!
			//GFInstance->WrongInvDropMsg(Hud);
			if (activePopUPId != ePopInactive)
			{
				PopupArray[activePopUPId]->HandleInventoryDrop(dropPos, itemVal);
			}
			else if (itemVal == eInv_L4_S1_Shoe1)
			{
				if (InvDestRectLeftShoe.Intersection(&dropPos) && !ProfileSystem::Instance->ActiveProfile.L4_S1_PutLeftShoe)
				{
					ProfileSystem::Instance->ActiveProfile.L4_S1_PutLeftShoe = true;
					HOScene->GetObjectByID("Character_LT_legShoe")->SetEnable(true);
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_Shoe1] = false;
					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S1_Shoe1);

					if (ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack)
					{
						HOScene->GetObjectByID("Character_Body-")->FadeOut(FADE_OUT_SPEED);
						HOScene->GetObjectByID("Character_LT_leg1")->FadeOut(FADE_OUT_SPEED);
						HOScene->GetObjectByID("Character_LT_leg2")->FadeOut(FADE_OUT_SPEED);
						HOScene->GetObjectByID("Character_LT_legShoe")->FadeOut(FADE_OUT_SPEED);
						HOScene->GetObjectByID("Character_LT_legfeet")->FadeOut(FADE_OUT_SPEED);
						HOScene->GetObjectByID("Character_RT_leg1")->FadeOut(FADE_OUT_SPEED);
						HOScene->GetObjectByID("Character_RT_leg2")->FadeOut(FADE_OUT_SPEED);
						HOScene->GetObjectByID("Character_RT_legShoe")->FadeOut(FADE_OUT_SPEED);
						HOScene->GetObjectByID("Character_RT_legFeet")->FadeOut(FADE_OUT_SPEED);
						HOScene->GetObjectByID("PocketOpenScene")->FadeOut(FADE_OUT_SPEED);
						HOScene->GetObjectByID("Cloth_HealingObj")->FadeOut(FADE_OUT_SPEED);
						HOScene->GetObjectByID("LegCleaned")->FadeOut(FADE_OUT_SPEED);

						//Show Objective completed
						if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L4O1_BandageTheWound] && 
							!ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L4O1_BandageTheWound] )
						{
							ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L4O1_BandageTheWound] = true;
							Hud->ShowObjectiveFX(eObj_L4O1_BandageTheWound,true);
						}

						//Objective Add
						if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L4O2_FindWayOutOfForest] )
						{
							ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L4O2_FindWayOutOfForest] = true;
							ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
							= eObj_L4O2_FindWayOutOfForest;
							//Increment Objectives
							ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
							Hud->ShowObjectiveFX(eObj_L4O2_FindWayOutOfForest);

							Control::Audio->PlaySample(aSFXMagic1);
						}



					}
				}
				else
				{
					Hud->CancelDrag();
				}
			}
			else if (itemVal == eInv_L4_S1_Shoe2)
			{
				if (InvDestRectLeftShoe.Intersection(&dropPos) && !ProfileSystem::Instance->ActiveProfile.L4_S1_PutLeftShoe)
				{
					Hud->ShowBannerText("L4oops");
					Hud->CancelDrag();
				}
				else if (PopRectArray[ePop1_RightLeg]->Intersection(&dropPos))
				{
					if (ProfileSystem::Instance->ActiveProfile.L4_S1_WoundTied && !ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack)
					{
						PopupArray[ePop1_RightLeg]->OpenPopup();
						activePopUPId = ePop1_RightLeg;
					}
					else
						Hud->CancelDrag();
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if (itemVal == eInv_L4_S1_LeafSpoon)
			{
				if (waterAreaPoly->Intersection(&dropPos) && !ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeafSpoonWithWaterTaken)
				{
					activePopUPId = ePop2_HealingObjects;
					PopupArray[ePop2_HealingObjects]->OpenPopup();
				}
			}
			else if (itemVal == eInv_L4_S1_LeafSpoonWithWater)
			{
				if (PopRectArray[ePop1_RightLeg]->Intersection(&dropPos))
				{
					if (ProfileSystem::Instance->ActiveProfile.L4_S1_ShoeRemoved && !ProfileSystem::Instance->ActiveProfile.L4_S1_BloodRemoved)
					{
						PopupArray[ePop1_RightLeg]->OpenPopup();
						activePopUPId = ePop1_RightLeg;
					}
					else
						Hud->CancelDrag();
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if (itemVal == eInv_L4_S1_Medicine)
			{
				if (PopRectArray[ePop1_RightLeg]->Intersection(&dropPos))
				{
					if (ProfileSystem::Instance->ActiveProfile.L4_S1_BloodRemoved && !ProfileSystem::Instance->ActiveProfile.L4_S1_MedPasteApplied)
					{
						PopupArray[ePop1_RightLeg]->OpenPopup();
						activePopUPId = ePop1_RightLeg;
					}
					else
						Hud->CancelDrag();
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if (itemVal == eInv_L4_S1_KerchiefPiece1)
			{
				if (PopRectArray[ePop1_RightLeg]->Intersection(&dropPos))
				{
					if (ProfileSystem::Instance->ActiveProfile.L4_S1_MedPasteApplied && !ProfileSystem::Instance->ActiveProfile.L4_S1_WoundTied)
					{
						PopupArray[ePop1_RightLeg]->OpenPopup();
						activePopUPId = ePop1_RightLeg;
					}
					else
						Hud->CancelDrag();
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
	TaskPendingUpdate();
}

float L4_S1_SwampEntrance::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L4_S1_SwampEntrance::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	if (!ProfileSystem::Instance->ActiveProfile.L4_S1_HOShoeCompleted)
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L4_S1_InvBinocularLensTaken && ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeftShoeTaken && ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack)
		isAnyTaskLeft = true;
	else if (ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeftShoeTaken && !ProfileSystem::Instance->ActiveProfile.L4_S1_PutLeftShoe)
		isAnyTaskLeft = true;
	/*else if (ProfileSystem::Instance->ActiveProfile.L4_S1_InvCrutchTaken && !ProfileSystem::Instance->ActiveProfile.L4_S1_CrutchUsed)
	isAnyTaskLeft = true;*/
	else if (ProfileSystem::Instance->ActiveProfile.L4_S2_InvBambooSpearTaken && !ProfileSystem::Instance->ActiveProfile.L4_S1_InvBraceletTaken)
		isAnyTaskLeft = true;
	else if (!ProfileSystem::Instance->ActiveProfile.L4_S1_InvBinocularLensTaken && ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeftShoeTaken && ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack)
		isAnyTaskLeft = true;

	for (int i = 0; i < (int)(PopupArray.size()); i++)
	{
		if(isPopupActive(i)) //if popup is active
		{
			if(PopupArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}


	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S1_SwampEntrance] = isAnyTaskLeft;

	if( ProfileSystem::Instance->ActiveProfile.L4_S1_InvBraceletTaken && !ProfileSystem::Instance->ActiveProfile.L4_S4_SkeletonHandOpened && ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L4_S4_HillTop])
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S4_HillTop] = true;
}

void L4_S1_SwampEntrance::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S1Collected)
		{
			InitHintTrailEmit(1060, 200);
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S1Pumpkin])
		{
			if (activePopUPId != ePopInactive)
				InitHintTrailEmit(450, 490);
			else
				InitHintTrailEmit(1120, 390);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}

	int iNonHOHint = 0; 
	int MaxNonHOHints = 10;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
			break;

		case 0: ++iNonHOHint;
			if( activePopUPId != ePopInactive )
			{
				PopupArray[activePopUPId]->ProcessHint();
				return;
			}
			break;
		case 1: ++iNonHOHint;
			if(!ProfileSystem::Instance->ActiveProfile.L4_S1_HOShoeCompleted )
			{
				InitHintTrailEmit(SceneRectShoeHO.GetCenterX(),SceneRectShoeHO.GetCenterY(),false);
				return;
			}
			break;
		case 2: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeftShoeTaken && !ProfileSystem::Instance->ActiveProfile.L4_S1_PutLeftShoe)
			{
				Hud->ShowHintForInvItem(eInv_L4_S1_Shoe1,&InvDestRectLeftShoe);
				return;
			}
			break;
		case 3: ++iNonHOHint;
			for (int i = 0; i < (int)(PopupArray.size()); i++)
			{
				if (isPopupActive(i))
				{
					if(PopupArray[i]->isTaskLeft())
					{
						CRectangle rect = PopRectArray[i];
						InitHintTrailEmit(rect.GetCenterX(),rect.GetCenterY(),false);
						return;
					}
				}
			}
			break;
		case 4:++iNonHOHint;
			if(!ProfileSystem::Instance->ActiveProfile.L4_S1_InvBinocularLensTaken && ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeftShoeTaken && ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack)
			{
				InitHintTrailEmit(MushroomMiniGameRect.GetCenterX(),MushroomMiniGameRect.GetCenterY(),false);
				return;
			}
			break;
		case 5: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeftShoeTaken && ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack && (ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S2_RiverSide] || !ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L4_S2_RiverSide]))
			{
				InitHintTrailEmit(&SceneRectRiverSide,true,CA_ExitUp);
				return;
			}
			break;
		case 6: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
		case 7: ++iNonHOHint;
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

void L4_S1_SwampEntrance::SparkleUpdate()
{
	if (ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		return;
	if (activePopUPId != ePopInactive)
		return;
	CheckForPendingTask();
	if (!isSparkleUpdatePending)
		return;
	HiddenObjectSystem::SparkleUpdate();
}

void L4_S1_SwampEntrance::CheckForPendingTask()
{
	isSparkleUpdatePending = false;
	for (int i = 0; i < (int)PopupArray.size(); i++)
	{
		Popup * popup = PopupArray[i];
		if (popup)
		{
			if(popup->GetPopupState() == ePopupState_Inactive || popup->GetPopupState() == ePopupState_Closed) 
			{
				if(popup->isTaskLeft())
				{
					sparklePointsVec.push_back(popup->GetSparkleUpdatePoint());
					isSparkleUpdatePending = true;
				}
			}
		}

	}
	if(!ProfileSystem::Instance->ActiveProfile.L4_S1_HOShoeCompleted )
	{
		sparklePointsVec.push_back(CPoint(SceneRectShoeHO.GetCenterX(), SceneRectShoeHO.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}
	if(ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeftShoeTaken && !ProfileSystem::Instance->ActiveProfile.L4_S1_PutLeftShoe)
	{
		sparklePointsVec.push_back(CPoint(InvDestRectLeftShoe.GetCenterX(), InvDestRectLeftShoe.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}
	if(!ProfileSystem::Instance->ActiveProfile.L4_S1_InvBinocularLensTaken && ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeftShoeTaken && ProfileSystem::Instance->ActiveProfile.L4_S1_PutShoeBack)
	{
		sparklePointsVec.push_back(CPoint(MushroomMiniGameRect.GetCenterX(), MushroomMiniGameRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}
}
void L4_S1_SwampEntrance::NavigateToScene()
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


