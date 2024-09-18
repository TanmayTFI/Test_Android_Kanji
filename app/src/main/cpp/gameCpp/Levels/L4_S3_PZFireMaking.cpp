//====================================
//	DQFYLH
//	HO System
//	Level 4 - Scene 3 :  PUZZLE FIRE MAKING
//====================================

#include "L4_S3_PZFireMaking.h"
#include "Hud.h"
#include "GameFlow.h"

const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);

L4_S3_PZFireMaking::L4_S3_PZFireMaking()
{	
	EmberPos = NULL;

	AltarPoly				= NULL;
	ChartPoly				= NULL;
	InstructionChart		= NULL;
	FireMakingAnim			= NULL;
	EmberDropAnim			= NULL;
	OilPourAnim				= NULL;
	S3_FireBoardSmokeFX		= NULL;

	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV4\\S3\\P1\\P1.SCN");
	InitSceneElements();
	InitAnimations();

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL4s3FireMakingPuzzleAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL4s3FireMakingPuzzleAmb) )		
	{
		Control::Audio->PlaySample(aAmbL4s3FireMakingPuzzleAmb,true);
	}


	InstructionChart = new L4S3_PZPopChart(0, Hud, HOScene, eZOrder_Popup, ChartPoly->center, CC_CALLBACK_1(L4_S3_PZFireMaking::OnPopupClosed, this));
	InstructionChart->SetPopupBoundingBox(PopAreaRect);

	//MUSIC_SFX
}

void L4_S3_PZFireMaking::OnPopupClosed(Popup * popup)
{
	IsChartActive = false;
}

