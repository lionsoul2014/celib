/**
 * mem module test program.
 *
 * @author chenxi <chenxin619315@gmail.com>
 */
#include "../cel_mem.h"
#include <string.h>

int main( int argc, char *args[] )
{
    char * a = "i love c";
    char b[65] = {0};

    printf("sizeof(ulong_t)=%d\n", sizeof(ulong_t));
    //uchar_t * c = ( uchar_t * ) a; 
    //printf("%d\n", CEL_BIT_FETCH(c, 16) != 0);
    //printf("%d\n", CEL_BIT_FETCH(c, 17) != 0);
    //printf("%d\n", CEL_BIT_FETCH(c, 18) != 0);
    //printf("%d\n", CEL_BIT_FETCH(c, 19) != 0);
    //printf("%d\n", CEL_BIT_FETCH(c, 20) != 0);
    //printf("%d\n", CEL_BIT_FETCH(c, 21) != 0);
    //printf("%d\n", CEL_BIT_FETCH(c, 22) != 0);
    //printf("%d\n", CEL_BIT_FETCH(c, 23) != 0);

    printf("+---test mem store: \n");
    cel_mem_bits("init: ", a, 8);
    cel_mem_store(a, 16, 8, b, 0);
    cel_mem_bits("_dst: ", b, 8);
    printf("a=%s, b=%s\n\n", a, b);

    //int x = 10, y;
    short x = 6, y;

    printf("+---test mem fetch: \n");
    cel_mem_fetch(&x, 0, 16, &y, 0);
    printf("x=%d, y=%d\n", x, y);

    return 0;
}
