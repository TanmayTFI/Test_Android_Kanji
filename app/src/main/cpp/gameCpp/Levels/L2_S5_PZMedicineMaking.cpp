//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 04 : Medicine Making Puzzle
//====================================

#include "L2_S5_PZMedicineMaking.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "TextureFactory.h"
#include "ParticleEffect.h"
#include "Rect.h"
#include "Polygon.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "Circle.h" 



//const CRectangle  MedicineTableRect(427.0f,239.0f,484.0f,227.0f);
//const CRectangle  StoveRect(880.0f,96.0f,185.0f,227.0f);
//const CRectangle  GrinderRect(214.0f,325.0f,141.0f,181.0f);		//218.0f,362.0f,183.0f,140.0f);
//const CRectangle  InvPotRect(205.0f,237.0f,176.0f,118.0f);
//const CRectangle  InvKindiRect(222.0f,108.0f,194.0f,139.0f);
const CRectangle  GrinderPestleRect(542.0f,543.0f,151.0f,67.0f);
const CRectangle  MortarPestleRect(1006.0f,434.0f,154.0f,63.0f);
//const CRectangle  MortarRect(916.0f,374.0f,149.0f,96.0f);
//const CRectangle  HoneyPotRect(353.0f,499.0f,117.0f,103.0f);
//const CRectangle  SpoonRect(747.0f,529.0f,215.0f,65.0f);
//const CRectangle  EmberRect(743.0f,68.0f,107.0f,102.0f);
//const CRectangle  MangoRect(466.0f,255.0f,169.0f,83.0f);
//const CRectangle  AloeveraRect(738.0f,247.0f,93.0f,72.0f);
//const CRectangle  TurmericRect(634.0f,308.0f,105.0f,65.0f);
//const CRectangle  HoneyRect(504.0f,383.0f,103.0f,51.0f);
const CRectangle  PopAreaRect(345.0f,90.0f,665.0f,501.0f);

enum ReturnObj
{
	eObj_Inactive = -1,
	eObj_GrinderPestle,
	eObj_MortarPestle,
	eObj_MangoCut,
	eObj_HoneyCut,
	eObj_AloeveraCut,
	eObj_TurmericCut,
	eObj_TurmericCrush,
	eObj_HoneyPot,
	eObj_EmptySpoon,
	eObj_SpoonWithMedicine
};

enum JAnims
{
	anim_AddTurmericToPot,
	anim_MangoCut,
	anim_HoneyCut,
	anim_AloveraCut,
	anim_WoodFire,
	anim_TurmericCut,
	anim_MedicineMix,
	anim_MedicinePour
};

enum L2_S5Pz_Popup
{
	ePopL2S5Pz_Chart = 0
};

L2_S5_PZMedicineMaking::L2_S5_PZMedicineMaking()
{
	ActivePopupID = ePopInactive;
	isChartPopupActivated = false;
	//ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineCreated = false;
	musicFadeOutTimer = 0;
	isAnimPlaying = false;
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV2\\S5\\POP2\\POP2.SCN");
	
	InitControlPolys();

	pop6Chart = new L2S5_Pop6Chart(0, Hud, HOScene, eZOrder_Popup, ChartPoly->center, CC_CALLBACK_1(L2_S5_PZMedicineMaking::OnPopupClosed, this));
	pop6Chart->SetPopupBoundingBox(PopAreaRect);

	InitVariables();
	SpoonWithMedicine	  = HOScene->GetObjectByID("SpoonWithMedicine");
	PotMask				  = HOScene->GetObjectByID("PotMask");
	HoneyInGrinder 		  = HOScene->GetObjectByID("HoneyInGrinder");
	MangoInGrinder		  = HOScene->GetObjectByID("MangoInGrinder");		  
	AloeVeraCut 		  = HOScene->GetObjectByID("AloeVeraCut");
	AloeVeraFull		  = HOScene->GetObjectByID("AloeVeraFull");
	AloeVeraHalf		  = HOScene->GetObjectByID("AloeVeraHalf");
	ChirattaEmber		  = HOScene->GetObjectByID("ChirattaEmber");
	CrusherInHand		  = HOScene->GetObjectByID("CrusherInHand");
	CrusherOnGround		  = HOScene->GetObjectByID("CrusherOnGround");
	Grinder				  = HOScene->GetObjectByID("Grinder");
	PestleInHand		  = HOScene->GetObjectByID("PestleInHand");
	PestleOnGround		  = HOScene->GetObjectByID("PestleOnGround");
	HoneyDrip1			  = HOScene->GetObjectByID("HoneyDrip1");
	HoneyDrip2			  = HOScene->GetObjectByID("HoneyDrip2");
	HoneyDrip3			  = HOScene->GetObjectByID("HoneyDrip3");
	HoneyMangoInGrinder0  = HOScene->GetObjectByID("HoneyMangoInGrinder0");
	HoneyMangoInGrinder1  = HOScene->GetObjectByID("HoneyMangoInGrinder1");
	HoneyMangoInGrinder2  = HOScene->GetObjectByID("HoneyMangoInGrinder2");
	HoneyPotDrop		  = HOScene->GetObjectByID("HoneyPotDrop");
	HoneyPotEmpty		  = HOScene->GetObjectByID("HoneyPotEmpty");
	HoneyPotFilled		  = HOScene->GetObjectByID("HoneyPotFilled");
	HoneyPotInHand		  = HOScene->GetObjectByID("HoneyPotInHand");
	HoneyWaxCut			  = HOScene->GetObjectByID("HoneyWaxCut");
	HoneyWaxFull		  = HOScene->GetObjectByID("HoneyWaxFull");
	MangoCut			  = HOScene->GetObjectByID("MangoCut");
	MangoFull			  = HOScene->GetObjectByID("MangoFull");
	MangoHalfCut		  = HOScene->GetObjectByID("MangoHalfCut");
	PotEmpty			  = HOScene->GetObjectByID("PotEmpty");
	SpoonPour			  = HOScene->GetObjectByID("SpoonPour");
	SpoonOnGround		  = HOScene->GetObjectByID("SpoonOnGround");
	SpoonInHand			  = HOScene->GetObjectByID("SpoonInHand");
	MortarEmpty			  = HOScene->GetObjectByID("MortarEmpty");
	Mortar1				  = HOScene->GetObjectByID("Mortar1");
	Mortar2				  = HOScene->GetObjectByID("Mortar2");
	Mortar3				  = HOScene->GetObjectByID("Mortar3");
	Mortar4				  = HOScene->GetObjectByID("Mortar4");
	TurmericCut 		  = HOScene->GetObjectByID("TurmericCut");
	TurmericFull		  = HOScene->GetObjectByID("TurmericFull");
	TurmericHalfCut		  = HOScene->GetObjectByID("TurmericHalfCut");
	MortarDrop			  = HOScene->GetObjectByID("MortarDrop");
	TurmericPice		  = HOScene->GetObjectByID("TurmericPice");
	WoodLog				  = HOScene->GetObjectByID("WoodLog");
	HangingRoot1		  = HOScene->GetObjectByID("HangingRoot1");
	HangingRoot2		  = HOScene->GetObjectByID("HangingRoot2");
	knife				  = HOScene->GetObjectByID("knife");
	PotFilled			  = HOScene->GetObjectByID("PotIFilled");
	MortarInHand		  = HOScene->GetObjectByID("MortarInHand");
	WindowLight			  = HOScene->GetObjectByID("WindowLight");
	Kindi				  = HOScene->GetObjectByID("Kindi");
	WoodFireAnim		  = HOScene->GetObjectByID("woodFire");
	TorchAnim			  = HOScene->GetObjectByID("torch");
	HoneyPotInHandHalf	  = HOScene->GetObjectByID("HoneyPotInHandHalf");
	HoneyPotInHandEmpty	  = HOScene->GetObjectByID("HoneyPotInHandEmpty");
	HoneyPourMask		  = HOScene->GetObjectByID("HoneyPourMask");
	AloeLeaf1			  = HOScene->GetObjectByID("Leaf1");
	AloeLeaf2			  = HOScene->GetObjectByID("Leaf2");
	AloeLeaf3			  = HOScene->GetObjectByID("Leaf3");
	AloeLeaf4			  = HOScene->GetObjectByID("Leaf4");
	AloeLeaf5			  = HOScene->GetObjectByID("Leaf5");
	AloeLeaf6			  = HOScene->GetObjectByID("Leaf6");
	MortarAnim1			  = HOScene->GetObjectByID("MortarAnim1");
	MortarAnim2			  = HOScene->GetObjectByID("MortarAnim2");
	MedicineMask		  = HOScene->GetObjectByID("MedicineMask");
	SpoonStir			  = HOScene->GetObjectByID("SpoonStir");
	KindiMask			  = HOScene->GetObjectByID("KindiMask");

	SetSceneElements();
	InitAnimations();

	if( ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired )
	{
		S5WaterBoilSteamFX->Start();
		S5WaterBoilSteamFX->AdvanceByTime(2.0f);
	}

	
	//to check if puzzle is active..
	if (ProfileSystem::Instance->ActiveProfile.L2_S5_PuzzleModeActive )
	{
		//Hud->InvBox_StartPuzzleMode();
		isPuzzleModeActive = true;
	}

	
	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L2_S4_Cave]  = true;

	TaskPendingUpdate();

	Control::Audio->LoadSample(aAmbs5MedicinePuzzle,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbs5MedicinePuzzle) )		
	{
		Control::Audio->PlaySample(aAmbs5MedicinePuzzle,true);
	}
}

