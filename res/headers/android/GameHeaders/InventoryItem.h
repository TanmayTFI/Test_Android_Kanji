#ifndef INVENTORYITEM_H
#define INVENTORYITEM_H

#include "Object.h"
#include "Rect.h"
#include "Control.h"
#include <functional>
#include "SuperFX.h"
enum InventoryItemState
{
	kInventoryItemState_NotLoaded = 0,
	kInventoryItemState_LoadingToInventoryBox,
	kInventoryItemState_NotCollected,
	kInventoryItemState_Collecting,
	kInventoryItemState_Collected,
	kInventoryItemState_Using,
	kInventoryItemState_UnloadingFromInventoryBox, 
	kInventoryItemState_UnloadCompleted
};


class InventoryItemInterface;

class InventoryItem : public UpdateListEntity
{

	private:
		InventoryItemInterface* _interfaceObject;
		InventoryItemInterface* _inventoryBox;

		InventoryItemState _itemState;
		CObject * _notCollectedIcon;
		CObject * _collectedIcon;
		int 	 _itemId;
		CPoint _clickOffset;
		CRectangle _itemClickArea;
		CPoint _position;
		std::vector<CPoint> _bezierControlPoints;
		float _bezierTimer;
		float _bezierTimerSpeed;
		bool _isVisible;
		bool _prevVisibilityOfCollectedIcon;
		bool _prevVisibilityOfNotCollectedIcon;
		SuperFX *_trailVFX;
		std::vector<int> _collectItemSFXIds;
		int _returnItemSFXId;
		int _useItemSFXId;
		bool _isDraggingByMouseClickLock;
		bool _isDraggingByMouseDown;
		int _mouseDownCounter;
		

	public:
	InventoryItem(CObject * notCollectedIcon, CObject * collectedIcon, int itemId, CPoint clickOffset, InventoryItemInterface* interfaceObject, std::string vfxPath, std::vector<int> collectItemSFXIds, int returnItemSFXId, int useItemSFXId);
	~InventoryItem();
	bool Init();
	InventoryItemState GetItemState();
	void OnMouseOver();
	void OnMouseClick();
	void OnMouseOut();
	void HandleMouseInput(CInput * mouseInput, bool skipNewMouseClick = false);
	bool GetIsDraggingByMouseDown();
	bool GetIsDraggingByMouseClickLock();

	void Update(float ds); //UpdateListEntity pure virtual fn
	void SetItemState(InventoryItemState itemState);

	
	void UseItem();
	void LoadItemToInventoryBox();
	void UnloadItemFromInventoryBox();
	void CollectItem(CPoint bezierStartPoint);
	void ReturnItem(CPoint bezierStartPoint);

	void SetIsVisible(bool visible);
	bool GetIsVisible();

	CPoint GetPosition();
	void SetPosition(CPoint position);
	void SetX(float x);
	void SetY(float y);
	void SetZ(int z);
	float GetX();
	float GetY();
	int GetZ();
	int GetItemId();
	bool Intersection(CPoint point);

	void SetInventoryBox(InventoryItemInterface * inventoryBox);

protected:


	void StartLoadingToInventoryBox();//load item (visually) to inventory box
	void UpdateLoadingEffect();

	void StartUnloadingFromInventoryBox();// Unload item (visually) from inventory box 
	void UpdateUnloadingEffect();
	void OnUnloadingComplete();

	void StartCollectingItemEffect();
	void UpdateCollectingItemEffect();
	void OnItemCollected();
	void BezierFrom(CPoint bezierStartPoint);
};

#endif