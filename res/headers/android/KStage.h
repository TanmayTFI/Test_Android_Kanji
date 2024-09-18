/**
 * Kanji game engine - 3d stage definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KSTAGE_H
#define  _KANJI_KSTAGE_H

/* Include definitions */
#include "KObjectBase.h"
#include "KWindow.h"
#include "KModel.h"

/**
 * Class modeling a 3d stage that can be used to render 3d models on
 */

class KStage : public KObjectBase {
public:
   /**
    * Constructor
    *
    * \param lpKWindow game window to render to
    */
   KStage (KWindow *lpKWindow);

   /** Destructor */
   ~KStage ();

   /**
    * Set projection matrix for this stage
    *
    * \param matProj projection matrix
    */
   void setProjection (KMatrix matProj);

   /**
    * Set camera matrix for this stage
    *
    * \param matCamera camera matrix
    */
   void setCamera (KMatrix matCamera);

   /**
    * Enable ambient light
    *
    * \param r red component
    * \param g green component
    * \param b blue component
    */
   void enableAmbientLight (float r, float g, float b);

   /**
    * Disable ambient light
    */
   void disableAmbientLight (void);

   /**
    * Enable light source
    *
    * \param nLightIdx index of light to be enabled, 0..7
    * \param nType light type (K_LIGHT_POINT, K_LIGHT_DIR)
    * \param r red component of light color, 0..1
    * \param g green component of light color, 0..1
    * \param b blue component of light color, 0..1
    * \param vPosition position in world coordinates (for K_LIGHT_POINT)
    * \param vDirection light direction vector (for K_LIGHT_DIR)
    * \param fConstAttenuation constant attenuation factor (typically set to 1): attenuation = 1 / ( fConstAttenuation + fLinearAttenuation * d + fQuadAttenuation * d^2)
    * \param fLinearAttenuation linear attenuation factor (typically set to 0)
    * \param fQuadAttenuation quadratic attenuation factor (typically set to 0)
    */
   void enableLight (long nLightIdx, KWindow3dLightType nType, float r, float g, float b, KVector3 vPosition, KVector3 vDirection,
                     float fConstAttenuation = 1, float fLinearAttenuation = 0, float fQuadAttenuation = 0);

   /**
    * Disable light source
    *
    * \param nLightIdx index of light to be disabled, 0..7
    */
   void disableLight (long nLightIdx);

   /**
    * Begin rendering models on this stage
    */
   void beginRender (void);

   /**
    * Render specified model
    *
    * \param lpModel model to be rendered
    * \param fFrame animation frame (0 .. lpModel->getFrameCount() - 1)
    * \param matWorld world transformation matrix for this model
    */
   void renderModel (KModel *lpModel, float fFrame, KMatrix matWorld);

   /**
    * End rendering models on this stage
    */
   void endRender (void);

private:
   /** Game window to render to */
   KWindow *_lpKWindow;

   /** Projection matrix */
   KMatrix _matProj;

   /** Camera matrix */
   KMatrix _matCamera;

   /** true if currently rendering */
   bool _bRendering;

   /** true if ambient light is enabled */
   bool _bAmbientLightEnabled;

   /** Ambient light color */
   float _fAmbientLight[3];

   /** For each light, true if enabled */
   bool _bLightEnabled[8];

   /** Lightsources */
   KWindow3dLight _light[8];
};

#endif   /* _KANJI_KSTAGE_H */

