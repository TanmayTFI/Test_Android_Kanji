//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 02 : Naga Temple
//====================================

#include "L2_S2_NagaTemple.h"
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

const CRectangle  ScnRectPuzzleMango(267.0f,7.0f,203.0f,218.0f);
const CRectangle  ScnRectHONaga(389.0f,181.0f,475.0f,136.0f);


const CRectangle  bellRect(429.0f,28.0f,545.0f,173.0f);
const CRectangle  smallDoorRect(661.0f,353.0f,69.0f,96.0f);
const CRectangle  alterRect(411.0f,211.0f,462.0f,235.0f);


//Inventory Item Rects
//Info Rects


L2_S2_NagaTemple::L2_S2_NagaTemple()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV2\\S2\\S2.SCN");
	musicFadeOutTimer = 0;
	//Scene Animations
	HOScene->GetObjectByID("light")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("light")->Animate(0.01f,1.0f,10.0f);

	HOScene->GetObjectByID("bell1")->SetPivot(-15, -102);
	HOScene->GetObjectByID("bell1")->Animate(0,0,4,0,3.0f);	

	HOScene->GetObjectByID("bell2")->SetPivot(-14, -142);
	HOScene->GetObjectByID("bell2")->Animate(0,0,3,0,4.0f);	

	HOScene->GetObjectByID("bell3")->SetPivot(-14, -142);
	HOScene->GetObjectByID("bell3")->Animate(0,0,2,0,5.0f);	

	HOScene->GetObjectByID("bell4")->SetPivot(-14, -142);
	HOScene->GetObjectByID("bell4")->Animate(0,0,2,0,6.0f);	

	HOScene->GetObjectByID("bell5")->SetPivot(-14, -142);
	HOScene->GetObjectByID("bell5")->Animate(0,0,3,0,7.0f);	

	HOScene->GetObjectByID("bell6")->SetPivot(-14, -142);
	HOScene->GetObjectByID("bell6")->Animate(0,0,4,0,8.0f);	

	HOScene->GetObjectByID("bell7")->SetPivot(-14, -142);
	HOScene->GetObjectByID("bell7")->Animate(0,0,3,0,8.0f);	

	HOScene->GetObjectByID("bell8")->SetPivot(-14, -142);
	HOScene->GetObjectByID("bell8")->Animate(0,0,3,0,7.0f);	

	HOScene->GetObjectByID("bell9")->SetPivot(-14, -142);
	HOScene->GetObjectByID("bell9")->Animate(0,0,4,0,6.0f);	

	HOScene->GetObjectByID("bell10")->SetPivot(-14, -142);
	HOScene->GetObjectByID("bell10")->Animate(0,0,2,0,5.0f);	

	HOScene->GetObjectByID("bell11")->SetPivot(-14, -142);
	HOScene->GetObjectByID("bell11")->Animate(0,0,2,0,4.0f);

	HOScene->GetObjectByID("bell12")->SetPivot(-14, -142);
	HOScene->GetObjectByID("bell12")->Animate(0,0,4,0,4.0f);	

	HOScene->GetObjectByID("bell13")->SetPivot(-14, -142);
	HOScene->GetObjectByID("bell13")->Animate(0,0,4,0,5.0f);	

	HOScene->GetObjectByID("plant1")->SetPivot(0, 165);
	HOScene->GetObjectByID("plant1")->Animate(0,0,4,0,5.0f);	

	HOScene->GetObjectByID("plant2")->SetPivot(145, 132);
	HOScene->GetObjectByID("plant2")->Animate(0,0,3,0,6.0f);	

	HOScene->GetObjectByID("plant3")->SetPivot(0, 91);
	HOScene->GetObjectByID("plant3")->Animate(0,0,2,0,7.0f);	

	HOScene->GetObjectByID("branch1")->SetPivot(-133, -124);
	HOScene->GetObjectByID("branch1")->Animate(0,0,1,0,6.0f);	

	HOScene->GetObjectByID("branch")->SetPivot(110, -136);
	HOScene->GetObjectByID("branch")->Animate(0,0,1,0,7.0f);	

	HOScene->GetObjectByID("leaf")->SetPivot(0, -43);
	HOScene->GetObjectByID("leaf")->Animate(0,0,2,0,7.0f);


	HOScene->GetObjectByID("mangoes")->SetScale(1.2f);
	HOScene->GetObjectByID("mangoes")->SetPivot(0, -169);
	HOScene->GetObjectByID("mangoes")->Animate(0,0,2,0,10.0f);

	HOScene->GetObjectByID("mangoHolder")->SetPivot(0, -169);
	HOScene->GetObjectByID("mangoHolder")->Animate(0,0,2,0,10.0f);

	HOScene->GetObjectByID("squirrelTail")->SetPivot(-25, -37);
	HOScene->GetObjectByID("squirrelTail")->Animate(0,0,1,0,1.0f);

	
	moonprtFX = new SuperFX("PRT\\L2\\S2MoonprtFX.INI",PointSystem::CreateCPoint(989,279,0), 100); 
	moonprtFX->Start();
	moonprtFX->AdvanceByTime(2.0f);

	GFInstance->PostUpdate(1);

	SceneObjectsUpdate();

	InitPopups();

	cv_nagaTemple = NULL;
	time = 0;
	subIndex = 0;
	subMax = 2; vidTimer = 0;


	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("naga1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("naga2"));

	isCutScneVideoPlaying = false;
	lnagaTempleVideoStartTimer =0.0f;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S2_CutSceneVideoPlayed)
	{
		//CRectangle RectVid(171, 96, 1024, 576);
		CRectangle RectVid(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
		cv_nagaTemple = new CVideo("OGV\\L2\\S2NAGACUTSCN.OGV", RectVid, false, eZOrder_CutsceneFull);
		//cv_nagaTemple->SetScale(1.334f);
		cv_nagaTemple->SetScale(GFApp->video_aspectRatio);
		
	}

	if(IsVisitedForFirstTime)
		Hud->ShowBannerText("L2live");
	
	//MUSIC_SFX
	
		Control::Audio->LoadSample(aAmbs2NagaCave,AudioAmb);
		if( !Control::Audio->IsPlaying(aAmbs2NagaCave) )		
		{
			Control::Audio->PlaySample(aAmbs2NagaCave,true);
		}

		Control::Audio->LoadSample(aVOL2MaryRefS2_1,	AudioVO);
	
	//MUSIC_SFX

	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L2_S1_ForestEntrance]  = true;


	holinesFX = new SuperFX("PRT\\L2\\holines.INI", PointSystem::CreateCPoint(615,249,0), 100); 

	hodotsFX = new SuperFX("PRT\\L2\\hodots.INI", PointSystem::CreateCPoint(615,249,0), 100); 

	if( !ProfileSystem::Instance->ActiveProfile.L2S2_HONagaTempleCompleted )
	{
		if( !ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles )
		{
			holinesFX->Start();
			holinesFX->AdvanceByTime(2.0f);

			hodotsFX->Start();
			hodotsFX->AdvanceByTime(2.0f);
		}
	}

	TaskPendingUpdate();
}

