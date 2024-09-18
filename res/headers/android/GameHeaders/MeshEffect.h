//MeshEffect class
#ifndef __MESHEFFECT_H__ 
#define __MESHEFFECT_H__ 

#include "DistMesh.h" 
#define _USE_MATH_DEFINES
#include <math.h>

using namespace Sexy; 

class MeshEffect
{
private:
	// the number of rows and cols for the mesh
	int _startRow, _startCol, _endRow, _endCol;
	int _numRows, _numColums;	
	float mDrawOffsetX, mDrawOffsetY;
	float _meshXDisp, _meshYDisp;
	float _meshSpeedX, _meshSpeedY;
	float colPhaseFactor, rowPhaseFactor;

	bool _isWaterwaves;
public:

	bool isTaperedDown;

	DistortionMesh	*mDistMesh;

	MeshEffect(KGraphic *Image, int numRows, int numCols);
	~MeshEffect() { delete mDistMesh; }

	void setPosOffset(float DrawOffsetX, float DrawOffsetY);
	void SetSpeedAndDisplacement( float meshSpeedX, float meshSpeedY, float meshXDisp, float meshYDisp);
	void SetPosAndParams( float DrawOffsetX, float DrawOffsetY, float meshSpeedX, float meshSpeedY, float meshXDisp, float meshYDisp);
	void SetMeshBox(int startCol, int startRow, int endCol, int endRow);

	void setWaterwaves(bool f){_isWaterwaves = f;};
	void Update( float t );
	void Draw(float offsetX=0, float offsetY=0);
	void SetOffset( float x, float y);
};
#endif