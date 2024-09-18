//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL6 - SCENE5 : MARYS DREAM
//====================================

#include "L6_S5_MarysDream.h"
#include "Util.h"

const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  PopCircleRect(484.0f,154.0f,376.0f,368.0f);

const CRectangle  SophiaRect(224.0f,334.0f,111.0f,163.0f);


const CRectangle  SceneRectDrawer1(3.0f,27.0f,275.0f,176.0f);
const CRectangle  SceneRectDrawer2(6.0f,210.0f,157.0f,234.0f);
const CRectangle  SceneRectDrawer3(11.0f,517.0f,303.0f,245.0f);
const CRectangle  SceneRectDrawer4(1206.0f,264.0f,115.0f,267.0f);
const CRectangle  SceneRectDrawer5(1034.0f,436.0f,56.0f,180.0f);

const CRectangle  PictrureRect1(52.0f,223.0f,99.0f,59.0f);
const CRectangle  PictrureRect2(77.0f,376.0f,142.0f,93.0f);
const CRectangle  PictrureRect3(785.0f,241.0f,67.0f,52.0f);

enum eL6S1HOPopEnums
{
	eL6S5Pop_Inactive = -1,
	eL6S5Pop1_Sophia
};


float	FinalPosX[] = {932,1126,630,675,1049,593,184,907,1006,163};
float	FinalPosY[] = {252,500,155,214,241,314,443,393,605,316};

const char* sophiaHOItemNames[] = {"Book_1",
	"Pencil",
	"Arm", 
	"Doll_Head", 
	"Book_2", 
	"Teddy_Lion", 
	"Picture_Frame", 
	"Pillow", 
	"Shoe", 
	"Dress",
	"Sophia"};

L6_S5_MarysDream::L6_S5_MarysDream()
{
	_cVideoCutScene = NULL;
	isCutscnPlaying = false;
	LoadHud(INVENTORY_V1);
	LoadScene("LV6\\S5\\S5.SCN");

	CObject* temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\EXTRAS\\KIDROOM\\BG_TEXT.SPR", 870, 180, 30);
	temp->SetUID("BGtext");
	HOScene->PushObject(temp);

	blurBg = HOScene->GetObjectByID("BlurBg");
	blurBg->SetAlpha(0);
	vidTimer = 0;

		
	if( !GFInstance->IsReplayingGame )
	{
		blurBg->SetScale(2);
		blurBg->FadeInOut(2);
		isNeedToBlur = true;
		Hud->ShowBannerText("L6head");
	}
	else
	{
		blurBg->Show(false);
		isNeedToBlur = false;
	}

	HOScene->GetObjectByID("sofiaHair")->SetEnable(false);
	HOScene->GetObjectByID("sofiaTabGlow")->SetEnable(false);
	HOScene->GetObjectByID("sofiaHead")->SetEnable(false);
	HOScene->GetObjectByID("sofiaRightHand")->SetEnable(false);
	HOScene->GetObjectByID("sofiaTab")->SetEnable(false);
	HOScene->GetObjectByID("sofiaBody")->SetEnable(false);
	HOScene->GetObjectByID("sofiaLegLeft")->SetEnable(false);

	_cloth = HOScene->GetObjectByID("Cloth");
	_sophiaUnderBed = HOScene->GetObjectByID("SophiaUnderBed");
	tablightCounter = 10;
	_tabletLight = HOScene->GetObjectByID("TabLight");
	if (ProfileSystem::Instance->ActiveProfile.L6_S5_HO_Complete)
	{
		_cloth->SetEnable(false);
		if( GFInstance->IsReplayingGame )
		{
			_sophiaUnderBed->SetEnable(false);
			_tabletLight->SetEnable(false);
		}
	}
	else
	{
		_sophiaUnderBed->Show(false);
		_tabletLight->Show(false);
	}

	DreamparticlesFX = new SuperFX("PRT\\L6\\S5_Dreamparticles.INI", PointSystem::CreateCPoint(683,384,0), 100); 
	DreamparticlesFX->Start();
	DreamparticlesFX->AdvanceByTime(2.0f);

	S5_WindowParticlesFX = new SuperFX("PRT\\L6\\S5_WindowParticles.INI", PointSystem::CreateCPoint(445,336,0), 100); 
	S5_WindowParticlesFX->Start();
	S5_WindowParticlesFX->AdvanceByTime(2.0f);

	S5_LampParticlesFX = new SuperFX("PRT\\L6\\S5_LampParticles.INI", PointSystem::CreateCPoint(1129,325,0), 100); 
	S5_LampParticlesFX->Start();
	S5_LampParticlesFX->AdvanceByTime(2.0f);

	int Pop1ArrayPts[] = {833,559,970,508,1058,631,893,707,829,558};
	ClickablePolygon *popClickPoly = new ClickablePolygon(eL6S5Pop1_Sophia, 5, Pop1ArrayPts);
	PopRectArray.push_back(popClickPoly);

	//popClickPoly = NULL;

	dialogSystem = NULL;
	//if condition to add
	dialogSystem = new DialogSystem();
	AddControl(dialogSystem);	
	isDialogActive = false;
	isSophiaPositionSet = false;

	L6S5_Pop1Sophia *pop1 = new  L6S5_Pop1Sophia(eL6S5Pop1_Sophia, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L6_S5_MarysDream::OnPopupClosed, this));
	pop1->dialogSystem = dialogSystem;
	pop1->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop1);

	ActivePopUpId = eL6S5Pop_Inactive;
	InitInvNames();

	if (!ProfileSystem::Instance->ActiveProfile.L6_S5_HO_Complete && ProfileSystem::Instance->ActiveProfile.L6_S5_DialogsOver)
	{
		_hoModeActive = true;

		Hud->InvBoxCtrl->SetLockBase(HO_PLACEOBJECTS_FRAME);//lock in HO anim
		Hud->IsHOScene = true;
		Hud->HOState = HOActive;
	}
	else
	{
		_hoModeActive = false;

		for (int i = 0; i < NUMBER_OF_HOs; i++)
		{
			_nameText[i]->SetAlpha(0);
		}
	}

	for (int i = 0; i < NUMBER_OF_HOs; i++)
	{
		string _id = "hid_obj_" + MKSTR(i) + "_01";
		MisplacedObjectsArray.push_back(HOScene->GetObjectByID(_id));
		InitialPosX[i] = HOScene->GetObjectByID(_id)->GetPosPointer()->x;
		InitialPosY[i] = HOScene->GetObjectByID(_id)->GetPosPointer()->y;
		InitialZorder[i] = HOScene->GetObjectByID(_id)->ZOrder;
		if (ProfileSystem::Instance->ActiveProfile.L6_S5_HOObjFound[i])
		{
			HOScene->GetObjectByID(_id)->PlayAnimation(2);
			HOScene->GetObjectByID(_id)->SetPos(PointSystem::CreateCPoint(FinalPosX[i],FinalPosY[i],0));
			if (_hoModeActive)
				_nameText[i]->SetAlpha(0.5f);
		}
	}

	_objectInHand = -1;

	InitFinalPolygon();
	InitRectPolygon();
	InitParticles();

	SceneObjectsUpdate();

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL6s5MarysDreamAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL6s5MarysDreamAmb) )		
	{
		Control::Audio->PlaySample(aAmbL6s5MarysDreamAmb,true);
	}

	//MUSIC_SFX

	//Journal Entry Add
	if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P11_EnterSophiaBedroom])
	{
		//Flag Set
		ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L6P11_EnterSophiaBedroom] = true;

		//Add to Note Array
		ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L6P11_EnterSophiaBedroom;

		ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

		//Increment Notes
		ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
		Hud->ShowJrnEntryFX();
	}

}

