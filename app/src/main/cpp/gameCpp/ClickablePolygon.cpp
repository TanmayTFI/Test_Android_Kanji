//===========================================================
// 
// Framework
//===========================================================


#include "ClickablePolygon.h"
#include "InteractiveObjectList.h"
#include "GameFlow.h"
#include "Hud.h"

ClickablePolygon::ClickablePolygon()
{

}

ClickablePolygon::ClickablePolygon(int id, int numPoints, int xyArr[], std::function<void (int)> mouseClickCallback)
{
	_id = id;
	polyRect = NULL;
	numPts = numPoints;
	xPts = new int[numPoints];
	yPts = new int[numPoints];

	int minRectX=1366;
	int minRectY=768;
	int maxRectX=0;
	int maxRectY=0;
	for (int i = 0; i < numPts; i++)
	{
		xPts[i] = xyArr[i*2];
		yPts[i] = xyArr[i*2+1];

		if(xPts[i]<minRectX)
			minRectX = xPts[i];
		if(xPts[i]>maxRectX)
			maxRectX = xPts[i];

		if(yPts[i]<minRectY)
			minRectY = yPts[i];
		if(yPts[i]>maxRectY)
			maxRectY = yPts[i];
	}

	polyRect = new CRectangle((float)minRectX,(float)minRectY,(float)(maxRectX-minRectX),(float)(maxRectY-minRectY));
	center.SetXY(polyRect->GetCenterX(), polyRect->GetCenterY());

	_mouseClickCallback = mouseClickCallback;
	_mouseDownCallback = nullptr;
	_mouseUpCallback = nullptr;
	_mouseOutCallback = nullptr;
	_mouseOverCallback = nullptr;
	_isInteractionEnabled = true;
	_currentMouseState = eMouseState_Default;

	CInteractiveObjectList::GetInstance()->Add((MouseInterface*) this);

}

ClickablePolygon::ClickablePolygon(int id, int numPoints, int xArr[], int yArr[], std::function<void (int)> mouseClickCallback)
{
	_id = id;
	polyRect = NULL;
	numPts = numPoints;
	xPts = new int[numPoints];
	yPts = new int[numPoints];

	int minRectX=1366;
	int minRectY=768;
	int maxRectX=0;
	int maxRectY=0;
	for (int i = 0; i < numPts; i++)
	{
		xPts[i] = xArr[i];
		yPts[i] = yArr[i];

		if(xArr[i]<minRectX)
			minRectX = xArr[i];
		if(xArr[i]>maxRectX)
			maxRectX = xArr[i];

		if(yArr[i]<minRectY)
			minRectY = yArr[i];
		if(yArr[i]>maxRectY)
			maxRectY = yArr[i];
	}

	polyRect = new CRectangle((float)minRectX,(float)minRectY,(float)(maxRectX-minRectX),(float)(maxRectY-minRectY));
	center.SetXY(polyRect->GetCenterX(), polyRect->GetCenterY());

	_mouseClickCallback = mouseClickCallback;
	_mouseDownCallback = nullptr;
	_mouseUpCallback = nullptr;
	_mouseOutCallback = nullptr;
	_mouseOverCallback = nullptr;
	_isInteractionEnabled = true;
	_currentMouseState = eMouseState_Default;

	CInteractiveObjectList::GetInstance()->Add((MouseInterface*) this);
}

ClickablePolygon::~ClickablePolygon()
{
	_mouseClickCallback = nullptr;
	_mouseDownCallback = nullptr;
	_mouseUpCallback = nullptr;
	_mouseOutCallback = nullptr;
	_mouseOverCallback = nullptr;

	CInteractiveObjectList::GetInstance()->Remove((MouseInterface*) this);
}

void ClickablePolygon::SetMouseClickCallback(std::function<void (int)> mouseClickCallback)
{
	_mouseClickCallback = mouseClickCallback;
}

void ClickablePolygon::SetMouseDownCallback(std::function<void (int)> mouseDownCallback)
{
	_mouseDownCallback = mouseDownCallback;
}

void ClickablePolygon::SetMouseOverCallback(std::function<void (int)> mouseOverCallback)
{
	_mouseOverCallback = mouseOverCallback;
}

void ClickablePolygon::SetMouseOutCallback(std::function<void (int)> mouseOutCallback)
{
	_mouseOutCallback = mouseOutCallback;
}

void ClickablePolygon::SetMouseUpCallback(std::function<void (int)> mouseUpCallback)
{
	_mouseUpCallback = mouseUpCallback;
}

void ClickablePolygon::HandleMouseInput(CInput * mouseInput)
{
	if (!_isInteractionEnabled ||  GFInstance == NULL || GFInstance->IsActivePopup || GFHud == NULL || GFHud->IsDiaryShowing || GFHud->IsComboInvPopupShowing)
	{
		return;
	}	

//	if(Intersection(&mouseInput->Mpos())) 
// Linux: gives error of taking address of temporary also potential crash factor
// below is fix
	CPoint mPos = mouseInput->Mpos();
	if(Intersection(&mPos))
	{

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

void ClickablePolygon::SetIsInteractionEnabled(bool isInteractionEnabled)
{
	_isInteractionEnabled = isInteractionEnabled;
}

bool ClickablePolygon::GetIsInteractionEnabled()
{
	return _isInteractionEnabled;
}

MouseState ClickablePolygon::GetCurrentMouseState()
{
	return _currentMouseState;
}

int ClickablePolygon::GetId()
{
	return _id;
}

void ClickablePolygon::OnMouseOver()
{
	_currentMouseState = eMouseState_Over;
	if (_mouseOverCallback != nullptr)
	{
		_mouseOverCallback(_id);
	}
}

void ClickablePolygon::OnMouseClick()
{
	_currentMouseState = eMouseState_Click;
	if (_mouseClickCallback != nullptr)
	{
		_mouseClickCallback(_id);
	}
}

void ClickablePolygon::OnMouseDown()
{
	_currentMouseState = eMouseState_Down;
	if (_mouseDownCallback != nullptr)
	{
		_mouseDownCallback(_id);
	}
}

void ClickablePolygon::OnMouseUp()
{
	if (_mouseUpCallback != nullptr)
	{
		_mouseUpCallback(_id);
	}

	_currentMouseState = eMouseState_Over;
}

void ClickablePolygon::OnMouseOut()
{
	_currentMouseState = eMouseState_Out;
	if (_mouseOutCallback != nullptr)
	{
		_mouseOutCallback(_id);
	}
}