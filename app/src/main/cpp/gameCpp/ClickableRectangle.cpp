//===========================================================
// 
// Framework
//===========================================================


#include "ClickableRectangle.h"
#include "InteractiveObjectList.h"
#include "Hud.h"
#include "GameFlow.h"

ClickableRectangle::ClickableRectangle()
{

}

ClickableRectangle::ClickableRectangle(int id, float x, float y, float tw, float th, std::function<void (int)> mouseClickCallback)
{
	_id = id;

	topleft.x = x;
	topleft.y = y;
	w = tw;
	h = th;

	_mouseClickCallback = mouseClickCallback;
	_mouseDownCallback = nullptr;
	_mouseUpCallback = nullptr;
	_mouseOutCallback = nullptr;
	_mouseOverCallback = nullptr;
	_isInteractionEnabled = true;
	_currentMouseState = eMouseState_Default;

	CInteractiveObjectList::GetInstance()->Add((MouseInterface*) this);

}

ClickableRectangle::~ClickableRectangle()
{
	_mouseClickCallback = nullptr;
	_mouseDownCallback = nullptr;
	_mouseUpCallback = nullptr;
	_mouseOutCallback = nullptr;
	_mouseOverCallback = nullptr;

	CInteractiveObjectList::GetInstance()->Remove((MouseInterface*) this);
}


void ClickableRectangle::SetMouseClickCallback(std::function<void (int)> mouseClickCallback)
{
	_mouseClickCallback = mouseClickCallback;
}

void ClickableRectangle::SetMouseDownCallback(std::function<void (int)> mouseDownCallback)
{
	_mouseDownCallback = mouseDownCallback;
}

void ClickableRectangle::SetMouseOverCallback(std::function<void (int)> mouseOverCallback)
{
	_mouseOverCallback = mouseOverCallback;
}

void ClickableRectangle::SetMouseOutCallback(std::function<void (int)> mouseOutCallback)
{
	_mouseOutCallback = mouseOutCallback;
}

void ClickableRectangle::SetMouseUpCallback(std::function<void (int)> mouseUpCallback)
{
	_mouseUpCallback = mouseUpCallback;
}

void ClickableRectangle::HandleMouseInput(CInput * mouseInput)
{
	if (!_isInteractionEnabled || GFInstance->IsActivePopup || GFHud->IsDiaryShowing || GFHud->IsComboInvPopupShowing)
	{
		return;
	}
//	if(Intersection(&mouseInput->Mpos())) 
// Linux: gives error of taking address of temporary also potential crash factor
// below is fix
	CPoint mPos = mouseInput->Mpos();
	if(Intersection(&mPos))	{

		if (_currentMouseState != eMouseState_Over && _currentMouseState != eMouseState_Down)
		{
			OnMouseOver();
		}

		if(mouseInput->LBdown() && _currentMouseState != eMouseState_Down)
		{
			OnMouseDown();
		}

		if (mouseInput->LBclicked())
		{
			OnMouseClick();
		}
		else if(!mouseInput->LBdown() && _currentMouseState == eMouseState_Down)
		{
			OnMouseUp();
		}
	}
	else
	{
		if (_currentMouseState != eMouseState_Out && _currentMouseState != eMouseState_Default)
		{
			OnMouseOut();
		}
	}
}

void ClickableRectangle::SetIsInteractionEnabled(bool isInteractionEnabled)
{
	_isInteractionEnabled = isInteractionEnabled;
}

bool ClickableRectangle::GetIsInteractionEnabled()
{
	return _isInteractionEnabled;
}

int ClickableRectangle::GetId()
{
	return _id;
}

void ClickableRectangle::OnMouseOver()
{
	_currentMouseState = eMouseState_Over;
	if (_mouseOverCallback != nullptr)
	{
		_mouseOverCallback(_id);
	}
}

void ClickableRectangle::OnMouseClick()
{
	_currentMouseState = eMouseState_Click;
	if (_mouseClickCallback != nullptr)
	{
		_mouseClickCallback(_id);
	}
}
void ClickableRectangle::OnMouseDown()
{
	_currentMouseState = eMouseState_Down;
	if (_mouseDownCallback != nullptr)
	{
		_mouseDownCallback(_id);
	}
}

void ClickableRectangle::OnMouseUp()
{
	if (_mouseUpCallback != nullptr)
	{
		_mouseUpCallback(_id);
	}

	_currentMouseState = eMouseState_Over;
}

void ClickableRectangle::OnMouseOut()
{
	_currentMouseState = eMouseState_Out;
	if (_mouseOutCallback != nullptr)
	{
		_mouseOutCallback(_id);
	}
}