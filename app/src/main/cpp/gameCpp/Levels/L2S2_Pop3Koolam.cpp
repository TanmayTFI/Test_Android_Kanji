//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 02 - POPUP 3: Koolam
//====================================

#include "L2S2_Pop3Koolam.h"
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
#include "InteractiveObjectList.h"
#include "exMath.h"


const CPoint	CenterPoint(683,334,0);
//Pop3 Kolam Color FX Points
const CPoint	StatColorGreenFxPos(455,130,0);
const CPoint	StatColorRedFxPos(385,140,0);
const CPoint	StatColorYellowFxPos(442,160,0);

const CPoint	ColorGreen(0,0.66f,0);
const CPoint	ColorRed(0.41f,0.05f,0);
const CPoint	ColorYellow(0.88f,0.61f,0.02f);

const CRectangle  InvDestRect(382.0f,112.0f,589.0f,444.0f);
const CRectangle  skipRect(650.0f,110.0f,60.0f,25.0f);

L2S2_Pop3Koolam::L2S2_Pop3Koolam(int popupId, HudControl *hud, CHOScene *scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	//Popup load
	Load("LV2\\S2\\POP\\POP3.SCN"); 
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	isAnimationPlaying = false;

	sfxTimer = 0;

	int ArrayPts14[] = {426,139,402,122,381,122,364,137,371,161,391,173,408,170};
	RedColorPoly=  new CPolygon(7);
	RedColorPoly->Init(ArrayPts14);

	int ArrayPts15[] = {427,146,462,143,490,160,484,184,446,192,412,178};
	YellowColorPoly=  new CPolygon(6);
	YellowColorPoly->Init(ArrayPts15);

	int ArrayPts16[] = {432,138,443,118,471,114,498,124,502,146,490,154,469,142};
	BlueColorPoly=  new CPolygon(7);
	BlueColorPoly->Init(ArrayPts16);


	int ArrayPts[] = {709,518,862,453,904,418,925,334,917,298,891,265,827,222,776,205,693,187,563,214,524,230,494,260,454,297,440,345,451,388,470,427,525,465,592,501};
	OutRing1Poly=  new CPolygon(18);
	OutRing1Poly->Init(ArrayPts);

	
	int ArrayPts1[] = {703,495,795,472,885,411,905,367,904,300,872,265,818,226,775,213,693,195,619,209,574,219,531,241,471,297,461,336,487,417,536,451,600,479};
	OutRing2Poly=  new CPolygon(17);
	OutRing2Poly->Init(ArrayPts1);

	
	int ArrayPts2[] = {704,475,781,462,874,403,892,334,892,303,815,232,696,203,581,223,538,246,490,292,481,335,503,407,556,436,609,460};
	OutRing3Poly=  new CPolygon(14);
	OutRing3Poly->Init(ArrayPts2);

	int ArrayPts3[] = {704,459,779,442,788,422,760,390,696,399,627,393,602,422,612,442};
	RedColor1Poly=  new CPolygon(8);
	RedColor1Poly->Init(ArrayPts3);

	int ArrayPts4[] = {790,419,766,390,809,363,825,334,869,342,876,356,863,392,835,399,820,420};
	RedColor2Poly=  new CPolygon(9);
	RedColor2Poly->Init(ArrayPts4);

	int ArrayPts5[] = {827,328,827,296,813,269,839,262,861,275,879,297,880,321,869,336};
	RedColor3Poly=  new CPolygon(8);
	RedColor3Poly->Init(ArrayPts5);

	int ArrayPts6[] = {749,233,755,221,807,230,837,256,808,266};
	RedColor4Poly=  new CPolygon(5);
	RedColor4Poly->Init(ArrayPts6);

	int ArrayPts7[] = {655,229,648,214,697,207,750,219,744,232};
	RedColor5Poly=  new CPolygon(5);
	RedColor5Poly->Init(ArrayPts7);

	int ArrayPts8[] = {651,228,642,213,586,224,551,253,578,264};
	RedColor6Poly=  new CPolygon(5);
	RedColor6Poly->Init(ArrayPts8);

	int ArrayPts9[] = {572,265,555,293,554,325,508,337,506,293,547,259};
	RedColor7Poly=  new CPolygon(6);
	RedColor7Poly->Init(ArrayPts9);

	int ArrayPts10[] = {510,341,550,328,573,360,618,395,597,422,525,398,506,341};
	RedColor8Poly=  new CPolygon(7);
	RedColor8Poly->Init(ArrayPts10);

	int ArrayPts11[] = {548,327,569,357,604,382,635,393,677,399,725,399,765,387,801,367,825,340,832,298,816,268,785,242,751,228,709,220,666,222,625,229,588,245,565,264,553,284};
	GreenInsidePoly=  new CPolygon(19);
	GreenInsidePoly->Init(ArrayPts11);

	
	int ArrayPts12[] = {602,360,683,385,759,373,808,323,791,272,732,239,666,234,612,254,582,282,573,320};
	RedInsidePoly=  new CPolygon(10);
	RedInsidePoly->Init(ArrayPts12);

	int ArrayPts13[] = {621,361,667,355,690,375,704,353,748,361,774,355,786,319,750,306,713,316,709,306,754,272,749,258,718,244,679,241,632,250,623,267,657,302,609,313,597,338};
	SnakePoly=  new CPolygon(19);
	SnakePoly->Init(ArrayPts13);


	ColorInHand = 0;

	StaticColorFX = NULL;
	DynamicColorFX = NULL;
	KolamEndFX = NULL;
	
	skipBtn = GetObjectByID("skipbtn");
	skipBtn->PlayAnimation(0);
	skipBtn->SetEnable(false);
	skipPuzzleThreshold = 6.0f;
	skipPuzzleTimer = 0.0f;


	if( !ProfileSystem::Instance->ActiveProfile.L2_S2_kolamComplete )
	{

		StaticColorFX = new SuperFX();
		StaticColorFX->LoadEffectIni("PRT\\L2\\StaticFX.INI");
		StaticColorFX->SetPos(&StatColorGreenFxPos);
		StaticColorFX->SetZ(eZOrder_Popup+100);
		//StaticColorFX->Start();
		
		DynamicColorFX = new SuperFX();
		DynamicColorFX->LoadEffectIni("PRT\\L2\\DynamicFX.INI");
		DynamicColorFX->SetPos(&CenterPoint);
		DynamicColorFX->SetZ(eZOrder_Popup+100);

		KolamEndFX = new SuperFX();
		KolamEndFX->LoadEffectIni("PRT\\L2\\KolamEndFX.INI");
		KolamEndFX->SetPos(PointSystem::CreateCPoint(692,316,0));
		KolamEndFX->SetZ(eZOrder_Popup+100);
		//KolamEndFX->Start();

		GetObjectByID("pop3snakeGlow")->SetEnable(false);

	}
	//ProfileSystem::Instance->ActiveProfile.L2_S2_ColorPlatePlaced = true; 
	//ProfileSystem::Instance->ActiveProfile.L2_S2_PoojaInstrPlaced = true;
	MouseOutKolamColors();

	Show(false);

}

