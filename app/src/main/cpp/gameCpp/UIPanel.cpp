//===========================================================
// 
// Framework
//===========================================================

#include "UIPanel.h"
#include "Util.h"
#include "Rect.h"
#include "Application.h"
#include "GameFlow.h"
#include "Fonts.h"
#include "InteractiveObjectList.h"
#include "MouseInterface.h"
#include "SuperLog.h"

UIPanel::UIPanel()
{

}

UIPanel::UIPanel(const std::string& panelAssetsPath, float x, float y, int zOrder, float panelHeight, float midPartHeight)
{
	//Top part is the sprite of the panel
	LoadSprite(panelAssetsPath+"\\PANELTOP.SPR");
	SetZ(zOrder);

	// int panelTopHeight = (int)GetWorldRect().h;  // Sac: Commenting Unused variable
	int panelTopWidth = (int)GetWorldRect().w;

	_midPartHeight = midPartHeight;

	int numOfMidParts = (int)(panelHeight/midPartHeight);
	
	if(numOfMidParts<=1)
		numOfMidParts = 1;

	for (int i = 0; i < numOfMidParts; i++)
	{
		CObject* midPart = new CObject();
		midPart->LoadInit(panelAssetsPath+"\\PANELMID.SPR", x, y, zOrder);

		_midPartArray.push_back(midPart);
	}

	_bottomPart = new CObject();
	_bottomPart->LoadInit(panelAssetsPath+"\\PANELBOTTOM.SPR", x, y, zOrder);

	_closeButton = new UIButton(panelAssetsPath+"\\CLOSEBTN.SPR", x+panelTopWidth*0.5f, y, zOrder+1, CC_CALLBACK_0(UIPanel::onCloseBtnClick, this));

	SetPos(PointSystem::CreateCPoint(x,y,0));
    
	Visible = true;//CRenderListEntity
}

void UIPanel::SetPos(const CPoint* pos)
{
	CPoint offset(pos->x - Pos.x, pos->y - Pos.y, 0);
	CObject::SetPos(pos);

	int panelTopHeight = (int)GetWorldRect().h;
	int panelTopWidth = (int)GetWorldRect().w;
	
	float yPos=pos->y+panelTopHeight*0.5f+_midPartHeight*0.5f;

    int numOfMidParts = (int) _midPartArray.size(); // Sac: Implicit converstion of int is made explicit
	for (int i = 0; i < numOfMidParts; i++)
	{
		_midPartArray.at(i)->SetXY(pos->x, yPos);

		if(i==numOfMidParts-1)
			yPos += _midPartHeight*0.5f+panelTopHeight*0.5f;
		else
			yPos += _midPartHeight;
	}

	_bottomPart->SetXY(pos->x, yPos);
	_closeButton->SetXY(pos->x+panelTopWidth*0.5f, pos->y);


	int size = (int)_uiChildren.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		_uiChildren.at(i)->SetPos(PointSystem::CreateCPoint(offset.x + _uiChildren.at(i)->Pos.x, offset.y+_uiChildren.at(i)->Pos.y,0));
	}
}

void UIPanel::onCloseBtnClick()
{
	//Show False
}

void UIPanel::addChild(CObject* obj)
{
	_uiChildren.push_back(obj);
	obj->SetPos(PointSystem::CreateCPoint(Pos.x + obj->Pos.x, Pos.y+obj->Pos.y,0));
}

UIPanel::~UIPanel()
{
	SAFE_DELETE(_bottomPart);

	int size = (int)_midPartArray.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		SAFE_DELETE(_midPartArray[i]);
	}
	_midPartArray.clear();

	size = (int)_uiChildren.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		SAFE_DELETE(_uiChildren[i]);
	}
	_uiChildren.clear();

	SAFE_DELETE(_closeButton);

}

void UIPanel::Update(float ds)
{
	if(Paused || !Visible)
		return;
	
	CObject::Update(ds);

	//if(_isDragging)
	//{
	//	CPoint mPos = Control::Input->Mpos();
	//	if(_sliderAreaRect.Intersection(&mPos))
	//	{
	//		_handleObj->Pos.x = mPos.x;
	//		//_handleObj->Pos.y = mPos.y;
	//	}

	//	if(!Control::Input->LBdown())
	//	{
	//		//mouse up
	//		_isDragging = false;
	//		_handleObj->PlayAnimation(0);
	//	}

	//	_updateSliderValues();
	//}
}

///////////////////////////////////////////////////////////
////////////         Protected Methods     ////////////////
///////////////////////////////////////////////////////////
