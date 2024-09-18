#ifndef DQFYLH_L2_S3_POP1_SAW_DEF
#define DQFYLH_L2_S3_POP1_SAW_DEF

#include "Popup.h"

class L3S3_Pop1Saw: public Popup
{
	public:

		L3S3_Pop1Saw(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L3S3_Pop1Saw();

		bool isBannerShown;

		void HandleInventoryDrop(CPoint dropPos, int itemVal);

		void OnPopupOpened();

		void Update(float ds );
		void Required();

		void ProcessHint();
		bool isTaskLeft();

	protected:
};

#endif


