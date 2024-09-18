//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL6 - SCENE2 : FLOATING ISLAND_2
//====================================

#include "L6_S2_FloatingIsland_2.h"
#include "MeshObject.h"
#include "Util.h"

const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);

const CRectangle  SceneRectFloatindIsland_4(553.0f,93.0f,121.0f,131.0f);
const CRectangle  SceneRectFloatindIsland_3(308.0f,391.0f,189.0f,197.0f);
const CRectangle  PineGumRect(787.0f,432.0f,418.0f,200.0f);

const CRectangle  FloatingRect1(241.0f,4.0f,81.0f,139.0f);
const CRectangle  FloatingRect2(662.0f,3.0f,67.0f,143.0f);
const CRectangle  FloatingRect3(726.0f,70.0f,36.0f,56.0f);
const CRectangle  FloatingRect4(946.0f,81.0f,36.0f,77.0f);
const CRectangle  FloatingRect5(997.0f,71.0f,42.0f,56.0f);
const CRectangle  FloatingRect6(1014.0f,136.0f,67.0f,96.0f);
const CRectangle  FloatingRect7(1009.0f,308.0f,76.0f,84.0f);
const CRectangle  FloatingRect8(1082.0f,17.0f,281.0f,217.0f);
const CRectangle  FloatingRect9(1182.0f,254.0f,180.0f,121.0f);
const CRectangle  FloatingRect10(368.0f,8.0f,201.0f,146.0f);

enum eL6S1HOPopEnums
{
	eL6S2Pop_Inactive = -1,
	eL6S2Pop1_Ice,
	eL6S2Pop2_Door
};

L6_S2_FloatingIsland_2::L6_S2_FloatingIsland_2()
{
	LoadHud(INVENTORY_V1);
	LoadScene("LV6\\S2\\S2.SCN");

	int Pop1ArrayPts[] = {386,271,358,217,291,211,273,257,293,304,363,312,384,273};
	ClickablePolygon *popClickPoly = new ClickablePolygon(eL6S2Pop1_Ice, 7, Pop1ArrayPts);
	PopRectArray.push_back(popClickPoly);
	pop1 = new  L6S2_Pop1Ice(eL6S2Pop1_Ice, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L6_S2_FloatingIsland_2::OnPopupClosed, this));
	pop1->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop1);
	popClickPoly = NULL;

	int Pop2ArrayPts[] = {298,414,264,482,351,518,463,485,435,406,300,411};
	popClickPoly = new ClickablePolygon(eL6S2Pop2_Door, 6, Pop2ArrayPts);
	PopRectArray.push_back(popClickPoly);
	L6S2_Pop2Door *pop2 = new  L6S2_Pop2Door(eL6S2Pop2_Door, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L6_S2_FloatingIsland_2::OnPopupClosed, this));
	pop2->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop2);
	popClickPoly = NULL;

	ActivePopUpId = eL6S2Pop_Inactive;

	int zorder = HOScene->GetObjectByID("BgMask")->ZOrder - 1;
	DoorToS3RayFX = new SuperFX("PRT\\L6\\S2_DoorToS3Ray.INI", PointSystem::CreateCPoint(390,536,0), zorder);
	DoorToS3RayFX->Start();
	std::vector<SuperFX*> superfxArray;
	superfxArray.push_back(DoorToS3RayFX);
	//	DoorToS3RayFX->AdvanceByTime(2.0f);

	Janim_doorOpen = new JSONAnimator(0);
	Janim_doorOpen->parseAndLoadJSON("ANIM\\L6\\S2_DOORTOS3OPEN.JSON", HOScene, &superfxArray);



	if (!ProfileSystem::Instance->ActiveProfile.L6_S2_DoorToS3Opened)
	{
		Janim_doorOpen->SetAnimTime(0);
	}
	else
	{
		Janim_doorOpen->SetAnimTime(1);
	}


	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL6s2FloatingIsland2Amb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL6s2FloatingIsland2Amb) )		
	{
		Control::Audio->PlaySample(aAmbL6s2FloatingIsland2Amb,true);
	}

	//MUSIC_SFX



#ifdef _CEBUILD
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S2Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("morphtarget"), HOScene->GetObjectByID("morphbase"));
		CRectangle  MorphRect(668.0f,356.0f,46.0f,77.0f);	
		Hud->setMorphObjRect(MorphRect);
	}
	else
	{
		HOScene->GetObjectByID("morphtarget")->SetEnable(false);
	}
