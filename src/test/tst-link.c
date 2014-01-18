#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cel_link.h"

static int string_compare( void * str1, void * str2 ) {
    return strcmp((char *) str1, (char *) str2);
}

#define __PRINT_LINK__							\
    for ( i = 0; i < link->size; i++ )					\
	printf("get(%d): %s\n", i, ( char * ) cel_link_get(link, i));	\
    putchar('\n');

int main( int argc, char *args[] )
{
    uint_t i = 0;
    //cel_link_t *link = new_cel_link();
    cel_link_t entry, *link;
    char * a[] = {"chenxin", "yanzi", "zhangrenfang", "yangjian", "pangzi", "last"};

    cel_link_create(&entry);
    link = &entry;

    //test add
    printf("+---element add: \n");
    for ( i = 0; i < 6; i++ )
	   cel_link_add_last( link, a[i]);
    printf("size: %d\n", cel_link_size(link));
    __PRINT_LINK__

    //test insert
    printf("+---element insert: \n");
    cel_link_insert( link, 0, "yangqinghua" );
    cel_link_insert( link, 0, "chenmanwen" );
    printf("size: %d\n", cel_link_size(link));
    __PRINT_LINK__

    //test add first
    printf("+---element add first: \n");
    cel_link_add_first( link, "first" );
    printf("size: %d\n", cel_link_size(link));
    __PRINT_LINK__

    //test get
    printf("+---element get: \n");
    __PRINT_LINK__


    //test set
    printf("+---element set: \n");
    printf("set(1): %s\n", (char *) cel_link_set(link, 1, "father"));
    printf("set(%s): %s\n", "father",
	    (char *) cel_link_set_obj(link, "yangqinghua", "mother", string_compare));
    printf("size: %d\n", cel_link_size(link));
    __PRINT_LINK__

    //test remove first
    printf("+---element remove first: \n");
    for ( i = 0; i < 3; i++ )
	printf("+-%d: %s\n", i, (char *) cel_link_remove_first(link));
    printf("size: %d\n", cel_link_size(link));
    __PRINT_LINK__

    //test remove object
    printf("+---element remove object: \n");
    printf("remove(last): %s\n", (char *)cel_link_remove_obj( link, "last", string_compare ));
    printf("size: %d\n", cel_link_size(link));
    __PRINT_LINK__
    
    //test remove last
    printf("+---element remove last: \n");
    for ( i = 0; i < 5; i++ )
	printf("+-%d: %s\n", i, (char *) cel_link_remove_last(link));
    printf("size: %d\n", cel_link_size(link));
    __PRINT_LINK__


    //free_cel_link( &link, NULL );
    cel_link_destroy(link, NULL);

    return 0;
}
