#ifndef DQFYLH_LEVEL4_SCENE1_PZ_MUSHROOM_DEF
#define DQFYLH_LEVEL4_SCENE1_PZ_MUSHROOM_DEF

#include "Popup.h"

#define NUM_MUSHROOMS	29
#define NUM_SYMBOLS	10

class Mushroom_c
{
public:
	int id;
	int neighbourList[6];
};

class L4_S1_PZMushroom: public Popup
{
	public:
		float musicFadeOutTimer;
		bool PlayModeActive;
		//bool wrongSymbolAdded;
		int MushroomsVisited;
		int SymbolsVisited;
		int OccupiedMushroomIndices[NUM_SYMBOLS];

		CBitmapText *_hintText1,*_hintText2;

		std::vector<Mushroom_c*> MushroomsArray;
		std::vector<int> SolutionPath;
		std::vector<int> TraversedPath;

		CObject *Mushroom_white[NUM_MUSHROOMS], *Mushroom_mouseOver[NUM_MUSHROOMS], *Mushroom[NUM_MUSHROOMS], *Symbols[NUM_SYMBOLS], *GreenSymbols[NUM_SYMBOLS], *RedSymbols[NUM_SYMBOLS];
		CObject *Lens;

		L4_S1_PZMushroom(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L4_S1_PZMushroom();

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();

		void ResetPuzzle(int startNode);
		void ResetTouch();

		void OnMouseClickOnMushroom(CObject * object);
		void OnMouseOverMushroom(CObject * object);
		void OnMouseUpOnBg(CObject * object);
		bool CheckIfPuzzleComplete();
		void OnTweenComplete(CObject *obj);
		void ProcessSkip();

		float easing(float t,float b,float c,float d);

	protected:

		void ReceiveMessage(int val);
		void ProcessNonHOHint();
};


#endif