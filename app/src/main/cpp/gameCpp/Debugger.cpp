//===========================================================
// 
// Framework
//===========================================================

#include "Debugger.h"
#include "Debugger.h"

CDebugger::CDebugger()
{
	DebugGraphic = KPTK::createKGraphic();
	DebugGraphic->setTextureQuality(true);

	colRGB.SetXYZ( (255/255.0f),(100/255.0f),(100/255.0f) );
	colYellow.SetXYZ( (255/255.0f),(242/255.0f),(0/255.0f) );
	colBlue.SetXYZ( (69/255.0f),(222/255.0f),(230/255.0f) );
	colGreen.SetXYZ( (81/255.0f),(206/255.0f),(74/255.0f) );

	ZOrder = 10000;
	scene = NULL;
}


CDebugger::~CDebugger()
{
	delete DebugGraphic;
	scene = NULL;

	int size = (int)Rects.size();
	for(int i=0; i < size; ++i)
	{
		delete Rects[i];
		Rects[i] = NULL;
	}
	Rects.clear();

	size = (int)Objects.size();
	for(int i=0; i < size; ++i)
	{
		Objects[i] = NULL;
	}

	size = (int)PolyArray.size();
	for(int i=0; i < size; ++i)
	{
		PolyArray[i] = NULL;
	}

	size = (int)BezierArray.size();
	for(int i=0; i < size; ++i)
	{
		BezierArray[i] = NULL;
	}
}

void CDebugger::AddPolyDebug( CPolygon* poly)
{	
	PolyArray.push_back(poly);
}

void CDebugger::AddBezierDebug( Bezier2D* bezier)
{
	BezierArray.push_back(bezier);
}

void CDebugger::AddRectDebug( CRectangle rect)
{
	CRectangle *TRect = new CRectangle(&rect);
	Rects.push_back(TRect);
}

void CDebugger::SetSceneDebug( CHOScene* scn)
{
	this->scene = scn;
	scene->RetrieveElements<CObject>(OBJECT, &Objects);
}

void CDebugger::ClearSceneDebug()
{
	scene = NULL;
}

void CDebugger::DrawLine( CPoint* p1, CPoint* p2)
{
	//DebugGraphic->drawLine(p1->x,p1->y,p2->x,p2->y,1.0f,0.2f,0.2f,1.0f);
}

void CDebugger::Render( const CPoint* pCam  )
{
	if(!_debugEnabled)
		return;
	//DebugGraphic->drawLine(0,0,500,500,1.0f,0.2f,0.2f,1.0f);
	if(scene!=NULL)
	{
		//draw rects of objects in it
		int size = (int)Objects.size();
		for( int i=0; i < size; ++i)
		{
			DrawObjRect(Objects[i]);
		}
	}

	int size = (int)Rects.size();
	for( int i=0; i < size; ++i)
	{
		DrawRect(Rects[i]);
	}

	size = (int)PolyArray.size();
	for( int i=0; i < size; ++i)
	{
		DrawPoly(PolyArray[i]);
	}

	for(int i=0; i < size; ++i)
	{
		DrawBezier(BezierArray[i]);
	}
}

void CDebugger::DrawPoly( CPolygon* poly)
{
	int numPts = poly->numPts;

	for (int i = 0; i < numPts-1; i++)
	{
		DebugGraphic->drawLine(poly->getPoint(i).x, poly->getPoint(i).y, poly->getPoint(i+1).x, poly->getPoint(i+1).y, colBlue.x, colBlue.y, colBlue.z ,0.8f);
		if(i==(numPts-1))
			DebugGraphic->drawLine(poly->getPoint(i).x, poly->getPoint(i).y, poly->getPoint(0).x, poly->getPoint(0).y, colBlue.x, colBlue.y, colBlue.z ,0.8f);
	}
}

void CDebugger::DrawObjRect( CObject* obj)
{
	CRectangle wRect = obj->GetWorldRect();

	//Top right down left
	DebugGraphic->drawLine(wRect.Left(), wRect.Top(), wRect.Right(), wRect.Top(), colRGB.x, colRGB.y, colRGB.z ,0.8f);
	DebugGraphic->drawLine(wRect.Right(), wRect.Top(), wRect.Right(), wRect.Bottom(), colRGB.x, colRGB.y, colRGB.z,0.8f);
	DebugGraphic->drawLine(wRect.Right(), wRect.Bottom(), wRect.Left(), wRect.Bottom(), colRGB.x, colRGB.y, colRGB.z,0.8f);
	DebugGraphic->drawLine(wRect.Left(), wRect.Bottom(), wRect.Left(), wRect.Top(), colRGB.x, colRGB.y, colRGB.z,0.8f);
}

void CDebugger::DrawRect( CRectangle* wRect)
{
	//DebugGraphic->setBlitColor(0.25f,0.25f,0.95f,0.85f);
	//DebugGraphic->blitRect(wRect->Left(), wRect->Top(), wRect->Right(), wRect->Bottom(),0,0);
	DebugGraphic->drawLine(wRect->Left(), wRect->Top(), wRect->Right(), wRect->Top(),colYellow.x,colYellow.y,colYellow.z,0.85f);
	DebugGraphic->drawLine(wRect->Right(), wRect->Top(), wRect->Right(), wRect->Bottom(), colYellow.x,colYellow.y,colYellow.z,0.85f);
	DebugGraphic->drawLine(wRect->Right(), wRect->Bottom(), wRect->Left(), wRect->Bottom(), colYellow.x,colYellow.y,colYellow.z,0.85f);
	DebugGraphic->drawLine(wRect->Left(), wRect->Bottom(), wRect->Left(), wRect->Top(), colYellow.x,colYellow.y,colYellow.z,0.85f);
}

void CDebugger::DrawBezier( Bezier2D* bezier)
{	
	float prevx=bezier->GetPointAt(0).x,prevy=bezier->GetPointAt(0).y;
	for(float i=0; i < 1.0; i+=0.02f)
	{

		DebugGraphic->drawLine(prevx, prevy, bezier->GetPointAt(i).x, bezier->GetPointAt(i).y,colGreen.x,colGreen.y,colGreen.z,0.85f);
		prevx= bezier->GetPointAt(i).x;
		prevy = bezier->GetPointAt(i).y;
	}
	DebugGraphic->drawLine(prevx, prevy, bezier->GetPointAt(1.0f).x, bezier->GetPointAt(1.0f).y,colYellow.x,colYellow.y,colYellow.z,0.85f);
}


void CDebugger::Update(float ds)
{

}