void L2_S5_PZMedicineMaking::InitControlPolys()
{
	int InvKindiArray[] = {268,45,238,88,220,127,211,188,251,257,315,293,378,287,418,250,444,191,436,111,353,42};
	InvKIndiPoly =  new CPolygon(11);
	InvKIndiPoly->Init(InvKindiArray);
	int ChartArray[] = {464,100,450,148,456,186,484,247,567,233,719,197,721,147,627,61};
	ChartPoly =  new CPolygon(8);
	ChartPoly->Init(ChartArray);
	int MedicineTableArray[] = {370,276,472,495,508,547,582,538,580,471,827,416,866,476,923,457,775,176,725,198,726,212,475,261,451,246};
	MedicineTablePoly =  new CPolygon(13);
	MedicineTablePoly->Init(MedicineTableArray);
	int GrinderArray[] = {417,430,413,483,370,499,341,536,327,586,219,580,175,462,191,340,219,274,349,299,407,362};
	GrinderPoly =  new CPolygon(11);
	GrinderPoly->Init(GrinderArray);
	int StoveArray[] = {1071,243,1079,292,932,308,867,261,859,150,909,31,1070,47,1095,140};
	StovePoly =  new CPolygon(8);
	StovePoly->Init(StoveArray);
	int MortarArray[] = {905,349,905,395,928,452,977,490,1042,454,1090,415,1145,353,1133,302,1080,287,1011,314,928,315};
	MortarPoly =  new CPolygon(11);
	MortarPoly->Init(MortarArray);
	int CrusherArray[] = {1147,374,1168,415,1168,487,1123,530,1054,541,1002,519,1002,485,1044,454,1093,432};
	CrusherPoly =  new CPolygon(9);
	CrusherPoly->Init(CrusherArray);
	int PestleArray[] = {553,525,537,562,532,602,572,626,701,601,716,537,655,512};
	PestlePoly =  new CPolygon(7);
	PestlePoly->Init(PestleArray);
	int SpoonArray[] = {741,527,736,578,951,598,966,553,918,508};
	SpoonPoly=  new CPolygon(5);
	SpoonPoly->Init(SpoonArray);
	int MangoArray[] = {457,266,446,296,462,326,501,348,520,355,569,351,610,335,642,300,653,252,624,234,546,242};
	MangoPoly =  new CPolygon(11);
	MangoPoly->Init(MangoArray);
	int HoneyArray[] = {479,367,512,363,547,359,584,360,616,390,629,428,601,447,534,447,493,437,476,399};
	HoneyPoly =  new CPolygon(10);
	HoneyPoly->Init(HoneyArray);
	int TurmericArray[] = {610,351,618,382,636,415,670,418,736,414,769,392,780,353,767,327,710,297,673,285,651,298,631,321};
	TurmericPoly =  new CPolygon(12);
	TurmericPoly->Init(TurmericArray);
	int AloveraArray[] = {725,216,718,246,715,279,721,295,763,318,804,344,834,343,853,311,825,253,808,218,787,199,744,190};
	AloveraPoly =  new CPolygon(12);
	AloveraPoly->Init(AloveraArray);
	int HoneyPotArray[] = {371,508,415,497,468,490,496,526,496,573,467,609,411,632,357,612,324,579};
	HoneyPotPoly=  new CPolygon(9);
	HoneyPotPoly->Init(HoneyPotArray);
	int ArrayPts[] = {897,126,859,164,873,272,898,357,975,363,1033,359,1105,325,1118,244,1055,125,952,93};
	FireWoodDestPoly =  new CPolygon(10);
	FireWoodDestPoly->Init(ArrayPts);


}


//void L2_S5_PZMedicineMaking::OnPopAreaClicked(int popId)
//{
//	switch (popId)
//	{
//	case 0:	pop6Chart->OpenPopup();
//		break;
//	default:
//		break;
//	}
//}

void L2_S5_PZMedicineMaking::OnPopupClosed(Popup * popup)
{
	ActivePopupID = ePopInactive;
}

void L2_S5_PZMedicineMaking::Required()
{
	HiddenObjectSystem::Required();	//forced update
	
	if( Control::Audio->IsPlaying(aTrackL2theme) )
	{
		Control::Audio->Samples[aTrackL2theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL2theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL2theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL2puzzle) )
				  Control::Audio->PlaySample(aTrackL2puzzle, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL2puzzle))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL2puzzle, true);
		}
	}

}

