#include "HOInventoryBox.h"
#include "GameFlow.h"

HOInventoryBox::HOInventoryBox(CRectangle openBoxRect, int maxVisibleItems)
{
	_openBoxRect = openBoxRect;
	_maxVisibleItems = maxVisibleItems;
	_dxBetweenItems = _openBoxRect.w / _maxVisibleItems;
	_position.x = _openBoxRect.topleft.x + _dxBetweenItems / 2.0f;
	_position.y = _openBoxRect.GetCenterY();
	_isTouchEnabled = true;
	_isEnabled = true;
	_currentDraggingItem = NULL;
}

HOInventoryBox::~HOInventoryBox()
{
    int size = (int)_listOfLoadedInventoryItems.size();  //Sac: converted Implicit to explicit

	for (int i = 0; i < size; i++)
	{
		SAFE_DELETE(_listOfLoadedInventoryItems[i]);
	}
	_listOfInventoryItems.clear();

    size = (int)_listOfInventoryItems.size();  //Sac: converted Implicit to explicit

	for (int i = 0; i < size; i++)
	{
		SAFE_DELETE(_listOfInventoryItems[i]);
	}
	_listOfLoadedInventoryItems.clear();
	_currentDraggingItem = NULL;
}

void HOInventoryBox::Update(float ds)
{

}


void HOInventoryBox::AddInventoryItem(InventoryItem * invItem)
{
	if(!_isEnabled)
	{
		return;
	}
	_listOfInventoryItems.push_back(invItem);
	invItem->Init();
	invItem->SetPosition(GetItemPositionForIndex(_maxVisibleItems));// placing all objects right side of visibleArea
	invItem->SetInventoryBox(this);
}

void HOInventoryBox::LoadInventoryItemsToVisibleArea(int startingIndex, int numItems)
{
	if(!_isEnabled)
	{
		return;
	}
    int size = (int)_listOfInventoryItems.size(); //Sac: converted Implicit to explicit

	for (int i = startingIndex; i < startingIndex + numItems; i++)
	{
		if (i < size)
		{
			LoadInventoryItemToVisibleArea(_listOfInventoryItems[i]); 
		}
	}

}

int HOInventoryBox::GetNextInventoryItemID()
{
	if(!_isEnabled)
	{
		return -1;
	}
	int nextIndex = _lastLoadedItemIndex + 1;
    int size = (int)_listOfInventoryItems.size(); //Sac: converted Implicit to explicit

	if(nextIndex < size)
	{
		return nextIndex;
	}

	return -1;
}

void HOInventoryBox::LoadNextInventoryItemToVisibleArea()
{
	if(!_isEnabled)
	{
		return;
	}
	int nextIndex = _lastLoadedItemIndex + 1;
    int size = (int)_listOfInventoryItems.size();  //Sac: converted Implicit to explicit

	if(nextIndex < size)
	{
		LoadInventoryItemToVisibleArea(_listOfInventoryItems[nextIndex]);
	}
}

void HOInventoryBox::LoadInventoryItemToVisibleArea(InventoryItem* invItem)
{
	if(!_isEnabled)
	{
		return;
	}
    int size = (int)_listOfLoadedInventoryItems.size();  //Sac: converted Implicit to explicit

	if(size < _maxVisibleItems)
	{
		_listOfLoadedInventoryItems.push_back(invItem);
	}
    invItem->SetPosition(GetItemPositionForIndex((int)_listOfLoadedInventoryItems.size() - 1));  //Sac: converted Implicit to explicit

	invItem->LoadItemToInventoryBox();
	_lastLoadedItemIndex = invItem->GetItemId();
}

void HOInventoryBox::LoadInventoryItemStates(int invItemStates[])
{

	if(!_isEnabled)
	{
		return;
	}
    int size = (int)_listOfInventoryItems.size();  //Sac: converted Implicit to explicit

	for(int i = 0; i < size; i++)
	{

		InventoryItemState itemState = (InventoryItemState)invItemStates[i];
		if (itemState > kInventoryItemState_NotLoaded && itemState < kInventoryItemState_UnloadCompleted)
		{
			LoadInventoryItemsToVisibleArea(i, 1);
		}
		_listOfInventoryItems.at(i)->SetItemState((InventoryItemState)invItemStates[i]);

	}


}

void HOInventoryBox::UpdateInventoryChangesToProfile(int (&invItemStates) [MAX_HOITEMS], int itemId)
{
	if(!_isEnabled)
	{
		return;
	}
	if (itemId != -1)
	{
        invItemStates[itemId] = (int)_listOfInventoryItems.at(itemId)->GetItemState();  //Sac: converted Implicit to explicit

	}
	else
	{
        int size = (int)_listOfInventoryItems.size();  //Sac: converted Implicit to explicit

		for(int i = 0; i < size; i++)
		{
			invItemStates[i] = _listOfInventoryItems.at(i)->GetItemState();
		}
	}

}



