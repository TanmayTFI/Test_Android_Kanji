/**
 * Kanji game engine - base64 encoder and decoder, definitions
 *
 * (c) 2010
 *
 * \file KBase64.h
 *
 * Based on the libb64 project, in the public domain. http://sourceforge.net/projects/libb64
 */

#ifndef  _KANJI_KBASE64_H
#define  _KANJI_KBASE64_H

/* Include definitions */
#include "KTypes.h"
#include "KObjectBase.h"

typedef enum {
	KBase64EncStepA, KBase64EncStepB, KBase64EncStepC
} KBase64EncodeStep;

typedef struct {
	KBase64EncodeStep step;
	char result;
	int stepcount;
} KBase64EncodeState;

typedef enum {
	KBase64DecStepA, KBase64DecStepB, KBase64DecStepC, KBase64DecStepD
} KBase64DecodeStep;

typedef struct {
	KBase64DecodeStep step;
	char plainchar;
} KBase64DecodeState;

/**
 * Class for decoding and encoding base64 data
 *
 * The KBezier class provides methods for decoding and encoding base64 data.
 */

class KBase64 : public KObjectBase {
public:
   /**
    * Begin decoding base64 data
    */
   void beginDecode (void);

   /**
    * Decode base64 data block
    *
    * \param lpBase64InData base64 encoded data block
    * \param nBase64InSize base64 encoded data block size
    * \param lpDecodeOutBuffer output buffer for decoded data (NULL to only get the decoded size)
    *
    * \return decoded size
    */
   size_t decodeBlock (const char *lpBase64InData, const size_t nBase64InSize, char *lpDecodeOutBuffer);

   /**
    * End decoding base64 data
    *
    * \param lpDecodeOutBuffer output buffer for decoded data (NULL to only get the decoded size)
    *
    * \return decoded size
    */
   size_t endDecode (char *lpDecodeBufferOut);

   /**
    * Begin encoding base64 data
    */
   void beginEncode (void);

   /**
    * Encode base64 data block
    *
    * \param lpDecodedInData decoded data block
    * \param nDecodedInSize decoded data block size
    * \param lpBase64OutBuffer output buffer for base64 encoded data (NULL to only get the encoded size)
    *
    * \return encoded size
    */
   size_t encodeBlock (const char *lpDecodedInData, const size_t nDecodedInSize, char *lpBase64OutBuffer);

   /**
    * End encoding base64 data
    *
    * \param lpBase64OutBuffer output buffer for base64 encoded data (NULL to only get the encoded size)
    *
    * \return encoded size
    */
   size_t endEncode (char *lpBase64OutBuffer);

private:
   /**
    * Decode base64 value
    *
    * \param value_in value to decode
    *
    * \return decoded value
    */
   static int decodeValue (signed char value_in);

   /**
    * Encode base value
    *
    * \param value_in value to encode
    *
    * \return encoded value
    */
   static char encodeValue (signed char value_in);

   /** Decoding state */
   KBase64DecodeState m_decoder;

   /** Encoding state */
   KBase64EncodeState m_encoder;
};

#endif   /* _KANJI_KBASE64_H */

