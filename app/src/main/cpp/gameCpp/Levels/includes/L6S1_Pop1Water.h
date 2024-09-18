#ifndef DQFYLH_LEVEL6_SCENE1_POP1_WATER_DEF
#define DQFYLH_LEVEL6_SCENE1_POP1_WATER_DEF

#include "Popup.h"
#include "JSONAnimator.h"
#include "CVideo.h"
#include "MeshObject.h"

#include "AssetsStringTable.h"
class L6S1_Pop1Water: public Popup
{
	CVideo *lpVidMemCutScn;
	float time;
	float vidTimer;
	int subIndex;
	subText subtitles[10];
	CBitmapText* subtitleText; int subMax;
	
	bool isOrbGlowing, isOrbFXFlying;
	bool isCollectingInventory;
public:
	JSONAnimator *Janim_steelPick;
	MeshObject *mo_water;
	L6S1_Pop1Water(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
	~L6S1_Pop1Water();

	void StartMemoryCutscene();
	bool isVideoPlaying;

	void ClosePopup(float delayTime = 0);
	void Update(float ds );
	void Required();
	void HandleInventoryDrop(CPoint dropPos, int itemVal);
	float easing(float t,float b,float c,float d);

	void OnAnimComplete(int animID);

protected:
	void ProcessHint();
	bool isTaskLeft();
};

#endif