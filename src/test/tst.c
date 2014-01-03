/**
 * test program.
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#include <stdio.h>
#include "../cel_mem.h"

int main( int argc, char *args[] )
{
    uint_t a = 65, i;
    char * str = ( char * ) &a;

    printf("sizeof(ullong_t)=%d\n", sizeof(ullong_t));
    printf("sizeof( ulong_t)=%d\n", sizeof(ulong_t));
    printf("sizeof(  uint_t)=%d\n", sizeof(uint_t));
    printf("sizeof(ushort_t)=%d\n", sizeof(ushort_t));
    printf("sizeof( uchar_t)=%d\n", sizeof(uchar_t));
    putchar('\n');
    
    //little-endian for local store.
    printf("+---test char ptr: \n");
    for ( i = 0; i < sizeof(uint_t); i++ ) 
	printf("byte-%d, %2c, %p\n", i, str[i], &str[i] );
    putchar('\n');

    //package them in bit-endian.
    printf("+---test bits opt: \n");
    printf("((a>>24) & 0xFF)=%2c\n", (char)((a>>24) & 0xFF));
    printf("((a>>16) & 0xFF)=%2c\n", (char)((a>>16) & 0xFF));
    printf("((a>> 8) & 0xFF)=%2c\n", (char)((a>> 8) & 0xFF));
    printf("((a>> 0) & 0xFF)=%2c\n", (char)((a>> 0) & 0xFF));
    
    putchar('\n');
    return 0;
}
