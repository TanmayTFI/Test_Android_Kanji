/**
 * Kanji game engine - UI element bounding hierarchy definitions
 *
 * (c) 2010
 *
 * \file KUIBounds.h
 */

#ifndef  _KANJI_UIBOUNDS_H
#define  _KANJI_UIBOUNDS_H

/* Include definitions */
#include "KList.h"
#include "KPrimitive.h"

/* Forward declarations */
class KUIElement;

/**
 * Bounding primitive of a KUIElement
 */

class KUIBounds : public KObjectListable {
   friend class KUIElement;

public:
   /**
    * Constructor
    *
    * \param lpParent parent bounding primitive, NULL for none
    */
   KUIBounds (KUIBounds *lpParent);

   /** Destructor */
   ~KUIBounds ();

   /**
    * Get current type of this bounding primitive
    *
    * \return type (K_PRIMITIVE_xxx)
    */
   KPrimitiveType getType (void) const;

   /**
    * Set drawing color. The color is multiplied with the one passed to KUIElement::enableDrawingBounds()
    *
    * \param r red component of drawing color
    * \param g green component of drawing color
    * \param b blue component of drawing color
    * \param a alpha component of drawing color
    */
   void setColor (float r, float g, float b, float a);

   /**
    * Set bounding rectangle
    *
    * \param x1 X coordinate of top, left of bounding rectangle, in this element's coordinate space
    * \param y1 Y coordinate of top, left of bounding rectangle, in this element's coordinate space
    * \param x2 X coordinate + 1 of bottom, right of bounding rectangle, in this element's coordinate space
    * \param y2 Y coordinate + 1 of bottom, right of bounding rectangle, in this element's coordinate space
    */
   void setRect (float x1, float y1, float x2, float y2);

   /**
    * Set bounding circle
    *
    * \param xc X coordinate of center of bounding circle, in this element's coordinate space
    * \param yc Y coordinate of center of bounding circle, in this element's coordinate space
    * \param fRadius bounding circle's radius
    */
   void setCircle (float xc, float yc, float fRadius);

   /**
    * Set bounding ring
    *
    * \param xc X coordinate of center of bounding ring, in this element's coordinate space
    * \param yc Y coordinate of center of bounding ring, in this element's coordinate space
    * \param fOuterRadius bounding ring's outer radius
    * \param fInnerRadius bounding ring's inner radius
    */
   void setRing (float xc, float yc, float fOuterRadius, float fInnerRadius);

   /**
    * Set bounding polygon
    *
    * \param nPoints number of points in bounding polygon
    * \param v points of the polygon, in this element's coordinate space
    */
   void setPoly (long nPoints, const KVector2 *v);

   /**
    * Set infinite plane
    */
   void setInfinity (void);

   /**
    * Get this bounding primitive's first child
    *
    * \return this bounding primitive's first child, NULL if none
    */
   KUIBounds *getFirstChild (void);

   /**
    * Get next sibling of this bounding primitive
    *
    * \return this bounding primitive's next sibling, NULL if none
    */
   KUIBounds *getNextSibling (void);

   /**
    * Get screen-space axis-aligned bounding box for this primitive
    *
    * \return bounding box
    */
   inline KRectangle getScrBoundingBox (void) const { return _scrBounds.getBoundingBox(); }

private:
   /**
    * Recursively check if the hierarchy contains the specified point, in element coordinate space
    *
    * \param vCheck point to be checked
    *
    * \return true if the hierarchy contains the specified point, false if not
    */
   bool elemContains (const KVector2 &vCheck) const;

   /**
    * Recursively check if the hierarchy intersects the specified primitive, in element coordinate space
    *
    * \param primCheck primitive to be checked
    *
    * \return true if the hierarchy intersects the specified primitive, false if not
    */
   bool elemIntersects (KPrimitive &primCheck) const;

   /**
    * Recursively draw this bounding hierarchy, in element coordinate space
    *
    * \param r red component of drawing color
    * \param g green component of drawing color
    * \param b blue component of drawing color
    * \param a alpha component of drawing color
    */
   void elemBlit (float r, float g, float b, float a);

   /**
    * Recursively transform this bounding hierarchy from element to screen space
    *
    * \param mat transformation matrix
    */
   void transform (KMatrix &mat);

   /**
    * Check that bounding hierarchies intersect in screen space
    *
    * \param lpCheckedBounds bounding hierarchy to check against
    *
    * \return true if hierarchies intersect, false if not
    */
   bool scrIntersects (const KUIBounds *lpCheckedBounds) const;

   /**
    * Check if one primitive intersects with the specified bounding hierarchy
    *
    * \return true if intersection, false if not
    */
   bool scrIntersectsTree (const KUIBounds *lpCheckedBounds) const;

   /** Bounding primitive in element coordinate space */
   KPrimitive _elemBounds;

   /** Bounding primitive in screen coordinate space */
   KPrimitive _scrBounds;

   /** Parent bounding primitive */
   KUIBounds *_lpParent;

   /** Color for this primitive */ 
   float _fColor[4];

   /** List of children */
   KList<KUIBounds> _lChildren;
};

#endif   /* _KANJI_UIBOUNDS_H */