#ifndef DQFYLH_LEVEL5_SCENE2_POP2_LOG_DEF
#define DQFYLH_LEVEL5_SCENE2_POP2_LOG_DEF

#include "Popup.h"
#include "MeshObject.h"
class L5S2_Pop2Log: public Popup
{
	public:
		MeshObject* waterObj1;
		L5S2_Pop2Log(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L5S2_Pop2Log();

		void TakeInventoryItem(EHOInventory ItemID);
		void Update(float ds);
		void Required();
		JSONAnimator  *JAnim_Log;
		void OnAnimationComplete(int animID);

		CObject *popCloseBtn;
		float easing(float t,float b,float c,float d);
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		bool isTaskLeft();

	protected:

		void ReceiveMessage(int val);
		void ProcessHint();

};

#endif