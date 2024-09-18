//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL4 - SCENE2 : RIVER SIDE
//====================================

#include "L4_S2_RiverSide.h"
#include "MeshObject.h"
#include "Util.h"

const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  PopCircleRect(484.0f,154.0f,376.0f,368.0f);

//SCENE RECTS
const CRectangle  SceneRectHillTop(935.0f,50.0f,133.0f,214.0f);
const CRectangle  SceneRectCave(701.0f,345.0f,111.0f,108.0f);

//PUZZLE RECTS
const CRectangle  PZAllegatorHeadRect(601.0f,499.0f,267.0f,91.0f);
const CRectangle  PZDoorRect(694.0f,322.0f,118.0f,134.0f);



L4_S2_RiverSide::L4_S2_RiverSide()
{
	LoadHud(INVENTORY_V1);
	LoadScene("LV4\\S2\\S2.SCN");


	HOScene->GetObjectByID("WaterFallAnim1")->SetScale(2.00f);
	HOScene->GetObjectByID("WaterFallAnim2")->SetScale(2.00f);
	HOScene->GetObjectByID("WaterFallAnim3")->SetScale(2.00f);
	waterMesh = NULL;
	waterMesh = new MeshObject(HOScene->GetObjectByID("WaterMesh"), 5, 5);
	waterMesh->meshFX->SetSpeedAndDisplacement(3, 3, 5, 5);
	HOScene->PushObject(waterMesh);
	leafMesh = NULL;
	leafMesh = new MeshObject(HOScene->GetObjectByID("BambooLeavesMesh"), 5, 5);
	leafMesh->meshFX->SetSpeedAndDisplacement(3, 3, 3, 3);
	HOScene->PushObject(leafMesh);
	HOScene->GetObjectByID("BambooPlant")->SetPivot(170, 206);
	HOScene->GetObjectByID("BambooPlant")->Animate(0, 0, 0.5, 0, 4, 180);

	blurBg = HOScene->GetObjectByID("BlurBg");
	blurBg->SetScale(2);
	blurBg->SetAlpha(0);
	for (int i = 0; i < 7; i++)
	{
		std::string eyeId = std::string("AlligatorEye_") + MKSTR(i);
		CObject* obj = HOScene->GetObjectByID(eyeId);
		obj->SetBlendMode(eBlend_AlphaAdditive);
		obj->Animate(0.0f, 2.0f, 5.0f + rand() % 3, 30.0f * (rand() % 7));
	}
	

	ActivePopupID = eL4S2Pop_Inactive;

	InitPopups();
	GFInstance->PostUpdate(2);
	
	

	InitSceneElements();
	initAnimations();

	SuperFX *S2_FruitGlowFX;


	S2_FruitGlowFX = new SuperFX("PRT\\L4\\S2_FruitGlow.INI", PointSystem::CreateCPoint(300,213,0), 100); 
	if (!ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsPlucked)
	{
		S2_FruitGlowFX->Start();
	}
	S2_FruitGlowFX->AdvanceByTime(2.0f);
	_arrayOfVFX.push_back(S2_FruitGlowFX);
	S2_FruitGlowFX = NULL;


	S2_FruitGlowFX = new SuperFX("PRT\\L4\\S2_FruitGlow.INI", PointSystem::CreateCPoint(480,278,0), 100); 
	S2_FruitGlowFX->Start();
	S2_FruitGlowFX->AdvanceByTime(2.0f);
	_arrayOfVFX.push_back(S2_FruitGlowFX);
	S2_FruitGlowFX = NULL;


	S2_FruitGlowFX = new SuperFX("PRT\\L4\\S2_FruitGlow.INI", PointSystem::CreateCPoint(571,154,0), 100); 
	S2_FruitGlowFX->Start();
	S2_FruitGlowFX->AdvanceByTime(2.0f);
	_arrayOfVFX.push_back(S2_FruitGlowFX);
	S2_FruitGlowFX = NULL;


	S2_FruitGlowFX = new SuperFX("PRT\\L4\\S2_FruitGlow.INI", PointSystem::CreateCPoint(174,111,0), 100); 
	S2_FruitGlowFX->Start();
	S2_FruitGlowFX->AdvanceByTime(2.0f);
	_arrayOfVFX.push_back(S2_FruitGlowFX);
	S2_FruitGlowFX = NULL;

	SuperFX *S2_PurplePlantGlowFX;

	S2_PurplePlantGlowFX = new SuperFX("PRT\\L4\\S2_PurplePlantGlow.INI", PointSystem::CreateCPoint(882,191,0), 100); 
	S2_PurplePlantGlowFX->Start();
	S2_PurplePlantGlowFX->AdvanceByTime(2.0f);
	_arrayOfVFX.push_back(S2_PurplePlantGlowFX);

	SuperFX *GlowPlantFX;

	GlowPlantFX = new SuperFX("PRT\\L4\\S5GlowPlant01.INI", PointSystem::CreateCPoint(1238,195,0), 100); 
	GlowPlantFX->Start();
	GlowPlantFX->AdvanceByTime(2.0f);
	_arrayOfVFX.push_back(GlowPlantFX);
	GlowPlantFX = NULL;

	GlowPlantFX = new SuperFX("PRT\\L4\\S5GlowPlant01.INI", PointSystem::CreateCPoint(1295,197,0), 100); 
	GlowPlantFX->Start();
	GlowPlantFX->AdvanceByTime(2.0f);
	_arrayOfVFX.push_back(GlowPlantFX);
	GlowPlantFX = NULL;

	SuperFX *S2_TreeBottomGlow1FX;

	S2_TreeBottomGlow1FX = new SuperFX("PRT\\L4\\S2_TreeBottomGlow1.INI", PointSystem::CreateCPoint(106,526,0), 100); 
	S2_TreeBottomGlow1FX->Start();
	S2_TreeBottomGlow1FX->AdvanceByTime(2.0f);
	_arrayOfVFX.push_back(S2_TreeBottomGlow1FX);
	S2_TreeBottomGlow1FX = NULL;


	S2_TreeBottomGlow1FX = new SuperFX("PRT\\L4\\S2_TreeBottomGlow2.INI", PointSystem::CreateCPoint(89,678,0), 100); 
	S2_TreeBottomGlow1FX->Start();
	S2_TreeBottomGlow1FX->AdvanceByTime(2.0f);
	_arrayOfVFX.push_back(S2_TreeBottomGlow1FX);
	S2_TreeBottomGlow1FX = NULL;


	S2_TreeBottomGlow1FX = new SuperFX("PRT\\L4\\S2_TreeBottomGlow2.INI", PointSystem::CreateCPoint(993,77,0), 100); 
	S2_TreeBottomGlow1FX->Start();
	S2_TreeBottomGlow1FX->AdvanceByTime(2.0f);
	_arrayOfVFX.push_back(S2_TreeBottomGlow1FX);
	S2_TreeBottomGlow1FX = NULL;

	SuperFX *S2_WaterFallBottomSmokeFX;

	S2_WaterFallBottomSmokeFX = new SuperFX("PRT\\L4\\S2_WaterFallBottomSmoke.INI", PointSystem::CreateCPoint(748,445,0), 100); 
	S2_WaterFallBottomSmokeFX->Start();
	S2_WaterFallBottomSmokeFX->AdvanceByTime(2.0f);
	_arrayOfVFX.push_back(S2_WaterFallBottomSmokeFX);
	S2_WaterFallBottomSmokeFX = NULL;

	SuperFX *S2_WaterFallMiddleSmokeFX;

	S2_WaterFallMiddleSmokeFX = new SuperFX("PRT\\L4\\S2_WaterFallMiddleSmoke.INI", PointSystem::CreateCPoint(749,283,0), 100); 
	S2_WaterFallMiddleSmokeFX->Start();
	S2_WaterFallMiddleSmokeFX->AdvanceByTime(2.0f);
	_arrayOfVFX.push_back(S2_WaterFallMiddleSmokeFX);
	S2_WaterFallMiddleSmokeFX = NULL;

	SceneObjectsUpdate();
	
	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL4s2RiverSideAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL4s2RiverSideAmb) )		
	{
		Control::Audio->PlaySample(aAmbL4s2RiverSideAmb,true);
	}
	
	//MUSIC_SFX
}

