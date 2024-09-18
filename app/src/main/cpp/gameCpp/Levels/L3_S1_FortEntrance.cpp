//====================================
// DQFULP
// Hidden Object System
// Author : Amal
// LEVEL3 - SCENE3 : FortEntrance
//====================================

#include "L3_S1_FortEntrance.h"
#include "ProfileSystem.h"
#include "TextureFactory.h"
#include "ParticleEffect.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "L3_S1_EmberPopup.h"
#include "L3_S1_SkullFacePopup.h"
#include "L3_S1_FlagPickPopup.h"

const CPoint		CenterPoint(683,334,0);

//Scene Rects
const CRectangle  SceneRectPool(431.0f,219.0f,97.0f,167.0f);
const CRectangle  SceneRectFortInteriorDoor(569.0f,262.0f,89.0f,150.0f);
const CRectangle  SceneRectFortInteriorLadder(702.0f,222.0f,80.0f,100.0f);
const CRectangle  SceneRectHOLadder(132.0f,317.0f,315.0f,195.0f);

//Objective rects
const CRectangle  FlagRect(770.0f,298.0f,87.0f,130.0f);
const CRectangle  LadderLocation(693.0f,273.0f,67.0f,152.0f);
const CRectangle  SkullFaceRect(337.0f,130.0f,118.0f,126.0f);
const CRectangle  EmberArea(621.0f,534.0f,71.0f,34.0f);

static CPoint hintPoints[ePopL3S1_Max] = { CPoint(EmberArea.GetCenterX(), EmberArea.GetCenterY(), 0), 
											CPoint(FlagRect.GetCenterX(), FlagRect.GetCenterY(), 0),
											CPoint(SkullFaceRect.GetCenterX(), SkullFaceRect.GetCenterY(), 0)
										 };

L3_S1_FortEntrance::L3_S1_FortEntrance()
{

	int ArrayPts[] = {542,546,641,589,765,585,730,538,597,511};
	PopAreaPolyRing=  new CPolygon(5);
	PopAreaPolyRing->Init(ArrayPts);
	musicFadeOutTimer = 0;
	Visible = false;

	LoadHud(INVENTORY_V1);
	LoadScene("LV3\\S1\\S1.SCN");

	/*profile data for this scene*/
	vector <bool> objectives(ProfileSystem::Instance->ActiveProfile.L3_S1_Objectives, ProfileSystem::Instance->ActiveProfile.L3_S1_Objectives 
		+ sizeof ProfileSystem::Instance->ActiveProfile.L3_S1_Objectives / sizeof ProfileSystem::Instance->ActiveProfile.L3_S1_Objectives[0]);
	_userCompletedObjectives = objectives;

	SaveChange(S1_Objctive_OpenPuzzleDoor, ProfileSystem::Instance->ActiveProfile.L3_S1_P1LionHeadPuzzleCompleted);
	SaveChange(S1_Objctive_CollectGemEye, ProfileSystem::Instance->ActiveProfile.L3_S1_invGreenEyeCollected);
	SaveChange(S1_Objctive_OpenDoor, ProfileSystem::Instance->ActiveProfile.L3_S2_BackDoorOpened);
	SaveChange(S1_Objctive_FindLadder, ProfileSystem::Instance->ActiveProfile.L3_S1_InvBrokenLadderTaken);
	/*--*/

	InitSceneProperties();
	InitPopups();
	SceneObjectsUpdate();

	Visible = true;	//To render anything with renderlistentity

	//Objective Add
	if( !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L3O1_ExploreArea] )
	{
		ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L3O1_ExploreArea] = true;
		ProfileSystem::Instance->ActiveProfile.CurrentLvLObjectives[ProfileSystem::Instance->ActiveProfile.NumObjectivesActive]
				= eObj_L3O1_ExploreArea;
		//Increment Objectives
		ProfileSystem::Instance->ActiveProfile.NumObjectivesActive += 1;
		Hud->ShowObjectiveFX(eObj_L3O1_ExploreArea);
	}

	if (!ProfileSystem::Instance->ActiveProfile.L3_S1_Visited)
	{
		Hud->ShowBannerText(Banner_L3_S1_FortEntrence);
		ProfileSystem::Instance->ActiveProfile.L3_S1_Visited = true;
	}

		//Morph Object
