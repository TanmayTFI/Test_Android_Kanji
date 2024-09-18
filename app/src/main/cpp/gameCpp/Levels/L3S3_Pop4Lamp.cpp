//====================================
//	DQFYLH
//	HO System
//	Level 3 - Scene 03 - POPUP 4: Lamp
//====================================

#include "L3S3_Pop4Lamp.h"
#include "Hud.h"

const CRectangle  DestRectGlassBreak(447.0f,107.0f,416.0f,473.0f);
const CRectangle  InvItemRectSecretBox(548.0f,313.0f,179.0f,109.0f);

L3S3_Pop4Lamp::L3S3_Pop4Lamp(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV3\\S3\\POP4\\P4.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	GetObjectByID("lampglow")->SetBlendMode(eBlend_AlphaAdditive);
	GetObjectByID("lampglow")->Animate(0.25f, 0.65f, 4.0f);

	JAnim_GlassBreaking = NULL;
	OilDropsVFX = NULL;

	/*std::vector<SuperFX*> superfxArray;
	OilDropsVFX = new SuperFX("PRT\\L2\\s4oildrops.INI", PointSystem::CreateCPoint(488,106,0), zOrder+10); 
	superfxArray.push_back(OilDropsVFX);*/
	
	JAnim_GlassBreaking = new JSONAnimator(0, CC_CALLBACK_1(L3S3_Pop4Lamp::OnAnimationComplete, this));
	JAnim_GlassBreaking->parseAndLoadJSON("ANIM\\L3\\S3GLASSBREAK.JSON", this);

	GetObjectByID("hammer")->SetEnable(false);
	GetObjectByID("glassbreak1")->SetEnable(false);
	GetObjectByID("glassbreak2")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L3_S3_IsTempleGlassBroken)
	{
		GetObjectByID("glassright")->SetEnable(false);
		GetObjectByID("glassbreak2")->SetEnable(true);
	}
}

void L3S3_Pop4Lamp::Required()
{

}

void L3S3_Pop4Lamp::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		return;
	}

	if (itemVal == eInv_L3S2_Hammer && DestRectGlassBreak.Intersection(&dropPos))
	{
		if(!ProfileSystem::Instance->ActiveProfile.L3_S3_IsTempleGlassBroken)
		{
			isAnimationPlaying = true;

			Control::Audio->QuickLoadAndPlaySFX("s3p4_glassbreak");

			JAnim_GlassBreaking->PlayAnim();

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.L3_S3_IsTempleGlassBroken = true;
		}
	}
	else
	{
		GFInstance->WrongInvDropMsg(Hud);
	}
}

void L3S3_Pop4Lamp::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open)
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if(!ProfileSystem::Instance->ActiveProfile.L3_S3_IsTempleGlassBroken)
	{
		if(DestRectGlassBreak.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if(Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L3box");
			}
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L3_S3_InvTempleBoxTaken)
	{
		if(InvItemRectSecretBox.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				GetObjectByID("box")->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L3_S3_InvTempleBoxTaken = true;
				Hud->TakeInventoryItem(eInv_L3S3_Box);
				ClosePopup(2.5f);
			}
		}
	}
}

void L3S3_Pop4Lamp::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		//Glass Breaking
		isAnimationPlaying = false;

		GetObjectByID("hammer")->SetEnable(false);
		GetObjectByID("glassright")->SetEnable(false);
		GetObjectByID("glassbreak1")->SetEnable(false);
	}
}

L3S3_Pop4Lamp::~L3S3_Pop4Lamp()
{
	SAFE_DELETE(OilDropsVFX);
	SAFE_DELETE(JAnim_GlassBreaking);
}

void L3S3_Pop4Lamp::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerTaken && !ProfileSystem::Instance->ActiveProfile.L3_S3_IsTempleGlassBroken)
	{
		Hud->ShowHintForInvItem(eInv_L3S2_Hammer, &DestRectGlassBreak);
	}
	else if(ProfileSystem::Instance->ActiveProfile.L3_S3_IsTempleGlassBroken && !ProfileSystem::Instance->ActiveProfile.L3_S3_InvTempleBoxTaken)
	{
		Hud->InitHintTrailEmit(&InvItemRectSecretBox);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L3S3_Pop4Lamp::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(ProfileSystem::Instance->ActiveProfile.L3_S2_InvHammerTaken && !ProfileSystem::Instance->ActiveProfile.L3_S3_IsTempleGlassBroken)
	{
		isAnyTaskLeft = true;
	}
	if(ProfileSystem::Instance->ActiveProfile.L3_S3_IsTempleGlassBroken && !ProfileSystem::Instance->ActiveProfile.L3_S3_InvTempleBoxTaken)
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}
