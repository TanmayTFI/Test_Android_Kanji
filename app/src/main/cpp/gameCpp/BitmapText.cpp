//===========================================================
// 
// Framework
//===========================================================
#include "KPTK.h"
#include "BitmapText.h"
#include "TextureFactory.h"
#include "IOStream.h"
#include "Rect.h"
#include "GameFlow.h"
#include "ParticleEffect.h"
#include "Util.h"
#include "Fonts.h"
#include "Application.h"

// #define min(a,b)            (((a) < (b)) ? (a) : (b))
// Sac: why redfine min ??
#ifndef JAPANESE_BUILD	
#define MAX_FONT_CHARS 259
#define RENDERY_OFFSET 18 // Sac: originally it was 10 and was embeded inside the code for rendering, I'm bringing it out to CONSTANT
#define ARABIC_RENDERX_OFFSET 0 
int eFont_Arr[eFont_Count][MAX_FONT_CHARS] = 
{	
	//ATF Antique - 25
	{
	   32
	  ,32
	  ,25
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,11
	  ,7
	  ,6
	  ,7
	  ,13
	  ,11
	  ,19
	  ,19
	  ,4
	  ,11
	  ,11
	  ,9
	  ,11
	  ,6
	  ,5
	  ,5
	  ,8
	  ,13
	  ,13
	  ,13
	  ,13
	  ,13
	  ,13
	  ,13
	  ,13
	  ,13
	  ,13
	  ,5
	  ,6
	  ,11
	  ,11
	  ,11
	  ,11
	  ,17
	  ,16
	  ,14
	  ,15
	  ,17
	  ,14
	  ,13
	  ,17
	  ,18
	  ,8
	  ,10
	  ,15
	  ,13
	  ,20
	  ,17
	  ,16
	  ,13
	  ,17
	  ,15
	  ,13
	  ,15
	  ,17
	  ,15
	  ,21
	  ,17
	  ,14
	  ,13
	  ,11
	  ,8
	  ,11
	  ,11
	  ,11
	  ,11
	  ,12
	  ,13
	  ,11
	  ,13
	  ,12
	  ,8
	  ,13
	  ,14
	  ,7
	  ,6
	  ,12
	  ,6
	  ,21
	  ,14
	  ,12
	  ,13
	  ,13
	  ,10
	  ,10
	  ,9
	  ,13
	  ,12
	  ,18
	  ,13
	  ,13
	  ,11
	  ,11
	  ,11
	  ,11
	  ,15
	  ,11
	  ,11
	  ,11
	  ,6
	  ,12
	  ,9
	  ,14
	  ,12
	  ,12
	  ,11
	  ,27
	  ,13
	  ,7
	  ,22
	  ,11
	  ,13
	  ,11
	  ,11
	  ,6
	  ,6
	  ,9
	  ,9
	  ,15
	  ,14
	  ,16
	  ,11
	  ,22
	  ,10
	  ,7
	  ,20
	  ,11
	  ,11
	  ,14
	  ,7
	  ,6
	  ,10
	  ,11
	  ,15
	  ,13
	  ,11
	  ,12
	  ,11
	  ,11
	  ,11
	  ,10
	  ,11
	  ,11
	  ,18
	  ,11
	  ,10
	  ,11
	  ,8
	  ,8
	  ,11
	  ,10
	  ,12
	  ,5
	  ,11
	  ,8
	  ,11
	  ,10
	  ,20
	  ,20
	  ,20
	  ,11
	  ,16
	  ,16
	  ,16
	  ,16
	  ,16
	  ,16
	  ,23
	  ,15
	  ,14
	  ,14
	  ,14
	  ,14
	  ,8
	  ,8
	  ,8
	  ,8
	  ,17
	  ,17
	  ,16
	  ,16
	  ,16
	  ,16
	  ,16
	  ,11
	  ,16
	  ,17
	  ,17
	  ,17
	  ,17
	  ,14
	  ,13
	  ,13
	  ,12
	  ,12
	  ,12
	  ,12
	  ,12
	  ,12
	  ,19
	  ,11
	  ,12
	  ,12
	  ,12
	  ,12
	  ,7
	  ,7
	  ,7
	  ,7
	  ,12
	  ,14
	  ,12
	  ,12
	  ,12
	  ,12
	  ,12
	  ,11
	  ,12
	  ,13
	  ,13
	  ,13
	  ,13
	  ,13
	  ,13
	  ,13
	},
	//ATF Antique 26
	{
	32
	,32
	,26
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,7
	,6
	,8
	,14
	,12
	,20
	,20
	,4
	,12
	,12
	,10
	,12
	,7
	,6
	,6
	,8
	,14
	,14
	,14
	,14
	,14
	,14
	,14
	,14
	,14
	,14
	,6
	,7
	,12
	,12
	,12
	,12
	,18
	,17
	,15
	,15
	,17
	,15
	,14
	,18
	,18
	,8
	,10
	,16
	,14
	,21
	,17
	,17
	,14
	,18
	,16
	,13
	,15
	,18
	,16
	,22
	,17
	,15
	,14
	,12
	,8
	,12
	,12
	,12
	,12
	,13
	,14
	,11
	,14
	,12
	,8
	,14
	,15
	,7
	,6
	,13
	,7
	,22
	,15
	,13
	,14
	,14
	,10
	,11
	,9
	,14
	,12
	,19
	,13
	,13
	,11
	,12
	,12
	,12
	,15
	,12
	,12
	,12
	,6
	,13
	,9
	,15
	,13
	,13
	,12
	,28
	,13
	,7
	,22
	,12
	,14
	,12
	,12
	,6
	,6
	,9
	,9
	,15
	,14
	,17
	,12
	,23
	,11
	,7
	,21
	,12
	,11
	,15
	,7
	,6
	,11
	,12
	,15
	,13
	,12
	,13
	,12
	,12
	,12
	,11
	,12
	,12
	,18
	,12
	,10
	,12
	,9
	,9
	,12
	,10
	,13
	,6
	,12
	,9
	,12
	,11
	,21
	,21
	,21
	,12
	,17
	,17
	,17
	,17
	,17
	,17
	,24
	,15
	,15
	,15
	,15
	,15
	,8
	,8
	,8
	,8
	,17
	,17
	,17
	,17
	,17
	,17
	,17
	,12
	,17
	,18
	,18
	,18
	,18
	,15
	,14
	,14
	,13
	,13
	,13
	,13
	,13
	,13
	,20
	,11
	,12
	,12
	,12
	,12
	,7
	,7
	,7
	,7
	,13
	,15
	,13
	,13
	,13
	,13
	,13
	,12
	,13
	,14
	,14
	,14
	,14
	,13
	,14
	,13
	},
	//ATF ANTIQUE ITALIC 24
	{
	32
	,32
	,24
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,11
	,6
	,6
	,7
	,12
	,11
	,18
	,18
	,4
	,11
	,11
	,9
	,11
	,6
	,5
	,5
	,7
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,12
	,5
	,6
	,11
	,11
	,11
	,11
	,16
	,15
	,14
	,14
	,16
	,13
	,13
	,16
	,17
	,8
	,9
	,15
	,13
	,19
	,16
	,15
	,13
	,16
	,14
	,12
	,14
	,16
	,15
	,20
	,16
	,14
	,13
	,11
	,7
	,11
	,11
	,11
	,11
	,11
	,13
	,10
	,13
	,11
	,8
	,13
	,13
	,6
	,6
	,11
	,6
	,20
	,13
	,12
	,13
	,13
	,9
	,10
	,8
	,13
	,11
	,17
	,12
	,12
	,10
	,11
	,11
	,11
	,14
	,11
	,11
	,11
	,6
	,12
	,8
	,14
	,12
	,12
	,11
	,26
	,12
	,6
	,21
	,11
	,13
	,11
	,11
	,6
	,6
	,8
	,8
	,14
	,13
	,15
	,11
	,21
	,10
	,6
	,19
	,11
	,10
	,14
	,6
	,6
	,10
	,11
	,14
	,12
	,11
	,12
	,11
	,11
	,11
	,10
	,11
	,11
	,17
	,11
	,9
	,11
	,8
	,8
	,11
	,9
	,12
	,5
	,11
	,8
	,11
	,10
	,19
	,19
	,19
	,11
	,15
	,15
	,15
	,15
	,15
	,15
	,22
	,14
	,13
	,13
	,13
	,13
	,8
	,8
	,8
	,8
	,16
	,16
	,15
	,15
	,15
	,15
	,15
	,11
	,15
	,16
	,16
	,16
	,16
	,14
	,13
	,13
	,11
	,11
	,11
	,11
	,11
	,11
	,18
	,10
	,11
	,11
	,11
	,11
	,6
	,6
	,6
	,6
	,12
	,13
	,12
	,12
	,12
	,12
	,12
	,11
	,12
	,13
	,13
	,13
	,13
	,12
	,13
	,12
	}
	
};