void L2_S2_NagaTemple::SparkleUpdate()
{
	if(isSparkleUpdatePending)
	{
		isSparkleUpdatePending = false;
		if(SparkleFX==NULL)
		{
			SparkleFX = new SuperFX("PRT\\glint.INI",PointSystem::CreateCPoint(0,0,0),120);		
		}
		//Game Logic Goes here
	}
}

void L2_S2_NagaTemple::SceneObjectsUpdate()
{
	if(ProfileSystem::Instance->ActiveProfile.L2S2_HONagaTempleCompleted)
	{
		HOScene->GetObjectByID("nagaHead")->SetEnable(true);
	}
	else
	{
		HOScene->GetObjectByID("nagaHead")->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L2_S2_InvMangoesTaken)
	{
		HOScene->GetObjectByID("mangoes")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("mangoes")->SetEnable(true);
	}

	if(ProfileSystem::Instance->ActiveProfile.L2_S2_kolamComplete)
	{
		HOScene->GetObjectByID("kollamColorFilled")->SetEnable(true);
		HOScene->GetObjectByID("kollamOutline")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("kollamColorFilled")->SetEnable(false);
		HOScene->GetObjectByID("kollamOutline")->SetEnable(true);
	}

	/*if(ProfileSystem::Instance->ActiveProfile.L2_S2_NagaLampLite)
	{
		HOScene->GetObjectByID("flame1")->SetEnable(true);
		HOScene->GetObjectByID("flame2")->SetEnable(true);
	}
	else
	{
		HOScene->GetObjectByID("flame1")->SetEnable(false);
		HOScene->GetObjectByID("flame2")->SetEnable(false);
	}*/

	if(ProfileSystem::Instance->ActiveProfile.L2_S2_CutSceneVideoPlayed)
	{
		HOScene->GetObjectByID("nagaDoorClosed")->SetEnable(false);
		HOScene->GetObjectByID("squirrelTail")->SetEnable(false);	
		HOScene->GetObjectByID("squirrelBody")->SetEnable(false);
		if(ProfileSystem::Instance->ActiveProfile.L2_S2_InvNagaPearlsCollected)
		{
			HOScene->GetObjectByID("pearls")->SetEnable(false);
		}
		else
		{
			HOScene->GetObjectByID("pearls")->SetEnable(true);
		}
	}
	else
	{
		HOScene->GetObjectByID("nagaDoorClosed")->SetEnable(true);
	}

	if(ProfileSystem::Instance->ActiveProfile.L2_S2_ColorPlatePlaced)
	{
		HOScene->GetObjectByID("colorPlate")->SetEnable(true);
	}
	else
	{
		HOScene->GetObjectByID("colorPlate")->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L2_S2_InvFrankinscenceTaken)
	{
		HOScene->GetObjectByID("frankensis")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("frankensis")->SetEnable(true);
	}

	#ifdef _CEBUILD
	//Morph Object Add
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S2Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("morphtarget"), HOScene->GetObjectByID("morphbase"));
		CRectangle  MorphRect(275.0f,232.0f,108.0f,84.0f);
		Hud->setMorphObjRect(MorphRect);
	}
	else
	{
		HOScene->GetObjectByID("morphtarget")->SetEnable(false);
	}