bool L6_S5_MarysDream::isPopupActive(int popId)
{
	switch (popId)
	{
	case eL6S5Pop1_Sophia:
		{
			if (ProfileSystem::Instance->ActiveProfile.L6_S5_HO_Complete)
			{
#ifdef _CEBUILD
				if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S5KidSandal] ||  !ProfileSystem::Instance->ActiveProfile.L6_S5_TabMinigameSolved)
				{
					return true;
				}
#else
				if (!ProfileSystem::Instance->ActiveProfile.L6_S5_TabMinigameSolved)
				{
					return true;
				}
#endif 


			}
		}
		
		break;
	default:
		break;
	}

	return false;
}


void L6_S5_MarysDream::InitInvNames()
{
	CRectangle InvAreaRect(368.0f,664.0f,624.0f,96.0f);
	float vertGap = 26, horizontalGap = InvAreaRect.w/4.0f;
	CPoint pos(0,0,0);

	for (int i = 0; i < NUMBER_OF_HOs; i++)
	{
		pos.x = InvAreaRect.Left() + horizontalGap*0.5f + horizontalGap*(i%4);
		pos.y = InvAreaRect.Top() + vertGap + vertGap*(i/4);

		ColorRGB gold = {(233/255.0f),(204/255.0f),(131/255.0f)};
		_nameText[i] = new CBitmapText();
		_nameText[i]->SetTextProps(StringTable::getStr(sophiaHOItemNames[i]), pos.x, pos.y, eZOrder_INVBOXITEMS, gold, eFont_Small, Align_Center);
		_nameText[i]->SetZ(eZOrder_INVBOXITEMS);
	}
}

void L6_S5_MarysDream::OnPopupClosed(Popup * popup)
{
	SceneObjectsUpdate();

	ActivePopUpId = eL6S5Pop_Inactive;

	/*if (HOScene->GetObjectByID("sofiaHair")->GetAlpha() == 0)
	{
	HOScene->GetObjectByID("sofiaHair")->FadeIn(0.5);
	HOScene->GetObjectByID("sofiaTabGlow")->FadeIn(0.5);
	HOScene->GetObjectByID("sofiaHead")->FadeIn(0.5);
	HOScene->GetObjectByID("sofiaRightHand")->FadeIn(0.5);
	HOScene->GetObjectByID("sofiaTab")->FadeIn(0.5);
	HOScene->GetObjectByID("sofiaBody")->FadeIn(0.5);
	HOScene->GetObjectByID("sofiaLegLeft")->FadeIn(0.5);
	}*/
}

