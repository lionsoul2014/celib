/**
 * cel bitmap interface header source file.
 *     package:  src/cel_bitmap.h
 *
 * @author chenxin <chenxin619315@gmail.com>
 * @date 2013-08-07
 */
#ifndef _cel_bitmap_h
#define _cel_bitmap_h

#include "cel_api.h"

typedef struct {
    uint_t length;
    uint_t size;
    char * bit;
} cel_bitmap_t;

#define cel_bitmap_length( map ) map->length
#define cel_bitmap_size( map ) map->size

/**
 * create a new bitmap with a specified opacity.
 *
 * @param    uint_t
 * @return    cel_bitmap_t *
 */
CEL_API cel_bitmap_t *new_cel_bitmap( int );

//free the specified cel_bitmap_t
CEL_API void free_cel_bitmap( cel_bitmap_t ** );

/*
 * initialize the specified cel_bitmap_t
 *
 * @param    uint_t opacity of the bitmap
 * @return    int (1 for success and 0 for failed)
 */
CEL_API int cel_bitmap_init( cel_bitmap_t *, int );

/*
 * destroy the specified cel_bitmap_t
 *
 * @param    cel_array_t
 * @return    int 1 for success and 0 for failed
 */
CEL_API int cel_bitmap_destroy( cel_bitmap_t * );

/**
 * set the specified bit to 1.
 *
 * @param    uint_t
 */
CEL_API void cel_bitmap_set( cel_bitmap_t *, uint_t );

/**
 * set the specified bit to 0
 * 
 * @param    uint_t
 */
CEL_API void cel_bitmap_remove( cel_bitmap_t *, uint_t );

/**
 * get the specified bit.
 *
 * @param    uint_t
 * @return    1 or 0
 */
CEL_API int cel_bitmap_get( cel_bitmap_t *, uint_t );

#endif