#endif

CBitmapText::CBitmapText()
:	R(1.0f),
	G(1.0f),
	B(1.0f),
	A(1.0f),
	FontHeight(0),
	StrikeThrough(false),
	Text("")
{

//#ifdef JAPANESE_BUILD 
	HeightPix = 0;

//#else
/*
	Image = NULL;
	Outline = NULL;
	ImageWidth = 0;
	ImageHeight = 0;
	
	CellWidth = 0;
	CellHeight = 0;	
//#endif
*/
	Rotate = 0.0f;

	IsFadingOut = false;
	IsFadingIn = false;
	Fadefactor = 0.0f;

	TypeWriter = false;
	TypeActive = false;
	IsInstantRemoval = false;
	TypeTime = 0.0f;
	TypeSpeed  = 1.5f;
	CurrentLength = 0;
	StrikeThroughWidth = 1;
	StrikeCharWidth = 0;
	Scale = 1.0f;		
	
	switch(StringTable::GetMyLangaugeID())
	{
			case LN_JAPANESE:
			case LN_KOREAN:
			case LN_SIMPLIFIED_CHINESE:
			case LN_TRADITIONAL_CHINESE:
				fontKerning=0.0f;
				break;
			case LN_HINDI:
				fontKerning=0.5f;
				break;
			case LN_SPANISH:
			case LN_GERMAN:
			case LN_FRENCH:
			case LN_ITALIAN:
			case LN_PORTUGUESE:
			case LN_BRAZILIAN_PORTUGUESE:
			case LN_RUSSIAN:
			case LN_UKRAINIAN:
			case LN_NEDERLANDS:
			case LN_HUNGARIAN:
			case LN_ENGLISH:
			default:
			fontKerning=1.0f;
			break;
	}
	Alignment = Align_Center;
	Visible = true;//CRenderListEntity
}