void L6_S5_MarysDream::SceneObjectsUpdate()
{
	TaskPendingUpdate();

	for (int i = 1; i < 5; i++)
	{
		string _id = "sofia0" + MKSTR(i);
		HOScene->GetObjectByID(_id)->SetAlpha(0);
	}

	if (!ProfileSystem::Instance->ActiveProfile.L6_S5_HO_Complete)
	{
		if (!ProfileSystem::Instance->ActiveProfile.L6_S5_DialogsOver)
		{
			HOScene->GetObjectByID("sofia04")->SetAlpha(1);
		}
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S5_HO_Complete)
	{
		_cloth->SetEnable(false);
	}
	
	/*	if (!ProfileSystem::Instance->ActiveProfile.L6_S5_FoundSophia)
	{
	HOScene->GetObjectByID("sofiaHair")->SetAlpha(0);
	HOScene->GetObjectByID("sofiaTabGlow")->SetAlpha(0);
	HOScene->GetObjectByID("sofiaHead")->SetAlpha(0);
	HOScene->GetObjectByID("sofiaRightHand")->SetAlpha(0);
	HOScene->GetObjectByID("sofiaTab")->SetAlpha(0);
	HOScene->GetObjectByID("sofiaBody")->SetAlpha(0);
	HOScene->GetObjectByID("sofiaLegLeft")->SetAlpha(0);
	}
	*/

}

void L6_S5_MarysDream::OnFirstTweenComplete(CObject *obj)
{
	if (!ProfileSystem::Instance->ActiveProfile.L6_S5_FoundSophia)
	{
		float valArray1[5] = {0,obj->GetPosPointer()->x,obj->GetPosPointer()->y,1,0};
		obj->TweenTo(valArray1,2,0,true,CC_CALLBACK_1(L6_S5_MarysDream::OnSecondTweenComplete,this));
	}
}

void L6_S5_MarysDream::OnSecondTweenComplete(CObject *obj)
{
	if (!ProfileSystem::Instance->ActiveProfile.L6_S5_FoundSophia)
	{
		string _id = "sofia0" + MKSTR(rand() % 4 + 1);

		float valArray2[5] = {1,HOScene->GetObjectByID(_id)->GetPosPointer()->x,HOScene->GetObjectByID(_id)->GetPosPointer()->y,1,0};
		HOScene->GetObjectByID(_id)->TweenTo(valArray2,2,0,true,CC_CALLBACK_1(L6_S5_MarysDream::OnFirstTweenComplete,this));
	}
}


void L6_S5_MarysDream::InitParticles()
{
	HOScene->GetObjectByID("WindBell")->SetPivot(0,-127);
	HOScene->GetObjectByID("WindBell")->Animate(0,0,5,0,5);
	HOScene->GetObjectByID("LightRaysFromWindows")->Animate(0.6f,1,7);
	/*HOScene->GetObjectByID("sofiaLegLeft")->SetPivot(0,25);
	HOScene->GetObjectByID("sofiaLegLeft")->Animate(0,0,2,0,2);
	HOScene->GetObjectByID("sofiaHead")->Animate(0,0.5,0,0,2);
	HOScene->GetObjectByID("sofiaHair")->Animate(0,0.5,0,0,2);
	HOScene->GetObjectByID("sofiaRightHand")->Animate(0,0.5f,0,0,3);
	HOScene->GetObjectByID("sofiaTabGlow")->Animate(0.6f,1,7);*/
	HOScene->GetObjectByID("Bedlamp_On")->Animate(0.8f,1,2);



	S5_HOCompleteFxFX = new SuperFX("PRT\\L6\\S5_HOCompleteFx.INI", PointSystem::CreateCPoint(683,384,0), 1100); 
	S5_HOCompleteFxFX->AdvanceByTime(2.0f);




}

L6_S5_MarysDream::~L6_S5_MarysDream()
{
	Hud->InvBoxCtrl->ResetToInvBox();
	SAFE_DELETE(_cVideoCutScene);
	MisplacedObjectsArray.clear();
	for (int i = 0; i < (int)PopupArray.size(); i++)
	{
		SAFE_DELETE(PopupArray.at(i));
	}
	PopupArray.clear();
	for (int i = 0; i < (int)PopRectArray.size(); i++)
	{
		SAFE_DELETE(PopRectArray.at(i));
	}
	PopRectArray.clear();
	//OrderObjectsArray.clear();
	//SAFE_DELETE(dialogSystem);

	for (int i = 0; i < NUMBER_OF_HOs; i++)
	{
		SAFE_DELETE(OrderObjectPos[i]);
		SAFE_DELETE(_nameText[i]);
		SAFE_DELETE(ObjectRect[i]);
	}

	SAFE_DELETE(DreamparticlesFX);
	SAFE_DELETE(S5_WindowParticlesFX);
	SAFE_DELETE(S5_LampParticlesFX);
	SAFE_DELETE(S5_HOCompleteFxFX);
	//MUSIC_SFX

	Control::Audio->StopSample(aAmbL6s5MarysDreamAmb);
	Control::Audio->UnloadSample(aAmbL6s5MarysDreamAmb);
	//MUSIC_SFX
}

