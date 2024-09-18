//====================================
// Framework
// MULTI_LINE_TEXT
//====================================

#ifndef DQFULP_MULTILINETEXT_DEF
#define DQFULP_MULTILINETEXT_DEF

#include "BitmapText.h"
#include "Fonts.h"

#define MAX_TEXT_LINES 7

class MultilineText 
{
	public:
		CBitmapText  *LineTexts[MAX_TEXT_LINES];
		int numTextLines;

		void SetTextMultiline(std::string multilineString);
		void setAlpha(float alpha);
		void SetColor(ColorRGB color);
		void FadeOut(float speed);
		void SetPosXY(float x, float y);
		void SetScale(float scale);
		void Show(bool f);

		MultilineText( int numTextLines, CPoint *refPos, int zOrder, ColorRGB color, float lineSpacing, float maxLineWidth, eTextAlignment AlignStyle = Align_Center, int fontId = eFont_26 );
		virtual ~MultilineText();

	protected:
		CPoint pos;
		float _lineSpacing;
		float _maxLineWidth;
};

#endif