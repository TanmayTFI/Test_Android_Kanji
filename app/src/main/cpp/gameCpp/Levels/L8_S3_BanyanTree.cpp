//====================================
// DQFULP
// Hidden Object System
// Author : NEETHU
// LEVEL8 - SCENE3 : BANYAN TREE
//====================================

#include "L8_S3_BanyanTree.h"
#include "MeshObject.h"
#include "Util.h"

const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);
const CRectangle  SceneRectGarden(1014.0f,292.0f,100.0f,145.0f);
const CRectangle  HORectLeaf(496.0f,142.0f,200.0f,128.0f);
const CRectangle  TreeRect1(396.0f,19.0f,406.0f,176.0f);
const CRectangle  TreeRect2(425.0f,201.0f,283.0f,53.0f);

const CRectangle  FloatingRect1(1039.0f,77.0f,91.0f,137.0f);
const CRectangle  FloatingRect2(135.0f,162.0f,57.0f,112.0f);
const CRectangle  FloatingRect3(293.0f,5.0f,136.0f,73.0f);

enum eL6S1HOPopEnums
{
	eL8S3Pop_Inactive = -1,
	eL8S3Pop1_SwordPopup,
	eL8S3Pop2_HiddenCompartment
};

#define X_OFFSET_MASK	170
#define Y_OFFSET_MASK	120

L8_S3_BanyanTree::L8_S3_BanyanTree()
{
	LoadHud(INVENTORY_V1);
	LoadScene("LV8\\S3\\S3.SCN");

	int Pop1ArrayPts[] = {438,385,406,423,404,478,464,478,478,411,435,382};
	ClickablePolygon *popClickPoly = new ClickablePolygon(eL8S3Pop1_SwordPopup, 6, Pop1ArrayPts);
	PopRectArray.push_back(popClickPoly);
	L8S3_Pop1SwordPopup *pop1 = new  L8S3_Pop1SwordPopup(eL8S3Pop1_SwordPopup, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L8_S3_BanyanTree::OnPopupClosed, this));
	pop1->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop1);
	popClickPoly = NULL;

	int Pop2ArrayPts[] = {818,481,793,532,846,547,892,525,869,482,814,480};
	popClickPoly = new ClickablePolygon(eL8S3Pop2_HiddenCompartment, 6, Pop2ArrayPts);
	PopRectArray.push_back(popClickPoly);
	L8S3_Pop2HiddenCompartment *pop2 = new  L8S3_Pop2HiddenCompartment(eL8S3Pop2_HiddenCompartment, Hud, HOScene, eZOrder_Popup, popClickPoly->center, CC_CALLBACK_1(L8_S3_BanyanTree::OnPopupClosed, this));
	pop2->SetPopupBoundingBox(PopAreaRect);
	PopupArray.push_back(pop2);
	popClickPoly = NULL;
	ActivePopUpId = eL8S3Pop_Inactive;

	sceneMagicGlass = new CHOScene();
	sceneMagicGlass->Load("LV8\\S_MG\\S_MG.SCN");
	sceneMagicGlass->SetZOff(eZOrder_FoundInvPopup);
	sceneMagicGlass->Show(false);
	isGlassInHand = false;

	HOScene->GetObjectByID("waterfallUp")->SetScale(2);

	InitParticles();

	for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
	{
		symbols[i] = HOScene->GetObjectByID(MKSTR(i));
		redSymbols[i] = HOScene->GetObjectByID(MKSTR(i) + "_2");
		if (i % 2 == 0)
		{
			silhouttes[i /2] = HOScene->GetObjectByID("sil_" + MKSTR(i / 2));
			silhouttes[i /2]->SetZ(eZOrder_HUD + 1);
		}
	}

	silhouttes[(MAX_L8_S1_MATCHING_SYMBOLS_COUNT / 2)] = HOScene->GetObjectByID("sil_" + MKSTR(MAX_L8_S1_MATCHING_SYMBOLS_COUNT / 2));
	silhouttes[(MAX_L8_S1_MATCHING_SYMBOLS_COUNT / 2)]->SetZ(eZOrder_HUD + 1);

	selectedSymbol = -1;

	SceneObjectsUpdate();
	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL8s3BanyanTreeAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL8s3BanyanTreeAmb) )
	{
		Control::Audio->PlaySample(aAmbL8s3BanyanTreeAmb,true);
	}

	//MUSIC_SFX
	HOScene->GetObjectByID("skipbtn")->SetEnable(false);

	SkipBtn = new CObject();
	SkipBtn->LoadInit("UI\\HUD\\BTNSKIP.SPR", 100, 50, eZOrder_HUD);
	SkipBtn->SetUID("SkipBtn");
	HOScene->PushObject(SkipBtn);
	SkipBtn->PlayAnimation(2);
	SkipBtn->SetEnable(false);

	skipBTN_text = new CObject();
	skipBTN_text->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\SKIP.SPR", 100, 50, eZOrder_HUD+1);
	skipBTN_text->SetUID("skipBTN_text");
	HOScene->PushObject(skipBTN_text);
	skipBTN_text->SetEnable(false);
}

