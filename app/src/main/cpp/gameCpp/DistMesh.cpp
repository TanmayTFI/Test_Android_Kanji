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


#include "DistMesh.h"
#include "string.h"

using namespace Sexy; 

// -----------------------------------
// Method:		constructor
// Description: set the (const) mesh dimensions and allocate
//				the displacements and vertices
// -----------------------------------
DistortionMesh::DistortionMesh(int Cols, int Rows)
{
	mROWS		= Rows;
	mCOLS		= Cols;
	mCellWidth	= mCellHeight = 0;
	mImageWidth = mImageHeight = 0;
	mImage		= NULL;
	mMeshColor	= SexyColor(255,255,255);
	mTransX = 0;
	mTransY = 0;
	
	// allocate and init displacement array
	mDisplacements = new Sexy::SexyTriVertex[mROWS * mCOLS];

	for (int Col=0; Col < mROWS * mCOLS; Col++)
	{
		mDisplacements[Col].x = 0.0f;
		mDisplacements[Col].y = 0.0f;
		mDisplacements[Col].u = 0.0f;
		mDisplacements[Col].v = 0.0f;
		
		mDisplacements[Col].color = Sexy::SexyColor(128, 128, 128).ToInt();
	}

	// allocate 2d-array with verts
	mVertices = (Sexy::SexyTriVertex (*)[3]) malloc(sizeof(Sexy::SexyTriVertex) * mROWS * mCOLS * 2 * 3);

	mBatch = KPTK::createKBatch();
	mBatch->allocateVertexBuffer(Cols*Rows, K_BATCH_DYNTL);
	//mBatch->allocateVertexBuffer(BATCH_BUFFER_SIZE, K_BATCH_DYNTL);
}

// -----------------------------------
// Method:		copy-constructor
// -----------------------------------
DistortionMesh::DistortionMesh(const DistortionMesh &aDistMesh)
{
	mROWS		= aDistMesh.mROWS;
	mCOLS		= aDistMesh.mCOLS;
	mCellWidth	= aDistMesh.mCellWidth;
	mCellHeight	= aDistMesh.mCellHeight;
	mImage		= aDistMesh.mImage;
	mImageWidth  = aDistMesh.mImageWidth;
	mImageHeight = aDistMesh.mImageHeight;
	mMeshColor	= aDistMesh.mMeshColor;
	mTransX = 0;
	mTransY = 0;
	
	// allocate and copy displacement array
	mDisplacements = new Sexy::SexyTriVertex[mROWS * mCOLS];
	memcpy(mDisplacements, aDistMesh.mDisplacements, sizeof(Sexy::SexyTriVertex) * mROWS * mCOLS);

	// allocate and copy 2d-array with verts
	mVertices = (Sexy::SexyTriVertex (*)[3]) malloc(sizeof(Sexy::SexyTriVertex) * mROWS * mCOLS * 2 * 3);
	memcpy(mVertices, aDistMesh.mVertices, sizeof(Sexy::SexyTriVertex) * mROWS * mCOLS * 2 * 3);

	
	mBatch = KPTK::createKBatch();
	mBatch->allocateVertexBuffer(mROWS*mCOLS, K_BATCH_DYNTL);
	//mBatch->allocateVertexBuffer(BATCH_BUFFER_SIZE, K_BATCH_DYNTL);
}

// -----------------------------------
// Method:		destructor
// -----------------------------------
DistortionMesh::~DistortionMesh()
{
	if (mBatch)
	{
		mBatch->freeBuffer();
		delete mBatch;
		mBatch = NULL;
	}
	free(mVertices); 
	//delete[] mVertices;
	delete[] mDisplacements;		
}

