//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L5S1_Pop1Bow.h"
#include "Hud.h"

const CRectangle  InvItemRectBow1(864.0f,262.0f,118.0f,114.0f);
const CRectangle  InvItemRectBow2(755.0f,424.0f,64.0f,57.0f);

L5S1_Pop1Bow::L5S1_Pop1Bow(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV5\\S1\\POP1\\P1.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	_leafMesh = new MeshObject(GetObjectByID("meshleaf"), 5, 5);
	_leafMesh->meshFX->SetSpeedAndDisplacement(3, 2, 4, 3);
	_leafMesh->Show(false);
	PushObject(_leafMesh);

	if(ProfileSystem::Instance->ActiveProfile.L5_S1_InvArrowHolderTaken)
		GetObjectByID("arrowholder")->SetEnable(false);
}

void L5S1_Pop1Bow::OnPopupOpened()
{
	if(ProfileSystem::Instance->ActiveProfile.L5_S1_InvArrowHolderTaken)
		GetObjectByID("arrowholder")->SetEnable(false);
}

void L5S1_Pop1Bow::Required()
{

}

void L5S1_Pop1Bow::Update(float ds)
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
	if(!ProfileSystem::Instance->ActiveProfile.L5_S1_InvBowTaken)
	{
		if(InvItemRectBow1.Intersection(&pos) || InvItemRectBow2.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				GetObjectByID("bowmasked")->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L5_S1_InvBowTaken = true;
				Hud->TakeInventoryItem(eInv_L5_S1_Bow);

				if(!ProfileSystem::Instance->ActiveProfile.L5_S1_InvArrowHolderTaken)
					Hud->ShowBannerText("L5bow");

				ClosePopup(1.25f);
			}
		}
	}
}

void L5S1_Pop1Bow::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	GFInstance->WrongInvDropMsg(Hud);
}

void L5S1_Pop1Bow::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here
	if(!ProfileSystem::Instance->ActiveProfile.L5_S1_InvBowTaken)
	{
		Hud->InitHintTrailEmit(&InvItemRectBow1);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L5S1_Pop1Bow::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L5_S1_InvBowTaken)
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

L5S1_Pop1Bow::~L5S1_Pop1Bow()
{
}
