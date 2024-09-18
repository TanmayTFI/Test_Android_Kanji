#include "InventoryItem.h"
#include "GameFlow.h"
#include "InventoryItemInterface.h"

InventoryItem::	InventoryItem(CObject * notCollectedIcon, CObject * collectedIcon, int itemId, CPoint clickOffset, InventoryItemInterface* interfaceObject, std::string vfxPath, std::vector<int> collectItemSFXIds, int returnItemSFXId, int useItemSFXId)
{
	_notCollectedIcon = notCollectedIcon;
	_collectedIcon = collectedIcon;
	_clickOffset = clickOffset;
	_itemClickArea.w = clickOffset.x;
	_itemClickArea.h = clickOffset.y;
	_itemState = kInventoryItemState_NotLoaded;
	_interfaceObject = interfaceObject;
	_itemId = itemId;
	_bezierTimer = 0.0f;
	_bezierTimerSpeed = 0;
	_isVisible = true;
	_trailVFX = new SuperFX(vfxPath, PointSystem::CreateCPoint(249,349,0), 1);
	_trailVFX->Show(false);
	_collectItemSFXIds = collectItemSFXIds;
	_useItemSFXId = useItemSFXId;
	_returnItemSFXId = returnItemSFXId;
	_isDraggingByMouseClickLock = false;
	_isDraggingByMouseDown = false;
	_inventoryBox = NULL;
}

InventoryItem::~InventoryItem()
{
	SAFE_DELETE(_notCollectedIcon);
	SAFE_DELETE(_collectedIcon);
	SAFE_DELETE(_trailVFX);
	_inventoryBox = NULL;
}

bool InventoryItem::Init()
{
	SetItemState(kInventoryItemState_NotLoaded);
	return true;
}

void InventoryItem::SetItemState(InventoryItemState itemState)
{
	_notCollectedIcon->SetAlpha(1);
	_collectedIcon->SetAlpha(1);
	
	_itemState = itemState;

	switch (itemState)
	{
	case kInventoryItemState_NotLoaded:
		{
			_notCollectedIcon->Show(false);
			_collectedIcon->Show(false);
		}
		break;
	case kInventoryItemState_NotCollected:
		{
			_notCollectedIcon->Show(true);
			_collectedIcon->Show(false);
			_interfaceObject->UpdateInventoryChangesToProfile(_itemId);

		}
		break;
	case kInventoryItemState_Collected:
		{
			_notCollectedIcon->Show(false);
			_collectedIcon->Show(true);
			_interfaceObject->UpdateInventoryChangesToProfile(_itemId);

		}
		break;
	case kInventoryItemState_Using:
		{
			_notCollectedIcon->Show(true);
			_collectedIcon->Show(false);
		}
		break;

	case kInventoryItemState_Collecting:
		{
			_notCollectedIcon->Show(true);
			_collectedIcon->Show(true);
			//_collectedIcon->SetAlpha(0);
		}
		break;
	case kInventoryItemState_LoadingToInventoryBox:
		{
			_notCollectedIcon->Show(true);
			_collectedIcon->Show(false);
			_notCollectedIcon->SetAlpha(0);
		}
		break;
	case kInventoryItemState_UnloadingFromInventoryBox:
		{
			_collectedIcon->Show(true);
			_notCollectedIcon->Show(false);
		}
		break;
	case kInventoryItemState_UnloadCompleted:
		{
			_collectedIcon->Show(false);
			_notCollectedIcon->Show(false);
			_interfaceObject->UpdateInventoryChangesToProfile(_itemId);
		}
	default:
		break;
	}

	_prevVisibilityOfCollectedIcon = _collectedIcon->Visible;
	_prevVisibilityOfNotCollectedIcon = _notCollectedIcon->Visible;
}

InventoryItemState InventoryItem::GetItemState()	
{
	return _itemState;
}

void InventoryItem::HandleMouseInput(CInput * mouseInput, bool skipNewMouseClick)
{
	

	if (skipNewMouseClick && _itemState == kInventoryItemState_Using)
	{
		if(Intersection(mouseInput->Mpos()))
		{
			if (mouseInput->LBclicked() && !_isDraggingByMouseClickLock && _isDraggingByMouseDown)
			{
				_isDraggingByMouseDown = false;
				_isDraggingByMouseClickLock = true;
				mouseInput->ClearMouse();
			}
		}
	}
	
	 if(Intersection(mouseInput->Mpos()))
	{
		if (!skipNewMouseClick)
		{
			OnMouseOver();
			if(mouseInput->LBdown())
			{
				_isDraggingByMouseDown = true;
				_isDraggingByMouseClickLock = false;
				OnMouseClick();
			}
		}	
	}
	else
	{
		OnMouseOut();
	}


	
}

