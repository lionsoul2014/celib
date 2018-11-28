/**
 * cel hash function interface .
 *     @package src/cel_hash.h.
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#ifndef _cel_hash_h
#define _cel_hash_h

#include "cel_api.h"
#include <stdint.h>
#include <string.h>

//simple BKDR hash algorithm
CEL_API uint32_t cel_bkdr_hash( const char * );

/**
 * FNV hash algorithm.
 *     wildly use in Unix system.
 */
CEL_API uint32_t cel_fnv_hash( const char * );

/**
 * FNV-1a hash algorithm.
 *     wildly use in Unix system.
 */
CEL_API uint32_t cel_fnv1a_hash( const char * );

/**
 * AP hash algorithm.
 *     design by Arash Partow.
 */
CEL_API uint32_t cel_ap_hash( const char * );

/**
 * DJP hash algorithm.
 *     invented by doctor Daniel J. Bernstein.
 */
CEL_API uint32_t cel_djp_hash( const char * );

/**
 * DJP hash algorithm 2.
 *     invented by doctor Daniel J. Bernstein.
 */
CEL_API uint32_t cel_djp2_hash( const char * );

/**
 * JS hash algorithm.
 *     invented by Justin Sobel.
 */
CEL_API uint32_t cel_js_hash( const char * );

/**
 * SDMS hash algorithm.
 *     came from open source simple database engin SDMS.
 */
CEL_API uint32_t cel_sdms_hash( const char * );

/**
 * RS hash algoritm.
 *     came from book [Algorithm in c] Written by Robert Sedgwicks.
 */
CEL_API uint32_t cel_rs_hash( const char * );

/**
 * DEK hash algorithm.
 *     came from [Art of Computer programing Volumes 3]
 *     written by Donald E.knuth.
 */
CEL_API uint32_t cel_dek_hash( const char * );

/**
 * ELF hash algorithm.
 *     executable and linking Format.
 */
CEL_API uint32_t cel_elf_hash( const char * );

/**
 * Bob Jenkins hash algorithm.
 *     @see http://burtleburtle.net/bob/hash/evahash.html.
 */
CEL_API uint32_t cel_bobJenkins_hash32( const void *, 
    uint_t length, uint32_t initval  );

CEL_API uint32_t cel_bobJenkins32_hash( const char * );


/**
 * Murmur hash algorithm is a relative fast hash function
 *     from http://murmurhash.googlepages.com for platforms
 * with efficient multiuoplication.
 *
 * Generate 32 bit hash from byte array of the given length and seed.
 */
CEL_API uint32_t cel_murmur_hash32( const void *, uint_t length, uint_t seed );

//CEL_API uint32_t cel_murmur32_hash( const char * );
#define cel_murmur32_hash(str) cel_murmur_hash32(str, strlen(str), 0x9747b28c)

/**
 * CRC32 hash algorithm.
 *     cyclic Redundancy check.
 */
CEL_API uint32_t cel_crc32( uint32_t, const void *, uint_t );

//CEL_API uint32_t cel_crc32_hash( const char * );
#define cel_crc32_hash(str) cel_crc32(-1, str, strlen(str))

/**
 * generate a crc32 code for a specified file.
 *
 * @param    file
 * @return    uint32_t
 */
CEL_API void cel_crc32_file( const char *, uint32_t * );


/**
 * MD5 hash algorithm.
 *     Message digest Algorithm 5.
 */
// struct cel_md5_entry {
//     ulong_t state[4];    /*state (ABCD)*/
//     ulong_t count[2];    /*number of bits, modulo 2^64*/
//     uchar_t buffer[64];    /*input buffer*/
// };
// typedef struct cel_md5_entry cel_md5_t;

/* Define the state of the MD5 Algorithm. */
typedef struct cel_md5_entry {
    uint_t count[2];	/* message length in bits, lsw first */
    uint_t abcd[4];		/* digest buffer */
    uchar_t buf[64];	/* accumulate block */
} cel_md5_t;

CEL_API void cel_md5_init( cel_md5_t * );
CEL_API void cel_md5_update( cel_md5_t *, const uchar_t *, int );
CEL_API void cel_md5_final( cel_md5_t *, uchar_t digest[16] );

CEL_API void cel_md5_string( const cstring, uchar_t digest[16] );
CEL_API int cel_md5_file( const cstring, uchar_t digest[16] );
CEL_API void cel_md5_print(uchar_t digest[16], char buffer[32]);

#endif    /*end ifndef*/

