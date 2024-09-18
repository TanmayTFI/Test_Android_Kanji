//====================================
// DQFULP
// Hidden Object System
// Author : Faisal
// LEVEL3 - SCENE3 : Pool
//====================================

#include "L3_S3_Pool.h"
#include "Banners.h"
#include "Hud.h"
#include "Application.h"
#include "SDrop.h"

#include "L3S3_Pop1Saw.h"
#include "L3S3_Pop2Showel.h"
#include "L3S3_Pop3Pool.h"
#include "L3S3_Pop4Lamp.h"
#include "L3S3_Pop5Door.h"

const CRectangle  	PopAreaRect(345.0f,90.0f,665.0f,501.0f);

const CRectangle  InfoTreeRect(208.0f,4.0f,358.0f,290.0f);

//Scene Rects
const CRectangle  SceneRectFortFountain(892.0f,301.0f,133.0f,149.0f);
const CRectangle  SceneRectPuzzleUnderwater(386.0f,394.0f,236.0f,179.0f);
const CRectangle  PuzzleRectBlocks(944.0f,464.0f,177.0f,123.0f);

//const CRectangle  InfoRectCreepers(736.0f,295.0f,94.0f,145.0f); //discarded

const CRectangle  LampMaskRect(493.0f,337.0f,193.0f,282.0f);

