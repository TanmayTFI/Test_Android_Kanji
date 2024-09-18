//===========================================================
// 
// 
//===========================================================

#include "RenderListEntity.h"
#include "Point.h"
#include "Constants.h"
#include "GameFlow.h"
#include "Application.h"

static bool SortObjects(CRenderListEntity* pObj1, CRenderListEntity* pObj2);

CRenderList* CRenderList::Instance = NULL;

void CRenderList::Create()
{
	if(CRenderList::Instance == NULL)
		CRenderList::Instance = new CRenderList();
}

void CRenderList::Destroy()
{
	if(CRenderList::Instance != NULL)
	{
		delete CRenderList::Instance;
		CRenderList::Instance = NULL;
	}
}


CRenderList::CRenderList()
{
	Sort = true;
	Dirty = true;
	DebugDraw = false;
	PermanentCount = 0;
}

CRenderList::~CRenderList()
{
}

int CRenderList::Add( CRenderListEntity* pObj )
{
	Dirty = true;

	ObjList.push_back(pObj);
	
	return PermanentCount++;
}

void CRenderList::Remove( int val )
{
	for(std::vector<CRenderListEntity*>::iterator b = ObjList.begin(); b != ObjList.end(); ++b)
	{
		if( (*b)->RenderValue == val )
		{
			ObjList.erase(b);
			break;
		}
	}
}


CPoint cam(0,0,0);
void CRenderList::Render()
{
	if( Dirty && Sort )
		SortList();

	int size = (int)ObjList.size();
	for(int i=0; i < size; ++i)
	{
		if( ObjList[i]->Visible )
			ObjList[i]->Render(&cam);
	}

	Dirty = false;

#ifdef DEBUG
	GFApp->lpFontSmall->drawStringFromLeft(GFApp->debugMsg.c_str(), 100, 50);
#endif // DEBUG

	//draws a black screen when exiting the game (prevents screen glitch)
    #ifdef TOUCH_UI
	if(!GFApp->AppActive)
	{
		KGraphic *lpExitGraphics;
		lpExitGraphics = KPTK::createKGraphic ();
		lpExitGraphics->drawRect(0, 0, MAX_DISPLAY_WIDTH, MAX_DISPLAY_HEIGHT, 0, 0, 0, 1);
	}
	/*
	std::string str = to_string(GFApp->noOfHints);
	std::string str2 =  to_string(ProfileSystem::Instance->NoOfHints);
	std::string unl = to_string(GFApp->unlocked);
	std::string str3 = str + ", P:" +str2 + ", Unlocked:" + unl;
	GFApp->lpFont20->drawStringFromLeft(str3.c_str(), 100, 600);
	*/
    #endif
}

int CRenderList::VisObjSize()
{
	int nVisObj = 0;
	int nObj = (int)ObjList.size();
	for(int i=0; i < nObj; ++i)
	{
		if( ObjList[i]->Visible )
			nVisObj++;
	}
	return nVisObj;
}

void CRenderList::Clear()
{
	if(CRenderList::Instance != NULL)
	{
		CRenderList::Instance->ObjList.clear();
	}
}

void CRenderList::EnableSort(bool f)
{
	Sort = f;
}



void CRenderList::SortList()
{
	std::sort(ObjList.begin(), ObjList.end(), SortObjects);
}

static bool SortObjects(CRenderListEntity* pObj1, CRenderListEntity* pObj2)
{
	return pObj1->ZOrder < pObj2->ZOrder;
}
