//===========================================================
// 
// 
//===========================================================

#include "InteractiveObjectList.h"
#include "Point.h"

#include "Application.h"

static CInteractiveObjectList * _instance = NULL;

CInteractiveObjectList* CInteractiveObjectList::GetInstance()
{
	if (!_instance)
	{
		_instance = new CInteractiveObjectList();
	}

	return _instance;
}
void CInteractiveObjectList::DestroyInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = NULL;
	}
}

void CInteractiveObjectList::Clear()
{
	_interactiveList.clear();
}

void CInteractiveObjectList::Add(MouseInterface * object)
{
	Remove(object);
	_interactiveList.push_back(object);
}
void CInteractiveObjectList::Remove(MouseInterface * object)
{
	std::vector<MouseInterface*>::iterator it;
	for (it = _interactiveList.begin(); it != _interactiveList.end(); it++)
	{
		if (*it == object)
		{
			_interactiveList.erase(it);
			break;
		}
	}
}

void CInteractiveObjectList::Update()
{
	std::vector<MouseInterface*>::iterator it;
	for (it = _interactiveList.begin(); it != _interactiveList.end(); it++)
	{
		(*it)->HandleMouseInput(Control::Input);
	}
}



CInteractiveObjectList::CInteractiveObjectList()
{

}

CInteractiveObjectList::~CInteractiveObjectList()
{
	_interactiveList.clear();
}
