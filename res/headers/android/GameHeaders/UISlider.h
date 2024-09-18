//===========================================================
// UI Button Object using mouseinterface
// Faisal
//===========================================================

#ifndef UISLIDER_H
#define UISLIDER_H

#include "Object.h"
#include "BitmapText.h"
#include "MouseInterface.h"

class UISlider : public CObject
{
	private:
		CBitmapText *_textSliderValue, *_textSliderName;
		CObject	*_handleObj;

		CRectangle _sliderAreaRect;

		float _sliderStartPos, _sliderEndPos;
		float _minValue, _maxValue, _defaultValue;
		float _sliderValue;

		bool _isDragging;

		void _updateSliderValues();

		std::function<void (CObject*)> _sliderDownCallback;
	public:
		UISlider();
		UISlider(const std::string& sliderName, const std::string& sliderBGSpr, const std::string& sliderHandleSpr, float x, float y, int zOrder, std::function<void (CObject*)> sliderDownCallback=nullptr);
		~UISlider();

		virtual void Update(float ds);
		//inherited
		virtual void SetPos(const CPoint* pos);

		void onMouseDown();
		void onMouseOver();
		void onMouseOut();

		void setSliderValue(float val);
		void setSliderParams(float minValue, float maxValue, float defaultValue=0);

		float getValue()			{return _sliderValue;};
		CObject* getSliderHandle()	{return _handleObj;};
		bool isDragging()	{return _isDragging;};
};
#endif