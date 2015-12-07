/**
 * cel int array list test program.
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#include "../cel_array.h"

static void print_array( cel_intArray_t *arr )
{
    uint_t i;
    printf("length=%d, size=%d: \n", arr->length, arr->size);
    for ( i = 0; i < arr->size; i++ )
        printf("%d, ", cel_intArray_get( arr, i ));
    putchar('\n');
}

int main( int argc, char *args[] )
{
    cel_intArray_t *arr = new_cel_intArray();

    printf("+---test add: \n");
    cel_intArray_add( arr, 1 );
    cel_intArray_add( arr, 2 );
    cel_intArray_add( arr, 3 );
    cel_intArray_add( arr, 4 );
    cel_intArray_add( arr, 5 );
    cel_intArray_add( arr, 6 );
    cel_intArray_add( arr, 7 );
    cel_intArray_add( arr, 8 );
    cel_intArray_add( arr, 9 );
    print_array( arr );

    printf("+---test del: \n");
    cel_intArray_del( arr, 0 );
    cel_intArray_del( arr, 0 );
    cel_intArray_del( arr, 100 );
    cel_intArray_del_obj( arr, 3 );
    cel_intArray_del_obj( arr, 100 );
    print_array( arr );

    printf("+---test set: \n");
    cel_intArray_set( arr, 0, 10 );
    cel_intArray_set( arr, 1, 11 );
    print_array( arr );

    printf("+---test insert: \n");
    cel_intArray_insert( arr, 0, 1 );
    cel_intArray_insert( arr, 1, 2 );
    print_array( arr );

    free_cel_intArray( &arr );

    return 0;
}
