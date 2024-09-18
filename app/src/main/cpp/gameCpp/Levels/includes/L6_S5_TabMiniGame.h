
#ifndef L6_S5_TABMINIGAME_H
#define L6_S5_TABMINIGAME_H

#include "HiddenObjectSystem.h"
#include "JSONAnimator.h"
#include "GameFlow.h"
#include "UIButton.h"
#include "CVideo.h"

#include "SuperFX.h"
#include "MeshObject.h"


class ParticleEffect;
class CHOScene;



enum TabMiniGameState
{
	eTabMiniGameState_S0_Started = 0,
	eTabMiniGameState_S0_Squirrel_Moved,
	eTabMiniGameState_S1_Started,
	eTabMiniGameState_S1_Tree_Cut,
	eTabMiniGameState_S2_Started,
	eTabMiniGameState_S2_Pipe_Unlocked,
	eTabMiniGameState_S3_Started
};


class L6S5TabMiniGame: public HiddenObjectSystem, CRenderListEntity
{
private:

	CObject * _rabbit;
	CObject * _whiteFlashBg;
	CObject * _bgMask;
	std::vector<CObject*> _arrayOfPickableObjects;
	std::vector<CObject*> _arrayOfTaskObjects;
	std::vector<CObject*> _arrayOfHandPointyObjects;
	bool _isMouseInteractionEnabled;
	int _currentSceneNumber;
	bool _isDraggingSomething;
	CObject * _sunRay;
	CObject * _sunEye1;
	CObject * _sunEye2;
	bool isCutscnPlaying;
	CVideo * _cVideoCutScene;

	float vidTimer;

	//SCENE_0
	CObject * _s0_squirrel;
	CObject * _s0_brick;
	CObject * _s0_fixedBrick;
	CObject * _s0_brickOnHand;
	bool _s0_isDraggingBrick;
	bool _s0_isDraggingFruit;
	CObject * _s0_fruitOnHand;

	std::vector<CObject*> _s0_arrayOfFruits;
	JSONAnimator * _s0_jsonAnimSquirrel;
	JSONAnimator * _s0_jsonAnimRabbitEscape;


	//SCENE_1

	CObject * _s1_sawOnHand;
	CObject * _s1_saw;
	CObject * _s1_stone;
	CObject * _s1_tree;
	CObject * _s1_treeTop;
	bool _s1_isDraggingSaw;

	JSONAnimator * _s1_jsonAnimTreeCut;
	JSONAnimator * _s1_jsonAnimRabbitEscape;

	//SCENE_2

	CObject * _s2_waterTubeOnHand;
	CObject * _s2_stoneOnHand;
	CObject * _s2_waterTube;
	CObject * _s2_waterTubeOnWater;
	CObject * _s2_waterPipe;
	CObject * _s2_stone;
	CObject * _s2_cloth;
	CObject * _s2_water;
	CPoint  * _s2_waterPos;
	MeshObject *_s2_meshObjectWater;
	bool _s2_isDraggingWaterTube;
	bool _s2_isDraggingStone;
	SuperFX * _s2_pipeWaterVFX;

	JSONAnimator * _s2_jsonAnimFillWater;
	JSONAnimator * _s2_jsonAnimRabbitEscape;

	//SCENE_3

	std::vector<CObject*> _s3_arrayOfCarrots;
	bool _s3_isDraggingCarrot;
	CObject * _s3_carrotOnHand;
	

	


public:
	float musicFadeOutTimer;
	L6S5TabMiniGame();
	~L6S5TabMiniGame();

	void InitializeWithProfileData();

	void Render(const CPoint* pCam);
	void Update();
	void UpdateCursor();
	void Required();

	void LoadScene(int sceneNumber);
	void UnloadScene();
	void LoadNextScene();
	void UpdateCursorObjects(TabMiniGameState gameState = (TabMiniGameState)ProfileSystem::Instance->ActiveProfile.L6_S5_TabMiniGameState);
	
	void StartFadeInWhiteBg();
	void OnFadeInWhiteBgComplete(CObject * object);
	void StartFadeOutWhiteBg();
	void OnCompleteFadeOutWhiteBg(CObject * object);
	void OnRabbitEscapeAnimComplete(int animId);
	void OnPuzzleComplete(UpdateListEntity * obj);


	//SCENE_0
	void OnS0FruitClick(CObject * obj);
	void OnS0BrickClick(CObject * obj);
	void OnS0SquirrelClick(CObject * obj);
	void OnS0FixedBrickClick(CObject * obj);
	void OnS0SquirrelRunAnimComplete(int id);
	void OnS0RabbitClick(CObject * obj);

	//SCENE_1
	void OnS1SawClick(CObject * obj);
	void OnS1TreeClick(CObject * obj);
	void OnS1StonelClick(CObject * obj);
	void OnS1TreeCutAnimComplete(int id);
	void OnS1TreeTopClick(CObject * obj);
	void OnS1RabbitClick(CObject * obj);

	//SCENE_2
	void OnS2ClothClick(CObject * obj);
	void OnS2StoneClick(CObject * obj);
	void OnS2PipeClick(CObject * obj);
	void OnS2WaterTubeClick(CObject * obj);
	void OnS2WaterClick(CObject * obj);
	void OnS2WaterFillingAnimComplete(int id);
	void OnS2RabbitClick(CObject * obj);

	//SCENE_3
	void OnS3CarrotClick(CObject * obj);
	void OnS3RabbitClick(CObject * obj);


	
protected:

	void ReceiveMessage(int val);
	void ProcessNonHOHint();
};

#endif