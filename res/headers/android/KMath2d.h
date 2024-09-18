/**
 * Kanji rendering and I/O engine - 2D math definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KMATH2D_H
#define  _KANJI_KMATH2D_H

/* Include definitions */
#include "KSysDefs.h"

/** Minimum of two values */
#define K_MIN(__a,__b) (((__a) < (__b)) ? (__a) : (__b))

/** Maximum of two values */
#define K_MAX(__a,__b) (((__a) > (__b)) ? (__a) : (__b))

/** Sign of a value */
#define K_SIGN(__v) (((__v) >= 0) ? 1 : -1)

/** Get the square of a number */
#define K_SQUARE(__n) ((__n)*(__n))

/** Swap two long values */
#define K_SWAPL(__a,__b) do { long t = (__a); (__a) = (__b); (__b) = t; } while (0)

/** Swap two floating point values */
#define K_SWAPF(__a,__b) do { float t = (__a); (__a) = (__b); (__b) = t; } while (0)

/** PI as a float */
#define K_PI_F 3.14159265358979323846f

/**
 * Class modeling a 2D vector
 */

class KVector2 {
public:
   /**
    * Constructor
    */
   inline KVector2 () {}

   /**
    * Constructor from initial coordinates
    *
    * \param x initial X coordinate
    * \param y initial Y coordinate
    */
   inline KVector2 (float x, float y) : x (x), y (y) {}

   /**
    * '+' operator. Add this vector and a second vector and return the result
    *
    * \param v2 vector to be added to this one (right hand vector)
    *
    * \return this vector + v2
    */
   inline KVector2 operator + (const KVector2 &v2) { return KVector2 (x + v2.x, y + v2.y); }

   /**
    * '+=' operator. Add a second vector to this one
    *
    * \param v2 vector to be added to this one (right hand vector)
    */
   inline void operator += (const KVector2 &v2) { x += v2.x; y += v2.y; }

   /**
    * '-' operator. Substract a second vector from this vector and return the result
    *
    * \param v2 vector to be substracted from this one (right hand vector)
    *
    * \return this vector - v2
    */
   inline KVector2 operator - (const KVector2 &v2) { return KVector2 (x - v2.x, y - v2.y); }

   /**
    * '-=' operator. Substract a second vector to this one
    *
    * \param v2 vector to be substracted from this one (right hand vector)
    */
   inline void operator -= (const KVector2 &v2) { x -= v2.x; y -= v2.y; }

   /**
    * '*' operator. Scale vector according to value and return result
    *
    * \param fMultiplier value to multiply all coordinates by
    *
    * \return this vector * fScale
    */
   inline KVector2 operator * (float fMultiplier) { return KVector2 (x * fMultiplier, y * fMultiplier); }

   /**
    * '*=' operator. Scale vector according to value
    *
    * \param fMultiplier value to multiply all coordinates by
    */
   inline void operator *= (float fMultiplier) { x *= fMultiplier; y *= fMultiplier; }

   /**
    * '/' operator. Divide vector according to value and return result
    *
    * \param fDivisor value to divide all coordinates by
    *
    * \return this vector * fScale
    */
   inline KVector2 operator / (float fDivisor) { return KVector2 (x / fDivisor, y / fDivisor); }

   /**
    * '/=' operator. Divide vector according to value
    *
    * \param fDivisor value to divide all coordinates by
    */
   inline void operator /= (float fDivisor) { x /= fDivisor; y /= fDivisor; }

   /**
    * Get length (norm) of this vector
    *
    * \return length
    */
   inline float length (void) const { return sqrtf (x * x + y * y); }

   /**
    * Get squared length (norm) of this vector
    *
    * \return squared length
    */
   inline float lengthSquared (void) const { return x * x + y * y; }

   /**
    * Get the angle of this vector, in radians
    *
    * \return angle
    */
   inline float angle (void) const {
      return atan2 (y, x);
   }

   /**
    * Calculate dot product of this vector and a specified vector
    *
    * \param v2 right hand vector in dot product
    *
    * \return dot product
    */
   inline float dotProduct (const KVector2 &v2) const { return x * v2.x + y * v2.y; }

   /**
    * Apply rotation of this point around a center point over the Z axis for a given angle
    *
    * \param vCenter center point
    * \param fZAngle rotation angle, in radians
    *
    * \return rotated point
    */
   inline KVector2 rotationZ (const KVector2 &vCenter, float fZAngle) {
      float fXRel = x - vCenter.x;
      float fYRel = y - vCenter.y;

      return KVector2 (vCenter.x + (fXRel * cosf (fZAngle) - fYRel * sinf (fZAngle)),
                       vCenter.y + (fYRel * cosf (fZAngle) + fXRel * sinf (fZAngle)));
   }

