#include "L3_S2_MazeDeadEnd.h"
#include "Hud.h"

L3_S2_MazeDeadEnd::L3_S2_MazeDeadEnd(void)
{
	LoadHud(INVENTORY_V1);
	LoadScene("LV3\\S2\\MAZECOMMON\\DEADEND\\DEADEND.SCN");


	EInfoTexts txtArray[] = {Banner_L3_S1_MazeWrongPathText1,
							Banner_L3_S1_MazeWrongPathText2,
							Banner_L3_S1_MazeWrongPathText3};

	int randomTxt = rand() % 3;

	Hud->ShowBannerText(txtArray[randomTxt]);

	CObject *vine_cr = HOScene->GetObjectByID("vine_cr");
	CObject *vine_rt_1 = HOScene->GetObjectByID("vine_rt_1");
	CObject *vine_rt_2 = HOScene->GetObjectByID("vine_rt_2");
	CObject *light_ray = HOScene->GetObjectByID("light_ray");

	light_ray->Animate(0.25f, 0.5f, 4);
	light_ray->SetBlendMode(eBlend_AlphaAdditive);

	vine_cr->Animate(0, 7, 0, 0, 8);

	vine_rt_1->Animate(0, 0, 6, 0, 7);
	vine_rt_1->SetPivot(0, 0 - 250);
	vine_rt_2->Animate(0, 5, 4, 0, 6);
	vine_rt_2->SetPivot(0, 0 - 150);

	LightParticleFX = new SuperFX("PRT\\L3\\S2MAZEDEADEND.INI", PointSystem::CreateCPoint(739,260,0), 1100); 
	LightParticleFX->Start();
	LightParticleFX->AdvanceByTime(2.0f);

	if(ProfileSystem::Instance->ActiveProfile.L3_S2_MazeMapUsed)
		HOScene->Load("LV3\\S2\\MAZECOMMON\\MAP\\MAPHAND.SCN");
}


L3_S2_MazeDeadEnd::~L3_S2_MazeDeadEnd(void)
{
	SAFE_DELETE(LightParticleFX);
}

void L3_S2_MazeDeadEnd::ReceiveMessage(int val)
{

}

void L3_S2_MazeDeadEnd::ProcessNonHOHint()
{
	Hud->InitHintTrailEmit(&HoveredBackBtnRectMax,true,CA_ExitDown);
}

void L3_S2_MazeDeadEnd::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing)
		return;

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	#ifdef ENABLE_ANDROID_BACK_BUTTON
	if (Control::Input->isKeyBackSensed())
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = ProfileSystem::Instance->ActiveProfile.L3_S2_MazeCurrentRoom;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}
	#endif

	if( IsMouseOverGoBackArea(&pos) )
	{

#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitDown);
#else
		Cursor::SetMode(CA_ExitDown);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(ProfileSystem::Instance->ActiveProfile.L3_S2_MazeCurrentRoom, 2);
			}
#else
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = ProfileSystem::Instance->ActiveProfile.L3_S2_MazeCurrentRoom;
			SendMessage(ControlExit, ID, HOSM_NewArea);
#endif

		}	
	}
	else
	{
		if (Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L3follow");
		}
	}
}

void L3_S2_MazeDeadEnd::NavigateToScene()
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