#else
	HOScene->GetObjectByID("morphtarget")->SetEnable(false);
#endif 

	InitParticles();


	StatueRayFX = new SuperFX("PRT\\L6\\S2_StatueRay2.INI", PointSystem::CreateCPoint(-5,277,0), 26); 
	StatueRayFX->AdvanceByTime(2.0);


	if (ProfileSystem::Instance->ActiveProfile.L6_S1_ChestPuzzleSolved)
	{
		StatueRayFX->Start();
	}



	SceneObjectsUpdate();

}

bool L6_S2_FloatingIsland_2::isPopupActive(int popId)
{
	switch (popId)
	{
	case eL6S2Pop1_Ice:	if (!ProfileSystem::Instance->ActiveProfile.L6_S2_InvIceKeyTaken || !ProfileSystem::Instance->ActiveProfile.L6_S2_InvPestleTaken || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S2MotherkidStatue])		return true;	break;
	case eL6S2Pop2_Door: if (!ProfileSystem::Instance->ActiveProfile.L6_S2_DoorToS3Opened)		return true;	break;
	default:
		break;
	}

	return false;
}


void L6_S2_FloatingIsland_2::InitPopups()
{

}

void L6_S2_FloatingIsland_2::OnPopupClosed(Popup * popup)
{
	SceneObjectsUpdate();

	ActivePopUpId = eL6S2Pop_Inactive;

	switch (popup->GetPopupId())
	{
	case 0:
		break;
	case 1:
		break;
	}
}

void L6_S2_FloatingIsland_2::SceneObjectsUpdate()
{
	TaskPendingUpdate();


	if (ProfileSystem::Instance->ActiveProfile.L6_S2_DoorToS3Opened)
	{
		HOScene->GetObjectByID("Clossed-Manhole")->SetEnable(false);
		HOScene->GetObjectByID("Open-Manhole")->SetEnable(true);
	}
	else
	{
		HOScene->GetObjectByID("Open-Manhole")->SetEnable(false);
	}

	if (ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[0] && ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[1])
	{
		HOScene->GetObjectByID("BeforeTask")->SetEnable(false);
	}

	if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[5] )
	{
		if( ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumStickCollected )
		{
			HOScene->GetObjectByID("Melt")->SetEnable(false);
			HOScene->GetObjectByID("PinePitch")->SetEnable(false);
		}
		else if( ProfileSystem::Instance->ActiveProfile.L6_S2_PZPineGumObjectsUsed[6] )
		{
			HOScene->GetObjectByID("PineGum")->SetEnable(false);
			HOScene->GetObjectByID("PinePitch")->SetEnable(false);
		}
		else
		{
			HOScene->GetObjectByID("Melt")->SetEnable(false);
			HOScene->GetObjectByID("PineGum")->SetEnable(false);
		}
	}
	else
	{
		HOScene->GetObjectByID("Pan")->SetEnable(false);
		HOScene->GetObjectByID("PinePitch")->SetEnable(false);
		HOScene->GetObjectByID("PineGum")->SetEnable(false);
		HOScene->GetObjectByID("Melt")->SetEnable(false);
	}



	if (ProfileSystem::Instance->ActiveProfile.L6_S2_InvIceKeyTaken)
	{
		HOScene->GetObjectByID("Ice-with-key")->SetEnable(false);
	}

	_powderBottle = HOScene->GetObjectByID("MagicPowder");
	_sphereGlow = HOScene->GetObjectByID("Green-Glow");
	std::vector<SuperFX*> superfxArray;
	_magicPowderVFX = new SuperFX("PRT\\L6\\S2_MagicPowder.INI", PointSystem::CreateCPoint(706,182,0), _powderBottle->ZOrder + 1); 
	superfxArray.push_back(_magicPowderVFX);
	_magicPowderVFX->StopImmediate();

	_magicPowderVFX->AdvanceByTime(2);



	Janim_SphereReveal = new JSONAnimator(2,  CC_CALLBACK_1(L6_S2_FloatingIsland_2::OnSphereRevealAnimComplete, this));
	Janim_SphereReveal->parseAndLoadJSON("ANIM\\L6\\S2_SPHEREREVEAL.JSON", HOScene, &superfxArray);

	if (ProfileSystem::Instance->ActiveProfile.L6_S2_ColoredPowderTrowed)
	{
		Janim_SphereReveal->SetAnimTime(5.0f);
		_sphereGlow->Show(true);
		HOScene->GetObjectByID("Sphere")->Animate(0,2,0,0,3);
	}
	else
	{
		Janim_SphereReveal->SetAnimTime(0);
		_sphereGlow->Show(false);
	}
	_magicPowderVFX->Show(false);
	_powderBottle->SetEnable(false);

	/*if (!ProfileSystem::Instance->ActiveProfile.L6_S2_ColoredPowderTrowed)
	{
	HOScene->GetObjectByID("Green-Glow")->SetEnable(false);
	HOScene->GetObjectByID("Sphere")->SetEnable(false);
	HOScene->GetObjectByID("Green-Glow")->SetAlpha(0);
	HOScene->GetObjectByID("Sphere")->SetAlpha(0);
	}*/
}