L3_S3_Pool::L3_S3_Pool()
{
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV3\\S3\\S3.SCN");
	
	musicFadeOutTimer = 0;
	HOScene->GetObjectByID("fireanim1")->SetRotate(-30);

	HOScene->GetObjectByID("lampglow")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("lampglow")->Animate(0.25f,0.85f, 5.0f, 1.34f);

	HOScene->GetObjectByID("poolglow")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("poolglow")->Animate(0.15f,0.75f, 4.0f, 1.34f);

	HOScene->GetObjectByID("yellowglowL2")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("yellowglowL2")->Animate(0.15f,0.75f, 5.0f, 1.34f);

	HOScene->GetObjectByID("yellowglowL1")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("yellowglowL1")->Animate(0.15f,0.75f, 5.0f, 1.34f);

	HOScene->GetObjectByID("yellowglowR1")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("yellowglowR1")->Animate(0.15f,0.75f, 5.0f, 1.34f);

	HOScene->GetObjectByID("yellowglowR2")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("yellowglowR2")->Animate(0.15f,0.75f, 8.0f);

	HOScene->GetObjectByID("blueglow1")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("blueglow1")->Animate(0.15f,0.75f, 7.0f, 1.34f);

	HOScene->GetObjectByID("blueglow1")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("blueglow1")->Animate(0.15f,0.75f, 6.0f);

	HOScene->GetObjectByID("blueglow2")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("blueglow2")->Animate(0.15f,0.75f, 3.0f, 1.34f);

	HOScene->GetObjectByID("blueglow3")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("blueglow3")->Animate(0.15f,0.75f, 4.0f, 1.34f);

	HOScene->GetObjectByID("blueglow4")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("blueglow4")->Animate(0.15f,0.75f, 8.0f, 1.34f);

	HOScene->GetObjectByID("blueglow5")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("blueglow5")->Animate(0.15f,0.95f, 8.0f, 1.34f);

	HOScene->GetObjectByID("pinkglow2")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("pinkglow2")->Animate(0.15f,0.75f, 6.0f, 1.34f);

	HOScene->GetObjectByID("pinkglow3")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("pinkglow3")->Animate(0.15f,0.95f, 5.0f, 1.34f);

	HOScene->GetObjectByID("pinkglow1")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("pinkglow1")->Animate(0.15f,0.85f, 7.0f);

	HOScene->GetObjectByID("blueglow6")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("blueglow6")->Animate(0.15f,0.95f, 6.0f, 1.34f);

	HOScene->GetObjectByID("plantfg")->SetPivot(2, 100);
	HOScene->GetObjectByID("plantfg")->Animate(0,0,3,0,6.0f);

	HOScene->GetObjectByID("treeleft")->SetPivot(-78,74);
	HOScene->GetObjectByID("treeleft")->Animate(0,4,2,0,7.0f);

	HOScene->GetObjectByID("leftvine")->Animate(0,4,1,0,6.0f);

	meshWater = NULL;
	meshWater = new MeshObject(HOScene->GetObjectByID("poolwatermesh"), 5, 5);
	meshWater->meshFX->SetSpeedAndDisplacement(4, 3, 3, 3);


	s3crystal1FX = new SuperFX("PRT\\L3\\s3crystal1.INI", PointSystem::CreateCPoint(409,217,0), 100); 
	s3crystal1FX->Start();
	s3crystal1FX->AdvanceByTime(2.0f);

	s3crystal2FX = new SuperFX("PRT\\L3\\s3crystal2.INI", PointSystem::CreateCPoint(615,614,0), 100); 
	s3crystal2FX->Start();
	s3crystal2FX->AdvanceByTime(2.0f);

	s3crystal3FX = new SuperFX("PRT\\L3\\s3crystal3.INI", PointSystem::CreateCPoint(331,568,0), 100); 
	s3crystal3FX->Start();
	s3crystal3FX->AdvanceByTime(2.0f);

	s3crystal4FX = new SuperFX("PRT\\L3\\s3crystal4.INI", PointSystem::CreateCPoint(1066,494,0), 100); 
	s3crystal4FX->Start();
	s3crystal4FX->AdvanceByTime(2.0f);

	s3smokeFX = new SuperFX("PRT\\L3\\s3smoke.INI", PointSystem::CreateCPoint(882,241,0), 100); 
	s3smokeFX->Start();
	s3smokeFX->AdvanceByTime(2.0f);	

	VFXArray.push_back(s3crystal1FX);
	VFXArray.push_back(s3crystal2FX);
	VFXArray.push_back(s3crystal3FX);
	VFXArray.push_back(s3crystal4FX);
	VFXArray.push_back(s3smokeFX);
	
	InitPopups();

	GFInstance->PostUpdate(2);

	SceneObjectsUpdate();

	s3riseupraysFX = NULL;
	s3riseupspreadfxFX = NULL;

	isLampRisingFromWater = false;

	LampRising = HOScene->GetObjectByID("lamp");
	LampRising->SetMaskRect(LampMaskRect);

	if(ProfileSystem::Instance->ActiveProfile.L3_S3_PZUnderwaterPuzzleSolved)
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S3_IsTempleRisenFromWater)
		{
			isLampRisingFromWater = true;

			s3riseupraysFX = new SuperFX("PRT\\L3\\s3riseuprays.INI", PointSystem::CreateCPoint(611,551,0), 100); 
			s3riseupraysFX->Start();
			//s3riseupraysFX->AdvanceByTime(2.0f);

			s3riseupspreadfxFX = new SuperFX("PRT\\L3\\s3riseupspreadfx.INI", PointSystem::CreateCPoint(610,572,0), 100); 
			s3riseupspreadfxFX->Start();
			//s3riseupspreadfxFX->AdvanceByTime(2.0f);

			LampRisingEndY = (int)(LampRising->Pos.y);
			LampRising->Pos.y += 140.0f;

			LampRising->SetEnable(true);
			HOScene->GetObjectByID("lamprisemask")->SetEnable(true);

			Control::Audio->PlaySample(aSFXMagic3);

			Control::Audio->QuickLoadAndPlaySFX("s3_lampmoveup");

			HOScene->Shake(1, 1, 2.5f);
		}
	}

	if(!ProfileSystem::Instance->ActiveProfile.L3_S3_VisitingPoolForFirsttime)
	{
		ProfileSystem::Instance->ActiveProfile.L3_S3_VisitingPoolForFirsttime = true;
		Hud->ShowBannerText("L3visualize");
	}

	//MUSIC_SFX
	Control::Audio->LoadSample(aAmbL3s3Pool,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL3s3Pool) )		
	{
		Control::Audio->PlaySample(aAmbL3s3Pool,true);
	}
	
	//MUSIC_SFX
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L3_S3_Pool]  = true;

	//Morph Object
#ifdef _CEBUILD
	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S3Collected)
	{
		Hud->setMorphObjects(HOScene->GetObjectByID("morphtarget"), HOScene->GetObjectByID("morphbase"));		
		CRectangle  MorphRect(824.0f,176.0f,40.0f,53.0f);
		Hud->setMorphObjRect(MorphRect);
	}
	else
	{
		HOScene->GetObjectByID("morphtarget")->SetEnable(false);
	}
#else
	HOScene->GetObjectByID("morphtarget")->SetEnable(false);
#endif 
}