void CBitmapText::SetTextProps(const std::string& text, float x, float y, int zOrder, ColorRGB color, int fontId, eTextAlignment align)
{
	LoadFont(fontId);
	Alignment = align;
	
	TypeActive = false;
	Text = text;
	CurrentLength = 0; //for typewriter
	
	SetZ(zOrder);

	//Color
	R = color.r;
	G = color.g;
	B = color.b;

	//Set Pos
	TruePos.x = x;	TruePos.y = y;
	RecalcPosition();
}

CBitmapText::~CBitmapText()
{
	/*
#ifndef JAPANESE_BUILD 
	CTexFactory::Instance->FreeImage(&Image);
	CTexFactory::Instance->FreeImage(&Outline);
#endif
*/
	Text = "";	
}

void CBitmapText::Update(float ds)
{
	if( IsFadingOut )
	{
		A -= Fadefactor*Control::LogicRate;
		if( A <= 0.0f )
		{
			A = 0.0f;
			IsFadingOut = false;
		}
	}

	if( IsFadingIn )
	{
		A += Fadefactor*Control::LogicRate;
		if( A >= 1.0f )
		{
			A = 1.0f;
			IsFadingIn = false;
		}
	}

	if( Visible )
	{
		if( IsTyping() && TypeActive )
		{
			TypeTime += ds;

			if( TypeTime >= TypeSpeed )
			{
				TypeTime = 0;
				CurrentLength++;
			}
		}		
	}
}

//Centers the text on the rendering point
void CBitmapText::SetPos(const CPoint* pos)
{
	TruePos = (*pos);
	RecalcPosition();
}

void CBitmapText::SetScale(float scale)
{
	Scale = scale;
	RecalcPosition();
}

void CBitmapText::RecalcPosition()
{
	int stringWidth = (int)(StringWidth());

	Pos.y = TruePos.y - (FontHeight/2)*Scale;

	switch( Alignment )
	{
		case Align_Center:
			Pos.x = TruePos.x - stringWidth/2;
			break;
		case Align_Left:
			Pos.x = TruePos.x;
			break;
		case Align_Right:
			Pos.x = TruePos.x - stringWidth;
			break;
	}
}

void CBitmapText::SetText(const std::string& text)
{
	if(text == Text)
		return;

	TypeActive = false;

	Text = text;

	CurrentLength = 0; //for typewriter

	RecalcPosition();
}