void L4_S2_RiverSide::InitPopups()
{
	ActivePopupID = eL4S2Pop_Inactive;
	for (int i = 0; i < eL4S2Pop_Max; i++)
	{
		PopupArray[i] = NULL;
	}

	int popID=0;

	//Popup 1
	popID = eL4S2Pop1_MagicFruit;

	if(!ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsPlucked )
	{
		int popPts[] = {237,135,389,78,576,158,566,263,418,281,305,247,235,133};
		ClickablePolygon* popClickPoly=  new ClickablePolygon(popID, 7, popPts, CC_CALLBACK_1(L4_S2_RiverSide::OnPopupClicked, this));

		Popup* Pop1Scene = new L4S2_Pop1MagicFroot(eL4S2Pop1_MagicFruit, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L4_S2_RiverSide::OnPopupClosed, this));
		Pop1Scene->SetPopupBoundingBox(PopAreaRect);
		Pop1Scene->SetClickAreaToOpenPopup(popClickPoly);
		PopupArray[popID]	= Pop1Scene;
	}

	//Popup 2
	popID = eL4S2Pop2_Bamboo;
	if(!ProfileSystem::Instance->ActiveProfile.L4_S2_Pop2BambooTaskCompleted || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S2Banana] )
	{

		int popPts[] = {1136,594,1031,446,976,365,1030,240,1149,198,1293,283,1329,369,1257,440,1181,471};
		ClickablePolygon* popClickPoly=  new ClickablePolygon(popID, 9, popPts, CC_CALLBACK_1(L4_S2_RiverSide::OnPopupClicked, this));
		Popup* Pop2Scene = new L4S2_Pop2Bamboo(eL4S2Pop2_Bamboo, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L4_S2_RiverSide::OnPopupClosed, this));
		Pop2Scene->SetPopupBoundingBox(PopAreaRect);
		Pop2Scene->SetClickAreaToOpenPopup(popClickPoly);
		PopupArray[popID]	= Pop2Scene;
	}

	//Popup 3
	popID = eL4S2Pop3_TreeBottom;
	if(!ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsTaken || !ProfileSystem::Instance->ActiveProfile.L4_S2_InvTwig1Taken)
	{

		int popPts[] = {63,502,143,464,215,429,286,473,356,511,414,523,405,562,363,584,190,566,75,569};
		ClickablePolygon* popClickPoly=  new ClickablePolygon(popID, 10, popPts, CC_CALLBACK_1(L4_S2_RiverSide::OnPopupClicked, this));
		Popup* Pop3Scene = new L4S2_Pop3TreeBottom(eL4S2Pop3_TreeBottom, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L4_S2_RiverSide::OnPopupClosed, this));
		Pop3Scene->SetPopupBoundingBox(PopAreaRect);
		Pop3Scene->SetClickAreaToOpenPopup(popClickPoly);
		Pop3Scene->SetSparkleUpdatePoint(HOScene->GetObjectByID("FruitUnderTree1")->Pos);
		PopupArray[popID]	= Pop3Scene;
	}

	//Popup 4
	popID = eL4S2Pop4_Grass;
	if(!ProfileSystem::Instance->ActiveProfile.L4_S2_GrassTaken )
	{
		int popPts[] = {388,440,468,455,491,496,444,495,409,510,359,494,353,440,385,439};
		ClickablePolygon* popClickPoly=  new ClickablePolygon(popID, 8, popPts, CC_CALLBACK_1(L4_S2_RiverSide::OnPopupClicked, this));
		Popup* Pop4Scene = new L4S2_Pop4Grass(eL4S2Pop4_Grass, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L4_S2_RiverSide::OnPopupClosed, this));
		Pop4Scene->SetPopupBoundingBox(PopAreaRect);
		Pop4Scene->SetClickAreaToOpenPopup(popClickPoly);
		PopupArray[popID]	= Pop4Scene;
	}

	GFInstance->PostUpdate(1);
}

