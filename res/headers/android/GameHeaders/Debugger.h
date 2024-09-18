//===========================================================
// Debugger UI for several debugging operations
// -Faisal
// Inspired by Shiver:Moonlit Grove Debug Scenario
// Framework
//===========================================================

#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "Framework.h"
#include "Point.h"
#include "Sprite.h"
#include "HOScene.h"
#include "Polygon.h"
#include "Bezier2D.h"
#include "RenderListEntity.h"
#include "UpdateListEntity.h"

class CDebugger : public CRenderListEntity, UpdateListEntity
{
	private:

	protected:
		KGraphic *DebugGraphic;
		CSprite *spr;
		CHOScene *scene;
		CPoint colRGB, colYellow, colBlue, colGreen;

		bool _debugEnabled;

		std::vector<CObject *>	Objects;
		std::vector<CRectangle *>	Rects;
		std::vector<CPolygon *>	PolyArray;
		std::vector<Bezier2D *>	BezierArray;
	public:		
		void SetSceneDebug(CHOScene* scn);
		void AddRectDebug( CRectangle rect);
		void AddPolyDebug( CPolygon* poly);
		void AddBezierDebug( Bezier2D* bezier);

		void ClearSceneDebug();
		void DrawLine( CPoint* p1, CPoint* p2);
		void DrawObjRect( CObject* obj);
		void DrawRect( CRectangle* wRect);
		void DrawPoly( CPolygon* poly);
		void DrawBezier( Bezier2D* bezier);

		virtual void Update(float ds);
		virtual void Render(const CPoint* pCam);

		void Enable() { _debugEnabled = true; };
		void Disable(){ _debugEnabled = false; };
		void Toggle() { _debugEnabled = !_debugEnabled; };

		CDebugger();
		virtual ~CDebugger();		
};

#endif