void L6_S2_FloatingIsland_2::OnSphereRevealAnimComplete(int id)
{
	_magicPowderVFX->StopEmit();
	HOScene->GetObjectByID("Sphere")->InitPos.x = HOScene->GetObjectByID("Sphere")->Pos.x;
	HOScene->GetObjectByID("Sphere")->InitPos.y = HOScene->GetObjectByID("Sphere")->Pos.y;
	HOScene->GetObjectByID("Sphere")->Animate(0,2,0,0,3);
	Hud->ShowBannerText("L6wait");
}

void L6_S2_FloatingIsland_2::InitParticles()
{

	HOScene->GetObjectByID("PlantAnim")->Animate(0,3,0,0,5,3);

	HOScene->GetObjectByID("FloatingLand01")->Animate(0,5,0,0,5,3);
	HOScene->GetObjectByID("FloatingLand02")->Animate(0,-5,0,0,7,2);
	HOScene->GetObjectByID("FloatingLand03")->Animate(0,4,0,0,4,4);
	HOScene->GetObjectByID("FloatingLand04")->Animate(0,6,0,0,8,1);
	HOScene->GetObjectByID("FloatingLand05")->Animate(0,-4,0,0,7,5);

	HOScene->GetObjectByID("FlowerGlow01")->Animate(0.8f,1.4f,8,2);
	HOScene->GetObjectByID("FlowerGlow02")->Animate(0.8f,1.4f,7,2);

	HOScene->GetObjectByID("Plant01_FlowerGlow1")->Animate(0.8f,1.4f,8,2);
	HOScene->GetObjectByID("Plant01_FlowerGlow2")->Animate(0.8f,1.4f,8,2);

	HOScene->GetObjectByID("Plant02_Anim")->SetPivot(109, 178);
	HOScene->GetObjectByID("Plant02_Anim")->Animate(0,0,0.5,0,6);
	HOScene->GetObjectByID("Plant01_Flower001")->SetPivot(17, 70);
	HOScene->GetObjectByID("Plant01_Flower001")->Animate(0,0,0.5,0,6);

	HOScene->GetObjectByID("PlantAnim02")->SetPivot(0,62);
	HOScene->GetObjectByID("PlantAnim02")->Animate(0,0,1,0,4,2);

	SnowFX_L = new SuperFX("PRT\\L6\\S2_SnowLeft.INI", PointSystem::CreateCPoint(120,150,0), 19); 
	SnowFX_L->Start();
	SnowFX_L->AdvanceByTime(2.0f);

	SnowFX_R = new SuperFX("PRT\\L6\\S2_SnowRight.INI", PointSystem::CreateCPoint(1200,400,0), 19); 
	SnowFX_R->Start();
	SnowFX_R->AdvanceByTime(2.0f);

	PlantGlowFX_L = new SuperFX("PRT\\L6\\S2_PlantGlowLeft.INI", PointSystem::CreateCPoint(70,355,0), 19); 
	PlantGlowFX_L->Start();
	PlantGlowFX_L->AdvanceByTime(2.0f);

	PlantGlowFX_R = new SuperFX("PRT\\L6\\S2_PlantGlowRight.INI", PointSystem::CreateCPoint(1300,400,0), 19); 
	PlantGlowFX_R->Start();
	PlantGlowFX_R->AdvanceByTime(2.0f);

	S2_LeafFX = new SuperFX("PRT\\L6\\S2_Leaf.INI", PointSystem::CreateCPoint(856,167,0), 1100); 
	S2_LeafFX->Start();
	S2_LeafFX->AdvanceByTime(2.0f);

	HOScene->GetObjectByID("Green-Glow")->Animate(0.7f,1,3);
	//
}