void L3_S3_Pool::OnLampRisingCompleted()
{
	HOScene->GetObjectByID("lamprisemask")->SetEnable(false);

	isLampRisingFromWater = false;
	ProfileSystem::Instance->ActiveProfile.L3_S3_IsTempleRisenFromWater = true;

	s3riseupraysFX->StopEmit();
	s3riseupspreadfxFX->StopEmit();
}

void L3_S3_Pool::InitPopups()
{	
	ActivePopupID = ePopInactive;

	for (int i = 0; i < ePopL3S3_Max; i++)
	{
		PopupArray[i] = NULL;
		PopStates[i] = ePopupState_Inactive;
		PopPolyArray[i] = NULL;
	}

	int popID=0;

	//Popup 1
	popID = ePop1_Saw;
	if(!ProfileSystem::Instance->ActiveProfile.L3_S3_InvSawTaken)
	{
		int pop1Pts[] = {723,233,771,215,826,227,829,265,777,297,785,422,717,426,705,271};
		CPolygon *pop1ClickPoly=  new CPolygon(8);
		pop1ClickPoly->Init(pop1Pts);

		Popup* Pop1Scene = new L3S3_Pop1Saw(popID, Hud, HOScene, eZOrder_Popup, pop1ClickPoly->center, CC_CALLBACK_1(L3_S3_Pool::OnPopupClosed, this));
		Pop1Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[popID] = pop1ClickPoly;
		PopupArray[popID]	= Pop1Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	//Popup 2
	popID = ePop2_Showel;
	if(!ProfileSystem::Instance->ActiveProfile.L3_S3_InvShowelTaken)
	{
		int pop2Pts[] = {335,116,418,137,450,188,449,312,362,347,261,324,239,216};
		CPolygon* pop2ClickPoly=  new CPolygon(7);
		pop2ClickPoly->Init(pop2Pts);


		Popup* Pop2Scene = new L3S3_Pop2Showel(popID, Hud, HOScene, eZOrder_Popup, pop2ClickPoly->center, CC_CALLBACK_1(L3_S3_Pool::OnPopupClosed, this));
		Pop2Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[popID] = pop2ClickPoly;
		PopupArray[popID]	= Pop2Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	//Popup 3
	popID = ePop3_Pool;
	if(!(ProfileSystem::Instance->ActiveProfile.L3_S3_InvLadderStep2Taken && ProfileSystem::Instance->ActiveProfile.L3_S3_InvCupFilledTaken))
	{
		int pop3Pts[] = {700,509,854,521,858,562,665,611,656,551};
		CPolygon *pop3ClickPoly=  new CPolygon(5);
		pop3ClickPoly->Init(pop3Pts);

		Popup* Pop3Scene = new L3S3_Pop3Pool(popID, Hud, HOScene, eZOrder_Popup, pop3ClickPoly->center, CC_CALLBACK_1(L3_S3_Pool::OnPopupClosed, this));
		Pop3Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[popID] = pop3ClickPoly;
		PopupArray[popID]	= Pop3Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	//Popup 4
	popID = ePop4_Lamp;
	if(ProfileSystem::Instance->ActiveProfile.L3_S3_PZUnderwaterPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L3_S3_InvTempleBoxTaken)
	{
		int pop4Pts[] = {555,620,638,596,650,544,662,440,608,403,542,420};
		CPolygon* pop4ClickPoly=  new CPolygon(6);
		pop4ClickPoly->Init(pop4Pts);

		Popup* Pop4Scene = new L3S3_Pop4Lamp(popID, Hud, HOScene, eZOrder_Popup, pop4ClickPoly->center, CC_CALLBACK_1(L3_S3_Pool::OnPopupClosed, this));
		Pop4Scene->SetPopupBoundingBox(PopAreaRect);

		PopPolyArray[popID] = pop4ClickPoly;
		PopupArray[popID]	= Pop4Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	//Popup 5
	popID = ePop5_Door;
	if(!ProfileSystem::Instance->ActiveProfile.L3_S3_IsDoorToS4Opened)
	{
		int pop5Pts[] = {916,297,981,293,1020,323,1026,445,892,443,880,336};
		CPolygon* pop5ClickPoly=  new CPolygon(6);
		pop5ClickPoly->Init(pop5Pts);

		L3S3_Pop5Door* Pop5Scene = new L3S3_Pop5Door(popID, Hud, HOScene, eZOrder_Popup, pop5ClickPoly->center, CC_CALLBACK_1(L3_S3_Pool::OnPopupClosed, this));
		Pop5Scene->SetPopupBoundingBox(PopAreaRect);		

		PopPolyArray[popID] = pop5ClickPoly;
		PopupArray[popID]	= Pop5Scene;

		PopStates[popID] = ePopupState_Closed;
	}

	GFInstance->PostUpdate(1);
}

L3_S3_Pool::~L3_S3_Pool()
{
	SAFE_DELETE(meshWater);

	int size = (int)VFXArray.size(); //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		SAFE_DELETE(VFXArray[i]);
	}
	VFXArray.clear();

	SAFE_DELETE(s3riseupraysFX);
	SAFE_DELETE(s3riseupspreadfxFX);

	for (int i = 0; i < ePopL3S3_Max; i++)
	{
		SAFE_DELETE(PopupArray[i]);
		SAFE_DELETE(PopPolyArray[i]);
	}

	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL3s3Pool);
	Control::Audio->UnloadSample(aAmbL3s3Pool);
	//MUSIC_SFX
}

void L3_S3_Pool::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;

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

void L3_S3_Pool::SparkleUpdate()
{
	if(isSparkleUpdatePending)
	{
		isSparkleUpdatePending = false;
		if(SparkleFX==NULL)
		{
			SparkleFX = new SuperFX("PRT\\glint.INI",PointSystem::CreateCPoint(0,0,0),120);		
		}
		//Game Logic Goes here
		numSparkles = 0;
		currentSparkleID = 0;
		
		//Check for Popup Tasks
		for (int i = 0; i < ePopL3S3_Max; i++)
		{
			if(PopStates[i] != ePopupState_Inactive) //if popup is active
			{
				if(PopupArray[i]->isTaskLeft())
				{
					sparklePoints[numSparkles].SetXY(PopPolyArray[i]->center.x, PopPolyArray[i]->center.y);
					numSparkles++;
				}
			}
		}

		if(ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlocksPlaced && !ProfileSystem::Instance->ActiveProfile.L3_S3_PZUnderwaterPuzzleSolved)
		{
			sparklePoints[numSparkles].SetXY(SceneRectPuzzleUnderwater.GetCenterX(), SceneRectPuzzleUnderwater.GetCenterY());
			numSparkles++;
		}

		if(!ProfileSystem::Instance->ActiveProfile.L3_S3_InvCupTaken)
		{
			sparklePoints[numSparkles].SetXY(PuzzleRectBlocks.GetCenterX(), PuzzleRectBlocks.GetCenterY());
			numSparkles++;
		}
	}

	sparkleTimer += Control::LogicRate;
	if(sparkleTimer>sparkleThreshold)
	{
		sparkleTimer = 0;
		if(numSparkles>0)
		{			
			SparkleFX->SetPos(&sparklePoints[currentSparkleID]);
			SparkleFX->Start();
			currentSparkleID += 1;
			if(currentSparkleID>=numSparkles)
				currentSparkleID = 0;
		}
	}
}

void L3_S3_Pool::SceneObjectsUpdate()
{
	if(!ProfileSystem::Instance->ActiveProfile.L3_S3_PZUnderwaterPuzzleSolved)
	{
		HOScene->GetObjectByID("puzboxL")->SetEnable(false);
		HOScene->GetObjectByID("puzboxR")->SetEnable(false);

		HOScene->GetObjectByID("lampglow")->SetEnable(false);
		HOScene->GetObjectByID("lamp")->SetEnable(false);
		HOScene->GetObjectByID("lamprisemask")->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlocksPlaced)
	{
		HOScene->GetObjectByID("puzboxL")->SetEnable(true);
		HOScene->GetObjectByID("puzboxR")->SetEnable(true);
	}

	if(ProfileSystem::Instance->ActiveProfile.L3_S3_IsDoorToS4Opened)
	{
		HOScene->GetObjectByID("doorright")->SetEnable(false);
	}

	TaskPendingUpdate();
}

void L3_S3_Pool::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	for (int i = 0; i < ePopL3S3_Max; i++)
	{
		if(PopStates[i] != ePopupState_Inactive) //if popup is active
		{
			if(PopupArray[i]->isTaskLeft())
			{
				isAnyTaskLeft = true;
			}
		}
	}

	if( !ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlocksPlaced )
	{
		if(ProfileSystem::Instance->ActiveProfile.L3_S2_M6_CollectedBoxIndex[0] > -1 && ProfileSystem::Instance->ActiveProfile.L3_S2_M6_CollectedBoxIndex[1] > -1)
		{
			isAnyTaskLeft = true;
		}
	}
	else if( !ProfileSystem::Instance->ActiveProfile.L3_S3_PZUnderwaterPuzzleSolved )
	{
		isAnyTaskLeft = true;
	}

	if(!ProfileSystem::Instance->ActiveProfile.L3_S3_InvCupTaken)
	{
		isAnyTaskLeft = true; //Block Puzzle Active
	}

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S3_Pool] = isAnyTaskLeft;

	if(ProfileSystem::Instance->ActiveProfile.L3_S3_InvSawTaken && !ProfileSystem::Instance->ActiveProfile.L3_S1_InvFlagFullTaken)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S1_FortEntrance] = true;

	if(ProfileSystem::Instance->ActiveProfile.L3_S3_InvCupFilledTaken && ProfileSystem::Instance->ActiveProfile.L3_S1_EmberPopupState == 0) //not fire extinguished
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S1_FortEntrance] = true;

	if(ProfileSystem::Instance->ActiveProfile.L3_S3_InvShowelTaken && ProfileSystem::Instance->ActiveProfile.L3_S1_EmberPopupState == 2) //charcoal removed
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S1_FortEntrance] = true;

	if(ProfileSystem::Instance->ActiveProfile.L3_S3_InvShowelTaken && ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L3_S2_P1Room3Lock] && !ProfileSystem::Instance->ActiveProfile.L3_S2_R3StonesRemoved)
		ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S2_P1Room1Start] = true;
}

