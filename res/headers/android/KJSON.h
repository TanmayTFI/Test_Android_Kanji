/**
 * Kanji game engine - JSON parser, definitions
 *
 * (c) 2010
 *
 * \file KJSON.h
 *
 * Based on the KJSON parser
 */

#ifndef  _KANJI_JSON_H
#define  _KANJI_JSON_H

/* Include definitions */
#include "KObjectBase.h"

/*
  Copyright (c) 2009 Dave Gamble
 
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
 
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
 
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

/* KJSON Types: */
#define KJSON_False 0
#define KJSON_True 1
#define KJSON_NULL 2
#define KJSON_Number 3
#define KJSON_String 4
#define KJSON_Array 5
#define KJSON_Object 6
	
#define KJSON_IsReference 256

typedef struct Hooks {
   void *(*malloc_fn)(size_t sz);
   void (*free_fn)(void *ptr);
} KJSON_Hooks;

/* Forward declarations */
class KJSON;

/* The KJSON class: */
class KJSON {
public:
   /* Supply malloc, realloc and free functions to KJSON */
   static void initHooks(Hooks* hooks);

   /* For analysing failed parses. This returns a pointer to the parse error. You'll probably need to look a few chars back to make sense of it. Defined when Parse() returns 0. 0 when Parse() succeeds. */
   static const char *getErrorPtr(void);

   /* Supply a block of JSON, and this returns a KJSON object you can interrogate. Call Delete when finished. */
   static KJSON *parse(const char *value);
   
   /* parseWithOpts allows you to require (and check) that the JSON is null terminated, and to retrieve the pointer to the final byte parsed. */
   static KJSON *parseWithOpts(const char *value,const char **return_parse_end,int require_null_terminated);

   /* Render a KJSON entity to text for transfer/storage. Free the char* when finished. */
   char *print(void);
   
   /* Render a KJSON entity to text for transfer/storage without any formatting. Free the char* when finished. */
   char *printUnformatted(void);
   
   /** Internal destructor */
   ~KJSON ();

   /* Delete a KJSON object and all its siblings and children */
   static void deleteObject(KJSON *c);

   /* Returns the number of items in an array (or object). */
   int getArraySize(void);
   
   /* Retrieve item number "item" from array "array". Returns NULL if unsuccessful. */
   KJSON *getArrayItem(int item);
   
   /* Get item "string" from object. Case insensitive. */
   KJSON *getObjectItem(const char *string);
	
   /* Accessors for the item name, type and value */
   const char *getItemName();
   long getItemType();
   int getItemValueAsInt();
   float getItemValueAsFloat();
   double getItemValueAsDouble();
   const char *getItemValueAsString ();

   /* These calls create a KJSON item of the appropriate type. */
   static KJSON *createNull(void);
   static KJSON *createTrue(void);
   static KJSON *createFalse(void);
   static KJSON *createBool(int b);
   static KJSON *createNumber(double num);
   static KJSON *createString(const char *string);
   static KJSON *createArray(void);
   static KJSON *createObject(void);

   /* These utilities create an Array of count items. */
   static KJSON *createIntArray(const int *numbers,int count);
   static KJSON *createFloatArray(const float *numbers,int count);
   static KJSON *createDoubleArray(const double *numbers,int count);
   static KJSON *createStringArray(const char **strings,int count);

   /* Append item to the specified array/object. */
   void addItemToArray(KJSON *item);
   void addItemToObject(const char *string,KJSON *item);
   
   /* Append reference to item to the specified array/object. Use this when you want to add an existing KJSON to a new KJSON, but don't want to corrupt your existing KJSON. */
   void addItemReferenceToArray(KJSON *item);
   void	addItemReferenceToObject(const char *string,KJSON *item);

   /* Remove/Detatch items from Arrays/Objects. */
   KJSON *detachItemFromArray(int which);
   void deleteItemFromArray(int which);
   KJSON *detachItemFromObject(const char *string);
   void deleteItemFromObject(const char *string);
	
   /* Update array items. */
   void replaceItemInArray(int which,KJSON *newitem);
   void replaceItemInObject(const char *string,KJSON *newitem);

   /* Duplicate a KJSON item. Duplicate will create a new, identical KJSON item to the one you pass, in new memory that will
      need to be released. With recurse!=0, it will duplicate any children connected to the item.
      The item->next and ->prev pointers are always zero on return from Duplicate. */
   KJSON *duplicate(int recurse);
   
   static void minify(char *json);

   /* Methods for creating things quickly. */

   void addNullToObject(const char *name) {
      addItemToObject(name, createNull());
   }
   void addTrueToObject(const char *name) {
      addItemToObject(name, createTrue());
   }
   void addFalseToObject(const char *name) {
      addItemToObject(name, createFalse());
   }
   void addBoolToObject(const char *name,bool b) {
      addItemToObject(name, createBool(b));
   }
   void addNumberToObject(const char *name,double n) {
      addItemToObject(name, createNumber(n));
   }
   void addStringToObject(const char *name,const char *s) {
      addItemToObject(name, createString(s));
   }

   void setIntValue(int val) {
      m_valueint=val;
      m_valuedouble=(double) val;
   }

private:
   static const char *parse_number(KJSON *item,const char *num);
   static const char *parse_value(KJSON *item,const char *value);
   static char *print_value(KJSON *item,int depth,int fmt);
   static const char *parse_array(KJSON *item,const char *value);
   static char *print_array(KJSON *item,int depth,int fmt);
   static const char *parse_object(KJSON *item,const char *value);
   static char *print_object(KJSON *item,int depth,int fmt);
   static char *print_number(KJSON *item);
   static const char *parse_string(KJSON *item,const char *str);
   static char *print_string(KJSON *item);
   static void suffix_object(KJSON *prev,KJSON *item);
   static KJSON *create_reference(KJSON *item);

   /* Internal constructor. */
   KJSON(void);

	KJSON *m_next,*m_prev;     /* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
	KJSON *m_child;            /* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */

	int m_type;                /* The type of the item, as above. */

	char *m_valuestring;       /* The item's string, if type==String */
	int m_valueint;            /* The item's number, if type==Number */
	double m_valuedouble;      /* The item's number, if type==Number */

	char *m_string;            /* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
};

#endif   /* _KANJI_JSON_H */
