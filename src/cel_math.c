/**
 * cel math module implemented functions.
 * 	@package src/cel_math.c
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#include "cel_math.h"


/**
 * get the next prime of the specified numeric.
 *
 * @param	n
 * @return	int
 */
CEL_API int cel_next_prime( int n )
{
    if ( n % 2 == 0 ) n++;
    for ( ; ! cel_is_prime( n ); n = n + 2 ) ;
    return n;
}

/**
 * check the specified numeric is a prime.
 *
 * @param	n
 * @return	int
 */
CEL_API int cel_is_prime( int n )
{
    int j;
    //simple check
    if ( n == 2 || n == 3 )
	return 1;
    if ( n == 1 || n % 2 == 0 )
	return 0;

    //loop check
    for ( j = 3; j * j < n; j++ )
	if ( n % j == 0 ) return 0;
    return 1;
}

