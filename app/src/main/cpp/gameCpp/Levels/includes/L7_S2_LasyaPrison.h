

#ifndef DQFYLH_L7_S2_LASYAPRISON_DEF
#define DQFYLH_L7_S2_LASYAPRISON_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"

#include "L7S2_Pop1SphereGlobe.h"


enum eL7S2PopEnums
{
	ePop1_L7S2Pop1Sphere,
	ePopL7S2_Max
};

class L7_S2_LasyaPrison : public HiddenObjectSystem
{
public:
	//Popups
	int ActivePopupID;
	float musicFadeOutTimer;

	Popup*		PopupArray[ePopL7S2_Max];
	int			PopStates[ePopL7S2_Max];
	CPolygon*	PopPolyArray[ePopL7S2_Max];

	void OnPopupClosed(Popup* popup);

	SuperFX *s2lasyaredraysFX, *s2lasyareddotsFX;
	SuperFX *s2statuedustFX, *s2bgstarsFX, *s2headglowFX, *s2smokefgFX, *s2sphereglobeFX, *s2lefthandFX, *s2righthandFX;

	SuperFX *s2handexplodeFX, *s2handexplodesmokeFX;;

	float LasyaStatueYPos;
	float lasyaRiseTimer;
	bool isStatueRising, isGlobeGoingDown;

	CObject *hypnoticCircle;

	CObject *globeStand, *globe, *lasyaStatue;

	float handExplodeTimer;
	CObject *explodeHandLight;
	bool isExplodeLightShining;
	bool isLeftHandExploding, isRightHandExploding;

	void OnTweenEndCallback(CObject* obj);

	L7_S2_LasyaPrison();
	~L7_S2_LasyaPrison();

	void SparkleUpdate();
	void TaskPendingUpdate();

	void InitPopups();
	void Update();
	void Required();

	void SceneObjectsUpdate();
	void NavigateToScene();

protected:
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
};

#endif