bool L8_S3_BanyanTree::isPopupActive(int popId)
{
	switch (popId)
	{
		case eL8S3Pop1_SwordPopup: if (!isGlassInHand && !ProfileSystem::Instance->ActiveProfile.L8_S3_InvCrystalKeyTaken)		return true;	break;
		case eL8S3Pop2_HiddenCompartment:	if (!ProfileSystem::Instance->ActiveProfile.L8_S3_InvIdolTaken && ((isGlassInHand && ProfileSystem::Instance->ActiveProfile.L8_S3_MatchingHOComplete) || ProfileSystem::Instance->ActiveProfile.L8_MagicSymbolFound[2]))		return true;	break;
		default:
			break;
	}

	return false;
}


void L8_S3_BanyanTree::InitPopups()
{

}

void L8_S3_BanyanTree::OnPopupClosed(Popup * popup)
{
	SceneObjectsUpdate();

	ActivePopUpId = eL8S3Pop_Inactive;

	switch (popup->GetPopupId())
	{
		case eL8S3Pop2_HiddenCompartment:
			isGlassInHand = false;
			break;
		default:
			break;
	}
}

void L8_S3_BanyanTree::SceneObjectsUpdate()
{
	TaskPendingUpdate();

	if (!ProfileSystem::Instance->ActiveProfile.L8_S3_FloorCracked)
	{
		HOScene->GetObjectByID("Popup_Crack")->SetEnable(false);
	}
	else
	{
		HOScene->GetObjectByID("Popup_Crack")->SetEnable(true);
	}
	HOScene->GetObjectByID("popup_Mark")->SetEnable(false);

	/*for (int i = 0; i < 3; i++)
	{
	string id ="BtnGlow0" + MKSTR(i + 1);
	if(!ProfileSystem::Instance->ActiveProfile.L8_MagicSymbolFound[i])
	{
	sceneMagicGlass->GetObjectByID(id)->SetEnable(false);
	}
	}*/

	for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
	{
		symbols[i]->SetEnable(false);
		redSymbols[i]->SetAlpha(0);
#ifdef TOUCH_UI
		symbols[i]->SetScale(1.5f);
		redSymbols[i]->SetScale(1.5f);
		//symbols[i]->SetTint(1,1,0);
#endif
		redSymbols[i]->SetPos(symbols[i]->GetPosPointer());
		silhouttes[i / 2]->SetEnable(false);
	}

	silhouttes[MAX_L8_S1_MATCHING_SYMBOLS_COUNT/2]->SetEnable(false);
}

void L8_S3_BanyanTree::InitParticles()
{
	HOScene->GetObjectByID("LightRays")->Animate(0.6f,1,4,2);

	HOScene->GetObjectByID("LeftCloud")->Animate(2,1,0,0,5,3);
	HOScene->GetObjectByID("RightCloud")->Animate(2,1,0,0,5,3);

	HOScene->GetObjectByID("Roots01")->SetPivot(0,-239);
	HOScene->GetObjectByID("Roots01")->Animate(0,0,2,0,3,2);
	HOScene->GetObjectByID("Roots02")->SetPivot(0,-241);
	HOScene->GetObjectByID("Roots02")->Animate(0,0,-3,0,4,2);

	HOScene->GetObjectByID("FloatingLand01")->Animate(0,-5,0,0,4,2);
	HOScene->GetObjectByID("FloatingLand02")->Animate(0,6,0,0,6,2);
	HOScene->GetObjectByID("FloatingLand03")->Animate(0,-5,0,0,4,2);
	HOScene->GetObjectByID("FloatingLand04")->Animate(0,6,0,0,6,2);

	HOScene->GetObjectByID("FlowerAnim01")->SetPivot(0,135);
	HOScene->GetObjectByID("FlowerAnim01")->Animate(0,0,1.5,0,5,2);
	HOScene->GetObjectByID("FlowerAnim02")->SetPivot(0,149);
	HOScene->GetObjectByID("FlowerAnim02")->Animate(0,0,1.5,0,5,2);

	sfx_WaterFall1 = new SuperFX("PRT\\L8\\S3_WaterFall1.INI", PointSystem::CreateCPoint(705,410,0), 19);
	sfx_WaterFall1->Start();
	sfx_WaterFall1->AdvanceByTime(2.0f);

	//sfx_WaterFall2 = new SuperFX("PRT\\L8\\S3_WaterFall2.INI", PointSystem::CreateCPoint(940,420,0), 19);
	//sfx_WaterFall2->Start();
	//sfx_WaterFall2->AdvanceByTime(2.0f);

	holinesFX = new SuperFX("PRT\\L2\\holines.INI", PointSystem::CreateCPoint(590,260,0), 100);

	hodotsFX = new SuperFX("PRT\\L2\\hodots.INI", PointSystem::CreateCPoint(584,267,0), 100);

	if( !ProfileSystem::Instance->ActiveProfile.l8_s3_invMaryLeafTaken)
	{
		if( !ProfileSystem::Instance->ActiveProfile.IsHideHOSparkles )
		{
			holinesFX->Start();
			holinesFX->AdvanceByTime(2.0f);

			hodotsFX->Start();
			hodotsFX->AdvanceByTime(2.0f);
		}
	}
}

