#ifndef DQFYLH_L8S1DoorHO_DEF
#define DQFYLH_L8S1DoorHO_DEF

//====================================
//  created by : Jithin
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Match3.h"

namespace Level8_Scene1
{
	enum UserClick
	{
		Click_None = 0,
		Click_OverlapObj,
		Click_HiddenObj
	};

	class HiddenObject : public UpdateListEntity
	{
		bool _isCompleted;
		bool _isActive;
		float _HUDhideDelay;
		float _completeTimer;
		CBitmapText* _nameText;

		C_SYNTHESIZE(int, _index, Index);
		C_SYNTHESIZE(SuperFX*, _completeParticle, CompleteParticle);
		C_SYNTHESIZE(CObject*, _hiddenObj, HiddenObj);
		C_SYNTHESIZE(CPolygon*, _polyArea, PolyArea);
		C_SYNTHESIZE(std::string, _name, Name);
		C_SYNTHESIZE_READONLY(bool, _isFound, isFound);

	public:

		HiddenObject();
		~HiddenObject();

		void Init();
		UserClick OnClickAtPoint(CPoint pos);
		void OnFound();
		void setAlreadyFound();
		void setActive(bool active);
		void UpdateFoundObj();
		void SwitchHO();
		void SwitchToMatch3();


	private:

		void HideHUDName();
		void Update(float ds);
	};

	
}

using namespace Level8_Scene1;

enum L8S1_DoorHOState
{
	L8S1_HOState_Idle = -1,
	L8S1_HOState_Initializing = 0,
	L8S1_HOState_Playing,
	L8S1_HOState_OnClickRegister,
	L8S1_HOState_OnFoundItem,
	L8S1_HOState_ChangeLevel,
	L8S1_HOState_StartMatch3,
	L8S1_HOState_StopMatch3,
	L8S1_HOState_Match3Completed,
	L8S1_HOState_FoundAllItems,
	L8S1_HOState_HOCompleted,
	L8S1_HOState_InExtrasComplete
};

class L8_S1_DoorHO : public HiddenObjectSystem
{
	int _currentLevel;
	std::map<int, std::vector<Level8_Scene1::HiddenObject*> > _hoMapByLevel;
	std::map<int, std::vector<CPolygon*> > _hoPolyList;
	std::map<int, std::vector<std::string> > _hoNameList;
	std::vector<Level8_Scene1::HiddenObject*> _foundObjects;

	float musicFadeOutTimer;
	L8S1_DoorHOState _hoState;
	float _changeLevelDelay;
	float _hoCompleteDelay;

	SuperFX *s1hoglaresFX, *s1hofliesFX, *s1hoglitrightFX, *s1hoglitleftFX;

	Match3 *_match3Ctrl;	
	CObject* _btnMatch3;
	CObject* _btnSwitchToHO;
	CObject *match3Back, *match3BackText;

	SuperFX *s1DoorHOFX;
	SuperFX *s1DoorHOInvBoxFXFX;


private:

	void InitPolyAndNameList();
	void InitHOGame();
	void OnListUpdate();
	void ChangeLevel(int level);
	void OnLevelComplete(int level);

public:
	L8_S1_DoorHO();
	~L8_S1_DoorHO();

	void Update();
	void Required();
	void NavigateToScene();

protected:

	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
};

#endif