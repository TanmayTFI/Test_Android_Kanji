#include "BitmapText.h"
#include "Fonts.h"

#ifndef MULTILINE_TEXTBOX
#define MULTILINE_TEXTBOX

#define MAX_TEXTBOX_LINES 100

class MultilineTextBox
{
public:
	CBitmapText* LineTexts[MAX_TEXTBOX_LINES];
	int numTextLines;
	float lineSpacing, X, Y;
	ColorRGB COLOR;
	std::string text;
	int ZOrder, FONT;


	//void SetText(std::string multilineString);

	
	void setAlpha(float alpha);
	void SetColor(ColorRGB color);
	void SetPosXY(float x, float y);
	CPoint GetPos();
	void SetScale(float scale);
	void TranslateY(float dy);
	void CenterText();
	
	void Show(bool f);
	void FadeIn(bool f);
	void FadeOut(bool f);
	void SetEnable(bool f);
	void SetText(const std::string& text);

	void Render(const CPoint* pCam);

	MultilineTextBox(std::string string, float x, float y, int zOrder, ColorRGB color, int scale, float _lineSpacing);
	virtual ~MultilineTextBox();

protected:
	CPoint pos;
};

#endif