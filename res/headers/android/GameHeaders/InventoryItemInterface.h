#ifndef INVENTORY_ITEM_INTERFACE_H
#define INVENTORY_ITEM_INTERFACE_H
class InventoryItemInterface
{
public:
	virtual void OnInventoryItemClick(int itemIndex) = 0;
	virtual void UpdateInventoryChangesToProfile(int itemId) = 0;
};
#endif

