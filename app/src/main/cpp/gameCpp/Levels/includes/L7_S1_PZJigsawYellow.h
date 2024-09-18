

#ifndef DQFYLH_L7_S1_PUZZLEJIGSAWYELLOW_DEF
#define DQFYLH_L7_S1_PUZZLEJIGSAWYELLOW_DEF

//====================================
//  created by : faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"

#define MAX_L7S1_JIGSAWPIECES 18

namespace JigsawYellow
{
	class JigsawPart
	{
		float _FIX_GAP_THRESHOLD;
		CPoint *_fixPos;

		SuperFX *FixFX;

		std::function<void (int, bool)> _jigsawPlaceCallback;

		C_SYNTHESIZE(int, _index, Index);
		C_SYNTHESIZE(CObject*, _scnObj, SceneObject);
		C_SYNTHESIZE(CObject*, _shadowObj, Shadow);
		C_SYNTHESIZE(CObject*, _fixObj, FixObject);
		C_SYNTHESIZE(CPolygon* ,_polygonArea, PolygonArea);
		C_SYNTHESIZE_READONLY(bool, _isFixed, IsFixed);
		C_SYNTHESIZE_READONLY(bool, _isActive, IsActive);
		C_SYNTHESIZE_READONLY(bool, _isDragging, IsDragging);

	public:

		JigsawPart();
		~JigsawPart();

		void InitObject(std::function<void (int, bool)> jigsawPlaceCallback = nullptr);
		bool HandleMouseUpdate(CPoint pos, bool clicked);
		void AlreadyFixed();	

		void SetActive(bool f){
			_isActive = f;
		}

	private:
		void _DragStart();
		void _CheckAndPlaceJigsaw();
	};
}

class L7_S1_PZJigsawYellow : public HiddenObjectSystem
{
public:
	SuperFX *s1pzmoonraysFX, *s1pzglareFX;

	JigsawYellow::JigsawPart* JigsawParts[MAX_L7S1_JIGSAWPIECES];
	bool isDragging;
	int dragID;
	CObject *ResetBtn;
	CHOScene *ResetDia;
	bool IsResetPop;

	float timer;
	bool isFadeIn;
	bool isFadeOut;
	bool isResetDiaFadeIn;
	bool isResetDiaFadeOut;

	void OnJigsawPlacedCallback(int index, bool isFixed);

	L7_S1_PZJigsawYellow();
	~L7_S1_PZJigsawYellow();
	float musicFadeOutTimer;

	void HandlePuzzleComplete();

	void Update();
	void Required();
	void NavigateToScene();

protected:
	void ReceiveMessage(int val);
	void ProcessSkip();
	void ProcessNonHOHint();
	void ResetPuzzle();
};

#endif