L6_S2_FloatingIsland_2::~L6_S2_FloatingIsland_2()
{
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
	SAFE_DELETE(PlantGlowFX_L);
	SAFE_DELETE(PlantGlowFX_R);
	SAFE_DELETE(SnowFX_L);
	SAFE_DELETE(SnowFX_R);
	SAFE_DELETE(DoorToS3RayFX);
	SAFE_DELETE(Janim_doorOpen);
	SAFE_DELETE(S2_LeafFX);
	SAFE_DELETE(StatueRayFX);
	SAFE_DELETE(Janim_SphereReveal);
	SAFE_DELETE(_magicPowderVFX);

	//MUSIC_SFX

	Control::Audio->StopSample(aAmbL6s2FloatingIsland2Amb);
	Control::Audio->UnloadSample(aAmbL6s2FloatingIsland2Amb);
	//MUSIC_SFX
}

void L6_S2_FloatingIsland_2::Required()
{
	HiddenObjectSystem::Required();	//forced update

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
			if( !Control::Audio->IsPlaying(aTrackL6theme) )
				Control::Audio->PlaySample(aTrackL6theme, true);
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
			if( !Control::Audio->IsPlaying(aTrackL6theme) )
				Control::Audio->PlaySample(aTrackL6theme, true);

		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL6theme))
		{
			Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL6theme, true);
		}
	}
}

void L6_S2_FloatingIsland_2::Update()
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
	if( Hud->PopupState != Popup_Inactive )
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;
	if (ActivePopUpId != eL6S2Pop_Inactive)
		return;
	HiddenObjectSystem::HOUpdate();
#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
#else
	Cursor::SetMode(CA_Normal);
#endif 

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
		} 
	}


	if (ProfileSystem::Instance->ActiveProfile.L6_S1_PathToS3Activated && !ProfileSystem::Instance->ActiveProfile.L6_S2_DoorToS3Opened)
	{
		ProfileSystem::Instance->ActiveProfile.L6_S2_DoorToS3Opened	 = true;
		HOScene->GetObjectByID("Open-Manhole")->SetEnable(true);
		Janim_doorOpen->PlayAnim();
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
		//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S1_FloatingIsland1;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif	
	if( IsMouseOverGoBackArea(&pos) )
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L6_S1_FloatingIsland1);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L6_S1_FloatingIsland1);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L6_S1_FloatingIsland1, 2);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S1_FloatingIsland1;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
			
		}	
	}
	else if (SceneRectFloatindIsland_3.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L6_S2_DoorToS3Opened)
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L6_S3_FloatingIsland3);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L6_S3_FloatingIsland3);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L6_S3_FloatingIsland3, 2, SceneRectFloatindIsland_3);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S3_FloatingIsland3;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
			
		}
	}
	else if (SceneRectFloatindIsland_4.Intersection(&pos))
	{
		
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitLeft,eArea_L6_S4_FloatingIsland4);
#else
		Cursor::SetMode(CA_ExitLeft,eArea_L6_S4_FloatingIsland4);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L6_S4_FloatingIsland4, 3, SceneRectFloatindIsland_4);
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S4_FloatingIsland4;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
			
		}
	}
	else if (ProfileSystem::Instance->ActiveProfile.L6_S2_ColoredPowderTrowed && HOScene->GetObjectByID("Sphere")->GetWorldRect().Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S2_HOComplete)
	{

#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_Look);
#else
		Cursor::SetMode(CA_Look);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L6_S2_ShereHO, 0, HOScene->GetObjectByID("Sphere")->GetWorldRect());
			}
#else			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S2_ShereHO;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
			
		}
	}
	else if( PineGumRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumStickCollected )
	{

#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_Look);
#else
		Cursor::SetMode(CA_Look);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				Hud->ShowBannerText("L6here");
				GFHud->InvBoxCtrl->GoToScene(eArea_L6_S2_PZPineGum, 0, PineGumRect);
			}
