//===========================================================
// 
// Framework
//===========================================================

#include "Sprite.h"
#include "TextureFactory.h"
#include "Util.h"
#include "Resources.h"
#include "Console.h"
#include <iostream>
#include <istream>
#include <string>

CSprite::CSprite( std::string Name )
:	Flipped(false),
	Visible(true),
	CurrentAnim(0),
	Image(NULL)
{
	Alpha = 1.0f;
	Scale = 1.0f;

	Rotate = 0.0f;
	BlendMode = eBlend_AlphaNormal;
	R = 1.0f;
	B = 1.0f;
	G = 1.0f;

	pivotX=0.0f;
	pivotY=0.0f;

	LeftMask = 0.0f;
	TopMask = 0.0f;
	RightMask = 0.0f;
	BottomMask = 0.0f;
	IsMasked = false;

	FailFlag = true;
	Load( Name );
}

//Only translational masking works perfect. Avoid Rotational masking.
void CSprite::SetMaskParams( float Left, float Top, float Right, float Bottom )
{
	LeftMask = Left;
	TopMask = Top;
	RightMask = Right;
	BottomMask = Bottom;
	IsMasked = true;
}

CSprite::~CSprite()
{
	int AnimSize = (int)Animations.size();	
	for(int i=0; i < AnimSize; ++i)	
	{			
		int FrameSize = (int)Animations[i]->Frames.size();
		for(int j=0; j < FrameSize; ++j)
		{			
			int CellSize = (int)Animations[i]->Frames[j]->Cells.size();
			for(int k=0; k < CellSize; ++k)
			{
				delete Animations[i]->Frames[j]->Cells[k];
				Animations[i]->Frames[j]->Cells[k] = NULL;
			}
			Animations[i]->Frames[j]->Cells.clear();

			delete Animations[i]->Frames[j];
			Animations[i]->Frames[j] = NULL;
		}
		Animations[i]->Frames.clear();

		delete Animations[i]; 
		Animations[i] = NULL;
	}
	Animations.clear();

	CTexFactory::Instance->FreeImage( &Image );	
}

void CSprite::Update(float ds)
{
	if( Animations[CurrentAnim]->Update(ds) )
	{
		//PlayAnimation( Animations[CurrentAnim]->NextAnimation );
	}
}

void CSprite::Render( const CPoint* pScreenPos, float xcamOffset)
{
	if( Visible == false )
		return;

	if(Image == NULL)
		return;

	CAnimation *Anim = Animations[CurrentAnim];
	CFrame *Frame = Anim->CurrentFrame();

	CCell *Cell = NULL;

	int CellSize = (int)Frame->Cells.size();
	Image->setBlitColor(R,G,B,Alpha);
	Image->setAlphaMode(BlendMode);

	for(int i=0; i < CellSize; ++i)
	{
		Cell = Frame->Cells[i];

		//center
		float centerx = Cell->rx + Cell->w/2.0f;
		float centery = Cell->ry + Cell->h/2.0f;

		float transx = centerx - (Scale * centerx);
		float transy = centery - (Scale * centery);

		float RenderXOffset = Cell->rx;
		float RenderYOffset = Cell->ry;

		float Rotation = Cell->rot;

		if( Flipped )
		{
			//Translate for horz flip
			RenderXOffset = Cell->rx - (Scale*centerx)*2;

			//Compensate rotate for flipped
			if(Rotation <= 180.0f)
				Rotation = 180.f - Rotation;
			else
				Rotation = (360.0f - Rotation) + 180.f;
		}
		
		RenderXOffset -= transx;
		RenderYOffset -= transy;

		/*RenderXOffset += pivotX;
		RenderYOffset += pivotY;*/
		
		float RenderX = pScreenPos->x + RenderXOffset;// * Scale;
		float RenderY = pScreenPos->y + RenderYOffset;// * Scale;			

		if( IsMasked )
		{
			float leftClip = LeftMask - RenderX+xcamOffset;				leftClip = leftClip<0?0:leftClip;
			float topClip = TopMask - RenderY;					topClip = topClip<0?0:topClip;
			float rightClip = (RenderX+Cell->w) - RightMask+xcamOffset/2;	rightClip = rightClip<0?0:rightClip;
			float bottomClip = (RenderY+Cell->h) - BottomMask;	bottomClip = bottomClip<0?0:bottomClip;

			Image->blitAlphaRectFxF(Cell->tx+leftClip, //StartImgX
									Cell->ty+topClip,  //StartImgY
									Cell->tx+Cell->w-rightClip, //EndImgX 
									Cell->ty+Cell->h-bottomClip, //EndImgY
									RenderX+leftClip,
									RenderY+topClip,
									Rotation + Rotate, Scale, Cell->alpha*Alpha, Flipped ^ Cell->mirror,false,pivotX,pivotY);
		}
		else
			Image->blitAlphaRectFxF( Cell->tx, Cell->ty, Cell->tx + Cell->w, Cell->ty + Cell->h,
									RenderX, RenderY, 
									Rotation + Rotate, Scale, Cell->alpha*Alpha, Flipped ^ Cell->mirror,false,pivotX,pivotY);
	}
	Image->setBlitColor(1.0,1.0,1.0,1.0);
}

