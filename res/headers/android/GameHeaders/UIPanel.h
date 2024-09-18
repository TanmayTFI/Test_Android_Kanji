//===========================================================
// UI Panel Container using mouseinterface
// Faisal
//===========================================================

#ifndef UIPANEL_H
#define UIPANEL_H

#include "Object.h"
#include "UIButton.h"
#include "MouseInterface.h"

class UIPanel : public CObject
{
	private:
		CObject	*_topPart, *_bottomPart;
		std::vector<CObject*> _midPartArray;
		UIButton *_closeButton;

		float _midPartHeight;

		std::vector<CObject*> _uiChildren;

	public:
		UIPanel();
		UIPanel(const std::string& panelAssetsPath, float x, float y, int zOrder, float panelHeight, float midPartHeight=20);
		~UIPanel();

		virtual void Update(float ds);

		void addChild(CObject* obj);
		
		//inherited
		virtual void SetPos(const CPoint* pos);

		void onCloseBtnClick();
		void onMouseOver();
		void onMouseOut();

};
#endif