void CBitmapText::LoadFont(int FontId )
{
//#if defined( JAPANESE_BUILD )						   	
	switch( FontId )
	{
		default:
/*
		case eFont_Italic24:HeightPix = 20;break;
		case eFont_25      :HeightPix = 21;break;
		case eFont_26      :HeightPix = 22;break;
*/
		case eFont_Italic24:HeightPix = HEIGHTPIX_20;break;
		case eFont_25      :HeightPix = HEIGHTPIX_21;break;
		case eFont_26      :HeightPix = HEIGHTPIX_22;break;		
		case eFont_Small   :HeightPix = HEIGHTPIX_SMALL;break;
		case eFont_InvPop  :HeightPix = HEIGHTPIX_INV_POP;break;
		case eFont_Diary   :HeightPix = HEIGHTPIX_DIARY; break;
		case eFont_Drop:HeightPix = HEIGHTPIX_DROP; break;

	}		
//#else
	//ATF ANTIQUE 25, 26, Italic 24
/*
	Image = CTexFactory::Instance->GetImage(FontImage[FontId]);
	Image->setTextureQuality(true);
	ImageWidth  = (int)Image->getWidth();
	ImageHeight = (int)Image->getHeight();

	LoadArrayBitmapFont(FontId);
*/
//#endif
}


void CBitmapText::SetColor(float r, float g, float b)
{
	R = r;
	G = g;
	B = b;
}

void CBitmapText::SetAlpha(float a)
{
	A = a;
}

void CBitmapText::SetRelativeAlpha(float relalpha)
{
	A = A * relalpha;
}


void CBitmapText::SetStrikethrough(bool f, int width)
{
	StrikeThrough = f;
	StrikeThroughWidth = 0;
	StrikeCharWidth = 0;			
}

void CBitmapText::SetAlignment(eTextAlignment a)
{
	Alignment = a;
}


void CBitmapText::SetTypeWriter(bool t, float speed)
{
	TypeWriter = t;
	TypeTime = 0.0f;
	TypeSpeed = speed;
	CurrentLength = 0;
}

void CBitmapText::SetTypeActive(bool f)
{
	TypeActive = f;
}

void CBitmapText::FlushTypeWriter()
{
	CurrentLength = (int)Text.length();
}

bool CBitmapText::IsTyping()
{
	return (TypeWriter ) && (CurrentLength != (int)Text.length());
}

bool CBitmapText::Collide(const CPoint* pos)
{
	CRectangle rect(Pos.x, Pos.y, (float)StringWidth(), (float)FontHeight);

	return rect.Intersection(pos);
}

void  CBitmapText::FadeOutText( float fadeFactor )
{
	A = 1.0f;
	IsFadingOut = true;
	Fadefactor = fadeFactor;
}

void CBitmapText::FadeInText( float fadeFactor )
{
	A = 0.0f;
	IsFadingIn = true;
	Fadefactor = fadeFactor;	
}

///////////////////////////////////////////////////////////////////////////////
/*
//#ifndef JAPANESE_BUILD 
bool CBitmapText::Truncate(std::string& in, int maxWidth, std::string& out)
{
	int i=0;
	int width=0;
	std::string original = in;
	in = "";
	
	int length = (int)original.length();
	while( width < maxWidth && i < length )
	{
		width += charWidth( original.at(i) );
		in += original.at(i);

		++i;
	}

	if( in.length() == original.length() )
		return false;

	out = original.substr(in.length());

	return true;
}
void CBitmapText::LoadOutline(const std::string& FontImage)
{
	Outline = CTexFactory::Instance->GetImage(FontImage);
	Outline->setTextureQuality(true);
}


void CBitmapText::LoadArrayBitmapFont( int FontId )
{	
	//Read cell width
	CellWidth  = eFont_Arr[FontId][0];
	CellHeight = eFont_Arr[FontId][1];

	//Font Height
	FontHeight = eFont_Arr[FontId][2];

	//Read character widths
	for(int i=0; i < MAX_ASCII_CHAR; ++i)
		CharWidths[i] = eFont_Arr[FontId][i+3];		
}

//Also Pass Image path also faisal - Custom loader to support CodeHead's BitmapFont Generator *.csv file, loading csv file is too much overhead
void CBitmapText::LoadCodeHeadBitmapFont(const std::string& FontData)
{	
	*/
	/*Image = CTexFactory::Instance->GetImage("FNT\\COURIERNEW24.PNG");
	Image->setTextureQuality(true);
	ImageWidth  = (int)Image->getWidth();
	ImageHeight = (int)Image->getHeight();*/
