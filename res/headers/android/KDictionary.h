/**
 * Kanji rendering and I/O engine - key/value pairs dictionary definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KDICTIONARY_H
#define  _KANJI_KDICTIONARY_H

/* Include definitions */
#include "KObjectListable.h"
#include "KList.h"
#include "KHashTable.h"
#include "KIniReader.h"
#include <string>

#ifdef K_WIN32
typedef __int64 kdictionary_int64_t;
#else
typedef long long kdictionary_int64_t;
#endif

/** Dictionary value type */
typedef enum {
   K_DICTIONARYVALUE_NONE = 0,         /**< Undefined */
   K_DICTIONARYVALUE_INT,              /**< Int */
   K_DICTIONARYVALUE_LONG,             /**< Long */
   K_DICTIONARYVALUE_FLOAT,            /**< Float */
   K_DICTIONARYVALUE_DOUBLE,           /**< Double */
   K_DICTIONARYVALUE_BOOL,             /**< Boolean */
   K_DICTIONARYVALUE_STRING,           /**< Zero-terminated UTF8 string */
   K_DICTIONARYVALUE_INT64,            /**< 64-bit integer */
   K_DICTIONARYVALUE_DICTIONARY,       /**< Sub-dictionary */
} KDictionaryValueType;

/** Forward declarations */
class KDictionary;

/**
 * One entry in a dictionary
 */

class KDictionaryValue : public KObjectHashable {
public:
   /**
    * Constructor
    *
    * \param lpParentDictionary dictionary that will contain this value
    * \param lpszKey key
    */
   KDictionaryValue (KDictionary *lpParentDictionary, const char *lpszKey);

   /**
    * Destructor
    */
   ~KDictionaryValue ();

   /**
    * Get this value's key as C string
    *
    * \return key
    */
   const char *getKey (void) const;

   /**
    * Get this value's key as std::string
    *
    * \return key
    */
   std::string getKeyAsStdString (void) const;

   /**
    * Get this value's type
    *
    * \return type
    */
   KDictionaryValueType getType (void) const;

   /**
    * Get integer representation of this value
    *
    * \return value
    */
   int getIntValue (void) const;

   /**
    * Set value type as integer and update value
    *
    * \param nValue new value
    */
   void setIntValue (int nValue);

   /**
    * Get long representation of this value
    *
    * \return value
    */
   long getLongValue (void) const;

   /**
    * Set value type as long and update value
    *
    * \param nValue new value
    */
   void setLongValue (long nValue);

   /**
    * Get 64-bit integer representation of this value
    *
    * \return value
    */
   kdictionary_int64_t getInt64Value(void) const;

   /**
    * Set value type as 64-bit integer and update value
    *
    * \param nValue new value
    */
   void setInt64Value(kdictionary_int64_t nValue);

   /**
    * Get float representation of this value
    *
    * \return value
    */
   float getFloatValue (void) const;

   /**
    * Set value type as float and update value
    *
    * \param fValue new value
    */
   void setFloatValue (float fValue);

   /**
    * Get double representation of this value
    *
    * \return value
    */
   double getDoubleValue (void) const;

   /**
    * Set value type as double and update value
    *
    * \param fValue new value
    */
   void setDoubleValue (double fValue);

   /**
    * Get boolean representation of this value
    *
    * \return value
    */
   bool getBoolValue (void) const;

   /**
    * Set value type as boolean and update value
    *
    * \param bValue new value
    */
   void setBoolValue (bool bValue);

   /**
    * Get string representation of this value
    *
    * \return string
    */
   const char *getStringValue (void);

   /**
    * Set value type as string and update value
    *
    * \param lpszValue new value
    */
   void setStringValue (const char *lpszValue);

   /**
    * Get string representation of this value
    *
    * \return string
    */
   std::string getStdStringValue(void);

   /**
    * Set value type as string and update value
    *
    * \param strValue new value
    */
   void setStdStringValue(const std::string &strValue);

   /**
    * Get sub-dictionary, or NULL if this value is of a different type
    *
    * \return dictionary or NULL
    */
   KDictionary *getDictionaryValue (void);

   /**
    * Set value type as dictionary and update value
    *
    * \param lpDictionary new value
    */
   void setDictionaryValue (const KDictionary *lpValue);

   /**
    * Get the next value in the dictionary. The values are in no specific order
    *
    * \return next value, or NULL if this was the last value in the dictionary
    */
   KDictionaryValue *getNextValue (void);

private:
   /** Key */
   const char *_lpszKey;

   /** Type */
   KDictionaryValueType _nValueType;

   /** Value */
   union {
      /** Value as integer */
      int _nAsInt;

      /** Value as long */
      long _nAsLong;

      /** Value as float */
      float _fAsFloat;

      /** Value as double */
      double _fAsDouble;

