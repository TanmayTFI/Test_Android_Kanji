//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 02 : Naga Temple HO
//====================================

#include "L2_S2_NagaTempleHO.h"
#include "Hud.h"
#include "HOScene.h"
#include "Rect.h"
#include "Application.h"
#include "SDrop.h"
#include "Util.h"
#include "exMath.h"
#include "AwardManager.h"

const CRectangle  BtnRectMatch3HO(500.0f,5.0f,265.0f,39.0f);

#pragma region L2S2HiddenObjectClass

L2S2HiddenObject::L2S2HiddenObject()
{
	_isActive = true;
	_isFound = false;
	_isAnimComplete = false;

	_startPoint = NULL;
	_endPoint = NULL;
	_ctrlPoint = NULL;	

	_inHudObj = NULL;
	_scnObj = NULL;

	_polygonArea = NULL;

	hoScaleTimer = hoFlyingTimer = 0;
	_flownToHUDCallback = nullptr;
	HOTrailFX = NULL;
	hoClickFX = NULL;

	_isObjectScaling = false;
}

L2S2HiddenObject::~L2S2HiddenObject()
{
	_startPoint = NULL;
	_endPoint = NULL;
	SAFE_DELETE(_ctrlPoint);

	_scnObj = NULL;	
	_polygonArea = NULL;	
	_isFound = false;
	SAFE_DELETE(HOTrailFX);
	SAFE_DELETE(hoClickFX);
}

void L2S2HiddenObject::InitObject(std::function<void (CObject*)> flownToHUDCallback)
{
	_flownToHUDCallback = flownToHUDCallback;
	_startPoint = &_polygonArea->center;
	_endPoint = &_inHudObj->Pos;
	_ctrlPoint = new CPoint((_startPoint->y + _endPoint->y)/2.0f, (_startPoint->y + _endPoint->y)/2.0f, 0);

	_inHudObj->SetZ(eZOrder_INVBOXITEMS);
	_flyingObj->SetZ(eZOrder_INVBOXITEMS);
	_flyingObj->Show(false);

	if(rand()%10>5)
		_ctrlPoint->x += (200+rand()%200);
	else
		_ctrlPoint->x -= (200+rand()%200);

	HOTrailFX = new SuperFX("PRT\\L2\\HOTrailFX.INI", PointSystem::CreateCPoint(1065,122,0), 1845);

	hoClickFX = new SuperFX("PRT\\L2\\HOFX.INI", PointSystem::CreateCPoint(564,368,0), 100); 
	hoClickFX->SetPos(_startPoint);
}

bool L2S2HiddenObject::MouseClicked(CPoint pos)
{
	if (_isFound || !_isActive)
	{
		return false;
	}

	if (_polygonArea->Intersection(&pos))
	{
		OnFound();
		return true;
	}

	return false;
}

void L2S2HiddenObject::OnFound()
{
	_scnObj->FadeOut(1.5f);
	_flyingObj->Show(true);
	_isFound = true;

	_isObjFlying = true;
	hoClickFX->Start();
	_isObjectScaling = true;
}

void L2S2HiddenObject::Update(float ds)
{
	if (_isAnimComplete || !_isFound)
		return;

	if(_isObjectScaling)
	{
		//scaling anim
		hoScaleTimer += Control::LogicRate;
		_flyingObj->SetScale(0.85f+Elixir::Sin(90+hoScaleTimer*360.0f)*0.15f);
		if(hoScaleTimer > 1.0f)
		{
			//start flying
			_isObjectScaling = false;
			HOTrailFX->Start();

		}
	}
	else
	{
		FXTimer += 1;
		if(FXTimer%3==0)
			HOTrailFX->Emit();

		//flying to HUD
		hoFlyingTimer += Control::LogicRate;

		CPoint temp = Elixir::GetBezierPoint(_startPoint, _ctrlPoint, _endPoint, hoFlyingTimer);
		_flyingObj->SetPos(&temp);
		HOTrailFX->SetPos(&temp);

		_flyingObj->SetAlpha(1.0f-0.5f*hoFlyingTimer);

		if(hoFlyingTimer>1.0f)
		{
			HOTrailFX->StopEmit();
			_isAnimComplete = true;
			_flyingObj->SetEnable(false);
			_inHudObj->PlayAnimation(1);
			_inHudObj->FadeIn(3.0f);

			if(_flownToHUDCallback!=nullptr)
				_flownToHUDCallback(this);
		}
	}
}

