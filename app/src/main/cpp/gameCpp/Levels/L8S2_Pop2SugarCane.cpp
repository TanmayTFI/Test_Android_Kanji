
//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "L8S2_Pop2SugarCane.h"
#include "Hud.h"

const CRectangle  SugarCaneRect(496.0f,82.0f,391.0f,363.0f);
const CRectangle  KeyRect(615.0f,344.0f,195.0f,101.0f);


L8S2_Pop2SugarCane::L8S2_Pop2SugarCane(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV8\\S2\\POP2\\POP2.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	GetObjectByID("Sword")->SetEnable(false);
	GetObjectByID("Weapon")->SetEnable(false);
	if(ProfileSystem::Instance->ActiveProfile.L8_S2_InvSugarCaneTaken )
		GetObjectByID("SugarCaneInScene")->SetEnable(false);

	if(ProfileSystem::Instance->ActiveProfile.L8_S2_InvHalfKeyTaken)
		GetObjectByID("Key")->SetEnable(false);

	if (!ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown )
	{
		GetObjectByID("SugarCaneInScene")->SetEnable(false);
		GetObjectByID("SugarCaneInScene02")->SetEnable(false);
		GetObjectByID("SugarCaneBunch")->SetEnable(false);
	}

	
	S2_SugarCaneGrowFX = new SuperFX("PRT\\L8\\S2_SugarCaneGrow.INI", PointSystem::CreateCPoint(670,259,0), 1100); 
	superfxArray.push_back( S2_SugarCaneGrowFX );
	CaneGrowAnim = NULL;
	CaneGrowAnim = new JSONAnimator( 0, CC_CALLBACK_1(L8S2_Pop2SugarCane::OnAnimationComplete, this ));
	CaneGrowAnim->parseAndLoadJSON("ANIM\\L8\\S2_SUGARCANECREATE.JSON", this, &superfxArray );

	CaneCutAnim = NULL;
	CaneCutAnim = new JSONAnimator( 1, CC_CALLBACK_1(L8S2_Pop2SugarCane::OnAnimationComplete, this ));
	CaneCutAnim->parseAndLoadJSON("ANIM\\L8\\S2SUGARCANECUT.JSON", this );


}

void L8S2_Pop2SugarCane::OnAnimationComplete(int index)
{	
	isAnimationPlaying = false;
	switch (index)
	{
	case 0:	GetObjectByID("Weapon")->SetEnable(false);
		Hud->SendUpdateSceneMessage();
		break;
	case 1:		Hud->TakeInventoryItem(eInv_L8_S2_Sugarcane);
		GetObjectByID("SugarCaneInScene")->SetEnable(false);
		Hud->SendUpdateSceneMessage();
		ClosePopup(1.5f);
		break;
	}
}


void L8S2_Pop2SugarCane::Required()
{

}

void L8S2_Pop2SugarCane::Update(float ds)
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
	if( KeyRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L8_S2_InvHalfKeyTaken)
	{
		Cursor::SetMode(CA_HandTake);
		if( Control::Input->LBclicked() )
		{
			ProfileSystem::Instance->ActiveProfile.L8_S2_InvHalfKeyTaken = true;
			Cursor::SetMode(CA_Normal);
			Hud->TakeInventoryItem(eInv_L8_S2_HalfKey, Control::Input->Mpos());
			GetObjectByID("Key")->SetEnable(false);
		}
	}

	else if( SugarCaneRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown && !Hud->IsDraggingItem() )
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked() )
			Hud->ShowBannerText("L8sugar");
	}
	else if( SugarCaneRect.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown && !ProfileSystem::Instance->ActiveProfile.L8_S2_InvSugarCaneTaken &&
		!Hud->IsDraggingItem() )
	{
		Cursor::SetMode(CA_Gear);
		if( Control::Input->LBclicked() )
			Hud->ShowBannerText("L8sword");
	}

}

void L8S2_Pop2SugarCane::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(SugarCaneRect.Intersection(&dropPos) && !ProfileSystem::Instance->ActiveProfile.L8_S2_InvSugarCaneTaken )
	{
		if( itemVal == eInv_L8_CB_MagicWandWithoutKey && !ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown)
		{
			ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown = true;
			CaneGrowAnim->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s2p2_sugarCaneGrow");
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_CB_MagicWandWithoutKey);
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_CB_MagicWandWithoutKey] = false;
			Hud->SendUpdateSceneMessage();
			return;
		}
		else if (itemVal == eInv_L8_S3_Sword && ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown)
		{
			ProfileSystem::Instance->ActiveProfile.L8_S2_InvSugarCaneTaken  = true;
			CaneCutAnim->PlayAnim();
			Control::Audio->QuickLoadAndPlaySFX("s2p2_sugarCaneCut");
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_S3_Sword);
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S3_Sword] = false;
			Hud->SendUpdateSceneMessage();
			return;
		}
	}
	GFInstance->WrongInvDropMsg(Hud);
}

void L8S2_Pop2SugarCane::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here
	if(!ProfileSystem::Instance->ActiveProfile.L8_S2_InvHalfKeyTaken)
		Hud->InitHintTrailEmit(&KeyRect);
	else if( !ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_CB_MagicWandWithoutKey])
		Hud->ShowHintForInvItem(eInv_L8_CB_MagicWandWithoutKey, &SugarCaneRect );
	else if( ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown && !ProfileSystem::Instance->ActiveProfile.L8_S2_InvSugarCaneTaken && 
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S3_Sword])
		Hud->ShowHintForInvItem(eInv_L8_S3_Sword, &SugarCaneRect );
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L8S2_Pop2SugarCane::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L8_S2_InvHalfKeyTaken)
		isAnyTaskLeft = true;
	else if( !ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_CB_MagicWandWithoutKey])
		isAnyTaskLeft = true;
	else if( ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown && !ProfileSystem::Instance->ActiveProfile.L8_S2_InvSugarCaneTaken && 
		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S3_Sword])
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

L8S2_Pop2SugarCane::~L8S2_Pop2SugarCane()
{
	SAFE_DELETE(CaneCutAnim);
	SAFE_DELETE(S2_SugarCaneGrowFX);
}