// -----------------------------------
// Method:		assignment
// -----------------------------------
DistortionMesh& DistortionMesh::operator= (const DistortionMesh &aDistMesh)
{
	if (this != &aDistMesh)
	{
		mROWS		= aDistMesh.mROWS;
		mCOLS		= aDistMesh.mCOLS;
		mCellWidth	= aDistMesh.mCellWidth;
		mCellHeight	= aDistMesh.mCellHeight;
		mImage		= aDistMesh.mImage;
		mImageWidth = aDistMesh.mImageWidth;
		mImageHeight = aDistMesh.mImageHeight;
		mMeshColor	= aDistMesh.mMeshColor;

		delete[] mDisplacements;
		mDisplacements = new Sexy::SexyTriVertex[mROWS * mCOLS];
		memcpy(mDisplacements, aDistMesh.mDisplacements, sizeof(Sexy::SexyTriVertex) * mROWS * mCOLS);

		delete[] mVertices;
		mVertices = (Sexy::SexyTriVertex (*)[3]) malloc(sizeof(Sexy::SexyTriVertex) * mROWS * mCOLS * 2 * 3);
		memcpy(mVertices, aDistMesh.mVertices, sizeof(Sexy::SexyTriVertex) * mROWS * mCOLS * 2 * 3);
	}

	return *this;
}

// -----------------------------------
// Method:		SetImage
// Description: set the image and init the displacements 
//				based on the image dimensions
// -----------------------------------
void DistortionMesh::SetImage(KGraphic* Image) 
{ 
	if (Image == NULL) return;

	mImage = Image; 

	// remember the image dimensions
	mImageWidth  = Image->getWidth()-1;
	mImageHeight = Image->getHeight()-1; 

	mCellWidth  = mImageWidth / (mCOLS - 1.0f);
	mCellHeight = mImageHeight/ (mROWS - 1.0f);

	// set the displacement array
	for (int Row = 0; Row < mROWS; Row++)
	{
		for (int Col = 0; Col < mCOLS; Col++)
		{
			mDisplacements[Row * mCOLS + Col].u = (Col * mCellWidth)  / mImageWidth;
			mDisplacements[Row * mCOLS + Col].v = (Row * mCellHeight) / mImageHeight;

			mDisplacements[Row * mCOLS + Col].x = Col * mCellWidth;
			mDisplacements[Row * mCOLS + Col].y = Row * mCellHeight;
		}
	}
}

// -----------------------------------
// Method:		ClearMesh
// Description: reset the displacements
// -----------------------------------
void DistortionMesh::ClearMesh(Sexy::SexyColor aColor)
{
	for(int Row = 0; Row < mROWS; Row++)
	{
		for(int Col = 0; Col < mCOLS; Col++)
		{
			mDisplacements[Row * mCOLS + Col].x		= Col * mCellWidth;
			mDisplacements[Row * mCOLS + Col].y		= Row * mCellHeight;
			mDisplacements[Row * mCOLS + Col].color = aColor.ToInt();
		}
	}
}

// -----------------------------------
// Method:		SetColor
// Description: set a color for a meshpoint
// -----------------------------------
void DistortionMesh::SetColor(int Col, int Row, Sexy::SexyColor aColor)
{
	if (Row < mROWS && Col < mCOLS) 
	{
		mDisplacements[Row * mCOLS + Col].color = aColor.ToInt();
	}
}

// -----------------------------------
// Method:		SetDisplacement
// Description: set the displacement for a meshpoint
// -----------------------------------
void DistortionMesh::SetDisplacement(int Col, int Row, float dX, float dY, int RefDisp)
{
	if (Row < mROWS && Col < mCOLS)
	{
		switch(RefDisp)
		{
			case DISP_NODE:		dX += Col * mCellWidth; 
								dY += Row * mCellHeight; 
								break;

			case DISP_CENTER:	dX += mCellWidth * (mCOLS-1)/2;
								dY += mCellHeight* (mROWS-1)/2; 
								break;

			case DISP_TOPLEFT:	break;
		}

		mDisplacements[Row * mCOLS + Col].x = dX;
		mDisplacements[Row * mCOLS + Col].y = dY;
	}
}



