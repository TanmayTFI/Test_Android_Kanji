//===========================================================
// 
// Framework
//===========================================================
//===========================================================
// Bitmap Text only supports single-byte languages for now.
// Eventually, there will be a derived class to support
// double-byte languages.
//===========================================================

#ifndef BITMAP_TEXT_H
#define BITMAP_TEXT_H

#include "Object.h"
#include "LangDefs.h"

enum eTextAlignment
{
	Align_Center,
	Align_Left,
	Align_Right
};

//#ifdef JAPANESE_BUILD
#define HEIGHTPIX_20 20
#define HEIGHTPIX_21 19
#define HEIGHTPIX_22 22
#define HEIGHTPIX_SMALL 16
#define HEIGHTPIX_INV_POP 18
#define HEIGHTPIX_DIARY 15
#define HEIGHTPIX_DROP 14
//#else
	#define MAX_ASCII_CHAR 256 //255+1
//#endif

struct ColorRGB
{
	float r,g,b;
};

class CBitmapText : public CObject
{
	public:
		eTextAlignment Alignment;
		CPoint TruePos;		
		float R,G,B,A;
		float Scale;
		bool  TypeWriter;
		bool  TypeActive;
		float TypeTime;
		float TypeSpeed;
		int   StrikeThroughWidth;
		int   StrikeCharWidth;
		int   CurrentLength;
		float fontKerning;

		void  RecalcPosition();
		int   FontHeight;
		
		
//#ifdef JAPANESE_BUILD 	
		int HeightPix;

//#else
/*
		int ImageWidth;
		int ImageHeight;
		KGraphic	*Outline;
		KGraphic	*Image;

		int  CellWidth;
		int  CellHeight;
		int  CharWidths[MAX_ASCII_CHAR];
		void SetCellWidth(int Width){ CellWidth = Width; }  
//		void LoadArrayBitmapFont( int FontId );
//		void LoadCodeHeadBitmapFont(const std::string& FontData);				
		void LoadOutline(const std::string& OutlineImage);
		bool Truncate(std::string& in, int width, std::string& extra);
*/		
//#endif

		int  charWidth(unsigned char TextChar);	
		
		float Rotate;
		void SetRotate( float f ){ Rotate = f; };
		virtual void Render(const CPoint* pCam);
		int  StringWidth();
		


		bool IsInstantRemoval;
		bool StrikeThrough;
		std::string Text;
		

		CBitmapText();
		virtual ~CBitmapText();

		//Inherited members
		virtual void Update(float ds);		
		virtual void SetPos(const CPoint* pos);
		virtual bool Collide(const CPoint* pos);
		void  FadeOutText( float fadeFactor );
		void  FadeInText( float fadeFactor );
		bool  IsFadingOut;
		bool  IsFadingIn;
		float Fadefactor;
		

		//Bitmap text members
		void SetText(const std::string& text);
		void LoadFont(int FontId);
		void SetTextProps(const std::string& text, float x, float y, int zOrder, ColorRGB color, int fontId, eTextAlignment align=Align_Left);
		
		void SetColor(float r, float g, float b);
		void SetRelativeAlpha(float relalpha);
		void SetAlpha(float a);
		void SetStrikethrough(bool f, int width = 1);
		void SetTypeWriter(bool t, float speed = 1.5f);
		void FlushTypeWriter();
		void SetAlignment(eTextAlignment);
		void SetTypeActive(bool f);
		void SetScale(float scale);				
		bool IsTyping();
		
};

#endif
