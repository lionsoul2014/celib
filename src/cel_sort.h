/**
 * cel sort module interface header file.
 * 	@package src/cel_sort.h.
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#ifndef _cel_sort_h
#define _cel_sort_h

#include "cel_api.h"

/**
 * insertion sort algorithm.
 *
 * @param	a
 * @param	length	- the length of the array.
 * @param	size	- the bytes that one element takes in the array.
 * @param	comp	- the compare function.
 * @param	start	- the start offset.
 * @param	end		- the end offset (not included).
 */
CEL_API void cel_subinsert_sort( void *, 
	uint_t, cel_compare_fn_t, uint_t, uint_t );

//insetion sort.
#define cel_insertion_sort( a, len, size, comp ) \
    cel_subinsert_sort(a, size, comp, 0, len - 1)

/**
 * shell sort algorithm.
 *
 * @param	length	- the length of the array. 
 * @param	size	- the bytes that one element in the array takes.
 * @param	comp	- comprare function
 */
CEL_API void cel_shell_sort( void *, uint_t, uint_t, cel_compare_fn_t );

/**merge sort algorithm.*/
CEL_API void cel_merge_sort( void *, uint_t, uint_t, cel_compare_fn_t );

/**quick sort algorithm.*/
CEL_API void cel_quick_sort( void *, uint_t, uint_t, cel_compare_fn_t );

/**
 * bucket sort algorithm.
 * 		yeah, this is fast, but also got lots of limits.
 * 	it applicable only for simple, unsigned integer (it could actually).
 * and the occurrence of the same numeric should be less than 2^(b - 1) .
 *
 * @param	a	- the array to be sort.
 * @param	length	- the length of the array.
 * @param	bits	- bits for one bucket,
 * 		it depened the occurrence of the same numeric.
 */
CEL_API void cel_bucket_sort( uint_t *, uint_t, uint_t );

#endif	/*end ifndef*/