/*
	CIOStream stream;
	if(!stream.Open(FileSystem::strPath(FontData), File_Read))//to Pak_Read
	{
		std::string SaveErrorMsg = std::string("Game can't read font file ")+FileSystem::strPath(FontData);
		K_LOG(SaveErrorMsg.c_str());
		#ifdef K_WIN32
			FATAL((const char*)(FileSystem::s2ws(SaveErrorMsg).c_str()) );
		#else
			FATAL((const char*)(SaveErrorMsg.c_str()) );
		#endif
	}

	//Discard lines 1-2
	stream.ReadString();
	stream.ReadString();

	//Read cell width
	CellWidth  = stream.ReadIntSplit(',');
	CellHeight = stream.ReadIntSplit(',');

	//Discard lines 5-6
	stream.ReadString();
	stream.ReadString();

	//Font Height
	FontHeight = stream.ReadIntSplit(',');

	//Discard line 8
	stream.ReadString();

	//Read character widths
	for(int i=0; i < MAX_ASCII_CHAR; ++i)
	{
		CharWidths[i] = stream.ReadIntSplit(',');
	}
	//Discard rest of the stream	
	stream.Close();
}
*/
//#endif



int CBitmapText::charWidth(unsigned char TextChar)
{
//#if defined( JAPANESE_BUILD )
	switch( HeightPix )
	{
		default:
		case HEIGHTPIX_20:
			if( GFApp->lpFont20 )
			{
				char TextCharStr[2];
				TextCharStr[0]= TextChar;
				TextCharStr[1]= '\0'; 		
				return (int)GFApp->lpFont20->getNCharWidth(TextCharStr,1,fontKerning,Scale);
			}
			else
				return 0;

		case HEIGHTPIX_DIARY:
			if (GFApp->lpFontDiary)
			{
				char TextCharStr[2];
				TextCharStr[0] = TextChar;
				TextCharStr[1] = '\0';
				return (int)GFApp->lpFontDiary->getNCharWidth(TextCharStr, 1, fontKerning, Scale);
			}
			else
				return 0;

		case HEIGHTPIX_21:
			if( GFApp->lpFont21 )
			{
				char TextCharStr[2];
				TextCharStr[0]= TextChar;
				TextCharStr[1]= '\0'; 		
				return (int)GFApp->lpFont21->getNCharWidth(TextCharStr,1,fontKerning,Scale);
			}
			else
				return 0;
		case HEIGHTPIX_22:
			if( GFApp->lpFont22 )
			{
				char TextCharStr[2];
				TextCharStr[0]= TextChar;
				TextCharStr[1]= '\0'; 		
				return (int)GFApp->lpFont22->getNCharWidth(TextCharStr,1,fontKerning,Scale);
			}
			else
				return 0;
			case HEIGHTPIX_SMALL:
			if( GFApp->lpFontSmall )
			{
				char TextCharStr[2];
				TextCharStr[0]= TextChar;
				TextCharStr[1]= '\0'; 		
				return (int)GFApp->lpFontSmall->getNCharWidth(TextCharStr,1,fontKerning,Scale);
			}
			else
				return 0;
		case HEIGHTPIX_INV_POP:
			if( GFApp->lpInvPopFont )
			{
				char TextCharStr[2];
				TextCharStr[0]= TextChar;
				TextCharStr[1]= '\0'; 		
				return (int)GFApp->lpInvPopFont->getNCharWidth(TextCharStr,1,fontKerning,Scale);
			}
			else
				return 0;

		case HEIGHTPIX_DROP:
			if (GFApp->lpFontDropShadow)
			{
				char TextCharStr[2];
				TextCharStr[0] = TextChar;
				TextCharStr[1] = '\0';
				return (int)GFApp->lpFontDropShadow->getNCharWidth(TextCharStr, 1, fontKerning, Scale);
			}
			else
				return 0;
	}
//#else
/*
	if( Image == NULL )
		return 0;	
	
	int i = (int)TextChar;

	if(i < 0 || i >= MAX_ASCII_CHAR )
	{
		std::string TmpStr = std::string("Bitmap Text index is out of ASCII range" + MKSTR(i)).c_str();		
		#ifdef K_WIN32
			WARN( (const char*)(FileSystem::s2ws(TmpStr).c_str()) );//WIDESTR WARNINGS		
		#else
			WARN( (const char*)(TmpStr.c_str()) );//WIDESTR WARNINGS		
		#endif
	}
	if(CharWidths[i] < 0 || CharWidths[i] > 100)
	{
		std::string TmpStr = std::string("Bitmap Text character has unexpected width: " + MKSTR(CharWidths[i])).c_str();		
		#ifdef K_WIN32
			WARN( (const char*)(FileSystem::s2ws(TmpStr).c_str()) );//WIDESTR WARNINGS		
		#else
			WARN( (const char*)(TmpStr.c_str()) );//WIDESTR WARNINGS		
		#endif
	}
	return CharWidths[i];
#endif
*/
}


