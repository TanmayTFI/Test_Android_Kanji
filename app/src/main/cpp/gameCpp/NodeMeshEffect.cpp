#include "NodeMeshEffect.h"
#include "Control.h"

using namespace Sexy;

//////////////////////////////////////////////////////////////////////
NodeMeshEffect::NodeMeshEffect( KGraphic *Image, int numRows, int numCols )
{
	_startCol = 0;
	_startRow = 0;
	_endRow = numRows;
	_endCol = numRows;
	_numRows = numRows;
	_numColums = numCols;

	_numNodes = numCols*numRows;

//	float imgGridWidth = Image->getWidth()/numCols; // Sac: Commenting Unused variable
//	float imgGridHeight = Image->getHeight()/numRows; // Sac: Commenting Unused variable

	for (int i = 0; i < numRows; i++)
	{
		for (int k = 0; k < numCols; k++)
		{
			Node node = {i,k, 0, 0};
			_nodeArray.push_back(node);
		}
	}

	// create and init the mesh
	mDistMesh = new DistortionMesh(numCols, numRows);

	// call SetImage to set the whole image as a mesh
	mDistMesh->SetImage(Image);

	_meshSpeedX = 0;
	_meshSpeedY = 0;
	
	mDrawOffsetX	= 0;
	mDrawOffsetY	= 0;

	_meshXDisp = 0;
	_meshYDisp = 0;

	colPhaseFactor = 1.0f;
	rowPhaseFactor = 1.0f;

	isTaperedDown = true;
}

void NodeMeshEffect::setNodeDisp(int id, float xval, float yval)
{
	if(id<_numNodes)
	{
		_nodeArray[id].x = xval;
		_nodeArray[id].y = yval;
	}
}

void NodeMeshEffect::setPosOffset(float DrawOffsetX, float DrawOffsetY)
{
	mDrawOffsetX	= DrawOffsetX;
	mDrawOffsetY	= DrawOffsetY;
}

void NodeMeshEffect::SetSpeedAndDisplacement( float meshSpeedX, float meshSpeedY, float meshXDisp, float meshYDisp)
{
	_meshSpeedX = meshSpeedX;
	_meshSpeedY = meshSpeedY;

	_meshXDisp = meshXDisp;
	_meshYDisp = meshYDisp;
}

void NodeMeshEffect::SetPosAndParams( float DrawOffsetX, float DrawOffsetY, float meshSpeedX, float meshSpeedY, float meshXDisp, float meshYDisp)
{
	mDrawOffsetX	= DrawOffsetX;
	mDrawOffsetY	= DrawOffsetY;

	_meshSpeedX = meshSpeedX;
	_meshSpeedY = meshSpeedY;

	_meshXDisp = meshXDisp;
	_meshYDisp = meshYDisp;
}

void NodeMeshEffect::SetMeshBox(int startCol, int startRow, int endCol, int endRow)
{
	_startCol = startCol;
	_startRow = startRow;
	_endRow = endRow;
	_endCol = endCol;
}

void NodeMeshEffect::Update( float t )
{
	for (int i = 0; i < _numNodes; i++)
	{
		mDistMesh->SetDisplacement(_nodeArray[i].col,_nodeArray[i].row,_nodeArray[i].x, _nodeArray[i].y, DistortionMesh::DISP_NODE);
	}
}

void NodeMeshEffect::SetOffset( float x, float y)
{
	mDrawOffsetX = x;
	mDrawOffsetY = y;
}

void NodeMeshEffect::Draw(float offsetX, float offsetY)
{
	mDistMesh->Draw( mDrawOffsetX+offsetX, mDrawOffsetY+offsetY );
}
/////////////////////////////////////////////////////////////////////////////////////////