void L4_S2_RiverSide::OnPopupClicked(int popupId)
{
	if (ActivePopupID == eL4S2Pop_Inactive)
	{
		ActivePopupID = popupId;
		PopupArray[popupId]->OpenPopup();
	}
}

void L4_S2_RiverSide::OnPopupClosed(Popup * popup)
{
	SceneObjectsUpdate();
//	int popID = popup->GetPopupId(); //Sac: Unused variable, so commented it
	ActivePopupID = eL4S2Pop_Inactive;
	TaskPendingUpdate();
}

L4_S2_RiverSide::~L4_S2_RiverSide()
{
	for (int i = 0; i < eL4S2Pop_Max; i++)
	{
		SAFE_DELETE(PopupArray[i]);
	}

	for (int i = 0; i < (int)_arrayOfVFX.size(); i++)
	{
		SAFE_DELETE(_arrayOfVFX.at(i));
	}
	_arrayOfVFX.clear();

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL4s2RiverSideAmb);
	Control::Audio->UnloadSample(aAmbL4s2RiverSideAmb);
	//MUSIC_SFX
}

void L4_S2_RiverSide::Required()
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

bool L4_S2_RiverSide::CheckIfPopupActive(int Id)
{
	bool flag  = true;
	switch (Id)
	{
	case eL4S2Pop1_MagicFruit:	if(ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsPlucked)
									flag = false;
		break;
	case eL4S2Pop2_Bamboo:		if( ProfileSystem::Instance->ActiveProfile.L4_S2_Pop2BambooTaskCompleted)
									flag = false;
		break;
	case eL4S2Pop3_TreeBottom:	if(ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsTaken && ProfileSystem::Instance->ActiveProfile.L4_S2_InvTwig1Taken )
									flag = false;
		break;
	case eL4S2Pop4_Grass:		if(ProfileSystem::Instance->ActiveProfile.L4_S2_GrassTaken)
									flag = false;
		break;
	default:
		break;
	}
	return(flag);
}