void L4_S3_PZFireMaking::InitSceneElements()
{
	//HACK
	//ProfileSystem::Instance->ActiveProfile.L4_S4_PotWithOilTaken = true;
	IsAnimPlaying			= false;
	ObjectInHandID			= -1;
	IsChartActive			= false;
	IsFadingIn				= false;
	IsFadingOut				= false;
	FadeAlpha				= 0.0f;
	ChartScene				= NULL;

	BowAnim					= HOScene->GetObjectByID("BowAnim");
	BowInHand				= HOScene->GetObjectByID("BowInHand");
	Bow						= HOScene->GetObjectByID("Bow");
	Bowl				    = HOScene->GetObjectByID("Bowl");
	BowlPour			    = HOScene->GetObjectByID("BowlPour");
	CoinAnim			    = HOScene->GetObjectByID("CoinAnim");
	CoinInHand			    = HOScene->GetObjectByID("CoinInHand");
	CoinInScene			    = HOScene->GetObjectByID("CoinInScene");
	HandL				    = HOScene->GetObjectByID("HandL");
	HandR				    = HOScene->GetObjectByID("HandR");
	Sheet					= HOScene->GetObjectByID("Sheet");
	OilDrop				    = HOScene->GetObjectByID("OilDrop");
	OilOnTwigs				= HOScene->GetObjectByID("OilOnTwigs");
	PlankAnim			    = HOScene->GetObjectByID("PlankAnim");
	PlankInHand			    = HOScene->GetObjectByID("PlankInHand");
	PlankInScene		    = HOScene->GetObjectByID("PlankInScene");
	StickAnim			    = HOScene->GetObjectByID("StakeAnim");
	Ember 				    = HOScene->GetObjectByID("Ember");
	EmberInHand				= HOScene->GetObjectByID("EmberInHand");
	Twig1					= HOScene->GetObjectByID("Twig1");
	Twig2					= HOScene->GetObjectByID("Twig2");
	Twig3					= HOScene->GetObjectByID("Twig3");
	Twig4					= HOScene->GetObjectByID("Twig4");
	FireAnim				= HOScene->GetObjectByID("FireLoop");

	EmberPos				= new CPoint(753,563,0); // &Ember->Pos;

	ScnObjects[e_plank]		= PlankInScene;
	ScnObjects[e_bow]		= Bow;
	ScnObjects[e_coin]		= CoinInScene;
	ScnObjects[e_ember]		= Ember;

	InHandObjects[e_plank]	= PlankInHand;
	InHandObjects[e_bow]	= BowInHand;
	InHandObjects[e_coin]	= CoinInHand;
	InHandObjects[e_ember]	= Ember;

	ObjStates[e_plank]		=  ObjL4S3_Inactive;
	ObjStates[e_bow]		=  ObjL4S3_Locked;
	ObjStates[e_coin]		=  ObjL4S3_Locked;
	ObjStates[e_ember]		=  ObjL4S3_Locked;

	if(ProfileSystem::Instance->ActiveProfile.L4_S3_IsPlankPlaced)
	{
		PlankInScene		->SetEnable(false);
		ObjStates[e_plank]		=  ObjL4S3_Used;
		ObjStates[e_bow]		=  ObjL4S3_Inactive;
	}
	else
		PlankAnim			->SetEnable(false);
	if( ProfileSystem::Instance->ActiveProfile.L4_S3_IsBowPlaced )
	{
		Bow					->SetEnable(false); 
		ObjStates[e_bow]		=  ObjL4S3_Used;
		ObjStates[e_coin]		=  ObjL4S3_Inactive;
	}
	else
	{
		BowAnim				->SetEnable(false);
		StickAnim			->SetEnable(false);
	}
	if( ProfileSystem::Instance->ActiveProfile.L4_S3_IsCoinPlaced )
	{
		CoinInScene			->SetEnable(false);
		ObjStates[e_coin]		=  ObjL4S3_Used;
	}
	else
		CoinAnim			->SetEnable(false);
	if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced )
	{
		Twig1				->SetEnable(false); 
		Twig2				->SetEnable(false); 
		Twig3				->SetEnable(false); 
		Twig4				->SetEnable(false); 
	}

	if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsEmberCreated )
	{
		Ember->SetEnable(false);
	}
	else
	{
		CoinAnim			->SetEnable(false);
		BowAnim				->SetEnable(false);
		StickAnim			->SetEnable(false);
		PlankAnim			->SetEnable(false);
		Sheet				->SetEnable(false);
		/*if( ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced &&
			ProfileSystem::Instance->ActiveProfile.L4_S3_IsOilApplied)*/
		ObjStates[e_ember]		=  ObjL4S3_Inactive;
	}

	if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsOilApplied )
		OilOnTwigs				->SetEnable(false);

	BowlPour				->SetEnable(false);
	Bowl					->SetEnable(false);
	HandL					->SetEnable(false);
	HandR					->SetEnable(false);
	OilDrop					->SetEnable(false);
	PlankInHand				->SetEnable(false);
	BowInHand				->SetEnable(false);
	CoinInHand				->SetEnable(false);
	EmberInHand				->SetEnable(false);
	FireAnim				->SetEnable(false);

	int AltarArray[] = {80,199,341,51,547,178,297,357};
	AltarPoly=  new CPolygon(4);
	AltarPoly->Init(AltarArray);

	int ChartArray[] = {997,277,1069,231,1148,334,1062,381};
	ChartPoly=  new CPolygon(4);
	ChartPoly->Init(ChartArray);

	int PlankInSceneArray[] = {856,182,821,73,893,52,904,94,911,119};
	int CoinInSceneArray[] = {997,192,1036,202,1049,158,1000,140};
	int BowArray[] = {827,417,830,474,1099,618,1176,658,1227,626,1074,409};
	int EmberInSceneArray[] = {714,545,847,598,793,676,656,617};

	ObjPoly[e_plank]		= new CPolygon(5);
	ObjPoly[e_plank]->Init(PlankInSceneArray);
	ObjPoly[e_bow]			= new CPolygon(6);
	ObjPoly[e_bow]->Init(BowArray);
	ObjPoly[e_coin]		= new CPolygon(4);
	ObjPoly[e_coin]->Init(CoinInSceneArray);
	ObjPoly[e_ember]		= new CPolygon(4);
	ObjPoly[e_ember]->Init(EmberInSceneArray);

	int SheetArray[] = {529,648,887,691,915,480,590,428};
	int PlankArray[] = {242,737,261,482,712,313,987,557,952,752};
	int CoinArray[] = {533,540,524,423,684,412,693,539};
	int EmberArray[] = {80,199,341,51,547,178,297,357};

	DestPoly[e_plank]		= new CPolygon(4);
	DestPoly[e_plank]->Init(SheetArray);
	DestPoly[e_bow]			= new CPolygon(5);
	DestPoly[e_bow]->Init(PlankArray);
	DestPoly[e_coin]		= new CPolygon(4);
	DestPoly[e_coin]->Init(CoinArray);
	DestPoly[e_ember]		= new CPolygon(4);
	DestPoly[e_ember]->Init(EmberArray);

	TaskPendingUpdate();

	
}


