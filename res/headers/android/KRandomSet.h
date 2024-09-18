/*
 * Kanji rendering and I/O engine - random dataset manager definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KRANDOMSET_H
#define  _KANJI_KRANDOMSET_H

/* Include definitions */
#include "KRandom.h"

/**
 * Random dataset manager: get a random number among a defined set of available numbers, and don't get that random number again
 * until all numbers in the dataset have been returned
 *
 * \param nMaxEntries maximum number of entries in the dataset
 */

template <long nMaxEntries> class KRandomSet {
public:
   /** Constructor */
   KRandomSet () :
         _nEntries (0),
         _nLastSelEntryIdx (-1) {
   }

   /** Destructor */
   ~KRandomSet () {
   }

   /**
    * Initialize this set
    *
    * \param nNewEntries count of actual numbers in the set, 1..max
    */    
   void reset (long nNewEntries) {
      long i;

      if (nNewEntries < 0) nNewEntries = 0;
      if (nNewEntries > nMaxEntries) nNewEntries = nMaxEntries;

      _nEntries = nNewEntries;
      for (i = 0; i < _nEntries; i++)
         bAvailable[i] = false;
      _nLastSelEntryIdx = -1;
   }

   /**
    * Mark specified number as available
    *
    * \param nEntry number to make available, 0..(_nEntries - 1)
    */
   inline void makeAvailable (long nEntry) {
      if (nEntry >= 0 && nEntry < _nEntries) bAvailable[nEntry] = true;
   }

   /**
    * Mark specified number as unavailable
    *
    * \param nEntry number to make unavailable, 0..(_nEntries - 1)
    */
   void makeUnavailable (long nEntry) {
      if (nEntry >= 0 && nEntry < _nEntries) bAvailable[nEntry] = false;
   }

   /**
    * Get a random number among the available ones in the set. This number will not be returned again until all available numbers in the
    * set have been returned. When no number is available anymore, all numbers are marked as available again
    *
    * \return random number, 0..(_nEntries - 1)
    */
   long getRandomEntry (void) {
      long nSelEntryIdx;

      if (!_nEntries) return 0;

      /* Select entry in set */
      nSelEntryIdx = selectRandomEntry ();
      if (nSelEntryIdx < 0) {
         long i;

         /* All entries are unavailable. Make them all available again. */
         for (i = 0; i < _nEntries; i++)
            bAvailable[i] = true;

         if (_nEntries > 1 && _nLastSelEntryIdx >= 0 && _nLastSelEntryIdx < _nEntries) {
            /* Don't re-select last selected entry right away */
            bAvailable[_nLastSelEntryIdx] = false;
         }

         /* Select entry again */
         nSelEntryIdx = selectRandomEntry ();
      }
      
      bAvailable[nSelEntryIdx] = false;
      _nLastSelEntryIdx = nSelEntryIdx;
      return nSelEntryIdx;
   }

   /**
    * Get a random number among the available ones in the set. If no numbers are available anymore, an error is returne
    *
    * \return random number, 0..(_nEntries - 1), or -1 if no numbers are available
    */
   long selectRandomEntry (void) {     
      long i, nAvailable;

      if (_nEntries <= 0) return -1;

      /* Find available entries */
      nAvailable = 0;
      for (i = 0; i < _nEntries; i++) {
         if (bAvailable[i])
            _nEntryIdx[nAvailable++] = i;
      }
      
      if (!nAvailable) {
         /* No entries available */
         return -1;
      }

      /* Randomly select an entry */
      return _nEntryIdx[((long)(KRandom::getRandomFloat() * nAvailable)) % nAvailable];
   }

   /** true if the corresponding number in the set is available */
   bool bAvailable[nMaxEntries];

private:
   /** Count of number in this set */
   long _nEntries;

   /** Index of last returned entry */
   long _nLastSelEntryIdx;

   /** Available entry indices, when selecting one */
   long _nEntryIdx[nMaxEntries];
};

#endif   /* _KANJI_KRANDOMSET_H */
