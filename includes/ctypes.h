#ifndef CTYPES_H
#define CTYPES_H

#include <QString>

#if (defined (_WIN32) || defined (_WIN64))  

typedef int8_t   int8;
typedef int16_t  int16;
typedef int32_t  int32;
typedef int64_t  int64;
typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#elif (defined (LINUX) || defined (__linux__))

typedef int8_t   int8;
typedef int16_t  int16;
typedef int32_t  int32;
typedef int64_t  int64;
typedef u_int8_t  uint8;
typedef u_int16_t uint16;
typedef u_int32_t uint32;
typedef u_int64_t uint64;

#endif


#endif // CTYPES_H