void L4_S3_PZFireMaking::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;	

	if( Control::Audio->IsPlaying(aTrackL4theme) )
	{
		Control::Audio->Samples[aTrackL4theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			Control::Audio->Samples[aTrackL4theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			Control::Audio->StopSample(aTrackL4theme);
			musicFadeOutTimer = 0;
			if( !Control::Audio->IsPlaying(aTrackL4puzzle) )
				Control::Audio->PlaySample(aTrackL4puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL4puzzle))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL4puzzle, true);
		}
	}
}

void L4_S3_PZFireMaking::Update()
{
#ifdef _PARTICLEEDITORENABLED
	if(_isParticleEditorActive)
		return;
#endif // _PARTICLEEDITORENABLED

	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	HiddenObjectSystem::HOUpdate();
	if(IsAnimPlaying)
	{
		Hud->AllowInput = false;
		return;
	}

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	if(IsChartActive)
		return;

	if( DestPoly[e_bow]->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L4_S3_IsCoinPlaced 
		&& !ProfileSystem::Instance->ActiveProfile.L4_S3_IsEmberCreated )
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked())
		{
			Cursor::SetMode(CA_Normal);
			IsAnimPlaying = true;
			FireMakingAnim->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s3pz1_creatingFire");
			//if(ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced && ProfileSystem::Instance->ActiveProfile.L4_S3_IsOilApplied)
			ObjStates[e_ember] = ObjL4S3_Inactive;
			ProfileSystem::Instance->ActiveProfile.L4_S3_IsEmberCreated = true;
		}
		return;
	}

	if (ObjectInHandID != -1 )
	{
		InHandObjects[ObjectInHandID]->SetPos( &pos );
		if( pos.y >= ( HoveredBackBtnRectMax.topleft.y + 60 ) )
		{
			InHandObjects[ObjectInHandID]->SetEnable(false);
			ObjStates[ObjectInHandID] = ObjL4S3_Inactive;
			ScnObjects[ObjectInHandID]->SetEnable(true);
			if(ObjectInHandID == e_ember)
			{
				Ember->SetEnable(true);
				Ember->SetPos( EmberPos );
			}
			InHandObjects[ObjectInHandID]->SetZ( InHandObjects[ObjectInHandID]->ZOrder - eZOrder_INVBOXITEMS );
			ObjectInHandID = -1;
		}
		if( Control::Input->LBclicked() )
		{
			InHandObjects[ObjectInHandID]->SetZ( InHandObjects[ObjectInHandID]->ZOrder - eZOrder_INVBOXITEMS );
			InHandObjects[ObjectInHandID]->SetEnable(false);
			if(DestPoly[ObjectInHandID]->Intersection(&pos) )
			{
				ObjStates[ObjectInHandID] = ObjL4S3_Used;
				switch (ObjectInHandID)
				{
				case e_plank: ObjStates[e_bow] = ObjL4S3_Inactive;
					PlankAnim->SetEnable(true);
					Control::Audio->QuickLoadAndPlaySFX("s3pz1_placingPlank");
					ProfileSystem::Instance->ActiveProfile.L4_S3_IsPlankPlaced = true;
					break;	
				case e_bow: ObjStates[e_coin] = ObjL4S3_Inactive;
					BowAnim->SetEnable(true);
					StickAnim->SetEnable(true);
					Control::Audio->QuickLoadAndPlaySFX("s3pz1_placingBowWithStick");
					ProfileSystem::Instance->ActiveProfile.L4_S3_IsBowPlaced = true;
					break;
				case e_coin: 
					CoinAnim->SetEnable(true);
					Control::Audio->QuickLoadAndPlaySFX("s3pz1_placingCoin");
					ProfileSystem::Instance->ActiveProfile.L4_S3_IsCoinPlaced = true;
					break;
				case e_ember: 
					if( ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced && ProfileSystem::Instance->ActiveProfile.L4_S3_IsOilApplied)
					{
						ProfileSystem::Instance->ActiveProfile.L4_S3_IsCaveLit = true;
						EmberDropAnim->PlayAnim();
						Control::Audio->QuickLoadAndPlaySFX("s3pz1_emberDrop");
						IsAnimPlaying			= true;
					}
					else
					{
						 if( ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced && !ProfileSystem::Instance->ActiveProfile.L4_S3_IsOilApplied )
						{
							Hud->ShowBannerText("L4need"); 
						}
						else
							Hud->ShowBannerText("L4burn");
						InHandObjects[ObjectInHandID]->SetEnable(false);
						ObjStates[ObjectInHandID] = ObjL4S3_Inactive;
						ScnObjects[ObjectInHandID]->SetEnable(true);
						Ember->SetEnable(true);
						Ember->SetPos( EmberPos );
					}
					break;
				}
			}
			else
			{
				InHandObjects[ObjectInHandID]->SetEnable(false);
				ObjStates[ObjectInHandID] = ObjL4S3_Inactive;
				ScnObjects[ObjectInHandID]->SetEnable(true);
				if(ObjectInHandID == e_ember)
				{
					Ember->SetEnable(true);
					Ember->SetPos( EmberPos );
				}
			}
			ObjectInHandID = -1;
		}
		return;
	}
	if( ObjectInHandID == -1 )
	{
		for (int i = 0; i < e_MaxL4S3; i++)
		{
			if( ObjPoly[i]!= NULL && ObjPoly[i]->Intersection(&pos)  )
			{
				if( ObjStates[i] == ObjL4S3_Inactive )
					Cursor::SetMode(CA_HandTake);
				if( Control::Input->LBclicked())
				{
					if( ObjStates[i] == ObjL4S3_Inactive )
					{
						ObjStates[i] = ObjL4S3_Active;
						ObjectInHandID = i;
						InHandObjects[i]->SetPos(&pos);
						ScnObjects[i]->SetEnable(false);
						InHandObjects[i]->SetEnable(true);
						InHandObjects[i]->SetZ( InHandObjects[i]->ZOrder + eZOrder_INVBOXITEMS );
						if(ObjectInHandID == e_ember)
							Sheet->SetEnable(false);
						return;
					}
					else if( ObjStates[i] == ObjL4S3_Locked )
					{
						ScnObjects[i]->Shake(0,2,0,0.5f);
						switch (i)
						{
						case e_bow:		Hud->ShowBannerText("L4add");
							break;
						case e_coin:	Hud->ShowBannerText("L4coin");
							break;
						case e_ember:	Hud->ShowBannerText("L4ember");
							Sheet->Shake(0,2,0,0.5f);
							break;
						}
						
					}
				}
				return;
			} 
		}
	}



	if(ChartPoly->Intersection(&pos) )
	{
		Cursor::SetMode(CA_Look);
		if( Control::Input->LBclicked())
		{
			Cursor::SetMode(CA_Normal);
			InstructionChart->OpenPopup(); 
			IsChartActive = true;
		}
		return;
	}

	UpdateBannerMessages(pos);

	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed()  && ObjectInHandID == -1)
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S3_Cave;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}	
	#endif

	if( IsMouseOverGoBackArea(&pos) && ObjectInHandID == -1 )
	{
		
		#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
	//		Cursor::SetMode(CA_ExitDown,eArea_L4_S3_Cave);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L4_S3_Cave);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L4_S3_Cave, 2);
			}
