/**
 * cel sort test program.
 * 
 * test report: 
 * 1. environment: ubuntu/2G/AMD 2.8GHZ
 * 2. result: (take 1000000 random integer as sample)
 * +------------+---------------+
 * | Algorithm	| Time(sec)	|
 * +------------+---------------+
 * | insertion	| forget it	|
 * +------------+---------------+
 * | shell	| 1.430000	|
 * +------------+---------------+
 * | merge	| 1.250000	|
 * +------------+---------------+
 * | quick	| 0.430000	|
 * +------------+---------------+
 * | bucket(1)	| 0.050000	|
 * +------------+---------------+
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#include "../cel_sort.h"
#include "../cel_mem.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define LEN 1000000 

void test_swap( void );
void random_array( uint_t *, uint_t );
int int_compare( void *, void * );
void print_r( uint_t *, uint_t );

int main( int argc, char *args[] )
{
    //uint_t a[] = {5, 4, 3, 9, 20, 2, 7, 17, 12, 10, 8, 15, 36, 24, 18, 16, 10, 6, 11, 13};
    //uint_t LEN = 20;
    //test_swap();

    uint_t a[LEN];
    random_array(a, LEN);
    
    clock_t s = clock();
    //cel_subinsert_sort( a, sizeof(int), int_compare, 0, 8 );
    //cel_subinsert_sort( a, sizeof(int), int_compare, 9, 19 );
    //cel_insertion_sort(a, LEN, sizeof(int), int_compare);
    //cel_shell_sort( a, LEN, sizeof(int), int_compare );
    //cel_merge_sort( a, LEN, sizeof(int), int_compare );
    //cel_quick_sort( a, LEN, sizeof(int), int_compare);
    cel_bucket_sort(a, LEN, 1);
    clock_t e = clock();

    printf("Done, cost %f sec\n", ( (double)(e -s) / CLOCKS_PER_SEC) );
    //print_r(a, LEN);

    return 0;
}

int int_compare( void * a, void * b )
{
    int * __a = (int *) a;
    int * __b = (int *) b;
    if ( *__a > * __b ) return 1;
    if ( *__a < * __b ) return -1;
    return 0;
}

//generate a random array.
void random_array( uint_t * a, uint_t length )
{
    uint_t i;

    for ( i = 0; i < length; i++ ) 
	a[i] = i;

    //set the seed of the rand.
    srand((unsigned) time(NULL));
    for ( i = length - 1; i > 0; i-- )
	cel_mem_swap(a+i, a + (rand() % i), sizeof(uint_t));
}

void print_r( uint_t * a, uint_t length )
{
    register uint_t i;
    for ( i = 0; i < length; i++ ) {
	if ( i == 0 )
	    printf("%d", a[i]);
	else
	    printf(", %d", a[i]);
    }
    putchar('\n');
}

//test cel_swap and cel_copy
void test_swap() 
{
    int a = 1, b = 2;
    int c;

    printf("+---test copy: \n");
    cel_mem_copy( &a, &c, sizeof(int) );
    printf("c=%d\n", c);
    putchar('\n');

    printf("+---test swap: \n");
    printf("before swap: a=%d, b=%d\n", a, b);
    cel_mem_swap(&a, &b, sizeof(int));
    printf("after  swap: a=%d, b=%d\n", a, b);
}
