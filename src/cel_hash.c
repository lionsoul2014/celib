/**
 * cel hash function implemented source file.
 *     @package src/cel_hash.c
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#include "cel_hash.h"
#include <string.h>
#include <stdint.h>

//get 32 bits
#undef get32bits
#if ( defined(__GNUC__) || defined(__i386__) || defined(__WATCOMC__) \
    || defined(_MSC_VER) || defined(__BORLANDC__) || defined(__TURBOC__) )
#define get32bits(d) (*((const uint32_t *) (d)))
#endif

#if ! defined(get32bits)
//type of d should be char *
#define get32bits(d) ( (uint32_t)(d)[0]     \
    + ((uint32_t)(d)[1] << 8)        \
    + ((uint32_t)(d)[2] << 16)        \
    + ((uint32_t)(d)[3] << 24) )
#endif

//get 16 bits
#undef get16bits
#if ( defined(__GNUC__) || defined(__i386__) || defined(__WATCOMC__) \
    || defined(_MSC_VER) || defined(__BORLANDC__) || defined(__TURBOC__) )
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if ! defined(get16bits)
//type of d should be char *
#define get16bits(d) ( (uint16_t)(d)[0] + ((uint16_t)(d)[1] << 8) )
#endif

//simple BKDR hash algorithm
CEL_API uint32_t cel_bkdr_hash( const char * str )
{
    register uint32_t h = 0;
    uint32_t seed = 131;    //31 131 1313 13131

    while ( *str != '\0' ) {
        h = h * seed + ( *str++ ); 
    }

    return (h & 0x7FFFFFFF);
}

/**
 * FNV hash algorithm wildly use in Unix system.
 */
CEL_API uint32_t cel_fnv_hash( const char *str )
{
    register uint32_t h = 2166136261UL;
    uint32_t prime = 16777619;

    while ( *str != '\0' ) {
        h *= prime;
        h ^= *str++;
    }

    return (h & 0x7FFFFFFF);
}

/**
 * FNV-1a hash algorithm. wildly use in Unix system.
 * the only difference between FNV-1 is the order of xor and multipy.
 * @link http://www.isthe.com/chongo/tech/comp/fnv/
 */
CEL_API uint32_t cel_fnv1a_hash( const char *str )
{
    register uint32_t h = 2166136261UL;
    uint32_t prime = 16777619;

    while ( *str != '\0' ) {
        h ^= (*str++);
        h *= prime;
    }

    return (h & 0x7FFFFFFF);
}

/**
 * AP hash algorithm. 
 * Design by Arash Partow.
 */
CEL_API uint32_t cel_ap_hash( const char *str )
{
    register uint32_t i = 0, h = 0;

    while ( *str != '\0' ) {
        if ( (i++ & 1) == 0) {
            h ^= ( (h << 7) ^ (*str++) ^ (h << 3) );
        } else {
            h ^= ( ~( (h << 11) ^ (*str++) ^ (h << 5) ) );
        }
    }

    return (h & 0x7FFFFFFF);
}

/**
 * DJP hash algorithm.
 * invented by doctor Daniel J. Bernstein.
 */
CEL_API uint32_t cel_djp_hash( const char *str )
{
    register uint32_t h = 5381;

    while ( *str != '\0' ) {
        h += (h << 5) + (*str++);
    }

    return ( h & 0x7FFFFFFF );
}

/**
 * DJP hash algorithm 2.
 * invented by doctor Daniel J. Bernstein.
 */
CEL_API uint32_t cel_djp2_hash( const char *str )
{
    register uint32_t h = 5381;

    while ( *str != '\0' ) {
        h = h * 33 ^ (*str++);
    }

    return (h & 0x7FFFFFFF);
}

/**
 * JS hash algorithm.
 * invented by Justin Sobel.
 */
CEL_API uint32_t cel_js_hash( const char *str )
{
    register uint32_t h = 0;

    while ( *str != '\0' ) {
        h ^= ( (h << 5) + (*str++) + (h << 2) );
    }

    return (h & 0x7FFFFFFF);
}

/**
 * SDMS hash algorithm.
 * came from open source simple database engin SDMS.
 */
CEL_API uint32_t cel_sdms_hash( const char *str )
{
    register uint32_t h = 0;

    while ( *str != '\0' ) {
        h = 65599 * h + (*str++);
    }

    return (h & 0x7FFFFFFF);
}

/**
 * RS hash algoritm.
 * came from book [Algorithm in c] Written by Robert Sedgwicks.
 */
CEL_API uint32_t cel_rs_hash( const char *str )
{
    register uint32_t h = 0;
    register uint32_t magic = 63689;

    while ( *str != '\0' ) {
        h = h * magic + (*str++);
        magic *= 378551;
    }

    return (h & 0x7FFFFFFF);
}

