/**
 * Kanji rendering and I/O engine - 3D math definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KMATH3D_H
#define  _KANJI_KMATH3D_H

/* Include definitions */
#include "KMath2d.h"

/**
 * Class modeling a 3D vector
 */

class KVector3 {
public:
   /**
    * Constructor
    */
   inline KVector3 () {}

   /**
    * Constructor from initial coordinates
    *
    * \param x initial X coordinate
    * \param y initial Y coordinate
    * \param z initial Z coordinate
    */
   inline KVector3 (float x, float y, float z) : x (x), y (y), z (z) {}

   /**
    * '+' operator. Add this vector and a second vector and return the result
    *
    * \param v2 vector to be added to this one (right hand vector)
    *
    * \return this vector + v2
    */
   inline KVector3 operator + (const KVector3 &v2) { return KVector3 (x + v2.x, y + v2.y, z + v2.z); }

   /**
    * '+=' operator. Add a second vector to this one
    *
    * \param v2 vector to be added to this one (right hand vector)
    */
   inline void operator += (const KVector3 &v2) { x += v2.x; y += v2.y; z += v2.z; }

   /**
    * '-' operator. Substract a second vector from this vector and return the result
    *
    * \param v2 vector to be substracted from this one (right hand vector)
    *
    * \return this vector - v2
    */
   inline KVector3 operator - (const KVector3 &v2) { return KVector3 (x - v2.x, y - v2.y, z - v2.z); }

   /**
    * '-=' operator. Substract a second vector to this one
    *
    * \param v2 vector to be substracted from this one (right hand vector)
    */
   inline void operator -= (const KVector3 &v2) { x -= v2.x; y -= v2.y; z -= v2.z; }

   /**
    * '*' operator. Scale vector according to value and return result
    *
    * \param fMultiplier value to multiply all coordinates by
    *
    * \return this vector * fScale
    */
   inline KVector3 operator * (float fMultiplier) { return KVector3 (x * fMultiplier, y * fMultiplier, z * fMultiplier); }

   /**
    * '*=' operator. Scale vector according to value
    *
    * \param fMultiplier value to multiply all coordinates by
    */
   inline void operator *= (float fMultiplier) { x *= fMultiplier; y *= fMultiplier; z *= fMultiplier; }

   /**
    * '/' operator. Divide vector according to value and return result
    *
    * \param fDivisor value to divide all coordinates by
    *
    * \return this vector * fScale
    */
   inline KVector3 operator / (float fDivisor) { return KVector3 (x / fDivisor, y / fDivisor, z / fDivisor); }

   /**
    * '/=' operator. Divide vector according to value
    *
    * \param fDivisor value to divide all coordinates by
    */
   inline void operator /= (float fDivisor) { x /= fDivisor; y /= fDivisor; z /= fDivisor; }

   /**
    * Get length (norm) of this vector
    *
    * \return length
    */
   inline float length (void) { return sqrtf (x * x + y * y + z * z); }

   /**
    * Calculate dot product of this vector and a specified vector
    *
    * \param v2 right hand vector in dot product
    *
    * \return dot product
    */
   inline float dotProduct (KVector3 &v2) { return x * v2.x + y * v2.y + z * v2.z; }

   /**
    * Calculate cross product of this vector and a specified vector
    *
    * \param v2 right hand vector in cross product
    *
    * \return cross product
    */
   inline KVector3 crossProduct (KVector3 &v2) { return KVector3 (y * v2.z - z * v2.y, z * v2.x - x * v2.z, x * v2.y - y * v2.x); }

   /**
    * Normalize this vector
    */
   void normalize (void);

   /**
    * Compute the normal vector of the plane defined by the specified points
    *
    * \param v1 first point defining the plane
    * \param v2 second point defining the plane
    * \param v3 third point defining the plane
    *
    * \return normal
    */
   static KVector3 normal (KVector3 &v1, KVector3 &v2, KVector3 &v3);

   /** X coordinate */
   float x;

   /** Y coordinate */
   float y;

   /** Z coordinate */
   float z;
};

/**
 * Class modeling a 4D (XYZW) vector
 */

class KVector4 {
public:
   /**
    * Constructor
    */
   inline KVector4 () {}