L8_S3_BanyanTree::~L8_S3_BanyanTree()
{
	Hud->InvBoxCtrl->ResetToInvBox();
	Hud->IsHOScene = false;

	for (int i = 0; i < (int)(PopupArray.size()); i++)
	{
		SAFE_DELETE(PopupArray[i]);
	}
	PopupArray.clear();

	for (int i = 0; i < (int)(PopRectArray.size()); i++)
	{
		SAFE_DELETE(PopRectArray[i]);
	}
	PopRectArray.clear();

	SAFE_DELETE(sfx_WaterFall1);
	//SAFE_DELETE(sfx_WaterFall2);
	SAFE_DELETE(sceneMagicGlass);
	SAFE_DELETE(holinesFX);
	SAFE_DELETE(hodotsFX);
	//MUSIC_SFX

	Control::Audio->StopSample(aAmbL8s3BanyanTreeAmb);
	Control::Audio->UnloadSample(aAmbL8s3BanyanTreeAmb);
	//MUSIC_SFX
}

void L8_S3_BanyanTree::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Control::Audio->IsPlaying(aTrackL8ho) )
	{
		Control::Audio->Samples[aTrackL8ho]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL8ho]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL8ho);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL8theme) )
				Control::Audio->PlaySample(aTrackL8theme, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else if( Control::Audio->IsPlaying(aTrackL8puzzle) )
	{
		Control::Audio->Samples[aTrackL8puzzle]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL8puzzle]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL8puzzle);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL8theme) )
				Control::Audio->PlaySample(aTrackL8theme, true);

		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL8theme))
		{
			Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL8theme, true);
		}
	}
}

void L8_S3_BanyanTree::Update()
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

	if( Hud->PopupState != Popup_Inactive )
		return;
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;
	if (ActivePopUpId != eL8S3Pop_Inactive)
		return;

#ifdef _CEBUILD
	if(!Hud->isMouseOverExtraButton)
		Cursor::SetMode(CA_Normal);
#else
	Cursor::SetMode(CA_Normal);
#endif

	HiddenObjectSystem::HOUpdate();

	CPoint pos = Control::Input->Mpos();
	NavigateToScene();

	for (int i = 0; i < (int)(PopupArray.size()); i++)
	{
		if( PopRectArray[i]->Intersection(&pos) && isPopupActive(i))
		{
			Cursor::SetMode(CA_Look);
			if (Control::Input->LBclicked())
			{
				ActivePopUpId = i;
				PopupArray[i]->OpenPopup();
				if (isGlassInHand)
				{
					sceneMagicGlass->Show(false);
					HOScene->GetObjectByID("popup_Mark")->SetEnable(false);
					for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
					{
						symbols[i]->SetEnable(false);
					}
					silhouttes[MAX_L8_S1_MATCHING_SYMBOLS_COUNT/2]->SetEnable(false);
					Hud->InvBoxCtrl->ResetToInvBox();
					Hud->IsHOScene = false;
				}
			}
		}
	}
#ifdef TOUCH_UI
	if(isGlassInHand)
		GFApp->pinchDisabled = true;
	else
		GFApp->pinchDisabled = false;