#else			
			Cursor::SetMode(CA_Normal);
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S3_Cave;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
#endif
								
		}	
	}

	

	


}

void L4_S3_PZFireMaking::UpdateBannerMessages(CPoint pos)
{
	if( Hud->IsDraggingItem() || IsMouseOverGoBackArea(&pos) )
	{
		return;
	}
	if( AltarPoly->Intersection( &pos) )
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked() )
		{
			if(!ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced && !ProfileSystem::Instance->ActiveProfile.L4_S4_InvTwig2Taken && 
				!ProfileSystem::Instance->ActiveProfile.L4_S2_InvTwig1Taken && !ProfileSystem::Instance->ActiveProfile.L4_S5_InvTwig3Taken)
			{
				Hud->ShowBannerText("L4stuff");
			}
			else if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced && ProfileSystem::Instance->ActiveProfile.L4_S4_InvTwig2Taken && 
				ProfileSystem::Instance->ActiveProfile.L4_S2_InvTwig1Taken && ProfileSystem::Instance->ActiveProfile.L4_S5_InvTwig3Taken )
			{
				Hud->ShowBannerText("L4burn"); 
			}
			else if( ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced && !ProfileSystem::Instance->ActiveProfile.L4_S3_IsOilApplied )
			{
				Hud->ShowBannerText("L4need"); 
			}
			else if( ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced && ProfileSystem::Instance->ActiveProfile.L4_S3_IsOilApplied 
				&& !ProfileSystem::Instance->ActiveProfile.L4_S3_IsEmberCreated)
			{
				Hud->ShowBannerText("L4how");
			}
			else if( ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced && ProfileSystem::Instance->ActiveProfile.L4_S3_IsOilApplied 
				&& ProfileSystem::Instance->ActiveProfile.L4_S3_IsEmberCreated)
			{
				Hud->ShowBannerText("L4light");
			}
		}
	}
	if(DestPoly[e_bow]->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L4_S3_IsBowPlaced && !ProfileSystem::Instance->ActiveProfile.L4_S3_IsCoinPlaced )
	{
		Cursor::SetMode(CA_Gear);
		if(Control::Input->LBclicked())
			Hud->ShowBannerText("L4stake");
	}
	else if(DestPoly[e_plank]->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L4_S3_IsBowPlaced && ProfileSystem::Instance->ActiveProfile.L4_S3_IsPlankPlaced )
	{
		Cursor::SetMode(CA_Gear);
		if(Control::Input->LBclicked())
			Hud->ShowBannerText("L4bow");
	}
}


