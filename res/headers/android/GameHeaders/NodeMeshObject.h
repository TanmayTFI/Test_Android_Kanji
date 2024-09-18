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

#ifndef NODEMESHOBJECT_H
#define NODEMESHOBJECT_H

#include "Object.h"
#include "NodeMeshEffect.h"
#include "Rect.h"

class NodeMeshObject : public CObject
{
	private:		
		float	  Alpha;
		float     Rotate;

		CObject* _refObj;
		int _dragNodeID;
		int _numNodes;
		std::vector<CObject*>	_nodeObjArray;
		std::vector<CPoint*>	_nodeRefPosArray;

	public:
		NodeMeshEffect *nodeMeshFX;
		KGraphic* Image;

		NodeMeshObject();
		NodeMeshObject(const std::string& strFile, int numRows, int numCols);
		NodeMeshObject(CObject *meshRefObject, int numRows, int numCols);
		~NodeMeshObject();

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