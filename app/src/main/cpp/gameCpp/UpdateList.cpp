//====================================
// Framework
// UpdateList Class
//====================================

#include "UpdateListEntity.h"

UpdateList* UpdateList::Instance = NULL;

void UpdateList::Create()
{
	if(UpdateList::Instance == NULL)
		UpdateList::Instance = new UpdateList();
}

void UpdateList::Destroy()
{
	if(UpdateList::Instance != NULL)
	{
		delete UpdateList::Instance;
		UpdateList::Instance = NULL;
	}
}

UpdateList::UpdateList()
{
	Paused = false;
	PermanentCount = 0;
}

UpdateList::~UpdateList()
{
}

int UpdateList::Add( UpdateListEntity* pObj )
{
	ObjList.push_back(pObj);
	
	return PermanentCount++;
}

void UpdateList::Remove( int val )
{
	for(std::vector<UpdateListEntity*>::iterator b = ObjList.begin(); b != ObjList.end(); ++b)
	{
		if( (*b)->GetUpdateValue() == val )
		{
			ObjList.erase(b);
			break;
		}
	}
}

void UpdateList::Update(float ds)
{
	if( Paused )
		return;
	for(int i=0; i < (int)(ObjList.size()); ++i)	
	{
		ObjList[i]->Update(ds);
	}
}

void UpdateList::Clear()
{
	if(UpdateList::Instance != NULL)
	{	
		UpdateList::Instance->ObjList.clear();
	}
}