#endif
	SkipMagicGlass();
	if (isGlassInHand)
	{
#ifdef TOUCH_UI
		if(Control::Input->LBdown())
		{
			if(!LBPressed)
			{
				LBPressed = true;
				prevPos.x = KInput::getMouseX();
				prevPos.y = KInput::getMouseY();
			}
			else
			{
				mouseDx = KInput::getMouseX() - prevPos.x;
				mouseDy = KInput::getMouseY() - prevPos.y;

				if(mouseDx < -50)
					mouseDx = -50;
				if(mouseDy < -50)
					mouseDy = -50;
				if(mouseDx > 50)
					mouseDx = 50;
				if(mouseDy > 50)
					mouseDy = 50;

				currentPos.x += (float)(mouseDx);
				currentPos.y += (float)(mouseDy);


				if(currentPos.x < -200)
					currentPos.x = -200;
				if(currentPos.y < -200)
					currentPos.y = -200;
				if(currentPos.x > 1500)
					currentPos.x = 1500;
				if(currentPos.y > 800)
					currentPos.y = 800;


				prevPos.x = KInput::getMouseX();
				prevPos.y = KInput::getMouseY();
			}
		}
		else
		{
			LBPressed = false;
		}

		sceneMagicGlass->GetObjectByID("MagnifyingGlass")->SetPos(&currentPos);
#else
		sceneMagicGlass->GetObjectByID("MagnifyingGlass")->SetPos(&pos);
#endif
		for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
		{
			if (!ProfileSystem::Instance->ActiveProfile.L8_S3_MatchingSymbolFound[i] && symbols[i]->GetWorldRect().Intersection(&pos))
			{
				Cursor::SetMode(CA_HandPointy);
				if(Control::Input->LBclicked())
				{
					if (selectedSymbol >= 0)
					{
						if (i % 2)
						{
							if (selectedSymbol == i - 1)
							{
								symbols[i]->PlayAnimation(1);
								symbols[selectedSymbol]->PlayAnimation(1);
								symbols[i]->FadeOut();
								symbols[selectedSymbol]->FadeOut();
								ProfileSystem::Instance->ActiveProfile.L8_S3_MatchingSymbolFound[i] = true;
								ProfileSystem::Instance->ActiveProfile.L8_S3_MatchingSymbolFound[selectedSymbol] = true;
								silhouttes[selectedSymbol / 2]->PlayAnimation(1);

								bool isComplete = true;
								for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
								{
									if (!ProfileSystem::Instance->ActiveProfile.L8_S3_MatchingSymbolFound[i])
									{
										isComplete = false;
									}
								}
								if (isComplete)
								{
									HOScene->GetObjectByID("popup_Mark")->SetEnable(true);
									silhouttes[MAX_L8_S1_MATCHING_SYMBOLS_COUNT/2]->SetEnable(true);
									for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
									{
										silhouttes[i / 2]->SetEnable(false);
									}
									ProfileSystem::Instance->ActiveProfile.L8_S3_MatchingHOComplete = true;
								}
							}
							else
							{
								symbols[selectedSymbol]->PlayAnimation(0);
								symbols[i]->PlayAnimation(0);
								redSymbols[selectedSymbol]->FadeInOut();
								redSymbols[i]->FadeInOut();
							}
						}
						else
						{
							if (selectedSymbol == i + 1)
							{
								symbols[i]->PlayAnimation(1);
								symbols[selectedSymbol]->PlayAnimation(1);
								symbols[i]->FadeOut();
								symbols[selectedSymbol]->FadeOut();
								ProfileSystem::Instance->ActiveProfile.L8_S3_MatchingSymbolFound[i] = true;
								ProfileSystem::Instance->ActiveProfile.L8_S3_MatchingSymbolFound[selectedSymbol] = true;
								silhouttes[i / 2]->PlayAnimation(1);

								bool isComplete = true;
								for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
								{
									if (!ProfileSystem::Instance->ActiveProfile.L8_S3_MatchingSymbolFound[i])
									{
										isComplete = false;
									}
								}
								if (isComplete)
								{
									HOScene->GetObjectByID("popup_Mark")->SetEnable(true);
									silhouttes[MAX_L8_S1_MATCHING_SYMBOLS_COUNT/2]->SetEnable(true);
									for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
									{
										silhouttes[i / 2]->SetEnable(false);
									}
									ProfileSystem::Instance->ActiveProfile.L8_S3_MatchingHOComplete = true;
								}
							}
							else
							{
								symbols[selectedSymbol]->PlayAnimation(0);
								symbols[i]->PlayAnimation(0);
								redSymbols[selectedSymbol]->FadeInOut();
								redSymbols[i]->FadeInOut();
							}
						}
						selectedSymbol = -1;
					}
					else
					{
						selectedSymbol  = i;
						symbols[i]->PlayAnimation(1);
					}
				}
				//return;

			}
		}


		float startX = HOScene->GetObjectByID("popup_Mark")->GetPosPointer()->x - HOScene->GetObjectByID("popup_Mark")->GetLocalRect().w/2;
		float startY = HOScene->GetObjectByID("popup_Mark")->GetPosPointer()->y - HOScene->GetObjectByID("popup_Mark")->GetLocalRect().h/2;
#ifndef TOUCH_UI
		CRectangle  *TmpRect = new CRectangle((pos.x - X_OFFSET_MASK + 40 > startX)?pos.x - X_OFFSET_MASK + 40:startX,(pos.y - Y_OFFSET_MASK > startY)?pos.y - Y_OFFSET_MASK :startY,pos.x + X_OFFSET_MASK - HOScene->GetObjectByID("popup_Mark")->GetPosPointer()->x,pos.y + Y_OFFSET_MASK - HOScene->GetObjectByID("popup_Mark")->GetPosPointer()->y);
#else
		CRectangle  *TmpRect = new CRectangle((currentPos.x - X_OFFSET_MASK + 40 > startX)?currentPos.x - X_OFFSET_MASK + 40:startX,(currentPos.y - Y_OFFSET_MASK > startY)?currentPos.y - Y_OFFSET_MASK :startY,currentPos.x + X_OFFSET_MASK - HOScene->GetObjectByID("popup_Mark")->GetPosPointer()->x,currentPos.y + Y_OFFSET_MASK - HOScene->GetObjectByID("popup_Mark")->GetPosPointer()->y);
#endif
		HOScene->GetObjectByID("popup_Mark")->SetMaskRect(TmpRect);
		delete(TmpRect);
		for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
		{
			float startX = HOScene->GetObjectByID(MKSTR(i))->GetPosPointer()->x - HOScene->GetObjectByID(MKSTR(i))->GetLocalRect().w/2;
			float startY = HOScene->GetObjectByID(MKSTR(i))->GetPosPointer()->y - HOScene->GetObjectByID(MKSTR(i))->GetLocalRect().h/2;
#ifndef TOUCH_UI
			CRectangle  *TmpRect = new CRectangle((pos.x - X_OFFSET_MASK + 40 > startX)?pos.x - X_OFFSET_MASK + 40:startX,(pos.y - Y_OFFSET_MASK > startY)?pos.y - Y_OFFSET_MASK :startY,pos.x + X_OFFSET_MASK - 30 - HOScene->GetObjectByID(MKSTR(i))->GetPosPointer()->x,pos.y + Y_OFFSET_MASK - HOScene->GetObjectByID(MKSTR(i))->GetPosPointer()->y);
#else
			CRectangle  *TmpRect = new CRectangle((currentPos.x - X_OFFSET_MASK + 40 > startX)?currentPos.x - X_OFFSET_MASK + 40:startX,(currentPos.y - Y_OFFSET_MASK > startY)?currentPos.y - Y_OFFSET_MASK :startY,currentPos.x + X_OFFSET_MASK - 30 - HOScene->GetObjectByID(MKSTR(i))->GetPosPointer()->x,currentPos.y + Y_OFFSET_MASK - HOScene->GetObjectByID(MKSTR(i))->GetPosPointer()->y);
#endif
			symbols[i]->SetMaskRect(TmpRect);
			redSymbols[i]->SetMaskRect(TmpRect);
			delete(TmpRect);
		}
#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed() && !isGlassInHand)
		{
			isGlassInHand = false;
			sceneMagicGlass->Show(false);
			HOScene->GetObjectByID("popup_Mark")->SetEnable(false);
			for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
			{
				symbols[i]->SetEnable(false);
				silhouttes[i / 2]->SetEnable(false);
			}
			silhouttes[MAX_L8_S1_MATCHING_SYMBOLS_COUNT/2]->SetEnable(false);
			Hud->InvBoxCtrl->ResetToInvBox();
			Hud->IsHOScene = false;
		}
#endif
		if (IsMouseOverGoBackArea(&pos))
		{
			Cursor::SetMode(CA_ExitDown);
			if (Control::Input->LBclicked())
			{
				isGlassInHand = false;
				sceneMagicGlass->Show(false);
				HOScene->GetObjectByID("popup_Mark")->SetEnable(false);
				for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
				{
					symbols[i]->SetEnable(false);
					silhouttes[i / 2]->SetEnable(false);
				}
				silhouttes[MAX_L8_S1_MATCHING_SYMBOLS_COUNT/2]->SetEnable(false);
				Hud->InvBoxCtrl->ResetToInvBox();
				Hud->IsHOScene = false;
			}
		}

		return;
	}
