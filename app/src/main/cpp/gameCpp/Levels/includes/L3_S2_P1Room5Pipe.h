//====================================
// DQFULP
// Hidden Object System
// Author : Sreeraj
// LEVEL3 - SCENE3 : P1Room5Pipe
//====================================

#ifndef DQFYLH_LEVEL3_SCENE3_P1Room5Pipe_DEF
#define DQFYLH_LEVEL3_SCENE3_P1Room5Pipe_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"

#include "VideoDefs.h" 
#include "KVideo.h"

#include "L3_S2_P1_MazePipePopup.h"
#include "JSONAnimator.h"

class CHOScene;

class L3_S2_P1Room5Pipe: public HiddenObjectSystem, CRenderListEntity
{
	bool _isPipePuzzleSolved;
	SuperFX *pipepumpFX;
	SuperFX *M5LightParticleFX;
	float musicFadeOutTimer;
	CObject *_solvedMask;
	vector<CObject*> _doors;

	CPolygon *_liverPolygon;
	JSONAnimator *_janim_liverAnim;

	int ActivePopupID;
	CPolygon *PuzzleAreaRect;
	L3_S2_P1_MazePipePopup *pipePuzzlePopup;

	CHOScene *mapHand;

public:

		L3_S2_P1Room5Pipe();
		~L3_S2_P1Room5Pipe();
		
		float easing(float t,float b,float c,float d);
	
		void Render(const CPoint* pCam);
		void Update();
		void Required();

		void TaskPendingUpdate();
		void NavigateToScene();

	protected:
		
		void SparkleUpdate();
		void ReceiveMessage(int val);
		void ProcessNonHOHint();
		void ProcessSkip();

		void OnClosePopup(Popup *popup);

private:

	void InitPopups();
	void InitSceneProperties();
	void OnAnimatonComplete(int animID);
	void OpenPuzzlePopup();
};

#endif