void L3_S3_Pool::OnPopupClosed(Popup* popup)
{
	ActivePopupID = ePopInactive;
	int popID = popup->GetPopupId();

	SceneObjectsUpdate();
	isSparkleUpdatePending = true;

	if(popID == ePop1_Saw)
	{
		if(ProfileSystem::Instance->ActiveProfile.L3_S3_InvSawTaken)
		{
			//disable popup
			PopStates[popID] = ePopupState_Inactive;
		}
	}
	else if(popID == ePop2_Showel)
	{
		if(ProfileSystem::Instance->ActiveProfile.L3_S3_InvShowelTaken)
		{
			//disable popup
			PopStates[popID] = ePopupState_Inactive;
		}
	}
	else if(popID == ePop3_Pool)
	{	
		if(ProfileSystem::Instance->ActiveProfile.L3_S3_InvLadderStep2Taken && ProfileSystem::Instance->ActiveProfile.L3_S3_InvCupFilledTaken)
		{
			//disable popups
			PopStates[popID] = ePopupState_Inactive;
		}
	}
	else if(popID == ePop4_Lamp)
	{	
		if(ProfileSystem::Instance->ActiveProfile.L3_S3_InvTempleBoxTaken)
		{
			//disable popups
			PopStates[popID] = ePopupState_Inactive;
		}
	}
	else if(popID == ePop5_Door)
	{
		if(ProfileSystem::Instance->ActiveProfile.L3_S3_IsDoorToS4Opened)
		{
			//disable popup
			PopStates[popID] = ePopupState_Inactive;
		}
	}
}