   /**
    * Scale this point
    *
    * \param vCenter center point
    * \param fScale scaling factor
    *
    * \return scaled point
    */
   inline KVector2 scaling (const KVector2 &vCenter, float fScale) {
      float fXRel = x - vCenter.x;
      float fYRel = y - vCenter.y;

      return KVector2 (vCenter.x + fXRel * fScale, vCenter.y + fYRel * fScale);
   }

   /**
    * Scale this point
    *
    * \param vCenter center point
    * \param fScaleX scaling factor over X
    * \param fScaleY scaling factor over Y
    *
    * \return scaled point
    */
   inline KVector2 scalingXY (const KVector2 &vCenter, float fScaleX, float fScaleY) {
      float fXRel = x - vCenter.x;
      float fYRel = y - vCenter.y;

      return KVector2 (vCenter.x + fXRel * fScaleX, vCenter.y + fYRel * fScaleY);
   }

   /**
    * Apply shearing to this point
    *
    * \param vCenter center point
    * \param fScaleX scaling factor over X
    * \param fScaleY scaling factor over Y
    *
    * \return point with shearing applied
    */

   inline KVector2 shearing (const KVector2 &vCenter, float fShearX, float fShearY) {
      float fXRel = x - vCenter.x;
      float fYRel = y - vCenter.y;

      return KVector2 (vCenter.x + fXRel - fShearX * fYRel, vCenter.y + fYRel - fShearY * fXRel);
   }

   /** X coordinate */
   float x;

   /** Y coordinate */
   float y;
};

/**
 * Class modeling a 2D line segment
 */

class KSegment {
public:
   /**
    * Constructor
    */
   inline KSegment () {}

   /**
    * Constructor from endpoints
    *
    * \param v1 first point
    * \param v2 second point
    */
   inline KSegment (const KVector2 &v1, const KVector2 &v2) : v1 (v1), v2 (v2) {}

   /**
    * Constructor from endpoints
    *
    * \param x1 X coordinate of first point
    * \param y1 Y coordinate of first point
    * \param x2 X coordinate of second point
    * \param y2 Y coordinate of second point
    */
   inline KSegment (float x1, float y1, float x2, float y2) : v1 (x1, y1), v2 (x2, y2) {}

   /**
    * Check if this line segment intersects with another line segment
    *
    * \param lnCheck line segment to be checked
    * \param vIntersection returned intersection point, if any
    *
    * \return true if line segments intersect, false if they don't
    */
   bool intersects (const KSegment &lnSegCheck, KVector2 &vIntersection) const;

   /** First point */
   KVector2 v1;

   /** Second point */
   KVector2 v2;
};

/**
 * Class modeling a 2D rectangle
 */

class KRectangle {
public:
   /**
    * Constructor
    */
   inline KRectangle () {}

   /**
    * Constructor
    *
    * \param v1 coordinates of top, left bound
    * \param v2 coordinates of bottom, right bound + 1
    */
   inline KRectangle (const KVector2 &v1, const KVector2 &v2) : v1 (v1), v2 (v2) {}

   /**
    * Constructor
    *
    * \param x1 X coordinate of top, left bound
    * \param y1 Y coordinate of top, left bound
    * \param x2 X coordinate of bottom, right bound + 1
    * \param y2 Y coordinate of bottom, right bound + 1
    */
   inline KRectangle (float x1, float y1, float x2, float y2) : v1 (KVector2 (x1, y1)), v2 (KVector2 (x2, y2)) {}

   /**
    * Get this rectangle's width
    *
    * \return width
    */
   inline float width (void) const { return v2.x - v1.x; }

   /**
    * Get this rectangle's height
    *
    * \return height
    */
   inline float height (void) const { return v2.y - v1.y; }

   /**
    * Get this rectangle's center point
    *
    * \return center point
    */
   inline KVector2 center (void) { return (v2 - v1) / 2 + v1; }

   /**
    * Check if this rectangle contains the specified point
    *
    * \param x X coordinate of point to be checked
    * \param y Y coordinate of point to be checked
    *
    * \return true if point is inside this rectangle, false if it is outside
    */
   inline bool contains (float x, float y) const { return x >= v1.x && x < v2.x && y >= v1.y && y < v2.y; }

   /**
    * Check if this rectangle contains the specified point
    *
    * \param vCheck point to be checked
    *
    * \return true if point is inside this rectangle, false if it is outside
    */
   inline bool contains (const KVector2 &vCheck) const { return vCheck.x >= v1.x && vCheck.x < v2.x && vCheck.y >= v1.y && vCheck.y < v2.y; }

