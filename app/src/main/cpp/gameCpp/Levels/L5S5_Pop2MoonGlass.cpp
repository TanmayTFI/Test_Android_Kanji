//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L5S5_Pop2MoonGlass.h"
#include "Hud.h"

const CRectangle  InvDestRectHammer(460.0f,102.0f,520.0f,398.0f);
const CRectangle  HammerDropRect(794.0f,175.0f,150.0f,134.0f);


L5S5_Pop2MoonGlass::L5S5_Pop2MoonGlass(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV5\\S5\\POP2\\P2.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	GetObjectByID("rays")->SetScale(2.00f);

	Show(false);

	GetObjectByID("hammer")->SetEnable(false);
	GetObjectByID("hammer")->SetZ(GetObjectByID("hammer")->ZOrder+120);

	JAnim_HammerHit = NULL;

	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsMoonGlassDoorOpened)
	{
		//closed
		GetObjectByID("rays")->SetEnable(false);
		GetObjectByID("bgopened")->SetEnable(false);
		GetObjectByID("dooropenanim2")->SetEnable(false);
		
		JAnim_HammerHit = new JSONAnimator(0, CC_CALLBACK_1(L5S5_Pop2MoonGlass::OnAnimationComplete, this));
		JAnim_HammerHit->parseAndLoadJSON("ANIM\\L5\\S5MOONDOOROPEN.JSON", this);
	}
	else
	{
		GetObjectByID("handlefull")->SetEnable(false);
		GetObjectByID("handlemask")->SetEnable(false);

		GetObjectByID("bgclosed")->SetEnable(false);

		GetObjectByID("rays")->Animate(0.35f,0.95f,3.0f, 0, eBlend_AlphaAdditive);
	}
}

void L5S5_Pop2MoonGlass::Required()
{

}

void L5S5_Pop2MoonGlass::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup || Hud->IsDraggingItem() || Hud->IsComboInvPopupShowing)
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
	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsMoonGlassDoorOpened)
	{
		if(InvDestRectHammer.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX("s5p2_stickstuck");

				GetObjectByID("handlefull")->Shake(0, 3, 1, 0.25f);
				Hud->ShowBannerText("L5stuck");
			}
		}
	}
	else
	{
		if(InvDestRectHammer.Intersection(&pos))
		{
			Cursor::SetMode(CA_Help);
			if(Control::Input->LBclicked())
			{				
				Hud->ShowBannerText("L5moon");
			}
		}
	}
}

void L5S5_Pop2MoonGlass::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		ProfileSystem::Instance->ActiveProfile.L5_S5_IsMoonGlassDoorOpened = true;

		GetObjectByID("rays")->Animate(0.35f,0.95f,3.0f, 1.57f, eBlend_AlphaAdditive);

		Hud->SendUpdateSceneMessage(); //Open the Moonlight Hatch
		isAnimationPlaying = false;

		Hud->ReleaseDrag();
			
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S2_Hammer] = false;
		Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L5_S2_Hammer);

		Hud->TakeInventoryItem(eInv_L5_S5_PickaxeHandle, GetObjectByID("handlefull")->Pos);
		ProfileSystem::Instance->ActiveProfile.L5_S5_InvPickaxeHandleTaken = true;
	}
}

void L5S5_Pop2MoonGlass::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(isAnimationPlaying)
	{
		GFInstance->WrongInvDropMsg(Hud);
		return;
	}

	if (itemVal == eInv_L5_S2_Hammer && InvDestRectHammer.Intersection(&dropPos))
	{
		if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsMoonGlassDoorOpened)
		{
			isAnimationPlaying = true;

			Control::Audio->QuickLoadAndPlaySFX("s5p2_hammerused");

			JAnim_HammerHit->PlayAnim();

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
		}
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
}

void L5S5_Pop2MoonGlass::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here
	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsMoonGlassDoorOpened && ProfileSystem::Instance->ActiveProfile.L5_S2_InvHammerTaken)
	{
		Hud->ShowHintForInvItem(eInv_L5_S2_Hammer, &HammerDropRect);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L5S5_Pop2MoonGlass::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	
	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsMoonGlassDoorOpened && ProfileSystem::Instance->ActiveProfile.L5_S2_InvHammerTaken)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

L5S5_Pop2MoonGlass::~L5S5_Pop2MoonGlass()
{
	SAFE_DELETE(JAnim_HammerHit);
}
