/**
 * Kanji rendering and I/O engine - strings hashtable definition
 *
 * (c) 2010
 */

#ifndef  _KANJI_KHASHTABLE_H
#define  _KANJI_KHASHTABLE_H

/** \cond private */

/* Include definitions */
#include "KObjectListable.h"
#include "KMath2d.h"

/** Number of hashtable buckets */
#define  _KHASH_SIZE   64

/** Base class of objects that can be inserted in a KHashTable */

class KObjectHashable : public KObjectListable {
public:
   /** Constructor */
   KObjectHashable ();

   /** Destructor */
   ~KObjectHashable ();

   /**
    * Set hashing key
    *
    * \param lpszHashKey hashing key
    */
   void setHashKey (const char *lpszHashKey);

   /** Hash key */
   const char *_lpszHashKey;

   /** Next object in the same hash bucket */
   KObjectHashable *_lpHashNext;

   /** Index of hashtable bucket that this object is in */
   long _nHash;
};

/** Hash table */

class KHashTable : public KObjectBase {
public:
   /** Constructor */
   KHashTable ();

   /** Destructor */
   ~KHashTable ();

   /**
    * Insert object into its hashtable bucket. If another object with the same name exists, the new object is inserted before the existing object
    *
    * \param lpObject object to be inserted
    */
   void hashInsert (KObjectHashable *lpObject);

   /**
    * Insert object into its hashtable bucket. If another object with the same name exists, the new object is inserted after the existing object
    *
    * \param lpObject object to be inserted
    */
   void hashInsertAfter (KObjectHashable *lpObject);

   /**
    * Remove object from its hashtable bucket
    *
    * \param lpObject object to be removed
    */
   void hashRemove (KObjectHashable *lpObject);

   /**
    * Find object corresponding to the specified case-sensitive key
    *
    * \param lpszKey key to find object for
    *
    * \return object, NULL if not found
    */
   KObjectHashable *hashFind (const char *lpszKey) const;

   /**
    * Find object corresponding to the specified case-insensitive key
    *
    * \param lpszKey key to find object for
    *
    * \return object, NULL if not found
    */
   KObjectHashable *hashFindNoCase (const char *lpszKey) const;

   /**
    * Hashing function
    *
    * \param lpszKey key to be hashed
    *
    * \return bucket number
    */
   long hashCompute (const char *lpszKey) const;

   /** Hashtable buckets */
   KObjectHashable *_lpHashFirst[_KHASH_SIZE];
};

/** \endcond */

#endif   /* _KANJI_KHASHTABLE_H */