bool InventoryItem::GetIsDraggingByMouseDown()
{
	return _isDraggingByMouseDown;
}

bool InventoryItem::GetIsDraggingByMouseClickLock()
{
	return _isDraggingByMouseClickLock;
}


void InventoryItem::OnMouseOver()
{
	_notCollectedIcon->SetScale(1.2f);
	_collectedIcon->SetScale(1.2f);
	Cursor::SetMode(CA_HandPointy);
}

void InventoryItem::OnMouseClick()
{
	if (_itemState == kInventoryItemState_Collected)
	{
		UseItem();
		_interfaceObject->OnInventoryItemClick(_itemId);
		if (_inventoryBox)
		{
			_inventoryBox->OnInventoryItemClick(_itemId);
		}
	}
}

void InventoryItem::OnMouseOut()
{
	if (_itemState == kInventoryItemState_Collected || _itemState == kInventoryItemState_NotCollected)
	{
		_collectedIcon->SetScale(1);
	}
			_notCollectedIcon->SetScale(1);

	//Cursor::SetMode(CA_Normal);
}

void InventoryItem::Update(float ds)
{
	switch (_itemState)
	{

	case kInventoryItemState_Collecting:
		{
			UpdateCollectingItemEffect();
		}
		break;
	case kInventoryItemState_LoadingToInventoryBox:
		{
			UpdateLoadingEffect();
		}
		break;
	case kInventoryItemState_UnloadingFromInventoryBox:
		{
			UpdateUnloadingEffect();
			if (true)//TODO check unloading completed
			{
				SetItemState(kInventoryItemState_UnloadCompleted);
			}
		}
		break;

	case kInventoryItemState_Using:
	case kInventoryItemState_NotCollected:
	case kInventoryItemState_UnloadCompleted:
	case kInventoryItemState_Collected:
	case kInventoryItemState_NotLoaded:
		break;

	default:
		break;
	}
}

void InventoryItem::UseItem()
{
	Control::Audio->QuickLoadAndPlaySFX((eAudio)_useItemSFXId);
	SetItemState(kInventoryItemState_Using);
}
void InventoryItem::LoadItemToInventoryBox()
{
	StartLoadingToInventoryBox();
}
void InventoryItem::UnloadItemFromInventoryBox()
{
	_isDraggingByMouseClickLock = false;
	_isDraggingByMouseClickLock = false;
	StartUnloadingFromInventoryBox();
}
void InventoryItem::CollectItem(CPoint bezierStartPoint)
{
	BezierFrom(bezierStartPoint);
	
	int randId = rand() % _collectItemSFXIds.size();
	int sfxId = _collectItemSFXIds.at(randId);
	Control::Audio->QuickLoadAndPlaySFX((eAudio)sfxId);	
}

void InventoryItem::ReturnItem(CPoint bezierStartPoint)
{
	_isDraggingByMouseClickLock = false;
	_isDraggingByMouseDown = false;
	Control::Audio->QuickLoadAndPlaySFX((eAudio)_returnItemSFXId);
	BezierFrom(bezierStartPoint);
}

void InventoryItem::BezierFrom(CPoint bezierStartPoint)
{
	_bezierControlPoints.clear();
	_collectedIcon->SetXY(bezierStartPoint.x, bezierStartPoint.y);
	_trailVFX->SetXY(bezierStartPoint.x, bezierStartPoint.y);
	_trailVFX->Start();
	_trailVFX->Show(true);
	_bezierTimer = 0;
	_bezierControlPoints.push_back(bezierStartPoint);

	

	//To multiply with Logic Rate, therby slowing down the flying we take inverse value
	CPoint ControlPos;
	ControlPos.x = (_position.x + bezierStartPoint.x)*0.5f;// mid point
	ControlPos.y = (_position.y + bezierStartPoint.y)*0.5f;// mid point

	int distance = (int)sqrt( ((_position.x - bezierStartPoint.x) * (_position.x - bezierStartPoint.x)) + ((_position.y - bezierStartPoint.y) * (_position.y - bezierStartPoint.y)) );

	float randValue = (float)(rand() % (int)(1.5 * distance));

    float maxDistance = 1200.0f; //approx


	_bezierTimerSpeed =	Elixir::Lerp(0.015f, 0.01f, (distance / maxDistance));
	

	if( bezierStartPoint.x > _position.x )
	{
		ControlPos.x -= randValue;
	}
	else
	{
		ControlPos.x += randValue;
	}
	_bezierControlPoints.push_back(ControlPos);
	_bezierControlPoints.push_back(_position);

	if (_itemState == kInventoryItemState_NotCollected || _itemState == kInventoryItemState_Using)
	{
			StartCollectingItemEffect();
	}	
}


