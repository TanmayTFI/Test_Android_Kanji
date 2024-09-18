#include "BitmapText.h"
#include "Fonts.h"
#include "Application.h"
#include "MultilineTextBox.h"


MultilineTextBox::MultilineTextBox(std::string string, float x, float y, int zOrder, ColorRGB color, int font, float _lineSpacing)//, eTextAlignment AlignStyle = Align_Center, int fontId = eFont_26)
{
	lineSpacing = _lineSpacing;
	text = string;
	ZOrder = zOrder;
	X = x;
	Y = y;
	COLOR = color;
	FONT = font;

	int length = (int)text.length();

	float stringWidth = GFApp->lpFontDiary->getStringWidth(text.c_str(), 1, 1);

	numTextLines = 0;
	LineTexts[numTextLines] = NULL;
	LineTexts[numTextLines] = new CBitmapText();
	LineTexts[numTextLines]->SetZ(zOrder);
	LineTexts[numTextLines]->LoadFont(font);
	//GFApp->lpFontSmall->setHeightPix(fontSize);
	LineTexts[numTextLines]->SetText("");
	//LineTexts[numTextLines]->SetScale(scale);
	LineTexts[numTextLines]->SetAlignment(Align_Left);
	LineTexts[numTextLines]->SetColor(color.r, color.g, color.b);
	LineTexts[numTextLines]->SetXY(x, y + (lineSpacing * numTextLines));

	for (int i = 0; i < length; i++)
	{
		if (text[i] != '$')
		{
			LineTexts[numTextLines]->Text += text[i];
		}
		else
		{
			numTextLines++;
			LineTexts[numTextLines] = NULL;
			LineTexts[numTextLines] = new CBitmapText();
			LineTexts[numTextLines]->SetZ(zOrder);
			LineTexts[numTextLines]->LoadFont(font);
			//GFApp->lpFontSmall->setHeightPix(fontSize);
			LineTexts[numTextLines]->SetText("");
			//LineTexts[numTextLines]->SetScale(scale);
			LineTexts[numTextLines]->SetAlignment(Align_Left);
			LineTexts[numTextLines]->SetColor(color.r, color.g, color.b);
			LineTexts[numTextLines]->SetXY(x, y + (lineSpacing * numTextLines));
		}
	}
}

void MultilineTextBox::SetText(const std::string& text)
{
	int length = (int)text.length();

	for (int i = 0; i < numTextLines; i++);
	{
		LineTexts[numTextLines] = NULL;
		delete(LineTexts[numTextLines]);
	}

	numTextLines = 0;
	LineTexts[numTextLines] = NULL;
	LineTexts[numTextLines] = new CBitmapText();
	LineTexts[numTextLines]->SetZ(ZOrder);
	LineTexts[numTextLines]->LoadFont(FONT);
	//GFApp->lpFontSmall->setHeightPix(fontSize);
	LineTexts[numTextLines]->SetText("");
	//LineTexts[numTextLines]->SetScale(scale);
	LineTexts[numTextLines]->SetAlignment(Align_Left);
	LineTexts[numTextLines]->SetColor(COLOR.r, COLOR.g, COLOR.b);
	LineTexts[numTextLines]->SetXY(X, Y + (lineSpacing * numTextLines));

	for (int i = 0; i < length; i++)
	{
		if (text[i] != '$')
		{
			LineTexts[numTextLines]->Text += text[i];
		}
		else
		{
			numTextLines++;
			LineTexts[numTextLines] = NULL;
			LineTexts[numTextLines] = new CBitmapText();
			LineTexts[numTextLines]->SetZ(ZOrder);
			LineTexts[numTextLines]->LoadFont(FONT);
			//GFApp->lpFontSmall->setHeightPix(fontSize);
			LineTexts[numTextLines]->SetText("");
			//LineTexts[numTextLines]->SetScale(scale);
			LineTexts[numTextLines]->SetAlignment(Align_Left);
			LineTexts[numTextLines]->SetColor(COLOR.r, COLOR.g, COLOR.b);
			LineTexts[numTextLines]->SetXY(X, Y + (lineSpacing * numTextLines));
		}
	}
	/*

	for (int i = 0; i < length; i++)
	{
		if (text[i] != '$')
		{
			LineTexts[numTextLines]->Text += text[i];
		}
		else
		{
			numTextLines++;
				LineTexts[numTextLines] = NULL;
				LineTexts[numTextLines] = new CBitmapText();
				LineTexts[numTextLines]->SetZ(ZOrder);
				LineTexts[numTextLines]->LoadFont(FONT);
				//GFApp->lpFontSmall->setHeightPix(fontSize);
				LineTexts[numTextLines]->SetText("");
				//LineTexts[numTextLines]->SetScale(scale);
				LineTexts[numTextLines]->SetAlignment(Align_Left);
				LineTexts[numTextLines]->SetColor(COLOR.r, COLOR.g, COLOR.b);
				LineTexts[numTextLines]->SetXY(X, Y + (lineSpacing * numTextLines));
		}
	}
	*/
}