void L2S2_Pop3Koolam::OnPopupCloseBtnClicked()
 {
	 if(StaticColorFX->IsEmitting)
		 StaticColorFX->StopImmediate();
	 if(DynamicColorFX->IsEmitting)
		 DynamicColorFX->StopImmediate();
	 if(KolamEndFX->IsEmitting)
		 KolamEndFX->StopImmediate();

 }

void L2S2_Pop3Koolam::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (itemVal == eInv_L2S5_SnakeChart &&  InvDestRect.Intersection(&dropPos))
	{
		GetObjectByID("pop3chart")->SetEnable(true);
		
		Hud->ReleaseDrag();
		Hud->CloseInventory(); 

		ProfileSystem::Instance->ActiveProfile.L2_S2_PoojaInstrPlaced = true;
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S5_SnakeChart] = false;
		Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S5_SnakeChart);
		Control::Audio->QuickLoadAndPlaySFX("s2p3_poojachartfixed");
		if(ProfileSystem::Instance->ActiveProfile.L2_S2_ColorPlatePlaced)
			Hud->ShowBannerText("L2color", SFX_SOFT_NOTIFY); 
		return;
	}
	else if (itemVal == eInv_L2S1_ColorPowder &&  InvDestRect.Intersection(&dropPos))
	{

		
		GetObjectByID("pop3colorstand")->SetEnable(true);
		
		Hud->ReleaseDrag();
		Hud->CloseInventory(); 

		ProfileSystem::Instance->ActiveProfile.L2_S2_ColorPlatePlaced = true;
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L2S1_ColorPowder] = false;
		Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L2S1_ColorPowder);
		Control::Audio->QuickLoadAndPlaySFX("s2p3_colorplateplaced");
		if(ProfileSystem::Instance->ActiveProfile.L2_S2_PoojaInstrPlaced)
			Hud->ShowBannerText("L2color", SFX_SOFT_NOTIFY);
		return;
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
}

