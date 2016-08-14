/**
 * @addtogroup mlsInclude
 * @{
 */

/**
 * @file mlsTypes.h
 * @brief	Common type definitions
 *
 * This file contains common type definition which are used a lot of time in project
 * @author	mlsvnusr001
 */

#ifndef __MTTYPES_H__
#define __MTTYPES_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*-----------------------------------------------------------------------------
 * Types and defines:
 *-----------------------------------------------------------------------------*/
#ifndef False
#define False           0
#endif

#ifndef True
#define True            1
#endif

#ifndef Null
#define Null            ((Void *) 0)
#endif

#undef NULL
#ifndef NULL
#define NULL            (0)
#endif

/*****************************************************************************
 *      Standard Types
 *****************************************************************************/
#define Const const
typedef signed char Int8, *pInt8; /*  8 bit   signed integer */
typedef signed short Int16, *pInt16; /* 16 bit   signed integer */
typedef signed int Int32, *pInt32; /* 32 bit   signed integer */
typedef unsigned char UInt8, *pUInt8; /*  8 bit unsigned integer */
typedef unsigned short UInt16, *pUInt16; /* 16 bit unsigned integer */
typedef unsigned int UInt32, *pUInt32; /* 32 bit unsigned integer */
typedef void Void, *pVoid; /* Void (typeless) */
typedef float Float, *pFloat; /* 32 bit floating point */
typedef double Double, *pDouble; /* 32/64 bit floating point */
typedef unsigned char Bool, *pBool; /* Boolean (True/False) */
typedef char Char, *pChar; /* character, character array ptr */
typedef char *String, **pString; /* Null terminated 8 bit char str, */
/* Null term 8 bit char str ptr */
typedef char const *ConstString;

typedef signed long long int Int64, *pInt64; /* 64-bit integer    */
typedef unsigned long long int UInt64, *pUInt64; /* 64-bit bitmask */

#ifdef __cplusplus
}
#endif

#endif /*__MTTYPES_H__  */

/*@}*/
