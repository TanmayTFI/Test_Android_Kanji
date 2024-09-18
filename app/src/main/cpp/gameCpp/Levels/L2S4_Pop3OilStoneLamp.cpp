//====================================
//	DQFYLH
//	HO System
//	Level 2 - Scene 04 - POPUP 3: Oil Stone Lamp
//====================================

#include "L2S4_Pop3OilStoneLamp.h"
#include "Hud.h"

const CRectangle  GrabRectTorchOnWall(835.0f,110.0f,117.0f,228.0f);
const CRectangle  MemoryRectTigerMask(852.0f,343.0f,109.0f,104.0f);

const CRectangle  DestRectOilArea(429.0f,231.0f,265.0f,213.0f);

L2S4_Pop3OilStoneLamp::L2S4_Pop3OilStoneLamp(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV2\\S4\\POP3\\P3.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	mesh_Oil = NULL;
	mesh_Oil = new MeshObject(GetObjectByID("p3meshoil"), 5, 5);
	mesh_Oil->meshFX->SetSpeedAndDisplacement(4, 2, 4, 2);
	PushObject(mesh_Oil);

	Show(false);
	Hud = hud;

	std::vector<SuperFX*> superfxArray;
	OilDropsVFX = NULL;
	OilDropsVFX = new SuperFX("PRT\\L2\\s4oildrops.INI", PointSystem::CreateCPoint(488,106,0), zOrder+10); 
	superfxArray.push_back(OilDropsVFX);
	
	obj_torchOnWall = GetObjectByID("p3torchstickonwall");
	obj_torchTaken = GetObjectByID("p3torchstick");

	JAnim_TorchOiling = new JSONAnimator(0, CC_CALLBACK_1(L2S4_Pop3OilStoneLamp::OnAnimationComplete, this));
	JAnim_TorchOiling->parseAndLoadJSON("ANIM\\L2\\S4TORCHSTICKOIL.JSON", this, &superfxArray);


	if(ProfileSystem::Instance->ActiveProfile.L2_S4_FireTorchOiled)
		obj_torchOnWall->SetEnable(false);

	obj_torchTaken->SetEnable(false);
	GetObjectByID("p3torchoiled")->SetEnable(false);

	obj_torchTaken->SetZ(obj_torchTaken->ZOrder+eZOrder_PopFrame); //over frame

	if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S4TigerMask])
	{		
		GetObjectByID("p3tigermask")->SetEnable(false);
	}


	isTorchDragging = false;
}

void L2S4_Pop3OilStoneLamp::Required()
{

}

void L2S4_Pop3OilStoneLamp::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	GFInstance->WrongInvDropMsg(Hud);
	//if (itemVal == eInv_L2S4_AxeSharpen && InvDestRectAxe.Intersection(&dropPos))
	//{
	//	/*else
	//	{
	//		GFInstance->WrongInvDropMsg(Hud);
	//	}*/
	//}
}

void L2S4_Pop3OilStoneLamp::Update(float ds)
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

	if(!ProfileSystem::Instance->ActiveProfile.L2_S4_FireTorchOiled)
	{
		if(!isTorchDragging)
		{
			if(DestRectOilArea.Intersection(&pos))
			{
				Cursor::SetMode(CA_Gear);
				if( Control::Input->LBclicked())
				{
					Hud->ShowBannerText("L2oil");
				}
			}
			else if(GrabRectTorchOnWall.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if( Control::Input->LBclicked())
				{
					Control::Audio->QuickLoadAndPlaySFX("s4p3_torchtake");
					isTorchDragging = true;

					obj_torchTaken->SetPos(&pos);
					obj_torchTaken->SetEnable(true);

					obj_torchOnWall->SetEnable(false);
				}
			}
		}
		else
		{
			obj_torchTaken->SetPos(&pos);
			if( Control::Input->LBclicked())
			{
				isTorchDragging = false;
				if(DestRectOilArea.Intersection(&pos))
				{
					isTorchDragging = false;

					//Restore old ZOrder
					obj_torchTaken->SetZ(obj_torchTaken->ZOrder-eZOrder_PopFrame);

					//play ANim
					JAnim_TorchOiling->PlayAnim();

					isAnimationPlaying = true;
				}
				else
				{
					obj_torchTaken->SetEnable(false);
					obj_torchOnWall->SetEnable(true);
				}
			}
		}
	}

#ifdef _CEBUILD
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S4TigerMask] && MemoryRectTigerMask.Intersection(&pos))
	{			
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("p3tigermask")->FadeOut();
			Hud->ShowMemoryObjectCollected_CE(eMem_L2S4TigerMask, &GetObjectByID("p3tigermask")->Pos);
			if(ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchStickOiledTaken)
			{
				ClosePopup(1.5f);
			}
		}
	}
#else
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S4TigerMask] && MemoryRectTigerMask.Intersection(&pos))
	{	
		Cursor::SetMode(CA_Help);
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("p3tigermask")->FadeOut();
			Hud->ShowMemoryObjectCollected_SE(eMem_L2S4TigerMask, &GetObjectByID("p3tigermask")->Pos);
		}
	}
#endif 
}

void L2S4_Pop3OilStoneLamp::OnAnimationComplete(int animID)
{
	if(animID == 0)
	{
		//Oiled Torch
		isAnimationPlaying = false;

		OilDropsVFX->StopImmediate();

		ProfileSystem::Instance->ActiveProfile.L2_S4_FireTorchOiled = true;

		GetObjectByID("p3torchoiled")->SetEnable(false);
		ProfileSystem::Instance->ActiveProfile.L2_S4_InvFireTorchStickOiledTaken = true;
		Hud->TakeInventoryItem(eInv_L2S4_UnlitOilTorch);

#ifdef _CEBUILD
		if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S4TigerMask])
		{			
			ClosePopup(1.5f);
		}
#else
		ClosePopup(1.5f);
#endif 

	}
}

L2S4_Pop3OilStoneLamp::~L2S4_Pop3OilStoneLamp()
{
	SAFE_DELETE(OilDropsVFX);
	SAFE_DELETE(JAnim_TorchOiling);
}

void L2S4_Pop3OilStoneLamp::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S4_FireTorchOiled)
	{
		Hud->ShowHintForSceneItem(&obj_torchOnWall->Pos);
		Hud->InitHintTrailEmit(DestRectOilArea.GetCenterX(), DestRectOilArea.GetCenterY());
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L2S4_Pop3OilStoneLamp::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L2_S4_FireTorchOiled)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}