void L2_S5_PZMedicineMaking::Update()
{
	NavigateToScene();
	#ifdef _PARTICLEEDITORENABLED
		if (_isParticleEditorActive)
		{
			return;
		}
	#endif

	if( Hud->PopupState != Popup_Inactive || Hud->IsMapShowing || Hud->IsDiaryShowing)
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	CPoint pos = Control::Input->Mpos();
	
	if( ActivePopupID != ePopInactive )
		return;


	Cursor::SetMode(CA_Normal);
	
	if( isAnimPlaying )
		Hud->AllowInput = false;

	if( isGrinderAnimPlaying )
	{
		if( CAnim_HoneyMangoGrind->IsAnimFinished() )
		{
			isGrinderAnimPlaying = false;
			isAnimPlaying = false;
			Hud->AllowInput = true;
			CAnim_HoneyMangoGrind->SetEnableObjects(false);
			CAnim_HoneyMangoDrip->SetEnableObjects(false);
			HoneyMangoInGrinder2->SetEnable(true);
			HoneyMangoInGrinder2->FadeOut(2.0f);
			HoneyDrip3->SetEnable(true);
			HoneyDrip3->FadeOut(2.0f);
			HoneyPotFilled->SetEnable(true);
			HoneyPotFilled->SetAlpha(0.0f);
			HoneyPotFilled->FadeIn(2.0f);
		}
		return;
	}
	else if( isAddHoneyToPotAnimPlaying )
	{
		if(	CAnim_AddHoneyToPot->IsAnimFinished() )
		{
			isAddHoneyToPotAnimPlaying= false;
			isAnimPlaying = false;
			Hud->AllowInput = true;
			CAnim_AddHoneyToPot->SetEnableObjects(false);
		}
		return;
	}
	else if( isAloeveraDropAnimPlaying )
	{
		if(	CAnim_AloeveraDrop->IsAnimFinished() )
		{
			isAloeveraDropAnimPlaying = false;
			isAnimPlaying = false;
			Hud->AllowInput = true;
			CAnim_AloeveraDrop->SetEnableObjects(false);
		}
		return;
	}
	else if( isTurmericCrushAnimPlaying )
	{
		if( CAnim_TurmericCrush->IsAnimFinished() ) 
		{
			isTurmericCrushAnimPlaying = false;
			isAnimPlaying = false;
			Hud->AllowInput = true;
			CrusherInHand->SetZ(CrusherInHand->ZOrder - eZOrder_INVBOXITEMS );
			CAnim_TurmericCrush->SetEnableObjects(false);
			Mortar4->SetEnable(true);
		}
		return;
	}
	else if( isAnimPlaying )
		return;


	//const CRectangle  TmpRect(402.0f,623.0f,562.0f,144.0f);

	if( isAloeveraInHand || isTurmericInHand || isMangoInHand || isHoneyInHand || isMortarInHand || isMortarPestleInHand || isGrinderPestleInHand || isHoneyPotInHand ||
		isSpoonInHand || isSpoonWithMedicineInHand )
	{
		if( pos.y >= ( HoveredBackBtnRectMax.topleft.y + 60 ) )
		{
			if(isAloeveraInHand)
			{
				isAloeveraInHand = false;
				returnObject = AloeVeraCut;
				objectID = eObj_AloeveraCut;
			}
			else if(isTurmericInHand)
			{
				isTurmericInHand = false;
				returnObject = TurmericCut;
				objectID = eObj_TurmericCut;
			}
			else if(isMangoInHand)
			{
				isMangoInHand = false;
				returnObject = MangoCut;
				objectID = eObj_MangoCut;
			}
			else if(isHoneyInHand)
			{
				isHoneyInHand = false;
				returnObject = HoneyWaxCut;
				objectID = eObj_HoneyCut;
			}
			else if(isMortarInHand)
			{
				isMortarInHand = false;
				returnObject = MortarInHand;
				objectID = eObj_TurmericCrush;
			}
			else if(isMortarPestleInHand)
			{
				isMortarPestleInHand = false;
				returnObject = CrusherInHand;
				objectID = eObj_MortarPestle;
			}
			else if(isGrinderPestleInHand)
			{
				isGrinderPestleInHand = false;
				returnObject = PestleInHand;
				objectID = eObj_GrinderPestle;
			}
			else if(isHoneyPotInHand)
			{
				isHoneyPotInHand = false;
				returnObject = HoneyPotInHand;
				objectID = eObj_HoneyPot;
			}
			else if(isSpoonInHand)
			{
				isSpoonInHand = false;
				returnObject = SpoonInHand;
				objectID = eObj_EmptySpoon;
			}
			else if(isSpoonWithMedicineInHand)
			{
				isSpoonWithMedicineInHand = false;
				returnObject = SpoonWithMedicine;
				objectID = eObj_SpoonWithMedicine;
			}
			return;
		}
	}


	if( isSpoonWithMedicineInHand )
	{
		SpoonWithMedicine->SetPos(&pos);
		if( Control::Input->LBclicked() )
		{
			isSpoonWithMedicineInHand = false;
			if (InvKIndiPoly->Intersection(&pos ))
			{
				Control::Audio->QuickLoadAndPlaySFX("s5pz1_spoondropinkindi");
				PotMask->SetEnable(true);
				JAnim_MedicinePour->PlayAnim();
				isAnimPlaying = true;
				ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineCreated = true; 

				//Show Objective completed
				if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L2O2_PrepareMedicine] && !ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L2O2_PrepareMedicine] )
				{
					ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L2O2_PrepareMedicine] = true;
					Hud->ShowObjectiveFX(eObj_L2O2_PrepareMedicine,true);
				}

				return; 
			}
			returnObject = SpoonWithMedicine;
			objectID = eObj_SpoonWithMedicine;
		}
		return; 
	}
	if( !isSpoonWithMedicineInHand && StovePoly->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineCreated && 
		ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineMixed && returnObject != SpoonWithMedicine )
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked() )
		{
			isSpoonWithMedicineInHand = true;
			SpoonStir->SetEnable(false);
			SpoonWithMedicine->SetEnable(true);
			SpoonWithMedicine->SetZ(SpoonWithMedicine->ZOrder + eZOrder_INVBOXITEMS );
		}
		return;
	}
	if( isAloeveraInHand )
	{
		AloeVeraCut->SetPos( &pos );
		if( Control::Input->LBclicked())
		{
			isAloeveraInHand = false;
			if( StovePoly->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced )
			{
				Control::Audio->QuickLoadAndPlaySFX("s5pz1_aloeveradropinpot");
				CAnim_AloeveraDrop->PlayAnim();
				isAnimPlaying = true;
				CAnim_AloeveraDrop->SetFadeOut(2.0f) ;
				isAloeveraDropAnimPlaying = true;
				ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraApplied = true;
				AloeVeraCut->SetEnable(false);
				return;
			}
			returnObject = AloeVeraCut;
			objectID = eObj_AloeveraCut;
		}
		return;
	}
	if( isHoneyInHand )
	{
		HoneyWaxCut->SetPos( &pos );
		if( Control::Input->LBclicked() )
		{
			isHoneyInHand = false;
			if (GrinderPoly->Intersection(&pos))
			{
				Control::Audio->QuickLoadAndPlaySFX("s5pz1_honeycombplacedinmortar");
				HoneyInGrinder->SetEnable(true);
				HoneyWaxCut->SetZ( HoneyWaxCut->ZOrder - eZOrder_INVBOXITEMS );
				HoneyWaxCut->SetEnable(false); 
				ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxAddedToPot = true;
				if(	!ProfileSystem::Instance->ActiveProfile.L2_S5_MangoAddedToPot )
					Hud->ShowBannerText("L2add");
				return;
			}
			else if(StovePoly->Intersection(&pos))
				Hud->ShowBannerText("L2recipe");
			else
				GFInstance->WrongInvDropMsg(Hud);
			returnObject = HoneyWaxCut;
			objectID = eObj_HoneyCut;
		}
		return;
	}
	if( isMangoInHand )
	{
		MangoCut->SetPos( &pos );
		if( Control::Input->LBclicked() )
		{
			isMangoInHand = false;
			if (GrinderPoly->Intersection(&pos))
			{
				Control::Audio->QuickLoadAndPlaySFX("s5pz1_honeycombplacedinmortar");
				MangoInGrinder->SetEnable(true);
				ProfileSystem::Instance->ActiveProfile.L2_S5_MangoAddedToPot = true;
				MangoCut->SetEnable(false);
				if(	!ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxAddedToPot )
					Hud->ShowBannerText("L2add");
				return; 
			}
			else if(StovePoly->Intersection(&pos))
				Hud->ShowBannerText("L2recipe");
			else
				GFInstance->WrongInvDropMsg(Hud);
		returnObject = MangoCut;
		objectID = eObj_MangoCut;
		}
		return;
	}
	if( isTurmericInHand )
	{
		TurmericCut->SetPos( &pos );
		if( Control::Input->LBclicked() )
		{
			isTurmericInHand = false;
			if (MortarPoly->Intersection(&pos))
			{
				Control::Audio->QuickLoadAndPlaySFX("s5pz1_turmericpiecesplaced");
				ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericAddedToPot = true;
				TurmericCut->SetEnable(false);
				MortarEmpty->SetEnable(false);
				Mortar1->SetEnable(true); 
				return;
			}
			else if(StovePoly->Intersection(&pos))
				Hud->ShowBannerText("L2recipe");
			else
				GFInstance->WrongInvDropMsg(Hud);
			returnObject = TurmericCut;
			objectID = eObj_TurmericCut;
		}
		return;
	}
	if(isMortarPestleInHand)
	{
		CrusherInHand->SetPos( &pos );
		if( Control::Input->LBclicked() )
		{
			isMortarPestleInHand = false;
			if (MortarPoly->Intersection(&pos)  && ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericAddedToPot  && !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderCreated )
			{
				Control::Audio->QuickLoadAndPlaySFX("s5pz1_turmericcrused");
				ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderCreated = true;
				CAnim_TurmericCrush->PlayAnim();
				isAnimPlaying = true;
				isTurmericCrushAnimPlaying = true; 
				return;
			}
			returnObject = CrusherInHand;
			objectID = eObj_MortarPestle;
		}
		return;
	}
	if(isMortarInHand)
	{
		MortarInHand->SetPos( &pos );
		if( Control::Input->LBclicked() )
		{
			isMortarInHand = false;
			if (StovePoly->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderApplied  && ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderCreated
				&&  ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced )
			{
				Control::Audio->QuickLoadAndPlaySFX("s5pz1_turmericpowderdropinpot");
				ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderApplied = true;
				MortarInHand->SetZ(MortarInHand->ZOrder - eZOrder_INVBOXITEMS );
				JAnim_TurmericAddToPot->PlayAnim();
				isAnimPlaying = true;
				isTurmericAddToPotPlaying = true; 
				return;
			}
			returnObject = MortarInHand;
			objectID = eObj_TurmericCrush;
		}
		return;
	}
	if( isGrinderPestleInHand )
	{
		PestleInHand->SetPos( &pos );
		if( Control::Input->LBclicked()  )
		{
			isGrinderPestleInHand = false;
			if( GrinderPoly->Intersection(&pos )  && ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxAddedToPot && ProfileSystem::Instance->ActiveProfile.L2_S5_MangoAddedToPot && 
				!ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidCreated )
			{
				Control::Audio->QuickLoadAndPlaySFX("s5pz1_honeycombcrushed");
				CAnim_HoneyMangoGrind->PlayAnim();
				CAnim_HoneyMangoDrip->PlayAnim();
				isAnimPlaying = true;
				isGrinderAnimPlaying = true;
				MangoInGrinder->SetEnable(false);
				HoneyInGrinder->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidCreated = true; 
				return;
			}
			returnObject = PestleInHand;
			objectID = eObj_GrinderPestle;
		}
		return;
	}
	if( isSpoonInHand )
	{
		SpoonInHand->SetPos(&pos);
		if( Control::Input->LBclicked() )
		{
			isSpoonInHand = false;
			if (StovePoly->Intersection(&pos ) && !ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineMixed
				&&ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidApplied && ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraApplied 
				&& ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderApplied && ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced 
				&& ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired)
			{
				Control::Audio->QuickLoadAndPlaySFX("s5pz1_spoondip");
				SpoonInHand->SetZ( SpoonInHand->ZOrder - eZOrder_INVBOXITEMS );
				JAnim_MedicineMix->PlayAnim();
				SpoonInHand->SetEnable(false);
				isAnimPlaying = true;
				ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineMixed = true;
				Hud->ShowBannerText("L2amount");
				return; 
			}
			else if( StovePoly->Intersection(&pos ) && ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced)
				Hud->ShowBannerText("L2mix");
			returnObject = SpoonInHand;
			objectID = eObj_EmptySpoon;
		}
		return; 
	}
	if( isHoneyPotInHand )
	{
		HoneyPotInHand->SetPos(&pos);
		if( Control::Input->LBclicked() )
		{
			isHoneyPotInHand = false;
			if (StovePoly->Intersection(&pos ) && ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced && !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidApplied 
				&& ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidCreated)
			{
				Control::Audio->QuickLoadAndPlaySFX("s5pz1_honeycombdropinpot");
				CAnim_AddHoneyToPot->PlayAnim();
				isAnimPlaying = true;
				isAddHoneyToPotAnimPlaying = true;
				ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidApplied = true; 
				return; 
			}
			returnObject = HoneyPotInHand;
			objectID = eObj_HoneyPot;
		}
		return; 
	}
	
	

	if( !isObjectReturning && !isAloeveraInHand && !isSpoonWithMedicineInHand && !isHoneyPotInHand && !isSpoonInHand && !isGrinderPestleInHand && !isMortarInHand
		&& !isMortarPestleInHand && !isTurmericInHand && !isMangoInHand && !isHoneyInHand && objectID == eObj_Inactive)
	{
		if( ChartPoly->Intersection( &pos ))
		{
			Cursor::SetMode(CA_Look);
			if (Control::Input->LBclicked())
			{
				//Control::Audio->QuickLoadAndPlaySFX("s5pz1_instructionpopupopen");
				if( !isChartPopupActivated )
					Hud->ShowBannerText("L2medicine");
				isChartPopupActivated = true;
				Cursor::SetMode(CA_Normal);
				ActivePopupID = ePopL2S5Pz_Chart;
				pop6Chart->OpenPopup(); 
			}
		}
		if( AloveraPoly->Intersection( &pos ) && ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraSliced && !ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraApplied && !isAloeveraCutAnimPlaying )
		{
			Cursor::SetMode(CA_HandTake );
			if( Control::Input->LBclicked() )
			{
				AloeVeraCut->SetZ(AloeVeraCut->ZOrder + eZOrder_INVBOXITEMS );
				isAloeveraInHand = true;
			}
			return;
		}
		if( MangoPoly->Intersection( &pos ) && ProfileSystem::Instance->ActiveProfile.L2_S5_MangoSliced && !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoAddedToPot && !isMangoCutAnimPlaying )
		{
			Cursor::SetMode(CA_HandTake );
			if( Control::Input->LBclicked() )
			{
				MangoCut->SetZ(MangoCut->ZOrder + eZOrder_INVBOXITEMS );
				isMangoInHand = true;
			}
			return;
		}
		if( TurmericPoly->Intersection( &pos ) && ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericSliced && !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericAddedToPot && !isTurmericCutAnimPlaying)
		{
			Cursor::SetMode(CA_HandTake );
			if( Control::Input->LBclicked() )
			{
				TurmericCut->SetZ( TurmericCut->ZOrder + eZOrder_INVBOXITEMS );
				isTurmericInHand = true;
			}
			return;
		}
		if( HoneyPoly->Intersection( &pos ) && ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxSliced && !ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxAddedToPot && !isHoneyCutAnimPlaying )
		{
			Cursor::SetMode(CA_HandTake );
			if( Control::Input->LBclicked() )
			{
				HoneyWaxCut->SetZ( HoneyWaxCut->ZOrder + eZOrder_INVBOXITEMS );
				isHoneyInHand = true;
			}
			return;
		}
		if( MortarPoly->Intersection( &pos ) && ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderCreated && !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderApplied && !isTurmericAddToPotPlaying )
		{
			Cursor::SetMode(CA_HandTake );
			if( Control::Input->LBclicked() )
			{
				isMortarInHand = true;
				MortarInHand->SetZ( MortarInHand->ZOrder + eZOrder_INVBOXITEMS );
				MortarInHand->Pos = Mortar4->Pos;
				MortarEmpty->SetEnable(false);
				Mortar4->SetEnable(false);
				MortarInHand->SetEnable(true);
			}
			return;
		}
		if( MortarPestleRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderCreated )
		{
			Cursor::SetMode(CA_HandTake);
			if( Control::Input->LBclicked() )
			{
				isMortarPestleInHand = true;
				CrusherInHand->SetZ( CrusherInHand->ZOrder + eZOrder_INVBOXITEMS );
				CrusherInHand->Pos = CrusherOnGround->Pos;
				CrusherOnGround->SetEnable(false);
				CrusherInHand->SetEnable(true);
			}
			return;
		}
		if( GrinderPestleRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidCreated  )
		{
			Cursor::SetMode(CA_HandTake);
			if( Control::Input->LBclicked() )
			{
				PestleInHand->SetZ( PestleInHand->ZOrder + eZOrder_INVBOXITEMS );
				isGrinderPestleInHand = true;
				PestleInHand->Pos = PestleOnGround->Pos;
				PestleOnGround->SetEnable(false);
				PestleInHand->SetEnable(true);
			}
			return;
		}
		if( SpoonPoly->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineMixed)
		{
			Cursor::SetMode(CA_HandTake);
			if( Control::Input->LBclicked() )
			{
				isSpoonInHand = true;
				SpoonOnGround->SetEnable(false);
				SpoonInHand->Pos = SpoonOnGround->Pos;
				SpoonInHand->SetZ( SpoonInHand->ZOrder + eZOrder_INVBOXITEMS );
				SpoonInHand->SetEnable(true);
			}
			return;
		}
		if( HoneyPotPoly->Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidCreated && !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidApplied )
		{
			Cursor::SetMode(CA_HandTake);
			if( Control::Input->LBclicked() )
			{
				isHoneyPotInHand = true;
				HoneyPotEmpty->SetEnable(false);
				HoneyPotFilled->SetEnable(false);
				HoneyPotInHand->SetZ( HoneyPotInHand->ZOrder + eZOrder_INVBOXITEMS );
				HoneyPotInHand->SetEnable(true);
			}
			return;
		}
	}
	if( objectID != eObj_Inactive && !isObjectReturning )
	{
		isObjectReturning = true;
		returnTimer = 0.0f;
		startPos = returnObject->Pos;
		endPos = returnObjPos[objectID];
		
		float distance = (float)sqrt( ((startPos.x - endPos.x)*(startPos.x - endPos.x)) + ((startPos.y - endPos.y) * (startPos.y - endPos.y)));
		returnEndTimer = distance / 1000;
	}

	if( InvKIndiPoly->Intersection(&pos))
	{
		if(ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineCreated && !ProfileSystem::Instance->ActiveProfile.L2_S5_InvMedicineTaken  )
		{
			Cursor::SetMode(CA_HandTake);
			if( Control::Input->LBclicked() )
			{
					//Hud->InvBox_StopPuzzleMode();
				Kindi->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L2_S5_InvMedicineTaken = true;
				Hud->TakeInventoryItem(eInv_L2S5_MedicineJar, Control::Input->Mpos());
				Hud->ShowBannerText("L2better");
				TaskPendingUpdate();
			}
		}
		else if(  !ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineCreated  )
		{
			Cursor::SetMode(CA_Gear);
			if( Control::Input->LBclicked() )
			{
				Kindi->Shake(0,2,0,0.5f);
				Hud->ShowBannerText("L2jar");
			}
		}
		return;
	}	
	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
	{
		//Control::Audio->PlayFootStepsFX();
		//Hud->InvBox_StopPuzzleMode();
		ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S5_MoopansDen;
		SendMessage(ControlExit, ID, HOSM_NewArea);						
	}
	#endif

	if( IsMouseOverGoBackArea(&pos) )
	{

#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitDown,eArea_L2_S5_MoopansDen);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L2_S5_MoopansDen);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L2_S5_MoopansDen, 2);
			}
