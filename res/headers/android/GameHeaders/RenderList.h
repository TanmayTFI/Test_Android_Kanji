//===========================================================
// 
// 
//===========================================================

#ifndef RENDER_LIST_H
#define RENDER_LIST_H

#include "Framework.h"

class CRenderListEntity;

class CRenderList
{
	public:	
		bool Dirty;

		
		//Singleton Instance
		static CRenderList* Instance;

		//Singleton Methods		
		static void	Create();
		static void	Destroy();
		static void Clear();

		int Add( CRenderListEntity* pObj );
		void Remove( int val );

		void Render();		
    int ObjSize(){ return (int)ObjList.size(); } //Sac: converted Implicit to explicit
		int VisObjSize();

		
		void EnableSort(bool f = true);
		void SortList();

		inline void SetDebugDraw(bool f) { DebugDraw = f; }
	
	private:
		CRenderList();
		~CRenderList();


		std::vector<CRenderListEntity *> ObjList;
		
		bool Sort;
		bool DebugDraw;

		int PermanentCount;
};

#endif
