#ifndef DQFYLH_LEVEL2_SCENE2_POP3_KOOLAM_DEF
#define DQFYLH_LEVEL2_SCENE2_POP3_KOOLAM_DEF

#include "Popup.h"

class L2S2_Pop3Koolam: public Popup
{
	public:

		CPolygon  *OutRing1Poly;
		CPolygon  *OutRing2Poly;
		CPolygon  *OutRing3Poly;

		CPolygon  *RedColor1Poly;
		CPolygon  *RedColor2Poly;
		CPolygon  *RedColor3Poly;
		CPolygon  *RedColor4Poly;
		CPolygon  *RedColor5Poly;
		CPolygon  *RedColor6Poly;
		CPolygon  *RedColor7Poly;
		CPolygon  *RedColor8Poly;

		CPolygon  *GreenInsidePoly;
		CPolygon  *RedInsidePoly;
		CPolygon  *SnakePoly;

		CPolygon  *RedColorPoly;
		CPolygon  *YellowColorPoly;
		CPolygon  *BlueColorPoly;

		CObject *skipBtn;

		int L2_S2_pop3_R1Color;
		int L2_S2_pop3_R2Color;
		int L2_S2_pop3_R3Color;
		int L2_S2_pop3_R4Color;
		int L2_S2_pop3_R5Color;
		int L2_S2_pop3_R6Color;
		int L2_S2_pop3_R7Color;
		int L2_S2_pop3_R8Color;
		int L2_S2_pop3_R9Color;
		int L2_S2_pop3_R10Color;
		int L2_S2_pop3_R11Color;
		int L2_S2_pop3_R12Color;
		int L2_S2_pop3_R13Color;
		int L2_S2_pop3_R14Color;

		int ColorInHand;

		float sfxTimer;
		float skipPuzzleThreshold;
		float skipPuzzleTimer;

		SuperFX *StaticColorFX;
		SuperFX *DynamicColorFX;
		SuperFX *KolamEndFX;

		L2S2_Pop3Koolam(int popupId, HudControl *hud, CHOScene * scene, int zOrder, CPoint  zoomOrigin, std::function<void (Popup*)> popupCloseCallback);
		~L2S2_Pop3Koolam();

		void TakeInventoryItem(EHOInventory ItemID);
		void Update(float ds);
		void Required();

		CObject *popCloseBtn;
		float easing(float t,float b,float c,float d);
		void HandleInventoryDrop(CPoint dropPos, int itemVal);
		void SetUserKolamColors(CObject *tintObj,int color );
		void Skip();
		void MouseOutKolamColors();
		void SetTintKolamColors(CObject *tintObj);
		bool isTaskLeft();
		void OnPopupCloseBtnClicked();

	protected:

		void ReceiveMessage(int val);
		void ProcessHint();
};

#endif