void L4_S2_RiverSide::Update()
{
#ifdef _PARTICLEEDITORENABLED
	if(_isParticleEditorActive)
		return;
#endif // _PARTICLEEDITORENABLED

	NavigateToScene();
	HiddenObjectSystem::HOUpdate();
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing)
		return;


	CPoint pos = Control::Input->Mpos();
	if (ActivePopupID == eL4S2Pop_Inactive)
	{
		if (!ProfileSystem::Instance->ActiveProfile.L4S2_Visited)
		{
			Hud->ShowBannerText("L4looks");
			ProfileSystem::Instance->ActiveProfile.L4S2_Visited = true;
		}

		bool isMouseOverOnClickToOpenPopup = false;
		for (int i = 0; i < eL4S2Pop_Max; i++)
		{
			Popup * popup = PopupArray[i];
			if (popup)
			{
				if(popup->GetClickAreaToOpenPopup()->GetCurrentMouseState() == eMouseState_Over || popup->GetClickAreaToOpenPopup()->GetCurrentMouseState() == eMouseState_Down)
				{
					if( CheckIfPopupActive(i))
						isMouseOverOnClickToOpenPopup = true;
					break;
				}
			}
		}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S1_SwampEntrance;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}	
	#endif	
		if (isMouseOverOnClickToOpenPopup)
		{
			Cursor::SetMode(CA_Look);
		}


		else if( SceneRectHillTop.Intersection(&pos) )
		{
			if (ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsTaken)
			{
				
				#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitUp,eArea_L4_S4_HillTop);
#else
		Cursor::SetMode(CA_ExitUp,eArea_L4_S4_HillTop);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L4_S4_HillTop, 0, SceneRectHillTop);
			}