#else
			//Control::Audio->PlayFootStepsFX();
			//Hud->InvBox_StopPuzzleMode();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S5_MoopansDen;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}	
	}
	UpdateBannerMessages(pos);
	if(isObjectReturning)
	{
		returnTimer += Control::LogicRate;

		CPoint temp;
		
		temp.x = startPos.x + (endPos.x - startPos.x) * easing(returnTimer, 0, 1, returnEndTimer);
		temp.y = startPos.y + (endPos.y - startPos.y) * easing(returnTimer, 0, 1, returnEndTimer);

		returnObject->Pos = temp;
		if(returnTimer >= returnEndTimer)
		{
			returnObject->SetZ( returnObject->ZOrder - eZOrder_INVBOXITEMS );
			returnObject->SetEnable(false);
			returnObject = NULL;
			isObjectReturning = false;
			returnTimer = 0.0f;

			switch (objectID)
			{
			case eObj_GrinderPestle:	PestleOnGround->SetEnable(true);
				GFInstance->WrongInvDropMsg(Hud);
				break;
			case eObj_MortarPestle:		CrusherOnGround->SetEnable(true);
				GFInstance->WrongInvDropMsg(Hud);
				break;
			case eObj_HoneyCut:			HoneyWaxCut->SetEnable(true);
				//GFInstance->WrongInvDropMsg(Hud);
				break;
			case eObj_HoneyPot:			HoneyPotFilled->SetEnable(true);
				Hud->ShowBannerText("L1follow");
				break;
			case eObj_AloeveraCut:		AloeVeraCut->SetEnable(true);
				GFInstance->WrongInvDropMsg(Hud);
				break;
			case eObj_EmptySpoon:		SpoonOnGround->SetEnable(true);
				GFInstance->WrongInvDropMsg(Hud);
				break;
			case eObj_SpoonWithMedicine:SpoonStir->SetEnable(true);
				SpoonStir->Show(true);
				SpoonStir->SetAlpha(1.0f);
				Hud->ShowBannerText("L2kind");
				break;
			case eObj_TurmericCut:		TurmericCut->SetEnable(true);
				//GFInstance->WrongInvDropMsg(Hud);
				break;
			case eObj_MangoCut:			MangoCut->SetEnable(true);
				//GFInstance->WrongInvDropMsg(Hud);
				break;
			case eObj_TurmericCrush:	Mortar4->SetEnable(true);
				Hud->ShowBannerText("L1follow");
				break;
			default:
				break;
			}
			objectID = eObj_Inactive;
		}
		return;
	}
}

void L2_S5_PZMedicineMaking::UpdateBannerMessages( const CPoint pos )
{
	

	if( !isAloeveraInHand && !isTurmericInHand && !isMangoInHand && !isHoneyInHand && !isMortarInHand && !isMortarPestleInHand && !isGrinderPestleInHand && !isHoneyPotInHand 
		&& !isSpoonInHand && !isSpoonWithMedicineInHand )
	{
		if( StovePoly->Intersection( &pos ))
		{
			Cursor::SetMode(CA_Gear);
			if (Control::Input->LBclicked())
			{
				if( !ProfileSystem::Instance->ActiveProfile.L2_S5_WoodPlaced )
				{
					Hud->ShowBannerText("L2wood");
					if(ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced )
						PotFilled->Shake(0,2,0,0.5f);
				}
				else if(ProfileSystem::Instance->ActiveProfile.L2_S5_WoodPlaced && !ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired)
				{
					Hud->ShowBannerText("L2heat");
					if(ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced)
						PotFilled->Shake(0,2,0,0.5f);
				}
				else if(!ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced)
				{
					Hud->ShowBannerText("L2cook");
				}
				else if( ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced && ( !ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraApplied &&
					!ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderApplied && !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidApplied))
				{
					PotFilled->Shake(0,2,0,0.5f);
					Hud->ShowBannerText("L2all");
				}
				else if( ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced && ( !ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraApplied || 
					!ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderApplied || !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidApplied))
				{
					PotFilled->Shake(0,2,0,0.5f);
					Hud->ShowBannerText("L2add");
				}
				else if( ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraApplied && ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired
					 && ( !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderApplied || !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidApplied))
				{
					PotFilled->Shake(0,2,0,0.5f);
					Hud->ShowBannerText("L2still");
				}
				else if( ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderApplied && ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired
					 && ( !ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraApplied || !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidApplied))
				{
					PotFilled->Shake(0,2,0,0.5f);
					Hud->ShowBannerText("L2still");
				}
				else if( ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidApplied && ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired
					 && ( !ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraApplied || !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderApplied))
				{
					PotFilled->Shake(0,2,0,0.5f);
					Hud->ShowBannerText("L2still");
				}
				else if(ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraApplied && ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderApplied 
				&& ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidApplied &&  ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced
				&& ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired && !ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineMixed)
				{
					PotFilled->Shake(0,2,0,0.5f);
					Hud->ShowBannerText("L2flavor");
				}
				else if( ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineCreated && !ProfileSystem::Instance->ActiveProfile.L2_S5_InvMedicineTaken)
				{
					PotFilled->Shake(0,2,0,0.5f);
					PotMask->Shake(0,2,0,0.5f);
					MedicineMask->Shake(0,2,0,0.5f);
					Hud->ShowBannerText("L2anymore");
				}

			}
		}
		else if( GrinderPoly->Intersection( &pos ))
		{
			Cursor::SetMode(CA_Gear);
			if (Control::Input->LBclicked())
			{
				if( ! ProfileSystem::Instance->ActiveProfile.L2_S5_MangoAddedToPot && !ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxAddedToPot )
					Hud->ShowBannerText("L2juice");
				else if( ProfileSystem::Instance->ActiveProfile.L2_S5_MangoAddedToPot && !ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxAddedToPot )
					Hud->ShowBannerText("L2add");
				else if( !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoAddedToPot && ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxAddedToPot)
					Hud->ShowBannerText("L2add");
				if( ProfileSystem::Instance->ActiveProfile.L2_S5_MangoAddedToPot && ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxAddedToPot 
						&& !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidCreated )
					Hud->ShowBannerText("L2extract");
			}
		}
		else if( MortarPoly->Intersection( &pos ))
		{
			Cursor::SetMode(CA_Gear);
			if (Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L2pestle");
				if( ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPlaced && !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderCreated )
					Mortar1->Shake(0,2,0,0.5f);
			}
		}
		else if( HoneyPotPoly->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidCreated )
		{
			if( Control::Input->LBclicked() )
			{
				if( ProfileSystem::Instance->ActiveProfile.L2_S5_MangoAddedToPot && ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxAddedToPot)
					Hud->ShowBannerText("L2extract");
				else if( !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoAddedToPot && !ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxAddedToPot )
					Hud->ShowBannerText("L2should");
				else 
					Hud->ShowBannerText("L2add");
				HoneyPotEmpty->Shake(0,2,0,0.5f);
			}
		}
		else if( !isPuzzleModeActive)
		{
			/*if(( (AloveraPoly->Intersection( &pos )  &&  ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraPlaced && !ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraSliced)  ||
				( HoneyPoly->Intersection( &pos )  && ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxPlaced && !ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxSliced) ||
				( TurmericPoly->Intersection( &pos )  && ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPlaced && !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericSliced) || 
				(MangoPoly->Intersection( &pos ) && ProfileSystem::Instance->ActiveProfile.L2_S5_MangoPlaced && !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoSliced )) &&
				!ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken)
			{
				if( Control::Input->LBclicked() )
				{
					Hud->ShowBannerText("There must be a knife around here."); //Sac: where is the external text for this ?
				}
			}*/
			if( AloveraPoly->Intersection( &pos )  &&  ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraPlaced)
			{
				Cursor::SetMode(CA_Gear);
				if( Control::Input->LBclicked() )
				{
					if( !ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraSliced )
					{
						AloeVeraFull->Shake(0,2,0,0.5f);
						Hud->ShowBannerText("L2aloe");
					}
					/*else if( ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraSliced && !ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraApplied) 
					{
						Hud->ShowBannerText("Sliced aloevera.");//Sac: where is the external text for this ?
					}*/
				}
			}
			else if( HoneyPoly->Intersection( &pos )  && ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxPlaced)
			{
				Cursor::SetMode(CA_Gear);
				if( Control::Input->LBclicked() )
				{
					if( !ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxSliced )
					{
						HoneyWaxFull->Shake(0,2,0,0.5f);
						Hud->ShowBannerText("L2wax");
					}
					/*else if( ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxSliced && !ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxAddedToPot) 
					{
						Hud->ShowBannerText("Sliced honeywax.");//Sac: where is the external text for this ?
					}*/
				}
			}
			else if( TurmericPoly->Intersection( &pos )  && ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPlaced)
			{
				Cursor::SetMode(CA_Gear);
				if( Control::Input->LBclicked() )
				{
					if( !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericSliced )
					{
						TurmericFull->Shake(0,2,0,0.5f);
						Hud->ShowBannerText("L2tur");
					}
					/*else if( ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericSliced && !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericAddedToPot) 
					{
						Hud->ShowBannerText("The turmeric should now be crushed.");//Sac: where is the external text for this ?
					}*/
				}
			}
			else if(MangoPoly->Intersection( &pos ) && ProfileSystem::Instance->ActiveProfile.L2_S5_MangoPlaced )
			{
				Cursor::SetMode(CA_Gear);
				if( Control::Input->LBclicked() )
				{
					if( !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoSliced )
					{
						MangoFull->Shake(0,2,0,0.5f);
						Hud->ShowBannerText("L2mangoes");
					}
					/*else if( ProfileSystem::Instance->ActiveProfile.L2_S5_MangoSliced && !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoAddedToPot) 
					{
						Hud->ShowBannerText("Sliced mangoes.");//Sac: where is the external text for this ?
					}*/
				}
			}
			else if( MedicineTablePoly->Intersection( &pos ))
			{
				Cursor::SetMode(CA_Gear);
				if (Control::Input->LBclicked())
				{
					if(!ProfileSystem::Instance->ActiveProfile.L2_S5_ClickedOnMooppan)
						Hud->ShowBannerText("L2food");
					else if( !ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraPlaced || !ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxPlaced ||
								!ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPlaced || !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoPlaced )
						Hud->ShowBannerText("L2make");
				}
			}
		}
	}
}



