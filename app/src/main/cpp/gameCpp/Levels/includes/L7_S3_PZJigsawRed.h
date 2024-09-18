

#ifndef DQFYLH_L7_S3_PUZZLEJIGSAWRED_DEF
#define DQFYLH_L7_S3_PUZZLEJIGSAWRED_DEF

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "HiddenObjectSystem.h"
#include "GameFlow.h"

#define MAX_L7S3_JIGSAWPIECES 16

namespace JigsawRed
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
		C_SYNTHESIZE_READONLY(bool, _isClicked, IsClicked);
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

class L7_S3_PZJigsawRed : public HiddenObjectSystem
{
public:
	SuperFX *s3pzglareFX, *s3pzmoonraysFX;
	float musicFadeOutTimer;

	CObject *ResetBtn;
	CHOScene *ResetDia;
	bool IsResetPop;

	float timer;
	bool isFadeIn;
	bool isFadeOut;
	bool isResetDiaFadeIn;
	bool isResetDiaFadeOut;


	JigsawRed::JigsawPart* JigsawParts[MAX_L7S3_JIGSAWPIECES];
	bool isDragging;
	int dragID;

	void OnJigsawPlacedCallback(int index, bool isFixed);

	L7_S3_PZJigsawRed();
	~L7_S3_PZJigsawRed();

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