void L3_S3_Pool::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;
#ifdef DISABLE_PREV_CUR
	Hud->PrevCursorMode = -1;
#endif
	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing)
		return;

	CPoint pos = Control::Input->Mpos();

	if(isLampRisingFromWater)
	{
		Cursor::SetMode(CA_Normal);
		HOScene->update(Control::LogicRate);
		LampRising->Pos.y -= Control::LogicRate*36.0f;
		if(LampRising->Pos.y<LampRisingEndY)
		{
			LampRising->Pos.y = (float)LampRisingEndY;
			OnLampRisingCompleted();

			Hud->ShowBannerText("L3wonder");
		}	
		return;
	}

	if(!ProfileSystem::Instance->ActiveProfile.IsHideSparkles)
		SparkleUpdate();
	
	if( ActivePopupID == ePopInactive)
	{
		Cursor::SetMode(CA_Normal);
		for (int i = 0; i < ePopL3S3_Max; i++)
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

		if(InfoTreeRect.Intersection(&pos))
		{
			//Cursor::SetMode(CA_Help);
			if( Control::Input->LBclicked() )
			{
				Hud->ShowBannerText("L3creepy");
			}
			return;
		}
		
		if( ProfileSystem::Instance->ActiveProfile.L3_S3_IsDoorToS4Opened && SceneRectFortFountain.Intersection(&pos) )
		{

#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
			//	Cursor::SetMode(CA_ExitUp,eArea_L3_S4_FortFountain);
#else
			Cursor::SetMode(CA_ExitUp,eArea_L3_S4_FortFountain);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L3_S4_FortFountain, 0, SceneRectFortFountain);
				}
#else
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S4_FortFountain;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
			return;
		}
		else if( !ProfileSystem::Instance->ActiveProfile.L3_S3_PZUnderwaterPuzzleSolved && SceneRectPuzzleUnderwater.Intersection(&pos) )
		{

#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
			//	Cursor::SetMode(CA_Look,eArea_L3_S3_PZUnderwater);
#else
			Cursor::SetMode(CA_Look,eArea_L3_S3_PZUnderwater);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L3_S3_PZUnderwater, 0, SceneRectPuzzleUnderwater);
				}
#else
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S3_PZUnderwater;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
			return;
		}
		else if( !ProfileSystem::Instance->ActiveProfile.L3_S3_InvCupTaken && PuzzleRectBlocks.Intersection(&pos) )
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
					GFHud->InvBoxCtrl->GoToScene(eArea_L3_S3_PZBlocks, 2, PuzzleRectBlocks);
				}
#else
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S3_PZBlocks;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}
			return;
		}
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
			//Control::Audio->PlayFootStepsFX();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S1_FortEntrance;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}	
	#endif
		if( IsMouseOverGoBackArea(&pos) )
		{

#ifdef TOUCH_UI
			//if(!GFHud->InvBoxCtrl->navTapped)
			//	Cursor::SetMode(CA_ExitDown,eArea_L3_S1_FortEntrance);
#else
			Cursor::SetMode(CA_ExitDown,eArea_L3_S1_FortEntrance);
#endif

			if( Control::Input->LBclicked() )
			{
#ifdef TOUCH_UI
				if(!GFHud->InvBoxCtrl->navTapped)
				{
					GFHud->InvBoxCtrl->GoToScene(eArea_L3_S1_FortEntrance, 2);
				}
#else
				//Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L3_S1_FortEntrance;
				SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

			}	
		}
	}
}

void L3_S3_Pool::ReceiveMessage(int val)
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
		if(isLampRisingFromWater)
		{
			return;
		}

		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )
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

void L3_S3_Pool::ProcessNonHOHint()
{
	if (GFInstance->IsReplayingGame)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S3Collected)
		{
			InitHintTrailEmit(835, 200);
		}
		else
		{
			Hud->teleportSystem->ShowTeleport();
		}
		return;
	}

	if(isLampRisingFromWater)
		return;

	int iNonHOHint = 0; 
	int MaxNonHOHints = 8;
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
					for (int i = 0; i < ePopL3S3_Max; i++)
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
					if( !ProfileSystem::Instance->ActiveProfile.L3_S3_PZBlocksPlaced )
					{
						if(ProfileSystem::Instance->ActiveProfile.L3_S2_M6_CollectedBoxIndex[0] > -1 && ProfileSystem::Instance->ActiveProfile.L3_S2_M6_CollectedBoxIndex[1] > -1)
						{
							InitHintTrailEmit(&SceneRectPuzzleUnderwater);
							return;
						}
					}
					else if( !ProfileSystem::Instance->ActiveProfile.L3_S3_PZUnderwaterPuzzleSolved )
					{
						//blocks placed
						InitHintTrailEmit(&SceneRectPuzzleUnderwater);
						return;
					}
					break;
			case 3: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L3_S3_InvCupTaken)
					{
						InitHintTrailEmit(&PuzzleRectBlocks);
						return;
					}
					break;
			case 4: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S4_FortFountain] && ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L3_S4_FortFountain])
					{
						InitHintTrailEmit(&SceneRectFortFountain,true,CA_ExitUp);
						return;
					}
					break;
			case 5: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L3_S1_FortEntrance])
					{
						InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
						return;
					}
					break;
			case 6: ++iNonHOHint;
				if (ProfileSystem::Instance->ActiveProfile.L3_S3_IsDoorToS4Opened && !ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L3_S4_FortFountain])
				{
					InitHintTrailEmit(&SceneRectFortFountain, true, CA_ExitUp);
					return;
				}
				break;
			case 7: ++iNonHOHint;
					{
						if(Hud->ShowHintForComboInvItems())
							return;
					}
					break;
			case 8: ++iNonHOHint;
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
void L3_S3_Pool::NavigateToScene()
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