void CSprite::Hide()
{
	Visible = false;
}

void CSprite::Show()
{
	Visible = true;
}

//Forced play!!
void CSprite::ForcePlayAnimation(int anim)
{
	if( anim < 0 || anim >= NumAnimations() )
		return;

	CurrentAnim = anim;
	Animations[anim]->Play();
}

void CSprite::SetAnimation(int anim)
{
	if( anim == CurrentAnim )
		return;

	if( anim < 0 || anim >= NumAnimations() )
		return;

	CurrentAnim = anim;
}

void CSprite::PlayAnimation(int anim)
{
	if( anim == CurrentAnim )
		return;

	if( anim < 0 || anim >= NumAnimations() )
		return;

	CurrentAnim = anim;
	Animations[anim]->Play();
}

bool CSprite::IsAnimationFinished()
{
	return (Animations[CurrentAnim]->Active == false);
}

int CSprite::GetFrame()
{
	return Animations[CurrentAnim]->Frame;
}

void CSprite::SetFrame( int f )
{
	Animations[CurrentAnim]->SetFrame(f);
}

void CSprite::SetAnimSpeed(float speed)
{
    int AnimSize = (int) Animations.size();  //Sac: converted Implicit to explicit

	for(int i=0; i < AnimSize; ++i)
		Animations[i]->SetSpeed(speed);
}

bool CSprite::Collide(const CPoint* screenpos, const CPoint* mousepos, int anim, int Offset)
{
	CRectangle CellRect;

	CAnimation *Anim;
	
	if( anim == -1 )
		Anim = Animations[CurrentAnim];
	else
		Anim = Animations[0];

	CFrame *Frame = Anim->CurrentFrame();

	CCell *Cell;
	int CellSize = (int)Frame->Cells.size();
    for(int i=0; i < CellSize; ++i)
	{
		Cell = Frame->Cells[i];
	
		CellRect.topleft.x = screenpos->x + Cell->rx - Offset;
		CellRect.topleft.y = screenpos->y + Cell->ry - Offset;
		CellRect.w = Cell->w+ (2*Offset);
		CellRect.h = Cell->h+ (2*Offset);

		if( CellRect.Intersection( mousepos ) )
			return true;
	}

	return false;
}

CRectangle CSprite::LocalRect()
{
	CRectangle FrameRect;

	CAnimation *Anim = Animations[CurrentAnim];
	CFrame *Frame = Anim->CurrentFrame();

	int    minx = 0xffffffff;//0x0fffffff;
	int    miny = 0xffffffff;//0x0fffffff;
	int    maxx = 0;//0xf0000000;
	int    maxy = 0;//0xf0000000;

	CCell *Cell;
	int CellSize = (int)Frame->Cells.size();
	for(int i=0; i < CellSize; ++i)
	{
		Cell = Frame->Cells[i];
		// Sac: changed max to g_max and min to g_min as declared in constants.h
		minx = (int)g_min(minx, Cell->rx);
		miny = (int)g_min(miny, Cell->ry);
		maxx = (int)g_max(maxx, (Cell->rx + Cell->w));
		maxy = (int)g_max(maxy, (Cell->ry + Cell->h));
	}

	FrameRect.topleft.x = (float)minx;
	FrameRect.topleft.y = (float)miny;
	FrameRect.w = (float)(maxx - minx);
	FrameRect.h = (float)(maxy - miny);

	return FrameRect;
}



