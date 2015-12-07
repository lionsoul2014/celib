/**
 * md5 test program. <br />
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#include "../cel_hash.h"
#include <stdio.h>

static void md5_print( uchar_t * digest )
{
    uint_t j;

    for ( j = 0; j < 16; j++ )
    printf("%02x", digest[j]);
}

int main( int argc, char *args[] )
{
    cstring str[] = {"", "a", "abc", "message digest", 
    "123456", "153759",  "619315",
    "abcdefghijklmnopqrstuvwxyz"};
    char filename[1024] = {0};

    uint_t length = 8, i;
    uchar_t digest[17] = {0};

    for ( i = 0; i < length; i++ ) 
    {
        cel_md5_string( str[i], digest );
        printf("cel_md5_string(\"%-26s\")=", str[i]);
        md5_print( digest );
        putchar('\n');
    }
    putchar('\n');

    printf("Enter file path: ");
    scanf("%s", filename);
    printf("File: %s\n", filename);
    if ( cel_md5_file( filename, digest ) == 0 )
    {
        printf("md5(\"%s\")=", filename);
        md5_print( digest );
        putchar('\n');
    } else
       printf("file not exists!");

    putchar('\n');

    return 0;
}
