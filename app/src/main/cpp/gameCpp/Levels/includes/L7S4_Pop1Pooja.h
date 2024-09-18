

#ifndef DQFYLH_L7S4_Pop1Pooja_DEF
#define DQFYLH_L7S4_Pop1Pooja_DEF

//====================================
//  created by : Jithin M
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"
#include "CVideo.h"

struct FlowerAreas
{
	int _index;
	CPoint _pos;
	bool _haveFlower;
};

struct Part
{
	bool _isDraw;
	CObject *_obj;

	void Init(CObject *obj) {_obj = obj; _isDraw = false;}
	CRectangle getObjRect(){return _obj->GetWorldRect();}
	void Draw()
	{
		if (!_isDraw)
		{
			_isDraw	= true;
			_obj->Show(false);
			_obj->SetEnable(true);
			_obj->FadeIn(2);
		}
	}

	void Reset()
	{
		_isDraw	= false;
		_obj->SetEnable(false);
		_obj->Status = SPR_INVISIBLE;
		_obj->SetAlpha(0);
	}


};

class Flower
{
	FlowerAreas *_currentArea;
	C_SYNTHESIZE(bool, _placed, Placed);
	C_SYNTHESIZE(CObject*, _Obj, Obj);
	C_SYNTHESIZE(FlowerAreas*, _finalArea, FinalArea);
	C_SYNTHESIZE_READONLY(bool, _isCompleted, IsCompleted);
	int _index;

public:

	Flower(int index);
	~Flower();

	void PlacedAt(FlowerAreas *area);
	bool isInArea(FlowerAreas *area);
	FlowerAreas* getCurrentArea();
};

enum PopupMode
{
	L7S4_Puzzle_None = -1,
	L7S4_Puzzle_BusmomPlaced = 0,
	L7S4_Puzzle_Busmom,
	L7S4_Puzzle_Flower,
	L7S4_Puzzle_Completed
};


class L7S4_Pop1Pooja : public Popup
{
	PopupMode _pzMode;
	bool _isInPuzzleMod;
	vector<Part> _parts;
	vector<CPolygon*> _boundries;
	bool _isTakeBusmomToHand, _isDrawing;
	bool _hashDrawComplete;
	std::vector<FlowerAreas*> _flowerPos;
	std::vector<Flower*> _flowers;
	std::vector<bool> _invTakenFlowers;
	bool _flowerPuzzleComplete, _placedAllFlowers;
	CObject *_hashBox_0, *_hashBox_1;
	bool _hashBoxUsed;
	JSONAnimator *JAnim_NoteOpen, *JAnim_NoteClose;
	SuperFX *s4BhasmaPowderFX, *s4BhasmaPartiFX;
	CVideo* _cvdo_handPowerGain;
	float vidTimer;
	Flower *_selectedFlower;
	int _fixedFlowerCount;
	float _vdoStartTimer;
	SuperFX *s4tableonfireFX;


public:
	L7S4_Pop1Pooja(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L7S4_Pop1Pooja();
	void OnAnimationComplete(int animID);
	void HandleInventoryDrop(CPoint dropPos, int itemVal);
	bool _isVdoStartPlaying;
	void Update(float ds );
	void Required();
	bool noteAnimating;
	CObject* flowerChart;
	void ProcessHint();
	void ProcessSkip();
	bool isTaskLeft();

protected:
	void OnPopupOpened();
	 void OnPopupCloseBtnClicked();
private:

	void InitPolys();
	void ResetLineDraw();
	void DrawAt(CPoint mPos);
	void CheckForFlowerCompleted();

};

#endif
