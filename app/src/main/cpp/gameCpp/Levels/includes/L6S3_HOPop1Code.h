

#ifndef DQFYLH_L6S3_HO_POPUP1_CODE_DEF
#define DQFYLH_L6S3_HO_POPUP1_CODE_DEF

//====================================
//  created by : Sajith
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"

class L6S3_HOPop1Code: public Popup
{
public:
	L6S3_HOPop1Code(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L6S3_HOPop1Code();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();
	void ProcessSkip();
protected:
	SuperFX *S3_HO_PuzzleEffectFX;
	CObject		*PuzzleSymbols[4];
	CObject		*PuzzleButtons[9];
	CRectangle	PuzzleRect[9];
	int			IndexArray[4];
	int			IndexCount;
	//void HandleInventoryDrop(CPoint dropPos,int itemVal );
	void OnPopupCloseBtnClicked();
	void OnPopupOpened();
	bool CheckIfPuzzleSolved();
	bool CheckIfValid();
	void ResetPuzzle();
};

#endif
