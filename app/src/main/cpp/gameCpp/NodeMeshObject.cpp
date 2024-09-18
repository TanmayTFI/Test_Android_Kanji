//===========================================================
// 
// Framework
//===========================================================

#include "NodeMeshObject.h"
#include "TextureFactory.h"
#include "IOStream.h"
#include "Util.h"
#include "Camera.h"
#include "Rect.h"
#include "Application.h"
#include "GameFlow.h"

NodeMeshObject::NodeMeshObject()
:	Image(NULL)
{	
	Alpha = 1.0f;
	Type = BACKGROUND;
	pSprite = NULL;
	nodeMeshFX = NULL;
	Rotate = 0.0f;
    
	Visible = true;//CRenderListEntity
}

NodeMeshObject::NodeMeshObject(const std::string& strFile, int numRows, int numCols)
{
	Image = NULL;

	Alpha = 1.0f;
	Type = BACKGROUND;
	pSprite = NULL;
	nodeMeshFX = NULL;
	Rotate = 0.0f;

	Image = CTexFactory::Instance->GetImage(strFile);
	if( !GFApp->IsBestResolution )//down scaling needed for low resolutions
		Image->setTextureQuality(true);

	nodeMeshFX = new NodeMeshEffect(Image, numRows, numCols);
    
	Visible = true;//CRenderListEntity
}

NodeMeshObject::NodeMeshObject(CObject *meshRefObject, int numRows, int numCols)
{
	Image = meshRefObject->pSprite->Image;

	Alpha = 1.0f;
	Type = BACKGROUND;
	pSprite = NULL;
	nodeMeshFX = NULL;
	Rotate = 0.0f;

	_refObj = meshRefObject;

	nodeMeshFX = new NodeMeshEffect(Image, numRows, numCols);
    nodeMeshFX->setPosOffset(meshRefObject->Pos.x, meshRefObject->Pos.y);

	_numNodes = numCols*numRows;
	_dragNodeID = -1;

	float imgGridWidth = Image->getWidth()/(numCols-1);
	float imgGridHeight = Image->getHeight()/(numRows-1);

	for (int i = 0; i < numRows; i++)
	{
		for (int k = 0; k < numCols; k++)
		{
			CObject *nodeObj = new CObject();
			nodeObj->LoadInit("NONGAME\\NODE.SPR", meshRefObject->Pos.x + k*imgGridWidth, meshRefObject->Pos.y + i*imgGridHeight, meshRefObject->ZOrder+10);
			_nodeObjArray.push_back(nodeObj);

			CPoint *refPoint = new CPoint(meshRefObject->Pos.x + k*imgGridWidth, meshRefObject->Pos.y + i*imgGridHeight, 0);
			_nodeRefPosArray.push_back(refPoint);
		}
	}

	SetZ(meshRefObject->ZOrder);

	meshRefObject->SetEnable(false);

	Visible = true;//CRenderListEntity
}

void NodeMeshObject::SetPosAndParams( float DrawOffsetX, float DrawOffsetY, float meshSpeedX, float meshSpeedY, float meshXDisp, float meshYDisp)
{
	nodeMeshFX->SetPosAndParams( DrawOffsetX, DrawOffsetY, meshSpeedX, meshSpeedY, meshXDisp, meshYDisp);
}

NodeMeshObject::~NodeMeshObject()
{
	CTexFactory::Instance->FreeImage(&Image);
	Image = NULL;
	if(nodeMeshFX!=NULL)
		delete nodeMeshFX;

    int size = (int)_nodeObjArray.size();  //Sac: converted Implicit to explicit

	for (int i = 0; i < size; i++)
	{
		SAFE_DELETE(_nodeObjArray[i]);
	}
	_nodeObjArray.clear();

    size = (int)_nodeRefPosArray.size();  //Sac: converted Implicit to explicit

	for (int i = 0; i < size; i++)
	{
		SAFE_DELETE(_nodeRefPosArray[i]);
	}
	_nodeRefPosArray.clear();
	
}

