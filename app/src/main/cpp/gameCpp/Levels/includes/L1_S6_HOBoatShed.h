//====================================
//DQFYLH
// Hidden Object System
//====================================

#ifndef DQFYLH_LEVEL1_SCENE6_BOATSHED_DEF
#define DQFYLH_LEVEL1_SCENE6_BOATSHED_DEF

#include "HiddenObjectSystem.h"
#include "GameFlow.h"
#include "CVideo.h"
#include "HOInventoryBox.h"
#include "InventoryItemInterface.h"
#include "CustomAnimGroup.h"
#include "AssetsStringTable.h"
#define MAX_HO_OBJECTS 11
typedef enum 
	{
		kInv_Acorn = 0,
		kInv_Stick,
		kInv_Cloth,
		kInv_Magnet,
		kInv_PaperClip,
		kInv_FunnelNose,
		kInv_OilDripper,
		kInv_Glove,
		kInv_Tape,
		kInv_Handle,
		kInv_Blade
	}HOInventoryItemIndex;
class CHOScene;

#include "Match3.h"

class L1_S6_HOBoatShed: public HiddenObjectSystem, CRenderListEntity, InventoryItemInterface
{
	

	public:

		//Scene Elements
		bool	IsVidSkippable, IsSkipMouseOver, IsSkipMouseClicked;
		bool isMouseAlreadyHandled;
		float	SkipShowTimer;
		CObject *skipObj;

		CVideo *cv_paperCutscn;
		float time; 
		float vidTimer;
		int subIndex;
		subText subtitles[5];
		CBitmapText* subtitleText; int subMax;
		bool isPaperCutSceneVideoPlaying;

		CObject *NewsPaperFlying;
		bool isNewsToJournalPlaying;
		float newsPaperTakeDelayTimer;

		float paperCutscnDelayTimer;
		float musicFadeOutTimer;
		CObject *squirrel;
		CObject *squirrelAngryAnim;
		CObject *SquirrelRunAnim[3];
		int squirrelRunAnimPointer;
		bool isSquirrelAngryAnimPlaying;
		bool isSquirrelRunAnimationPlaying;

		float squirrelTimer;
		float squirrelThreshold;

		CObject *paper;
		CPolygon *polyPaper;
		CPolygon *PolyToolArr[3];
		CPolygon *leftMiniBoxPolyToolArr[3];
		CObject *acon;
		CObject *beettle;
		bool isBeetleAnimPlaying;
		float beetleTimer,beetleTime;

		CObject *blade;
		CObject *bladeOnHand;
		CObject *boxDoorOpen;
		CObject *closedBox;

		CObject *cloth;
		CObject *clothOnHand;
		//CObject *ClothAnim;
		bool isClothAnimationPlaying;
		CObject *funnelFull;
		CObject *funnelFullWithTape;
		CObject *funnelFullWithTapeCut;
		CObject *funnelHead;
		CObject *funnelNose;
		bool isTapeRollingAnimPlaying;
		bool isTapeCuttingAnimPlaying;
		CObject *glove;
		CObject *gloveOnHand;
		CObject *magnet;
		CObject *magnet2;
		CObject *tapeRollAnimObj1;
		CObject *tapeRollAnimObj2;
		CObject *tapeRollAnimObj3;
		CObject *tapeCutAnimObjects[5];

		CObject *ResetBtn;
		CHOScene *ResetDia;
		bool IsResetPop;

		float timer;
		bool isFadeIn;
		bool isFadeOut;
		bool isResetDiaFadeIn;
		bool isResetDiaFadeOut;

	//	CObject *magnetOnHand;
		bool isMagnetAnimating;



		CObject *nutNBolt;
		CObject *oilDipper;
		CObject *oilDipperOnHand;
		CObject *oilDripAnim;
		bool isOilDrippingAnimPlaying;
		int oilDripAnimTimer;

		CObject *handle;
		CObject *handleOnMiniDoor;
		CObject	*openedLeftMiniDoor;
		CObject *leftMiniBoxOpenAnimObject1;
		CObject *leftMiniBoxOpenAnimObject2;
		CObject *leftMiniBoxOpenAnimObject3;
		CObject	*closedLeftMiniDoor;
		CObject *paperclip;
		CObject *paperclip2;
		CObject *PaperClipAnim;
		bool isPaperClipAnimPlaying;
		CObject *rust1;
		CObject *rustOnSmallBox;
		CObject *smallBoxDoorClosed;
		CObject *smallBoxDoorOpen;
		CObject *SmallBoxDoorOpenAnim;
		bool isSmallBoxDoorOpenAnimPlaying;
		bool isLeftMiniBoxDoorOpenAnimPlaying;
		CObject *stick;
		CObject *beetleAnimObject;
		bool isStickAnimationPlaying;
		CObject *tape;
		CObject *tapeOnHand;
		CObject *tool1;
		CObject *tool2;
		CObject *tool3;

