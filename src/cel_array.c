/**
 * cel array interface implemented file src/cel_array.c
 *
 * @author chenxin<chenxin619315@gmail.com>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cel_array.h"

//default size of the array
#define _cel_array_default_length 16

static void ** create_array_blocks( uint_t );
static void resize_array( cel_array_t *, uint_t );

/**
 * create a new cel array with a default size.
 *
 * @return	cel_array_t
 */
//CEL_API cel_array_t *new_cel_array() {
//    return new_cel_array_opacity( _cel_array_default_length );
//} 

//create a new cel array with a specified size
CEL_API cel_array_t *new_cel_array_opacity( int opacity ) 
{
    cel_array_t *ptr = ( cel_array_t * ) cel_malloc( sizeof( cel_array_t ) );
    if ( ptr == NULL ) 
    {
	   CEL_ALLOCATE_ERROR("new_cel_array_opacity", sizeof(cel_array_t));
    }

    if ( cel_array_create(ptr, opacity) == 0 )
    {
        cel_free(ptr);
        CEL_ALLOCATE_ERROR("cel_array_create", opacity);
    }

    return ptr;
}

//free the specified cel array
CEL_API void free_cel_array( 
    cel_array_t **array, cel_release_callback_fn_t relfunc ) 
{
    if ( array == NULL ) return;
    if ( *array != NULL )
    {
        cel_array_destroy(*array, relfunc); 
        cel_free( *array );
        *array = NULL;
    }
}

/*
 * initialize the specified cel_array_t
 *
 * @param   cel_array_t *
 * @return  1 for success and 0 for failed
 */
CEL_API int cel_array_create( cel_array_t *array, int opacity )
{
    //check and use default opacity
    if ( opacity < 0 ) opacity = _cel_array_default_length;

    array->items = create_array_blocks( opacity ); 
    if ( array->items == NULL ) return 0;
    array->length = opacity;
    array->size = 0;

    return 1;
}

/*
 * destroy the specified cel_array_t
 * 
 * @param   cel_array_t *
 * @return  1 for success and 0 for failed
 */
CEL_API int cel_array_destroy( 
    cel_array_t *array, 
    cel_release_callback_fn_t relfunc )
{
    register int i;

    if ( array != NULL )
    {
        if ( relfunc != NULL ) {
            for ( i = 0; i < array->size; i++ ) 
                relfunc( array->items[i] );
        }

        cel_free( array->items );
        array->items = NULL;
    }

    return 1;
}

/**
 * create a specified number of ptr blocks.
 *
 * @param	__blocks
 * @return 	void **
 */
static void **create_array_blocks( uint_t __blocks ) 
{
    void ** items = ( void ** ) cel_calloc( sizeof( void * ), __blocks );
    if ( items == NULL ) return NULL;
    return items;
}


CEL_API void cel_array_add( cel_array_t *array, void * value ) 
{
    if ( array->size >= array->length ) 
	resize_array( array, array->size * 2 + 1  );
    array->items[array->size++] = value;
}

/**
 * resize the array to a specified opacity .
 *
 * @param	array
 * @param	opacity
 */
static void resize_array( cel_array_t *array, uint_t opacity ) 
{
    void ** nitems = create_array_blocks( opacity );

    //register uint_t i;
    //for ( i = 0; i < array->size; i++ )
    //	nitems[i] = array->items[i];
    memcpy(nitems, array->items, array->size * sizeof(void *));

    //free the old items
    cel_free( array->items );
    array->items = nitems;
    array->length = opacity;
}

CEL_API void cel_array_insert( 
	cel_array_t *array, uint_t idx, void * value ) 
{
    register uint_t i;
    if ( idx < array->size ) {
    	if ( array->size >= array->length ) 
    	    resize_array( array, array->size * 2 + 1 );

    	//copy the items
    	for ( i = array->size; i > idx; i-- ) 
    	    array->items[i] = array->items[i - 1];	
    	array->items[idx] = value;
    	array->size++;
    }
}

/**
 * deletet the element at a specified position .
 * 	the deleted element will be returned .
 *
 * @param	idx
 * @return	void *
 */
CEL_API void * cel_array_del( cel_array_t *array, uint_t idx ) 
{
    register uint_t i;
    void * old;
    if ( idx >= array->size ) return NULL; 

    //bak the element to deleted
    old = array->items[idx];
    for ( i = idx; i < array->size; i++ )
	array->items[i] = array->items[i+1];
    array->size--;		//minus the size

    return old;
}

/**
 * search the object in the array and delete it .
 *
 * @param	obj
 * @param	cfunc
 * @return	void *
 */
CEL_API void * cel_array_del_obj( 
	cel_array_t *array,
	void * obj,
	cel_compare_fn_t cfunc ) 
{
    int i = cel_array_get_obj( array, obj, cfunc );
    if ( i == -1 ) return NULL;
    return cel_array_del( array, i );
}


CEL_API void * cel_array_get( cel_array_t *array, uint_t idx ) 
{
    if ( idx >= array->size ) return NULL;
    return array->items[idx];
}

/**
 * search the object in the array .
 *
 * @param	obj
 * @param	cfunc
 * @return	int
 */
CEL_API int cel_array_get_obj( 
	cel_array_t *array, 
	void * obj,
	cel_compare_fn_t cfunc )
{
    register uint_t i;
    for ( i = 0; i < array->size; i++ ) {
	if ( obj == array->items[i] 
		|| cfunc( array->items[i], obj ) == 0 )
	    return i;
    }
    return -1;
}

/**
 * replace the element at a specified position 
 * 	with a new value, and the old element will be return .
 *
 * @param	idx
 * @param	value
 * @reutrn	void *
 */
