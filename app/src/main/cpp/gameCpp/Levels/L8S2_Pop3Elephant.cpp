
//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "L8S2_Pop3Elephant.h"
#include "Hud.h"

const CRectangle  ElephantRect(453.0f,112.0f,511.0f,450.0f);


L8S2_Pop3Elephant::L8S2_Pop3Elephant(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	IsMammothNodAnimPlaying	= false;
	IsMammothEatAnimPlaying	= false;
	EatAnimCount			= 0;
	//Popup load
	Load("LV8\\S2\\POP3\\POP3.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	
	GetObjectByID("SugarcaneAnim")->SetEnable(false);
	if(!ProfileSystem::Instance->ActiveProfile.L8_S2_IsSugarCaneGivenToElephant )
		GetObjectByID("SugarcaneInHand")->SetEnable(false);

	MammothEat1 = GetObjectByID("MammothEat1");
	MammothEat2 = GetObjectByID("MammothEat2");
	MammothNod = GetObjectByID("MammothNod");

	MammothEat1->SetEnable(false);
	MammothEat2->SetEnable(false);
	MammothNod->SetEnable(false);
	InitAnimations();
	Control::Audio->LoadSample(aVOL8GuruRefS2_1,	AudioVO);
	Show(false);
}

void L8S2_Pop3Elephant::Required()
{

}

void L8S2_Pop3Elephant::InitAnimations()
{
	Ear					 = GetObjectByID("Ear");
	ElephantAnim		 = GetObjectByID("ElephantAnim");
	EyeLid				 = GetObjectByID("EyeLid");
	Head				 = GetObjectByID("Head");
	SugarcaneAnim		 = GetObjectByID("SugarcaneAnim");
	SugarcaneInHand		 = GetObjectByID("SugarcaneInHand");
	Trunk1				 = GetObjectByID("Trunk1");
	Trunk2				 = GetObjectByID("Trunk2");
	Trunk3				 = GetObjectByID("Trunk3");
	Tusk				 = GetObjectByID("Tusk");
	Wing1				 = GetObjectByID("Wing1");
	Wing2				 = GetObjectByID("Wing2");

	Wing1->SetMaskRect(GetObjectByID("Bg")->GetWorldRect());
	Wing2->SetMaskRect(GetObjectByID("Bg")->GetWorldRect());

	Trunk2->SetEnable(false);
	Trunk3->SetEnable(false);
	SugarcaneInHand->SetEnable(false);
	SugarcaneAnim->SetEnable(false);

	MammothIdleAnim = NULL;
	MammothIdleAnim = new JSONAnimator( 0, CC_CALLBACK_1(L8S2_Pop3Elephant::OnAnimationComplete, this ));
	MammothIdleAnim->parseAndLoadJSON("ANIM\\L8\\S2P3_MAMMOTHIDLE.JSON", this );
	MammothIdleAnim->PlayAnim();

}

void L8S2_Pop3Elephant::OnAnimationComplete(int index)
{
	switch (index)
	{
	case 0:		MammothIdleAnim->PlayAnim();
		break;
	}
}

void L8S2_Pop3Elephant::Update(float ds)
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

	if( IsMammothNodAnimPlaying )
	{
		if (MammothNod->IsAnimationFinished() )
		{
			MammothNod->SetEnable(false);
			MammothIdleAnim->SetAlphaForObjects(1.0f);
			MammothIdleAnim->PlayAnim();
			IsMammothNodAnimPlaying = false;
			Wing1->SetAlpha(1.0f);
			Wing2->SetAlpha(1.0f);
			ElephantAnim->SetAlpha(1.0f);
			MammothNod->SetAlpha(1.0f);
		}
	}

	if( IsMammothEatAnimPlaying )
	{
		if (EatAnimCount == 0 )
		{
			if (MammothEat1->IsAnimationFinished() )
			{
				MammothEat1->SetEnable(false);
				MammothEat2->SetEnable(true);
				MammothEat2->ForcePlayAnimation(0);
				EatAnimCount++;
			} 
		}
		else
		{
			if (MammothEat2->IsAnimationFinished() )
			{
				MammothEat2->SetEnable(false);
				MammothIdleAnim->PlayAnim();
				EatAnimCount++;
				IsMammothEatAnimPlaying = false;
			} 
		}

	}

	if(ElephantRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown && !Hud->IsDraggingItem() )
	{
		Cursor::SetMode(CA_Help);
		if(  Control::Input->LBclicked())
		{
			Hud->ShowBannerText("L8hello");
			MammothIdleAnim->StopAnim();
			//MammothIdleAnim->SetEnableObjects(false);
			MammothIdleAnim->FadeOutObjects(1.0f);
			Wing1->SetEnable(true);
			Wing2->SetEnable(true);
			ElephantAnim->SetEnable(true);
			MammothNod->SetEnable(true);
			/*Wing1->SetAlpha(0.0f);
			Wing2->SetAlpha(0.0f);
			ElephantAnim->SetAlpha(0.0f);
			MammothNod->SetAlpha(0.0f);
			Wing1->FadeIn(2.0f);
			Wing2->FadeIn(2.0f);
			ElephantAnim->FadeIn(2.0f);*/
			MammothNod->FadeIn(2.0f);

			MammothNod->ForcePlayAnimation(0);
			IsMammothNodAnimPlaying = true;
		}
	}
	else if( ElephantRect.Intersection(&pos) && !Hud->IsDraggingItem() && ProfileSystem::Instance->ActiveProfile.L8_S2_SugarCaneGrown && 
		!ProfileSystem::Instance->ActiveProfile.L8_S2_IsSugarCaneGivenToElephant )
	{
		Cursor::SetMode(CA_Gear);
		if(  Control::Input->LBclicked())
			Hud->ShowBannerText("L8feed");
	}
	if(ElephantRect.Intersection(&pos) && ProfileSystem::Instance->ActiveProfile.L8_S2_IsSugarCaneGivenToElephant && !Hud->IsDraggingItem() && 
		!ProfileSystem::Instance->ActiveProfile.L8_S4_InvContainerWithLeafTaken )
	{
		Cursor::SetMode(CA_Help);
		if(  Control::Input->LBclicked())
			Hud->ShowBannerText("L8hang");
	}
	if(ElephantRect.Intersection(&pos) && !ProfileSystem::Instance->ActiveProfile.L8_S2_IsSugarCaneGivenToElephant && !Hud->IsDraggingItem() && 
		ProfileSystem::Instance->ActiveProfile.L8_S4_InvContainerWithLeafTaken )
	{
		Cursor::SetMode(CA_Help);
		if(  Control::Input->LBclicked())
			Hud->ShowBannerText("L8perfect");
	}
	//handle interactions here
}