float L2_S5_PZMedicineMaking::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L2_S5_PZMedicineMaking::~L2_S5_PZMedicineMaking()
{
	GFHud->InvBoxCtrl->ResetToInvBox();	

	TaskPendingUpdate();

	SAFE_DELETE(CAnim_AloeveraDrop);
	SAFE_DELETE(CAnim_TurmericCrush);
	SAFE_DELETE(CAnim_HoneyMangoGrind);
	SAFE_DELETE(CAnim_HoneyMangoDrip);
	SAFE_DELETE(CAnim_MedicineTake);
	SAFE_DELETE(CAnim_AddHoneyToPot);
	//SAFE_DELETE(CAnim_AddTurmericToPot);

	SAFE_DELETE(JAnim_AloeveraCut);
	SAFE_DELETE(JAnim_HoneyCut);
	SAFE_DELETE(JAnim_MangoCut);
	SAFE_DELETE(JAnim_TurmericCut);
	SAFE_DELETE(JAnim_WoodFire);
	SAFE_DELETE(JAnim_MedicineMix);
	SAFE_DELETE(JAnim_MedicinePour);
	SAFE_DELETE(JAnim_TurmericAddToPot);

	SAFE_DELETE(MedicineWindowDustFX);
	SAFE_DELETE(TurmericFX);
	
	SAFE_DELETE(pop6Chart);
	SAFE_DELETE(S5WaterBoilSteamFX);

	
	SAFE_DELETE(InvKIndiPoly);
	SAFE_DELETE(ChartPoly);
	SAFE_DELETE(MedicineTablePoly);
	SAFE_DELETE(StovePoly);
	SAFE_DELETE(GrinderPoly);
	SAFE_DELETE(HoneyPotPoly);
	SAFE_DELETE(MortarPoly);
	SAFE_DELETE(CrusherPoly);
	SAFE_DELETE(PestlePoly);
	SAFE_DELETE(SpoonPoly);
	SAFE_DELETE(MangoPoly);
	SAFE_DELETE(HoneyPoly);
	SAFE_DELETE(TurmericPoly);
	SAFE_DELETE(AloveraPoly);
	//MUSIC_SFX


	Control::Audio->StopSample(aAmbs5MedicinePuzzle);
	Control::Audio->UnloadSample(aAmbs5MedicinePuzzle);
	//MUSIC_SFX
}

void L2_S5_PZMedicineMaking::ReceiveMessage(int val)
{	
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			if( isAnimPlaying )
				return;

			if( itemVal == eInv_L2S4_LitTorch )
			{
				//permit him to drop fire over stove or on the woods :)
				if( ( StovePoly->Intersection( &dropPos ) || FireWoodDestPoly->Intersection( &dropPos ))
					&& ProfileSystem::Instance->ActiveProfile.L2_S5_WoodPlaced && !ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired)
				{
					Control::Audio->QuickLoadAndPlaySFX("s5pz1_firewoodburned");
					JAnim_WoodFire->PlayAnim();
					isAnimPlaying = true;
					isWoodFireAnimPlaying = true;
					ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired  = true;
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 

					if (ProfileSystem::Instance->ActiveProfile.L2_S6_IsP6OilLampLightened)
					{
						ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S4_LitTorch] = false;
						Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S4_LitTorch); 
					}
					
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if( itemVal == eInv_L2S1_Aloevera )
			{
				if( MedicineTablePoly->Intersection( &dropPos ) && !ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraPlaced )
				{
					Control::Audio->QuickLoadAndPlaySFX("s5pz1_mangoplaced");
					AloeVeraFull->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraPlaced  = true;
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S1_Aloevera] = false;
					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S1_Aloevera);
				}
				else if( StovePoly->Intersection( &dropPos ) && ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced )
				{
					Hud->ShowBannerText("L1follow");
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if( itemVal == eInv_L2S1_HoneyComb )
			{
				if( MedicineTablePoly->Intersection( &dropPos ) && !ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxPlaced )
				{
					Control::Audio->QuickLoadAndPlaySFX("s5pz1_mangoplaced");
					HoneyWaxFull->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxPlaced  = true;
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S1_HoneyComb] = false;
					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S1_HoneyComb);
				}
				else if( StovePoly->Intersection( &dropPos ) && ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced )
				{
					Hud->ShowBannerText("L1follow");
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if( itemVal == eInv_L2S3_Turmeric )
			{
				if( MedicineTablePoly->Intersection( &dropPos ) && !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPlaced )
				{
					Control::Audio->QuickLoadAndPlaySFX("s5pz1_turmericpiecesplaced");
					TurmericFull->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPlaced  = true;
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S3_Turmeric] = false;
					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S3_Turmeric);
				}
				else if( StovePoly->Intersection( &dropPos ) && ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced )
				{
					Hud->ShowBannerText("L1follow");
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if( itemVal == eInv_L2S2_Mangoes )
			{
				if( MedicineTablePoly->Intersection( &dropPos ) && !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoPlaced )
				{
					Control::Audio->QuickLoadAndPlaySFX("s5pz1_mangoplaced");
					MangoFull->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L2_S5_MangoPlaced  = true;
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S2_Mangoes] = false;
					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S2_Mangoes);
				}
				else if( StovePoly->Intersection( &dropPos ) && ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced )
				{
					Hud->ShowBannerText("L1follow");
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if( itemVal == eInv_L2S1_PotWithWater )
			{
				if( StovePoly->Intersection( &dropPos ) && !ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced )
				{
					Control::Audio->QuickLoadAndPlaySFX("s5pz1_potplaced");
					PotFilled->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced  = true;
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S1_PotWithWater] = false;
					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S1_PotWithWater);
					if( ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired )
					{
						S5WaterBoilSteamFX->Start();
						S5WaterBoilSteamFX->AdvanceByTime(2.0f);
					}
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if( itemVal == eInv_L2S4_Firewoods )
			{
				if( FireWoodDestPoly->Intersection( &dropPos ) && !ProfileSystem::Instance->ActiveProfile.L2_S5_WoodPlaced )
				{
					Control::Audio->QuickLoadAndPlaySFX("s5pz1_woodplace");
					WoodLog->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L2_S5_WoodPlaced  = true;
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 
					ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S4_Firewoods] = false;
					Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S4_Firewoods);
				}
				else
					GFInstance->WrongInvDropMsg(Hud);
			}
			else if( itemVal == eInv_L2S6_KnifeSharpen )
			{
				if(MangoPoly->Intersection( &dropPos ) && ProfileSystem::Instance->ActiveProfile.L2_S5_MangoPlaced && !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoSliced )
				{
					Control::Audio->QuickLoadAndPlaySFX("s5pz1_mangocut");
					JAnim_MangoCut->PlayAnim();
					isAnimPlaying = true;
					isMangoCutAnimPlaying = true;
					ProfileSystem::Instance->ActiveProfile.L2_S5_MangoSliced  = true;
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 
				}
				else if( AloveraPoly->Intersection( &dropPos ) && ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraPlaced && !ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraSliced)
				{
					Control::Audio->QuickLoadAndPlaySFX("s5pz1_aloeveracut");
					JAnim_AloeveraCut->PlayAnim();
					isAnimPlaying = true;
					isAloeveraCutAnimPlaying= true;
					ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraSliced  = true;
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 
				}
				else if( HoneyPoly->Intersection( &dropPos ) && ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxPlaced && !ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxSliced)
				{
					Control::Audio->QuickLoadAndPlaySFX("s5pz1_honeycombcut");
					JAnim_HoneyCut->PlayAnim();
					isAnimPlaying = true;
					isHoneyCutAnimPlaying= true;
					ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxSliced  = true;
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 
				}
				else if( TurmericPoly->Intersection( &dropPos ) && ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPlaced && !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericSliced)
				{
					Control::Audio->QuickLoadAndPlaySFX("s5pz1_turmericcut");
					JAnim_TurmericCut->PlayAnim();
					isAnimPlaying = true;
					isTurmericCutAnimPlaying= true;
					ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericSliced  = true;
					Hud->ReleaseDrag();
					Hud->CloseInventory(); 
				}
				else 
					GFInstance->WrongInvDropMsg(Hud);

				
			}
			else  if( itemVal == eInv_L2S6_Gloves )
			{
				if( ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired &&  ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced && MedicineTablePoly->Intersection(&dropPos))
				{
					Hud->ShowBannerText("L2thick");
				}
				Hud->ReleaseDrag();
				Hud->CloseInventory(); 
			}
			else  if( itemVal == eInv_L2S5_PotEmpty )
			{
				if( StovePoly->Intersection(&dropPos))
				{
					Hud->ShowBannerText("L2use");
				}
				Hud->ReleaseDrag();
				Hud->CloseInventory(); 
			}
			else
				GFInstance->WrongInvDropMsg(Hud);

			//to check if puzzle is active..
			if( ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericSliced && ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxSliced && ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraSliced
				&& ProfileSystem::Instance->ActiveProfile.L2_S5_MangoSliced && ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired && ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced )
			{
				//Hud->InvBox_StartPuzzleMode();
				isPuzzleModeActive = true;
				ProfileSystem::Instance->ActiveProfile.L2_S5_PuzzleModeActive = true;
			}

		}
		TaskPendingUpdate();
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		TaskPendingUpdate();
		Hud->ShowMap();
	}
	else if( val == INV_CLOSE )
	{		
		if( ProfileSystem::Instance->ActiveProfile.L2_S5_InvMedicineTaken )
		{
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L2_S5_MoopansDen;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
		}
	}	
}

