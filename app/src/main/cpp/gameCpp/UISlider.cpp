//===========================================================
// 
// Framework
//===========================================================

#include "UISlider.h"
#include "Util.h"
#include "Rect.h"
#include "Application.h"
#include "GameFlow.h"
#include "Fonts.h"
#include "InteractiveObjectList.h"
#include "MouseInterface.h"

UISlider::UISlider()
{

}

UISlider::UISlider(const std::string& sliderName, const std::string& sliderBGSpr, const std::string& sliderHandleSpr, float x, float y, int zOrder, std::function<void (CObject*)> sliderDownCallback)
{
	LoadSprite(sliderBGSpr);
	SetZ(zOrder);

	_handleObj = new CObject();
	_handleObj->LoadInit(sliderHandleSpr, x, y, zOrder+1);

	_textSliderValue = new CBitmapText();
	ColorRGB grey={(118/255.0f),(118/255.0f),(118/255.0f)};
	_textSliderValue->SetTextProps("10", x+GetWorldRect().w*0.5f-15, y-18, zOrder+2, grey, eFont_Italic24, Align_Right);
	_textSliderValue->SetScale(0.75f);

	_textSliderName = new CBitmapText();
	_textSliderName->SetTextProps(sliderName, x-GetWorldRect().w*0.5f, y-18, zOrder+2, grey, eFont_Italic24, Align_Left);
	_textSliderName->SetScale(0.75f);

	_minValue = 0;
	_maxValue = 1.0f;
	_defaultValue = 0;

	_sliderValue = 0;

	_isDragging = false;

	//Set Callbacks
	_sliderDownCallback = sliderDownCallback;

	_handleObj->SetMouseDownCallback(CC_CALLBACK_0(UISlider::onMouseDown, this));
	_handleObj->SetMouseOverCallback(CC_CALLBACK_0(UISlider::onMouseOver, this));
	_handleObj->SetMouseOutCallback(CC_CALLBACK_0(UISlider::onMouseOut, this));

	CInteractiveObjectList::GetInstance()->Add((MouseInterface*)_handleObj);

	SetPos(PointSystem::CreateCPoint(x,y,0));
    
	Visible = true;//CRenderListEntity
}

void UISlider::onMouseOver()
{
	if(!_isDragging)
		_handleObj->PlayAnimation(1);
}

void UISlider::onMouseDown()
{
	if(!_isDragging)
	{
		_isDragging = true;
		_handleObj->PlayAnimation(2);

		if(_sliderDownCallback!=nullptr)
			_sliderDownCallback(this);
	}
}

void UISlider::onMouseOut()
{
	if(!_isDragging)
		_handleObj->PlayAnimation(0);
}

void UISlider::setSliderParams(float minValue, float maxValue, float defaultValue)
{
	_minValue = minValue;
	_maxValue = maxValue;

	_defaultValue = defaultValue;

	setSliderValue(_defaultValue);
}

void UISlider::setSliderValue(float val)
{
	_sliderValue = val;

	if(val>_maxValue)
		_sliderValue = _maxValue;
	else if(val<_minValue)
		_sliderValue = _minValue;

	//update handle position
	_handleObj->Pos.x = _sliderStartPos + ((_sliderValue-_minValue)/(_maxValue-_minValue)) * (_sliderEndPos-_sliderStartPos);

	//round to 2 decimal places
	_sliderValue = (float)((int)(_sliderValue * 100)/100.0f);

	//update text
	_textSliderValue->SetText( std::string("" + MKSTR(_sliderValue)).c_str() );
}

UISlider::~UISlider()
{
	SAFE_DELETE(_handleObj);
	SAFE_DELETE(_textSliderValue);
	SAFE_DELETE(_textSliderName);
}

void UISlider::SetPos(const CPoint* pos)
{
	CObject::SetPos(pos);

	_sliderAreaRect = GetWorldRect();

	//slider length is 95% of slider area width
	_sliderAreaRect.topleft.x += _sliderAreaRect.w*0.05f*0.5f;
	_sliderAreaRect.w*=0.95f;

	_sliderStartPos = pos->x - _sliderAreaRect.w*0.5f;
	_sliderEndPos= pos->x + _sliderAreaRect.w*0.5f;

	_handleObj->SetXY(pos->x, pos->y);
	setSliderValue(_sliderValue);

	_textSliderValue->SetPos(PointSystem::CreateCPoint(pos->x+_sliderAreaRect.w*0.5f-15, pos->y-18,0));
	_textSliderName->SetPos(PointSystem::CreateCPoint(pos->x-_sliderAreaRect.w, pos->y,0));
}

void UISlider::Update(float ds)
{
	if(Paused || !Visible)
		return;
	
	CObject::Update(ds);

	if(_isDragging)
	{
		CPoint mPos = Control::Input->Mpos();
		if(_sliderAreaRect.Intersection(&mPos))
		{
			_handleObj->Pos.x = mPos.x;
			//_handleObj->Pos.y = mPos.y;
		}

		if(!Control::Input->LBdown())
		{
			//mouse up
			_isDragging = false;
			_handleObj->PlayAnimation(0);
		}

		_updateSliderValues();
	}
}

///////////////////////////////////////////////////////////
////////////         Protected Methods     ////////////////
///////////////////////////////////////////////////////////

void UISlider::_updateSliderValues()
{
	_sliderValue = _minValue + (_maxValue-_minValue) * ( (_handleObj->Pos.x - _sliderStartPos)/ (_sliderEndPos-_sliderStartPos));

	//round to 2 decimal places
	float rndVal = (float)((int)(_sliderValue * 100)/100.0f);

	_sliderValue = rndVal;

	_textSliderValue->SetText( std::string("" + MKSTR(rndVal)).c_str() );
}