void L2S2HiddenObject::AlreadyFound()
{
	_isFound = true;
	_isAnimComplete = true;
	_scnObj->SetEnable(false);	
	_inHudObj->PlayAnimation(1);
}

void L2S2HiddenObject::SwitchToMatch3()
{
	if(_isActive)
		_inHudObj->Show(false);
}

void L2S2HiddenObject::SwitchToHO()
{
	if(_isActive)
		_inHudObj->Show(true);
}

#pragma endregion



L2_S2_NagaTempleHO::L2_S2_NagaTempleHO()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV2\\S2\\HO\\HO.SCN");

	musicFadeOutTimer = 0;

	HOScene->GetObjectByID("flame1")->SetScale(1.80f);
	HOScene->GetObjectByID("flame2")->SetScale(1.80f);


	//Scene animations
	HOScene->GetObjectByID("plantfg")->SetPivot(0, 120);
	HOScene->GetObjectByID("plantfg")->Animate(0,0,4,0,7.0f);

	HOScene->GetObjectByID("rayleft1")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("rayleft1")->Animate(0.15f, 1.0f, 5.0f);

	HOScene->GetObjectByID("rayleft1")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("rayleft1")->Animate(0.15f, 0.95f, 6.0f, 2.25f);

	HOScene->GetObjectByID("rayright1")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("rayright1")->Animate(0.15f, 0.95f, 7.0f);

	HOScene->GetObjectByID("rayright2")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("rayright2")->Animate(0.15f, 0.95f, 4.0f, 0.45f);

	FgPlantMesh = NULL;

	FgPlantMesh = new MeshObject("LV2\\S2\\HO\\meshplant.jpa", 5, 5);
	FgPlantMesh->SetPosAndParams(315, 3, 3, 2, 5, 2);
	FgPlantMesh->SetZ(12);

	HORightVFX = new SuperFX("PRT\\L2\\nagahoright.INI", PointSystem::CreateCPoint(1135,128,0), 100); 
	HORightVFX->Start();
	HORightVFX->AdvanceByTime(2.0f);

	HOLeftVFX = new SuperFX("PRT\\L2\\nagahoright.INI", PointSystem::CreateCPoint(187,145,0), 100); 
	HOLeftVFX->Start();
	HOLeftVFX->AdvanceByTime(2.0f);

	HOEndVFX = new SuperFX("PRT\\L2\\nagaendfx.INI", PointSystem::CreateCPoint(924,395,0), 100);

	for (int i = 0; i < MAX_L2S2_HOOBJECTS; i++)
	{
		HOPolyArray[i] = NULL;
	}

	//Polygon Init
	int pts0[] = {677,50,653,55,654,71,635,107,646,152,672,145,670,114,682,100,699,97,690,64};
	HOPolyArray[0]=  new CPolygon(10);
	HOPolyArray[0]->Init(pts0);
	int pts1[] = {527,232,522,232,518,260,524,269,529,257,546,264,537,275,544,277,579,277,586,263,583,232,576,232,571,251,563,255,547,259,530,253};
	HOPolyArray[1]=  new CPolygon(16);
	HOPolyArray[1]->Init(pts1);
	int pts2[] = {229,563,215,583,224,589,269,595,304,574,304,558,278,560,248,570};
	HOPolyArray[2]=  new CPolygon(8);
	HOPolyArray[2]->Init(pts2);
	int pts3[] = {928,311,925,295,961,285,971,302,953,319,929,322,903,321,888,339,873,346,882,324,901,310,905,316,929,318};
	HOPolyArray[3]=  new CPolygon(13);
	HOPolyArray[3]->Init(pts3);
	int pts4[] = {384,411,447,361,448,336,428,313,392,314,361,328,359,354,356,381};
	HOPolyArray[4]=  new CPolygon(8);
	HOPolyArray[4]->Init(pts4);
	int pts5[] = {776,222,776,236,757,243,722,237,718,223,741,213,758,212};
	HOPolyArray[5]=  new CPolygon(7);
	HOPolyArray[5]->Init(pts5);
	int pts6[] = {914,489,889,482,863,486,856,501,873,511,896,509,905,497};
	HOPolyArray[6]=  new CPolygon(7);
	HOPolyArray[6]->Init(pts6);
	int pts7[] = {714,398,725,404,722,421,708,430,695,422,691,407,697,399};
	HOPolyArray[7]=  new CPolygon(7);
	HOPolyArray[7]->Init(pts7);
	int pts8[] = {826,268,836,274,839,286,832,300,814,299,806,285,811,274};
	HOPolyArray[8]=  new CPolygon(7);
	HOPolyArray[8]->Init(pts8);
	int pts9[] = {1182,297,1177,280,1165,255,1142,249,1114,268,1111,291,1119,317,1130,331,1140,309,1166,303,1180,306};
	HOPolyArray[9]=  new CPolygon(11);
	HOPolyArray[9]->Init(pts9);
	//Polygon Init End

	isHOCompleted =  false;

	for (int i = 0; i < MAX_L2S2_HOOBJECTS; i++)
	{
		CObject *scnObj = HOScene->GetObjectByID(std::string("hoscnitem" + MKSTR(i)).c_str());
		CObject *flyObj = HOScene->GetObjectByID(std::string("hoflyitem" + MKSTR(i)).c_str());
		CObject *invObj = HOScene->GetObjectByID(std::string("hoinvboxitem" + MKSTR(i)).c_str());

		L2S2HiddenObject* ho = new L2S2HiddenObject();
		ho->setObjectID(i);
		ho->setSceneObject(scnObj);
		ho->setFlyingObj(flyObj);
		ho->setInHudObj(invObj);
		ho->setPolygonArea(HOPolyArray[i]);
		ho->InitObject(CC_CALLBACK_1(L2_S2_NagaTempleHO::OnObjFlyComplete, this));
		_hoList.push_back(ho);

		if(ProfileSystem::Instance->ActiveProfile.L2S2_HOItemsFound[i])
		{
			ho->AlreadyFound();
			_founfObjs.push_back(ho);
		}
	}

	

	//End Combining Animation assets
	NagaHeadFull = HOScene->GetObjectByID("NagaHeadFull");
	NagaHeadFixed = HOScene->GetObjectByID("NagaHeadFixed");

	NagaHeadFull->Show(false);
	NagaHeadFixed->Show(false);

	NagaHeadParts[0] = HOScene->GetObjectByID("p1headsback");
	NagaHeadParts[1] = HOScene->GetObjectByID("p2rodleft");
	NagaHeadParts[2] = HOScene->GetObjectByID("p2rodright");
	NagaHeadParts[3] = HOScene->GetObjectByID("p3crown");
	NagaHeadParts[4] = HOScene->GetObjectByID("p5eyes");
	NagaHeadParts[5] = HOScene->GetObjectByID("p6head");
	NagaHeadParts[6] = HOScene->GetObjectByID("p7chin");
	NagaHeadParts[7] = HOScene->GetObjectByID("p8earrings");

	for (int i = 0; i < 8; i++)
	{
		NagaHeadParts[i]->Show(false);
		NagaHeadParts[i]->SetZ(eZOrder_INVBOXITEMS);

		NagaPartEndPt[i] = NULL;		
		NagaPartEndPt[i] = new CPoint( NagaHeadParts[i]->Pos );
	}

	NagaPartStartPt[0] = new CPoint(725,708,0);
	NagaPartStartPt[1] = new CPoint(549,715,0);
	NagaPartStartPt[2] = new CPoint(625,713,0);
	NagaPartStartPt[3] = new CPoint(401,714,0);
	NagaPartStartPt[4] = new CPoint(844,742,0);
	NagaPartStartPt[5] = new CPoint(948,708,0);
	NagaPartStartPt[6] = new CPoint(474,716,0);
	NagaPartStartPt[7] = new CPoint(850,696,0);

	NagaPartStartScale[0] = 0.41f;
	NagaPartStartScale[1] = 0.5f;
	NagaPartStartScale[2] = 0.5f;
	NagaPartStartScale[3] = 0.5f;
	NagaPartStartScale[4] = 1.0f;
	NagaPartStartScale[5] = 0.85f;
	NagaPartStartScale[6] = 0.65f;
	NagaPartStartScale[7] = 1.26f;

	NagaPartCtrlPt[0] = new CPoint(826,550,0);
	NagaPartCtrlPt[1] = new CPoint(310,450,0);
	NagaPartCtrlPt[2] = new CPoint(810,520,0);
	NagaPartCtrlPt[3] = new CPoint(380,240,0);
	NagaPartCtrlPt[4] = new CPoint(590,590,0);
	NagaPartCtrlPt[5] = new CPoint(880,330,0);
	NagaPartCtrlPt[6] = new CPoint(720,630,0);
	NagaPartCtrlPt[7] = new CPoint(470,600,0);

	BzStartPt = new CPoint(470,600,0);
	BzEndPt = new CPoint(470,600,0);
	BzCtrlPt = new CPoint(470,600,0);

	IsHeadMovingToFix = false;
	headMovingTimer = 0;
	//End Combining Animation

	//Match 3
	match3Ctrl  = NULL;
	ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
	//BtnMatch3 = HOScene->GetObjectByID("btnMatch3");
	//BtnSwitchToHO = HOScene->GetObjectByID("btnSwitchToHO");


	match3Back = new CObject();
	match3Back->LoadInit("MNU\\EXTRAS\\BACKBUTTON.SPR", 1266, 30, eZOrder_Popup + 300);
	match3Back->SetUID("match3Back");
	match3Back->Show(false);
	HOScene->PushObject(match3Back);

	match3BackText = new CObject();
	match3BackText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\BACK.SPR", 1250, 27, eZOrder_Popup + 301);
	match3BackText->SetUID("match3BackText");
	match3BackText->Show(false);
	HOScene->PushObject(match3BackText);