void CSprite::Load(std::string Name)
{	
	/////////////////////////////////////////////////////////////////////
	//  		    Converts unsigned char_array to stream             //
	/////////////////////////////////////////////////////////////////////	
	unsigned char *lpCompressedData = NULL;
	size_t nCompressedDataSize = 0;
	if(! KResource::loadResource (FileSystem::charPath(Name), lpCompressedData, nCompressedDataSize) )
	{
		FailFlag = true;

		std::string TmpStr = std::string("Er0x1501- Sprite failed to open2: " + FileSystem::strPath(Name));		
		#ifdef K_WIN32
			WARN(TmpStr.c_str());//WIDESTR WARNINGS	////Removed Wide string conversion for Windows	
		#else
			WARN( (const char*)(TmpStr.c_str()) );//WIDESTR WARNINGS	
		#endif
		return;
	}
	/* Copy contents and add a terminal 0 */
    char *_lpfileBytes = new char [nCompressedDataSize+1];
    memcpy (_lpfileBytes, lpCompressedData, nCompressedDataSize);//copy unsigned_chararray to const_chararray
    _lpfileBytes[nCompressedDataSize] = 0;
	std::stringstream stream(_lpfileBytes);//create stream from "const"_chararray 
    delete [] _lpfileBytes;
    _lpfileBytes = NULL;
	/////////////////////////////////////////////////////////////////////	
	
	
	
	if( stream.fail() )
	{
		FailFlag = true;
		std::string TmpStr = std::string("Er0x1502-Sprite failed to open: " + FileSystem::strPath(Name)).c_str();		
		#ifdef K_WIN32
			WARN((TmpStr).c_str());//WIDESTR WARNINGS	////Removed Wide string conversion for Windows	
		#else
			WARN( (const char*)(TmpStr.c_str()) );//WIDESTR WARNINGS	
		#endif
		return;
	}
	FailFlag = false;

	std::string TexFile;
	stream >> TexFile;//string

	std::string Path = CUtil::StripFileName(Name);
	Path += TexFile;

	Image = CTexFactory::Instance->GetImage( Path );
	Image->setTextureQuality(true);

	CAnimation *Anim;
	CFrame *Frame;
	CCell *Cell;

	int NumAnimations;
	stream >> NumAnimations;//int

	for(int i=0; i < NumAnimations; ++i)
	{
		Anim = new CAnimation();
	
		stream >> Anim->Name;//string
		stream >> Anim->NextAnimation ;//string
		stream >> Anim->Loop;//bool
		Anim->Loop = !Anim->Loop;

		int NumFrames;
		stream >> NumFrames;//int

		for(int j=0; j < NumFrames; ++j)
		{
			Frame = new CFrame();

			stream >> Frame->msDuration;//int

			int NumCells;
			stream >> NumCells;//int

			for(int k=0; k < NumCells; ++k)
			{
				Cell = new CCell();

				//float - mirror alone bool
				stream >> Cell->tx >> Cell->ty >> Cell->w >> Cell->h >> Cell->rx >> Cell->ry >> Cell->rot >> Cell->alpha >> Cell->mirror;
				Cell->rot = 360.0f - Cell->rot;

				if( Cell->mirror )
					Cell->rot = 360.0f - Cell->rot;

				int tempZ;
				stream >> tempZ; //int - discard unused Z value;

				Frame->Cells.push_back(Cell);
			}

			Anim->Frames.push_back( Frame );
		}

		Animations.push_back(Anim);
	}
}

void CSprite::Write( std::string Name )
{

}


