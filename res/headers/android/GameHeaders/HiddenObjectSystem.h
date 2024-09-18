//====================================
//DQFULP
// Hidden Object System
//====================================

#ifndef DQFULP_HIDDENOBJECTSYSTEM_DEF
#define DQFULP_HIDDENOBJECTSYSTEM_DEF

#include "Control.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "LangDefs.h"
#include "SuperFX.h"
#include "Polygon.h"
#include "AwardManager.h"

#ifdef _PARTICLEEDITORENABLED
	#include "ParticleEditor.h"
#endif

#ifdef _DEBUG
	#include "SuperLog.h"
#endif // _DEBUG


class HudControl;
class CHOScene;

#define MAX_NUMSPARKLES 10

#ifdef _TABINGAMEUI
	#define MAX_SCENE_PER_LEVEL 20
	#define MAX_INVITEMS_PER_LEVEL 50

	#define MAX_AWARD_EVENTS 50
#endif //_TABINGAMEUI

class HiddenObjectSystem : public Control
{
	public:
		HudControl	*Hud;

		HiddenObjectSystem();
		virtual ~HiddenObjectSystem();

		//Inherited Methods
		virtual void Required();
		virtual void ProcessMessage(sMessage message);
		
		bool IsMouseOverGoBackArea(const CPoint* pPos);

		bool IsVisitedForFirstTime;

		CObject *extrasGoBackBtn, *extrasGoBackBtnText;

		virtual void SceneObjectsUpdate();

	protected:

		#ifdef _TABINGAMEUI
			CObject *TabMenuMaskBg;

			CBitmapText	*_AwardEventNames[eAwardEventMax];
			bool	_TabInGameAwardEventShowing;

			bool	_TabInGameMenuShowing;
			CBitmapText	*_SceneNames[MAX_SCENE_PER_LEVEL];
			int _sceneStartIndex, _sceneEndIndex;
			int numScenes;

			int _invStartIndex, _invEndIndex;
			int numInvItems;
			CBitmapText	*_InvNamesTexts[MAX_INVITEMS_PER_LEVEL];

			void updateProfileVarForMultiInventory(int eInvId);
		#endif //_TABINGAMEUI

		#ifdef _PARTICLEEDITORENABLED
			bool _isParticleEditorActive;
			ParticleEditorControl *_particleEditor;
		#endif

		//Hint to HUD
		void  InitHintTrailEmit(float x=0, float y=0, bool isInstantRecharge=false, eCursorAnim cursorId=CA_Normal);
		void  InitHintTrailEmit(CPoint *destPoint, bool isInstantRecharge=false, eCursorAnim cursorId=CA_Normal);
		void  InitHintTrailEmit(const CRectangle *destRect, bool isInstantRecharge=false, eCursorAnim cursorId=CA_Normal);
		
		CHOScene	*HOScene;

		void LoadHud(int InventoryMode = INVENTORY_V1);
		void LoadScene(const std::string scene);
		

		//Sparkle Handling
		bool isSparkleUpdatePending;
		float sparkleTimer, sparkleThreshold;
		CPoint sparklePoints[MAX_NUMSPARKLES];
		std::vector<CPoint> sparklePointsVec;
		SuperFX *SparkleFX;
		int currentSparkleID, numSparkles;
		virtual void SparkleUpdate();
		//Sparkle Handling

		virtual void ProcessNonHOHint(){};
		virtual void ProcessSkip(){};
		virtual void ReceiveMessage(int val) = 0;

		void HOUpdate();

		bool _handleExtrasGameComplete(bool isPuzzle = true);
		float _extrasCompleteTimer; 
};

#endif