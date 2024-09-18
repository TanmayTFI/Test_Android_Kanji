//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L7S3_Pop4Ameeha.h"
#include "Hud.h"

const CRectangle  InfoRectAmeeha(368.0f,91.0f,632.0f,481.0f);

L7S3_Pop4Ameeha::L7S3_Pop4Ameeha(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV7\\S3\\POP4\\P4.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	meshAmeeha = NULL;
	meshAmeeha = NULL;
	meshAmeeha = new MeshObject(GetObjectByID("meshface"), 5, 5);
	meshAmeeha->meshFX->SetSpeedAndDisplacement(2, 2, 6, 5);
	InsertObject(meshAmeeha, 1);

	GetObjectByID("hair")->Animate(5, 7, 0, 0, 7.0f);

	Show(false);
}

void L7S3_Pop4Ameeha::Required()
{

}

void L7S3_Pop4Ameeha::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || Hud->IsDraggingItem()  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing)
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
	if(InfoRectAmeeha.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if(Control::Input->LBclicked())
		{				
			Hud->ShowBannerText("L7poor");
		}
	}
}

void L7S3_Pop4Ameeha::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	GFInstance->WrongInvDropMsg(Hud);
}

void L7S3_Pop4Ameeha::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here
	//else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L7S3_Pop4Ameeha::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	return isAnyTaskLeft;
}

L7S3_Pop4Ameeha::~L7S3_Pop4Ameeha()
{
}
