//===========================================================
// 
// Framework
//===========================================================

#include "UIButton.h"
#include "TextureFactory.h"
#include "IOStream.h"
#include "Util.h"
#include "Camera.h"
#include "Rect.h"
#include "Application.h"
#include "GameFlow.h"
#include "InteractiveObjectList.h"
#include "MouseInterface.h"

UIButton::UIButton()
{	
}

UIButton::UIButton(const std::string& sprFile, float x, float y, int zOrder, std::function<void (CObject*)> buttonClickCallback)
{
	LoadSprite(sprFile);
	SetPos(PointSystem::CreateCPoint(x,y,0));
	SetZ(zOrder);

	_buttonClickCallback = buttonClickCallback;

	_isClicked = false;
	_clickDownTimer = 0;
    
	Visible = true;//CRenderListEntity

	CInteractiveObjectList::GetInstance()->Add((MouseInterface*)this);
}
UIButton::UIButton(CObject* btnAnimObject, std::function<void (CObject*)> buttonClickCallback)
{
	LoadSprite(btnAnimObject->Filename);
	SetPos(&btnAnimObject->Pos);
	SetZ(btnAnimObject->ZOrder);

	_buttonClickCallback = buttonClickCallback;

	_isClicked = false;
	_clickDownTimer = 0;

	Visible = true;//CRenderListEntity

	CInteractiveObjectList::GetInstance()->Add((MouseInterface*)this);
	btnAnimObject->SetEnable(false);
	btnAnimObject->Show(false);
	
}

void UIButton::OnMouseOver()
{
	CObject::OnMouseOver();
	
	if(!_isClicked)
		PlayAnimation(1);
}

void UIButton::OnMouseClick()
{
	CObject::OnMouseClick();

	if(Paused || !Visible || !IsActive)
		return;

	if(!_isClicked)
	{
		_isClicked = true;
		_clickDownTimer = 10;
		PlayAnimation(2);
	}
}

void UIButton::OnMouseOut()
{
	CObject::OnMouseOut();

	if(!_isClicked)
		PlayAnimation(0);
}

UIButton::~UIButton()
{

}

void UIButton::Update(float ds)
{
	if(Paused || !Visible || !IsActive)
		return;
	
	CObject::Update(ds);

	if(_isClicked)
	{
		_clickDownTimer--;
		if(_clickDownTimer<0)
		{
			if(IsIntersect(Control::Input->Mpos()))
				PlayAnimation(1);
			else
				PlayAnimation(0);
			_isClicked = false;

			if(_buttonClickCallback!=nullptr)
				_buttonClickCallback(this);
		}
	}
}


void UIButton::Enable()
{
	_isInteractionEnabled = true;
	PlayAnimation(0);
}

void UIButton::Disable()
{
	_isInteractionEnabled = false;
	PlayAnimation(3);
}
