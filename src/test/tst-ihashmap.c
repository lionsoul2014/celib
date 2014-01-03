/**
 * cel ihashmap test program.
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#include "../cel_hashmap.h"
#include <stdio.h>

int main( int argc, char *args[] )
{
    cel_ihashmap_t imap = new_cel_ihashmap();

    printf("+---test put: \n");
    cel_ihashmap_put(imap, "01", 1);
    cel_ihashmap_put(imap, "02", 2);
    cel_ihashmap_put(imap, "03", 3);
    cel_ihashmap_put(imap, "04", 4);
    cel_ihashmap_put(imap, "05", 5);
    cel_ihashmap_put(imap, "06", 6);
    cel_ihashmap_put(imap, "07", 7);
    cel_ihashmap_put(imap, "08", 8);
    printf("size=%d, length=%d\n", cel_ihashmap_size(imap), cel_ihashmap_length(imap));

    printf("+---test set: \n");
    printf("set(\"03\", 10)=%d\n", cel_ihashmap_set(imap, "03", 10));
    printf("set(\"06\", 12)=%d\n", cel_ihashmap_set(imap, "06", 12));

    uint_t i;
    char buff[3] = {0};
    printf("+---test get: \n");
    for ( i = 1; i <= 8; i++ ) {
	buff[0] = '0';
	buff[1] = i+'0';
	printf("get(\"%s\")=%d\n", buff, cel_ihashmap_get(imap, buff));
    }

    printf("+---test remove: \n");
    printf("remove(\"02\")=%d\n", cel_ihashmap_remove(imap, "02", NULL));
    printf("remove(\"01\")=%d\n", cel_ihashmap_remove(imap, "01", NULL));
    printf("size=%d, length=%d\n", cel_ihashmap_size(imap), cel_ihashmap_length(imap));

    printf("+--test exists: \n");
    for ( i = 1; i < 10; i++ ) {
	buff[0] = '0';
	buff[1] = '0'+i;
	printf("exists(\"%s\")=%d\n", buff, cel_ihashmap_exists(imap, buff));
    }

    free_cel_ihashmap( imap, NULL );
    return 0;
}