#else
	HOScene->GetObjectByID("morphtarget")->SetEnable(false);
#endif // _CEBUIL


}

void L2_S2_NagaTemple::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	for (int i = 0; i < ePopS2_Max; i++)
	{
		if(PopStates[i] != ePopupState_Inactive) //if popup is active
		{
			if(PopupArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}

	
	if( !ProfileSystem::Instance->ActiveProfile.L2S2_HONagaTempleCompleted )
			isAnyTaskLeft = true;		
	if(!ProfileSystem::Instance->ActiveProfile.L2_S2_InvMangoesTaken && ProfileSystem::Instance->ActiveProfile.l2S6_Inv3XStonesTaken)
			isAnyTaskLeft = true;		

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S2_NagaTemple] = isAnyTaskLeft;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S1_HoneyCombSmoked &&	ProfileSystem::Instance->ActiveProfile.L2_S2_InvFrankinscenceTaken)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S1_ForestEntrance] = true;
	if(!ProfileSystem::Instance->ActiveProfile.L2_S5_MangoPlaced &&	ProfileSystem::Instance->ActiveProfile.L2_S2_InvMangoesTaken)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S5_MoopansDen] = true;
	if(!ProfileSystem::Instance->ActiveProfile.L2_S6_NagaPearlFixed &&	ProfileSystem::Instance->ActiveProfile.L2_S2_InvNagaPearlsCollected &&  ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S6_TunnelEnd] = true;

}

