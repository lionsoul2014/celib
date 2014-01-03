/**
 * utf-8 unicode convert interface.
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#include <stdio.h>
#include <string.h>
#include "../cel_string.h"

int main( int argc, char *args[] )
{
    cstring str = "平凡|执着";
    int i, j, length = strlen( str );
    uint_t ucode[5], bytes = 0;

    char buffer[13] = {0};
    cstring buff = buffer;

    //Utf-8 to unicode.
    for ( i = 0, j = 0; i < length; j++ )
    {
	//printf("bytes=%d, i=%d\n", bytes, i);
	ucode[j] = cel_utf8_to_ucode( &str[i] );
	bytes = cel_utf8_charbytes( str[i] );
	i += bytes;
	printf("%d, ", ucode[j]);
    }
    putchar('\n');

    //Unicode to utf-8
    for ( i = 0; i < 5; i++ ) {
    	j = cel_ucode_to_utf8( ucode[i], buff );
	buff += j;
    }
    printf("buffer=%s\n", buffer);

    return 0;
}