void L6_S5_MarysDream::Required()
{
	HiddenObjectSystem::Required();	//forced update
	if ( isCutscnPlaying  )
		return;

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Control::Audio->IsPlaying(aTrackL6ho) )
	{
		Control::Audio->Samples[aTrackL6ho]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL6ho]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL6ho);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL6s3Puzzle) )
				Control::Audio->PlaySample(aTrackL6s3Puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else if( Control::Audio->IsPlaying(aTrackL6puzzle) )
	{
		Control::Audio->Samples[aTrackL6puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL6puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL6puzzle);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL6s3Puzzle) )
				Control::Audio->PlaySample(aTrackL6s3Puzzle, true);

		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL6s3Puzzle))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL6s3Puzzle, true);
		}
	}
}

void L6_S5_MarysDream::Update()
{

	
	HiddenObjectSystem::HOUpdate();
	
	if (isCutscnPlaying )
	{
#ifdef FORCE_CLOSE_VIDEO
		vidTimer += Control::LogicRate;
#endif
		if( _cVideoCutScene && (
#ifndef FORCE_CLOSE_VID_END
			_cVideoCutScene->isEndReached() ||
#endif
			vidTimer > 13))
		{
			isCutscnPlaying = false;
			AwardManager::GetInstance()->CompleteHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			ProfileSystem::Instance->ClearAllCurrentObjectives();ProfileSystem::Instance->ClearInventoryBox();
			Hud->AllowInput = true;
			Hud->ShowHud(false);
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L7_S1_Volcano;
			ProfileSystem::Instance->ActiveProfile.CurrentLevel = Level_7;
			ProfileSystem::Instance->SaveProfileBackup(ProfileSystem::Instance->ProfileSlot);
			SendMessage(ControlExit, ID, HOSM_NewArea);
			return;
		}
		return;//No other processing during cutscene!!
	}

	if (isNeedToBlur)
	{
		if(blurBg->Status != SPR_FADEINOUT)
		{
			blurBg->FadeInOut();
			isNeedToBlur = false;
		}
		return;
	}
	if(isDialogActive)
	{
		if( dialogSystem->IsSophiaTalking )
		{
			if( !isSophiaPositionSet )
			{
				switch (dialogSystem->SophiaPosition )
				{
				case 0:	HOScene->GetObjectByID("sofia04")->SetEnable(true);
					break;
				case 1: ShowSophia(3);
					HOScene->GetObjectByID("sofia04")->FadeOut(2.0f);
					break;
				case 2: ShowSophia(2);
					HOScene->GetObjectByID("sofia03")->FadeOut(2.0f);
					break;
				case 3: ShowSophia(5);
					HOScene->GetObjectByID("sofia02")->FadeOut(2.0f);
					break;
				default:
					break;
				}
				isSophiaPositionSet = true;
			}
		}
		else
			isSophiaPositionSet = false;
		dialogSystem->Update();
		return;
	}

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


	if (ProfileSystem::Instance->ActiveProfile.L6_S5_HO_Complete)
	{
		tablightCounter --;

		if (tablightCounter < 0)
		{

			tablightCounter = rand() % 30;
			float alpha = (30 + (rand() % 100))  / 100.0f;
			_tabletLight->SetAlpha(alpha);
		}
	}

	if( Hud->PopupState != Popup_Inactive )
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;
	if (ActivePopUpId != eL6S5Pop_Inactive)
		return;

		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed()) {
				if(Hud!=NULL && GFInstance->IsInGameMenuPopup != true)Hud->CallPauseMenu();
			}
		#endif
	if( GFInstance->IsReplayingGame )
	{
		if(!ProfileSystem::Instance->ActiveProfile.IsMainChapterCompleted) 
		{
			GFInstance->IsReplayingGame = false;
		}
		bool isPendingCollectableInLevel = false;
		if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S1Collected ||
			!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S2Collected ||
			!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S3Collected ||
			!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S4Collected )
		{
			isPendingCollectableInLevel = true;					
			if( !Hud->MessageText->Visible && Hud->BannerAlpha == 0.0f && Hud->BannerState == eHMS_Idle  )//avoid looped banner calls!!
				Hud->ShowBannerText("L1mismo");
		}
		else 
		{
			for( int i = 0; i < 5; i++ )
				if( !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[(ProfileSystem::Instance->ActiveProfile.CurrentLevel*5)+i] )
				{
					isPendingCollectableInLevel = true;
					if( !Hud->MessageText->Visible && Hud->BannerAlpha == 0.0f && Hud->BannerState == eHMS_Idle )//avoid looped banner calls!!
						 Hud->ShowBannerText("L1mismc");
					break;
				}
		}

		//No pending collectables during replaying!! force popup & exit!!
		if( !isPendingCollectableInLevel  && ActivePopUpId == eL6S5Pop_Inactive && !isCutscnPlaying )
		{
			//CRectangle RectVid(171, 96, 1024, 576);
			CRectangle RectVid(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
			_cVideoCutScene = new CVideo("OGV\\L6\\S5ENDCUTSCENE.OGV", RectVid, false, eZOrder_CutsceneFull);
			//_cVideoCutScene->SetScale(1.334f);
			_cVideoCutScene->SetScale(GFApp->video_aspectRatio);
			_cVideoCutScene->PlayVideo();
			_cVideoCutScene->SetSkipActive();
			isCutscnPlaying = true;
			
			Hud->EnableInput(false);
			Hud->HideHud(false);
			return;
		}
	}