int CBitmapText::StringWidth()
{
	
//#if defined( JAPANESE_BUILD )	
	switch( HeightPix )
	{
		default:
		case HEIGHTPIX_20:
			if( GFApp->lpFont20 )	
				return (int)GFApp->lpFont20->getStringWidth(Text.c_str(),fontKerning,Scale);	
			else
				return 0;
		case HEIGHTPIX_DIARY:
			if (GFApp->lpFontDiary)
				return (int)GFApp->lpFontDiary->getStringWidth(Text.c_str(), fontKerning, Scale);
			else
				return 0;
		case HEIGHTPIX_21:
			if( GFApp->lpFont21 )	
				return (int)GFApp->lpFont21->getStringWidth(Text.c_str(),fontKerning,Scale);	
			else
				return 0;
		case HEIGHTPIX_22:
			if( GFApp->lpFont22 )	
				return (int)GFApp->lpFont22->getStringWidth(Text.c_str(),fontKerning,Scale);	
			else
				return 0;
		case HEIGHTPIX_SMALL:
			if( GFApp->lpFontSmall )	
				return (int)GFApp->lpFontSmall->getStringWidth(Text.c_str(),fontKerning,Scale);	
			else
				return 0;
		case HEIGHTPIX_DROP:
			if (GFApp->lpFontDropShadow)
				return (int)GFApp->lpFontDropShadow->getStringWidth(Text.c_str(), fontKerning, Scale);
			else
				return 0;
		case HEIGHTPIX_INV_POP:
			if( GFApp->lpInvPopFont )	
				return (int)GFApp->lpInvPopFont->getStringWidth(Text.c_str(),fontKerning,Scale);	
			else
				return 0;
	}
/*
#else
	int w = 0;

	int textLen = (int)Text.length(); //Sac: converted Implicit to explicit
	for(int i=0; i < textLen; ++i)
	{
		w += charWidth(Text.at(i));
	}

	return (int)(w* Scale);
#endif
*/
}


