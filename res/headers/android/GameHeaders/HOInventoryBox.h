#ifndef HO_INVENTORY_BOX_H
#define HO_INVENTORY_BOX_H


#include "Object.h"
#include "Rect.h"
#include "Control.h"
#include "InventoryItem.h"
#include "Profile.h"
#include "InventoryItemInterface.h"

class HOInventoryBox : public UpdateListEntity , InventoryItemInterface
{

	private:

	std::vector<InventoryItem*> _listOfInventoryItems;
	std::vector<InventoryItem*> _listOfLoadedInventoryItems;
	int _maxVisibleItems;
	CRectangle _openBoxRect;
	int _lastLoadedItemIndex;
//	CPoint _originItemPos;
	float _dxBetweenItems;
	CPoint _position; 
	bool _isEnabled;
	bool _isTouchEnabled;
	InventoryItem * _currentDraggingItem;

public:
	HOInventoryBox(CRectangle openBoxRect, int maxVisibleItems);
	~HOInventoryBox();

	void Update(float ds); //UpdateListEntity pure virtual fn
	void HandleMouseInput(CInput * mouseInput);
	void UpdateInventoryChangesToProfile(int (&invItemStates) [MAX_HOITEMS], int itemId = -1);

	void AddInventoryItem(InventoryItem * invItem);
	void CollectItem(int index, CPoint itemPos);
	void ReturnItem(int index, CPoint itemPos);
	void UseItem(int index);

	void LoadInventoryItemsToVisibleArea(int startingIndex, int numItems);
	void LoadInventoryItemToVisibleArea(InventoryItem* invItem);
	void LoadNextInventoryItemToVisibleArea();
	int	 GetNextInventoryItemID();
	void LoadInventoryItemStates(int invItemStates[]);

	void UnloadInventoryItemFromVisibleArea(InventoryItem* invItem);
	void UnloadInventoryItemFromVisibleArea(int itemId);

	void SetisEnabled(bool enable);
	bool GetisEnabled();
	void SetisTouchEnabled(bool enable);
	bool GetisTouchEnabled();

	bool GetIsDraggingByMouseClickLock();
	bool GetIsDraggingByMouseDown();


	InventoryItemState GetInventoryItemStateOf(int itemId);
	CPoint GetItemPositionForIndex(int index);
	CPoint GetPosition();
	InventoryItem* GetInventoryItem(int index);
	int GetLastLoadedItemIndex();

	void SetPosition(CPoint position);
	void SetPosition(float x, float y, float z);

	void ReArrangeItemsInVisibleArea();
	void OnInventoryItemClick(int invItemType);
	void UpdateInventoryChangesToProfile(int itemId);

};

#endif