void L2_S2_NagaTemple::InitPopups()
{	
	ActivePopupID = ePopInactive;

	for (int i = 0; i < ePopS2_Max; i++)
	{
		PopupArray[i] = NULL;
		PopStates[i] = ePopupState_Inactive;
		PopPolyArray[i] = NULL;
	}
		
	//Popup 1
	if(!ProfileSystem::Instance->ActiveProfile.L2_S2_NagaLampLite && ProfileSystem::Instance->ActiveProfile.L2S2_HONagaTempleCompleted)
	{
		/*int p1rect[] =  {598,326,768,327,812,203,689,132,581,181};
		PopPolyArray[ePop1_NagaHead] = new CPolygon(5);
		PopPolyArray[ePop1_NagaHead]->Init(p1rect);

		PopupArray[ePop1_NagaHead] = new L2S2_Pop1NagaHead(ePop1_NagaHead,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop1_NagaHead]->center, CC_CALLBACK_1(L2_S2_NagaTemple::onPopupClosed, this));
		PopupArray[ePop1_NagaHead]->SetPopupBoundingBox(PopAreaRect);*/

		PopStates[ePop1_NagaHead] = ePopupState_Inactive;
	}

	//Popup 2
	if(!ProfileSystem::Instance->ActiveProfile.L2_S2_InvFrankinscenceTaken )
	{
		int p2rect[] =  {1006,117,1070,106,1107,156,1107,218,1027,232,1005,182,1003,118};
		PopPolyArray[ePop2_Frankinscence] = new CPolygon(7);
		PopPolyArray[ePop2_Frankinscence]->Init(p2rect);

		PopupArray[ePop2_Frankinscence] = new L2S2_Pop2Frankinscence(ePop2_Frankinscence,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop2_Frankinscence]->center, CC_CALLBACK_1(L2_S2_NagaTemple::onPopupClosed, this));
		PopupArray[ePop2_Frankinscence]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePop2_Frankinscence] = ePopupState_Closed;
	}
	
	//Popup 3
	if(!ProfileSystem::Instance->ActiveProfile.L2_S2_kolamComplete )
	{
		int p3rect[] = {667,484,907,467,960,528,946,599,823,622,685,576,634,530};
		PopPolyArray[ePop3_Koolam] = new CPolygon(7);
		PopPolyArray[ePop3_Koolam]->Init(p3rect);

		PopupArray[ePop3_Koolam] = new L2S2_Pop3Koolam(ePop3_Koolam,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop3_Koolam]->center, CC_CALLBACK_1(L2_S2_NagaTemple::onPopupClosed, this));

		CObject *skipBtn = new CObject();
		skipBtn->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\SKIP.SPR", PopupArray[ePop3_Koolam]->GetObjectByID("skipbtn")->Pos.x, PopupArray[ePop3_Koolam]->GetObjectByID("skipbtn")->Pos.y, PopupArray[ePop3_Koolam]->GetObjectByID("skipbtn")->ZOrder + 1);
		skipBtn->SetUID("skipbtntxt");
		skipBtn->SetScale(0.8);
		skipBtn->Show(false);
		skipBtn->SetEnable(false);
		if (ProfileSystem::Instance->ActiveProfile.L2_S2_ColorPlatePlaced && ProfileSystem::Instance->ActiveProfile.L2_S2_PoojaInstrPlaced)
			skipBtn->SetEnable(true);
		PopupArray[ePop3_Koolam]->PushObject(skipBtn);
		
		PopupArray[ePop3_Koolam]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePop3_Koolam] = ePopupState_Closed;
	}
	
	//Popup 1
	if( ProfileSystem::Instance->ActiveProfile.L2_S2_kolamComplete && !ProfileSystem::Instance->ActiveProfile.L2_S2_InvNagaPearlsCollected)
	{
		int p4rect[] = {660,463,751,461,750,342,649,332};
		PopPolyArray[ePop4_Pearls] = new CPolygon(4);
		PopPolyArray[ePop4_Pearls]->Init(p4rect);

		PopupArray[ePop4_Pearls] = new L2S2_Pop4Pearls(ePop4_Pearls,Hud,HOScene, eZOrder_Popup, PopPolyArray[ePop4_Pearls]->center, CC_CALLBACK_1(L2_S2_NagaTemple::onPopupClosed, this));
		PopupArray[ePop4_Pearls]->SetPopupBoundingBox(PopAreaRect);

		PopStates[ePop4_Pearls] = ePopupState_Closed;
	}

	TaskPendingUpdate();
}

void L2_S2_NagaTemple::onPopupClosed(Popup * popup)
{
	ActivePopupID = ePopInactive;
	SceneObjectsUpdate();
	TaskPendingUpdate();
	switch (popup->GetPopupId())
	{
	case 0:
		if(ProfileSystem::Instance->ActiveProfile.L2_S2_NagaLampLite)
		{
			//disable popup
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}
		break;
	case 1:
		if(ProfileSystem::Instance->ActiveProfile.L2_S2_InvFrankinscenceTaken)
		{
			//disable popup
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}
		break;
	case 2:
		if(ProfileSystem::Instance->ActiveProfile.L2_S2_kolamComplete)
		{
			//disable popup
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}
		break;
	case 3:
		if(ProfileSystem::Instance->ActiveProfile.L2_S2_InvNagaPearlsCollected)
		{
			//disable popup
			PopStates[popup->GetPopupId()] = ePopupState_Inactive;
		}
		break;
	default:
		break;
	}
}