   /**
    * Constructor from initial coordinates
    *
    * \param x initial X coordinate
    * \param y initial Y coordinate
    * \param z initial Z coordinate
    * \param w initial W coordinate
    */
   inline KVector4 (float x, float y, float z, float w) : x (x), y (y), z (z), w (w) {}

   /**
    * '+' operator. Add this vector and a second vector and return the result
    *
    * \param v2 vector to be added to this one (right hand vector)
    *
    * \return this vector + v2
    */
   inline KVector4 operator + (const KVector4 &v2) { return KVector4 (x + v2.x, y + v2.y, z + v2.z, w + v2.w); }

   /**
    * '+=' operator. Add a second vector to this one
    *
    * \param v2 vector to be added to this one (right hand vector)
    */
   inline void operator += (const KVector4 &v2) { x += v2.x; y += v2.y; z += v2.z; }

   /**
    * '-' operator. Substract a second vector from this vector and return the result
    *
    * \param v2 vector to be substracted from this one (right hand vector)
    *
    * \return this vector - v2
    */
   inline KVector4 operator - (const KVector4 &v2) { return KVector4 (x - v2.x, y - v2.y, z - v2.z, w - v2.w); }

   /**
    * '-=' operator. Substract a second vector to this one
    *
    * \param v2 vector to be substracted from this one (right hand vector)
    */
   inline void operator -= (const KVector4 &v2) { x -= v2.x; y -= v2.y; z -= v2.z; w -= v2.w; }

   /**
    * '*' operator. Scale vector according to value and return result
    *
    * \param fMultiplier value to multiply all coordinates by
    *
    * \return this vector * fScale
    */
   inline KVector4 operator * (float fMultiplier) { return KVector4 (x * fMultiplier, y * fMultiplier, z * fMultiplier, w * fMultiplier); }

   /**
    * '*=' operator. Scale vector according to value
    *
    * \param fMultiplier value to multiply all coordinates by
    */
   inline void operator *= (float fMultiplier) { x *= fMultiplier; y *= fMultiplier; z *= fMultiplier; w *= fMultiplier; }

   /**
    * '/' operator. Divide vector according to value and return result
    *
    * \param fDivisor value to divide all coordinates by
    *
    * \return this vector * fScale
    */
   inline KVector4 operator / (float fDivisor) { return KVector4 (x / fDivisor, y / fDivisor, z / fDivisor, w / fDivisor); }

   /**
    * '/=' operator. Divide vector according to value
    *
    * \param fDivisor value to divide all coordinates by
    */
   inline void operator /= (float fDivisor) { x /= fDivisor; y /= fDivisor; z /= fDivisor; w /= fDivisor; }

   /** X coordinate */
   float x;

   /** Y coordinate */
   float y;

   /** Z coordinate */
   float z;

   /** W coordinate */
   float w;
};

/**
 * Class modeling a 4x4 matrix
 */

class KMatrix {
public:
   /** Constructor */
   KMatrix ();

   /**
    * Constructor
    *
    * \param m11 value at 0,0
    * \param m12 value at 0,1
    * \param m13 value at 0,2
    * \param m14 value at 0,3
    * \param m21 value at 1,0
    * \param m22 value at 1,1
    * \param m23 value at 1,2
    * \param m24 value at 1,3
    * \param m31 value at 2,0
    * \param m32 value at 2,1
    * \param m33 value at 2,2
    * \param m34 value at 2,3
    * \param m41 value at 3,0
    * \param m42 value at 3,1
    * \param m43 value at 3,2
    * \param m44 value at 3,3
    */
   inline KMatrix (float m11, float m12, float m13, float m14,
                   float m21, float m22, float m23, float m24,
                   float m31, float m32, float m33, float m34,
                   float m41, float m42, float m43, float m44) :
      _11(m11), _12(m12), _13(m13), _14(m14),
      _21(m21), _22(m22), _23(m23), _24(m24),
      _31(m31), _32(m32), _33(m33), _34(m34),
      _41(m41), _42(m42), _43(m43), _44(m44) {
   }
                 
   /**
    * "*" operator
    *
    * \param m2 second matrix
    *
    * \return this matrix * second matrix
    */
   const KMatrix operator * (const KMatrix &m2);