#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
#else
	Cursor::SetMode(CA_Normal);
#endif 

	if (!ProfileSystem::Instance->ActiveProfile.L6_S5_Visited)
	{
		//Hud->ShowBannerText("L6shoes");
		ProfileSystem::Instance->ActiveProfile.L6_S5_Visited = true;
		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L6_S1_FloatingIsland1] = true;
		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L6_S2_FloatingIsland2] = true;
		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L6_S3_FloatingIsland3] = true;
		ProfileSystem::Instance->ActiveProfile.IsAreaLocked[eArea_L6_S4_FloatingIsland4] = true;
	}

	CPoint pos = Control::Input->Mpos();

	for (int i = 0; i < (int)(PopupArray.size()); i++)
	{
		if( PopRectArray[i]->Intersection(&pos) && isPopupActive(i))
		{
			Cursor::SetMode(CA_Look);
			if (Control::Input->LBclicked())
			{
				ActivePopUpId = i;
				PopupArray[i]->OpenPopup();
			}
			return;

		} 
	}

	if( SophiaRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S5_DialogsOver)
	{
		Cursor::SetMode(CA_Speech);
		if( Control::Input->LBclicked() )
		{
			dialogSystem->ShowDialog(eDialog_L6S5Sophia);
			isDialogActive = true;
			Hud->AllowInput = false;
			Hud->HideHud(true);				
		}	
	}



	if (_hoModeActive)
	{
		if (_objectInHand == -1)//No object in hand
		{
			for (int i = 0; i < (int)MisplacedObjectsArray.size(); i++)
			{
				if (ObjectRect[i]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S5_HOObjFound[i])
				{
					Cursor::SetMode(CA_HandTake);
					if (Control::Input->LBclicked())
					{
						Control::Audio->QuickLoadAndPlaySFX("s5ho_itemClick");
						MisplacedObjectsArray[i]->PlayAnimation(1);
						_objectInHand = i;
					}
				}
			}
		}
		else//object in hand
		{

			if (Control::Input->LBclicked())//hand released
			{
				//if (OrderObjectsArray[_objectInHand]->GetWorldRect().Intersection(&pos))//correct slot
				if (OrderObjectPos[_objectInHand]->Intersection(&pos))
				{
					Control::Audio->QuickLoadAndPlaySFX("s5ho_itemPlaceCorrect");
					//MisplacedObjectsArray[_objectInHand]->SetEnable(false);
					//OrderObjectsArray[_objectInHand]->SetEnable(true);
					MisplacedObjectsArray[_objectInHand]->SetPos(PointSystem::CreateCPoint(FinalPosX[_objectInHand],FinalPosY[_objectInHand],0));
					MisplacedObjectsArray[_objectInHand]->PlayAnimation(2);
					ProfileSystem::Instance->ActiveProfile.L6_S5_HOObjFound[_objectInHand] = true;
					_nameText[_objectInHand]->SetStrikethrough(true);

					bool _hoComplete = true;

					for (int i = 0; i < (int)MisplacedObjectsArray.size(); i++)
					{
						if (!ProfileSystem::Instance->ActiveProfile.L6_S5_HOObjFound[i])
						{
							_hoComplete = false;
							break;
						}
					}

					if (_hoComplete)
					{
						_hoModeActive = false;
						ProfileSystem::Instance->ActiveProfile.L6_S5_HO_Complete = true;
						_cloth->FadeOut();
						_tabletLight->Show(true);
						_sophiaUnderBed->Show(true);
						_sophiaUnderBed->FadeIn();
						S5_HOCompleteFxFX->Start();
						_tabletLight->SetEnable(true);
						for (int i = 0; i < NUMBER_OF_HOs; i++)
						{
							_nameText[i]->SetAlpha(0);
						}
						Hud->InvBoxCtrl->ResetToInvBox();
						Hud->IsHOScene = false;
						Control::Audio->QuickLoadAndPlaySFX(aSFXKidGiggle);
					}
				}
				else//otherwise
				{
					Control::Audio->QuickLoadAndPlaySFX("s5ho_itemPlaceincorrect");
					MisplacedObjectsArray[_objectInHand]->SetPos(PointSystem::CreateCPoint(InitialPosX[_objectInHand],InitialPosY[_objectInHand],0));
					MisplacedObjectsArray[_objectInHand]->PlayAnimation(0);
				}
				MisplacedObjectsArray[_objectInHand]->SetZ(InitialZorder[_objectInHand]);
				_objectInHand = -1;
			}
			else
			{
				MisplacedObjectsArray[_objectInHand]->SetZ(MisplacedObjectsArray[_objectInHand]->ZOrder + 200);
				MisplacedObjectsArray[_objectInHand]->SetPos(&pos);
			}
		}
		if ((PictrureRect1.Intersection(&pos) || PictrureRect2.Intersection(&pos) || PictrureRect3.Intersection(&pos)) && _objectInHand == -1)
		{
			Cursor::SetMode(CA_Help);
			if (Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L6busy");
			}
		}
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S5_HO_Complete && !ProfileSystem::Instance->ActiveProfile.L6_S5_FoundSophia)
		{
			Cursor::SetMode(CA_Help);
			if (Control::Input->LBclicked())
			{
				int id = rand()%4;
				switch (id)
				{
				case 0:
					Hud->ShowBannerText("L6hide");
					break;
				case 1:
					Hud->ShowBannerText("L6where");
					break;
				case 2:
					Hud->ShowBannerText("L6cant");
					break;
				case 3:
					Hud->ShowBannerText("L6xray");
					break;
				default:
					break;
				}

			}
		}

	if (GFInstance->IsReplayingGame)
	{
		
#ifdef ENABLE_ANDROID_BACK_BUTTON
		NavigateToScene();
		if (Control::Input->isKeyBackSensed())
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S2_FloatingIsland2;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
#endif	
		CPoint pos = Control::Input->Mpos();
		if (IsMouseOverGoBackArea(&pos))
		{

#ifdef TOUCH_UI
	//		if (!GFHud->InvBoxCtrl->navTapped)
	//			Cursor::SetMode(CA_ExitDown, eArea_L6_S4_FloatingIsland4);
#else
			Cursor::SetMode(CA_ExitDown, eArea_L6_S4_FloatingIsland4);
#endif

			if (Control::Input->LBclicked())
			{
#ifdef TOUCH_UI
				if (!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L6_S4_FloatingIsland4, 2);
				}
#else			
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S4_FloatingIsland4;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
		}
	}
}

