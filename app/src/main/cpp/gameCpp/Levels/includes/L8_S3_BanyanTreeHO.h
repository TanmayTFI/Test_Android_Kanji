

#ifndef DQFYLH_L8S3BanyanTreeHO_DEF
#define DQFYLH_L8S3BanyanTreeHO_DEF

//====================================
//  created by : Jithin
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "Match3.h"

namespace Level8_Scene3
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
		std::vector<CObject*> _overlappedObj;

		CObject* _hudObj;
		SuperFX* _completeParticleHUD;
		C_SYNTHESIZE(SuperFX*, _completeParticle, CompleteParticle);
		C_SYNTHESIZE(CObject*, _hiddenObj, HiddenObj);
		C_SYNTHESIZE(CRectangle, _overlapedArea, OverlapRect);
		C_SYNTHESIZE(CPolygon*, _polyArea, PolyArea);
		C_SYNTHESIZE_READONLY(bool, _isFound, isFound);

	public:

		HiddenObject();
		~HiddenObject();

		void setHUDObj(CObject* obj, SuperFX* completeParticleHUD);
		void AddOverlapObj(CObject* obj);
		UserClick OnClickAtPoint(CPoint pos);
		void OnFound();
		void setAlreadyFound();
		void setActive(bool active);
		void UpdateFoundObj();
		void SetEnable(bool enable);
		void SwitchToMatch3();


	private:

		void HideHUDLeaf();
		void Update(float ds);
	};

	
}

using namespace Level8_Scene3;

enum L8S3_TreeHOState
{
	L8S3_HOState_Idle = -1,
	L8S3_HOState_Initializing = 0,
	L8S3_HOState_Playing,
	L8S3_HOState_OnClickRegister,
	L8S3_HOState_OnFoundItem,
	L8S3_HOState_ChangeLevel,
	L8S3_HOState_StartMatch3,
	L8S3_HOState_StopMatch3,
	L8S3_HOState_Match3Completed,
	L8S3_HOState_FoundAllItems,
	L8S3_HOState_HOCompleted,
	L8S3_HOState_InExtrasComplete
};

class L8_S3_BanyanTreeHO : public HiddenObjectSystem
{
	int _currentLevel;
	std::map<int, std::vector<Level8_Scene3::HiddenObject*> > _hoMapByLevel;
	std::map<int, std::vector<CPolygon*> > _hoPolyList;
	std::map<int, std::vector<CRectangle> > _hoRectList;
	std::vector<Level8_Scene3::HiddenObject*> _foundObjects;
	SuperFX *s3BanyanTreeHOFX;
	SuperFX *s3BanyanTreeHOHudLeafFX;		
	SuperFX *s3BanyanTreeHOLeafFallFX;
	float musicFadeOutTimer;
	L8S3_TreeHOState _hoState;
	float _changeLevelDelay;
	float _hoCompleteDelay;

	Match3 *_match3Ctrl;	
	CObject* _btnMatch3;
	CObject* _btnSwitchToHO;
	CObject *match3Back, *match3BackText;


private:

	void InitPolyAndRectList();
	void InitHOGame();
	void OnListUpdate();
	void ChangeLevel(int level);
	void OnLevelComplete(int level);

public:
	L8_S3_BanyanTreeHO();
	~L8_S3_BanyanTreeHO();

	void Update();
	void Required();
	void NavigateToScene();

protected:

	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
};

#endif