#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed() && !isGlassInHand)
	{
		Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S2_Garden;
		SendMessage(ControlExit, ID, HOSM_NewArea);
	}
#endif
	if (IsMouseOverGoBackArea(&pos))
	{
		if (!isGlassInHand)
		{

#ifdef TOUCH_UI
			//	if(!GFHud->InvBoxCtrl->navTapped)
			//		Cursor::SetMode(CA_ExitDown,eArea_L8_S2_Garden);
#else
			Cursor::SetMode(CA_ExitDown,eArea_L8_S2_Garden);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L8_S2_Garden, 2);
				}
#else
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S2_Garden;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
		}
		/*else
		{
		Cursor::SetMode(CA_ExitDown);
		if( Control::Input->LBclicked() )
		{
		sceneMagicGlass->Show(false);
		isGlassInHand = false;
		HOScene->GetObjectByID("popup_Mark")->SetEnable(false);
		}
		}*/
	}
	else if (HORectLeaf.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.l8_s3_invMaryLeafTaken)
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
				GFHud->InvBoxCtrl->GoToScene(eArea_L8_S3_BanyanTreeHO, 0, HORectLeaf);
			}
#else

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S3_BanyanTreeHO;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif
		}
	}
	else if((TreeRect1.Intersection(&pos) || TreeRect2.Intersection(&pos)) && ProfileSystem::Instance->ActiveProfile.l8_s3_invMaryLeafTaken)
	{
		Cursor::SetMode(CA_Help);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L8tree");
		}
	}
	else if (FloatingRect1.Intersection(&pos) || FloatingRect2.Intersection(&pos) || FloatingRect3.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L8float");
		}
	}

	//if (isGlassInHand)
	//{
	//	sceneMagicGlass->GetObjectByID("MagnifyingGlass")->SetPos(&pos);
	///*	sceneMagicGlass->GetObjectByID("BtnGlow01")->SetPos(new CPoint(pos.x - 78,pos.y + 114,pos.z));
	//	sceneMagicGlass->GetObjectByID("BtnGlow02")->SetPos(new CPoint(pos.x,pos.y + 129,pos.z));
	//	sceneMagicGlass->GetObjectByID("BtnGlow03")->SetPos(new CPoint(pos.x + 78,pos.y + 113,pos.z));*/
	//}
	//float startX = HOScene->GetObjectByID("popup_Mark")->GetPosPointer()->x - HOScene->GetObjectByID("popup_Mark")->GetLocalRect().w/2;
	//float startY = HOScene->GetObjectByID("popup_Mark")->GetPosPointer()->y - HOScene->GetObjectByID("popup_Mark")->GetLocalRect().h/2;
	//CRectangle  TmpRect = new CRectangle((pos.x - X_OFFSET_MASK + 40 > startX)?pos.x - X_OFFSET_MASK + 40:startX,(pos.y - Y_OFFSET_MASK > startY)?pos.y - Y_OFFSET_MASK :startY,pos.x + X_OFFSET_MASK - HOScene->GetObjectByID("popup_Mark")->GetPosPointer()->x,pos.y + Y_OFFSET_MASK - HOScene->GetObjectByID("popup_Mark")->GetPosPointer()->y);
	//HOScene->GetObjectByID("popup_Mark")->SetMaskRect(TmpRect);
}

