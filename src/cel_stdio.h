/**
 * cel memory module interface header file.
 * 	@package src/cel_stdio.h.
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#ifndef _cel_stdio_h
#define _cel_stdio_h

#include "cel_api.h"
#include <limits.h>

//bit macro functions
#define CEL_BIT_OPEN(str, n) 	\
    (str[(n)/CHAR_BIT] |= (0x01<<((n)%CHAR_BIT)))

#define CEL_BIT_SHUT(str, n) 	\
    (str[(n)/CHAR_BIT] &= (~(0x01<<((n)%CHAR_BIT))))

#define CEL_BIT_FETCH(str, n)	\
    (str[(n)/CHAR_BIT] &  (0x01<<((n)%CHAR_BIT)))

//swap the content between a and b for specified bytes.
CEL_API void cel_mem_swap( void *, void *, int );

/**
 * copy specified bytes from source to the destination.
 *
 * @param	_src	- copy source.
 * @param	_dst	- copy destination.
 * @param	size	- bytes to copy
 */
CEL_API void cel_mem_copy( void *, void *, int );

/**
 * print the content with forms of bits.
 *
 * @param	name
 * @param	ptr
 * @param	length	- bytes of the ptr.
 */
CEL_API void cel_mem_bits( const char *, const void *, uint_t );

/**
 * store specified bits from the specified index to
 * 	the specified address.
 *
 * @param	src
 * @param	start	- the start index.
 * @param	bits	- number of bits to store.
 * 					include the bit at start index.
 * @param	dst		- data to store.
 * @param	idx		- the start index.
 */
CEL_API void cel_mem_store( const void *, uint_t, uint_t, void *, uint_t );

/**
 * copy specified bits from a specified index of specified source.
 * 	to a given buffer.
 *
 * @param	src
 * @param	start	- the start index.
 * @param	bits	- number of bits to fetch.
 * @param	dst		- data buffer.
 * @param	idx		- the start index.
 */
//CEL_API void cel_mem_fetch( const void *, uint_t, uint_t, void *, uint_t );
#define cel_mem_fetch(src, start, bits, dst, idx)	\
    cel_mem_store(src, start, bits, dst, idx)


/*{{{ basic data type encode/decode interface.
 * */

/* get the bytes of a specified file.
 *
 * @return size of file or -1 when fail.
 * */
CEL_API llong_t cel_filesize( char * );
#endif	/*end ifndef*/