// -----------------------------------
// Method:		Draw
// -----------------------------------
void DistortionMesh::Draw(float TopLeftX, float TopLeftY)
{
	int DispIndex;
	int VertsIndex = 0;
	
	// init helper
	Sexy::SexyTriVertex TempVertices[4];

	// call SetImage first !
	if (mImage == NULL) return;

	for (int Row = 0; Row < mROWS-1; Row++)
	{
		for (int Col = 0; Col < mCOLS-1; Col++)
		{
			DispIndex = Row * mCOLS + Col;

			TempVertices[0].u		= mDisplacements[DispIndex].u;
			TempVertices[0].v		= mDisplacements[DispIndex].v;
			TempVertices[0].x		= TopLeftX + mDisplacements[DispIndex].x;
			TempVertices[0].y		= TopLeftY + mDisplacements[DispIndex].y;
			TempVertices[0].color	= mDisplacements[DispIndex].color;

			TempVertices[1].u		= mDisplacements[DispIndex + 1].u;
			TempVertices[1].v		= mDisplacements[DispIndex + 1].v;
			TempVertices[1].x		= TopLeftX + mDisplacements[DispIndex + 1].x;
			TempVertices[1].y		= TopLeftY + mDisplacements[DispIndex + 1].y;
			TempVertices[1].color	= mDisplacements[DispIndex + 1].color;

			TempVertices[2].u		= mDisplacements[DispIndex + mCOLS].u;
			TempVertices[2].v		= mDisplacements[DispIndex + mCOLS].v;
			TempVertices[2].x		= TopLeftX + mDisplacements[DispIndex + mCOLS].x;
			TempVertices[2].y		= TopLeftY + mDisplacements[DispIndex + mCOLS].y;
			TempVertices[2].color	= mDisplacements[DispIndex + mCOLS].color;

			TempVertices[3].u		= mDisplacements[DispIndex + mCOLS + 1].u;
			TempVertices[3].v		= mDisplacements[DispIndex + mCOLS + 1].v;
			TempVertices[3].x		= TopLeftX + mDisplacements[DispIndex + mCOLS + 1].x;
			TempVertices[3].y		= TopLeftY + mDisplacements[DispIndex + mCOLS + 1].y;
			TempVertices[3].color	= mDisplacements[DispIndex + mCOLS + 1].color;
			
			// create the verts in clockwise order
			mVertices[VertsIndex][0] = TempVertices[0];
			mVertices[VertsIndex][1] = TempVertices[1];
			mVertices[VertsIndex][2] = TempVertices[2];
			
			VertsIndex++;
			
			mVertices[VertsIndex][0] = TempVertices[2];
			mVertices[VertsIndex][1] = TempVertices[1];
			mVertices[VertsIndex][2] = TempVertices[3];
			
			VertsIndex++;
		}
	}

	// draw
	DrawTrianglesTex(mVertices, (mROWS-1) * (mCOLS-1) * 2,false,K_ALPHA_NORMAL); 
	// draw again in additive mode to enable color blending	
	//DrawTrianglesTex(mVertices, (mROWS-1) * (mCOLS-1) * 2,true,K_ALPHA_ADDITIVE);
}

// -----------------------------------
// Method:		GetColor
// Description: get the color of a meshpoint
// -----------------------------------
Sexy::SexyColor DistortionMesh::GetColor(int Col, int Row) const
{
	if (Row < mROWS && Col < mCOLS) 
	{
        return Sexy::SexyColor((int) mDisplacements[Row * mCOLS + Col].color); //Sac: Converting Implcit to explicit
	}
	else 
	{
		return Sexy::SexyColor(0, 0, 0);
	}
}

