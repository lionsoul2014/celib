/**
 * cel memory operation module implements functions
 *     source file.
 * @package src/cel_stdio.c
 *
 * @author chenxin <chenxin619315@gmail.com>
 */

#include "cel_stdio.h"
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <limits.h>

//swap the content between a and b for specified bytes.
CEL_API void cel_mem_swap( void * a, void * b, int size )
{
    uchar_t * __a = a;
    uchar_t * __b = b;
    uchar_t temp;

    //swap the content.
    //do {
    //    temp = *__a;    
    //    *__a++ = *__b;
    //    *__b++ = temp;
    //} while ( --size > 0);
    while ( size-- > 0 ) 
    {
        temp = *__a;    
        *__a++ = *__b;
        *__b++ = temp;
    }
}

/**
 * copy specified bytes from source to the destination.
 *
 * @param    _src    - copy source.
 * @param    _dst    - copy destination.
 * @param    size    - bytes to copy
 */
CEL_API void cel_mem_copy( void * _src, void * _dst, int size )
{
    uchar_t * __src = _src;
    uchar_t * __dst = _dst;

    //do {
    //    *__dst++ = *__src++;
    //} while ( --size > 0 );
    while ( size-- > 0 )
    {
        //printf("size=%d\n", size);
        *__dst++ = *__src++;
    }
}

/**
 * print the content with forms of bits.
 *
 * @param    ptr
 * @param    __bytes    - bytes of the ptr.
 */
CEL_API void cel_mem_bits( const char * name, const void * ptr, uint_t __bytes )
{
    uint_t i;
    uchar_t * str = ( uchar_t * ) ptr;
    uint_t length = __bytes * CHAR_BIT;

    if ( name != NULL ) printf("%s", name);
    for ( i = 0; i < length; )
    {
        if ( CEL_BIT_FETCH(str, i) != 0 )
            printf("1");
        else
            printf("0");
        //print a space for every 8 bits
        if ( (++i % 8) == 0 ) printf(" ");
    }
    printf("\n");
}

/**
 * store specified bits from the specified index to
 *     the specified address.
 *
 * @param    src
 * @param    start    - the start index.
 * @param    bits    - number of bits to store.
 * @param    dst    - data to store.
 * @param    idx    - the start index.
 */
CEL_API void cel_mem_store( 
    const void * _src,
    uint_t start,
    uint_t bits,
    void * _dst, 
    uint_t idx )
{
    uchar_t * __src = ( uchar_t * ) _src;
    uchar_t * __dst = ( uchar_t * ) _dst;

    register uint_t i;

    //cel_mem_bits("_src: ", _src, 8);
    for ( i = 0; i < bits; i++ )
    {
        //printf("%2d: %d\n", start + i, CEL_BIT_FETCH(__src, start + i) != 0);
        if ( CEL_BIT_FETCH(__src, start + i) != 0 )
            CEL_BIT_OPEN( __dst, idx + i );
        else
            CEL_BIT_SHUT( __dst, idx + i );
    }    

    //uint32_t * __dst = ( uint32_t * ) _dst;
    //uint32_t * __src = ( uint32_t * ) _src;

    //uint32_t seed, v;
    //uint_t b = idx % 32;
    //idx = idx / 32;

    ////Handle the first byte
    //if ( b != 0 ) {
    //    //Take b bits from the source.
    //    seed = (uint32_t) ( pow(2, 32 - b) - 1 );
    //    v = ( (*__src) & seed );

    //    //Copy the source bits to the desitination.
    //    seed = (uint32_t) (pow(2, b) - 1);
    //    __dst[idx] = ( (__dst[idx] & seed) );
    //}

    //register uint_t i = 0;
    //idx = idx / 32;

    //do {
    //    //Get the souce bits.
    //    b = (bits > 32) ? 32 : bits;
    //    seed = (uint32_t) pow(2, b) - 1;
    //    v = ( (* ( __src + i )) & seed );

    //    //Copy the source bits to the destination.
    //    __dst[idx+i] = (__dst[idx+i] & (0x7FFFFFFE << (b - 1))) + v;  
    //    bits -= 32; i++;
    //} while ( bits > 0);
}


/* get the bytes of a specified file.
 *
 * @return size of file or -1 when fail.
 * */
CEL_API llong_t cel_filesize( char * file )
{
    FILE * fp;
    llong_t length = 0;

    if ( (fp = fopen( file, "rb" )) == NULL )
       return -1;
    else 
    {
        fseek( fp, 0L, SEEK_END );
        length = ftell( fp );
        fclose( fp );
    }
    
    //printf("cel_filesize#done");
    return length;
}
