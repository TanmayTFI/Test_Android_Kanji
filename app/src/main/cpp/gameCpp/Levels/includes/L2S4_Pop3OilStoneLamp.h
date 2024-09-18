#ifndef DQFYLH_L2_S4_POP3_OILLAMP_DEF
#define DQFYLH_L2_S4_POP3_OILLAMP_DEF

#include "Popup.h"
#include "SuperLog.h"

#include "JSONAnimator.h"
#include "MeshObject.h"

class L2S4_Pop3OilStoneLamp: public Popup
{
	public:
		JSONAnimator *JAnim_TorchOiling;

		bool isTorchDragging;
		CObject *obj_torchOnWall;
		CObject *obj_torchTaken;

		MeshObject *mesh_Oil;

		SuperFX *OilDropsVFX;

		L2S4_Pop3OilStoneLamp(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L2S4_Pop3OilStoneLamp();

		void HandleInventoryDrop(CPoint dropPos, int itemVal);

		void OnAnimationComplete(int animID);

		void Update(float ds );
		void Required();

		void ProcessHint();
		bool isTaskLeft();

	protected:
};

#endif