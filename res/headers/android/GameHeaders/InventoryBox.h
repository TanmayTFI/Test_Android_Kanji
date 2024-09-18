//====================================
// Framework
// InvBox Control
//====================================

#ifndef DQFULP_INVBOX_DEF
#define DQFULP_INVBOX_DEF

#include "Control.h"
#include "ParticleEffect.h"
#include "SConstants.h"
#include "iPadDefs.h"
#include "Banners.h"
#include "BitmapText.h"
#include "LangDefs.h"


enum IBoxFrame { INVBOX_LR_FRAME, PUZBOX_FRAME, HOBOX_FRAME, MATCH3_FRAME, HO_USEOBJECTS_FRAME, HO_PLACEOBJECTS_FRAME, HO_FINDPAIRS_FRAME };

enum eInvBoxState
{
	InvBox_Inactive,
	InvBox_TransIn,
	InvBox_Idle,
	InvBox_TransOut	
};

enum eSlidingState
{
	Sliding_Nil,
	Sliding_Up,
	Sliding_Down
};

enum eFlyStatus 
{
	Inv_StandStill,
	Inv_Flying,
	Inv_WaitForVFX,
	Inv_StopFlying
};

class ParticleEffect;
class CObject;
class CHOObject;
class CBitmapText;

typedef struct INVBOX_OBJ
{
	CHOObject* HOObj;
	CRectangle Rectangle;


	int  nPiecePicked;
	int  MaxPieces;
	CBitmapText   *Text;	
}INVBOX_OBJ;

#define HELP_TEXT_LINES 7
class InventoryBox : public Control
{
	public:	
		CBitmapText   *HelpTexts[HELP_TEXT_LINES];
		void SetMultilineText(std::string bigstring, int OffsetY = 0,eTextAlignment AlignStyle = Align_Center, int OffsetX = 0);

		INVBOX_OBJ InvBoxObj[MAX_INVBOX_OBJECTS];
		void InitMultiPiece_InvBoxObj( int iItem, int eInvId, int nPiecesPicked, int MaxPieces );	
		void InitMultiPiece_InvBoxObj( int iItem, int eInvId, int nPiecesPicked, int MaxPieces, int nonRemappedeInvId );		
		void createMultiPiece_InvBoxObjTxt( int iItem );		
		void HandleMultiPiece_InvObjUpdate( int Slot, int eInvId );
		bool IsMultiPiece_InvBoxObj( int eInvId );
		int  GetMultiPiece_InvBoxObjSlot( int RemapInvId );
		int  GetRemapInvId( int eInvId );
/*
#ifdef TOUCH_UI
        int InitTouchX, InitTouchY;
#endif
 */
		int InvBoxLeftAnimatingTicks;
		int InvBoxRightAnimatingTicks;
		int HoverItemId;
		bool IsHoverItem;
		bool IsDragging;
		int  dragTicks;
		
		InventoryBox();
		virtual ~InventoryBox();

		void ShowInvBox(bool fading);
		void HideInvBox(bool fading);
		bool isInvBoxVisible;

		//Inherited method
		void Update();
		void Required();
		void ClearItemFromInvBox( int InvId );
		void CreateInvBoxItems();

		//Gonna need Process Message
		void Activate( int nFlyingObjects = 0 );
		void ExternalClose();
		void ReleaseDrag();
		void GlowDragInvObject(int ID, CRectangle rect);
		void CancelDrag();
		void EndDrag();
		void ResetDragging();
		
		void ShowDragInv( bool status);
		bool GetDropInfo(CPoint& hotspot, int& itemVal);
		int  GetMouseOverItem();		
		
		void AddPendingInvToInvBox(int eInvId);
		void PostUpdateAddingToInvBox(int eInvId);
		void AddToInvBox( int eInvId , CPoint *Pos = NULL );
		void UpdateAllComboInvItems();
		
		void ShiftInvBoxLeft( int ShiftCnt , bool instant = false);
		void ShiftInvBoxRight( int ShiftCnt , bool instant = false);

		void HandleFlyDrop( int i );
		void HandleDirectDrop( int i );

		bool IsOpen() { return (State == InvBox_Idle); }
		void SetLockBase( int Frame );
		void ResetToInvBox();

		void InitArrowsLock();
		void TurnOffArrowsLock();		
		void InitInvBoxUpIdle();
		void HidePuzzleTexts();

		bool IsForcedCloseItem( int ItemPublicValue );
		
		int  IsPuzzleModeActive;
		int	 DragIndex;
		bool IsLocked;		
		int  InvBoxFrame;


		CPoint Origin;
		CObject	  *InvBox, *IBLArr, *IBRArr, *IBLock;
		void Close();	
		bool   NoAutoClose;
		int   ItemFlyStatus;
		void FadeIn( float fade );
		void Deactivate();
		int   SlidingState;

		eInvBoxState State;

		void updateComboItemStatus(int invId);
		int GetActiveComboItemId();

		void ShowInvObj(bool f);
		CObject   *ToolTip;
		CBitmapText *ToolText;

		void CrossCheckInventoryItems();
		int SearchInventoryItem(int ID);
		int GetInvBoxLastIndex();
		float _timer;
		inline void CheckAndSetItem(int ID);
		inline void CheckAndSetItem(int ID, int ID2);
		inline void CheckAndSetItem(int ID, int ID2, int ID3);
		inline void CheckAndSetItem(int ID, int ID2, int ID3, int ID4);
		inline void CheckAndSetItem(int ID, int ID2, int ID3, int ID4, int ID5);
		inline void CheckAndSetItem(int ID, int ID2, int ID3, int ID4, int ID5, int ID6);

		void CheckForMissingItemDeadlocks();
		bool isAreaHOorPuzzle(int level, int area);

		CObject *Navigate, *NavArrow;
		CBitmapText *NavigateText;
		int travelArea;
		void GoToScene(int area, int mode, CRectangle rectArea = CRectangle (0,0,0,0));
		bool navTapped;
		CPoint tapPosition;
		float navRectX1, navRectX2, navRectY1, navRectY2;

	protected:
		

		int MouseOverItem;
		float MouseHoverTime;
		bool	IsToolTipShowing;

		void FadeInInvObj(float speed = 1.0f);
		void FadeOutInvObj(float speed = 1.0f);
		void IBSetPos(const CPoint* pos);

		int activeComboItemIndex;
		bool CheckIfItemIsComboInv(int invId);
		bool IsComboItemActive(int invId, int invPosID);		

		void UpdateTransIn();
		void UpdateTransOut();

		bool SlideDown();
		bool SlideUp();
};

#endif
