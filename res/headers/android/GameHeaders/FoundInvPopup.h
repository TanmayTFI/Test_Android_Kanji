//====================================
// Framework
// Inventory Control
//====================================

#ifndef DQFULP_INVENTORY_DEF
#define DQFULP_INVENTORY_DEF

#include "Control.h"
#include "Constants.h"
#include "SConstants.h"
#include "BitmapText.h"
#include "Fonts.h"


enum eInventoryState
{
	FoundInvPopup_Inactive,
	FoundInvPopup_TransIn,
	FoundInvPopup_Idle,
	FoundInvPopup_Waiting,
	FoundInvPopup_TransOut
};

class CObject;
class CHOObject;

class FoundInvPopup : public Control
{
	public:
		void CreateFoundInvSprText();
		CBitmapText   *FoundInvSprText, *caseLabel;
		int InventorySize;
		FoundInvPopup( int InvSize );
		virtual ~FoundInvPopup();

		void FlyToInvBoxSlot(int iObj, int iSlot);

		bool IsEmptyInvBoxSlot(int i);
		int  GetFreeSlotPos( int size );
		bool InvRectClicked;

		//Inherited method
		void Update();
		void Required();

		//Gonna need Process Message
		void Close();
		void Activate(std::vector<int>* vecInvIds);
		void ActivateDirect(std::vector<int>* vecInvIds, CPoint pos);

		eInventoryState State;
		float WaitingCloseTimer;
		bool IsWaitingClose;

		std::vector<CHOObject *> getFoundObjects(){return FIHOObjects;};
		void disposeObjects();
	protected:
		
		CObject	  *Case, *BkGndBlackMaskSpr;

		std::vector<CHOObject *> FIHOObjects;

		
		void Show(bool f);
		void ShowDirect(bool f);

		float Alpha;

		void UpdateTransIn();
		void UpdateTransOut();
};

#endif