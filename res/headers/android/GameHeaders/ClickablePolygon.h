//===========================================================
// UI Button Object using mouseinterface
// Faisal
//===========================================================

#ifndef CLICKABLEPOLYGON_H
#define CLICKABLEPOLYGON_H

#include "MouseInterface.h"
#include "Polygon.h"
#include <functional>

class ClickablePolygon : public CPolygon, MouseInterface
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

		ClickablePolygon();
		ClickablePolygon(int id, int numPoints, int xyArr[], std::function<void (int)> mouseClickCallback = nullptr);
		ClickablePolygon(int id, int numPoints, int xArr[], int yArr[], std::function<void (int)> mouseClickCallback = nullptr);
		virtual ~ClickablePolygon();

		void SetMouseClickCallback(std::function<void (int)> mouseClickCallback);
		void SetMouseDownCallback(std::function<void (int)> mouseDownCallback);
		void SetMouseOverCallback(std::function<void (int)> mouseOverCallback);
		void SetMouseOutCallback(std::function<void (int)> mouseOutCallback);
		void SetMouseUpCallback(std::function<void (int)> mouseUpCallback);

		void HandleMouseInput(CInput * mouseInput);

		void SetIsInteractionEnabled(bool isInteractionEnabled);
		bool GetIsInteractionEnabled();
		MouseState GetCurrentMouseState();
		int GetId();

		virtual void OnMouseOver();
		virtual void OnMouseClick();
		virtual void OnMouseDown();
		virtual void OnMouseUp();
		virtual void OnMouseOut();
};
#endif