void L6_S5_MarysDream ::SparkleUpdate()
{
	if (ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		return;

	if (ActivePopUpId != ePopInactive)
		return;

	CheckForPendingTask();

	if (!isSparkleUpdatePending)
		return;

	HiddenObjectSystem::SparkleUpdate();
}

void L6_S5_MarysDream::CheckForPendingTask()
{
	isSparkleUpdatePending = false;
	for (int i = 0; i < 1; i++)
	{
		Popup * popup = PopupArray[i];
		if (popup &&  isPopupActive(i) )
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

	if (!ProfileSystem::Instance->ActiveProfile.L6_S5_DialogsOver)
	{
		sparklePointsVec.push_back(CPoint(SophiaRect.GetCenterX(), SophiaRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}

	if (!ProfileSystem::Instance->ActiveProfile.L6_S5_TabMinigameSolved && ProfileSystem::Instance->ActiveProfile.L6_S5_HO_Complete)
	{
		sparklePointsVec.push_back(PopRectArray[eL6S5Pop1_Sophia]->center);
		isSparkleUpdatePending = true;
	}
}

void L6_S5_MarysDream::InitFinalPolygon()
{
	int ArrayPts[] = {842,218,836,296,1037,293,1062,186,843,215};
	OrderObjectPos[0] =  new CPolygon(5);
	OrderObjectPos[0]->Init(ArrayPts);

	int ArrayPts5[] = {1085,468,1090,572,1184,596,1175,467,1084,464};
	OrderObjectPos[1]=  new CPolygon(5);
	OrderObjectPos[1]->Init(ArrayPts5);

	int ArrayPts1[] = {560,98,551,188,665,191,682,120,629,71,560,94};
	OrderObjectPos[2] =  new CPolygon(6);
	OrderObjectPos[2]->Init(ArrayPts1);

	int ArrayPts2[] = {652,184,654,262,726,264,714,186,650,181};
	OrderObjectPos[3] =  new CPolygon(5);
	OrderObjectPos[3]->Init(ArrayPts2);

	int ArrayPts3[] = {848,218,849,288,1040,283,1047,194,845,216};
	OrderObjectPos[4]=  new CPolygon(5);
	OrderObjectPos[4]->Init(ArrayPts3);

	int ArrayPts4[] = {512,187,510,347,733,344,732,178,510,185};
	OrderObjectPos[5]=  new CPolygon(5);
	OrderObjectPos[5]->Init(ArrayPts4);

	int ArrayPts6[] = {5,380,188,368,356,476,5,523,3,384};
	OrderObjectPos[6]=  new CPolygon(5);
	OrderObjectPos[6]->Init(ArrayPts6);

	int ArrayPts7[] = {779,315,773,411,1013,465,1060,317,781,313};//Pillow TODO: Increase area
	OrderObjectPos[7]=  new CPolygon(5);
	OrderObjectPos[7]->Init(ArrayPts7);

	int ArrayPts8[] = {1022,540,803,656,885,728,1093,713,1089,594,1023,542};
	OrderObjectPos[8]=  new CPolygon(6);
	OrderObjectPos[8]->Init(ArrayPts8);

	int ArrayPts9[] = {163,199,156,370,264,333,265,208,167,207};
	OrderObjectPos[9]=  new CPolygon(5);
	OrderObjectPos[9]->Init(ArrayPts9);

}

void L6_S5_MarysDream::InitRectPolygon()
{
	int ArrayPts[] = {820,400,802,435,806,441,878,465,879,454,884,418,816,401};//book 1
	ObjectRect[0] =  new CPolygon(7);
	ObjectRect[0]->Init(ArrayPts);

	int ArrayPts5[] = {246,465,253,462,329,471,329,476,329,479,246,470,243,465};
	ObjectRect[1]=  new CPolygon(7);
	ObjectRect[1]->Init(ArrayPts5);

	int ArrayPts1[] = {558,432,500,444,501,458,508,462,568,450,560,433};
	ObjectRect[2] =  new CPolygon(6);
	ObjectRect[2]->Init(ArrayPts1);

	int ArrayPts2[] = {552,529,537,541,531,553,542,555,551,565,571,566,581,544,569,527,553,531};//head
	ObjectRect[3] =  new CPolygon(9);
	ObjectRect[3]->Init(ArrayPts2);

	int ArrayPts3[] = {464,561,531,545,527,532,514,501,456,515,455,553,460,559};//book 2
	ObjectRect[4]=  new CPolygon(7);
	ObjectRect[4]->Init(ArrayPts3);

	int ArrayPts4[] = {730,335,703,337,695,356,687,383,681,400,697,399,745,402,749,377,728,333};//teddy
	ObjectRect[5]=  new CPolygon(9);
	ObjectRect[5]->Init(ArrayPts4);

	int ArrayPts6[] = {161,455,139,490,223,495,218,457,159,453};//photoframe
	ObjectRect[6]=  new CPolygon(5);
	ObjectRect[6]->Init(ArrayPts6);

	int ArrayPts7[] = {590,567,603,627,697,621,697,555,608,572,588,565};//pillow
	ObjectRect[7]=  new CPolygon(6);
	ObjectRect[7]->Init(ArrayPts7);

	int ArrayPts8[] =  {669,442,672,461,683,464,693,474,711,474,726,461,702,443,667,440};//shoe
	ObjectRect[8]=  new CPolygon(8);
	ObjectRect[8]->Init(ArrayPts8);

	int ArrayPts9[] = {720,447,761,489,761,613,775,621,840,586,845,593,881,567,850,496,812,466,777,427,759,428,756,443,718,445};//frok
	ObjectRect[9]=  new CPolygon(13);
	ObjectRect[9]->Init(ArrayPts9);
}


void L6_S5_MarysDream::Render(const CPoint* pCam)
{

}


void L6_S5_MarysDream::TakeInventoryItem(EHOInventory ItemID)
{

}


void L6_S5_MarysDream::ReceiveMessage(int val)
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
	if( val == HOSM_DialogClosed ) 
	{
		if( !ProfileSystem::Instance->ActiveProfile.L6_S5_DialogsOver )
		{
			ProfileSystem::Instance->ActiveProfile.L6_S5_DialogsOver = true;


			//Objective Add
			if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L6O4_FindSophia] )
			{
				ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L6O4_FindSophia] = true;
				ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
				= eObj_L6O4_FindSophia;
				//Increment Objectives
				ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
				Hud->ShowObjectiveFX(eObj_L6O4_FindSophia);

				Control::Audio->PlaySample(aSFXMagic1);
			}



			_hoModeActive = true;
			InitInvNames();
			Hud->InvBoxCtrl->SetLockBase(HO_PLACEOBJECTS_FRAME);//lock in HO anim
			Hud->IsHOScene = true;
			Hud->HOState = HOActive;
			ShowSophia(5);
			//HOScene->GetObjectByID("sofia04")->SetAlpha(0);
			//string _id = "sofia0" + MKSTR(rand() % 4 + 1);
			///*HOScene->GetObjectByID(_id)->Animate(0,1,4);*/
			//float valArray[5] = {1,HOScene->GetObjectByID(_id)->GetPosPointer()->x,HOScene->GetObjectByID(_id)->GetPosPointer()->y,1,0};
			//HOScene->GetObjectByID(_id)->TweenTo(valArray,2,0,true,CC_CALLBACK_1(L6_S5_MarysDream::OnFirstTweenComplete,this));
		}
		else if( !ProfileSystem::Instance->ActiveProfile.L6_S5_SophiaTabletDialogOver )
			ProfileSystem::Instance->ActiveProfile.L6_S5_SophiaTabletDialogOver = true;
		isDialogActive = false;
		Hud->EnableInput(true);
		Hud->ShowHud(true);
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )
		{
			if (ActivePopUpId != ePopInactive)
			{
				PopupArray[ActivePopUpId]->HandleInventoryDrop(dropPos, itemVal);
				return;
			}
			//If no drop items in this scene !!
			GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		


	}
	TaskPendingUpdate();
}

