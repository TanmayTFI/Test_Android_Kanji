//====================================
// Framework
// UpdateList Class
//====================================

#ifndef FRMWRK_UPDATELIST_DEF
#define FRMWRK_UPDATELIST_DEF

#include "Framework.h"

class UpdateListEntity;

class UpdateList
{
	public:	
		
		//Singleton Instance
		static UpdateList* Instance;


		//Singleton Methods
		static void			Create();
		static void			Destroy();
		static void Clear();

		int Add( UpdateListEntity* pObj );
		void Remove( int val );

		void Update(float ds);
    int  ObjSize(){ return (int) ObjList.size(); }  //Sac: converted Implicit to explicit

		

		inline void Pause(bool f) { Paused = f; }
	
	private:
		UpdateList();
		~UpdateList();

		std::vector<UpdateListEntity *> ObjList;
		bool Paused;
		int PermanentCount;
};

#endif