#else			
			Control::Audio->PlayFootStepsFX();
					ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S4_HillTop;
					SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif

					
				}
			}
			else
			{
				Cursor::SetMode(CA_ExitUp,eArea_L4_S4_HillTop);
				if( Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L4long");
					blurBg->FadeInOut();
				}
			}
		}
		else if( PZDoorRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S2_DoorToCaveOpened)
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
				GFHud->InvBoxCtrl->GoToScene(eArea_L4_S2_PZDoor, 0, PZDoorRect);
			}
#else			
			Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S2_PZDoor;
				SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
									
			}			
			return;		
		}
		else if( SceneRectCave.Intersection(&pos) )
		{
			
			#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitUp,eArea_L4_S3_Cave);
#else
		Cursor::SetMode(CA_ExitUp,eArea_L4_S3_Cave);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L4_S3_Cave, 0, SceneRectCave);
			}
#else			
			Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S3_Cave;
				SendMessage(ControlExit, ID, HOSM_NewArea);		
#endif
								
			}
		}
		else if( PZAllegatorHeadRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S2_InvGrinderHandleTaken)
		{
#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_Look);
#else
		Cursor::SetMode(CA_Look);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L4_S2_PZAlligatorHead, 0, PZAllegatorHeadRect);
			}
#else			
			Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S2_PZAlligatorHead;
				SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
									
			}			
			return;		
		}
		else if( IsMouseOverGoBackArea(&pos) )
		{
			
			#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitDown,eArea_L4_S1_SwampEntrance);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L4_S1_SwampEntrance);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L4_S1_SwampEntrance, 2);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S1_SwampEntrance;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
				
			}	
		}
		else
		{
			Cursor::SetMode(CA_Normal);
		}
	}


}


//void L4_S2_RiverSide::SparkleUpdate()
//{
//	if(isSparkleUpdatePending)
//	{
//		isSparkleUpdatePending = false;
//		if(SparkleFX==NULL)
//		{
//			SparkleFX = new SuperFX("PRT\\glint.INI",PointSystem::CreateCPoint(0,0,0),120);		
//		}
//		//Game Logic Goes here
//		numSparkles = 0;
//		currentSparkleID = 0;
//
//		//Check for Popup Tasks
//		for (int i = 0; i < eL4S2Pop_Max; i++)
//		{
//			Popup * popup = PopupArray[i];
//			if (popup)
//			{
//				if(popup->GetPopupState() == ePopupState_Inactive) //if popup is active
//				{
//					if(popup->isTaskLeft())
//					{
//						sparklePoints[numSparkles].SetXY(popup->GetClickAreaToOpenPopup()->center.x, popup->GetClickAreaToOpenPopup()->center.y);
//						numSparkles++;
//					}
//				}
//			}
//
//		}
//	}
//	sparkleTimer += Control::LogicRate;
//	if(sparkleTimer>sparkleThreshold)
//	{
//		sparkleTimer = 0;
//		if(numSparkles>0)
//		{			
//			SparkleFX->SetPos(&sparklePoints[currentSparkleID]);
//			SparkleFX->Start();
//			currentSparkleID += 1;
//			if(currentSparkleID>=numSparkles)
//				currentSparkleID = 0;
//		}
//	}
//}

void L4_S2_RiverSide::Render(const CPoint* pCam)
{

}


void L4_S2_RiverSide::TakeInventoryItem(EHOInventory ItemID)
{

}