   /**
    * "*=" operator
    *
    * \param m2 second matrix
    *
    * \return this matrix * second matrix
    */
   inline const KMatrix& operator *= (const KMatrix &m2) { *this = *this * m2; return *this; }

   /**
    * Multiply a 2d translation by the specified full matrix, quicker than filling a translation matrix and doing a full multiply
    *
	 * \param tx X coordinate of the translation vector
	 * \param ty Y coordinate of the translation vector
    * \param m2 second matrix (full matrix to multiply 2d translation by)
    *
    * \return 2d translation matrix * m2
    */
   static const KMatrix applyTranslation2d (float tx, float ty, const KMatrix &m2);

   /**
    * Multiply a rotation over the Z axis by the specified full matrix, quicker than filling a rotation matrix and doing a full multiply
    *
    * \param fZAngle angle of rotation over the Z axis, in radians
    * \param m2 second matrix (full matrix to multiply rotation over the Z axis by)
    *
    * \return rotation matrix * m2
    */
   static const KMatrix applyRotationZ (float fZAngle, const KMatrix &m2);

   /**
    * Multiply a 2d scaling by the specified full matrix, quicker than filling a scaling matrix and doing a full multiply
    *
	 * \param sx scaling over the X axis
	 * \param sy scaling over the Y axis
    * \param m2 second matrix (full matrix to multiply scaling by)
    *
    * \return scaling matrix * m2
    */
   static const KMatrix applyScaling2d (float sx, float sy, const KMatrix &m2);

   /**
    * Multiply a 2d shear by the specified full matrix, quicker than filling a shear matrix and doing a full multiply
    *
    * \param fXY shear factor to shear X by Y
    * \param fYX shear factor to shear Y by X
    * \param m2 second matrix (full matrix to multiply shear by)
    *
    * \return m1 * m2
    */
   static const KMatrix applyShear2d (float fXY, float fYX, const KMatrix &m2);

   /**
    * Multiply two matrices quicker than a full multiplication, by knowing that the first matrix is a translation matrix
    *
    * \param m1 first matrix (translation matrix)
    * \param m2 second matrix (full matrix to multiply translation matrix by)
    *
    * \return m1 * m2
    */
   static const KMatrix multiplyTranslation (const KMatrix &m1, const KMatrix &m2);

   /**
    * Multiply two matrices quicker than a full multiplication, by knowing that the first matrix is a rotation over the Z axis matrix
    *
    * \param m1 first matrix (rotation over the Z axis matrix)
    * \param m2 second matrix (full matrix to multiply rotation over the Z axis matrix by)
    *
    * \return m1 * m2
    */
   static const KMatrix multiplyRotationZ (const KMatrix &m1, const KMatrix &m2);

   /**
    * Multiply two matrices quicker than a full multiplication, by knowing that the first matrix is a scaling matrix
    *
    * \param m1 first matrix (scaling matrix)
    * \param m2 second matrix (full matrix to multiply scaling matrix by)
    *
    * \return m1 * m2
    */
   static const KMatrix multiplyScaling (const KMatrix &m1, const KMatrix &m2);

   /**
    * Multiply two matrices quicker than a full multiplication, by knowing that the first matrix is a shear matrix
    *
    * \param m1 first matrix (shear matrix)
    * \param m2 second matrix (full matrix to multiply shear matrix by)
    *
    * \return m1 * m2
    */
   static const KMatrix multiplyShear (const KMatrix &m1, const KMatrix &m2);

	/** Initialize matrix to identity */
   void identity (void);

   /**
    * Calculate the determinant of this matrix
    *
    * \return determinant
    */
   float determinant (void);

   /**
    * Calculate the inverse of this matrix
    *
    * \return inverse
    */
   KMatrix inverse (void);

	/**
	 * Initialize this matrix for an orthographic projection
    *
    * \param fWidth width of viewport
    * \param fHeight height of viewport
	 * \param fZNear near Z clipping plane
	 * \param fZFar far Z clipping plane
    */
   void ortho (float fWidth, float fHeight, float fZNear, float fZFar);
   