void L8_S3_BanyanTree::Render(const CPoint* pCam)
{

}


void L8_S3_BanyanTree::TakeInventoryItem(EHOInventory ItemID)
{

}


void L8_S3_BanyanTree::ReceiveMessage(int val)
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
			if (itemVal == eInv_L8_S1_MagicGlass && !ProfileSystem::Instance->ActiveProfile.L8_MagicSymbolFound[2])
			{

				sceneMagicGlass->Show(true);
				isGlassInHand = true;
				currentPos = Control::Input->Mpos();
				//HOScene->GetObjectByID("popup_Mark")->SetEnable(true);
				Hud->ReleaseDrag();
				Hud->CloseInventory();
				bool isComplete = true;
				for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
				{
					if (!ProfileSystem::Instance->ActiveProfile.L8_S3_MatchingSymbolFound[i])
					{
						isComplete = false;
						symbols[i]->SetEnable(true);
					}
					else
					{
						silhouttes[i / 2]->PlayAnimation(1);
					}
					if (!ProfileSystem::Instance->ActiveProfile.L8_S3_MatchingHOComplete)
						silhouttes[i / 2]->SetEnable(true);
				}

				if (ProfileSystem::Instance->ActiveProfile.L8_S3_MatchingHOComplete && !ProfileSystem::Instance->ActiveProfile.L8_S3_InvIdolTaken)
				{
					HOScene->GetObjectByID("popup_Mark")->SetEnable(true);
					silhouttes[MAX_L8_S1_MATCHING_SYMBOLS_COUNT/2]->SetEnable(true);
				}

				Hud->InvBoxCtrl->SetLockBase(HO_FINDPAIRS_FRAME);
				Hud->IsHOScene = true;
				Hud->HOState = HOActive;
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


	}
}

