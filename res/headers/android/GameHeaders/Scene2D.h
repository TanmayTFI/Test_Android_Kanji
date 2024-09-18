//===========================================================
// 
// Framework
//===========================================================

#ifndef SCENE_2D_H
#define SCENE_2D_H

#include "Object.h"

struct SceneData
{
	std::string objType;	
	std::string uid;
	std::string spriteString;
	int	spriteAnim, x, y, ZOrder;
	std::string texName, HOName;
	int ScaleFactor, InteractiveObj;
};


class CScene2D
{
	public:
		int SWidth, SHeight;
		void RestoreXY();
		void SaveXY();
		void ShiftXY( float x, float y);

		CScene2D();
		virtual ~CScene2D();

		virtual void SetCam(const Camera* cam);

		virtual void SetSceneAlpha(float alpha);

		virtual void Show(bool f);
		virtual void RenderToTarget(float offX=0, float offY=0);		

		virtual void SetMaskRect(CRectangle maskrect);

		virtual void Offset(const CPoint& off);

		void Load(const std::string& Name);
		virtual CObject* CreateObject(SceneData data);

		template<class T>
		void RetrieveElements(const std::string& objType, std::vector<T*> *pVecObjs);

		CObject* GetObjectByID(const std::string& id);
		CObject* pCollideWithObject(const CPoint* pMouse,  bool AllowBGs =true);
		void DeleateAllObjects();

	protected:
		std::vector<CObject *>	Objects;

	private:
		CObject* pCreateObject(const std::string& objType);
};

template<class T>
void CScene2D::RetrieveElements(const std::string& objType, std::vector<T*> *pVecObjs)
{
	int size = (int)Objects.size();
	for(int i=0; i < size; ++i)
	{
		if( Objects[i]->Type == objType )
		{
			pVecObjs->push_back(reinterpret_cast<T*>(Objects[i]));
		}
	}
}

#endif