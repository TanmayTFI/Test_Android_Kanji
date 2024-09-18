#ifndef DQFYLH_LEVEL7_SCENE1_POP2_FLOWER_DEF
#define DQFYLH_LEVEL7_SCENE1_POP2_FLOWER_DEF

#include "Popup.h"
#include "MeshObject.h"

class L7S1_Pop2Well: public Popup
{
	

	

	public:
		MeshObject* waterObj1;
		L7S1_Pop2Well(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L7S1_Pop2Well();

		void TakeInventoryItem(EHOInventory ItemID);
		void Update(float ds);
		void Required();
		JSONAnimator *JAnim_Wheel, *JAnim_Puzzle, *JAnim_Pot;
		void OnAnimationComplete(int animID);
		int currentPattern;
		bool intialPatternPlaying,intialPatternPlayed;
		int patternTimer;

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