float L4_S3_PZFireMaking::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L4_S3_PZFireMaking::Render(const CPoint* pCam)
{  
	if( !Visible ) 
		return;


}

L4_S3_PZFireMaking::~L4_S3_PZFireMaking()
{	
	Hud->InvBoxCtrl->ResetToInvBox();
	SAFE_DELETE(EmberPos);
	
	for (int i = 0; i < e_MaxL4S3; i++)
	{
		SAFE_DELETE(ObjPoly[i]); 
		SAFE_DELETE(DestPoly[i]); 
	}
	SAFE_DELETE(AltarPoly);
	SAFE_DELETE(ChartPoly);
	SAFE_DELETE(InstructionChart);

	SAFE_DELETE(FireMakingAnim);
	SAFE_DELETE(EmberDropAnim);
	SAFE_DELETE(OilPourAnim);
	SAFE_DELETE(S3_FireBoardSmokeFX);

	//MUSIC_SFX

	Control::Audio->StopSample(aAmbL4s3FireMakingPuzzleAmb);
	Control::Audio->UnloadSample(aAmbL4s3FireMakingPuzzleAmb);
	//MUSIC_SFX
}

void L4_S3_PZFireMaking::InitAnimations()
{
	S3_FireBoardSmokeFX = new SuperFX("PRT\\L4\\S3_FireBoardSmoke.INI", PointSystem::CreateCPoint(719,584,0), 1100); 
	superfxArray.push_back( S3_FireBoardSmokeFX );
	FireMakingAnim = NULL;
	FireMakingAnim = new JSONAnimator( 0, CC_CALLBACK_1(L4_S3_PZFireMaking::OnAnimationComplete, this ));
	FireMakingAnim->parseAndLoadJSON("ANIM\\L4\\S3FIREMAKEANIM.JSON", HOScene, &superfxArray );

	EmberDropAnim = NULL;
	EmberDropAnim = new JSONAnimator( 1, CC_CALLBACK_1(L4_S3_PZFireMaking::OnAnimationComplete, this ));
	EmberDropAnim->parseAndLoadJSON("ANIM\\L4\\S3EMBERDROPANIM.JSON", HOScene );

	OilPourAnim = NULL;
	OilPourAnim = new JSONAnimator( 2, CC_CALLBACK_1(L4_S3_PZFireMaking::OnAnimationComplete, this ));
	OilPourAnim->parseAndLoadJSON("ANIM\\L4\\S3OILPOURANIM.JSON", HOScene );
}