void L8S2_Pop3Elephant::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if(ElephantRect.Intersection(&dropPos) && !ProfileSystem::Instance->ActiveProfile.L8_S2_IsSugarCaneGivenToElephant )
	{
		if( itemVal == eInv_L8_S2_Sugarcane )
		{
			//SugarCane->SetEnable(true);
			//SugarCane->FadeOut(1.5f);
			MammothIdleAnim->StopAnim();
			MammothIdleAnim->SetEnableObjects(false);
			IsMammothEatAnimPlaying = true;
			Wing1->SetEnable(true);
			Wing2->SetEnable(true);
			ElephantAnim->SetEnable(true);
			MammothEat1->SetEnable(true);
			MammothEat1->ForcePlayAnimation(0);
			Control::Audio->QuickLoadAndPlaySFX("s2p3_giveSugarCaneToElephant");
			ProfileSystem::Instance->ActiveProfile.L8_S2_IsSugarCaneGivenToElephant = true;
			Hud->ReleaseDrag();
			Hud->CloseInventory(); 
			Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L8_S2_Sugarcane);
			ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S2_Sugarcane] = false;
			Hud->SendUpdateSceneMessage();
			Hud->ShowBannerText("L8fill");
			Control::Audio->PlaySample(aVOL8GuruRefS2_1);
			return;
		}
	}
	GFInstance->WrongInvDropMsg(Hud);
}

void L8S2_Pop3Elephant::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here

	if( !ProfileSystem::Instance->ActiveProfile.L8_S2_IsSugarCaneGivenToElephant && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S2_Sugarcane]  )
		Hud->ShowHintForInvItem(eInv_L8_S2_Sugarcane, &ElephantRect);
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L8S2_Pop3Elephant::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if( !ProfileSystem::Instance->ActiveProfile.L8_S2_IsSugarCaneGivenToElephant && ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L8_S2_Sugarcane]  )
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

L8S2_Pop3Elephant::~L8S2_Pop3Elephant()
{
	SAFE_DELETE(MammothIdleAnim);
	Control::Audio->UnloadSample(aVOL8GuruRefS2_1);
}
