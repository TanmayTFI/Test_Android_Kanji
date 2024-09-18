//====================================
// Framework
// 2D Camera Class
//====================================

#ifndef FRMWRK_CAMERA2D_DEF
#define FRMWRK_CAMERA2D_DEF

#include "Framework.h"
#include "Point.h"

class Camera
{
	protected:
		CPoint Eye;		
		CPoint Bounds;

		virtual void UpdateOrigin();
		virtual void UpdateEye();

	public:
		CPoint ViewOrigin;
		Camera();
		virtual ~Camera();

		virtual void SetEye(float x, float y);
		virtual void SetEye(const CPoint* eye);

		virtual void SetOrigin(float x, float y);
		virtual void SetOrigin(const CPoint* origin);

		virtual void SetBounds(float x, float y);
		virtual void SetBounds(const CPoint* bounds);

		virtual CPoint GetEye() const;
		virtual CPoint GetBounds() const;

		virtual void BindEye();
		virtual void BindOrigin();
};

#endif