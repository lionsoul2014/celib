/**
 * cel hashmap test program .
 *
 * @author chenxin<chenxin6519315@gmail.com>
 */
#include "../cel_hashmap.h"
#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *args[] )
{
    uint_t i, length;
    cel_hashmap_t map = new_cel_hashmap();
    char *name[] = {
    	"chenmanwen", "yangqinghua", "chenxin", "yanzi", "yangjian", "zhangrenfang",
	"panzi", "zhangqiang", "webssky", "jcseg", "friso", "robbe", "lionsoul",
	"tankwar", "jteach"
    };
    length = 15;

    //test put
    printf("+---test put: \n");
    for ( i = 0; i < length; i++ ) {
	printf("put(%15s, %15s);\n", name[i], name[i]);
	cel_hashmap_put( map, name[i], name[i] );
    }
    printf("length: %d, size: %d, threshold: %d\n", 
	    cel_hashmap_length(map),
	    cel_hashmap_size(map), 
	    cel_hashmap_threshold(map));
    putchar('\n');

    //test get
    printf("+---test get: \n");
    for ( i = 0; i < length; i++ )
	printf("get(%15s): %15s\n", name[i], (char *)cel_hashmap_get(map, name[i]));
    printf("get(%15s): %15s\n", "lionsoul", (char *)cel_hashmap_get(map, "lionsoul"));
    putchar('\n');

    //test exists
    printf("+---test exists: \n");
    for ( i = 0; i < length; i++ )
	printf("exists(%15s): %d\n", name[i], cel_hashmap_exists(map, name[i]));
    printf("exists(%15s): %d\n", "lionsoul", cel_hashmap_exists(map, "lionsoul"));
    putchar('\n');

    //test set
    printf("+---test set: \n");
    printf("set(%15s, %15s): %15s\n", "chenmanwen", "father", 
	    (char *)cel_hashmap_set(map, "chenmanwen", "father"));
    printf("set(%15s, %15s): %15s\n", "yangqinghua", "mother", 
	    (char *)cel_hashmap_set(map, "yangqinghua", "mother"));
    putchar('\n');

    //test get
    printf("+---test get: \n");
    for ( i = 0; i < length; i++ ) {
	printf("get(%15s): %15s\n", name[i], (char *)cel_hashmap_get(map, name[i]));
    }
    putchar('\n');

    //test remove
    printf("+---test remove: \n");
    printf("remove(%15s): %15s\n", "lionsoul", (char *)cel_hashmap_remove(map, "lionsoul"));
    printf("remove(%15s): %15s\n", "chenxin", (char *)cel_hashmap_remove(map, "chenxin"));
    printf("size: %d\n", cel_hashmap_size(map));

    //free the hashmap
    free_cel_hashmap(map, NULL);
    return 0;
}