void L4_S2_RiverSide::ReceiveMessage(int val)
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
		if(ActivePopupID != ePopInactive)
		{
			if(PopupArray[ActivePopupID]!=NULL)
				PopupArray[ActivePopupID]->UpdateBgGraphic();
		}
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )
		{
			//If no drop items in this scene !!
			if (ActivePopupID != eL4S2Pop_Inactive)
			{
				PopupArray[ActivePopupID]->HandleInventoryDrop(dropPos, itemVal);
			}
			else
			{
				GFInstance->WrongInvDropMsg(Hud);
			}
		}
	}
	else if( val == INV_CLOSE )
	{		


	}	
	TaskPendingUpdate();
}

float L4_S2_RiverSide::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L4_S2_RiverSide::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;
	for (int i = 0; i < eL4S2Pop_Max; i++)
	{
		Popup * popup = PopupArray[i];
		if (popup)
		{
			if(popup->GetPopupState() == ePopupState_Inactive) //if popup is active
			{
				isAnyTaskLeft = popup->isTaskLeft();
			}
		}

	}

	if( !ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleSolved && ProfileSystem::Instance->ActiveProfile.L4_S5_InvAlligatorHeadTaken )
		isAnyTaskLeft = true;
	if( !ProfileSystem::Instance->ActiveProfile.L4_S2_InvGrinderHandleTaken && ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleSolved )
		isAnyTaskLeft = true;
	if( !ProfileSystem::Instance->ActiveProfile.L4_S2_DoorToCaveOpened && ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorKeyTaken )
		isAnyTaskLeft = true;
	if( !ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L4_S4_HillTop])
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S2_RiverSide] = isAnyTaskLeft;

	if( ProfileSystem::Instance->ActiveProfile.L4_S2_InvGrinderHandleTaken && ProfileSystem::Instance->ActiveProfile.L4_S4_OilMachinePlaced &&  !ProfileSystem::Instance->ActiveProfile.L4_S4_GrinderHandlePlaced )
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S4_HillTop] = true;


}

void L4_S2_RiverSide::SparkleUpdate()
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

