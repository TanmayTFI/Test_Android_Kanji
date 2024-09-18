//===========================================================
// 
// Framework
//===========================================================

#include "HOScene.h"
#include "IOStream.h"

CHOScene::CHOScene()
{
	IsShaking = false;
}

CHOScene::~CHOScene()
{
}


CHOObject* CHOScene::pCollideWithHO(const CPoint* pMouse)
{
	int size = (int)Objects.size();
	for(int i=0; i < size; ++i)
	{
		if( Objects[i]->Type == HIDDEN_OBJECT )
		{
			if( Objects[i]->Collide(pMouse) )
			{
				CHOObject *pObj = reinterpret_cast<CHOObject*> (Objects[i]);
				return pObj;
			}
		}
	}

	return NULL;
}


CObject* CHOScene::pGetAvailableObject()
{
	int size = (int)Objects.size();
	int random = (int)(rand() % size);
	int step = random;

	do
	{
		if( Objects[step]->Type == HIDDEN_OBJECT )
		{
			if( Objects[step]->IsDirty() == false )
				return Objects[step];
		}

		step++;

		if( step >= size )
			step = 0;
	}
	while( random != step );

	return NULL;
}

//Cleans items that are marked as dirty
//but have not been deactivated yet.
void CHOScene::CleanDirtyHOs()
{
	int size = (int)Objects.size();
	for( int i=0; i < size; ++i)
	{
		if( Objects[i]->Type == HIDDEN_OBJECT )
		{
			if( Objects[i]->IsDirty()  &&
				Objects[i]->Visible )
			{
				Objects[i]->SetDirty(false);
			}
		}
	}
}

void CHOScene::SetZOff(int z)
{
	int size = (int)Objects.size();
	for( int i=0; i < size; ++i)
	{
		Objects[i]->SetZ(Objects[i]->ZOrder + z);
	}
}

void CHOScene::SetPosOff(float x, float y)
{
	CPoint pos;

	int size = (int)Objects.size();
	for( int i=0; i < size; ++i)
	{
		pos = Objects[i]->Pos;
		pos.x += x;
		pos.y += y;

		Objects[i]->SetPos(&pos);
	}
}

void CHOScene::FadeOut(float fadeSpeed)
{
	int size = (int)Objects.size();
	for( int i=0; i < size; ++i)
	{
		Objects[i]->FadeOut(fadeSpeed);
	}
}

void CHOScene::FadeIn(float fadeSpeed)
{
	int size = (int)Objects.size();
	for( int i=0; i < size; ++i)
	{
		Objects[i]->FadeIn(fadeSpeed);

	}
}

void CHOScene::SetEnable(bool enable)
{
	int size = (int)Objects.size();
	for( int i=0; i < size; ++i)
	{
		Objects[i]->SetEnable(enable);
	}
}

void CHOScene::PauseObjects(bool paused)
{
	int size = (int)Objects.size();
	for( int i=0; i < size; ++i)
	{
		Objects[i]->Paused = paused;
	}
}

void CHOScene::PushObject(CObject* obj)
{
	Objects.push_back(obj);
}

void CHOScene::InsertObject(CObject* obj, int offset)
{
	std::vector<CObject *>::iterator it = Objects.begin();
	Objects.insert ( it+offset , obj );
}

void CHOScene::PopObject()
{
	Objects.pop_back();	
}

void CHOScene::update(float ds)
{
	if(IsShaking)
	{
		ShakeTimer -= ds;
		ShiftXY((shakeOffX*0.5f-rand()%shakeOffX),(shakeOffY*0.5f-rand()%shakeOffY));
		if(ShakeTimer<0)
		{
			IsShaking = false;
			RestoreXY();
		}
	}
}

void CHOScene::Shake(int xOff, int yOff, float duration)
{
	if(!IsShaking)
	{
		SaveXY();
		IsShaking = true;
		shakeOffX = xOff;
		shakeOffY = yOff;
		ShakeTimer = duration;
	}
}

void CHOScene::RemoveObject( std::string UID )
{
	for( std::vector<CObject *>::iterator it = Objects.begin(); it != Objects.end(); ++it )
		if( (*it)->UID == UID )
		{
			Objects.erase(it);
			return;
		}
}

void CHOScene::RemoveObject( CObject *obj )
{
	RemoveObject(obj->UID);
}