   /**
    * Check if this rectangle intersects with another rectangle
    *
    * \param rcCheck rectangle to be checked
    *
    * \return true if rectangles intersect, false if not
    */
   inline bool intersects (const KRectangle &rcCheck) const {
      return ((rcCheck.v1.x >= v2.x || rcCheck.v1.y >= v2.y || v1.x >= rcCheck.v2.x || v1.y >= rcCheck.v2.y) == false);
   }

   /**
    * Check if this rectangle contains the specified rectangle
    *
    * \param rcCheck rectangle to be checked
    *
    * \return true if this rectangle contains the checked rectangle, false if not
    */
   inline bool contains (const KRectangle &rcCheck) const {
      return (rcCheck.v1.x >= v1.x && rcCheck.v1.y >= v1.y && rcCheck.v2.x <= v2.x && rcCheck.v2.y <= v2.y);
   }

   /** Top, left bound */
   KVector2 v1;

   /** Bottom, right bound */
   KVector2 v2;
};

/**
 * Class modeling a 2D circle
 */

class KCircle {
public:
   /**
    * Constructor
    */
   inline KCircle () {}

   /**
    * Constructor
    *
    * \param vCenter center point
    * \param fRadius radius
    */
   inline KCircle (const KVector2 &vCenter, float fRadius) : vCenter (vCenter), fRadius (fRadius) {}

   /**
    * Constructor
    *
    * \param xc X coordinate of the center
    * \param yc Y coordinate of the center
    * \param fRadius radius
    */
   inline KCircle (float xc, float yc, float fRadius) : vCenter (KVector2 (xc, yc)), fRadius (fRadius) {}

   /**
    * Check if this circle contains the specified point
    *
    * \param v point to be checked
    *
    * \return true if point is inside this circle, false if it is outside
    */
   inline bool contains (const KVector2 &vCheck)const { float dx = vCheck.x - vCenter.x; float dy = vCheck.y - vCenter.y; return (K_SQUARE (dx) / K_SQUARE (fRadius) + K_SQUARE (dy) / K_SQUARE (fRadius)) <= 1; }

   /**
    * Check if this circle intersects with another circle
    *
    * \param crCheck circle to be checked
    *
    * \return true if circles intersect, false if not
    */
   inline bool intersects (const KCircle &crCheck) const { float dx = crCheck.vCenter.x - vCenter.x; float dy = crCheck.vCenter.y - vCenter.y; return (K_SQUARE (dx) + K_SQUARE (dy)) < K_SQUARE (crCheck.fRadius + fRadius); }

   /**
    * Check if this circle contains the specified circle
    *
    * \param crCheck circle to be checked
    *
    * \return true if this circle contains the checked circle, false if not
    */
   inline bool contains (const KCircle &crCheck) const { float dx = crCheck.vCenter.x - vCenter.x; float dy = crCheck.vCenter.y - vCenter.y; return (sqrtf (K_SQUARE (dx) + K_SQUARE (dy)) + crCheck.fRadius) <= fRadius;}

   /**
    * Check if this circle intersects with a rectangle
    *
    * \param rcCheck rectangle to be checked
    *
    * \return true if circle and rectangle intersect, false if they don't
    */
   bool intersects (const KRectangle &rcCheck) const;

   /**
    * Check if this circle intersects with a line segment
    *
    * \param lnSegCheck line segment to be checked
    *
    * \return true if circle and line segment intersect, false if they don't
    */
   bool intersects (const KSegment &lnSegCheck) const;

   /** Center */
   KVector2 vCenter;

   /** Radius */
   float fRadius;
};

/**
 * Class modeling a 2D ring
 */

class KRing {
public:
   /**
    * Constructor
    */
   inline KRing () {}

   /**
    * Constructor
    *
    * \param vCenter center point
    * \param fOuterRadius outer radius
    * \param fInnerRadius inner radius
    */
   inline KRing (const KVector2 &vCenter, float fOuterRadius, float fInnerRadius) : vCenter (vCenter), fOuterRadius (fOuterRadius), fInnerRadius (fInnerRadius) {}

   /**
    * Constructor
    *
    * \param xc X coordinate of the center
    * \param yc Y coordinate of the center
    * \param fRadius radius
    */
   inline KRing (float xc, float yc, float fOuterRadius, float fInnerRadius) : vCenter (KVector2 (xc, yc)), fOuterRadius (fOuterRadius), fInnerRadius (fInnerRadius) {}

   /**
    * Check if this ring contains the specified point
    *
    * \param v point to be checked
    *
    * \return true if point is inside this ring, false if it is outside
    */
   inline bool contains (const KVector2 &vCheck) const {
      float dx = vCheck.x - vCenter.x;
      float dy = vCheck.y - vCenter.y;
      float fDistSq = K_SQUARE (dx) + K_SQUARE (dy);
      
      return (fDistSq <= K_SQUARE (fOuterRadius) && fDistSq >= K_SQUARE (fInnerRadius));
   }