void L2S2_Pop3Koolam::Required()
{

}

void L2S2_Pop3Koolam::Update(float ds)
{

	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

		if(isAnimationPlaying)
		{
			sfxTimer --;
			if(sfxTimer <= 0)
			{
				
				StaticColorFX->StopImmediate();
				DynamicColorFX->StopImmediate();
				KolamEndFX->StopImmediate();

				isAnimationPlaying = false;
				ClosePopup(1.5f);
				sfxTimer = 0;
			}
			return;
		}

		CPoint pos = Control::Input->Mpos();
		if(!isMouseOverCloseButton)
			Cursor::SetMode(CA_Normal);

		if((!ProfileSystem::Instance->ActiveProfile.L2_S2_ColorPlatePlaced || !ProfileSystem::Instance->ActiveProfile.L2_S2_PoojaInstrPlaced ) && InvDestRect.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if(Control::Input->LBclicked())
			{
				if(!ProfileSystem::Instance->ActiveProfile.L2_S2_PoojaInstrPlaced && !ProfileSystem::Instance->ActiveProfile.L2_S2_ColorPlatePlaced)
					Hud->ShowBannerText("L2snake", SFX_SOFT_NOTIFY);
				else if(!ProfileSystem::Instance->ActiveProfile.L2_S2_PoojaInstrPlaced)
					Hud->ShowBannerText("L2pattern", SFX_SOFT_NOTIFY);
				else if(!ProfileSystem::Instance->ActiveProfile.L2_S2_ColorPlatePlaced)
					Hud->ShowBannerText("L2sketch", SFX_SOFT_NOTIFY);
			}
			return;
		}

		if(ColorInHand!=0)
		{
			if(!DynamicColorFX->IsEmitting && !ProfileSystem::Instance->ActiveProfile.L2_S2_kolamComplete)
				DynamicColorFX->Start();
			DynamicColorFX->SetPos(PointSystem::CreateCPoint(pos.x+10,pos.y+5,0));
		}

		if(RedColorPoly->Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				if(!StaticColorFX->IsEmitting && !ProfileSystem::Instance->ActiveProfile.L2_S2_kolamComplete)
					StaticColorFX->Start();
				Control::Audio->QuickLoadAndPlaySFX("s3p3_colorclicked");
				StaticColorFX->SetPos(&StatColorRedFxPos);
				StaticColorFX->R = ColorRed.x;
				StaticColorFX->G = ColorRed.y;
				StaticColorFX->B = ColorRed.z;
				StaticColorFX->TR = ColorRed.x;
				StaticColorFX->TG = ColorRed.y;
				StaticColorFX->TB = ColorRed.z;

				DynamicColorFX->R = ColorRed.x;
				DynamicColorFX->G = ColorRed.y;
				DynamicColorFX->B = ColorRed.z;
				DynamicColorFX->TR = ColorRed.x;
				DynamicColorFX->TG = ColorRed.y;
				DynamicColorFX->TB = ColorRed.z;
				ColorInHand = 1;
			}

		}
		else if(BlueColorPoly->Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				if(!StaticColorFX->IsEmitting && !ProfileSystem::Instance->ActiveProfile.L2_S2_kolamComplete)
					StaticColorFX->Start();
				Control::Audio->QuickLoadAndPlaySFX("s3p3_colorclicked");
				StaticColorFX->SetPos(&StatColorGreenFxPos);
				StaticColorFX->R = ColorGreen.x;
				StaticColorFX->G = ColorGreen.y;
				StaticColorFX->B = ColorGreen.z;
				StaticColorFX->TR = ColorGreen.x;
				StaticColorFX->TG = ColorGreen.y;
				StaticColorFX->TB = ColorGreen.z;

				DynamicColorFX->R = ColorGreen.x;
				DynamicColorFX->G = ColorGreen.y;
				DynamicColorFX->B = ColorGreen.z;
				DynamicColorFX->TR = ColorGreen.x;
				DynamicColorFX->TG = ColorGreen.y;
				DynamicColorFX->TB = ColorGreen.z;
				ColorInHand = 2;
			}
		}
		else if(YellowColorPoly->Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				if(!StaticColorFX->IsEmitting && !ProfileSystem::Instance->ActiveProfile.L2_S2_kolamComplete)
					StaticColorFX->Start();
				Control::Audio->QuickLoadAndPlaySFX("s3p3_colorclicked");
				StaticColorFX->SetPos(&StatColorYellowFxPos);
				StaticColorFX->R = ColorYellow.x;
				StaticColorFX->G = ColorYellow.y;
				StaticColorFX->B = ColorYellow.z;
				StaticColorFX->TR = ColorYellow.x;
				StaticColorFX->TG = ColorYellow.y;
				StaticColorFX->TB = ColorYellow.z;

				DynamicColorFX->R = ColorYellow.x;
				DynamicColorFX->G = ColorYellow.y;
				DynamicColorFX->B = ColorYellow.z;
				DynamicColorFX->TR = ColorYellow.x;
				DynamicColorFX->TG = ColorYellow.y;
				DynamicColorFX->TB = ColorYellow.z;
				ColorInHand = 3;
			}
		}

		if(SnakePoly->Intersection(&pos))
		{
			SetTintKolamColors(GetObjectByID("pop3col14"));
			GetObjectByID("pop3col14")->SetAlpha(0.65f);
			MouseOutKolamColors();
			GetObjectByID("pop3col14")->SetEnable(true);
			if(Control::Input->LBclicked())
			{
				if (ColorInHand>0)
				{
					Control::Audio->QuickLoadAndPlaySFX("s3p3_colorfill");
					SetTintKolamColors(GetObjectByID("pop3col14"));
					GetObjectByID("pop3col14")->SetAlpha(1.0);
					GetObjectByID("pop3col14")->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R14Color = ColorInHand;
					MouseOutKolamColors(); 
				}
				else
				{
					Hud->ShowBannerText("L2first");
				}
			}
		}
		else if(RedInsidePoly->Intersection(&pos))
		{
			SetTintKolamColors(GetObjectByID("pop3col13"));
			GetObjectByID("pop3col13")->SetAlpha(0.65f);
			MouseOutKolamColors();
			GetObjectByID("pop3col13")->SetEnable(true);
			if(Control::Input->LBclicked())
			{
				if (ColorInHand>0)
				{
					Control::Audio->QuickLoadAndPlaySFX("s3p3_colorfill");
					SetTintKolamColors(GetObjectByID("pop3col13"));
					GetObjectByID("pop3col13")->SetAlpha(1.0);
					GetObjectByID("pop3col13")->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R13Color= ColorInHand;
					MouseOutKolamColors(); 
				}
				else
				{
					Hud->ShowBannerText("L2first");
				}
			}
		}
		else if(GreenInsidePoly->Intersection(&pos))
		{
			SetTintKolamColors(GetObjectByID("pop3col12"));
			GetObjectByID("pop3col12")->SetAlpha(0.65f);
			MouseOutKolamColors();
			GetObjectByID("pop3col12")->SetEnable(true);
			if(Control::Input->LBclicked())
			{
				if (ColorInHand>0)
				{
					Control::Audio->QuickLoadAndPlaySFX("s3p3_colorfill");
					SetTintKolamColors(GetObjectByID("pop3col12"));
					GetObjectByID("pop3col12")->SetAlpha(1.0);
					GetObjectByID("pop3col12")->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R12Color= ColorInHand;
					MouseOutKolamColors(); 
				}
				else
				{
					Hud->ShowBannerText("L2first");
				}
			}
		}
		else if(RedColor1Poly->Intersection(&pos))
		{
			SetTintKolamColors(GetObjectByID("pop3col4"));
			GetObjectByID("pop3col4")->SetAlpha(0.65f);
			MouseOutKolamColors();
			GetObjectByID("pop3col4")->SetEnable(true);
			if(Control::Input->LBclicked())
			{
				if (ColorInHand>0)
				{
					Control::Audio->QuickLoadAndPlaySFX("s3p3_colorfill");
					SetTintKolamColors(GetObjectByID("pop3col4"));
					GetObjectByID("pop3col4")->SetAlpha(1.0);
					GetObjectByID("pop3col4")->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R4Color= ColorInHand;
					MouseOutKolamColors(); 
				}
				else
				{
					Hud->ShowBannerText("L2first");
				}
			}
		}
		else if(RedColor2Poly->Intersection(&pos))
		{
			SetTintKolamColors(GetObjectByID("pop3col5"));
			GetObjectByID("pop3col5")->SetAlpha(0.5);
			MouseOutKolamColors();
			GetObjectByID("pop3col5")->SetEnable(true);
			if(Control::Input->LBclicked())
			{
				if (ColorInHand>0)
				{
					Control::Audio->QuickLoadAndPlaySFX("s3p3_colorfill");
					SetTintKolamColors(GetObjectByID("pop3col5"));
					GetObjectByID("pop3col5")->SetAlpha(1.0);
					GetObjectByID("pop3col5")->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R5Color= ColorInHand;
					MouseOutKolamColors(); 
				}
				else
				{
					Hud->ShowBannerText("L2first");
				}
			}
		}
		else if(RedColor3Poly->Intersection(&pos))
		{
			SetTintKolamColors(GetObjectByID("pop3col6"));
			GetObjectByID("pop3col6")->SetAlpha(0.5);
			MouseOutKolamColors();
			GetObjectByID("pop3col6")->SetEnable(true);
			if(Control::Input->LBclicked())
			{
				if (ColorInHand>0)
				{
					Control::Audio->QuickLoadAndPlaySFX("s3p3_colorfill");
					SetTintKolamColors(GetObjectByID("pop3col6"));
					GetObjectByID("pop3col6")->SetAlpha(1.0);
					GetObjectByID("pop3col6")->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R6Color= ColorInHand;
					MouseOutKolamColors(); 
				}
				else
				{
					Hud->ShowBannerText("L2first");
				}
			}
		}
		else if(RedColor4Poly->Intersection(&pos))
		{
			SetTintKolamColors(GetObjectByID("pop3col7"));
			GetObjectByID("pop3col7")->SetAlpha(0.5);
			MouseOutKolamColors();
			GetObjectByID("pop3col7")->SetEnable(true);
			if(Control::Input->LBclicked())
			{
				if (ColorInHand>0)
				{
					Control::Audio->QuickLoadAndPlaySFX("s3p3_colorfill");
					SetTintKolamColors(GetObjectByID("pop3col7"));
					GetObjectByID("pop3col7")->SetAlpha(1.0);
					GetObjectByID("pop3col7")->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R7Color= ColorInHand;
					MouseOutKolamColors(); 
				}
				else
				{
					Hud->ShowBannerText("L2first");
				}
			}
		}
		else if(RedColor5Poly->Intersection(&pos))
		{
			SetTintKolamColors(GetObjectByID("pop3col8"));
			GetObjectByID("pop3col8")->SetAlpha(0.5);
			MouseOutKolamColors();
			GetObjectByID("pop3col8")->SetEnable(true);
			if(Control::Input->LBclicked())
			{
				if (ColorInHand>0)
				{
					Control::Audio->QuickLoadAndPlaySFX("s3p3_colorfill");
					SetTintKolamColors(GetObjectByID("pop3col8"));
					GetObjectByID("pop3col8")->SetAlpha(1.0);
					GetObjectByID("pop3col8")->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R8Color= ColorInHand;
					MouseOutKolamColors(); 
				}
				else
				{
					Hud->ShowBannerText("L2first");
				}
			}
		}
		else if(RedColor6Poly->Intersection(&pos))
		{
			SetTintKolamColors(GetObjectByID("pop3col9"));
			GetObjectByID("pop3col9")->SetAlpha(0.5);
			MouseOutKolamColors();
			GetObjectByID("pop3col9")->SetEnable(true);
			if(Control::Input->LBclicked())
			{
				if (ColorInHand>0)
				{
					Control::Audio->QuickLoadAndPlaySFX("s3p3_colorfill");
					SetTintKolamColors(GetObjectByID("pop3col9"));
					GetObjectByID("pop3col9")->SetAlpha(1.0);
					GetObjectByID("pop3col9")->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R9Color= ColorInHand;
					MouseOutKolamColors(); 
				}
				else
				{
					Hud->ShowBannerText("L2first");
				}
			}
		}
		else if(RedColor7Poly->Intersection(&pos))
		{
			SetTintKolamColors(GetObjectByID("pop3col10"));
			GetObjectByID("pop3col10")->SetAlpha(0.5);
			MouseOutKolamColors();
			GetObjectByID("pop3col10")->SetEnable(true);
			if(Control::Input->LBclicked())
			{
				if (ColorInHand>0)
				{
					Control::Audio->QuickLoadAndPlaySFX("s3p3_colorfill");
					SetTintKolamColors(GetObjectByID("pop3col10"));
					GetObjectByID("pop3col10")->SetAlpha(1.0);
					GetObjectByID("pop3col10")->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R10Color= ColorInHand;
					MouseOutKolamColors(); 
				}
				else
				{
					Hud->ShowBannerText("L2first");
				}
			}
		}
		else if(RedColor8Poly->Intersection(&pos))
		{
			SetTintKolamColors(GetObjectByID("pop3col11"));
			GetObjectByID("pop3col11")->SetAlpha(0.5);
			MouseOutKolamColors();
			GetObjectByID("pop3col11")->SetEnable(true);
			if(Control::Input->LBclicked())
			{
				if (ColorInHand>0)
				{
					Control::Audio->QuickLoadAndPlaySFX("s3p3_colorfill");
					SetTintKolamColors(GetObjectByID("pop3col11"));
					GetObjectByID("pop3col11")->SetAlpha(1.0);
					GetObjectByID("pop3col11")->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R11Color= ColorInHand;
					MouseOutKolamColors(); 
				}
				else
				{
					Hud->ShowBannerText("L2first");
				}
			}
		}
		else if(OutRing3Poly->Intersection(&pos))
		{
			SetTintKolamColors(GetObjectByID("pop3col3"));
			GetObjectByID("pop3col3")->SetAlpha(0.5);
			MouseOutKolamColors();
			GetObjectByID("pop3col3")->SetEnable(true);
			if(Control::Input->LBclicked())
			{
				if (ColorInHand>0)
				{
					Control::Audio->QuickLoadAndPlaySFX("s3p3_colorfill");
					SetTintKolamColors(GetObjectByID("pop3col3"));
					GetObjectByID("pop3col3")->SetAlpha(1.0);
					GetObjectByID("pop3col3")->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R3Color= ColorInHand;
					MouseOutKolamColors(); 
				}
				else
				{
					Hud->ShowBannerText("L2first");
				}
			}
		}
		else if(OutRing2Poly->Intersection(&pos))
		{
			SetTintKolamColors(GetObjectByID("pop3col2"));
			GetObjectByID("pop3col2")->SetAlpha(0.5);
			MouseOutKolamColors();
			GetObjectByID("pop3col2")->SetEnable(true);
			if(Control::Input->LBclicked())
			{
				if (ColorInHand>0)
				{
					Control::Audio->QuickLoadAndPlaySFX("s3p3_colorfill");
					SetTintKolamColors(GetObjectByID("pop3col2"));
					GetObjectByID("pop3col2")->SetAlpha(1.0);
					GetObjectByID("pop3col2")->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R2Color= ColorInHand;
					MouseOutKolamColors(); 
				}
				else
				{
					Hud->ShowBannerText("L2first");
				}
			}
					
		}
		else if(OutRing1Poly->Intersection(&pos))
		{
					
			SetTintKolamColors(GetObjectByID("pop3col1"));
			GetObjectByID("pop3col1")->SetAlpha(0.5);
			MouseOutKolamColors();
			GetObjectByID("pop3col1")->SetEnable(true);
					
			if(Control::Input->LBclicked())
			{
				if (ColorInHand>0)
				{
					Control::Audio->QuickLoadAndPlaySFX("s3p3_colorfill");
					SetTintKolamColors(GetObjectByID("pop3col1"));
					GetObjectByID("pop3col1")->SetAlpha(1.0);
					GetObjectByID("pop3col1")->SetEnable(true);
					ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R1Color= ColorInHand;
					MouseOutKolamColors(); 
				}
				else
				{
					Hud->ShowBannerText("L2first");
				}
			}
				
		}
		else
		{
			MouseOutKolamColors();
			//SetUserKolamColors();
		}

		
		//Skip Code
		if(!skipBtn->IsActive)
		{
			if (ProfileSystem::Instance->ActiveProfile.L2_S2_ColorPlatePlaced && ProfileSystem::Instance->ActiveProfile.L2_S2_PoojaInstrPlaced)
			{
				skipBtn->SetEnable(true);
				if (GetObjectByID("skipbtntxt"))
				{
					GetObjectByID("skipbtntxt")->SetEnable(true);
					GetObjectByID("skipbtntxt")->Show(true);
				}
			}
		}
		else
		{
			if(skipPuzzleTimer >= skipPuzzleThreshold)
			{
				skipBtn->PlayAnimation(1);
				CPoint mosPos = Control::Input->Mpos();
				if(mosPos.x >= 650 && mosPos.x <= 710 && mosPos.y >= 110 && mosPos.y <= 135)
				{
					Cursor::SetMode(CA_HandPointy);
					if( Control::Input->LBclicked())
					{
						Skip();
					}
				}
			}
			else
			{
				skipPuzzleTimer += Control::LogicRate;
				skipBtn->PlayAnimation(int(skipPuzzleTimer*5) + 2);

				CPoint mosPos = Control::Input->Mpos();
				if(mosPos.x >= 650 && mosPos.x <= 710 && mosPos.y >= 110 && mosPos.y <= 135)
				{
					Cursor::SetMode(CA_Help);
					if( Control::Input->LBclicked() )
					{
						Hud->ShowBannerText("L1srecharge");
					}
				}
			}
		}			
}