#ifdef _CEBUILD
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S1Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("Eagle"), HOScene->GetObjectByID("Snake"));		
		CRectangle  MorphRect(605.0f,179.0f,58.0f,47.0f);
		Hud->setMorphObjRect(MorphRect);
	}
	else
	{
		HOScene->GetObjectByID("Eagle")->SetEnable(false);
	}
#else
	HOScene->GetObjectByID("Eagle")->SetEnable(false);
#endif 
		//MUSIC_SFX
	
	Control::Audio->LoadSample(aAmbL3s1FortEntrance,AudioAmb);
		if( !Control::Audio->IsPlaying(aAmbL3s1FortEntrance) )		
		{
			Control::Audio->PlaySample(aAmbL3s1FortEntrance,true);
		}
	
	//MUSIC_SFX
	TaskPendingUpdate();
}

void L3_S1_FortEntrance::InitSceneProperties()
{
	WaterFallLeftFX1 = new SuperFX("PRT\\L3\\WaterFallLeft1.INI", PointSystem::CreateCPoint(238,10,0), 1100); 
	WaterFallLeftFX1->Start();
	WaterFallLeftFX1->AdvanceByTime(2.0f);

	WaterFallLeftFX2 = new SuperFX("PRT\\L3\\WaterFallLeft2.INI", PointSystem::CreateCPoint(262,102,0), 1100); 
	WaterFallLeftFX2->Start();
	WaterFallLeftFX2->AdvanceByTime(2.0f);

	WaterFallbotomFX = new SuperFX("PRT\\L3\\WaterFallbotom.INI", PointSystem::CreateCPoint(262,287,0), 1100); 
	WaterFallbotomFX->Start();
	WaterFallbotomFX->AdvanceByTime(2.0f);

	WaterFallRightFX = new SuperFX("PRT\\L3\\WaterFallRight.INI", PointSystem::CreateCPoint(1008,171,0), 1100); 
	WaterFallRightFX->Start();
	WaterFallRightFX->AdvanceByTime(2.0f);

	fireflamesFX = new SuperFX("PRT\\L3\\fireflames.INI", PointSystem::CreateCPoint(654,534,0), 122); 
	fireflamesFX->Start();
	fireflamesFX->AdvanceByTime(2.0f);

	firefogFX = new SuperFX("PRT\\L3\\firefog.INI", PointSystem::CreateCPoint(655,548,0), 108); 
	firefogFX->Start();
	firefogFX->AdvanceByTime(2.0f);

	hodotsFX = NULL;
	holinesFX = NULL;

	if( !ProfileSystem::Instance->ActiveProfile.L3_S1_HOLadderCompleted )
	{
		if( !ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles )
		{
			//HO Particles
			hodotsFX = new SuperFX("PRT\\L3\\hodots.INI", PointSystem::CreateCPoint(313,431,0), 100); 
			hodotsFX->Start();
			hodotsFX->AdvanceByTime(2.0f);

			holinesFX = new SuperFX("PRT\\L3\\holines.INI", PointSystem::CreateCPoint(321,427,0), 100); 
			holinesFX->Start();
			holinesFX->AdvanceByTime(2.0f);
		}
	}

	string uids[] = {"Eye","DoorOpen","ladder","Ring_Open","Ring_Closed","Ring_Charcol","Ring_Fire","ho-after_Mask",
					 "plant-animation","Leaf_01","Leaf_02","Leaf_03","Crystalglow","popfireanim1","popfireanim2","popfireanim3",
					 "flaganim", "MouthOpen", "CrystalOnMouth"};

	int lengthOfUids = sizeof uids / sizeof uids[0];
	
	SceneProperties disabledObjsIndex[] = {S1_Eye,S1_OpenDoor,S1_Ladder,S1_RingOpen,S1_RingCharcol,S1_RingClose,S1_HoAfterMask,S1_MouthOpen, S1_CrystalOnMouth};

	for (int i = 0; i < lengthOfUids; i++)
	{
		SceneProperties p = SceneProperties(i);
		_sceneProperties[p] = HOScene->GetObjectByID(uids[i]);
	}

	int disabledArrayLength = sizeof disabledObjsIndex / sizeof disabledObjsIndex[0];
	for (int i = 0; i < disabledArrayLength; i++)
	{
		_sceneProperties[disabledObjsIndex[i]]->SetEnable(false);
	}

	_sceneProperties[S1_CrystelGlow]->SetBlendMode(eBlend_AlphaAdditive);
	_sceneProperties[S1_CrystelGlow]->Animate(0.2f, 1, 2);

	_sceneProperties[S1_Fire1]->SetFrame(12);
	_sceneProperties[S1_Fire1]->SetScale(1.4f);
	_sceneProperties[S1_Fire2]->SetFrame(50);
	_sceneProperties[S1_Fire2]->SetScale(1.6f);
}

