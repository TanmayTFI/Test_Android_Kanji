//====================================
//DQFULP
// Combination Inventory Handling
//====================================

#ifndef DQFULP_COMBOINVENTORY_DEF
#define DQFULP_COMBOINVENTORY_DEF

#include "Control.h"
#include "Polygon.h"
#include "HOObject.h"
#include "CustomAnimator.h"
#include "CustomAnimGroup.h"

#include "JSONAnimator.h"
#include <string>

class CObject;
class CHOScene;
class HudControl;
class Tutorial;
class CBitmapText;

enum eComboAnims{
	_Anim_L2S4_AxeSharpen,
	_Anim_L2S5_CorkRemove,
	_Anim_L2S5_RustRemove,
	_anim_L3S1_FlagComboRemove,
	_Anim_L3S1_LadderPartsPlacing,
	_Anim_L3S2_CoconutOuterShell,
	_Anim_L3S2_CoconutBreak,
	_Anim_L3S2_CoconutShellThreadPlace,
	_Anim_L3S2_HammerFix,
	_Anim_L4S1_LeafSpoonMaking,
	_Anim_L4S2_BambooKnife,
	_Anim_L4S2_GrassRope,
	_Anim_L4S5_BambooStrainer,
	_Anim_L5S1_ArrowBowFix,
	_Anim_L5S1_ArrowStringFix,
	_Anim_L5S4_PickaxeFix,
	_Anim_L5S6_Locket,
	_Anim_L6S4_HeartLocket,
	_Anim_L6S4_RopeHorseShoe,

	_Anim_L7S1_PotChainTry,
	_Anim_L7S1_PotWireFix,
	_Anim_L7S2_GlobeFix,
	_Anim_L8S2_MagicWand1,
	_Anim_L8S2_MagicWand2,
	_Anim_L8S2_Key

};

class ComboInvSystem : public Control, CRenderListEntity
{
	public:
		int ComboState;

		//Scn Elements
		CObject *BtnClose;

		bool isAnyAnimActive;

		CHOScene *ComboScene;
		CHOScene *ComboPopupScene;

		bool isSkipTransition;
		int ComboPopState;

		CRectangle *ComboBoundaryRect;

		CObject *PopCloseBtn, *ComboCloseBtn;

		CObject	*ComboMaskBg, *ComboPopMaskBg;

		float clickDelayTimer;

		// -- Item specific Objects -- //
#ifdef _MSC_VER
	#pragma region Level1ComboItems
#endif
		//eInv_BagPopup
		CPolygon *BagZipperPoly;
		CObject *bag_ZipOpened, *bag_Knife, *bag_Zipper, *bag_CoverClosed, *bag_CoverOpened, *bag_InvCreeksLetter, *bag_KidsDrawing, *bag_LocketFixed;
		CPolygon *BagOpenPoly;
		bool IsbagOpeningPlaying, IsKidDrawingZooming, IsKidDrawingZoomed, IsKidDrawingAddingToJournal;
		CustomAnimator *CAnim_kidDrawingSlide1, *CAnim_kidDrawingSlide2, *CAnim_combo_knife, *CAnim_combo_kerchief;
		JSONAnimator * JSONAnim_Combo_BambooKnife, * JSONAnim_Combo_BambooStrainer, *JSONAnim_Combo_HeartLocket, *JSONAnim_Combo_RopeHorseShoe;
		bool isKerchiefAnimPlaying,isKnifeAnimPlaying,isMagicWandKeyInHand;
		CustomAnimGroup *CAnim_ZipperOpen;
		CObject * rope;

		CObject *bag_CreekLetterZoomed, *bag_CreekLetterZoomedText;
		bool isCreekletterZooming, isCreekletterZoomedIn;

		// Ruby Popup
		bool ruby_IsPartFixed[4];
		CObject *ruby_FullPart, *ruby_Parts[4];
		CRectangle *ruby_takeRects[4], *ruby_destRects[4];
		CPoint *ruby_FixPos[4];
		int activeRubyPart;

		//eInv_L1S4_PhotoFrame
		CObject *photo_leftArrow, *photo_rightArrow, *photo_bgfront, *photo_bgback, *photo_paperOverlay, *photo_BeetlePart, *photo_SecretCode;

