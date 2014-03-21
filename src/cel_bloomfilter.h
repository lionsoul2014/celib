/**
 * cel bloom filter interface header file src/bloomfilter.h
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#ifndef _cel_bloomfilter_h
#define _cel_bloomfilter_h

#include "cel_api.h"
#include <stdlib.h>
#include <stdint.h>

typedef uint32_t ( * cel_hash_fn_t ) ( const char * );

typedef struct {
    uint_t 	length;
    uint_t 	size;
    char 	*str;
    //about the hash functions
    cel_hash_fn_t *hfuncs;
    uint_t hlength;
    uint_t hsize;
} cel_bloomfilter_t;

//quick macro define
#define cel_bloomfilter_length( bloom )		bloom->length
#define cel_bloomfilter_size( bloom ) 		bloom->size
#define cel_bloomfilter_hlength( bloom ) 	bloom->hlength
#define cel_bloomfilter_hsize( bloom ) 		bloom->hsize

/**
 * create a new bloom filter with a specified length.
 *
 * @param	length
 * @param	hfuncs
 * @return	cel_bloomfilter_t *
 */
CEL_API cel_bloomfilter_t *new_cel_bloomfilter( int, int );

//free the specified bloom filter.
CEL_API void free_cel_bloomfilter( cel_bloomfilter_t ** );

/**
 * create a default bloom filter with a specified length.
 * 	also, this will load the hash functions from cel,
 * defined in header file src/cel_hash.h, and 8 hash functions is loaded for default.
 *
 * math: 
 * k = ln2 * m/n
 * statistics
 * m/n		false positive rate
 * 9		0.0145
 * 10		0.00846
 * 11		0.00509
 * 12		0.00314
 * 	.
 * 	.
 * 	.
 * 30		9.01e-06
 * 31		7.16e-06
 * 32		5.73e-06
 *
 * @param	length
 * @return	cel_bloomfilter_t *
 */
CEL_API int cel_bloomfilter_init( cel_bloomfilter_t *, int, int );

/*
 * destroy the specified bloomfilter
 *
 * @param	cel_bloomfilter_t *
 * @return	int 1 for success and 0 for failed
 */
CEL_API int cel_bloomfilter_destroy( cel_bloomfilter_t * );

/**
 * add a new hash function for the bloom filter.
 *
 * @param	cel_bloomfilter_t *
 * @param	cel_hash_fn_t
 * @return	int
 */
CEL_API int cel_bloomfilter_add_func( cel_bloomfilter_t *, cel_hash_fn_t  );

/**
 * add a new string to the specified bloom filter.
 *
 * @param	cel_bloomfilter_t *
 * @param	char *
 * @return	int
 */
CEL_API int cel_bloomfilter_add( cel_bloomfilter_t *, char * );

/**
 * check the specified string is in the specified bloom filter.
 *
 * @param	cel_bloomfilter_t *
 * @param	char *
 * @return	int (1 for true and 0 for false)
 */
CEL_API int cel_bloomfilter_exists( cel_bloomfilter_t *, char * );

#endif	/*end ifndef*/