void L2_S5_PZMedicineMaking::TaskPendingUpdate()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraPlaced && ProfileSystem::Instance->ActiveProfile.L2_S1_InvAloveraTaken )
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPlaced && ProfileSystem::Instance->ActiveProfile.L2_S3_InvTurmericPicked )
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S5_MangoPlaced && ProfileSystem::Instance->ActiveProfile.L2_S2_InvMangoesTaken )
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxPlaced && ProfileSystem::Instance->ActiveProfile.L2_S1_InvHoneyCombPicked )
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced && ProfileSystem::Instance->ActiveProfile.L2_S1_InvPotWithWaterTaken)
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S5_WoodPlaced && ProfileSystem::Instance->ActiveProfile.L2_S4_InvFirewoodTaken)
		isAnyTaskLeft = true;
	else if(ProfileSystem::Instance->ActiveProfile.L2_S5_WoodPlaced && ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchFlamed &&
			!ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired)
		isAnyTaskLeft = true;

	else if(ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken && !ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraSliced)
		isAnyTaskLeft = true;
	else if(ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken && !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericSliced)
		isAnyTaskLeft = true;
	else if(ProfileSystem::Instance->ActiveProfile.L2_S5_MangoPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken && !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoSliced)
		isAnyTaskLeft = true;
	else if(ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken && !ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxSliced)
		isAnyTaskLeft = true;

	ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S5_MoopansDen] = isAnyTaskLeft;

	
}

void L2_S5_PZMedicineMaking::ProcessNonHOHint()
{
	if( ActivePopupID != ePopInactive )
	{
		return;
	}
	
	int iNonHOHint = 0; 
	int MaxNonHOHints = 24;
	do
	{
		switch( iNonHOHint )
		{
			default:			
			//Exit coordinates
			case 0: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraPlaced && ProfileSystem::Instance->ActiveProfile.L2_S1_InvAloveraTaken )
					{
						Hud->ShowHintForInvItem( eInv_L2S1_Aloevera, MedicineTablePoly);
						return;
					}
					break;
			case 1: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPlaced && ProfileSystem::Instance->ActiveProfile.L2_S3_InvTurmericPicked )
					{
						Hud->ShowHintForInvItem( eInv_L2S3_Turmeric, MedicineTablePoly );
						return;
					}
					break;
			case 2: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L2_S5_MangoPlaced && ProfileSystem::Instance->ActiveProfile.L2_S2_InvMangoesTaken )
					{
						Hud->ShowHintForInvItem( eInv_L2S2_Mangoes, MedicineTablePoly ); 
						return;
					}
					break;
			case 3: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxPlaced && ProfileSystem::Instance->ActiveProfile.L2_S1_InvHoneyCombPicked )
					{
						Hud->ShowHintForInvItem( eInv_L2S1_HoneyComb, MedicineTablePoly ); 
						return;
					}
				break;
			case 4: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced && ProfileSystem::Instance->ActiveProfile.L2_S1_InvPotWithWaterTaken)
					{
						Hud->ShowHintForInvItem( eInv_L2S1_PotWithWater, StovePoly );
						return;
					}
				break;
			case 5: ++iNonHOHint;
					if(!ProfileSystem::Instance->ActiveProfile.L2_S5_WoodPlaced && ProfileSystem::Instance->ActiveProfile.L2_S4_InvFirewoodTaken)
					{
						Hud->ShowHintForInvItem( eInv_L2S4_Firewoods, StovePoly );
						return;
					}
				break;
			case 6: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L2_S5_WoodPlaced && ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchFlamed && 
						!ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired)
					{
						Hud->ShowHintForInvItem( eInv_L2S4_LitTorch, StovePoly );
						return;
					}
				break;
			case 7: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken 
						&& !ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraSliced)
					{
						Hud->ShowHintForInvItem( eInv_L2S6_KnifeSharpen, AloveraPoly );
						return;
					}
				break;
			case 8: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken 
						&& !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericSliced)
					{
						Hud->ShowHintForInvItem( eInv_L2S6_KnifeSharpen, TurmericPoly );
						return;
					}
				break;
			case 9: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L2_S5_MangoPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken 
						&& !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoSliced)
					{
						Hud->ShowHintForInvItem( eInv_L2S6_KnifeSharpen, MangoPoly );
						return;
					}
				break;
			case 10: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_KnifeSharpTaken
						&& !ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxSliced)
					{
						Hud->ShowHintForInvItem( eInv_L2S6_KnifeSharpen, HoneyPoly );
						return;
					}
				break;
			case 11: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraSliced && !ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraApplied 
						&& ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired)
					{
						CPoint *StoveRectPos = new CPoint( StovePoly->center.x, StovePoly->center.y , 0);
						Hud->ShowHintForSceneItem( StoveRectPos);
						InitHintTrailEmit(&AloveraPoly->center,true);
						SAFE_DELETE(StoveRectPos);
						return;
					}
				break;
			case 12: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L2_S5_MangoSliced && !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoAddedToPot )
					{
						CPoint *GrinderRectPos = new CPoint( GrinderPoly->center.x, GrinderPoly->center.y , 0);
						InitHintTrailEmit(&MangoPoly->center,true);
						Hud->ShowHintForSceneItem( GrinderRectPos);
						SAFE_DELETE(GrinderRectPos);
						return;
					}
				break;
			case 13: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxSliced && !ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxAddedToPot )
					{
						CPoint *GrinderRectPos = new CPoint( GrinderPoly->center.x, GrinderPoly->center.y , 0);
						Hud->ShowHintForSceneItem( GrinderRectPos);
						InitHintTrailEmit(&HoneyPoly->center,true);
						SAFE_DELETE(GrinderRectPos);
						return;
					}
				break;
			case 14: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericSliced && !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericAddedToPot )
					{
						CPoint *MortarRectPos = new CPoint( MortarPoly->center.x, MortarPoly->center.y , 0);
						Hud->ShowHintForSceneItem( MortarRectPos);
						InitHintTrailEmit(&TurmericPoly->center,true);
						SAFE_DELETE(MortarRectPos);
						return;
					}		
				break;
			case 15: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L2_S5_MangoAddedToPot && ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxAddedToPot 
						&& !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidCreated )
					{
						CPoint *GrinderRectPos = new CPoint( GrinderPoly->center.x, GrinderPoly->center.y , 0);
						Hud->ShowHintForSceneItem( GrinderRectPos);
						InitHintTrailEmit(&GrinderPestleRect,true);
						SAFE_DELETE(GrinderRectPos);
						return;
					}
				break;
			case 16: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidCreated && !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidApplied 
						&& ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired )
					{
						CPoint *StoveRectPos = new CPoint( StovePoly->center.x, StovePoly->center.y , 0);
						Hud->ShowHintForSceneItem( StoveRectPos);
						InitHintTrailEmit(&HoneyPotPoly->center,true);
						SAFE_DELETE(StoveRectPos);
						return;
					}
				break;
			case 17: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericAddedToPot && !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderCreated )
					{
						CPoint *MortarRectPos = new CPoint( MortarPoly->center.x, MortarPoly->center.y , 0);
						Hud->ShowHintForSceneItem( MortarRectPos);
						InitHintTrailEmit(&MortarPestleRect,true);
						SAFE_DELETE(MortarRectPos);
						return;
					}
				break;
			case 18: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderCreated && !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderApplied
						&& ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired)
					{
						CPoint *StoveRectPos = new CPoint( StovePoly->center.x, StovePoly->center.y , 0);
						Hud->ShowHintForSceneItem( StoveRectPos);
						InitHintTrailEmit(&MortarPoly->center,true);
						SAFE_DELETE(StoveRectPos);
						return;
					}
				break;
			case 19: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraApplied && ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderApplied &&
						ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidApplied && ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired && 
						!ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineMixed)
					{
						CPoint *StoveRectPos = new CPoint( StovePoly->center.x, StovePoly->center.y , 0);
						Hud->ShowHintForSceneItem( StoveRectPos);
						InitHintTrailEmit(&SpoonPoly->center,true);
						SAFE_DELETE(StoveRectPos);
						return;
					}
				break;
			case 20: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineMixed && !ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineCreated )
					{
						CPoint *KindiRectPos = new CPoint( InvKIndiPoly->center.x, InvKIndiPoly->center.y , 0);
						Hud->ShowHintForSceneItem(KindiRectPos);
						InitHintTrailEmit(&StovePoly->center,true);
						SAFE_DELETE(KindiRectPos);
						return;
					}
				break;
			case 21: ++iNonHOHint;
					if(ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineCreated && !ProfileSystem::Instance->ActiveProfile.L2_S5_InvMedicineTaken )
					{
						InitHintTrailEmit(&InvKIndiPoly->center,true);
						return;
					}
				break;
			case 22: ++iNonHOHint;
					if(  ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L2_S5_MoopansDen] )
					{
						InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
						return;
					}
					break;		
			case 23: ++iNonHOHint;
					if(Hud->ShowHintForComboInvItems())
							return;
			case 24: ++iNonHOHint;
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

