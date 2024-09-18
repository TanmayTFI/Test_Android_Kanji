
#ifndef L2_S7_SOLDIERS_PUZZLE_H
#define L2_S7_SOLDIERS_PUZZLE_H

#include "HiddenObjectSystem.h"
#include "JSONAnimator.h"
#include "GameFlow.h"
#include "UIButton.h"
#include "CVideo.h"
#include "ClickableRectangle.h"
#include "AssetsStringTable.h"
#include "SuperFX.h"


class ParticleEffect;
class CHOScene;
class L2S7Soldier;
class L2S7Shield;

enum L2S7SoldiersState
{
	eL2S7SoldiersState_Default = 0,
	eL2S7SoldiersState_Closed,
	eL2S7SoldiersState_Opening,
	eL2S7SoldiersState_Opened,
	eL2S7SoldiersState_Attacking,
	eL2S7SoldiersState_Attacked,
};

enum SoldierPuzzleState
{
	eSoldierPuzzleState_Started = 0,
	eSoldierPuzzleState_P2_ShldRotated,
	eSoldierPuzzleState_p2_ShldOpened,
	eSoldierPuzzleState_p3_ShldOpened,
	eSoldierPuzzleState_p4_LeverFixed,
	eSoldierPuzzleState_p4_ShldOpened,
	eSoldierPuzzleState_p5_ShldOpened,
	eSoldierPuzzleState_p6_Web0Removed,
	eSoldierPuzzleState_p6_Web1Removed,
	eSoldierPuzzleState_p6_AllWebsRemoved,
	eSoldierPuzzleState_p6_ShldOpened,
	eSoldierPuzzleState_p7_Cutscene
};


class L2S7SoldiersPuzzle: public HiddenObjectSystem, CRenderListEntity
{
private:

	CObject * _soldierBackLayer;
	CObject * _screenShot;
	CObject * screenshot;
	CHOScene * _bgScene;
	CObject * _fog;
	CObject * _bg;
	CObject * _path;
	CObject * _whiteFlashBg;
	CPoint _firstSoldierPos;
	float _lastSoldierPos;
	std::vector<L2S7Soldier*> _arrayOfSoldiers;
	std::vector<L2S7Shield*> _arrayOfShields;
	std::vector<UIButton*> _arrayOfArrowBtns;
	std::vector<CObject*> _arrayOfInteractiveObjects;
	int _currentPuzzleNumber;
	int _isSoldiersOpened;


	int _p3ShieldClickCount;
	CObject * _p3Stone0;
	CObject * _p3Stone1;

	JSONAnimator * _jsonAnimP4LeverPullingAnim;
	JSONAnimator * _jsonAnimP4LeverPlacingAnim;
	bool _isP4LeverOnHand;
	CObject * _p4Lever;
	CObject * _p4LeverHO;
	CObject * _p4LeverBase;

	L2S7Shield * _p5PrevSelectedShield;
	CObject * _p5PathDirection0;

	
	CObject * _p6ClothHO;
	CObject * _p6ClothOnHand;
	CObject * _web0;
	CObject * _web1;
	bool	  _isP6ClothOnHand;
	JSONAnimator * _jsonAnimP6Cloth0;
	JSONAnimator * _jsonAnimP6Cloth1;
	int _numOfWebsRemoved;
	bool _isMouseInteractionEnabled;
	bool _isWrongClick;
	ClickableRectangle * _gamePlayAreaRect;
	SuperFX * _bgFogVFX;
	
    KGraphic * rtGraphic;

	CVideo *_cVideoCutScene;
	float time;
	int subIndex; 
	float vidTimer;
	subText subtitles[15];
	CBitmapText* subtitleText; int subMax;
	bool IsCutscenePlaying;

public:

	L2S7SoldiersPuzzle();
	~L2S7SoldiersPuzzle();

	void InitializeWithProfileData();

	void Render(const CPoint* pCam);
	void Update();
	void UpdateChangesToProfile();
	void Required();
	float easing(float t,float b,float c,float d);

	void OnGamePlayAreaClick(int id);
	void StartSoldiersOpenAnim();
	void OnSoldiersOpenAnimComplete(int animId);
	void StartSoldiersAttackAnim();
	void OnSoldiersAttackAnimComplete(int animId);
	void OnPuzzleFailed();

	void MoveToNextLayer();
	void OnMoveToNextLayerComplete(CObject * object);

	void LoadLayer(int layerNumber, CHOScene * scene);
	void LoadPuzzle(int puzzleNumber);
	void UnLoadPuzzle();
	void LoadNextPuzzle();

	void OnP2ShieldClick(CObject * obj);
	void OnP2ShieldRotationComplete(CObject * obj);
	void OnP2ShieldOpenComplete(CObject* obj);

	void OnP3ShieldClick(CObject * obj);
	void FadeInP3Stones();

	void OnP4LeverClicked(CObject * obj);
	void OnP4LeverHOClicked(CObject * obj);
	void OnP4LeverBaseAreaClicked(CObject * obj);
	void OnP4LeverPullingAnimComplete(int id);
	void OnP4LeverPlacingAnimComplete(int id);

	void OnP5ShieldClick(CObject * obj);
	void OnP5ShieldSwapingComplete(CObject * obj);
	void OnP5ShieldOpenComplete(CObject* obj);

	void OnP6ShieldClick(CObject * obj);
	void OnP6ClothAnimComplete(int id);
	void OnP6ClothHOClicked(CObject * obj);
	void OnP6WebAreaClicked(CObject * obj);

	void OnRightArrowClick(CObject * obj);
	void OnWrongArrowClick(CObject * obj);

	void FadeInArrows();
	void OnFadeInArrowsComplete(CObject * obj);

	void FlashScreen();
	void OnFlashScreenHalfComplete(CObject* obj);
	void OnFlashScreenFullComplete(CObject* obj);
	
protected:

	void ReceiveMessage(int val);
	void ProcessNonHOHint();
	void ProcessSkip();
};

class L2S7Soldier 
{
private:
	L2S7SoldiersState _state;
	int _layerId;
public:

	int symbolId;
	int shieldSymbolId;
	CObject * symbol;

	JSONAnimator * jsonAnimOpening;
	JSONAnimator * jsonAnimAttacking;


	L2S7Soldier(int layerId);
	~L2S7Soldier();
	void SetState(L2S7SoldiersState state);
	
	
};

class L2S7Shield
{
	
public:
	int layerId;
	int shieldId;
	int shieldSolutionIndex;
	int shieldCurrentIndex;
	CObject * shieldClosed;
	CObject * shieldAttacked;
	CObject * shieldOpened;
	CObject * shieldSymbol;
	CObject * shieldArrow;
	CObject * shieldArrowGlow;
	CObject * shieldArrowCenter;
	CObject * shieldMask;
	CObject * shieldDoorUp;
	CObject * shieldDoorDown;
	CObject * shieldYellowGlow;
	CObject * shieldRedGlow;

	int shieldSymbolId;

	L2S7Shield();
	~L2S7Shield();
	void SetState(L2S7SoldiersState state);
};




#endif
