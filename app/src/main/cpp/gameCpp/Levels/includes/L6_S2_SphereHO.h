//====================================
// DQFYLH
// Hidden Object System
// Level 6 - Scene 02 : Sphere HO
//====================================

#ifndef DQFYLH_LEVEL6_SCENE2_HO_DEF
#define DQFYLH_LEVEL6_SCENE2_HO_DEF

#include "HiddenObjectSystem.h"
#include "Match3.h"

class CHOScene;

#define MAX_L6S2_HOOBJECTS 7

class L6_S2_SphereHO: public HiddenObjectSystem
{
	public:
		float musicFadeOutTimer;
		//Scene Elements
		CPolygon *HOPolyArray[MAX_L6S2_HOOBJECTS];

		CObject *HOScnObjects[MAX_L6S2_HOOBJECTS];
		CObject *HOFlyingObjects[MAX_L6S2_HOOBJECTS];
		CObject *HOInvboxObjects[MAX_L6S2_HOOBJECTS];

		int foundItemsCount;
		bool isHOCompleted;

		float animPartsFadeInTime;
		bool IsAnimPartsFadingIn;
		float endAnimTime;

		bool IsAnimPartsJoining;
		float animPartJoinTimer;

		bool IsHeadMovingToFix;
		float headMovingTimer;
		
		SuperFX *HOFoundFX, *HOTrailFX;
		int FXTimer;

		CPoint *BzStartPt, *BzEndPt, *BzCtrlPt;
		
		bool isHOObjectFlying, isHOObjectScaling;
		float hoScaleTimer, hoFlyingTimer;
		int flyingHOItemId;

		void HandleHOComplete();

		float easing(float t,float b,float c,float d);

		L6_S2_SphereHO();
		~L6_S2_SphereHO();

		CPoint GetBezierPoint(CPoint* p1,CPoint* c1,CPoint* p2,float t);
		
		void Update();
		void Required();
		void NavigateToScene();

	protected:

		//Match 3 code
		Match3 *match3Ctrl;
		CObject *BtnMatch3;
		CObject *BtnSwitchToHO;
		//Match 3
		CObject *match3Back, *match3BackText;


	void ReceiveMessage(int val);
		void ProcessNonHOHint();
};

#endif