/**
 * cel array interface header file src/cel_array.h
 *
 * @author chenxin<chenxin619315@gmail.com>
 */
#ifndef _cel_array_h
#define _cel_array_h

#include <stdio.h>
#include <stdlib.h>
#include "cel_api.h"

//default size of the array
#define _cel_array_default_length 16

typedef struct {
    uint_t size;		//size of the array
    uint_t length;		//the length of the arrary 
    void **items;		//array pointer
} cel_array_entry;
typedef cel_array_entry * cel_array_t;

/**
 * create a new cel array with a default size. */
//CEL_API cel_array_t new_cel_array(); 
#define new_cel_array()\
    new_cel_array_opacity(_cel_array_default_length)

/**
 * create a new cel array with a specified size.
 *
 * @param	uint_t
 * @return	cel_array_t
 */
CEL_API cel_array_t new_cel_array_opacity( uint_t ); 

//free the specified cel array
CEL_API void free_cel_array( cel_array_t, cel_release_callback_fn_t );

//add a new element to the array
CEL_API void cel_array_add( cel_array_t, void * );

//insert a specified element to at a specified position
CEL_API void cel_array_insert( cel_array_t, uint_t, void * );

/**
 * deletet the element at a specified position .
 * 	the deleted element will be returned .
 *
 * @param	idx
 * @return	void *
 */
CEL_API void * cel_array_del( cel_array_t, uint_t );

/**
 * search the object in the array and delete it .
 *
 * @param	obj
 * @param	cfunc
 * @return	void *
 */
CEL_API void * cel_array_del_obj( cel_array_t, void *, cel_compare_fn_t );

/**
 * get the element at a specified postion.
 *
 * @param	uint_t
 * @return	void *
 */
CEL_API void * cel_array_get( cel_array_t, uint_t );

/**
 * search the object in the array .
 *
 * @param	obj
 * @param	cfunc
 * @return	int
 */
CEL_API int cel_array_get_obj( cel_array_t, void *, cel_compare_fn_t );

/**
 * set the element at the specified position,
 * 	and the old element will be returned . 
 *
 * @param	uint_t
 * @param	void *
 * @return 	void * ptr of the old element
 */
CEL_API void * cel_array_set( cel_array_t, uint_t, void * );

//replace the old value with the new value
CEL_API void * cel_array_set_obj( cel_array_t, void *, void *, cel_compare_fn_t );

//size get macro
#define cel_array_size( array ) array->size

//count the size of the specified cel array
//CEL_API uint_t cel_array_size( cel_array_t );
//




////
//int array list block.
//@date 2013-09-27
typedef struct {
    uint_t size;		//size of the array
    uint_t length;		//the length of the arrary 
    int *items;			//int array pointer
} cel_intArray_entry;
typedef cel_intArray_entry * cel_intArray_t;

/**
 * create a new cel int array with a default size. */
//CEL_API cel_intArray_t new_cel_intArray(); 
#define new_cel_intArray()\
    new_cel_intArray_opacity(_cel_array_default_length)

/**
 * create a new cel int array with a specified size.
 *
 * @param	uint_t
 * @return	cel_array_t
 */
CEL_API cel_intArray_t new_cel_intArray_opacity( uint_t ); 

//free the specified cel array
CEL_API void free_cel_intArray( cel_intArray_t );

//add a new element to the array
CEL_API int cel_intArray_add( cel_intArray_t, int );

//insert a specified element to at a specified position
CEL_API int cel_intArray_insert( cel_intArray_t, uint_t, int );

/**
 * delete the element at a specified position .
 * 	the deleted element will be returned .
 *
 * @param	idx
 * @return	int 0 for fail and 1 for true.
 */
CEL_API int cel_intArray_del( cel_intArray_t, uint_t );

/**
 * search the object in the array and delete it .
 *
 * @param	obj
 * @param	cfunc
 * @return	int 0 for fail and 1 for true.
 */
CEL_API int cel_intArray_del_obj( cel_intArray_t, int );

/**
 * get the element at a specified postion.
 *
 * @param	uint_t
 * @return	int - item value.
 */
//CEL_API int cel_intArray_get( cel_intArray_t, uint_t );
#define cel_intArray_get( array, idx ) array->items[idx]

/**
 * search the object in the array .
 *
 * @param	obj
 * @return	int - the index of the searching object.
 */
CEL_API int cel_intArray_get_obj( cel_intArray_t, int );

/**
 * set the element at the specified position,
 * 	and the old element will be returned . 
 *
 * @param	uint_t
 * @param	idx
 */
//CEL_API int cel_intArray_set( cel_intArray_t, uint_t, int );
#define cel_intArray_set( array, idx, val )\
    array->items[idx] = val

//size get macro
#define cel_intArray_size( array ) array->size

#endif	/*end ifndef*/
