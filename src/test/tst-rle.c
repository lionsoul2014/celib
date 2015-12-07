/**
 * Run length encoding compress algorithim test program.
 *
 * @author chenxin <chenxin619315@gmail.com>
 * @date 2013-09-20
 */
#include "../cel_compress.h"
#include "../cel_string.h"
#include <stdio.h>
#include <string.h>

int main( int argc, char *args[] )
{
    uint_t inlen, flen;
    //char * str = "#0AAABBBBCCCCCDEFGGGGGGHHIJKLMNOPQRRSTTTTT";
    //char * str = "#1234567891011";
    //char * str = "ABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABA";
    //char * str = "{{{{AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABCDEFGHIJKLMNOPQRSTUVWXYZ#}}}}";
    char * str = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABAB";
    cel_strbuff_t ensb = new_cel_strbuff();
    cel_strbuff_t desb = new_cel_strbuff();
    inlen = strlen(str);
    printf("inlen=%d\n", inlen);

    printf("+---test encode: \n");
    printf("rle_encode(\"%s\")\n", str);
    flen = cel_rle_encode_string( str, (uint_t) strlen(str), ensb );
    //Count the compress
    printf("ensb->buffer=%s\n", ensb->buffer);
    printf("inlen=%d, flen=%d, bit=%.2f\n", inlen, 
        ensb->length, ((float)ensb->length/(float)inlen));

    putchar('\n');
    printf("+---test decode: \n");
    printf("rle_decode(\"%s\")\n", ensb->buffer);
    cel_rle_decode_string( ensb->buffer, ensb->length, desb );
    printf("desb=%s\n", desb->buffer);

    free_cel_strbuff(ensb);
    free_cel_strbuff(desb);

    return 0;
}