void L2_S2_NagaTemple::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if(isCutScneVideoPlaying)
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

void L2_S2_NagaTemple::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	CPoint pos = Control::Input->Mpos();


	if( ActivePopupID == ePopInactive )
	{
		#ifdef _CEBUILD
		if(!Hud->isMouseOverExtraButton)
			Cursor::SetMode(CA_Normal);
	#else
		Cursor::SetMode(CA_Normal);
	#endif 
		if(!ProfileSystem::Instance->ActiveProfile.L2_S2_CutSceneVideoPlayed && ProfileSystem::Instance->ActiveProfile.L2_S2_kolamComplete && ProfileSystem::Instance->ActiveProfile.L2S2_HONagaTempleCompleted)
		{
			if(!cv_nagaTemple->isSkipMouseOver())
				Cursor::SetMode(CA_Normal);
			if(isCutScneVideoPlaying)
			{
				//Subtitles Code
				time = cv_nagaTemple->GetTime();

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
					cv_nagaTemple->isEndReached() ||
#endif
					vidTimer > 28)
				{
					isCutScneVideoPlaying = false;
					subtitleText->SetText("");
					ProfileSystem::Instance->ActiveProfile.L2_S2_CutSceneVideoPlayed = true;
					Hud->ShowBannerText("L2secret");
					Control::Audio->PlaySample(aVOL2MaryRefS2_1);
					cv_nagaTemple->StopAndFadeOutVideo(2.0f);
					Hud->AllowInput = true;
					Hud->ShowHud(false);
					if( !Control::Audio->IsPlaying(aAmbs2NagaCave) )		
						Control::Audio->PlaySample(aAmbs2NagaCave,true);

					//Journal Entry Add
					if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L2P3_SnakeTempleStorm])
					{
						//Flag Set
						ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L2P3_SnakeTempleStorm] = true;

						//Add to Note Array
						ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L2P3_SnakeTempleStorm;

						ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

						//Increment Notes
						ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
						Hud->ShowJrnEntryFX();
					}
			
					InitPopups();
					SceneObjectsUpdate();
					
				}
				return;
			}
			else
			{
				lnagaTempleVideoStartTimer -= Control::LogicRate;
				if(lnagaTempleVideoStartTimer<0)
				{
					cv_nagaTemple->SetSkipActive();
					isCutScneVideoPlaying = true;
					Hud->EnableInput(false);
					Hud->HideHud(false);
					cv_nagaTemple->PlayFadeInVideo(1.5f);
					if( Control::Audio->IsPlaying(aTrackL2theme) )
						 Control::Audio->StopSample(aTrackL2theme);
					if( Control::Audio->IsPlaying(aAmbs2NagaCave) )		
						Control::Audio->StopSample(aAmbs2NagaCave);
				}
			}
			return;
		}

		for (int i = 0; i < ePopS2_Max; i++)
		{
			if(PopStates[i] != ePopupState_Inactive) //if popup is active
			{
				if(PopPolyArray[i]!=NULL && PopPolyArray[i]->Intersection(&pos))
				{
					Cursor::SetMode(CA_Look);
					if( Control::Input->LBclicked() )
					{
						Cursor::SetMode(CA_Normal);
						ActivePopupID = i;
						
						PopupArray[i]->OpenPopup();
					}
					return;
				}
			}
		}

		if( !ProfileSystem::Instance->ActiveProfile.L2_S2_InvMangoesTaken && ScnRectPuzzleMango.Intersection(&pos) )
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
					GFHud->InvBoxCtrl->GoToScene(eArea_L2_S2_PZMango, 0, ScnRectPuzzleMango);
				}
#else
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S2_PZMango;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
		}
		else if( !ProfileSystem::Instance->ActiveProfile.L2S2_HONagaTempleCompleted && ScnRectHONaga.Intersection(&pos) )
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
					GFHud->InvBoxCtrl->GoToScene(eArea_L2_S2_HONagaTemple, 0, ScnRectHONaga);
				}
#else
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S2_HONagaTemple;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
		}
		else if( IsMouseOverGoBackArea(&pos) )
		{

#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
			//	Cursor::SetMode(CA_ExitDown,eArea_L2_S1_ForestEntrance);
#else
			Cursor::SetMode(CA_ExitDown,eArea_L2_S1_ForestEntrance);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L2_S1_ForestEntrance, 2);
				}
