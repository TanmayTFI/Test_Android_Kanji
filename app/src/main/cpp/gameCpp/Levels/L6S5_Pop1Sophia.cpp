//====================================
//	DQFYLH
//	HO System
//	Level 6 - Scene 05 - POPUP 1: 
//====================================

#include "L6S5_Pop1Sophia.h"

const CRectangle  MemObjRect(826.0f,426.0f,98.0f,96.0f);
const CRectangle  LayngSophiaRect(593.0f,164.0f,187.0f,228.0f);
const CRectangle  TabletRect(636.0f,297.0f,154.0f,111.0f);

L6S5_Pop1Sophia::L6S5_Pop1Sophia(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{

	
	//Popup load
	Load("LV6\\S5\\P1\\P1.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	if( GFInstance->IsReplayingGame )
	{
		GetObjectByID("Sofia_Righthand")->SetEnable(false);
		GetObjectByID("Sofia_Lefthand")->SetEnable(false);
		GetObjectByID("Sofia_Head")->SetEnable(false);
		GetObjectByID("Sofia_BodyWithTab")->SetEnable(false);
		GetObjectByID("TabGlow_Reflection")->SetEnable(false);
	}
	else
	{
		GetObjectByID("Sofia_Righthand")->Animate(1,0,0,0,4,3);
		GetObjectByID("Sofia_Lefthand")->Animate(-1,0,0,0,4,3);
		GetObjectByID("Sofia_Head")->Animate(0,1,0,0,3,3);
	}
	 
	

	/*if (ProfileSystem::Instance->ActiveProfile.L6_S5_FoundSophia)
	{
		GetObjectByID("Sofia_Head")->SetEnable(false);
		GetObjectByID("Sofia_Lefthand")->SetEnable(false);
		GetObjectByID("Sofia_Righthand")->SetEnable(false);
		GetObjectByID("Sofia_BodyWithTab")->SetEnable(false);
	}*/

	
	if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S5KidSandal])
	{
		GetObjectByID("MemObj")->SetEnable(false);
	}

	SetSparkleUpdate(false);
	Control::Audio->LoadSample(aVOL6MaryRefS5_2,	AudioVO);

	isDialogActive = false;
	dialogSystem = NULL;
	Show(false);
}

void L6S5_Pop1Sophia::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || Hud->IsComboInvPopupShowing || GFInstance->IsActivePopup || Hud->IsDraggingItem())
		return;

	if (ProfileSystem::Instance->ActiveProfile.L6_S5_SophiaTabletDialogOver)
	{
		isDialogActive = false;
	}
	if(isDialogActive)
	{
		dialogSystem->Update();
		return;
	}

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton) 
		Cursor::SetMode(CA_Normal);
	#ifdef _CEBUILD
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S5KidSandal] && MemObjRect.Intersection(&pos))
	{
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("MemObj")->FadeOut();
			Hud->ShowMemoryObjectCollected_CE(eMem_L6S5KidSandal, &(GetObjectByID("MemObj")->Pos));
		}
	}
#else
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S5KidSandal] && MemObjRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("MemObj")->FadeOut();
			Hud->ShowMemoryObjectCollected_SE(eMem_L6S5KidSandal, &(GetObjectByID("MemObj")->Pos));
		}
	}