#ifndef M3_BUTTON
	match3Back->SetEnable(false);
    match3BackText->SetEnable(false);
#endif
	if (ProfileSystem::Instance->_isExtrasGamePlay)
	{
		match3Back->SetEnable(false);
		match3BackText->SetEnable(false);
	}
	BtnMatch3 = new CObject();
	BtnMatch3->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNMATCH3.SPR", 630, 25, 17);
	BtnMatch3->SetUID("btnMatch3");
	HOScene->PushObject(BtnMatch3);

	BtnSwitchToHO = new CObject();
	BtnSwitchToHO->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNSWITCHTOHO.SPR", 630, 25, 18);
	BtnSwitchToHO->SetUID("btnSwitchToHO");
	HOScene->PushObject(BtnSwitchToHO);

	BtnMatch3->SetZ(100);
	BtnSwitchToHO->SetZ(eZOrder_PopupOverPopup + 300);
	BtnSwitchToHO->Show(false);

	ResetBtn = new CObject();
	ResetBtn->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\RESETBTN.SPR", HOScene->GetObjectByID("ResetBtn")->Pos.x, HOScene->GetObjectByID("ResetBtn")->Pos.y, HOScene->GetObjectByID("ResetBtn")->ZOrder);
	ResetBtn->SetUID("resetBtn");
	HOScene->PushObject(ResetBtn);

	ResetBtn->SetEnable(false);
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		ResetBtn->SetEnable(false);

	ResetDia = new CHOScene();
	ResetDia->Load("UI\\RESETDIA.SCN");

	CObject* resetText = new CObject();
	resetText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\HUD\\RESETCNF.SPR", 670, 308, 2);
	resetText->SetUID("resetText");
	ResetDia->PushObject(resetText);

	CObject* mask = new CObject();
	mask->LoadInit("UI\\MASK.SPR", 683, 384, -1);
	mask->SetUID("mask");
	mask->SetScale(2);
	ResetDia->PushObject(mask);

	CObject* btnText = new CObject();
	btnText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\YES.SPR", 590, 420, 5);
	btnText->SetUID("yesBTNText");
	ResetDia->PushObject(btnText);

	btnText = new CObject();
	btnText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\NO.SPR", 780, 420, 5);
	btnText->SetUID("noBTNText");
	ResetDia->PushObject(btnText);

	ResetDia->SetZOff(eZOrder_Popup);
	ResetDia->Show(false);
	IsResetPop = false;

	timer = 10.0f;
	isFadeIn = false;
	isFadeOut = false;
	isResetDiaFadeIn = false;
	isResetDiaFadeOut = true;

	int pts[] = {862,346,792,381,791,444,874,475,995,466,1064,418,1039,365,962,335,933,264,894,271};
	polyStatueInfo =  new CPolygon(10);
	polyStatueInfo->Init(pts);

	AwardManager::GetInstance()->StartHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);//lock in HO anim
	Hud->IsHOScene = true;

	animPartsFadeInTime = 0;
	IsAnimPartsFadingIn = false;

	IsAnimPartsJoining = false;
	animPartJoinTimer = 0;
	headMovingTimer = 0;
	IsHeadMovingToFix = false;

	GFInstance->PostUpdate(2);

	if (_founfObjs.size() == _hoList.size())
	{
		HandleHOComplete();
	}

	if(!ProfileSystem::Instance->ActiveProfile.L2S2_HONagaTempleVisitedOnce)
	{
		ProfileSystem::Instance->ActiveProfile.L2S2_HONagaTempleVisitedOnce = true;
		Hud->ShowBannerText("L2look");
	}

	Control::Audio->LoadSample(aAmbs2NagaHead,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbs2NagaHead) )		
	{
		Control::Audio->PlaySample(aAmbs2NagaHead,true);
	}
	
}