void L4_S3_PZFireMaking::OnAnimationComplete(int index)
{	
	IsAnimPlaying			= false;
	switch (index)
	{
	case 0:	FireMakingAnim->SetEnableObjects(false);
		Ember->SetEnable(true);
		PlankAnim->SetEnable(false);
		S3_FireBoardSmokeFX->StopImmediate();
		break;
	case 1:
		Sheet->SetEnable( false);
		EmberInHand->SetEnable(false);
		//Show Objective completed
		if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L4O3_FindWayToMakeFire] && 
			!ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L4O3_FindWayToMakeFire] )
		{
			ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L4O3_FindWayToMakeFire] = true;
			Hud->ShowObjectiveFX(eObj_L4O3_FindWayToMakeFire,true);
		}

		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S3_Cave;
		SendMessage(ControlExit, ID, HOSM_NewArea);	
		break;
	case 2:
		OilPourAnim->SetEnableObjects(false);
		OilOnTwigs->SetEnable(true);
		break;
	}
	Hud->AllowInput = true;
}

void L4_S3_PZFireMaking::ReceiveMessage(int val)
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
			if( itemVal == eInv_L4_S2_Twig1 )
			{
				if(ProfileSystem::Instance->ActiveProfile.L4_S2_InvTwig1Taken && ProfileSystem::Instance->ActiveProfile.L4_S5_InvTwig3Taken 
					&& ProfileSystem::Instance->ActiveProfile.L4_S4_InvTwig2Taken )
				{
					if( AltarPoly->Intersection( &dropPos ) )
					{
						Hud->ReleaseDrag();
						Hud->CloseInventory(); 

						if (!ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced )
						{
							Control::Audio->QuickLoadAndPlaySFX("s3pz1_twigsdrop");

							Twig1->SetEnable(true); 
							Twig2->SetEnable(true); 
							Twig3->SetEnable(true); 
							Twig4->SetEnable(true); 
							ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced = true;
							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S2_Twig1] = false;
							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S4_Twig2] = false;
							ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S5_Twig3] = false;
							Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S2_Twig1); 
						//Sac: Crashes the game because the first twig1 is removed and twig2 and twig3 are referencing to twig1
						//	Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S4_Twig2);
						//	Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S5_Twig3);
							return;
						}
					}
					else
						GFInstance->WrongInvDropMsg(Hud);
				}
				else 
				{
					Hud->ReleaseDrag();
					Hud->CloseInventory();
					Hud->ShowBannerText("L4twigs");
				}
			}
			else if( itemVal == eInv_L4_S3_PotWithOlive )
			{
				if( AltarPoly->Intersection( &dropPos ) && ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced )
				{
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 

					if (!ProfileSystem::Instance->ActiveProfile.L4_S3_IsOilApplied )
					{
						Control::Audio->QuickLoadAndPlaySFX("s3pz1_Pouringoliveoil");

						IsAnimPlaying = true;
						OilPourAnim->PlayAnim();
						ProfileSystem::Instance->ActiveProfile.L4_S3_IsOilApplied = true;
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L4_S3_PotWithOlive] = false;
						Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L4_S3_PotWithOlive); 
						return;
					}
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