#endif 


	if (!ProfileSystem::Instance->ActiveProfile.L6_S5_TabMinigameSolved)
	{
		if (LayngSophiaRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L6_S5_SophiaTabletDialogOver )
		{
			Cursor::SetMode(CA_Help);
			if (Control::Input->LBclicked())
			{
				dialogSystem->ShowDialog(eDialog_L6S5SophiaPlayingWithTablet);
				isDialogActive = true;
				Hud->AllowInput = false;
				Hud->HideHud(true);	
			}
		}
		else if (TabletRect.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L6_S5_SophiaTabletDialogOver )
		{
			Cursor::SetMode(CA_Look);
			if (Control::Input->LBclicked())
			{
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L6_S5_TabMiniGame;
				Hud->SendNewAreaMessage();
				//SendMessage(ControlExit, ID, HOSM_NewArea);
			}
		}
	}
	else if( GFInstance->IsReplayingGame )//tablet minigame solved && replaying for mem collectable!!
	{
		if(!ProfileSystem::Instance->ActiveProfile.IsMainChapterCompleted) 
		{
			GFInstance->IsReplayingGame = false;
		}
		bool isPendingCollectableInLevel = false;
		if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S1Collected ||
			!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S2Collected ||
			!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S3Collected ||
			!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S4Collected )
		{
			isPendingCollectableInLevel = true;					
			if( !Hud->MessageText->Visible && Hud->BannerAlpha == 0.0f && Hud->BannerState == eHMS_Idle  )//avoid looped banner calls!!
				Hud->ShowBannerText("L1mismo");
		}
		else 
		{
			for( int i = 0; i < 5; i++ )//picked mem collectable till 4th floating world!!
				if( !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[(ProfileSystem::Instance->ActiveProfile.CurrentLevel*5)+i] )
				{
					isPendingCollectableInLevel = true;
					if( !Hud->MessageText->Visible && Hud->BannerAlpha == 0.0f && Hud->BannerState == eHMS_Idle )//avoid looped banner calls!!
						 Hud->ShowBannerText("L1mismc");
					break;
				}
		}

		//No pending collectables during replaying!! force popup & exit!!
		if( !isPendingCollectableInLevel )
		{
			ClosePopup(2.0f);//wait till mem collectable flies to hud!!
		}	
	}
}

void L6S5_Pop1Sophia::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	/* switch (itemVal)
	{
	
	default:
	*/
		GFInstance->WrongInvDropMsg(Hud);
		/*break;
	}*/
}

void L6S5_Pop1Sophia::OnAnimComplete(int animID)
{

}

float L6S5_Pop1Sophia::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

L6S5_Pop1Sophia::~L6S5_Pop1Sophia()
{
	Control::Audio->UnloadSample(aVOL6MaryRefS5_2);
	
}

void L6S5_Pop1Sophia::Required()
{
	
}

void L6S5_Pop1Sophia::ClosePopup(float delayTime)
{
	
	Popup::ClosePopup(delayTime);
}

void L6S5_Pop1Sophia::OnPopupOpened()
{
	if (!ProfileSystem::Instance->ActiveProfile.L6_S5_FoundSophia)
	{
		Hud->ShowBannerText("L6you");
		Control::Audio->PlaySample(aVOL6MaryRefS5_2);
		ProfileSystem::Instance->ActiveProfile.L6_S5_FoundSophia = true;

		/*GetObjectByID("Sofia_Head")->SetEnable(false);
		GetObjectByID("Sofia_Lefthand")->SetEnable(false);
		GetObjectByID("Sofia_Righthand")->SetEnable(false);
		GetObjectByID("Sofia_BodyWithTab")->SetEnable(false);*/

		//Show Objective completed
		if( ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveAdded[eObj_L6O4_FindSophia] && 
			!ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L6O4_FindSophia] )
		{
			ProfileSystem::Instance->ActiveProfile.IsLVLObjectiveCompleted[eObj_L6O4_FindSophia] = true;
			Hud->ShowObjectiveFX(eObj_L6O4_FindSophia,true);
		}
	}
	Popup::OnPopupOpened();
}

bool L6S5_Pop1Sophia::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	if (!ProfileSystem::Instance->ActiveProfile.L6_S5_TabMinigameSolved)
	{
		isAnyTaskLeft = true;
	}
	/*if (!ProfileSystem::Instance->ActiveProfile.L6_S5_FoundSophia)
	{
		isAnyTaskLeft = true;
	}*/

	return isAnyTaskLeft;
}

void L6S5_Pop1Sophia::ProcessHint()
{
	if( GetPopupState()!= ePopupState_Open )
		return;

	if (!ProfileSystem::Instance->ActiveProfile.L6_S5_TabMinigameSolved)
	{
		Hud->InitHintTrailEmit(&TabletRect);
	}
	/*if(!ProfileSystem::Instance->ActiveProfile.L6_S5_FoundSophia)
	{
		Hud->InitHintTrailEmit(&GetObjectByID("Sofia_Head")->GetWorldRect());
	}*/
	else
	{
		/*	Shows Close Button*/
		Popup::ProcessHint();
	}
}