		//eInv_L1S4_Diary
		CObject *diary_page1, *diary_nextPagebtn, *diary_prevPagebtn, *diary_creekNotice, *diary_coin, *creekLetterText, * diary_page1_text;
		CObject *diary_item1, *diary_item2, *diary_item3, *diary_item4, *diary_item5;

		bool isCreekNoticeZoomedIn, isCreekNoticeZooming, isCreekNoticeGoingToJournal;

		//eInv_L1S4_BeetleHead
		CustomAnimGroup *CAnim_BeetleJoin;
		bool isBeetleJoinPaying;

		//eInv_L1S5_HammerHead
		CustomAnimGroup *CAnim_HammerJoin;
		bool isHammerJoinPaying; 
#ifdef _MSC_VER
	#pragma endregion

	#pragma region Level2ComboItems
#endif
		//eInv_L2S4_AxeBlunt
		JSONAnimator *JAnim_AxeSharpen;
		bool isAxeSharpenAnimPlaying;
		SuperFX* axeSparklesFX;

		//eInv_L2S6_KnifeBlunt
		JSONAnimator *JAnim_CorkRemove, *JAnim_RustRemove;
		SuperFX* vinegarDropsFX; 
#ifdef _MSC_VER
	#pragma endregion

	#pragma region Level3ComboItems

#endif		//eInv_L3S1_LadderBroken
		JSONAnimator *JAnim_FlagComboRemove;
		JSONAnimator *JAnim_PiecesPlacing;
		bool ladderClothFadingIn;
		float clothTieTimer;

		JSONAnimator *JAnim_CoconutOuterRemove, *JAnim_CoconutBreak; 
		//eInv_L3S2_CocoShellPowderFilled
		JSONAnimator *JAnim_TwinePlacing;

		JSONAnimator *JAnim_HammerFix;
		//eInv_L3S3_Box  
#ifdef _MSC_VER
	#pragma endregion

	#pragma region Level4ComboItems
#endif
		JSONAnimator *JAnim_LeafSpoon;
		//eInv_L4_S1_Leaf
#ifdef _MSC_VER
	#pragma endregion

	#pragma region Level5ComboItems
#endif
		//eInv_L5_S1_Bow
		JSONAnimator *JAnim_BowStringFix, *JAnim_BowArrowFix;

		//eInv_L5_S4_PickaxeBlade
		JSONAnimator *JAnim_PickaxeHandleFix;
#ifdef _MSC_VER
	#pragma endregion

	#pragma region Level6ComboItems
	#pragma endregion

	#pragma region Level7ComboItems
#endif
		//eInv_L7_S1_PotEmpty
		JSONAnimator *JAnim_PotChainTry;
		JSONAnimator *JAnim_PotWireFix;
		bool isPotAnimPlaying;

		//eInv_L7_S2_SphereHalfBase
		JSONAnimator *JAnim_GlobeFix;
		bool isGlobeFixPlaying;
#ifdef _MSC_VER
	#pragma endregion

	#pragma region Level8ComboItems
#endif
		JSONAnimator *JAnim_Locket;
		JSONAnimator *Janim_MagicWandTool;
		JSONAnimator *Janim_MagicWand;
		JSONAnimator *Janim_KeyFix;
#ifdef _MSC_VER
	#pragma endregion
#endif
		

		

		

		std::vector<SuperFX*> superfxArray;

		ComboInvSystem();
		~ComboInvSystem();

		void OnAnimationComplete(int animID);

		void  ShowComboInventoryPopup(int invId, float offsetX=0, float offsetY=0, bool skipTransition=false);
		void  HideComboInventoryPopup(bool instantClose=false);
		void  HandleInventoryDrop();
		void  InitRenderTarget(CHOScene *Popup);
		float PopupAlpha;
		bool  IsPopTransitionActive;
		
		bool ShowHint();

		void TaskPendingUpdate();

		int popOffsetX, popOffsetY;

		//Inherit this CRenderListEntity function to render anything to scene!! 
		void Render(const CPoint* pCam);

		//Inherited method
		void Update();
		void Required();

	protected:
		int _invId;
};

#endif