void L4_S3_PZFireMaking::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced && ProfileSystem::Instance->ActiveProfile.L4_S2_InvTwig1Taken && 
		ProfileSystem::Instance->ActiveProfile.L4_S4_InvTwig2Taken &&  ProfileSystem::Instance->ActiveProfile.L4_S5_InvTwig3Taken )
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L4_S3_IsOilApplied && ProfileSystem::Instance->ActiveProfile.L4_S4_PotWithOilTaken && 
		ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced) 
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L4_S3_IsPlankPlaced )
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L4_S3_IsBowPlaced )
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L4_S3_IsCoinPlaced )
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L4_S3_IsEmberCreated && ProfileSystem::Instance->ActiveProfile.L4_S3_IsCoinPlaced )
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L4_S3_IsCaveLit && ProfileSystem::Instance->ActiveProfile.L4_S3_IsEmberCreated )
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S3_Cave] = isAnyTaskLeft;


	if (!ProfileSystem::Instance->ActiveProfile.L4_S5_InvTwig3Taken)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S5_HillSide] = true;
}


void L4_S3_PZFireMaking::ProcessNonHOHint()
{
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
			if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced && ProfileSystem::Instance->ActiveProfile.L4_S2_InvTwig1Taken && 
				ProfileSystem::Instance->ActiveProfile.L4_S4_InvTwig2Taken &&  ProfileSystem::Instance->ActiveProfile.L4_S5_InvTwig3Taken )
			{
				Hud->ShowHintForInvItem( eInv_L4_S2_Twig1, PointSystem::CreateCPoint(AltarPoly->center.x,AltarPoly->center.y,0) );
				return;
			}
			break;
		case 1: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsOilApplied && ProfileSystem::Instance->ActiveProfile.L4_S4_PotWithOilTaken &&
				ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced ) 
			{
				Hud->ShowHintForInvItem( eInv_L4_S3_PotWithOlive, PointSystem::CreateCPoint(AltarPoly->center.x,AltarPoly->center.y,0) );
				return;
			}
			break;
		case 2: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsPlankPlaced )
			{
				Hud->ShowHintForSceneItem( &ObjPoly[e_ember]->center );
				InitHintTrailEmit(&ObjPoly[e_plank]->center ,true);
				return;
			}
			break;
		case 3: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsBowPlaced && ProfileSystem::Instance->ActiveProfile.L4_S3_IsPlankPlaced )
			{
				Hud->ShowHintForSceneItem( &DestPoly[e_bow]->center );
				InitHintTrailEmit(&ObjPoly[e_bow]->center ,true);
				return;
			}
			break;
		case 4: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsCoinPlaced && ProfileSystem::Instance->ActiveProfile.L4_S3_IsBowPlaced )
			{
				Hud->ShowHintForSceneItem( &DestPoly[e_coin]->center );
				InitHintTrailEmit(&ObjPoly[e_coin]->center ,true);
				return;
			}
			break;
		case 5: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsEmberCreated && ProfileSystem::Instance->ActiveProfile.L4_S3_IsCoinPlaced )
			{
				InitHintTrailEmit(&DestPoly[e_bow]->center ,true);
				return;
			}
			break;
		case 6: ++iNonHOHint;
			if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsCaveLit && ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced 
				&& ProfileSystem::Instance->ActiveProfile.L4_S3_IsOilApplied && ProfileSystem::Instance->ActiveProfile.L4_S3_IsEmberCreated)
			{
				Hud->ShowHintForSceneItem( &ObjPoly[e_ember]->center );
				InitHintTrailEmit(&AltarPoly->center ,true);
				return;
			}
			break;
		case 7: ++iNonHOHint;
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L4_S3_Cave] )
			{
				InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
				return;
			}
			break;
		case 8: ++iNonHOHint;
				{
					if(Hud->ShowHintForComboInvItems())
						return;
				}
				break;
		case 9: ++iNonHOHint;
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