void L2_S2_NagaTempleHO::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Control::Audio->IsPlaying(aTrackL2theme) )
	{
		Control::Audio->Samples[aTrackL2theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL2theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL2theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL2ho) )
				  Control::Audio->PlaySample(aTrackL2ho, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else if( Control::Audio->IsPlaying(aTrackL2theme1) )
	{
		Control::Audio->Samples[aTrackL2theme1]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL2theme1]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL2theme1);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL2ho) )
				  Control::Audio->PlaySample(aTrackL2ho, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL2ho))
		{
			Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL2ho, true);
		}
	}
}

float L2_S2_NagaTempleHO::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L2_S2_NagaTempleHO::Update()
{
	NavigateToScene();
#ifdef _PARTICLEEDITORENABLED
	if (_isParticleEditorActive)
	{
		return;
	}
#endif

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	if(!BtnMatch3->Visible && !ProfileSystem::Instance->ActiveProfile.L2S2_HONagaTempleCompleted)
	{
		match3Back->Show(true);
		match3BackText->Show(true);
	}
	else
	{
		match3Back->Show(false);
		match3BackText->Show(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L2S2_HONagaTempleCompleted)
	{
		match3Back->Show(false);
		match3BackText->Show(false);
		BtnMatch3->Show(false);
		BtnSwitchToHO->Show(false);
	}

	//ResetHO();
	if(IsResetPop)
	{
		GFHud->AllowInput = false;
		return;
	}
	else
		GFHud->AllowInput = true;

#ifdef _CHEATS
	if(Control::Input->KpressedWithShift(K_VK_SPACE))
	{
		if(!ProfileSystem::Instance->ActiveProfile.L2S2_HONagaTempleCompleted)
		{
			Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);

			//Set all items to end states
			for (int i = 0; i < MAX_L2S2_HOOBJECTS; i++)
			{
			}
			HandleHOComplete();
			return;
		}
	}
#endif // _CHEATS



	if(!ProfileSystem::Instance->ActiveProfile.L2S2_HONagaTempleCompleted && ProfileSystem::Instance->ActiveProfile.L2S2_HONagaCompletedWithMatch3)
	{
		//switch from Match3 to Find Objects
		Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);

		//Set all items to end states
		
		HandleHOComplete();
		return;
	}

	if(isHOCompleted)
	{
		if(IsAnimPartsFadingIn)
		{
			animPartsFadeInTime += Control::LogicRate;
			if(animPartsFadeInTime>1.2f)
			{
				IsAnimPartsFadingIn = false;
				IsAnimPartsJoining = true;
			}
		}
		else if(IsAnimPartsJoining)
		{
			//animate the parts
			animPartJoinTimer += 0.5f*Control::LogicRate;
			for (int i = 0; i < 8; i++)
			{
				CPoint temp = Elixir::GetBezierPoint(NagaPartStartPt[i],NagaPartCtrlPt[i],NagaPartEndPt[i],animPartJoinTimer);
				NagaHeadParts[i]->SetPos(&temp);
				NagaHeadParts[i]->SetScale(NagaPartStartScale[i] + (1.0f-NagaPartStartScale[i])*easing(animPartJoinTimer,0,1,1.0f));
			}

			if(animPartJoinTimer>0.5f)
			{
				if(!NagaHeadFull->Visible)
				{
					NagaHeadFull->FadeIn(1.0f);
					for (int i = 0; i < 8; i++)
						NagaHeadParts[i]->FadeOut(1.0f);
				}
			}

			if(animPartJoinTimer>1.0f)
			{
				IsAnimPartsJoining = false;
				IsHeadMovingToFix = true;

				HOEndVFX->Start();

				BzStartPt->x = NagaHeadFull->Pos.x;
				BzStartPt->y = NagaHeadFull->Pos.y;

				BzEndPt->x = 924;
				BzEndPt->y = 216;

				BzCtrlPt->x = 714;
				BzCtrlPt->y = 118;
			}
		}
		else if(IsHeadMovingToFix)
		{
			headMovingTimer += 0.75f*Control::LogicRate;
			if(headMovingTimer<1.0f)
			{
				CPoint temp = Elixir::GetBezierPoint(BzStartPt,BzCtrlPt,BzEndPt,headMovingTimer);
				NagaHeadFull->SetPos(&temp);

				if(headMovingTimer>0.5f)
				{
					if(!NagaHeadFixed->Visible)
					{
						NagaHeadFixed->FadeIn(2.0f);
						NagaHeadFull->FadeOut(2.0f);
					}
				}
			}

			if(headMovingTimer>2.75f)
			{
				IsHeadMovingToFix = false;
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S2_NagaTemple;
				SendMessage(ControlExit, ID, HOSM_NewArea);
			}
		}
		return;
	}


	//HO Click Checking
	if(!ProfileSystem::Instance->ActiveProfile._IsMatch3Playing) //active only when no match3
	{
		for (int i = 0; i < (int)_hoList.size(); i++)
		{
			if( Control::Input->LBclicked() )
			{
				if (_hoList[i]->MouseClicked(pos))
				{
					int randId = rand()%3;
					switch (randId)
					{
					case 0:		Control::Audio->QuickLoadAndPlaySFX("S2NagaHOObjectFoundv1");		break;
					case 1:		Control::Audio->QuickLoadAndPlaySFX("S2NagaHOObjectFoundv2");		break;
					case 2:		Control::Audio->QuickLoadAndPlaySFX("S2NagaHOObjectFoundv3");		break;
					default:
						break;
					}

					ProfileSystem::Instance->ActiveProfile.L2S2_HOItemsFound[i] = true;
				}
			}
		}


		if(polyStatueInfo->Intersection(&pos))
		{
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L2scatter");
			}
		}
	}

	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{
		if(BtnRectMatch3HO.Intersection(&pos) && !match3Ctrl->isGemAnimPlaying())
		{
			Cursor::SetMode(CA_HandPointy);
			BtnSwitchToHO->PlayAnimation(1);
			if( Control::Input->LBclicked() )
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXClick);

				//Show Inv Box Items
				for (int i = 0; i < MAX_L2S2_HOOBJECTS; i++)
				{
					_hoList[i]->SwitchToHO();
				}

				match3Ctrl->clearBoard();
				BtnMatch3->Show(true);
				BtnSwitchToHO->Show(false);
				BtnSwitchToHO->PlayAnimation(0);

				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

				Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
				return;
			}
		}
		else
		{
			BtnSwitchToHO->PlayAnimation(0);
		}
	}
	else
	{
		if(!isHOCompleted && BtnRectMatch3HO.Intersection(&pos) )
		{
			Cursor::SetMode(CA_HandPointy);
			BtnMatch3->PlayAnimation(1);
			if( Control::Input->LBclicked() )
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXClick);

				if(match3Ctrl == NULL)
				{
					match3Ctrl = new Match3(4);
					AddControl(match3Ctrl);
				}
				else
				{
					match3Ctrl->ShowBoard();
				}

				//Hide Inv Box Items
				for (int i = 0; i < MAX_L2S2_HOOBJECTS; i++)
				{
					_hoList[i]->SwitchToMatch3();
				}

				BtnMatch3->Show(false);
				BtnMatch3->PlayAnimation(0);
				BtnSwitchToHO->Show(true);

				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = true;

				Hud->InvBoxCtrl->SetLockBase(MATCH3_FRAME);
				return;
			}
		}
		else
		{
			BtnMatch3->PlayAnimation(0);
		}
	}
	if(match3Back->IsIntersect(pos) && !ProfileSystem::Instance->_isExtrasGamePlay && ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{
		if( Control::Input->LBclicked() )
		{
			if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
			{
				if(match3Ctrl!=NULL)
					match3Ctrl->clearBoard();
				BtnMatch3->Show(false);
				BtnSwitchToHO->Show(false);
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
			}

			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S2_NagaTemple;
			SendMessage(ControlExit, ID, HOSM_NewArea);
		}
	}
	if( IsMouseOverGoBackArea(&pos)
		#ifdef M3_BUTTON
		&& BtnMatch3->Visible
        #endif
        )
	{

#ifdef TOUCH_UI
		//if(!GFHud->InvBoxCtrl->navTapped)
			//Cursor::SetMode(CA_ExitDown,eArea_L2_S2_NagaTemple);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L2_S2_NagaTemple);
#endif

		if( Control::Input->LBclicked() )
		{
			
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L2_S2_NagaTemple, 2);
			}
#else
			if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
			{
				if(match3Ctrl!=NULL)
					match3Ctrl->clearBoard();
				BtnMatch3->Show(false);
				BtnSwitchToHO->Show(false);
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
			}

			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S2_NagaTemple;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			return;
		}
	}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
			if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
			{
				if(match3Ctrl!=NULL)
					match3Ctrl->clearBoard();
				BtnMatch3->Show(false);
				BtnSwitchToHO->Show(false);
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
			}
			Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S2_NagaTemple;
			SendMessage(ControlExit, ID, HOSM_NewArea);
			return;
	}
	#endif
}

