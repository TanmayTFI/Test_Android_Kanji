//====================================
//	DQFYLH
//	HO System
//	Level 4 - Scene 2 :  PUZZLE ALLIGATORHEAD
//====================================

#include "L4_S2_PZAlligatorHead.h"
#include "Hud.h"
#include "GameFlow.h"


L4_S2_PZAlligatorHead::L4_S2_PZAlligatorHead()
{	
	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV2\\S6\\P1\\P1.SCN");
}

void L4_S2_PZAlligatorHead::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Hud->PopupState != Popup_Inactive )
		return;	
}

void L4_S2_PZAlligatorHead::Update()
{
	#ifdef _PARTICLEEDITORENABLED
		if(_isParticleEditorActive)
			return;
	#endif // _PARTICLEEDITORENABLED

	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup )
		return;

	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S2_RiverSide;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}	
	#endif	

	CPoint pos = Control::Input->Mpos();
	Cursor::SetMode(CA_Normal);

	if( IsMouseOverGoBackArea(&pos) )
	{
		Cursor::SetMode(CA_ExitDown,eArea_L4_S2_RiverSide);
		if( Control::Input->LBclicked() )
		{
			//Control::Audio->PlayFootStepsFX();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S2_RiverSide;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}	
	}	
}

float L4_S2_PZAlligatorHead::easing(float t,float b,float c,float d)
{
	return (c * t / d + b);
}

void L4_S2_PZAlligatorHead::Render(const CPoint* pCam)
{  
	if( !Visible ) 
		return; 
}

L4_S2_PZAlligatorHead::~L4_S2_PZAlligatorHead()
{	
	Hud->InvBoxCtrl->ResetToInvBox();	
}

void L4_S2_PZAlligatorHead::ReceiveMessage(int val)
{	

	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		//TaskPendingUpdate();
		Hud->ShowMap();
	}
	else if( val == INVBOX_DROP ) 
	{		
	//	CPoint dropPos; // Sac: Why is this declared here and not used?
	//	int itemVal;
		
	}
	else if( val == INV_CLOSE )
	{		
		
		
	}	
}

void L4_S2_PZAlligatorHead::ProcessNonHOHint()
{
	Hud->ShowNothingToDoMsgBanner();
}