float L8_S3_BanyanTree::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}


void L8_S3_BanyanTree::TaskPendingUpdate()
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
	if (!ProfileSystem::Instance->ActiveProfile.l8_s3_invMaryLeafTaken)
		isAnyTaskLeft = true;
	if (ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicGlassTaken && !ProfileSystem::Instance->ActiveProfile.L8_S3_FloorCracked)
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S3_BanyanTree] = isAnyTaskLeft;

	if( ((ProfileSystem::Instance->ActiveProfile.L8_S3_InvIdolTaken && !ProfileSystem::Instance->ActiveProfile.L8_S4_BronzeIdolPlaced ) ||
		 (ProfileSystem::Instance->ActiveProfile.L8_S2_InvIdolTaken && !ProfileSystem::Instance->ActiveProfile.L8_S4_SilverIdolPlaced ) ||
		 (ProfileSystem::Instance->ActiveProfile.L8_S1_InvIdolTaken && !ProfileSystem::Instance->ActiveProfile.L8_S4_GoldIdolPlaced )) &&
		(ProfileSystem::Instance->ActiveProfile.L8_S2_GateOpened ))
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S4_Pool] = true;

	if (ProfileSystem::Instance->ActiveProfile.L8_S3_InvCrystalKeyTaken && !ProfileSystem::Instance->ActiveProfile.L8_S2_GateKeyPlaced)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S2_Garden] = true;
	if (ProfileSystem::Instance->ActiveProfile.L8_S3_InvSwordTaken && ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown &&  !ProfileSystem::Instance->ActiveProfile.L8_S2_InvSugarCaneTaken)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S2_Garden] = true;
	if (ProfileSystem::Instance->ActiveProfile.l8_s3_invMaryLeafTaken &&  !ProfileSystem::Instance->ActiveProfile.L8_S4_IsLeafPlacedInContainer &&
		(ProfileSystem::Instance->ActiveProfile.L8_S2_GateOpened ))
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S4_Pool] = true;

	/*if (ProfileSystem::Instance->ActiveProfile.L6_S2_InvIceKeyTaken && !ProfileSystem::Instance->ActiveProfile.L6_S4_KeyInIcePlaced)
	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L6_S4_FloatingIsland4] = true;*/
}

void L8_S3_BanyanTree::ProcessNonHOHint()
{
	int iNonHOHint = 0;
	int MaxNonHOHints = 16;
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
				/*case 3: ++iNonHOHint;
                if(ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicGlassTaken && !ProfileSystem::Instance->ActiveProfile.L8_S3_FloorCracked)
                {
                Hud->ShowHintForInvItem(eInv_L8_S1_MagicGlass, &HOScene->GetObjectByID("Bg")->GetWorldRect());
                return;
                }
                break;*/
			case 2:++iNonHOHint;
				if (ProfileSystem::Instance->ActiveProfile.L8_S1_InvMagicGlassTaken && !ProfileSystem::Instance->ActiveProfile.L8_S3_FloorCracked && !isGlassInHand)
				{
					//Hud->ShowHintForInvItem(eInv_L8_S1_MagicGlass, &HOScene->GetObjectByID("Bg")->GetWorldRect());
					// Linux: gives error of taking address of temporary also potential crash factor
					// below is fix
					CRectangle ObjRect =HOScene->GetObjectByID("Bg")->GetWorldRect();
					Hud->ShowHintForInvItem(eInv_L8_S1_MagicGlass, &ObjRect);
					return;
				}
				break;
			case 3:++iNonHOHint;
				if (isGlassInHand && !ProfileSystem::Instance->ActiveProfile.L8_S3_MatchingHOComplete)
				{
					if (selectedSymbol >= 0)
					{
						if (selectedSymbol % 2)
						{
							//Hud->InitHintTrailEmit(&symbols[selectedSymbol - 1]->GetWorldRect());
							// Linux: gives error of taking address of temporary also potential crash factor
							// below is fix
							CRectangle selectObjRect =symbols[selectedSymbol - 1]->GetWorldRect();
							Hud->InitHintTrailEmit(&selectObjRect);
							return;
						}
						else
						{
							//Hud->InitHintTrailEmit(&symbols[selectedSymbol + 1]->GetWorldRect());
							// Linux: gives error of taking address of temporary also potential crash factor
							// below is fix
							CRectangle selectObjRect =symbols[selectedSymbol + 1]->GetWorldRect();
							Hud->InitHintTrailEmit(&selectObjRect);

							return;
						}
					}
					else
					{
						for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
						{
							if (!ProfileSystem::Instance->ActiveProfile.L8_S3_MatchingSymbolFound[i])
							{
								//Hud->InitHintTrailEmit(&symbols[i]->GetWorldRect());
								// Linux: gives error of taking address of temporary also potential crash factor
								// below is fix
								CRectangle selectObjRect =symbols[i]->GetWorldRect();
								Hud->InitHintTrailEmit(&selectObjRect);

								return;
							}
						}
					}
				}
				break;

			case 4: ++iNonHOHint;
				if(!ProfileSystem::Instance->ActiveProfile.l8_s3_invMaryLeafTaken )
				{
					InitHintTrailEmit(&HORectLeaf);
					return;
				}
				break;
			case 5: ++iNonHOHint;
				if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L8_S2_Garden] )
				{
					InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
					return;
				}
				break;
			case 6: ++iNonHOHint;
				if(Hud->ShowHintForComboInvItems())
					return;
				break;

			case 7: ++iNonHOHint;
				Hud->teleportSystem->ShowTeleport();
				return;
				break;
		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();
}

