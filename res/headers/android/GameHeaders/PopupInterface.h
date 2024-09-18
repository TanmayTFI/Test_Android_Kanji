

#ifndef Popup_Interface_
#define Popup_Interface_

#include "Popup.h"

class PopupInterface
{
public:

	virtual void UpdateHOSystem(Popup *popup, int UpdateVal = -1) = 0;
	virtual void NewArea() = 0;
};

#endif