void L2S2_Pop3Koolam::SetTintKolamColors(CObject *tintObj)
{
	//tintObj->SetTint(1.0,0.0,0.0);
}

void L2S2_Pop3Koolam::SetUserKolamColors(CObject *tintObj,int color )
{
	if(color == 0)
	{
		tintObj->SetEnable(false);
	}
	else if(color == 1)
	{
		tintObj->SetAlpha(1.0f);
		//tintObj->SetTint(0.88f,0.11f,0.11f);
		tintObj->PlayAnimation(1);
		tintObj->SetEnable(true);
	}
	else if(color == 2)
	{
		tintObj->SetAlpha(1.0f);
		//tintObj->SetTint(0.10f,0.66f,0.06f);
		tintObj->PlayAnimation(0);
		tintObj->SetEnable(true);
	}
	else if(color == 3)
	{
		tintObj->SetAlpha(1.0);
		//tintObj->SetTint(0.88f,0.64f,0.40f);
		tintObj->PlayAnimation(2);
		tintObj->SetEnable(true);
	}

	if(!ProfileSystem::Instance->ActiveProfile.L2_S2_kolamComplete && 
		ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R1Color == 2 &&
		ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R2Color == 1 &&
		ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R3Color == 3 &&
		ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R4Color == 2 &&
		ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R5Color == 1 &&
		ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R6Color == 2 &&
		ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R7Color == 1 &&
		ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R8Color == 2 &&
		ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R9Color == 1 &&
		ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R10Color == 2 &&
		ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R11Color == 1 &&
		ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R12Color == 2 &&
		ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R13Color == 1 &&
		ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R14Color == 3 )
	{
		ProfileSystem::Instance->ActiveProfile.L2_S2_kolamComplete = true;
		KolamEndFX->Start();
		isAnimationPlaying = true;
		sfxTimer = 100;
		 if(StaticColorFX->IsEmitting)
			StaticColorFX->StopImmediate();
		 if(DynamicColorFX->IsEmitting)
			DynamicColorFX->StopImmediate();

		 Control::Audio->QuickLoadAndPlaySFX("s3p3_colorfillcomplete");
		//ClosePopup(1.5f);

	}
}