#define GLOW_LR_XTRAOFFSET 10
void CBitmapText::Render(const CPoint* pCam)
{
	if(!IsActive)
		return;
	
//#if defined( JAPANESE_BUILD )
	float RenderX = (Pos.x - pCam->x);
	float RenderY = (Pos.y - pCam->y);
	
	if( Text == "" )
		return;	
//	if( Text == "" )
//		return;
	switch( HeightPix )
	{
		default:
		case HEIGHTPIX_20:
			if( GFApp->lpFont20 )//draw truetext font
			{		
				GFApp->lpFont20->setBlitColor (R, G, B, A);
			//	K_LOG("~~~~~~~~~~~~~~~~~~~~~~~~~~Font20 The text to show is '%s'",Text.c_str());
				if(StringTable::GetMyLangaugeID()==LN_ARABIC)
					GFApp->lpFont20->drawStringFromRightF (Text.c_str(),RenderX+StringWidth(), RenderY-RENDERY_OFFSET*Scale,fontKerning);
				else
					GFApp->lpFont20->drawStringFromLeftF (Text.c_str(),RenderX, RenderY-RENDERY_OFFSET*Scale,fontKerning,Scale);
				GFApp->lpFont20->setBlitColor (1, 1, 1, 1);
				RenderX += (float)GFApp->lpFont20->getStringWidth(Text.c_str(),fontKerning,Scale);
			} 
			break;
		case HEIGHTPIX_DIARY:
			if (GFApp->lpFontDiary)//draw truetext font
			{
				GFApp->lpFontDiary->setBlitColor(R, G, B, A);
				//	K_LOG("~~~~~~~~~~~~~~~~~~~~~~~~~~Font20 The text to show is '%s'",Text.c_str());
				if (StringTable::GetMyLangaugeID() == LN_ARABIC)
					GFApp->lpFontDiary->drawStringFromRightF(Text.c_str(), RenderX + StringWidth(), RenderY - RENDERY_OFFSET * Scale, fontKerning);
				else
					GFApp->lpFontDiary->drawStringFromLeftF(Text.c_str(), RenderX, RenderY - RENDERY_OFFSET * Scale, fontKerning, Scale);
				GFApp->lpFontDiary->setBlitColor(1, 1, 1, 1);
				RenderX += (float)GFApp->lpFontDiary->getStringWidth(Text.c_str(), fontKerning, Scale);
			}
			break;
		case HEIGHTPIX_21:
			if( GFApp->lpFont21 )//draw truetext font
			{		
				GFApp->lpFont21->setBlitColor (R, G, B, A);
			//	K_LOG("~~~~~~~~~~~~~~~~~~~~~~~~~~Font21 The text to show is '%s'",Text.c_str());
				if(StringTable::GetMyLangaugeID()==LN_ARABIC)
					GFApp->lpFont21->drawStringFromRightF (Text.c_str(),RenderX+StringWidth(), RenderY-RENDERY_OFFSET*Scale,fontKerning);
				else
					GFApp->lpFont21->drawStringFromLeftF (Text.c_str(),RenderX, RenderY-RENDERY_OFFSET*Scale,fontKerning,Scale);
				GFApp->lpFont21->setBlitColor (1, 1, 1, 1);
				RenderX += (float)GFApp->lpFont21->getStringWidth(Text.c_str(),fontKerning,Scale);
			} 
			break;
		case HEIGHTPIX_22:
			if( GFApp->lpFont22 )//draw truetext font
			{		
				GFApp->lpFont22->setBlitColor (R, G, B, A);
			//	K_LOG("~~~~~~~~~~~~~~~~~~~~~~~~~~Font22 The text to show is '%s'",Text.c_str());
				if(StringTable::GetMyLangaugeID()==LN_ARABIC)
					GFApp->lpFont22->drawStringFromRightF (Text.c_str(),RenderX+StringWidth(), RenderY-RENDERY_OFFSET*Scale,fontKerning);
				else
					GFApp->lpFont22->drawStringFromLeftF (Text.c_str(),RenderX, RenderY-RENDERY_OFFSET*Scale,fontKerning,Scale);
				GFApp->lpFont22->setBlitColor (1, 1, 1, 1);
				RenderX += (float)GFApp->lpFont22->getStringWidth(Text.c_str(),fontKerning,Scale);
				
			} 
			break;
		case HEIGHTPIX_SMALL:
			if( GFApp->lpFontSmall )//draw truetext font
			{		
				GFApp->lpFontSmall->setBlitColor (R, G, B, A);
			//	K_LOG("~~~~~~~~~~~~~~~~~~~~~~~~~~Font18 The text to show is '%s'",Text.c_str());
				if(StringTable::GetMyLangaugeID()==LN_ARABIC)
					GFApp->lpFontSmall->drawStringFromRightF (Text.c_str(),RenderX+StringWidth(), RenderY-RENDERY_OFFSET*Scale,fontKerning);
				else
					GFApp->lpFontSmall->drawStringFromLeftF (Text.c_str(),RenderX, RenderY-RENDERY_OFFSET*Scale + 5,fontKerning,Scale);
				GFApp->lpFontSmall->setBlitColor (1, 1, 1, 1);
				RenderX += (float)GFApp->lpFontSmall->getStringWidth(Text.c_str(),fontKerning,Scale);
				
			} 
			break;
		case HEIGHTPIX_DROP:
			if (GFApp->lpFontDropShadow)//draw truetext font
			{
				GFApp->lpFontDropShadow->setBlitColor(R, G, B, A);
				//	K_LOG("~~~~~~~~~~~~~~~~~~~~~~~~~~Font18 The text to show is '%s'",Text.c_str());
				if (StringTable::GetMyLangaugeID() == LN_ARABIC)
					GFApp->lpFontDropShadow->drawStringFromRightF(Text.c_str(), RenderX + StringWidth(), RenderY - RENDERY_OFFSET * Scale, fontKerning);
				else
					GFApp->lpFontDropShadow->drawStringFromLeftF(Text.c_str(), RenderX, RenderY - RENDERY_OFFSET * Scale + 5, fontKerning, Scale);
				GFApp->lpFontDropShadow->setBlitColor(1, 1, 1, 1);
				RenderX += (float)GFApp->lpFontDropShadow->getStringWidth(Text.c_str(), fontKerning, Scale);

			}
			break;
		case HEIGHTPIX_INV_POP:
			if( GFApp->lpInvPopFont )//draw truetext font
			{		
				GFApp->lpInvPopFont->setBlitColor (R, G, B, A);
			//	K_LOG("~~~~~~~~~~~~~~~~~~~~~~~~~~Font18 The text to show is '%s'",Text.c_str());
				if(StringTable::GetMyLangaugeID()==LN_ARABIC)
					GFApp->lpInvPopFont->drawStringFromRightF (Text.c_str(),RenderX+StringWidth(), RenderY-RENDERY_OFFSET*Scale,fontKerning);
				else
					GFApp->lpInvPopFont->drawStringFromLeftF (Text.c_str(),RenderX, RenderY-RENDERY_OFFSET*Scale + 5,fontKerning,Scale);
				GFApp->lpInvPopFont->setBlitColor (1, 1, 1, 1);
				RenderX += (float)GFApp->lpInvPopFont->getStringWidth(Text.c_str(),fontKerning,Scale);
				
			} 
			break;
	}
/*	
#else
	short RenderX = (short)(Pos.x - pCam->x);
	short RenderY = (short)(Pos.y - pCam->y);

	if( Image == NULL || Text == "" )
		return;	

	Image->setBlitColor(R,G,B,A);
	
	if( Outline )
		Outline->setBlitColor(1.0f-R, 1.0f-G, 1.0f-B, 1.0f);
		*/
/*
	int length = (int)Text.length();

	if( IsTyping() )
		length = g_min(length, CurrentLength);
		// Sac: changed min to g_min
	int CellCols  = ImageWidth / CellWidth;		
	float tx, ty;
	for(int i=0; i < length; ++i)
	{
		int ch = (int)((unsigned char)Text[i]) - 32;		
		if( ch!= 0 )//No need of rendering empty characters!!
		{
			tx = float((ch%CellCols)*CellWidth);
			ty = float((ch/CellCols)*CellHeight);

			Image->setTextureQuality(true);
			if( Rotate != 0.0f )//rotated text!!
				Image->blitAlphaRectFx( tx, ty, tx + CellWidth-1, ty + CellHeight-1,
								  RenderX, short(RenderY-(RenderX*Elixir::Sin(Rotate)/Elixir::Cos(Rotate))),Rotate,Scale, A);
			else //normal text
				Image->blitAlphaRectFx( tx, ty, tx + CellWidth-1, ty + CellHeight-1,
								  RenderX, RenderY,Rotate,Scale, A);
			if( Outline )
			{
				Outline->setTextureQuality(true);
				Outline->blitAlphaRectFx( tx, ty, tx+ CellWidth-1, ty + CellHeight-1,
								  RenderX-1, RenderY,Rotate,Scale, A);
			}		
		}
		RenderX += (short)(((short)CharWidths[ch+32]) * Scale) ;
	}
	Image->setBlitColor(1,1,1,1);  

	if( Outline )
		Outline->setBlitColor(1.0f, 1.0f, 1.0f, 1.0f);
//#endif
*/
	if( StrikeThrough && !IsInstantRemoval )
	{		
		bool IsStriking = false;
		if( (Pos.x-pCam->x+StrikeCharWidth) < (RenderX+GLOW_LR_XTRAOFFSET) )
		{
			StrikeCharWidth += 3;								
			IsStriking = true;				
		}
		//Fully rendered - time to fade out from 1.0f => 0.75f
		else 
		{
			if( A > 0.5f )
				A -= Control::LogicRate;				
		}

		if( IsStriking )
		{
			int EndX = (int)(Pos.x- pCam->x+StrikeCharWidth +1), 
				  EndY = (int)(RenderY + (FontHeight/2)-2);

			GFInstance->strikeGlowFX->SetXY((float)(EndX-GLOW_LR_XTRAOFFSET),(float)EndY);
			GFInstance->strikeGlowFX->Emit();			
		}			  		
	}
}