void L3_S1_FortEntrance::InitPopups()
{	
	ActivePopupID = ePopL3S1_Inactive;

	if ( !_userCompletedObjectives[S1_Objctive_OpenPuzzleDoor] ||
		!_userCompletedObjectives[S1_Objctive_FindLadderStick] ||
		!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S1MemMirror])
	{
		L3_S1_EmberPopup *EmberPopup = new L3_S1_EmberPopup(ePopL3S1_Ember, Hud, HOScene, eZOrder_Popup, PopAreaPolyRing->center, CC_CALLBACK_1(L3_S1_FortEntrance::OnClosePopup, this));
		EmberPopup->setInterface(this);
		_popupsMap[ePopL3S1_Ember] = EmberPopup;
	}

	if (!_userCompletedObjectives[S1_Objctive_PlaceGemEye] || !_userCompletedObjectives[S1_Objctive_CollectCrystelkFromMouth])
		
	{
		CPoint centerPos = CPoint(SkullFaceRect.GetCenterX(), SkullFaceRect.GetCenterY(), 0);
		L3_S1_SkullFacePopup *SkullFacePopup = new L3_S1_SkullFacePopup(ePopL3S1_SkullFace, Hud, HOScene, eZOrder_Popup, centerPos, CC_CALLBACK_1(L3_S1_FortEntrance::OnClosePopup, this));
		SkullFacePopup->setInterface(this);
		SkullFacePopup->SavedState(_userCompletedObjectives[S1_Objctive_PlaceGemEye]);
		_popupsMap[ePopL3S1_SkullFace] = SkullFacePopup;
	}

	if (!_userCompletedObjectives[S1_Objctive_PickFlag])
	{
		CPoint centerPos = CPoint(FlagRect.GetCenterX(), FlagRect.GetCenterY(), 0);
		L3_S1_FlagPickPopup *FlagPopup = new L3_S1_FlagPickPopup(ePopL3S1_Flag, Hud, HOScene, eZOrder_Popup, centerPos, CC_CALLBACK_1(L3_S1_FortEntrance::OnClosePopup, this));
		FlagPopup->setInterface(this);
		_popupsMap[ePopL3S1_Flag] = FlagPopup;
	}
}

void L3_S1_FortEntrance::SaveChange(SceneObjectives objective, bool status)
{
	ProfileSystem::Instance->ActiveProfile.L3_S1_Objectives[objective] = status;
	_userCompletedObjectives[objective] = status;
}

void L3_S1_FortEntrance::UpdateHOSystem(Popup *popup, int objectiveVal)
{
	if (objectiveVal == -1)
		return;

	SceneObjectives objective = SceneObjectives(objectiveVal);
	ProfileSystem::Instance->ActiveProfile.L3_S1_Objectives[objective] = true;
	_userCompletedObjectives[objective] = true;
	SceneObjectsUpdate();
}

void L3_S1_FortEntrance::NewArea()
{
	SendMessage(ControlExit, ID, HOSM_NewArea);	
}