void L2S2_Pop3Koolam::MouseOutKolamColors()
{
	if( !ProfileSystem::Instance->ActiveProfile.L2_S2_kolamComplete )
	{
		SetUserKolamColors(GetObjectByID("pop3col14"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R14Color);
		SetUserKolamColors(GetObjectByID("pop3col13"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R13Color);
		SetUserKolamColors(GetObjectByID("pop3col12"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R12Color);
		SetUserKolamColors(GetObjectByID("pop3col11"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R11Color);
		SetUserKolamColors(GetObjectByID("pop3col10"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R10Color);
		SetUserKolamColors(GetObjectByID("pop3col9"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R9Color);
		SetUserKolamColors(GetObjectByID("pop3col8"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R8Color);
		SetUserKolamColors(GetObjectByID("pop3col7"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R7Color);
		SetUserKolamColors(GetObjectByID("pop3col6"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R6Color);
		SetUserKolamColors(GetObjectByID("pop3col5"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R5Color);
		SetUserKolamColors(GetObjectByID("pop3col4"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R4Color);
		SetUserKolamColors(GetObjectByID("pop3col3"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R3Color);
		SetUserKolamColors(GetObjectByID("pop3col2"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R2Color);
		SetUserKolamColors(GetObjectByID("pop3col1"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R1Color);

		if(!ProfileSystem::Instance->ActiveProfile.L2_S2_ColorPlatePlaced)
		{
			GetObjectByID("pop3colorstand")->SetEnable(false);
		}

		if(!ProfileSystem::Instance->ActiveProfile.L2_S2_PoojaInstrPlaced)
		{
			GetObjectByID("pop3chart")->SetEnable(false);
		}
	}

	
}

float L2S2_Pop3Koolam::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L2S2_Pop3Koolam::~L2S2_Pop3Koolam()
{
	
	SAFE_DELETE(StaticColorFX);
	SAFE_DELETE(DynamicColorFX);
	SAFE_DELETE(KolamEndFX);
}

void L2S2_Pop3Koolam::TakeInventoryItem(EHOInventory ItemID)
{
	/*
	std::vector<int> vecInvIds;
	switch (ItemID)
	{
	
	default:
		break;
	}
	*/
}

void L2S2_Pop3Koolam::ReceiveMessage(int itemVal)
{	
	
	
}

bool L2S2_Pop3Koolam::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S2_ColorPlatePlaced && ProfileSystem::Instance->ActiveProfile.L2_S1_InvColorPowderTaken)
		isAnyTaskLeft = true;
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S2_PoojaInstrPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_InvPujaChartTaken )
		isAnyTaskLeft = true;
	else if(ProfileSystem::Instance->ActiveProfile.L2_S2_PoojaInstrPlaced && ProfileSystem::Instance->ActiveProfile.L2_S2_ColorPlatePlaced)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

void L2S2_Pop3Koolam::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S2_ColorPlatePlaced && ProfileSystem::Instance->ActiveProfile.L2_S1_InvColorPowderTaken)
	{
		Hud->ShowHintForInvItem(eInv_L2S1_ColorPowder, &InvDestRect);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L2_S2_PoojaInstrPlaced && ProfileSystem::Instance->ActiveProfile.L2_S5_InvPujaChartTaken )
	{
		Hud->ShowHintForInvItem(eInv_L2S5_SnakeChart, &InvDestRect);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L2_S2_PoojaInstrPlaced && ProfileSystem::Instance->ActiveProfile.L2_S2_ColorPlatePlaced)
	{
		Hud->InitHintTrailEmit(InvDestRect.GetCenterX(), InvDestRect.GetCenterY());
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

void L2S2_Pop3Koolam::Skip()
{
	ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R1Color = 2;
	ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R2Color = 1;
	ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R3Color = 3;
	ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R4Color = 2;
	ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R5Color = 1;
	ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R6Color = 2;
	ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R7Color = 1;
	ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R8Color = 2;
	ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R9Color = 1;
	ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R10Color = 2;
	ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R11Color = 1;
	ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R12Color = 2;
	ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R13Color = 1;
	ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R14Color = 3;

	SetUserKolamColors(GetObjectByID("pop3col14"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R14Color);
	SetUserKolamColors(GetObjectByID("pop3col13"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R13Color);
	SetUserKolamColors(GetObjectByID("pop3col12"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R12Color);
	SetUserKolamColors(GetObjectByID("pop3col11"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R11Color);
	SetUserKolamColors(GetObjectByID("pop3col10"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R10Color);
	SetUserKolamColors(GetObjectByID("pop3col9"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R9Color);
	SetUserKolamColors(GetObjectByID("pop3col8"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R8Color);
	SetUserKolamColors(GetObjectByID("pop3col7"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R7Color);
	SetUserKolamColors(GetObjectByID("pop3col6"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R6Color);
	SetUserKolamColors(GetObjectByID("pop3col5"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R5Color);
	SetUserKolamColors(GetObjectByID("pop3col4"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R4Color);
	SetUserKolamColors(GetObjectByID("pop3col3"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R3Color);
	SetUserKolamColors(GetObjectByID("pop3col2"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R2Color);
	SetUserKolamColors(GetObjectByID("pop3col1"),ProfileSystem::Instance->ActiveProfile.L2_S2_pop3_R1Color);

	ProfileSystem::Instance->ActiveProfile.L2_S2_kolamComplete = true;

	KolamEndFX->Start();
	isAnimationPlaying = true;
	sfxTimer = 100;
	if(StaticColorFX->IsEmitting)
		StaticColorFX->StopImmediate();
	if(DynamicColorFX->IsEmitting)
		DynamicColorFX->StopImmediate();

	Control::Audio->QuickLoadAndPlaySFX("s3p3_colorfillcomplete");
}
