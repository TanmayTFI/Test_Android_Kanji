//  -------------------------------------------------------------
//  Distortion Mesh class, Version 1.0, 11/24/2009
//  Heiko Fischer: heiko@fischeronline.de
//  -------------------------------------------------------------
//	
//	inspired and ported to Sexy from HGE´s hgeDistortionMesh helper class:
//  Haaf's Game Engine
//  The zlib/libpng License
//  
//  Copyright (c) 2003-2008 Relish Games
//  
//  This software is provided 'as-is', without any express or implied warranty. In no event will the authors 
//  be held liable for any damages arising from the use of this software. 
//  Permission is granted to anyone to use this software for any purpose, including commercial applications, 
//  and to alter it and redistribute it freely, subject to the following restrictions: 
//  
//		1. The origin of this software must not be misrepresented; you must not claim that you wrote the 
//		   original software. If you use this software in a product, an acknowledgment in the product 
//		   documentation would be appreciated but is not required. 
//		2. Altered source versions must be plainly marked as such, and must not be misrepresented 
//		   as being the original software. 
//		3. This notice may not be removed or altered from any source distribution. 
//
//  -------------------------------------------------------------

#ifndef __DISTORTIONMESH_H__ 
#define __DISTORTIONMESH_H__ 

#include "SexyTriVertex.h"
#include "SexyColor.h"
#include "KPTK.h"

namespace Sexy 
{ 
// forward class declarations

class DistortionMesh
{
public:
	static const int BATCH_BUFFER_SIZE = 500;
		
	enum
	{	// displacement reference 
		DISP_NODE,
		DISP_TOPLEFT,
		DISP_CENTER
	};

	KBatch				*mBatch;

	DistortionMesh();

	Sexy::SexyTriVertex		(*mVertices)[3];	// the verts to display
	Sexy::SexyTriVertex		*mDisplacements;	// the displacement array
	KGraphic			*mImage;			// the image to use
	
	int					mROWS, mCOLS;		// mesh size
	float				mImageWidth, 
						mImageHeight;		// just for returning the image dimensions
	float				mCellWidth,			// cell size
						mCellHeight;
	Sexy::SexyColor		mMeshColor;			// color to draw the mesh itself

	// cons / decons
	DistortionMesh(int Cols, int Rows);
	DistortionMesh(const DistortionMesh &aDistMesh);
	~DistortionMesh();

	DistortionMesh& operator= (const DistortionMesh &aDistMesh);

	// call SetImage to set the whole Image as a mesh 
	void			SetImage(KGraphic* Image);

	// called to the color and displacements in the mesh
	void			SetColor(int Col, int Row, Sexy::SexyColor aColor);
	void			SetDisplacement(int Col, int Row, float dX, float dY, int RefDisp);

	// the color to draw the mesh itself (pass bDrawMesh = true to the draw function)
	void			SetMeshColor(Sexy::SexyColor aColor) { mMeshColor = aColor; };

	// getter
	Sexy::SexyColor		GetColor(int Col, int Row) const;
	void			GetDisplacement(int Col, int Row, float *dX, float *dY, int RefDisp);
	
	// draw the mesh
	void			Draw(float TopLeftX, float TopLeftY);
	void			ClearMesh(Sexy::SexyColor aColor = Sexy::SexyColor(128, 128, 128));

	float mTransX, mTransY;
	void  DrawTrianglesTex( const Sexy::SexyTriVertex theVertices[][3], int theNumTriangles, bool IsColorizeImages, int DrawMode);
};


}	// namespace

#endif // __DISTORTIONMESH_H__  
