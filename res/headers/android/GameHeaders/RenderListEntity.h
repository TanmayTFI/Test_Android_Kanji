//===========================================================
// 
// Framework
//===========================================================

#ifndef RENDER_LIST_ENTITY_H
#define RENDER_LIST_ENTITY_H

#include "RenderList.h"

class CPoint;

class CRenderListEntity
{
	public:
		bool Visible;
		int ZOrder;
		int RenderValue;
		CRenderListEntity()
		{
			Visible = false;//default init!!
			ZOrder = 0;
			RenderValue = CRenderList::Instance->Add(this);
		}
		virtual ~CRenderListEntity()
		{
			Visible = false;
			CRenderList::Instance->Remove(RenderValue);
		}

		virtual void Render(const CPoint* pCam) = 0;
		virtual void SetZ(int z)
		{
			ZOrder = z;
			CRenderList::Instance->Dirty = true;
		}
};

#endif