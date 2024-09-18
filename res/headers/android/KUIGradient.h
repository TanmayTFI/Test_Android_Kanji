/**
 * Kanji game engine - color gradient UI element definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_UIGRADIENT_H
#define  _KANJI_UIGRADIENT_H

/* Include definitions */
#include "KUIElement.h"
#include "KGraphic.h"

/**
 * Gradient UI element
 */

class KUIGradient : public KUIElement {
public:
   /**
    * Check if the supplied element is an image, and cast as KUIGradient if it is
    *
    * \return KUIGradient if element is an image, NULL otherwise
    */
   static KUIGradient *isGradient (KUIElement *lpElement);

   /**
    * Constructor
    *
    * \param lpParentElem parent element (the root or another element)
    */
   KUIGradient (KUIElement *lpParentElem);

   /** Destructor */
   virtual ~KUIGradient ();

   /**
    * Copy all parameters except for the name, identifier, user data, state, layer number, visibility, input and message delivery status, and message callback, from another element
    *
    * \param lpFromElement element to copy from
    */
   virtual void copyFromTemplate (KUIElement *lpFromElement);

   /**
    * Get color at one of the four edges
    *
    * \param nEdge edge number, in clockwise order (0=top left, 1=top right, 2=bottom right, 3=bottom left)
    * \param lpColor 4 floats where to store red, green, blue and alpha components
    */
   void getColor (long nEdge, float *lpColor);

   /**
    * Set color at one of the four edges
    *
    * \param nEdge edge number, in clockwise order (0=top left, 1=top right, 2=bottom right, 3=bottom left)
    * \param r red component
    * \param g green component
    * \param b blue component
    * \param a alpha component
    */
   void setColor (long nEdge, float r, float g, float b, float a);

   /**
    * Get lightness factor (multiplied to each of the rgb components, used for darkening this gradient)
    *
    * \return lightness factor, 0..1 (fully dark..normal color)
    */
   float getLightness (void);

   /**
    * Set lightness factor (multiplied to each of the rgb components, used for darkening this gradient)
    *
    * \param fLightness lightness factor, 0..1 (fully dark..normal color)
    */
   void setLightness (float fLightness);

   /**
    * Set alpha blending mode
    *
    * \param nAlphaMode new alpha blending mode (K_ALPHA_xxx)
    */
   void setAlphaMode (long nAlphaMode);

   /**
    * Get source alpha blending operation for the color channels of this graphic
    *
    * \return source alpha blending op (K_ALPHAOP_xxx)
    */
   eKanjiAlphaOp getSrcAlphaOp (void);
   
   /**
    * Get destination alpha blending operation for the color channels of this graphic
    *
    * \return destination alpha blending op (K_ALPHAOP_xxx)
    */
   eKanjiAlphaOp getDstAlphaOp (void);
   
   /**
    * Get source alpha blending operation for the alpha channel of this graphic
    *
    * \return source alpha blending op (K_ALPHAOP_xxx)
    */
   eKanjiAlphaOp getAlphaSrcAlphaOp (void);
   
   /**
    * Get destination alpha blending operation for the alpha channel of this graphic
    *
    * \return destination alpha blending op (K_ALPHAOP_xxx)
    */
   eKanjiAlphaOp getAlphaDstAlphaOp (void);

   /**
    * Set alpha blending operations for this graphic
    *
    * \param nSrcAlphaOp new source alpha blending op for the color channels (K_ALPHAOP_xxx)
    * \param nDstAlphaOp new destination alpha blending op for the color channels (K_ALPHAOP_xxx)
    * \param nAlphaSrcAlphaOp new source alpha blending op for the alpha channel (K_ALPHAOP_xxx)
    * \param nAlphaDstAlphaOp new destination alpha blending op for the alpha channel (K_ALPHAOP_xxx)
    */
   void setAlphaOp (eKanjiAlphaOp nSrcAlphaOp, eKanjiAlphaOp nDstAlphaOp,
                    eKanjiAlphaOp nAlphaSrcAlphaOp = (eKanjiAlphaOp) -1, eKanjiAlphaOp nAlphaDstAlphaOp = (eKanjiAlphaOp) -1);

protected:
   /**
    * Draw this element
    *
    * \param fBlend blending factor
    * \param lpMatrix world transformation matrix applied for this element
    */
   virtual void blit (float fBlend, KMatrix *lpMatrix);

private:
   /** Color at each edge */
   float _fColor[4][4];

   /** Lightness factor, 0..1 (fully dark..normal color) */
   float _fLightness;

   /** Source alpha operation for the color channels */
   eKanjiAlphaOp _nSrcAlphaOp;

   /** Destination alpha operation for the color channels */
   eKanjiAlphaOp _nDstAlphaOp;

   /** Source alpha operation for the alpha channel */
   eKanjiAlphaOp _nAlphaSrcAlphaOp;

   /** Destination alpha operation for the alpha channel */
   eKanjiAlphaOp _nAlphaDstAlphaOp;
};

#endif   /* _KANJI_UIGRADIENT_H */