		CObject *match3Back, *match3BackText;

		CObject *UseObjects;

		CPolygon *PolyBoxDoorArea;

		Match3 *match3;
		bool isMatch3Playing;
		bool flagForText;

		CObject *leftMiniBoxMovableTools[3];
		int leftMiniBoxToolPointer;
		bool isToolMovingInLeftMiniBox;
		bool isAllToolsInLeftMiniBoxMoved;
		float leftMiniBoxToolMovingTimer, leftMiniBoxToolMoveTime;
		CPoint leftMiniBoxToolStartPos, leftMiniBoxToolEndPos;

		CObject *moveableTools[3];
		bool isAllToolsMoved;
		int toolPointer;
		

		CPoint toolStartPos, toolEndPos;
		float toolStartRot,toolEndRot;
		bool isToolMoving;
		float movingTimer,moveTime;

		CPolygon *polyAconGet;
		CPolygon *polyAconSet;

		CPolygon *polyClothGet;
		CPolygon *polyClothSet;

		CPolygon *polyStickGet;
		CPolygon *polyStickSet;

		CPolygon *polyMagnetGet;
		CPolygon *polyMagnetSet;

		CPolygon *polyPaperClipGet;
		CPolygon *polyPaperClipSet;

		CPolygon *polyFunnelGet;
		CPolygon *polyFunnelSet;

		CPolygon *polyHandleGet;
		CPolygon *polyHandleSet;

		CPolygon *polyOilDipperGet;
		CPolygon *polyOilDipperSet;

		CPolygon *polyGloveGet;
		CPolygon *polyGloveSet;

		CPolygon *polyTapeGet;
		CPolygon *polyTapeSet;
		CPolygon *polyTapeSet2;

		CPolygon *polyBladeGet;
		CPolygon *polyBladeSet;

		CPolygon *getPoly[MAX_HO_OBJECTS];
		CPolygon *setPoly[MAX_HO_OBJECTS];
		CObject *getObject[MAX_HO_OBJECTS];
		CObject *setObject[MAX_HO_OBJECTS];
		std::string mousePointerString[11];

		CPolygon *polyInvFunnel1;
		CPolygon *polyInvFunnel2;

		bool isObjectReturning;
		CPoint startPos,endPos;
		CObject *returnObject;
		int objectID;
		float returnTimer;
		float returnEndTimer;
		CObject *pointerInHand;
		bool isObjectInHand;

		CObject *txtMatch3;
		CObject *txtMatch3MouseOver;
		CObject *txtPuzzle, *txtPuzzleMouseOver;
		float tweenTime;
		float swapTime;
		SuperFX *chestDoorVFX_0;
		SuperFX *chestDoorVFX_1;
		SuperFX *chestDoorVFX_2;
		SuperFX *rayLeftVFX;
		SuperFX	*rayRightVFX;

		HOInventoryBox * hoInventoryBox;

		CustomAnimGroup * cAnimCloth;
		CustomAnimGroup * cAnimOilDipping;
		CustomAnimGroup * cAnimStick;
		CustomAnimGroup * cAnimMagnet;
		CustomAnimGroup * cAnimTapeRoll;
		CustomAnimGroup * cAnimTapeCut;
		CustomAnimGroup * cAnimLeftMiniBoxOpen;


		void NavigateToScene();



		void ForceCompleteHO();
		void UpdateInsideBoxProperties();
		float easing(float t,float b,float c,float d);

		L1_S6_HOBoatShed();
		~L1_S6_HOBoatShed();

		float RandomRange(int min, int max);

		//Inherit this CRenderListEntity function to render anything to scene!! 
		void Render(const CPoint* pCam);

		void TakeInventoryItem(EHOInventory ItemID);
		void Update();
		void Required();
		void OnInventoryItemClick(int itemIndex);

		void ResetHO();

	protected:

		void ReceiveMessage(int val);

		void ProcessNonHOHint();
		void UpdateInventoryChangesToProfile(int itemId);

		void InitializeInventoryBox();
		void HandleMouseForCollectItem();
		void HandleMouseForPlaceItem();

};

#endif