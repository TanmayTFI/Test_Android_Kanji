#ifndef DQFYLH_LEVEL4_SCENE2_POP2_BAMBOO_DEF
#define DQFYLH_LEVEL4_SCENE2_POP2_BAMBOO_DEF

#include "Popup.h"
#include "JSONAnimator.h"




class L4S2_Pop2Bamboo: public Popup
{
	JSONAnimator * _jsonAnimBambooCut;


	CObject * _smallStick0;
	CObject * _smallStick1;
	CObject * _smallStick2;
	CObject * _smallStick3;
	CObject * _bambooStick;


	std::vector<CObject*> _arrayOfObjects_Cursor_Take;
	CPolygon * _knifeDropPolygon;

	public:



		L4S2_Pop2Bamboo(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L4S2_Pop2Bamboo();

		void InitializeWithProfileData();

		void OnBambooStickClick(CObject * stick);
		void OnBambooCutAnimComplete(int id);

		void ClosePopup(float delayTime = 0);
		void OpenPopup();
		void OnPopupOpened();
		void Update(float ds );
		void Required();
		void HandleInventoryDrop(CPoint dropPos, int itemVal);

	protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif