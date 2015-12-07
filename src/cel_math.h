/**
 * cel math module interface header file.
 *     @package src/cel_math.h.
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#ifndef _cel_math_h
#define _cel_math_h

#include "cel_api.h"

/**
 * get the next prime of the specified numeric.
 *
 * @param    int
 * @return    int
 */
CEL_API int cel_next_prime( int );

/**
 * check the specified numeric is a prime.
 *
 * @param    int
 * @return    int
 */
CEL_API int cel_is_prime( int );

#endif    /*end ifndef*/