void L2_S5_PZMedicineMaking::InitVariables()
{
	isPuzzleModeActive				= false;
	isMangoCutAnimPlaying			= false;
	isAloeveraCutAnimPlaying		= false;
	isAloeveraDropAnimPlaying		= false;
	isHoneyCutAnimPlaying			= false;
	isTurmericCutAnimPlaying		= false;
	isTurmericCrushAnimPlaying		= false;
	isGrinderAnimPlaying			= false;
	isWoodFireAnimPlaying			= false;
	isAddHoneyToPotAnimPlaying		= false;
	isTurmericAddToPotPlaying		= false;
	isAloeveraInHand				= false;
	isHoneyInHand					= false;
	isSpoonInHand					= false;
	isTurmericInHand				= false;
	isMangoInHand					= false;
	isGrinderPestleInHand			= false;
	isMortarInHand					= false;
	isMortarPestleInHand			= false;
	isHoneyPotInHand				= false;
	isSpoonWithMedicineInHand		= false;
	
	isObjectInHand					= false;
	isObjectReturning				= false;
	objectID						= -1;
	returnTimer						= 0.0f;
	returnEndTimer					= 0.0f;
	StartZorder						= 0;
	
}







void L2_S5_PZMedicineMaking::InitAnimations()
{
	S5WaterBoilSteamFX = new SuperFX("PRT\\L2\\S5WaterBoilSteam.INI", PointSystem::CreateCPoint(971,142,0), 1100); 
	
	//Animation
	HangingRoot1->Animate(0,3,0,0,4.0f);	
	HangingRoot2->Animate(0,2,0,0,5.0f);
	WindowLight->SetBlendMode(eBlend_AlphaAdditive);
	WindowLight->Animate(0.15f, 0.65f, 6.0f);

	MedicineWindowDustFX = new SuperFX("PRT\\L2\\MedicineWindowDust.INI", PointSystem::CreateCPoint(42,45,0), 1100); 
	MedicineWindowDustFX->Start();
	MedicineWindowDustFX->AdvanceByTime(2.0f);

	TurmericFX = new SuperFX("PRT\\L2\\Turmeric.INI", PointSystem::CreateCPoint(1014,98,0), MortarAnim2->ZOrder + 5 ); 

	CAnim_TurmericCrush = NULL;
	CObject* objArr4[] = {Mortar1, Mortar2,  Mortar3, Mortar4, CrusherInHand};
	std::string animArr4[] = {"ANIM\\L2\\S5Mortar1.anim", "ANIM\\L2\\S5Mortar2.anim", "ANIM\\L2\\S5Mortar3.anim", "ANIM\\L2\\S5Mortar4.anim", "ANIM\\L2\\S5CrusherMortar.anim"};
	CAnim_TurmericCrush = new CustomAnimGroup(5, animArr4, objArr4);

	CAnim_HoneyMangoGrind = NULL;
	CObject* objArr5[] = {HoneyMangoInGrinder0, HoneyMangoInGrinder1,  HoneyMangoInGrinder2, PestleInHand};
	std::string animArr5[] = {"ANIM\\L2\\S5Pop2Honeymango0.anim", "ANIM\\L2\\S5Pop2Honeymango1.anim", "ANIM\\L2\\S5Pop2Honeymango2.anim", "ANIM\\L2\\S5Pop2PestleInHand.anim"};
	CAnim_HoneyMangoGrind = new CustomAnimGroup(4, animArr5, objArr5);
	
	CAnim_HoneyMangoDrip = NULL;
	CObject* objArr6[] = {HoneyDrip1, HoneyDrip2,  HoneyDrip3};
	std::string animArr6[] = {"ANIM\\L2\\S5Pop2HoneyDrip1.anim", "ANIM\\L2\\S5Pop2HoneyDrip2.anim", "ANIM\\L2\\S5Pop2HoneyDrip3.anim"};
	CAnim_HoneyMangoDrip = new CustomAnimGroup(3, animArr6, objArr6);

	CAnim_MedicineTake = NULL;
	CObject* objArr8[] = {SpoonInHand, SpoonWithMedicine};
	std::string animArr8[] = {"ANIM\\L2\\S5SpoonHand_MedicineTake.anim", "ANIM\\L2\\S5MedSpoon_MedicineTake.anim"};
	CAnim_MedicineTake = new CustomAnimGroup(2, animArr8, objArr8);

	CAnim_AddHoneyToPot = NULL;
	CObject* objArr9[] = {HoneyPotInHand, HoneyPotDrop, HoneyPotInHandHalf, HoneyPotInHandEmpty, HoneyPourMask};
	std::string animArr9[] = {"ANIM\\L2\\S5HoneyPotInHand.anim", "ANIM\\L2\\S5HoneyPotDrop.anim", "ANIM\\L2\\S5HoneyPotInHandHalf.anim", "ANIM\\L2\\S5HoneyPotInHandEmpty.anim", "ANIM\\L2\\S5HoneyPourMask.anim" };
	CAnim_AddHoneyToPot = new CustomAnimGroup(5, animArr9, objArr9);

	CAnim_AloeveraDrop = NULL;
	CObject* objArr10[] = {AloeLeaf1, AloeLeaf2, AloeLeaf3, AloeLeaf4, AloeLeaf5, AloeLeaf6};
	std::string animArr10[] = {"ANIM\\L2\\S5AloeLeafDrop1.anim", "ANIM\\L2\\S5AloeLeafDrop2.anim", "ANIM\\L2\\S5AloeLeafDrop3.anim", "ANIM\\L2\\S5AloeLeafDrop4.anim", "ANIM\\L2\\S5AloeLeafDrop5.anim", "ANIM\\L2\\S5AloeLeafDrop6.anim"};
	CAnim_AloeveraDrop = new CustomAnimGroup(6, animArr10, objArr10);

	JAnim_AloeveraCut = NULL;
	JAnim_AloeveraCut = new JSONAnimator( anim_AloveraCut, CC_CALLBACK_1(L2_S5_PZMedicineMaking::OnAnimationComplete, this ));
	JAnim_AloeveraCut->parseAndLoadJSON("ANIM\\L2\\S5ALOEVERACUT.JSON", HOScene );

	JAnim_HoneyCut = NULL;
	JAnim_HoneyCut = new JSONAnimator( anim_HoneyCut, CC_CALLBACK_1(L2_S5_PZMedicineMaking::OnAnimationComplete, this ));
	JAnim_HoneyCut->parseAndLoadJSON("ANIM\\L2\\S5HONEYCUT.JSON", HOScene );
	
	JAnim_MangoCut = NULL;
	JAnim_MangoCut = new JSONAnimator( anim_MangoCut, CC_CALLBACK_1(L2_S5_PZMedicineMaking::OnAnimationComplete, this ));
	JAnim_MangoCut->parseAndLoadJSON("ANIM\\L2\\S5MANGOCUT.JSON", HOScene );

	JAnim_TurmericCut = NULL;
	JAnim_TurmericCut = new JSONAnimator( anim_TurmericCut, CC_CALLBACK_1(L2_S5_PZMedicineMaking::OnAnimationComplete, this ));
	JAnim_TurmericCut->parseAndLoadJSON("ANIM\\L2\\S5TURMERICCUT.JSON", HOScene );

	JAnim_WoodFire = NULL;
	JAnim_WoodFire = new JSONAnimator( anim_WoodFire, CC_CALLBACK_1(L2_S5_PZMedicineMaking::OnAnimationComplete, this ));
	JAnim_WoodFire->parseAndLoadJSON("ANIM\\L2\\S5WOODFIRE.JSON", HOScene );

	JAnim_MedicineMix = NULL;
	JAnim_MedicineMix = new JSONAnimator( anim_MedicineMix, CC_CALLBACK_1(L2_S5_PZMedicineMaking::OnAnimationComplete, this ));
	JAnim_MedicineMix->parseAndLoadJSON("ANIM\\L2\\S5MEDICINEMIX.JSON", HOScene );

	JAnim_MedicinePour = NULL;
	JAnim_MedicinePour = new JSONAnimator( anim_MedicinePour, CC_CALLBACK_1(L2_S5_PZMedicineMaking::OnAnimationComplete, this ));
	JAnim_MedicinePour->parseAndLoadJSON("ANIM\\L2\\S5MEDICINEPOUR.JSON", HOScene );
	
	superfxArray.push_back( TurmericFX );
	JAnim_TurmericAddToPot = NULL;
	JAnim_TurmericAddToPot = new JSONAnimator( anim_AddTurmericToPot, CC_CALLBACK_1(L2_S5_PZMedicineMaking::OnAnimationComplete, this ));
	JAnim_TurmericAddToPot->parseAndLoadJSON("ANIM\\L2\\S5TURMERICADDTOPOT.JSON", HOScene, &superfxArray );
}