      /** Value as 64-bit integer */
      kdictionary_int64_t _nAsInt64;

      /** Value as boolean */
      bool _bAsBool;
   } value;

   /** Value as string */
   const char *_lpszAsString;

   /** Value as sub-dictionary */
   KDictionary *_lpAsSubDictionary;

   /** Parent dictionary */
   KDictionary *_lpParentDictionary;
};

/**
 * Class managing a dictionary of key/value pairs
 */

class KDictionary {
   friend class KDictionaryValue;

public:
   /** Constructor */
   KDictionary ();

   /**
    * Copy constructor
    *
    * \param lpDictionary dictionary to copy
    */
   KDictionary (const KDictionary *lpDictionary);
   
   /** Destructor */
   ~KDictionary ();

   /** Empty the dictionary */
   void clear (void);

   /**
    * Merge values from another dictionary into this one
    *
    * \param lpDictionary dictionary to merge from
    */
   void merge (const KDictionary *lpDictionary);

   /**
    * Check if a key exists in the dictionary
    *
    * \param lpszKey key
    *
    * \return true if key exists, false if not
    */
   bool hasKey (const char *lpszKey) const;

   /**
    * Get value type for key
    *
    * \param lpszKey key
    *
    * \return type, K_DICTIONARYVALUE_NONE if not found
    */
   KDictionaryValueType getValueType (const char *lpszKey) const;

   /**
    * Get value as an integer
    *
    * \param lpszKey key
    * \param nDefaultValue default value
    *
    * \return value
    */
   int getInt (const char *lpszKey, int nDefaultValue = 0) const;

   /**
    * Set value as an integer
    *
    * \param lpszKey key
    * \param nValue value
    */
   void setInt (const char *lpszKey, int nValue);

   /**
    * Get value as a long integer
    *
    * \param lpszKey key
    * \param nDefaultValue default value
    *
    * \return value
    */
   long getLong (const char *lpszKey, long nDefaultValue = 0) const;

   /**
    * Set value as a long integer
    *
    * \param lpszKey key
    * \param nValue value
    */
   void setLong (const char *lpszKey, long nValue);

   /**
    * Get value as a 64-bit integer
    *
    * \param lpszKey key
    * \param nDefaultValue default value
    *
    * \return value
    */
   kdictionary_int64_t getInt64 (const char *lpszKey, kdictionary_int64_t nDefaultValue = 0) const;

   /**
    * Set value as a 64-bit integer
    *
    * \param lpszKey key
    * \param nValue value
    */
   void setInt64 (const char *lpszKey, kdictionary_int64_t nValue);

   /**
    * Get value as a float
    *
    * \param lpszKey key
    * \param fDefautValue default value
    *
    * \return value
    */
   float getFloat (const char *lpszKey, float fDefautValue = 0.0f) const;

   /**
    * Set value as a float
    *
    * \param lpszKey key
    * \param fValue value
    */
   void setFloat (const char *lpszKey, float fValue);

   /**
    * Get value as a float
    *
    * \param lpszKey key
    * \param fDefautValue default value
    *
    * \return value
    */
   double getDouble (const char *lpszKey, double fDefautValue = 0.0) const;

   /**
    * Set value as a float
    *
    * \param lpszKey key
    * \param fValue value
    */
   void setDouble (const char *lpszKey, double fValue);

   /**
    * Get value as a boolean
    *
    * \param lpszKey key
    * \param bDefaultValue default value
    *
    * \return value
    */
   bool getBool (const char *lpszKey, bool bDefaultValue = false) const;

   /**
    * Set value as a boolean
    *
    * \param lpszKey key
    * \param bValue value
    */
   void setBool (const char *lpszKey, bool bValue);

   /**
    * Get value as a C string
    *
    * \param lpszKey key
    * \param lpszDefaultValue default value
    *
    * \return value
    */
   const char *getString (const char *lpszKey, const char *lpszDefaultValue = NULL);

   /**
    * Set value as a C string
    *
    * \param lpszKey key
    * \param lpszValue value
    */
   void setString (const char *lpszKey, const char *lpszValue);

   /**
    * Get value as a std::string
    *
    * \param lpszKey key
    * \param strDefaultValue default value
    *
    * \return value
    */
   std::string getStdString (const char *lpszKey, const std::string &strDefaultValue = "");

   /**
    * Set value as a std::string
    *
    * \param lpszKey key
    * \param lpszValue value
    */
   void setStdString (const char *lpszKey, const std::string &strValue);

   /**
    * Get value as a dictionary
    *
    * \param lpszKey key
    * \param lpDefaultValue default value
    *
    * \return value
    */
   KDictionary *getDictionary (const char *lpszKey, KDictionary *lpDefaultValue = NULL);