#else			
			Hud->ShowBannerText("L6here");
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S2_PZPineGum;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
			
		}
	}
	else if (FloatingRect1.Intersection(&pos) || FloatingRect2.Intersection(&pos) || FloatingRect3.Intersection(&pos) || FloatingRect4.Intersection(&pos) || FloatingRect5.Intersection(&pos) || FloatingRect6.Intersection(&pos) || FloatingRect7.Intersection(&pos) || FloatingRect8.Intersection(&pos) || FloatingRect9.Intersection(&pos) || FloatingRect10.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L6live");
		}
	}
}

void L6_S2_FloatingIsland_2::Render(const CPoint* pCam)
{

}


void L6_S2_FloatingIsland_2::TakeInventoryItem(EHOInventory ItemID)
{

}


void L6_S2_FloatingIsland_2::ReceiveMessage(int val)
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
			if (itemVal == eInv_L6_S4_ColouredPowder)
			{

				_sphereGlow->Show(true);
				_magicPowderVFX->Show(true);
				_powderBottle->SetEnable(true);
				Janim_SphereReveal->PlayAnim();

				ProfileSystem::Instance->ActiveProfile.L6_S2_ColoredPowderTrowed = true;

				//HOScene->GetObjectByID("Green-Glow")->SetEnable(true);
				//HOScene->GetObjectByID("Sphere")->SetEnable(true);
				//HOScene->GetObjectByID("Green-Glow")->FadeIn(6);
				//HOScene->GetObjectByID("Sphere")->FadeIn(3);

				Hud->ReleaseDrag();
				Hud->CloseInventory(); 

				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L6_S4_ColouredPowder] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L6_S4_ColouredPowder);
			}
			else
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
	}
	else if( val == INV_CLOSE )
	{		
		if (ActivePopUpId == eL6S2Pop1_Ice)
		{
			pop1->ReceiveMessage(val);
		}
	}	
	TaskPendingUpdate();
}

float L6_S2_FloatingIsland_2::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}


void L6_S2_FloatingIsland_2::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

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

	if( !ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanTaken )
		isAnyTaskLeft = true;
	if( ProfileSystem::Instance->ActiveProfile.L6_S4_InvColoredPowderTaken && !ProfileSystem::Instance->ActiveProfile.L6_S2_ColoredPowderTrowed )
		isAnyTaskLeft = true;
	if (ProfileSystem::Instance->ActiveProfile.L6_S1_PathToS3Activated && !ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L6_S3_FloatingIsland3])
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S2_FloatingIsland2] = isAnyTaskLeft;

	if (ProfileSystem::Instance->ActiveProfile.L6_S2_InvIceKeyTaken && !ProfileSystem::Instance->ActiveProfile.L6_S4_KeyInIcePlaced)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S4_FloatingIsland4] = true;
	if (ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanTaken && !ProfileSystem::Instance->ActiveProfile.L6_S4_PanPlaced)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S4_FloatingIsland4] = true;
	if (!ProfileSystem::Instance->ActiveProfile.L6_S2_InvPanTaken )
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S2_PZPineGum] = true;
	if (!ProfileSystem::Instance->ActiveProfile.L6_S2_InvPestlePlaced && ProfileSystem::Instance->ActiveProfile.L6_S2_InvPestleTaken)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S2_PZPineGum] = true;
	if(ProfileSystem::Instance->ActiveProfile.L6_S2_InvPuzzlePiecesCollected && !ProfileSystem::Instance->ActiveProfile.L6_S4_PuzzlePiecesPlaced)
	{
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S4_PortalChest] = true;
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S4_FloatingIsland4] = true;
	}
	if(ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumStickCollected && !ProfileSystem::Instance->ActiveProfile.L6_S4_GlueApplied)
	{
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S4_PortalChest] = true;
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S4_FloatingIsland4] = true;
	}
	if (!ProfileSystem::Instance->ActiveProfile.L6_S4_InvPineGumStickCollected)
	{
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S2_FloatingIsland2] = true;
	}

}

