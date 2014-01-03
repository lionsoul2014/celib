/**
 * common api define of celib .
 *
 * @author chenxin<chenxin619315@gmail.com>
 */
#ifndef _cel_api_h
#define _cel_api_h

#include <stdio.h>
#include <stdlib.h>

//version of the celib
#define CEL_VERSION 1

#ifdef _WIN32
#	define CEL_API extern __declspec(dllexport)
#	define STA_INLINE static
#else
#	define CEL_API extern
#	define STA_INLINE static inline
#endif


//memory allocation error
#define __ALLOCATION_ERROR__	do {						\
    printf("Unable to do the memory allocation, program will now exit\n");	\
    exit(1);								\
} while (0);				

/*
 * memory allocation macro define .
 * 	change it to the the serial of function you may to
 * want to use, like the api of your memory pool .
 */
#define cel_calloc( _bytes, _blocks ) calloc( _bytes, _blocks )
#define cel_malloc( _bytes ) malloc( _bytes )
#define cel_free( _ptr ) free( _ptr )

//short data type
typedef long long int llong_t;
typedef unsigned long long int ullong_t;

typedef long int long_t;
typedef unsigned long int ulong_t;

typedef unsigned int uint_t;
typedef unsigned short ushort_t;
typedef unsigned char uchar_t;
typedef char * cstring;

//generics compare function
typedef int ( * cel_compare_fn_t ) ( void *, void * );
typedef void ( * cel_release_callback_fn_t ) ( void * );

#endif	/*end ifndef*/
