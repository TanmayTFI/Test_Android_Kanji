//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L7S2_Pop1SphereGlobe.h"
#include "Hud.h"

const CRectangle  InvDestRectSphere(418.0f,161.0f,484.0f,395.0f);

L7S2_Pop1SphereGlobe::L7S2_Pop1SphereGlobe(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV7\\S2\\POP1\\P1.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);
	
	std::vector<SuperFX*> superfxArray;
	s2spherepopFX = new SuperFX("PRT\\L7\\s2spherepop.INI", PointSystem::CreateCPoint(645,419,0), zOrder+2); 
	superfxArray.push_back(s2spherepopFX);

	JAnim_GlobePlace = new JSONAnimator(0, CC_CALLBACK_1(L7S2_Pop1SphereGlobe::OnAnimationComplete,this));
	JAnim_GlobePlace->parseAndLoadJSON("ANIM\\L7\\S2GLOBEPLACE.JSON", this, &superfxArray);

	if(!ProfileSystem::Instance->ActiveProfile.L7_S2_PopSphereFullPlaced)
	{
		GetObjectByID("globefix_effect")->SetEnable(false);
		GetObjectByID("globeglow")->SetEnable(false);
	}
}

void L7S2_Pop1SphereGlobe::Required()
{

}

void L7S2_Pop1SphereGlobe::OnAnimationComplete(int id)
{
	isAnimationPlaying = false;
	ClosePopup(0.25f);
}


void L7S2_Pop1SphereGlobe::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || Hud->IsDraggingItem() || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing)
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
	if(!ProfileSystem::Instance->ActiveProfile.L7_S2_PopSphereFullPlaced)
	{
		if(InvDestRectSphere.Intersection(&pos))
		{
			Cursor::SetMode(CA_Gear);
			if(Control::Input->LBclicked())
			{
				Hud->ShowBannerText("L7place");
			}
		}
	}
}

void L7S2_Pop1SphereGlobe::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (itemVal == eInv_L7_S2_SphereFull && InvDestRectSphere.Intersection(&dropPos))
	{
		if(!ProfileSystem::Instance->ActiveProfile.L7_S2_PopSphereFullPlaced)
		{
			isAnimationPlaying = true;

			Control::Audio->QuickLoadAndPlaySFX("s2p1_sphereplaced");

			JAnim_GlobePlace->PlayAnim();

			ProfileSystem::Instance->ActiveProfile.L7_S2_PopSphereFullPlaced = true;

			Hud->SendUpdateSceneMessage();
			UpdateBgGraphic();

			//Setting here because if user exits half way, saved state will be lost
			//ProfileSystem::Instance->ActiveProfile.L7_S2_LasyaStatueRisenUp = true;

			Hud->ReleaseDrag();
			Hud->CloseInventory(); 

			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L7_S2_SphereFull] = false;
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L7_S2_SphereFull);
		}
	}
	else
		GFInstance->WrongInvDropMsg(Hud);
}

void L7S2_Pop1SphereGlobe::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here
	if(ProfileSystem::Instance->ActiveProfile.L7_S2_InvSphereFullTaken && !ProfileSystem::Instance->ActiveProfile.L7_S2_PopSphereFullPlaced)
	{
		Hud->ShowHintForInvItem(eInv_L7_S2_SphereFull, &InvDestRectSphere);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L7S2_Pop1SphereGlobe::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(ProfileSystem::Instance->ActiveProfile.L7_S2_InvSphereFullTaken && !ProfileSystem::Instance->ActiveProfile.L7_S2_PopSphereFullPlaced)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

L7S2_Pop1SphereGlobe::~L7S2_Pop1SphereGlobe()
{
	SAFE_DELETE(s2spherepopFX);
	SAFE_DELETE(JAnim_GlobePlace);
}
