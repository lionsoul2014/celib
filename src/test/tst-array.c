/**
 * dynamic array test program .
 *
 * @author chenxin<chenxin619315@gmail.com>
 */
#include "../cel_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int strcompare( void * str1, void * str2 )
{
    return strcmp( (char *) str1, (char *) str2 );
}

//static int scomp( void * str1, void * str2 ) {
//    cel_array_t *a1 = ( cel_array_t * ) str1;
//    cel_array_t *a2 = ( cel_array_t * ) str2;
//    if ( a1->size == a2->size ) return 0;
//    if ( a1->size < a2->size ) return -1;
//    return 1;
//}

void rmcallback( void * value ) 
{
    printf("+-%s\n", (char *) value);
}

int main (int argc, char **args) 
{
    uint_t i;
    //create a new cel array
    //cel_array_t *array = new_cel_array();
    cel_array_t arr, *array;
    cel_array_create(&arr, -1);
    array = &arr;

    char * a[]= {
        "chenxin", "yanzi", "zhangrenfang", "yangjiang", "panzi"
    };
    
    for ( i = 0; i < 5; i++ ) 
    {
        printf("+--add %s\n", a[i]);
        cel_array_add( array, a[i] );
    }
    printf("size: %d\n", cel_array_size(array));

    printf("\n+-----------get/del--------------+\n");
    printf("get(0): %s\n", (char *) cel_array_get(array, 0));
    printf("del(0): %s\n", (char *) cel_array_del(array, 0));
    printf("get(0): %s\n", (char *) cel_array_get(array, 0));

    printf("\n+---------------------set/get-----------------+\n");
    printf("size: %d\n", cel_array_size(array));
    printf("set(1): %s\n", (char *) cel_array_set( array, 1, "second" ));
    printf("get(1): %s\n", (char *) cel_array_get( array, 1 ));

    printf("\n+----------------insert-------------------+\n");
    printf("insert(0): first\n");
    cel_array_insert( array, 0, "first" );
    printf("size: %d\n", cel_array_size( array ));
    printf("get(0): %s\n", (char *) cel_array_get( array, 0 ));

    printf("\n+----------------obj find-----------------+\n");
    printf("get_obj(%s): %d\n", "yanzi", cel_array_get_obj(array, "yanzi", strcompare));
    printf("del_obj(%s): %s\n", "yanzi", (char *)cel_array_del_obj(array, "yanzi", strcompare));
    printf("size: %d\n", cel_array_size(array));
    
    cel_array_destroy(array, rmcallback);

    //free the array
    //free_cel_array( &array, rmcallback );
    //printf("array=NULL ? %d\n", (array==NULL));
    //free_cel_array(&array, NULL);       //test double free
    
    return 0;
}

