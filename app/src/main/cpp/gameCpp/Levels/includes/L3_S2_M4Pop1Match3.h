#ifndef DQFYLH_L3_S2_MAZE4_POP1_DEF
#define DQFYLH_L3_S2_MAZE4_POP1_DEF

//====================================
//  created by : Faisal Rasak
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"
#define MAX_MATCH3_ROWS 8
#define MAX_MATCH3_COLS 8
#define MAX_MATCH3_TILES 64
#define MAX_MATCH3_LEFTSLIDERS 8
#define MAX_MATCH3_RIGHTSLIDERS 8

#define MAX_MATCH3_SLIDERS 16
#define MAX_BREAK_ANIMS 64

#define MAX_MATCH3_VARIETY 6
class L3_S2_M4Pop1Match3 : public Popup
{
public:
	L3_S2_M4Pop1Match3(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L3_S2_M4Pop1Match3();

	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void Update(float ds );
	void Required();

	void SkipPuzzle();

	void PlaySuccessSFX();

	float easing(float t,float b,float c,float d);

	SuperFX *s2m4match3endFX;

	void InitPopup();
	void OnPopupOpened();
	void OnPopupCloseBtnClicked();

	void DisposeObjects();

	void InitPuzzle();
	void AddOpeningSliders();
	void deleteMarkedGems();
	void ShowGemBreakAnim();
	void markRemovableGems();
	bool isMatch3Found();
	void insertNewGem(int x, int y);
	void fillDropingGemsArray();

	void ProcessHint();
	bool isTaskLeft();

protected:

	//Match3 Elements - Sreeraj
	CObject *m4Image1, *m4MouseOver;
	CObject *leftSlider[MAX_MATCH3_LEFTSLIDERS];
	CObject *rightSlider[MAX_MATCH3_RIGHTSLIDERS];
	CObject *board[MAX_MATCH3_ROWS][MAX_MATCH3_COLS];
	CObject *gems[MAX_MATCH3_ROWS][MAX_MATCH3_COLS];
	CObject *gemSel[2];
	bool removableGems[MAX_MATCH3_ROWS][MAX_MATCH3_COLS];

	int countGemsToRemove;
	int selX;
	int selY;
	bool isTileSwaping;
	bool isReverseSwaping;
	int fromX,toX,fromY,toY;
	float tweenTime, swapTime, dropTime;
	bool isRemovablesMarked;
	bool isPuzzleSolved;
	CObject *breakAnim[MAX_BREAK_ANIMS];
	int indexBreakAnim;
	bool isBreakAnimPlaying;

	CObject *m4SliderOpenAnim[MAX_MATCH3_SLIDERS];
	CObject *tempSliderOpenAnim;

	CObject *dropingGems[MAX_MATCH3_TILES];
	CPoint dropingGemsStartPos[MAX_MATCH3_TILES];
	CPoint dropingGemsEndPos[MAX_MATCH3_TILES];

	CObject *openingSliders[MAX_MATCH3_SLIDERS];
	float openingSlidersToXPos[MAX_MATCH3_SLIDERS];
	float openingSlidersFromXPos[MAX_MATCH3_SLIDERS];
	int sliderIndex;
	float timerSliders[MAX_MATCH3_SLIDERS];
	int sliderPos;
	bool isSliding;

	bool isSelected[MAX_MATCH3_ROWS][MAX_MATCH3_COLS];
	int selIndex;
	int gemType[MAX_MATCH3_ROWS][MAX_MATCH3_COLS];
	std::string strGems[MAX_MATCH3_VARIETY];
	bool isGemsDroping;

};

#endif