   /**
    * Initialize this matrix for an orthographic projection
    *
    * \param fLeft leftmost X coordinate of viewport
    * \param fRight rightmost X coordinate of viewport
    * \param fBottom bottomost Y coordinate of viewport
    * \param fTop topmost Y coordinate of viewport
	 * \param fZNear near Z clipping plane
	 * \param fZFar far Z clipping plane
    */
   void orthoRect (float fLeft, float fRight, float fBottom, float fTop, float fZNear, float fZFar);

	/**
	 * Initialize this matrix for a perspective projection
	 *
	 * \param fFov field of view angle, in radians
	 * \param fRatio aspect ratio expressed as viewport width divided by viewport height
	 * \param fZNear distance from the viewer to the near Z clipping plane
	 * \param fZFar distance form the viewer to the far Z clipping plane
	 */
   void perspective (float fFov, float fRatio, float fZNear, float fZFar);

   /**
    * Set camera matrix to look at specified point from specified location
    *
    * \param vEye position of the eye point (camera)
    * \param vAt position of the reference point (point being looked at)
    * \param vUp direction of the up vector (for instance 0, 1, 0)
    */
   void camera (KVector3 &vEye, KVector3 &vAt, KVector3 &vUp);

	/**
	 * Initialize this matrix for a translation
	 *
	 * \param tx X coordinate of the translation vector
	 * \param ty Y coordinate of the translation vector
	 * \param tz Z coordinate of the translation vector
	 */
   void translation (float tx, float ty, float tz);

	/**
	 * Initialize this matrix for a rotation over the X axis
	 *
	 * \param fXAngle angle of rotation over the X axis, in radians
	 */
   void rotationX (float fXAngle);

	/**
	 * Initialize this matrix for a rotation over the Y axis
	 *
	 * \param fYAngle angle of rotation over the Y axis, in radians
	 */
   void rotationY (float fYAngle);

	/**
	 * Initialize this matrix for a rotation over the Z axis
	 *
	 * \param fZAngle angle of rotation over the Z axis, in radians
	 */
   void rotationZ (float fZAngle);

	/**
	 * Set matrix to a scaling matrix
	 *
	 * \param sx scaling over the X axis
	 * \param sy scaling over the Y axis
	 * \param sz scaling over the Z axis
	 */
   void scaling (float sx, float sy, float sz);

   /**
    * Set matrix to a shear matrix
    *
    * \param fXY shear factor to shear X by Y
    * \param fXZ shear factor to shear X by Z
    * \param fYX shear factor to shear Y by X
    * \param fYZ shear factor to shear Y by Z
    * \param fZX shear factor to shear Z by X
    * \param fZY shear factor to shear Z by Y
    */
   void shear (float fXY, float fXZ, float fYX, float fYZ, float fZX, float fZY);

	/**
	 * Multiply a 2D vector by this matrix
	 *
	 * \param v vector to be multiplied by this matrix
	 *
	 * \return vector multiplied by this matrix
	 */
   KVector2 transform2 (const KVector2 &v);

	/**
	 * Multiply a 2D vector by this matrix and reverse Y component before and after the transform. This is the suitable transform for 2D coordinates where (0,0) is the top,left
	 *
	 * \param v vector to be multiplied by this matrix
	 *
	 * \return vector multiplied by this matrix
	 */
   KVector2 transform2Up (const KVector2 &v);

	/**
	 * Multiply a 3D vector by this matrix
	 *
	 * \param v vector to be multiplied by this matrix
	 *
	 * \return vector multiplied by this matrix
	 */
   KVector3 transform (const KVector3 &v);

	/**
	 * Multiply a 3D vector by this matrix and return a 4D vector
	 *
	 * \param v vector to be multiplied by this matrix
	 *
	 * \return vector multiplied by this matrix, including W
	 */
   KVector4 transform4 (const KVector3 &v);

   union {
      /* Matrix expressed as one-based row-column positions */
      struct {
	      float _11, _12, _13, _14;
	      float _21, _22, _23, _24;
	      float _31, _32, _33, _34;
	      float _41, _42, _43, _44;
      };

      /** Matrix expressed as zero-based row-column positions */
      float _mat[4][4];
   };
};

#endif   /* _KANJI_KMATH3D_H */
