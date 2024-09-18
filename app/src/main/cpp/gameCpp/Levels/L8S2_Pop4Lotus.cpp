
//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "L8S2_Pop4Lotus.h"
#include "Hud.h"

const CRectangle  LotusRect(485.0f,174.0f,365.0f,346.0f);
const CRectangle  SwordRect(555.0f,200.0f,172.0f,158.0f);

L8S2_Pop4Lotus::L8S2_Pop4Lotus(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	isAnimationPlaying = false;
	//Popup load
	Load("LV8\\S2\\POP4\\POP4.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	InitSceneElements();
	isAnimationPlaying = false;
}

void L8S2_Pop4Lotus::Required()
{

}

void L8S2_Pop4Lotus::InitSceneElements()
{
	LotusClosed				 = GetObjectByID("LotusClosed");
	LotusClosedEffects		 = GetObjectByID("LotusClosedEffects");
	LotusOpen				 = GetObjectByID("LotusOpen");
	LotusOpenEffects		 = GetObjectByID("LotusOpenEffects");
	SwordHandle				 = GetObjectByID("SwordHandle");
	SwordHandleEffect		 = GetObjectByID("SwordHandleEffect");

	if( ProfileSystem::Instance->ActiveProfile.L8_S2_InvSwordHandleTaken )
	{
		LotusClosed->SetEnable(false);
		LotusClosedEffects->SetEnable(false);
		SwordHandle->SetEnable(false);
		SwordHandleEffect->SetEnable(false);
	}
	else if( ProfileSystem::Instance->ActiveProfile.L8_S2_IsClickedOnLotus )
	{
		LotusClosed->SetEnable(false);
		LotusClosedEffects->SetEnable(false);
		LotusOpenEffects->SetBlendMode(eBlend_AlphaAdditive);
		LotusOpenEffects->Animate(0.45f,0.85f,10.0f);
		SwordHandleEffect->SetBlendMode(eBlend_AlphaAdditive);
		SwordHandleEffect->Animate(0.55f,0.95f,9.0f);
	}
	else
	{
		LotusOpen->SetEnable(false);
		LotusOpenEffects->SetEnable(false);
		SwordHandle->SetEnable(false);
		SwordHandleEffect->SetEnable(false);
		LotusClosedEffects->SetBlendMode(eBlend_AlphaAdditive);
		LotusClosedEffects->Animate(0.75f,0.85f,12.0f);
	}

}


void L8S2_Pop4Lotus::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup)
		return;

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	//handle interactions here

	int ArrayPts[] = {630,188,782,187,843,336,760,472,570,492,487,335};
	LotusPoly=  new CPolygon(6);
	LotusPoly->Init(ArrayPts);


	if( LotusPoly->Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L8_S2_IsClickedOnLotus )
	{
		Cursor::SetMode(CA_HandPointy);
		if( Control::Input->LBclicked() )
		{
			Hud->ShowBannerText("L8responsibl");
			Cursor::SetMode(CA_Normal);
			ProfileSystem::Instance->ActiveProfile.L8_S2_IsClickedOnLotus = true;
			LotusOpen->SetEnable(true);
			LotusOpenEffects->SetEnable(true);
			SwordHandle->SetEnable(true);
			SwordHandleEffect->SetEnable(true);
			LotusOpen->SetAlpha(0.0f);
			SwordHandle->SetAlpha(0.0f);
			Control::Audio->QuickLoadAndPlaySFX("s2p4_lotusBloom");
			LotusClosed->FadeOut(1.5f);
			LotusOpen->FadeIn(1.5f);
			SwordHandle->FadeIn(1.5f);
			LotusOpenEffects->SetBlendMode(eBlend_AlphaAdditive);
			LotusOpenEffects->Animate(0.55f,0.99f,13.0f);
			SwordHandleEffect->SetBlendMode(eBlend_AlphaAdditive);
			SwordHandleEffect->Animate(0.65f,0.95f,9.0f);
		}
		return;
	}

	if( SwordRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L8_S2_InvSwordHandleTaken )
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked() )
		{
			Cursor::SetMode(CA_Normal);
			ProfileSystem::Instance->ActiveProfile.L8_S2_InvSwordHandleTaken = true;
			SwordHandle->SetEnable(false);
			SwordHandleEffect->SetEnable(false);
			Hud->TakeInventoryItem(eInv_L8_S2_SwordHandle);
			UpdateBgGraphic();
			ClosePopup(1.5f);
		}
		return;
	}
}

void L8S2_Pop4Lotus::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	GFInstance->WrongInvDropMsg(Hud);
}

void L8S2_Pop4Lotus::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here
	if(!ProfileSystem::Instance->ActiveProfile.L8_S2_IsClickedOnLotus)
		Hud->InitHintTrailEmit(LotusPoly->center.x, LotusPoly->center.y);
	else if( !ProfileSystem::Instance->ActiveProfile.L8_S2_InvSwordHandleTaken )
		Hud->InitHintTrailEmit( &SwordRect );
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L8S2_Pop4Lotus::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L8_S2_IsClickedOnLotus)
		isAnyTaskLeft = true;
	else if( !ProfileSystem::Instance->ActiveProfile.L8_S2_InvSwordHandleTaken )
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

L8S2_Pop4Lotus::~L8S2_Pop4Lotus()
{
	SAFE_DELETE(LotusPoly);
}
