/**
 * cel sort module implements functions.
 *     @package src/cel_sort.c
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#include "cel_sort.h"
#include "cel_stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <string.h>

/**
 * insertion sort algorithm.
 *
 * @param   a - the array to sort.
 * @param   size - the bytes that one element in the array takes.
 * @param   comp - the compare function.
 * @param   start - the start offset.
 * @param   end - the end offset.
 */
CEL_API void cel_subinsert_sort(
    void *a, uint_t size, cel_compare_fn_t comp, uint_t start, uint_t end )
{
    uchar_t * tmp = ( uchar_t * ) cel_malloc(size);
    uchar_t * ptr = ( uchar_t * ) a;
    uchar_t * p;

    register int i, j;

    if ( tmp == NULL ) return;

    //printf("size: %d, start: %d, end: %d\n", size, start, end);
    
    for ( i = start + 1; i <= end; i++ ) {
        cel_mem_copy( ptr + i * size, tmp, size );
        //find the right index for tmp.
        for ( j = i; j > start && comp(tmp, (p = ptr + (j - 1) * size)) < 0; j-- ) {
            cel_mem_copy( p, p + size, size );
        }

        //Copy the element to the right position.
        if ( j < i ) {
            cel_mem_copy( tmp, ptr + j * size, size );
        }
    }

    //free the tmp allocations.
    cel_free(tmp);
}


/**
 * gaps array for shell sort.
 * this array was generated with 
 *  9 * pow(4, j) - 9 * pow(2, j) + 1
 *  and 
 *  pow(4, j) - 3 * pow(2, j) + 1 .
 */
static const int gaps[] = {
    1, 5,
    19, 41,
    109, 209, 505, 929,
    2161, 8929,
    16001, 36289, 64769,            //10000th
    146305, 260609, 587521,    
    1045505, 2354689, 4188161, 9427969,    
    16764929, 37730305, 67084289,
    150958081, 268386305, 603906049,
    1073643521, 2147483647
};


/**
 * shell sort algorithm.
 *
 * @param   a - the array to sort.
 * @param   length - the length of the array. 
 * @param   size - the bytes that one element in the array takes.
 * @param   comp - comprare function.
 */
CEL_API void cel_shell_sort(
    void *a, uint_t length, uint_t size, cel_compare_fn_t comp)
{

    uchar_t * tmp = ( uchar_t * ) malloc( size );
    uchar_t * ptr = ( uchar_t * ) a;
    uchar_t * p;

    register int i, j, gap;
    register int k = 0;

    if ( tmp == NULL ) return;

    while ( gaps[k] < length ) k++;

    while ( --k >= 0 ) {
        //Get the gap
        gap = gaps[k];
        for ( i = gap;  i < length; i++ ) {
            cel_mem_copy( ptr + i * size, tmp, size );
            //find the right index of the tmp
            for ( j = i; j >= gap 
                && comp(tmp, (p = ptr + (j - gap) * size)) < 0; j -= gap ) {
                cel_mem_copy( p, p + gap * size, size );
            }

            //located the tmp
            if ( j < i ) {
                cel_mem_copy( tmp, ptr + j * size, size );
            }
        }
    }

    //free the temp allocations.
    free(tmp);
}


/**
 * merge the two half array.
 *
 * @param   a
 * @param   tmp - temp array.
 * @param   l   - left index.
 * @param   m   - middle index.
 * @param   r   - right index.
 */
static void merge(
    uchar_t *a, uchar_t *tmp, uint_t size, 
    cel_compare_fn_t comp, uint_t l, uint_t r, uint_t rend)
{
    uint_t ptmp = l;
    uint_t lend = r - 1;
    uint_t p = l;

    register uchar_t * ltmp, * rtmp;

    while ( l <=lend && r <= rend ) {
        ltmp = a + l * size;
        rtmp = a + r * size;
        if ( comp( ltmp, rtmp ) < 0 ) {
            //Copy the left current one to the temp.
            cel_mem_copy(ltmp, tmp + p * size, size);
            l++;
        } else {
            //Copy the right current one to the temp.
            cel_mem_copy(rtmp, tmp + p * size, size);
            r++;
        }
        p++;
    }

    //Copy the rest of the left to the temp.
    while ( l <= lend ) {
        cel_mem_copy( a + l * size, tmp + p * size, size );
        l++; p++;
    }

    //Copy the rest of the right to the temp.
    while ( r <= rend ) {
        cel_mem_copy( a + r * size, tmp + p * size, size );
        r++; p++;
    }

    //Copy the items back from the temp to the origin one.
    while ( ptmp <= rend ) {
        cel_mem_copy( tmp + ptmp * size, a + ptmp * size, size );
        ptmp++;
    }
}

/**
 * merge sort algorithm.
 *
 * @see cel_sort#cel_shell_sort
 */