void L3_S1_FortEntrance::SceneObjectsUpdate()
{
	if (_userCompletedObjectives[S1_Objctive_PickFlag])
		_sceneProperties[S1_Flag]->SetEnable(false);	

	if (_userCompletedObjectives[S1_Objctive_FindLadder])
		_sceneProperties[S1_HoAfterMask]->SetEnable(true);

	if (_userCompletedObjectives[S1_Objctive_PlaceLadder])
			_sceneProperties[S1_Ladder]->SetEnable(true);

	if (_userCompletedObjectives[S1_Objctive_OpenDoor])
		_sceneProperties[S1_OpenDoor]->SetEnable(true);

	if (_userCompletedObjectives[S1_Objctive_OffTheFlames])
	{
		_sceneProperties[S1_RingCharcol]->SetEnable(true);
		_sceneProperties[S1_RingFire]->SetEnable(false);
		
		fireflamesFX->StopImmediate();
		firefogFX->StopImmediate();
	}

	if (_userCompletedObjectives[S1_Objctive_RemoveCharcols])
	{
		_sceneProperties[S1_RingCharcol]->SetEnable(false);
		_sceneProperties[S1_RingClose]->SetEnable(true);
	}

	if (_userCompletedObjectives[S1_Objctive_OpenPuzzleDoor] || _userCompletedObjectives[S1_Objctive_CollectGemEye])
	{
		_sceneProperties[S1_RingClose]->SetEnable(false);
		_sceneProperties[S1_RingOpen]->SetEnable(true);
	}

	if (_userCompletedObjectives[S1_Objctive_PlaceGemEye])
	{
		_sceneProperties[S1_Eye]->SetEnable(true);
		_sceneProperties[S1_MouthOpen]->SetEnable(true);
		_sceneProperties[S1_CrystalOnMouth]->SetEnable(true);
	}

	if (_userCompletedObjectives[S1_Objctive_CollectCrystelkFromMouth])
	{
		_sceneProperties[S1_CrystalOnMouth]->SetEnable(false);
	}
}

L3_S1_FortEntrance::~L3_S1_FortEntrance()
{
	SAFE_DELETE(PopAreaPolyRing);

	SAFE_DELETE(WaterFallLeftFX1);
	SAFE_DELETE(WaterFallLeftFX2);
	SAFE_DELETE(WaterFallbotomFX);
	SAFE_DELETE(WaterFallRightFX);
	SAFE_DELETE(fireflamesFX);
	SAFE_DELETE(firefogFX);

	SAFE_DELETE(hodotsFX);
	SAFE_DELETE(holinesFX);

	map<L3_Popup_Types, Popup*>::iterator popIt;
	for (popIt = _popupsMap.begin(); popIt != _popupsMap.end(); popIt ++)
	{
		SAFE_DELETE(popIt->second);
	}

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL3s1FortEntrance);
	Control::Audio->UnloadSample(aAmbL3s1FortEntrance);
	//MUSIC_SFX
}

