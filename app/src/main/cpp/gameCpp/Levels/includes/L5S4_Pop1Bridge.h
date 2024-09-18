#ifndef DQFYLH_LEVEL5_SCENE4_POP1_BRIDGE_DEF
#define DQFYLH_LEVEL5_SCENE4_POP1_BRIDGE_DEF

#include "Popup.h"

typedef enum 
{
	L5S4_KState_WaitToStart = 100,
	L5S4_KState_Playing,
	L5S4_KState_Skip,
	L5S4_KState_AutoComplete,
	L5S4_KState_Won,
	L5S4_KState_Complete = 280
}L5S4_BridgePuzzleState;

struct RingToRotate
{
	float startAngle;
	float endAngle;
	CObject *_ringToRoate;

	void Clear()
	{
		startAngle = 0;
		endAngle = 0;
		_ringToRoate = NULL;
	}

};

class L5S4_Pop1Bridge: public Popup
{
	int _pzState;
	CPoint _ringCenter;
	vector<CObject*> _rings;
	
	bool _isPuzzleSolved;

	float rotateTimer;
	RingToRotate _mainRing;
	RingToRotate _subRing;
	
	vector<SuperFX*> s4BridgePuzzleCompleteFXs;

private:

	void ShuffleRings();

	//Normal Game play
	void InitRings();
	void RotateRing(int index);
	void OnRotateComplete();
	void OnPuzzleComplete();

	// On skip
	void StartAutoCompletePuzzle();
	void RotateRingToZero(int index);

	void OnPopupOpened();
	void OnPopupCloseBtnClicked();

	public:

		L5S4_Pop1Bridge(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L5S4_Pop1Bridge();

		void TakeInventoryItem(EHOInventory ItemID);
		void Update(float ds);
		void Required();
		JSONAnimator *JAnim_Grease;
		void OnAnimationComplete(int animID);

		CObject *popCloseBtn;
		float easing(float t,float b,float c,float d);
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		bool isTaskLeft();
		void ProcessSkip();
	protected:

		void ReceiveMessage(int val);
		void ProcessHint();
};

#endif