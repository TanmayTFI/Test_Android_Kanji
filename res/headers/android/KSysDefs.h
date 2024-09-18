/**
 * Kanji rendering and I/O engine - Android system-specific definitions
 *
 * (c) 2010
 */

#ifndef  _KANJI_KSYSDEFS_H
#define  _KANJI_KSYSDEFS_H

/* Include definitions */
#define NOMINMAX
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <fcntl.h>
#include <wchar.h>
#include <unistd.h>
#include <sys/stat.h>
#include <GLES/gl.h>

/** System is Android */
#define K_ANDROID

/** System doesn't support streaming Oggs */
#define K_DISABLE_OGG_STREAMING

/** System is little-endian (both on the simulator and device) */
#define K_LITTLE_ENDIAN

#if defined (__x86_64__) || defined (__aarch64__) || defined (__mips64)
/** System is 64 bits */
#define K_64BIT
#endif

/** Maximum number of characters in a fully qualified path */
#define K_MAXPATH 260

/** Use GL extensions */
#define GL_GLEXT_PROTOTYPES

/** Portable name for snprintf */
#define K_snprintf snprintf

/** Portable name for vsnprintf */
#define K_vsnprintf vsnprintf

/** Portable name for strcasecmp */
#define K_strcasecmp strcasecmp

/** Portable name for strncasecmp */
#define K_strncasecmp strncasecmp

/** Copy string */
#define KStringCpy(__buffer,__str,__len) do { strncpy (__buffer,__str,__len); __buffer[(__len)-1] = 0; } while (0)

/** Concatenate string */
#define KStringCat(__buffer,__str,__len) do { strlcat (__buffer,__str,__len); __buffer[(__len)-1] = 0; } while (0)

/** Compare string in a case-insensitive fashion */
#define KStringCaseCmp(__buffer,__str,__len) strncasecmp (__buffer,__str,__len)

/** Convert string to lowercase */
#define KStringToLower(__buffer) do { size_t n = 0; while (__buffer[n]) { __buffer[n] = tolower (__buffer[n]); n++; } } while (0)

/** Format string in a buffer with a maximum number of characters, and make sure it is always zero-terminated */
#define KStringFmt(__buffer,__len,__fmt,...) do { if (__len) { snprintf (__buffer,(__len)-1,__fmt,__VA_ARGS__); __buffer[(__len)-1] = 0; } } while (0)

/** String pointer */
typedef char *K_LPCSTR;

/** Const string pointer */
typedef const char *K_LPSTR;

/** Open file in a way that works with unicode paths - use K_fopen() instead of fopen() when dealing with files directly */
#define K_fopen(__path,__mode) fopen(__path,__mode)

extern "C" {
   /** Game's actual entry point */
   extern int kanjiMain (int argc, char **argv);
}

#endif   /* _KANJI_KSYSDEFS_H */