void L2_S2_NagaTempleHO::HandleHOComplete()
{
	Control::Audio->QuickLoadAndPlaySFX("s2ho_nagaheadcombine");

	ProfileSystem::Instance->ActiveProfile.L2S2_HONagaTempleCompleted = true;
	isHOCompleted = true;

	if(_handleExtrasGameComplete(false))
		return;

	IsAnimPartsFadingIn = true;

	for (int i = 0; i < 8; i++)
	{
		NagaHeadParts[i]->SetPos(NagaPartStartPt[i]);
		NagaHeadParts[i]->SetScale(NagaPartStartScale[i]);
		NagaHeadParts[i]->FadeIn(1.0f);
	}

	for (int i = 0; i < MAX_L2S2_HOOBJECTS; i++)
	{
		_hoList[i]->getInHudObj()->FadeOut();
	}
}

L2_S2_NagaTempleHO::~L2_S2_NagaTempleHO()
{
	_founfObjs.clear();
	SAFE_DELETE(polyStatueInfo);
	SAFE_DELETE(FgPlantMesh);

	SAFE_DELETE(HORightVFX);
	SAFE_DELETE(HOLeftVFX);
	SAFE_DELETE(HOEndVFX);

	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
		ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;

	Hud->InvBoxCtrl->ResetToInvBox();//Reset back to Inv Objects
	Hud->IsHOScene = false;

	for (int i = 0; i < MAX_L2S2_HOOBJECTS; i++)
	{
		SAFE_DELETE(HOPolyArray[i]);
	}

	for (int i = 0; i < (int)_hoList.size(); i++)
	{
		SAFE_DELETE(_hoList[i]);
	}

	SAFE_DELETE(BzStartPt);
	SAFE_DELETE(BzEndPt);
	SAFE_DELETE(BzCtrlPt);

	for (int i = 0; i < 8; i++)
	{
		SAFE_DELETE(NagaPartStartPt[i]);
		SAFE_DELETE(NagaPartCtrlPt[i]);
		SAFE_DELETE(NagaPartEndPt[i]);
	}
	SAFE_DELETE(ResetDia);
	//MUSIC_SFX


	Control::Audio->StopSample(aAmbs2NagaHead);
	Control::Audio->UnloadSample(aAmbs2NagaHead);
	//MUSIC_SFX

}