// -----------------------------------
// Method:		GetDisplacement
// Description: get the displacement of a meshpoint
// -----------------------------------
void DistortionMesh::GetDisplacement(int Col, int Row, float *dX, float *dY, int RefDisp)
{
	if (Row < mROWS && Col < mCOLS)
	{
		switch(RefDisp)
		{
			case DISP_NODE:		*dX = mDisplacements[Row * mCOLS + Col].x - Col * mCellWidth;
								*dY = mDisplacements[Row * mCOLS + Col].y - Row * mCellHeight;
								break;

			case DISP_CENTER:	*dX = mDisplacements[Row * mCOLS + Col].x - mCellWidth * (mCOLS-1)/2;
								*dY = mDisplacements[Row * mCOLS + Col].x - mCellHeight* (mROWS-1)/2;
								break;

			case DISP_TOPLEFT:	*dX = mDisplacements[Row * mCOLS + Col].x;
								*dY = mDisplacements[Row * mCOLS + Col].y;
								break;
		}
	}
}


void DistortionMesh::DrawTrianglesTex( const Sexy::SexyTriVertex theVertices[][3], int theNumTriangles, bool IsColorizeImages, int DrawMode)
{
	if( IsColorizeImages )
		mImage->setBlitColor((float)mMeshColor.mRed/255.0f, (float)mMeshColor.mGreen/255.0f, (float)mMeshColor.mBlue/255.0f, (float)mMeshColor.mAlpha/255.0f);
	else
		mImage->setBlitColor(1, 1, 1, 1);

	mImage->setAlphaMode(DrawMode);
	mImage->setTextureQuality(true);

	// Begin drawing to batch.
	mBatch->beginBatch(mImage);

	for (int i = 0; i < theNumTriangles; ++i)
	{
		Sexy::SexyTriVertex v1 = theVertices[i][0];
		Sexy::SexyTriVertex v2  = theVertices[i][1];
		Sexy::SexyTriVertex v3 = theVertices[i][2];

		float src1_x = v1.u * mImageWidth;
		float src1_y = v1.v * mImageHeight;
		float src2_x = v2.u * mImageWidth;
		float src2_y = v2.v * mImageHeight;
		float src3_x = v3.u * mImageWidth;
		float src3_y = v3.v * mImageHeight;

		SexyColor color_v1((int)v1.color, (int)v1.color>>24);  //Sac: Converting Implcit to explicit
		SexyColor color_v2((int)v2.color, (int)v2.color>>24);  //Sac: Converting Implcit to explicit
		SexyColor color_v3((int)v3.color, (int)v3.color>>24);  //Sac: Converting Implcit to explicit

		float z = 0.0f;
		float w = 1.0f;

		if(color_v1.GetAlpha() == 0 && color_v2.GetAlpha() == 0 && color_v3.GetAlpha() == 0) 
			continue;
		//mBatch->blitPerspectiveTriangle(src1_x, src1_y, src2_x, src2_y, src3_x, src3_y, v1.x, v1.y, z, w, v2.x, v2.y, z, w, v3.x, v3.y, z, w);
		if(mImage->isSpliced())
			mImage->blitArbitraryQuad(src1_x,src1_y, src2_x,src2_y,src3_x, src3_y, src3_x, src3_y,
											v1.x + mTransX,v1.y + mTransY,v2.x + mTransX,v2.y + mTransY,v3.x + mTransX,v3.y + mTransY,v3.x + mTransX,v3.y + mTransY);
		else
		mBatch->blitPerspectiveTriangleColor (src1_x, src1_y, src2_x, src2_y, src3_x, src3_y,
			v1.x + mTransX, v1.y + mTransY, z, w, (float)color_v1.mRed/255.0f, (float) color_v1.mGreen/255.0f, (float) color_v1.mBlue/255.0f,
			v2.x + mTransX, v2.y + mTransY, z, w, (float)color_v2.mRed/255.0f, (float) color_v2.mGreen/255.0f, (float) color_v2.mBlue/255.0f,
			v3.x + mTransX, v3.y + mTransY, z, w, (float)color_v3.mRed/255.0f, (float) color_v3.mGreen/255.0f, (float) color_v3.mBlue/255.0f);
	}

	// End drawing to batch.
	mBatch->endBatch();
}
