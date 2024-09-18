//====================================
// DQFYLH
// Hidden Object System
// Level 6 - Scene 01 : DESCRIPTION HO
//====================================

#ifndef DQFYLH_LEVEL6_SCENE1_DESCRIPTION_HO_DEF
#define DQFYLH_LEVEL6_SCENE1_DESCRIPTION_HO_DEF

#include "HiddenObjectSystem.h"
#include "Match3.h"
//#include "L6S1_PopNaginNote.h"
#include "L6S4_PopAmeehaNote.h"
//#include "UIButton.h"

class L6_S1_DescriptionHO: public HiddenObjectSystem, CRenderListEntity
{
	//UIButton *Back_Btn, *Next_Btn;
	CHOScene *Ho_Scene[MAX_L6S1_HOSCENES - 1];
	int activeHO;
	int _objectInHand;
	CObject *_hiddenObj;
	int _delay;
	L6S4_PopAmeehaNote *popNote;
	int activePopupID;
public:
	L6_S1_DescriptionHO();
	~L6_S1_DescriptionHO();
	float musicFadeOutTimer;

	void ResetHO();
	CObject *ResetBtn;
	CHOScene *ResetDia;
	bool IsResetPop;
	float timer;
	bool isFadeIn;
	bool isFadeOut;
	bool isResetDiaFadeIn;
	bool isResetDiaFadeOut;
	void NavigateToScene();

private:

	//Match 3 code
	Match3 *match3Ctrl;
	CObject *BtnMatch3;
	CObject *BtnSwitchToHO;
	//Match 3
	CObject *match3Back, *match3BackText;


	void ReceiveMessage(int val);
	void Update();
	void Required();
	void Render(const CPoint* pCam);
	/*void OnBackArrowClick(CObject * obj);
	void OnNextArrowClick(CObject * obj);*/
	void ShowHintText(int currHO);
	void OnFadeInComplete(CObject *obj);
	void OnDepthEffectTweenComplete(CObject *obj);
	void OnLeftMoveTweenComplete(CObject *obj);
	void InitTweens(int index, int numObj, int nextSceneNumObjects);
	void ProcessNonHOHint();
	void OnPopupClosed(Popup * popup);
};

#endif