void L2_S2_NagaTempleHO::ReceiveMessage(int val)
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
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L1_S2_Frontyard;
		SendMessage(ControlExit, ID, HOSM_NewArea);
	}	
}

void L2_S2_NagaTempleHO::ProcessNonHOHint()
{
	int iNonHOHint = 0; 
	int MaxNonHOHints = 2;
	do
	{
		switch( iNonHOHint )
		{
		default:			
			//Exit coordinates
		case 0: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
			{
				return;
			}
			break;
		case 1: ++iNonHOHint;
			AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			for (int i = 0; i < MAX_L2S2_HOOBJECTS; i++)
			{
				int size = (int)_hoList.size();  //Sac: converted Implicit to explicit
				for (int i = 0; i < size; i++)
				{
					if (!_hoList[i]->getIsFound())
					{
						InitHintTrailEmit(_hoList[i]->getSceneObject()->Pos.x, _hoList[i]->getSceneObject()->Pos.y);
						return;
					}
				}
			}
			break;


		}
	}
	while( iNonHOHint < MaxNonHOHints );//Loop till we reach old value!!
	Hud->ShowNothingToDoMsgBanner();
}

void L2_S2_NagaTempleHO::OnObjFlyComplete(CObject *obj)
{
	_founfObjs.push_back((L2S2HiddenObject*)obj);

	if (_hoList.size() == _founfObjs.size())
	{
		AwardManager::GetInstance()->CompleteHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
		HandleHOComplete();
	}
}

