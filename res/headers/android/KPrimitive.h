/**
 * Kanji game engine - geometric primitive definition
 *
 * (c) 2010
 */

#ifndef  _KANJI_KPRIMITIVE_H
#define  _KANJI_KPRIMITIVE_H

/* Include definitions */
#include "KMath2d.h"
#include "KMath3d.h"

/** Primitive type */
typedef enum {
   K_PRIMITIVE_UNDEFINED = 0, /**< Not defined yet */
   K_PRIMITIVE_RECT,          /**< Rectangle */
   K_PRIMITIVE_CIRCLE,        /**< Circle */
   K_PRIMITIVE_RING,          /**< Ring */
   K_PRIMITIVE_POLY,          /**< Polygon */
   K_PRIMITIVE_INFINITY,      /**< Infinite plane that always collides */
} KPrimitiveType;

/**
 * Class representing a geometry primitive (rectangle, circle, ring, polygon) that can be intersected with another
 */

class KPrimitive {
public:
   /** Constructor */
   inline KPrimitive () : _nType (K_PRIMITIVE_UNDEFINED) {
      _pol.v = NULL;
   }

   /** Destructor */
   inline ~KPrimitive () {
      freePrimitive ();
   }

   /** Free resources used by this primitive */
   inline void freePrimitive (void) {
      if (_pol.v) {
         delete [] _pol.v;
         _pol.v = NULL;
      }
      _nType = K_PRIMITIVE_UNDEFINED;
   }

   /**
    * Get current type of this primitive
    *
    * \return type (K_PRIMITIVE_xxx)
    */
   inline KPrimitiveType getType (void) const { return _nType; }

   /**
    * Set this primitive as a rectangle
    *
    * \param x1 X coordinate of top, left of rectangle
    * \param y1 Y coordinate of top, left of rectangle
    * \param x2 X coordinate + 1 of bottom, right of rectangle
    * \param y2 Y coordinate + 1 of bottom, right of rectangle
    */
   inline void setRect (float x1, float y1, float x2, float y2) {
      freePrimitive ();
      _nType = K_PRIMITIVE_RECT;
      _rect.v1 = KVector2 (x1, y1);
      _rect.v2 = KVector2 (x2, y2);
   }

   /**
    * Set this primitive as a circle
    *
    * \param xc X coordinate of center of the circle
    * \param yc Y coordinate of center of the circle
    * \param fRadius circle's radius
    */
   inline void setCircle (float xc, float yc, float fRadius) {
      freePrimitive ();
      _nType = K_PRIMITIVE_CIRCLE;
      _circle.vCenter = KVector2 (xc, yc);
      _circle.fRadius = fRadius;
   }

   /**
    * Set this primitive as a ring
    *
    * \param xc X coordinate of center of the ring
    * \param yc Y coordinate of center of the ring
    * \param fOuterRadius ring's outer radius
    * \param fInnerRadius ring's inner radius
    */
   inline void setRing (float xc, float yc, float fOuterRadius, float fInnerRadius) {
      freePrimitive ();
      _nType = K_PRIMITIVE_RING;
      _ring.vCenter = KVector2 (xc, yc);
      _ring.fOuterRadius = fOuterRadius;
      _ring.fInnerRadius = fInnerRadius;
   }

   /**
    * Set this primitive as a polygon
    *
    * \param nPoints number of points in bounding polygon
    * \param v points of the polygon, in this element's coordinate space
    */
   inline void setPoly (long nPoints, const KVector2 *v) {
      if (_nType == K_PRIMITIVE_POLY && _pol.nPoints == nPoints) {
         memcpy (_pol.v, v, nPoints * sizeof (KVector2));
      }
      else {
         freePrimitive ();
         _nType = K_PRIMITIVE_POLY;
         _pol.nPoints = nPoints;
         _pol.v = new KVector2 [nPoints];
         memcpy (_pol.v, v, nPoints * sizeof (KVector2));
      }
   }

   /**
    * Set this primitive as an infinite plane
    */
   inline void setInfinity (){
      freePrimitive ();
      _nType = K_PRIMITIVE_INFINITY;
   }

   /**
    * Transform this primitive by the specified matrix
    *
    * \param mat matrix to transform this primitive by
    * \param primTransformed resulting transformed primitive
    */
   void transform (KMatrix &mat, KPrimitive &primTransformed);

   /**
    * Get axis-aligned bounding box for this primitive
    *
    * \return bounding box
    */
   KRectangle getBoundingBox (void) const;

   /**
    * Check if this primitive intersects another
    *
    * \param primCheck primitive to be checked
    *
    * \return true if primitives intersect, false if not
    */
   bool intersects (const KPrimitive &primCheck) const;

   /**
    * Check if the primitive contains the specified point
    *
    * \param vCheck point to be checked
    *
    * \return true if the primitive contains the specified point, false if not
    */
   bool contains (const KVector2 &vCheck) const;

   /**
    * Draw this primitive
    *
    * \param r red component of drawing color
    * \param g green component of drawing color
    * \param b blue component of drawing color
    * \param a alpha component of drawing color
    */
   void blit (float r, float g, float b, float a);

private:
   /** Primitive type */
   KPrimitiveType _nType;

   /** Rectangle primitive */
   KRectangle _rect;

   /** Circle primitive */
   KCircle _circle;

   /** Ring primitive */
   KRing _ring;

   /** Polygon primitive */
   KPolygon _pol;
};

#endif   /* _KANJI_KPRIMITIVE_H */