void L4_S2_RiverSide::CheckForPendingTask()
{
	isSparkleUpdatePending = false;
	for (int i = 0; i < eL4S2Pop_Max; i++)
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
	if( !ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleSolved && ProfileSystem::Instance->ActiveProfile.L4_S5_InvAlligatorHeadTaken )
	{
		sparklePointsVec.push_back(CPoint(PZAllegatorHeadRect.GetCenterX(), PZAllegatorHeadRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}
	else if( !ProfileSystem::Instance->ActiveProfile.L4_S2_InvGrinderHandleTaken && ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleSolved )
	{
		sparklePointsVec.push_back(CPoint(PZAllegatorHeadRect.GetCenterX(), PZAllegatorHeadRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}
	
	if( !ProfileSystem::Instance->ActiveProfile.L4_S2_DoorToCaveOpened && ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorKeyTaken )
	{
		sparklePointsVec.push_back(CPoint(PZDoorRect.GetCenterX(), PZDoorRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}
}


void L4_S2_RiverSide::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S2Banana])
		{
			if (ActivePopupID != ePopInactive)
				InitHintTrailEmit(890, 490);
			else
				InitHintTrailEmit(1150, 480);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}

	if( ActivePopupID != ePopInactive )
	{
		PopupArray[ActivePopupID]->ProcessHint();
		return;
	}

	int iNonHOHint = 0; 
	int MaxNonHOHints = 16;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
		case 0: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L4_S2_AlligatorPuzzleSolved && ProfileSystem::Instance->ActiveProfile.L4_S5_InvAlligatorHeadTaken )
			{
				Hud->InitHintTrailEmit(PZAllegatorHeadRect.GetCenterX(), PZAllegatorHeadRect.GetCenterY());
				return;
			}
			break;
		case 1: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L4_S2_DoorToCaveOpened && ProfileSystem::Instance->ActiveProfile.L4_S5_InvDoorKeyTaken )
			{
				Hud->InitHintTrailEmit(PZDoorRect.GetCenterX(), PZDoorRect.GetCenterY());
				return;
			}
			break;
		case 2: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L4_S2_BambooStickTaken && ProfileSystem::Instance->ActiveProfile.L4_S1_InvKnifeCollected )
			{
				Hud->InitHintTrailEmit( PopupArray[eL4S2Pop2_Bamboo]->GetClickAreaToOpenPopup()->center.x, PopupArray[eL4S2Pop2_Bamboo]->GetClickAreaToOpenPopup()->center.y);
				return;
			}
			break;
		case 3: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L4_S2_GrassTaken &&
				( ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S1_Knife] ||  ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife] ))
			{
				Hud->InitHintTrailEmit( PopupArray[eL4S2Pop4_Grass]->GetClickAreaToOpenPopup()->center.x, PopupArray[eL4S2Pop4_Grass]->GetClickAreaToOpenPopup()->center.y);
				return;
			}
			break;
		case 4: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsPlucked && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_CB_BambooWithKnife] )
			{
				Hud->InitHintTrailEmit( PopupArray[eL4S2Pop1_MagicFruit]->GetClickAreaToOpenPopup()->center.x, PopupArray[eL4S2Pop1_MagicFruit]->GetClickAreaToOpenPopup()->center.y);
				return;
			}
			break;
		case 5: ++iNonHOHint;
			if(( !ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsTaken && ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsPlucked ) || 
				! ProfileSystem::Instance->ActiveProfile.L4_S2_InvTwig1Taken)
			{
				Hud->InitHintTrailEmit( FruitUnderTree1->Pos.x, FruitUnderTree1->Pos.y);
				return;
			}
			break;
		case 6: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick0Taken || !ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick1Taken ||
				!ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick2Taken || !ProfileSystem::Instance->ActiveProfile.L4_S2_InvSmallBambooStick3Taken )
			{
				Hud->InitHintTrailEmit( PopupArray[eL4S2Pop2_Bamboo]->GetClickAreaToOpenPopup()->center.x, PopupArray[eL4S2Pop2_Bamboo]->GetClickAreaToOpenPopup()->center.y);
				return;
			}
			break;
		case 7: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S4_HillTop] || (!ProfileSystem::Instance->ActiveProfile.L4_S4_Visited  && 
				ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsTaken ))
			{
				InitHintTrailEmit(SceneRectHillTop.GetCenterX(), SceneRectHillTop.GetCenterY(),true,CA_ExitUp); 
				return;
			}
			break;
		case 8: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S1_SwampEntrance] )
			{
				InitHintTrailEmit(HoveredBackBtnRectMax.GetCenterX(), HoveredBackBtnRectMax.GetCenterY(),true,CA_ExitDown); 
				return;
			}
			break;
		case 9: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
		case 10: ++iNonHOHint;
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

void L4_S2_RiverSide::SceneObjectsUpdate()
{
	if (ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsPlucked)
	{
		_arrayOfVFX.at(0)->StopImmediate();
	}

	TaskPendingUpdate();
}