void L8_S3_BanyanTree::SparkleUpdate()
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

void L8_S3_BanyanTree::CheckForPendingTask()
{
	isSparkleUpdatePending = false;
	for (int i = 0; i < (int)PopupArray.size(); i++)
	{
		Popup * popup = PopupArray[i];
		if (popup && isPopupActive(i))
		{
			//	if(popup->GetPopupState() == ePopupState_Inactive || popup->GetPopupState() == ePopupState_Closed)
			{
				if(popup->isTaskLeft())
				{
					sparklePointsVec.push_back(popup->GetSparkleUpdatePoint());
					isSparkleUpdatePending = true;
				}
			}
		}

	}

	if(!ProfileSystem::Instance->ActiveProfile.l8_s3_invMaryLeafTaken )
	{
		sparklePointsVec.push_back(CPoint(HORectLeaf.GetCenterX(), HORectLeaf.GetCenterY(), 0));
		isSparkleUpdatePending = true;

	}

}

void L8_S3_BanyanTree::SkipMagicGlass()
{
	if(ProfileSystem::Instance->ActiveProfile.L8_S3_MatchingHOComplete || !isGlassInHand)
	{
		SkipBtn->SetEnable(false);
		skipBTN_text->SetEnable(false);
		return;
	}
	SkipBtn->SetEnable(true);
	skipBTN_text->SetEnable(true);
	if(SkipBtn->IsIntersect(Control::Input->Mpos()))
	{
		SkipBtn->SetAnimation(1);
		Cursor::SetMode(CA_HandPointy);
		if(Control::Input->LBclicked())
		{
			for (int i = 0; i < MAX_L8_S1_MATCHING_SYMBOLS_COUNT; i++)
				ProfileSystem::Instance->ActiveProfile.L8_S3_MatchingSymbolFound[i] = true;
			ProfileSystem::Instance->ActiveProfile.L8_S3_MatchingHOComplete = true;

			ProfileSystem::Instance->ActiveProfile.L8_S3_InvIdolTaken = true;
			ProfileSystem::Instance->ActiveProfile.L8_S3_FloorCracked = true;
			ProfileSystem::Instance->ActiveProfile.L8_MagicSymbolFound[2] = true;
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S3_IdolBronze] = true;

			bool allSymbolsFound = true;
			for (int i = 0; i < MAX_L8_MAGIC_SYMBOLS; i++)
			{
				if(!ProfileSystem::Instance->ActiveProfile.L8_MagicSymbolFound[i])
				{
					allSymbolsFound = false;
					break;
				}
			}

			if (allSymbolsFound)
			{
				ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S1_MagicGlass] = false;
				Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_S1_MagicGlass);
			}

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L8_S3_BanyanTree;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
	}
	else
	{
		SkipBtn->SetAnimation(0);
		Cursor::SetMode(CA_Normal);
	}
}

void L8_S3_BanyanTree::NavigateToScene()
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