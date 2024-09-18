

#ifndef DQFYLH_L5S3_PopStone_DEF
#define DQFYLH_L5S3_PopStone_DEF

//====================================
//  created by : Jithin M
//  copyright  : Tuttifrutti Games
//====================================

#include "Popup.h"
#include "PopupInterface.h"
#include "CVideo.h"
#include "AssetsStringTable.h"

enum L5S2_Pop2States
{
	L5S3_Pop2_Initial = 0,
	L5S3_Pop2_PlayingMemVdo,
	L5S3_Pop2_EmblemPlaced,
	L5S3_Pop2_PaperTaken,
	L5S3_Pop2_PaperCollcetd
};

class L5S3_PopStone : public Popup
{
	
	JSONAnimator *_janim_StoneBreak, *_janim_PaperOpen;
	CObject* _paper;
	CVideo* _clothMemoryVdo;
	float time;
	float vidTimer;
	int subIndex;
	subText subtitles[10];
	CBitmapText* subtitleText; int subMax;

	C_SYNTHESIZE(PopupInterface*, _interface, Interface);

public:
	L5S3_PopStone(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L5S3_PopStone();
	L5S2_Pop2States _state;
	void HandleInventoryDrop(CPoint dropPos, int itemVal);

	void StartMemoryCutscene();

	void Update(float ds );
	void Required();

	void ProcessHint();
	bool isTaskLeft();

protected:

	void OnAnimationComplete(int animId);

};

#endif