   /**
    * Set value as a C string
    *
    * \param lpszKey key
    * \param lpszValue value
    */
   void setDictionary (const char *lpszKey, const KDictionary *lpValue);

   /**
    * Check if a key exists in the dictionary
    *
    * \param strKey key
    *
    * \return true if key exists, false if not
    */
   bool hasKey (const std::string &strKey) const;

   /**
    * Get value type for key
    *
    * \param strKey key
    *
    * \return type, K_DICTIONARYVALUE_NONE if not found
    */
   KDictionaryValueType getValueType (const std::string &strKey) const;

   /**
    * Get value as an integer
    *
    * \param strKey key
    * \param nDefaultValue default value
    *
    * \return value
    */
   int getInt (const std::string &strKey, int nDefaultValue = 0) const;

   /**
    * Set value as an integer
    *
    * \param strKey key
    * \param nValue value
    */
   void setInt (const std::string &strKey, int nValue);

   /**
    * Get value as a long integer
    *
    * \param strKey key
    * \param nDefaultValue default value
    *
    * \return value
    */
   long getLong (const std::string &strKey, long nDefaultValue = 0) const;

   /**
    * Set value as a long integer
    *
    * \param strKey key
    * \param nValue value
    */
   void setLong (const std::string &strKey, long nValue);

   /**
    * Get value as a 64-bit integer
    *
    * \param strKey key
    * \param nDefaultValue default value
    *
    * \return value
    */
   kdictionary_int64_t getInt64 (const std::string &strKey, kdictionary_int64_t nDefaultValue = 0) const;

   /**
    * Set value as a 64-bit integer
    *
    * \param strKey key
    * \param nValue value
    */
   void setInt64 (const std::string &strKey, kdictionary_int64_t nValue);

   /**
    * Get value as a float
    *
    * \param strKey key
    * \param fDefautValue default value
    *
    * \return value
    */
   float getFloat (const std::string &strKey, float fDefautValue = 0.0f) const;

   /**
    * Set value as a float
    *
    * \param strKey key
    * \param fValue value
    */
   void setFloat (const std::string &strKey, float fValue);

   /**
    * Get value as a float
    *
    * \param strKey key
    * \param fDefautValue default value
    *
    * \return value
    */
   double getDouble (const std::string &strKey, double fDefautValue = 0.0) const;

   /**
    * Set value as a float
    *
    * \param strKey key
    * \param fValue value
    */
   void setDouble (const std::string &strKey, double fValue);

   /**
    * Get value as a boolean
    *
    * \param strKey key
    * \param bDefaultValue default value
    *
    * \return value
    */
   bool getBool (const std::string &strKey, bool bDefaultValue = false) const;

   /**
    * Set value as a boolean
    *
    * \param strKey key
    * \param bValue value
    */
   void setBool (const std::string &strKey, bool bValue);

   /**
    * Get value as a C string
    *
    * \param strKey key
    * \param lpszDefaultValue default value
    *
    * \return value
    */
   const char *getString (const std::string &strKey, const char *lpszDefaultValue = NULL);

   /**
    * Set value as a C string
    *
    * \param strKey key
    * \param lpszValue value
    */
   void setString (const std::string &strKey, const char *lpszValue);

   /**
    * Get value as a std::string
    *
    * \param strKey key
    * \param strDefaultValue default value
    *
    * \return value
    */
   std::string getStdString (const std::string &strKey, const std::string &strDefaultValue = "");

   /**
    * Set value as a std::string
    *
    * \param strKey key
    * \param lpszValue value
    */
   void setStdString (const std::string &strKey, const std::string &strValue);

   /**
    * Get value as a dictionary
    *
    * \param lpszKey key
    * \param lpDefaultValue default value
    *
    * \return value
    */
   KDictionary *getDictionary (const std::string &strKey, KDictionary *lpDefaultValue = NULL);

   /**
    * Set value as a C string
    *
    * \param lpszKey key
    * \param lpszValue value
    */
   void setDictionary (const std::string &strKey, const KDictionary *lpValue);

   /**
    * Get the first value enumerated by this dictionary. The values are in no specific order
    *
    * \return first value, or NULL if the dictionary is empty
    */
   KDictionaryValue *getFirstValue (void);

private:
   /**
    * Get value in dictionary, and create it first if it doesn't exist
    *
    * \param lpszKey key
    *
    * \return value
    */
   KDictionaryValue *getOrCreateValue (const char *lpszKey);

   /**
    * Duplicate string
    *
    * \param lpszString source string
    *
    * \return duplicated string
    */
   static const char *copyString (const char *lpszString);

   /** Hashtable of values for this dictionary, by key */
   KHashTable _hashValues;

   /** List of managed values */
   KList<KDictionaryValue> _lValues;
};

#endif   /* _KANJI_KDICTIONARY_H */

