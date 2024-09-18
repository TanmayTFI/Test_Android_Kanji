//====================================
// Framework
// 2D Camera Class
//====================================

#include "Camera.h"
#include "Constants.h"
#include "exAlgorithms.h"




Camera::Camera()
:	Eye(512.0f, 384.0f, 0.0f),
	Bounds(1366.0f, 768.0f, 0.0f),	
	ViewOrigin(0.0f, 0.0f, 0.0f)
	//Bounds(1024.0f, 768.0f, 0.0f)	
{
}

Camera::~Camera()
{
}

void Camera::SetEye(float x, float y)
{
	Eye.x = x;
	Eye.y = y;

	BindEye();

	UpdateOrigin();
}

void Camera::SetEye(const CPoint* eye)
{
	SetEye( eye->x, eye->y );
}

void Camera::SetOrigin(float x, float y)
{
	ViewOrigin.x = x;
	ViewOrigin.y = y;

	BindOrigin();

	UpdateEye();
}

void Camera::SetOrigin(const CPoint* origin)
{
	SetOrigin( origin->x, origin->y );
}

void Camera::SetBounds(float x, float y)
{
	Bounds.x = x;
	Bounds.y = y;
}

void Camera::SetBounds(const CPoint* bounds)
{
	SetBounds( bounds->x, bounds->y );
}

CPoint Camera::GetEye() const
{
	return Eye;
}



CPoint Camera::GetBounds() const
{
	return Bounds;
}

void Camera::BindEye()
{
	Eye.x = Elixir::Clamp( MAX_DISPLAY_WIDTH/2, Eye.x, Bounds.x - MAX_DISPLAY_WIDTH/2 );
	Eye.y = Elixir::Clamp( MAX_DISPLAY_HEIGHT/2, Eye.y, Bounds.y - MAX_DISPLAY_HEIGHT/2 );
}

void Camera::BindOrigin()
{
	ViewOrigin.x = Elixir::Clamp( 0.0f, ViewOrigin.x, Bounds.x - MAX_DISPLAY_WIDTH );
	ViewOrigin.y = Elixir::Clamp( 0.0f, ViewOrigin.y, Bounds.y - MAX_DISPLAY_HEIGHT );
}

void Camera::UpdateEye()
{
	Eye.x = ViewOrigin.x + MAX_DISPLAY_WIDTH/2;
	Eye.y = ViewOrigin.y + MAX_DISPLAY_HEIGHT/2;
}

void Camera::UpdateOrigin()
{
	ViewOrigin.x = Eye.x - MAX_DISPLAY_WIDTH/2;
	ViewOrigin.y = Eye.y - MAX_DISPLAY_HEIGHT/2;
}