void L6_S2_FloatingIsland_2::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S2Collected)
		{
			InitHintTrailEmit(680, 400);
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S2MotherkidStatue])
		{
			if (ActivePopUpId != ePopInactive)
				InitHintTrailEmit(920, 350);
			else
				InitHintTrailEmit(320, 290);
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
		case 0: ++iNonHOHint;
			if( ActivePopUpId != ePopInactive )
			{
				PopupArray[ActivePopUpId]->ProcessHint();
				return;
			}
			break;
		case 1: ++iNonHOHint;
			for (int i = 0; i < (int)(PopupArray.size()); i++)
			{
				if(isPopupActive(i)) //if popup is active
				{
					if(PopupArray[i]->isTaskLeft())
					{
						ClickablePolygon *rect = PopRectArray[i];
						InitHintTrailEmit(rect->center.x,rect->center.y,false);
						return;
					}
				}
			}
			break;
		case 2: ++iNonHOHint;
			if( ProfileSystem::Instance->ActiveProfile.L6_S4_InvColoredPowderTaken && !ProfileSystem::Instance->ActiveProfile.L6_S2_ColoredPowderTrowed )
			{
				//Hud->ShowHintForInvItem(eInv_L6_S4_ColouredPowder,&HOScene->GetObjectByID("Sphere")->GetWorldRect());
				// Linux: gives error of taking address of temporary also potential crash factor
				// below is fix
				CRectangle sphereObjRect =HOScene->GetObjectByID("Sphere")->GetWorldRect();
				Hud->ShowHintForInvItem(eInv_L6_S4_ColouredPowder,&sphereObjRect);
				return;
			}
			break;
		case 3: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.L6_S2_ColoredPowderTrowed && !ProfileSystem::Instance->ActiveProfile.L6_S2_InvPuzzlePiecesCollected)
			{
				//InitHintTrailEmit(&HOScene->GetObjectByID("Sphere")->GetWorldRect(),true);
				// Linux: gives error of taking address of temporary also potential crash factor
				// below is fix
				CRectangle sphereObjRect =HOScene->GetObjectByID("Sphere")->GetWorldRect();
				InitHintTrailEmit(&sphereObjRect,true);
				return;
			}
			break;

		case 4: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S2_PZPineGum] )
			{
				InitHintTrailEmit(&PineGumRect,true);
				return;
			}
			break;
		case 5: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S1_FloatingIsland1] )
			{
				InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
				return;
			}
			break;
		case 6: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S3_FloatingIsland3] || (ProfileSystem::Instance->ActiveProfile.L6_S1_PathToS3Activated && !ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L6_S3_FloatingIsland3]))
			{
				InitHintTrailEmit(&SceneRectFloatindIsland_3,true,CA_ExitDown);
				return;
			}
			break;
		case 7: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S4_FloatingIsland4] )
			{
				InitHintTrailEmit(&SceneRectFloatindIsland_4,true,CA_ExitLeft);
				return;
			}
			break;
		case 8: ++iNonHOHint;
			if(Hud->ShowHintForComboInvItems())
				return;
			break;

		case 9: ++iNonHOHint;
			Hud->teleportSystem->ShowTeleport();
			return;
			break;
		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();
}

void L6_S2_FloatingIsland_2::SparkleUpdate()
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

void L6_S2_FloatingIsland_2::CheckForPendingTask()
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

	if( ProfileSystem::Instance->ActiveProfile.L6_S4_InvColoredPowderTaken && !ProfileSystem::Instance->ActiveProfile.L6_S2_ColoredPowderTrowed )
	{
		sparklePointsVec.push_back(CPoint(HOScene->GetObjectByID("Sphere")->GetWorldRect().GetCenterX(), HOScene->GetObjectByID("Sphere")->GetWorldRect().GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}

	else if(ProfileSystem::Instance->ActiveProfile.L6_S2_ColoredPowderTrowed && !ProfileSystem::Instance->ActiveProfile.L6_S2_InvPuzzlePiecesCollected)
	{
		sparklePointsVec.push_back(CPoint(HOScene->GetObjectByID("Sphere")->GetWorldRect().GetCenterX(), HOScene->GetObjectByID("Sphere")->GetWorldRect().GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}

	if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S2_PZPineGum] )
	{
		sparklePointsVec.push_back(CPoint(PineGumRect.GetCenterX(), PineGumRect.GetCenterY(), 0));
		isSparkleUpdatePending = true;
	}

}

void L6_S2_FloatingIsland_2::NavigateToScene()
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