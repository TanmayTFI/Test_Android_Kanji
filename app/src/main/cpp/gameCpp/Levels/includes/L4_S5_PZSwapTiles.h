//====================================
// DQFYLH
// Hidden Object System
// Level 4 - Scene 05 : PUZZLE SWAP TILE
//====================================

#ifndef DQFYLH_LEVEL4_SCENE5_PZSWAP_TILE_DEF
#define DQFYLH_LEVEL4_SCENE5_PZSWAP_TILE_DEF

#include "HiddenObjectSystem.h"

class L4_S5_PZSwapTiles: public HiddenObjectSystem, CRenderListEntity
{
	public:
		float musicFadeOutTimer;
		//Scene Elements
		

		
		L4_S5_PZSwapTiles();
		~L4_S5_PZSwapTiles();

	

		//Inherit this CRenderListEntity function to render anything to scene!! 
		void Render(const CPoint* pCam);
		void Update();
		void Required();

		float easing(float t,float b,float c,float d);

		CObject *PuzzleDoor, *Key, *AligatorHead;
		CObject *LightAnim;
		CObject *Tiles[12];
		CObject *Hilight[2];
		CObject *WrongHilight;
		CObject *ResetBtn;
		CHOScene *ResetDia;
		bool IsResetPop;

		float timer;
		bool isFadeIn;
		bool isFadeOut;
		bool isResetDiaFadeIn;
		bool isResetDiaFadeOut;

		int		TilesIndex[12];
		int		RandomArray[12];
		CPoint  PositionArray[12];
		bool	IsTilesSwapping;
		bool	IsPuzzleComplete;
		int		ActiveTileIndex[2];
		bool	IsTakingInvItem;
		
		void NavigateToScene();

	protected:
		SuperFX *S5PZ1_PuzzleCompleteFX, *S5PZ1_PuzzleComplete1FX, *S5PZ1_PuzzleComplete2FX, *S5PZ1_PuzzleComplete3FX, *S5PZ1_PuzzleComplete4FX;
		SuperFX *S5PZ1_PuzzleComplete5FX, *S5PZ1_PuzzleComplete6FX, *S5PZ1_PuzzleComplete7FX, *S5PZ1_PuzzleComplete8FX;

		void InitSceneElements();
		void InitAnimations();
		void RandomizeTiles();
		void ResetTiles();
		void UpdateTiles( CPoint pos );
		bool CheckIfPuzzleSolved();
		void OnPuzzleComplete();
		void ProcessSkip();
		bool CheckIfValid(int index1, int index2);
		void SwapTiles(int index1, int index2);
		void ReceiveMessage(int val);
		void ProcessNonHOHint();
		void ResetPuzzle();
};

#endif