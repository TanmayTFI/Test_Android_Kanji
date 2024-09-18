//===========================================================
// 
// Framework
//===========================================================

#include "MeshObject.h"
#include "TextureFactory.h"
#include "IOStream.h"
#include "Util.h"
#include "Camera.h"
#include "Rect.h"
#include "Application.h"
#include "GameFlow.h"

MeshObject::MeshObject()
:	Image(NULL)
{	
	Alpha = 1.0f;
	Type = BACKGROUND;
	pSprite = NULL;
	meshFX = NULL;
	Rotate = 0.0f;
    SwayTime = 0.0f;
	Visible = true;//CRenderListEntity
}

MeshObject::MeshObject(const std::string& strFile, int numRows, int numCols)
{
	Image = NULL;

	Alpha = 1.0f;
	Type = BACKGROUND;
	pSprite = NULL;
	meshFX = NULL;
	Rotate = 0.0f;
	SwayTime = 0.0f;

	Image = CTexFactory::Instance->GetImage(strFile);
	if( !GFApp->IsBestResolution )//down scaling needed for low resolutions
		Image->setTextureQuality(true);

	meshFX = new MeshEffect(Image, numRows, numCols);
    
	Visible = true;//CRenderListEntity
}

MeshObject::MeshObject(CObject *meshRefObject, int numRows, int numCols)
{
	Image = meshRefObject->pSprite->Image;

	Alpha = 1.0f;
	Type = BACKGROUND;
	pSprite = NULL;
	meshFX = NULL;
	Rotate = 0.0f;
	SwayTime = 0.0f;

	meshFX = new MeshEffect(Image, numRows, numCols);
    meshFX->setPosOffset(meshRefObject->Pos.x, meshRefObject->Pos.y);

	SetZ(meshRefObject->ZOrder);

	meshRefObject->SetEnable(false);

	Visible = true;//CRenderListEntity
}

void MeshObject::SetPosAndParams( float DrawOffsetX, float DrawOffsetY, float meshSpeedX, float meshSpeedY, float meshXDisp, float meshYDisp)
{
	meshFX->SetPosAndParams( DrawOffsetX, DrawOffsetY, meshSpeedX, meshSpeedY, meshXDisp, meshYDisp);
}

MeshObject::~MeshObject()
{
	CTexFactory::Instance->FreeImage(&Image);
	Image = NULL;
	if(meshFX!=NULL)
		delete meshFX;
}

void MeshObject::Load(std::string Str)
{
	LoadImg(Str);
}

void MeshObject::Update(float ds)
{
	if(Paused || !Visible)
		return;
	
	CObject::Update(ds);

	if(meshFX!=NULL)
	{
		SwayTime += 0.005f;
		meshFX->Update(SwayTime);
	}
}

void MeshObject::Render(const CPoint* pCam)
{	
	if( !GFInstance->IsRenderingToRT )
		if( !Visible || !IsActive)
			return;

	CPoint originPos;

	if( Cam )
	{
		originPos = Cam->ViewOrigin;
	}

	if(meshFX!=NULL)
	{
		meshFX->Draw(pCam->x, pCam->y);
	}
}

CRectangle MeshObject::GetLocalRect()
{
	CRectangle rect;

	if(Image != NULL)
	{
		rect.w =Image->getWidth();
		rect.h =Image->getHeight();
	}

	return rect;
}

void MeshObject::SetAlpha(float alpha)
{
	Alpha = alpha;
}

void MeshObject::SetRotate(float rot)
{
	Rotate = rot;
}

void MeshObject::SetPosOffset(float x, float y)
{
	meshFX->SetOffset(x, y);
}

void MeshObject::LoadImg(const std::string& strFile)
{
	Image = CTexFactory::Instance->GetImage(strFile);
	if( !GFApp->IsBestResolution )//down scaling needed for low resolutions
		Image->setTextureQuality(true);

	meshFX = new MeshEffect(Image,16,16);
}