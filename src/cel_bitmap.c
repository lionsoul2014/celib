/**
 * cel bitmap interface implemented source file src/cel_bitmap.c
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#include "cel_bitmap.h"
#include <string.h>
#include <limits.h>

/**
 * create a new bitmap with a specified opacity.
 *
 * @param	opacity
 * @return	cel_bitmap_t
 */
CEL_API cel_bitmap_t new_cel_bitmap( uint_t opacity )
{
    uint_t bytes;
    cel_bitmap_t map = ( cel_bitmap_t ) cel_malloc( sizeof( cel_bitmap_entry ) );
    if ( map == NULL ) return NULL;	

    bytes = (opacity + CHAR_BIT - 1) / CHAR_BIT;
    map->bit = (char *) cel_calloc( sizeof(char), bytes );
    if ( map->bit == NULL ) {
	cel_free( map->bit );
	return NULL;
    }

    map->length = bytes * CHAR_BIT;
    map->size = 0;
    //reset the memeory to 0x00
    memset( map->bit, 0x00, bytes );

    return map;
}

//free the specified cel_bitmap_t
CEL_API void free_cel_bitmap( cel_bitmap_t map )
{
    if ( map != NULL ) {
	cel_free(map->bit);
	cel_free(map);
    }
}

/**
 * set the specified bit to 1.
 *
 * @param	idx
 */
CEL_API void cel_bitmap_set( cel_bitmap_t map, uint_t idx )
{
    if ( map != NULL && idx < map->length ) {
	map->bit[idx/CHAR_BIT] |= (0x01<<(idx%CHAR_BIT));
	map->size++;
    }
}

//set the specified bit to 0
CEL_API void cel_bitmap_remove( cel_bitmap_t map, uint_t idx )
{
    if ( map != NULL && idx < map->length ) {
	map->bit[idx/CHAR_BIT] &= (~(0x01<<(idx%CHAR_BIT)));
	map->size--;
    }
}

/**
 * get the specified bit.
 *
 * @param	idx
 * @return	1 or 0
 */
CEL_API int cel_bitmap_get( cel_bitmap_t map, uint_t idx )
{
    if ( map == NULL ) return 0;
    if ( idx >= map->length ) return 0;
    if ( ( map->bit[idx/CHAR_BIT] & (0x01<<(idx%CHAR_BIT)) ) != 0 )
	return 1;
    return 0;
}