void L3_S1_FortEntrance::Required()
{
	HiddenObjectSystem::Required();	

	if( Control::Audio->IsPlaying(aTrackL3ho) )
		{
			Control::Audio->Samples[aTrackL3ho]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL3ho]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL3ho);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL3theme) )
					  Control::Audio->PlaySample(aTrackL3theme, true);
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
	else if( Control::Audio->IsPlaying(aTrackL3puzzle) )
		{
			Control::Audio->Samples[aTrackL3puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
			if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
			{
				 Control::Audio->Samples[aTrackL3puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
				 Control::Audio->StopSample(aTrackL3puzzle);
				 musicFadeOutTimer = 0;
				 if( !Control::Audio->IsPlaying(aTrackL3theme) )
					  Control::Audio->PlaySample(aTrackL3theme, true);
			}
			musicFadeOutTimer += Control::LogicRate * 24;
		}
		else
		{
			if( !Control::Audio->IsPlaying(aTrackL3theme))
			{
				Control::Audio->TurnOffLvlMscTracks();
				Control::Audio->PlaySample(aTrackL3theme, true);
			}
		}
}

void L3_S1_FortEntrance::Update()
{
	NavigateToScene();

	HiddenObjectSystem::HOUpdate();

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing)
		return;

	#ifdef _PARTICLEEDITORENABLED
		if (_isParticleEditorActive)
		{
			return;
		}
	#endif

	if (ActivePopupID != ePopL3S1_Inactive)
		return;


	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed()) {
				if(Hud!=NULL && GFInstance->IsInGameMenuPopup != true)Hud->CallPauseMenu();
			}
	#endif
	
	CPoint mPos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);
	if (SceneRectPool.Intersection(&mPos))
	{


#ifdef TOUCH_UI
       // if(!GFHud->InvBoxCtrl->navTapped)
         //   Cursor::SetMode(CA_ExitLeft, eArea_L3_S3_Pool);
#else
        Cursor::SetMode(CA_ExitLeft, eArea_L3_S3_Pool);
#endif

        if( Control::Input->LBclicked() )
        {
#ifdef TOUCH_UI
            if(!GFHud->InvBoxCtrl->navTapped)
            {
                GFHud->InvBoxCtrl->GoToScene(eArea_L3_S3_Pool, 3, SceneRectPool);
            }
#else
            ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S3_Pool;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}
	}

	if (SceneRectFortInteriorDoor.Intersection(&mPos))
	{
		if( _userCompletedObjectives[S1_Objctive_OpenDoor])
		{


#ifdef TOUCH_UI
           // if(!GFHud->InvBoxCtrl->navTapped)
            //    Cursor::SetMode(CA_ExitUp, eArea_L3_S2_FortInterior);
#else
            Cursor::SetMode(CA_ExitUp, eArea_L3_S2_FortInterior);
#endif

            if( Control::Input->LBclicked() )
            {
#ifdef TOUCH_UI
                if(!GFHud->InvBoxCtrl->navTapped)
                {
                    GFHud->InvBoxCtrl->GoToScene(eArea_L3_S2_FortInterior, 0, SceneRectFortInteriorDoor);
                }
#else
                ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_FortInterior;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
		}
		else
		{
			if (Control::Input->LBclicked())
			{
				Hud->ShowBannerText(Banner_L3_S1_ClickingOnFortInteriorDoor);
			}
		}
	}

	if (!_userCompletedObjectives[S1_Objctive_OpenDoor] && SceneRectFortInteriorLadder.Intersection(&mPos))
	{
		if(_userCompletedObjectives[S1_Objctive_PlaceLadder])
		{


#ifdef TOUCH_UI
          //  if(!GFHud->InvBoxCtrl->navTapped)
           //     Cursor::SetMode(CA_ExitLeft, eArea_L3_S2_FortInterior);
#else
            Cursor::SetMode(CA_ExitLeft, eArea_L3_S2_FortInterior);
#endif

            if( Control::Input->LBclicked() )
            {
#ifdef TOUCH_UI
                if(!GFHud->InvBoxCtrl->navTapped)
                {
                    GFHud->InvBoxCtrl->GoToScene(eArea_L3_S2_FortInterior, 3, SceneRectFortInteriorLadder);
                }
#else
                ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S2_FortInterior;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
		}
		else
		{
			Cursor::SetMode(CA_Gear);
		}
	}

	if (SceneRectFortInteriorLadder.Intersection(&mPos) && !_userCompletedObjectives[S1_Objctive_PlaceLadder])
	{
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText(Banner_L3_S1_ClickNavFortWitoutLadder);
		}
	}

	if (SceneRectHOLadder.Intersection(&mPos) && !_userCompletedObjectives[S1_Objctive_FindLadder])
	{


#ifdef TOUCH_UI
       // if(!GFHud->InvBoxCtrl->navTapped)
          //  Cursor::SetMode(CA_Look);
#else
        Cursor::SetMode(CA_Look);
#endif

        if( Control::Input->LBclicked() )
        {
#ifdef TOUCH_UI
            if(!GFHud->InvBoxCtrl->navTapped)
            {
                GFHud->InvBoxCtrl->GoToScene(eArea_L3_S1_HOLadder, 0, SceneRectHOLadder);
            }
#else
            ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S1_HOLadder;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}
	}

	if (PopAreaPolyRing->Intersection(&mPos))
	{
		bool isDoorOpened = true;

		if (!_userCompletedObjectives[S1_Objctive_OpenPuzzleDoor])
		{
			Cursor::SetMode(CA_Look);
			isDoorOpened = false;
			if (Control::Input->LBclicked())
			{
				ActivePopupID = ePopL3S1_Ember;
				OnOpenPopup();
			}
		}

		if (_userCompletedObjectives[S1_Objctive_OpenPuzzleDoor] && !_userCompletedObjectives[S1_Objctive_CollectGemEye])
		{


#ifdef TOUCH_UI
          //  if(!GFHud->InvBoxCtrl->navTapped)
           //     Cursor::SetMode(CA_Look);
#else
            Cursor::SetMode(CA_Look);
#endif

            if( Control::Input->LBclicked() )
            {
#ifdef TOUCH_UI
                if(!GFHud->InvBoxCtrl->navTapped)
                {
                    GFHud->InvBoxCtrl->GoToScene(eArea_L3_S1_PZLionHead, 0, CRectangle(542,546, 100, 100));
                }
#else
                ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S1_PZLionHead;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
		}

		if ((isDoorOpened &&
			!_userCompletedObjectives[S1_Objctive_FindLadderStick]) ||
			(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S1MemMirror]))
		{
			Cursor::SetMode(CA_Look);

			if (Control::Input->LBclicked())
			{
				ActivePopupID = ePopL3S1_Ember;
				OnOpenPopup();
			}
		}
	}

	if (SkullFaceRect.Intersection(&mPos))
	{
		if (!_userCompletedObjectives[S1_Objctive_PlaceGemEye] || !_userCompletedObjectives[S1_Objctive_CollectCrystelkFromMouth])
		{
			Cursor::SetMode(CA_Look);

			if (Control::Input->LBclicked())
			{
				ActivePopupID = ePopL3S1_SkullFace;
				OnOpenPopup();
			}
		}
	}

	if (!_userCompletedObjectives[S1_Objctive_PickFlag] && FlagRect.Intersection(&mPos))
	{
		Cursor::SetMode(CA_Look);

		if (Control::Input->LBclicked())
		{
			ActivePopupID = ePopL3S1_Flag;
			OnOpenPopup();
		}
	}
}


