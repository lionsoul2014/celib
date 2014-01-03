/**
 * RLE file compress test program.
 *
 * @author chenxin <chenxin619315@gmail.com>
 */
#include "../cel_compress.h"
#include "../cel_stdio.h"
#include <stdio.h>

#define inputFileName(infile, outfile)\
    printf("source file: ");		\
    scanf("%s", infile);		\
    printf("destination file: ");	\
    scanf("%s", outfile);

int main( int argc, char *args[] )
{
    char infile[128];
    char outfile[128];
    uint_t len, flen;

    printf("+---test encode: \n");
    inputFileName( infile, outfile );
    //encode the file.
    len = cel_filesize(infile);
    if ( len == -1 )
    {
        printf("Error: file not exists\n");
        exit(0);
    }
    flen = cel_rle_encode_file( infile, outfile );
    printf("srcLen=%d, dstLen=%d, bit=%.2f\n", len, flen, ((float)flen)/((float)len));
    putchar('\n');

    printf("+---test decode: \n");
    inputFileName( infile, outfile );

    //get the source file size
    len = cel_filesize(infile);
    if ( len == -1 )
    {
	printf("Error: file not exists\n");
	exit(0);
    }
    flen = cel_rle_decode_file( infile, outfile );
    printf("srcLen=%d, dstLen=%d\n", len, flen );
    return 0;
}
