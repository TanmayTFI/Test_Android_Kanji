#ifndef DQFYLH_LEVEL2_SCENE1_POP3_ALOEVERA_DEF
#define DQFYLH_LEVEL2_SCENE1_POP3_ALOEVERA_DEF

#include "Popup.h"
#include "MeshObject.h"

class L2S1_Pop3AloeveraPlant: public Popup
{
	bool _isTweening;
	public:

		//scene objects

		CObject *knife;
		CObject *stoneBall;
		CObject *leafSet;
		CObject *bigLeaf;
		CObject *aloeveraLeafTop;
		CObject *aloeveraLeafBotttom;
		CObject *gloveWithoutLeaf;
		CObject *gloveWithLeaf;

		MeshObject *pondWater;

		L2S1_Pop3AloeveraPlant(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L2S1_Pop3AloeveraPlant();

		void TakeInventoryItem(EHOInventory ItemID);
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		void Update(float ds);
		void Required();
		virtual void ClosePopup(float delayTime = 0);
		void OnScaleUpComplete();

		float easing(float t,float b,float c,float d);

		protected:

		void ProcessHint();
		bool isTaskLeft();
		void OnTweenComplete(CObject *obj);
		void OnSecondTweenComplete(CObject *obj);
};

#endif