void L2_S5_PZMedicineMaking::OnAnimationComplete(int id )
{
	switch (id)
	{
	case anim_AddTurmericToPot:
		JAnim_TurmericAddToPot->SetEnableObjects(false);
		PotFilled->SetEnable(true);
		break;
	case anim_MedicineMix:	isSpoonWithMedicineInHand = true;
		SpoonWithMedicine->SetEnable(true);
		PotMask->SetEnable(false);
		Hud->ShowBannerText("L2right");
		break;
	case anim_MedicinePour: JAnim_MedicinePour->SetEnableObjects(false);
		Hud->InvBox_StopPuzzleMode();
		Kindi->SetEnable(true);
		break;
	case anim_AloveraCut:
		isAloeveraCutAnimPlaying = false;
		JAnim_AloeveraCut->SetEnableObjects(false);
		AloeVeraCut->SetEnable(true);
		if( ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericSliced && ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxSliced && 
			ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraSliced	&& ProfileSystem::Instance->ActiveProfile.L2_S5_MangoSliced )
		{
			Hud->ReleaseDrag();
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S6_KnifeSharpen);
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S6_KnifeSharpen] = false;
		}
		break;
	case anim_HoneyCut:
		isHoneyCutAnimPlaying = false;
		JAnim_HoneyCut->SetEnableObjects(false);
		HoneyWaxCut->SetEnable(true);
		if( ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericSliced && ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxSliced && 
			ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraSliced	&& ProfileSystem::Instance->ActiveProfile.L2_S5_MangoSliced )
		{
			Hud->ReleaseDrag();
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S6_KnifeSharpen);
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S6_KnifeSharpen] = false;
		}
		break;
	case anim_MangoCut:
		isMangoCutAnimPlaying = false;
		JAnim_MangoCut->SetEnableObjects(false);
		MangoCut->SetEnable(true);
		if( ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericSliced && ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxSliced && 
			ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraSliced && ProfileSystem::Instance->ActiveProfile.L2_S5_MangoSliced )
		{
			Hud->ReleaseDrag();
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S6_KnifeSharpen);
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S6_KnifeSharpen] = false;
		}
		break;
	case anim_WoodFire:
		isWoodFireAnimPlaying= false;
		TorchAnim->SetEnable(false);
		if( ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired )
		{
			S5WaterBoilSteamFX->Start();
			S5WaterBoilSteamFX->AdvanceByTime(2.0f);
		}
		break;
	case anim_TurmericCut:
		isTurmericCutAnimPlaying = false;
		JAnim_TurmericCut->SetEnableObjects(false);
		TurmericCut->SetEnable(true);
		if( ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericSliced && ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxSliced && 
			ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraSliced	&& ProfileSystem::Instance->ActiveProfile.L2_S5_MangoSliced )
		{
			Hud->ReleaseDrag();
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S6_KnifeSharpen);
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S6_KnifeSharpen] = false;
		}
		break;
	default:
		break;
	}
	isAnimPlaying = false;
	Hud->AllowInput = true;
}

void L2_S5_PZMedicineMaking::SetSceneElements()
{
	AloeVeraHalf				->SetEnable(false);
	if( ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraPlaced && !ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraApplied )
	{
		if( ProfileSystem::Instance->ActiveProfile.L2_S5_AloveraSliced )
			AloeVeraFull				->SetEnable(false);
		else
			AloeVeraCut					->SetEnable(false);
	}
	else
	{
		AloeVeraFull				->SetEnable(false);
		AloeVeraCut					->SetEnable(false);
	}

	if( !ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxPlaced || ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidCreated )
	{
		HoneyWaxFull				->SetEnable(false);
		HoneyWaxCut					->SetEnable(false);
		HoneyInGrinder				->SetEnable(false);
	}
	else
	{
		if( !ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxSliced )
		{
			HoneyWaxCut				->SetEnable(false);
			HoneyInGrinder			->SetEnable(false);
		}
		else 
		{
			HoneyWaxFull			->SetEnable(false);
			if( !ProfileSystem::Instance->ActiveProfile.L2_S5_HoneyWaxAddedToPot )
				HoneyInGrinder			->SetEnable(false);
			else
				HoneyWaxCut				->SetEnable(false);
		}
	}

	TurmericHalfCut				->SetEnable(false);
	if( !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPlaced )
	{
		TurmericFull				->SetEnable(false);
		TurmericCut					->SetEnable(false);
	}
	else
	{
		if( !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericSliced )
		{
			TurmericCut				->SetEnable(false);
		}
		else 
		{
			TurmericFull			->SetEnable(false);
			if( ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericAddedToPot )
				TurmericCut				->SetEnable(false);
		}
	}
		
	MangoHalfCut				->SetEnable(false);
	if( !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoPlaced || ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidCreated  )
	{
		MangoFull				->SetEnable(false);
		MangoCut					->SetEnable(false);
		MangoInGrinder				->SetEnable(false);
	}
	else
	{
		if( !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoSliced )
		{
			MangoCut				->SetEnable(false);
			MangoInGrinder			->SetEnable(false);
		}
		else 
		{
			MangoFull			->SetEnable(false);
			if( !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoAddedToPot )
				MangoInGrinder			->SetEnable(false);
			else
				MangoCut				->SetEnable(false);
		}
	}


	if( !ProfileSystem::Instance->ActiveProfile.L2_S5_WoodPlaced  )
	{
		WoodLog						->SetEnable(false);
	}
	if ( !ProfileSystem::Instance->ActiveProfile.L2_S5_WoodFired )
	{
		WoodFireAnim				->SetEnable(false); 
	}
	if( !ProfileSystem::Instance->ActiveProfile.L2_S5_PotWithWaterPlaced )
	{
		PotFilled					->SetEnable(false);
	}

	if( !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidCreated )
	{
		HoneyPotFilled				->SetEnable(false);
	}
	else if( !ProfileSystem::Instance->ActiveProfile.L2_S5_MangoHoneyLiquidApplied )
	{
		PestleOnGround				->SetEnable(false);
		HoneyPotEmpty				->SetEnable(false);
	}
	else
	{
		PestleOnGround				->SetEnable(false);
		HoneyPotEmpty				->SetEnable(false);
		HoneyPotFilled				->SetEnable(false);
	}
	
	if( !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderApplied )
	{
		if( !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericPowderCreated )
		{
			if( !ProfileSystem::Instance->ActiveProfile.L2_S5_TurmericAddedToPot )
			{
				Mortar4					->SetEnable(false);
				Mortar1					->SetEnable(false);
			}
			else
			{
				Mortar4					->SetEnable(false);
				MortarEmpty				->SetEnable(false);
			}
		}
		else
		{
			CrusherOnGround				->SetEnable(false);
			MortarEmpty					->SetEnable(false);
			Mortar1						->SetEnable(false);
		}
	}
	else
	{
		CrusherOnGround				->SetEnable(false);
		MortarEmpty					->SetEnable(false);
		Mortar4						->SetEnable(false);
		Mortar1						->SetEnable(false);
	}
	if( !ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineCreated )
	{
		if( ProfileSystem::Instance->ActiveProfile.L2_S5_MedicineMixed  )
			SpoonOnGround				->SetEnable(false);
		else
		{
			MedicineMask				->SetEnable(false);
			SpoonStir					->SetEnable(false);
		}
	}
	else
	{
		SpoonOnGround				->SetEnable(false);
		SpoonStir					->SetEnable(false);
	}

	KindiMask					->SetEnable(false);
	AloeLeaf1					->SetEnable(false);
	AloeLeaf2					->SetEnable(false);
	AloeLeaf3					->SetEnable(false);
	AloeLeaf4					->SetEnable(false);
	AloeLeaf5					->SetEnable(false);
	AloeLeaf6					->SetEnable(false);
	HoneyPourMask				->SetEnable(false);
	HoneyPotInHandEmpty			->SetEnable(false);
	HoneyPotInHandHalf			->SetEnable(false);
	SpoonWithMedicine			->SetEnable(false);
	TorchAnim					->SetEnable(false);
	PotEmpty					->SetEnable(false);
	ChirattaEmber				->SetEnable(false);
	CrusherInHand				->SetEnable(false);
	PestleInHand				->SetEnable(false);
	HoneyDrip1					->SetEnable(false);
	HoneyDrip2					->SetEnable(false);
	HoneyDrip3					->SetEnable(false);
	HoneyMangoInGrinder0		->SetEnable(false);
	HoneyMangoInGrinder1		->SetEnable(false);
	HoneyMangoInGrinder2		->SetEnable(false);
	HoneyPotDrop				->SetEnable(false);
	
	HoneyPotInHand				->SetEnable(false); 
	knife						->SetEnable(false);
	SpoonPour					->SetEnable(false);
	SpoonInHand					->SetEnable(false);
	MortarAnim1					->SetEnable(false);
	MortarAnim2					->SetEnable(false);
	Mortar2						->SetEnable(false);
	Mortar3						->SetEnable(false);
	MortarDrop					->SetEnable(false);
	TurmericPice				->SetEnable(false);
	MortarInHand				->SetEnable(false);
	PotMask						->SetEnable(false);
	
	

	returnObjPos.push_back( PestleOnGround->Pos);
	returnObjPos.push_back( CrusherOnGround->Pos);
	returnObjPos.push_back( MangoCut->Pos);
	returnObjPos.push_back( HoneyWaxCut->Pos);
	returnObjPos.push_back( AloeVeraCut->Pos);
	returnObjPos.push_back( TurmericCut->Pos);
	returnObjPos.push_back( Mortar4->Pos);
	returnObjPos.push_back( HoneyPotFilled->Pos);
	returnObjPos.push_back( SpoonOnGround->Pos);
	returnObjPos.push_back( SpoonStir->Pos);
}
void L2_S5_PZMedicineMaking::NavigateToScene()
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