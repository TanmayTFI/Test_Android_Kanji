#ifndef INVENTORY_MOUSE_INTERFACE_H
#define INVENTORY_MOUSE_INTERFACE_H
#include "Input.h"
enum MouseState
{
	eMouseState_Default = 0,
	eMouseState_Down,
	eMouseState_Up,
	eMouseState_Over,
	eMouseState_Out,
	eMouseState_Click
};
class MouseInterface
{

public:
	virtual void HandleMouseInput(CInput * mouseInput) = 0;
};
#endif