void HOInventoryBox::SetisEnabled(bool enable)
{
	_isEnabled = enable;
	_isTouchEnabled = enable;
	std::vector<InventoryItem*>::iterator it;
	for (it = _listOfLoadedInventoryItems.begin(); it != _listOfLoadedInventoryItems.end(); it++)
	{
		(*it)->SetIsVisible(enable);
	}
}
bool HOInventoryBox::GetisEnabled()
{
	return _isEnabled;
}
void HOInventoryBox::SetisTouchEnabled(bool enable)
{
	_isTouchEnabled = enable;
}
bool HOInventoryBox::GetisTouchEnabled()
{
	return _isTouchEnabled;
}



void HOInventoryBox::CollectItem(int index, CPoint itemPos)
{
	if(!_isEnabled)
	{
		return;
	}
	
	_listOfInventoryItems.at(index)->CollectItem(itemPos);
}

void HOInventoryBox::ReturnItem(int index, CPoint itemPos)
{
	if(!_isEnabled)
	{
		return;
	}
	/*int randId = rand()%3;
	switch (randId)
	{
		case 0:		Control::Audio->QuickLoadAndPlaySFX(aSFXInvCollectv1);		break;
		case 1:		Control::Audio->QuickLoadAndPlaySFX(aSFXInvCollectv2);		break;
		case 2:		Control::Audio->QuickLoadAndPlaySFX(aSFXInvCollectv3);		break;
		default:
			break;
	}*/
	_listOfInventoryItems.at(index)->ReturnItem(itemPos);
	_currentDraggingItem = NULL;
}

void HOInventoryBox::UseItem(int index)
{
	if(!_isEnabled)
	{
		return;
	}
	_listOfInventoryItems.at(index)->UseItem();
}


void HOInventoryBox::UnloadInventoryItemFromVisibleArea(InventoryItem* invItem)
{
	if(!_isEnabled)
	{
		return;
	}
	std::vector<InventoryItem*>::iterator it;
	for (it = _listOfLoadedInventoryItems.begin(); it != _listOfLoadedInventoryItems.end(); it++)
	{
		if (*it == invItem)
		{
			_listOfLoadedInventoryItems.erase(it);
			break;
		}
	}
	ReArrangeItemsInVisibleArea();
}

void HOInventoryBox::UnloadInventoryItemFromVisibleArea(int itemId)
{
	if(!_isEnabled)
	{
		return;
	}
	InventoryItem* invItem = _listOfInventoryItems.at(itemId); 
	invItem->UnloadItemFromInventoryBox();
	std::vector<InventoryItem*>::iterator it;
	for (it = _listOfLoadedInventoryItems.begin(); it != _listOfLoadedInventoryItems.end(); it++)
	{
		if (*it == invItem)
		{
			_listOfLoadedInventoryItems.erase(it);
			break;
		}
	}
	ReArrangeItemsInVisibleArea();
}

InventoryItemState HOInventoryBox::GetInventoryItemStateOf(int itemId)
{
	return _listOfInventoryItems.at(itemId)->GetItemState();
}

CPoint HOInventoryBox::GetItemPositionForIndex(int index)
{
	CPoint point = CPoint(_position.x + (_dxBetweenItems * index), _position.y, _position.z);
	return point;
}

int HOInventoryBox::GetLastLoadedItemIndex()
{
	return _lastLoadedItemIndex;
}

CPoint HOInventoryBox::GetPosition()
{
	return _position;
}

void HOInventoryBox::SetPosition(CPoint position)
{
	_position = position;
}

void HOInventoryBox::SetPosition(float x, float y, float z)
{
	_position = CPoint(x, y, z);
}

void HOInventoryBox::ReArrangeItemsInVisibleArea()
{
	_currentDraggingItem = NULL;
    int size = (int)_listOfLoadedInventoryItems.size();  //Sac: converted Implicit to explicit

	for (int i = 0; i < size; i++)
	{
		_listOfLoadedInventoryItems[i]->SetPosition(GetItemPositionForIndex(i));
	}
}

void HOInventoryBox::HandleMouseInput(CInput * mouseInput)
{
	if (!_isTouchEnabled)
	{
		return;
	}
    int size = (int)_listOfLoadedInventoryItems.size(); //Sac: converted Implicit to explicit


	bool skipNewMouseClick = false;

	if (GetIsDraggingByMouseClickLock() || GetIsDraggingByMouseDown())
	{
		skipNewMouseClick = true;
	}

	for (int i = 0; i < size; i++)
	{
		_listOfLoadedInventoryItems[i]->HandleMouseInput(mouseInput, skipNewMouseClick);
	}
}

bool HOInventoryBox::GetIsDraggingByMouseDown()
{
	if (_currentDraggingItem)
	{
		return _currentDraggingItem->GetIsDraggingByMouseDown();
	}
	return false;
}

bool HOInventoryBox::GetIsDraggingByMouseClickLock()
{
	if (_currentDraggingItem)
	{
		return _currentDraggingItem->GetIsDraggingByMouseClickLock();
	}
	return false;
}


InventoryItem* HOInventoryBox::GetInventoryItem(int index)
{
	return _listOfInventoryItems.at(index);
}

void HOInventoryBox::OnInventoryItemClick(int invItemType)
{
	_currentDraggingItem = GetInventoryItem(invItemType);
}

void HOInventoryBox::UpdateInventoryChangesToProfile(int itemId)
{

}
