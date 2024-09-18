//===========================================================
// UI Button Object using mouseinterface
// Faisal
//===========================================================

#ifndef CLICKABLERECTANGLE_H
#define CLICKABLERECTANGLE_H

#include "MouseInterface.h"
#include "Rect.h"
#include "Polygon.h"
#include <functional>

class ClickableRectangle : public CRectangle, MouseInterface
{
	protected:

		std::function<void (int)> _mouseClickCallback;
		std::function<void (int)> _mouseDownCallback;
		std::function<void (int)> _mouseOverCallback;
		std::function<void (int)> _mouseOutCallback;
		std::function<void (int)> _mouseUpCallback;

		MouseState _currentMouseState;
		bool _isInteractionEnabled;

		int _id;

	public:

		ClickableRectangle();
		ClickableRectangle(int id, float x, float y, float w, float h, std::function<void (int)> mouseClickCallback = nullptr);

		virtual ~ClickableRectangle();

		void SetMouseClickCallback(std::function<void (int)> mouseClickCallback);
		void SetMouseDownCallback(std::function<void (int)> mouseDownCallback);
		void SetMouseOverCallback(std::function<void (int)> mouseOverCallback);
		void SetMouseOutCallback(std::function<void (int)> mouseOutCallback);
		void SetMouseUpCallback(std::function<void (int)> mouseUpCallback);

		virtual void OnMouseOver();
		virtual void OnMouseClick();
		virtual void OnMouseDown();
		virtual void OnMouseUp();
		virtual void OnMouseOut();

		virtual void HandleMouseInput(CInput * mouseInput);

		void SetIsInteractionEnabled(bool isInteractionEnabled);
		bool GetIsInteractionEnabled();

		int GetId();
};
#endif