void InventoryItem::StartLoadingToInventoryBox()
{
	SetItemState(kInventoryItemState_LoadingToInventoryBox);
}
void InventoryItem::UpdateLoadingEffect()
{
	_notCollectedIcon->SetAlpha(_notCollectedIcon->pSprite->Alpha + 0.05f);//TODO:
	if(_notCollectedIcon->pSprite->Alpha > 0.99f)
	{
		SetItemState(kInventoryItemState_NotCollected);
	}
}

void InventoryItem::StartUnloadingFromInventoryBox()
{
	SetItemState(kInventoryItemState_UnloadingFromInventoryBox);
}
void InventoryItem::UpdateUnloadingEffect()
{
	_collectedIcon->SetAlpha(_collectedIcon->pSprite->Alpha - 0.05f);//TODO:
	if(_collectedIcon->pSprite->Alpha < 0.0f)
	{
		OnUnloadingComplete();
	}
}

void InventoryItem::OnUnloadingComplete()
{
	SetItemState(kInventoryItemState_UnloadCompleted);
}

void InventoryItem::StartCollectingItemEffect()
{
	SetItemState(kInventoryItemState_Collecting);

}
void InventoryItem::UpdateCollectingItemEffect()
{
	_collectedIcon->Show(true);
	_collectedIcon->SetAlpha(1);
	float timer = (_bezierTimer > 1.0f)? 1 : _bezierTimer;

	CPoint pos = Elixir::GetBezierPoint(&_bezierControlPoints.at(0), &_bezierControlPoints.at(1), &_bezierControlPoints.at(2), timer);
	_collectedIcon->SetXY(pos.x, pos.y);
	_trailVFX->ResetPos(&pos);

	if(_bezierTimer < 0.5)
	{
		_collectedIcon->SetScale(_collectedIcon->GetScale() + 0.05f);
	}
	else
	{
		_collectedIcon->SetScale(_collectedIcon->GetScale() - 0.05f);
	}

	if(_collectedIcon->GetScale() < 1.0f)
	{
		_collectedIcon->SetScale(1);
	}
	if(_collectedIcon->GetScale() > 2.5f)
	{
		_collectedIcon->SetScale(2.5f);
	}

	if(_bezierTimer > 1.2f)
	{
		OnItemCollected();
	}
	_bezierTimer += _bezierTimerSpeed;

}
void InventoryItem::OnItemCollected()
{
	//_trailVFX->Show(false);
	_trailVFX->StopEmit();
	SetItemState(kInventoryItemState_Collected);
}

void InventoryItem::SetIsVisible(bool visible)
{
	
	_isVisible = visible;
	if(_isVisible)
	{
		_collectedIcon->Visible = _prevVisibilityOfCollectedIcon;
		_notCollectedIcon->Visible = _prevVisibilityOfNotCollectedIcon;
	}
	else
	{
		_prevVisibilityOfCollectedIcon = _collectedIcon->Visible;
		_prevVisibilityOfNotCollectedIcon = _notCollectedIcon->Visible;
		_collectedIcon->Visible = false;
		_notCollectedIcon->Visible = false;
	}
}

bool InventoryItem::GetIsVisible()
{
	return _isVisible;
}

CPoint InventoryItem::GetPosition()
{
	return _position;
}

void InventoryItem::SetPosition(CPoint position)
{
	SetX(position.x);
	SetY(position.y);
	SetZ((int)position.z);
}

void InventoryItem::SetX(float x)
{
	_position.x = x;
	_notCollectedIcon->SetX(x);
	_collectedIcon->SetX(x);
}

void InventoryItem::SetY(float y)
{
	_position.y = y;
	_notCollectedIcon->SetY(y);
	_collectedIcon->SetY(y);
}

void InventoryItem::SetZ(int z)
{
	_trailVFX->SetZ(z + 2);
	_position.z = (float)z;
	_notCollectedIcon->SetZ(z);
	_collectedIcon->SetZ(z + 1);
}

float InventoryItem::GetX()
{
	return _position.x;
}

float InventoryItem::GetY()
{
	return _position.y;
}

int InventoryItem::GetZ()
{
	return (int)_position.z;
}

int InventoryItem::GetItemId()
{
	return _itemId;
}

bool InventoryItem::Intersection(CPoint point)
{
	_itemClickArea.topleft.x = _position.x - (_clickOffset.x / 2.0f);
	_itemClickArea.topleft.y = _position.y - (_clickOffset.y / 2.0f);

	return(_itemClickArea.Intersection(&point));
}

void InventoryItem::SetInventoryBox(InventoryItemInterface * inventoryBox)
{
	_inventoryBox = inventoryBox;
}