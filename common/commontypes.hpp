/// @file
/// @brief general definitions

#pragma once

#ifdef _WIN32
	#define EXPORT [[dllexport]]
#else
	#define EXPORT [[visibility("default")]]
#endif

#define C_EXPORT extern "C" EXPORT

#if !defined BYTE_DEFINED
typedef unsigned char byte;
#define BYTE_DEFINED 1
#endif

#undef true
#undef false

#ifdef __cplusplus
typedef int qboolean;
#else
typedef enum { false,
	           true } qboolean;
#endif

#define BIT(n) 1<<(n)