void L2_S2_NagaTempleHO::ResetHO()
{
	if(ProfileSystem::Instance->_isExtrasGamePlay)
		return;
	if(timer > 0)
	{
		timer -= (float(Control::MasterTmr->RealDMS) / 1000.0f);
		ResetBtn->SetEnable(false);
		if(!IsResetPop)
		{
			if(!isResetDiaFadeOut)
				ResetDia->FadeOut(2);
			isResetDiaFadeOut = true;
		}
		return;
	}
	else
	{
		ResetBtn->SetEnable(true);	
		if(!isFadeIn)
			ResetBtn->FadeIn(1);
		isFadeIn = true;
	}
	if(ResetBtn->IsIntersect(Control::Input->Mpos()))
	{
		ResetBtn->SetAnimation(1);
		if(!IsResetPop)
			Cursor::SetMode(CA_HandPointy);
		if(KInput::getLeftButtonState() == ISDOWN)
		{
			IsResetPop = true;
		}
	}
	else
	{
		ResetBtn->SetAnimation(2);
		Cursor::SetMode(CA_Normal);
	}

	if(IsResetPop)
	{
		if(!isResetDiaFadeIn)
		{
			ResetDia->FadeIn(2);
		}
		if(!isFadeOut)
			ResetBtn->FadeOut(1);
		isFadeOut = true;
		isResetDiaFadeIn = true;
		CObject *resetYes = ResetDia->GetObjectByID("BtnYes");
		CObject *resetNo = ResetDia->GetObjectByID("BtnNo");

		if(Hud->CheckPopupButton(resetYes) == Popup_Click)
		{
			
			for (int i = 0; i < MAX_L2S2_HOOBJECTS; i++)
			{
				ProfileSystem::Instance->ActiveProfile.L2S2_HOItemsFound[i] = false;
			}

			ProfileSystem::Instance->ActiveProfile.L2S2_HONagaTempleCompleted = false;
			ProfileSystem::Instance->ActiveProfile.L2S2_HONagaCompletedWithMatch3 = false;

			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S2_HONagaTemple;
			SendMessage(ControlExit, ID, HOSM_NewArea);
			IsResetPop = false;
		}
		if(Hud->CheckPopupButton(resetNo) == Popup_Click)
		{
			IsResetPop = false;
			timer = 10.0f;
			isFadeIn = false;
			isFadeOut = false;
			isResetDiaFadeOut = false;
			isResetDiaFadeIn = false;
		}
	}
	else
	{
		ResetBtn->SetEnable(true);
	}
}
void L2_S2_NagaTempleHO::NavigateToScene()
{
	if(GFHud->InvBoxCtrl->navTapped)
	{
		if( Control::Input->LBclicked() )
		{
			if(Control::Input->Mpos().x > GFHud->InvBoxCtrl->navRectX2 && Control::Input->Mpos().x < GFHud->InvBoxCtrl->navRectX1 && Control::Input->Mpos().y > GFHud->InvBoxCtrl->navRectY2 && Control::Input->Mpos().y < GFHud->InvBoxCtrl->navRectY1)
			{
				//Cursor::SetMode(CA_ExitUp);
				if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
				{
					if(match3Ctrl!=NULL)
						match3Ctrl->clearBoard();
					BtnMatch3->Show(false);
					BtnSwitchToHO->Show(false);
					ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
				}
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