void NodeMeshObject::Load(std::string Str)
{
	LoadImg(Str);
}

void NodeMeshObject::Update(float ds)
{
	if(Paused || !Visible)
		return;

	if(_dragNodeID!=-1)
	{
        CPoint mpos = Control::Input->Mpos();
		_nodeObjArray[_dragNodeID]->SetPos(&mpos);

		float xval = _nodeObjArray[_dragNodeID]->Pos.x - _nodeRefPosArray[_dragNodeID]->x;
		float yval = _nodeObjArray[_dragNodeID]->Pos.y - _nodeRefPosArray[_dragNodeID]->y;

		nodeMeshFX->setNodeDisp(_dragNodeID, xval, yval);
		
		if(!Control::Input->LBdown())
		{
			_dragNodeID = -1;
			return;
		}
	}
	else	
	{
         CPoint mpos = Control::Input->Mpos();
		for (int i = 0; i < _numNodes; i++)
		{
			if(_nodeObjArray[i]->Collide(&mpos))
			{
				Cursor::SetMode(CA_HandPointy);
				_nodeObjArray[i]->PlayAnimation(1);
				if(Control::Input->LBdown())
				{	
					_dragNodeID = i;
					break;
				}
			}
			else
			{
				_nodeObjArray[i]->PlayAnimation(0);
			}
		} 
	}

	static float angle=0;
	angle += Control::LogicRate*100.0f;

	for (int i = 0; i < _numNodes; i++)
	{
		nodeMeshFX->setNodeDisp(i, i*5.0f*Elixir::Sin(angle), 0);
	} 
	
	CObject::Update(ds);

	if(nodeMeshFX!=NULL)
	{
		static float SwayTime = 0.0f;
		SwayTime += 0.005f;
		nodeMeshFX->Update(SwayTime);
	}
}

void NodeMeshObject::Render(const CPoint* pCam)
{	
	if( !GFInstance->IsRenderingToRT )
		if( !Visible || !IsActive)
			return;

	CPoint originPos;

	if( Cam )
	{
		originPos = Cam->ViewOrigin;
	}

	if(nodeMeshFX!=NULL)
	{
		nodeMeshFX->Draw(pCam->x, pCam->y);
	}

	//Draw Outline - With Scaling
	float leftX = _refObj->Pos.x;
	float topY = _refObj->Pos.y;
	float rightX = _refObj->Pos.x + _refObj->pSprite->Image->getWidth();
	float bottomY = _refObj->Pos.y + _refObj->pSprite->Image->getHeight();

	Image->drawLine(leftX, topY, rightX, topY, 1.0f,0.2f,0.2f,1.0f);
	Image->drawLine(rightX, topY, rightX, bottomY, 1.0f,0.2f,0.2f,1.0f);

	Image->drawLine(rightX, bottomY, leftX, bottomY, 1.0f,0.2f,0.2f,1.0f);
	Image->drawLine(leftX, topY, leftX, bottomY, 1.0f,0.2f,0.2f,1.0f);
}

CRectangle NodeMeshObject::GetLocalRect()
{
	CRectangle rect;

	if(Image != NULL)
	{
		rect.w =Image->getWidth();
		rect.h =Image->getHeight();
	}

	return rect;
}

void NodeMeshObject::SetAlpha(float alpha)
{
	Alpha = alpha;
}

void NodeMeshObject::SetRotate(float rot)
{
	Rotate = rot;
}

void NodeMeshObject::SetPosOffset(float x, float y)
{
	nodeMeshFX->SetOffset(x, y);
}

void NodeMeshObject::LoadImg(const std::string& strFile)
{
	Image = CTexFactory::Instance->GetImage(strFile);
	if( !GFApp->IsBestResolution )//down scaling needed for low resolutions
		Image->setTextureQuality(true);

	nodeMeshFX = new NodeMeshEffect(Image,16,16);
}
