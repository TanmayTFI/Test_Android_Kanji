//===========================================================
// 
// 
//===========================================================

#ifndef INTERACTIVE_OBJECT_LIST_H
#define INTERACTIVE_OBJECT_LIST_H

#include "Framework.h"
#include "MouseInterface.h"


class CInteractiveObjectList
{
	public:	

		
		static CInteractiveObjectList* GetInstance();
		static void	DestroyInstance();

		void Clear();

		void Add(MouseInterface * object);
		void Remove(MouseInterface * object);

		void Update();		

	
	private:
		CInteractiveObjectList();
		~CInteractiveObjectList();
		std::vector<MouseInterface *> _interactiveList;
};

#endif