void L4_S3_PZFireMaking::SparkleUpdate()
{
	if (ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		return;

	

	CheckForPendingTask();

	if (!isSparkleUpdatePending)
		return;

	HiddenObjectSystem::SparkleUpdate();
}

void L4_S3_PZFireMaking::CheckForPendingTask()
{
	isSparkleUpdatePending = false;

	if(InstructionChart->GetPopupState() == ePopupState_Inactive) 
	{
		if(InstructionChart->isTaskLeft())
		{
			sparklePointsVec.push_back(InstructionChart->GetSparkleUpdatePoint());
			isSparkleUpdatePending = true;
		}
	}

	if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced && ProfileSystem::Instance->ActiveProfile.L4_S2_InvTwig1Taken && 
		ProfileSystem::Instance->ActiveProfile.L4_S4_InvTwig2Taken &&  ProfileSystem::Instance->ActiveProfile.L4_S5_InvTwig3Taken )
	{
		sparklePointsVec.push_back(CPoint(AltarPoly->center.x, AltarPoly->center.y, 0));
		isSparkleUpdatePending = true;
	}
	else if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsOilApplied && ProfileSystem::Instance->ActiveProfile.L4_S4_PotWithOilTaken &&
		ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced ) 
	{
		sparklePointsVec.push_back(CPoint(AltarPoly->center.x, AltarPoly->center.y, 0));
		isSparkleUpdatePending = true;
	}
	
	if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsCaveLit && ProfileSystem::Instance->ActiveProfile.L4_S3_IsFireSticksPlaced 
		&& ProfileSystem::Instance->ActiveProfile.L4_S3_IsOilApplied && ProfileSystem::Instance->ActiveProfile.L4_S3_IsEmberCreated)
	{
		sparklePointsVec.push_back(CPoint(ObjPoly[e_ember]->center.x, ObjPoly[e_ember]->center.y, 0));
		isSparkleUpdatePending = true;
	}

	if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsPlankPlaced)
	{
		sparklePointsVec.push_back(CPoint(ObjPoly[e_plank]->center.x, ObjPoly[e_plank]->center.y, 0));
		isSparkleUpdatePending = true;
	}

	if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsBowPlaced && ProfileSystem::Instance->ActiveProfile.L4_S3_IsPlankPlaced)
	{
		sparklePointsVec.push_back(CPoint(ObjPoly[e_bow]->center.x, ObjPoly[e_bow]->center.y, 0));
		isSparkleUpdatePending = true;

	}

	if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsCoinPlaced && ProfileSystem::Instance->ActiveProfile.L4_S3_IsBowPlaced )
	{
		sparklePointsVec.push_back(CPoint(ObjPoly[e_coin]->center.x, ObjPoly[e_coin]->center.y, 0));
		isSparkleUpdatePending = true;
	}

	if( !ProfileSystem::Instance->ActiveProfile.L4_S3_IsEmberCreated && ProfileSystem::Instance->ActiveProfile.L4_S3_IsCoinPlaced )
	{
		sparklePointsVec.push_back(CPoint(ObjPoly[e_ember]->center.x, ObjPoly[e_ember]->center.y, 0));
		isSparkleUpdatePending = true;
	}

}
void L4_S3_PZFireMaking::NavigateToScene()
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