float L6_S5_MarysDream::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}


void L6_S5_MarysDream::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	if (!ProfileSystem::Instance->ActiveProfile.L6_S5_DialogsOver)
		isAnyTaskLeft = true;
	if (!ProfileSystem::Instance->ActiveProfile.L6_S5_HO_Complete && ProfileSystem::Instance->ActiveProfile.L6_S5_DialogsOver)
		isAnyTaskLeft = true;
	if (!ProfileSystem::Instance->ActiveProfile.L6_S5_FoundSophia && ProfileSystem::Instance->ActiveProfile.L6_S5_HO_Complete)
		isAnyTaskLeft = true;
	if (ProfileSystem::Instance->ActiveProfile.L6_S5_FoundSophia && !ProfileSystem::Instance->ActiveProfile.L6_S5_TabMinigameSolved)
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S5_MarysDream] = isAnyTaskLeft;
}

void L6_S5_MarysDream::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S5KidSandal])
		{
			if (ActivePopUpId != ePopInactive)
				InitHintTrailEmit(850, 450);
			else
				InitHintTrailEmit(1000, 590);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}
	bool haveHint = false;

	if(!ProfileSystem::Instance->ActiveProfile.L6_S5_HO_Complete && ProfileSystem::Instance->ActiveProfile.L6_S5_DialogsOver)
	{
		haveHint = true;
		for (int i = 0; i < (int)MisplacedObjectsArray.size(); i++)
		{
			if (!ProfileSystem::Instance->ActiveProfile.L6_S5_HOObjFound[i])
			{
				Hud->InitHintTrailEmit(MisplacedObjectsArray[i]->GetPosPointer()->x,MisplacedObjectsArray[i]->GetPosPointer()->y);
				Hud->ShowHintForSceneItem(PointSystem::CreateCPoint(OrderObjectPos[i]->center.x,OrderObjectPos[i]->center.y,0));
				break;
			}
		}
	}

	if (!ProfileSystem::Instance->ActiveProfile.L6_S5_DialogsOver)
	{
		haveHint = true;
		Hud->InitHintTrailEmit(&SophiaRect);
	}
	if( ActivePopUpId != ePopInactive )
	{
		haveHint = true;
		PopupArray[ActivePopUpId]->ProcessHint();
	}
	if (!ProfileSystem::Instance->ActiveProfile.L6_S5_TabMinigameSolved && ProfileSystem::Instance->ActiveProfile.L6_S5_HO_Complete)
	{
		haveHint = true;
		ClickablePolygon *rect = PopRectArray[eL6S5Pop1_Sophia];
		InitHintTrailEmit(&rect->center);
	}
	
	if (!haveHint)
		Hud->ShowNothingToDoMsgBanner();
}

void L6_S5_MarysDream::ShowSophia(int _id)//_id (1 to 4)
{
	if (_id >= 5)
	{
		for (int i = 1; i < 5; i++)
		{
			string _id = "sofia0" + MKSTR(i);
			HOScene->GetObjectByID(_id)->SetAlpha(1.0f);
			HOScene->GetObjectByID(_id)->SetEnable(false);
		} 
	}
	if(_id >= 5 )
		return;
	string objId = "sofia0" + MKSTR(_id);
	HOScene->GetObjectByID(objId)->SetEnable(true);
	HOScene->GetObjectByID(objId)->SetAlpha(0.0f);
	HOScene->GetObjectByID(objId)->FadeIn(2.0f);
}

void L6_S5_MarysDream::NavigateToScene()
{
	if (GFHud->InvBoxCtrl->navTapped)
	{
		if (Control::Input->LBclicked())
		{
			if (Control::Input->Mpos().x > GFHud->InvBoxCtrl->navRectX2&& Control::Input->Mpos().x < GFHud->InvBoxCtrl->navRectX1 && Control::Input->Mpos().y > GFHud->InvBoxCtrl->navRectY2&& Control::Input->Mpos().y < GFHud->InvBoxCtrl->navRectY1)
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