   /**
    * Check if this ring intersects with another ring
    *
    * \param rgCheck ring to be checked
    *
    * \return true if rings intersect, false if not
    */
   inline bool intersects (const KRing &rgCheck) const {
       float dx = rgCheck.vCenter.x - vCenter.x;
       float dy = rgCheck.vCenter.y - vCenter.y;
       float fDist = sqrtf (K_SQUARE (dx) + K_SQUARE (dy));
       
       /* First verify that the checked ring isn't contained in the inner circle of this ring; if not, and the outer circles intersect, the rings intersect */
       return ((fDist + rgCheck.fOuterRadius) > fInnerRadius) && (fDist < (rgCheck.fOuterRadius + fOuterRadius));
   }

   /**
    * Check if this ring intersects with a circle
    *
    * \param rgCheck ring to be checked
    *
    * \return true if rings intersect, false if not
    */
   inline bool intersects (const KCircle &crCheck) const {
       float dx = crCheck.vCenter.x - vCenter.x;
       float dy = crCheck.vCenter.y - vCenter.y;
       float fDist = sqrtf (K_SQUARE (dx) + K_SQUARE (dy));
       
       /* First verify that the checked circle isn't contained in the inner circle of this ring; if not, and the outer circle intersects the checked circle, the ring and circle intersect */
       return ((fDist + crCheck.fRadius) > fInnerRadius) && (fDist < (crCheck.fRadius + fOuterRadius));
   }

   /**
    * Check if this ring intersects with a rectangle
    *
    * \param rcCheck rectangle to be checked
    *
    * \return true if ring and rectangle intersect, false if they don't
    */
   bool intersects (const KRectangle &rcCheck) const;

   /**
    * Check if this ring intersects with a line segment
    *
    * \param lnSegCheck line segment to be checked
    *
    * \return true if ring and line segment intersect, false if they don't
    */
   bool intersects (const KSegment &lnSegCheck) const;

   /** Center */
   KVector2 vCenter;

   /** Outer radius */
   float fOuterRadius;

   /** Inner radius */
   float fInnerRadius;
};

/**
 * Class modeling a 2D polygon
 */

class KPolygon {
public:
   /** Constructor */
   inline KPolygon() {}

   /**
    * Check if this polygon contains the specified point
    *
    * \param vCheck point to be checked
    *
    * \return true if point is inside this polygon, false if it is outside
    */
   bool contains (const KVector2 &vCheck) const;

   /**
    * Check if this polygon contains the specified polygon
    *
    * \param polCheck polygon to be checked
    *
    * \return true if it does, false if it doesn't
    */
   bool contains (const KPolygon &polCheck) const;

   /**
    * Check if this polygon intersects with another polygon
    *
    * \param polCheck polygon to be checked
    *
    * \return true if polygons intersect, false if they don't
    */
   bool intersects (const KPolygon &polCheck) const;

   /**
    * Check if this polygon intersects with a rectangle
    *
    * \param rcCheck rectangle to be checked
    *
    * \return true if polygon and rectangle intersect, false if they don't
    */
   bool intersects (const KRectangle &rcCheck) const;

   /**
    * Check if this polygon intersects with a circle
    *
    * \param crCheck circle to be checked
    *
    * \return true if polygon and circle intersect, false if they don't
    */
   bool intersects (const KCircle &crCheck) const;

   /**
    * Check if this polygon intersects with a circle
    *
    * \param rgCheck ring to be checked
    *
    * \return true if polygon and ring intersect, false if they don't
    */
   bool intersects (const KRing &rgCheck) const;

   /**
    * Compute area of this polygon
    *
    * \return area
    */
   float area (void) const;

   /**
    * Triangulate this polygon
    *
    * \param vTriangle array of points to be populated, in sets of 3 points per triangle
    *
    * \return number of triangles generated if triangulation succeeded, 0 if triangulation failed
    */
   long triangulate (KVector2 *vTriangle);

   /** Number of points */
   long nPoints;

   /** Points of this polygon */
   KVector2 *v;

private:
   /**
    * While triangulating, cut a point from this polygon and add a triangle to describe the location of the cut
    * 
    * \param u the index of the first point
    * \param v the index of the second point
    * \param w the index of the third point
    * \param n number of points in polygon
    * \param V the array to populate with indices of triangles
    *
    * \return true if a triangle was found, false if not
    */
   bool snip (long u, long v, long w, long n, long *V);
};

#endif   /* _KANJI_KMATH2D_H */
