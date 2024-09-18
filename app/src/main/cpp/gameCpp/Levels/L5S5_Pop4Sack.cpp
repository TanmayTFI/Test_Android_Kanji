//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L5S5_Pop4Sack.h"
#include "Hud.h"

const CRectangle  ActionRectSack(398.0f,192.0f,317.0f,367.0f);

L5S5_Pop4Sack::L5S5_Pop4Sack(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV5\\S5\\POP4\\POP4.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	GetObjectByID("ray4")->SetScale(1.38f);

	GetObjectByID("ray1")->Animate(0.15f,0.65f,4.5f,0,eBlend_AlphaAdditive);
	GetObjectByID("ray2")->Animate(0.15f,0.75f,5.5f,2.5f,eBlend_AlphaAdditive);
	GetObjectByID("ray3")->Animate(0.15f,0.65f,4.0f,0,eBlend_AlphaAdditive);
	GetObjectByID("ray4")->Animate(0.15f,0.75f,5.0f,0,eBlend_AlphaAdditive);

	s5sacksmokeFX = new SuperFX("PRT\\L5\\s5sacksmoke.INI", PointSystem::CreateCPoint(672,520,0), 1100);
	s5sackdustFX = new SuperFX("PRT\\L5\\s5sackdust.INI", PointSystem::CreateCPoint(662,540,0), 1100);

	std::vector<SuperFX*> superfxArray;
	superfxArray.push_back(s5sackdustFX);
	superfxArray.push_back(s5sacksmokeFX);	

	GetObjectByID("sackempty")->SetEnable(false);

	_JAnim_SackFall = new JSONAnimator(1, CC_CALLBACK_1(L5S5_Pop4Sack::OnAnimationComplete, this));
	_JAnim_SackFall->parseAndLoadJSON("ANIM\\L5\\S5SACKFALL.JSON", this, &superfxArray);

}

void L5S5_Pop4Sack::Required()
{

}

void L5S5_Pop4Sack::OnAnimationComplete(int animID)
{
	Hud->SendUpdateSceneMessage();
	UpdateBgGraphic();

	isAnimationPlaying = false;
}

void L5S5_Pop4Sack::Update(float ds)
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
	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_HOGanapathiActivated)
	{
		if(ActionRectSack.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX("s5p4_sackfall");

				ProfileSystem::Instance->ActiveProfile.L5_S5_HOGanapathiActivated = true;
				_JAnim_SackFall->PlayAnim();
				isAnimationPlaying = true;
			}
		}
	}
	else
	{
		ClosePopup();
	}
}

void L5S5_Pop4Sack::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	GFInstance->WrongInvDropMsg(Hud);
}

void L5S5_Pop4Sack::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here
	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_HOGanapathiActivated)
	{
		Hud->InitHintTrailEmit(&ActionRectSack);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L5S5_Pop4Sack::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_HOGanapathiActivated)
	{
		isAnyTaskLeft = true;
	}


	return isAnyTaskLeft;
}

L5S5_Pop4Sack::~L5S5_Pop4Sack()
{
	SAFE_DELETE(s5sacksmokeFX);
	SAFE_DELETE(s5sackdustFX);
	SAFE_DELETE(_JAnim_SackFall);
}