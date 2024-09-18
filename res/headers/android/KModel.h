/**
 * Kanji game engine - 3d model definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KMODEL_H
#define  _KANJI_KMODEL_H

/* Include definitions */
#include "KObjectBase.h"
#include "KWindow.h"
#include "KGraphic.h"
#include "KBatch.h"
#include "KMath2d.h"
#include "KMath3d.h"

/** Model formats */
enum KModelFormat {
   K_MODEL_UNKNOWN = -1,               /**< Unknown */
   K_MODEL_3DS,                        /**< .3ds */
   K_MODEL_MS3D,                       /**< .ms3d */
   K_MODEL_OBJ,                        /**< .obj */
};

/** Maximum number of smoothing groups */
#define  K_MODEL_MAX_SMOOTHING   4

/** Vertex data for one frame */
typedef struct {
   KVector3 *lpVertex;                 /**< Vertex positions for this frame */
   KVector3 *lpVertexNormal[K_MODEL_MAX_SMOOTHING]; /**< Vertex normals for this frame */
   KVector3 *lpTriNormal;              /**< Triangle normals for this frame */
   KVector3 *lpTriVertexNormal;        /**< For each triangle, vertex normals for this frame */
} KModelFrame;

/** Data for a triangle in the model */
typedef struct {
   long nVertexIdx[3];                 /**< Index of each vertex of the triangle, in clockwise order */
   long nSmoothingGroup;               /**< Index of smoothing group for this triangle */
   long nMaterial;                     /**< Index of material */
   KVector2 vMatCoord[3];              /**< UV coordinates of each vertex of the triangle */
} KModelTriangle;

/** Data for a material of the model */
typedef struct {
   char szName[256];                   /**< Name of this material in the model file */
   char szGraphicsFileName[K_MAXPATH]; /**< Filename of this material's color or color+alpha texture */
   char szAlphaFileName[K_MAXPATH];    /**< Filename of this material's alpha texture */

   bool bTwoSided;                     /**< true if material is two-sided, false if one-sided */
   bool bTransparent;                  /**< true if material is at least partially transparent, false if it is fully opaque */
   float fDiffuse[3];                  /**< Diffuse color's r,g,b */

   bool bLoadedInternally;             /**< true if texture was loaded internally by KModel, false if it was set by the game; automatically set to false when passed to createModel() */
   KGraphic *lpMatGraphics;            /**< Loaded texture */
} KModelMaterial;

/* Forward definitions */
class KModelHandler3ds;
class KModelHandlerMs3d;
class KModelHandlerObj;

/**
 * Class representing a 3d model that can be positioned and rendered
 */

class KModel : public KObjectBase {
   friend class KModelHandler3ds;
   friend class KModelHandlerMs3d;
   friend class KModelHandlerObj;

public:
   /** Constructor */
   KModel ();

   /** Destructor */
   ~KModel ();

   /**
    * Detect model format from the supplied filename's extension (.obj, .3ds, ...)
    *
    * \param lpszFileName filename to detect model format from
    *
    * \return detected format, or K_MODEL_UNKNOWN if unsupported
    */
   static long detectFormat (const char *lpszFileName);

   /**
    * Load model file into this instance
    *
    * \param lpszFileName name of file to load model from
    * \param nStep loading step (0=load+upload, 1=load only, 2=upload only)
    *
    * \return true for success, false for failure
    */
   bool loadModel (const char *lpszFileName, long nStep = 0);

   /**
    * Load this model's textures
    *
    * \param lpszTextureFileNamePrefix string to be prefixed to all texture filenames
    * \param nStep loading step (0=load+upload, 1=load only, 2=upload only)
    *
    * \return true for success, false for failure
    */
   bool loadTextures (const char *lpszTextureFileNamePrefix, long nStep = 0);

   /**
    * Create this model dynamically
    *
    * \param nFrames number of frames (vertex positions) for this model
    * \param nVertices number of vertices for this model
    * \param lpVertex array of vertex arrays, one for each frame, each containing the number of specified vertices
    * \param nTriangles number of triangles for this model
    * \param lpTriangle array of triangles
    * \param nMaterials number of materials for this model
    * \param lpMaterial array of materials
    */
   void createModel (long nFrames, long nVertices, KVector3 **lpVertex, long nTriangles, KModelTriangle *lpTriangle, long nMaterials, KModelMaterial *lpMaterial);
   
   /**
    * Free all resources used by this model
    */
   void freeModel (void);

   /**
    * Draw this model
    *
    * \param lpKWindow window that this model is rendered to
    * \param fFrame animation frame to be drawn (0..getFrameCount() - 1)
    */
   void blitModel (KWindow *lpKWindow, float fFrame);

   /**
    * Get number of frames in this model
    *
    * \return number of frames
    */
   long getFrameCount (void);

   /**
    * Get number of vertices in this model
    *
    * \return number of vertices
    */
   long getVertexCount (void);

   /**
    * Get number of triangles in this model
    *
    * \return number of triangles
    */
   long getTriangleCount (void);

   /**
    * Get number of materials in this model
    *
    * \return number of materials
    */
   long getMaterialCount (void);

   /**
    * Get material information from this model
    *
    * \param nMaterialIdx index of material to get information about (0..getMaterialCount() - 1)
    * \param lpMaterial material, filled by this method if successful
    *
    * \return true if material index was within bounds and material information was filled, false if not
    */
   bool getMaterial (long nMaterialIdx, KModelMaterial *lpMaterial);

private:
   /**
    * Decode model from a memory buffer into this instance
    *
    * \param lpszFileName model's filename (used to look at the extension, and in case additional files such as material libraries need to be loaded)
    * \param lpBuffer memory buffer to decode model from
    * \param nBufferSize size of memory buffer containing encoded model, in bytes
    * \param nStep loading step (0=load+upload, 1=load only, 2=upload only)
    *
    * \return true for success, false for failure
    */
   bool loadModelFromPtr (const char *lpszFileName, const unsigned char *lpBuffer, long nBufferSize, long nStep);

   /** Compute triangle and vertex normals for all frames */
   void computeNormals (void);

   /** Sort triangles by material */
   void sortTriangles (void);

   /** Upload or re-upload model data to a vertex buffer */
   void updateVertexBuffer (void);

   /** Number of frames in this model */
   long _nFrames;

   /** Number of vertices in this model */
   long _nVertices;

   /** Number of triangles in this model */
   long _nTriangles;

   /** Number of materials in this model */
   long _nMaterials;

   /** Vertex data for each frame */
   KModelFrame *_lpFrame;

   /** Triangle data */
   KModelTriangle *_lpTriangle;

   /** Material data */
   KModelMaterial *_lpMaterial;

   /** When computing vertex normals, for each vertex, number of triangles whose normal contributes to the vertex normal */
   long *_lpVertexNormalTriCount;

   /** Vertex buffer containing the model ready to be drawn */
   KBatch *_lpBatch;

   /** Number of entries in the list of vertices to be drawn */
   long _nDrawListLen;

   /** Starting vertex, number of vertices, and material to be used, for drawing all triangles in the model */
   long *_lpnDrawList;

   /** true if UV coordinates in materials have been fixed to account for the graphics being aligned on pow2 sizes */
   bool _bMatCoordFixed;
};

#endif   /* _KANJI_KMODEL_H */