/**
 * DEK hash algorithm.
 * came from [Art of Computer programing Volumes 3] written by Donald E.knuth.
 */
CEL_API uint32_t cel_dek_hash( const char *str )
{
    register uint32_t h = 0;

    while ( *str != '\0' ) {
       h = ( (h << 5) ^ (h >> 27) ^ (*str++) );
    }

    return (h & 0x7FFFFFFF);
}

/**
 * ELF hash algorithm.
 * executable and linking Format.
 */
CEL_API uint32_t cel_elf_hash( const char *str )
{
    register uint32_t h = 0;
    register uint32_t x = 0;

    while ( *str != '\0' ) {
        h = (h << 24) + (*str++);
        if ( (x = h & 0xF0000000L) != 0 ) {
            h ^= (h >> 24);
            h &= ~x;
        }
    }

    return (h & 0x7FFFFFFF);
}

#define cel_bj_mix( a, b, c ) \
{\
    a -= b; a -= c; a ^= (c>>13);    \
    b -= c; b -= a; b ^= (a<<8);    \
    c -= a; c -= b; c ^= (b>>13);    \
    a -= b; a -= c; a ^= (c>>12);    \
    b -= c; b -= a; b ^= (a<<16);    \
    c -= a; c -= b; c ^= (b>>5);    \
    a -= b; a -= c; a ^= (c>>3);     \
    b -= c; b -= a; b ^= (a<<10);    \
    c -= a; c -= b; c ^= (b>>15);    \
}

/**
 * Bob Jenkins hash algorithm.
 * @see http://burtleburtle.net/bob/hash/evahash.html.
 */
CEL_API uint32_t cel_bobJenkins_hash32( 
    const void *str,
    uint_t length,          /*the length of the key in bytes*/
    uint32_t initval  )     /*the previous hash, or an arbitrary value*/
{
    register uint32_t a,b,c;
    register uint32_t len;    /*how many bytes still need to mixing*/
    uchar_t * k = (uchar_t *) str;

    //Set up internal state.
    len = length;
    a = b = 0x9e3779b9;     /*Golden ratio: an arbitrary value.*/
    c = initval;            /*variable initialization of internal state*/
   

    //handle most of the bytes of the str
    while ( len >= 12 ) {
        a += get32bits(k);
        b += get32bits(k + 4);
        c += get32bits(k + 8);
        cel_bj_mix(a, b, c);
        k += 12; len -= 12;
    }

    //handle the last 11 bytes
    c += length;
    switch ( len ) {
    /* all the case statements fall through */
    case 11: c+=((uint32_t)k[10]<<24);
    case 10: c+=((uint32_t)k[9]<<16);
    case 9 : c+=((uint32_t)k[8]<<8);
         // the first byte of c is reserved for the length 
    case 8 : b+=((uint32_t)k[7]<<24);
    case 7 : b+=((uint32_t)k[6]<<16);
    case 6 : b+=((uint32_t)k[5]<<8);
    case 5 : b+=k[4];
    case 4 : a+=((uint32_t)k[3]<<24);
    case 3 : a+=((uint32_t)k[2]<<16);
    case 2 : a+=((uint32_t)k[1]<<8);
    case 1 : a+=k[0];
         /* case 0: nothing left to add */
    }
    cel_bj_mix(a,b,c);

    return c;
}

CEL_API uint32_t cel_bobJenkins32_hash( const char *str )
{
    return cel_bobJenkins_hash32(str, strlen(str), 0x00);
}

/**
 * Murmur hash algorithm is a relative fast hash function
 * from http://murmurhash.googlepages.com for platforms
 * with efficient multiuoplication.
 *
 * Generate 32 bit hash from byte array of the given length and seed.
 */
CEL_API uint32_t cel_murmur_hash32( const void *str, uint_t length, uint_t seed )
{
    //The 'm' and 'r' are mixing constants generated offline.
    //They are not really 'magic', they just happen to work well.

    const uint32_t m = 0x5bd1e995;
    const int r = 24;

    uint32_t k;

    //Initialize the hash to a random value.

    uint32_t h = seed & length;

    //Mix 4 bytes at a time into the hash.

    const uchar_t * data = (uchar_t *) str;
    while ( length >= 4 ) {
        k = *(uint32_t *) data;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        length -= 4;
    }

    //Handle the last few bytes of the input array.
    switch ( length ) {
    case 3: h ^= data[2] << 16; break;
    case 2: h ^= data[1] << 8; break;
    case 1: h ^= data[0]; h *= m; break;
    }

    //Do a few final mixes of the hash to ensure the last few
    //bytes are well-incorporated.

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;
    h &= 0x7FFFFFFF;

    return h;
}
