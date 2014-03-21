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
 * @return	cel_bitmap_t *
 */
CEL_API cel_bitmap_t *new_cel_bitmap( int opacity )
{
    cel_bitmap_t *map = ( cel_bitmap_t * ) cel_malloc( sizeof( cel_bitmap_t ) );
    if ( map == NULL ) 
    {
        CEL_ALLOCATE_ERROR("new_cel_bitmap", sizeof(cel_bitmap_t));
    }

    if ( cel_bitmap_init(map, opacity) == 0 )
    {
        cel_free(map);
        CEL_ALLOCATE_ERROR("cel_bitmap_create", opacity);
    }

    return map;
}

//free the specified cel_bitmap_t
CEL_API void free_cel_bitmap( cel_bitmap_t **map )
{
    if ( map == NULL ) return;
    if ( *map != NULL ) 
    {
	   cel_bitmap_destroy(*map);
	   cel_free(*map);
       *map = NULL;
    }
}

/*
 * initialize the specified cel_bitmap_t
 *
 * @param   uint_t opacity of the bitmap
 * @return  int (1 for success and 0 for failed)
 */
CEL_API int cel_bitmap_init( cel_bitmap_t *map, int opacity )
{
    uint_t bytes = (opacity + CHAR_BIT - 1) / CHAR_BIT;
    map->bit = (char *) cel_calloc( sizeof(char), bytes );
    if ( map->bit == NULL ) return 0;

    map->length = bytes * CHAR_BIT;
    map->size = 0;
    //reset the memeory to 0x00
    memset( map->bit, 0x00, bytes );

    return 1;
}

/*
 * destroy the specified cel_bitmap_t
 *
 * @param   cel_array_t
 * @return  int 1 for success and 0 for failed
 */
CEL_API int cel_bitmap_destroy( cel_bitmap_t *map )
{
    if ( map != NULL )
    {
        cel_free(map->bit);
        map->bit = NULL;
    }

    return 1;
}

/**
 * set the specified bit to 1.
 *
 * @param	idx
 */
CEL_API void cel_bitmap_set( cel_bitmap_t *map, uint_t idx )
{
    if ( map != NULL && idx < map->length ) 
    {
	   map->bit[idx/CHAR_BIT] |= (0x01<<(idx%CHAR_BIT));
	   map->size++;
    }
}

//set the specified bit to 0
CEL_API void cel_bitmap_remove( cel_bitmap_t *map, uint_t idx )
{
    if ( map != NULL && idx < map->length ) 
    {
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
CEL_API int cel_bitmap_get( cel_bitmap_t *map, uint_t idx )
{
    if ( map == NULL ) return 0;
    if ( idx >= map->length ) return 0;
    if ( ( map->bit[idx/CHAR_BIT] & (0x01<<(idx%CHAR_BIT)) ) != 0 )
	   return 1;
    return 0;
}
