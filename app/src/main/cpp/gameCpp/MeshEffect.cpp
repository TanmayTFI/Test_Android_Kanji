#include "MeshEffect.h"
#include "Control.h"

using namespace Sexy;

//////////////////////////////////////////////////////////////////////
MeshEffect::MeshEffect( KGraphic *Image, int numRows, int numCols )
{
	_startCol = 0;
	_startRow = 0;
	_endRow = numRows;
	_endCol = numRows;
	_numRows = numRows;
	_numColums = numCols;
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

	_isWaterwaves = false;
	isTaperedDown = true;
}

void MeshEffect::setPosOffset(float DrawOffsetX, float DrawOffsetY)
{
	mDrawOffsetX	= DrawOffsetX;
	mDrawOffsetY	= DrawOffsetY;
}

void MeshEffect::SetSpeedAndDisplacement( float meshSpeedX, float meshSpeedY, float meshXDisp, float meshYDisp)
{
	_meshSpeedX = meshSpeedX;
	_meshSpeedY = meshSpeedY;

	_meshXDisp = meshXDisp;
	_meshYDisp = meshYDisp;
}

void MeshEffect::SetPosAndParams( float DrawOffsetX, float DrawOffsetY, float meshSpeedX, float meshSpeedY, float meshXDisp, float meshYDisp)
{
	mDrawOffsetX	= DrawOffsetX;
	mDrawOffsetY	= DrawOffsetY;

	_meshSpeedX = meshSpeedX;
	_meshSpeedY = meshSpeedY;

	_meshXDisp = meshXDisp;
	_meshYDisp = meshYDisp;
}

void MeshEffect::SetMeshBox(int startCol, int startRow, int endCol, int endRow)
{
	_startCol = startCol;
	_startRow = startRow;
	_endRow = endRow;
	_endCol = endCol;
}

void MeshEffect::Update( float t )
{
	//if(false)	//only inside the mesh
	//{
	//	for (int i = 1; i < _numRows-1; i++)
	//	{
	//		for (int j = 1; j < _numColums-1; j++)
	//		{
	//			float dispX = sinf(t*_meshSpeedX+(j*colPhaseFactor))*(_meshXDisp);
	//			float dispY = sinf(t*_meshSpeedY+(i*rowPhaseFactor))*(_meshYDisp);

	//			mDistMesh->SetDisplacement(j,i, dispX, dispY, DistortionMesh::DISP_NODE);
	//		}
	//	}
	//	return;
	//}

	if(_isWaterwaves)
	{
		for (int i = _startRow; i < _endRow; i++)
		{
			float angOffset = i*1.44f;
			for (int j = _startCol; j < _endCol; j++)
			{
				mDistMesh->SetDisplacement(j,i,cosf(t*_meshSpeedX+angOffset)*(_meshXDisp),0,DistortionMesh::DISP_NODE);
			}
		}
		return;
	}

	for (int i = _startRow; i < _endRow; i++)
	{
		for (int j = _startCol; j < _endCol; j++)
		{
			//_meshYDisp - i is given to reduce the displacement as we move from top to bottom, for swaying of plants for example
			if(isTaperedDown)
				mDistMesh->SetDisplacement(j,i,cosf(t*_meshSpeedX+(j))*(_meshXDisp-i),sinf(t*_meshSpeedY+(i))*(_meshYDisp-i),DistortionMesh::DISP_NODE);
			else
				mDistMesh->SetDisplacement(j,i,cosf(t*_meshSpeedX+(i))*((i/_numRows*_meshXDisp)-_meshXDisp),sinf(t*_meshSpeedY+(_numRows-i))*((j/_numColums*_meshYDisp)-_meshYDisp),DistortionMesh::DISP_NODE);

			

			/*float dispX = sinf(t*_meshSpeedX+(j*colPhaseFactor))*(isTaperedDown?(_numRows-i)/_numRows*_meshXDisp:_meshXDisp);
			float dispY = sinf(t*_meshSpeedY+(i*rowPhaseFactor))*(_meshYDisp);*/

			//mDistMesh->SetDisplacement(j,i, dispX, dispY, DistortionMesh::DISP_NODE);

			//mDistMesh->SetDisplacement(j,i,cosf(t*3+(j))*(12-i),sinf(t*2+(i))*(8-i),DistortionMesh::DISP_NODE);
			//mDistMesh->SetDisplacement(j, i, cosf(t*_meshSpeed+(j))*(_meshXDisp-i), sinf(t*_meshSpeed+(i))*(_meshYDisp-i), DistortionMesh::DISP_NODE);

			//Color blending not used right now. To enable color blending we've to use additive in distortion mesh render code.
			//int color=int((sinf(t)*0x00FFFF));
			////mDistMesh->SetColor(j, i, 0xFF<<24 | 0xFE<<24 | 0xFA<<24 | 0x88<<24);
			//Sexy::SexyColor aColor(0x0F+0xF0*abs(cos(t)),0x0F+0xF0*abs(sinf(t)),0xFF);
			//mDistMesh->SetColor(j, i, aColor);
		}
	}
}

void MeshEffect::SetOffset( float x, float y)
{
	mDrawOffsetX = x;
	mDrawOffsetY = y;
}

void MeshEffect::Draw(float offsetX, float offsetY)
{
	mDistMesh->Draw( mDrawOffsetX+offsetX, mDrawOffsetY+offsetY );
}
/////////////////////////////////////////////////////////////////////////////////////////