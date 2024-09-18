//====================================
//DQFULP
// Extras Menu
// Hidden Object System
//====================================

#ifndef DQFULP_EXTRAS_COMIC_DEF
#define DQFULP_EXTRAS_COMIC_DEF

#include "Control.h"
#include "BitmapText.h"
#include "JSONAnimator.h"
#include <string>

class CObject;
class CHOScene;

enum eComicState{
	eComic_Inactive,
	eComic_FadingIn,
	eComic_Active,
	eComic_FadingOut
};

class ExtrasComicControl : public Control
{
	public:

		ExtrasComicControl();
		~ExtrasComicControl();
		
		float PopupAlpha;

		float pageTurnTimer;

		CHOScene	*ExtrasComicScn;

		eComicState comicState;
		void ShowComicScene();

		void dispose();
		void setAllAlpha(float alpha=0);

		int bazeZOrder;

		int  pageIndex;

		CPoint centerPos, leftPagePos, rightPagePos;

		bool isPageTurning;

		//JSONAnimator *_JAnim_PageTurn;
		void OnAnimationComplete(int animID);

		//Inherited method
		void Update();
		void Required();

		void updateComic();

		void SetCloseComicCallback(std::function<void ()> closeComicCallback) {
			_closeComicCallback = closeComicCallback;
		}

		CObject* backButton, *backBtnTxt;

		CObject* pageLeft;
		CObject* pageRight;

		CObject* newLeftPage;
		CObject* newRightPage;

		void LoadCurrentPages(bool isQuickShow = false);

		bool isActive;

	protected:
		std::function<void ()> _closeComicCallback;

		CObject* _maskSpr;
};

#endif