void L3_S1_FortEntrance::Render(const CPoint* pCam)
{

}

void L3_S1_FortEntrance::ReceiveMessage(int val)
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

		if( Hud->GetDropInfo( dropPos, itemVal ) )
		{
			if (ActivePopupID != ePopL3S1_Inactive)
			{
				_popupsMap[ActivePopupID]->HandleInventoryDrop(dropPos, itemVal);
			}
			else
			{
				if (itemVal == eInv_L3S1_LadderFixed)
				{
					if (LadderLocation.Intersection(&dropPos))
					{
						Hud->ReleaseDrag();
						Hud->CloseInventory(); 

						//Clear Inventory. One Time Use Items.
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L3S1_LadderFixed] = false;
						Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L3S1_LadderFixed);

						SaveChange(S1_Objctive_PlaceLadder, true);
						SceneObjectsUpdate();
						TaskPendingUpdate();

						Control::Audio->QuickLoadAndPlaySFX("s1_ladderplaced");
					}
				}
				else
				{
					GFInstance->WrongInvDropMsg(Hud);
				}
			}
		}
	}
	else if( val == INV_CLOSE )
	{		
		if (ActivePopupID == ePopL3S1_Ember)
		{
			if (ProfileSystem::Instance->ActiveProfile.L3_S1_InvLadderStep1Taken && 
				ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S1MemMirror] &&
				ProfileSystem::Instance->ActiveProfile.L3_S1_EmberPopupState >= L3_Ember_PuzzleDoorOpened)
			{
				_popupsMap[ActivePopupID]->ClosePopup();
			}
		}
		else if (ActivePopupID == ePopL3S1_SkullFace)
		{
			if (_userCompletedObjectives[S1_Objctive_CollectCrystelkFromMouth])
			{
				_popupsMap[ActivePopupID]->ClosePopup();
			}
		}
		else if (ActivePopupID == ePopL3S1_Flag)
		{
			_popupsMap[ActivePopupID]->ClosePopup();
		}
	}	
}

float L3_S1_FortEntrance::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L3_S1_FortEntrance::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S1Collected)
		{
			InitHintTrailEmit(630, 200);
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S1MemMirror])
		{
			if (ActivePopupID != ePopInactive)
				InitHintTrailEmit(940, 380);
			else
				InitHintTrailEmit(650, 550);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}

	if (ActivePopupID != ePopL3S1_Inactive)
	{
		_popupsMap[ActivePopupID]->ProcessHint();
		return;
	}
		
	// popups
	for (int i = 0; i < (int)(_popupsMap.size()); i++)
	{
		if (_popupsMap[L3_Popup_Types(i)] && _popupsMap[L3_Popup_Types(i)]->isTaskLeft())
		{
			int index = _popupsMap[L3_Popup_Types(i)]->GetPopupId(); 
			Hud->InitHintTrailEmit(hintPoints[index].x, hintPoints[index].y);
			return;
		}
	}

	if (!_popupsMap[ePopL3S1_Ember] && 
		!ProfileSystem::Instance->ActiveProfile.L3_S1_invGreenEyeCollected)
	{
		Hud->InitHintTrailEmit(EmberArea.GetCenterX(), EmberArea.GetCenterY());
		return;
	}

	if (!_userCompletedObjectives[S1_Objctive_FindLadder])
	{
		Hud->InitHintTrailEmit(SceneRectHOLadder.GetCenterX(), SceneRectHOLadder.GetCenterY());
		return;
	}

	if (_userCompletedObjectives[S1_Objctive_OpenDoor])
	{
		if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_FortInterior])
		{
			Hud->InitHintTrailEmit(SceneRectFortInteriorDoor.GetCenterX(), SceneRectFortInteriorDoor.GetCenterY());
			return;
		}
	}
	else if ( _userCompletedObjectives[S1_Objctive_PlaceLadder])
	{
		if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_FortInterior])
		{
			Hud->InitHintTrailEmit(SceneRectFortInteriorLadder.GetCenterX(), SceneRectFortInteriorLadder.GetCenterY(), true, CA_ExitLeft);
			return;
		}
	}
	else if (ProfileSystem::Instance->ActiveProfile.L3_S1_InvLadderFixedTaken)
	{
		Hud->ShowHintForInvItem(eInv_L3S1_LadderFixed, &LadderLocation);
		return;
	}

	if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S3_Pool] ||
		!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L3_S3_Pool])
	{
		Hud->InitHintTrailEmit(SceneRectPool.GetCenterX(), SceneRectPool.GetCenterY(), true, CA_ExitLeft);
		return;
	}

	if(Hud->ShowHintForComboInvItems())
		return;

	Hud->teleportSystem->ShowTeleport();
	return;

}

