#ifndef DQFYLH_L3_S3_POP3_POOL_DEF
#define DQFYLH_L3_S3_POP3_POOL_DEF

#include "Popup.h"

#include "JSONAnimator.h"
#include "MeshObject.h"

class L3S3_Pop3Pool: public Popup
{
	public:
		JSONAnimator *JAnim_CupWaterTaking;

		MeshObject *mesh_Water;

		SuperFX *OilDropsVFX;

		L3S3_Pop3Pool(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L3S3_Pop3Pool();

		void HandleInventoryDrop(CPoint dropPos, int itemVal);

		void OnAnimationComplete(int animID);

		void Update(float ds );
		void Required();

		void ProcessHint();
		bool isTaskLeft();

	protected:
};

#endif