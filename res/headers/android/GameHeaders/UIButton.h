//===========================================================
// UI Button Object using mouseinterface
// Faisal
//===========================================================

#ifndef UIBUTTON_H
#define UIBUTTON_H

#include "Object.h"
#include "MouseInterface.h"

class UIButton : public CObject
{
	private:
		int _clickDownTimer;
		bool _isClicked;

		std::function<void (CObject*)> _buttonClickCallback;
	public:
		UIButton();
		UIButton(const std::string& sprFile, float x, float y, int zOrder, std::function<void (CObject*)> buttonClickCallback=nullptr);
		UIButton(CObject* btnAnimObject, std::function<void (CObject*)> buttonClickCallback=nullptr);
		~UIButton();

		virtual void	Update(float ds);

		void OnMouseOver();
		void OnMouseClick();
		void OnMouseOut();

		void Enable();
		void Disable();
};
#endif