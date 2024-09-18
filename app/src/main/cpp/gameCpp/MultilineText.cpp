//====================================
// Framework
// MultilineText
// faisal
//====================================

#include "MultilineText.h"

MultilineText::MultilineText( int numTextLines, CPoint *refPos, int zOrder, ColorRGB color, float lineSpacing, float maxLineWidth, eTextAlignment AlignStyle, int fontId )
{
	for (int i = 0; i < MAX_TEXT_LINES; i++)
	{
		LineTexts[i] = NULL;
		if( i<numTextLines )
		{
			LineTexts[i] = new CBitmapText();
			LineTexts[i]->SetZ(zOrder);
			LineTexts[i]->LoadFont(fontId);
			LineTexts[i]->SetText("String Placeholder");
			LineTexts[i]->SetAlignment(AlignStyle);
			LineTexts[i]->SetColor(color.r,color.g,color.b);
		}
	}
	pos.SetXY(refPos->x, refPos->y);
	this->numTextLines = numTextLines;
	_maxLineWidth = maxLineWidth;
	_lineSpacing = lineSpacing;
}

void MultilineText::SetColor(ColorRGB color)
{
	for (int i = 0; i < numTextLines; i++)
	{
		if(LineTexts[i]!=NULL)
			LineTexts[i]->SetColor(color.r,color.g,color.b);
	}
}

void MultilineText::setAlpha(float alpha)
{
	for (int i = 0; i < numTextLines; i++)
	{
		if(LineTexts[i]!=NULL)
			LineTexts[i]->SetAlpha(alpha);
	}
}

void MultilineText::SetPosXY(float x, float y)
{
	pos.SetXY(x, y);
}

void MultilineText::SetScale(float scale)
{
	_lineSpacing *= scale;
	for (int i = 0; i < numTextLines; i++)
	{
		if(LineTexts[i]!=NULL)
			LineTexts[i]->SetScale(scale);
	}
}

void MultilineText::FadeOut(float speed)
{
	/*for (int i = 0; i < numTextLines; i++)
	{
		if(LineTexts[i]!=NULL)
			LineTexts[i]->FadeInText(alpha);
	}*/
}

void MultilineText::Show(bool f)
{
	for (int i = 0; i < numTextLines; i++)
	{
		if(LineTexts[i]!=NULL)
			LineTexts[i]->Show(f);
	}
}


void MultilineText::SetTextMultiline(std::string multilineString)
{
	std::vector<std::string> Strings;
	std::string CurString = "";
	std::string CurWord = "";
	int CurWidth = 0;
	int WordLength = 0;
	//if(!StringTable::IsMyLangaugeLatin()) // checking if it is Latin based language or Japanese kind of language
	//{
		unsigned int Char; // treating all chars as unicode int
	//}	
//	else
//	{
//		unsigned char Char;
//	}
	
	//Hide all current strings
	CPoint textPos;
	for(int i=0; i < numTextLines; ++i)
	{
		LineTexts[i]->SetText(" ");

		textPos = pos;		
		textPos.y += _lineSpacing * i;		
		
		LineTexts[i]->SetPos(&textPos);
	}

	//Split String
	int length = (int)multilineString.length();
	int CharWidth = 0;
	for(int i=0; i < length; ++i)
	{
		Char = multilineString.at(i);//japanese will have multibyte characters!!
	if(!StringTable::IsMyLangaugeLatin()) // checking if it is Latin based language or Japanese kind of language
		CharWidth = 0;//We split words based on $ sign only!! :) 
	else
		CharWidth = LineTexts[0]->charWidth(Char);

		if( Char == '\\'  || Char == '$')//for japanese $
		{
			CurWidth = 0;
			Strings.push_back(CurString);

			CurString = "";
		}
		//If current word cannot fit on the line
		//add line, clear line, and add char to current word
		else if( (CharWidth + CurWidth + WordLength) > _maxLineWidth )
		{
			CurWidth = 0;
			Strings.push_back(CurString);

			CurString = "";

			CurWord += Char;
			WordLength += CharWidth;			
		}
		else
		{
			WordLength += CharWidth;
			CurWord += Char;

			//Hit a space or end of string, add a word
			if( Char == 32 || i == (length - 1))
			{
				CurWidth += WordLength;
				WordLength = 0;

				CurString += CurWord;
				CurWord = "";

				//Add last string
				if( i == (length - 1) )
				{
					Strings.push_back(CurString);
				}
			}
		}
	}
	int size = (int)Strings.size();  //Sac: converted Implicit to explicit
	if( size >= numTextLines)
	{
		LineTexts[0]->SetText("String is too big.");
	}
	else
	{
		int size = (int)Strings.size();
		if( size == 1 )
			LineTexts[1]->SetText(Strings[0]);
		else
		{
			for(int i=0; i < size; ++i)
			{
				if( size == 2 )
				{
					textPos.y = pos.y + (_lineSpacing * i)	+ (_lineSpacing/2);	
					LineTexts[i]->SetPos(&textPos);	
				}
				LineTexts[i]->SetText(Strings[i]);			
			}
		}
	}	
}

MultilineText::~MultilineText()
{
	for (int i = 0; i < numTextLines; i++)
	{
		if(LineTexts[i]!=NULL)
			delete LineTexts[i];
		LineTexts[i] = NULL;
	}
}
