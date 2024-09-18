/**
 * Kanji rendering and I/O engine - doubly linked list
 *
 * (c) 2010
 */

#ifndef  _KANJI_KLIST_H
#define  _KANJI_KLIST_H

/* Include definitions */
#include "KObjectListable.h"

/**
 * Doubly linked list of objects derived from KObjectListable
 */

template <class T> class KList : public KObjectListable {
public:
   /** Constructor */
   KList () :
      _nCount (0),
      _lpFirst (NULL),
      _lpLast (NULL) {
   }

   /** Destructor */
   ~KList () {
   }

   /**
    * Get how many objects are in the list
    *
    * \return count of objects
    */
   inline int getCount (void) const {
      return _nCount;
   }

   /**
    * Check if there is no object in the list
    *
    * \return true if the list is empty, false if there is at least an object
    */
   inline bool isEmpty (void) const {
      return (_lpFirst == NULL);
   }

   /**
    * Get object at the head of the list
    *
    * \param lpObject returned object, if there is one; otherwise the passed variable isn't modified
    *
    * \return true if there is an object at the head and it was returned, false if the list is empty
    */
   bool getHead (T *&lpObject) const {
      if (_lpFirst) {
         lpObject = (T *) _lpFirst;
         return true;
      }
      else {
         return false;
      }
   }

   /**
    * Get object at the head of the list
    *
    * \return object if there is one at the head and it was returned, NULL if the list is empty
    */
   inline T *getHeadObject (void) const {
      return (T *) _lpFirst;
   }

   /**
    * Get object at the tail of the list
    *
    * \param lpObject returned object, if there is one; otherwise the passed variable isn't modified
    *
    * \return true if there is an object at the tail and it was returned, false if the list is empty
    */
   bool getTail (T *&lpObject) const {
      if (_lpLast) {
         lpObject = (T *) _lpLast;
         return true;
      }
      else {
         return false;
      }
   }

   /**
    * Get object at the tail of the list
    *
    * \return object if there is one at the tail and it was returned, NULL if the list is empty
    */
   inline T *getTailObject (void) const {
      return (T *) _lpLast;
   }

   /**
    * Get next object in the list
    *
    * \param lpObject object to get next of, returned object if there is one; otherwise the passed variable isn't modified
    *
    * \return true if there is an object after the specified one and it was returned, false if the specified object is at the tail of the list
    */
   static bool getNext (T *&lpObject) {
      if (lpObject->_lpNext) {
         lpObject = (T *) lpObject->_lpNext;
         return true;
      }
      else {
         return false;
      }
   }

   /**
    * Get previous object in the list
    *
    * \param lpObject object to get previous of, returned object if there is one; otherwise the passed variable isn't modified
    *
    * \return true if there is an object before the specified one and it was returned, false if the specified object is at the head of the list
    */
   static bool getPred (T *&lpObject) {
      if (lpObject->_lpPrev) {
         lpObject = (T *) lpObject->_lpPrev;
         return true;
      }
      else {
         return false;
      }
   }

   /**
    * Get next object in the list
    *
    * \param lpObject object to get next of
    *
    * \return next object if there is one after the specified one, NULL if the specified object is at the tail of the list
    */
   static inline T *getNextObject (T *lpObject) {
      return (T *) lpObject->_lpNext;
   }

   /**
    * Get previous object in the list
    *
    * \param lpObject object to get previous of
    *
    * \return previous object if there is an object before the specified one, NULL if the specified object is at the head of the list
    */
   static inline T *getPredObject (T *lpObject) {
      return (T *) lpObject->_lpPrev;
   }

   /**
    * Check if specified object is at the head of the list
    *
    * \return true if object is at the head of the list, false if it isn't
    */
   inline bool isHead (T *lpObject) const {
      return (lpObject == _lpFirst);
   }

   /**
    * Check if specified object is at the tail of the list
    *
    * \return true if object is at the tail of the list, false if it isn't
    */
   inline bool isTail (T *lpObject) const {
      return (lpObject == _lpLast);
   }

   /**
    * Insert specified object at the head of this list
    *
    * \param lpObject object to be inserted at the head
    */
   void addToHead (T *lpObject) {
      lpObject->_lpNext = _lpFirst;
      lpObject->_lpPrev = NULL;
      if (_lpFirst)
         _lpFirst->_lpPrev = lpObject;
      _lpFirst = lpObject;
      if (lpObject->_lpNext == NULL)
         _lpLast = lpObject;
      _nCount++;
   }

   /**
    * Insert specified object at the tail of this list
    *
    * \param lpObject object to be inserted at the tail
    */
   void addToTail (T *lpObject) {
      lpObject->_lpNext = NULL;
      lpObject->_lpPrev = _lpLast;
      if (_lpLast)
         _lpLast->_lpNext = lpObject;
      _lpLast = lpObject;
      if (lpObject->_lpPrev == NULL)
         _lpFirst = lpObject;
      _nCount++;
   }

   /**
    * Insert specified object before an object already present in the list
    *
    * \param lpRefObject insertion point
    * \param lpObject object to be inserted
    */
   void addBefore (T *lpRefObject, T *lpObject) {
      lpObject->_lpNext = lpRefObject;
      lpObject->_lpPrev = lpRefObject->_lpPrev;
      lpRefObject->_lpPrev = lpObject;
      if (lpObject->_lpPrev)
         lpObject->_lpPrev->_lpNext = lpObject;
      
      if (lpRefObject == _lpFirst)
         _lpFirst = lpObject;

      _nCount++;
   }
   
   /**
    * Insert specified object after an object already present in the list
    *
    * \param lpRefObject insertion point
    * \param lpObject object to be inserted
    */
   void addAfter (T *lpRefObject, T *lpObject) {
      lpObject->_lpNext = lpRefObject->_lpNext;
      lpObject->_lpPrev = lpRefObject;
      lpRefObject->_lpNext = lpObject;

      if (lpObject->_lpNext)
         lpObject->_lpNext->_lpPrev = lpObject;

      if (lpRefObject == _lpLast)
         _lpLast = lpObject;

      _nCount++;
   }

   /**
    * Remove specified object from the list
    *
    * \param lpObject object to be removed
    */
   void remove (T *lpObject) {
      if (lpObject->_lpPrev)
         lpObject->_lpPrev->_lpNext = lpObject->_lpNext;
      if (lpObject->_lpNext)
         lpObject->_lpNext->_lpPrev = lpObject->_lpPrev;
      if (lpObject == _lpFirst)
         _lpFirst = lpObject->_lpNext;
      if (lpObject == _lpLast)
         _lpLast = lpObject->_lpPrev;
      _nCount--;
   }

private:
   /** Number of objects in list */
   int _nCount;

   /** First object in list */
   KObjectListable *_lpFirst;

   /** Last object in list */
   KObjectListable *_lpLast;
};

#endif   /* _KANJI_KLIST_H */
