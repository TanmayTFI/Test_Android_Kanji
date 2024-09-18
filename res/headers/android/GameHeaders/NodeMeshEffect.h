//Nodes MeshEffect class
#ifndef __NODE_MESHEFFECT_H__ 
#define __NODE_MESHEFFECT_H__ 

#include "DistMesh.h" 
#define _USE_MATH_DEFINES
#include <math.h>

using namespace Sexy;

struct Node{
	int row,col;
	float x,y;
};

class NodeMeshEffect
{
private:
	// the number of rows and cols for the mesh
	int _startRow, _startCol, _endRow, _endCol;
	int _numRows, _numColums;	
	float mDrawOffsetX, mDrawOffsetY;
	float _meshXDisp, _meshYDisp;
	float _meshSpeedX, _meshSpeedY;
	float colPhaseFactor, rowPhaseFactor;

	int _numNodes;
	std::vector<Node>	_nodeArray;
public:

	void setNodeDisp(int id, float xval, float yval);

	bool isTaperedDown;

	DistortionMesh	*mDistMesh;

	NodeMeshEffect(KGraphic *Image, int numRows, int numCols);
	~NodeMeshEffect() { delete mDistMesh; }

	void setPosOffset(float DrawOffsetX, float DrawOffsetY);
	void SetSpeedAndDisplacement( float meshSpeedX, float meshSpeedY, float meshXDisp, float meshYDisp);
	void SetPosAndParams( float DrawOffsetX, float DrawOffsetY, float meshSpeedX, float meshSpeedY, float meshXDisp, float meshYDisp);
	void SetMeshBox(int startCol, int startRow, int endCol, int endRow);

	void Update( float t );
	void Draw(float offsetX=0, float offsetY=0);
	void SetOffset( float x, float y);
};
#endif