CEL_API void * cel_array_set( 
	cel_array_t *array, uint_t idx, void * value ) 
{
    void * old;
    if ( idx >= array->size ) return NULL;
    //bak the old element
    old = array->items[idx];
    array->items[idx] = value;

    return old;
}


CEL_API void * cel_array_set_obj( 
	cel_array_t *array,
	void * obj,
	void * value,
	cel_compare_fn_t cfunc ) 
{
    int i = cel_array_get_obj( array, obj, cfunc );
    if ( i == -1 ) return NULL;
    return cel_array_set( array, i, value );
}

//instead this will macro define
//CEL_API uint_t cel_array_size( cel_array_t *array ) {
//    return array->size;
//}





//int array block
//@date 2013-09-28
/**
 * create a new cel int array with a default size. */
//CEL_API cel_intArray_t *new_cel_intArray()
//{
//	return new_cel_intArray_opacity( _cel_array_default_length );
//}

#define create_int_blocks( opacity )\
    ( int * ) cel_calloc( sizeof(int), opacity )

static void resize_intArray( cel_intArray_t *array, uint_t opacity ) 
{
    int * nitems = create_int_blocks( opacity ); 

    //register uint_t i;
    //for ( i = 0; i < array->size; i++ )
    //	nitems[i] = array->items[i];
    memcpy( nitems, array->items, array->size * sizeof(int *) );

    //free the old items
    cel_free( array->items );
    array->items = nitems;
    array->length = opacity;
}

/**
 * create a new cel int array with a specified size.
 *
 * @param	uint_t
 * @return	cel_array_t
 */
CEL_API cel_intArray_t *new_cel_intArray_opacity( uint_t opacity )
{
    cel_intArray_t *ptr = ( cel_intArray_t * ) 
	cel_malloc( sizeof( cel_intArray_t ) );
    if ( ptr == NULL ) {
	   CEL_ALLOCATE_ERROR("new_cel_intArray_opacity", sizeof(cel_intArray_t));
    }

    if ( cel_intArray_create(ptr, opacity) == 0 )
    {
        cel_free(ptr);
        CEL_ALLOCATE_ERROR("cel_intArray_create", opacity);
    }

    return ptr;
}

//free the specified cel array
CEL_API void free_cel_intArray( cel_intArray_t **arr )
{
    cel_intArray_t *array;

    if ( arr == NULL ) return;
    if ( *arr != NULL ) 
    {
        array = *arr;
        cel_intArray_destroy(array);
        cel_free( array );
    }

    arr = NULL;
}

/*
 * initialize the specified cel_array_t
 *
 * @param   cel_intArray_t *
 * @param   uint_t initialize opacity of the array
 * @return  1 for success and 0 for failed
 */
CEL_API int cel_intArray_create( cel_intArray_t *array, uint_t opacity )
{
    array->items = create_int_blocks( opacity );
    if ( array->items == NULL ) return 0;
    array->length = opacity;
    array->size = 0;

    return 1;
}

/*
 * destroy the specified cel_array_t
 * 
 * @param   cel_intArray_t *
 * @return  1 for success and 0 for failed
 */
CEL_API int cel_intArray_destroy( cel_intArray_t *array )
{
    if ( array != NULL )
    {
        cel_free( array->items );
    }

    return 1;
}


//add a new element to the array
CEL_API int cel_intArray_add( cel_intArray_t *array, int val )
{
    if ( array->size >= array->length )
	resize_intArray( array, array->size * 2 + 1 );
    array->items[array->size++] = val;
    return 1;
}

//insert a specified element to at a specified position
CEL_API int cel_intArray_insert( 
	cel_intArray_t *array, uint_t idx, int val )
{
	register uint_t i;
    if ( idx >= array->size ) return 0;

    if ( array->size >= array->length ) 
		resize_intArray( array, array->size * 2 + 1 );

    //copy the items
    for ( i = array->size; i > idx; i-- ) 
	array->items[i] = array->items[i - 1];	
    array->items[idx] = val;
    array->size++;

    return 1;
}

/**
 * delete the element at a specified position .
 * 	the deleted element will be returned .
 *
 * @param	idx
 * @return	int 0 for fail and 1 for bingo.
 */
CEL_API int cel_intArray_del( cel_intArray_t *array, uint_t idx )
{
	register uint_t i;
    if ( idx >= array->size ) return 0;
    
    for ( i = idx; i < array->size; i++ )
	array->items[i] = array->items[i+1];
    array->size--;		//minus the size

    return 1;
}

/**
 * search the object in the array and delete it .
 *
 * @param	obj
 * @param	cfunc
 * @return	int 0 for fail and 1 for true.
 */
CEL_API int cel_intArray_del_obj( 
	cel_intArray_t *array, int val )
{
    int i = cel_intArray_get_obj( array, val );
    if ( i == -1 ) return 0; 
    return cel_intArray_del( array, i );
}

/**
 * get the element at a specified postion.
 *
 * @param	uint_t
 * @return	void *
 */
//CEL_API int cel_intArray_get( cel_intArray_t *array, uint_t idx )
//{
//	return array->items[idx];
//}

/**
 * search the object in the array .
 *
 * @param	obj
 * @return	int - the index of the search object.
 */
CEL_API int cel_intArray_get_obj( cel_intArray_t *array, int val )
{
    register uint_t i;
    for ( i = 0; i < array->size; i++ ) {
	if ( array->items[i] == val )
	    return i;
    }
    return 0;
}

/**
 * set the element at the specified position,
 * 	and the old element will be returned . 
 *
 * @param	uint_t
 * @param	void *
 * @return 	void * ptr of the old element
 */
//CEL_API int cel_intArray_set( 
//		cel_intArray_t *array, uint_t idx, int val )
//{
//	array->items[idx] = val;
//}