void L4_S2_RiverSide::InitSceneElements()
{
	BambooPlant				= HOScene->GetObjectByID("BambooPlant");
	BambooSingle			= HOScene->GetObjectByID("BambooSingle");
	BgLight					= HOScene->GetObjectByID("BgLight");
	CaveDoor01				= HOScene->GetObjectByID("CaveDoor01");
	CaveDoor02				= HOScene->GetObjectByID("CaveDoor02");
	FriutOnTree01			= HOScene->GetObjectByID("FriutOnTree01");
	FriutOnTree03			= HOScene->GetObjectByID("FriutOnTree03");
	if (ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsPlucked)
	{
		FriutOnTree03->SetEnable(false);

	}
	FriutOnTree04			= HOScene->GetObjectByID("FriutOnTree04");
	FriutOnTree05			= HOScene->GetObjectByID("FriutOnTree05");
	FriutOnTree06			= HOScene->GetObjectByID("FriutOnTree06");
	FriutOnTree07			= HOScene->GetObjectByID("FriutOnTree07");
	FruitOnTree02			= HOScene->GetObjectByID("FruitOnTree02");
	FruitUnderTree1			= HOScene->GetObjectByID("FruitUnderTree1");
	if (!ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsPlucked || ProfileSystem::Instance->ActiveProfile.L4_S2_FruitsTaken)
	{
		FruitUnderTree1->SetEnable(false);
	}
	GlowDots01				= HOScene->GetObjectByID("GlowDots01");
	GlowDots02				= HOScene->GetObjectByID("GlowDots02");
	MagicPlant				= HOScene->GetObjectByID("MagicPlant");
	MagicPlantGlow			= HOScene->GetObjectByID("MagicPlantGlow");
	PinkFruitGlow			= HOScene->GetObjectByID("PinkFruitGlow");
	ScaryGlow01				= HOScene->GetObjectByID("ScaryGlow01");
	ScaryGlow02				= HOScene->GetObjectByID("ScaryGlow02");
	ScaryGlow03				= HOScene->GetObjectByID("ScaryGlow03");
	ScaryGlow04				= HOScene->GetObjectByID("ScaryGlow04");

	if( ProfileSystem::Instance->ActiveProfile.L4_S3_IsDoorPuzzleSolved )
		CaveDoor01				->SetEnable(false);
	else
		CaveDoor02				->SetEnable(false);



}

void L4_S2_RiverSide::initAnimations()
{
	GlowDots01->SetBlendMode(eBlend_AlphaAdditive);
	GlowDots01->Animate(0.15f, 0.99f, 2.0f);

	GlowDots02->SetBlendMode(eBlend_AlphaAdditive);
	GlowDots02->Animate(0.15f, 0.95f, 2.0f, 90);

	MagicPlantGlow->SetBlendMode(eBlend_AlphaAdditive);
	MagicPlantGlow->Animate(0.45f, 0.95f, 3.0f);

	PinkFruitGlow->SetBlendMode(eBlend_AlphaAdditive);
	PinkFruitGlow->Animate(0.45f, 0.99f, 3.0f, 180);

	ScaryGlow01->SetBlendMode(eBlend_AlphaAdditive);
	ScaryGlow01->Animate(0, 2, 3.0f);
	ScaryGlow02->SetBlendMode(eBlend_AlphaAdditive);
	ScaryGlow02->Animate(0, 2, 3.0f, 90);
	ScaryGlow03->SetBlendMode(eBlend_AlphaAdditive);
	ScaryGlow03->Animate(0, 2, 3.0f, 180);
	ScaryGlow04->SetBlendMode(eBlend_AlphaAdditive);
	ScaryGlow04->Animate(0, 2, 3.0f, 270);

	FriutOnTree01->SetBlendMode(eBlend_AlphaAdditive);
	FriutOnTree01->Animate(0.85f, 0.99f, 3.0f, 30);
	FriutOnTree03->SetBlendMode(eBlend_AlphaAdditive);
	FriutOnTree03->Animate(0.85f, 0.99f, 2.0f, 60);
	FriutOnTree04->SetBlendMode(eBlend_AlphaAdditive);
	FriutOnTree04->Animate(0.85f, 0.99f, 4.0f, 90);
	FriutOnTree05->SetBlendMode(eBlend_AlphaAdditive);
	FriutOnTree05->Animate(0.85f, 0.99f, 3.0f, 120);
	FriutOnTree06->SetBlendMode(eBlend_AlphaAdditive);
	FriutOnTree06->Animate(0.85f, 0.99f, 2.0f, 150);
	FriutOnTree07->SetBlendMode(eBlend_AlphaAdditive);
	FriutOnTree07->Animate(0.85f, 0.99f, 4.0f, 180);
	FruitOnTree02->SetBlendMode(eBlend_AlphaAdditive);
	FruitOnTree02->Animate(0.85f, 0.99f, 3.0f, 210);
}
void L4_S2_RiverSide::NavigateToScene()
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