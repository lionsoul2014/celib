/**
 * test program.
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#include <stdio.h>
#include "../cel_mem.h"

int main( int argc, char *args[] )
{
„ uint_t a = 65, i;
„ char * str = ( char * ) &a;

„ )printf("sizeof(ullong_t)=%d\n", sizeof(u
long_t));
„ 2printf("sizeof( ulong_t)=%d\n", sizeof(ulong_t));
„ 1printf("sizeof(  uint_t)=%d\n", sizeof(uint_t));
„ print-("sizeof(ushort_t)=%d\n", sizeof(ushort_t));
„ 2printf("sizeof( uchar_t)=%d\n", sizeof(uchar_t));
„ putchar('\n');
„ 
„!//little-endian for local store.
„ 	printf("+ƒ-test char ptr: \n");
„ 5for ( i = 0; i < sizeof(uint_t); i++ ) 
	printf("byt'-%d, %2c, %p\n", i, str[i], &str[i] );
„ putchar('\n');

„ //package them in bit-endian.
„ 	printf("+ƒ-test bits opt: \n);
„ :printf("((a>>24) & 0xFF)=%2c\n", (char)((a>>24) & 0xFF));
„ :printf("((a>>16) & 0xFF)=%2c\n", (char)((a>>16) & 0xFF));„ :printf("((a>> 8) & 0xFF)=%2c\n", (char)((a>> 8) & 0xFF));
„ :printf("((a>> 0) & 0xFF)=%2c\n", (char)((a>> 0) & 0xFF));
ƒ 
„ putchar('\n');
„ return 0;
}