void MultilineTextBox::SetEnable(bool f)
{
	for (int i = 0; i < numTextLines; i++)
	{
		if (LineTexts[i] != NULL)
			LineTexts[i]->SetEnable(f);
	}
}

void MultilineTextBox::SetColor(ColorRGB color)
{
	for (int i = 0; i < numTextLines; i++)
	{
		if(LineTexts[i]!=NULL)
			LineTexts[i]->SetColor(color.r,color.g,color.b);
	}
}

void MultilineTextBox::setAlpha(float alpha)
{
	for (int i = 0; i < numTextLines; i++)
	{
		if(LineTexts[i]!=NULL)
			LineTexts[i]->SetAlpha(alpha);
	}
}

void MultilineTextBox::SetPosXY(float x, float y)
{
	for (int i = 0; i <= numTextLines; i++)
	{
		if (LineTexts[i] != NULL)
			LineTexts[i]->SetXY(x, y + (lineSpacing * i));
	}
}

CPoint MultilineTextBox::GetPos()
{
	int i = std::floor(numTextLines / 2);
	const CPoint *position = (LineTexts[i]->GetPosPointer());
	return *position;
}

void MultilineTextBox::TranslateY(float dy)
{
	for (int i = 0; i <= numTextLines; i++)
	{
		if (LineTexts[i] != NULL)
			LineTexts[i]->Pos.y += dy;
	}
}

void MultilineTextBox::CenterText()
{
	for (int i = 0; i <= numTextLines; i++)
	{
		if (LineTexts[i] != NULL)
		{
			LineTexts[i]->Pos.x -= LineTexts[i]->StringWidth() / 2;
		}
	}
}

void MultilineTextBox::SetScale(float scale)
{
	//lineSpacing *= scale;
	for (int i = 0; i < numTextLines; i++)
	{
		if(LineTexts[i]!=NULL)
			LineTexts[i]->SetScale(scale);
	}
}

void MultilineTextBox::Render(const CPoint* pCam)
{
	//lineSpacing *= scale;
	for (int i = 0; i < numTextLines; i++)
	{
		if (LineTexts[i] != NULL)
			LineTexts[i]->Render(pCam);
	}
}

void MultilineTextBox::Show(bool f)
{
	for (int i = 0; i <= numTextLines; i++)
	{
		if(LineTexts[i]!=NULL)
			LineTexts[i]->Show(f);
	}
}

void MultilineTextBox::FadeIn(bool f)
{
	for (int i = 0; i <= numTextLines; i++)
	{
		if (LineTexts[i] != NULL)
			LineTexts[i]->FadeIn(f);
	}
}

void MultilineTextBox::FadeOut(bool f)
{
	for (int i = 0; i <= numTextLines; i++)
	{
		if (LineTexts[i] != NULL)
			LineTexts[i]->FadeOut(f);
	}
}

/*
void MultilineTextBox::SetText(std::string multilineString)
{

	int length = (int)multilineString.length();

	float stringWidth = GFApp->lpFont20->getStringWidth(multilineString.c_str(), 1, 1);

	numTextLines = 0;
	for (int i = 0; i < length; i++)
	{
		if (multilineString[i] != '$')
		{
			LineTexts[numTextLines]->Text += multilineString[i];
		}
		else
		{
			numTextLines++;
		}
	}
}

/*
int MultilineTextBox::StringWidth(std::string Text, KTrueText* font, float fontKerning = 1, float Scale = 1)
{

	return 

}



*/

MultilineTextBox::~MultilineTextBox()
{
	for (int i = 0; i <= numTextLines; i++)
	{
		if(LineTexts[i]!=NULL)
			delete LineTexts[i];
		LineTexts[i] = NULL;
	}
}