#else
				//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S1_ForestEntrance;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}	
		}
		else if(!ProfileSystem::Instance->ActiveProfile.L2_S2_CutSceneVideoPlayed && smallDoorRect.Intersection(&pos) )
		{
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L2small");
			}
		}
		/*else if(ProfileSystem::Instance->ActiveProfile.L2S2_HONagaTempleCompleted && alterRect.Intersection(&pos))
		{
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("This altar was built around the tree a long time ago."); //Sac: where is the external transaltion for this ?
			}
		}*/
		else if(bellRect.Intersection(&pos))
		{
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L2bell");
			}
		}
		else 
		{
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L2tree");
			}
		}

		#ifdef ENABLE_ANDROID_BACK_BUTTON
			if (Control::Input->isKeyBackSensed())
			{
				//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S1_ForestEntrance;
				SendMessage(ControlExit, ID, HOSM_NewArea);				
			}
		#endif
	}

}

L2_S2_NagaTemple::~L2_S2_NagaTemple()
{
	TaskPendingUpdate();	
	SAFE_DELETE(moonprtFX);
	SAFE_DELETE(hodotsFX);
	SAFE_DELETE(holinesFX);
	SAFE_DELETE(cv_nagaTemple);

	for (int i = 0; i < ePopS2_Max; i++)
	{
		SAFE_DELETE(PopupArray[i]);
		SAFE_DELETE(PopPolyArray[i]);
	}


	SAFE_DELETE(subtitleText);
	
	//MUSIC_SFX
	//Quick SFX unloaded in HUD
	Control::Audio->UnloadSample(aVOL2MaryRefS2_1);
	Control::Audio->StopSample(aAmbs2NagaCave);
	Control::Audio->UnloadSample(aAmbs2NagaCave);
	//MUSIC_SFX
}

void L2_S2_NagaTemple::ReceiveMessage(int val)
{
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == UPDATE_SCENE_OBJECTS )
	{
		SceneObjectsUpdate();
		if(ActivePopupID != ePopInactive)
		{
			if(PopupArray[ActivePopupID]!=NULL)
				PopupArray[ActivePopupID]->UpdateBgGraphic();
		}
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
			if(ActivePopupID == ePopInactive)
			{
				//check for popup actions
				//UpdatePopups(dropPos,true);
				if(ActivePopupID == ePopInactive)
				{
					//If No popups responded
					GFInstance->WrongInvDropMsg(Hud);
				}
				return;
			}
			else
			{
				if(PopupArray[ActivePopupID]!=NULL)
					PopupArray[ActivePopupID]->HandleInventoryDrop(dropPos, itemVal);
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
		}
	}
	else if( val == INV_CLOSE )
	{		
		
		
	}	
}

void L2_S2_NagaTemple::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S2Collected)
		{
			InitHintTrailEmit(330, 270);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}

	int iNonHOHint = 0; 
	int MaxNonHOHints = 7;
	do
	{
		switch( iNonHOHint )
		{
			default:			
				//Exit coordinates
				break;
			
			case 0: ++iNonHOHint;
					if( ActivePopupID != ePopInactive )
					{
						PopupArray[ActivePopupID]->ProcessHint();
						return;
					}
					break;
			case 1: ++iNonHOHint;
					for (int i = 0; i < ePopS2_Max; i++)
					{
						if(PopStates[i] != ePopupState_Inactive) //if popup is active
						{
							if(PopupArray[i]->isTaskLeft())
							{
								InitHintTrailEmit(&PopPolyArray[i]->center,false);
								return;
							}
						}
					}
					break;
			case 2: ++iNonHOHint;
					if( !ProfileSystem::Instance->ActiveProfile.L2S2_HONagaTempleCompleted )
					{
						InitHintTrailEmit(&ScnRectHONaga,false);
						return;
					}
					break;
			case 3: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L2_S2_InvMangoesTaken && ProfileSystem::Instance->ActiveProfile.l2S6_Inv3XStonesTaken)
					{
						InitHintTrailEmit(&ScnRectPuzzleMango,false);
						return;
					}
					break;
			case 4: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S1_ForestEntrance])
					{
						InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
						return;
					}
					break;
			case 5: ++iNonHOHint;
					{
						if(Hud->ShowHintForComboInvItems())
							return;
					}
			case 6: ++iNonHOHint;
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


void L2_S2_NagaTemple::NavigateToScene()
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