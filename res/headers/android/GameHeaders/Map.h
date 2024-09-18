//====================================
//DQFULP
// Hidden Object Map
//====================================

#ifndef DQFULP_MAP_DEF
#define DQFULP_MAP_DEF

#include "Control.h"
#include "BuildConstants.h"
#include "RenderListEntity.h"
#include <string>

class CObject;
class CHOScene;

enum MapIconStatus{
	MAP_ICON_LOCKED,
	MAP_ICON_PLAYERHERE,
	MAP_ICON_TASKPENDING,
	MAP_ICON_NIL
};



class Map : public Control, CRenderListEntity
{
	public:
		int MapState;

		CHOScene  *MapScene;
		CObject	*MapIcons[10], *TaskIcons[10];
		CObject	*SceneThumbs[10], *ThumbGlows[10];

		//Extras
		CObject	*MorphIcons[10], *MemoryIcons[10];

		bool IsMemoryItemAvaialbleToCollect(int areaID);
		bool IsMorphItemAvaialbleToCollect(int areaID);

		CObject *BtnTabObjective, *BtnTabJournal, *BtnTabCharacter;

		std::vector<CRectangle*> sceneRectArray;
		std::vector<int> sceneIDArray;

		int MaxCurrentLevelScenes;

		int CurrentLevel;
		
		Map();
		~Map();		

		bool isPlayerInMainScene;
		void  SetYouAreHereMapArea();

		void  SetUpAndShowMap(bool isQuick=false);
		void  HideMap(bool isQuick=false);

		void  InitRenderTarget(CHOScene *Popup);
		float MapPopupAlpha;

		bool IsMapActive, IsTransitionActive, IsMapClosed, IsOtherSceneClicked;

		//Inherit this CRenderListEntity function to render anything to scene!! 
		void Render(const CPoint* pCam);

		//Inherited method
		void Update();
		void Required();

	protected:
};

#endif