CEL_API void cel_merge_sort(void *a, uint_t length, uint_t size, cel_compare_fn_t comp)
{
    //temp array.
    uchar_t * tmp = ( uchar_t * ) cel_calloc(length, sizeof(void *));
    uchar_t * arr = ( uchar_t * ) a;

    register uint_t offset = 2;
    register uint_t len, i, j;

    if ( tmp == NULL ) return;

    //merge sort
    while ( offset <= length ) {
        len = length / offset;
        for ( j = 0, i = 0; j < len - 1; j++ ) {
            //The right end was included, so plus 1 with it.
            merge(arr, tmp, size, comp, i, i + offset / 2, i + offset - 1);
            i += offset;
        }

        //merge the len offset. (make sure you saw the i < len - 1)
        merge(arr, tmp, size, comp, i, i + offset / 2, i + offset - 1);
        i += offset;

        //Check if any items left for the array.
        len = i + offset / 2;
        if ( len <= length - 1 ) {
            merge(arr, tmp, size, comp, i, i + offset / 2, length - 1);
        }

        //Double size the offset.
        offset += offset;
    }

    //Check and merge the rest of the left items.
    offset /= 2;
    if ( offset < length ) {
        merge( arr, tmp, size, comp, 0, offset, length - 1 );
    }

    //free the temp array.
    cel_free(tmp);
}



/**
 * internal function to get the median privot.
 */
static uchar_t *median3(
    uchar_t *arr, uint_t size, cel_compare_fn_t comp, uint_t left, uint_t right)
{
    uchar_t * lp = arr + left * size;
    uchar_t * cp = arr + ( (left + right) / 2 ) * size;
    uchar_t * rp = arr + right * size;

    if ( comp( lp, cp ) > 0 )
        cel_mem_swap( lp, cp, size );
    if ( comp( lp, rp ) > 0 )
        cel_mem_swap( lp, rp, size );
    if ( comp( cp, rp ) > 0 )
        cel_mem_swap( cp, rp, size );

    //move the pivot to the right - 1 positon.
    cel_mem_swap( cp, rp - size, size );

    return rp - size;
}

/**
 * internal static function to do the partition.
 *
 * @see cel_quick_sort
 */
static void quicksort(
    uchar_t *arr, uint_t size, cel_compare_fn_t comp, uint_t left, uint_t right) 
{
    //printf("left: %d, right: %d\n", left, right);
    if ( left + 11 <= right ) {
        //get the privot of the subarray.
        uchar_t * pivot = median3( arr, size, comp, left, right );
        //printf("pivote: %d\n", *pivot);
        //start partitioning.
        register uint_t i = left, j = right - 1;
        for ( ; ; ) {
            while ( comp( arr + (++i * size), pivot ) < 0 ) ;
            while ( comp( arr + (--j * size), pivot ) > 0 ) ;
            if ( i < j ) {
                cel_mem_swap( arr + i * size, arr + j * size, size );
            } else {
                break;
            }
        }

        //swap the privot back to the small colleciton.
        cel_mem_swap( arr + i * size, pivot, size );

        quicksort( arr, size, comp, left, i - 1 );
        quicksort( arr, size, comp, i, right );

    } else {
        //if the number of the items is less than CUTOFF use insertion sort instead.
        cel_subinsert_sort( arr, size, comp, left, right );
    }
}

/**
 * quick sort algorithm.
 *
 * @see cel_sort#cel_shell_sort
 */
CEL_API void cel_quick_sort(
    void *a, uint_t length, uint_t size, cel_compare_fn_t comp)
{
    uchar_t * arr = (uchar_t *) a;
    quicksort(arr, size, comp, 0, length - 1);
}



/**
 * bucket sort algorithm.
 * yeah, this is fast, but also got lots of limits.
 * it applicable only for simple, unsigned integer (it could actually).
 * and the occurrence of the same numeric should be less than 2^(b - 1) .
 *
 * @param   a - the array to be sort.
 * @param   length - the length of the array.
 * @param   bits - bits for one bucket it depened the occurrence of the same numeric.
 */
CEL_API void cel_bucket_sort(
    uint_t * a, uint_t length, uint_t bits)
{
    register uint_t i;

    uint_t __bytes;
    uchar_t * key;
    uint_t idx;
    int v;

    //find the max item in the array.
    uint_t max = a[0];
    for ( i = 1; i < length; i++ ) {
        if ( a[i] > max ) max = a[i];
    }    

    //make the allocations accorading to the max.
    __bytes = ( max * bits + CHAR_BIT - 1 ) / CHAR_BIT;
    key = ( uchar_t * ) cel_calloc( __bytes, sizeof(char) );
    if ( key == NULL ) return;
    memset(key, __bytes, 0x00);

    //Set the bits. (sorting)
    for ( i = 0; i < length; i++ ) {
        v = 0;
        cel_mem_fetch( key, a[i] * bits, bits, &v, 0UL );
        v++;
        cel_mem_store( &v, 0UL, bits, key, a[i] * bits );
        //printf("step%2d, %2d, %2d, step%2d: ", i, a[i], v);
        //cel_mem_bits(NULL, key, __bytes);
    }

    idx = 0;
    for ( i = 0; i <= max; i++ ) {
        v = 0;
        cel_mem_fetch( key, i * bits, bits, &v, 0UL );
        if ( v > 0 ) {
            while ( --v >= 0 ) a[idx++] = i;
        }
    }

    //free the allocations.
    cel_free( key );
}
