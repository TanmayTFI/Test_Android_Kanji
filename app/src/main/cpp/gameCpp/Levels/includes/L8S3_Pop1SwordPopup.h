#ifndef DQFYLH_LEVEL8_SCENE3_POP1_SWORD_DEF
#define DQFYLH_LEVEL8_SCENE3_POP1_SWORD_DEF

#include "Popup.h"
#include "JSONAnimator.h"
#include "L8S3_Pop1CrystalKey.h"

class L8S3_Pop1SwordPopup: public Popup
{
	bool isSwordPieceInHand;
	int currentItem;
	int activePopupId;
	
	CPolygon *SwordPoly;
	JSONAnimator *Janim_sword;
	L8S3_Pop1CrystalKey *popPuzzle;
	public:
		L8S3_Pop1SwordPopup(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L8S3_Pop1SwordPopup();

		void ClosePopup(float delayTime = 0);
		void Update(float ds );
		void Required();
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		float easing(float t,float b,float c,float d);

		void OnAnimComplete(int animID);
		void OnPopupClosed(Popup * popup);

	protected:
		void ProcessHint();
		bool isTaskLeft();
};

#endif