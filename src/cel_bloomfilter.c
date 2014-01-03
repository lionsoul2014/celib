/**
 * cel bloom filter implemented functions source file.
 * 	@package	src/cel_bloomfilter.c
 *
 * @author	chenxin <chenxin619315@gmail.com>
 */
#include "cel_bloomfilter.h"
#include "cel_stdio.h"
#include "cel_hash.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>

/**
 * create a new bloom filter with a specified length.
 *
 * @param	length
 * @param	hfuncs
 * @return	cel_bloomfilter_t
 */
CEL_API cel_bloomfilter_t new_cel_bloomfilter( uint_t length, uint_t hfuncs )
{
    uint_t __bytes;
    cel_bloomfilter_t bloom = ( cel_bloomfilter_t ) \
			      cel_malloc( sizeof( cel_bloomfilter_entry ) );
    if ( bloom == NULL ) return NULL;

    //make the space for the str
    __bytes = ( length + CHAR_BIT - 1 ) / CHAR_BIT;
    if ( (bloom->str = (char *) \
		cel_calloc(__bytes, sizeof(char))) == NULL) {
	cel_free(bloom);
	return NULL;
    }

    //hash functions buckets allocations
    if ( (bloom->hfuncs = (cel_hash_fn_t *) \
		cel_calloc(hfuncs, sizeof(cel_hash_fn_t)) ) == NULL ) {
	cel_free(bloom->str);
	cel_free(bloom);
	return NULL;
    }

    //initialize
    bloom->length = length;
    bloom->size = 0;
    bloom->hlength = hfuncs;
    bloom->hsize = 0;
    memset(bloom->str, 0x00, __bytes);

    return bloom;
}


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
 * @return	cel_bloomfilter_t
 */
CEL_API cel_bloomfilter_t default_cel_bloomfilter( uint_t length )
{
    cel_bloomfilter_t bloom = new_cel_bloomfilter(length, 8);
    if ( bloom == NULL ) return NULL; 

    //Load the hash functions
    //most of the hash functions is simple and fast, but efficient.

    cel_bloomfilter_add_func(bloom, cel_bkdr_hash);
    cel_bloomfilter_add_func(bloom, cel_fnv1a_hash);
    cel_bloomfilter_add_func(bloom, cel_ap_hash);
    cel_bloomfilter_add_func(bloom, cel_djp_hash);
    cel_bloomfilter_add_func(bloom, cel_js_hash);
    cel_bloomfilter_add_func(bloom, cel_sdms_hash);
    cel_bloomfilter_add_func(bloom, cel_rs_hash);
    cel_bloomfilter_add_func(bloom, cel_dek_hash);

    return bloom;
}


//free the specified bloom filter.
CEL_API int free_cel_bloomfilter( cel_bloomfilter_t bloom )
{
    if ( bloom == NULL ) return 0;
    cel_free(bloom->hfuncs);
    cel_free(bloom->str);
    cel_free(bloom);
    return 1;
}

/**
 * add a new hash function for the bloom filter.
 *
 * @param	bloom
 * @param	func
 * @return	int
 */
CEL_API int cel_bloomfilter_add_func( 
	cel_bloomfilter_t bloom,
	cel_hash_fn_t func  )
{
    if ( bloom == NULL ) return 0;
    if ( bloom->hsize < bloom->hlength ) 
	bloom->hfuncs[bloom->hsize++] = func;
    return 1;
}

/**
 * add a new string to the specified bloom filter.
 *
 * @param	bloom
 * @param	str
 * @return	int
 */
CEL_API int cel_bloomfilter_add( cel_bloomfilter_t bloom, char * str )
{
    register uint_t i;
    if ( bloom == NULL ) return 0;
    for ( i = 0; i < bloom->hsize; i++ )
	CEL_BIT_OPEN(bloom->str, bloom->hfuncs[i](str) % bloom->length);
    bloom->size++;
    return 1;
}

/**
 * check the specified string is in the specified bloom filter.
 *
 * @param	bloom
 * @param	str
 * @return	int (1 for true and 0 for false)
 */
CEL_API int cel_bloomfilter_exists( cel_bloomfilter_t bloom, char * str )
{
    register uint_t i;
    if ( bloom == NULL ) return 0; 
    for ( i = 0; i < bloom->hsize; i++ ) {
	if ( CEL_BIT_FETCH(bloom->str, 
		    bloom->hfuncs[i](str) % bloom->length) == 0 )
	    return 0;
    }
    return 1;
}

