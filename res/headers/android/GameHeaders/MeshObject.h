//===========================================================
// 
// Framework
//===========================================================
//===========================================================
//
// Mesh effect applied Object
// Faisal
//
//===========================================================

#ifndef MESHOBJECT_H
#define MESHOBJECT_H

#include "Object.h"
#include "MeshEffect.h"
#include "Rect.h"

class MeshObject : public CObject
{
	private:		
		float	  Alpha;
		float     Rotate;
		float     SwayTime;

	public:
		MeshEffect *meshFX;
		KGraphic* Image;

		MeshObject();
		MeshObject(const std::string& strFile, int numRows, int numCols);
		MeshObject(CObject *meshRefObject, int numRows, int numCols);
		~MeshObject();

		KGraphic* GetImage(){ return Image; }

		void SetPosAndParams( float DrawOffsetX, float DrawOffsetY, float meshSpeedX, float meshSpeedY, float meshXDisp, float meshYDisp);
		
		//Inherited methods
		virtual void	Load(std::string Str);
		virtual void	Load(std::string Str, bool IsInteractiveObj){}
		virtual void	Update(float ds);
		virtual void	Render(const CPoint* pCam);
		virtual void	SetAlpha(float alpha);
		virtual void    SetRotate(float rot);
		void SetPosOffset(float x, float y);		


		CRectangle GetLocalRect();

		//Background methods
		void LoadImg(const std::string& strFile);
};

#endif