void L3_S1_FortEntrance::OnOpenPopup()
{
#ifdef TOUCH_UI
	GFHud->InvBoxCtrl->NavigateText->Text = "";
	GFHud->InvBoxCtrl->travelArea = -1;
	GFHud->InvBoxCtrl->navTapped = false;
	GFHud->InvBoxCtrl->Navigate->Show(false);
	GFHud->InvBoxCtrl->NavArrow->Show(false);
#endif

	WaterFallLeftFX1->Show(false);
	WaterFallLeftFX2->Show(false);
	WaterFallbotomFX->Show(false);
	WaterFallRightFX->Show(false);

	_popupsMap[ActivePopupID]->OpenPopup();
}

void L3_S1_FortEntrance::OnClosePopup(Popup *popup)
{
	WaterFallLeftFX1->Show(true);
	WaterFallLeftFX2->Show(true);
	WaterFallbotomFX->Show(true);
	WaterFallRightFX->Show(true);

	ActivePopupID = ePopL3S1_Inactive;

	TaskPendingUpdate();
}

void L3_S1_FortEntrance::SparkleUpdate()
{
	if (ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		return;

	if (ActivePopupID != ePopL3S1_Inactive)
		return;

	CheckForPendingTask();

	if (!isSparkleUpdatePending)
		return;

	HiddenObjectSystem::SparkleUpdate();
}

void L3_S1_FortEntrance::CheckForPendingTask()
{
	isSparkleUpdatePending = false;

	for (int i = 0; i < (int)(_popupsMap.size()); i++)
	{
		if (_popupsMap[L3_Popup_Types(i)] && _popupsMap[L3_Popup_Types(i)]->isTaskLeft())
		{
			int index = _popupsMap[L3_Popup_Types(i)]->GetPopupId(); 
			sparklePointsVec.push_back(hintPoints[index]);
			isSparkleUpdatePending = true;
		}
	}

	if (!_userCompletedObjectives[S1_Objctive_FindLadder])
	{
		sparklePointsVec.push_back(CPoint(SceneRectHOLadder.GetCenterX(), SceneRectHOLadder.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}
}

void L3_S1_FortEntrance::TaskPendingUpdate()
{
	for (int i = 0; i < (int)(_popupsMap.size()); i++)
	{
		if (_popupsMap[L3_Popup_Types(i)] && _popupsMap[L3_Popup_Types(i)]->isTaskLeft())
		{
			ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S1_FortEntrance] = true;
			return;
		}
	}
	
	if (ProfileSystem::Instance->ActiveProfile.L3_S1_InvLadderFixedTaken &&
		!_userCompletedObjectives[S1_Objctive_PlaceLadder])
	{
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S1_FortEntrance] = true;
		return;
	}

	if (!ProfileSystem::Instance->ActiveProfile.L3_S1_HOLadderCompleted)
	{
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S1_FortEntrance] = true;
		return;
	}

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S1_FortEntrance] = false;

	if (!ProfileSystem::Instance->ActiveProfile.L3_S2_WallBrocken)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_FortInterior] = true;
}


void L3_S1_FortEntrance::NavigateToScene()
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